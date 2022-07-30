#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cio.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <malloc.h> 
#include <pausa.h> 
#include <tkt.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#include <dos.h>
#endif
#ifdef INVEL_W
#include <io.h>
#endif
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
#undef EOF
struct ticket_cab_pie
{
    char id;
    char nro_linea;
    char string[40];
};
/* Obtain en DBRouter *************************************************** */
int DBROUTER_TICKET_EN_BIN( void )
/**************************************************************************/
{
    char *buffer_para_recibir;
    const char orderby[] = "ORDER BY ID, NRO_LINEA";
    int registros_obtenidos = 0, rta = 1, open_ok;
    char texto_ticket[41];
    char fin_archivo = 0x1A;
    struct _Peticion peticion;
    struct
    {
        int rta;
        char *buffer;
        int l_buffer;
    } rta_drv;
    int handle_cab = 0, handle_pie = 0;
    handle_cab = _CREAT( "cab.tmp", S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
    handle_pie = _CREAT( "pie.tmp", S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
    if( !handle_pie ) {
        return 0;
    }
    if( !handle_cab ) {
        _CLOSE( handle_pie, __LINE__, __FILE__ );
        return 0;
    }
    if( ( buffer_para_recibir =(char *)MALLOC( MAXIMO_BUFFER_TRANSACCION_RECIBIR ) ) == NULL ) {
        // Error de Memoria.
        return 0;
    }
    //-------------------------------------------------
    // ARMAR-ENVIAR LA PETICION DE OBTENER TABLA
    //-------------------------------------------------
    // Empieza una transaccion nueva.
    if( !EMPEZAR_VTRANSACCION() ) {
        ELIMINAR_VTRANSACCION();
        if( !EMPEZAR_VTRANSACCION() ) {
            return 0;
        }
    }
    // Armar los contenidos del Header de la peticion.
    peticion.Header.wCodPet = OBTENER_REGISTROS;
    peticion.Header.wBufferLength = 0;
    strncpy( peticion.Header.szTabla, "TICKET_CAB_PIE", 15 );
    peticion.Header.szTabla[15] = 0;
    peticion.Buffer.sData = 0;
    peticion.Header.bNull = 0;
    peticion.Header.wSelectLength = strlen( orderby ) + 1;
    //peticion.Buffer.szSelectCondition = ( char * ) orderby;
    peticion.Buffer.szSelectCondition = ( char* )orderby;
    // Insertar la peticion.
    if( INCLUIR_PETICION( &peticion ) ) {
        // Envia las transacciones pendientes.
        if( ENVIAR_VTRANSACCION( 1, 0 ) ) {
            //-------------------------------------------------
            // ESPERAR LA RESPUESTA DESDE EL DBROUTER - HOST
            //-------------------------------------------------
            #ifdef INVEL_L
            open_ok = open( "cab.tmp", O_RDWR | O_CREAT, S_IREAD | S_IWRITE );
            #else
            open_ok = _open( "cab.tmp", _O_RDWR | _O_CREAT | _O_BINARY, _S_IREAD | _S_IWRITE );
            #endif
            if( open_ok != -1 ) {
                do {
                    int bytes = 0;      // bytes leidos desde TCP-IP.
                    int h,i;
                    // Escuchar TCP-IP hasta recibir algo o salir por TIME-OUT
                    //for( h = 0; h < 100 * TIME_OUT_SERVER_NO_RESPONDIO && !bytes; h++ ) {
                    for( h = 0;h < 10 * TIME_OUT_SERVER_NO_RESPONDIO;h++ ) {
                        DRIVER( _DRV_tcp, _TCP_status, 0, &bytes, sizeof( bytes ) );
                        if( bytes != 0 ) {
                            break;
                        }
                        #if !defined(INVEL_W) && !defined(INVEL_L)
                        else {
                            SLEEP(100);
                        }
                        #endif
                    }
                    if( bytes ) {
                        #if !defined(INVEL_W) || !defined(INVEL_L)
                        rta_drv.buffer = buffer_para_recibir;
                        rta_drv.l_buffer = MAXIMO_BUFFER_TRANSACCION_RECIBIR;
                        rta_drv.rta = 0;
                        DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
                        #else
                        rta_drv.buffer = puntero_memoria_real_driver_invel;
                        rta_drv.l_buffer = MAXIMO_BUFFER_TRANSACCION_RECIBIR;
                        DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
                        memcpy( buffer_para_recibir, puntero_memoria_driver_invel,
                                MAXIMO_BUFFER_TRANSACCION );
                        #endif
                        if( !rta_drv.rta ) {
                            //-------------------------------------------------
                            // RECIBE LA RESPUESTA DESDE EL DBROUTER - HOST
                            //-------------------------------------------------
                            unsigned short nro_caja;
                            long nro_transaccion;
                            char *stmp = buffer_para_recibir;
                            nro_caja = *( unsigned short* )stmp;
                            stmp += sizeof( unsigned short );
                            nro_transaccion = *( long* )stmp;
                            stmp += sizeof( long );
                            if( nro_caja == NRO_CAJA && nro_transaccion == ultima_transaccion ) {
                                // unsigned short secuencia;
                                unsigned short cant_item = 0;
                                char error_flag;
                                error_flag = *( char* )stmp;
                                stmp += sizeof( char );
                                // secuencia = *(unsigned short *) stmp;
                                stmp += sizeof( unsigned short );
                                cant_item = *( unsigned short* )stmp;
                                stmp += sizeof( unsigned short );
                                //
                                // RECORRE LAS PETICIONES DENTRO DE LA
                                // TRANSACCION.
                                //
                                if( error_flag ) {
                                    GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_FLAG_ON , LOG_COMUNICACIONES,3);
                                    rta = 0;
                                }
                                else {
                                    for( i = 0;rta && i < cant_item;i++ ) {
                                        // int length = 0;
                                        // unsigned short item_secuencia;
                                        short estado;
                                        // item_secuencia = *(unsigned short *) stmp;
                                        // check item_secuencia
                                        stmp += sizeof( unsigned short );
                                        estado = *( short* )stmp;
                                        stmp += sizeof( short );
                                        // Chequeo de Estado
                                        if( estado < 0 ) {
                                            // Registro con Error;
                                            GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_FLAG_ON , LOG_COMUNICACIONES,3);
                                            rta = 0;
                                        }
                                        else if( estado == 0 ) {
                                            // Ultimo Registro Obtenido OK;
                                            registros_obtenidos = 1;
                                        }     // (estado > 0) Registro
                                        else {
                                            // Obtenido.
                                            struct ticket_cab_pie *ticket = (
                                                struct ticket_cab_pie * )stmp;
                                            switch( ticket->id ) {
                                                case 'C':
                                                case 'c':
                                                    if( ticket->nro_linea == 0 ) {
                                                        ticket->string[20] = 0;
                                                        sprintf( texto_ticket, "%-20s\r\n",
                                                                 ticket->string );
                                                        if( _WRITE( handle_cab, texto_ticket, 22,
                                                                    __LINE__, __FILE__ ) ) {
                                                            rta = 0;
                                                        }
                                                    }
                                                    else {
                                                        ticket->string[40] = 0;
                                                        sprintf( texto_ticket, "%-40s\r\n",
                                                                 ticket->string );
                                                        if( _WRITE( handle_cab, texto_ticket, 42,
                                                                    __LINE__, __FILE__ ) ) {
                                                            rta = 0;
                                                        }
                                                    }
                                                    break;
                                                case 'P':
                                                case 'p':
                                                    ticket->string[40] = 0;
                                                    sprintf( texto_ticket, "%-40s\r\n",
                                                             ticket->string );
                                                    strncpy( texto_ticket, ticket->string,
                                                             strlen( ticket->string ) );
                                                    if( _WRITE( handle_pie, texto_ticket, 42,
                                                                __LINE__, __FILE__ ) ) {
                                                        rta = 0;
                                                    }
                                                    break;
                                                default:
                                                    GRABAR_LOG_DBROUTER( "CODTICKINV",LOG_COMUNICACIONES,4 );
                                                    rta = 0;
                                            }
                                            stmp += estado;
                                        }
                                    }
                                }
                            }
                            else {
                                char texto[100];
                                sprintf( texto, "ERROR DE RUTEO: (%d,%li) ESPERADA (%d,%li)",
                                         nro_caja, nro_transaccion, NRO_CAJA, ultima_transaccion );
                                GRABAR_LOG_DBROUTER( texto ,LOG_COMUNICACIONES,3);
                                rta = 0;
                            }
                        }
                        else {
                            rta = 0;
                            GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_TCP_IP, LOG_COMUNICACIONES,3 );
                        }
                    }
                    else {
                        rta = 0;
                        GRABAR_LOG_ID_DBROUTER( SDBR_TIME_OUT_RETORNO_TCP_IP, LOG_COMUNICACIONES,3 );
                    }
                }
                while( rta && !registros_obtenidos );
            }
            // grabar fin de archivo.
            if( _WRITE( handle_pie, &fin_archivo, sizeof( fin_archivo ), __LINE__, __FILE__ )
             || _WRITE( handle_cab, &fin_archivo, sizeof( fin_archivo ), __LINE__, __FILE__ ) ) {
                rta = 0;
            }
            _CLOSE( handle_cab, __LINE__, __FILE__ );
            _CLOSE( handle_pie, __LINE__, __FILE__ );
            if( !( rta && !COPIAR_ARCHIVO( "cab.tmp", "cab.dat", __LINE__, __FILE__ )
                && !COPIAR_ARCHIVO( "pie.tmp", "pie.dat", __LINE__, __FILE__ ) ) ) {
                rta = 0;
            }
        }
        else {
            rta = 0;
        }                              // env trans
    }
    else {
        rta = 0;
    }                                  // inc pet
    free( buffer_para_recibir );
    /* if ( rta ) GRABAR_LOG_DBROUTER( "TICKET CAB-PIE OBT OK" ); else
     * GRABAR_LOG_DBROUTER( "TICKET CAB-PIE OBT ERR" ); */
    return rta;
}
/* Obtain en DBRouter *************************************************** */
int DBROUTER_OBTENER_TABLA_BIN( char *nombre_archivo, char *nombre_tabla, char *codicion_select )
/**************************************************************************/
{
    char log_tabla[100];
    char *buffer_para_recibir,*p;
    char nombre_temporal[32];
    int registros_obtenidos = 0, rta = 1;
    struct _Peticion peticion;
    struct
    {
        int rta;
        char *buffer;
        int l_buffer;
    } rta_drv;
    int handle_arch = 0;
    strcpy( nombre_temporal, nombre_archivo );
    // Toma el Archivo binario vacio
    if( ( p = ( char* )strchr( nombre_temporal, '.' ) ) != 0 ) {
        strcpy( p + 1, "tmp" );               // nombre vacio contiene el archivo.###
    }
    else {
        char texto[200];
        sprintf( texto, "TABLA BIN: NOM INV %s", nombre_archivo );
        GRABAR_LOG_DBROUTER( texto ,LOG_COMUNICACIONES,4);
    }
    //handle_arch = _CREAT( nombre_temporal, S_IFREG, __LINE__, __FILE__ );
    handle_arch = _CREAT( nombre_temporal, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
    if( !handle_arch ) {
        return 0;
    }
    if( ( buffer_para_recibir = malloc( MAXIMO_BUFFER_TRANSACCION_RECIBIR ) ) == NULL ) {
        // Error de Memoria.
        return 0;
    }
    // LOG
    sprintf( log_tabla, "OBTENIENDO TABLA %s EN BIN", nombre_archivo );
    GRABAR_LOG_DBROUTER( log_tabla ,LOG_COMUNICACIONES,4);
    //-------------------------------------------------
    // ARMAR-ENVIAR LA PETICION DE OBTENER TABLA
    //-------------------------------------------------
    // Empieza una transaccion nueva.
    if( !EMPEZAR_VTRANSACCION() ) {
        ELIMINAR_VTRANSACCION();
        if( !EMPEZAR_VTRANSACCION() ) {
            return 0;
        }
    }
    peticion.Header.wCodPet = OBTENER_REGISTROS;
    peticion.Header.wBufferLength = 0;
    strncpy( peticion.Header.szTabla, nombre_tabla, 29 );
    peticion.Header.szTabla[29] = 0;
    peticion.Buffer.sData = 0;
    peticion.Header.bNull = 0;
    if( codicion_select != NULL ) {
        peticion.Header.wSelectLength = strlen( codicion_select ) + 1;
        peticion.Buffer.szSelectCondition = codicion_select;
    }
    else {
        peticion.Header.wSelectLength = 0;
        peticion.Buffer.szSelectCondition = 0;
    }
    // Insertar la peticion.
    if( INCLUIR_PETICION( &peticion ) ) {
        // Envia las transacciones pendientes.
        if( ENVIAR_VTRANSACCION( 1, 0 ) ) {
            //-------------------------------------------------
            // ESPERAR LA RESPUESTA DESDE EL DBROUTER - HOST
            //-------------------------------------------------
            do {
                int bytes = 0;          // bytes leidos desde TCP-IP.
                int h,i;
                // Escuchar TCP-IP hasta recibir algo o salir por TIME-OUT
                //for( h = 0; h < 100 * TIME_OUT_SERVER_NO_RESPONDIO && !bytes; h++ ) {
                for( h = 0;h < 10 * TIME_OUT_SERVER_NO_RESPONDIO;h++ ) {
                    DRIVER( _DRV_tcp, _TCP_status, 0, &bytes, sizeof( bytes ) );
                    if( bytes != 0 ) {
                        break;
                    }
                    #if !defined(INVEL_W) && !defined(INVEL_L)
                    else {
                        SLEEP(100);
                    }
                    #endif
                }
                if( bytes ) {
                    #if defined(INVEL_W) || defined(INVEL_L)
                    rta_drv.buffer = buffer_para_recibir;
                    rta_drv.l_buffer = MAXIMO_BUFFER_TRANSACCION_RECIBIR;
                    rta_drv.rta = 0;
                    DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
                    #else
                    rta_drv.buffer = puntero_memoria_real_driver_invel;
                    rta_drv.l_buffer = MAXIMO_BUFFER_TRANSACCION_RECIBIR;
                    DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
                    memcpy( buffer_para_recibir, puntero_memoria_driver_invel,
                            MAXIMO_BUFFER_TRANSACCION_RECIBIR );
                    #endif
                    if( !rta_drv.rta ) {
                        //-------------------------------------------------
                        // RECIBE LA RESPUESTA DESDE EL DBROUTER - HOST
                        //-------------------------------------------------
                        unsigned short nro_caja;
                        long nro_transaccion;
                        char *stmp = buffer_para_recibir;
                        nro_caja = *( unsigned short* )stmp;
                        stmp += sizeof( unsigned short );
                        nro_transaccion = *( long* )stmp;
                        stmp += sizeof( long );
                        if( nro_caja == NRO_CAJA && nro_transaccion == ultima_transaccion ) {
                            // unsigned short secuencia;
                            unsigned short cant_item = 0;
                            char error_flag;
                            error_flag = *( char* )stmp;
                            stmp += sizeof( char );
                            // secuencia = *(unsigned short *) stmp;
                            stmp += sizeof( unsigned short );
                            cant_item = *( unsigned short* )stmp;
                            stmp += sizeof( unsigned short );
                            //
                            // RECORRE LAS PETICIONES DENTRO DE LA
                            // TRANSACCION.
                            //
                            if( error_flag ) {
                                GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_FLAG_ON, LOG_COMUNICACIONES,2 );
                                rta = 0;
                            }
                            else {
                                for( i = 0;rta && i < cant_item;i++ ) {
                                    // int length = 0;
                                    // unsigned short item_secuencia;
                                    short estado;
                                    // item_secuencia = *(unsigned short *) stmp;
                                    // check item_secuencia
                                    stmp += sizeof( unsigned short );
                                    estado = *( short* )stmp;
                                    stmp += sizeof( short );
                                    // Chequeo de Estado
                                    if( estado < 0 ) {
                                        // Registro con Error;
                                        GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_FLAG_ON , LOG_COMUNICACIONES,2);
                                        rta = 0;
                                    }
                                    else if( estado == 0 ) {
                                        // Ultimo Registro Obtenido OK;
                                        registros_obtenidos = 1;
                                    }         // (estado > 0) Registro
                                    else {
                                        // Obtenido.
                                        if( _WRITE( handle_arch, stmp, estado, __LINE__,
                                                    __FILE__ ) ) {
                                            rta = 0;
                                        }
                                        stmp += estado;
                                    }
                                }
                            }
                        }
                        else {
                            char texto[100];
                            sprintf( texto, "ERROR DE RUTEO: (%d,%li) ESPERADA (%d,%li)", nro_caja,
                                     nro_transaccion, NRO_CAJA, ultima_transaccion );
                            GRABAR_LOG_DBROUTER( texto,LOG_DEBUG,2 );
                            rta = 0;
                        }
                    }
                    else {
                        rta = 0;
                        GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_TCP_IP, LOG_COMUNICACIONES,2 );
                    }
                }
                else {
                    rta = 0;
                    GRABAR_LOG_ID_DBROUTER( SDBR_TIME_OUT_RETORNO_TCP_IP , LOG_COMUNICACIONES,2);
                }
            }
            while( rta && !registros_obtenidos );
            _CLOSE( handle_arch, __LINE__, __FILE__ );
            if( !( rta && !COPIAR_ARCHIVO( nombre_temporal, nombre_archivo, __LINE__, __FILE__ ) ) ) {
                rta = 0;
            }
        }
        else {
            rta = 0;
        }                              // env trans
    }
    else {
        rta = 0;
    }                                  // inc pet
    free( buffer_para_recibir );
    if( rta ) {
        GRABAR_LOG_DBROUTER( "TABLA BIN OK" ,LOG_DEBUG,4);
    }
    else {
        GRABAR_LOG_DBROUTER( "TABLA BIN  ERR",LOG_DEBUG,4 );
    }
    return rta;
}

