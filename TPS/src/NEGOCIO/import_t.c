#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h>
#include <cio.h>
#include <clave.h>
#include <cmemory.h>
#include <cr.h>
#include <_cr0.h>
#include <cr_disco.h>
#include <dbrouter.h>
#include <eventos.h>
#include <get_fech.h>
#include <import_t.h>
#include <imp_item.h>
#include <imp_tick.h>
#include <malloc.h>
#include <mensaje.h>
#include <mstring.h>
#include <m_cadena.h>
#include <pagos.h>
#include <pant.h>
#include <p_fecha.h>
#include <recupero.h>
#include <round.h>
#include <stack.h>
#include <tkt.h>
#include <_cr1.h>
#include <bridge.h>
#include <actions.h>
#include <math.h>
#include <comprob.h>
#include <path.h>
#include <deftables.h>
#include <db.h>
#include <fecha.h>
#include <cr_pant.h>
#include <cobros.h>
#include <ventas2.h>
#include <log.h>
#include <t_espera.h>
#include <htoa.h>
#include <menu.h>
#include <day.h>
#include "ini.h"
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef EOF
#undef MK_FP
#ifdef INVEL_W
#include <dos.h>
#include <sys/stat.h>
#endif
#ifdef INVEL_L
#include <sys/stat.h>
#endif
#include <bridge.h>
#include <clientes.h>
int _ticket_caja;
int _ticket_fecha;
int _ticket_tipo_comp;
long _ticket_n_ticket;
long _ticket_secuencia;
long _ticket_id_evento;

#define _TICKET_CAJA           GET_MEMORY_INT( _ticket_caja )
#define _TICKET_FECHA          GET_MEMORY_INT( _ticket_fecha )
#define _TICKET_TIPO_COMP      GET_MEMORY_INT( _ticket_tipo_comp )
#define _TICKET_N_TICKET       GET_MEMORY_LONG( _ticket_n_ticket )
#define _TICKET_SECUENCIA      GET_MEMORY_LONG( _ticket_secuencia )
#define _TICKET_ID_EVENTO      GET_MEMORY_LONG( _ticket_id_evento )

#define _X1   7
#define _X2  30
#define _Y1   9
#define _Y2  18
long id_variacion;
int en_importacion_de_ticket = 0;
int articulos = 0, __cliente = 0;
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_ev_cod_barra, tipo_ev_dpago, tipo_t_ev_b_promo;
extern int ram_diferencia_precio;
int PEDIR_TICKET_EN_ESPERA( void );
extern void GRABAR_B_PROMO(struct _ev_bpromo promocionados);
extern int DEBE_PEDIR_PRECIO_DEVOL_Y_NC( int operacion );
void SETEOS_DE_COMPROBANTE_ORIGINAL( int error );
int CUANTOS_TICK_ESPERA( long secuencia, int estado );
int CONTAR_TICKET_ESPERA_SIN_PROCESAR(int ev_requerido, int caja, long nro_z, long secuencia );
extern char * GET_WHERE(  );
extern double TRUNCAR(double f, int n) ;
void MUESTRA_PERFIL();
/*****************************************************************************/
int IMPORTAR_TICKET( int remoto, int ev_requerido, int reimpresion, char *titulo,
                     int pedir_supervisor, char origen, int modo ,int comprob_original, int dias_limites)
/*****************************************************************************/
{
    int pant = 0, cant_tick_espera = 0, online = 0, tecla = 0;
    int _ptr = 0, _handle = 0, _estado = 0, pedir_datos = 1, bultos_ticket = 0;
    int error = 0, secuencia = 0;
    int restore_screen = 0;
	int fecha = 0;
    long id_evento = 0, nro_ticket = 0;
	char mensaje[100];

    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    /*  _estado = STACK_INS( sizeof( int ) );
        _ticket_caja = STACK_INS( sizeof( int ) );
        _ticket_fecha = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
    
	_ticket_caja = STACK_INS( sizeof( short ) );
    _ticket_fecha = STACK_INS( sizeof( short ) );
	_ticket_tipo_comp = STACK_INS( sizeof( short ) );
    _ticket_n_ticket = STACK_INS( sizeof( long ) );
    _ticket_secuencia = STACK_INS( sizeof( long ) );
	_ticket_id_evento = STACK_INS( sizeof( long ) );
    en_importacion_de_ticket = 1;

    //--- PRUEBA
    //_SET_ESTADO( 1 );
    if( PEDIR_DATOS_EN_DEVOLUCIONES && RAM_MODO_DEVOLUCION &&
		( !PEDIR_CAJA_Y_SECUENCIA || origen != _ORIGEN_TICKET_EN_ESPERA ) ) {
        if( RAM_SUCURSAL_ANTERIOR == config.sucursal ) {
            SET_MEMORY_INT( _ticket_caja, RAM_CAJA_ANTERIOR );
            SET_MEMORY_INT( _ticket_fecha, RAM_FECHA_ANTERIOR );
			if( config.pais != MEXICO ) {
				SET_MEMORY_LONG( _ticket_n_ticket, RAM_BOLETA_ANTERIOR );
			} else {
				SET_MEMORY_LONG( _ticket_secuencia, RAM_BOLETA_ANTERIOR );
			}
            pedir_datos = 0;
        }
        else {
            error = 1;
            _SET_ESTADO( 99 );
            MENSAJE_STRING( S_EL_COMP_ORIGINAL_NO_PERTENECE_A_LA_SUCURSAL );
        }
    }
    if( _ESTADO == 0 ) {
        if( SOLICITAR_CLAVES( 30 - ancho_panel, 21, NO, SI, pedir_supervisor, titulo, NULL,
                              RAM_COD_FUNCION, SI ) ) {
            //PANTALLA_IMPORTAR_T( titulo );
            if( pedir_datos ) {
                if( PIDE_DATOS_IMPORTAR_T( remoto, titulo, modo  ) ) {//__dias_en_importacion_en_devolucio
					if(_TICKET_TIPO_COMP > 0 )//PEDI UN COMPROBANTE ESPECIAL
						ev_requerido = ( int )  _TICKET_TIPO_COMP;
					
					fecha = _TICKET_FECHA;
					

					if( dias_limites > 0 && !( fecha == 0xffff || !fecha )){
						
						int dif_dias = DIF_DIAS(  GET_FECHA_DOS(),fecha );
						DAY(GET_FECHA_DOS());
						
						if( dif_dias > dias_limites ) {
							sprintf( mensaje, "%s %d DIAS PREVIOS",ST( S_SOLO_IMPORTACIONES_HASTA ) ,dias_limites );
							MENSAJE( mensaje );
							error = 1;
							//SI HACE FALTA SUPERVISOR PARA AUTORIZAR ESTO PONER ACA.
						}
						
					}
					if( !error )
						_SET_ESTADO( 1 );
                }
                else {
                    error = 1;
                }
            }
            else {
                _SET_ESTADO( 1 );
            }
        }
        else {
            error = 1;
        }
    }
    if( _ESTADO == 1 ) {
		/*Se verifica si es OFFLINE o ONLINE la importacion del ticket*/
		if( origen == _ORIGEN_TICKET_EN_ESPERA ) {
				online = 1;
		} else {
			//online = 0;
			online = 1;
		}
        if( !remoto ) {
            error = ABRIR_ARCHIVOS_EVENTOS_LOCAL(  );
        }
        else {
			if( origen == _ORIGEN_TICKET_EN_ESPERA ) {
                //Si se obtuvo por menu, sabemos el id_evento del evento a importar
                if( modo == _TE_CON_MENU ) {
                    id_evento = _TICKET_ID_EVENTO;
                }
				if( online ) {
					/*Indica si esta en la base el ticket en espera ESPECIFICO no necesariamente de la misma caja*/
					if( modo == _TE_TRADICIONAL ) {
						if( PEDIR_CAJA_Y_SECUENCIA ) {
							if( IMPRESORA_FISCAL == FISCAL ) { //LA TM220 NO IMPRIME SECUENCIA EN UN TICKET ESPERA
								nro_ticket = _TICKET_N_TICKET;
								fecha = _TICKET_FECHA;
							} else {
								secuencia = _TICKET_SECUENCIA;
							}
						} else {
							secuencia = _TICKET_SECUENCIA;
						}
					} else 
						secuencia = -1;

					cant_tick_espera = CONTAR_TICKET_ESPERA( ev_requerido,
							_TICKET_CAJA, NRO_Z, secuencia, nro_ticket, fecha );
					if( cant_tick_espera == 0 && origen == _ORIGEN_TICKET_EN_ESPERA && _TICKET_CAJA == NRO_CAJA ) {
						/*Verifico si el ticket en espera solicitado no ha sido enviado a la BD.
						Si fue enviado y no se encontro en la base significa que una Caja levanto ese ticket
						Si no fue enviado significa que la misma caja levanto el ticket o aun esta en espera */
                        GRABAR_LOG_SISTEMA("NO SE ENCONTRO EL TICKET A IMPORTAR EN EL SERVIDOR O YA FUE PROCESADO - SE PASA A LOCAL", LOG_VENTAS,3 );
						//if( VERIFICAR_ESTADO_TICK_ESP( _TICKET_CAJA, _TICKET_SECUENCIA, OFF ) ) {
						if( cant_tick_espera = CONTAR_TICK_ESP_SIN_PROCESAR( _TICKET_SECUENCIA, ON_OFF ) ) {
							online = 0;
							if( cant_tick_espera > 0 ) {
								cant_tick_espera = 1;
							}
						}
					}
				} else {
					/*Indica la cantidad de ticket en espera local*/
					cant_tick_espera = CONTAR_TICK_ESP_SIN_PROCESAR( _TICKET_SECUENCIA, ON_OFF );
				}
			}
			fecha = _TICKET_FECHA;
			if( origen != _ORIGEN_TICKET_EN_ESPERA || cant_tick_espera ) {
				error = OBTENER_EVENTOS_TEMPORALES( ev_requerido, _TICKET_CAJA, _TICKET_N_TICKET,
                        _TICKET_SECUENCIA, _TICKET_FECHA, id_evento, origen, online, NULL,comprob_original );
				bultos_ticket = RAM_BULTOS;
				SET_MEMORY_INT( __ram_bultos, 0 );
            }else{
                MENSAJE_STRING( S_TICKET_NO_ENCONTRADO );
                error = 1;
            }
            //--- PRUEBA
            //error = 0;
        }
		if( !error ) {
            if( !remoto && !BUSCA_TICK_ALL( _TICKET_FECHA, _TICKET_N_TICKET ) ) {
                MENSAJE_STRING( S_TICKET_NO_ENCONTRADO );
                error = 1;
            }
            else {
                if( VERIFICA_DATOS_IMPORTACION( remoto, ev_requerido, _TICKET_FECHA,
                                                _TICKET_N_TICKET, _TICKET_CAJA,
												_TICKET_SECUENCIA )&& !__cliente ) {
                    restore_screen = 1;
					//debo setear el tipo de comprobante y demas para que imprima si debe o no el articulo.
					if( !VERIFICAR_TIPO_COMPROBANTE( event_ticket.tipo_evento_orig ) ){
						SETEOS_DE_COMPROBANTE_ORIGINAL( error );
					}
                    if( !reimpresion ) {
                        int tf2=RAM_TICKET_FACTURA;
						tf2=RAM_FACTURA;
						SET_MEMORY_CHAR( __ram_en_importacion_de_ticket, 1 );
						if( remoto && origen == _ORIGEN_TICKET_EN_ESPERA && !__cliente ) {
							if( PERMITE_MODIFICAR_TICKET_IMPORTA == SI ) {
								tecla = MENSAJE_CON_TECLA_S_N("SI AGREGA ART. EL PRECIO PUEDE VARIAR <ENTER>=SI <ESC>=NO",13,27);
								if( tecla == 13 ) {
									ASIGNAR_PRECIO_PADRON( );
									SET_MEMORY_INT( __ram_agrega_item_tick_import, 2 );
								} else {//escape = tecla 27
									SET_MEMORY_INT( __ram_agrega_item_tick_import, 1 );
								}
							} 
							else 
							{
								SET_MEMORY_INT( __ram_agrega_item_tick_import, 1 );
							}
						}
                        if( ev_requerido != _EVENTO_COBRO_ESPECIAL && !__cliente ) //si no es COBROS_ESPECIALES
                        {
							if( origen == _ORIGEN_TICKET_EN_ESPERA ) {
								AGRUPAR_ART_IMPORT( NO, NO, NO/*SI*/, 0 );
							}
                            if( AGREGAR_ITEMS_AL_TICKET( remoto )){
								SET_MEMORY_CHAR( __ram_en_importacion_de_ticket, 2 );
							}else{
								error = 2;
							}
                        }
						tf2=RAM_TICKET_FACTURA;
						tf2=RAM_FACTURA;
                        //SET_MEMORY_CHAR( __ram_en_importacion_de_ticket, 0 );
                        SET_MEMORY_INT( __ram_item_supervisado, RAM_P_TICKET - 1);
					}
                    else {
                        SET_MEMORY_CHAR( __ram_en_importacion_de_ticket, 2 );
                    }
                }
                else {
                    error = 2;
                }
            }
        }
        _SET_ESTADO( 2 );
    }

    en_importacion_de_ticket = 0;
    if( ev_requerido != _EVENTO_COBRO_ESPECIAL ) {
        //si no es COBROS_ESPECIALES
        if( !reimpresion && !error && !remoto ) {
            //marco ticket como importado
            BUSCA_TICK_ALL( _TICKET_FECHA, _TICKET_N_TICKET );
            event_ticket.importado = 1;
            UPDATE2();
        }
		if(articulos || __cliente) {
			MENSAJE_CON_PAUSA(ST(S_TICKET_ESPERA_CANCELADO),25);
			error = 1;
		}
    }

    if( !RAM_REFERENCIA_NC && config.pais != COLOMBIA && RAM_MODO_DEVOLUCION ) //VENEZUELA
	   SET_MEMORY_INT( __ram_bultos, bultos_ticket );

    if( !error || ( MODO_NEGOCIO == SERVICIOS && MODO_DEVOLUCION ) ) {
        SET_MEMORY_INT( __ram_fecha_anterior, _TICKET_FECHA );
        SET_MEMORY_LONG( __ram_boleta_anterior, _TICKET_N_TICKET );
        SET_MEMORY_INT( __ram_caja_anterior, _TICKET_CAJA );
        SET_MEMORY_LONG( __nro_evento_anterior, _TICKET_SECUENCIA );
    } else {
		CEREAR_PERFIL_CLIENTE();
		CEREAR_DATOS_COMPROB_ANTERIOR();
    }

    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
    return ( !error );
}
/*****************************************************************************/
void ASIGNAR_PRECIO_PADRON( )
/*****************************************************************************/
{
    int i, encontro = 0,devolucion = 0, rta = 0, signo = 0, art_anu_dif = 1;
	int size, p_art = 0, j = 0, error = 0;
	char cod_barra[17], grabarlog[150];

    struct _eventos eventos;
    struct _ev_cont ev_cont;
	struct _articulo_control
	{
		long cod_articulo;
		char cod_barra[17];
	};
	struct _articulo_control *articulo_control;

	memset( &ev_cont, 0, sizeof( struct _ev_cont ) );
	memset( &eventos, 0, sizeof( struct _eventos ) );


	size = sizeof( struct _articulo_control ) * _LARGO_TICKET;
    articulo_control = MALLOC( size );
	memset( articulo_control, 0, sizeof( struct _articulo_control ) );
	
	if(	!OPENED( T_EV_TMP, TT_ORIG ) )
		error = OPEN_TABLE( T_EV_TMP, TT_ORIG, ( char* )&eventos, sizeof( struct _eventos ) );

	if( !error && !OPENED( T_EVC_TMP, TT_ORIG ) )
		 error += OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )&ev_cont, sizeof( struct _ev_cont ) );
	
	memset( cod_barra, 0, sizeof( cod_barra ) );
	memset( grabarlog, 0, sizeof( grabarlog ) );

	if( !error && articulo_control ) {
		memset( articulo_control, 0, size );
		SELECT_TABLE( T_EV_TMP, TT_ORIG );
		SET_WHERE("");
		RUN_QUERY(NO);

		/*En la Devolución, el articulo asume la caracteristica de Pide Precio*/
		if( eventos.tipo_evento_orig == 21 ) {
			devolucion = 1;
		}

		SELECT_TABLE( T_EVC_TMP, TT_ORIG );
		SET_WHERE("");
		RUN_QUERY(NO);

		while( !dbEOF() && !devolucion  && articulo_control ) {
			if( ( ev_cont.origen != _ORIGEN_DESC_PESOS
					&& ev_cont.origen != _ORIGEN_DESC_PORC
					&& ev_cont.origen != _ORIGEN_PROMOCION
					&& ev_cont.origen != _ORIGEN_DIF_PRECIO
					&& ev_cont.cod_movimiento != _CAMBIO_DE_MERCADERIA_MENOS
					&& ev_cont.cod_movimiento != _CAMBIO_DE_MERCADERIA_MAS )
					&& !( ev_cont.tipo2 & _PEDIR_PRECIO ) ) {

				signo = ( ev_cont.tipo & _RESTA ) ? -1 : 1;

				if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
					memcpy( cod_barra, ev_cont.cod_barra, sizeof( cod_barra ) );
					encontro = BUSCA_COD_INTERNO_COD_BARRA( ev_cont.cod_articulo, cod_barra );
				}
				else {
					encontro = BUSCA_CODIGO_ESPECIAL( ev_cont.cod_articulo );
				}

				SELECT_TABLE( T_EVC_TMP, TT_ORIG );
				//Control p/ art. anulados con previa diferencia de precio sobre el artículo que anula
				for( j = 0; j < p_art && encontro ; j++ ) {
					if( articulo_control[j].cod_articulo == ev_cont.cod_articulo
							&& ( strcmp( ev_cont.cod_barra, articulo_control[j].cod_barra ) == 0 ) ) {
						art_anu_dif = 0;
						articulo_control[j].cod_articulo = 0;
						strncpy( articulo_control[j].cod_barra, "0", sizeof( articulo_control[j].cod_barra ) );
					}
				}
				//ev_cont.importe_sin_iva se graba sin impuestos internos
				if( encontro 
						&& fabs( ((ev_cont.importe_sin_iva  +ev_cont.exento) * signo) - ( articulo.precio_sin_iva - articulo.imp_int ) ) > 0.01 
						&& art_anu_dif && !ev_cont.cod_cobro ) {
					_snprintf( grabarlog, sizeof( grabarlog ) - 1 , "MODIFICACION DE PRECIO EN COD_ART %ld COD_BARRA %s", ev_cont.cod_articulo, cod_barra );
					GRABAR_LOG_SISTEMA( grabarlog , LOG_VENTAS,3 );
					SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva * signo );
					SET_MEMORY_CHAR( __item_cod_iva, INDICE_IVA_ARTICULO() );
					SET_MEMORY_DOUBLE( __item_imp_int, articulo.imp_int * signo );
					SET_MEMORY_DOUBLE( __item_costo, articulo.costo * signo );
					SET_MEMORY_CHAR( __item_gravado, ( articulo.gravado == 'S' ) ? 1 : 0 );
					SET_MEMORY_DOUBLE( __item_recargo, 0 );

					CALCULA_ARTICULO_EXT( );
					SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva * signo );
					for( i = 0;i < 4;i++ ) {
						_SET_MEMORY_FLOAT( __item_porc_impuesto, i, articulo_ext.porc_imp[i] * signo );
						_SET_MEMORY_FLOAT( __item_suma_impuesto_al_total, i, articulo_ext.sumar_al_tot[i] * signo );
					}
					//SET_MEMORY_FLOAT( __item_porc_ila, articulo_ext.porc_ila );
					//SET_MEMORY_FLOAT( __item_porc_iha, articulo_ext.porc_iha );
					//SET_MEMORY_FLOAT( __item_porc_ica, articulo_ext.porc_ica );
					if( CR1_ITEM_GRAVADO( ) ) {
						ev_cont.importe_sin_iva = articulo.precio_sin_iva * signo;
						ev_cont.exento = 0;
					} else {
						ev_cont.exento = articulo.precio_sin_iva * signo;
						ev_cont.importe_sin_iva = 0;
					}
					ev_cont.imp_int = articulo.imp_int * signo;
					ev_cont.costo = articulo.costo * signo;

					CR1_ADOPTA_PRESENTACION( 1, SI );
					ev_cont.iva1 = ITEM_IVA1_PRECIO * signo;
					ev_cont.iva2 = ITEM_IVA2_PRECIO * signo;
					//SET_MEMORY_DOUBLE( __item_iva1_rec, 0);
					//SET_MEMORY_DOUBLE( __item_iva2_rec, 0 );
					ev_cont.monto_impuesto[0] = ( float )( _ITEM_MONTO_IMPUESTO( 0 ) * signo );
					ev_cont.monto_impuesto[1] = ( float )( _ITEM_MONTO_IMPUESTO( 1 ) * signo );
					ev_cont.monto_impuesto[2] = ( float )( _ITEM_MONTO_IMPUESTO( 2 ) * signo );
					ev_cont.monto_impuesto[3] = ( float )( _ITEM_MONTO_IMPUESTO( 3 ) * signo );
					ev_cont.precio_unitario = ROUND( articulo.precio_sin_iva + ITEM_IVA1_PRECIO + ITEM_IVA2_PRECIO
							+ _ITEM_MONTO_IMPUESTO( 0 )
							+ _ITEM_MONTO_IMPUESTO( 1 )
							+ _ITEM_MONTO_IMPUESTO( 2 )
							+ _ITEM_MONTO_IMPUESTO( 3 ), ENTEROS, DECIMALES );
					ev_cont.total = ROUND( ITEM_PRECIO * ev_cont.cantidad, ENTEROS, DECIMALES )* signo ;
					/* Actualizo Precio */
					UPDATE2( );

				} else {
					_snprintf( grabarlog, sizeof( grabarlog )-1, "ARTICULO COD_ART %ld COD_BARRA %s Encontro %i dif %lf Art_Anu_dif %i ", 
							ev_cont.cod_articulo, cod_barra, encontro,
							fabs( ev_cont.importe_sin_iva - ( articulo.precio_sin_iva - articulo.imp_int ) ),
							art_anu_dif );
					GRABAR_LOG_SISTEMA( grabarlog , LOG_VENTAS,4 );
				}
			}
			if( ev_cont.origen == _ORIGEN_DIF_PRECIO ) {
				articulo_control[p_art].cod_articulo = ev_cont.cod_articulo;
				strncpy( articulo_control[p_art].cod_barra, ev_cont.cod_barra,  sizeof( articulo_control[p_art].cod_barra ) );
				p_art ++;
			}
			SKIP2( 1 );
		}
		if( articulo_control ) {
			FREEMEM( (char *)articulo_control );
		}
		CLOSE_TABLE( T_EV_TMP, TT_ORIG );
		CLOSE_TABLE( T_EVC_TMP, TT_ORIG );
	} 

}
/*****************************************************************************/
int VERIFICA_DATOS_IMPORTACION( int remoto, int ev_requerido, int fecha, long nro_ticket, int caja,
                                long secuencia )
