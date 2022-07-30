#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <math.h>
#include <actual.h> 
#include <btrv.h> 
#include <bridge.h>
#include <clientes.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h>
#include <cio.h>
#include <dbrouter.h> 
#include <descuent.h> 
#include <eventos.h> 
#include <ext_tick.h> 
#include <get_fech.h> 
#include <import_t.h> 
#include <log.h> 
#include <malloc.h> 
#include <pagos.h> 
#include <pago_tc.h> 
#include <perfiles.h> 
#include <recupero.h> 
#include <remito.h> 
#include <round.h> 
#include <stack.h> 
#include <t_pago.h> 
#include <_cr1.h> 
#include <path.h>
#include <deftables.h>
#include <db.h>
#include <cr_pant.h>
#include <tarjetas.h>
#include <imp_tick.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Actualizacion de acumuladores en memoria y disco
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <btrv.h>
#include <config.h>
#include "actions.h"
#include <recupero.h>
#include <path.h>
#include <motor.h>
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_ev_dpago, tipo_t_ev_despro;
extern int tipo_t_ev_b_promo;
void _ACUMULA_DEPARTAMENTO( int depto, double importe, double cantidad );
void CARGA_IMPORTADOS( void );
extern double MUTUALES_ML_A_ME( double importe_me, int h );
extern int MUTUAL_MONEDA_EXTRANJERA( int h );
void GRABAR_EVENTOS_PROMOCIONES(void);
extern int BUSCA_CODIGO_ESPECIAL( long cod );
double  DEVOLVER_PARCIALMENTE_DE_B_PROMO(unsigned long cod_articulo,double cantidad_articulo,char *cod_barra,
		double precio_articulo,struct _ev_bpromo * evento_b_promo, 
		int tabla, int tipo, double total_descontado );

