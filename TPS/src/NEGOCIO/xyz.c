#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cio.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <eventos.h> 
#include <fecha.h> 
#include <fiscal.h> 
#include <get_fech.h> 
#include <malloc.h> 
#include <mstring.h> 
#include <round.h> 
#include <tkt.h> 
#include <ventas2.h> 
#include <xyz.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <config.h>
#include "bridge.h"
#if !defined(INVEL_W) && !defined(INVEL_L)
#include <dos.h>
#endif
#ifdef INVEL_L
#include <sys/io.h>
#else
#endif
#include <math.h>
#include <archivos.h>
#include <path.h>
struct _xyz
{
    unsigned short fecha;
    long nro;
    unsigned short cant_tickets;
    unsigned short dia_operativo;
    double gran_total;
    char reserva[20];
};
extern long GRABAR_CIERRE( void );
double REDONDEA_X_PARA_Z(double importe_de_x );
int _GRABAR_XYZ( char *nom, int est, struct _xyz *cab_xyz );
extern void ACTUALIZAR_X( void );
/*****************************************************************************/
void RESETEA_ACUMULADORES_X()
/*****************************************************************************/
{
    CEREAR_STRUCT( _x );
    SET_MEMORY_LONG( __x_nro_primer_ticket, NRO_TICKET+1);
    SET_MEMORY_LONG( __x_cantidad_tickets, 0 );
    //Para servicios la transaccion inicial se utiliza para determinar
    //el numero de id_evento del ultimo cierre X o Z
    if( MODO_NEGOCIO == SERVICIOS ) {
        SET_MEMORY_LONG( __x_nro_transaccion_inicial, NRO_EVENTO );
    }
    else {
        SET_MEMORY_LONG( __x_nro_transaccion_inicial, NRO_EVENTO );
    }
}
/*****************************************************************************/
void RESETEA_ACUMULADORES_Z()
/*****************************************************************************/
{
    int h;
    double total = 0, aux = 0;
    long nro_z = 0;
    for( h = 0;h < 10;h++ ) {
        total += _Z_VENTAS_IMPORTE( h );
    }
    ADD_MEMORY_DOUBLE( __nro_gran_total, total );
    COPY_STRUCT( _z_ant, _z );
    CEREAR_STRUCT( _z );
    #if defined(INVEL_W) || defined(INVEL_L)
    if( IMPRESORA_FISCAL ) {
        //CONSULTAR_DATOS_IMPRESORA_FISCAL( NULL, &nro_z );
        //SET_MEMORY_LONG( __nro_z, nro_z );
    }
    else
            #endif
    {
        ADD_MEMORY_LONG( __nro_z, 1L );
    }

    SET_MEMORY_LONG( __z_nro_primer_ticket, NRO_TICKET+1);
    /*SET_MEMORY_LONG( __z_nro_primer_ticket,
                     ( ( RAM_FACTURA && MODO_FACTURACION_POR_DEFECTO )
                    || ( NC_PERMANENTE && RAM_NOTA_CR ) ) ? 0 : NRO_TICKET );*/
    SET_MEMORY_LONG( __z_cantidad_tickets, 0 );
    SET_MEMORY_LONG( __z_nro_transaccion_inicial, NRO_EVENTO );
    if( NRO_GRAN_TOTAL >= pow( ENTEROS + 4, 10 ) ) {
        SET_MEMORY( __nro_gran_total, ( char* )&aux );
    }
    if( RESET_BONOS ) {
        SET_MEMORY_LONG( __nro_total_bonos, 0 );
    }
}
/*****************************************************************************/
void ACUMULAR_X_EN_Z()
/*****************************************************************************/
{
    int h,i;
    for( h = 0;h < 10;h++ ) {
        _SET_MEMORY_DOUBLE( __z_ventas_importe_sin_redondeo, h,
                            _Z_VENTAS_IMPORTE_SIN_REDONDEO( h )
                          + REDONDEA_X_PARA_Z(_X_VENTAS_IMPORTE_SIN_REDONDEO( h ) ));
        _SET_MEMORY_DOUBLE( __z_ventas_importe, h,
                            _Z_VENTAS_IMPORTE( h ) + REDONDEA_X_PARA_Z(_X_VENTAS_IMPORTE( h ) ));
        _ADD_MEMORY_INT( __z_ventas_cantidad, h, _X_VENTAS_CANTIDAD( h ) );
        _SET_MEMORY_DOUBLE( __z_retiros_importe, h,
                            _Z_RETIROS_IMPORTE( h ) + REDONDEA_X_PARA_Z(_X_RETIROS_IMPORTE( h ) ));
        _ADD_MEMORY_INT( __z_retiros_cantidad, h, _X_RETIROS_CANTIDAD( h ) );
        _SET_MEMORY_DOUBLE( __z_cobros_importe, h,
                            _Z_COBROS_IMPORTE( h ) + REDONDEA_X_PARA_Z(_X_COBROS_IMPORTE( h )) );
        _SET_MEMORY_DOUBLE( __z_cobros_importe_me, h,
                            _Z_COBROS_IMPORTE_ME( h ) + REDONDEA_X_PARA_Z(_X_COBROS_IMPORTE_ME( h ) ));
        _ADD_MEMORY_INT( __z_cobros_cantidad, h, _X_COBROS_CANTIDAD( h ) );
        _SET_MEMORY_DOUBLE( __z_fondo_fijo_multiple_importe, h,
                            _Z_FONDO_FIJO_MULTIPLE_IMPORTE( h )
                          + REDONDEA_X_PARA_Z(_X_FONDO_FIJO_MULTIPLE_IMPORTE( h )) );
        _ADD_MEMORY_INT( __z_fondo_fijo_multiple_cantidad, h,
                         _X_FONDO_FIJO_MULTIPLE_CANTIDAD( h ) );
        _SET_MEMORY_DOUBLE( __z_cambios_importe, h,
                            _Z_CAMBIOS_IMPORTE( h ) + REDONDEA_X_PARA_Z(_X_CAMBIOS_IMPORTE( h ) ) );
        _ADD_MEMORY_INT( __z_cambios_cantidad, h, _X_CAMBIOS_CANTIDAD( h ) );
        _SET_MEMORY_DOUBLE( __z_fondo_fijo_multiple_imp_me, h,
                            _Z_FONDO_FIJO_MULTIPLE_IMP_ME( h )
                          + REDONDEA_X_PARA_Z(_X_FONDO_FIJO_MULTIPLE_IMP_ME( h ) ) );
        _SET_MEMORY_DOUBLE( __z_retiros_importe_me, h,
                            _Z_RETIROS_IMPORTE_ME( h ) + REDONDEA_X_PARA_Z( _X_RETIROS_IMPORTE_ME( h ) ) );
        _SET_MEMORY_DOUBLE( __z_retiro_fondo_importe_pagos, h, 
                            _Z_RETIRO_FONDO_IMPORTE_PAGOS(h) + REDONDEA_X_PARA_Z(_X_RETIRO_FONDO_IMPORTE_PAGOS(h)) );
    }
    #ifdef COMPILAR_MOZO
    for( h = 0;h < CANT_MOZOS;h++ ) {
        _SET_MEMORY_DOUBLE( __z_mozos_importe, h, _Z_MOZOS_IMPORTE( h ) + REDONDEA_X_PARA_Z(_X_MOZOS_IMPORTE( h ) ) );
        _SET_MEMORY_DOUBLE( __z_mozos_cantidad, h,
                            _Z_MOZOS_CANTIDAD( h ) + _X_MOZOS_CANTIDAD( h ) );
    }
    #endif
    for( h = 0;h < 24;h++ ) {
        _ADD_MEMORY_UINT( __z_demora_segundos, h, _X_DEMORA_SEGUNDOS( h ) );
        _ADD_MEMORY_UINT( __z_demora_ventas, h, _X_DEMORA_VENTAS( h ) );
    }
    SET_MEMORY_DOUBLE( __z_adelantos_importe, Z_ADELANTOS_IMPORTE + REDONDEA_X_PARA_Z(X_ADELANTOS_IMPORTE ));
    ADD_MEMORY_INT( __z_adelantos_cantidad, X_ADELANTOS_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_intereses_importe, Z_INTERESES_IMPORTE + REDONDEA_X_PARA_Z(X_INTERESES_IMPORTE ));
    ADD_MEMORY_INT( __z_intereses_cantidad, X_INTERESES_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_fondo_fijo_importe, Z_FONDO_FIJO_IMPORTE + REDONDEA_X_PARA_Z(X_FONDO_FIJO_IMPORTE ));
    ADD_MEMORY_INT( __z_fondo_fijo_cantidad, X_FONDO_FIJO_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_anulaciones_importe, Z_ANULACIONES_IMPORTE + REDONDEA_X_PARA_Z(X_ANULACIONES_IMPORTE ));
    ADD_MEMORY_INT( __z_anulaciones_cantidad, X_ANULACIONES_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_dev_envases_importe, Z_DEV_ENVASES_IMPORTE + REDONDEA_X_PARA_Z(X_DEV_ENVASES_IMPORTE ));
    ADD_MEMORY_INT( __z_dev_envases_cantidad, X_DEV_ENVASES_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_devoluciones_importe, Z_DEVOLUCIONES_IMPORTE + REDONDEA_X_PARA_Z(X_DEVOLUCIONES_IMPORTE ));
    ADD_MEMORY_INT( __z_devoluciones_cantidad, X_DEVOLUCIONES_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_ventas_gravadas, Z_VENTAS_GRAVADAS + REDONDEA_X_PARA_Z(X_VENTAS_GRAVADAS ));
    SET_MEMORY_DOUBLE( __z_ventas_exentas, Z_VENTAS_EXENTAS + REDONDEA_X_PARA_Z(X_VENTAS_EXENTAS ));
    SET_MEMORY_DOUBLE( __z_imp_int, Z_IMP_INT + REDONDEA_X_PARA_Z( X_IMP_INT ) );
    if( INF10_HABILITADO ) {
        for( h = 0;h < CANT_DEPTOS_RNV;h++ ) {
            _ADD_MEMORY_DOUBLE( __z_dep_imp, h, _X_DEP_IMP( h ) );
            _ADD_MEMORY_DOUBLE( __z_dep_can, h, _X_DEP_CAN( h ) );
        }
    }
    SET_MEMORY_DOUBLE( __z_tot_dinero, Z_TOT_DINERO + REDONDEA_X_PARA_Z( X_TOT_DINERO ) );
    SET_MEMORY_DOUBLE( __z_iva1, Z_IVA1 + REDONDEA_X_PARA_Z( X_IVA1 ) );
    SET_MEMORY_DOUBLE( __z_iva2, Z_IVA2 + REDONDEA_X_PARA_Z( X_IVA2 ) );
    SET_MEMORY_DOUBLE( __z_retiro_fondo_importe, Z_RETIRO_FONDO_IMPORTE + REDONDEA_X_PARA_Z( X_RETIRO_FONDO_IMPORTE ));
    ADD_MEMORY_INT( __z_retiro_fondo_cantidad, X_RETIRO_FONDO_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_pagos_por_caja_importe,
                       Z_PAGOS_POR_CAJA_IMPORTE + REDONDEA_X_PARA_Z(X_PAGOS_POR_CAJA_IMPORTE ) );
    ADD_MEMORY_INT( __z_pagos_por_caja_cantidad, X_PAGOS_POR_CAJA_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_facturas_a_importe, Z_FACTURAS_A_IMPORTE + REDONDEA_X_PARA_Z( X_FACTURAS_A_IMPORTE ));
    ADD_MEMORY_INT( __z_facturas_a_cantidad, X_FACTURAS_A_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_facturas_b_importe, Z_FACTURAS_B_IMPORTE + REDONDEA_X_PARA_Z(X_FACTURAS_B_IMPORTE ));
    ADD_MEMORY_INT( __z_facturas_b_cantidad, X_FACTURAS_B_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_nc_a_importe, Z_NC_A_IMPORTE + REDONDEA_X_PARA_Z(X_NC_A_IMPORTE ));
    ADD_MEMORY_INT( __z_nc_a_cantidad, X_NC_A_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_nc_b_importe, Z_NC_B_IMPORTE + REDONDEA_X_PARA_Z( X_NC_B_IMPORTE ));
    ADD_MEMORY_INT( __z_nc_b_cantidad, X_NC_B_CANTIDAD );
    SET_MEMORY_DOUBLE( __z_vueltos_importe, Z_VUELTOS_IMPORTE + REDONDEA_X_PARA_Z( X_VUELTOS_IMPORTE ));
    ADD_MEMORY_INT( __z_vueltos_cantidad, X_VUELTOS_CANTIDAD );
    ADD_MEMORY_UINT( __z_bonos_entregados, X_BONOS_ENTREGADOS );
    //SET_MEMORY_DOUBLE( __z_ila, ROUND( Z_ILA + X_ILA, 10, 2 ) );
    //SET_MEMORY_DOUBLE( __z_iha, ROUND( Z_IHA + X_IHA, 10, 2 ) );
    //SET_MEMORY_DOUBLE( __z_alicuota, ROUND( Z_ALICUOTA + X_ALICUOTA, 10, 2 ) );
    for( i = 0;i < 4;i++ ) {
        _SET_MEMORY_DOUBLE( __z_impuestos, i, _Z_IMPUESTOS( i ) + REDONDEA_X_PARA_Z(_X_IMPUESTOS( i ) ));
    }

    SET_MEMORY_DOUBLE( __z_diferencia, Z_DIFERENCIA + REDONDEA_X_PARA_Z(X_DIFERENCIA) );
    for( h = 0;h < MAX_COD_IVA;h++ ) {
        _ADD_MEMORY_DOUBLE( __z_importe_iva, h, REDONDEA_X_PARA_Z(_X_IMPORTE_IVA( h ) ));
        _ADD_MEMORY_DOUBLE( __z_importe_sobretasa_iva, h, REDONDEA_X_PARA_Z(_X_IMPORTE_SOBRETASA_IVA( h ) ) );
    }
    SET_MEMORY_DOUBLE( __z_correccion_importe, Z_CORRECCION_IMPORTE + REDONDEA_X_PARA_Z( X_CORRECCION_IMPORTE ) );
    ADD_MEMORY_INT( __z_correccion_cantidad, X_CORRECCION_CANTIDAD );
    ADD_MEMORY_LONG( __z_cantidad_tickets, X_CANTIDAD_TICKETS );
    ADD_MEMORY_INT( __z_cobros_cantidad_servicios, X_COBROS_CANTIDAD_SERVICIOS );
	ADD_MEMORY_DOUBLE( __z_cobros_importes_servicios, REDONDEA_X_PARA_Z( X_COBROS_IMPORTES_SERVICIOS ));
    ADD_MEMORY_DOUBLE( __z_retencion_dgr, REDONDEA_X_PARA_Z( X_RETENCION_DGR ));
    ADD_MEMORY_DOUBLE( __z_percepcion_dgi, REDONDEA_X_PARA_Z( X_PERCEPCION_DGI ));
    ADD_MEMORY_DOUBLE( __z_percepcion_212_iva1, REDONDEA_X_PARA_Z( X_PERCEPCION_212_IVA1 ));
    ADD_MEMORY_DOUBLE( __z_percepcion_212_iva2, REDONDEA_X_PARA_Z(X_PERCEPCION_212_IVA2 ));
    ADD_MEMORY_DOUBLE( __z_diferencia_redondeo, X_DIFERENCIA_REDONDEO );
    ADD_MEMORY_INT( __z_cant_articulos, X_CANT_ARTICULOS );
    ADD_MEMORY_INT( __z_cant_art_escaneados, X_CANT_ART_ESCANEADOS );
    ADD_MEMORY_UINT( __z_tmpo_reg_articulo, X_TMPO_REG_ARTICULO );
    ADD_MEMORY_UINT( __z_tmpo_pago, X_TMPO_PAGO );
    ADD_MEMORY_UINT( __z_tmpo_standby, X_TMPO_STANDBY );
    ADD_MEMORY_UINT( __z_tmpo_ocio, X_TMPO_OCIO );
    ADD_MEMORY_UINT( __z_tmpo_no_venta, X_TMPO_NO_VENTA );
    ADD_MEMORY_DOUBLE( __z_imp_cancelados, REDONDEA_X_PARA_Z( X_IMP_CANCELADOS ));
    ADD_MEMORY_INT( __z_cant_cancelados, X_CANT_CANCELADOS );

    ADD_MEMORY_DOUBLE( __z_donaciones_importe, REDONDEA_X_PARA_Z( X_DONACIONES_IMPORTE ));
    ADD_MEMORY_INT( __z_donaciones_cantidad, X_DONACIONES_CANTIDAD );
    for( h = 0 ;h < 6 ;h++ ) {
        _ADD_MEMORY_DOUBLE( __z_desc_porc_importe, h, REDONDEA_X_PARA_Z(_X_DESC_PORC_IMPORTE( h ) ));
        _ADD_MEMORY_INT( __z_desc_porc_cantidad, h, _X_DESC_PORC_CANTIDAD( h ) );
        _ADD_MEMORY_DOUBLE( __z_desc_porc_importe_sin_iva, h, REDONDEA_X_PARA_Z(_X_DESC_PORC_IMPORTE_SIN_IVA( h ) ));
        _ADD_MEMORY_DOUBLE( __z_desc_pesos_importe_sin_iva, h, REDONDEA_X_PARA_Z(_X_DESC_PESOS_IMPORTE_SIN_IVA( h ) ));
        _ADD_MEMORY_DOUBLE( __z_desc_pesos_importe, h, REDONDEA_X_PARA_Z(_X_DESC_PESOS_IMPORTE( h ) ));
        _ADD_MEMORY_INT( __z_desc_pesos_cantidad, h, _X_DESC_PESOS_CANTIDAD( h ) );
    }
    ADD_MEMORY_DOUBLE( __z_desc_porc_ult_art_importe, REDONDEA_X_PARA_Z(X_DESC_PORC_ULT_ART_IMPORTE ));
    ADD_MEMORY_INT( __z_desc_porc_ult_art_cantidad, X_DESC_PORC_ULT_ART_CANTIDAD );
    ADD_MEMORY_DOUBLE( __z_desc_porc_utl_art_imp_sin_iva, REDONDEA_X_PARA_Z(X_DESC_PORC_ULT_ART_IMP_SIN_IVA ));
    ADD_MEMORY_DOUBLE( __z_desc_pesos_ult_art_imp_sin_iva, REDONDEA_X_PARA_Z(X_DESC_PESOS_ULT_ART_IMP_SIN_IVA ));
    ADD_MEMORY_DOUBLE( __z_desc_pesos_ult_art_importe, REDONDEA_X_PARA_Z(X_DESC_PESOS_ULT_ART_IMPORTE ));
    ADD_MEMORY_INT( __z_desc_pesos_ult_art_cantidad, X_DESC_PESOS_ULT_ART_CANTIDAD );

    for( h = 0 ;h < 11 ;h++ ) {
        _ADD_MEMORY_DOUBLE( __z_descuentos_medio, h, _X_DESCUENTOS_MEDIO( h ) );
        _ADD_MEMORY_DOUBLE( __z_devoluciones_medio, h, _X_DEVOLUCIONES_MEDIO( h ) );
    }
    ADD_MEMORY_DOUBLE( __z_factura_a_importe_exento, REDONDEA_X_PARA_Z(X_FACTURA_A_IMPORTE_EXENTO ));
    ADD_MEMORY_DOUBLE( __z_factura_a_importe_gravado, REDONDEA_X_PARA_Z(X_FACTURA_A_IMPORTE_GRAVADO ));
    ADD_MEMORY_DOUBLE( __z_nc_a_importe_exento, REDONDEA_X_PARA_Z(X_NC_A_IMPORTE_EXENTO ));
    ADD_MEMORY_DOUBLE( __z_nc_a_importe_gravado, REDONDEA_X_PARA_Z(X_NC_A_IMPORTE_GRAVADO ));
    ADD_MEMORY_DOUBLE( __z_cons_final_importe_exent_fact, REDONDEA_X_PARA_Z(X_CONS_FINAL_IMPORTE_EXENT_FACT ));
    ADD_MEMORY_DOUBLE( __z_cons_final_importe_grav_fact, REDONDEA_X_PARA_Z(X_CONS_FINAL_IMPORTE_GRAV_FACT ));
    ADD_MEMORY_INT( __z_cons_final_cantidad_fact, X_CONS_FINAL_CANTIDAD_FACT );
    ADD_MEMORY_DOUBLE( __z_dev_cons_f_importe_exent_fact, REDONDEA_X_PARA_Z(X_DEV_CONS_F_IMPORTE_EXENT_FACT ));
    ADD_MEMORY_DOUBLE( __z_dev_cons_f_importe_grav_fact, REDONDEA_X_PARA_Z(X_DEV_CONS_F_IMPORTE_GRAV_FACT ));
    ADD_MEMORY_INT( __z_dev_cons_f_cantidad_fact, X_DEV_CONS_F_CANTIDAD_FACT );
    ADD_MEMORY_DOUBLE( __z_ventas_diplom_importe, REDONDEA_X_PARA_Z(X_VENTAS_DIPLOM_IMPORTE ));
    ADD_MEMORY_INT( __z_ventas_diplom_cantidad, X_VENTAS_DIPLOM_CANTIDAD );
    ADD_MEMORY_DOUBLE( __z_ventas_export_importe, REDONDEA_X_PARA_Z(X_VENTAS_EXPORT_IMPORTE ));
    ADD_MEMORY_INT( __z_ventas_export_cantidad, X_VENTAS_EXPORT_CANTIDAD );
    ADD_MEMORY_DOUBLE( __z_devoluciones_diplom_importe, REDONDEA_X_PARA_Z(X_DEVOLUCIONES_DIPLOM_IMPORTE ));
    ADD_MEMORY_INT( __z_devoluciones_diplom_cantidad, X_DEVOLUCIONES_DIPLOM_CANTIDAD );
    ADD_MEMORY_DOUBLE( __z_devoluciones_export_importe, REDONDEA_X_PARA_Z(X_DEVOLUCIONES_EXPORT_IMPORTE ));
    ADD_MEMORY_INT( __z_devoluciones_export_cantidad, X_DEVOLUCIONES_EXPORT_CANTIDAD );
    //Para modo servicios, __z_cons_final_importe_exent_tkt y __z_cons_final_cantidad_tkt
    //se utilizan para cheques para reintegros y se cargan en la funcion
    //"RECUPERO_DE_CHEQUES_CARGADOS"
    if( MODO_NEGOCIO != SERVICIOS ) {
		ADD_MEMORY_DOUBLE( __z_cons_final_importe_exent_tkt, REDONDEA_X_PARA_Z(X_CONS_FINAL_IMPORTE_EXENT_TKT ));
    }
    ADD_MEMORY_DOUBLE( __z_cons_final_importe_grav_tkt, REDONDEA_X_PARA_Z(X_CONS_FINAL_IMPORTE_GRAV_TKT ));
    if( MODO_NEGOCIO != SERVICIOS ) {
		ADD_MEMORY_INT( __z_cons_final_cantidad_tkt, X_CONS_FINAL_CANTIDAD_TKT );
    }
    ADD_MEMORY_DOUBLE( __z_dev_cons_f_importe_exent_tkt, REDONDEA_X_PARA_Z(X_DEV_CONS_FINAL_IMPORTE_EXENT_TKT ));
    ADD_MEMORY_DOUBLE( __z_dev_cons_f_importe_grav_tkt, REDONDEA_X_PARA_Z(X_DEV_CONS_F_IMPORTE_GRAV_TKT ));
    ADD_MEMORY_INT( __z_dev_cons_f_cantidad_tkt, X_DEV_CONS_F_CANTIDAD_TKT );
    ADD_MEMORY_DOUBLE( __z_iva_factura_a, REDONDEA_X_PARA_Z(X_IVA_FACTURA_A ));
    ADD_MEMORY_DOUBLE( __z_iva_nc_a, REDONDEA_X_PARA_Z(X_IVA_NC_A ));
    ADD_MEMORY_DOUBLE( __z_iva_cons_final, REDONDEA_X_PARA_Z(X_IVA_CONS_FINAL ));
    ADD_MEMORY_DOUBLE( __z_iva_devol_consumidor_final, REDONDEA_X_PARA_Z(X_IVA_DEVOL_CONSUMIDOR_FINAL ));
    SET_MEMORY_LONG( __z_nro_transaccion_final, X_NRO_TRANSACCION_FINAL );
    ADD_MEMORY_DOUBLE( __z_oferta_importe, REDONDEA_X_PARA_Z(X_OFERTAS_IMPORTE ));
	ADD_MEMORY_DOUBLE( __z_dif_redondeo_vtas_contrib, X_DIF_REDONDEO_VTAS_CONTRIB);
	ADD_MEMORY_DOUBLE( __z_dif_redondeo_iva_contrib, X_DIF_REDONDEO_IVA_CONTRIB);
	ADD_MEMORY_DOUBLE( __z_dif_redondeo_nc, X_DIF_REDONDEO_NC);
	ADD_MEMORY_DOUBLE( __z_dif_redondeo_iva_nc, X_DIF_REDONDEO_IVA_NC);
	
}
/***********************************************************************/
double REDONDEA_X_PARA_Z(double importe_de_x ){
/***********************************************************************/
	int redondea = NO;//permite dejar sin efecto el redondeo sin sacarlo de todas las funciones, pued ser configurable
	double retorno = 0.0;

	if( redondea == SI ){
		retorno=ROUND( importe_de_x , ENTEROS, DECIMALES );
		/*
		memset( cadena_debug, 0, strlen( cadena_debug ) );
		sprintf( cadena_debug, "X_en_Z Import = %lf, R(Import,%i,%i)= %lf",
                 importe_de_x , ENTEROS, DECIMALES, retorno );
        GRABAR_LOG_SISTEMA( cadena_debug );
		*/
    }else
		retorno=importe_de_x ;
	

	
	return retorno;
}

