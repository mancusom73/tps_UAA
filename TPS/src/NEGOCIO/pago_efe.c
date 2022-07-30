#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cmemory.h> 
#include <cr.h> 
#include <pago_efe.h> 
#include <round.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_TECLA_REPROCESAR
/*****************************************************************************/
void REPROCESAR_EFECTIVO( char tipo_pago )
/*****************************************************************************/
{
    int h;
    double tot_efectivo = 0, tot_vueltos_propios = 0, tot_vueltos_medios = 0;
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        if( medios[_PAGO_MODO( h )].efectivo ) {
            tot_efectivo = _PAGO_IMPORTE( h );
            ADD_MEMORY_DOUBLE( __x_tot_dinero, -( _PAGO_IMPORTE( h ) ) );
            if( tipo_pago == 1 )                  //Cobros
            {
                _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_MODO( h ), -( _PAGO_IMPORTE( h ) ) );
                _ADD_MEMORY_INT( __x_cobros_cantidad, _PAGO_MODO( h ), -1 );
            }                                     //ventas
            else {
                _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_MODO( h ), -( _PAGO_IMPORTE( h ) ) );
                _ADD_MEMORY_INT( __x_ventas_cantidad, _PAGO_MODO( h ), -1 );
            }
        }
        tot_vueltos_propios += _PAGO_VUELTO( h );
        tot_vueltos_medios += _PAGO_VUELTO_MEDIO( h );
    }
    if( RAM_VUELTO - tot_vueltos_medios > 0.009 ) {
        if( tipo_pago == 1 ) {
            _ADD_MEMORY_DOUBLE( __x_cobros_importe, _medio_efectivo,
                                RAM_VUELTO - tot_vueltos_medios );
        }                                         //ventas
        else {
            _ADD_MEMORY_DOUBLE( __x_ventas_importe, _medio_efectivo,
                                RAM_VUELTO - tot_vueltos_medios );
        }
        ADD_MEMORY_DOUBLE( __x_tot_dinero,
                           ROUND( RAM_VUELTO - tot_vueltos_medios, 10, DECIMALES ) );
    }
    if( RAM_VUELTO - tot_efectivo - tot_vueltos_propios - tot_vueltos_medios > 0.009 ) {
        ADD_MEMORY_DOUBLE( __x_vueltos_importe,
                           -( ROUND( RAM_VUELTO - tot_efectivo - tot_vueltos_propios, 10,
                                     DECIMALES ) ) );
        ADD_MEMORY_INT( __x_vueltos_cantidad, -1 );
    }
}
#endif

