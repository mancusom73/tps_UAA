#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
//#include <browse.h> 
#include <btrv.h> 
#include <cajon.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <cr_displ.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <facrem.h> 
#include <ftoc.h> 
#include <get_fech.h> 
#include <log.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <m_caract.h> 
#include <pagos.h> 
#include <pant.h> 
#include <pg_fecha.h> 
#include <_cr1.h> 
#include <path.h>
#include <deftables.h>
#include <db.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
#ifdef COMPILAR_REMITOS
static int fecha_desde,fecha_hasta,marcados;
struct _eventos eventos;
struct _ev_cont ev_cont;

/****************************************************************************/
int PEDIR_FECHAS_LIMITE()
/****************************************************************************/
{
    int rta = 0;
    if( PEDIR_FECHAS_AL_FACTURAR_REMITOS ) {
        //LIMPIAR_RECUADRO_PETICION( ST( S_FECHA_INICIAL ) );
        LIMPIAR_RECUADRO_PETICION( S_FECHA_INICIAL );
        fecha_desde = PEDIR_GRAN_FECHA( 30 - ancho_panel / 2, 19, NULL );
        if( fecha_desde != -1 ) {
            //LIMPIAR_RECUADRO_PETICION( ST( S_FECHA_FINAL ) );
            LIMPIAR_RECUADRO_PETICION( S_FECHA_FINAL );
            fecha_hasta = PEDIR_GRAN_FECHA( 30 - ancho_panel / 2, 19, NULL );
            if( fecha_hasta != -1 ) {
                rta = 1;
            }
        }
    }
    else {
        fecha_desde = 0;
        fecha_hasta = 0x7fff;
        rta = 1;
    }
    return ( rta );
}

/*****************************************************************************/
void GO_REMITOS( int modo, int inicio )
/*****************************************************************************/
{
    char where[100];
    int fecha, error = 0;
        if( inicio ) {
            fecha = fecha_desde;
            _snprintf(where, sizeof(where)-1, " tipo_evento = %d AND cod_cliente = %ld AND fecha_ticket > %d",
                     11, RAM_NRO_CLIENTE, fecha );
            GRABAR_LOG_SISTEMA( where ,LOG_DEBUG,3);
            MENSAJE_SIN_SONIDO_STRING( S_LEYENDO_DATOS );            
			if( OBTENER_TABLA_SERVIDOR( T_EV_TMP, TT_ORIG, "eventos", where, SI, NULL ) ) {
                error = 1;
            }
            if( !error
            //(Nota Unific MySQL-Linux): aun no se usan las tt_temp para los eventos importados
			//&& ( OPEN_TABLE( T_EVENTOS, TT_TEMP, ( char* )&eventos,
			  && ( OPEN_TABLE( T_EV_TMP, TT_ORIG, ( char* )&eventos,
			 sizeof( struct _eventos ) ) == 0 ) ) {
            }
        }
        else {
            //SELECT( AREA_AUX3 );
			SELECT_TABLE( T_EV_TMP, TT_ORIG );
            if( modo == TOP ) {
                ind2_eventos_all.fecha = (unsigned short)fecha_desde;
                //GET_GREATER_E( ( char* )&ind2_event_ticket_all );
				SET_WHERE("TIPO_EVENTO='%d' AND NRO_CLIENTE='%ld' AND FECHA >= '%d'", ind2_eventos_all.tipo_evento, 
						ind2_eventos_all.nro_cliente, ind2_eventos_all.fecha );
				RUN_QUERY(NO);
            }
            else if( modo == BOTTOM ) {
                ind2_eventos_all.fecha = (unsigned short)fecha_hasta + 1;
                //GET_GREATER_E( ( char* )&ind2_event_ticket_all );
				SET_WHERE("TIPO_EVENTO='%d' AND NRO_CLIENTE='%ld' AND FECHA >= '%d'", ind2_eventos_all.tipo_evento, 
						ind2_eventos_all.nro_cliente, ind2_eventos_all.fecha );

				RUN_QUERY(NO);
                if( /*!BtrvEOF()*/ !dbEOF() ) {
                    SKIP2( -1 );
                }
            }
        }
}
/*****************************************************************************/
int REMITO_EN_FILTRO()
/*****************************************************************************/
{
    int rta = 1;
        if( eventos.tipo_evento != ind2_eventos_all.tipo_evento
         || eventos.nro_cliente != ind2_eventos_all.nro_cliente || eventos.fecha < fecha_desde
         || eventos.fecha > fecha_hasta ) {
            rta = 0;
        }
    return ( rta );
}
/*****************************************************************************/
void DESMARCAR_REMITOS()
/*****************************************************************************/
{
    GO_REMITOS( TOP, NO );
    while( /*!BtrvEOF()*/ !dbEOF() && REMITO_EN_FILTRO() ) {
        if( DELETED() ) {
            RECALL();
        }
        SKIP2( 1 );
    }
}

