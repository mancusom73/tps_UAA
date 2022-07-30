#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <global.h>
#include <abort.h> 
#include <aut_cheq.h> 
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <dtoa.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <netcom.h> 
#include <tkt.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Manejo de autorizaciones de cheques
 * ------------------------------------------------------------------------------
 */
#include <aarch.h>
#undef MK_FP
#ifndef INVEL_L
#include <dos.h>
#endif
#include <aut_tcp.h>
struct _autorizador_envio *aut_envio;
struct _autorizador_respuesta *aut_respuesta;
int VALIDAR_CHEQUE( int ind_medio, int sucursal, int nro_caja, long cajero, long nro_boleta,
                    char *nro_rut_comprador, char *nro_rut_girador, int nro_banco, char *nro_cuenta,
                    long nro_cheque, int nro_plaza, int tipo_credito, double monto,
                    unsigned fecha_vencimiento, int habilitado );
void COMIENZO_AUTORIZADOR();
void LIBERA_MEMORIA_AUTORIZADOR();
/****************************************************************************/
int ABRIR_CANAL_AUTORIZADOR( int protocolo, int reintentos )
/****************************************************************************/
{
    int cantidad = 0, abrio_canal = 0, rta = 1;
    char auxi[50],numip[16];
    do {
        strcpy( numip, NUMERO_IP_AUTORIZADOR_CHEQUES );
        rta = ABRIR_CANAL( protocolo, _CANAL_AUTORIZADOR, numip, _FORMATO_MENSAJE_INVEL );
        if( !rta ) {
            if( reintentos ) {
                _snprintf( auxi, sizeof(auxi)-1, "Error abrir AUTORIZADOR intento %d", cantidad + 1 );
                GRABAR_LOG_SISTEMA( auxi ,LOG_ERRORES,2);
            }
        }
        else {
            abrio_canal = 1;
        }
        cantidad++;
    }
    while( cantidad < reintentos && !abrio_canal );
    return rta;
}
/****************************************************************************/
void FORMATEAR_STRING( char *destino, char *origen, int caracteres )
/****************************************************************************/
{
    int longitud, i, indice = 0, len = 0;
    char aux[50],aux2[50];
    longitud = strlen( origen );
    strcpy( destino, "" );
    for( i = 0;i < longitud;i++ ) {
        if( isdigit( origen[i] ) ) {
            aux[indice] = origen[i];
            indice++;
        }
    }
    aux[indice] = 0;
    longitud = strlen( aux );
    len = caracteres - longitud;
    if( len > 0 ) {
        //agrego los ceros que me faltan
        for( i = 0;i < len;i++ ) {
            sprintf( &aux2[i], "0" );
        }
        strcat( aux2, aux );
        aux2[caracteres + 1] = 0;
        longitud = caracteres;
    }
    //   strcpy( destino, "" );
    //   for( i = longitud; i < 19; i++ )
    //    strcat( destino, "0" );
    //   if( longitud )
    //    strcat( destino, aux );
    if( longitud ) {
        strcat( destino, aux2 );
    }
}
/****************************************************************************/
void CARGAR_DATOS_AUTORIZADOR( int sucursal, int nro_caja, long cajero, long nro_boleta,
                          char *nro_rut_comprador, char *nro_rut_girador, int nro_banco,
                          char *nro_cuenta, long nro_cheque, int nro_plaza, int tipo_credito,
                          double monto, unsigned fecha_vencimiento )
/****************************************************************************/
{
    char aux[30];
    memset( aut_envio, 0, sizeof( struct _autorizador_envio ) );
    memcpy( aut_envio->tipo_trx, "CHQ", 3 );
    _snprintf( aux, sizeof(aux)-1, "%03d", sucursal );
    memcpy( aut_envio->sucursal, aux, 3 );
    _snprintf( aux, sizeof(aux)-1, "%03d", nro_caja );
    memcpy( aut_envio->nro_caja, aux, 3 );
    _snprintf( aux, sizeof(aux)-1, "%08li", cajero );
    memcpy( aut_envio->cajero, aux, 8 );
    memset( aux, 0, 30 );
    _snprintf( aux, sizeof(aux)-1, "%08li", nro_boleta );
    memcpy( aut_envio->nro_boleta, aux, 8 );
    FORMATEAR_STRING( aux, nro_rut_comprador, 10 );
    memcpy( aut_envio->nro_rut_comprador, aux, 10 );
    FORMATEAR_STRING( aux, nro_rut_girador, 10 );
    memcpy( aut_envio->nro_rut_girador, aux, 10 );
    _snprintf( aux, sizeof(aux)-1, "%04d", nro_banco );
    memcpy( aut_envio->nro_banco, aux, 4 );
    FORMATEAR_STRING( aux, nro_cuenta, 19 );
    memcpy( aut_envio->nro_cuenta, aux, 19 );
    _snprintf( aux, sizeof(aux)-1, "%09li", nro_cheque );
    memcpy( aut_envio->nro_cheque, aux, 9 );
    _snprintf( aux, sizeof(aux)-1, "%04d", nro_plaza );
    memcpy( aut_envio->nro_plaza, aux, 4 );
    _snprintf( aux, sizeof(aux)-1, "%02d", tipo_credito );
    memcpy( aut_envio->tipo_credito, aux, 2 );
    _snprintf( aux, sizeof(aux)-1, "%012.2f", monto );
    aux[9] = aux[10];
    aux[10] = aux[11];
    aux[11] = 0;
    memcpy( aut_envio->monto, aux, 11 );
    DTOA( fecha_vencimiento, aux, SI );
    memcpy( aut_envio->fecha_vencimiento, aux, 8 );
}
/****************************************************************************/
int ENVIAR_DATOS_CHEQUE( int protocolo )
/****************************************************************************/
{
    int ok = 0, error = 1, reintentar = 2;
    while( reintentar ) {
        MENSAJE_SIN_SONIDO_STRING( S_SOLICITANDO_AUTORIZACION );
        error = _ENVIAR_PAQUETE(
        ( int )protocolo, ( char* )aut_envio, sizeof( struct _autorizador_envio ), //envio
        ( char* )aut_respuesta, sizeof( struct _autorizador_respuesta ),    //rta
        ( int )TIME_OUT_SERVER_AUTORIZADOR_CHEQ, SI, ( int )_CANAL_AUTORIZADOR );
        BORRAR_MENSAJE();
        if( !error ) {
            ok = 1;
            reintentar = 0;
        }
        else {
            reintentar--;
        }
    }
    return ( ok );
}
/****************************************************************************/
int VALIDAR_CHEQUE( int ind_medio, int sucursal, int nro_caja, long cajero, long nro_boleta,
                    char *nro_rut_comprador, char *nro_rut_girador, int nro_banco, char *nro_cuenta,
                    long nro_cheque, int nro_plaza, int tipo_credito, double monto,
                    unsigned fecha_vencimiento, int habilitado )
