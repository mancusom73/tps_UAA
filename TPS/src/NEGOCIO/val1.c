#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <clave.h> 
#include <cr.h> 
#include <pausa.h> 
#include <val.h> 
#include <val1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
#undef MK_FP
#ifndef INVEL_L
#endif
int __RECIBIR_RESPUESTA( unsigned char *rta, unsigned char cant );
/*----------------- Variables y definiciones globales -----------------*/
unsigned char status,cant_bytes,rta1,rta2;
static int _PUERTO = 1;
#define STX 0x06
char *MK_FP_BIOS_PROTEGIDO( unsigned off );
static int TICKS_ESPERA1 = 10;
void VAL_ENVIAR_DATOS( unsigned char *datos, unsigned char cant );
//static int TICKS_ESPERA2 = 18;
/***************************************************************************/
void VAL_SET_PUERTO( int puerto )
/***************************************************************************/
{
    _PUERTO = puerto - 1;
}
/***************************************************************************/
int VAL_INICIALIZAR_PUERTO()
/***************************************************************************/
{
    unsigned status = 1;
    #if !defined(INVEL_W) && !defined(INVEL_L)
    status = bioscom( 0, 0xE3, _PUERTO );         /* 111 00 0 11  = 0xE3 */
    #endif
    PAUSA( 27 );
    return ( status );
}
/***************************************************************************/
void VAL_DESINSTALAR_PUERTO()
/***************************************************************************/
{
}
/***************************************************************************/
int VAL_ENVIAR_COMANDO( buf_datos, long_datos, buffer_rta )
unsigned char *buf_datos,long_datos,*buffer_rta;
/***************************************************************************/
{
    unsigned i, resultado = 0, bytes, len;
    unsigned char lrc,buffer[256],buf_envio[256];
    /*---------------------- Peparamos el envio ------------------*/
    buf_envio[0] = STX;
    buf_envio[1] = long_datos;
    memcpy( &buf_envio[2], buf_datos, long_datos );
    for( i = 0, lrc = 0;i < ( unsigned )( long_datos + 2 );i++ ) {
        lrc = lrc ^ buf_envio[i];
    }
    buf_envio[2 + long_datos] = lrc;
    /*------------------------ Envia comando ---------------------*/
    VAL_ENVIAR_DATOS( buf_envio, ( char )( long_datos + 3 ) );
    /*---------------------- Espera Respuesta --------------------*/
    memset( buffer, 0, 255 );
    bytes = VAL_RECIBIR_RESPUESTA( &buffer[0] );
    /*----------------------- controla LRC -----------------------*/
    if( bytes ) {
        if( buffer[0] == STX ) {
            len = buffer[1];
            for( i = 0, lrc = 0;i < bytes && i < len + 2;i++ ) {
                lrc ^= buffer[i];
            }
            resultado = ( lrc == buffer[i] ) ? 1 : 0;
        }
    }
    /*---------------------- Copia respuesta ---------------------*/
    if( resultado && len ) {
        status = buffer[2];
        memcpy( buffer_rta, &buffer[3], len );
        cant_bytes = len;
    }
    else {
        cant_bytes = 0;
    }
    return ( resultado );
}
/***************************************************************************/
int VAL_RECIBIR_RESPUESTA( unsigned char *rta )
/***************************************************************************/
{
    int bytes = 0;
    if( __RECIBIR_RESPUESTA( rta, 3 ) ) {
        bytes = 3;
        if( rta[1] > 250 ) {
            rta[1] = 250;
        }
        if( __RECIBIR_RESPUESTA( &rta[3], rta[1] ) ) {
            bytes += rta[1];
        }
    }
    return ( bytes );
}
/***************************************************************************/
int __RECIBIR_RESPUESTA( unsigned char *rta, unsigned char cant )
/***************************************************************************/
{
    int ok = 0;
    #if !defined(INVEL_W) && !defined(INVEL_L)
    unsigned status;
    int i = 0, time_out = 0;
    unsigned long t_inicial,*ta;
    ta = ( unsigned long* )MK_FP_BIOS_PROTEGIDO( 0x6c );
    t_inicial = *ta;
    while( i < cant && !time_out ) {
        status = bioscom( 3, 0, _PUERTO );
        if( status & 0x100 ) {
            status = bioscom( 2, 0, _PUERTO );
            rta[i] = ( unsigned char ) ( status & 0x00ff );
            i++;
        }
        if( *ta < t_inicial ) {
            t_inicial = *ta;
        }
        else if( *ta - t_inicial > 9 ) {
            time_out = 1;
        }
    }
    if( i == cant ) {
        ok = 1;
    }
    #endif
    return ( ok );
}
/***************************************************************************/
void VAL_ENVIAR_DATOS( unsigned char *datos, unsigned char cant )
/***************************************************************************/
{
    int i;
    for( i = 0;i < cant;i++ ) {
        #if !defined(INVEL_W) && !defined(INVEL_L)
        bioscom( 1, datos[i], _PUERTO );
        #endif
    }
}
/***************************************************************************/
int VAL_GET_ESPERA_TICKS()
/***************************************************************************/
{
    return ( TICKS_ESPERA1 );
}
/***************************************************************************/
void VAL_SET_ESPERA_TICKS( int ticks )
/***************************************************************************/
{
    TICKS_ESPERA1 = ticks;
}