void EQUILIBRA_IMPORTES_TOTALES(double * total, double * importe_sin_iva, double * iva1, double * exento ,float iva);
int  GET_STATIC_REINTENTO_QR();
/*****************************************************************************/
void ACTUALIZA_DEMORA()
/*****************************************************************************/
{
    time_t hora_actual;
    struct _demora
    {
        unsigned segundos;
        unsigned hora;
    } demora;
    time( &hora_actual );
    demora.segundos = ( unsigned )difftime( hora_actual, hora_inicio_ticket ) + 1;
    demora.hora = ( unsigned )difftime( hora_inicio_ticket, hora_ingreso_cajero ) / 3600 + 1;
    /*--------- actualiza acumuladores -----------*/
    if( demora.hora < 24 ) {
        _ADD_MEMORY_UINT( __x_demora_segundos, demora.hora, demora.segundos );
        _ADD_MEMORY_UINT( __x_demora_ventas, demora.hora, 1 );
    }
}
/*****************************************************************************/
void ACTUALIZA_ACUMULADORES_IVA( double signo )
/*****************************************************************************/
{
    int i;
    ADD_MEMORY_DOUBLE( __x_ventas_gravadas,
                       ( TOT_TICK_IMPORTE_SIN_IVA - TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA )
                     * signo );
    ADD_MEMORY_DOUBLE( __x_iva1, TOT_TICK_IVA1 * signo );
    ADD_MEMORY_DOUBLE( __x_iva2, TOT_TICK_IVA2 * signo );
    /*  ADD_MEMORY_DOUBLE( __x_ila, TOT_TICK_ILA * signo );
        ADD_MEMORY_DOUBLE( __x_iha, TOT_TICK_IHA * signo );
       ADD_MEMORY_DOUBLE( __x_alicuota, TOT_TICK_ALICUOTA * signo );*/
    for( i = 0;i < 4;i++ ) {
        _ADD_MEMORY_DOUBLE( __x_impuestos, i, _TOT_TICK_IMPUESTOS( i ) * signo );
    }
    ADD_MEMORY_DOUBLE( __x_ventas_exentas, TOT_TICK_EXENTO * signo );
    ADD_MEMORY_DOUBLE( __x_imp_int, TOT_TICK_IMP_INT * signo );
    ADD_MEMORY_DOUBLE( __x_diferencia, TOT_TICK_DIFERENCIA );
}
/*****************************************************************************/
void ACTUALIZA_ACUMULADORES_RAM( int h )
/*****************************************************************************/
{
    double signo = 1, recargo_neto, imp_iva1, imp_iva2, precio = 0;
    double importe_sin_iva,imp_int,costo,recargo_tarjeta,exento = 0.00,iva1,iva2;
    int cod_iva,i;
    double impuesto[4], imp[4], diferencia_temporal = 0.0;

    /*------------------------ Calcula valores ----------------------------*/
    signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
	recargo_neto = _TICKET_RECARGO( h ) - _TICKET_IVA1_REC( h ) - _TICKET_IVA2_REC( h );
    imp_iva1 = _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h );
    imp_iva2 = _TICKET_IVA2_PRECIO( h ) + _TICKET_IVA2_REC( h );
    /*  imp_ila = _TICKET_ILA_PRECIO( h ) + _TICKET_ILA_REC( h );
        imp_iha = _TICKET_IHA_PRECIO( h ) + _TICKET_IHA_REC( h );
        imp_alicuota = _TICKET_ALICUOTA_PRECIO( h );*/
    impuesto[0] = _TICKET_IMPUESTO1_PRECIO( h );
    impuesto[1] = _TICKET_IMPUESTO2_PRECIO( h );
    impuesto[2] = _TICKET_IMPUESTO3_PRECIO( h );
    impuesto[3] = _TICKET_IMPUESTO4_PRECIO( h );
    /*---------------- Calculos para encontrar las diferencias ---------------*/
    precio =
      ROUND( _TICKET_CANTIDAD( h ) * signo * ( _TICKET_PRECIO( h ) +
          _TICKET_RECARGO( h ) ), 10, DECIMALES );
    importe_sin_iva = _TICKET_IMPORTE_SIN_IVA( h ) * signo * _TICKET_CANTIDAD( h );

    imp_int = _TICKET_IMP_INT( h ) * signo * _TICKET_CANTIDAD( h );
    costo = _TICKET_COSTO( h ) * signo * _TICKET_CANTIDAD( h );
    recargo_tarjeta = recargo_neto * signo * _TICKET_CANTIDAD( h );
    exento = _TICKET_EXENTO( h ) * signo * _TICKET_CANTIDAD( h );
    iva1 = imp_iva1 * signo * _TICKET_CANTIDAD( h );
    iva2 = imp_iva2 * signo * _TICKET_CANTIDAD( h );
    //  ila = imp_ila * signo * _TICKET_CANTIDAD( h );
    //  iha = imp_iha * signo * _TICKET_CANTIDAD( h );
    // alicuota = imp_alicuota * signo * _TICKET_CANTIDAD( h );
    for( i = 0;i < 4;i++ ) {
        imp[i] = impuesto[i] * signo * _TICKET_CANTIDAD( h );
        _ADD_MEMORY_DOUBLE( __tot_tick_impuestos, i, imp[i] );
    }

		
	//
    /*----------------------- Actualiza acumuladores ----------------------*/
    ADD_MEMORY_DOUBLE( __tot_tick_importe_sin_iva, importe_sin_iva );
    ADD_MEMORY_DOUBLE( __tot_tick_imp_int, imp_int );
    ADD_MEMORY_DOUBLE( __tot_tick_costo, costo );
    ADD_MEMORY_DOUBLE( __tot_tick_recargo_tarjeta, recargo_tarjeta );
    if( IMPRESORA_FISCAL == FISCAL && RAM_NOTA_CR == 2  && _TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION )
		ROUND2( &exento, ENTEROS, 2);
	
    ADD_MEMORY_DOUBLE( __tot_tick_exento, exento );

    ADD_MEMORY_DOUBLE( __tot_tick_iva1, iva1 );
    ADD_MEMORY_DOUBLE( __tot_tick_iva2, iva2 );
    /*  ADD_MEMORY_DOUBLE( __tot_tick_ila, ila );
        ADD_MEMORY_DOUBLE( __tot_tick_iha, iha );
       ADD_MEMORY_DOUBLE( __tot_tick_alicuota, alicuota );*/
    //fabiana 15-02-05 ** nuevo manejo de redondeos--se usara esta variable
    //ADD_MEMORY_DOUBLE( __tot_tick_diferencia,
    //   precio - importe_sin_iva - recargo_tarjeta - exento - iva1 - iva2 - ila - iha );

    if( ( ES_FACTURA_A( clientes.cond_iva ) || MOSTRAR_PRECIO_SIN_IVA_EN_VENTAS 
         || MOSTRAR_SIN_IVA_ELSALVADOR( ) ) ) {
         diferencia_temporal = ( ROUND( _TICKET_IMPORTE_SIN_IVA( h ) * _TICKET_CANTIDAD( h ), ENTEROS, DECIMALES ) - ( _TICKET_IMPORTE_SIN_IVA( h ) * _TICKET_CANTIDAD( h ) ) );
      } else { //POR ACA ENTRA SI NO ES FACTURA A. En caso de ser diplomatico o exportacion el iva esta en cero.
         diferencia_temporal = ( ROUND( ( _TICKET_IMPORTE_SIN_IVA( h ) + _TICKET_IVA1_PRECIO( h ) ) * _TICKET_CANTIDAD( h ), ENTEROS, DECIMALES ) -
                                      ( ( _TICKET_IMPORTE_SIN_IVA( h ) + _TICKET_IVA1_PRECIO( h ) ) * _TICKET_CANTIDAD( h ) ) );
    }
    ADD_MEMORY_DOUBLE( __tot_tick_diferencia, diferencia_temporal * ( -1 ) );      
    if( _TICKET_TIPO2( h ) & _HABILITADO_CUOTAS ) {
        ADD_MEMORY_DOUBLE( __tot_tick_total_en_cuotas, precio );
    }
    cod_iva = _TICKET_COD_IVA( h );

    if( IMPRESORA_FISCAL == FISCAL &&( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1 )) { 
		double ticket_siva= 0.00, ticket_iva = 0.00, cantidad = 0.00, precios= 0.00, corregir =0.00, impuestos_int = 0.00;
		//iva en tf (redondear( importe_sin_iva * cantidad),2) * iva[ITEM_COD_IVA].porc_iva1 / 100.0 

		ticket_siva =_TICKET_IMPORTE_SIN_IVA( h );
		cantidad = _TICKET_CANTIDAD( h ) ;

		ROUND2(&ticket_siva,ENTEROS, 4);
		ROUND2(&cantidad,ENTEROS, 3);
		impuestos_int = _TICKET_IMP_INT(h) * cantidad ;
		precios = ticket_siva * cantidad ;
		corregir= CORRECCION_REDONDEO_SISTEMA( &precio );
		corregir= CORRECCION_REDONDEO_SISTEMA( &precios );
		ROUND2(&precios, ENTEROS, 2);
		ROUND2(&impuestos_int, ENTEROS, 2);
		ticket_iva = (precios - impuestos_int)* iva[ITEM_COD_IVA].porc_iva1 / 100.0 
                      * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 ) ;
		
		/*if( RAM_NOTA_CR == 1 && _TICKET_TIPO( h ) & _RESTA  )
			ticket_iva = -ticket_iva;*/
		_ADD_MEMORY_DOUBLE( __tot_tick_imp_tasa, cod_iva, ticket_iva ) ;

		ticket_siva = ( ticket_siva - _TICKET_IMP_INT( h ) ) * _TICKET_CANTIDAD( h ) *  ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 )  ;
		/*if( RAM_NOTA_CR == 1 && _TICKET_TIPO( h ) & _RESTA  )
			ticket_siva = -ticket_siva;*/
		corregir= CORRECCION_REDONDEO_SISTEMA( &ticket_siva );
		ROUND2(&ticket_siva,ENTEROS, 2); //mmm será?
		_ADD_MEMORY_DOUBLE( __tot_tick_imp_neto_gravado, cod_iva,ticket_siva);
		if( _TICKET_TIPO2( h ) & _HABILITADO_CUOTAS ) {
			double total_precio = 0.00;
			ROUND2(&ticket_iva, ENTEROS,2);
			ROUND2(&ticket_siva, ENTEROS,2);
			total_precio = ticket_iva + ticket_siva + imp_int + exento;
			if(fabs( precio - total_precio) > 0.001) {
				ADD_MEMORY_DOUBLE( __tot_tick_total_en_cuotas, -precio );
				ADD_MEMORY_DOUBLE( __tot_tick_total_en_cuotas, total_precio );
			}
		}
	}else {	
		_ADD_MEMORY_DOUBLE( __tot_tick_imp_tasa, cod_iva,
                        _TICKET_IVA1_PRECIO( h ) * _TICKET_CANTIDAD( h )
                      * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 ) );

    _ADD_MEMORY_DOUBLE( __tot_tick_imp_neto_gravado, cod_iva,
                        ( _TICKET_IMPORTE_SIN_IVA( h ) - _TICKET_IMP_INT( h ) )
                      * _TICKET_CANTIDAD( h ) * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 ) );
	}
    
    _ADD_MEMORY_DOUBLE( __tot_tick_imp_sobretasa, cod_iva,
                        _TICKET_IVA2_PRECIO( h ) * _TICKET_CANTIDAD( h )
                      * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 ) );

	if( !_TICKET_EXENTO( h ) ) {
		if( cod_iva >= 0 && cod_iva <= 9 ) {
			_ADD_MEMORY_DOUBLE( __tot_tick_total_por_cod_iva, cod_iva, precio );
			_ADD_MEMORY_DOUBLE( __tot_tick_iva1_por_cod_iva, cod_iva, iva1 );
			_ADD_MEMORY_DOUBLE( __tot_tick_iva2_por_cod_iva, cod_iva, iva2 );
			_ADD_MEMORY_DOUBLE( __tot_tick_imp_int_por_cod_iva, cod_iva, imp_int );
			_ADD_MEMORY_DOUBLE( __tot_tick_base_por_cod_iva, cod_iva, importe_sin_iva );
		}
	} /*else {
		_ADD_MEMORY_DOUBLE( __tot_tick_total_por_cod_iva, 9, precio );
	}*/ // no tiene que acumular aca se usar tot_tick_exento

    //fabiana 15-02-05 ** nuevo manejo de redondeos
    #ifdef DEBUG_R     
    if( MODO_DEBUG == _DEBUG_REDONDEO ) {
        memset( cadena_debug, 0, strlen( cadena_debug ) );
        sprintf( cadena_debug, "12.1.ActAcum, TTISI=%lf, TTISIr=%lf,TTI1=%lf, TTD=%lf",
                 TOT_TICK_IMPORTE_SIN_IVA, ROUND( TOT_TICK_IMPORTE_SIN_IVA, ENTEROS, DECIMALES ),
                 TOT_TICK_IVA1, TOT_TICK_DIFERENCIA );
        //GRABAR_LOG_SISTEMA( cadena_debug );      
        memset( cadena_debug, 0, strlen( cadena_debug ) );
        sprintf( cadena_debug, "12.2.ActAcum, TISI=%lf, TISIr=%lf, TC=%lf",
                 _TICKET_IMPORTE_SIN_IVA( h ),
                 ROUND( _TICKET_IMPORTE_SIN_IVA( h ), ENTEROS, DECIMALES ), _TICKET_CANTIDAD( h ) );
        //GRABAR_LOG_SISTEMA( cadena_debug );            
        memset( cadena_debug, 0, strlen( cadena_debug ) );
        sprintf( cadena_debug, "12.3.ActAcum, TTD=%lf, temp=%lf", TOT_TICK_DIFERENCIA,
                 diferencia_temporal );
        GRABAR_LOG_SISTEMA( cadena_debug );
    }            
    #endif
}
/*****************************************************************************/
int ACTUALIZA_EVENTOS()
/*****************************************************************************/
{
    int error = 0, i;
    int _ptr, _handle, _estado, transac = 0, h, modo = 0;
    double puntos;
    struct _eventos event_ticket_recup; //estructura del ticket recuperado
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );

	/*-------------------- Graba modos de pago del ticket -------------------*/
	if( _ESTADO == 0 ) {
		SELECT_TABLE( T_PAGOS, tipo_t_pagos );
        if( con_request ) {
            if( !GET_ERROR_EN_RED() ) {
                //BEGIN_TRANSACTION();
                transac = 1;
            }
            else {
                transac = 0;
            }
        }
        if( !EMITIR_REMITO() && ( !RAM_NOTA_CR || !MEDIO_PAGO_NC ) ) {
            GRABA_EVENTOS_PAGO( 0, NO );
        }
		/*SELECT_TABLE( T_PAGOS, tipo_t_pagos );
        if( con_request && transac ) {
            END_TRANSACTION();
        }*/
        _SET_ESTADO( 1 );
    } 

    if( _ESTADO == 1 ) {
        GRABA_EVENTOS_EXT_IMPUESTOS();
        _SET_ESTADO( 2 );
    }

    if( _ESTADO == 2 ) {
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            if( _TICKET_COD_SOLICITUD( h ) ) {
                AGREGA_BIN_EV_DMEDIO_A_BTRV( h, D_ITEMS );
            }
        }
        _SET_ESTADO( 3 );
    }

    if( _ESTADO == 3 ) {
        AGREGA_BIN_EV_DMEDIO_A_BTRV( 0, D_CLIENTES );
        _SET_ESTADO( 4 );
    }
	
	if( _ESTADO == 4 ) {
		if( RAM_FACTURA || RAM_NOTA_CR || RAM_REFERENCIA_NC || RAM_MODO_DEVOLUCION ) {
            modo = 1;
        }
        memset( &event_ticket, 0, sizeof( event_ticket ) ); //cerea la estructura
        if( CANTIDAD_REGISTROS_REC(ARCH_REC_IMPORT,sizeof( struct _eventos ) ) ) { //si existe una importacion
            //recupera la estructura ticket del evento importado
            RECUPERAR_ARCHIVO_REC(ARCH_REC_IMPORT,(char *)&event_ticket_recup,sizeof( struct _eventos),0);
            event_ticket.cajaz_importado = event_ticket_recup.caja_z;
            event_ticket.id_evento_importado = event_ticket_recup.id_evento;
            if( MODO_DEVOLUCION && event_ticket_recup.tipo_evento == 30 
					&& event_ticket_recup.tipo_evento_importado == 4 )
				event_ticket.tipo_evento_importado = event_ticket_recup.tipo_evento_importado;
            else
				event_ticket.tipo_evento_importado = event_ticket_recup.tipo_evento;
		} else {
            event_ticket.cajaz_importado = 0;
            event_ticket.id_evento_importado = 0;
            event_ticket.tipo_evento_importado = 0;        
        }
        //event_ticket.nro_ticket = NRO_TICKET;
        if( config.pais == COLOMBIA || config.pais == ECUADOR ) {
            event_ticket.nro_ticket = NRO_EVENTO;
        }
        else {
            event_ticket.nro_ticket = ( comp.nro_comp > 0 && modo == 1 ) ? 0 : NRO_TICKET;
            if( event_ticket.nro_ticket && ( config.pais == CHILE || config.pais == BRASIL )
					&& IMPRESORA_FISCAL == FISCAL ) {
                event_ticket.nro_ticket = NRO_TICKET - 1;
            }
        }
        event_ticket.bultos = RAM_BULTOS;
        event_ticket.recargo = RAM_INTERESES;
        event_ticket.nro_cliente = RAM_NRO_CLIENTE;
        event_ticket.suc_comprobante = RAM_NRO_SUC;
		if( event_ticket.nro_ticket ){
			event_ticket.nro_comprobante_new = 0;
		} else {
			event_ticket.nro_comprobante_new = RAM_NRO_COMP;
		}
        event_ticket.importe_sin_iva = TOT_TICK_IMPORTE_SIN_IVA - TOT_TICK_IMP_INT;
        event_ticket.imp_int = TOT_TICK_IMP_INT;
        event_ticket.costo = TOT_TICK_COSTO;
        event_ticket.recargo_tarjeta = TOT_TICK_RECARGO_TARJETA;
        event_ticket.exento = TOT_TICK_EXENTO;
        event_ticket.iva1 = TOT_TICK_IVA1;
        event_ticket.iva2 = TOT_TICK_IVA2;
        //event_ticket.ila = TOT_TICK_ILA;
        //event_ticket.iha = TOT_TICK_IHA;
        //event_ticket.alicuota = TOT_TICK_ALICUOTA;
		if (IMPRESORA_FISCAL == FISCAL)
			event_ticket.nro_z = NRO_Z;
        for( i = 0;i < 4;i++ ) {
            event_ticket.monto_impuesto[i] = _TOT_TICK_IMPUESTOS( i );
        }

        event_ticket.tot_variacion = RAM_VARIACION;
        event_ticket.cliente_afinidad = RAM_CLIENTE_AFINIDAD;
        if( config.pais == CHILE || config.pais == BRASIL ) {
            event_ticket.nro_vendedorl = RAM_CODIGO_SUPERVISOR;
            SET_MEMORY_LONG( __ram_codigo_supervisor, 0L );
        }
        else {
            event_ticket.nro_vendedorl = NRO_VENDEDOR_LONG;
        }
        event_ticket.nro_vendedor = NRO_VENDEDOR;
        event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;
        event_ticket.retencion_dgr = ( ( RAM_NOTA_CR || MODO_DEVOLUCION ) ? -1 : 1 ) * RAM_RETENCION_DGR;
        event_ticket.percepcion_dgi = ( ( RAM_NOTA_CR ) ? -1 : 1 ) * RAM_PERCEPCION_DGI;
        puntos = RAM_ACU_PUNTOS;
        event_ticket.puntos = ( float )puntos;
        event_ticket.percep_212_iva1 = ( ( ( RAM_NOTA_CR ) ? -1 : 1 ) * RAM_PERCEPCION_212_IVA1 );
        event_ticket.percep_212_iva2 = ( ( ( RAM_NOTA_CR ) ? -1 : 1 ) * RAM_PERCEPCION_212_IVA2 );
        strcpy( event_ticket.legajo_empleado, RAM_STRING_LEGAJO_EMPLEADO );
        if( EMITIR_FACTURA_REMOTA() ) {
            if( RAM_NOTA_CR ) {
                event_ticket.tipo_evento = ( ES_FACTURA_A( clientes.cond_iva ) ? 19 : 20 );
            }
            else {
                event_ticket.tipo_evento = ( ES_FACTURA_A( clientes.cond_iva ) ? 16 : 17 );
            }
        }
        else if( EMITIR_FACTURA() ) {
            if( RAM_NOTA_CR ) {
                event_ticket.tipo_evento = ( ES_FACTURA_A( clientes.cond_iva ) ? 5 : 6 );
            }
            else {
                event_ticket.tipo_evento = ( ES_FACTURA_A( clientes.cond_iva ) ? 3 : 4 );
            }
		} else if( RAM_TICKET_FACTURA ) {
			if( RAM_TICKET_FACTURA == 1 ) {
				event_ticket.tipo_evento = 35;
			} else {
				event_ticket.tipo_evento = 36;
			}
		}
        else if( EMITIR_REMITO_REMOTO() ) {
            event_ticket.tipo_evento = 18;
        }
        else if( EMITIR_REMITO() ) {
            event_ticket.tipo_evento = 11;
        }
        if( RAM_NRO_CLIENTE ) {
            strcpy( event_ticket.cuit, clientes.cuit );
            strcpy( event_ticket.nri, clientes.nri );
        }
        if( MODO_DEVOLUCION && !NC_PERMANENTE ) {
            event_ticket.tipo_evento = 21;
        }
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
        if( con_request ) {
            if( !GET_ERROR_EN_RED() ) {
                //BEGIN_TRANSACTION();
                transac = 1;
            }
            else {
                transac = 0;
            }
        }
		error = GRABAR_EVENT_TICKET( DEBE_GRABAR_ALL() );
		_SET_ESTADO( 5 );
	}

	if( _ESTADO == 5 ) {
		if( event_ticket.tipo_evento == 0 ) {
            ADD_MEMORY_LONG( __x_cantidad_tickets, 1L );
        }
		_SET_ESTADO( 6 );
	}

	if( _ESTADO == 6 ) {
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
        if( con_request && transac ) {
            //END_TRANSACTION();
        }
		/*if( error == 1)
			_SET_ESTADO( 7 ); //no hay forma de anular todo el evento porque ya se cerro el ticket y pagos.
		else*/
		ACTUALIZA_ID_EVENTO_NEW();
        _SET_ESTADO( 99 );

    }
    CLOSE_RECUPERO( _ptr, _handle );

    return ( error );
}
/*****************************************************************************/
void GRABA_EVENTOS_PAGO( int offset_posicion, int devolucion_cobro )
/*****************************************************************************/
{
    int h, submedio = 0, j, asignar, tarjeta = 0;
    long id;
    double resto,efectivo,v;
    int hay_cheques_en_dpago = 0;
    memset( &event_pago, 0, sizeof( event_pago ) );
    /*------------ Se obtiene el vuelto en efectivo -----------*/
    resto = RAM_VUELTO;
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        v = _PAGO_VUELTO( h );
        if( v ) {
            resto -= v;
        }
        v = _PAGO_VUELTO_MEDIO( h );
        if( v ) {
            resto -= v;
        }
    }
    /*------------------- Calcula vuelto en medios de pago ------------------
    for( h = 0; h < RAM_P_PAGO; h++ ) {
        if( medios[_PAGO_MODO( h )].efectivo ) {
            efectivo = _PAGO_IMPORTE( h );
        }
    }*/
    if( resto < 0 ) {
        resto = 0;
    }
    /*------------------------- Genera Eventos -------------------------------*/
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        /*------------------------ Define ID ----------------------------------*/
        //id = 0;
        id = _PAGO_NUMERO( h );
        submedio = 0;
        if( medios[_PAGO_MODO( h )].efectivo ) {
			double precio_final;
			precio_final = _PAGO_IMPORTE( h );
			ROUND2(&precio_final,ENTEROS,2);
            //efectivo = _PAGO_IMPORTE( h ); 
			efectivo = precio_final;
        }
        if( medios[_PAGO_MODO( h )].cta_cte ) {
            if( UTILIZA_SOCIOS ) {
                id = _PAGO_NUMERO( h );
            }
            else {
                id = _PAGO_CUENTA( h );
            }
        }
        if( medios[_PAGO_MODO( h )].cheque ) {
            id = _PAGO_NUMERO( h );
            submedio = _PAGO_COD_SUBMEDIO( h );
            //id_registro = _PAGO_ID_REGISTRO( h ); es el codigo de medio         
        }
        if( medios[_PAGO_MODO( h )].tarjeta ) {
            //id = _PAGO_NRO_CUPON_INT1( h );
            id = _PAGO_CUENTA( h );
            submedio = _PAGO_TARJETA( h ) + 1;
			tarjeta = _PAGO_TARJETA( h ) + 1;
        }
        if( medios[_PAGO_MODO( h )].ticket_compra ) {
            id = _PAGO_CUENTA( h );
            submedio = id;
			if( GET_STATIC_REINTENTO_QR()> 0 ) 
				 event_pago.datafono =GET_STATIC_REINTENTO_QR() - 1;
        }
        if( medios[_PAGO_MODO( h )].mutual ) {
            id = _PAGO_CUENTA( h );
            submedio = id;
        }
        /*-------------------- Calcula importe y vuelto ----------------------*/
        if( medios[_PAGO_MODO( h )].efectivo ) {
            event_pago.importe = efectivo;
            event_pago.vuelto_propio = 0;
            event_pago.vuelto_medio = 0;
        }
        else {
            event_pago.vuelto_propio = _PAGO_VUELTO( h );
            event_pago.importe = _PAGO_IMPORTE( h );
            event_pago.vuelto_medio = _PAGO_VUELTO_MEDIO( h );
			if( medios[_PAGO_MODO( h )].mutual && MUTUAL_MONEDA_EXTRANJERA( _PAGO_COD_SUBMEDIO_VUELTO( h ) ) ){
				event_pago.vuelto_medio_me = MUTUALES_ML_A_ME( _PAGO_VUELTO_MEDIO( h ), _PAGO_CUENTA( h ) );
			}
        }
        for( asignar = 1, j = h + 1;j < RAM_P_PAGO;j++ ) {
            if( ( _PAGO_TIPO_VUELTO( j ) == _VUELTO_EN_EFECTIVO ) ) {
                asignar = 0;
            }
        }
        if( asignar ) {
            if( RAM_RESTO_ME ) {
                event_pago.vuelto_efectivo = 0;
                event_pago.vuelto_medio = resto;
                event_pago.cod_medio_vuelto = (char)_medio_moneda_extranjera + 1;
                event_pago.cod_submedio_vuelto = 0;
                event_pago.vuelto_medio_me = -RAM_RESTO_ME;
            }
            else {
                event_pago.vuelto_efectivo = resto;
            }
        }
        else {
            event_pago.vuelto_efectivo = 0.0;
        }
        resto -= event_pago.vuelto_efectivo;
        if( resto < 0 ) {
            resto = 0;
        }
        /*------------------------ Graba el evento --------------------------*/
        event_pago.modo_pago = _PAGO_MODO( h ) + 1;
        event_pago.numero_id = id;
        event_pago.retirado = 0;
        //event_pago.reproceso   = 0;
        event_pago.reproceso = ( ( MODO_NEGOCIO == SERVICIOS && RAM_REPROCESANDO )
                               ? EVENTO_REPROCESADO : 0 );
        event_pago.tarjeta = tarjeta;
        event_pago.buffer1_long = _PAGO_BUFFER1_LONG( h );
        event_pago.banco = _PAGO_BANCO( h );
        event_pago.fecha_vto = _PAGO_FECHA_VTO( h );
        event_pago.cantidad_cupones = _PAGO_CANTIDAD_CUPONES( h );
        event_pago.valor_cupones = _PAGO_VALOR_CUPONES( h );
        event_pago.cod_post_cheq = _PAGO_COD_POST_CHEQ( h );
        event_pago.cod_hora_acre = _PAGO_COD_HORA_ACRE( h );
        event_pago.nro_cupon1 = _PAGO_NRO_CUPON1( h );
        event_pago.cuotas = _PAGO_CUOTAS( h );
        event_pago.nro_boletin = _PAGO_NRO_BOLETIN( h );
        event_pago.status_tarjeta = _PAGO_STATUS_TARJETA( h );
        event_pago.variacion = _PAGO_VARIACION( h );
        event_pago.nro_cupon_int1 = _PAGO_NRO_CUPON_INT1( h );
        event_pago.moneda = _PAGO_MONEDA( h );
        event_pago.cod_vigencia = _PAGO_COD_VIGENCIA( h );
        event_pago.primer_vto = _PAGO_PRIMER_VTO( h );
        event_pago.importe_entrega = _PAGO_IMPORTE_ENTREGA( h );
        event_pago.id_registro = _PAGO_ID_REGISTRO( h );
        event_pago.cod_sub_medio = submedio;
		
		if( medios[_PAGO_MODO( h )].ticket_compra && GET_STATIC_REINTENTO_QR()> 0 ) 
			strncpy( event_pago.nro_tarjeta, _PAGO_NRO_TARJETA( h ), SIZEOF_VAR( __pago_nro_tarjeta ) );
		else            
			memset(event_pago.nro_tarjeta,0, sizeof(event_pago.nro_tarjeta) );

        if( !( asignar && RAM_RESTO_ME ) ) {
            event_pago.cod_medio_vuelto = _PAGO_COD_MEDIO_VUELTO( h );
            event_pago.cod_submedio_vuelto = _PAGO_COD_SUBMEDIO_VUELTO( h );
        }
		event_pago.recargo_tarjeta = ( medios[_PAGO_MODO( h )].tarjeta ) ? 
			_PAGO_IMPORTE( h ) - ( _PAGO_IMPORTE( h ) / ( 1 + ( VAR_TARJETAS_RECARGO / 100 ) ) ) : 
			0.0;
