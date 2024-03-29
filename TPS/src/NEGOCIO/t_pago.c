#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cajon.h> 
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <coop.h> 
#include <cr.h> 
#include <cr_disc2.h> 
#include <cr_disco.h> 
#include <cr_displ.h> 
#include <cr_pant.h> 
#include <c_ticket.h> 
#include <dbrouter.h> 
#include <descuent.h> 
#include <envios.h> 
#include <eventos.h> 
#include <fiscal.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <import_t.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <netcom.h> 
#include <novios.h> 
#include <pagos.h> 
#include <perfiles.h> 
#include <print.h> 
#include <promo.h> 
#include <ram.h> 
#include <recupero.h> 
#include <rnv.h> 
#include <round.h> 
#include <scanner.h> 
#include <stack.h> 
#include <tkt.h> 
#include <tomra.h> 
#include <t_pago.h> 
#include <t_reproc.h> 
#include <valida.h> 
#include <ventas.h> 
#include <ventas2.h> 
#include <xyz.h> 
#include <_cr1.h> 
#include <band_hor.h>
#include <nc.h>
#include <db.h>
#include <deftables.h>
#include <imp_tick.h>
#include <lib_dreq.h>
#include <ftoc.h>
#include <trim.h>
#include <cio.h>
#include <valida.h>
#include <tarjetas.h>
#include <cobros.h>
#include <ini.h>
#include <devol_am.h>
#include <_cr0.h>
#include <init_pr.h>
#include <pausa.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <config.h>
#include <math.h>
#include <string.h>
#include <aarch.h>
#include <btrv.h>
#include <bridge.h>
#include "actions.h"
#include "interpr.h"
#ifdef PROMO_FAULT_TOLERANCE
#include "fault.h"
#endif //PROMO_FAULT_TOLERANCE
#include "lib_req.h"
#include "lib_dreq.h"
#include "libcache.h"
#include <recupero.h>
#include "path.h"
#include "motor.h"
#define _RTA     GET_MEMORY_INT( _rta )
int nro_cuerpo = 0;
static struct _art *art_imp,*art_act;
static int p_art_imp,p_art_act;
static long comp_nro_comp;
static short int comp_nro_suc;
static short int comp_copias;
extern void GRABAR_EVENTOS_PROMOCIONES(void);
int EJECUTAR_TAREAS_MOTOR( int tipo_tarea, int lugar_impresion, int asignar );
extern int CARGAR_MONEDERO_ELECTRONICO( int origen_promo, char *mensaje );
void MARCAR_ASIGNADO(int pos, int tipo_beneficio);
void CAMBIAR_BENEFICIO(int benef_ariginal, int benef_nuevo);
extern int imprimiendo;
static int envio_off_line;
extern 	int tipo_t_cierre_lote;
static short int nro_sucursal;
extern int DEBE_AGRUPAR_ARTICULOS_CON_DECIMALES( int h );
//extern struct _tarjetas *TARJ;
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_t_ev_super, tipo_ev_dpago;
extern int tipo_t_ev_despro, tipo_t_ev_factu, tipo_t_ev_b_promo, tipo_t_ev_cierre, tipo_ev_cod_barra;
extern double diferencia_parcial[100]; 
extern int  i_contador;
extern int PIDE_SUBTOTAL( double *importe );
extern int CR1_VERIFICAR_LIMITES_FACTURA( double total, int modo );
extern void CR1_INCOSISTENCIA_FISCAL( int sumar, int origen, int t_espera, int cerear );
/*****************************************************************************/
int TECLA_PAGO()
/*****************************************************************************/
{
    int h,i,/*area,*/ cant = 1, tipo = 0/*, j*/, tabla_ant, tipo_ant;
    char permitido = 1;
    int _ptr, _handle, _rta, _estado, rta = 1, tecla, seguir = 0, cliente_ok = 1;
    long id;
    char buffer[sizeof( struct _cuerpo )];
	double temp_t=0.00;

    nro_cuerpo = 0;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //_rta = STACK_INS( sizeof( int ) );
    _rta = STACK_INS( sizeof( short ) );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    //CALCULA_AHORRO_OBTENIDO();
    /*------------- Verifica si hay precios para autorizar ------------*/
    if( config.precio_a_autorizar ) {
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            if( _TICKET_PRECIO( h ) >= config.precio_a_autorizar ) {
                permitido = 0;
            }
        }
        if( !permitido ) {
            if( !AUTORIZAR_PRECIOS() ) {
                CLOSE_RECUPERO( _ptr, _handle );
                return ( 0 );
            }
        }
    }
    if( IDENTIFICACION_CLIENTES_HABILITA && !RAM_NRO_CLIENTE
		&& RAM_TOTAL > config.piso_identificacion_clientes ) {
        do{
            MENSAJE_STRING( S_DEBE_IDENTIFICAR_AL_CLIENTE );
            rta = ELEGIR_CLIENTE_NUMERICO( NO, NO );
            if( !rta ) {
                MENSAJE_SIN_SONIDO_STRING( S_DEBE_IDENTIFICAR_EL_CLIENTE_ESC_RECHAZA );
                tecla = 0;
                while( tecla != 13 && tecla != 27 ) {
                    tecla = GETCH();
                }
                if( tecla == 13 ) {
                    rta = 1;
                    cliente_ok = 0;
                }
            }
		} while( !rta );
        if( !rta ) {
            CLOSE_RECUPERO( _ptr, _handle );
            return ( 0 );
        }
    }
    /*--------- Verifica si hay anulaciones sin autorizar y envases ----------*/
	if( _ESTADO == 0 ) {
        if( !CR1_VERIFICAR_LIMITES_FACTURA( RAM_TOTAL, VERIFICAR_LIMITE_AL_FINAL ) ) {
            ;
        }
        else if( !VERIFICA_ANULACIONES_AUTORIZADAS() ) {
            MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_DEBE_AUTORIZAR_LAS_ANULACIONES ), 18 );
        }
        else if( !VALIDA_ENVASES_ENTREGADOS() ) {
            MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_EXCESO_ENVASES_ENTREGADOS ), 18 );
        }
        else if( !VERIFICAR_BANDA_HORARIA() ) {
            MENSAJE_CON_PAUSA(
            BUSCA_MENSAJE_STRING( S_DEBE_ANULAR_EL_ITEM_INVALIDO_PARA_CONTINUAR ), 18 );
        }
        else if( !VERIFICA_NC_AUTORIZADAS() ) {
            MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_DEBE_AUTORIZAR_LOS_PRECIOS ), 18 );
        }
        else if(!cliente_ok){
            MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_DEBE_IDENTIFICAR_AL_CLIENTE ), 18 );
        }
        else {
            seguir = 1;
            if( MODALIDAD_TOMRA && _RAM_NUMERO_TICKET_TOMRA( 0 ) ) {
                if( ( MODALIDAD_TOMRA == _POR_TIPO ) || !GET_ERROR_EN_RED() ) {
                    VACIAR_TABLA_AUXILIAR();
                    if( MODALIDAD_TOMRA == _POR_ARCHIVO ) {
                        i = 0;
                        while( _RAM_NUMERO_TICKET_TOMRA( i ) ) {
                            BUSCAR_VOUCHER_TOMRA( _RAM_NUMERO_TICKET_TOMRA( i ), 1 );
                            i++;
                        }
                        BUSCAR_ENVASES_FACTURADOS();
                        RECORRER_ENVASES( 1 );
                    }
                    else if( MODALIDAD_TOMRA == _POR_TIPO ) {
                        BUSCAR_ENVASES_FACTURADOS();
                        if( !CARGAR_VOUCHER_ENVASE() ) {
                            MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
                        }
                        RECORRER_ENVASES( 1 );
                    }
                }
                else {
                    MENSAJE_STRING( S_SIN_RED_NO_SE_PROCESARON_TICKETS_DEV_ENVASES );
                }
            }
            VERIFICA_EL_TOTAL();
            #ifdef COMPILAR_COMPROBANTES
            if( RAM_FACTURA  || RAM_TICKET_FACTURA ) {
                SET_MEMORY_INT( _rta, OBTENER_DATOS_RETENCIONES() );
            }
            #endif
            PROCESA_DESCUENTOS_ESPECIALES();
			/*if( _RTA ) {
				//Se coloca en este lugar porque si hay corte de luz en este estado 
				//vuelve a obtener las retenciones para la autorizacion
				AUTORIZAR_RETENCION_NC( );
				AUTORIZAR_PERCEPCION_NC( );
			}*/
        }
    }
    else {
        seguir = 1;
    }
    if( seguir ) {
        /*---------------------- Procesa las promociones ----------------------*/
		if( _ESTADO == 0 ||  _ESTADO == 80 ||  _ESTADO == 81 ||  _ESTADO == 82 ||  _ESTADO == 83 ) {
            SET_MEMORY_DOUBLE( __ram_subtotal, RAM_TOTAL );
            temp_t = RAM_TOTAL ; 
            AGRUPAR_ARTICULOS_TRANSACCION_ACTIVA();

            APLICAR_DESCUENTOS_PENDIENTES();

            if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ) {
                if( !RAM_NOTA_CR && !MODO_DEVOLUCION ) {
					SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );
					/*----------------- motor ----------------------------*/
					/**/glog( "Evaluando PROMOS PREPAGO",LOG_PAGOS,4 );
					// se guarda el estado actual de la caja y de las
					// variables de las promos para permitir revertir
					// los efectos de la evaluacion prepago.
					state_venta = actual_info( ss );

					if( _ESTADO == 0 ) _SET_ESTADO( 81 );
			        if( _ESTADO == 81 ) {
						#ifdef PROMO_FAULT_TOLERANCE
						logPanelPagos();
						#endif //PROMO_FAULT_TOLERANCE
						_SET_ESTADO( 82 );
					}
			        if( _ESTADO == 82 ) {
						_SET_ESTADO( 83 );
						//cereemos el vector de diferencia
						memset( &diferencia_parcial, 0, sizeof( diferencia_parcial) );
						i_contador = 0;
						if( eval( PREPAGO, pr, &ss ) != NORM ) {
							glog( "PROMO: T_PAGO.C: eval(PREPAGO): ERROR in PROMOS" ,LOG_PAGOS,3);
						}
					}
					
					/**/glog( "PROMOs PREPAGO Evaluadas correctamente" ,LOG_PAGOS,4);
					if( !RAM_NOTA_CR ){
						PromoSQL_MxN(RAM_ID_EVENTO);
						DIRECT_DB_QUERY("drop table if exists activas_disparantes;");
						DIRECT_DB_QUERY("drop table if exists activas_beneficiado;");
					}
					/*----------------- motor ----------------------------*/
					SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
                } //fi( !RAM_NOTA_CR ) {
                else if( RAM_REFERENCIA_NC ) { //Solo verificamos para las NC/Dev Referenciadas
                    CARGAR_DEVOLUCION_AM( );
                }
			}
            //GRABAR_EVENTO_DESC( MOTIVO_DESC_MANUAL_PORC, NO );
            //GRABAR_EVENTO_DESC( MOTIVO_DESC_MANUAL_PESOS, NO );
            //IMPRIMIR_CUPONES_DE_PROMOCION( _PRE_PAGO );
            _SET_ESTADO( 111 );
        }
        /*------------------- Llama a PAGOS o paga la NC ---------------------*/
        if( _ESTADO == 111 ) {
			//agregamos el subtotal
			int rta_f;
			if( IMPRESORA_FISCAL == FISCAL) {
				double toti_fiscal;
				rta_f = PIDE_SUBTOTAL( &toti_fiscal ); //aca si hay error de impresora hay que anular el ticket
				// aca si da error de impresora debe anular
				if( rta_f == 3) {
					glog( ST(ERROR_DE_IMPRESORA___ERROR_GENERAL) ,LOG_ERRORES,1);
					CR1_INCOSISTENCIA_FISCAL( SI, 0 /*origen*/, SI, SI );
					_SET_ESTADO( 99 );
				}
				//toti_fiscal = 28.34;
				SET_MEMORY_DOUBLE( __ram_puntos_esp_3, toti_fiscal);
			}
			//
            COPY_STRUCT( _tot_tick_sv, _tot_tick );
            /*----------------- Busca los datos de la factura ---------------------*/
            #ifdef COMPILAR_COMPROBANTES
            if( EMITIR_FACTURA() ) {
                SET_MEMORY_INT( _rta, DATOS_COMPROBANTE( _FACTURA ) );
            }
            #endif
			if( RAM_NOTA_CR ) {
                if( !MEDIO_PAGO_NC ) {
                    SET_MEMORY_INT( _rta, PAGOS( _MODO_NC, NULL, NO ) );
                }
                else {
                    SET_MEMORY_INT( _rta, 1 );
                    SET_MEMORY_DOUBLE( __ram_importe_total, ROUND( -RAM_TOTAL, 10, DECIMALES ) );
                    CALCULAR_RETENCIONES();
                    ABRIR_CAJON();
                    if( MEDIO_PAGO_NC == 1 ) {
                        ADD_MEMORY_DOUBLE( __x_tot_dinero, -PAGOS_TOTAL_A_PAGAR() );
                    }
                    else if( MEDIO_PAGO_NC && MEDIO_PAGO_NC <= 9 ) {
                        _ADD_MEMORY_DOUBLE( __x_ventas_importe, MEDIO_PAGO_NC - 1,
                                            -PAGOS_TOTAL_A_PAGAR() );
                    }
                }
			}
				else {

					if( rta_f != 3)  {
				//BORRAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO );  // FR: Se comenta para que no se borren los datos de las promociones.
				/**/glog( "ERROR TECLADO: GENERAR_REQ_MEDIOS" ,LOG_PAGOS,4);
				//GENERAR_REQ_MEDIOS( );
				/**/glog( "ERROR TECLADO: ingresa a pagos" ,LOG_PAGOS,4);
								//mamcnsuo vamos por las pospago reversion
				if( RAM_P_PAGO > 0 && ( NRO_PAGARE == 99 || NRO_PAGARE == 1 )){ //se corto en la ventana de confirmacion de promo post_pago
					SET_MEMORY_INT( _rta, 0 ); //fuerzo que vuelva a ventas para cancelar descuento
					SET_MEMORY_CHAR( __ram_p_pago, 0 );

				} else {
	                SET_MEMORY_INT( _rta, PAGOS( _MODO_VENTA, NULL, NO ) );
					SET_MEMORY_LONG( __nro_pagare, 0 );
				}
				/*if( RAM_HAY_COBRO_VENTA  && _RTA ){
					SET_MEMORY_INT( _rta, _RTA && VALIDACIONES_COBROS_VENTA( ) );
					if( _RTA ){
						SET_MEMORY_CHAR( __ram_hay_cobro_venta, 0 );
						BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA );
						BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_GARANTIA );
						BORRAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO );
					}
				}*/
					} else {
						SET_MEMORY_INT( _rta, 0 );
					}
			}
            if( _RTA ) {
                _SET_ESTADO( 2 );
            }
            else {
                //SALVADOR_RESETEAR_TICKET_CANT_DESC();


                if( UTILIZA_PROMOCIONES && !RAM_NOTA_CR ) {
                    if( !UTILIZA_MODULO_PROMOCIONES ) {
                        //REVERTIR_PROMOCIONES();
                        /*if( BORRAR_PERFIL_AL_SALIR_DE_PAGOS && !TOMAR_PERFIL_DESDE_CLIENTE ) {
                            SET_MEMORY_INT( __ram_perfil, 0 );
                            MUESTRA_PERFIL();
                        }*/
                    }
                }
                if( BORRAR_TARJETA_AL_SALIR_DE_PAGOS ) {
                    CEREAR_STRUCT( _var_tarjetas );
                    CR1_MUESTRA_TARJETA();
                }
                if( _RAM_NUMERO_TICKET_TOMRA( 0 ) ) {
                    REVERTIR_ENVASES();
                    VACIAR_TABLA_AUXILIAR();
                }
                LIMPIA_RECUADRO_DEL_TICKET();
                buscar_articulo_en_mostrar_item = 1;
                RECUPERA_VENTANA_TICKET();
                buscar_articulo_en_mostrar_item = 0;
                MUESTRA_SUBTOTAL(NO);
                MUESTRA_BULTOS();
                REVERTIR_ARTICULO_DESCUENTO();
                REVERTIR_DESCUENTOS_FINAL_TICKET();
            }
        }
        /*---------------- Acumula totales de facturas e iva ------------------*/
        if( _ESTADO == 2 ) {
            ACTUALIZA_ACU();
            _SET_ESTADO( 3 );
        }
        /*----------- Actualiza departamentos, anulaciones y envases ----------*/
        if( _ESTADO == 3 ) {
            ACTUALIZA_MEMORIA();
            _SET_ESTADO( 4 );
        }
        /*--------------------- Actualiza demora y bonos ----------------------*/
        if( _ESTADO == 4 ) {
            ACTUALIZA_DEMORA();
            //ACUMULAR_BONOS();
            #ifdef COMPILAR_MOZO
            ACUMULA_MOZOS( NRO_MOZO );
            #endif
            _SET_ESTADO( 5 );
        }
        /*------------------------- Calcula promociones -----------------------*/
        if( _ESTADO == 5 || _ESTADO == 85 || _ESTADO == 86 || _ESTADO == 87 ) {
/*            //if( UTILIZA_MODULO_PROMOCIONES && !RAM_NOTA_CR ) {
			 if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && !RAM_NOTA_CR ) {
				SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );
                //MENSAJE("Evaluando PROMOs POSTPAGO");
                glog( "Evaluando PROMOS POSTPAGO" ,LOG_VENTAS,2);

                // se guarda el estado actual de la caja y de las
                // variables de las promos para permitir revertir
                // los efectos de la evaluacion postpago.
                state_pago = actual_info( ss );



				if( _ESTADO == 5 ) _SET_ESTADO( 85 );

			    if( _ESTADO == 85 ) {

					#ifdef PROMO_FAULT_TOLERANCE
					logPanelPosPago();
					#endif //PROMO_FAULT_TOLERANCE

					_SET_ESTADO( 86 );
				}

			    if( _ESTADO == 86 ) {

					_SET_ESTADO( 87 );

					if( eval( POSTPAGO, pr, &ss ) != NORM ) {
						glog( "PROMO: T_PAGO.C: eval(POSTPAGO): ERROR in PROMOS",LOG_PAGOS,4 );
					}
				}
                /glog( "PROMOs POSTPAGO Evaluadas correctamente" ,LOG_PAGOS,4);
                //MENSAJE("PROMOs Evaluadas correctamente");
				SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
            }
            else {
                if( UTILIZA_PROMOCIONES && !RAM_NOTA_CR ) {
                    //CALCULAR_PROMOCIONES( _POS_PAGO );
                }
            }*/
            _SET_ESTADO( 61 );
        }
        
            if( _ESTADO == 61 ) {
                EJECUTAR_TAREAS_MOTOR( ASIG_BENEF_MONEDERO, DENTRO_DEL_TICKET, SI  );
                EJECUTAR_TAREAS_MOTOR( ASIG_BENEF_STR_TICKET, DENTRO_DEL_TICKET, NO  );
            //EJECUTAR_TAREAS_MOTOR( ASIG_BENEF_BONO, DENTRO_DEL_TICKET, NO  );
            if( !RAM_NOTA_CR && !EMITIR_FACTURA() && ( !EMITIR_FACTURA_REMOTA() || IMPRIMIR_TICKET_SI_ES_REMOTA ) ) {
					tipo = ( RAM_TICKET_FACTURA )? _TICKET_FACTURA: _TICKET;
					if( tipo == _TICKET_FACTURA ) {
						cant = ( IMPRIMIR_COPIA_TF )? 0 : 1 ;
					} 
                OBTENER_DATOS_COMPROBANTE( tipo, 0, cant , &comp_nro_suc, &comp_nro_comp, &comp_copias, SI, NO );
				if(IMPRESORA_FISCAL != FISCAL)
				{long auc=0L;
						auc=	RAM_NRO_COMP;
					SET_MEMORY_LONG( __nro_ticket, RAM_NRO_COMP );
					//SET_MEMORY_LONG( __ram_nro_comp, 0 );
                    IMPRIME_PIE();
				}
				else {
					SET_MEMORY_LONG( __nro_ticket, RAM_NRO_COMP+1 );
					//SET_MEMORY_LONG( __ram_nro_comp, 0 );
				}
                
                    imprimiendo = 1;    
		            while( imprimiendo )  // se queda en este while hasta que la funcion entry setee la variable en 0 cuando termine de imprimir
			            SLEEP(10);
            } else {
                    if( VALIDAR_CLIENTE_AFINIDAD() ) {
                        VALIDAR_LIMITE_DGI();
                        PRN_FIN_TICKET();
                    }
				if(IMPRESORA_FISCAL == FISCAL){
					SET_MEMORY_LONG( __nro_ticket, RAM_NRO_COMP+1 );
					//SET_MEMORY_LONG( __ram_nro_comp, 0 );
				}
            }//fin pie
			EJECUTAR_TAREAS_MOTOR( ASIG_BENEF_BONO, DENTRO_DEL_TICKET, NO  );

            if( IMPRESORA_FISCAL != FISCAL && !EMITIR_FACTURA() && ( !EMITIR_FACTURA_REMOTA() || IMPRIMIR_TICKET_SI_ES_REMOTA ) ) {
                IMPRIME_FIN_TICKET( );
            }
			if( SI_FRANQUE0_AL_FINAL ) {
				//Antes de ACTUALIZA_EVENTOS para que no borre el archivo que guarda datos adicionales.
				VALIDAR_MEDIOS( _MODO_VENTA );
				SI_FRANQUE0_AL_FINAL = 0;
			}
            if( ( RAM_MODO_DEVOLUCION || RAM_REFERENCIA_NC || MODO_DEVOLUCION )
             && REVERSAR_ACUMULADORES_EN_DEVOLUC ) {
                REVERSAR_PAGO_BOLETA_PLATA_EN_DEVOLUCIONES();
            }
            _SET_ESTADO( 62 );
        }
        /*------------------------- Actualiza eventos -------------------------*/
        if( _ESTADO == 62 ) {
            /*if( !GET_ERROR_EN_RED() ) {
                MENSAJE_STRING_SIN_PAUSA( S_GRABANDO_EN_RED );
            }*/
            ACTUALIZA_DESCUENTOS_BOLETIN_ARTICULOS();
            ACTUALIZA_BASE_DTO_EVENTO();
            SETEAR_TIEMPO( _INDEFINIDO );
            SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
            ACTUALIZA_EVENTOS( );
            ACUMULAR_TIEMPOS( );
            CEREAR_TIEMPOS( );
            SETEAR_TIEMPO( _ENTRADA_OCIO );
            //Guarda los eventos de promociones
            GRABAR_EVENTOS_PROMOCIONES();
            ACTUALIZA_PUNTOS_CLIENTES();
            _SET_ESTADO( 7 );
		#ifdef COMPILAR_LOGS_VIA_TCP
         {
          LOGS_VIA_TCP("--FIN TICKET--");
         }
		#endif
        }
        /*------------- Actualiza acumuladores de ventas (ACU_VTA) ------------*/
        if( _ESTADO == 7 ) {
            ACTUALIZA_VENTAS();
            _SET_ESTADO( 71 );
        }
        /*------------------ Graba o imprime la factura -----------------------*/
        if( _ESTADO == 71 ) {
            #ifdef COMPILAR_COMPROBANTES
			if( ( config.pais == ELSALVADOR && RAM_TICKET_FACTURA ) || RAM_FACTURA ) {
				if(IMPRESORA_FISCAL!=FISCAL) 
				{
					if( EMITIR_FACTURA_REMOTA() ) {
						IMPRIME_RETIRAR_FACTURA( PAGOS_TOTAL( 1 ) );
						IMPRIMIR_TICKET_FACTURA( 1 );
					}
					else {
						if( RAM_NOTA_CR ) {
							SET_MEMORY_INT( _rta, IMPRIMIR_COMPROBANTE( _NOTA_CREDITO, NO, SI ) );
						}
						else {
							SET_MEMORY_INT( _rta, IMPRIMIR_COMPROBANTE( _FACTURA, NO, SI ) );
						}
						if( _RTA ) {
							//Actualizar el numero de comprobante del ticket.btr
							long id_ev_ant = RAM_ID_EVENTO;
							//SET_MEMORY_LONG( __ram_id_evento, id_ev_ant - 1 );
							SET_MEMORY_LONG( __ram_id_evento, RAM_ID_ULTIMO_EVENTO_FACTURA );
							if( config.pais == CHILE || config.pais == BRASIL ) {
								/*--- Cuerpos existentes en Varios ---*/
								for( i = 0;i < nro_cuerpo;i++ ) {
									GRABAR_EVENT_FACTURA( i );
									memcpy( buffer, &CUERPOS[i], sizeof( struct _cuerpo ) );
									GRABAR_EVENT_VARIOS( buffer, sizeof( buffer ), NO );
								}
								nro_cuerpo = 0;
							}
							else {
								//area = SELECTED();
								tabla_ant = SELECTED2();
								tipo_ant = SELECTED_TIPO();
								//SELECT( AREA_TICKET );
								//SET_ORDER( 1 );   //Busqueda por Id_evento
								id = RAM_ID_EVENTO;
								//GET_EQUAL( ( char* )&id );
								SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
								SET_ORDER2("ID_EVENTO");
								SET_WHERE("ID_EVENTO = '%ld'",id);
								RUN_QUERY(NO);
								if( FOUND2() ) {
									//Actualizar el nro_comp
									if( event_ticket.nro_comprobante_new != comp.nro_comp ) {
										event_ticket.nro_comprobante_new = comp.nro_comp;
										#ifdef INVEL_L
										//BEGIN_TRANSACTION();
										#endif
										UPDATE2();
										#ifdef INVEL_L
										//END_TRANSACTION();
										#endif
									}
								}
								//SELECT( area );
								SELECT_TABLE( tabla_ant, tipo_ant );
							}
							SET_MEMORY_LONG( __ram_id_evento, id_ev_ant );
						}
                    }
				}//Seba
                if( RAM_NOTA_CR ) {
                    GRABAR_LOG_SISTEMA( "Nota de credito" ,LOG_PAGOS,4);
                }
                else {
                    GRABAR_LOG_SISTEMA( "Factura" ,LOG_PAGOS,4);
                }
            }
            #endif
            _SET_ESTADO( 8 );
        }
        /*------------------- graba en servidor -------------------*/
        if( _ESTADO == 8 ) {

            ACTUALIZA_VENTAS2( NO );
            _SET_ESTADO( 9 );
        }
        /*------------------- Enva transacciones de novios -------------------*/
        if( _ESTADO == 9 ) {
            #ifdef COMPILAR_NOVIOS
            if( RAM_NOVIOS ) {
                ENVIAR_TRANSACCIONES_NOVIOS();
            }
            #endif
            _SET_ESTADO( 10 );
        }
        /*------------------ Imprime los cupones de promocin -----------------*/
        if( _ESTADO == 10 ) {
            //IMPRIMIR_CUPONES_DE_PROMOCION( _POS_PAGO );
            IMPRIMIR_CUPONES_TOMRA();
			if( !RAM_HAY_COBRO_VENTA )
				VALIDAR_COBROS_POR_ARTICULO();

			if( RAM_HAY_COBRO_VENTA  && _RTA ){
				SET_MEMORY_INT( _rta, _RTA && VALIDACIONES_COBROS_VENTA( ) );
				if( _RTA ){
					SET_MEMORY_CHAR( __ram_hay_cobro_venta, 0 );
					BORRAR_ARCHIVOS_COBROS_VENTA( NO);
				}
			}

            _SET_ESTADO( 11 );
            EJECUTAR_TAREAS_MOTOR( ASIG_BENEF_STR_TICKET, FUERA_DEL_TICKET, NO  );
            EJECUTAR_TAREAS_MOTOR( ASIG_BENEF_BONO, FUERA_DEL_TICKET, NO  );
            
        }
        /*-------------------- Finaliza el procedimiento ----------------------*/
        if( _ESTADO == 11 ) {
            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 || IMPRESORA_FISCAL == FISCAL_TMT88 ) {
                FIN_DOCUMENTO( NO, NO );
                PRN_FIN_TICKET();
            }
            GRABAR_XYZ( 'X' );
            //MENSAJE_SIN_SONIDO_STRING( S_PROCESANDO_SCRIPT );
			//VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS();
            ACTUALIZAR_X();
			// se comenta por TPS-293            CERRAR_CAJON();
            if( TKT_CONFIGURABLE && RAM_TICKET_FACTURA && IMPRIMIR_COPIA_TF && RAM_COD_FUNCION == 30 ) {
				OBTENER_DATOS_COMPROBANTE( _TICKET_FACTURA, 0, 1 , NULL, &comp_nro_comp, NULL, SI, NO );
				SET_MEMORY_LONG( __nro_ticket, RAM_NRO_COMP );
			}
			
            //FIN_DE_TICKET( NO );
			//if( config.pais == MEXICO || ( config.pais == ELSALVADOR && IMPRIMIR_TICKET_AL_FINAL ) ) {
			if( ORDEN_VOUCHER_DE_TARJETA == _VOUCHER_PRETICKET ) {
				//se imprime el voucher de tarjeta antes del ticket x un problema de lentitud.
				//pero esto solo funciona si la impresión del ticket es al final
				if( IMPRIMIR_TICKET_AL_FINAL ) {
					PAGOS_IMPRIME_PAGOS_ESPECIALES( NO, NO );
				} else {
					MENSAJE( "ERROR EN CONFIGURACION FORMATOS IMPRESION" );
				}
			} else {
				if( IMPRIMIR_TICKET_AL_FINAL )
					PAGOS_IMPRIME_PAGOS_ESPECIALES( NO, NO );
				else
					PAGOS_IMPRIME_PAGOS_ESPECIALES( NO, NO ); //para el vouche de cta cte
			}
			VALIDAR_VUELTO( );
			//El arch. de los Datos Adicionales fueron borrados en la funcion ACTUALIZA_EVENTOS
			//de manera que hay que recurrir la tabla.
			//La variable COD_VALIDACION_AL_FINAL se configura en los arch. extension CFG 
			//primero deberia leer la validacion para su impresion.
			if( COD_VALIDACION_AL_FINAL ) {
				VALIDAR_COMPROBANTE( );
				COD_VALIDACION_AL_FINAL = 0;
			}
			IMPRIMIR_COPIA_TICKET_FACTURA( );
			FIN_DE_TICKET( SI );

			/*----------------- motor ----------------------------*/
            if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ) {
				BORRAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO );
				reinit_promos();
                }
			/*----------------- motor ----------------------------*/

            GRABAR_LOG_SISTEMA_CONFIG( "FIN PROMOS", LOG_PAGOS,4 );
            #ifdef COMPILAR_DISPLAY            
			PAUSA( 6 ); //Permite visualizar el el vuelto
			{char tiempo[15];sprintf(tiempo,"tiempo2");glog(tiempo,LOG_PAGOS,5);}
			CR1_DISPLAY_CAJA_ABIERTA();
            #endif
            if( !EMITIR_FACTURA() ) {
                FISCAL_FIN_BOLETA();
            }
            if( SALIR_DESPUES_DE_CADA_TICKET ) {
                SET_MEMORY_INT( __ram_fin_ticket, 1 );
            }
            GRABAR_LOG_SISTEMA_CONFIG( "Borrando archivos de comunicación motor promos",LOG_PAGOS,5 );
            /*Borra el archivo de ticket importados*/
            BORRAR_ARCHIVO_REC(ARCH_REC_IMPORT);
            /*Borra el archivo de pagos realizados con medios activados por promocion*/
            BORRAR_ARCHIVO_REC(ARCH_REC_PAGOS_PROMO);
            BORRAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO);
            /*Borra el archivo de activacion de medios desde el motor de promociones*/
            BORRAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR);
			CERRAR_CAJON(); //  caso TPS-293
        }
    }
    /*------------------- Termina recupero de memoria -----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
    CLOSE_RNV_VIRTUAL();
    OPEN_RNV_VIRTUAL();
    GRABAR_LOG_SISTEMA_CONFIG( "Sale Tecla Pago..........",LOG_PAGOS,4 );
    return ( 1 );
}
/*****************************************************************************/
int VERIFICA_ANULACIONES_AUTORIZADAS()
/*****************************************************************************/
{
    int ok = 1, h;

    if( AUTORIZAR_ANULACIONES == 1
		//No solicitar autorizacion en articulos anulados de importaciones totales
		&& !( RAM_MODO_DEVOLUCION == _DEVOL_TOTAL || RAM_MODO_DEVOLUCION == _NC_TOTAL ) ) {
        for( h = RAM_ITEM_SUPERVISADO + 1;h < RAM_P_TICKET;h++ ) {
            if( _TICKET_TIPO( h ) & _RESTA && !( _TICKET_TIPO2( h ) & _CORRECCION ) ) {
                ok = 0;
            }
			if( _TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION )
				ok = 1;
        }
    }
    return ( ok );
}
/*****************************************************************************/
void VERIFICA_EL_TOTAL()
/*****************************************************************************/
{
    int h = 0, fatal= 0;
    double tot = 0, imp = 0.0, dif = 0.0;
    char aux[80];
    SET_RAM( ON );
    for( h = 0;h < RAM_P_TICKET;h++ ) {
        imp = ROUND( _TICKET_CANTIDAD( h ) * ( _TICKET_PRECIO( h ) + _TICKET_RECARGO( h ) ), 10,
                     2 );
        if( _TICKET_TIPO( h ) & _RESTA ) {
            imp = -imp;
        }
        tot = ROUND( tot + imp, 10, 2 );
    }
    dif = fabs( tot - RAM_TOTAL );
    if( IMPRESORA_FISCAL ) {
        fatal = ( dif > 1000.00 ) ? 1 : 0;
    }
    else {
        fatal = ( dif > 10.00 ) ? 1 : 0;
    }
    if( fatal ) {
        MENSAJE_STRING( S_ERROR_FATAL_LLAME_AL_SERVICIO_TECNICO );
        //strcpy( aux, "Error en total = " );
        //FTOA( tot - RAM_TOTAL, aux2, ENTEROS + 1, DECIMALES, 1, 0 );
        //strcat( aux, aux2 );
        _snprintf(aux, sizeof(aux)-1, "Error en total: %lf %lf %i", tot, RAM_TOTAL, RAM_P_TICKET );
        GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,1);
        //CR0_EXIT( 1 );
    }
}
/*****************************************************************************/
int AUTORIZAR_PRECIOS()
/*****************************************************************************/
{
    int rta = 0;
    //   LIMPIAR_RECUADRO_PETICION( " AUTORIZACION DE PRECIOS " );
    if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_DE_PRECIOS_ ),
                          ST( S_AUTORIZO_PRECIOS ), _F_SUP_AUT_PRECIO, SI ) ) {
        //      GRABAR_LOG_SUPERVISOR( "Autoriz precios", USUARIO_CLAVE_ANTERIOR() );
        SET_MEMORY_INT( __ram_item_supervisado, RAM_P_TICKET - 1 );
        rta = 1;
    }
    LIMPIAR_RECUADRO_PETICION( 0 );
    return ( rta );
}
/*****************************************************************************/
int VERIFICA_SUPERA_LIMITE_AL_FINAL()
/*****************************************************************************/
{
    int ok = 1;
    int mostrar=0;
    double total = RAM_TOTAL;
    char *mensaje_exed, *tit_auth, *tit_box;
    int f_sup;
    if ( config.importe_minimo > total && config.pais == MEXICO && !RAM_FACTURA && !RAM_MODO_DEVOLUCION) {
        mensaje_exed = ST( S_IMPORTE_MINIMO_NO_ALCANZADO ); 
        tit_auth = ST ( S_AUTORIZO_IMPORTE_MINIMO_NO_ALCANZADO );
        f_sup = _F_SUP_IMPORTE_MINIMO_TICKET;
        tit_box = tit_auth;
        mostrar = 1;
    }
    if (total > ( double )config.limite && !RAM_FACTURA){
        mensaje_exed = ST( S_IMPORTE_TICKET_EXCEDIDO );
        tit_auth = ST( S_AUTORIZO_IMPORTE_EXCEDIDO );
        f_sup = _F_SUP_LIMITE_TICKET;
        tit_box = mensaje_exed;
        mostrar = 1;
    }
    if(mostrar){
        if( PERMITE_SUPERAR_LIMITE_TICKET ) {
            if( VERIFICAR_LIMITE_DGI == VERIFICAR_LIMITE_AL_FINAL ) {
                MENSAJE( mensaje_exed );
                if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, SI, tit_box,
                                    tit_auth, f_sup, SI ) ) {
                    ok = SI;
                }
                else {
                    ok = NO;
                }
            }
        }
        else {
            MENSAJE( mensaje_exed );
            ok = NO;
        }
    }
    return ok;
}

 
#define NO_ESPERA_RESPUESTA 0
#define ESPERA_RESPUESTA     1
#define ARCHIVOS_HISTORICOS 2


