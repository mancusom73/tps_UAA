#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h>  
#include <cr_disco.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <log.h> 
#include <tcp_ip.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Funciones para el acceso a bases de datos mediante dbrouter
 * ------------------------------------------------------------------------------
 */
static struct
{
    char *buffer_transaccion;                 // cant petciones (2) + peticiones.
    char *agregar_buffer_transaccion;
}
estructura_transaccion = {0,0};
static int canal_abierto = -1;
#define cant_trans   (*(int *)estructura_transaccion.buffer_transaccion)
#define buffer_trans     estructura_transaccion.buffer_transaccion
#define trans_actual  estructura_transaccion.agregar_buffer_transaccion
#define trans_tcpip   estructura_transaccion
/**************************************************************************/
int GET_DBR_ON_LINE( void )
/**************************************************************************/
{
    return ( canal_abierto == 1 ) ? 1 : 0;
}
/**************************************************************************/
void CERRAR_CANAL_DBR( void )
/**************************************************************************/
{
    if( canal_abierto == 1 ) {
        CERRAR_CANAL_TCP_IP( _CANAL_DBROUTER );
        canal_abierto = 0;
    }
}
/**************************************************************************/
int INSERTAR_PETICION_TCPIP( struct _Peticion *peticion )
/**************************************************************************/
{
    if( !buffer_trans ) {
        INIT_BUFFER_TRANSACCION();
    }
    if( !ENTRA_EN_BUFFER_TCPIP( peticion ) ) {
        GRABAR_LOG_ID_DBROUTER( SDBR_PET_NO_ENTRA_TCP_IP ,LOG_ERRORES,2);
        return 0;
    }
    // Copia la Cabecera.
    memcpy( trans_actual, peticion, sizeof( struct _Header ) );
    trans_actual += sizeof( struct _Header );
    // Copia la Condicion del select.
    if( peticion->Header.wSelectLength ) {
        strcpy( trans_actual, peticion->Buffer.szSelectCondition );
        trans_actual += peticion->Header.wSelectLength;
    }
    // Copia los Datos
    if( peticion->Header.wBufferLength ) {
        memcpy( trans_actual, peticion->Buffer.sData, peticion->Header.wBufferLength );
        trans_actual += peticion->Header.wBufferLength;
    }
    cant_trans++;
    return 1;
}
/**************************************************************************/
int MANDAR_BUFFER_TCPIP( void )
/**************************************************************************/
{
    int rta = 0;
    char ip[30];
    if( !PING() ) {
        GRABAR_LOG_DBROUTER( "PING ERROR" ,LOG_ERRORES,2);
        CERRAR_CANAL_DBR();
        return 0;
    }
    if( cant_trans ) {
        int largo_transaccion = ( trans_actual - buffer_trans ) + sizeof( cant_trans );
        if( canal_abierto != 1 ) {
            strcpy( ip, DIRECCION_IP_SERVIDOR );
            if( ABRIR_CANAL_TCP_IP( _CANAL_DBROUTER, ip, _FORMATO_MENSAJE_INVEL ) ) {
                canal_abierto = 1;
            }
            else {
                canal_abierto = 0;
                GRABAR_LOG_ID_DBROUTER( SDBR_ERROR_AL_ENVIAR ,LOG_ERRORES,2);
                return 0;
            }
        }
        rta = _ENVIAR_PAQUETE_TCP_IP( buffer_trans, largo_transaccion, NULL, 0, 0, NO,
                                      _CANAL_DBROUTER );
        if( rta ) {
            // error al enviar
            GRABAR_LOG_ID_DBROUTER( SDBR_ERROR_AL_ENVIAR ,LOG_ERRORES,2);
            CERRAR_CANAL_TCP_IP( _CANAL_DBROUTER );
            canal_abierto = 0;
            // MUESTRA_ERROR_EN_RED();
            {
                char debug[100];
                _snprintf(debug, sizeof(debug)-1,"MANDAR_BUFFER_TCPIP ERROR _ENVIAR_PAQUETE: %d largo %d", rta,
                         largo_transaccion );
                GRABAR_LOG_SISTEMA( debug ,LOG_DEBUG,4);
            }
            return 0;
        }
        else {
            INIT_BUFFER_TRANSACCION();
        }
    }
    return 1;
}
/**************************************************************************/
int INIT_BUFFER_TRANSACCION()
/**************************************************************************/
{
    // inicializa el buffer
    // // cant trans
    if( !buffer_trans ) {
        buffer_trans = (char *)malloc( MAXIMO_BUFFER_TRANSACCION + sizeof( short int ) );
    }
    memset( buffer_trans, 0, sizeof( buffer_trans ) );
    trans_actual = buffer_trans + sizeof( short int );
    // cant_trans = 0;
    return 1;
}
/**************************************************************************/
int ENTRA_EN_BUFFER_TCPIP( struct _Peticion *peticion )
/**************************************************************************/
{
    int tamano_peticion = peticion->Header.wSelectLength + peticion->Header.wBufferLength
                        + sizeof( struct _Header );
    int paquete_ocupado = ( trans_actual - buffer_trans );
    if( ( tamano_peticion + paquete_ocupado + BYTES_AGREGADOS_POR_DRIVER )
      > MAXIMO_BUFFER_TRANSACCION ) {
        return 0;
    }
    return 1;
}
#undef cant_trans
#undef buffer_trans
#undef trans_actual