#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
		event_pago.fecha_vto = 0;
		if( medios[_PAGO_MODO( h )].tarjeta )  {
			if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
				strncpy( event_pago.nro_tarjeta, _PAGO_NRO_TARJETA( h ), SIZEOF_VAR( __pago_nro_tarjeta ) );
				strncpy( &event_pago.nro_tarjeta[2], MASCARA( '*', 6, "" ), 6 );
			} else {
				strncpy( event_pago.nro_tarjeta, _PAGO_NRO_TARJETA( h ), SIZEOF_VAR( __pago_nro_tarjeta ) );	
				ENMASCARA_NUMERO_TARJETA( _PAGO_TARJETA( h ),_PAGO_NRO_TARJETA( h ),event_pago.nro_tarjeta);
			}
			event_pago.nro_tarjeta[20]= '\0';
		}
#else
	event_pago.fecha_vto = _PAGO_FECHA_VTO( h );
	strncpy( event_pago.nro_tarjeta, _PAGO_NRO_TARJETA( h ), SIZEOF_VAR( __pago_nro_tarjeta ) );	
#endif

        memcpy( event_pago.auto_tarjeta, _PAGO_AUTO_TARJETA( h ),
                SIZEOF_VAR( __pago_auto_tarjeta ) );
        strcpy( event_pago.nro_comercio, _PAGO_BUFFER( h ) );
        memcpy( event_pago.tabla, _PAGO_TABLA( h ), SIZEOF_VAR( __pago_tabla ) );
        strcpy( event_pago.cuit, _PAGO_CUIT( h ) );
        strcpy( event_pago.telefono, _PAGO_TELEFONO( h ) );
        event_pago.importe_me = _PAGO_IMPORTE_ME( h );
        event_pago.iva_proporcional = _PAGO_IVA_PROPORCIONAL( h );
        if( RAM_NOTA_CR || MODO_DEVOLUCION || devolucion_cobro ) {
            event_pago.importe *= -1;
            event_pago.vuelto_propio *= -1;
            event_pago.vuelto_efectivo *= -1;
            event_pago.variacion *= -1;
            event_pago.vuelto_medio *= -1;
            event_pago.importe_me *= -1;
        }
        event_pago.posicion = h + offset_posicion;
        GRABAR_EVENT_PAGO( NO );
        AGREGA_BIN_EV_T_COMPRA_A_BTRV();
        AGREGA_BIN_EV_DMEDIO_A_BTRV( h, D_MEDIOS );
        AGREGA_BIN_EV_DMEDIO_A_BTRV( h, D_MEDIOS_PRE_IMPORTE );
        if( MODO_DEVOLUCION ) {
            //reimpresion y reverso
            if( MODO_CHEQUE_COMO_UN_MEDIO && medios[_PAGO_MODO( h )].cheque
             && event_ticket.tipo_evento != _EVENT_VALORES_EN_SUCURSAL
             && event_ticket.tipo_evento != _EVENT_PAGO_ESPECIAL ) {
                //copio directamente la dpago importada en la dpago
                //CARGA_IMPORTADOS();
                hay_cheques_en_dpago = 1;
            }
        } else {
            if( MODO_CHEQUE_COMO_UN_MEDIO && medios[_PAGO_MODO( h )].cheque
             && event_ticket.tipo_evento != _EVENT_VALORES_EN_SUCURSAL
                //ESTa linea q sigue es una truchada. Hay q corregirla
                && event_ticket.tipo_evento != _EVENT_PAGO_ESPECIAL ) {
                int i, ind = _PAGO_MODO( h ) * 100;         
                for( i = 0;i < _PAGO_NUMERO( h );i++ ) {
                    AGREGA_BIN_EV_DMEDIO_A_BTRV( ind, D_MEDIOS_PRE_IMPORTE );
                    ind++;
                }
            }
        }
	tarjeta = 0;
	}
    if( hay_cheques_en_dpago ) {
        //copio directamente la dpago importada en la dpago
        CARGA_IMPORTADOS();
    }
}
/*****************************************************************************/
void GRABA_EVENTOS_PAGOS_ESP( int offset_posicion )
/*****************************************************************************/
{
    int h, submedio = 0, j, asignar;
    long id;
    double resto = 0.0, efectivo = 0;
    memset( &event_pago, 0, sizeof( event_pago ) );
    /*------------------------- Genera Eventos -------------------------------*/
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        /*------------------------ Define ID ----------------------------------*/
        //id = 0;
        id = _PAGO_NUMERO( h );
        submedio = 0;
        if( medios[_PAGO_MODO( h )].cta_cte ) {
            if( UTILIZA_SOCIOS ) {
                id = _PAGO_NUMERO( h );
            }
            else {
                id = _PAGO_CUENTA( h );
            }
        }
        if( medios[_PAGO_MODO( h )].cheque ) {
            id = _PAGO_NUMERO( h );
        }
		//Se comenta porque ocaciona problemas con el mapeo 
		//debido que la tabla de la base del servidor no tiene
		//este campo. Además se utiliza solo en esta funcion que no es 
		//llamada en ninguna parte del codigo
        /*if( medios[_PAGO_MODO( h )].tarjeta ) {
            //id = _PAGO_NRO_CUPON_INT1( h );
            id = _PAGO_CUENTA( h );
            submedio = _PAGO_TARJETA( h ) + 1;
            event_pago.tipo_cuenta_tarj = VAR_TARJETAS_TIPO_CUENTA;
        }*/
        if( medios[_PAGO_MODO( h )].ticket_compra ) {
            id = _PAGO_CUENTA( h );
            submedio = id;
        }
        if( medios[_PAGO_MODO( h )].mutual ) {
            id = _PAGO_CUENTA( h );
            submedio = id;
        }
        /*-------------------- Calcula importe y vuelto ----------------------*/
        if( medios[_PAGO_MODO( h )].efectivo ) {
            event_pago.importe = efectivo;
            event_pago.vuelto_propio = 0;
            event_pago.vuelto_medio = 0;
        }
        else {
            event_pago.vuelto_propio = _PAGO_VUELTO( h );
            event_pago.importe = _PAGO_IMPORTE( h );
            event_pago.vuelto_medio = _PAGO_VUELTO_MEDIO( h );
        }
        for( asignar = 1, j = h + 1;j < RAM_P_PAGO;j++ ) {
            if( ( _PAGO_TIPO_VUELTO( j ) == _VUELTO_EN_EFECTIVO ) ) {
                asignar = 0;
            }
        }
        if( asignar ) {
            if( RAM_RESTO_ME ) {
                event_pago.vuelto_efectivo = 0;
                event_pago.vuelto_medio = resto;
                event_pago.cod_medio_vuelto = (char)_medio_moneda_extranjera + 1;
                event_pago.cod_submedio_vuelto = 0;
                event_pago.vuelto_medio_me = -RAM_RESTO_ME;
            }
            else {
                event_pago.vuelto_efectivo = resto;
            }
        }
        else {
            event_pago.vuelto_efectivo = 0;
        }
        resto -= event_pago.vuelto_efectivo;
        if( resto < 0 ) {
            resto = 0;
        }
        /*------------------------ Graba el evento --------------------------*/
        event_pago.modo_pago = _PAGO_MODO( h ) + 1;
        event_pago.numero_id = id;
        event_pago.tarjeta = _PAGO_TARJETA( h );
        event_pago.buffer1_long = _PAGO_BUFFER1_LONG( h );
        event_pago.banco = _PAGO_BANCO( h );
        event_pago.fecha_vto = _PAGO_FECHA_VTO( h );
        event_pago.cantidad_cupones = _PAGO_CANTIDAD_CUPONES( h );
        event_pago.valor_cupones = _PAGO_VALOR_CUPONES( h );
        event_pago.cod_post_cheq = _PAGO_COD_POST_CHEQ( h );
        event_pago.cod_hora_acre = _PAGO_COD_HORA_ACRE( h );
        event_pago.nro_cupon1 = _PAGO_NRO_CUPON1( h );
        event_pago.cuotas = _PAGO_CUOTAS( h );
        event_pago.nro_boletin = _PAGO_NRO_BOLETIN( h );
        event_pago.status_tarjeta = _PAGO_STATUS_TARJETA( h );
        event_pago.variacion = _PAGO_VARIACION( h );
        event_pago.nro_cupon_int1 = _PAGO_NRO_CUPON_INT1( h );
        event_pago.moneda = _PAGO_MONEDA( h );
        event_pago.cod_vigencia = _PAGO_COD_VIGENCIA( h );
        event_pago.primer_vto = _PAGO_PRIMER_VTO( h );
        event_pago.importe_entrega = _PAGO_IMPORTE_ENTREGA( h );
        event_pago.id_registro = _PAGO_ID_REGISTRO( h );
        event_pago.cod_sub_medio = submedio;
        if( !( asignar && RAM_RESTO_ME ) ) {
            event_pago.cod_medio_vuelto = _PAGO_COD_MEDIO_VUELTO( h );
            event_pago.cod_submedio_vuelto = _PAGO_COD_SUBMEDIO_VUELTO( h );
        }
        strncpy( event_pago.nro_tarjeta, _PAGO_NRO_TARJETA( h ), SIZEOF_VAR( __pago_nro_tarjeta ) );
        memcpy( event_pago.auto_tarjeta, _PAGO_AUTO_TARJETA( h ),
                SIZEOF_VAR( __pago_auto_tarjeta ) );
        strcpy( event_pago.nro_comercio, _PAGO_BUFFER( h ) );
        memcpy( event_pago.tabla, _PAGO_TABLA( h ), SIZEOF_VAR( __pago_tabla ) );
        strcpy( event_pago.cuit, _PAGO_CUIT( h ) );
        strcpy( event_pago.telefono, _PAGO_TELEFONO( h ) );
        event_pago.importe_me = _PAGO_IMPORTE_ME( h );
        event_pago.iva_proporcional = _PAGO_IVA_PROPORCIONAL( h );
        if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
            event_pago.importe *= -1;
            event_pago.vuelto_propio *= -1;
            event_pago.vuelto_efectivo *= -1;
            event_pago.variacion *= -1;
            event_pago.vuelto_medio *= -1;
            event_pago.importe_me *= -1;
        }
        event_pago.posicion = h + offset_posicion;
        GRABAR_EVENT_PAGO( NO );
		#ifdef COMPILAR_LOGS_VIA_TCP
		{
	      char pago_aux[80];
      	memset(pago_aux,0,80);
      	sprintf(pago_aux,"Modo Pago:[%i]:$%7.2lf ,ME:$%7.2lf,Vuelto:$%7.2lf,Vuelt:$%7.2lf",
      	event_pago.modo_pago,
      	event_pago.importe,
      	event_pago.importe_me,
      	event_pago.vuelto_efectivo,
      	event_pago.vuelto_medio);
	      LOGS_VIA_TCP(pago_aux);
	   }
		#endif
    }
}

