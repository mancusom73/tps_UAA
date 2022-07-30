#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h> 
#include <cr_displ.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <log.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <remito.h> 
#include <scanner.h> 
#include <_cr1.h> 
#include <clientes.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_REMITOS
/*****************************************************************************/
void PEDIR_REMITO()
/*****************************************************************************/
{
    long nro_remito = 0;
    if( !NRO_REMITO ) {
        /*---------------- Pide el numero ----------------*/
        if( PEDIR_NRO_REMITO ) {
            if( IMPRESORA_FISCAL || config.pais == CHILE || config.pais == BRASIL ) {
                MOSTRAR_CADENA( 35 - ancho_panel / 2, 18, ST( S__N__DE_GUIA_ ) );
            }
            else {
                MOSTRAR_CADENA( 34 - ancho_panel / 2, 18, ST( S__N__DE_REMITO_ ) );
            }
            nro_remito = PEDIR_GRAN_ENTERO( 26 - ancho_panel / 2, 19, 8 );
            if( nro_remito < 0 ) {
                nro_remito = 0;
            }
        }
        else {
            nro_remito = 1;
        }
        if( nro_remito && !RAM_NRO_CLIENTE ) {
            if( !PEDIR_NRO_CLIENTE( 1, NO ) ) {
                nro_remito = 0;
            }
        }
    }
    SET_MEMORY_LONG( __nro_remito, nro_remito );
    /*------------------- Termina -----------------*/
    MUESTRA_COMPROBANTE();
}
#endif
/***************************************************************************/
int EMITIR_REMITO()
/***************************************************************************/
{
    #ifdef COMPILAR_REMITOS
    return ( NRO_REMITO );
    #else
    return ( 0 );
    #endif
}
/***************************************************************************/
int EMITIR_REMITO_REMOTO()
/***************************************************************************/
{
    int rta = 0;    
    #ifdef COMPILAR_REMITOS
    rta = NRO_REMITO && ( TIPO_REMITO == REMOTO );
    #endif
    return ( rta );
}
#ifdef COMPILAR_REMITOS
/*****************************************************************************/
void FIN_REMITO()
/*****************************************************************************/
{
    if( !EMITIR_REMITO_REMOTO() ) {
        DATOS_COMPROBANTE( _REMITO );
        if( PEDIR_NRO_REMITO ) {
            comp.nro_comp = NRO_REMITO;
        }
    }
    ACTUALIZA_EVENTOS();
    if( !EMITIR_REMITO_REMOTO() ) {
        IMPRIMIR_COMPROBANTE( _REMITO, NO, NO );
    }
    if( config.pais == CHILE || config.pais == BRASIL ) {
        GRABAR_LOG_SISTEMA( "Guia", LOG_VENTAS,2 );
    }
    else {
        GRABAR_LOG_SISTEMA( "Remito" , LOG_VENTAS,2);
    }
    FIN_DE_TICKET( SI );
    #ifdef COMPILAR_DISPLAY
    CR1_DISPLAY_CAJA_ABIERTA();
    #endif
}
#endif

