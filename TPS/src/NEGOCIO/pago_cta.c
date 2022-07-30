#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <baucher.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <m_cadena.h> 
#include <pago_cta.h> 
#include <pant.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
long PIDE_CUENTA( char largo )
/*****************************************************************************/
{
    long cta;
    cta = PEDIR_GRAN_ENTERO( ( 80 - ancho_panel - largo * 2 ) / 2, 23, largo );
    return ( cta );
}
/*****************************************************************************/
int PIDE_COMPROBANTE()
/*****************************************************************************/
{
    int cta;
    cta = ( int )PEDIR_GRAN_ENTERO( 35 - ancho_panel / 2, 23, 4 );
    return ( cta );
}
/*****************************************************************************/
void REPROCESAR_CTA_CTE( char tipo_pago )
/*****************************************************************************/
{
    int h,cant;
    long anulado;
    for( h = 0, cant = 0;h < RAM_P_PAGO;h++ ) {
        if( medios[( int )_PAGO_MODO( h )].cta_cte ) {
            cant++;
            if( tipo_pago == 1 ) {
                _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_MODO( h ), -( _PAGO_IMPORTE( h ) ) );
                _ADD_MEMORY_INT( __x_cobros_cantidad, _PAGO_MODO( h ), -1 );
            }
            else {
                _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_MODO( h ), -( _PAGO_IMPORTE( h ) ) );
                _ADD_MEMORY_INT( __x_ventas_cantidad, _PAGO_MODO( h ), -1 );
            }
            if( EMITIR_VOUCHER_CTA_CTE ) {
                /*---------- VOUCHER de anulacion -----------*/
                anulado = _PAGO_NRO_CUPON_INT1( h );
                IMPRIME_VOUCHER_CTACTE( h, 1, anulado, _MODO_VENTA );
                IMPRIME_VOUCHER_CTACTE( h, 0, anulado, _MODO_VENTA );
            }
        }
    }
}
/*****************************************************************************/
void IMPRIME_DATOS_CTA_CTE( long cta )
/*****************************************************************************/
{
    if( USAR_CUIT_COMO_NRO_CLIENTE ) {
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_CUENTA_N_ ) );
        EXISTE_CLIENTE( cta );
        MOSTRAR_CLIENTE( 11, PRN, clientes.cod_cliente );
    }
    else {
        MOSTRAR_CADENA( 21, PRN, ST( S_CUENTA_N_ ) );
        MOSTRAR_ENTERO( 31, PRN, "00000000", cta );
    }
}
/*****************************************************************************/
void IMPRIME_NOMBRE_CLIENTE( char *nom )
/*****************************************************************************/
{
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CLIENTE__ ) );
    MOSTRAR_CADENA( 11, PRN, nom );
}