/*****************************************************************************/
void ACTUALIZA_MEMORIA()
/*****************************************************************************/
{
    double importe,signo,cant,recargo,importe_total,imp_int;
    int h;
    int _ptr,_handle,_estado,_h;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    /*  _estado = STACK_INS( sizeof( int ) );
        _h = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
    _h = STACK_INS( sizeof( short ) );
    for( ;GET_MEMORY_INT( _h ) < RAM_P_TICKET;ADD_MEMORY_INT( _h, 1 ) ) {
        h = GET_MEMORY_INT( _h );
        /*------------------- Calcula los importes ----------------------------*/
        signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
        cant = _TICKET_CANTIDAD( h ) * signo;
        importe_total = cant * ( _TICKET_PRECIO( h ) + _TICKET_RECARGO( h ) - _TICKET_IMP_INT( h )
                               * _TICKET_PRESENTACION( h ) );
        imp_int = cant * _TICKET_IMP_INT( h ) * _TICKET_PRESENTACION( h );
        importe = cant * _TICKET_PRECIO( h );
        recargo = cant * _TICKET_RECARGO( h );
        /*--------------- Actualiza acumuladores de departamentos --------------*/
        if( _ESTADO == 0 ) {
            _ACUMULA_DEPARTAMENTO( _TICKET_DEPTO( h ), importe + recargo,
                                   cant * ( ( RAM_NOTA_CR ) ? -1 : 1 ) );
            _SET_ESTADO( 1 );
        }
        /*---------------- Actualiza acumulador de anulaciones -----------------*/
		if( _ESTADO == 1 ) {
			if( !ES_ENVASE( h ) ) {
				if(!((_TICKET_ORIGEN( h ) & _ORIGEN_PROMOCION) ||(_TICKET_ORIGEN( h ) & _ORIGEN_PROMOCION_DESC))){
					if( ( _TICKET_TIPO( h ) & _RESTA ) && !RAM_EN_IMPORTACION_DE_TICKET ) {
						if( _TICKET_TIPO2( h ) & _CORRECCION ) {
							SET_MEMORY_DOUBLE( __x_correccion_importe,
								X_CORRECCION_IMPORTE + ( importe_total + imp_int )
								* signo );
							ADD_MEMORY_INT( __x_correccion_cantidad, 1 );
						}else {
							SET_MEMORY_DOUBLE( __x_anulaciones_importe,
								X_ANULACIONES_IMPORTE + ( importe_total + imp_int )
								* signo );/*Guarda la cantidad de anulaciones*/	
							if(!_TICKET_TIPO3(h)){//si no es pesable
								ADD_MEMORY_INT( __x_anulaciones_cantidad, (int)(cant * signo) );
							}else{
								ADD_MEMORY_INT( __x_anulaciones_cantidad, 1 );
							}
						}
					}
				}
			}
            _SET_ESTADO( 2 );
		}
        /*------------------------ Devolucion de envases -----------------------*/
        if( _ESTADO == 2 ) {
            if( ES_ENVASE( h ) ) {
				if( _TICKET_TIPO( h ) & _RESTA ) {
					SET_MEMORY_DOUBLE( __x_dev_envases_importe,
										X_DEV_ENVASES_IMPORTE 
										//+ ( ( importe_total + imp_int ) * ( -1 ) ) );
										+ ( importe_total + imp_int ));
					ADD_MEMORY_INT( __x_dev_envases_cantidad, ( int )cant );
				} else {
					//Los importes deben acumularse en negativo y las cantidades en positivo
					SET_MEMORY_DOUBLE( __x_dev_envases_importe,
										X_DEV_ENVASES_IMPORTE 
										//- ( importe_total + imp_int ) );
										+ ( importe_total + imp_int ) );
					ADD_MEMORY_INT( __x_dev_envases_cantidad, ( int )( -1 ) * cant ); 
				}
            } 
        }
        _SET_ESTADO( 0 );
    }
    if( RAM_TOTAL_DONACION > 0.005 ) {
        ADD_MEMORY_DOUBLE( __x_donaciones_importe, RAM_TOTAL_DONACION );
        ADD_MEMORY_INT( __x_donaciones_cantidad, 1 );
    }
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
void RESETEA_TOT_TICK()
/*****************************************************************************/
{
    CEREAR_STRUCT( _tot_tick );
}
/*****************************************************************************/
void ACUMULA_FACTURAS()
/*****************************************************************************/
{
    int i = 0;
    double tot_imp = 0.0, total_gral=0.0, total_a_ctrl = 0.0, iva_a_ctrl = 0.0,total_grav_a_ctrl = 0.0,extras_a_ctrl=0.0;
	total_gral = PAGOS_TOTAL( 0 );
	
	
	total_a_ctrl = total_gral;
	iva_a_ctrl =  TOT_TICK_IVA1 ;
	total_grav_a_ctrl = TOT_TICK_IMPORTE_SIN_IVA;
	extras_a_ctrl = ( total_gral - ( TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1) );
	EQUILIBRA_IMPORTES_TOTALES(&total_a_ctrl, &total_grav_a_ctrl, &iva_a_ctrl, &extras_a_ctrl, iva[0].porc_iva1  );

    ADD_MEMORY_DOUBLE( __x_retencion_dgr, 
		( ( RAM_NOTA_CR || MODO_DEVOLUCION ) ? -1 : 1 ) * RAM_RETENCION_DGR );
    ADD_MEMORY_DOUBLE( __x_percepcion_dgi, 
		( ( RAM_NOTA_CR ) ? -1 : 1 ) * RAM_PERCEPCION_DGI );
    ADD_MEMORY_DOUBLE( __x_percepcion_212_iva1,
        ( ( ( RAM_NOTA_CR ) ? -1 : 1 ) * RAM_PERCEPCION_212_IVA1 ) );
    ADD_MEMORY_DOUBLE( __x_percepcion_212_iva2,
        ( ( ( RAM_NOTA_CR ) ? -1 : 1 ) * RAM_PERCEPCION_212_IVA2 ) );
    ADD_MEMORY_DOUBLE( __x_ventas_gravadas,
		( TOT_TICK_IMPORTE_SIN_IVA - TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA ) );
	ADD_MEMORY_DOUBLE( __x_iva1, TOT_TICK_IVA1 );
	ADD_MEMORY_DOUBLE( __x_iva2, TOT_TICK_IVA2 );
	//ADD_MEMORY_DOUBLE( __x_ila, TOT_TICK_ILA );
	//ADD_MEMORY_DOUBLE( __x_iha, TOT_TICK_IHA );
	ADD_MEMORY_DOUBLE( __x_ventas_exentas, TOT_TICK_EXENTO );
	ADD_MEMORY_DOUBLE( __x_imp_int, TOT_TICK_IMP_INT  );
    ADD_MEMORY_DOUBLE( __x_diferencia, TOT_TICK_DIFERENCIA );
    for( i = 0;i < 4;i++ ) {
        tot_imp += _TOT_TICK_IMPUESTOS( i );
    }
    if( ES_FACTURA_A( clientes.cond_iva ) ) {
        if( RAM_NOTA_CR ) {
            //fabiana 15-02-05 ** nuevo manejo de redondeos             
            SET_MEMORY_DOUBLE( __x_nc_a_importe, X_NC_A_IMPORTE + total_a_ctrl );
            SET_MEMORY_DOUBLE( __x_nc_a_importe_exento, X_NC_A_IMPORTE_EXENTO + TOT_TICK_EXENTO );
            SET_MEMORY_DOUBLE( __x_nc_a_importe_gravado,X_NC_A_IMPORTE_GRAVADO +( total_grav_a_ctrl
                               /*TOT_TICK_IMPORTE_SIN_IVA*/ - TOT_TICK_IMP_INT
                             + TOT_TICK_RECARGO_TARJETA ));
            SET_MEMORY_DOUBLE( __x_iva_nc_a,
                               X_IVA_NC_A +  ( iva_a_ctrl/*TOT_TICK_IVA1 */+ TOT_TICK_IVA2 + tot_imp  ) );
            ADD_MEMORY_INT( __x_nc_a_cantidad, 1 );
        }
        else {
			if( !RAM_MODO_DEVOLUCION ) {
				//fabiana 15-02-05 ** nuevo manejo de redondeos
				
				SET_MEMORY_DOUBLE( __x_facturas_a_importe, X_FACTURAS_A_IMPORTE + total_a_ctrl );
				SET_MEMORY_DOUBLE( __x_factura_a_importe_exento,
								X_FACTURA_A_IMPORTE_EXENTO + TOT_TICK_EXENTO );

				SET_MEMORY_DOUBLE( __x_factura_a_importe_gravado,
								X_FACTURA_A_IMPORTE_GRAVADO + ( total_grav_a_ctrl 
								/*TOT_TICK_IMPORTE_SIN_IVA*/
								- TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA ));

				SET_MEMORY_DOUBLE( __x_iva_factura_a,
								X_IVA_FACTURA_A + 
								( /*TOT_TICK_IVA1 */ iva_a_ctrl + TOT_TICK_IVA2 + tot_imp ) );

				ADD_MEMORY_INT( __x_facturas_a_cantidad, 1 );
			}
        }
    }
    else {
        if( RAM_NOTA_CR ) {
            SET_MEMORY_DOUBLE( __x_nc_b_importe, X_NC_B_IMPORTE + total_gral );
            ADD_MEMORY_INT( __x_nc_b_cantidad, 1 );
        }else {
			    if( !RAM_MODO_DEVOLUCION ) {
					SET_MEMORY_DOUBLE( __x_facturas_b_importe, X_FACTURAS_B_IMPORTE + total_gral );
					ADD_MEMORY_INT( __x_facturas_b_cantidad, 1 );
				}
            
        }
        if( clientes.cond_iva != DIPLOMATICO_EXPORTACION ) {
            //Para clientes que no son diplomaticos ni de exportacion
            ACUMULA_CONSUM_FINAL_FACT();
        }
        else {
            if( clientes.cond_iva == DIPLOMATICO_EXPORTACION ) {
                //Diplomaticos
                /* Cuando se hacen devoluciones de Facturas B y
                   no Notas de Credito, entonces RAM_NOTA_CR esta en 0*/
                if( RAM_NOTA_CR || RAM_MODO_DEVOLUCION ) {
                    SET_MEMORY_DOUBLE( __x_devoluciones_diplom_importe,
                                       X_DEVOLUCIONES_DIPLOM_IMPORTE + TOT_TICK_EXENTO
                                     + total_grav_a_ctrl/*TOT_TICK_IMPORTE_SIN_IVA*/ - TOT_TICK_IMP_INT );
                    ADD_MEMORY_INT( __x_devoluciones_diplom_cantidad, 1 );
                }
                else {
                    SET_MEMORY_DOUBLE( __x_ventas_diplom_importe,
                                       X_VENTAS_DIPLOM_IMPORTE + TOT_TICK_EXENTO
                                     + total_grav_a_ctrl/*TOT_TICK_IMPORTE_SIN_IVA*/ - TOT_TICK_IMP_INT );
                    ADD_MEMORY_INT( __x_ventas_diplom_cantidad, 1 );
                }
            }
            else {
                //Exportacion
                /* Cuando se hacen devoluciones de Facturas B y
                   no Notas de Credito, entonces RAM_NOTA_CR esta en 0*/
                if( RAM_NOTA_CR || RAM_MODO_DEVOLUCION ) {
                    SET_MEMORY_DOUBLE( __x_devoluciones_export_importe,
                                       X_DEVOLUCIONES_EXPORT_IMPORTE + TOT_TICK_EXENTO
                                     + total_grav_a_ctrl/*TOT_TICK_IMPORTE_SIN_IVA */- TOT_TICK_IMP_INT );
                    ADD_MEMORY_INT( __x_devoluciones_export_cantidad, 1 );
                }
                else {
                    SET_MEMORY_DOUBLE( __x_ventas_export_importe,
                                       X_VENTAS_EXPORT_IMPORTE + TOT_TICK_EXENTO
                                     + total_grav_a_ctrl/*TOT_TICK_IMPORTE_SIN_IVA*/ - TOT_TICK_IMP_INT );
                    ADD_MEMORY_INT( __x_ventas_export_cantidad, 1 );
                }
            }
        }
    }
}
// ej EQUILIBRA_IMPORTES_TOTALES(&total_a_ctrl		, &total_grav_a_ctrl		, &iva_a_ctrl	, &extras_a_ctrl	, iva[0].porc_iva1  );
/*****************************************************************************/
void EQUILIBRA_IMPORTES_TOTALES(double * total	, double * importe_sin_iva	, double * iva1	, double * exento	,float iva){
/*****************************************************************************/
/* Esta funcion contempla un solo iva a fin de probar el desarrollo para verificar si funciona y 
extenderlo a ivas multiples, barriendo los importes correspondientes....
*/	


	if(*iva1 && iva > 0.001 ){
		double total_calculado=0.0;
		//float iva_aux = 0.0;
		/*SELECT sum(importe_sin_iva+iva1) AS 'importe_sin_iva+iva1', SUM(	(((ROUND(importe_sin_iva,2) + ROUND(iva1,2))/113)*100)
		+ (((ROUND(importe_sin_iva,2) + ROUND(iva1,2))/113)*13) )  AS 'tOTAL' FROM eventos where nro_secuencia = 22 or nro_secuencia = 20 or nro_secuencia = 24 or nro_secuencia = 21*/

		memset( cadena_debug, 0, strlen( cadena_debug ) );
		_snprintf( cadena_debug, 80, " ##Entra total:%fl, importe_sin_iva:%fl iva1:%fl porc_iva: %fl",
					 *total,  *importe_sin_iva,  *iva1,  iva);
                                cadena_debug[79] = '\0';
                                GRABAR_LOG_SISTEMA( cadena_debug ,LOG_DEBUG,4);
		
		total_calculado = ROUND(*importe_sin_iva , ENTEROS, DECIMALES )+  ROUND(*iva1 , ENTEROS, DECIMALES );

		*importe_sin_iva = ((total_calculado/(100+iva))*100);//13
		*iva1 = ((total_calculado/(100+iva))*iva);

		*total =  *importe_sin_iva + *iva1 + *exento ;

		memset( cadena_debug, 0, strlen( cadena_debug ) );
		_snprintf( cadena_debug, 80, "##Sale total:%fl, importe_sin_iva:%fl iva1:%fl porc_iva: %fl",
					*total, *importe_sin_iva, *iva1 ,iva);
                                cadena_debug[79] = '\0';
                                GRABAR_LOG_SISTEMA( cadena_debug ,LOG_DEBUG,4);
		

	}
}


