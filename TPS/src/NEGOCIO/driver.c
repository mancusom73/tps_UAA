#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <cr.h> 
#include <dd.h> 
#include <driver.h> 
#include <mstring.h> 
#include <power.h>
#include <cmemory.h>
#include <cajon.h>
#include <tar_mag.h>
#include <tcp_ip.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
extern int INT_DRIVER;
//static char driver_ok;
extern int TCP( unsigned funcion, unsigned bx, char *buff, int longitud );
/***************************************************************************/
void DRIVER( int funcion, int subfuncion, int bx, void *valor, int longitud )
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    int rta = 0;
    switch( funcion ) {
        case _DRV_tcp:
            rta = TCP( subfuncion, bx, valor, longitud );
            break;
    }
    // copiamos la respuesta
    *( int* )valor = rta;
    #else
    REGS16 regs;
    char *pun;
    SEL selector;
    USHORT paragraph;
    if( DosAllocRealSeg( longitud, &paragraph, &selector ) != 0 ) {
        printf( "DosAllocRealSeg() failed\n" );
        exit( 255 );
    }
    pun = MK_FP( selector, 0 );
    if( valor && longitud >= 0 ) {
        memcpy( pun, valor, longitud );
    }
    regs.ax = ( funcion << 8 ) + subfuncion;
    regs.bx = bx;
    regs.ds = paragraph;
    regs.si = 0;
    DosRealIntr( INT_DRIVER, &regs, 0L, 0 );
    if( valor && longitud >= 0 ) {
        memcpy( valor, pun, longitud );
    }
    DosFreeRealSeg( selector );
    #endif
}
/***************************************************************************/
void INIT_DRIVERS_PRIMARIOS()
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    char *p;
    /*------------- Carga el vector de interrupcion del driver ------------*/
    p = getenv( "DRIVER_TPV_INT" );
    if( p ) {
        INT_DRIVER = atoi( p );
    }
    /*---------------- Verifica si el driver esta cargado -----------------*/
    if( getvect( INT_DRIVER ) ) {
        driver_ok = 1;
    }
    if( !_RNV_VIRTUAL ) {
        if( !INIT_RNV() ) {
            ABORT( "ERROR AL INICIALIZAR EL DRIVER DE RNV" );
        }
    }
    #endif
}
/***************************************************************************/
void INIT_DRIVERS_SECUNDARIOS()
/***************************************************************************/
{
    if( !INIT_POWER_DOWN() ) {
        if( MODO_DEBUG == 3 || MODO_DEBUG == 50 ) {
            GRABAR_LOG_SISTEMA_STRING( S_ERROR_DRIVER_POWER, LOG_ERRORES,2 );
        }
        ABORT_STRING( S_ERROR_DRIVER_POWER );
    }
    if( !INIT_CAJON() ) {
        ABORT_STRING( S_ERROR_DRIVER_CAJON );
    }
    if( !INIT_TARJETA_MAGNETICA() ) {
        ABORT_STRING( S_ERROR_LECTOR_TARJ_MAG );
    }
    if( !INICIALIZAR_TECLADO() ) {
        ABORT_STRING( S_ERROR_TECLADO );
    }
    if( !INIT_DRV_TCP_IP() ) {
        if( MODO_DEBUG == 3 || MODO_DEBUG == 50 ) {
            GRABAR_LOG_SISTEMA_STRING( S_ERROR_TCP_IP , LOG_ERRORES,2);
        }
        MENSAJE_STRING( S_ERROR_TCP_IP );
    }
    #ifdef COMPILAR_PINPAD
    if( !INIT_PINPAD() ) {
        if( MODO_DEBUG == 3 || MODO_DEBUG == 50 ) {
			GRABAR_LOG_SISTEMA_STRING( S_ERROR_PINPAD, LOG_ERRORES,2 );
        }
        MENSAJE_STRING( S_ERROR_PINPAD );
    }
    #endif
    /*
     * if( !INIT_T_CHIP() ) {
     * //if ( MODO_DEBUG == 3 )  GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_MONEDERO );
     * //MENSAJE_STRING( S_ERROR_EN_MONEDERO );
     * }
     */
}