/***********************************************************************/
int GRABAR_XYZ( int modo )
/***********************************************************************/
{
    char nom[30] = {"M:-."},
         marca;
    int ok, est = -1;
    struct _xyz cab_xyz;
    if( UTILIZA_DBROUTER ) {
        return 1;
    }
    switch( modo ) {
        case 'X':
            /*----------- X ------------*/
            NRO_ARCHIVO = ARCH_M_X;
            cab_xyz.nro = NRO_X;
            est = _x;
            ACTUALIZA_VENTAS_XYZ( ( char )modo );
            break;
        case 'Y':
            /*----------- Y ------------*/
            NRO_ARCHIVO = ARCH_M_Y;
            cab_xyz.nro = NRO_Y;
            est = _z;
            break;
        case 'Z':
            /*----------- Z ------------*/
            NRO_ARCHIVO = ARCH_M_Z;
            cab_xyz.nro = NRO_Z;
            est = _z;
            break;
    }
    nom[2] = modo;
    strcat( nom, NRO_NOM_CAJA );
    ok = _GRABAR_XYZ( nom, est, &cab_xyz );
    if( ok ) {
        marca = 0;
    }
    else {
        MENSAJE_STRING( S_ERROR_EN_LA_RED );
        marca = 1;
    }
    SET_MEMORY( __varios_marca_error_red, &marca );
    return ( ok );
}
/***********************************************************************/
int GRABAR_BACKUP_XYZ( int modo )
/***********************************************************************/
{
    char nom[30] = {"--.BAK"};
    int ok, est = -1;
    struct _xyz cab_xyz;
    char nom_aux[30];
    switch( modo ) {
        case 'X':
            /*----------- X ------------*/
            NRO_ARCHIVO = ARCH_X_BAK;
            cab_xyz.nro = NRO_X;
            est = _x;
            break;
        case 'Y':
            /*----------- Y ------------*/
            NRO_ARCHIVO = ARCH_Y_BAK;
            cab_xyz.nro = NRO_Y;
            est = _z;
            break;
        case 'Z':
            /*----------- Z ------------*/
            NRO_ARCHIVO = ARCH_Z_BAK;
            cab_xyz.nro = NRO_Z;
            est = _z;
            break;
    }
    nom[0] = modo;
    nom[1] = ( char )DOW( GET_FECHA_BACKUP() ) + '0';    
    _snprintf(nom_aux, sizeof(nom_aux)-1, "%s%s", PATH_BACKUPS, nom );
    ok = _GRABAR_XYZ( nom_aux, est, &cab_xyz );
    return ( ok );
}
/***********************************************************************/
int _GRABAR_XYZ( char *nom, int est, struct _xyz *cab_xyz )
/***********************************************************************/
{
    int handle, ok = 0, l_buffer;
    char *buffer,basura[64];
    handle = _OPEN_O_CREAT( nom, O_RDWR, __LINE__, __FILE__ );
    if( handle > 0 ) {
        l_buffer = SIZEOF_STRUCT( est );
        buffer = (char *)MALLOC( l_buffer );
        if( buffer ) {
            COPY_STRUCT_TO_BUFFER( buffer, est );
            if( est == _x && MULTIPLES_USUARIOS_PARA_VENTA ) {
                SUMAR_X_USUARIOS( buffer );
            }
            cab_xyz->fecha = GET_FECHA_DOS();
            if( est == _x ) {
                cab_xyz->cant_tickets = ( unsigned )( X_CANTIDAD_TICKETS );
            }
            else {
                cab_xyz->cant_tickets = ( unsigned )( Z_CANTIDAD_TICKETS );
            }
            cab_xyz->dia_operativo = NRO_DIA_OPERATIVO;
            cab_xyz->gran_total = NRO_GRAN_TOTAL;
            memset( cab_xyz->reserva, 0, sizeof( cab_xyz->reserva ) );
            memset( basura, 0, sizeof( basura ) );
            _WRITE( handle, ( char* )cab_xyz, sizeof( struct _xyz ), __LINE__, __FILE__ );
            _WRITE( handle, buffer, l_buffer, __LINE__, __FILE__ );
            _WRITE( handle, basura, sizeof( basura ), __LINE__, __FILE__ );
            if( UTILIZAR_DIA_OPERATIVO ) {
                _SET_FECHA_ARCHIVO( handle, NRO_DIA_OPERATIVO );
            }
            free( buffer );
        }
        _CLOSE( handle, __LINE__, __FILE__ );
        ok = 1;
    }
    return ( ok );
}
/*****************************************************************************/
void GRABAR_EVENTO_YZ( int modo )
/*****************************************************************************/
{
    int l_buffer, est = -1;
    char *buffer,*cuerpo;
    struct _xyz *cabecera;
    switch( modo ) {
        case 'Y':
            est = _x;
            break;
        case 'Z':
            est = _z;
            break;
    }
    l_buffer = SIZEOF_STRUCT( _x ) + sizeof( struct _xyz );
    buffer = MALLOC( l_buffer );
    if( buffer ) {
        cabecera = ( struct _xyz * )buffer;
        cuerpo = buffer + sizeof( struct _xyz );
        memset( buffer, 0, l_buffer );
        
        memset( &event_ticket, 0, sizeof( event_ticket ) );
        switch( modo ) {
            case 'Y':
                /*----------- Y ------------*/
                event_ticket.tipo_evento = _EVENT_Y;//9
				event_ticket.nro_comprobante_new = 0;
                event_ticket.nro_ticket = NRO_Y;
                cabecera->nro = NRO_Y;
                break;
            case 'Z':
                /*----------- Z ------------*/
                event_ticket.tipo_evento = _EVENT_Z;//10
				event_ticket.nro_comprobante_new = 0;
                event_ticket.nro_ticket = NRO_Z;
                cabecera->nro = NRO_Z;
                break;
        }
        COPY_STRUCT_TO_BUFFER( cuerpo, est );
        cabecera->fecha = GET_FECHA_DOS();
        cabecera->cant_tickets = ( unsigned )( NRO_TICKET );
        cabecera->dia_operativo = NRO_DIA_OPERATIVO;
        cabecera->gran_total = NRO_GRAN_TOTAL;
        GRABAR_EVENT_VARIOS( buffer, l_buffer, NO );
		if( modo == 'Z' ) {
			if( !GRABAR_CIERRE() )
				ACTUALIZAR_X();
		}
		GRABAR_EVENT_TICKET( NO );
		ACTUALIZA_ID_EVENTO_NEW();
        free( buffer );
    }
}
/***********************************************************************/
void SUMAR_X_USUARIOS( char *destino )
/***********************************************************************/
{
    char *origen,nombre[8];
    int i,handle,l_buffer;
    l_buffer = SIZEOF_STRUCT( _x );
    origen = MALLOC( l_buffer );
    if( origen ) {
        for( i = 0;i < DD_ELEMENTOS( __varios_usuarios_habilitados );i++ ) {
            if( i + 1 != VARIOS_CAJON_ACTIVO && _VARIOS_USUARIOS_HABILITADOS( i ) ) {
                _snprintf(nombre, sizeof(nombre)-1, "X.%.3i", i + 1 );
                #ifdef INVEL_L
                handle = _OPEN( nombre, O_RDWR, __LINE__, __FILE__ );
                #else
                handle = _OPEN( nombre, O_RDWR | O_BINARY, __LINE__, __FILE__ );
                #endif
                if( handle > 0 ) {
                    if( _READ( handle, origen, l_buffer, __LINE__, __FILE__ ) == 0 ) {
                        ADD_BUFFER_TO_BUFFER( origen, destino, _x );
                    }
                    _CLOSE( handle, __LINE__, __FILE__ );
                }
            }
        }
        FREEMEM( origen );
    }
}