/*****************************************************************************/
int ABRIR_ARCHIVOS_EVENTOS_HISTORICOS()
/*****************************************************************************/
{
    int iRta = 0;
    int isNotNull = 0;
    int nError = 0;

    FREEMEM( ( char* )event_ticket_hist );
    FREEMEM( ( char* )event_e_ticket_hist );
    FREEMEM( ( char* )event_pago_hist );
    FREEMEM( ( char* )event_cod_barra_hist );
    FREEMEM( ( char* )event_cierre_hist );
    FREEMEM( ( char* )event_dpago_hist );
    FREEMEM( ( char* )event_super_hist );
    FREEMEM( ( char* )event_despro_hist );
    FREEMEM( ( char* )event_factura_hist );
    FREEMEM( ( char* )event_b_promo_hist );
    event_ticket_hist = NULL;
    event_e_ticket_hist = NULL;
    event_pago_hist = NULL;
    event_cod_barra_hist = NULL;
    event_cierre_hist = NULL;
    event_dpago_hist = NULL;
    event_super_hist = NULL;
    event_despro_hist = NULL;
    event_factura_hist = NULL;
    event_b_promo_hist = NULL;
    event_ticket_hist = ( struct _eventos * )malloc( sizeof( struct _eventos ) );
    event_e_ticket_hist = ( struct _ev_cont * )malloc( sizeof( struct _ev_cont ) );
    event_pago_hist = ( struct _ev_medios * )malloc( sizeof( struct _ev_medios ) );
    event_cod_barra_hist = ( struct _ev_cod_barra * )malloc( sizeof( struct _ev_cod_barra ) );
    event_cierre_hist = ( struct _ev_cierre * )malloc( sizeof( struct _ev_cierre ) );
    event_dpago_hist = ( struct _ev_dmedios * )malloc( sizeof( struct _ev_dmedios ) );
    event_super_hist = ( struct _ev_super * )malloc( sizeof( struct _ev_super ) );
    event_despro_hist = ( struct _ev_despro * )malloc( sizeof( struct _ev_despro ) );
    event_factura_hist = ( struct _ev_factu * )malloc( sizeof( struct _ev_factu ) );
    event_b_promo_hist = ( struct _ev_bpromo * )malloc( sizeof( struct _ev_bpromo ) );
    
    isNotNull = ( event_ticket_hist != NULL ) && ( event_e_ticket_hist != NULL )
             && ( event_pago_hist != NULL ) && ( event_cod_barra_hist != NULL )
             && ( event_cierre_hist != NULL ) && ( event_dpago_hist != NULL )
             && ( event_super_hist != NULL ) && ( event_despro_hist != NULL )
             && ( event_factura_hist != NULL ) && (event_b_promo_hist != NULL);
//(Nota Unific MySQL-Linux): comento la asignacion a tipo_t_eventos ya que esta pensada para alternar entre tt_orig y tt_entr
// tt_hist se mantiene independiente
    if( isNotNull ) {
        /*if( !USE_DB( AREA_TICKET_HIST, _TICKET_HIST_BTR, ( char* )event_ticket_hist,
                     sizeof( struct _eventos ), NULL, 0, 0 )*/
		if( !nError ) {
			memset( event_ticket_hist, 0, sizeof( event_ticket) );
		nError = OPEN_TABLE(T_EVENTOS, TT_HIST,( char* )event_ticket_hist,sizeof( event_ticket ));
		//tipo_t_eventos = TT_HIST;
		}
        /* && !USE_DB( AREA_E_TICKET_HIST, _E_TICKET_HIST_BTR, ( char* )event_e_ticket_hist,
                     sizeof( struct _ev_cont ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_e_ticket_hist, 0, sizeof( event_e_ticket) );
		nError += OPEN_TABLE(T_EV_CONT, TT_HIST,( char* )event_e_ticket_hist, sizeof( event_e_ticket ));
		//tipo_t_ev_cont = TT_HIST;
		}
        /* && !USE_DB( AREA_PAGO_HIST, _PAGO_HIST_BTR, ( char* )event_pago_hist,
                     sizeof( struct _ev_medios ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_pago_hist, 0, sizeof( event_pago) );
		nError += OPEN_TABLE(T_PAGOS, TT_HIST, (char* )event_pago_hist, sizeof( event_pago ));
		//tipo_t_pagos = TT_HIST;
		}
        /* && !USE_DB( AREA_COD_BARRA_HIST, _EV_T_COMPRA_HIST_SIC, ( char* )event_cod_barra_hist,
                     sizeof( struct _event_cod_barra ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_cod_barra_hist, 0, sizeof( event_cod_barra) );
		nError += OPEN_TABLE( T_EV_COD_BARRA, TT_HIST, ( char* )event_cod_barra_hist, sizeof( event_cod_barra ) );
        //tipo_ev_cod_barra = TT_HIST;
		}
        /* && !USE_DB( AREA_CIERRE_HIST, _EV_CIERRE_HIST_BTR, ( char* )event_cierre_hist,
                     sizeof( struct _event_cierre ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_cierre_hist, 0, sizeof(struct _ev_cierre) );
		nError += OPEN_TABLE( T_EV_CIERRE, TT_HIST, ( char* )event_cierre_hist, sizeof( struct _ev_cierre ) );
        //tipo_t_ev_cierre = TT_HIST;
		}
        /* && !USE_DB( AREA_DPAGO_HIST, _EV_DPAGO_HIST_BTR, ( char* )event_dpago_hist,
                     sizeof( struct _ev_dmedios ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_dpago_hist, 0, sizeof(struct _ev_dmedios) );
		nError += OPEN_TABLE( T_EV_DMEDIOS, TT_HIST, ( char* )event_dpago_hist, sizeof( struct _ev_dmedios ) );
        //tipo_ev_dpago = TT_HIST;
		}
        /* && !USE_DB( AREA_SUPER_HIST, _EV_SUPER_HIST_BTR, ( char* )event_super_hist,
                     sizeof( struct _ev_super ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_super_hist, 0, sizeof(struct _ev_super) );
		nError += OPEN_TABLE( T_EV_SUPER, TT_HIST, ( char* )event_super_hist, sizeof( struct _ev_super ) );
        //tipo_t_ev_super = TT_HIST;
		}
        /* && !USE_DB( AREA_E_DESPRO_HIST, _E_DES_PRO_HIST, ( char* )event_despro_hist,
                     sizeof( struct _ev_despro ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_despro_hist, 0, sizeof(struct _ev_despro) );
		nError += OPEN_TABLE( T_EV_DESPRO, TT_HIST, ( char* )event_despro_hist, sizeof( struct _ev_despro ) );
        //tipo_t_ev_despro = TT_HIST;
		}
        /* && !USE_DB( AREA_FACTURA_HIST, _EV_FACTU_HIS, ( char* )event_factura,
                     sizeof( struct _ev_factu ), 0, 0, 0 )*/
		if( !nError ) {
			memset( event_factura_hist, 0, sizeof(struct _ev_factu) );
		nError += OPEN_TABLE( T_EV_FACTU, TT_HIST, ( char* )event_factura_hist, sizeof( struct _ev_factu ) );
        //tipo_t_ev_factu = TT_HIST;
		}
        /* && !USE_DB( AREA_EV_B_PROMO_HIST, _EV_B_PROMO_HIST_BTR, ( char* )event_b_promo_hist,
                     sizeof( struct _ev_bpromo ), 0, 0, 0 )){*/
		if( !nError ) {
			memset( event_b_promo_hist, 0, sizeof(struct _ev_bpromo) );
		nError += OPEN_TABLE( T_EV_B_PROMO, TT_HIST, ( char* )event_b_promo_hist, sizeof( struct _ev_bpromo ) );
        //tipo_t_ev_b_promo = TT_HIST;
		}
        if( !nError )
			iRta = 1;
			
		}
        else {
            iRta = 0;
        }
    return iRta;
}
/*****************************************************************************/
int CERRAR_ARCHIVOS_EVENTOS_HISTORICOS()
/*****************************************************************************/
{
    int iRta = 1;
    //CLOSE_DATABASE( AREA_TICKET_HIST );
	CLOSE_TABLE( T_EVENTOS, TT_HIST );
    //CLOSE_DATABASE( AREA_E_TICKET_HIST );
	CLOSE_TABLE( T_EV_CONT, TT_HIST);
	//CLOSE_DATABASE( AREA_PAGO_HIST );
	CLOSE_TABLE( T_PAGOS, TT_HIST);
    //CLOSE_DATABASE( AREA_COD_BARRA_HIST );
	CLOSE_TABLE(T_EV_COD_BARRA, TT_HIST);
    //CLOSE_DATABASE( AREA_CIERRE_HIST );
	CLOSE_TABLE( T_EV_CIERRE, TT_HIST );
    //CLOSE_DATABASE( AREA_DPAGO_HIST );
	CLOSE_TABLE( T_EV_DMEDIOS, TT_HIST );
    //CLOSE_DATABASE( AREA_SUPER_HIST );
	CLOSE_TABLE( T_EV_SUPER, TT_HIST );
    //CLOSE_DATABASE( AREA_E_DESPRO_HIST );
	CLOSE_TABLE( T_EV_DESPRO, TT_HIST );
    //CLOSE_DATABASE( AREA_FACTURA_HIST );
	CLOSE_TABLE( T_EV_FACTU, TT_HIST );
    //CLOSE_DATABASE( AREA_EV_B_PROMO_HIST );
    CLOSE_TABLE( T_EV_B_PROMO, TT_HIST );
    
    if( event_ticket_hist != NULL ) {
        FREEMEM( ( char* )event_ticket_hist );
        event_ticket_hist = NULL;
    }
    if( event_e_ticket_hist != NULL ) {
        FREEMEM( ( char* )event_e_ticket_hist );
        event_e_ticket_hist = NULL;
    }
    if( event_pago_hist != NULL ) {
        FREEMEM( ( char* )event_pago_hist );
        event_pago_hist = NULL;
    }
    if( event_cod_barra_hist != NULL ) {
        FREEMEM( ( char* )event_cod_barra_hist );
        event_cod_barra_hist = NULL;
    }
    if( event_cierre_hist != NULL ) {
        FREEMEM( ( char* )event_cierre_hist );
        event_cierre_hist = NULL;
    }
    if( event_dpago_hist != NULL ) {
        FREEMEM( ( char* )event_dpago_hist );
        event_dpago_hist = NULL;
    }
    if( event_super_hist != NULL ) {
        FREEMEM( ( char* )event_super_hist );
        event_super_hist = NULL;
    }
    if( event_despro_hist != NULL ) {
        FREEMEM( ( char* )event_despro_hist );
        event_despro_hist = NULL;
    }
    if( event_factura_hist != NULL ) {
        FREEMEM( ( char* )event_factura_hist );
        event_factura_hist = NULL;
    }
    if( event_b_promo_hist != NULL ) {
        FREEMEM( ( char* )event_b_promo_hist );
        event_b_promo_hist = NULL;
    }
    return iRta;
}