/*****************************************************************************/
{
    int rta = 0;
    char temp[100];
    struct _eventos eventos, *ticket;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

    //--- PRUEBA
    //return( 1 );
	if( remoto ) {
        if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                      &ev_cod_barra, &ev_b_promo ) ) {
            SELECT_TABLE( T_EV_TMP, TT_ORIG );
			ticket = &eventos;
           if( (REFERENCIAR_POR_SECUENCIA || PEDIR_CAJA_Y_SECUENCIA ) && secuencia > 0)
	            SET_WHERE("CAJA='%ld' AND NRO_SECUENCIA='%ld'", caja, secuencia);
	        else {
	            if( RAM_FACTURA != 0 )
	               SET_WHERE("CAJA='%ld' AND nro_comprobante='%ld'", caja, nro_ticket);
	            else
	               SET_WHERE("CAJA='%ld' AND NRO_TICKET='%ld'", caja, nro_ticket);

	        }

	        RUN_QUERY(NO);


            if( FOUND2( ) ) {
               int cantidad = 0;
               SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, eventos.tipo_evento );
			      SELECT_TABLE( T_EVC_TMP, TT_ORIG );
			   /*SET_ORDER2("CAJA_Z,ID_EVENTO,POSICION");
	           SET_WHERE("ID_EVENTO=%li",eventos.id_evento);
	           RUN_QUERY(NO);*/
	           cantidad = RECCOUNT2(NO,"ID_EVENTO=%ld",eventos.id_evento);//por eso uso esta

              // if( FOUND2( ) ) {
			   if( cantidad > 0 ) {
				  if( ev_requerido != 99 && ev_requerido != 30 ) {
				      SELECT_TABLE( T_EVM_TMP, TT_ORIG );
	                  SET_WHERE("ID_EVENTO=%ld",eventos.id_evento);
	                  RUN_QUERY(NO);
					  if( FOUND2( ) ) {
					      rta = 1;
					  }
					  else {
					      double total = 0.00;
						  total = eventos.importe_sin_iva + eventos.exento + eventos.iva1;
						  if( ( total < 0.05 ) && ( eventos.cambio == 1 ) ) {
						      rta = 1;
                          }
					  }
				  }
                  else {
                      rta = 1;
                  }
               }
            }
            if( !rta ) {
                MENSAJE_STRING( S_LOS_EVENTOS_NO_SON_CONSISTENTES );
            }
            if( rta && eventos.tipo_evento != ev_requerido && !PEDIR_CAJA_Y_SECUENCIA ) {
                MENSAJE_STRING( S_EL_EVENTO_NO_ES_DEL_TIPO_REQUERIDO );
                rta = 0;
            }
            if( rta && ev_requerido != 99
					&& !REFERENCIAR_POR_SECUENCIA ) {
                //se diferencia entre comprobante,nc y ticket
                if( ( ev_requerido == _EVENT_FAC_A ) || ( ev_requerido == _EVENT_FAC_B )
                 || ( ev_requerido == _EVENT_NC_A ) || ( ev_requerido == _EVENT_NC_B )
                 || ( ev_requerido == _EVENT_FAC_A_REMOTA )
                 || ( ev_requerido == _EVENT_FAC_B_REMOTA )
                 || ( ev_requerido == _EVENT_NC_A_REMOTA )
                 || ( ev_requerido == _EVENT_NC_B_REMOTA ) ) {
                    if( eventos.nro_comprobante_new != nro_ticket ) {
                        MENSAJE_STRING( S_EL_NUMERO_DE_TICKET_ES_INVALIDO );
                        rta = 0;
                    }
                }
                else if( 0 != nro_ticket && eventos.nro_ticket != nro_ticket  ) {
                    MENSAJE_STRING( S_EL_NUMERO_DE_TICKET_ES_INVALIDO );
                    rta = 0;
                }
            }
            if( rta && !PEDIR_CAJA_Y_SECUENCIA && eventos.fecha != fecha ) {
                MENSAJE_STRING( S_LA_FECHA_DEL_TICKET_ES_INVALIDA );
                rta = 0;
            }
            if( rta && PEDIR_CAJA_Y_SECUENCIA && eventos.caja != caja ) {
                MENSAJE_STRING( S_EL_NUMERO_DE_CAJA_NO_COINCIDE );
                rta = 0;
            }
            if( rta && secuencia && PEDIR_CAJA_Y_SECUENCIA && !(eventos.tipo_evento == _EVENT_TICKET_ESPERA && IMPRESORA_FISCAL == FISCAL) && //LA TM220 NO IMPRIME SECUENCIA EN TE
				eventos.nro_evento != secuencia ) {
                char test[150];
                MENSAJE_STRING( S_EL_NUMERO_DE_SECUENCIA_ES_INVALIDO );
                _snprintf(test, sizeof( test ), "[evento: nro_evento %ld, id_evento %ld, nro_z %ld] secuencia %ld,",
                    eventos.nro_evento, eventos.id_evento, eventos.nro_z, secuencia );
                GRABAR_LOG_SISTEMA( test , LOG_VENTAS,4 );
                rta = 0;
            }
            if( rta && eventos.importado ) {
                //MENSAJE_STRING( S_TICKET_YA_IMPORTADO );
                MENSAJE( "ESA OPERACION YA FUE REPROCESADA" );
                rta = 0;
            }
            /*if( rta && !ES_CREDITO_FISCAL( eventos.tipo_evento ) ) {
                MENSAJE( "NO ES POSIBLE EMITIR ESTE TIPO DE COMPROBANTE" );
                rta = 0;
            }*/
            if( MODO_NEGOCIO == SERVICIOS && eventos.caja != NRO_CAJA ) {
                MENSAJE( "TRANSACCION CORRESPONDIENTE A OTRA CAJA" );
                rta = 0;
            }
            if( MODO_NEGOCIO == SERVICIOS && eventos.fecha != GET_FECHA_DOS() ) {
                MENSAJE( "TRANSACCION CORRESPONDIENTE A OTRA FECHA" );
                rta = 0;
            }
			if( rta && ev_requerido == 30 ) {
				if( eventos.tipo_evento_orig == _EVENT_TICKET_FACTURA_A ) {
					SET_MEMORY_CHAR ( __ram_ticket_factura, 1 );
				}
				if( eventos.tipo_evento_orig == _EVENT_TICKET_FACTURA_B){
					SET_MEMORY_CHAR ( __ram_ticket_factura, 2 );
				}
			   memcpy( &event_ticket, &eventos, sizeof( struct _eventos ) );
			}
		}
        CERRAR_EVENTOS_TEMPORALES();
    }
	else {
        //SELECT( AREA_PAGO );
        //GET_EQUAL( ( char* )&event_ticket.id_evento );
		SELECT_TABLE( T_PAGOS, tipo_t_pagos );
		SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
        if( FOUND2() ) {
            rta = 1;
			SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, event_ticket.tipo_evento );
            while( !dbEOF() && event_ticket.id_evento == event_pago.id_evento && rta ) {
                if( ( ( event_pago.modo_pago == _medio_tarjeta
                     || event_pago.modo_pago == _medio_tarjeta2 ) && event_pago.cuotas > 1 )
                 || fabs( event_pago.variacion ) > .09 ) {
                    id_variacion = medios[event_pago.modo_pago].art_variacion;
                }
                SKIP2( 1 );
            }
        }
        else {
            rta = 0;
            MENSAJE_STRING( S_PAGO_NO_ENCONTRADO );
        }
        //SELECT( AREA_TICKET );
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
		ticket = &event_ticket;
        if( event_ticket.cajero != NRO_CAJERO && rta ) {
            _ATRIBUTO = 7;
            if( USAR_LEGAJO == RUT ) {
                sprintf( temp, ST( S_CAJERO_ACTUAL________S_ ), RAM_DOC_CAJERO );
                MOSTRAR_CADENA( 5 - ancho_panel / 2, 15, temp );
                sprintf( temp, ST( S_CAJERO_DEL_TICKET____09LI_ ), event_ticket.nro_legajo_cajero );
                MOSTRAR_CADENA( 5 - ancho_panel / 2, 16, temp );
            }
            else if( IMPRIMIR_LEGAJO_CAJERO ) {
                sprintf( temp, ST( S_CAJERO_ACTUAL________06LI_ ), NRO_LEGAJO_CAJERO );
                MOSTRAR_CADENA( 5 - ancho_panel / 2, 15, temp );
                sprintf( temp, ST( S_CAJERO_DEL_TICKET____06LI_ ), event_ticket.nro_legajo_cajero );
                MOSTRAR_CADENA( 5 - ancho_panel / 2, 16, temp );
            }
            else {
                sprintf( temp, ST( S_CAJERO_ACTUAL________D_ ), NRO_CAJERO );
                MOSTRAR_CADENA( 5 - ancho_panel / 2, 15, temp );
                sprintf( temp, ST( S_CAJERO_DEL_TICKET____D_ ), event_ticket.cajero );
                MOSTRAR_CADENA( 5 - ancho_panel / 2, 16, temp );
            }
            MOSTRAR_CADENA( 5 - ancho_panel / 2, 18, ST( S_CAJEROS_DISTINTOS ) );
            if( SOLICITAR_CLAVES( 30 - ancho_panel, 21, NO, SI, SI,
                                  ST( S_AUTORIZA_CAJEROS_DISTINTOS ), NULL,
                                  _F_SUP_CAJEROS_DISTINTOS, SI ) ) {
                rta = 1;
            }
            else {
                rta = 0;
            }
        }
    }




	if( rta ) {
		//SET_MEMORY_DOUBLE( __ram_percepcion_tacis,eventos.percepcion_municipal );
		SET_MEMORY_DOUBLE( __ram_retencion_dgr, ticket->retencion_dgr );
		SET_MEMORY_DOUBLE( __ram_percepcion_dgi, ticket->percepcion_dgi );
		SET_MEMORY_DOUBLE( __ram_percepcion_212_iva1, ticket->percep_212_iva1 );
		SET_MEMORY_DOUBLE( __ram_percepcion_212_iva2, ticket->percep_212_iva2 );
	}

	return rta;
}
/*****************************************************************************/
 void AGRUPAR_ART_IMPORT( int permite_exclusion_art, int agrupar_des,
		int agrupar_precio, int _operacion )