/*****************************************************************************/
void ACTUALIZA_ACU()
/*****************************************************************************/
{
    int h;
    ADD_MEMORY_DOUBLE( __x_ofertas_importe, RAM_AHORRO_OBTENIDO );  
    ACUMULA_DESCUENTOS();
    if( EMITIR_FACTURA() || EMITIR_FACTURA_REMOTA() || RAM_TICKET_FACTURA ) {
        ACUMULA_FACTURAS();
    }
    else {
        ACTUALIZA_ACUMULADORES_IVA( 1 );
        ACTUALIZA_CONSUM_FINAL_TKT();
    }
    if( RAM_MODO_DEVOLUCION ) {
        ADD_MEMORY_DOUBLE( __x_devoluciones_importe, RAM_TOTAL );
        ADD_MEMORY_INT( __x_devoluciones_cantidad, 1 );
    }
    //if( config.pais == ELSALVADOR ) {
        if( RAM_NOTA_CR || RAM_MODO_DEVOLUCION ) {
            for( h = 0;h < RAM_P_PAGO;h++ ) {
                if( TIPO_PERFIL() == _TIPO_PERFIL_EMPLEADO ) {
                    _ADD_MEMORY_DOUBLE( __x_devoluciones_medio, 10,
                                        _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) );
                }
                else {
                    _ADD_MEMORY_DOUBLE( __x_devoluciones_medio, ( int )_PAGO_MODO( h ),
                                        _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) );
                }
                if( _PAGO_VUELTO_MEDIO( h ) ) {
                    _ADD_MEMORY_DOUBLE( __x_devoluciones_medio,
                                        ( int )( _PAGO_COD_MEDIO_VUELTO( h ) - 1 ),
                                        -_PAGO_VUELTO_MEDIO( h ) );
                }
            }
        }
    //}
    }