/*****************************************************************************/
int BORRA_EVENTO( long nro_z, long id_evento )
/*****************************************************************************/
{
    int error = 0;
    struct _eventos *ticket;
    struct _ev_cont *e_ticket;
    struct _ev_medios *pago;
    struct _ev_cod_barra *cod_barra;
    struct _ev_cierre *cierre;
    struct _ev_dmedios *dpago;
    struct _ev_super *super;
    struct _ev_despro *e_despro;
    struct _ev_factu *factura;
    struct _ev_bpromo *evento_promo;
	OBTIENE_PUNTEROS_A_EVENTOS( &ticket, &e_ticket, &pago, &cod_barra, &cierre, &dpago, &super,
                                &e_despro, &factura, &evento_promo, nro_z );

	if( POSICIONA_EN_TICKET( nro_z, id_evento ) ) {
        // ------------------------------------------------
        // PROCESA TICKET.
        // ------------------------------------------------
        if( ticket->tipo_evento == 9 || ticket->tipo_evento == 10 ) {
            if( POSICIONA_EN_CIERRE( ticket->nro_z, ticket->id_evento ) ) {
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                error = DELETE2();
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
        }
        else {
            // ------------------------------------------------
            // PROCESA E_TICKET.
            // ------------------------------------------------
            if( POSICIONA_EN_E_TICKET( ticket->nro_z, ticket->id_evento ) ) {
                while( e_ticket->nro_z == ticket->nro_z && e_ticket->id_evento == ticket->id_evento
                    && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                    error = DELETE2();
                }
            }
            if( !error ) {
                // ------------------------------------------------
                // PROCESA E_DESPRO.
                // ------------------------------------------------
                if( POSICIONA_EN_E_DESPRO( ticket->nro_z, ticket->id_evento ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    while( e_despro->nro_z == ticket->nro_z
                        && e_despro->id_evento == ticket->id_evento && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                        error = DELETE2();
                    }
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
            if( !error ) {
                // ------------------------------------------------
                // PROCESA FACTURA.
                // ------------------------------------------------
                if( POSICIONA_EN_FACTURA( ticket->nro_z, ticket->id_evento ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    while( factura->nro_z == ticket->nro_z
                        && factura->id_evento == ticket->id_evento && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                        error = DELETE2();
                    }
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
            if( !error ) {
                // ------------------------------------------------
                // PROCESA PAGOS.
                // ------------------------------------------------
                if( POSICIONA_EN_PAGO( ticket->nro_z, ticket->id_evento ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    while( pago->nro_z == ticket->nro_z && pago->id_evento == ticket->id_evento
                        && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                        error = DELETE2();
                    }
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
            if( !error ) {
                // ------------------------------------------------
                // PROCESA DPAGO.
                // ------------------------------------------------
                if( POSICIONA_EN_DPAGO( ticket->nro_z, ticket->id_evento ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    while( dpago->nro_z == ticket->nro_z && dpago->id_evento == ticket->id_evento
                        && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                        error = DELETE2();
                    }
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
            if( !error ) {
                // ------------------------------------------------
                // PROCESA SUPER.
                // ------------------------------------------------
                if( POSICIONA_EN_SUPER( ticket->nro_z, ticket->id_evento ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    while( super->nro_z == ticket->nro_z && super->id_evento == ticket->id_evento
                        && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                        error = DELETE2();
                    }
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
            if( !error ) {
                // ------------------------------------------------
                // PROCESA COD_BARRA.
                // ------------------------------------------------
                if( POSICIONA_EN_COD_BARRA( ticket->nro_z, ticket->id_evento ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    while( cod_barra->nro_z == ticket->nro_z
                        && cod_barra->id_evento == ticket->id_evento && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                        error = DELETE2();
                    }
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
            if( !error ) {
                // ------------------------------------------------
                // PROCESA EV_B_PROMO.
                // ------------------------------------------------
                if( POSICIONA_EN_EV_B_PROMO( ticket->nro_z, ticket->id_evento ) ) {
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    while( super->nro_z == ticket->nro_z && super->id_evento == ticket->id_evento
                        && /*!BtrvEOF()*/ !dbEOF() && !error ) {
                        error = DELETE2();
                    }
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                }
            }
        }
        POSICIONA_EN_TICKET( nro_z, id_evento );
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        DELETE2();
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
    }
    return !error;
}
/*****************************************************************************/
void OBTIENE_PUNTEROS_A_EVENTOS( struct _eventos **ticket, 
								 struct _ev_cont **e_ticket,
                                 struct _ev_medios **pago, 
								 struct _ev_cod_barra **cod_barra,
                                 struct _ev_cierre **cierre, 
								 struct _ev_super **super, 
								 struct _ev_dmedios **dpago,
                                 struct _ev_despro **e_despro, 
                                 struct _ev_bpromo **e_b_promo,
								 struct _ev_factu **factura,
							     long nro_z )
/*****************************************************************************/
{
    if( ticket != NULL ) {
        *ticket = ( nro_z == NRO_Z ) ? &event_ticket : event_ticket_hist;
    }
    if( e_ticket != NULL ) {
        *e_ticket = ( nro_z == NRO_Z ) ? &event_e_ticket : event_e_ticket_hist;
    }
    if( pago != NULL ) {
        *pago = ( nro_z == NRO_Z ) ? &event_pago : event_pago_hist;
    }
    if( cod_barra != NULL ) {
        *cod_barra = ( nro_z == NRO_Z ) ? &event_cod_barra : event_cod_barra_hist;
    }
    if( cierre != NULL ) {
        *cierre = ( nro_z == NRO_Z ) ? event_cierre : event_cierre_hist;
    }
    if( dpago != NULL ) {
        *dpago = ( nro_z == NRO_Z ) ? event_dpago : event_dpago_hist;
    }
    if( super != NULL ) {
        *super = ( nro_z == NRO_Z ) ? event_super : event_super_hist;
    }
    if( e_despro != NULL ) {
        *e_despro = ( nro_z == NRO_Z ) ? event_despro : event_despro_hist;
    }
    if( factura != NULL ) {
        *factura = ( nro_z == NRO_Z ) ? event_factura : event_factura_hist;
    }
    if( e_b_promo != NULL ) {
        *e_b_promo = ( nro_z == NRO_Z ) ? &event_b_promo : event_b_promo_hist;
    }
    
}
/*****************************************************************************/
void ACTUALIZA_PUNTOS_CLIENTES()
/*****************************************************************************/
{
    long cod= 0;
    double puntos= 0.00; 
    if( !GET_ERROR_EN_RED() && RAM_CLIENTE_AFINIDAD ) {
        if( EXISTE_CLIENTE( RAM_CLIENTE_AFINIDAD ) ) {
			long cliente_aux = 0;
            ///Si existe....
            if( clientes.cod_cliente != RAM_CLIENTE_AFINIDAD ) {
                //Me posiciono
                //SELECT( AREA_CLIENTES );
				cliente_aux = clientes.cod_cliente;
                cod = RAM_CLIENTE_AFINIDAD;
                //GET_EQUAL( ( char* )&cod );
				SELECT_TABLE( T_CLIENTES, TT_ORIG );
				SET_WHERE("COD_CLIENTE = %ld",cod);
				RUN_QUERY(SI);
            }
            puntos = RAM_ACU_PUNTOS;
            clientes.puntos += ( float )puntos;  //Acumulo
            if( RAM_ACU_PUNTOS_CUMPLE > 0.02 || RAM_HOY_CUMPLE_ANIOS == 2 ) {
                clientes.fec_entrega_ptos_cumple = _GET_FECHA_DOS();
            }
            SET_MEMORY_DOUBLE( __ram_acu_puntos_cumple, 0.0 );
            clientes.replicar = 'S';
            /*#ifdef INVEL_L
            BEGIN_TRANSACTION();
            #endif*/
            UPDATE2();
			if( RAM_NRO_CLIENTE == cliente_aux && cliente_aux != RAM_CLIENTE_AFINIDAD ) //me vuelvo a posicionar sobre el cliente
				EXISTE_CLIENTE( cliente_aux );
            /*#ifdef INVEL_L
            END_TRANSACTION();
            #endif*/
        }
    }
}
/***************************************************************************/
void REVERTIR_ARTICULO_DESCUENTO( void )
/***************************************************************************/
{
    double porc_desc = 0.0;
    long cliente = 0;
	long cliente_aux= 0;
    if( RAM_CLIENTE_AFINIDAD_ADICIONAL ) {
        porc_desc = RAM_DESCUENTO_AFINIDAD_ADICIONAL;
    }
    else {
        if( RAM_CLIENTE_AFINIDAD ) {
            cliente = RAM_CLIENTE_AFINIDAD;
			cliente_aux = RAM_NRO_CLIENTE;
        }
        else {
            cliente = RAM_NRO_CLIENTE;
        }
        if( EXISTE_CLIENTE( cliente ) ) {
            porc_desc = clientes.porc_desc;
        }
        else {
            porc_desc = 0;
        }
    }
    if( porc_desc < 100 ) {
        if( ( RAM_NRO_CLIENTE || RAM_CLIENTE_AFINIDAD ) && ( fabs( porc_desc ) > 0.0 ) ) {
            long id_evento = RAM_ID_EVENTO;
            if( BUSCAR_ARTICULO_DESCUENTO( id_evento ) ) {
                AGREGAR_DESCUENTO_PORC_CLIENTE( ( float ) - porc_desc );
            }
        }
    }
    else {
        MENSAJE_STRING( S_PORC_DE_DESCUENTO_INVALIDO );
    }
	if( RAM_NRO_CLIENTE == cliente_aux && cliente_aux != RAM_CLIENTE_AFINIDAD ) //me vuelvo a posicionar sobre el cliente
		EXISTE_CLIENTE( cliente_aux );
}
/***************************************************************************/
int BUSCAR_ARTICULO_DESCUENTO( long id_evento )
/***************************************************************************/
{
    int /*area_ant,*/ ok = 0;
    //area_ant = SELECTED();
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_E_TICKET );
    //GO( TOP );
    //GET_EQUAL( ( char* )&id_evento );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	SET_WHERE("ID_EVENTO = '%ld'",id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    while( event_e_ticket.id_evento == id_evento && !dbEOF() ) {
        if( event_e_ticket.cod_articulo == config.cod_descuento_cliente ) {
            ok = 1;
        }
        SKIP2( 1 );
    }
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return ( ok );
}
/****************************************************************************/
void PROCESA_DESCUENTOS_ESPECIALES()
/***************************************************************************/
{
    double porc_desc;
    long cliente, cliente_aux=0;
    if( RAM_CLIENTE_AFINIDAD_ADICIONAL ) {
        porc_desc = RAM_DESCUENTO_AFINIDAD_ADICIONAL;
    }
    else {
        if( RAM_CLIENTE_AFINIDAD ) {
            cliente = RAM_CLIENTE_AFINIDAD;
			if( RAM_NRO_CLIENTE)
				cliente_aux = RAM_NRO_CLIENTE;
        }
        else {
            cliente = RAM_NRO_CLIENTE;
        }
        if( EXISTE_CLIENTE( cliente ) ) {
            porc_desc = clientes.porc_desc;
        }
        else {
            porc_desc = 0;
        }
    }
    if( porc_desc < 100 ) {
        if( ( RAM_NRO_CLIENTE || RAM_CLIENTE_AFINIDAD ) && porc_desc ) {
            AGREGAR_DESCUENTO_PORC_CLIENTE( ( float )porc_desc );
        }
    }
    else {
        MENSAJE_STRING( S_PORC_DE_DESCUENTO_INVALIDO );
    }
	if( RAM_CLIENTE_AFINIDAD && RAM_NRO_CLIENTE && RAM_CLIENTE_AFINIDAD != RAM_NRO_CLIENTE ) { 
		// si hay cliente de facturacion cargado debemos volver a el si es distinto al afinidad
		EXISTE_CLIENTE( cliente_aux );
	}
}
/***************************************************************************/
void REVERSAR_PAGO_BOLETA_PLATA_EN_DEVOLUCIONES()
/***************************************************************************/
{
    int /*area_ant,*/i,j, tabla_ant, tipo_ant;
    double total = 0.0, cantidad = 0;
    long id_evento_act;
    struct   _eventos eventos;
    struct   _ev_cont ev_cont;
    struct   _ev_medios ev_pago;
    struct    _ev_despro ev_despro;
    struct   _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;
		tabla_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();

		if( RAM_MODO_DEVOLUCION && RAM_MODO_DEVOLUCION == _NC_TOTAL ) {
            if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                          &ev_cod_barra, &ev_b_promo ) ) {
                //SELECT( AREA_AUX8 );//dpago importado
				SELECT_TABLE( T_EVP_TMP, TT_ORIG );
				SET_WHERE("");
				RUN_QUERY(NO);
                while( /*!BtrvEOF()*/ !dbEOF() ) {
                    if( ev_dpago.accion == _GENERAR_BOLETIN ) {
                        INSERTAR_REVERSA_BOLETIN( ev_dpago, ev_dpago.importe );
                    }
                    SKIP2( 1 );
                }
            }
        }
        else {
            if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                          &ev_cod_barra, &ev_b_promo ) ) {
                //SELECT( /*AREA_AUX4*/ AREA_AUX ); //e_ticket importado
				SELECT_TABLE( T_EVC_TMP, TT_ORIG );
				SET_WHERE("");
				RUN_QUERY(NO);
	
                p_art_imp = RECCOUNT2(NO, NULL);
                if( p_art_imp ) {
                    art_imp = calloc( p_art_imp, sizeof( struct _art ) );
					SELECT_TABLE( T_EV_TMP, TT_ORIG );
					SET_WHERE("");
					RUN_QUERY(NO);
                    CONSOLIDAR_E_TICKET( eventos.id_evento, SELECTED2(), SELECTED_TIPO(), &ev_cont, art_imp,
                                         &p_art_imp );
                    //SELECT( AREA_E_TICKET );
					SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
					SET_WHERE("");
					RUN_QUERY(NO);
                    //p_art_act = RECCOUNT(  );
                    p_art_act = RAM_P_TICKET;
                    if( p_art_act ) {
                        id_evento_act = RAM_ID_EVENTO;
                        art_act = calloc( p_art_act, sizeof( struct _art ) );
                        CONSOLIDAR_E_TICKET( id_evento_act, SELECTED2(), SELECTED_TIPO(), &event_e_ticket, art_act,
                                             &p_art_act );
                    }
                }
                if( p_art_imp && p_art_act ) {
                    //SELECT( AREA_AUX8 );//dpago importado
                    //GO( TOP );
					SELECT_TABLE( T_EVP_TMP, TT_ORIG );
					SET_WHERE("");
					RUN_QUERY(NO);
                    while( /*!BtrvEOF()*/ !dbEOF() ) {
                        if( ev_dpago.accion == _GENERAR_BOLETIN ) {
                            total = 0.0;
                            for( i = 0;i < p_art_act;i++ ) {
                                cantidad = 0;
                                for( j = 0;j < p_art_imp;j++ ) {
                                    if( art_act[i].cod_articulo == art_imp[j].cod_articulo ) {
                                        cantidad = fabs( art_act[i].cantidad );
                                        //SELECT( AREA_AUX7 ); //e_des_pro importado
                                        //SET_ORDER( 4 );
                                        //GO( TOP );
										SELECT_TABLE(T_EVD_TMP, TT_ORIG );
										SET_WHERE("");
										RUN_QUERY(NO);
                                        while( /*!BtrvEOF()*/ !dbEOF() ) {
                                            if( art_imp[j].posicion == ev_despro.posicion
                                             && ev_despro.id_motivo == ev_dpago.nro_acumulador
                                             && ev_despro.tipo_motivo == MOTIVO_CONCEPTOS_SIN_DTOS ) {
                                                total += cantidad * ( ev_despro.importe
                                                                    / art_imp[j].cantidad );
                                            }
                                            SKIP2( 1 );
                                        }
                                    }
                                }//for e_ticket importado
                            }//for e_ticket actual
                            if( total > 0.0 ) {
                                INSERTAR_REVERSA_BOLETIN( ev_dpago, total );
                            }
                        }
                        //SELECT( AREA_AUX8 );
                        //SKIP( 1 );
						SELECT_TABLE( T_EVP_TMP, TT_ORIG );
						SKIP2( 1 );
                    }
                }
                if( art_imp ) {
                    free( art_imp );
                }
                if( art_act ) {
                    free( art_act );
                }
            }//if abrir eventos         
        }
        CERRAR_EVENTOS_TEMPORALES();
        //SELECT( area_ant );
		SELECT_TABLE( tabla_ant, tipo_ant );
}
/*****************************************************************************/
int CONSOLIDAR_E_TICKET( long id_eventos, int /*area*/ tabla, int tipo_t, struct _ev_cont *ev,
                         struct _art *art, int *p )
/*****************************************************************************/
{
    int rta = 1, renglon, i, encontro, _p_art = 0, p_art = *p;
    double precio;
    if( !art ) {
        rta = 0;
    }
    else {
        memset( art, 0, sizeof( struct _art ) * p_art );
        //SELECT( area );
		SELECT_TABLE( tabla, tipo_t );
        //SET_ORDER( 1 );
        //GO( TOP );
        //GET_EQUAL( ( char* )&id_eventos );
		SET_ORDER2("ID_EVENTO");
		SET_WHERE("ID_EVENTO = '%ld'", id_eventos );
		RUN_QUERY(NO);

        if( FOUND2() ) {
            for( renglon = 0;renglon < p_art && /*!BtrvEOF()*/ !dbEOF();renglon++ ) {
                if( ev->cod_movimiento != _DIFERENCIA_PRECIO
                 && ev->cod_movimiento != _COMPROMISO_200
                 && ev->cod_movimiento != _COMPROMISO_PRECIO_MAS_BAJO ) {
                    BUSCA_CODIGO_ESPECIAL( ev->cod_articulo );
                    encontro = 0;
                    i = 0;
                    while( i < _p_art && !encontro ) {
                        //--------------------- Calculo precio ------------------------
                        if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                            precio = ev->importe_sin_iva + ev->exento + ev->recargo_tarjeta;
                        }
                        else {
                            precio = ROUND( ev->importe_sin_iva + ev->exento + ev->iva1 + ev->ila
                                          + ev->iha + ev->recargo_tarjeta, 5, DECIMALES );
                        }
                        //------------------- Busco el artculo -----------------------
                        if( ev->cod_articulo == art[i].cod_articulo
                         && fabs( art[i].precio - ev->precio_unitario ) < 0.01 ) {
                            encontro = 1;
                        }
                        else {
                            i++;
                        }
                    }
                    //--------------------- Actualizo importes -----------------------
                    art[i].cantidad += ( ev->total < 0 ) ? -ev->cantidad : ev->cantidad;
                    //art[i].precio     += ev->precio_unitario;
                    art[i].total += ev->total;
                    if( !encontro ) {
                        //----------------- Copio datos fijos -------------------------
                        art[i].cod_articulo = ev->cod_articulo;
                        art[i].posicion = ev->posicion;
                        art[i].origen = ev->origen;
                        art[i].tipo = ev->tipo;
                        art[i].precio = ev->precio_unitario;
                        _p_art++;
                    }
                }
                //SELECT( area );
				SELECT_TABLE( tabla, tipo_t );
                SKIP2( 1 );
            }
            *p = _p_art;
        }
    }
    return ( rta );
}
/*****************************************************************************/
void INSERTAR_REVERSA_BOLETIN( struct _ev_dmedios dpago, double importe )
/*****************************************************************************/
{
    struct _ev_dmedios dpago_boletin;
    memcpy( &dpago_boletin, &dpago, sizeof( struct _ev_dmedios ) );
    dpago_boletin.caja = NRO_CAJA;
    dpago_boletin.nro_z = NRO_Z;
    dpago_boletin.id_evento = RAM_ID_EVENTO;
    dpago_boletin.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    dpago_boletin.cod_sucursal = config.sucursal;
	if( !NRO_FECHA_OPERACION )
		SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

	dpago_boletin.fecha_ticket = NRO_FECHA_OPERACION;
    dpago_boletin.importe = importe;
    dpago_boletin.accion = _USAR_BOLETIN;
    INSERTAR_EN_DPAGO( dpago_boletin );
}

/*****************************************************************************/
double BUSCA_PRECIO(int h)
/*****************************************************************************/
{
	int i;
	char cod[25];
	strncpy(cod,_TICKET_COD_BARRA(h),25);
	for(i = h ;i >= 0;i--){
		if(!strcmp(cod,_TICKET_COD_BARRA( i )) && 
			_TICKET_COD_MOVIMIENTO( i ) == _VENTA_NORMAL &&
			!(_TICKET_TIPO( i ) & _RESTA) 
            && _TICKET_ORIGEN( i ) == _ORIGEN_TICKET){
			return _TICKET_PRECIO( i );
		}
	}
	return 0;
}

/*****************************************************************************/
void CALCULA_AHORRO_OBTENIDO( void )
/*****************************************************************************/
{
    int h;
    double signo;
    SET_MEMORY_DOUBLE( __ram_ahorro_obtenido, 0.0 );
    for( h = 0;h < RAM_P_TICKET;h++ ) {
        if( ( _TICKET_TIPO( h ) & _CREDITO ) /*&& EXCLUIR_ART_AFECTADOS_POR_DESC*/ 
            && !( _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO 
                || _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO_ANULADA
                || _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PESOS
				|| _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PORC
				|| _TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION
                || MODO_DEVOLUCION ) ) {
            signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
            ADD_MEMORY_DOUBLE( __ram_ahorro_obtenido,
                               signo * _TICKET_CANTIDAD( h )
                             * ROUND( ( _TICKET_PRECIO_ORIGINAL( h ) - /*_TICKET_PRECIO( h )*/ BUSCA_PRECIO( h ) ) , ENTEROS, DECIMALES ) );
        }
		/*-------------- Descarga de Articulos en Promocion --------------*/
	
		if( ( _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PESOS
				||_TICKET_ORIGEN( h ) == _ORIGEN_DESC_PORC
				|| _TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION
				|| _TICKET_ORIGEN( h ) == _ORIGEN_DESC_PORC_FIJO
				|| _TICKET_ORIGEN( h ) == _ORIGEN_DESC_TARJETA )
			&& !( _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO 
				|| _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO_ANULADA 
				|| MODO_DEVOLUCION ) ) {
				
			//Viene en negativo el precio de los articulos descarga
			signo = ( _TICKET_TIPO( h ) & _RESTA ) ? 1 : -1;
			ADD_MEMORY_DOUBLE( __ram_ahorro_obtenido,
					signo * _TICKET_CANTIDAD( h )
					* ROUND( _TICKET_PRECIO( h ) , ENTEROS, DECIMALES ) );
		}
	}

}

/*****************************************************************************/
int EJECUTAR_TAREAS_MOTOR( int tipo_tarea, int lugar_impresion, int asignar  )
/*****************************************************************************/
{
    struct asig_beneficio beneficios;
    int cant = 0;
    int cnt = 0;
    int cortar = 0;
    int valid_monedero;
	int station= 0;
    int rta = 1;
    double monto_monedero = 0;
    char msg_string_monedero[256];
    int vigencia_monedero = 0;
    if((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_ASIG_BENEFICIO,sizeof( struct asig_beneficio ))) > 0){
        while(cnt < cant){
            memset(&beneficios,0,sizeof( struct asig_beneficio ));
            RECUPERAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO,(char *)&beneficios,sizeof(struct asig_beneficio),cnt);
            if(beneficios.asignado == 0 && tipo_tarea == beneficios.tipo){
                switch( beneficios.tipo ){
                    case ASIG_BENEF_BONO:
                        if(beneficios.lugar_impresion == lugar_impresion ){
                            IMPRIME_CUPON_PROMOCION( beneficios.nro_validacion, beneficios.monto,
                                         0,beneficios.cantidad, 0, beneficios.msg_string, beneficios.vigencia );
                            if(beneficios.lugar_impresion == FUERA_DEL_TICKET){
                                PRN_FIN_TICKET();
                            }else{
                                LF(2);
                            }
                            beneficios.asignado = 1;
                        }
                        break;
                    case ASIG_BENEF_VALE:
                        beneficios.asignado = 1;
                        break;
                    case ASIG_BENEF_STR_TICKET:
                        /*si esta en el lugar correcto de impresion*/
                        if(beneficios.lugar_impresion == lugar_impresion){
                            if(beneficios.msg_string){
                                int i = 0;
                                int cnt = 0;
                                char buffer[256];
                                TKT_SET_FILA( 1 );
                                memset( buffer, 0, sizeof(buffer));
								//Si se activan promociones en factura no debe imprimir por receipt
								if( RAM_FACTURA ) {
									getData( ACTION_GET_PRINT_STATION, ( char* )&station, 0 );
									PRN_SELECT( JOURNAL );
								}
                                for( i=0; i < (int)strlen(beneficios.msg_string); i++ ){
                                    if(beneficios.msg_string[i] == '&'){
                                        memset( buffer, 0, sizeof(buffer));
                                        strncpy( buffer, &beneficios.msg_string[cnt], i-cnt);
                                        if(strlen(buffer) > 0){
                                            IMPRIMIR_CADENA( 1, buffer );
                                            LF(1);
                                        }
                                        cnt = i + 1;
                                    }else if( (beneficios.msg_string[i] < 45 || beneficios.msg_string[i] > 175) && beneficios.msg_string[i] != 32){
                                        beneficios.msg_string[i] = '?';
                                    }
                                }
                                memset( buffer, 0, sizeof(buffer));
                                strncpy( buffer, &beneficios.msg_string[cnt], i-cnt);
                                if(strlen(buffer) > 0){
                                    IMPRIMIR_CADENA( 1, buffer );
                                    LF(1);
                                }
								if( RAM_FACTURA ) {
									PRN_SELECT( station );
								}
                            }
                            if( beneficios.lugar_impresion == FUERA_DEL_TICKET )
                                cortar = 1;
                            beneficios.asignado = 1;
                        }
                        break;
                    case ASIG_BENEF_MONEDERO:
                        /*no lo marca como asignado, porque tiene que sumar todo lo asignado a monedero 
                          y luego carga todo junto, al final de la funcion lo marca como asignado*/
                        monto_monedero += beneficios.monto;
                        valid_monedero = beneficios.nro_validacion;
                        strcpy(msg_string_monedero, beneficios.msg_string);
                        vigencia_monedero = beneficios.vigencia;
                        break;
                }
                if(beneficios.asignado == 1){
                    /*lo marca como asignado*/
                    ACTUALIZAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO,(char *)&beneficios,sizeof(struct asig_beneficio),cnt);
                }
            }
            cnt++;
        }
    }
    if( asignar == 1 && monto_monedero > 0 ){
        char tmp[15];
        char msg[100];
        int salir = 0;
        int cargar = 1;
        while(!salir){
            cargar = 1;
        if( strlen( RAM_CUENTA_CLIENTE ) ) { //si hay cliente monedero cargado verifico si la tarjeta
            LEE_TARJETAS( 0, TARJETA_MONEDERO ); // esta habilitada para promo.
			NUMERO_DE_COMERCIO_TARJETA(  );
			if( TARJ[VAR_TARJETAS_TARJETA].hab_promo == 'N' ) {
				rta = 0;
				MENSAJE( "TARJETA NO HABILITADA PARA PROMOCIONES" );
			}
		} else if( !CARGAR_MONEDERO_ELECTRONICO( SI, "PASE TARJETA MONEDERO PARA OTORGAR PROMOCION" ) ) { //si no hay cliente monedero cargado pido tarjeta
			rta = 0;
		}
		if( !rta ) {
			    while( LASTKEY() != 27 && !(rta = CARGAR_MONEDERO_ELECTRONICO( SI, "PASE TARJETA MONEDERO PARA OTORGAR PROMOCION" )) ) {
				    /*MENSAJE_CON_TECLA_S_N( ST( S_ERROR_DE_LECTURA_ENTER_REINTENTAR_ESC_CANCELAR ),
					    13, 27 );*/
			}
		}
        if( LASTKEY(  ) == 27 || rta == 0 ) {
            /*sino cargo el cliente monedero*/
                if(MENSAJE_CON_TECLA_S_N("ANULAR PROMOCION A MONEDERO? <ENTER>=SI <ESC>=NO",13,27) == 13){
                    MENSAJE( "ANULANDO PROMOCION A MONEDERO..." );
                    cargar = 0;
                    rta = 0;
                    salir = 1;
                    //cambia el beneficio para los eventos, indicando que no se asigno
                    CAMBIAR_BENEFICIO( BENEF_MONEDERO, BENEF_MONEDERO_NO_ASIG );
                }else{
                    salir = 0;
                    rta = 1;
                }
            }else{
                salir = 1;
            }
        }
        if( cargar ){
            double importeRnd = 0.0;
            FTOC( monto_monedero, tmp, ENTEROS + 1, DECIMALES, 0, 0 );
            LTRIM(tmp);
            _snprintf(msg, sizeof(msg)-1,"ASIGNANDO %s A MONEDERO...", tmp);
            MENSAJE(msg);
            importeRnd = ROUND( monto_monedero,ENTEROS + 1, 2 );    
            /*analizar respuesta....*/
            if( !ENVIAR_IMPORTE_MONEDERO( importeRnd,  TARJETA_MONEDERO ) ){
                MENSAJE("NO HAY CONEXION. IMPRIMIENDO VALE...");
                /*si falla imprimimos un bono de la validacion enviada por el motor*/
                IMPRIME_CUPON_PROMOCION( valid_monedero, importeRnd,
                                0,0,0, msg_string_monedero, vigencia_monedero );
                //cambia el beneficio para los eventos
                CAMBIAR_BENEFICIO(BENEF_MONEDERO, BENEF_BONOS);
			} else {
                if( TKT_CONFIGURABLE 
                    && ( strcmp( RAM_CUENTA_CLIENTE, "" ) != 0 ) ) {
				IMPRIME_TKT_DESCRIPCION_TARJETA( 1,1 );
			}
        }
        }
        /*marca todas la asignaciones monedero*/
        MARCAR_ASIGNADO( 0,ASIG_BENEF_MONEDERO );
    }
    if(cortar){
        PRN_FIN_TICKET();   
    }
    return rta;
    
}

/*****************************************************************************/
void MARCAR_ASIGNADO(int pos, int tipo_beneficio)
/*****************************************************************************/
{
    /*si se pasa parametro tipo_beneficio, se marcan todos los beneficios, si se pasa pos, solo se marca esa pos*/
    struct asig_beneficio beneficios;
    int cant = 0, cnt = 0;
    if( tipo_beneficio && pos == 0){
        if((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_ASIG_BENEFICIO,sizeof( struct asig_beneficio ))) > 0){
            while(cnt < cant){
                memset(&beneficios,0,sizeof( struct asig_beneficio ));
                RECUPERAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO,(char *)&beneficios,sizeof(struct asig_beneficio),cnt);
                if( tipo_beneficio ==  beneficios.tipo){
                    /*lo marca como asignado*/
                    beneficios.asignado = SI;
                    ACTUALIZAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO,(char *)&beneficios,sizeof(struct asig_beneficio),cnt);
                }
                cnt++;
            }
        }
    }else{
        /*solo marca el pos*/
        memset(&beneficios,0,sizeof( struct asig_beneficio ));
        RECUPERAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO,(char *)&beneficios,sizeof(struct asig_beneficio),pos);
        /*lo marca como asignado*/
        beneficios.asignado = SI;
        ACTUALIZAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO,(char *)&beneficios,sizeof(struct asig_beneficio),pos);
    }
}
/*****************************************************************************/
void CAMBIAR_BENEFICIO(int benef_ariginal, int benef_nuevo)
/*****************************************************************************/
{
    struct eventos_promociones motor_event_promo;
    int cant = 0, cnt = 0;
    if((cant = CANTIDAD_REGISTROS_REC(ARCH_REC_EVENT_PROMO,sizeof(struct eventos_promociones))) > 0){
        while(cnt < cant){
            memset(&motor_event_promo,0,sizeof( struct eventos_promociones ));
            RECUPERAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO,(char *)&motor_event_promo,sizeof(struct eventos_promociones),cnt);
            if(motor_event_promo.b_promo == benef_ariginal){
                //cambia el beneficio
                motor_event_promo.b_promo = benef_nuevo;
                ACTUALIZAR_ARCHIVO_REC(ARCH_REC_EVENT_PROMO, (char *)&motor_event_promo,sizeof(struct eventos_promociones),cnt);
            }
            cnt++;
        }
    }
}

/*****************************************************************************/
int AGRUPAR_ARTICULOS_TRANSACCION_ACTIVA( void )
/*****************************************************************************/
{
   struct _ev_cont *e_ticket_agrupados, *tmp;
   int error = 0, /*area_ant = 0,*/ j = 0, tipo_ant, tabla_ant;
   //char barra1[17], barra2[17];
   long id_evento = RAM_ID_EVENTO;

   /*struct {
		long caja_z;
		long id_evento;
		int posicion;
	} indice3;*/
   //long caja_z;
      
   int registro = 0, i = 0, h = 0, _signo = 0, p_art = 0, encontro = 0;   
   
   if( ( e_ticket_agrupados = MALLOC( sizeof( struct _ev_cont ) ) ) ) {
      if( !( tmp = MALLOC( sizeof( struct _ev_cont ) * RAM_P_TICKET ) ) ) {
         return ( error );
      }
   } else {
       return ( error );
   }

   //area_ant = SELECTED(  );
   tabla_ant = SELECTED2();
   tipo_ant = SELECTED_TIPO();
   
   GRABAR_LOG_SISTEMA( "AGRUPANDO ARTICULOS",LOG_VENTAS,3 );
   //COMENTO HASTA QUE SE ARREGLE
   /*if( OPENED( T_E_TICKET_AGRUPADOS, TT_ORIG ) ) {
       CLOSE_TABLE( T_E_TICKET_AGRUPADOS, TT_ORIG );
   }
   if( OPEN_TABLE( T_E_TICKET_AGRUPADOS, ( char * ) e_ticket_agrupados, sizeof( struct _ev_cont ) ) == 0 ) {
      SELECT_TABLE( T_E_TICKET_AGRUPADOS, TT_ORIG );
      DIRECT_DB_QUERY( "truncate table %s", "e_ticket_agrupados" );
      memset( e_ticket_agrupados, 0, sizeof( struct _ev_cont ) );
      memset( tmp, 0, sizeof( struct _ev_cont ) * RAM_P_TICKET );
    
	  caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );                       
	  //indice3.id_evento = id_evento;
	  //indice3.posicion = 1;
      //SELECT( AREA_E_TICKET );
      //GO( TOP );
	  //SET_ORDER( 3 );
      //GET_EQUAL( ( char* )&indice3 );
	  SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	  SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION='%d'", caja_z, id_evento, 1 );
	  RUN_QUERY(NO);
      
      if( !FOUND2( ) ) {
          return ( error );
      }
            
      h = 0;
      for( registro = 0; registro < RAM_P_TICKET; registro++ ) {         
         encontro = 0;
         i = 0;
         strncpy( barra1, _TICKET_COD_BARRA( h ), sizeof( barra1 )-1 );
         strncpy( barra2, _TICKET_COD_BARRA_ORIGINAL( h ), sizeof( barra2 ) - 1 );
         _signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
         if( ( ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO ) || 
			 //( fabs( fmod( _TICKET_CANTIDAD( h ), 1.00 ) ) > 0.0005 )
			 !DEBE_AGRUPAR_ARTICULOS_CON_DECIMALES( h )
                   || strncmp( barra1, barra2, sizeof( barra1 ) ) ) && _signo > 0 ) {
            i = p_art;
         }
         
         while( i < p_art && !encontro ) {
            //------------------- Busco el artculo -----------------------
            if( event_e_ticket.cod_articulo == tmp[i].cod_articulo                             
               && ( strncmp( event_e_ticket.cod_barra, tmp[i].cod_barra, sizeof( event_e_ticket.cod_barra ) ) == 0
               || BUSQUEDA_DE_ARTICULOS != BUSQUEDA_COD_INT_Y_COD_BARRA ) ) {
               encontro = 1;
               //if( ( ( _TICKET_TIPO( h ) & _DEV_ENVASE ) && !( _TICKET_TIPO( tmp[i].pos_ticket ) & _DEV_ENVASE ) ) ) {
               //   encontro = 0;
               //   i++;
               //}
            }
            else {
               i++;
            }
         }              

         //--------------------- Actualizo importes -----------------------
         tmp[i].cantidad += event_e_ticket.cantidad;
         tmp[i].importe_sin_iva += event_e_ticket.importe_sin_iva;         
         tmp[i].imp_int += event_e_ticket.imp_int;
         tmp[i].exento += event_e_ticket.exento;
         tmp[i].iva1 += event_e_ticket.iva1;
         tmp[i].iva2 += event_e_ticket.iva2;
         tmp[i].precio_promocion += event_e_ticket.precio_promocion;
         tmp[i].total += event_e_ticket.total;
         tmp[i].descuento += event_e_ticket.descuento;
         for( j = 0;j < 4;j++ ) {
            tmp[i].cod_impuesto[j] = event_e_ticket.cod_impuesto[j];
            tmp[i].monto_impuesto[j] += event_e_ticket.monto_impuesto[j];
         }         

         if( !encontro ) {
            //----------------- Copio datos fijos -------------------------
            tmp[i].id_evento = event_e_ticket.id_evento;
            tmp[i].cod_articulo = event_e_ticket.cod_articulo;
            tmp[i].cant_presentacion = event_e_ticket.cant_presentacion;
            tmp[i].costo = event_e_ticket.costo;
            tmp[i].recargo_tarjeta = event_e_ticket.recargo_tarjeta;
            strncpy( tmp[i].cod_barra, event_e_ticket.cod_barra, sizeof( tmp[i].cod_barra ) - 1 );
            tmp[i].cod_presentacion = event_e_ticket.cod_presentacion;
            tmp[i].tipo_precio = event_e_ticket.tipo_precio;
            tmp[i].nro_vendedor = event_e_ticket.nro_vendedor;         
            tmp[i].stockeable = event_e_ticket.stockeable;
            tmp[i].origen = event_e_ticket.origen;
            tmp[i].tipo = event_e_ticket.tipo;
            tmp[i].habilitado_cuotas = event_e_ticket.habilitado_cuotas;
            tmp[i].cod_iva = event_e_ticket.cod_iva;
            tmp[i].cod_movimiento = event_e_ticket.cod_movimiento;
            tmp[i].precio_unitario = event_e_ticket.precio_unitario;
            tmp[i].caja_z = event_e_ticket.caja_z;
            tmp[i].caja = event_e_ticket.caja;
            tmp[i].nro_z = event_e_ticket.nro_z;         
            tmp[i].tipo2 = event_e_ticket.tipo2;
            tmp[i].cod_depto = event_e_ticket.cod_depto;
            tmp[i].comprobante = event_e_ticket.comprobante;
            tmp[i].motivo = event_e_ticket.motivo;
            tmp[i].cod_sucursal = event_e_ticket.cod_sucursal;
            tmp[i].fecha_ticket = event_e_ticket.fecha_ticket;
            tmp[i].hab_desc_cliente = event_e_ticket.hab_desc_cliente;
            strncpy( tmp[i].nro_vendedors, event_e_ticket.nro_vendedors, sizeof( tmp[i].nro_vendedors ) - 1 );
            tmp[i].tipo3 = event_e_ticket.tipo3;
            tmp[i].participa_donacion = event_e_ticket.participa_donacion;
            tmp[i].precio_original = event_e_ticket.precio_original;
            strncpy( tmp[i].cod_articulo_alfa, event_e_ticket.cod_articulo_alfa, sizeof( tmp[i].cod_articulo_alfa ) - 1 );
            strncpy( tmp[i].cod_clasificacion, event_e_ticket.cod_clasificacion, sizeof( tmp[i].cod_clasificacion ) - 1 );
            tmp[i].excluir_promocion = event_e_ticket.excluir_promocion;   
            
            // la posicion es diferente
            tmp[i].posicion = i+1;          

            p_art++;
         }
         //SELECT( AREA_E_TICKET );
         //SKIP( 1 );
		 SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		 SKIP2( 1 );
         h++;
     }
     SELECT_TABLE( T_E_TICKET_AGRUPADOS, TT_ORIG );
     for( j=0; j<p_art; j++ ) {
        memcpy( e_ticket_agrupados, &tmp[j], sizeof( struct _ev_cont ) );
        error = INSERT2( SI );
     }
     if( error ) {         
         DIRECT_DB_QUERY( "truncate table %s", "e_ticket_agrupados" );
     }
     CLOSE_TABLE( T_E_TICKET_AGRUPADOS, TT_ORIG );
   }*/
   FREEMEM( ( char * ) e_ticket_agrupados );
   FREEMEM( ( char * ) tmp );
   //SELECT( area_ant );
   SELECT_TABLE( tabla_ant, tipo_ant );
   return ( error );
}
/*****************************************************************************/
int CONSOLIDAR_E_TICKET_SQL( long id_eventos, int table, int tipo, struct _ev_cont *ev,
                         struct _art *art, int *p )
/*****************************************************************************/
{
    int rta = 1, renglon, i, encontro, _p_art = 0, p_art = *p;
    double precio;
    if( !art ) {
        rta = 0;
    }
    else {
        memset( art, 0, sizeof( struct _art ) * p_art );
        SELECT_TABLE( table, tipo );
        //SET_ORDER2( 1 );
        //GO( TOP );
        //GET_EQUAL( ( char* )&id_eventos );
		SET_WHERE("ID_EVENTO = '%ld'",id_eventos);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
        if( FOUND2() ) {
            for( renglon = 0;renglon < p_art && !dbEOF();renglon++ ) {
                if( ev->cod_movimiento != _DIFERENCIA_PRECIO
                 && ev->cod_movimiento != _COMPROMISO_200
                 && ev->cod_movimiento != _COMPROMISO_PRECIO_MAS_BAJO ) {
                    BUSCA_CODIGO_ESPECIAL( ev->cod_articulo );
                    encontro = 0;
                    i = 0;
                    while( i < _p_art && !encontro ) {
                        /*--------------------- Calculo precio ------------------------*/
                        if( ES_FACTURA_A( comp.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                            precio = ev->importe_sin_iva + ev->exento + ev->recargo_tarjeta;
                        }
                        else {
                            precio = ROUND( ev->importe_sin_iva + ev->exento + ev->iva1 + ev->ila
                                          + ev->iha + ev->recargo_tarjeta, 5, DECIMALES );
                        }
                        /*------------------- Busco el artculo -----------------------*/
                        if( ev->cod_articulo == art[i].cod_articulo
                         && fabs( art[i].precio - ev->precio_unitario ) < 0.01 ) {
                            encontro = 1;
                        }
                        else {
                            i++;
                        }
                    }
                    //--------------------- Actualizo importes -----------------------
                    art[i].cantidad += ( ev->total < 0 ) ? -ev->cantidad : ev->cantidad;
                    //art[i].precio     += ev->precio_unitario;
                    art[i].total += ev->total;
                    if( !encontro ) {
                        /*----------------- Copio datos fijos -------------------------*/
                        art[i].cod_articulo = ev->cod_articulo;
                        art[i].posicion = ev->posicion;
                        art[i].origen = ev->origen;
                        art[i].tipo = ev->tipo;
                        art[i].precio = ev->precio_unitario;
                        _p_art++;
                    }
                }
                SELECT_TABLE( table, tipo );
                SKIP2( 1 );
            }
            *p = _p_art;
        }
    }
    return ( rta );
}