/*****************************************************************************/
{
	struct _ev_cont *acu_art_imp;
	//char barra1[17],barra2[17];
    int h = 0, i = 0, size = 0, size1 = 0, signo = 0, encontro = 0, p_art = 0, agrupar, ok = 0;
	int rta = 0, dif = 0, acumular = 0, cerear = 0, k = 0, ka = 0, p_dif = 0;
	double cantidad_div = 0;
    float cantidad = 0;
    struct _ev_cont ev_cont;
	double tot_imp = 0, precio_imp = 0,precio = 0, dif_precio = 0 ;
	struct _articulo_dif *articulo_dif;

	if( OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )&ev_cont,	sizeof( struct _ev_cont ) ) == 0 ) {
		SET_WHERE("");
		RUN_QUERY(NO);
		size1 = sizeof( struct _articulo_dif ) * _LARGO_TICKET;
		articulo_dif = MALLOC( size1 );

		size = sizeof( struct _ev_cont ) * _LARGO_TICKET;
		acu_art_imp = MALLOC( size );

		if( acu_art_imp ) {
			memset( articulo_dif, 0, size1 );
			memset( acu_art_imp, 0, size );
			GO2( TOP );
			while( /*!BtrvEOF( )*/ !dbEOF() ) {
				cantidad = ev_cont.cantidad;
				encontro = 0;
				agrupar = 1;
				if( !agrupar_des ) {
					/*No acumulo los items de promo ni los items de anulacion de promo(TOTAL-ESC)*/
					if( ( ev_cont.origen == _ORIGEN_DESC_PESOS
							|| ev_cont.origen == _ORIGEN_DESC_PORC
							/*|| ev_cont.origen == _ORIGEN_PROMOCION*/
							|| ( ev_cont.origen == _ORIGEN_DIF_PRECIO
								&& RAM_REFERENCIA_NC ) )
								//Si en un ticket hay Cobros venta, estos se cancelan 
								//cuando se envia el Ticket a Espera generando los items de 
								//cancelacion correspondientes.
								//Se habilita la agrupación de cobro venta para la 
								//funcion Ticket en Espera.
							/*|| ev_cont.cod_cobro != 0*//*para Cobros venta*/ ) {
						agrupar = 0;
					}
				}
				if( !agrupar_precio ) {
					//strncpy( barra1, ev_cont.cod_barra , sizSUMAR_IMPORTE_SUBMEDIO( barra1 ) );
					//strncpy( barra2, ev_cont.cod_barra, sizeof( barra2 ) );
					/*No acumulo los items pide precio ni los items pide precio anulados*/
					if( ( ( ev_cont.tipo2 & _PEDIR_PRECIO )
							/*|| ( fabs( fmod( ev_cont.cantidad, 1.00 ) ) > 0.0005 )*/
							/*|| strcmp( barra1, barra2 )*/ ) ) {
						agrupar = 0;
					}
				}
				/*Para cargar el primer Articulo diferencia de precio*/
				if( agrupar && ev_cont.origen == _ORIGEN_DIF_PRECIO
						&& !RAM_REFERENCIA_NC ) {
					if( ( ( ev_cont.tipo2 & _PEDIR_PRECIO )
							|| ( fabs( fmod( ev_cont.cantidad, 1.00 ) ) > 0.0005 )
							|| ( ev_cont.tipo3 & _PESABLE )
							|| ev_cont.tipo & _RESTA ) ) {
						precio_imp = ev_cont.total;
					} else {
						for( h = 0; h < 4; h++ ) {
							tot_imp += ev_cont.monto_impuesto[h];
						}
						precio_imp = ev_cont.importe_sin_iva
							+ ev_cont.imp_int
							+ ev_cont.exento + ev_cont.iva1
							+ ev_cont.iva2 + ev_cont.ila
							+ ev_cont.iha + ev_cont.alicuota + tot_imp;
					}
				}
/**************** Agrupamos articulos con sus anulaciones ****************/
				for( i = 0; i < p_art && !encontro && agrupar; i++ ) {
					tot_imp = 0;
					precio_imp = 0;
					dif_precio = 0;
					//alicuota reutilizada para diferencia en importe enviado impresora
					if( ev_cont.alicuota  && RAM_NOTA_CR > 0 ) {
						//como en alicuota se guarda el pe
						dif_precio = fabs( fabs(ev_cont.importe_sin_iva+ev_cont.exento )- fabs(ev_cont.alicuota) );
						dif_precio = ( ( dif_precio * iva[ev_cont.cod_iva].porc_iva1 ) / 100 ) + dif_precio; 
						if(( fabs(ev_cont.importe_sin_iva+ev_cont.exento ) - fabs(ev_cont.alicuota)  > 0.0) 
							 && ev_cont.origen == _ORIGEN_PROMOCION && ev_cont.cantidad < 0.0   )
							 dif_precio = - dif_precio;

						
					}

					if( ( ( ev_cont.tipo2 & _PEDIR_PRECIO )
							|| ( fabs( fmod( ev_cont.cantidad, 1.00 ) ) > 0.0005 )
							|| ( ev_cont.tipo3 & _PESABLE ) ) ) {
						precio_imp = ev_cont.total + dif_precio;
					} else {
						for( h = 0; h < 4; h++ ) {
							tot_imp += ev_cont.monto_impuesto[h];
						}
								
						precio_imp = ev_cont.importe_sin_iva
									+ ev_cont.exento 
									+ ev_cont.imp_int
									+ ev_cont.iva1
									+ ev_cont.iva2 
									+ ev_cont.ila
									+ ev_cont.iha 
									+ tot_imp
									+ ev_cont.recargo_tarjeta
									+ dif_precio;
					}
					//DEBE TENER UN CLIENTE CARGADO PARA AGRUPAR DE ACUERDO AL COMPROBANTE
					if( ES_FACTURA_A( clientes.cond_iva ) && !FACTURA_CON_PRECIO_FINAL ) {
						if( RAM_TICKET_FACTURA == 1 && ev_cont.origen == _ORIGEN_PROMOCION ) {
							precio = ROUND(ev_cont.importe_sin_iva , ENTEROS, DECIMALES)
									+ ROUND(ev_cont.exento, ENTEROS, DECIMALES)
									+ ROUND(ev_cont.imp_int, ENTEROS, DECIMALES)
									+ ROUND(ev_cont.iva1, ENTEROS, DECIMALES)
									+ ROUND(ev_cont.iva2, ENTEROS, DECIMALES) 
									+ ROUND(ev_cont.ila, ENTEROS, DECIMALES) 
									+ ROUND(ev_cont.iha, ENTEROS, DECIMALES) 
									+ ROUND(tot_imp, ENTEROS, DECIMALES) 
									+ ev_cont.recargo_tarjeta
									+ ROUND( dif_precio, ENTEROS, DECIMALES) ;
						} else	{						
							precio = ev_cont.importe_sin_iva 
									+ ev_cont.exento
									+ ev_cont.imp_int
									+ ev_cont.iva1 
									+ ev_cont.iva2 
									+ ev_cont.ila
									+ ev_cont.iha 
									+ tot_imp
									+ ev_cont.recargo_tarjeta
									+ dif_precio;																		
						}
					} else {
                        precio = ev_cont.importe_sin_iva 
								+ ev_cont.exento
								+ ev_cont.imp_int
								+ ev_cont.iva1 
								+ ev_cont.iva2 
								+ ev_cont.ila
								+ ev_cont.iha 
								+ tot_imp
								+ ev_cont.recargo_tarjeta
								+ dif_precio;
						for( k = 0;k <= 3;k++ ) {
							precio += ev_cont.monto_impuesto[k];
						}
                    }
					//debo chequear que no me de error de division por 0
					//Se utiliza cantidad_div para no modificar el valor de acu_art_imp[i].cantidad en el
					//caso que el acumulativo de 0 por la acumulacion de cancelaciones.
					cantidad_div = (double) acu_art_imp[i].cantidad;
					/*debo controlar el caso de que la cantidad este entre 1 y 0 , ej: 0.188 */

					if( fabs(cantidad_div) < 0.001 )
						cantidad_div = 1.0;

					if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
						double proviso= fabs( acu_art_imp[i].total /  cantidad_div  );
						float sensibilidad =0.01;
												
						/*****************************************************************************
						veamos de ser menos estrictos para que acumule los que son ingresados mal, en  estos
						casos contemplan por ejemplo vender un articulo pesable ingresando el  codigo 
						template y anularlo ingresando el cod interno y el peso....(dif redondeo)
						*/
						if(( strcmp( ev_cont.cod_barra, acu_art_imp[i].cod_barra ) == 0 )
							&&( ev_cont.tipo3 & _PESABLE ) && ( ev_cont.tipo & _RESTA)){
							sensibilidad =0.05;
						}
						/*******************************************************************************/


						if( ev_cont.cod_articulo == acu_art_imp[i].cod_articulo
							&& ( fabs( fabs( acu_art_imp[i].total /  cantidad_div  ) 
									- fabs( precio ) ) < sensibilidad )
								&& ( strcmp( ev_cont.cod_barra, acu_art_imp[i].cod_barra ) == 0 )
								&& ev_cont.origen == acu_art_imp[i].origen ) {
							encontro = 1;
							if( encontro
									&& ( ( ev_cont.tipo & _DEV_ENVASE )
									&& !( acu_art_imp[i].tipo & _DEV_ENVASE ) ) ) {
								encontro = 0;
							}
						}
					}
					else {
						double difTF;
						difTF = 0.01;
						if(ev_cont.cod_articulo == acu_art_imp[i].cod_articulo && ev_cont.origen == acu_art_imp[i].origen
							&& RAM_TICKET_FACTURA == 1 && ev_cont.origen == _ORIGEN_PROMOCION )
						    difTF = 0.03;
						if( ev_cont.cod_articulo == acu_art_imp[i].cod_articulo
							&& ( fabs( fabs( acu_art_imp[i].total / cantidad_div ) - fabs( precio ) ) < difTF )
								&& ev_cont.origen == acu_art_imp[i].origen ) {
							encontro = 1;
						}
					}
					if( encontro ) {
						/*Articulo pide precio no acumulan cantidades*/
						if( ( !( ev_cont.tipo2 & _PEDIR_PRECIO )
								|| ( ( ev_cont.tipo2 & _PEDIR_PRECIO
									|| fabs( fmod( ev_cont.cantidad, 1.00 ) ) > 0.0005 )
									|| ( ev_cont.tipo3 & _PESABLE )
									&& ( ev_cont.tipo & _RESTA || acu_art_imp[i].cantidad == 0 ) ) )
								&& ev_cont.origen == acu_art_imp[i].origen ) {
							acu_art_imp[i].cantidad += cantidad;
						}
						/*Diferencia de articulos pesables*/
						if( ( ( fabs( fmod( ev_cont.cantidad, 1.00 ) ) > 0.0005 )
									|| ( ev_cont.tipo3 & _PESABLE ) )
								/*&& !RAM_REFERENCIA_NC*/
								&& ev_cont.origen == acu_art_imp[i].origen ) {
							acumular = 1;
							cantidad = 1;
						}
                        //Si se graban en negativo tanto cantidades como importes
                        if( cantidad < 0 && precio_imp < 0 ) {
                            cantidad *= -1;
                        }
						/*Cereo los importes para evitar basura en los acumuladores*/
						cerear = ( !acu_art_imp[i].cantidad )? 0 : 1;
						if( cerear ) {
							//Nota: el siguiente if estaba comentado en la version de Selectos al momento
							// del merge, pero tambien se encontraba comentado en la version unificada anteriormente
							//por lo que fue descomentada por alguna razon en la version MySQL.
							//para mi se comento mal pero por las dudas si es una resta(nulacion) no lo hago pasar.
							//HABRIA QUE VER LOS OTROS CASOS, solo importa aumentar el total.
							/*Los articulos pesables no acumulan sus importes*/
							//if( !( fabs( fmod( ev_cont.cantidad, 1.00 ) ) > 0.0005)
									//|| ( ev_cont.tipo3 & _PESABLE  ) || acumular  ) {
								//Se pasa el cambio realizado en Selecto.
								//Al comentarse el acumulativo pierde el objetivo de la funcion que es acumular
								//acu_art_imp[i].total += precio_imp;// * cantidad;
							//	acu_art_imp[i].importe_sin_iva  /*+*/= ev_cont.importe_sin_iva /** cantidad*/;
							//	acu_art_imp[i].exento  /*+*/= ev_cont.exento /** cantidad*/;
							//	acu_art_imp[i].iva1  /*+*/= ev_cont.iva1 /** cantidad*/;
							//	acu_art_imp[i].iva2  /*+*/= ev_cont.iva2 /** cantidad*/;
							//	acu_art_imp[i].ila  /*+*/= ev_cont.ila /** cantidad*/;
							//	acu_art_imp[i].iha  /*+*/= ev_cont.iha /** cantidad*/;
							//	acu_art_imp[i].alicuota  /*+*/= ev_cont.alicuota /** cantidad*/;
							/*	for( h = 0; h < 4; h++ ) {
									acu_art_imp[i].monto_impuesto[h] += ev_cont.monto_impuesto[h] * cantidad;
								}*/
							//} else {
							//LAS PROMO Y ANULACIONES DE PROMO tienen el mismo 
							if( ev_cont.total > 0.00 && (precio_imp * cantidad) < 0.00 && ev_cont.origen == _ORIGEN_PROMOCION)
								acu_art_imp[i].total += precio_imp * cantidad * -1;
							else
								acu_art_imp[i].total += precio_imp * cantidad;
							//}
						} else {
//							if( ev_cont.origen != _ORIGEN_DIF_PRECIO ) {
                     if( ev_cont.origen != _ORIGEN_DIF_PRECIO
                        && ( ! ( ev_cont.tipo & _RESTA && (_operacion == _DEVOL_PARCIAL || _operacion == _NC_RF_PARCIAL) ))) {
								
							    acu_art_imp[i].total = 0;
								acu_art_imp[i].importe_sin_iva = 0;
								acu_art_imp[i].imp_int = 0;
								acu_art_imp[i].exento = 0;
								acu_art_imp[i].iva1 = 0;
								acu_art_imp[i].iva2 = 0;
								acu_art_imp[i].ila = 0;
								acu_art_imp[i].iha = 0;
								acu_art_imp[i].alicuota = 0;
							}
							/*Para el caso en que haya 2 diferencias de precio != para el mismo articulo.
							Se concidera la ultima */
							for( k = 0; k < p_dif; k++ ) {
								if( ev_cont.cod_articulo == articulo_dif[ k ].cod_articulo
										&& ( strcmp( ev_cont.cod_barra, articulo_dif[ k ].cod_barra ) == 0 ) ) {
									acu_art_imp[articulo_dif[ k ].pos].cantidad = 0;
									acu_art_imp[articulo_dif[ k ].pos].total = 0;
									acu_art_imp[articulo_dif[ k ].pos].importe_sin_iva = 0;
									acu_art_imp[articulo_dif[ k ].pos].imp_int = 0;
									acu_art_imp[articulo_dif[ k ].pos].exento = 0;
									acu_art_imp[articulo_dif[ k ].pos].iva1 = 0;
									acu_art_imp[articulo_dif[ k ].pos].iva2 = 0;
									acu_art_imp[articulo_dif[ k ].pos].ila = 0;
									acu_art_imp[articulo_dif[ k ].pos].iha = 0;
									acu_art_imp[articulo_dif[ k ].pos].alicuota = 0;
									for( ka = 0; ka < 4; ka++ ) {
										acu_art_imp[articulo_dif[ k ].pos].monto_impuesto[ka] = 0;
									}
								}
							}
						}
					}
					
				}
/***********  FIN DE LA AGRUPACION ***************************************/
				if( !encontro ) {
					if( ev_cont.origen == _ORIGEN_DIF_PRECIO ) {
						articulo_dif[ p_dif ].pos = p_art;
						articulo_dif[ p_dif ].cod_articulo = ev_cont.cod_articulo;
						memcpy( articulo_dif[ p_dif ].cod_barra, ev_cont.cod_barra, 17 );
						p_dif ++;
					}
					if( config.pais == ELSALVADOR ) {
						rta = ( ( ev_cont.tipo2 & _HABILITADO_PROMO ) ? 1 : 0 )
								&&( ( ev_cont.tipo & _CREDITO ) ? 0 : 1 );
					}
					else {
						rta = ( ev_cont.tipo2 & _HABILITADO_PROMO ) ? 1 : 0;
					}
					if( cantidad && ( rta || !permite_exclusion_art ) ) {
						memcpy( &acu_art_imp[ p_art ], &ev_cont, sizeof( struct _ev_cont ) );
						p_art ++;
					}
				}
				SKIP2( 1 );
			}//fin while
			/******************************** Limpio Tabla ********************************/
			if( p_art ) {
				#ifdef INVEL_L
				#else
				ZAP2();
				#endif
			}
			/***************** Insertamos en tabla los articulos agrupados ****************/
			GO2( TOP );
			for( h = 0; h < p_art; h++ ) {
				memset( &ev_cont, 0, sizeof( struct _ev_cont ) );
				if( fabs(acu_art_imp[h].cantidad * (acu_art_imp[h].importe_sin_iva+acu_art_imp[h].exento)) > 0.009) {
					memcpy( &ev_cont, &acu_art_imp[h], sizeof( struct _ev_cont ) );
					INSERT2(NO);
					SKIP2( 1 );
				}
			}
			//CLOSE_DATABASE( AREA_AUX );
			CLOSE_TABLE( T_EVC_TMP, TT_ORIG );
			FREEMEM( (char *)acu_art_imp );
			FREEMEM( (char *)articulo_dif );
		}
	}
}
/*****************************************************************************/
int COMPARAR_DATOS_IMPORTACION( void )
/*****************************************************************************/
{
    int rta = 1;
    char texto[100];
	struct tick_tmp evento_tmp;
	int /*area_ant= 0*/ tabla_ant, tipo_ant;

    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	//if( USE_DATABASE( AREA_TICK_TMP, "tick_tmp.btr", ( char* )&evento_tmp, sizeof( struct tick_tmp ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_TICK_TMP, TT_ORIG, ( char* )&evento_tmp, sizeof( struct tick_tmp ) ) == 0 ) {
		//GET_EQUAL( ( char* )&event_ticket.id_evento );
		SET_WHERE("ID_EVENTO='%ld'", event_ticket.id_evento );
		RUN_QUERY(NO);
		if( FOUND2( ) ) {
			if( event_ticket.fecha != evento_tmp.fecha ) {
                rta = 0;
				GRABAR_LOG_SISTEMA( BUSCA_MENSAJE_STRING( S_LA_FECHA_DEL_TICKET_ES_INVALIDA ), LOG_VENTAS,2  );
            }
			if( rta && event_ticket.tipo_evento != evento_tmp.tipo_evento ) {
				rta = 0;
				GRABAR_LOG_SISTEMA( BUSCA_MENSAJE_STRING( S_EL_EVENTO_NO_ES_DEL_TIPO_REQUERIDO ), LOG_VENTAS,2  );
			}
			if( rta && event_ticket.nro_evento != evento_tmp.nro_evento ) {
				rta = 0;
				GRABAR_LOG_SISTEMA( BUSCA_MENSAJE_STRING( S_EL_NUMERO_DE_CAJA_NO_COINCIDE ), LOG_VENTAS,2  );
			}
		} else {
			rta = 0;
		}
    } else {
		GRABAR_LOG_SISTEMA( "Error al abrir evento temporal", LOG_VENTAS,2  );
    }
	if( !rta ){
		sprintf( texto, "El evento %ld con secuencia %ld de la caja_z %ld no fue enviado",
				event_ticket.id_evento, event_ticket.nro_evento, event_ticket.caja_z );
		GRABAR_LOG_SISTEMA( texto , LOG_DEBUG,2);
	}

	//SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );

	return ( rta );
}
/*****************************************************************************/
int BUSCA_TICK_ALL( int fecha, long nro_ticket )
/*****************************************************************************/
{
    struct
    {
        UINT16 fecha;
        INT16 caja;
        long nro_ticket;
    } ind3_all;
    int rta = 0;
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    ind3_all.fecha = fecha;
    ind3_all.caja = NRO_CAJA;
    ind3_all.nro_ticket = nro_ticket;
    //SET_ORDER( 3 );
	SET_WHERE("FECHA = '%i' and NRO_CAJA_DGI = '%i' and NRO_TICKET = '%ld'",
				ind3_all.fecha,
				ind3_all.caja,
				ind3_all.nro_ticket);
	SET_ORDER2("FECHA,NRO_CAJA_DGI,NRO_TICKET");
	RUN_QUERY(NO);
    //GET_EQUAL( ( char* )&ind3_all );
    if( FOUND2() && !event_ticket.importado ) {
        rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
int AGREGAR_ITEMS_AL_TICKET( int remoto )
/*****************************************************************************/
{
    long id = 0,provisorio=0;
    int handle_archivo;
    int i, tipo2, dto_importe = 0;
    double *base_dto = NULL,importe_provisorio=0.0, dife = 0.0;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;
	int rta = 1;//si existe algun error en la carga del articulo se debe contemplar..
	int esdescuento =0;
    if( remoto ) {
        ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                  &ev_cod_barra, &ev_b_promo );

        SELECT_TABLE( T_EV_TMP, TT_ORIG );
		SET_ORDER2("CAJA_Z, ID_EVENTO");
		RUN_QUERY(NO);
		id = eventos.id_evento;

        //SELECT( AREA_AUX4 );          //AREA_E_TICKET
		//SET_ORDER( 2 );
		//GO( TOP );
		SELECT_TABLE( T_EVC_TMP, TT_ORIG );
		SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION");
		RUN_QUERY(NO);
    }
    else {
        //SELECT( AREA_E_TICKET );
        //SET_ORDER( 1 );
        //GO( TOP );
        id = event_ticket.id_evento;
        //GET_EQUAL( ( char* )&id );
        SELECT_TABLE( T_EVENTOS, tipo_t_ev_cont );
		SET_WHERE("ID_EVENTO = '%ld'",id);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);

		SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
		SET_WHERE("ID_EVENTO = '%ld'",id);
		SET_ORDER2("ID_EVENTO, POSICION");
		RUN_QUERY(NO);
    }
    i = 0;
    //Creamos un archivo de bases de descuentos
    base_dto = ( double* )calloc( _LARGO_TICKET, sizeof( double ) );
    if( base_dto ) {
        memset( base_dto, 0, sizeof( double ) * _LARGO_TICKET );
    }
    while( rta == 1 &&( ( remoto ) ? /*1 */id == ev_cont.id_evento : id == event_e_ticket.id_evento ) 
			&& /*!BtrvEOF()*/ !dbEOF() ) {
		i++;
        if( remoto ) {
            memcpy( &event_e_ticket, &ev_cont, sizeof( struct _ev_cont ) );
            memcpy( &event_ticket, &eventos, sizeof( struct _eventos ) );
            //i = 1; //comento para cargar todos los items..
        }
		{
			char buff[100];
			_snprintf( buff, sizeof( buff )-1,"item1[%i] %s,%li,%i,%lf",event_e_ticket.posicion, event_e_ticket.cod_barra,event_e_ticket.cod_articulo,event_e_ticket.origen,event_e_ticket.total);
			GRABAR_LOG_SISTEMA(buff, LOG_DEBUG,4);
		}
		//Se reutiliza este campo para indicar el cod_iva de articulo.cod_exento (codigo iva aplicado al articulo)
		if( event_e_ticket.cod_depto_old ) {
			if( event_e_ticket.cod_depto_old <= MAX_COD_IVA
					&& DIRECT_DB_QUERY("select * from articulo_iva where cod_iva=%i", event_e_ticket.cod_depto_old )  > 0 ) { //= 0->No existe registro, = 1->Existe registro
				event_e_ticket.cod_iva = event_e_ticket.cod_depto_old;	
			} else {
				char aux[150];
				memset( aux, 0, sizeof( aux ) );
				_snprintf(aux, sizeof(aux) - 1, "cod_exento %i inexistente en tabla articulo_iva (Art. COD_ART %ld COD_BARRA %s)",
						event_e_ticket.cod_depto_old, event_e_ticket.cod_articulo, event_e_ticket.cod_barra );
				GRABAR_LOG_SISTEMA(aux, LOG_ERRORES, 1);
			}
		}

		if( CARGA_ITEM( i - 1, base_dto ) ) {
            if( event_e_ticket.origen == _ORIGEN_DESC_PESOS
             || event_e_ticket.origen == _ORIGEN_DESC_PORC
             || event_e_ticket.origen == _ORIGEN_PROMOCION ) {
                tipo2 = event_e_ticket.tipo;
                if( config.pais == ELSALVADOR ) {
                    ADD_MEMORY_DOUBLE( __ram_imp_descuento,
									( event_e_ticket.importe_sin_iva 
										+ event_e_ticket.exento
										+ event_e_ticket.iva1 + event_e_ticket.iva2
										+ event_e_ticket.ila + event_e_ticket.iha )
										* event_e_ticket.cantidad );
                }
            }
            else {
                tipo2 = 0;
				if( provisorio == 0 ){
					provisorio = event_e_ticket.cod_articulo;
					importe_provisorio =( event_e_ticket.importe_sin_iva 
										+ event_e_ticket.exento
										+ event_e_ticket.iva1 + event_e_ticket.iva2
										+ event_e_ticket.ila + event_e_ticket.iha )
										* event_e_ticket.cantidad ;

				}
            }
            if( event_e_ticket.origen == _ORIGEN_DESC_PESOS && event_e_ticket.tipo != 9 ) {
                dto_importe = ( RAM_ANULACION ) ? 0 : 1;
            }
            /*Si el total del articulo en eventos, es diferente al original, recalcula el precio*/
			dife = fabs(ROUND(event_e_ticket.total,ENTEROS, DECIMALES))- fabs(ROUND( ITEM_PRECIO * event_e_ticket.cantidad,
				ENTEROS, DECIMALES ));
			 esdescuento = ( event_e_ticket.origen == _ORIGEN_DESC_PESOS || event_e_ticket.origen == _ORIGEN_DESC_PORC
				     || event_e_ticket.origen == _ORIGEN_PROMOCION)?1:0;
			 if( esdescuento ==0 && ( RAM_AGREGA_ITEM_TICK_IMPORT == 2 && event_ticket.tipo_evento == 30 && articulo.precio!='S' &&  fabs (fabs (articulo.precio_sin_iva )- fabs (event_e_ticket.importe_sin_iva))> 0.01 ))   
				dife = 0.00;
            //if(fabs(ROUND(event_e_ticket.total,ENTEROS, DECIMALES)) != fabs(ROUND( ITEM_PRECIO * event_e_ticket.cantidad,
			if( fabs(dife) > 0.0101 ){ // si el evento original es una devolucion, recalcula el total
                int signo = 1, signo_dev = 1;
                signo_dev =( MODO_DEVOLUCION
						|| ( RAM_NOTA_CR
							&& ( RAM_MODO_DEVOLUCION == _NC_TOTAL ||
								( RAM_MODO_DEVOLUCION == _NC_RF_INTERME ) ) ) )? -1 : 1;
                signo = ( event_e_ticket.tipo & _RESTA )? -1 : 1;
				if ( signo == -1 &&( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME ))
					signo = 1;
//              RECALCULA_ITEM_POR_PRECIO( ROUND( event_e_ticket.total / event_e_ticket.cantidad * signo * signo_dev,ENTEROS, DECIMALES) );
				RECALCULA_ITEM_POR_PRECIO( (event_e_ticket.total / event_e_ticket.cantidad) * signo * signo_dev );
            }else{
				RECALCULA_ITEM_POR_PRECIO( (ITEM_PRECIO) );
            }
			{
				char buff[120];
				_snprintf( buff, sizeof( buff )-1,"item2[%i] %s,%li,%i,%lf,%d",event_e_ticket.posicion,event_e_ticket.cod_barra,event_e_ticket.cod_articulo,event_e_ticket.origen,
					event_e_ticket.total,RAM_ANULACION);
				GRABAR_LOG_SISTEMA(buff, LOG_DEBUG,4);
			}
            if( CR1_AGREGA_ITEM( event_e_ticket.tipo & _ENGANCHADO, 
					event_e_ticket.origen, 0,
					(remoto)? eventos.tipo_evento_orig : event_ticket.tipo_evento_orig ) ) {
						if( tipo2 ) {
                    _SET_MEMORY_CHAR( __ticket_tipo2, RAM_P_TICKET - 1, tipo2 );
                    SET_MEMORY_CHAR( __ram_prohibir_mas, 1 );
                }

				/******************* Para Cobros venta *******************/
				{
					//EV_COD_BARRA
					/*int area_ant;*/
					int tabla_ant, tipo_ant;
					//struct _ev_dmedios event_dpago;
					struct {
						long caja_z;
						long id_evento;
						int posicion;
					}indice;

					indice.caja_z = ev_cont.caja_z;
					indice.id_evento = ev_cont.id_evento;
					indice.posicion = ev_cont.posicion;

					//area_ant = SELECTED( );
					tabla_ant = SELECTED2();
					tipo_ant = SELECTED_TIPO();
					//SELECT( AREA_COD_BARRA );
					//SET_ORDER( 3 );
					//GET_EQUAL( (char*)&indice );
					SELECT_TABLE( T_EV_COD_BARRA, tipo_ev_cod_barra );
					SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION_COD_BARRA");
					SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION_COD_BARRA='%d' ", indice.caja_z,
									indice.id_evento, indice.posicion );
					RUN_QUERY(NO);

					if( FOUND2( ) ){
						event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
						event_cod_barra.id_evento = RAM_ID_EVENTO;
						event_cod_barra.posicion_medio = RAM_P_TICKET;
						#ifdef INVEL_L
						//BEGIN_TRANSACTION();
						#endif
						INSERT2(NO);
						#ifdef INVEL_L
						//END_TRANSACTION();
						#endif
						//para cobros garantia
						if( TIPO_COBRO_VENTA( ev_cont.cod_cobro ) == COBRO_GARANTIA ){
							//SELECT( AREA_COD_BARRA );
							//SET_ORDER( 3 );
							//GET_EQUAL( (char*)&indice );
							//SKIP( 1 );
							SELECT_TABLE( T_EV_COD_BARRA, tipo_ev_cod_barra );
	                        SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION");
         					SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld', AND POSICION='%d' ", indice.caja_z,
		     							indice.id_evento, indice.posicion );
			         		RUN_QUERY(NO);
							SKIP2(1);
							event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
							event_cod_barra.id_evento = RAM_ID_EVENTO;
							event_cod_barra.posicion_medio = RAM_P_TICKET;
							#ifdef INVEL_L
							//BEGIN_TRANSACTION();
							#endif
							INSERT2(NO);
							#ifdef INVEL_L
							//END_TRANSACTION();
							#endif
						}
					}

					//DPAGO
					//SELECT( AREA_AUX8 );
                    SELECT_TABLE( T_EVP_TMP, TT_ORIG );
					GO2(TOP);
					while( RECCOUNT2( NO, NULL) > 0 && /*!BtrvEOF()*/ !dbEOF() ) {
						//Se utiliza variable provisoria para indicar una importacion con datos adicionales
						SET_MEMORY_CHAR( __ram_importacion_activa, 1 );
						memset( event_dpago, 0, sizeof( struct _ev_dmedios ) );
						memcpy( event_dpago, &ev_dpago, sizeof( struct _ev_dmedios ) );
						event_dpago->caja = NRO_CAJA;
						event_dpago->nro_z =  NRO_Z;
						event_dpago->id_evento = RAM_ID_EVENTO;
						event_dpago->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
						if( !NRO_FECHA_OPERACION )
							SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

						event_dpago->fecha_ticket = NRO_FECHA_OPERACION;
						//SELECT( AREA_DPAGO );
                        SELECT_TABLE( T_EV_DMEDIOS, TT_ORIG );
						if( INSERT2(NO) ) {
							glog("NO INSERTO EVENTO EN DPAGO.BTR ", LOG_VENTAS,2 );
						}
						//SELECT( AREA_AUX8 );
                        SELECT_TABLE( T_EVP_TMP, TT_ORIG );
						SKIP2( 1 );
					}
					//SELECT( area_ant );
                    SELECT_TABLE( tabla_ant, tipo_ant );
				}
				if( RAM_NOTA_CR > 0 || event_ticket.tipo_evento == _EVENT_TICKET_ESPERA )
					MUESTRA_SUBTOTAL(SI);
				/*********************************************************************/
			} else {
				rta =0;
				//SET_ORDER2("ID_EVENTO");
			}
		} else
			rta = 0;

		if( rta == 1 ) {
			REVERTIR_DIFERENCIA_PRECIO( event_e_ticket.caja_z, event_e_ticket.id_evento, event_e_ticket.posicion );
			CR1_INICIALIZAR_VAR_TICKET( NO );
			if( remoto ) {
				//SELECT( AREA_AUX4 );
				SELECT_TABLE( T_EVC_TMP, TT_ORIG );
				SKIP2( 1 );
			}
			else {
				//SELECT( AREA_E_TICKET );
				//GO( TOP );
				//GET_EQUAL( ( char* )&id );
				SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
				SET_WHERE("ID_EVENTO = '%ld'",id);
				SET_ORDER2("ID_EVENTO");
				RUN_QUERY(NO);
				SKIP2( i );
			}
		}
	}

	if( rta == 1 ) {
		if( remoto ) {
		   if( eventos.tipo_evento_orig == 4 && eventos.tipo_evento == _EVENT_TICKET_ESPERA && eventos.tipo_evento_importado == 4){
			   SET_MEMORY_CHAR( __modo_devolucion, 1 ); //no venia con la bandera levantada en ticket espera
			}
		}

		if( remoto &&(  RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_TOTAL
			|| (eventos.tipo_evento == _EVENT_TICKET_ESPERA && ( MODO_DEVOLUCION ||  RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL  || RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL  )))){
			//SELECT( AREA_AUX10 );
			SELECT_TABLE( T_EVBP_TMP, TT_ORIG );
			GO2( TOP );
			while( /*!BtrvEOF()*/ !dbEOF() ) {
					if( ev_cont.id_evento == ev_b_promo.id_evento && ev_cont.caja_z == ev_b_promo.caja_z ){
						ev_b_promo.posicion;
						if( eventos.tipo_evento != _EVENT_TICKET_ESPERA)
						ev_b_promo.precio_fijo *= -1;
						GRABAR_B_PROMO(ev_b_promo);
					}
				SKIP2( 1 );
			}
		}



		if( !VARIOS_SCANNER_HABILITADO ) {
			PETICION_DE_CODIGO( 1 );
		}
		if( base_dto ) {
			NRO_ARCHIVO = 0;
			handle_archivo = _CREAT( "dp_imp.dat", S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
			if( handle_archivo > 0 ) {
				_WRITE( handle_archivo, ( char* )base_dto, sizeof( double ) * _LARGO_TICKET, __LINE__,
						__FILE__ );
				_CLOSE( handle_archivo, __LINE__, __FILE__ );
			}
			free( base_dto );
		}
		#ifdef COMPILAR_EV_DESPRO
		IMPORTAR_DESCUENTOS( remoto, id, &ev_despro );
		#endif
		SET_MEMORY_INT( __nro_cod_cuota, event_ticket.cod_financiacion );
		SET_MEMORY_LONG( __ram_cliente_afinidad, event_ticket.cliente_afinidad );
		SET_MEMORY( __ram_string_cliente_afinidad, event_ticket.str_cliente_afinidad );
		SET_MEMORY_DOUBLE( __ram_acu_puntos, event_ticket.puntos );
		SET_MEMORY_CHAR( __ram_descuento_importe, dto_importe );
		if( remoto ) {
			if(eventos.tipo_evento_orig == 21 || ( eventos.tipo_evento_orig == 4 && eventos.tipo_evento == _EVENT_TICKET_ESPERA && eventos.tipo_evento_importado == 4)){ //si es una devolucion o
				SET_MEMORY_INT( __ram_modo_devolucion, 1);
				SET_MEMORY_CHAR( __modo_devolucion, 1 );
				MOSTRAR_MODO_DEVOLUCION();
			}
			CERRAR_EVENTOS_TEMPORALES();
		}
	}

	return rta;
}
/*****************************************************************************/
int CARGA_ITEM( int h, double *base_dto )
/*****************************************************************************/
{
    char cod_barra[17];
    int encontro = 0, tipo = 0, tipo2, corregir = 0;
    int signo;
    int importa_desde_negativa = 0, i;
    double cantidad;
	double tot = 0;
    if( event_e_ticket.origen == _ORIGEN_DESC_PESOS || event_e_ticket.origen == _ORIGEN_DESC_PORC
     || event_e_ticket.origen == _ORIGEN_VARIACION_MEDIO ) {
        if( event_e_ticket.importe_sin_iva != 0 ) {
            tipo = _GRAVADO;
        }
        tipo2 = 0;
    }
    else {
        tipo = event_e_ticket.tipo;
        tipo2 = event_e_ticket.tipo2;
    }
    signo = ( tipo & _RESTA ) ? -1 : 1;
    SET_MEMORY_INT( __ram_correccion, ( tipo2 & _CORRECCION ) );
    // invertimos importe, ya que es una importacion para una caja en modo
    // devolucion.
    if( ( event_ticket.importe_sin_iva + event_ticket.exento >= 0 ) ) {
    }
    else {
        importa_desde_negativa = 1;
    }
    if( MODO_DEVOLUCION
			|| ( RAM_NOTA_CR
				&& ( RAM_MODO_DEVOLUCION == _NC_TOTAL || ( RAM_MODO_DEVOLUCION == _NC_RF_INTERME ) ) )
			|| ( ( RAM_NOTA_CR || MODO_DEVOLUCION )
				&& IMPORTACION_CON_SELECCION_ART
				&& ( RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL
					||  RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL ) ) ) {
        signo *= -1;
    }
	if( ( RAM_NOTA_CR || MODO_DEVOLUCION ) && ( tipo & _DEV_ENVASE && event_e_ticket.total > 0 ) ) {
        signo *= -1;
    }

    if(( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_TOTAL )
       && ( tipo & _ENVASE ) && event_e_ticket.cantidad < 0 ) {//DEVOLUCION DE ENVASE
      signo *= -1;
    }

   if( event_ticket.tipo_evento == _EVENT_TICKET_ESPERA 
		&&  RAM_AGREGA_ITEM_TICK_IMPORT == 2 //enter en ticket en espera muy parchado
		&& ( MODO_DEVOLUCION 
			|| ( ( RAM_NOTA_CR || ( RAM_FACTURA == 2 && event_ticket.tipo_evento_importado == 4 ) ) 
				&& event_e_ticket.origen != 4 ) ) ) {
        signo *= -1;
    }

    //encontro = ASIGNA_CODIGO_ESPECIAL( event_e_ticket.cod_articulo );
	{//Viene basura al final del vector de codigo en barra
		char cadena[17];
		int stop = 0, dig = 0,pos=0;
		

		memset(cadena, 0, 17);
		memcpy( cadena, event_e_ticket.cod_barra, 17 );
		dig = strlen( cadena );
		for( pos = 0; pos < dig && !stop ;pos++ ) {
			if( cadena[pos] == ' ' ){
				cadena[pos] = 0;
				stop = 1 ;
			}
		}
		memset(event_e_ticket.cod_barra, 0, 17);
		memcpy( event_e_ticket.cod_barra, cadena, 17 );
	}

    encontro = ASIGNA_COD_ESPECIAL_COD_BARRA( event_e_ticket.cod_articulo , event_e_ticket.cod_barra );
    if( encontro ) {
        if( base_dto ) {
            base_dto[event_e_ticket.posicion - 1] = event_e_ticket.descuento * signo;
        }
		if( event_ticket.tipo_evento == _EVENT_TICKET_ESPERA && tipo & _RESTA  ) {
			/* agregué la condicion para los pesables, para la recuperación 
			de los articulos pesables anulados.. que si bien se agrupan, 
			existiendo diferencia de redondeo .. no se agrupan y ocasionan errores al sumer en lugar de restar*/
			if( !((event_e_ticket.tipo2 & _PEDIR_PRECIO)||(event_e_ticket.tipo3 & _PESABLE)))
				signo *= -1;
			event_e_ticket.cantidad *= signo;
		} else
        event_e_ticket.cantidad *= ( tipo & _RESTA ) ? -1 : 1;
        if( ITEM_PESABLE && event_e_ticket.precio_unitario ) {
            //cantidad = event_e_ticket.total / event_e_ticket.precio_unitario;
            cantidad = event_e_ticket.cantidad;
            event_e_ticket.cantidad = ( float )( fabs( cantidad )
                                               * ( ( event_e_ticket.cantidad < 0 ) ? -1 : 1 ) );
        }
        event_e_ticket.importe_sin_iva *= signo;
        event_e_ticket.imp_int *= signo;
        event_e_ticket.costo *= signo;
        event_e_ticket.recargo_tarjeta *= signo;
        event_e_ticket.exento *= signo;
        event_e_ticket.iva1 *= signo;
        event_e_ticket.iva2 *= signo;
        event_e_ticket.ila *= signo;
        event_e_ticket.iha *= signo;
        event_e_ticket.alicuota *= signo;
		event_e_ticket.monto_impuesto[0] *= signo;
		event_e_ticket.monto_impuesto[1] *= signo;
		event_e_ticket.monto_impuesto[2] *= signo;
		event_e_ticket.monto_impuesto[3] *= signo;
        // asignamos el contenido de un item (e_ticket.all) a las variables
        // en la RNV
        SET_MEMORY_CHAR( __item_envase, ( tipo & _ENVASE ) ? 1 : 0 );
        SET_MEMORY_CHAR( __item_dev_envase, ( tipo & _DEV_ENVASE ) ? 1 : 0 );
        SET_MEMORY_INT( __ram_anulacion, ( tipo & _RESTA ) ? 1 : 0 );
        SET_MEMORY_CHAR( __item_gravado, ( tipo & _GRAVADO ) ? 1 : 0 );
        SET_MEMORY_CHAR( __item_cupon_dev_env, ( tipo & _CUPON_DEV_ENV ) ? 1 : 0 );
        SET_MEMORY_CHAR( __item_credito, ( tipo & _CREDITO ) ? 1 : 0 );
        SET_MEMORY_CHAR( __item_scanner, ( tipo & _SCANNER ) ? 1 : 0 );
        SET_MEMORY_CHAR( __item_cod_iva, event_e_ticket.cod_iva );
        SET_MEMORY_INT( __item_cod_bajo, event_e_ticket.cod_depto_old );
		SET_MEMORY_INT( __item_depto, event_e_ticket.cod_depto );

		{
		double diferencia=0.0, itemprecio = ( event_e_ticket.importe_sin_iva 
						   + event_e_ticket.exento
							+ event_e_ticket.iva1 + event_e_ticket.iva2 + event_e_ticket.ila
							+ event_e_ticket.iha + event_e_ticket.iva2 +  event_e_ticket.imp_int/*+ event_e_ticket.alicuota*/ );
		itemprecio = (int) (itemprecio * 100000) ;
		itemprecio = itemprecio / 100000 ;
		diferencia =fabs( (fabs( itemprecio) - fabs(ITEM_PRECIO)));
		if( RAM_NOTA_CR > 0 
			&& ( RAM_MODO_DEVOLUCION == _NC_TOTAL 
				|| RAM_MODO_DEVOLUCION == _NC_RF_INTERME 
				|| RAM_MODO_DEVOLUCION == _DEVOL_INTERME )
			&& ( event_e_ticket.origen == _ORIGEN_PROMOCION_DESC 
				|| event_e_ticket.origen == _ORIGEN_VARIACION_MEDIO 
				|| ( diferencia ) > 0.01 ) ) {
					if( articulo.precio =='S' && fabs(event_e_ticket.alicuota) > 0.01) { //ojo que tenemos que tocar el precio
						itemprecio = event_e_ticket.alicuota  * ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
						if( RAM_NOTA_CR == 2 && event_ticket.tipo_evento != _EVENT_TICKET_FACTURA_B )
							ROUND2(&itemprecio, ENTEROS,2);
						event_e_ticket.iva1 = itemprecio - event_e_ticket.alicuota;
						event_e_ticket.importe_sin_iva =  event_e_ticket.alicuota;
					} else {
						if( articulo.precio =='S') {
							itemprecio = event_e_ticket.importe_sin_iva  * ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
							if( RAM_NOTA_CR == 2 && event_ticket.tipo_evento != _EVENT_TICKET_FACTURA_B)
								ROUND2(&itemprecio, ENTEROS,2);
							event_e_ticket.iva1 = itemprecio - event_e_ticket.importe_sin_iva;
						}
					}

			SET_MEMORY_DOUBLE( __item_precio, itemprecio);
		}
		if( diferencia < 0.0015 && diferencia > 0.00001 ){
			int signoss= 1;
			if( (event_e_ticket.tipo & _RESTA && event_e_ticket.tipo & _ENGANCHADO && event_e_ticket.tipo & _ENVASE) && RAM_NOTA_CR > 0 && RAM_MODO_DEVOLUCION == _NC_TOTAL )
				//para estos casos calcula mal el signo			
				signoss = -1;
			if( articulo.precio == 'S') {//ojo con estos porque introducen diferencias de redondeo cuando deben dar 1.0000 y dan 1.0000400 ej
				double dds = 0.00;
				dds = TRUNCAR(itemprecio, 4) ;
				itemprecio = dds;
			}
			SET_MEMORY_DOUBLE( __item_precio, itemprecio * signoss); //para ajustar al maximo la diferencia
		}

		
        /*SET_MEMORY_DOUBLE( __item_precio, 
                           event_e_ticket.importe_sin_iva 
						   + event_e_ticket.exento
							+ event_e_ticket.iva1 + event_e_ticket.iva2 + event_e_ticket.ila
							+ event_e_ticket.iha + event_e_ticket.iva2 + event_e_ticket.alicuota );*/
		 
		}
		/*Se agrega este for para la diferencia de precio automatica*/
		for( i = 0;i < 4;i++ ) {
            _SET_MEMORY_FLOAT( __item_monto_impuesto, i, event_e_ticket.monto_impuesto[i] );
        }
		if( PRECALCULAR_IMPUESTOS ) {
			for( i = 0;i < 4;i++ ) {
				tot += _ITEM_MONTO_IMPUESTO( i );
			}
			SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO + tot );
		}
        SET_MEMORY_DOUBLE( __ram_cantidad, event_e_ticket.cantidad );
        SET_MEMORY_DOUBLE( __ram_cantidad_original, ROUND( event_e_ticket.cantidad, 3, 3 ) );
		if( ( RAM_NOTA_CR || MODO_DEVOLUCION ) && ( tipo & _DEV_ENVASE  && articulo.precio_sin_iva < 0 )) {
            SET_MEMORY_DOUBLE( __ram_cantidad, -RAM_CANTIDAD );
            SET_MEMORY_DOUBLE( __ram_cantidad_original, -RAM_CANTIDAD_ORIGINAL );
        }
        SET_MEMORY_INT( __item_presentacion, event_e_ticket.cant_presentacion );
        SET_MEMORY_CHAR( __item_cod_presentacion, event_e_ticket.cod_presentacion );
        SET_MEMORY_LONG( __item_cod_especial, event_e_ticket.cod_articulo );
		ROUND2(&event_e_ticket.imp_int, ENTEROS,3);
		corregir = CORRECCION_REDONDEO_SISTEMA( &event_e_ticket.imp_int );

        SET_MEMORY_DOUBLE( __item_imp_int, event_e_ticket.imp_int );
        SET_MEMORY_DOUBLE( __item_costo, event_e_ticket.costo );
        SET_MEMORY_DOUBLE( __item_recargo, 0 );
        SET_MEMORY_DOUBLE( __item_iva1_precio, event_e_ticket.iva1 );
        SET_MEMORY_DOUBLE( __item_iva1_rec, 0.0 );
        SET_MEMORY_DOUBLE( __item_iva2_precio, event_e_ticket.iva2 );
        SET_MEMORY_DOUBLE( __item_iva2_rec, 0.0 );
        for( i = 0;i < 4;i++ ) {
            _SET_MEMORY_FLOAT( __item_monto_impuesto, i, event_e_ticket.monto_impuesto[i] );
        }
        SET_MEMORY_LONG( __item_cod_clasificacion, event_e_ticket.cod_clasificacion_old );
		SET_MEMORY( __item_cod_clasificacion_str, event_e_ticket.cod_clasificacion );
        if( MODO_DEVOLUCION || NC_PERMANENTE ) {
            if( importa_desde_negativa ) {
                SET_MEMORY_CHAR( __item_cod_movimiento, event_e_ticket.cod_movimiento );
            }
            else {
                SET_MEMORY_CHAR( __item_cod_movimiento, _DEVOLUCION );
            }
		} else if( event_e_ticket.cod_movimiento == _DIFERENCIA_PRECIO
			|| event_e_ticket.cod_movimiento == _DIFERENCIA_PRECIO_ANULADA ) {
			SET_MEMORY_CHAR( __item_cod_movimiento, event_e_ticket.cod_movimiento );
		} else {
            SET_MEMORY_CHAR( __item_cod_movimiento, _VENTA_NORMAL );
        }
        strncpy( cod_barra, event_e_ticket.cod_barra, 17 );
        cod_barra[L_CODIGO] = 0;
        SET_MEMORY( __item_cod_barra, cod_barra );
        SET_MEMORY( __ram_cod_barra_original, cod_barra );
        if( tipo & _GRAVADO ) {
			//double diff = fabs( articulo.precio_sin_iva ) + event_e_ticket.importe_sin_iva + event_e_ticket.imp_int;
			//aca tener cuenta si tomo el precio original ESC
			double diff =0.00;
			if( RAM_AGREGA_ITEM_TICK_IMPORT == 2  )
				diff = fabs( articulo.precio_sin_iva ) - fabs(event_e_ticket.importe_sin_iva + event_e_ticket.imp_int);
			else
				diff = 0.0;
			diff = fabs( diff);
			//ojo que en una NC el articulo.precio_sin_iva es negativo ?¿?¿
			if(( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME )
				&&( diff > 0.00003 && diff < 0.03  && event_e_ticket.origen != _ORIGEN_PROMOCION  ) ) {
					int signos =1;
				if( event_e_ticket.tipo & _RESTA )
						signos = -1;
				SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva * signos );
			} else {
				if( fabs(event_e_ticket.alicuota) > 0.01 && event_e_ticket.origen == _ORIGEN_PROMOCION ) 
					SET_MEMORY_DOUBLE( __item_precio_sin_iva, fabs(event_e_ticket.alicuota) );
				else { 
					if( RAM_AGREGA_ITEM_TICK_IMPORT == 2  )
						SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva);
				else 
            SET_MEMORY_DOUBLE( __item_precio_sin_iva, event_e_ticket.importe_sin_iva );
				}
        }
        }
        else {
			//esto se puede optimizar
			int signos =1;
			double diff = fabs( articulo.precio_sin_iva ) + event_e_ticket.exento;
			diff = fabs( diff);
			if(( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME )
				&&( diff > 0.0004 && diff < 0.03 && event_e_ticket.origen != _ORIGEN_PROMOCION  ) ) {
					if( event_e_ticket.tipo & _RESTA )
						signos = -1;
					SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva * signos);
			} else {
				/*if( event_e_ticket.tipo & _RESTA )
						signos = -1;*/
				SET_MEMORY_DOUBLE( __item_precio_sin_iva, event_e_ticket.exento /**signos*/ );
        }
        }
        if( !ITEM_PEDIR_PRECIO && !ITEM_PESABLE ) {
            SET_MEMORY_INT( __ram_cant_bultos, ( int )( RAM_CANTIDAD ) );
        }
        SET_MEMORY_LONG( __item_comprobante, event_e_ticket.comprobante );
        SET_MEMORY_INT( __ram_motivo_descuento, event_e_ticket.motivo );
        SET_MEMORY_INT( __item_motivo_devolucion, event_e_ticket.motivodev );
    }
	else {
		MENSAJE_CON_PAUSA(ST(S_NO_SE_ENCUENTRA_EL_ARTICULO),25);
		articulos = 1;
	}
	/********************* Cobros Venta *************************/
	SET_MEMORY_INT( __item_cod_cobro, event_e_ticket.cod_cobro );

    return encontro;
}
/*****************************************************************************/
void PANTALLA_IMPORTAR_T( char *titulo )
/*****************************************************************************/
{
}
/*****************************************************************************/
int PIDE_DATOS_IMPORTAR_T( int remoto, char *titulo, int modo )
/*****************************************************************************/
{
    int rta = 0;
	char cad_aux[30];
    int len1 = 0, len2 = 0, len3 = 0, len4 = 0, len5 = 0, len6 = 0, pedir_dato=0;
    char *cad = NULL, *tit_box_dev = NULL;

	SET_MEMORY_LONG( _ticket_secuencia, 0L );
	SET_MEMORY_LONG( _ticket_n_ticket, 0L );
	SET_MEMORY_INT( _ticket_fecha, 0 );
	SET_MEMORY_INT( _ticket_caja, 0 );
	SET_MEMORY_INT( _ticket_tipo_comp, 0L );//PARA INDICAR QUE CAMPO BUSCAR TIPO COMPROBANTE O NRO TICKET,

	if( modo == _TE_CON_MENU ) {
		/* no tengo que pedir los datos porque el cajero lo selecciona del menu */
		if( PEDIR_TICKET_EN_ESPERA( ) ) {
			rta = 1;
		} else {
			rta = 0;
		}
		return ( rta );
	}
	if( modo == _DATOS_CON_MENU ) {
		
        /*---------------------- Dibuja la pantalla ----------------------------*/
        len1 = strlen( ST( S_FECHA________ ) );
		
		if( PEDIR_CAJA_Y_SECUENCIA ){
			sprintf( cad_aux, "%s", ST( S_SECUENCIA____ /* S_TRANSACCION__ */) );
			len2 = strlen( cad_aux );
		}else{
			len2 = strlen( ST( S_TICKET_______ ) );
			sprintf( cad_aux, "%s", ST(  S_TIPO_DE_COMP_ANTERIOR ) );
			len6 = strlen( cad_aux );
	

			/*
			#define  _EVENT_TICKET                          0
			#define  _EVENT_FAC_A                           3
			#define  _EVENT_FAC_B                           4
			#define  _EVENT_NC_A                            5
			#define  _EVENT_TICKET_FACTURA_A                35
			#define  _EVENT_TICKET_FACTURA_B                36
			
			*/
		}
        
		len3 = remoto ? ( strlen( ST( S_CAJA_________ ) ) ) : 0;
        len4 = len1 + len2 + len3 + len6 + 4;
        cad = ( char* )MALLOC( len4 );
        len5 = strlen( titulo ) + 1;
        tit_box_dev = ( char* )MALLOC( len5 );
		if( cad != NULL && tit_box_dev != NULL ) {
            memset( cad, 0, len4 );
            
			memset( tit_box_dev, 0, len5 );
            strncpy( tit_box_dev, titulo, len5 );
            
			strncat( cad, ST( S_FECHA________ ), len1 );
            strncat( cad, "~", 1 );// "~" sirve de separador en Java

			if( PEDIR_CAJA_Y_SECUENCIA ){
				sprintf( cad_aux, "%s", ST( S_SECUENCIA____/* S_TRANSACCION__ */) );
			}else{
				sprintf( cad_aux, "%s", ST(  S_TICKET_______ ) );
				
			}
			strncat( cad, cad_aux, len2 );
			if(len6){
				strcat( cad, "~" );
				sprintf( cad_aux, "%s", ST(  S_TIPO_DE_COMP_ANTERIOR ) );
				strncat( cad, cad_aux, len6 );
			}

			if( remoto ) {
                strcat( cad, "~" );
                strncat( cad, ST( S_NRO__CAJA____ ), len3 );
            }
            
			ExecuteSSAction( A_PEDIR, _CONCATENADO, tit_box_dev, cad, NULL, " " );
            /*------------------------ Pide los datos -----------------------------*/
			
			
			ExecuteSSAction( A_PEDIR_DATO, pedir_dato++, " ", " ", NULL, " " );
            SET_MEMORY_INT( _ticket_fecha, PEDIR_FECHA( 44 - ancho_panel / 2, pedir_dato+8, NULL ) );
            
			if( _TICKET_FECHA > 0 ) {

				ExecuteSSAction( A_PEDIR_DATO, pedir_dato++, " ", " ", NULL, " " );
				if( PEDIR_CAJA_Y_SECUENCIA ){
					SET_MEMORY_LONG( _ticket_secuencia, PEDIR_ENTERO( 44 - ancho_panel / 2, pedir_dato+8, 8, NO ) );

				}else{
					struct _cabmenu *cab;
					int columnas = 1;
					struct _menu items[] = {
 							{" 1) TICKET           ",'1', _EVENT_TICKET },
							{" 2) FACURA A         ",'2', _EVENT_FAC_A },
							{" 3) FACURA B         ",'3', _EVENT_FAC_B },
							{" 4) TICKET FACTURA A ",'4', _EVENT_TICKET_FACTURA_A },
							{" 5) TICKET FACTURA B ",'5', _EVENT_TICKET_FACTURA_B }
						};
					SET_MEMORY_LONG( _ticket_n_ticket,  PEDIR_ENTERO( 44 - ancho_panel / 2, pedir_dato+8, 8, NO ) );
					cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
					if( cab != NULL ) {
						cab[0].nombre = "Tipo de Comprobante";
						cab[0].align = ALIGN_LEFT;
						cab[0].width = 40;
						ExecuteSSAction( A_PEDIR_DATO, pedir_dato++, " ", " ", NULL, " ");
						memset(cad_aux,0,sizeof(cad_aux));
						
						rta = MENU( ST( S_TIPO_DE_COMP_ANTERIOR ), items, 5, cab, columnas );
						sprintf(cad_aux,"%i",rta);
						//MOSTRAR_ENTERO( 44 - ancho_panel / 2, pedir_dato+8, "99", rta );//10
						MOSTRAR_INPUT_CADENA( cad_aux, pedir_dato+8 );
						SET_MEMORY_INT( _ticket_tipo_comp,  rta);
						rta = 0;
						free( cab );
					}
				}
				
                if( _TICKET_N_TICKET > 0 || _TICKET_SECUENCIA > 0 ) {
                    if( remoto ) {
                        ExecuteSSAction( A_PEDIR_DATO, pedir_dato++, " ", " ", NULL, " " );
                        SET_MEMORY_INT( _ticket_caja, PEDIR_ENTERO( 44 - ancho_panel / 2, pedir_dato+8, 4, NO ) );//11
                        if( _TICKET_CAJA > 0 ) {
                            rta = 1;
                        }
                    }
                    else {
                        SET_MEMORY_INT( _ticket_caja, NRO_CAJA );
                        if( _TICKET_CAJA > 0 ) {
                            rta = 1;
                        }
                    }
                }



            }
            ExecuteAction( A_FIN_PEDIR );
        }
	
	}else //TRADICIONAL
		if( PEDIR_CAJA_Y_SECUENCIA &&
			!(modo == _TE_TRADICIONAL && IMPRESORA_FISCAL == FISCAL ) ){ //LA TM220 NO IMPRIME SECUENCIA EN UN TICKET ESPERA
			
	        
			//if( MODO_NEGOCIO == SERVICIOS ) {
				//sprintf( cad_aux, "SECUENCIA :" );
			//} else {
				sprintf( cad_aux, "%s", ST( S_TRANSACCION__ ) );
			//}
			len1 = strlen( ST( S_CAJA_________ ) );
			len2 = strlen( cad_aux );
			len3 = strlen( titulo ) + 1;
			len4 = len1 + len2 + 2;
			cad = ( char* )MALLOC( len4 );
			tit_box_dev = ( char* )MALLOC( len3 );
			if( cad != NULL && tit_box_dev != NULL ) {
				memset( cad, 0, len4 );
				memset( tit_box_dev, 0, len3 );
				strncat( cad, ST( S_CAJA_________ ), len1 );
				strcat( cad, "~" );// "~" sirve de separador en Java
				strncat( cad, cad_aux, len2 );
				strncpy( tit_box_dev, titulo, len3 );
				ExecuteSSAction( A_PEDIR, _CONCATENADO, tit_box_dev, cad, NULL, " " );
				/*------------------------ Pide los datos -----------------------------*/
				ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL, " " );
				SET_MEMORY_INT( _ticket_caja, PEDIR_ENTERO( 44 - ancho_panel / 2, 9, 4, NO ) );
				if( _TICKET_CAJA > 0 ) {
					ExecuteSSAction( A_PEDIR_DATO, 1, " ", " ", NULL, " " );
					SET_MEMORY_LONG( _ticket_secuencia, PEDIR_ENTERO( 44 - ancho_panel / 2, 10, 8, NO ) );
					if( _TICKET_SECUENCIA > 0 ) {
						rta = 1;
					}
				}
				ExecuteAction( A_FIN_PEDIR );
			}
		}
		else {
			/*---------------------- Dibuja la pantalla ----------------------------*/
			len1 = strlen( ST( S_FECHA________ ) );
			len2 = strlen( ST( S_TICKET_______ ) );
			len3 = remoto ? ( strlen( ST( S_NRO__CAJA____ ) ) ) : 0;
			len4 = len1 + len2 + len3 + 3;
			cad = ( char* )MALLOC( len4 );
			len5 = strlen( titulo ) + 1;
			tit_box_dev = ( char* )MALLOC( len5 );
			if( cad != NULL && tit_box_dev != NULL ) {
				memset( cad, 0, len4 );
				memset( tit_box_dev, 0, len5 );
				strncpy( tit_box_dev, titulo, len5 );
				strncat( cad, ST( S_FECHA________ ), len1 );
				strncat( cad, "~", 1 );// "~" sirve de separador en Java
				strncat( cad, ST( S_TICKET_______ ), len2 );
				if( remoto ) {
					strcat( cad, "~" );
					strncat( cad, ST( S_NRO__CAJA____ ), len3 );
				}
				ExecuteSSAction( A_PEDIR, _CONCATENADO, tit_box_dev, cad, NULL, " " );
				/*------------------------ Pide los datos -----------------------------*/
				ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL, " " );
				SET_MEMORY_INT( _ticket_fecha, PEDIR_FECHA( 44 - ancho_panel / 2, 9, NULL ) );
				if( _TICKET_FECHA > 0 ) {
					ExecuteSSAction( A_PEDIR_DATO, 1, " ", " ", NULL, " " );
					SET_MEMORY_LONG( _ticket_n_ticket,
									 PEDIR_ENTERO( 44 - ancho_panel / 2, 10, 8, NO ) );
					if( _TICKET_N_TICKET > 0 ) {
						if( remoto ) {
							ExecuteSSAction( A_PEDIR_DATO, 2, " ", " ", NULL, " " );
							SET_MEMORY_INT( _ticket_caja, PEDIR_ENTERO( 44 - ancho_panel / 2, 11, 4, NO ) );
							if( _TICKET_CAJA > 0 ) {
								rta = 1;
							}
						}
						else {
							SET_MEMORY_INT( _ticket_caja, NRO_CAJA );
							if( _TICKET_CAJA > 0 ) {
								rta = 1;
							}
						}
					}
				}
				ExecuteAction( A_FIN_PEDIR );
			}
		}
	if( _TICKET_CAJA > 0 )
		SET_MEMORY_INT( __ram_caja_anterior, _TICKET_CAJA );
	if( _TICKET_SECUENCIA > 0L )
		SET_MEMORY_LONG( __nro_evento_anterior, _TICKET_SECUENCIA );
	if( _TICKET_TIPO_COMP > 0L )//GET_MEMORY_LONG( _ticket_tipo_comp )
		SET_MEMORY_INT( __ram_tipo_comp_anterior, (char)_TICKET_TIPO_COMP );
	if( _TICKET_FECHA > 0 )
		SET_MEMORY_INT( __ram_fecha_anterior, _TICKET_FECHA );
	if( _TICKET_N_TICKET > 0L )
		SET_MEMORY_LONG( __ram_boleta_anterior, _TICKET_N_TICKET );
    FREEMEM( cad );
    FREEMEM( tit_box_dev );

    return ( rta );
}