/*****************************************************************************/
void _ACUMULA_DEPARTAMENTO( int depto, double importe, double cantidad )
/*****************************************************************************/
{
    if( config.pais != COLOMBIA && config.pais != ECUADOR ) {
        if( depto > 100 ) {
            depto = 99;
        }
        _ADD_MEMORY_DOUBLE( __x_dep_imp, depto, importe );
        _ADD_MEMORY_DOUBLE( __x_dep_can, depto, cantidad );
    }
}
#ifdef COMPILAR_MOZO
/*****************************************************************************/
void ACUMULA_MOZOS( int mozo )
/*****************************************************************************/
{
    _ADD_MEMORY_DOUBLE( __x_mozos_importe, mozo, PAGOS_TOTAL() );
    _ADD_MEMORY_DOUBLE( __x_mozos_cantidad, mozo, 1 );
}
#endif
/*****************************************************************************/
void ACTUALIZA_IVAS_MULTIPLES()
/*****************************************************************************/
{
    int t,cod_iva;
    int _ptr,_handle,_t;
    OPEN_RECUPERO( &_ptr, &_handle );
    //_t = STACK_INS( sizeof( int ) );
    _t = STACK_INS( sizeof( short ) );
    if( !( EMITIR_FACTURA() || EMITIR_FACTURA_REMOTA() ) ) {
        for( ;GET_MEMORY_INT( _t ) < RAM_P_TICKET;ADD_MEMORY_INT( _t, 1 ) ) {
            t = GET_MEMORY_INT( _t );
            cod_iva = _TICKET_COD_IVA( t );
            _ADD_MEMORY_DOUBLE( __x_importe_iva, cod_iva,
                                _TICKET_IVA1_PRECIO( t ) * _TICKET_CANTIDAD( t )
                              * ( ( _TICKET_TIPO( t ) & _RESTA ) ? -1 : 1 ) );
            _ADD_MEMORY_DOUBLE( __x_importe_sobretasa_iva, cod_iva,
                                _TICKET_IVA2_PRECIO( t ) * _TICKET_CANTIDAD( t )
                              * ( ( _TICKET_TIPO( t ) & _RESTA ) ? -1 : 1 ) );
        }
    }
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
void ACTUALIZA_BASE_DTO_EVENTO()
/*****************************************************************************/
{
    long id_evento;
    int i= 0,cant_reg = 0;
    double *base_total = NULL;
	int tabla_ant, tipo_ant;
	char where1[35];
	
	memset(where1,0,sizeof(where1));

    base_total = (double *)calloc( _LARGO_TICKET, sizeof( double ) );
    if( base_total ) {
        ACUMULAR_BASE_DTO_EVENTO( base_total );
        //Busco el contenido del evento
        id_evento = RAM_ID_EVENTO;
	    tabla_ant = SELECTED2();
    	tipo_ant = SELECTED_TIPO();    
		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		_snprintf( where1, sizeof(where1)-1, "ID_EVENTO = '%ld'", id_evento );
		cant_reg = RECCOUNT2( NO, where1 );
		while( fabs(base_total[i])< 0.0001 && cant_reg > i )//para evitar consultar por los importes 0, posicion directamente
				++i;
		if( cant_reg > 0 && cant_reg >= i ) // existe al menos 1
			do{
				SET_WHERE("ID_EVENTO = '%ld' and POSICION = '%i' ", id_evento, i + 1 );
				SET_ORDER2("ID_EVENTO, POSICION");
				RUN_QUERY(NO);
				if( FOUND2( )&& event_e_ticket.origen != _ORIGEN_PROMOCION  && fabs(base_total[i])>= 0.0001) {
						event_e_ticket.descuento = ROUND( base_total[i], ENTEROS, DECIMALES );
						#ifdef INVEL_L
						//BEGIN_TRANSACTION();
						#endif
						UPDATE2();
						#ifdef INVEL_L
						//END_TRANSACTION();
						#endif
				}
	//			while( fabs(base_total[i])< 0.0001 && cant_reg > 0 && cant_reg >= i )//para evitar consultar por los importes 0
				++i;
			}while( !dbEOF() && cant_reg > i );


        //SELECT( area );
        SELECT_TABLE( tabla_ant, tipo_ant );
    }
    if( base_total ) {
        FREEMEM( ( char* )base_total );
    }
}
/*****************************************************************************/
void ACUMULAR_BASE_DTO_EVENTO( double *base_total )
/*****************************************************************************/
{
    double *base_aux = NULL;
    int i,ok,h;
    base_aux = (double *) calloc( _LARGO_TICKET, sizeof( double ) );
    if( base_aux ) {
        memset( base_total, 0, sizeof( double ) * _LARGO_TICKET );
        for( h = 0;h < 5;h++ ) {
            //Vemos si tenemos descuentos aplicados
            ok = CARGA_BASE_DESCUENTO( h, base_aux );
            if( ok ) {
                for( i = 0;i < RAM_P_TICKET;i++ ) {
                    base_total[i] += base_aux[i];
                }
            }
        }
        /*---Cargamos los descuentos % libre ---*/
        ok = CARGA_BASE_DESCUENTO( 5, base_aux );
        if( ok ) {
            for( i = 0;i < RAM_P_TICKET;i++ ) {
                base_total[i] += base_aux[i];
            }
        }
        /*---Cargamos los descuentos pesos ---*/
        ok = CARGA_BASE_DESCUENTO( 6, base_aux );
        if( ok ) {
            for( i = 0;i < RAM_P_TICKET;i++ ) {
                base_total[i] += base_aux[i];
            }
        }
        /*---Cargamos los descuentos pesos ultimo articulo ---*/
        ok = CARGA_BASE_DESCUENTO( 7, base_aux );
        if( ok ) {
            for( i = 0;i < RAM_P_TICKET;i++ ) {
                base_total[i] += base_aux[i];
            }
        }
        /*---Cargamos los descuentos de las promociones---*/
        ok = CARGA_BASE_DESCUENTO( 99, base_aux );
        if( ok ) {
            for( i = 0;i < RAM_P_TICKET;i++ ) {
                base_total[i] += base_aux[i];
            }
        }
        /*---Cargamos los descuentos las importaciones---*/
        ok = CARGA_BASE_DESCUENTO( 98, base_aux );
        if( ok ) {
            for( i = 0;i < RAM_P_TICKET;i++ ) {
                base_total[i] += base_aux[i];
            }
        }
        FREEMEM( ( char* )base_aux );
    }
}
/*****************************************************************************/
void ACTUALIZA_DESCUENTOS_BOLETIN_ARTICULOS()
/*****************************************************************************/
{
    int i,  consolido = 0, p_art_act, error = 0, j, distribuir;
    struct  _art *art_act = NULL;
    for( i = 0;i < RAM_P_TICKET && !error ;i++ ) {
        if( _TICKET_ORIGEN( i ) == _ORIGEN_BOLETIN_D_ADIC && _TICKET_PRECIO( i ) < 0.00
         && !( _TICKET_TIPO( i ) & _RESTA ) ) {
            for( j = i + 1, distribuir = 1;j < RAM_P_TICKET && distribuir ;j++ ) {
                if( _TICKET_ORIGEN( j ) == _ORIGEN_BOLETIN_D_ADIC
                 && fabs( _TICKET_PRECIO( i ) - _TICKET_PRECIO( j ) ) < 0.01
                 && _TICKET_TIPO( j ) & _RESTA ) {
                    distribuir = 0;
                }
            }
            if( distribuir ) {
                if( !consolido ) {
                    consolido = 1;
                    p_art_act = RAM_P_TICKET;
                    art_act = (struct _art*)calloc( p_art_act, sizeof( struct _art ) );
                    if( art_act ) {
                        CONSOLIDAR_E_TICKET( RAM_ID_EVENTO, /*AREA_E_TICKET*/ T_EV_CONT, tipo_t_ev_cont, 
                                             &event_e_ticket, art_act, &p_art_act );
                    }
                    else {
                        error = 1;
                    }
                }
                if( !error ) {
                    PRORRATEA_DESCUENTO_BOLETIN_ARTICULO( i, art_act, p_art_act );
                }
            }
        }
    }
    if( art_act ) {
        free( art_act );
    }
}
/*****************************************************************************/
void PRORRATEA_DESCUENTO_BOLETIN_ARTICULO( int pos, struct _art *art, int cant )
/*****************************************************************************/
{
    int i,/*area_ant,*/ultimo;
    double base,porc,monto,total;
    //area_ant = SELECTED();
    int tabla_ant, tipo_ant;

    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    for( i = 0, base = 0.0;i < cant;i++ ) {
        if( art[i].origen == 1 ) {
            base += art[i].total;
            ultimo = i;
        }
    }
    porc = fabs( _TICKET_PRECIO( pos ) ) / base;
    for( i = 0, total = 0.0;i < cant;i++ ) {
        if( art[i].origen == 1 && fabs( art[i].total ) > 0.005 ) {
            monto = ROUND( art[i].total * porc, ENTEROS, DECIMALES );
            if( i == ultimo ) {
                monto = fabs( _TICKET_PRECIO( pos ) ) - total;
            }
            total += monto;
            memset( event_despro, 0, sizeof( struct _ev_despro ) );
            //SELECT( AREA_E_DES_PRO );
            SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
            event_despro->status = 0;
            event_despro->id_evento = RAM_ID_EVENTO;
            event_despro->posicion = art[i].posicion;
            event_despro->tipo_motivo = MOTIVO_DESC_BOLETIN_ART;
            event_despro->id_motivo = pos + 1;
            event_despro->importe = monto;
            strcpy( event_despro->codigo_barra, _TICKET_COD_BARRA( art[i].posicion - 1 ) );
            event_despro->cod_articulo = _TICKET_COD_ESPECIAL( art[i].posicion - 1 );
            event_despro->cod_sucursal = config.sucursal;
            event_despro->fecha_ticket = NRO_FECHA_OPERACION;
            event_despro->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
            event_despro->caja = NRO_CAJA;
            event_despro->nro_z = NRO_Z;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2( NO );
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
        }
    }
    //SELECT( area_ant );
    SELECT_TABLE( tabla_ant, tipo_ant );
}
/*****************************************************************************/
void ACUMULA_CONSUM_FINAL_FACT( void )
/*****************************************************************************/
{
    int i = 0;
    double tot_imp = 0.0, total_gral=0.0, total_a_ctrl = 0.0, iva_a_ctrl = 0.0,total_grav_a_ctrl = 0.0,extras_a_ctrl=0.0;

	total_gral = PAGOS_TOTAL(0);
	total_a_ctrl = total_gral;
	iva_a_ctrl =  TOT_TICK_IVA1 ;
	total_grav_a_ctrl = TOT_TICK_IMPORTE_SIN_IVA;
	extras_a_ctrl = ( total_a_ctrl - ( TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1) );
	
	EQUILIBRA_IMPORTES_TOTALES(&total_a_ctrl, &total_grav_a_ctrl, &iva_a_ctrl, &extras_a_ctrl, iva[0].porc_iva1  );
    
	for( i = 0;i < 4;i++ ) {
        tot_imp += _TOT_TICK_IMPUESTOS( i );
    }

    if( RAM_NOTA_CR || RAM_MODO_DEVOLUCION) {
        SET_MEMORY_DOUBLE( __x_dev_cons_f_importe_exent_fact,
                           X_DEV_CONS_F_IMPORTE_EXENT_FACT + TOT_TICK_EXENTO );
        SET_MEMORY_DOUBLE( __x_dev_cons_f_importe_grav_fact,
                           X_DEV_CONS_F_IMPORTE_GRAV_FACT + /*TOT_TICK_IMPORTE_SIN_IVA*/total_grav_a_ctrl
                         - TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA );
        SET_MEMORY_DOUBLE( __x_iva_devol_consumidor_final,
                           X_IVA_DEVOL_CONSUMIDOR_FINAL
                         + ( /*TOT_TICK_IVA1*/iva_a_ctrl + TOT_TICK_IVA2 + tot_imp ) );
        ADD_MEMORY_INT( __x_dev_cons_f_cantidad_fact, 1 );
    }
    else {
        SET_MEMORY_DOUBLE( __x_cons_final_importe_exent_fact,
                           X_CONS_FINAL_IMPORTE_EXENT_FACT + TOT_TICK_EXENTO );
        SET_MEMORY_DOUBLE( __x_cons_final_importe_grav_fact,
                           X_CONS_FINAL_IMPORTE_GRAV_FACT + total_grav_a_ctrl/*TOT_TICK_IMPORTE_SIN_IVA*/
                         - TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA );
        SET_MEMORY_DOUBLE( __x_iva_cons_final,
                           X_IVA_CONS_FINAL + ( iva_a_ctrl/*TOT_TICK_IVA1*/ + TOT_TICK_IVA2 + tot_imp ) );
        ADD_MEMORY_INT( __x_cons_final_cantidad_fact, 1 );
    }
}
/*****************************************************************************/
void ACTUALIZA_CONSUM_FINAL_TKT( void )
/*****************************************************************************/
{
    int i = 0;
    double tot_imp = 0.0, /*total_gral=0.0,*/ total_a_ctrl = 0.0, iva_a_ctrl = 0.0,total_grav_a_ctrl = 0.0,extras_a_ctrl=0.0;
	total_a_ctrl = PAGOS_TOTAL( 0 );

    for( i = 0;i < 4;i++ ) {
        tot_imp += _TOT_TICK_IMPUESTOS( i );
    }
    if( MODO_NEGOCIO != SERVICIOS ) {
        //ver si falta preguntar por exportacion o diplom
			iva_a_ctrl =  TOT_TICK_IVA1 ;
			total_grav_a_ctrl = TOT_TICK_IMPORTE_SIN_IVA;
			extras_a_ctrl = ( total_a_ctrl - ( TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1) );
			EQUILIBRA_IMPORTES_TOTALES( &total_a_ctrl, &total_grav_a_ctrl, &iva_a_ctrl, &extras_a_ctrl, iva[0].porc_iva1  );

        if( RAM_MODO_DEVOLUCION ) {
			
			
			//TOT_TICK_EXENTO + TOT_TICK_IMPORTE_SIN_IVA - TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA+( TOT_TICK_IVA1 + TOT_TICK_IVA2 + tot_imp );
            SET_MEMORY_DOUBLE( __x_dev_cons_final_importe_exent_tkt,
                               X_DEV_CONS_FINAL_IMPORTE_EXENT_TKT + TOT_TICK_EXENTO );
            
			SET_MEMORY_DOUBLE( __x_dev_cons_f_importe_grav_tkt,
                               X_DEV_CONS_F_IMPORTE_GRAV_TKT + ( total_grav_a_ctrl /*TOT_TICK_IMPORTE_SIN_IVA*/
                             - TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA ));
            
			SET_MEMORY_DOUBLE( __x_iva_devol_consumidor_final,
                               X_IVA_DEVOL_CONSUMIDOR_FINAL
                             + ( iva_a_ctrl/*TOT_TICK_IVA1*/ + TOT_TICK_IVA2 + tot_imp ) );
            
			ADD_MEMORY_INT( __x_dev_cons_f_cantidad_tkt, 1 );
        }
        else {

			SET_MEMORY_DOUBLE( __x_cons_final_importe_exent_tkt,
                               X_CONS_FINAL_IMPORTE_EXENT_TKT + TOT_TICK_EXENTO );

            SET_MEMORY_DOUBLE( __x_cons_final_importe_grav_tkt,
                               X_CONS_FINAL_IMPORTE_GRAV_TKT + ( total_grav_a_ctrl/*TOT_TICK_IMPORTE_SIN_IVA*/
                             - TOT_TICK_IMP_INT + TOT_TICK_RECARGO_TARJETA ));

            SET_MEMORY_DOUBLE( __x_iva_cons_final,
                               X_IVA_CONS_FINAL + ( iva_a_ctrl/*TOT_TICK_IVA1 */+ TOT_TICK_IVA2 + tot_imp ) );

            ADD_MEMORY_INT( __x_cons_final_cantidad_tkt, 1 );
        }
    }
}
/*****************************************************************************/
void ACUMULA_DESCUENTOS( void )
/*****************************************************************************/
{
    int h, tabla_ant, tipo_ant;
    double total_descuento,total,vuelto_efect;
	long id_evento;
    
	total = 0.00;
	total_descuento = 0.00;
    id_evento = RAM_ID_EVENTO;
    tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_E_TICKET );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	SET_WHERE("");
	RUN_QUERY(NO);
    //GET_EQUAL( ( char* )&id_evento );
   // if( FOUND2() ) {
            while( event_e_ticket.id_evento == id_evento && !BtrvEOF() ) {
                if( event_e_ticket.origen == _ORIGEN_PROMOCION ) {
					total_descuento += event_e_ticket.importe_sin_iva + event_e_ticket.iva1;
				}
				SKIP2( 1 );
			}
	//}
    //total_descuento = RAM_IMP_DESCUENTO + RAM_AHORRO_OBTENIDO;

    for( h = 0;h < RAM_P_PAGO;h++ ) {
        total = total + ( _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h ) - _PAGO_VUELTO_MEDIO( h ) );
    }
    total -= RAM_VUELTO_EFECTIVO;
	if(total != 0){ //controlamos que total no sea 0 ya que se haria una divicion por cero.
		EXISTE_CLIENTE( RAM_NRO_CLIENTE );
		if( TIPO_PERFIL() == _TIPO_PERFIL_EMPLEADO && config.pais == ELSALVADOR ) {
			for( h = 0;h < RAM_P_PAGO;h++ ) {
				vuelto_efect = ( medios[_PAGO_MODO( h )].efectivo ) ? RAM_VUELTO_EFECTIVO : 0;
				_ADD_MEMORY_DOUBLE( __x_descuentos_medio, 10,
									total_descuento * ( _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h )
													- _PAGO_VUELTO_MEDIO( h ) - vuelto_efect )
								/ total );
			}
		}
		else {
			for( h = 0;h < RAM_P_PAGO;h++ ) {
				vuelto_efect = ( medios[_PAGO_MODO( h )].efectivo ) ? RAM_VUELTO_EFECTIVO : 0;
				_ADD_MEMORY_DOUBLE( __x_descuentos_medio, ( int )_PAGO_MODO( h ),
									total_descuento * ( _PAGO_IMPORTE( h ) - _PAGO_VUELTO( h )
													- _PAGO_VUELTO_MEDIO( h ) - vuelto_efect )
								/ total );
			}
		}
	}
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/*****************************************************************************/
void CARGA_IMPORTADOS( void )
/*****************************************************************************/
{
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;
    /*int area_ant = SELECTED(); */
    int tabla_ant, tipo_ant;
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
    if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
        &ev_cod_barra, &ev_b_promo ) ) {
        //SELECT( AREA_AUX8 );
        SELECT_TABLE( T_EVP_TMP, TT_ORIG );
        SET_ORDER2("ID_EVENTO");
        SET_WHERE("");
        RUN_QUERY(NO);
        while( RECCOUNT2(NO,NULL) > 0 && /*!BtrvEOF()*/ !dbEOF()) {
            memset( event_dpago, 0, sizeof( struct _ev_dmedios ) );
            memcpy( event_dpago, &ev_dpago, sizeof( struct _ev_dmedios ) );
            event_dpago->caja = event_ticket.caja;
            event_dpago->nro_z = event_ticket.nro_z;
            event_dpago->id_evento = RAM_ID_EVENTO;
            event_dpago->caja_z = event_ticket.caja_z;
           if( !NRO_FECHA_OPERACION )
			  SET_MEMORY_INT( __nro_fecha_operacion, event_ticket.fecha );

            event_dpago->fecha_ticket = NRO_FECHA_OPERACION;//GET_FECHA_DOS();
            //SELECT( AREA_DPAGO );
            SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2( NO );
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            /*Variable __nro_mozo utilizada para la posicion del dato adicional en un mismo evento*/
            ADD_MEMORY_INT( __nro_mozo, 1 );
            //SELECT( AREA_AUX8 );
            SELECT_TABLE( T_EVP_TMP, TT_ORIG );
            //SKIP( 1 );
            SKIP2( 1 );
        }
        CERRAR_EVENTOS_TEMPORALES();
    }
    //SELECT( area_ant );
    SELECT_TABLE( tabla_ant, tipo_ant );
}
void GRABAR_EVENTOS_PROMOCIONES(void)
{
    struct eventos_promociones motor_event_promo;
    int grabar = 0/*, area_ant = 0*/;
    int cnt = 0, cant = 0;
    //area_ant = SELECTED();
	int tabla_ant, tipo_ant;
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();    
    //SELECT(AREA_EV_B_PROMO);
    SELECT_TABLE( T_EV_B_PROMO, tipo_t_ev_b_promo );
    if((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_EVENT_PROMO,sizeof(struct eventos_promociones))) > 0){
        while(cnt < cant){
            memset(&motor_event_promo,0,sizeof( struct eventos_promociones ));
            memset(&event_b_promo,0,sizeof( struct _ev_bpromo ));
            RECUPERAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO,(char *)&motor_event_promo,sizeof(struct eventos_promociones),cnt);
            if(motor_event_promo.procesado == 0){
                grabar = 1;
                /*****Campos comunes a todas las promociones********/
                event_b_promo.sucursal = config.sucursal;
                event_b_promo.id_evento = RAM_ID_ULTIMO_EVENTO;
                event_b_promo.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
                event_b_promo.nro_z = NRO_Z;
                //event_b_promo.fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
                if( !NRO_FECHA_OPERACION )
            	   SET_MEMORY_INT( __nro_fecha_operacion, event_ticket.fecha );
                event_b_promo.fecha_ticket = NRO_FECHA_OPERACION; 
                event_b_promo.cod_promo = motor_event_promo.cod_promo;
                event_b_promo.posicion = cnt;
                event_b_promo.b_promo = motor_event_promo.b_promo;
                event_b_promo.id_grupo = motor_event_promo.id_grupo;
				memcpy(event_b_promo.cod_ext, motor_event_promo.cod_promo_ext, 25);
                event_b_promo.aplicado = 'S';
				event_b_promo.puede_revertir = 'N';//Se setea el valor para que no grabe basura. Cuando se implemente se vera el valor correcto
                event_b_promo.cantidad = (float) motor_event_promo.cantidad;
				/*---------Cobros Promo------------*/
				{	
					int pos;
					for( pos = 0 ; pos < RAM_P_TICKET 
						&& strncmp( _TICKET_COD_BARRA( pos ), motor_event_promo.cod_barr, sizeof( motor_event_promo.cod_barr ) )
						&& _TICKET_COD_ESPECIAL( pos ) != motor_event_promo.cod_arti
						; pos++ ){}
					if ( pos != RAM_P_TICKET )
						event_b_promo.tipo = (unsigned char)_TICKET_TIPO2( pos );
				}
				/*--------------------------------*/
                switch( motor_event_promo.b_promo ){
                    case BENEF_ARTICULOS:
                    case BENEF_DEPARTAMENTO:
                        event_b_promo.cod_arti = motor_event_promo.cod_arti;
                        if(strlen(motor_event_promo.cod_barr) > 0){ //si el motor pasa el codigo de barra
                            strcpy(event_b_promo.cod_barra, motor_event_promo.cod_barr);
                        }else{// si el motor no pasa el codigo de barra, lo busca en la tabla articulos
                            if(BUSCA_CODIGO_ESPECIAL( motor_event_promo.cod_arti )){
                                strcpy(event_b_promo.cod_barra, articulo.cod_barra);
                            }else{
                                char msg[150];
                                _snprintf( msg, sizeof(msg)-1,"E_B_PROMO, No se encuentra el articulo (cod_interno = %ld) utilizado en promocion",
                                    motor_event_promo.cod_arti );
                                GRABAR_LOG_SISTEMA( msg ,LOG_ERRORES,3);
                                grabar = 0;
                            }
                        }
                        break;
                    case BENEF_ACTIVACION_MEDIO:
                    case BENEF_MEDIO:
                        {
                            int cnt = 0;
                            event_b_promo.cod_medio = (char)motor_event_promo.cod_medio;
                            event_b_promo.cod_submedio = (char)motor_event_promo.cod_submedio;
                            event_b_promo.modo_plan_pago = (char)motor_event_promo.modo_plan_pago;
                            /*busca medio, submedio, plan para obtener el numero de tarjeta, solo para tarjeta*/
							/*while( (medios[motor_event_promo.cod_medio - 1].tarjeta && cnt < 10) && 
                             *   (_PAGO_COD_SUBMEDIO(cnt) != motor_event_promo.cod_submedio ||
                             *   _PAGO_MODO(cnt) != motor_event_promo.cod_medio - 1 || 
                             *   _PAGO_CUENTA(cnt) != motor_event_promo.modo_plan_pago)){
                             *   cnt++;
                            }*/
							while( ( ( medios[motor_event_promo.cod_medio - 1].tarjeta && cnt < 10 ) 
								&& ( _PAGO_COD_SUBMEDIO(cnt) != motor_event_promo.cod_submedio 
								|| _PAGO_MODO(cnt) != motor_event_promo.cod_medio - 1 
								|| _PAGO_CUENTA(cnt) != motor_event_promo.modo_plan_pago ) ) 
								||
								( ( medios[motor_event_promo.cod_medio - 1].mutual 
								|| medios[motor_event_promo.cod_medio - 1].ticket_compra )
								&& ( cnt < 10 )
								&& ( _PAGO_COD_SUBMEDIO(cnt) != motor_event_promo.cod_submedio 
								|| _PAGO_MODO(cnt) != motor_event_promo.cod_medio - 1 ) )
								) {
                                cnt++;
                            }
                            if(cnt < 10 && medios[motor_event_promo.cod_medio - 1].tarjeta){
                                strcpy(event_b_promo.nro_tarjeta_string,_PAGO_NRO_TARJETA(cnt));
                            }else{
								if( cnt < 10 && ( medios[motor_event_promo.cod_medio - 1].mutual || medios[motor_event_promo.cod_medio - 1].ticket_compra )) {
									//se debe grabar el campo aplicado en 'S', para éstos medios.
									event_b_promo.aplicado = 'S';
								} else {
									event_b_promo.aplicado = 'N';
									//grabar = 0;
								}
							}								
                        }
                        break;
                    case BENEF_INCONDICIONAL:
                        /*POR AHORA NO SE AGREGA NADA PARA ESTE BENEFICIO*/
                        break;
                    case BENEF_DESCUENTO:
                        event_b_promo.porc_desc = motor_event_promo.porc_desc;
                        //event_b_promo.precio_fijo = ROUND(motor_event_promo.precio_fijo,ENTEROS, DECIMALES );
                        event_b_promo.precio_fijo = motor_event_promo.precio_fijo;
                        break;
                    case BENEF_BONOS:
                        event_b_promo.cantidad = (float) motor_event_promo.cantidad;
                        event_b_promo.precio_fijo = ROUND(motor_event_promo.precio_fijo,ENTEROS, DECIMALES );
                        event_b_promo.tipo = motor_event_promo.tipo_bono;
                        break;
                    case BENEF_PUNTOS:
                        //event_b_promo.tot_puntos = 0; //no se usa por ahora
                        //event_b_promo.nro_tarjeta_string = 0; //no se usa por ahora
                        break;
                    case BENEF_MONEDERO:
                        event_b_promo.precio_fijo = ROUND(motor_event_promo.precio_fijo,ENTEROS, DECIMALES );
                        strcpy(event_b_promo.nro_tarjeta_string,RAM_CUENTA_CLIENTE);
                        break;
                    case BENEF_REGALO:
                        event_b_promo.porc_desc = motor_event_promo.porc_desc;
                        event_b_promo.precio_fijo = motor_event_promo.precio_fijo;
                        break;
                    case BENEF_MONEDERO_NO_ASIG:
                        //indica que no se asigno a monedero, porque el cajero no quiso
                        event_b_promo.b_promo = BENEF_MONEDERO;
                        event_b_promo.aplicado = 'N';
                        break;
                }
                if(grabar){
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    INSERT2( NO );
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
                /*lo marca como procesado*/
                motor_event_promo.procesado = 1;
                ACTUALIZAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&motor_event_promo,sizeof(struct eventos_promociones),cnt);
            }
            cnt++; //siguiente evento guardado en el archivo rec por el motor de promociones
        }
        BORRAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO);
    }
    //SELECT( area_ant );
    SELECT_TABLE( tabla_ant, tipo_ant );
}
/****************************************************************************************/
void GRABAR_B_PROMO(struct _ev_bpromo promocionados)
/****************************************************************************************/
{
    int grabar = 1, /*area_ant = 0,*/ tabla_ant, tipo_ant, posicion=0;

	//area_ant = SELECTED();
    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();    
    //SELECT(AREA_EV_B_PROMO);
    SELECT_TABLE( T_EV_B_PROMO, tipo_t_ev_b_promo );
	//SET_ORDER( 2 );
    SET_ORDER2("ID_EVENTO, CAJA_Z");
    SET_WHERE("");
    RUN_QUERY(NO);
	//GO(BOTTOM);
	GO2( BOTTOM );
    {
		{
			if( event_b_promo.id_evento == RAM_ID_EVENTO && event_b_promo.cod_promo == promocionados.cod_promo ) {
				posicion = event_b_promo.posicion + 1;
			}
			else {
				posicion = 1;
			}
            memset(&event_b_promo,0,sizeof( struct _ev_bpromo ));
		    memcpy( &event_b_promo, &promocionados,sizeof( struct _ev_bpromo ));
			{
            /*****Campos comunes a todas las promociones********/
            event_b_promo.sucursal = config.sucursal;
			event_b_promo.id_evento = RAM_ID_EVENTO;
            event_b_promo.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
            event_b_promo.nro_z = NRO_Z;
            //event_b_promo.fecha_ticket = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
            event_b_promo.fecha_ticket =  GET_FECHA_DOS();
			event_b_promo.posicion = posicion;
            /*--------------------------------*/
			  if(grabar){
                    INSERT2( NO );
                }
            }
        }
    }
    //SELECT( area_ant );
    SELECT_TABLE( tabla_ant, tipo_ant );
}

