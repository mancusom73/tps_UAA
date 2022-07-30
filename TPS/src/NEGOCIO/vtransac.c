#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <malloc.h> 
#include <pausa.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
static struct _Peticion **peticiones = 0;
static int nueva_transaccion = -1;
//
// Nro Ultima transaccion enviada por TCP-IP
//
long ultima_transaccion = 0;
/**************************************************************************/
int EMPEZAR_VTRANSACCION( void )
/**************************************************************************/
{
    // Si hay transacciones obligar a llamar a TERMINAR_VTRANSACCION(void)
    // antes de empezar una nueva.
    if( nueva_transaccion == -1 ) {
        GRABAR_LOG_ID_DBROUTER( SDBR_VTRAN_INIT, LOG_COMUNICACIONES,2 );
        peticiones = calloc( MAXIMO_PETICIONES_VTRANSACCION, sizeof( struct _Peticion * ) );
        memset( peticiones, 0, ( MAXIMO_PETICIONES_VTRANSACCION * sizeof( struct _Peticion * ) ) );
    }
    if( peticiones[0] ) {
        GRABAR_LOG_ID_DBROUTER( SDBR_EMPEZAR_VTRAN_SIN_TERM , LOG_COMUNICACIONES,2);
        return 0;
    }
    nueva_transaccion = 1;
    return 1;
}
/**************************************************************************/
int ELIMINAR_VTRANSACCION()
/**************************************************************************/
{
    int i = 0;
    for( ;i < MAXIMO_PETICIONES_VTRANSACCION && peticiones != NULL &&peticiones[i];i++ ) {
       if( peticiones[i]->Buffer.szSelectCondition ) {
            free( peticiones[i]->Buffer.szSelectCondition );
        }
        if( peticiones[i]->Buffer.sData ) {
            free( peticiones[i]->Buffer.sData );
        }
        free( peticiones[i] );
        peticiones[i] = 0;
        // Borra buffer en TCP-IP
        INIT_BUFFER_TRANSACCION();
    }
    return 1;
}
/**************************************************************************/
int ENVIAR_VTRANSACCION( int espera_respuesta_tcp, int espera_por_ack )
/**************************************************************************/
{
    int i = 0, rta = 1;
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        return 1;
    }
    #endif
    if( nueva_transaccion == -1 ) {
        return 0;
    }
    // Si esta vacio enviar OK.
    if( !peticiones[0] ) {
        return 1;
    }
    // Si es la Ultima agrega la Bandera de END-FLAG
    for( ;i < MAXIMO_PETICIONES_VTRANSACCION && peticiones[i];i++ ) {
        ;
    }
    if( i < MAXIMO_PETICIONES_VTRANSACCION && i > 0 ) {
        if( ultima_transaccion == ( long )peticiones[i - 1]->Header.lNroTransaccion ) {
            if( espera_por_ack || espera_respuesta_tcp ) {
                peticiones[i - 1]->Header.bEsperaRespuesta |= FIN_TRANSACCION_CON_RESPUESTA;
            }
            else {
                peticiones[i - 1]->Header.bEsperaRespuesta |= FIN_TRANSACCION_SIN_RESPUESTA;
            }
        }
        else {
            rta = 0;
            GRABAR_LOG_ID_DBROUTER( SDBR_INCONSISTENCIA_CANT_TRANS , LOG_COMUNICACIONES,2);
        }
    }
    else if( i ) {
        rta = 0;
        GRABAR_LOG_ID_DBROUTER( SDBR_INCONSISTENCIA_CANT_TRANS , LOG_COMUNICACIONES,2);
    }
    //}
    for( i = 0;i < MAXIMO_PETICIONES_VTRANSACCION && peticiones[i] && rta;i++ ) {
        if( ENTRA_EN_BUFFER_TCPIP( peticiones[i] ) ) {
            if( !INSERTAR_PETICION_TCPIP( peticiones[i] ) ) {
                rta = 0;
            }
        }
        else {
            if( !MANDAR_BUFFER_TCPIP() ) {
                rta = 0;
            }
            else if( !INSERTAR_PETICION_TCPIP( peticiones[i] ) ) {
                rta = 0;
            }
        }
    }
    if( !MANDAR_BUFFER_TCPIP() ) {
        rta = 0;
        GRABAR_LOG_DBROUTER( "Error al enviar dbr.",LOG_ERRORES,2 );
    }
    if( espera_por_ack && rta ) {
        //
        // Esperar de una Insert el ACK con el nro transaccion
        // de la ultima enviada.
        //
        int h, bytes = 0;                   // bytes leidos desde TCP-IP.
        // Escuchar TCP-IP hasta recibir algo o salir por TIME-OUT
        for( h = 0;h < 10 * TIME_OUT_SERVER_NO_RESPONDIO && !bytes;h++ ) {
            DRIVER( _DRV_tcp, _TCP_status, 0, &bytes, sizeof( bytes ) );
            if( !bytes ) {
                SLEEP(100);
            }
            //delay(5);
        }
        if( ( rta = bytes ) != 0 ) {
            char buffer_para_recibir[512];
            struct
            {
                int rta;
                char *buffer;
                int l_buffer;
            } rta_drv;
            #if defined(INVEL_W) || defined(INVEL_L)
            rta_drv.l_buffer = 512;
            rta_drv.rta = 0;
            rta_drv.buffer = buffer_para_recibir;
            DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
            #else
            rta_drv.buffer = puntero_memoria_real_driver_invel;
            rta_drv.l_buffer = 512;
            DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
            memcpy( buffer_para_recibir, puntero_memoria_driver_invel, 512 );
            #endif
            if( !rta_drv.rta ) {
                //-------------------------------------------------
                // RECIBE LA RESPUESTA DESDE EL DBROUTER - ACK
                //-------------------------------------------------
                // unsigned short cant_item = 0;
                unsigned short nro_caja;
                long nro_transaccion;
                char *stmp = buffer_para_recibir;
                nro_caja = *( unsigned short* )stmp;
                stmp += sizeof( unsigned short );
                nro_transaccion = *( long* )stmp;
                stmp += sizeof( long );
                if( nro_caja == NRO_CAJA && nro_transaccion == ultima_transaccion ) {
                    if( *( char* )stmp )        // error_flag
                    {
                        rta = 0;
                        GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_FLAG_ON , LOG_COMUNICACIONES,2);
                    }                             /* else { // Transaccion OK Rta 1. rta = 1; } */
                }
                else {
                    char texto[100];
                    _snprintf(texto, sizeof(texto)-1, "ERROR DE RUTEO: (%d,%li) ESPERADA (%d,%li)", nro_caja,
                             nro_transaccion, NRO_CAJA, ultima_transaccion );
                    GRABAR_LOG_DBROUTER( texto, LOG_DEBUG,2 );
                    rta = 0;
                }
            }
            else {
                GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_TCP_IP_ACK , LOG_COMUNICACIONES,2);
                rta = 0;
            }
        }
        else {
            GRABAR_LOG_ID_DBROUTER( SDBR_TIME_OUT_RETORNO_TCP_IP_ACK , LOG_COMUNICACIONES,2);
        }
    }
    if( rta ) {
        // la Mando Bien la Borra.
        ELIMINAR_VTRANSACCION();
        nueva_transaccion = 1;
    }
    /*else {
        CERRAR_CANAL_DBR();
            ABRIR_CANAL_DBR();
    }*/
    return rta;
}
/**************************************************************************/
int INCLUIR_PETICION( struct _Peticion *peticion )
/**************************************************************************/
{
    int i = 0;
    if( nueva_transaccion == -1 ) {
        //    GRABAR_LOG_DBROUTER( "INC PET NOT INT");
        return 0;
    }
    for( ;i < MAXIMO_PETICIONES_VTRANSACCION && peticiones[i];i++ ) {
        ;
    }
    if( i < MAXIMO_PETICIONES_VTRANSACCION ) {
        // Copiar los datos en un puntero
        // Reserva memoria para el header
        peticiones[i] = ( struct _Peticion * )MALLOC( sizeof( struct _Peticion ) );
        if( peticiones[i] == NULL ) {
            GRABAR_LOG_DBROUTER( "Error NULL peticiones[i]" ,LOG_COMUNICACIONES,2);
            return 0;
        }
        memcpy( ( void* )peticiones[i], &peticion->Header, sizeof( struct _Header ) );
        // Reserva memoria para la condicion del select
        if( peticion->Header.wSelectLength ) {
            char *condicion_select = ( char * )malloc( peticion->Header.wSelectLength );
            strcpy( condicion_select, peticion->Buffer.szSelectCondition );
            peticiones[i]->Buffer.szSelectCondition = condicion_select;
        }
        else {
            peticiones[i]->Buffer.szSelectCondition = 0;
        }
        // Reserva memoria para los datos
        if( peticion->Header.wBufferLength ) {
            char *buffer_data = ( char * )malloc( peticion->Header.wBufferLength );
            memcpy( buffer_data, peticion->Buffer.sData, peticion->Header.wBufferLength );
            peticiones[i]->Buffer.sData = buffer_data;
        }
        else {
            peticiones[i]->Buffer.sData = 0;
        }
        // bandera de nueva transaccion.
        if( nueva_transaccion ) {
            nueva_transaccion = 0;
            peticiones[i]->Header.bEsperaRespuesta = COMIENZO_TRANSACCION;
        }
        else {
            peticiones[i]->Header.bEsperaRespuesta = ITEM_TRANSACCION;
        }
        peticiones[i]->Header.wNroCaja = NRO_CAJA;// RNV
        peticiones[i]->Header.lNroTransaccion = ++ultima_transaccion;
        peticiones[i]->Header.wFecha = GET_FECHA_DOS();
        peticiones[i]->Header.wHora = GET_HORA_DOS();
        return 1;
    }
    else {
        GRABAR_LOG_ID_DBROUTER( SDBR_MAXIMO_PETICIONES_EXCEDIDO , LOG_COMUNICACIONES,2);
    }
    return 0;
}