/***************************************************************************/
int OBTENER_EVENTOS_TEMPORALES( int ev_requerido, int caja, long nro_ticket, long secuencia,
	int fecha, long id_evento, char origen, int online, int *res_online ,int comprob_original)
/***************************************************************************/
{
    int error = 0;

    if ( res_online ) {
        *res_online = NO; //Indica si la obtencion fue local
    }
    error = OBTENER_EVENTOS_TEMPORALES_LOCAL( ev_requerido, caja, nro_ticket, secuencia,
	                                      fecha, id_evento, origen,comprob_original );
    ///error = 2 -> error en cambio de cliente
	if( error && error != 2 && ConfIni.odbcServer[0] != 0 ) {
        //if( GET_DBR_ON_LINE( ) ) {
            if( res_online ) {
                *res_online = SI;
            }
            error = OBTENER_EVENTOS_TEMPORALES_ONLINE( ev_requerido, caja, nro_ticket, secuencia,
    	                                               fecha, id_evento, origen );
        /*}
        else {
            MENSAJE_STRING( S_ERROR_EN_EVENTOS_TEMPORALES );
        }*/
    }

    return error;
}

/***************************************************************************/
int OBTENER_EVENTOS_TEMPORALES_LOCAL( int ev_requerido, int caja, long nro_ticket, long secuencia,
	int fecha, long evento, char origen, int comprob_original )
/***************************************************************************/
{
    int error = 0, historico = 0, inexistente = 0, aux = 0, ticket_factura = 0, cambio_cliente = 1;
    long id_evento, caja_z=0;
    struct _eventos eventos;
    char log[150] = {'/0'};
	
	if( fecha < GET_FECHA_DOS() && ev_requerido != 30 ){
		caja_z = 0;//no tengo la fecha de caja z anterior la debo buscar
	}else{
		caja_z = ( long )( caja )*100000L + ( NRO_Z % 100000L );
	}
	
    
	GRABAR_LOG_SISTEMA( "OBTENIENDO EVENTOS TEMPORALES LOCALES" , LOG_VENTAS,2 );
    _snprintf( log, sizeof(log),
        "evento_req %ld, caja %d, caja_z %ld nro_ticket %ld, secuencia %ld, fecha %d, origen %i",
        ev_requerido, caja, ( long )( caja )*100000L + ( NRO_Z % 100000L ), nro_ticket, secuencia, fecha, origen );
    GRABAR_LOG_SISTEMA(log, LOG_DEBUG,2);

    if( evento > 0  ) {
        CEREAR_TEMPORALES ();
        if (!LOCAL_OBTENER_TABLA(/*_TICKET_BTR*/ T_EVENTOS, caja, secuencia, evento, ev_requerido,
                origen, nro_ticket, fecha,&caja_z ) ) {
            error = 1;
        }
    }
    else if ( PEDIR_CAJA_Y_SECUENCIA || REFERENCIAR_POR_SECUENCIA ) {
        CEREAR_TEMPORALES ();
        if (!LOCAL_OBTENER_TABLA(/*_TICKET_BTR*/ T_EVENTOS, caja, secuencia, NO, ev_requerido,
                origen, nro_ticket, fecha ,&caja_z)) {
            error = 1;
        }
    }
    else {
        error = 1;
    }
        CLOSE_TABLE( T_EV_TMP, TT_ORIG );
        if (!error
            //&& ( USE_DATABASE (AREA_AUX3, EV_TMP, (char *) &eventos, sizeof (struct _eventos),
            //                   NULL, 0, 0) == 0 ) ) {
			&& ( OPEN_TABLE( T_EV_TMP, TT_ORIG, (char *) &eventos, sizeof (struct _eventos) ) == 0 ) ) {
				SELECT_TABLE( T_EV_TMP, TT_ORIG );
			    SET_WHERE("");
	            RUN_QUERY(SI);

			   //memcpy( &eventos, &event_ticket, sizeof( struct _eventos ) );

            if ( RECCOUNT2(NO, NULL) > 0 && ( !fecha || eventos.fecha == fecha ) ) {

			id_evento = eventos.id_evento;

            if (!RAM_REFERENCIA_NC && !RAM_CAMBIO ) {
                SET_MEMORY_INT (__ram_bultos, eventos.bultos);
            }
                //CLOSE_DATABASE (AREA_AUX3);
				    CLOSE_TABLE( T_EV_TMP, TT_ORIG );
                if (!LOCAL_OBTENER_TABLA
                    (/*_E_TICKET_BTR*/ T_EV_CONT, caja, NO, id_evento, NO,
                    origen, NO, NO,&caja_z)) {
                    error = 1;
                } else {
	                 //aca en un devolucion y de recuperar ticket en espera el id_evento cambia para obtener
	                 //pago
	                 int evento_aux = id_evento;
	                 if(( eventos.tipo_evento_orig == 21 || eventos.tipo_evento_orig == 6 || eventos.tipo_evento_orig == 5)
	                     && ( eventos.tipo_evento == _EVENT_TICKET_ESPERA ) ){
	                    id_evento =  eventos.id_evento_importado;
					}
                    LOCAL_OBTENER_TABLA ( /*_PAGO_BTR*/ T_PAGOS, caja, NO, id_evento, NO, origen, NO, NO ,&caja_z);
					LOCAL_OBTENER_TABLA ( /*_EV_DPAGO_BTR*/ T_EV_DMEDIOS, caja, NO, id_evento, NO, origen, NO, NO ,&caja_z);
	                id_evento = evento_aux ;
                    LOCAL_OBTENER_TABLA ( /*_EV_B_PROMO_BTR*/ T_EV_B_PROMO , caja, NO, id_evento, NO, origen, NO, NO ,&caja_z);
					cambio_cliente = CAMBIO_CLIENTE( ( origen != _ORIGEN_TICKET_EN_ESPERA )? 
							eventos.tipo_evento : eventos.tipo_evento_orig, 
								eventos.nro_cliente, eventos.cod_financiacion, origen );
                }
            }
            else {
                inexistente = 1;
                error = 1;
				   CLOSE_TABLE( T_EV_TMP, TT_ORIG );
                //CLOSE_DATABASE (AREA_AUX3);
            }
        }

    if( inexistente ) {
        GRABAR_LOG_SISTEMA( ST( S_COMPROBANTE_SOLICITADO_NO_ENCONTRADO ) , LOG_VENTAS,2 );
        //MENSAJE_STRING(S_COMPROBANTE_SOLICITADO_NO_ENCONTRADO);
    }
    else if( error ) {
        //MENSAJE_STRING(S_ERROR_EN_EVENTOS_TEMPORALES);
        GRABAR_LOG_SISTEMA( ST( S_ERROR_EN_EVENTOS_TEMPORALES ) , LOG_VENTAS,2 );
    }
    else {
		if( !cambio_cliente ) {
			error = 2;
		}
        BORRAR_ARCHIVO_REC (ARCH_REC_IMPORT);
    /*Guarda la estructura en un archivo */
        if( GUARDAR_ARCHIVO_REC (ARCH_REC_IMPORT, (char *) &eventos,
            sizeof (struct _eventos)) == -1) {
            GRABAR_LOG_SISTEMA("OBTENER_EVENTOS_TEMPORALES_LOCAL: ERROR GRABANDO REC_IMPORT", LOG_VENTAS,2 );
            }
    }

    return error;
}