/*****************************************************************************/
void ACTUALIZA_ACU_DEPTOS()
/*****************************************************************************/
{
    char _where[100];
    double importe;
    int area_ant, tipo_ant, error = 0;
        _snprintf(_where, sizeof(_where)-1, " id_evento = %li", eventos.id_evento );
        GRABAR_LOG_SISTEMA( _where ,LOG_DEBUG,4);
		if( OBTENER_TABLA_SERVIDOR( T_EVC_TMP, TT_ORIG, "ev_cont", _where, SI, NULL ) ) {
            error = 1;
        }
        area_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();
        OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )&ev_cont, sizeof( struct _ev_cont ) );
        GO_REMITOS( TOP, NO );
        /*---------- Recorre remitos --------------*/
        while( !dbEOF() && REMITO_EN_FILTRO() ) {
            //if( DELETED() && !TESTEAR( 1 ) ) {
                SELECT_TABLE( T_EVC_TMP, TT_ORIG );
				SET_WHERE( "ID_EVENTO = '%ld'", eventos.id_evento );
				RUN_QUERY( NO );
                while( !dbEOF() && eventos.id_evento == ev_cont.id_evento ) {
                    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                        BUSCA_COD_INTERNO_COD_BARRA( ev_cont.cod_articulo, ev_cont.cod_barra );
                    }
                    else {
                        BUSCA_CODIGO_ESPECIAL( ev_cont.cod_articulo );
                    }
                    /*--------------- Actualiza departamento --------------*/
                    importe = ev_cont.importe_sin_iva + ev_cont.imp_int + ev_cont.recargo_tarjeta
                            + ev_cont.exento + ev_cont.iva1 + ev_cont.iva2
                            + ev_cont.monto_impuesto[0] + ev_cont.monto_impuesto[1]
                            + ev_cont.monto_impuesto[2] + ev_cont.monto_impuesto[3];
                    _ACUMULA_DEPARTAMENTO( articulo.cod_dpto, importe, ev_cont.cantidad );
                    /*-----------------------------------------------------*/
                    SELECT_TABLE( T_EVC_TMP, TT_ORIG );
                    SKIP2( 1 );
                }
            //}
            SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
			//SELECT( AREA_AUX3 );
            SKIP2( 1 );
        }
        SELECT_TABLE( T_EVC_TMP, TT_ORIG );
}
/*****************************************************************************/
void PASA_TOTALES( struct _tot tot )
/*****************************************************************************/
{
    int i;
    ADD_MEMORY_DOUBLE( __tot_tick_importe_sin_iva, tot.importe_sin_iva );
    ADD_MEMORY_DOUBLE( __tot_tick_imp_int, tot.imp_int );
    ADD_MEMORY_DOUBLE( __tot_tick_costo, tot.costo );
    ADD_MEMORY_DOUBLE( __tot_tick_recargo_tarjeta, tot.recargo_tarjeta );
    ADD_MEMORY_DOUBLE( __tot_tick_exento, tot.exento );
    ADD_MEMORY_DOUBLE( __tot_tick_iva1, tot.iva1 );
    ADD_MEMORY_DOUBLE( __tot_tick_iva2, tot.iva2 );
    //ADD_MEMORY_DOUBLE( __tot_tick_ila, tot.ila );
    //ADD_MEMORY_DOUBLE( __tot_tick_iha, tot.iha );
    //ADD_MEMORY_DOUBLE( __tot_tick_alicuota, tot.alicuota );
    for( i = 0;i < 4;i++ ) {
        _ADD_MEMORY_DOUBLE( __tot_tick_impuestos, i, tot.monto_impuesto[i] );
    }
}
#endif