/*****************************************************************************/
char CALCULAR_TIPO_EVENTO( )
/*****************************************************************************/
{
    char tipo_evento = 0;
	if( EMITIR_FACTURA_REMOTA(  ) ) {
		if( RAM_NOTA_CR ) {
			tipo_evento = ( clientes.cond_iva < 3 ) ? 19 : 20;
		} else {
			//tipo_evento = ( clientes.cond_iva < 3 ) ? 16 : 17;
			if(( FACT_INDEP_DEL_TIPO_DE_CLIENTE && RAM_FACTURA == FACTURA_TIPO_A )
				|| ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva < 3 ))
				tipo_evento = 16;
			else
				tipo_evento = 17;
		}
	} else if( EMITIR_FACTURA(  ) ) {
		if( RAM_NOTA_CR ) {							//A : B
			if(( FACT_INDEP_DEL_TIPO_DE_CLIENTE && RAM_NOTA_CR == FACTURA_TIPO_A )
				|| ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva < 3 ))
				tipo_evento = 5 ;
			else
				tipo_evento = 6;
		} else {
			//tipo_evento = (( clientes.cond_iva < 3 ) ? 3 : 4);
			if(( FACT_INDEP_DEL_TIPO_DE_CLIENTE && RAM_FACTURA == FACTURA_TIPO_A )
				|| ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva < 3 ))
				tipo_evento = 3;
			else
				tipo_evento = 4;
		}
	} else if( EMITIR_REMITO_REMOTO(  ) ) {
		tipo_evento = 18;
	} else if( EMITIR_REMITO(  ) ) {
		tipo_evento = 11;
	} else if( RAM_TICKET_FACTURA && !RAM_MODO_DEVOLUCION ) {
        if( RAM_TICKET_FACTURA == 1 ) {
			tipo_evento = 35;
        }
        else {
			tipo_evento = 36;
        }
    }else if( RAM_MODO_DEVOLUCION ) {
        tipo_evento = 21;
	} else {
		tipo_evento = 0;
	}
	return tipo_evento;
}
/*****************************************************************************************/
double RECUPERA_IMPORTE_PROMOCION( unsigned long cod_articulo, double cant_articulo,char *cod_barra, double precio_articulo ){
/*****************************************************************************************/
   return (0.00);
}
/*Por lo hablado con JC Murgui no se devuelve el prorratero de las NC parciales con promo.
*/
/*	struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo evento_b_promo;

	int error = 0;
	double importe = 0;
	double total_ticket_articulo=0.0;
	double total_ticket=0.0;



	if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,&ev_cod_barra, &evento_b_promo )){
		//SELECT(AREA_AUX10)
		SELECT_TABLE( T_EVBP_TMP, TT_ORIG );
		SET_WHERE("ID_EVENTO='%ld' AND CAJA_Z='%ld'", ev_cont.id_evento, ev_cont.caja_z);
		SET_ORDER2("POSICION");
		RUN_QUERY(NO);
		if( RECCOUNT2( NO, NULL ) ){//hay descuentos

			GO2(TOP);
			error = 1;
			while( !dbEOF()  && error ){
				if( evento_b_promo.b_promo == BENEF_DESCUENTO )//HAY DESCUENTO
					error = 0;
				SKIP2(1);
			}
			//ver en el ticket la relacion del articulo
			if(!error){
				//SELECT(AREA_AUX);
				SELECT_TABLE( T_EVC_TMP, TT_ORIG );
				GO2( TOP );
				error=1;
				while(  !dbEOF() ){
					error = 0;

					if( cod_articulo > 0 ){
						if( ev_cont.cod_articulo == cod_articulo )
							total_ticket_articulo += ev_cont.cantidad * (ev_cont.importe_sin_iva+ev_cont.imp_int+ev_cont.iva1+ev_cont.iva2+ev_cont.exento);
					}else{
						if( strcmp( cod_barra, ev_cont.cod_barra ) == 0 )
							total_ticket_articulo += ev_cont.cantidad * (ev_cont.importe_sin_iva+ev_cont.imp_int+ev_cont.iva1+ev_cont.iva2+ev_cont.exento);
					}

					if( ev_cont.origen!= _ORIGEN_PROMOCION ){//TOTAL DEL TICKET
						total_ticket+=ev_cont.cantidad * (ev_cont.importe_sin_iva+ev_cont.imp_int+ev_cont.iva1+ev_cont.iva2+ev_cont.exento);
					}
				SKIP2(1);
				}


			}


			//SELECT(AREA_AUX10);
			SELECT_TABLE( T_EVBP_TMP, TT_ORIG );
			GO2(TOP);
			if(!error){ //hay un descuento a devolver al menos
				importe = DEVOLVER_PARCIALMENTE_DE_B_PROMO( cod_articulo, cant_articulo,
													cod_barra, precio_articulo,	&evento_b_promo, 
													 T_EVBP_TMP, TT_ORIG, total_ticket );//dnc x3
				//evento_b_promo.precio_fijo = importe * -1;
				//GRABAR_B_PROMO( evento_b_promo );
			}
		}
	}
	CERRAR_EVENTOS_TEMPORALES();

	return importe;
}
*/