/***************************************************************************/
int OBTENER_EVENTOS_TEMPORALES_ONLINE( int ev_requerido, int caja, long nro_ticket, long secuencia,
	int fecha, long evento, char origen )
/***************************************************************************/
{
    int dd, mm, aa;
    char _where[300], cadena[60], car = 0;
    int error = 0, historico = 0, inexistente = 0, aux = 0;
    long caja_z = 0, id_evento = 0;
    struct _eventos eventos;

#ifdef FECHA_ORACLE_SQL	
		if(GetConfIniclienteDbServer(ODBC_SERVIDOR)==4){
		_snprintf( cadena, sizeof( cadena )," %02ld%02ld%04ld ",dd, mm, aa);
	}else{
		/*sql */
		_snprintf( cadena, sizeof( cadena )," %04ld%02ld%02ld",aa, mm, dd);
	}

#endif	
	
	memset( cadena, 0, sizeof( cadena ) );
    GRABAR_LOG_SISTEMA( "OBTENIENDO EVENTOS TEMPORALES ONLINE" , LOG_VENTAS,2 );
    //se usaba y despues se saco...el tema es porque parece ser que el campo fecha en
	//una base oracle no la convierte bien entonces se saco para libertad pero
	//en selectos que es SQL se tiene en cuenta.
    FECHA__ (fecha, &dd, &mm, &aa);
    //Se genera la sentencia WHERE segun los datos a solicitar
    if( evento > 0 ) {
        _snprintf( _where, sizeof( _where ),
            //" WHERE id_evento = %ld AND caja = %d AND nro_z = %ld",
			"id_evento = %ld AND caja = %d AND nro_z = %ld",
            evento, caja, NRO_Z );
        _where[ sizeof( _where ) - 1] = '\0';
    }
    else if (!( ev_requerido == 30 && IMPRESORA_FISCAL == FISCAL ) && PEDIR_CAJA_Y_SECUENCIA) { //LA TM220 NO IMPRIME SECUENCIA EN TE
        if( ev_requerido ) {
            _snprintf( _where, sizeof( _where ),
                //" WHERE caja = %d AND nro_secuencia = %ld AND tipo_evento = %d",/*AND fecha_ticket = '%04ld%02ld%02ld' --- , aa, mm, dd*/
				" caja = %d AND nro_secuencia = %ld AND tipo_evento = %d",/*AND fecha_ticket = '%04ld%02ld%02ld' --- , aa, mm, dd*/
				caja, secuencia, ev_requerido, aa, mm, dd);
                _where[sizeof( _where ) - 1] = '\0';
        }
        else {
            _snprintf( _where, sizeof( _where ),
				//" WHERE caja = %d AND nro_secuencia = %ld AND fecha_ticket = '%04ld%02ld%02ld'",
                " caja = %d AND nro_secuencia = %ld AND fecha_ticket = '%04ld%02ld%02ld'",
                caja, secuencia, aa, mm, dd);
                _where[sizeof( _where ) - 1] = '\0';
       }
        }
        else {
        //if (config.pais != MEXICO) {
        if( REFERENCIAR_POR_SECUENCIA ) {

			/*Para el Piloto de Libertad, Se saca del WHERE "fecha_ticket" porque se tiene que definir un formato fecha standar tanto
			para sql como para oracle, porque el que actualmente envia la caja falla para oracle*/

			if( RAM_FACTURA != 0 ) {
				if( secuencia > 0 )
					_snprintf( _where, sizeof( _where ),
						/*" WHERE caja = %ld AND nro_secuencia = %ld AND tipo_evento = %ld AND fecha_ticket = '%04ld%02ld%02ld'",
							caja, secuencia, ev_requerido, aa, mm, dd);*/
							//" WHERE caja = %ld AND nro_secuencia = %ld AND tipo_evento = %ld ",
							" caja = %ld AND nro_secuencia = %ld AND tipo_evento = %ld ",
							caja, secuencia, ev_requerido);
				else
					_snprintf( _where, sizeof( _where ),
                				//" WHERE caja = %d AND nro_comprobante = %ld AND tipo_evento = %d ",
								" caja = %d AND nro_comprobante = %ld AND tipo_evento = %d ",
								 caja, nro_ticket, ev_requerido );

			} else {
				if( secuencia > 0 )
					_snprintf( _where, sizeof( _where ),
							//" WHERE caja = %ld AND nro_ticket = %ld AND tipo_evento = %ld ",
							" caja = %ld AND nro_ticket = %ld AND tipo_evento = %ld ",
							caja, secuencia, ev_requerido);
				else
					_snprintf( _where, sizeof( _where ),
							//" WHERE caja = %d AND nro_ticket = %ld AND tipo_evento = %d ",
							" caja = %d AND nro_ticket = %ld AND tipo_evento = %d ",
							caja, nro_ticket, ev_requerido );

			}
				_where[sizeof( _where ) - 1] = '\0';
            /*}
            else {
                FECHA__ (_TICKET_FECHA, &dd, &mm, &aa);
                sprintf (_where,
                            " WHERE caja = %ld AND nro_secuencia = %ld AND tipo_evento = %ld AND fecha_ticket = '%04ld%02ld%02ld'",
                            caja, secuencia, ev_requerido, aa, mm, dd);
            }*/
        }
        else {// POR COMPROBANTE

			/*Para el Piloto de Libertad, Se saca del WHERE "fecha_ticket" porque se tiene que definir un formato fecha standar tanto
			para sql como para oracle, porque el que actualmente envia la caja falla para oracle*/

           if( RAM_FACTURA != 0 )
            _snprintf( _where, sizeof( _where ),
                /*" WHERE caja = %d AND nro_comprobante = %ld AND tipo_evento = %d AND fecha_ticket = '%04ld%02ld%02ld'",
                caja, nro_ticket, ev_requerido, aa, mm, dd);*/
				//" WHERE caja = %d AND nro_comprobante = %ld AND tipo_evento = %d ",
				" caja = %d AND nro_comprobante = %ld AND tipo_evento = %d ",
                caja, nro_ticket, ev_requerido );

           else
             _snprintf( _where, sizeof( _where ),
                /*" WHERE caja = %d AND nro_comprobante = %ld AND tipo_evento = %d AND fecha_ticket = '%04ld%02ld%02ld'",
                caja, nro_ticket, ev_requerido, aa, mm, dd);*/
				//" WHERE caja = %d AND nro_ticket = %ld AND tipo_evento = %d ",
				" caja = %d AND nro_ticket = %ld AND tipo_evento = %d ",
                caja, nro_ticket, ev_requerido );

            _where[ sizeof( _where ) - 1] = '\0';
        }
    }

    //Fin genreacion de sentencia WHERE
    GRABAR_LOG_DBROUTER (_where,LOG_DEBUG,2);
    if( OBTENER_TABLA_SERVIDOR( T_EV_TMP, TT_ORIG, "eventos", _where, SI, NULL ) ) {
        error = 1;
    }
    if (!error
        //&& (USE_DATABASE (AREA_AUX3, EV_TMP, (char *) &eventos,
        && (OPEN_TABLE( T_EV_TMP, TT_ORIG, (char *) &eventos,
                            sizeof (struct _eventos) ) == 0 ) ) {
        if( RECCOUNT2(NO, NULL) == 0 ) {
            //La tabla eventos se obtuvo correctamente, pero no existe el evento
            historico = 1;
            //CLOSE_DATABASE (AREA_AUX3);
			CLOSE_TABLE( T_EV_TMP, TT_ORIG );
            if (OBTENER_TABLA_SERVIDOR( T_EV_TMP, TT_ORIG, "eventosh", _where, SI, NULL)) {
                error = 1;
            }
            if (!error) {
                //if (USE_DATABASE( AREA_AUX3, EV_TMP, (char *) &eventos, sizeof (struct _eventos),
				//						NULL, 0, 0) != 0) {
				if( OPEN_TABLE( T_EV_TMP, TT_ORIG, (char *) &eventos, sizeof (struct _eventos) ) != 0 ) {
                    error = 1;
                }
            }
        }
        if (!error) {
			if ( RECCOUNT2(NO, NULL) > 0 ) {
                SET_WHERE("");
	             RUN_QUERY(NO);
                caja_z = eventos.caja_z; //no se usa
                id_evento = eventos.id_evento; // no se usa
                _snprintf( _where, sizeof( _where ),
                            //" WHERE caja_z = %li AND id_evento = %li ORDER BY posicion",
							//" caja_z = %li AND id_evento = %li ORDER BY posicion",
							//Tiene que ir el "ORDER BY posicion" pero da error en el count(), CORREGIR!!!
							" caja_z = %li AND id_evento = %li",
							caja_z, id_evento);
                 _where[sizeof( _where ) - 1] = '\0';
				if( OBTENER_TABLA_SERVIDOR( T_EVC_TMP, TT_ORIG, (historico) ? "ev_conth" : "ev_cont",
										_where, SI, NULL ) ) {
                    error = 1;
                }
                if (!error
                    && OBTENER_TABLA_SERVIDOR( T_EVM_TMP, TT_ORIG,
							(historico) ? "ev_mediosh" : "ev_medios", _where, SI, NULL ) ) {
                    error = 1;
                }
#ifdef COMPILAR_EV_DESPRO
                if (!error
                    //&& !DBROUTER_OBTENER_TABLA (EVD_TMP, (historico) ? "ev_desproh" : "ev_despro", _where, SI)) {
                    && OBTENER_TABLA_SERVIDOR( T_EVD_TMP, TT_ORIG,
									(historico) ? "ev_desproh" : "ev_despro", _where, SI, NULL ) ) {
                    error = 1;
                }
#endif
                if (!error
                    && OBTENER_TABLA_SERVIDOR( T_EVP_TMP, TT_ORIG,
									(historico) ? "ev_dmediosh" : "ev_dmedios", _where, SI, NULL ) ) {
                    error = 1;
                }
                if (!error
                    && OBTENER_TABLA_SERVIDOR( T_EVCB_TMP, TT_ORIG,
									(historico) ? "ev_cod_barrah" : "ev_cod_barra", _where, SI, NULL ) ) {
                    error = 1;
                }
                if (!error
                    && OBTENER_TABLA_SERVIDOR( T_EVBP_TMP, TT_ORIG,
									(historico) ? "ev_bpromoh" : "ev_bpromo", _where, SI, NULL ) ) {
                    error = 1;
                }
                if (!error && ( MODO_DEVOLUCION || RAM_CAMBIO ) && origen == _ORIGEN_DEVOLUCION) {
                    /*if (eventos.tipo_evento == 35) {
                        SET_MEMORY_CHAR (__ram_ticket_factura, 1);
                    }
                    if (!RAM_NRO_CLIENTE) { //Si no hay un cliente previamente seleccionado
                        SET_MEMORY_LONG (__ram_nro_cliente,
                                            eventos.nro_cliente);
                    }
                    else {
                        if (eventos.tipo_evento == 35) {
                            MENSAJE (ST
                                        (S_TF_CLIENTE_DESEA_CAMBIARLO_S_N));
                        }
                        aux = TECLADO_ALFABETICO;
                        TECLADO_ALFABETICO = 1;
                        do {
                            car = GETCH ();
                        }
                        while (car != 'S' && car != 'N' && car != 's'
                                && car != 'n');
                        if (car == 'N' || car == 'n') {
                            SET_MEMORY_LONG (__ram_nro_cliente,
                                                eventos.nro_cliente);
                        }
                        TECLADO_ALFABETICO = aux;
                    }*/
                }
                if(  RAM_COD_FUNCION != 126  ) {//SE DEBE MEJORAR LA CONDICION
					if(!EXISTE_CLIENTE( eventos.nro_cliente ) && eventos.nro_cliente != 0){
						__cliente = 1;
						MENSAJE_CON_PAUSA(ST(S_NO_SE_ENCUENTRA_EL_CLIENTE),25);
					}
					else{
						__cliente = 0;
						CAMBIO_CLIENTE( eventos.tipo_evento, eventos.nro_cliente, eventos.cod_financiacion, origen );
					}
                }
                if (!RAM_REFERENCIA_NC && RAM_CAMBIO != 1 ) {
                    SET_MEMORY_INT (__ram_bultos, eventos.bultos);
                }
                //CLOSE_DATABASE (AREA_AUX3);
				CLOSE_TABLE( T_EV_TMP, TT_ORIG );
            }
            else {
                inexistente = 1;
                error = 1;
                //CLOSE_DATABASE (AREA_AUX3);
				CLOSE_TABLE( T_EV_TMP, TT_ORIG );
            }
        }
    }
    else {
        error = 2;
    }
	/*if(origen == _ORIGEN_TICKET_EN_ESPERA ){
		//controlo que a pesar de traerlo no sea ya importado, lee tablas local cuando es igual caja_z
		if( eventos.cajaz_importado == 0 || ( eventos.tipo_evento = 30 && eventos.cajaz_importado ) ) {
			char patron_nulls[50];
			//eventos.tipo_evento_importado=30;
			//eventos.id_evento_importado=NRO_EVENTO;
			eventos.cajaz_importado= ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
			memset( patron_nulls, 0, 50 );
			patron_nulls[18] = 1;
			DBROUTER_REQ_CON_NULL( ACTUALIZAR_REGISTRO, 60, patron_nulls, "eventos",
                                               ( char* )&eventos,
                                               sizeof( struct _eventos)
												, _where, 0 );
		}else{
			error = 1;
			inexistente = 1;
		}
	}*/
    if(inexistente) {
		_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_COMPROBANTE_NO_ENCONTRADO ), 
			ST( S____PRESIONE_UNA_TECLA__ ) );
    }
    else if(error) {
		_snprintf( cadena, sizeof(cadena)-1, "%s %s", ST( S_ERROR_EN_EVENTOS_TEMPORALES ), 
			ST( S____PRESIONE_UNA_TECLA__ ) );
    }
    else {
        BORRAR_ARCHIVO_REC (ARCH_REC_IMPORT);
		/*Guarda la estructura en un archivo */
        if( GUARDAR_ARCHIVO_REC (ARCH_REC_IMPORT, (char *) &eventos, 
				sizeof (struct _eventos)) == -1 ) {
			GRABAR_LOG_SISTEMA("OBTENER_EVENTOS_TEMPORALES_ONLINE: ERROR GRABANDO REC_IMPORT"
				, LOG_VENTAS,2 );
        }
    }
	if( inexistente || error ) {
		MENSAJE_TECLA( cadena );
	}

	return error;
}

/***************************************************************************/
int CAMBIO_CLIENTE( char tipo_evento_original, long nro_cliente_evento, int cod_financiacion, char origen )
/***************************************************************************/
{
    char ticket_factura = 0, pregunta = NO;
    int aux = 0, subtipo = 0, tipo_comprobante = 0, cli_habilitado = 0, clientess = 0,
		ok = 1;
    char car = 0;

	//Caso de Corte de luz en la validacion de Cambio de Cliente
	if( RAM_NRO_CLIENTE <= 0 ) {
		if( RAM_NRO_CLIENTE_ANT != -999 ) {
			SET_MEMORY_LONG ( __ram_nro_cliente, RAM_NRO_CLIENTE_ANT );
		} else {
			SET_MEMORY_LONG ( __ram_nro_cliente, 0 );
			return( 0 ) ;
		}
	}
    /* Si es Dev o NC, no se carga el cliente si no esta permitido
       devolver el comprobante original*/
    if( origen == _ORIGEN_DEVOLUCION ) {
        if( !VERIFICA_EVENTO_NC_DEV( tipo_evento_original ) ) {
            return(0);
        }
    }
	SET_MEMORY_LONG ( __ram_nro_cliente_ant, nro_cliente_evento );
    if( !RAM_NRO_CLIENTE || RAM_NRO_CLIENTE == nro_cliente_evento ) {
		SET_MEMORY_LONG ( __ram_nro_cliente, nro_cliente_evento );
		SET_MEMORY_INT( __nro_cod_cuota, cod_financiacion );
		if( EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
			CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
		}
    }
	else {  /*Se selecciona previamente al cliente */
		/*Verificar Condicion Iva del Cliente*/
		if( VALIDAR_CAMBIO_DE_CLIENTE( nro_cliente_evento, NO, origen, 0 ) ) {
			if( RAM_NRO_CLIENTE != nro_cliente_evento ) {
				if ( tipo_evento_original == 35  || tipo_evento_original == 36  ) {
					MENSAJE( "EL COMPROBANTE ORIGINAL YA POSEE UN CLIENTE. DESEA CAMBIARLO? S/N" );
					pregunta = SI;
				}
				else {
					if ( !nro_cliente_evento ) {
						MENSAJE( "DESEA UTILIZAR EL CLIENTE CARGADO? S/N" );
					}
					else {
						MENSAJE
							( "EL COMPROBANTE ORIGINAL YA POSEE UN CLIENTE. DESEA CAMBIARLO? S/N" );
					}
					pregunta = SI;

				}
			}
		} else {
			ok = 0;
		}

		if( pregunta ) {
            aux = TECLADO_ALFABETICO;
            TECLADO_ALFABETICO = 1;
            do {
                car = GETCH ();
            }
            while( car != 'S' && car != 'N' && car != 's' && car != 'n' );

            TECLADO_ALFABETICO = aux;
			if( car == 'N' || car == 'n' ) {
				SET_MEMORY_LONG ( __ram_nro_cliente, nro_cliente_evento );
				if( !RAM_NRO_CLIENTE ) {//No hay cliente cargado en el comprobante original
					SET_MEMORY_INT( __ram_perfil, 0 );
					//Utilizamos _ORIGEN_DEVOLUCION paRa identificar un Nota de Credito ya que 
					//aun no seteo la variable RAM_FACTURA o RAM_NOTA_CR
					if( RAM_FACTURA || RAM_TICKET_FACTURA 
							|| origen == _ORIGEN_DEVOLUCION ) {//Comprobante que si o si necesitan un cliente
						
						SET_MEMORY_INT( __nro_cod_cuota, 0 );
						clientess = VALIDAR_CAMBIO_DE_CLIENTE( nro_cliente_evento, SI, origen, 0 );
						
						if( RAM_NRO_CLIENTE && clientess != 2 && !NRO_COD_CUOTA ) {
							_ELEGIR_CLIENTE( );
						}
						SET_MEMORY_CHAR( __ram_cond_iva_cliente, clientes.cond_iva );
						if( !RAM_NRO_CLIENTE ) {//Si no cargo ningun cliente
							/*SET_MEMORY_CHAR(__ram_nota_cr        ,0);  
							SET_MEMORY_CHAR(__ram_referencia_nc  ,0);  
							SET_MEMORY_CHAR(__ram_factura        ,0);  
							SET_MEMORY_CHAR(__ram_ticket_factura ,0);  
							SET_MEMORY_LONG(__nro_remito         ,0);  
							SET_MEMORY_CHAR( __ram_cond_iva_cliente, 0 );*/
							MUESTRA_PERFIL();
							MUESTRA_COMPROBANTE( );
							ok = 0;
						} 
					} 
				} else {
					EXISTE_CLIENTE( RAM_NRO_CLIENTE );
					CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
				}
            }
		} 
    }
	
	return( ok );
}

/***************************************************************************/
int VALIDAR_CAMBIO_DE_CLIENTE( int nro_cliente_ant, int pedir_cliente, int origen, int operacion )
/***************************************************************************/
{
	long cliente_actual = 0;
	int ok = 1, clientess = 0, reintento = 0, validar = 1;
	int veces = 3;

	do {
		if( pedir_cliente ) {
			clientess = PEDIR_NRO_CLIENTE( 1, NO );
			reintento ++;
		}
		cliente_actual = RAM_NRO_CLIENTE;

		if( origen == _ORIGEN_TICKET_EN_ESPERA ) {
			if( nro_cliente_ant <= 0 ) {
				validar = 0;
			}
		}

	 	if( cliente_actual && nro_cliente_ant != cliente_actual ) {
			if( validar && !VALIDAR_CAMBIO_DE_CLIENTE_PARA_COMPROBANTE(
						( nro_cliente_ant ) ? nro_cliente_ant : -1, operacion ) ) {
				if( reintento != veces ) {
					SET_MEMORY_LONG ( __ram_nro_cliente_ant, RAM_NRO_CLIENTE );
				} else {
					//Control para corte de luz
					SET_MEMORY_LONG ( __ram_nro_cliente_ant, -999 );
				}
				SET_MEMORY_LONG( __ram_nro_cliente, 0 );
				pedir_cliente = SI;
			} else {
				reintento = veces;
			}
		} else {
			reintento = veces;	
		}

	} while( reintento < veces  &&  LASTKEY()!= 27 );

	if( !RAM_NRO_CLIENTE ) {//Si no cargo ningun cliente
		//Seteo la variable ram_nro_cliente_ant en caso de corte de luz para recuperar 
		//El valor del ram_nro_cliente
		SET_MEMORY_LONG( __ram_nro_cliente_ant, -999 );
		if( LASTKEY()!= 27 )
			MENSAJE_TECLA( ST( S_NO_SELECCIONO_CLIENTE_CONDICION_FISCAL_CORRECTA_Pres_Tecla ) );
		SET_MEMORY_CHAR(__ram_nota_cr        ,0);  
		SET_MEMORY_CHAR(__ram_referencia_nc  ,0);  
		SET_MEMORY_CHAR(__ram_factura        ,0);  
		SET_MEMORY_CHAR(__ram_ticket_factura ,0);  
		SET_MEMORY_LONG(__nro_remito         ,0);  
		SET_MEMORY_CHAR( __ram_cond_iva_cliente, 0 );
		ok = 0;
	} else {
		//Si se selecciono el cliente con exito, seteo el ram_nro_cliente_ant 
		//con el cliente anterior
		SET_MEMORY_LONG ( __ram_nro_cliente_ant, nro_cliente_ant );
	}

	if( clientess == 2 && ok ) { 
		ok = 2;//No pide Cliente "PEDIR_NRO_CLIENTE" porque ya tenia cargado uno
	}

	return ( ok );
}
/***************************************************************************/
int ABRIR_EVENTOS_TEMPORALES( struct _eventos *eventos, struct _ev_cont *ev_cont,
                              struct _ev_medios *ev_pago, struct _ev_despro *ev_despro,
                              struct _ev_dmedios *ev_dpago, struct _ev_cod_barra *ev_cod_barra,
                              struct _ev_bpromo *ev_b_promo )
