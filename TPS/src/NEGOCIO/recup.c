#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cio.h>
#include <clientes.h>
#include <cmemory.h>
#include <cr.h>
#include <dd.h>
#include <descuent.h>
#include <eventos.h>
#include <flush.h>
#include <malloc.h>
#include <mstring.h>
#include <recup.h>
#include <tkt.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#include <sys/io.h>
#else
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#endif
static int l_buf_ticket,handle_ticket;
extern char importando_ticket_devolucion;
extern long cod_especial;
extern    char drivevirtual;
extern int cobro_on_tick;
extern int cobro_marginal;
extern int BLOQUEAR_DATOS;
extern void CEREAR_STATIC_REINTENTO_QR();
/*****************************************************************************/
void CRR_INICIALIZAR_RAM_NOVOL()
/*****************************************************************************/
{
    int tmpo_no_venta, tmpo_ocio, tmpo_ocio_ant;

    if( !INIT_VARIABLES_ESPEJADAS() ) {
        MENSAJE_STRING( S_ERROR_AL_INICIALIZAR_RECUPERO_DE_TICKET );
    }
    tmpo_no_venta = RAM_TMPO_NO_VENTA;
    tmpo_ocio = RAM_TMPO_OCIO;
    tmpo_ocio_ant = RAM_TMPO_OCIO_AUX;

    if( strcmp( RAM_SIGN, "RAMPAC" ) ) {
        CEREAR_STRUCT( _ram );
		SET_MEMORY( __ram_sign, "RAMPAC" );
		CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL();
		SET_MEMORY_INT( ____no_se_usa_002, SI );
		ACTUALIZA_ID_EVENTO_NEW();
    }
    SET_MEMORY_INT( __ram_tmpo_no_venta, tmpo_no_venta );
    SET_MEMORY_INT( __ram_tmpo_ocio, tmpo_ocio );
    SET_MEMORY_INT( __ram_tmpo_ocio_aux, tmpo_ocio_ant );

	RECUPERAR_TICKET();
	CRR_RECUPERA_POSICION_ARCHIVOS();
}
/*****************************************************************************/
void CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL()
/*****************************************************************************/
{
    int i;
	SET_MEMORY_LONG( __ram_nro_comp, 0 );
	SET_MEMORY_INT( __ram_instancia_evento, 0 );
	SET_MEMORY_INT( __ram_val_status_st, 0 );
	SET_MEMORY_INT( __ram_val_status_status, 0 );
	SET_MEMORY_INT( __ram_impresion_comprobante, 0 );
    SET_MEMORY_INT( __ram_p_ticket, 0 );
	SET_MEMORY_CHAR( __ram_p_pago, 0 );
    SET_MEMORY_INT( __ram_fin_ticket, 0 );
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    SET_MEMORY_DOUBLE( __ram_total, 0 );
    SET_MEMORY_CHAR( __ram_empieza_ticket, 1 );
    SET_MEMORY_INT( __ram_item_supervisado, 0 );
    SET_MEMORY_INT( __ram_anulacion, 0 );
    SET_MEMORY_CHAR( __ram_correccion, 0 );
    SET_MEMORY_INT( __ram_present_ant, -1 );
    SET_MEMORY_INT( __ram_present_ant_bak, -1 );
    SET_MEMORY_INT( __ram_bultos, 0 );
    SET_MEMORY_INT( __ram_cant_bultos, 1 );
    SET_MEMORY_LONG( __ram_nro_cliente, 0L );
	SET_MEMORY_LONG ( __ram_nro_cliente_ant, 0 );
	SET_MEMORY_CHAR( __ram_cond_iva_cliente, 0 );
    SET_MEMORY_CHAR( __ram_variar_precio, 0 );
    SET_MEMORY_CHAR( __ram_en_importacion_de_ticket, 0 );
    SET_MEMORY_CHAR( __varios_restringir_volver_de_pagos, NO );
    SET_MEMORY_LONG( __ram_cliente_afinidad, 0 );
    CEREAR_MEMORY( __ram_string_cliente_afinidad );
	CEREAR_MEMORY( __ram_cadena_cheque );
    SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );
    SET_MEMORY_INT( __nro_vendedor, 0 );
    SET_MEMORY_LONG( __nro_vendedor_long, 0L );
    SET_MEMORY_INT( __nro_cod_cuota, 0 );
    SET_MEMORY_LONG( __nro_remito, 0L );
    SET_MEMORY_FLOAT( __nro_recargo_financiacion, 0.0 );
    SET_MEMORY_INT( __nro_mozo, 0 );
    SET_MEMORY_DOUBLE( __ram_cupon_envases, 0 );
    SET_MEMORY_CHAR( __ram_usar_precio_cr, ( CREDITO_DEFAULT ) ? 1 : 0 );
    SET_MEMORY_DOUBLE( __ram_total_vta_contado, 0 );
    SET_MEMORY_CHAR( __ram_factura, 0 );
    SET_MEMORY_CHAR( __ram_nota_cr, 0 );
	SET_MEMORY_INT( __ram_copia, 0 );
	SET_MEMORY_INT( __ram_pos_rev_promo, 0 );
	SET_MEMORY_INT( __ram_cobro_servicios, 0);
	SET_MEMORY_INT( __ram_cod_promo, 0 );
    //Se reemplaza porque la RAM_POS_PAGO no funciona bien.
	//SET_MEMORY_INT( __ram_pos_pago, 0 );
    SET_MEMORY_INT( __ram_ptr, 0 );
	SET_MEMORY_INT( __ram_agrega_item_tick_import, 0 );
    SET_MEMORY_LONG( __ram_nro_pedido_pin, 0L );
	SET_MEMORY_CHAR( __ram_importacion_activa, 0 );
	SET_MEMORY_LONG( __nro_pagare, 0 );
    if( MODO_FACTURACION_POR_DEFECTO ) {
        SET_MEMORY_CHAR( __ram_factura, 1 );
        SET_MEMORY_LONG( __ram_nro_cliente, NRO_CLIENTE_POR_DEFECTO );
    }
    if( NC_PERMANENTE ) {
        if( config.pais == CHILE || config.pais == BRASIL ) {
            SET_MEMORY_CHAR( __ram_nota_cr, 1 );
            SET_MEMORY_CHAR( __ram_factura, 1 );
        }
        SET_MEMORY_LONG( __ram_nro_cliente, NRO_CLIENTE_POR_DEFECTO );
    }
    SET_MEMORY_INT( __ram_renglones, 0 );
    SET_MEMORY_DOUBLE( __ram_cantidad_total, 0 );
    SET_MEMORY_DOUBLE( __ram_intereses, 0 );
    SET_MEMORY_INT( __ram_p_ext_impuestos, 0 );
    CEREAR_STRUCT( _ext_impuestos );
    SET_MEMORY_CHAR( __ram_cambio, 0 );
    SET_MEMORY_CHAR( __ram_pedir_precio, 0 );
    SET_MEMORY_CHAR( __ram_negativo, 0 );
    SET_MEMORY_LONG( __ram_boleta_anterior, 0L );
    SET_MEMORY_CHAR( __ram_descuento_porcentual, 0 );
    SET_MEMORY_CHAR( __ram_descuento_importe, 0 );
    SET_MEMORY_CHAR( __ram_utiliza_precio_afinidad, 0 );
    SET_MEMORY_DOUBLE( __ram_diferencia_afinidad, 0.0 );
    SET_MEMORY_INT( __ram_cant_articulos_afinidad, 0 );
    SET_MEMORY_DOUBLE( __ram_puntos_usados, 0 );
    for( i = 0;i < 5;i++ ) {
        _SET_MEMORY_CHAR( __ram_desc_porc_pendientes, i, 0 );
    }
    SET_MEMORY_CHAR( __ram_bien_de_uso, 0 );
    SET_MEMORY_INT( __ram_perfil, 0 );
    SET_MEMORY_DOUBLE( __ram_retencion_dgr, 0 );
    SET_MEMORY_DOUBLE( __ram_percepcion_dgi, 0 );
    SET_MEMORY_CHAR( __ram_anular_ticket, 0 );
    SET_MEMORY_INT( __ram_sucursal_anterior, 0 );
    SET_MEMORY_INT( __ram_caja_anterior, 0 );
    SET_MEMORY_CHAR( __ram_tipo_comp_anterior, 0 );
    SET_MEMORY_INT( __ram_fecha_anterior, 0 );
    SET_MEMORY_CHAR( __ram_novios, 0 );
    CEREAR_ARRAY( __ram_medios_obligatorios );
    SET_MEMORY_LONG( __ram_transac_en_cobro, 0 );
    SET_MEMORY_CHAR( __ram_imprimir_voucher_cmr, 0 );
    SET_MEMORY_INT( __ram_subindice_pago_cmr, 0 );
    SET_MEMORY_DOUBLE( __ram_acu_puntos, 0.0 );
    SET_MEMORY_DOUBLE( __ram_vuelto_efectivo, 0.0 );
    SET_MEMORY_CHAR( __ram_precio_mas_bajo, 0 );
    SET_MEMORY_CHAR( __ram_compromiso_200, 0 );
    SET_MEMORY_CHAR( __ram_diferencia_precio, 0 );
    SET_MEMORY_CHAR( __ram_enviar_offline, 0 );
    SET_MEMORY_CHAR( __ram_mostrar_saldo_cheque, 0 );
    CEREAR_MEMORY( __ram_cod_barra );
	CEREAR_MEMORY( __ram_cuenta_cliente );
	SET_MEMORY_INT( __ram_ptr, 0 );
    SET_MEMORY_DOUBLE( __ram_diferencia_redondeo, 0.0 );
    SET_MEMORY_INT( __ram_tmpo_ocio, 0 );
    SET_MEMORY_INT( __ram_tmpo_reg_articulo, 0 );
    SET_MEMORY_INT( __ram_tmpo_standby, 0 );
    SET_MEMORY_INT( __ram_tmpo_pago, 0 );
    SET_MEMORY_INT( __ram_tmpo_no_venta, 0 );
    SET_MEMORY_FLOAT( __ram_porc_interes, 0 );
	SET_MEMORY_INT( __ram_calcular_retenc_nc , NO );
    for( i = 0;i < 8;i++ ) {
        ELIMINA_BASE_DTO( i );
    }
    ELIMINA_BASE_DTO( 99 );       //Eliminamos el archivo de promociones
    ELIMINA_BASE_DTO( 98 );       //Eliminamos el archivo de importaciones
    if( RAM_MODO_DEVOLUCION ) {
        SET_MEMORY_CHAR( __modo_devolucion, 0 );
        SET_MEMORY_INT( __ram_modo_devolucion, 0 );
		SET_MEMORY_INT( __ram_devolucion_parcial_ref, 0 );
    }
    SET_MEMORY_DOUBLE( __ram_total_donacion, 0.00 );
    SET_MEMORY_CHAR( __ram_control_cambio, 0 );
    SET_MEMORY_INT( __ram_motivo_devolucion, 0 );
    CEREAR_MEMORY( __ram_string_legajo_empleado );
    CEREAR_STRUCT( _mutual );
	BLOQUEAR_DATOS=0;
    if( MODO_DEVOLUCION || NC_PERMANENTE ) {
        switch( ITEM_COD_MOVIMIENTO ) {
            default:
                SET_MEMORY_CHAR( __item_cod_movimiento, _DEVOLUCION );
                break;
        }
    }
    else {
        switch( ITEM_COD_MOVIMIENTO ) {
            case _COMPROMISO_200:
            case _COMPROMISO_PRECIO_MAS_BAJO:
            case _DIFERENCIA_PRECIO:
            case _DEVOLUCION:
                SET_MEMORY_CHAR( __item_cod_movimiento, _VENTA_NORMAL );
                break;
        }
    }
    SET_MEMORY_CHAR( __ram_referencia_nc, 0 );
    for( i = 0;i < 5;i++ ) {
        _SET_MEMORY_DOUBLE( __ram_acumulador, i, 0.0 );
    }
    CEREAR_MEMORY( __ram_cod_boleta_plata );
    for( i = 0;i < 4;i++ ) {
        _SET_MEMORY_INT( __ram_cant_envases, i, 0 );
    }
    for( i = 0;i < 5;i++ ) {
        _SET_MEMORY_LONG( __ram_numero_ticket_tomra, i, 0 );
    }
    SET_MEMORY_CHAR( __ram_ticket_factura, NO );
    SET_MEMORY_LONG( __nro_evento_anterior, 0 );
    SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, ( char ) - 1 );
    SET_MEMORY_INT( __ram_cant_comp, 0 );
    SET_MEMORY_DOUBLE( __ram_ahorro_obtenido, 0.0 );
    SET_MEMORY_INT( __ram_motivo_descuento, 0 );
	for( i = 0;i < 5;i++ ) {
        _SET_MEMORY_CHAR( __ram_motivo_descuento_pendiente, i, 0 );
    }
    //Cerear variables Cobros Marginales
    cobro_on_tick = 0;
    cobro_marginal = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteAction( A_INIT_VENTAS );
    #endif
    cod_especial = 0;
    buscar_articulo_en_mostrar_item = 0;
	//Se desactiva variable que se usa en caso de que se corte la caja y no la printer
	NO_INC_NRO = 0;
    SET_MEMORY_DOUBLE( __ram_resto, 0.0 );
	SET_MEMORY_DOUBLE( __ram_vuelto, 0.0 );
	SET_MEMORY_INT( __ram_bonos_a_entregar, 0 );
	_SET_MEMORY_CHAR( __ram_promociones_afinidad_usadas,0,0  );
	SET_MEMORY_CHAR( __ram_venta_fraccion, 0 );
    CEREAR_MEMORY( __ram_buffer);
    CEREAR_MEMORY( __ram_oc_gd );
	CEREAR_STRUCT( _item );
	importando_ticket_devolucion=0;
	CEREAR_STATIC_REINTENTO_QR();

}
/*****************************************************************************/
void CRR_RECUPERA_POSICION_ARCHIVOS()
/*****************************************************************************/
{
    if( RAM_NRO_CLIENTE ) {
        EXISTE_CLIENTE( RAM_NRO_CLIENTE );
    }
}
/*****************************************************************************/
int INIT_VARIABLES_ESPEJADAS()
/*****************************************************************************/
{
    int ok = 1;
    char nombre[20];
    l_buf_ticket = _SIZEOF_STRUCT( _ticket );
    NRO_ARCHIVO = 0;
    #ifdef INVEL_L
    handle_ticket = _OPEN( "ticket.ram", O_RDWR, __LINE__, __FILE__ );
    #else
    _snprintf(nombre, sizeof(nombre)-1, "%c:ticket.ram", drivevirtual );
    handle_ticket = _OPEN( nombre, O_RDWR | O_BINARY, __LINE__, __FILE__ );
    #endif
    if( handle_ticket < 0 ) {
        ok = 0;
    }
    return ( ok );
}
/*****************************************************************************/
void GRABAR_TICKET( int h )
/*****************************************************************************/
{
    long pos;
    char *buffer;
    buffer = (char *)MALLOC( l_buf_ticket );
    if( buffer ) {
        _COPY_STRUCT_TO_BUFFER( buffer, _ticket, h );
        pos = ( long )l_buf_ticket * h;
        NRO_ARCHIVO = 0;
        LSEEK( handle_ticket, pos, SEEK_SET, __LINE__, __FILE__ );
        _WRITE( handle_ticket, buffer, l_buf_ticket, __LINE__, __FILE__ );
        if( buffer )
        free( buffer );
        /*----------------- Graba impuestos ------------------*/
        if( _TICKET_COD_EXT( h ) ) {
            pos = l_buf_ticket * _LARGO_TICKET;
            NRO_ARCHIVO = 0;
            LSEEK( handle_ticket, pos, SEEK_SET, __LINE__, __FILE__ );
            _WRITE( handle_ticket, DD_PTR( __ext_impuestos ), SIZEOF_ARRAY( __ext_impuestos ),
                    __LINE__, __FILE__ );
        }
        FLUSH();
    }
}
/*****************************************************************************/
void RECUPERAR_TICKET()
/*****************************************************************************/
{
    char *buffer;
    int h,size;
    long pos;
    buffer = MALLOC( l_buf_ticket );
    if( buffer ) {
        NRO_ARCHIVO = 0;
        LSEEK( handle_ticket, 0L, SEEK_SET, __LINE__, __FILE__ );
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            _READ( handle_ticket, buffer, l_buf_ticket, __LINE__, __FILE__ );
            _COPY_BUFFER_TO_STRUCT( buffer, _ticket, h );
        }
        if( buffer )
        free( buffer );
        /*----------------- Recupera impuestos ------------------*/
        size = DD_LARGO( __ext_impuestos ) * RAM_P_EXT_IMPUESTOS * 20;
        if( size ) {
            pos = l_buf_ticket * _LARGO_TICKET;
            NRO_ARCHIVO = 0;
            LSEEK( handle_ticket, pos, SEEK_SET, __LINE__, __FILE__ );
            _READ( handle_ticket, DD_PTR( __ext_impuestos ), size, __LINE__, __FILE__ );
        }
    }
}