/****************************************************************************/
{
    int rta = 0;
    int reintentos_autorizador = 1;
    if( !ABRIR_CANAL_AUTORIZADOR( TCP_IP, reintentos_autorizador ) ) {
        MENSAJE_STRING( S_ERROR_AL_ABRIR_AUTORIZADOR );
    }
    else {
        CARGAR_DATOS_AUTORIZADOR( sucursal, nro_caja, cajero, nro_boleta, nro_rut_comprador,
                                  nro_rut_girador, nro_banco, nro_cuenta, nro_cheque, nro_plaza,
                                  tipo_credito, monto, fecha_vencimiento );
        rta = ENVIAR_DATOS_CHEQUE( TCP_IP );
        if( rta ) {
            char aux[150];
            memset( aux, 0, sizeof( aux ) );
            memcpy( aux, ( char* )aut_respuesta, sizeof( aut_respuesta ) );
            aux[sizeof( aut_respuesta )] = 0x00;
            memset( aux, 0, sizeof( aux ) );
            memcpy( aux, ( char* )aut_respuesta->codigo_respuesta, 4 );
            aux[4] = 0x00;
            rta = atoi( aux );
            memset( aux, 0, sizeof( aux ) );
            memcpy( aux, ( char* )aut_respuesta->desc_respuesta, 32 );
            aux[32] = 0x00;
            switch( rta ) {
                case 0:
                    // Aprobado
                    MENSAJE( aux );
                    FORMATEAR_STRING( aux, aut_respuesta->autorizacion, 10 );
                    SET_MEMORY( __var_tarjetas_autorizacion, aux );
                    rta = 1;
                    break;
                case 1:
                    // Rechazado
                    strcat( aux, ST( S____PRESIONE_UNA_TECLA__ ) );
                    MENSAJE_TECLA( aux );
                    rta = 0;
                    if( habilitado == 1 && PIDE_SUPERVISOR_EN_CHEQUE_RECHAZ ) {
                        medios[ind_medio].autorizacion = 2;
                        rta = 1;
                    }
                    break;
                case 2:
                    // Pedir Supervisor
                    strcat( aux, ST( S____PRESIONE_UNA_TECLA__ ) );
                    MENSAJE_TECLA( aux );
                    medios[ind_medio].autorizacion = 2;
                    rta = 1;
                    break;
            }
        }
        else {
            if( PEDIR_SUPERVISOR_AUTORIZADOR_CHE ) {
                medios[ind_medio].autorizacion = 2;
                rta = 1;
            }
        }
        CERRAR_CANAL( TCP_IP, _CANAL_AUTORIZADOR );
    }
    return rta;
}
/****************************************************************************/
void COMIENZO_AUTORIZADOR()
/****************************************************************************/
{
    aut_envio = NULL;
    aut_respuesta = NULL;
    if( UTILIZA_AUTORIZADOR_CHEQUES ) {
        aut_envio = (struct _autorizador_envio *)MALLOC( sizeof( struct _autorizador_envio ) );
        aut_respuesta = (struct _autorizador_respuesta *)MALLOC( sizeof( struct _autorizador_respuesta ) );
        if( !aut_envio || !aut_respuesta ) {
            ABORT( "ERROR AL ABRIR AUTORIZADOR" );
        }
    }
    FREEMEM( ( char* )aut_envio );
    FREEMEM( ( char* )aut_respuesta );
}
/****************************************************************************/
void LIBERA_MEMORIA_AUTORIZADOR()
/****************************************************************************/
{
    if( aut_envio ) {
        free( aut_envio );
    }
    if( aut_respuesta ) {
        free( aut_respuesta );
    }
}