/***************************************************************************/
{
    int error = 0;

	//if( USED( AREA_AUX3 ) || USED( AREA_AUX ) || USED( AREA_AUX5 ) ) {
       //GRABAR_LOG_SISTEMA( "Areas en uso abriendo temporales" );
	//}

	if( OPENED( T_EVC_TMP, TT_ORIG ) || OPENED( T_EV_TMP, TT_ORIG ) || OPENED(T_EVM_TMP, TT_ORIG) ) //aunque no lo parezca es necesario esto
	    CERRAR_EVENTOS_TEMPORALES();

    /*error = USE_DATABASE( AREA_AUX3, EV_TMP, ( char* )eventos, sizeof( struct _eventos ),
                          NULL, 0, 0 );
	if( error == 0 ) {
	   char buff[100];
	   sprintf(buff,"EVENTOS:REGISTROS(%i)",RECCOUNT(  ));
	   GRABAR_LOG_SISTEMA(buff);
	}*/
    error = OPEN_TABLE( T_EV_TMP, TT_ORIG, ( char* )eventos, sizeof( struct _eventos ) );
    /*error += USE_DATABASE( AREA_AUX, EVC_TMP, ( char* )ev_cont,
                           sizeof( struct _ev_cont ), NULL, 0, 0 );
	if( error == 0 ) {
	   char buff[100];
	   sprintf(buff,"EVCONT:REGISTROS(%i)",RECCOUNT(  ));
	   GRABAR_LOG_SISTEMA(buff);
	}*/
    error += OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )ev_cont, sizeof( struct _ev_cont ) );
    /*error += USE_DATABASE( AREA_AUX5, EVM_TMP, ( char* )ev_pago, sizeof( struct _ev_medios ),
                           NULL, 0, 0 );
	if( error == 0 ) {
	   char buff[100];
	   sprintf(buff,"EVMEDIOS:REGISTROS(%i)",RECCOUNT(  ));
	   GRABAR_LOG_SISTEMA(buff);
	}*/
    error += OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )ev_pago, sizeof( struct _ev_medios ) );
    #ifdef COMPILAR_EV_DESPRO
    /*error += USE_DATABASE( AREA_AUX7, EVD_TMP, ( char* )ev_despro,
                           sizeof( struct _ev_despro ), NULL, 0, 0 );*/
    error += OPEN_TABLE( T_EVD_TMP, TT_ORIG, ( char* )ev_despro, sizeof( struct _ev_despro ) );
    #endif
    /*error += USE_DATABASE( AREA_AUX8, EVP_TMP, ( char* )ev_dpago,
                           sizeof( struct _ev_dmedios ), NULL, 0, 0 );
	if( error == 0 ) {
	   char buff[100];
	   sprintf(buff,"EVPAGO:REGISTROS(%i)",RECCOUNT(  ));
	   GRABAR_LOG_SISTEMA(buff);
	}*/
    error += OPEN_TABLE( T_EVP_TMP, TT_ORIG, ( char* )ev_dpago, sizeof( struct _ev_dmedios ) );
 	/*error += USE_DATABASE( AREA_AUX9, EVCB_TMP, ( char* )ev_cod_barra,
                           sizeof( struct _event_cod_barra ), NULL, 0, 0 );
	if( error == 0 ) {
	   char buff[100];
	   sprintf(buff,"EVCODBAR:REGISTROS(%i)",RECCOUNT(  ));
	   GRABAR_LOG_SISTEMA(buff);
	}*/
    error += OPEN_TABLE( T_EVCB_TMP, TT_ORIG, ( char* )ev_cod_barra, sizeof( struct _ev_cod_barra ) );
  	/*error += USE_DATABASE( AREA_AUX10, EVBP_TMP, ( char* )ev_b_promo,
                           sizeof( struct _ev_bpromo), NULL, 0, 0 );
	if( error == 0 ) {
	   char buff[100];
	   sprintf(buff,"EVBPROMO:REGISTROS(%i)",RECCOUNT(  ));
	   GRABAR_LOG_SISTEMA(buff);
	}*/
    error += OPEN_TABLE( T_EVBP_TMP, TT_ORIG, (char*)ev_b_promo, sizeof( struct _ev_bpromo));
	if( !error ) {
        SELECT_TABLE( T_EV_TMP, TT_ORIG );
        if( !RECCOUNT2( NULL, NULL ) ) {
            MENSAJE_STRING( S_LA_TRANSACCION_ESPECIFICADA_NO_EXISTE );
            error = 1;
        }
        if( OPENED( T_EVC_TMP, TT_ORIG ) ) {
        SELECT_TABLE( T_EVC_TMP, TT_ORIG );
        SET_ORDER2( "CAJA_Z, ID_EVENTO" );
         SET_WHERE("");
	      RUN_QUERY(NO);
        GO2( TOP );
         }
    }
    return !error;
}
/***************************************************************************/
void CERRAR_EVENTOS_TEMPORALES()
/***************************************************************************/
{
    CLOSE_TABLE( T_EV_TMP, TT_ORIG );
    CLOSE_TABLE( T_EVC_TMP, TT_ORIG );
    CLOSE_TABLE( T_EVM_TMP, TT_ORIG );
    CLOSE_TABLE( T_EVD_TMP, TT_ORIG );
    CLOSE_TABLE( T_EVP_TMP, TT_ORIG );
    CLOSE_TABLE( T_EVCB_TMP, TT_ORIG );
    CLOSE_TABLE( T_EVBP_TMP, TT_ORIG );
    CLOSE_TABLE( T_EVCI_TMP, TT_ORIG ); //AREA_CAJ_FLOTANTE
}

/***************************************************************************/
int REIMPRIMIR_TICKET_LOCAL(char *archivo)
/***************************************************************************/
{
	int handle, posicion = 0;
	char bufferaso[10];
	char textOut[15000], text[50];
	int index = 0;
	int salir = 0, error = 0;
	int i=0, cant = 0;
	int bandera = 0;
	int intentos = 0;

	CLOSE_COPIA_TICKET_PRN(  );
	CLOSE_COPIA_VOUCHER_PRN(  ); //MMM ESTO PUEDE CAUSAR PROBLEMAS
    CLOSE_COPIA_Z_PRN();
	handle = _OPEN( archivo, _O_BINARY/*O_RDONLY*/, __LINE__, __FILE__ );
	memset(textOut, 0 ,15000);
	//sprintf( textOut,"!|2CReimpresion de\ncomprobante\n\n  ");
    sprintf( textOut,"!|2CReimpresión de}Comprobante}} "); //'}' Es salto de linea para javaServer
	index = 33;
	while( !salir ) {
		if( index < 14999 && !( error = _READ( handle, ( char* )bufferaso, 1, __LINE__, __FILE__ ) ) ) {
			textOut[index++] = bufferaso[0];
			if( textOut[index-1] == '<' || textOut[index-1] == '>' ) {
				/*Este caracter significa centrar, el javaserver es el encargado de interpretar este
				caracter, pero para reimpresion lo hace mal y genera que se clave el javaserver y
				este la caja*/
				textOut[index-1] = ' ';
			}
			if( ( textOut[index-4] == '!' && textOut[index-3] == '|' && textOut[index-2] == '1' && textOut[index-1] == 'X' ) ){
				index -=  4;
				textOut[index] = 0;
			}
			intentos = 0;
			if( ( textOut[index-6] == '!' && textOut[index-5] == '|' && textOut[index-4] == '8'
					&& textOut[index-3] == '0' && textOut[index-2] == 'f' && textOut[index-1] == 'P' ) ) {
				strcpy( text,"!|2CReimpresión de}Comprobante}} ");
				sprintf( textOut + index, text);
				posicion = index;
				cant = strlen( text );
				index += cant;
			}
		}
		else if( intentos == 0 ) {
			intentos++;
			LSEEK( handle, 0, index + 2, __LINE__, __FILE__ );
		} else {
			salir = 1;
			/*Se elimina el ultimo msj "Reimpresion de Comprobante"*/
			if( cant ) {
				for( i = 0; i < cant; i++ ) {
					textOut[posicion + i] = 0;
				}
				index -= cant;
			}
		}
	}
	sprintf( textOut+index,"!|1X");
	//!( error && index == 33 )--> Si no hay nada para reimprimir no imprimir el titulo Reimpresion de
	//Comprobante.
	if( index > 0 && !( error && index == 33 ) ) {
		PRINT( 0, 100,textOut);
		setData( ACTION_FORCE_PRINT, NULL, 0 );
		setData( ACTION_PRINT, NULL, 0 );
	}
	if(!error){
		GRABAR_CABECERA_EVENTO_REIMPRESION( 0 );
	}

	if( error && index == 33 ) {
		char msj[100];
		char reimp[15];
		strncpy( reimp, archivo, sizeof(reimp)-1 );
		_snprintf( msj, sizeof(msj)-1,"La Reimpresión es solo para %s", strupr( strtok (reimp,".") ) );
		MENSAJE( msj );
		BORRAR_MENSAJE();
	}
	_CLOSE( handle, __LINE__, __FILE__ );
	return((error)?NO:SI);
}


/***************************************************************************/
void REIMPRIMIR_TICKET_REMOTO()
/***************************************************************************/
{
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;
    int _ptr,_handle,_estado;
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    if( _ESTADO == 0 ) {
        //Obtengo el ticket en archivos temporales...
        if( IMPORTAR_TICKET( SI, 0, SI, ST( S_REIMPRESION ), SI, 0, _NO_APLICA, SI, NO ) ) {
            _SET_ESTADO( 1 );
        }
    }
    if( _ESTADO == 1 ) {
        //Ahora se debe cargar la estructura de item para poder imprimir el articulo
        SET_MEMORY_LONG( __ram_cliente_afinidad, RAM_ID_EVENTO );
        SET_MEMORY_LONG( __ram_boleta_anterior, NRO_EVENTO );
        if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                      &ev_cod_barra, &ev_b_promo ) ) {
            SELECT_TABLE( T_EV_TMP, TT_ORIG );
			SET_ORDER2("CAJA_Z, ID_EVENTO");
			RUN_QUERY(NO);

			SET_MEMORY_LONG( __ram_id_evento, eventos.id_evento - 1 );
            SET_MEMORY_LONG( __nro_evento, eventos.nro_ticket );
            SET_MEMORY_INT( __ram_caja_anterior, eventos.caja );
            SET_MEMORY_INT( __ram_sucursal_anterior, eventos.fecha );
            IMPRIMIR_TKT_CABECERA( 1 );
            SELECT_TABLE( T_EVC_TMP, TT_ORIG );      //e_ticket temporal
            while( !dbEOF() ) {
                //Recorro los items
                memcpy( &event_e_ticket, &ev_cont, sizeof( struct _ev_cont ) );
                CARGA_ITEM( 0, NULL );
                IMPRIME_ITEM( RAM_P_TICKET );
                AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, event_e_ticket.tipo & _RESTA,
                                       event_e_ticket.tipo & _ENGANCHADO, NO,
                                       event_e_ticket.origen );
                ADD_MEMORY_INT( __ram_p_ticket, 1 );
                SELECT_TABLE( T_EVC_TMP, TT_ORIG );
                SKIP2( 1 );
            }
            //Cargo los pagos...
            SELECT_TABLE( T_EVM_TMP, TT_ORIG );
            SET_MEMORY_CHAR( __ram_p_pago, 0 );
            SET_MEMORY_DOUBLE( __ram_total, 0 );
            while( !dbEOF() ) {
                _SET_MEMORY_CHAR( __pago_modo, RAM_P_PAGO, ev_pago.modo_pago - 1 );
                _SET_MEMORY_DOUBLE( __pago_vuelto, RAM_P_PAGO, ev_pago.vuelto_propio );
                if( medios[ev_pago.modo_pago - 1].tarjeta ) {
                    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, ev_pago.tarjeta );
                }
                if( medios[ev_pago.modo_pago - 1].efectivo ) {
                    SET_MEMORY_DOUBLE( __ram_vuelto, ev_pago.vuelto_efectivo );
                }
                if( medios[ev_pago.modo_pago - 1].mutual
                 || medios[ev_pago.modo_pago - 1].ticket_compra ) {
                    _SET_MEMORY_INT( __pago_cuenta, RAM_P_PAGO, ev_pago.numero_id );
                }
                _SET_MEMORY_DOUBLE( __pago_importe, RAM_P_PAGO, ev_pago.importe );
                ADD_MEMORY_CHAR( __ram_p_pago, 1 );
                SELECT_TABLE( T_EVM_TMP, TT_ORIG );
                SKIP2( 1 );
            }
            SET_MEMORY( __ram_string_cliente_afinidad, eventos.str_cliente_afinidad );
            SET_MEMORY_DOUBLE( __ram_total,
								eventos.importe_sin_iva + eventos.iva1 + eventos.iva2
                             + eventos.exento );
            SET_MEMORY_INT( __ram_bultos, eventos.bultos );
            CERRAR_EVENTOS_TEMPORALES();    //Cierro los temporales...
            if( TKT_CONFIGURABLE ) {
				IMPRIME_TKT_IMPORTE_TOTAL( 1 );
			}
			PAGOS_IMPRIME_PAGO( _MODO_VENTA );
			if( RAM_ANULAR_TICKET > 100 )
				FIN_DE_TICKET( NO );

            IMPRIME_PIE();
        }
        _SET_ESTADO( 2 );
    }
    if( _ESTADO == 2 ) {
        SET_MEMORY_LONG( __ram_id_evento, RAM_CLIENTE_AFINIDAD );
        SET_MEMORY_LONG( __nro_evento, RAM_BOLETA_ANTERIOR );
        FIN_DE_TICKET( SI );
        _SET_ESTADO( 3 );
    }
    CLOSE_RECUPERO( _ptr, _handle );
}

/***************************************************************************/
void IMPORTAR_DESCUENTOS( int remoto, long id, struct _ev_despro *ev_despro )
/***************************************************************************/
{
    #ifdef COMPILAR_EV_DESPRO
    int seguir = 1, renglon = 0;
    if( remoto ) {
        SELECT_TABLE( T_EVD_TMP, TT_ORIG );
        GO2( TOP );
    }
    else {
        //SELECT( AREA_E_DES_PRO );
        SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
        //SET_ORDER( 1 );
        //GO( TOP );
        //GET_EQUAL( ( char* )&id );
        SET_WHERE( "ID_EVENTO = '%ld'", id );
        SET_ORDER2( " ID_EVENTO, POSICION, TIPO_MOTIVO, ID_MOTIVO" );
        RUN_QUERY( NO );
        if( id != event_despro->id_evento ) {
            seguir = 0;
        }
    }
    while( seguir && /*!BtrvEOF()*/ !dbEOF ) {
        memcpy( event_despro, ev_despro, sizeof( struct _ev_despro ) );
        if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
            event_despro->importe = -event_despro->importe;
        }
        event_despro->id_evento = RAM_ID_EVENTO;
        event_despro->cod_sucursal = config.sucursal;
        event_despro->fecha_ticket = ( NRO_FECHA_OPERACION ) ? NRO_FECHA_OPERACION : _GET_FECHA_DOS();//!!!
        event_despro->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
        event_despro->caja = NRO_CAJA;
        event_despro->nro_z = NRO_Z;
        //SELECT( AREA_E_DES_PRO );
        SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
        #ifdef INVEL_L
        //BEGIN_TRANSACTION();
        #endif
        INSERT2( NO );
        #ifdef INVEL_L
        //END_TRANSACTION();
        #endif
        if( remoto ) {
            SELECT_TABLE( T_EVD_TMP, TT_ORIG );
            SKIP2( 1 );
        }
        else {
            //SELECT( AREA_E_DES_PRO );
            SELECT_TABLE( T_EV_DESPRO, tipo_t_ev_despro );
            renglon++;
            //GET_EQUAL( ( char* )&id );
            //SKIP( renglon );
            SET_WHERE( "ID_EVENTO = '%ld'", id );
            SET_ORDER2( " ID_EVENTO, POSICION, TIPO_MOTIVO, ID_MOTIVO" );
            RUN_QUERY( NO );
            SKIP2( renglon );
            if( id != event_despro->id_evento ) {
                seguir = 0;
            }
        }
    }
    #endif
}

/*****************************************************************************/
int CONTAR_TICKET_ESPERA(int ev_requerido, int caja, long nro_z, long secuencia,
						 long nro_ticket, long fecha )
/*****************************************************************************/
{
    char _where[100];
    struct _eventos eventos;
    int error = 0;
    int cant = 0;
    char log[190];
	int dd, mm, aa;


#ifdef FECHA_ORACLE_SQL	
		if(GetConfIniclienteDbServer(ODBC_SERVIDOR)==4){
		_snprintf( cadena, sizeof( cadena )," %02ld%02ld%04ld ",dd, mm, aa);
	}else{
		/*sql */
		_snprintf( cadena, sizeof( cadena )," %04ld%02ld%02ld",aa, mm, dd);
	}

#endif	

	FECHA__ (fecha, &dd, &mm, &aa);
    if( ev_requerido ) {
		//LA TM220 NO IMPRIME SECUENCIA EN UN TICKET ESPERA
		if( nro_ticket > 0 && fecha > 0 ) {
			_snprintf( _where, sizeof( _where ), 
				" caja = %ld AND nro_ticket = %ld AND fecha_ticket = '%04ld%02ld%02ld'",
				caja, nro_ticket, aa, mm, dd );
		} else {
			if( secuencia <= 0 ) {
				//sprintf( where, " WHERE caja = %d AND tipo_evento = %d AND nro_z = %ld",
				_snprintf( _where, sizeof( _where )- 1," caja = %d AND tipo_evento = %d AND nro_z = %ld"
					,caja, ev_requerido, nro_z );
				/*sprintf( where, " caja = %d AND tipo_evento = %d AND nro_z = %ld",
				caja, ev_requerido, nro_z );*/
			} else {
				//sprintf( where, " WHERE caja = %d AND tipo_evento = %d AND nro_z = %ld AND nro_secuencia = %ld",
				if( caja == NRO_CAJA )
					_snprintf( _where, sizeof( _where )- 1," caja = %d AND tipo_evento = %d AND nro_z = %ld AND nro_secuencia = %ld"
						, caja, ev_requerido, nro_z, secuencia);

					/*sprintf( where, " caja = %d AND tipo_evento = %d AND nro_z = %ld AND nro_secuencia = %ld",
						caja, ev_requerido, nro_z, secuencia );*/
				else
					_snprintf( _where, sizeof( _where )- 1," caja = %d AND tipo_evento = %d AND nro_secuencia = %ld"
						,caja, ev_requerido, secuencia );

					/*sprintf( where, " caja = %d AND tipo_evento = %d AND nro_secuencia = %ld",
							caja, ev_requerido, secuencia );*/
				}
		}
	} else {
		_where[0] = 0;
	}
    GRABAR_LOG_DBROUTER( _where ,LOG_DEBUG,2);
    /*obtiene la vista de los ticket en espera que tiene la caja*/
    // Se utiliza EV_TMP2 para q no se sobreescriban los eventos temporales en caso
    // de una caida de la caja en medio de una importacion.
    if( OBTENER_TABLA_SERVIDOR( T_EV_TMP, TT_TEMP, "V_Ticket_Espera", _where, SI, "eventos" ) ) {
		error = 1;
    }
	if( !error && OPENED( T_EV_TMP, TT_TEMP ) ) 
		CLOSE_TABLE( T_EV_TMP, TT_TEMP );
	
    if( !error
		&& ( OPEN_TABLE( T_EV_TMP, TT_TEMP, ( char* )&eventos, sizeof( struct _eventos ) ) == 0 ) ) {
		SET_WHERE("");
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
		cant = RECCOUNT2( NULL, NULL );
		while( cant && !dbEOF() ) {
			/*Verificar si existen eventos locales que no fueron enviados a la base del Servidor (status 0)
			y que procesaron un evento (tipo_evento_importado != 0) remoto.*/
			SELECT_TABLE(T_EVENTOS, TT_ORIG);
			SET_ORDER2("CAJAZ_IMPORTADO, ID_EVENTO_IMPORTADO");
			SET_WHERE("CAJAZ_IMPORTADO ='%ld' AND ID_EVENTO_IMPORTADO ='%ld' AND TIPO_EVENT_IMPORTADO = 30 AND STATUS = 0",
					eventos.caja_z, eventos.id_evento, eventos.tipo_evento );
			RUN_QUERY(NO);
			if( FOUND2() ) {
				cant --;
			}
			SKIP2( 1 );
		}
    }
    if( OPENED( T_EV_TMP, TT_TEMP ) ) 
		CLOSE_TABLE( T_EV_TMP, TT_TEMP );

    _snprintf( log, sizeof( log )- 1,"1CONTAR_TICKET_ESPERA (REMOTO): (%s) cant: %d", _where, cant );
    GRABAR_LOG_SISTEMA( log , LOG_DEBUG,5);

    return( cant );
}

/*****************************************************************************/
int CONTAR_TICKET_ESPERA_SIN_PROCESAR(int ev_requerido, int caja, long nro_z, long secuencia )
/*****************************************************************************/
{
    char where[300];
    struct _eventos eventos;
    int error = 0;
    int cant = 0;
    char log[190];

  	if( ev_requerido ) {
		_snprintf( where, sizeof( where )- 1,
			" CAJA = %d and TIPO_EVENTO = %d  and nro_z = %ld ",caja, ev_requerido, nro_z );
			/*
			CAJA = %d and --> caja que pregunta
			TIPO_EVENTO = 30 and  -->eventos en espera 
			caja,ev_requerido );
			*/

	} else {
		where[0] = 0;
	}

    if( OBTENER_TABLA_SERVIDOR( T_EV_TMP, TT_TEMP, "V_Ticket_Espera", where, SI, "eventos" ) ) {
		error = 1;
    }
	if( !error && OPENED( T_EV_TMP, TT_TEMP ) ) 
		CLOSE_TABLE( T_EV_TMP, TT_TEMP );
	
    if( !error
		&& ( OPEN_TABLE( T_EV_TMP, TT_TEMP, ( char* )&eventos, sizeof( struct _eventos ) ) == 0 ) ) {
		_snprintf( where, sizeof( where )- 1,"ID_EVENTO NOT IN(SELECT ID_EVENTO_IMPORTADO FROM EVENTOS WHERE CAJAZ_IMPORTADO = %d and TIPO_EVENT_IMPORTADO = %d  )",( ( long )( caja )*100000L + ( nro_z % 100000L )), ev_requerido);
			
		SET_WHERE(where);
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
		cant = RECCOUNT2( NULL, NULL );
    }
    if( OPENED( T_EV_TMP, TT_TEMP ) ) 
		CLOSE_TABLE( T_EV_TMP, TT_TEMP );

    _snprintf( log, sizeof( log )- 1,"2CONTAR_TICKET_ESPERA (REMOTO): (%s) cant: %d", where, cant );
    GRABAR_LOG_SISTEMA( log , LOG_DEBUG,5);

    return( cant );
}
/*******************************************************************3*******************/
int LOCAL_OBTENER_TABLA( int nombre_tabla_local, long caja, long secuencia, long evento,
						int ev_requerido, int origen, long nro_ticket, int fecha, long *caja_z )