/*****************************************************************************************/
double DEVOLVER_PARCIALMENTE_DE_B_PROMO(unsigned long cod_articulo,double cantidad_articulo,char *cod_barra,
										double precio_articulo,struct _ev_bpromo * evento_b_promo, 
										/*int area,*/ int tabla, int tipo, double total_descontado )//dnc x3
/*****************************************************************************************/
{/****/


	int NO_CARGADO = -1;
	int  /*area_ant = 0,*/un_grupo = 0, participa = NO,cargar_item = NO, por_grupo = NO, fin_grupo = NO, es_total=NO,beneficio=0,descargado=0;
	INT16  cod_promocion = NO_CARGADO, cod_grupo = NO_CARGADO,posicion = NO_CARGADO;
	double  cant_beneficio = 0.0, importe_beneficio = 0.0,total_item=0.0,total_grupo=0.0,total_grupo_acumulado=0.0,total_final=0.0, cantis =0.00;
    int tabla_ant, tipo_ant;

	if( cantidad_articulo == 0.0 && precio_articulo == 0.0 )
		return total_final;

	if(cantidad_articulo == 0.0)
		cantidad_articulo = 1.0;

	if( precio_articulo == 0.0 )
		precio_articulo =  1.0;

    tabla_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();
	//area_ant = SELECTED();
	//SELECT(area);
	SELECT_TABLE( tabla, tipo );
	RUN_QUERY(NO);
	GO2(TOP);
	while( /*!BtrvEOF( )*/ !dbEOF()  )
	{

		if( (cod_promocion == NO_CARGADO )|| (cod_grupo == NO_CARGADO )||
			( cod_promocion != evento_b_promo->cod_promo)){//IDENTIFICO EL GRUPO
				cod_promocion = evento_b_promo->cod_promo;
				cod_grupo = evento_b_promo->id_grupo;
				cant_beneficio = 0.0;
				participa = NO;
				fin_grupo = NO;
				por_grupo = NO;
				es_total = NO;
				cargar_item = NO;
				total_grupo_acumulado = 0.0;
				total_item = 0.0;

		}

		if( cod_promocion == evento_b_promo->cod_promo ){

			switch(evento_b_promo->b_promo){
				case BENEF_ARTICULOS:{
						por_grupo = SI;

						if( cod_articulo > 0 ){
							if( evento_b_promo->cod_arti == cod_articulo ){
								participa = SI;
								cargar_item = SI;
								beneficio = evento_b_promo->b_promo;
							}
						}else{
							if( strcmp( cod_barra,  evento_b_promo->cod_barra ) == 0 ){
								participa = SI;
								cargar_item = SI;
								beneficio = evento_b_promo->b_promo;
							}

						}
						if( por_grupo == SI ) { //en la devolucion tengo que ver la cantidad
						   cantis = evento_b_promo->cantidad;
						}

				}
				break;
				case BENEF_DESCUENTO:{
					if( evento_b_promo->cantidad == 0.00){
						cant_beneficio = 1;
					}
					beneficio = evento_b_promo->b_promo;
					total_grupo += cant_beneficio *evento_b_promo->precio_fijo;
					if( cargar_item == SI ){
						total_item += cant_beneficio *evento_b_promo->precio_fijo;
						cargar_item = NO;
					}
					//0.75
				}
				break;
				case BENEF_INCONDICIONAL:
				case BENEF_PERFIL:{
					if( evento_b_promo->cantidad == 0.00){
						cant_beneficio = 1;
					}
					es_total = SI;
					descargado=evento_b_promo->b_promo;
					total_grupo += cant_beneficio *evento_b_promo->precio_fijo;
					//contra todo el ticket
				}
				break;
			}
		}

//		SKIP2(1);
		//final de la promo!!!!
		if( /*BtrvEOF( )*/ !dbEOF() || cod_promocion != evento_b_promo->cod_promo ){

			//se terminaron las promos o cambia el codigo de promo

			if( participa == SI  ){
				// DENTRO DE UN GRUPO DETALLADO EN EV_b_PROMO
				double cantidad_original = 0.0, porcentaje =0.0;

				//porcentaje aplicado
				porcentaje =( total_item )/( precio_articulo );

				/***************** cantidad original del ticket***********************************/
				if( total_item > 0.00)
					cantidad_original = fabs((precio_articulo * porcentaje)/total_item);

				if( cantidad_original > cantidad_articulo )
					cantidad_original = cantidad_articulo;
				/********************************************************************************/

				fin_grupo = SI;
				if( fin_grupo == SI ){//participa el articulo por lo tanto calculo el total
					if( por_grupo == SI && cantis > 0 && cantidad_articulo < cantis )
					   total_item =  ROUND( total_item, ENTEROS, 2) / cantis * cantidad_articulo ;
					total_grupo_acumulado= ((total_item)/(fabs(precio_articulo)))*(cantidad_original*precio_articulo);
					total_grupo =0.0;
				}
			}else{
				// CONTRA TODO EL TICKET
				if( es_total == SI ) {
					if( evento_b_promo->b_promo == BENEF_DESCUENTO ) {
						double porcentaje =0.0;
						
						if( total_descontado )
							porcentaje = ( total_grupo * 100 ) / total_descontado;

						total_grupo_acumulado = ( ( ( cantidad_articulo * precio_articulo ) 
								* porcentaje ) / 100 );
					} else {
						total_grupo_acumulado =( ( total_grupo ) / ( total_descontado ) ) 
								* ( cantidad_articulo * precio_articulo );
					}
					//Se agrega esta condicion porque cuando se aplica la promo perfiles en la ev_bpromo
					//graba dos registros, el primero con b_promo = 4 y el segundo con b_promo = 101.
					//Cuando analiza b_promo = 4 ponia es_total = NO; de manera que cuando analizaba
					//b_promo = 101 no pasaba por esta parte de manera que total_grupo_acumulado = 0 y 
					//no se aplicaba la dev del descuento. Tarea 4525
					if( total_grupo /*&& evento_b_promo->b_promo == BENEF_PERFIL*/)
						es_total = NO;

					total_grupo =0.0;
				}
			}
			/**************************GRABADO PARCIAL******************************************/
			if( fabs( total_grupo_acumulado ) > 0.005 ){
				struct _ev_bpromo grabar_b_promo;
				participa = NO;
				memset(&grabar_b_promo,0,sizeof( struct _ev_bpromo ));
				grabar_b_promo.cod_arti = cod_articulo;
				strcpy(grabar_b_promo.cod_barra, cod_barra);
				grabar_b_promo.b_promo = descargado;

				//event_b_promo.aplicado = 'S'; //asi estaba en devolucion no lo grababa
				grabar_b_promo.aplicado = 'S';
				grabar_b_promo.cod_promo = cod_promocion;
				GRABAR_B_PROMO( grabar_b_promo);

				memset(&grabar_b_promo,0,sizeof( struct _ev_bpromo ));
				total_grupo_acumulado *= -1;
				grabar_b_promo.precio_fijo = total_grupo_acumulado;
				grabar_b_promo.b_promo = beneficio;
				event_b_promo.aplicado = 'S';
				grabar_b_promo.cod_promo = cod_promocion;
				GRABAR_B_PROMO( grabar_b_promo );

				if( total_grupo_acumulado!= 0.0 ){
               double totalll;

               totalll =  total_grupo_acumulado - ROUND( total_grupo_acumulado, 10, DECIMALES);

               if( fabs( totalll ) < 0.0049 && fabs(totalll) > 0.0045  ) { //parche contra el redondeo en x.xx5
					    total_grupo_acumulado = total_grupo_acumulado + 0.005 ;
					  }

					SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
					if( IMPRESORA_FISCAL == FISCAL && RAM_NOTA_CR > 1) {
						int corregir = CORRECCION_REDONDEO_SISTEMA( &total_grupo_acumulado );
						ROUND2(&total_grupo_acumulado, ENTEROS, 2);
					}
					SET_MEMORY_DOUBLE( __item_imp_int, 0 );
					RECALCULA_ITEM_POR_PRECIO( total_grupo_acumulado );
					AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, ((RAM_ANULACION)?SI:NO), NO, NO, _ORIGEN_PROMOCION );
					ADD_MEMORY_INT( __ram_p_ticket, 1 );
					ADD_MEMORY_INT( __ram_renglones, 1 );
					GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, NO, _ORIGEN_PROMOCION, 0, NO );
					SET_MEMORY_DOUBLE( __ram_total,
					ROUND( RAM_TOTAL + ITEM_PRECIO * RAM_CANTIDAD * ( RAM_ANULACION ? -1 : 1), 10, DECIMALES ) );
					if( DEBE_IMPRIMIR_ITEM() ) {
						IMPRIME_ITEM( RAM_P_TICKET - 1 ) ;
					}
					ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
					MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
					MUESTRA_BULTOS();
					MUESTRA_SUBTOTAL(SI);
        			MUESTRA_DATOS_CONTADOR_PANTALLA( RAM_P_TICKET - 1 , SI );
				}
				total_final+= total_grupo_acumulado;
				total_grupo_acumulado=0.0;
				//SELECT(area);
				SELECT_TABLE( tabla, tipo );

			}


		}
			SKIP2(1);
	}



/**************************GRABADO un solo articulo******************************************/
/*				if( fabs( total_final ) > 0.005 ){
			struct _ev_bpromo grabar_b_promo;
			participa = NO;
			memset(&grabar_b_promo,0,sizeof( struct _ev_bpromo ));
			grabar_b_promo.cod_arti = cod_articulo;
			grabar_b_promo.b_promo = descargado;
			GRABAR_B_PROMO( grabar_b_promo);

			memset(&grabar_b_promo,0,sizeof( struct _ev_bpromo ));
			total_final *= -1;
			grabar_b_promo.precio_fijo = total_final ;
			grabar_b_promo.b_promo = beneficio;
			GRABAR_B_PROMO( grabar_b_promo );
			if( total_final!= 0.0 ){
					SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
					RECALCULA_ITEM_POR_PRECIO( total_final );
					CR1_AGREGA_ITEM( NO, _ORIGEN_PROMOCION, 0, 0 );
					MUESTRA_SUBTOTAL();
			}
		}*/
	//SELECT(area_ant);
	SELECT_TABLE( tabla_ant, tipo_ant );
	MUESTRA_SUBTOTAL(NO);
	return total_final;
}/***/
/************************************************************************/
	int ARTICULO_BENEFICIADO( char tipo ){//filtro para algunas tipos de promos
/************************************************************************/
	int rta = NO;
	switch( tipo ){
		case BENEF_ARTICULOS :
			rta = BENEF_ARTICULOS;
			break;
		case BENEF_DEPARTAMENTO:
			rta = BENEF_DEPARTAMENTO;
			break;
	  //case BENEF_CLASIFICACION:rta = BENEF_CLASIFICACION;break;
		case BENEF_PERFIL:
			rta = BENEF_PERFIL;
			break;
		case BENEF_MEDIO:
			rta = BENEF_MEDIO;
			break;
		case BENEF_INCONDICIONAL:
			rta = BENEF_INCONDICIONAL;
			break;
	}
	return rta;
}
/************************************************************************/
	int TIPO_DE_BENEFICIO( char tipo ){
/************************************************************************/
	int rta = NO;
	switch( tipo ){
		case BENEF_DESCUENTO       :
			rta = BENEF_DESCUENTO;
			break;
		case BENEF_BONOS           :
			rta = BENEF_BONOS;
			break;
		case BENEF_PUNTOS          :
			rta = BENEF_PUNTOS;
			break;
		case BENEF_MONEDERO        :
			rta = BENEF_MONEDERO;
			break;
		case BENEF_ACTIVACION_MEDIO:
			rta = BENEF_ACTIVACION_MEDIO ;
			break;
		case BENEF_MONEDERO_NO_ASIG:
			rta = BENEF_MONEDERO_NO_ASIG ;break;
	}
	return rta;
}

/************************************************************************/
int CORRECCION_REDONDEO_SISTEMA( double *importes )
/************************************************************************/
	{
		int dec, sign, ndig = 2;
		char *str;
		int rta = 0;
		str = fcvt( *importes, 6, &dec, &sign );
		if( str[dec+2] == '5' && str[dec+3] == '0' ) {
			double numero ;
			str[dec+4]='0';
			str[dec+5]='1';
			numero =atof(str);
			numero = numero / 1000000;
			if( sign == 1)
				numero = -numero;
			if(fabs(numero - *importes) < 0.0001) {
				*importes = numero;
				rta = 1;
			}
		} else {
			//82.6449584 -> 82.65  en cambio 8.2648264 -> 8.26
			if( str[dec+2] == '4' && ( str[dec+3] == '9' && str[dec+4] == '5' )) {
				double numero ;
				str[dec+2]='5';
				str[dec+3]='0';
				str[dec+4]='0';
				numero =atof(str);
				numero = numero / 1000000;
				if( sign == 1)
					numero = -numero;
				if(fabs(numero - *importes) < 0.001) {
					*importes = numero;
					rta = 1;
				}
			}
		}
      return ( rta);
	}