/**************************************************************************************/
{
	struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_dmedios ev_dpago;
    //struct _ev_despro ev_despro;
    //struct _event_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;
	int rta = 0, skip = 0, area_ant = 0, verif_ev_req = 1, fin = 1;
    char log[150];
	//int tabla_ant, tipo_ant;

	//area_ant = SELECTED();
/*	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();*/
	
	//SI IMPORTO DE DIAS ANTERIORES.... CON NROz DISTINTA
	

	switch( nombre_tabla_local ) {
			case /*_TICKET_BTR*/ T_EVENTOS:
				{
					/*struct
                    {
	                    long caja_z;
						long id_evento;
					} ind3;
                    struct
					{
						long caja_z;
						long secuencia;
					} ind3_all;
					struct
					{
						int fecha;
						int caja;
						long nro_ticket;
					} ind2_all;*/

					//SELECT( AREA_TICKET );
					/*ind3_all.caja_z = caja_z;
					ind3_all.secuencia = secuencia;*/
					SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
                    //si se pasa el id_evento, es la forma de busqueda preferida
                    if ( evento ) {
                        //SET_ORDER( 4 );
    				    //GET_EQUAL( ( char* )&ind3 );
						if( *caja_z > 0 )
							SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld'", *caja_z, evento);
						else
							SET_WHERE("CAJA='%li' AND ID_EVENTO='%ld' AND FECHA='%ld' ", caja, evento, fecha);	
                        RUN_QUERY(NO);

                    }
					else if( ( PEDIR_CAJA_Y_SECUENCIA || REFERENCIAR_POR_SECUENCIA )
						&& secuencia ){
						if( *caja_z > 0 )
							SET_WHERE("CAJA_Z='%ld' AND NRO_SECUENCIA='%ld'", *caja_z, secuencia);
						else
							SET_WHERE("CAJA='%li' AND NRO_SECUENCIA='%ld' AND FECHA='%ld' ", caja, secuencia, fecha);	

					    
				       RUN_QUERY(NO);
                   }
					else {
						//SET_ORDER( 3 );
						GO2( TOP );
						switch(ev_requerido ){
							case _EVENT_FAC_A             :// 3 
							case _EVENT_FAC_B             :// 4 
							{	
								if( *caja_z > 0 )
									SET_WHERE("CAJA_Z='%ld' AND NRO_COMPROBANTE='%ld' AND FECHA='%ld'", *caja_z, nro_ticket,fecha);		
								else
									SET_WHERE("CAJA ='%ld' AND NRO_COMPROBANTE='%ld' AND FECHA='%ld'", caja, nro_ticket,fecha);		
							break;
							}
						default:{
							if( *caja_z > 0 )
								SET_WHERE("CAJA_Z='%ld' AND NRO_TICKET='%ld' AND FECHA='%ld'", *caja_z, nro_ticket,fecha);		
							else
								SET_WHERE("CAJA='%ld' AND NRO_TICKET='%ld' AND FECHA='%ld'", caja, nro_ticket,fecha);		
								}
						}
						
				      RUN_QUERY(NO);

					}
					if( FOUND2( ) || skip ) {
					   if( secuencia == 0  ) {
						if( event_ticket.tipo_evento != ev_requerido ) {
							verif_ev_req = 0;
							rta = 0;
						}
      				 }
						//No esta chequeando si el ticket en espera fue procesado a nivel local y tampoco el remoto
				   
                   if( rta && origen == _ORIGEN_TICKET_EN_ESPERA ) {
							if( VERIFICAR_TICKET_ESPERA_PROCESADO( event_ticket.caja_z, event_ticket.id_evento ) ) {
								/*El Ticket en Espera ya se ha recuperado*/
								MENSAJE("EL TICKET YA FUE PROCESADO");
								verif_ev_req = 0;
								rta = 0;
							} else {
								/*Nos posicionamos en el evento requerido*/
							    if ( evento ) {
                                    //SET_ORDER( 4 );
							        //GET_EQUAL( ( char* )&ind3 );
								    SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld'", *caja_z, evento );
							   	    RUN_QUERY(NO);
                                }
                                else {
								    //SET_ORDER( 6 );
								    //GET_EQUAL( ( char* )&ind3_all );
								    SET_WHERE("CAJA_Z='%ld' AND NRO_SECUENCIA='%ld'", *caja_z, secuencia);
							   	    RUN_QUERY(NO);
                                }
							}
						}
					} else {
						rta = 0;
						verif_ev_req = 0;
					}
					if( FOUND2( ) && *caja_z == 0)
					   *caja_z = event_ticket.caja_z ; 

					if( verif_ev_req ) {
						rta = 1;

						//if( USE_DATABASE( AREA_AUX3, EV_TMP, ( char* )&eventos, sizeof( struct _eventos ), NULL, 0, 0 ) == 0 ) {
						memcpy( &eventos, &event_ticket , sizeof( struct _eventos ) );
						if( OPEN_TABLE( T_EV_TMP, TT_ORIG, ( char* )&eventos,
												sizeof( struct _eventos ) ) == 0 ) {
							memcpy( &eventos, &event_ticket , sizeof( struct _eventos ) );
							#ifdef INVEL_L
							//BEGIN_TRANSACTION( );
							#endif
                            if( INSERT2(NO) ) {
                                GRABAR_LOG_SISTEMA("LOCAL_OBTENER_TABLA: NO SE PUDO INSERTAR EN EV_TMP", LOG_VENTAS,2  );
                            } else {
                                _snprintf(log, sizeof(log), "LOCAL_OBTENER_TABLA: Insertado en EV_TMP: id_evento %ld sec %ld caja %d z %ld",
                                    eventos.id_evento, eventos.nro_evento, eventos.caja, eventos.nro_z );
                                GRABAR_LOG_SISTEMA(log, LOG_VENTAS,4 );
                            }
							#ifdef INVEL_L
							//END_TRANSACTION( );
							#endif
						} else {
							rta = 0;
							GRABAR_LOG_SISTEMA( "ERROR AL ABRIR EV_TMP" , LOG_VENTAS,2 );
						}
					}
				}
				break;

			case /*_E_TICKET_BTR*/ T_EV_CONT:
				{
					struct
					{
						long caja__z;
						long id_evento;
					} ind3_all;

					//if( USE_DATABASE( AREA_AUX, EVC_TMP, ( char* )&ev_cont, sizeof( struct _ev_cont ),	NULL, 0, 0 ) == 0 ) {
					if( OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )&ev_cont, sizeof( struct _ev_cont ) ) == 0 ) {
						//SELECT( AREA_E_TICKET );
						SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
						ind3_all.caja__z = *caja_z;
						ind3_all.id_evento = evento;
						//SET_ORDER( 2 );
                        //GO( TOP );
						//GET_EQUAL( ( char* )&ind3_all );
							SET_ORDER2("CAJA_Z, ID_EVENTO");
							SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld'", *caja_z, evento);
						RUN_QUERY(NO);
						if( FOUND2( ) ) {
							rta = 1;
							while( /*!BtrvEOF( )*/ !dbEOF() && fin ) {
								if( event_e_ticket.id_evento == evento &&
										event_e_ticket.caja_z == *caja_z ) {
									memcpy( &ev_cont, &event_e_ticket, sizeof( struct _ev_cont ) );
									//SELECT( AREA_AUX );
									SELECT_TABLE( T_EVC_TMP, TT_ORIG );
									#ifdef INVEL_L
									//BEGIN_TRANSACTION();
									#endif
									INSERT2(NO);
									#ifdef INVEL_L
									//END_TRANSACTION();
									#endif
									//SELECT( AREA_E_TICKET );
									SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
								} else {
									fin = 0;
								}
								SKIP2( 1 );
							}
						}
					} else {
						rta = 0;
						GRABAR_LOG_SISTEMA( "ERROR AL ABRIR EVC_TMP" , LOG_VENTAS,2 );
					}
					break;
				}
            case /*_PAGO_BTR*/ T_PAGOS:
				{
					struct
					{
						long caja__z;
						long id_evento;
					} ind4_all;
                    int order = 0;

					//if( USE_DATABASE( AREA_AUX5, EVM_TMP, ( char* )&ev_pago, sizeof( struct _ev_medios ), NULL, 0, 0 ) == 0 ) {
					if( OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&ev_pago,
											sizeof( struct _ev_medios ) ) == 0 ) {
						//SELECT( AREA_PAGO );
						SELECT_TABLE( T_PAGOS, tipo_t_pagos );
						ind4_all.caja__z = *caja_z;
						ind4_all.id_evento = evento;
						//order = GET_ORDER();
                        //SET_ORDER( 4 );
						//GET_EQUAL( ( char* )&ind4_all );
							SET_ORDER2("CAJA_Z, ID_EVENTO");
							SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld'", *caja_z, evento);
					
						RUN_QUERY(NO);
						if( FOUND2( ) ) {
							rta = 1;
							while( /*!BtrvEOF( )*/ !dbEOF() && fin ) {
								if( event_pago.id_evento == evento &&
										event_pago.caja_z == *caja_z ) {
									memcpy( &ev_pago, &event_pago, sizeof( struct _ev_medios ) );
									//SELECT( AREA_AUX5 );
									SELECT_TABLE( T_EVM_TMP, TT_ORIG );
									#ifdef INVEL_L
									//BEGIN_TRANSACTION();
									#endif
									INSERT2(NO);
									#ifdef INVEL_L
									//END_TRANSACTION();
									#endif
									//SELECT( AREA_PAGO );
									SELECT_TABLE( T_PAGOS, tipo_t_pagos );
								} else {
									fin = 0;
								}
								SKIP2( 1 );
							}
						}
                        //SET_ORDER( order );
					} else {
						rta = 0;
						GRABAR_LOG_SISTEMA( "ERROR AL ABRIR EVM_TMP" , LOG_VENTAS,2 );
					}
					break;
				}
			case /*_EV_DPAGO_BTR*/ T_EV_DMEDIOS:
				{
					/*struct
					{
						long caja_z;
						long id_evento;
					} ind2_all;*/
                    int order = 0;

					/*if( USE_DATABASE( AREA_AUX8, EVP_TMP, ( char* )&ev_dpago,
                           sizeof( struct _ev_dmedios ), NULL, 0, 0 ) == 0 ) {*/
					if( OPEN_TABLE( T_EVP_TMP, TT_ORIG, ( char* )&ev_dpago,
											sizeof( struct _ev_dmedios ) ) == 0 ) {
						//SELECT( AREA_DPAGO );
                        SELECT_TABLE( T_EV_DMEDIOS, TT_ORIG );
						//ind2_all.caja_z = caja_z;
						//ind2_all.id_evento = evento;
						//order = GET_ORDER( );
                        //SET_ORDER( 2 );
						//GET_EQUAL( ( char* )&ind2_all );
                        SET_ORDER2("CAJA_Z, ID_EVENTO");
                        SET_WHERE("CAJA_Z = %ld AND ID_EVENTO = %ld", *caja_z, evento );
                        RUN_QUERY(NO);
						if( FOUND2( ) ) {
							rta = 1;
							while( !dbEOF() /*!BtrvEOF( )*/ && fin ) {
								if( event_dpago->id_evento == evento &&
										event_dpago->caja_z == *caja_z ) {
									memcpy( &ev_dpago, event_dpago, sizeof( struct _ev_dmedios ) );
									//SELECT( AREA_AUX8 );
                                    SELECT_TABLE( T_EVP_TMP, TT_ORIG );
									INSERT2(NO);
									//SELECT( AREA_DPAGO );
                                    SELECT_TABLE( T_EV_DMEDIOS, TT_ORIG );
								} else {
									fin = 0;
								}
								SKIP2( 1 );
							}
						}
                        //SET_ORDER( order );
					} else {
						rta = 0;
						GRABAR_LOG_SISTEMA( "ERROR AL ABRIR EVP_TMP" , LOG_VENTAS,2 );
					}
					break;
				}
            case /*_EV_B_PROMO_BTR*/ T_EV_B_PROMO:
				{
					struct
					{
						long id_evento;
                        long caja__z;
					} ind2_all;
                    int order = 0;

					//if( USE_DATABASE( AREA_AUX10, EVBP_TMP, ( char* )&ev_b_promo, sizeof( struct _ev_bpromo ), NULL, 0, 0 ) == 0 ) {
					if( OPEN_TABLE( T_EVBP_TMP, TT_ORIG, ( char* )&ev_b_promo,
											sizeof( struct _ev_bpromo ) ) == 0 ) {
						//SELECT( AREA_EV_B_PROMO );
						SELECT_TABLE( T_EV_B_PROMO, tipo_t_ev_b_promo );
						ind2_all.id_evento = evento;
                        ind2_all.caja__z = *caja_z;
            			//order = GET_ORDER();
                        //SET_ORDER( 2 );
						//GET_EQUAL( ( char* )&ind2_all );
						SET_ORDER2("ID_EVENTO, CAJA_Z");
						SET_WHERE("ID_EVENTO='%ld' AND CAJA_Z='%ld'", evento, *caja_z);
						RUN_QUERY(NO);
						if( FOUND2( ) ) {
							rta = 1;
							while( /*!BtrvEOF( )*/ !dbEOF() && fin ) {
								if( event_b_promo.id_evento == evento &&
										event_b_promo.caja_z == *caja_z ) {
									memcpy( &ev_b_promo, &event_b_promo, sizeof( struct _ev_bpromo ) );
									//SELECT( AREA_AUX10 );
									SELECT_TABLE( T_EVBP_TMP, TT_ORIG );
									#ifdef INVEL_L
									//BEGIN_TRANSACTION();
									#endif
									INSERT2(NO);
									#ifdef INVEL_L
									//END_TRANSACTION();
									#endif
									//SELECT( AREA_EV_B_PROMO );
									SELECT_TABLE( T_EV_B_PROMO, tipo_t_ev_b_promo );
								} else {
									fin = 0;
								}
								SKIP2( 1 );
							}
						}
                        //SET_ORDER( order );
					} else {
						rta = 0;
						GRABAR_LOG_SISTEMA( "ERROR AL ABRIR EVBP_TMP", LOG_VENTAS,2  );
					}
					break;
				}
        }
	//SELECT( area_ant );
/*	if( tabla_ant!= -1 )
	   SELECT_TABLE( tabla_ant, tipo_ant );*/

    return ( rta );
}
/****************************************************************************/
int VERIFICAR_TICKET_ESPERA_PROCESADO( long caja_z, long id_evento )
/****************************************************************************/
{
    int /*area_ant = 0 ,*/ tabla_ant, tipo_ant, rta = 0;
	/*struct
	{
		long caja_z;
		long id_evento;
	} ind3_all;*/

	/* Verificamos si el Ticket en Espera fue Procesado Localmente*/

	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	//SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	//ind3_all.caja_z = caja_z;
	//ind3_all.id_evento= id_evento;
	//SET_ORDER( 4 );
	//GET_EQUAL( ( char* )&ind3_all );
	SET_ORDER2("CAJA_Z, ID_EVENTO");
	if ( MODALIDAD_TICKET_EN_ESPERA == 0 )
		SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO ='%ld'", caja_z, id_evento );
	else //busquemos si fue procesado o no localmente
		SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO_IMPORTADO ='%ld' AND TIPO_EVENT_IMPORTADO = 30", caja_z, id_evento );

	RUN_QUERY(NO);
	if( FOUND2( ) ) {
		while( /*!BtrvEOF( )*/ !dbEOF() && !rta ) {
			if( event_ticket.cajaz_importado == caja_z &&  event_ticket.id_evento_importado == id_evento ) {
				rta = 1;
			}
			SKIP2( 1 );
		}
	}
	SELECT_TABLE( tabla_ant, tipo_ant );

    return ( rta );
}
/*****************************************************************************/
int CUANTOS_TICK_ESPERA( long secuencia, int estado )
/* Contamos Ticket en Espera que no haya sido Levantado o Procesado */
/* estado:
#define ON       1 --> Contamos ticket en espera que estan en la base
#define OFF      0 --> Contamos ticket en espera que no estan en la base
#define ON_OFF   2 --> Contamos ticket en espera que estan y no en la base
*/
/*****************************************************************************/
{

	int tabla_ant = 0, tipo_ant = 0, cant = 0, cant_sin_ir = 0, cant_en_bd = 0;
	long caja_z = 0;
	char wheres[250];
	
	GRABAR_LOG_SISTEMA("##CUANTOS_TICK_ESPERA",LOG_DEBUG,4);
	caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
	
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );	
	/*localmente enviados sin procesar */
	_snprintf( wheres, sizeof( wheres )- 1,	"CAJA_Z='%ld' AND TIPO_EVENTO ='30'  AND STATUS = 1 AND ID_EVENTO NOT IN (SELECT ID_EVENTO_IMPORTADO FROM eventos WHERE  ID_EVENTO_IMPORTADO !=0 AND TIPO_EVENT_IMPORTADO = 30 and CAJAZ_IMPORTADO ='%ld' )",caja_z,caja_z);
	cant_en_bd = RECCOUNT2( NO, wheres );
	
	/*localmente sin enviar y sin procesar */
	_snprintf( wheres, sizeof( wheres )- 1,
		"CAJA_Z='%ld' AND TIPO_EVENTO ='30'  AND STATUS = 0 AND ID_EVENTO NOT IN (SELECT ID_EVENTO_IMPORTADO FROM eventos WHERE  ID_EVENTO_IMPORTADO !=0 AND TIPO_EVENT_IMPORTADO = 30 and CAJAZ_IMPORTADO ='%ld' )",caja_z,caja_z);
	cant_sin_ir = RECCOUNT2( NO, wheres );
	
	if( cant_en_bd ) {
		int encontrados=0;
		/*Evento Fue Enviado a la Base del Servidor.
		Verificamos si el Ticket en Espera fue Procesado Remotamente. 
		De ser asi, no lo insertamos en la tabla*/
		cant_en_bd = CONTAR_TICKET_ESPERA_SIN_PROCESAR( 30, NRO_CAJA,NRO_Z,0l);
		/*
		haria falta por las dudas la v_ticket_espera este retrasada.. consultar directo a los eventos. para ver si otra caja lo tomo
		if( !encontrados ){
		
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );	
			//estaban enviados y no estan en V_ticket_espera
			_snprintf( wheres, sizeof( wheres )- 1,"CAJA_Z='%ld' AND TIPO_EVENTO ='30' AND STATUS = 1 AND ID_EVENTO NOT IN (SELECT ID_EVENTO_IMPORTADO FROM eventos WHERE  ID_EVENTO_IMPORTADO !=0 AND TIPO_EVENT_IMPORTADO = 30 and CAJAZ_IMPORTADO ='%ld' )",caja_z,caja_z);
			cant_en_bd =  RECCOUNT2( NO, wheres);

		}else{// los de la base menos los que estan procesados
			cant_en_bd = encontrados;
		}*/
	}
	SELECT_TABLE( tabla_ant, tipo_ant );
	
	cant = cant_en_bd + cant_sin_ir;

    return ( cant );
}
/*****************************************************************************/
int CONTAR_TICK_ESP_SIN_PROCESAR( long secuencia, int estado )
/* Contamos Ticket en Espera que no haya sido Levantado o Procesado */
/* estado:
#define ON       1 --> Contamos ticket en espera que estan en la base
#define OFF      0 --> Contamos ticket en espera que no estan en la base
#define ON_OFF   2 --> Contamos ticket en espera que estan y no en la base
*/
/*****************************************************************************/
{

	int tabla_ant = 0, tipo_ant = 0, cant = 0, rta = 1, insert = 0;
	long id_evento = 0, caja_z = 0, pos = 0;
	char wheres[100];
	struct
	{
		long caja_z;
		long secuencia;
	} ind3_all;
	struct _eventos eventos;

	if( secuencia == 0 && estado == ON_OFF && !(RAM_COD_FUNCION == 38 || RAM_COD_FUNCION == 124) ){
		/* Solo consulto para verificar el label en pantalla*/
		 rta = CUANTOS_TICK_ESPERA( 0, ON_OFF );
		 return rta ;
	}
	/*sino proceso los ticket en espera */


	GRABAR_LOG_SISTEMA("##CONTAR_TICK_ESP_SIN_PROCESAR",LOG_DEBUG,4);
	caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
	//Es necesario cerrar la  tabla ya que si fue abierta por otra funcion da error 
	//si la abrimos nuevamente. 
	if( OPENED( T_EV_TMP, TT_TEMP1 ) ) {
        CLOSE_TABLE( T_EV_TMP, TT_TEMP1 );
    }
	if( OPEN_TABLE( T_EV_TMP, TT_TEMP1, ( char* )&eventos, sizeof( struct _eventos ) ) == 0 ) {
		ZAP2( );
	} else {
		rta = 0;
		MENSAJE( "ERROR AL ABRIR EV_TMP" );
		BORRAR_MENSAJE();
		GRABAR_LOG_SISTEMA( "ERROR AL ABRIR EV_TMP - CONTAR_TICK_ESP_SIN_PROCESAR" , LOG_VENTAS,2 );
		return ( 0 );
	}

	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );

	if( secuencia ) {
		ind3_all.caja_z = caja_z;
		ind3_all.secuencia = secuencia;
		//SET_ORDER( 6 );
		//GET_EQUAL( ( char* )&ind3_all );
		//SET_ORDER2("CAJA_Z, NRO_SECUENCIA");
		/*Se puede levantar un ticket en espera y enviarlo a espera nuevamente por esa razon
		TIPO_EVENT_IMPORTADO = 0 no puede ir en la consulta.*/
		SET_WHERE("CAJA_Z='%ld' AND NRO_SECUENCIA ='%ld'", caja_z, secuencia );
		//SET_WHERE("CAJA_Z='%ld' AND NRO_SECUENCIA ='%ld' AND TIPO_EVENT_IMPORTADO = 0", caja_z, secuencia );
		//SET_WHERE("CAJA_Z='%ld' AND NRO_SECUENCIA ='%ld' AND TIPO_EVENT_IMPORTADO = 0 AND STATUS = 0 ", caja_z, secuencia );
	} else {
		if( _TICKET_N_TICKET > 0 ) {
   			//SET_ORDER2("CAJA_Z, NRO_TICKET");
			/*Se puede levantar un ticket en espera y enviarlo a espera nuevamente por esa razon
			TIPO_EVENT_IMPORTADO = 0 no puede ir en la consulta.*/
			SET_WHERE("CAJA_Z='%ld' AND NRO_TICKET ='%ld'", caja_z, _TICKET_N_TICKET );
   			//SET_WHERE("CAJA_Z='%ld' AND NRO_TICKET ='%ld' AND TIPO_EVENT_IMPORTADO = 0", caja_z, _TICKET_N_TICKET );
			//SET_WHERE("CAJA_Z='%ld' AND NRO_TICKET ='%ld' AND TIPO_EVENT_IMPORTADO = 0 AND STATUS = 0 ", caja_z, _TICKET_N_TICKET );
		} else {
			//SET_ORDER2("CAJA_Z, NRO_TICKET");
			/*Se puede levantar un ticket en espera y enviarlo a espera nuevamente por esa razon
			TIPO_EVENT_IMPORTADO = 0 no puede ir en la consulta.*/
			SET_WHERE("CAJA_Z='%ld' AND TIPO_EVENTO ='30'", caja_z );
   			//SET_WHERE("CAJA_Z='%ld' AND TIPO_EVENTO ='30' AND TIPO_EVENT_IMPORTADO = 0", caja_z );
   			//SET_WHERE("CAJA_Z='%ld' AND TIPO_EVENTO ='30' AND TIPO_EVENT_IMPORTADO = 0 AND STATUS = 0 ", caja_z );
		}
	}

	strcpy(wheres, GET_WHERE ());
	RUN_QUERY(NO);
	rta = FOUND2( );

	while( !dbEOF() && rta ) {

		//barre todos los eventos segun el where tipo=30 para los T_espera
/*************************************************************************************************/

		if( event_ticket.tipo_evento == 30 ) {
			id_evento = event_ticket.id_evento;
			/*Verifico si existe localmente un evento asociado al TE
			aca deberiamos ver si el ticket en espera ya fue cancelado localmente por estar off la caja
			en el local anda bien porque se sobreescribe pero en el TE remoto no*/
			pos = GET_POSITION_REG();


			/* Verificamos si el Ticket en Espera fue Procesado Localmente*/
			if( !VERIFICAR_TICKET_ESPERA_PROCESADO( event_ticket.caja_z, event_ticket.id_evento ) ) {
				/*Me posiciono nuevamente en el TE*/
                SET_ORDER2("ID_EVENTO");
				SET_WHERE("ID_EVENTO='%ld'", id_evento );
				RUN_QUERY(NO);
				insert = 1;
				if( estado == OFF ) {
					if( EVENTO_A_PROCESAR( &event_ticket ) ) {
						cant ++;//Cantidad de ticket en espera no enviados
					}
				} else {
					if( estado == ON ) {
						if( !EVENTO_A_PROCESAR( &event_ticket ) ) {
						   cant ++;//Cantidad de ticket en espera enviados
						}
					} else {//Cantidad de ticket en espera no enviados y enviados
						cant ++;
					}
				}
				/*Evento Fue Enviado a la Base del Servidor.
				Verificamos si el Ticket en Espera fue Procesado Remotamente. 
				De ser asi, no lo insertamos en la tabla*/
				if( cant && event_ticket.status == 1 ) {
					if( !CONTAR_TICKET_ESPERA( event_ticket.tipo_evento, event_ticket.caja,
							event_ticket.nro_z, event_ticket.nro_evento, NO, NO ) ) {
						cant --;
						insert = 0;
					}
				}
			}
			/*Me posiciono nuevamente en el TE*/
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
			SET_WHERE("%s",wheres);
			//NUEVO
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
			GET_DIRECT_REG(pos, 0);
			if( event_ticket.id_evento < id_evento ) { //no me reposicione bien
				while( event_ticket.id_evento < id_evento ){
					SKIP2(1);
				}
			}
			if( cant && insert ) {
				SELECT_TABLE( T_EV_TMP, TT_TEMP1 );
				memcpy( &eventos, &event_ticket , sizeof( struct _eventos ) );
      			INSERT2(NO);
				SKIP2( 1 );
				SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
			}
			insert = 0;
		}
		if( secuencia ) {
			/*Para una secuencia especifica se procesa una solo vez*/
			rta = 0;
		}
		SKIP2( 1 );
	}
 
	if( OPENED( T_EV_TMP, TT_TEMP1 ) ) {
        CLOSE_TABLE( T_EV_TMP, TT_TEMP1 );
    }

	SELECT_TABLE( tabla_ant, tipo_ant );

    return ( cant );

}


/*****************************************************************************/
int VERIFICAR_ESTADO_TICK_ESP( long nro_caja, long nro_secuencia, int estado )
/*****************************************************************************/
{
   /* struct
    {
        long caja_z;
        long secuencia;
    } ind3_all;*/
    int rta = 0;
	long nro_caja_z;

	nro_caja_z = ( long )( nro_caja )*100000L + ( NRO_Z % 100000L );

    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    /*ind3_all.caja_z = nro_caja_z;
    ind3_all.secuencia = nro_secuencia;
	//SET_ORDER( 6 );
	//GET_EQUAL( ( char* )&ind3_all );*/
	if( PEDIR_CAJA_Y_SECUENCIA ) {
		if( nro_secuencia ) {
			SET_WHERE("CAJA_Z='%ld' AND NRO_SECUENCIA='%ld' AND TIPO_EVENT_IMPORTADO = 0 ", nro_caja_z, nro_secuencia);
		} else {
			SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO ='%ld' AND TIPO_EVENT_IMPORTADO = 0 ", nro_caja_z, _TICKET_ID_EVENTO);
		}
	} else
	   SET_WHERE("CAJA_Z='%ld' AND NRO_TICKET='%ld' AND TIPO_EVENT_IMPORTADO = 0 ", nro_caja_z, _TICKET_N_TICKET);

	RUN_QUERY(NO);

	if( FOUND2( ) && event_ticket.tipo_evento == 30 ) {
		if( estado == OFF ) {
			if( EVENTO_A_PROCESAR( &event_ticket ) ) {
				rta = 1;
			}
		} else {
			if( !EVENTO_A_PROCESAR( &event_ticket ) ) {
				rta = 1;
			}
		}
	}
	return ( rta );
}

/**
 * Se almacenan datos de un evento importado para facilitar su re-importacion.
 *****************************************************************************/
void GUARDAR_INFORMACION_IMPORTACION( int tipo_dev )
/*****************************************************************************/
{
    /*int actualizar = NO, cantidad = 0, iter;*/
    struct _comprob_importados comp_imp;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

    ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago,
                              &ev_despro, &ev_dpago, &ev_cod_barra, &ev_b_promo );

    /*cantidad = CANTIDAD_REGISTROS_REC( ARCH_REC_INFO_IMPORT,
                                       sizeof( struct _comprob_importados ) );*/
    /*se determina si ya existe un registro del mismo evento y si ocurre, sobreescribe*/
    /*for( iter = 0; iter < cantidad; iter++ ){
        RECUPERAR_ARCHIVO_REC( ARCH_REC_INFO_IMPORT, (char *) &comp_imp,
                               sizeof( struct _comprob_importados ), 0 );
        if( comp_imp.id_evento == id_evento &&
            comp_imp.caja_z == caja_z ){
            actualizar = SI;
            break;
        }
    }

    if( actualizar ) {
        ACTUALIZAR_ARCHIVO_REC( ARCH_REC_INFO_IMPORT, (char *) &comp_imp,
                                sizeof( struct _comprob_importados ), iter );
    }
    else {*/
		SELECT_TABLE( T_EV_TMP, TT_ORIG );
		SET_ORDER2("CAJA_Z, ID_EVENTO");
		RUN_QUERY(NO);

        comp_imp.id_evento = eventos.id_evento;
        comp_imp.caja_z = eventos.caja_z;
        comp_imp.caja = eventos.caja;
        comp_imp.secuencia = eventos.nro_evento;
        comp_imp.fecha = eventos.fecha;
        comp_imp.tipo_dev = tipo_dev;

        GUARDAR_ARCHIVO_REC ( ARCH_REC_INFO_IMPORT, (char *) &comp_imp,
                                sizeof ( struct _comprob_importados ) );
    /*}*/

    CERRAR_EVENTOS_TEMPORALES();
}
/**
 * Se carga la estructura con los datos para importar.
 * @param id_evento el id que identifica el evento
 * @param caja_z la caja_z que identifica el evento
 * @param pComp_imp la estructura que almacena el resultado
 * @returns 1 si se encontro el registro. 0 caso contrario.
 *****************************************************************************/
int CARGAR_INFORMACION_IMPORTACION( long id_evento, long caja_z,
                                    struct _comprob_importados *pComp_imp )
/*****************************************************************************/
{
    int cantidad = 0, iter;

    cantidad = CANTIDAD_REGISTROS_REC( ARCH_REC_INFO_IMPORT,
                                       sizeof( struct _comprob_importados ) );
    for( iter = ( cantidad - 1 ); iter >= 0 ; iter-- ){
        RECUPERAR_ARCHIVO_REC( ARCH_REC_INFO_IMPORT, (char *)pComp_imp,
                               sizeof( struct _comprob_importados ), iter );
        if( pComp_imp->id_evento == id_evento &&
            pComp_imp->caja_z == caja_z ){
            return 1;
        }
    }
    return 0;
}

/****************************************************************************/
int PEDIR_TICKET_EN_ESPERA( void )
/****************************************************************************/
{
   char i, ticket;
   int seleccion = -1, ok = 0, columnas = 0, cant_pend = 0/*, area_ant = 0*/;
	int tabla_ant, tipo_ant;
   struct _cabmenu *cab;
	struct _eventos eventos;
	char hora[20];
	struct _menu items[80];
	char nombres[80][50];

	//area_ant = SELECTED( );
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();



    if( !RAM_P_TICKET ) {
        ///deberiamos ver si tenemos ticket en espera no procesados
         cant_pend = CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF );

        // Hay Ticket Pendientes
        if( cant_pend ) {
			if( OPEN_TABLE( T_EV_TMP, TT_TEMP1, ( char* )&eventos, sizeof( struct _eventos ) ) == 0 ) {
				SET_WHERE("");
				RUN_QUERY(NO);
				GO2( TOP );
				ticket = 0;
				i = 0;
				while(!dbEOF() ) {
					if( ticket < MAX_CANTIDAD_TICKET_EN_ESPERA && i < MAX_CANTIDAD_TICKET_EN_ESPERA ) {
						// Armar string hora
						HTOAL( eventos.hora, hora );
						// Armar string menu
						_snprintf( ( char* )&nombres[i][0], sizeof(nombres[i]), " %d ) %04ld - %04ld - %s ", ticket, eventos.nro_evento, eventos.nro_ticket, hora );
						nombres[i][sizeof(nombres[i])-1] = '\0';
						items[i].nom = ( char* )&nombres[i][0];
						items[i].tecla = ticket + 48;   // para obtener el ascii
						// '1',...'n'
						items[i].rta = ticket;
						i++;
						ticket++;
					} else {
						GRABAR_LOG_SISTEMA( "SE SUPERO MAX_CANTIDAD_TICKET_EN_ESPERA", LOG_VENTAS,2 );
					}
					SKIP2( 1 );
				}
				columnas = 4;
				cab = ( struct _cabmenu * )malloc( sizeof( struct _cabmenu ) * columnas );
				if( cab != NULL ) {
					cab[0].nombre = "ID";
					cab[0].align = ALIGN_LEFT;
					cab[0].width = 8;
					cab[1].nombre = "SEC.";
					cab[1].align = ALIGN_LEFT;
					cab[1].width = 8;
					cab[2].nombre = "TICKET";
					cab[2].align = ALIGN_LEFT;
					cab[2].width = 8;
					cab[3].nombre = "HORA";
					cab[3].align = ALIGN_LEFT;
					cab[3].width = 20;
					seleccion = MENU( ST( S_TICKETS_EN_ESPERA ), items, i, cab, columnas );
					FREEMEM( (char *)cab );
					//La funcion GETCH llama a VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS( NO ) y esta 
					//funcion cierra la tabla T_EV_TMP
					if( !OPENED( T_EV_TMP, TT_TEMP1 ) ) {
						OPEN_TABLE( T_EV_TMP, TT_TEMP1, ( char* )&eventos, sizeof( struct _eventos ) );
						SET_WHERE("");
						RUN_QUERY(NO);
					}
				}
			}
			if( seleccion != -1 ) {
			   GO2( TOP );
			   SKIP2( seleccion );
               if( AUTORIZA_TICKET_EN_ESPERA( ST( S_TICKET_ESPERA ) ) ) {
					   SET_MEMORY_INT( _ticket_caja, NRO_CAJA );
					   SET_MEMORY_LONG( _ticket_secuencia, eventos.nro_evento );
					   SET_MEMORY_INT( _ticket_fecha, eventos.fecha );
					   SET_MEMORY_LONG( _ticket_n_ticket, eventos.nro_ticket );
                       SET_MEMORY_LONG( _ticket_id_evento, eventos.id_evento );
					   SET_MEMORY_INT( __ram_caja_anterior, _TICKET_CAJA );
					   SET_MEMORY_LONG( __nro_evento_anterior, _TICKET_SECUENCIA );
					   ok = 1;
               }
		   }
		   CLOSE_TABLE( T_EV_TMP, TT_TEMP1 );
		}
        else {
            MENSAJE_STRING( S_No_hay_ticket_en_espera_para_recuperar );
        }
    }

	SELECT_TABLE( tabla_ant, tipo_ant );

	return( ok );
}
/*******************************************************************************************************/
void SETEOS_DE_COMPROBANTE_ORIGINAL( int error )
/*******************************************************************************************************/
{
   	if( !error ) {
			struct _comprob_importados comp_imp;

			CARGAR_INFORMACION_IMPORTACION( event_ticket.id_evento_importado,
                                        event_ticket.cajaz_importado, &comp_imp );
			
			switch( event_ticket.tipo_evento_orig ) {
				case _EVENT_NC_A:
				case _EVENT_NC_B:
				case _EVENT_NC_A_REMOTA:
				case _EVENT_NC_B_REMOTA :
					{
						SET_MEMORY_CHAR( __ram_nota_cr, 1 );
						if( event_ticket.tipo_evento_orig == _EVENT_NC_B_REMOTA ||
							event_ticket.tipo_evento_orig == _EVENT_NC_B )
							SET_MEMORY_CHAR( __ram_factura, 2 );
						else
							SET_MEMORY_CHAR( __ram_factura, 1 );

						SET_MEMORY_INT( __ram_modo_devolucion, comp_imp.tipo_dev );

						if( DEBE_PEDIR_PRECIO_DEVOL_Y_NC( comp_imp.tipo_dev )){

						/*if( PEDIR_PRECIO_EN_NC && ( comp_imp.tipo_dev == _NC_RF_PARCIAL ||
													comp_imp.tipo_dev == _NC_LIBRE ) ) {*/
							SET_MEMORY_CHAR( __ram_pedir_precio, 1 );
						}
						if( comp_imp.tipo_dev != _NC_LIBRE ) {
						SET_MEMORY_INT( __ram_referencia_nc, 1 );
						}
					}
					break;

				case _EVENT_DEVOLUCION:
				{
					SET_MEMORY_INT( __modo_devolucion, 1 );
						SET_MEMORY_INT( __ram_modo_devolucion, comp_imp.tipo_dev );
						if( DEBE_PEDIR_PRECIO_DEVOL_Y_NC( comp_imp.tipo_dev )){
						/*if( PEDIR_PRECIO_EN_NC && ( comp_imp.tipo_dev == _DEVOL_PARCIAL ||
													comp_imp.tipo_dev == _DEVOL_LIBRE ) ) {*/
							SET_MEMORY_CHAR( __ram_pedir_precio, 1 );
						}
						if( comp_imp.tipo_dev != _DEVOL_LIBRE ) {
							SET_MEMORY_INT( __ram_referencia_nc, 1 );
						}
				}
				break;

				case _EVENT_TICKET_FACTURA_A  :
					SET_MEMORY_CHAR (__ram_ticket_factura, 1 );
					break;
				case _EVENT_TICKET_FACTURA_B  :
					SET_MEMORY_CHAR (__ram_ticket_factura, 2 );
				break;

				case _EVENT_FAC_A :
						SET_MEMORY_CHAR (__ram_factura, 1 );
				break;
				case _EVENT_FAC_B :
					SET_MEMORY_CHAR (__ram_factura, 2 );
				break;
			}
		}

    if( !error ) {
        //Se acumulan los tiempos invertidos en este Ticket
        SUMAR_VAR_UINT( __ram_tmpo_pago, event_ticket.tmpo_pago );
        SUMAR_VAR_UINT( __ram_tmpo_reg_articulo, event_ticket.tmpo_reg_articulo );
        SUMAR_VAR_UINT( __ram_tmpo_no_venta, event_ticket.tmpo_no_venta );
        SUMAR_VAR_UINT( __ram_tmpo_ocio, event_ticket.tmpo_ocio );
        //Se restan de el total x ya que luego sera acumulado nuevamente
        SUMAR_VAR_UINT( __x_tmpo_pago, -event_ticket.tmpo_pago );
        SUMAR_VAR_UINT( __x_tmpo_reg_articulo, -event_ticket.tmpo_reg_articulo );
        SUMAR_VAR_UINT( __x_tmpo_no_venta, -event_ticket.tmpo_no_venta );
        SUMAR_VAR_UINT( __x_tmpo_ocio, -event_ticket.tmpo_ocio );

    }
}

/**************************************************************************/
void SETEAR_COMPROBANTE( int tipo_comprobante )
/**************************************************************************/
{
	switch( tipo_comprobante ) {
		case 3://FACTURA A o CREDITO FISCAL
			SET_MEMORY_CHAR( __ram_factura, 1 );
			break;
		case 4://FACTURA B o FACTURA CONSUMIDOR FINAL
			SET_MEMORY_CHAR( __ram_factura, 2 );
			break;
		case 5://NOTA DE CREDITO A
			SET_MEMORY_CHAR( __ram_factura, 1 );
			SET_MEMORY_CHAR( __ram_nota_cr, 1 );
			break;
		case 6://NOTA DE CREDITO B
			SET_MEMORY_CHAR( __ram_factura, 2 );
			SET_MEMORY_CHAR( __ram_nota_cr, 1 );
			break;
		case 35://TICKET FACTURA A
			SET_MEMORY_CHAR (__ram_ticket_factura, 1 );
			break;
		case 36://TICKET FACTURA B
			SET_MEMORY_CHAR (__ram_ticket_factura, 2 );
			break;
	}
}
/***************************************************************************/
int  CARGAR_ARTICULOS_IMPORT_EN_PANEL(  )
/***************************************************************************/
{
    struct  _ev_cont ctrl_nc;
    struct  _ev_cont articulos_imp;
    int     cargar_articulo = 0, msj_error = 0;
    int     rta = 0, posicionTrabajoImport = 0;
    int     i = 0;
    char    cod_barra[17], cantidad[10], importe[10];
    double tot_imp = 0;
    unsigned long pos_reg = -1;

    //if( USED( AREA_AUX4 ) ) {
    //    CLOSE_DATABASE( AREA_AUX4 );
    //}

    //if( !USED( AREA_AUX4 ) ) {
	    //USE_DB( AREA_AUX4, _CTRL_NC_BTR, ( char* )&ctrl_nc, sizeof( event_e_ticket ), NULL, 0, 0 );
		OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc, sizeof( event_e_ticket ) );
		SET_WHERE("");
		RUN_QUERY(NO);
        GO2( TOP );
        while( /*!BtrvEOF( )*/ !dbEOF() ) {
            cargar_articulo = 1;
            if( ( ctrl_nc.origen == _ORIGEN_DESC_PESOS
			        || ctrl_nc.origen == _ORIGEN_DESC_PORC
			        || ctrl_nc.origen == _ORIGEN_PROMOCION
			        || ( ctrl_nc.origen == _ORIGEN_DIF_PRECIO
			            && RAM_REFERENCIA_NC ) )
			        || ctrl_nc.cod_cobro != 0 ) {
			    cargar_articulo = 0;
			}

			if( cargar_articulo ) {
                MENSAJE_SIN_SONIDO( ST( S_CARGANDO_ARTICULOS ), NO );
                pos_reg = GET_POSITION_REG();

 				ASIGNAR_PRECIO_TICKET_ORIGINAL( ctrl_nc );

                //CLOSE_DATABASE( AREA_AUX4 );
				CLOSE_TABLE( T_CTRL_NC, TT_ORIG );
                //USE_DB( AREA_AUX4, _CTRL_NC_BTR, ( char* )&ctrl_nc, sizeof( event_e_ticket ), NULL, 0, 0 );
                OPEN_TABLE( T_CTRL_NC, TT_ORIG, ( char* )&ctrl_nc, sizeof( event_e_ticket ) );
				GET_DIRECT_REG( pos_reg, 0 );

				if( ram_diferencia_precio ) {

					for( i = 0;i < 4;i++ ) {
                        tot_imp += ctrl_nc.monto_impuesto[i];
                    }

                    if( ( ( ctrl_nc.tipo3 & _PESABLE )
							|| ( ( ctrl_nc.tipo2 & _PEDIR_PRECIO ) && FUNCION_PIDE_PRECIO ) )
							&& ram_diferencia_precio ) {
                        ctrl_nc.total = ( -1 ) * ITEM_PRECIO * ctrl_nc.cantidad;
                    } else {
						ctrl_nc.total = ( -1 ) * ITEM_PRECIO * ctrl_nc.cantidad;
					}
					ctrl_nc.imp_int = ( -1 ) * ITEM_IMP_INT;
					if( ctrl_nc.origen == _ORIGEN_TICKET ) {
                        if( ctrl_nc.tipo & _GRAVADO ) {
                            ctrl_nc.importe_sin_iva = ( -1 ) * ITEM_PRECIO_SIN_IVA;
                        }
                        else {
                            ctrl_nc.exento = ( -1 ) * ITEM_PRECIO_SIN_IVA;
                        }
                    } else {
                        ctrl_nc.importe_sin_iva  = ( -1 ) * ITEM_PRECIO_SIN_IVA;
                    }
                    ctrl_nc.costo = ITEM_COSTO;
                    if( ITEM_PEDIR_PRECIO ) {
                        ctrl_nc.cantidad =(float) RAM_CANTIDAD;
                    }
                    //--- Fin ---
                    ctrl_nc.iva1 = ( -1 ) * ITEM_IVA1_PRECIO;
                    ctrl_nc.iva2 = ( -1 ) * ITEM_IVA2_PRECIO;

                    for( i = 0;i < 4;i++ ) {
                        ctrl_nc.monto_impuesto[i] = ( -1 ) * _ITEM_MONTO_IMPUESTO( i );
                    }
                    ram_diferencia_precio = 0;
                }
                GUARDAR_ARCHIVO_REC ( ARCH_REC_IMPORT_EN_PANEL, (char *) &ctrl_nc,
						sizeof( struct _ev_cont ) );
            }
            SKIP2( 1 );
        }
    //}

    /********************* Cargamos Articulos Importados en Panel *********************/

    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORT_EN_PANEL, ( char* )&articulos_imp,
           sizeof( struct _ev_cont ), posicionTrabajoImport ) == 0 ) {

        memset( cantidad, 0, sizeof( cantidad ) );
        memset( importe, 0, sizeof( importe ) );
		memset( cod_barra, 0, sizeof( cod_barra ) );
		memcpy( cod_barra, articulos_imp.cod_barra, 17 );

        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
            if( !BUSCA_COD_INTERNO_COD_BARRA( articulos_imp.cod_articulo, cod_barra ) ) {
                msj_error = 1;
            }
        }
        else {
            if( !BUSCA_CODIGO_ESPECIAL( articulos_imp.cod_articulo ) ) {
                msj_error = 1;
            }
        }

        if(  msj_error ) {
            MENSAJE( ST ( S_EL_ARTICULO_NO_ESTA_EN_PADRON ) );
            BORRAR_MENSAJE( );
            return 0;

        } else {
            _snprintf( cantidad, sizeof( cantidad ), "%.3f", articulos_imp.cantidad );
            cantidad[ sizeof( cantidad ) - 1] = '\0';
            _snprintf( importe, sizeof( importe ), "%.2lf", articulos_imp.total );
            importe[ sizeof( importe ) - 1] = '\0';

            agregarImportacion( cantidad, cod_barra, articulo.nombre, importe );

        }
        posicionTrabajoImport++;
    }

    BORRAR_MENSAJE( );

    /********************* Mostrar Articulos Importados en Panel *********************/

    if( CANTIDAD_REGISTROS_REC( ARCH_REC_IMPORT_EN_PANEL, sizeof( struct _ev_cont ) ) ) {

        ExecuteAction( A_ACTIVAR_PANEL_IMPORTACION );

        rta = ManipularImportacion( posicionTrabajoImport );

        ExecuteAction( A_DESACTIVAR_PANEL_IMPORTACION );

    }

    return rta;
}

/***********************************************************************/
int ManipularImportacion( int cant )
/***********************************************************************/
{
    int seguir = 1, k, rta = 0, cont = 0;
    int grilla = A_SCROLL_IMPORTACION;

    ExecuteSAction( grilla, "0" );            // 0 habilita

    while( seguir ) {
        k = GETCH();

        switch( k ) {
            case 27:
                /*-------- ESC -----------*/
                seguir = 0;
                ExecuteSAction( grilla, "-99" ); //-99 deshabilita
                rta = 0;    //sin accion
                break;
            case 32:
                /*-------- TOTAL ---------*/
                 if( MARCAR_ARTICULOS_IMPORT_EN_PANEL( cont ) ) {
                     ExecuteSAction( grilla, "100" );// deshabilita
                 } else {
                     ExecuteSAction( grilla, "-100" );// habilita
                 }
                 break;
            case 13:
                /*-------- ENTER ---------*/
                seguir = 0;
                rta = 1;  //editar
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSAction( grilla, "-1" );
                if( cont > 0 ) {
                    rta = 1;
                    cont--;
                }
                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSAction( grilla, "1" );
                if( cont < cant ) {
                    rta = 1;
                    cont++;
                }
                break;
         }
    }

    return ( rta );
}

/*****************************************************************************/
int MARCAR_ARTICULOS_IMPORT_EN_PANEL( int registro )
/*****************************************************************************/
{
    struct _ev_cont articulos_imp;

    RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORT_EN_PANEL, ( char* )&articulos_imp,
            sizeof( struct _ev_cont ), registro );
	if( !( articulos_imp.tipo & _ENGANCHADO ) ) {
		articulos_imp.status = !articulos_imp.status;

		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_IMPORT_EN_PANEL, ( char* )&articulos_imp,
							sizeof( struct _ev_cont ), registro );
	} else {
		MENSAJE( ST ( S_ARTICULO_ENGANCHE ) );
		BORRAR_MENSAJE();
	}

    return( articulos_imp.status );
}

/*****************************************************************************/
void AGREGAR_ITEMS_IMPORT_EN_PANEL_AL_TICKET(  )
/*****************************************************************************/
{
    struct _ev_cont articulos_imp;
    int posicionTrabajoImport = 0;

    while( RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORT_EN_PANEL, ( char* )&articulos_imp,
            sizeof( struct _ev_cont ), posicionTrabajoImport ) == 0 ) {
		if( articulos_imp.status ) {
            memcpy( &event_e_ticket, &articulos_imp, sizeof( struct _ev_cont ) );
            CARGA_ITEM( 0, NULL );
            CR1_AGREGA_ITEM( NO, articulos_imp.origen, 0, 0 );
			/*--------------------------- Busca enganche -----------------------------*/
			if( RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORT_EN_PANEL, ( char* )&articulos_imp,
					sizeof( struct _ev_cont ), posicionTrabajoImport + 1 ) == 0 ) {
				if( articulos_imp.tipo & _ENGANCHADO ) {
					memcpy( &event_e_ticket, &articulos_imp, sizeof( struct _ev_cont ) );
					CARGA_ITEM( 0, NULL );
					CR1_AGREGA_ITEM( SI, articulos_imp.origen, 0, 0 );
					posicionTrabajoImport ++;
				}

			}
        }
        posicionTrabajoImport ++;
    }

}

/*****************************************************************************/
int VERIFICAR_TIPO_COMPROBANTE( int tipo_evento_original )
/*****************************************************************************/
{
	int tipo_evento_actual = 0, modificar = NO, tipo = 0;

	if( RAM_FACTURA == 1 ) {
		tipo_evento_actual = _EVENT_FAC_A;
		tipo = FACTURA_TIPO_A;
	} else {
		if( RAM_FACTURA == 2 ) {
			tipo_evento_actual = _EVENT_FAC_B;
			tipo = FACTURA_TIPO_B;
		}
	}
	if( RAM_NOTA_CR == 1 ) {
		tipo_evento_actual = _EVENT_NC_A;
		tipo = FACTURA_TIPO_A;
	} else {
		if( RAM_NOTA_CR == 2 ) {
			tipo_evento_actual = _EVENT_NC_B;
			tipo = FACTURA_TIPO_B;
		}
	}
	if( RAM_TICKET_FACTURA == 1 ) {
		tipo_evento_actual = _EVENT_TICKET_FACTURA_A;
		tipo = FACTURA_TIPO_A;
	} else {
		if( RAM_TICKET_FACTURA == 2 ) {
			tipo_evento_actual = _EVENT_TICKET_FACTURA_B;
			tipo = FACTURA_TIPO_A;
		} else {
			tipo_evento_actual = _EVENT_TICKET;
			tipo = 0;
		}
	}	
	if( tipo_evento_actual != tipo_evento_original ) 
	{
		if( MENSAJE_CON_TECLA_S_N
				("DESEA MODIFICAR EL TIPO DE COMPROBANTE ORIGINAL? <ENTER>=SI <ESC>=NO",
					13,27 ) == 13 ) {
			modificar = SI;
		} else {
			SET_MEMORY_CHAR( __ram_factura, 0 );
			SET_MEMORY_CHAR( __ram_nota_cr, 0 );
			SET_MEMORY_INT( __ram_referencia_nc, 0 );
			SET_MEMORY_CHAR (__ram_ticket_factura, 0 );
			SET_MEMORY_INT( __modo_devolucion, 0 );
			MUESTRA_COMPROBANTE();
		}
	} 

	return( modificar );

}

/*****************************************************************************/
int VERIFICAR_EVENTO_EN_PROMOCION( void )
/*****************************************************************************/
{
    struct _ev_cont ev_cont;
	struct _eventos eventos;
	int area_ant = 0, tipo_ant = 0, tipo_evento = 0, verifica = NO, rta = 0;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	if( OPENED( T_EV_TMP, TT_ORIG ) ) {
		CLOSE_TABLE( T_EV_TMP, TT_ORIG );
	}
	if( OPEN_TABLE( T_EV_TMP, TT_ORIG, ( char* )&eventos, sizeof( struct _eventos ) ) == 0 ) {
		memset( &eventos, 0, sizeof( struct _eventos ) );
		SET_WHERE("");
		RUN_QUERY(NO);
		tipo_evento = eventos.tipo_evento;
		
		switch( tipo_evento ) {
            case _EVENT_TICKET :
            case _EVENT_FAC_A :
            case _EVENT_FAC_B :
            case _EVENT_NC_A :
            case _EVENT_TICKET_FACTURA_A :
            case _EVENT_TICKET_FACTURA_B :
                verifica = SI;
		}
		if( verifica ) {
			if( OPENED( T_EVC_TMP, TT_ORIG ) ) {
				CLOSE_TABLE( T_EVC_TMP, TT_ORIG );
			}
			if( OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )&ev_cont,	sizeof( struct _ev_cont ) ) == 0 ) {
				memset( &ev_cont, 0, sizeof( struct _ev_cont ) );
				SET_WHERE( "ORIGEN = '%i'", _ORIGEN_PROMOCION );
				RUN_QUERY(NO);
				rta = RECCOUNT2(NO,NULL);
			}
		}
	}
	
	CLOSE_TABLE( T_EV_TMP, TT_ORIG );
	CLOSE_TABLE( T_EVC_TMP, TT_ORIG );
	SELECT_TABLE( area_ant, tipo_ant );

    return rta;
}


/***********************************************************************/
int RECUPERAR_ALICUOTA_DE_ARTICULO(  int tabla, int cod_exento, char *cod_barra, long cod_interno )
/***********************************************************************/
{
	int _cod_iva=-999, iva = -1;	
	
	if( tabla == NO ) {
		if( config_tps.AlicuotasEnArticulo == 1 ) {
			if( cod_exento > 0 ) {
				DIRECT_DB_QUERY("select cod_iva from articulo where cod_interno=%ld and codigo_barra='%s'",
					cod_interno, cod_barra );
				GET_DATO(1, C_INT, (char *) &_cod_iva, 4);
				if( _cod_iva!=-999 && _cod_iva <= MAX_COD_IVA) {
					iva = _cod_iva;//Cod iva del padron
				}
			} 
		}
	} else {
		if( event_e_ticket.cod_depto_old ) {
			if( event_e_ticket.cod_depto_old <= MAX_COD_IVA
					&& DIRECT_DB_QUERY("select * from articulo_iva where cod_iva=%i", event_e_ticket.cod_depto_old )  > 0 ) { //= 0->No existe registro, = 1->Existe registro
				iva = event_e_ticket.cod_iva = event_e_ticket.cod_depto_old;		
			} else {
				char aux[150];
				memset( aux, 0, sizeof( aux ) );
				_snprintf(aux, sizeof(aux) - 1, "Val Art. cod_exento %i inexistente en tabla articulo_iva (Art. COD_ART %ld COD_BARRA %s)",
						event_e_ticket.cod_depto_old, event_e_ticket.cod_articulo, event_e_ticket.cod_barra );
				GRABAR_LOG_SISTEMA(aux, LOG_ERRORES, 1);
			}
		}	
	}

	return iva;
}
