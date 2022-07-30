#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <bancos.h>
#include <btrv.h>
#include <b_mens.h>
#include <cierre.h>
#include <clave.h>
#include <cmemory.h>
#include <cr.h>
#include <dbrouter.h>
#include <eventos.h>
#include <fiscal.h>
#include <ftoa.h>
#include <getch.h>
#include <get_fech.h>
#include <get_hora.h>
#include <imp_ret.h>
#include <inf.h>
#include <inf00.h>
#include <inf10.h>
#include <inf_dif.h>
#include <lista.h>
#include <log.h>
#include <mensaje.h>
#include <mstring.h>
#include <m_cadena.h>
#include <pagos.h>
#include <panel.h>
#include <pant.h>
#include <print.h>
#include <p_float.h>
#include <recupero.h>
#include <round.h>
#include <stack.h>
#include <stringt.h>
#include <tkt.h>
#include <trim.h>
#include <t_pago.h>
#include <ventas2.h>
#include <xyz.h>
#include <_cr1.h>
#include <assert.h>
#include <deftables.h>
#include <db.h>
#include <cio.h>
#include <str.h>
#include <_cr0.h>
#include <cont_sub.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define _CIERRE_C
#undef  _CIERRE_C
int c = 0;

double importe_inf = 0;
double importe_dec = 0;
int sub_medios = 0;

extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos, tipo_t_cierre_lote;
/*struct _datosCierre
{
    double tot[10],dif[10],cierre[10],tot_me[10],dif_me[10],cierre_me[10];
};*/
extern double MUTUALES_ML_A_ME( double importe_me, int h );
extern double MUTUALES_ME_A_ML( double importe_me, int h );
extern int MUTUAL_MONEDA_EXTRANJERA( int h );
extern int ABRIR_CAJON();
void CALCULAR_IMPORTES_ARQUEO( char tecla,char *operacion, double importe_orig, double *importe_calculado, double *importe_ingresado);
/*****************************************************************************/
int CIERRE()
/*****************************************************************************/
{
    //double tot[10],dif[10],cierre[10],tot_me[10],dif_me[10],cierre_me[10]/*, tot2[10]*/;
    struct _datosCierre datosCierre;
    char h,cadena[30];
    int salir = 0, hay_dif = 0, cierre_confirmado = 1;
    int _ptr,_handle,_estado_cierre,j;
    #if defined(INVEL_W) || defined(INVEL_L)
    char stringData[17],stringData2[17]/*, stringData3[40]*/,aux[10];
    _itoa( TIT_CIERRE, aux, 10 );
    #endif
    /*memset( tot, 0, sizeof( tot ) );
    memset( cierre, 0, sizeof( cierre ) );
    memset( dif, 0, sizeof( dif ) );
    memset( tot_me, 0, sizeof( tot_me ) );
    memset( cierre_me, 0, sizeof( cierre_me ) );
    memset( dif_me, 0, sizeof( dif_me ) );*/
	memset( aux, 0, sizeof( aux ) );
	memset( stringData, 0, sizeof( stringData ) );
    memset( stringData2, 0, sizeof( stringData2 ) );
    memset( &datosCierre, 0, sizeof( struct _datosCierre ) );
    INIT_VEC_DIFERENCIAS( -1 ); //negativo lo borra totalmente
	MENSAJE_SIN_SONIDO_STRING( S_CERRANDO_CAJA );
    OPEN_RECUPERO( &_ptr, &_handle );
    //_estado_cierre = STACK_INS( sizeof( int ) );
    _estado_cierre = STACK_INS( sizeof( short ) );
    Orden = NUMERO_PANEL_BILLETES;
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
    if( _ESTADO_CIERRE == 0 ) {
		if( !CAJEROS_FLOTANTES ) {
            //*---------------------- prepara pantalla --------------------------------*/
			if( PEDIR_DIFERENCIAS ) {
                CREAR_ARRAY_DIFERENCIAS();
                if( !LLENAR_ARRAY_DIFERENCIAS() ) {
                    MENSAJE_STRING( S_ERROR_LLENAR_ARRAY );
                    return 0;
                }
				if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ  ) {
					//VACIAR_DEC_SUBMEDIOS();
					#if defined(INVEL_W) || defined(INVEL_L)
					ExecuteSSAction( A_DES_VTAS_ACT_CIERRE, T_CIERRE_SUBMEDIOS, aux, "", NULL, " " );
					#endif
					CIERRE_SUBMEDIOS_CONFECCIONA_PANTALLA( SI );
				}
				else {
					#if defined(INVEL_W) || defined(INVEL_L)
					//ExecuteSSAction( A_DES_VTAS_ACT_CIERRE, T_CIERRE_MEDIOS, aux, "", NULL, " " );
					//Se utiliza la Pant. de Submedios hasta que se cree la Pantalla de Cierre donde solo visualiza los medios
					ExecuteSSAction( A_DES_VTAS_ACT_CIERRE, T_CIERRE_SUBMEDIOS, aux, "", NULL, " " );
					#endif
					CIERRE_SUBMEDIOS_CONFECCIONA_PANTALLA( SI );
					//CIERRE_CONFECCIONA_PANTALLA( SI );
				}
			}
			MENSAJE_SIN_SONIDO_STRING( S_CERRANDO_CAJA );
            BORRAR_MENSAJE( );
			while( !salir ) {
                cajero_operando = 0;
                //*--------------------- calcula lo que hay -------------------------------*/
                for( h = 0;h < 9;h++ ) {
                    if( medios[h].nro ) {
                        if( medios[h].efectivo ) {
                            datosCierre.tot[h] = X_TOT_DINERO;
                        }
                        else {
                            datosCierre.tot[h] = _X_VENTAS_IMPORTE( h ) + _X_COBROS_IMPORTE( h )
                                               - _X_RETIROS_IMPORTE( h )
                                               + _X_FONDO_FIJO_MULTIPLE_IMPORTE( h )
                                               + _X_CAMBIOS_IMPORTE( h );
                            datosCierre.tot_me[h] = _X_VENTAS_IMPORTE_ME( h )
                                                  + _X_COBROS_IMPORTE_ME( h )
                                                  - _X_RETIROS_IMPORTE_ME( h )
                                                  + _X_FONDO_FIJO_MULTIPLE_IMP_ME( h );
                        }
                    }
                }
                /*controla si tiene que recuperar*/
                if( CANTIDAD_REGISTROS_REC( ARCH_REC_CIERRE, sizeof( struct _datosCierre ) ) == 0 ) {
                    /*guarda el vector en blanco*/
                    GUARDAR_ARCHIVO_REC( ARCH_REC_CIERRE, ( char* )&datosCierre,
                                         sizeof( struct _datosCierre ) );
                }
                else {
                    RECUPERAR_ARCHIVO_REC( ARCH_REC_CIERRE, ( char* )&datosCierre,
                                           sizeof( struct _datosCierre ), 0 );
                    for( h = 0;h < 9;h++ ) {
                        if( datosCierre.cierre[h] > 0 ) {
                            // si hay algun importe cargado
                            FTOA( datosCierre.cierre[h], stringData, ENTEROS + 3, DECIMALES, 1, 1 );
                            if( MOSTRAR_DIFERENCIAS ) {
                                FTOA( ( datosCierre.cierre[h] - datosCierre.tot[h] ), stringData2,
                                      ENTEROS + 3, DECIMALES, 1, 1 );
                            }
                            if( medios[h].nro ) {
                                ExecuteSSAction( A_RESALTAR_MEDIO_CIERRE, h, medios[h].nombre, "",
                                                 NULL, " " );
                                ExecuteSSAction( A_MOSTRAR_IMPO_EN_CIERRE, h, medios[h].nombre,
                                                 stringData, stringData2, " " );
                            }
                        }
                    }
                }
                //*---------------------- Pide los importes ------------------------------*/
				if( /* DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ */ PEDIR_DIFERENCIAS ) {
					MENSAJE_SIN_SONIDO( "TECLA TOTAL FINALIZA",NO );
                    while( 1 ) {
                        int tecla = GETCH();
                        if( tecla == 32 ) {
                            // espacio
                            break;
                        }
                        else if( tecla == 27 ) {
                            // escape
                            if( ABORTA_CIERRE() ) {
                                cierre_confirmado = 0;
                                salir = 1;
                                MENSAJE_STRING( S_CIERRE_ABORTADO );
                                BORRAR_MENSAJE( );
                                break;
                            }
                        }else if( tecla == 137 && SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI,ST(S__ABRIR_CAJON_), NULL, RAM_COD_FUNCION, SI ) ){ //la configurada para abrir el cajon en el teclado
							ABRIR_CAJON();
						}
						if( tecla == 27)
							MENSAJE_SIN_SONIDO( "TECLA TOTAL FINALIZA",NO );
                        tecla -= 48;     // obtener tecla en int
                        if( tecla > 0 && tecla < 10 ) {
                            h = tecla - 1;
                            if( medios[h].nro ) {
								if( GetConfig( CONF_TOUCH_SCREEN ) ) {
                                    /* Informo al touch screen un cambio de pantalla */
                                    ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "20" );
                                    INGRESA_IMPORTE_DISC_CIERRE( h, datosCierre.tot[h],
                                                                 &datosCierre.cierre[h],
                                                                 &datosCierre.dif[h],
                                                                 datosCierre.tot_me[h],
                                                                 &datosCierre.cierre_me[h],
                                                                 &datosCierre.dif_me[h] );
                                    /* Vuelvo a la pantalla anterior del touch */
                                    ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "04" );
                                }
                                else {
                                    INGRESA_IMPORTE_DISC_CIERRE( h, datosCierre.tot[h],
                                                                 &datosCierre.cierre[h],
                                                                 &datosCierre.dif[h],
                                                                 datosCierre.tot_me[h],
                                                                 &datosCierre.cierre_me[h],
                                                                 &datosCierre.dif_me[h] );
                                }
								if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ ){
									if( medios[h].tarjeta || medios[h].ticket_compra
										|| medios[h].cheque ) {
											ExecuteAction( A_BORRA_DIF_SUBMEDIOS );// Borro los valores cargados en la columna diferencia en submedios.
									}
								}
                                _ATRIBUTO = 120;  // Prende el Numero
                                MOSTRAR_ENTERO( 5, 6 + h, "9", h + 1 );
                                _ATRIBUTO = 52;
                                /*Actualiza el vector cierre*/
                                ACTUALIZAR_ARCHIVO_REC( ARCH_REC_CIERRE, ( char* )&datosCierre,
                                                        sizeof( struct _datosCierre ), 0 );
                            							}
							if( tecla != -999)
								MENSAJE_SIN_SONIDO( "TECLA TOTAL FINALIZA",NO );
						}//else 
						//		MENSAJE_SIN_SONIDO( "TECLA TOTAL FINALIZA",NO );
                    }
                }
                /*else {
					//Se comenta hasta que se cree la pantalla de cierre donde solo visualiza los medios.
					for( h = 0;h < 9;h++ ) {
						if( medios[h].nro ) {
							INGRESA_IMPORTE_CIERRE( h, datosCierre.tot[h], &datosCierre.cierre[h], &datosCierre.dif[h], datosCierre.tot_me[h],
								&datosCierre.cierre_me[h], &datosCierre.dif_me[h] );
						}
					}
                }*/
				if( PEDIR_DIFERENCIAS ) {
					//Se muestran las diferencias de todos los medios
					if( cierre_confirmado && MODO_NEGOCIO == SERVICIOS ) {
						for( h = 0;h < 9;h++ ) {
							FTOA( datosCierre.cierre[h], stringData, ENTEROS + 3, DECIMALES, 1, 1 );
							//sprintf( stringData3, stringData );
							if( MOSTRAR_DIFERENCIAS ) {
								//strcat( stringData3, "$" );
								FTOA( ( datosCierre.cierre[h] - datosCierre.tot[h] ), stringData2,
									  ENTEROS + 4, DECIMALES, 1, 1 );
								//strcat(stringData3, stringData2);
							}
							if( medios[h].nro ) {
								#if defined(INVEL_W) || defined(INVEL_L)
								ExecuteSSAction( A_RESALTAR_MEDIO_CIERRE, h, medios[h].nombre, "",
												 NULL, " " );
								ExecuteSSAction( A_MOSTRAR_IMPO_EN_CIERRE, h, medios[h].nombre,
												 stringData, stringData2, " " );
								#endif
							}
						}
						if( MOSTRAR_DIFERENCIAS ) {
							int ok_super = 0;hay_dif = 0;
							for( h = 0;h < 9;h++ ) {
								if( fabs( datosCierre.tot[h] - datosCierre.cierre[h] ) > 0.00001 ) {
									hay_dif = 1;
									break;
								}
							}
							do {
								ok_super = SOLICITAR_CLAVES( 30, 20, NO, SI, hay_dif,
															 "  AUTORIZAR CIERRE CON DIF",
															 "AUTORIZO CIERRE CON DIFERENCIAS",
															 _F_SUP_AUT_CIERRE_CON_DIF, SI );
								if( hay_dif ) {
									if( 27 == LASTKEY() && ABORTA_CIERRE() ) {
										ok_super = 1;
										cierre_confirmado = 0;
										MENSAJE_STRING( S_CIERRE_ABORTADO );
									}
									//Si hubo dif se sale si o si (por abortado o por aprobado)
									salir = 1;
								}
							}
							while( !ok_super );
						}
					}
					if( cierre_confirmado && !hay_dif ) {
						//Solo permite reintentar si no hubo diferencias
						if( REINTENTA_RENDICION() && IMPRESORA_FISCAL != FISCAL_IBM_4610
						 && IMPRESORA_FISCAL != FISCAL_TMT88 ) {
							PRN_SELECT( JOURNAL );
							_snprintf(cadena, sizeof(cadena)-1, ST( S_REINTENTO_ARQUEO___LI_ ), USUARIO_CLAVE_ANTERIOR() );
							MOSTRAR_CADENA( 1, PRN, cadena );
							LF( 1 );
							PRN_HABILITA_AVANCES();
							/*#if defined(INVEL_W) || defined(INVEL_L)
							ExecuteAction( A_REINTENT_ARQUEO_CIERRE );
							#endif*/
						}
						else {
							salir = 1;
						}
					}//cierre_confirmado
				} else {
					//cierre_confirmado sin arqueo
					salir = 1;
				}

                #ifdef _DEBUG_DIF
                DEBUG_VERIFICAR_CONSISTENCIA_DIFERENCIAS( datosCierre.tot );
                #endif

				SET_MEMORY_LONG(__x_nro_transaccion_final, NRO_EVENTO-1);//seba

				if( cierre_confirmado && salir ) {
                    GRABAR_CABECERA_EVENTO_INFORMES( !salir, NO );
                    /*  for( h = 0; h < 9; h++ ) {
                       if( medios[h].nro ) {
                          memset( &event_pago, 0, sizeof( event_pago ) );
                          event_pago.posicion = h;
                          GRABAR_EVENTO_DIFERENCIAS( medios[h].nro, cierre[h], tot[h],   //DIEGO
                             cierre_me[h], tot_me[h] );
                       }
                    }*/
                    for( h = 0;h < 9;h++ ) {
                        if( medios[h].nro ) {
                            memset( &event_pago, 0, sizeof( event_pago ) );
                            if( medios[h].efectivo ) {
                                //event_pago.posicion = c;
                                GRABAR_EVENTO_DIFERENCIAS( medios[h].nro, datosCierre.cierre[h],
                                                           datosCierre.tot[h],
                                                           datosCierre.cierre_me[h],
                                                           datosCierre.tot_me[h], 0 );
                                //c++;
                            }
                            else if( medios[h].moneda_extranjera ) {
                                //event_pago.posicion = c;
                                GRABAR_EVENTO_DIFERENCIAS( medios[h].nro, datosCierre.cierre[h],
                                                           datosCierre.tot[h],
                                                           datosCierre.cierre_me[h],
                                                           datosCierre.tot_me[h], 0 );
                                //c++;
                            }
                            else if( medios[h].cheque ) {
                                //event_pago.posicion = c;
                                GRABAR_EVENTO_DIFERENCIAS( medios[h].nro, datosCierre.cierre[h],
                                                           datosCierre.tot[h],
                                                           datosCierre.cierre_me[h],
                                                           datosCierre.tot_me[h], 0 );
                                //c++;
                            }
                            else if( medios[h].cta_cte ) {
                                //event_pago.posicion = c;
                                GRABAR_EVENTO_DIFERENCIAS( medios[h].nro, datosCierre.cierre[h],
                                                           datosCierre.tot[h],
                                                           datosCierre.cierre_me[h],
                                                           datosCierre.tot_me[h], 0 );
                                //c++;
                            }
							else {
								if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ){
									if( medios[h].mutual || medios[h].ticket_compra || medios[h].tarjeta ) {
										for( j = 0;j < cant_mem_rendicion_medio[h] ;j++ ) {
											//event_pago.posicion = c;
											if((RENDICION_MEDIO( h, j )->monto + RENDICION_MEDIO( h, j )->monto_retiro )!= 0.0)
												GRABAR_EVENTO_DIFERENCIAS( medios[h].nro, datosCierre.cierre[h],
																   datosCierre.tot[h],
																   datosCierre.cierre_me[h],
																   datosCierre.tot_me[h],
																   RENDICION_MEDIO( h, j )->codigo );
											//c++;
										}
									}
								}else{
									GRABAR_EVENTO_DIFERENCIAS( medios[h].nro, datosCierre.cierre[h],
                                    datosCierre.tot[h],
                                    datosCierre.cierre_me[h],
                                    datosCierre.tot_me[h], 0 );
								}
							}
                        }
                    }//for
                    ACTUALIZA_CABECERA_EVENTO();
                    VACIAR_SUBMEDIO();
                    c = 0;
                    ACTUALIZA_ID_EVENTO_NEW();
                }//cierre_confirmado && salir
            }//while(!salir)
            if( cierre_confirmado && config.pais != CHILE && config.pais != BRASIL ) {
                if( !IMPRIMIR_Y_EN_Z ) {
                    CIERRE_RETIRO( datosCierre.cierre, datosCierre.cierre_me );
                }
            }
            cajero_operando = 1;
        }
        _SET_ESTADO_CIERRE( 1 );
    }
    if( cierre_confirmado ) {
        if( _ESTADO_CIERRE == 1 ) {
            if( !IMPRIMIR_Y_EN_Z && EMITIR_INF00_EN_Y ) {
                INFORME_00( _x, SI, NO );
            }
            //_SET_ESTADO_CIERRE( 2 );
			_SET_ESTADO_CIERRE( 3 );
        }
        if( _ESTADO_CIERRE == 3 ) {
            if( !IMPRIMIR_Y_EN_Z && INF_TAR_HABILITADO == 1 ) {
            	INFORME_TARJETAS( _x );
            }
            _SET_ESTADO_CIERRE( 4 );
        }
        if( _ESTADO_CIERRE == 4 ) {
            if( !IMPRIMIR_Y_EN_Z && EMITIR_INFORME_10_EN_Y ) {
                INFORME_10( _x );
            }
            _SET_ESTADO_CIERRE( 5 );
        }
        if( _ESTADO_CIERRE == 5 ) {
            if( !CAJEROS_FLOTANTES ) {
                if( !IMPRIMIR_Y_EN_Z && EMITIR_INFORME_DIFERENCIAS ) {
                    //INFORME_DIFERENCIAS( datosCierre.dif, SI, datosCierre.dif_me );//Seba
                    if( MODO_NEGOCIO == SERVICIOS ) {
                        INFORME_DIFERENCIAS_SUBMEDIOS_CONTINUO();
                    }
                }
                if( !IMPRIMIR_Y_EN_Z && EMITIR_INFORME_DIFERENCIAS_SUBME
                 && DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ && PEDIR_DIFERENCIAS ) {
                    /*                  if(!TKT_CONFIGURABLE){
                                            INFORME_DIFERENCIAS_SUBMEDIOS( datosCierre.cierre, datosCierre.tot,
                                                              SI, datosCierre.cierre_me, datosCierre.tot_me );
                                        }*/
                    if( MODO_NEGOCIO == RETAIL ) {
                        INFORME_DIFERENCIAS_SUBMEDIOS_RETAIL( 'Y' );
                    }
                }
            }
            _SET_ESTADO_CIERRE( 6 );
        }
        if( _ESTADO_CIERRE == 6 ) {
            if( MODO_NEGOCIO == SERVICIOS ) {
                //                for( h = 0;h < 9;h++ ) {
                //                    if( fabs( datosCierre.tot[h] - datosCierre.cierre[h] ) > 0.00001 ) {
                //                        IMPRIME_VALE_DIFERENCIA( h, NO, ( datosCierre.tot[h] - datosCierre.cierre[h] ), NO );
                //                        IMPRIME_VALE_DIFERENCIA( h, NO, ( datosCierre.tot[h] - datosCierre.cierre[h] ), SI ); // duplicado
                //                    }
                IMPRIME_VALE_DIFERENCIA_CONTINUO( &datosCierre, NO );
                IMPRIME_VALE_DIFERENCIA_CONTINUO( &datosCierre, SI );
            }
            _SET_ESTADO_CIERRE( 7 );
        }
        if( _ESTADO_CIERRE == 7 ) {
            SETEAR_TIEMPO( _INDEFINIDO );
            RETIRAR_TODOS_LOS_COMPROBANTES();
            SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );
            ACUMULAR_TIEMPOS( );
            GRABAR_EVENTO_YZ( 'Y' );
            GRABAR_CIERRE_CAJERO();
            CEREAR_TIEMPOS( );
            SET_MEMORY_INT( __ram_tmpo_ocio_aux, 0 );
			SET_MEMORY_INT( ____no_se_usa_002, NO );
            if( COPIAR_EVENTOS_EN_CIERRE ) {
                COPIAR_EVENTOS_ACTUAL();
                ABRIR_ARCHIVOS_EVENTOS();
            }
            ClearPanel( pPanel );
            ACTUALIZA_VENTAS2_ANTERIORES( NO );
            MENSAJE_SIN_SONIDO_STRING( S_CERRANDO_CAJA );
            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
                FIN_DOCUMENTO( NO, NO );
            }
        }
        SET_MEMORY_INT( __ram_fin_ticket, 0 );
        if( /*DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ &&*/ PEDIR_DIFERENCIAS ) {
            FREE_ARRAY_DIFERENCIAS();
        }
        STACK_INIT_DATOS();
    }
    CLOSE_RECUPERO( _ptr, _handle );
    if( MODO_NEGOCIO == SERVICIOS && salir != 1 ) {
        setData( ACTION_DOPRINT, NULL, 0 );
    }
    VACIAR_SUBMEDIO();
    BORRAR_ARCHIVO_REC( ARCH_REC_CIERRE );
    BORRAR_ARCHIVO_REC( ARCH_REC_DIFERENCIAS );
    BORRAR_ARCHIVO_REC(ARCH_REC_INFO_IMPORT);
	BORRAR_ARCHIVO_REC(ARCH_REC_TARJETAS_FISC);	
    BORRAR_MENSAJE( );
    return cierre_confirmado;
}
/*****************************************************************************/
void CIERRE_CONFECCIONA_PANTALLA( int cierre )
/*****************************************************************************/
{
    int h;
    for( h = 0;h < 9;h++ ) {
        if( medios[h].nro ) {
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteSAction( A_AGREGAR_MEDIO_CIERRE, medios[h].nombre );
            #endif
            MOSTRAR_CADENA( 24 - ancho_panel / 2, 6 + h, medios[h].nombre );
        }
    }
}
/*****************************************************************************/
void CIERRE_SUBMEDIOS_CONFECCIONA_PANTALLA( int cierre )
/*****************************************************************************/
{
    int h;
    for( h = 0;h < 9;h++ ) {
        if( medios[h].nro ) {
            _ATRIBUTO = 52;
            MOSTRAR_CADENA( 7, 6 + h, medios[h].nombre );
            MOSTRAR_ENTERO( 5, 6 + h, "9", h + 1 );
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteSAction( A_AGREGAR_MEDIO_CIERRE, medios[h].nombre );
            #endif
        }
    }
}
/*****************************************************************************/
void INGRESA_IMPORTE_CIERRE( int h, double tot, double *cierre, double *dif, double tot_me,
                             double *cierre_me, double *dif_me )
/*****************************************************************************/
{
    int salir = 0;
    double importe, total = 0;
    char stringData[17],stringData2[17],stringData3[40];

	memset( stringData, 0, sizeof( stringData ) );
    memset( stringData2, 0, sizeof( stringData2 ) );
	memset( stringData3, 0, sizeof( stringData3 ) );
    while( !salir ) {
        importe = PEDIR_GRAN_FLOTANTE( 29 - ancho_panel / 2, 20, ENTEROS + 1, DECIMALES );
        switch( LASTKEY() ) {
            case 13:
                salir = 1;
                total += importe;
                break;
            case '+':
                total += importe;
                FTOA( importe, stringData, 8, 2, 0, 0 );
                sprintf( stringData3, stringData );
                ExecuteSSAction( A_MOSTRAR_SUMA_EN_CIERRE, h, medios[h].nombre, stringData, NULL, " " );
                break;
            case '-':
                total = 0;
                break;
        }
        _ATRIBUTO = 52;
        MOSTRAR_FLOTANTE( 36 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 0, 1, total );
        FTOA( total, stringData, 8, 2, 1, 1 );
        ExecuteSSAction( A_MOSTRAR_IMPO_EN_CIERRE, h, medios[h].nombre, stringData, "", " " );
    }
    /*------------------ muestra datos totales ----------------*/
    if( medios[h].moneda_extranjera ) {
        #ifdef _DEBUG
        double tot_real;
        #endif
        if( ( ( *dif_me = total - tot_me ) < 0.009 ) && ( *dif_me > -0.009 ) ) {
            *dif_me = 0;
            *cierre_me = total;
            *dif = 0;
            *cierre = tot;
        }
        else {
            *cierre_me = total;
            *dif = PAGOS_ME_A_ML( *dif_me, h );
            *cierre = PAGOS_ME_A_ML( total, h );
        }
        #ifdef _DEBUG
        //Que no se pierda mas de un centavo
        tot_real = ROUND( total * medios[h].cotizacion - *dif_me * medios[h].cotizacion, 10,
                          DECIMALES );
        assert( tot_real - tot <= 0.01 );
        #endif
    }
    else {
        *dif = total - tot;
        *cierre = total;
    }
    if( MOSTRAR_DIFERENCIAS ) {
        MOSTRAR_FLOTANTE( 48 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 1, 1,
                          ( medios[h].moneda_extranjera ) ? *dif_me : *dif );
        #if defined(INVEL_W) || defined(INVEL_L)
        sprintf( stringData3, stringData );
        FTOA( ( total - tot ), stringData2, 8, 2, 1, 1 );
        //strcat(stringData3, "$" );
        //strcat(stringData3, stringData2);
        ExecuteSSAction( A_MOSTRAR_IMPO_EN_CIERRE, h, medios[h].nombre, stringData, stringData2, " " );
        #endif
    }
}
/*****************************************************************************/
void INGRESA_IMPORTE_DISC_CIERRE( int h, double tot, double *cierre, double *dif, double tot_me,
                                  double *cierre_me, double *dif_me )
/*****************************************************************************/
{
    int salir = 0, pedir_flotante = 0;
	char restar=0;
    double importe=0.00, total_orig = 0.00, total = 0.00;

    #if defined(INVEL_W) || defined(INVEL_L)
    char stringData[17],stringData2[17]/*, stringData3[35]*/;

	memset( stringData, 0, sizeof( stringData ) );
    memset( stringData2, 0, sizeof( stringData2 ) );
    #endif
    MOSTRAR_CADENA( 7, 6 + h, medios[h].nombre );
    ExecuteSSAction( A_RESALTAR_MEDIO_CIERRE, h, medios[h].nombre, "", NULL, " " );

	if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ ) {
		if( medios[h].mutual || medios[h].ticket_compra || medios[h].tarjeta ) {
			total = DISCRIMINAR_CIERRE( &medios[h], RENDICION_MEDIO( h, 0 ),
										cant_mem_rendicion_medio[h] );
		} else {
			pedir_flotante = 1;
		}
	} else
		pedir_flotante = 1;

    if( pedir_flotante ) {
        total_orig = ( medios[h].moneda_extranjera ) ? *cierre_me : *cierre;
		total = total_orig;
		
        while( !salir ) {
			FTOA( total, stringData, 8, 2, 0, 0 );
            ExecuteSSAction( A_MOSTRAR_SUMA_EN_CIERRE, h, medios[h].nombre, stringData,NULL, " " );
            importe = PEDIR_GRAN_FLOTANTE_CON_VALOR( 29 - ancho_panel / 2,				20, ENTEROS + 1, DECIMALES, 0.0 );
			switch( LASTKEY() ) {
				case 27:
					CALCULAR_IMPORTES_ARQUEO( 27,&restar, total_orig, &total, &importe);
					salir = 1;
					break;
				case 13:
					salir = 1;
					CALCULAR_IMPORTES_ARQUEO( 13,&restar, total_orig, &total, &importe);
					break;
				case '+':
					CALCULAR_IMPORTES_ARQUEO( '+',&restar, total_orig, &total, &importe);
					_ATRIBUTO = 52;
                    MOSTRAR_FLOTANTE( 16 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 0, 1,total );
                    if( MOSTRAR_DIFERENCIAS ) {
                        MOSTRAR_FLOTANTE( 27 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 1, 1,total
							- ( ( medios[h].moneda_extranjera ) ? tot_me : tot ) );
                    }
                    FTOA( total, stringData, 8, 2, 0, 0 );
                    ExecuteSSAction( A_MOSTRAR_SUMA_EN_CIERRE, h, medios[h].nombre, stringData,
                                     NULL, " " );
					break;
				case '-':
					CALCULAR_IMPORTES_ARQUEO( '-',&restar, total_orig, &total, &importe);
                    _ATRIBUTO = 52;
                    MOSTRAR_FLOTANTE( 16 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 0, 1,
                                      total );
                    if( MOSTRAR_DIFERENCIAS ) {
                        MOSTRAR_FLOTANTE( 27 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 1, 1,
                                          total
                                        - ( ( medios[h].moneda_extranjera ) ? tot_me : tot ) );
                    }
                    FTOA( total, stringData, 8, 2, 0, 0 );
                    ExecuteSSAction( A_MOSTRAR_SUMA_EN_CIERRE, h, medios[h].nombre, stringData,
                                     NULL, " " );
                    break;
				/*original -->case '-':
					{
                   	restar=	SI;
					total += total_orig;
					restar = 1;
					_ATRIBUTO = 52;
                    MOSTRAR_FLOTANTE( 16 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 0, 1,
                                      total );
                    if( MOSTRAR_DIFERENCIAS ) {
                        MOSTRAR_FLOTANTE( 27 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 1, 1,
                                          total
                                        - ( ( medios[h].moneda_extranjera ) ? tot_me : tot ) );
					}
					}
                    break;*/
            }
            PAGOS_MUESTRA_IMPORTE_TEMPORAL( total, NO );
        }
    }
    MOSTRAR_FLOTANTE( 16 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 0, 1, total );
    MOSTRAR_CADENA( 7, 6 + h, medios[h].nombre );
    //*------------------ muestra datos totales ----------------*/
    if( medios[h].moneda_extranjera ) {
        #ifdef _DEBUG
        double tot_real;
        #endif
        //Calcula diferencia entre lo ingresado y lo registrado en ME
        if( ( ( *dif_me = total - tot_me ) < 0.009 ) && ( *dif_me > -0.009 ) ) {
            *dif_me = 0;
            *cierre_me = total;
            *dif = 0;
            *cierre = tot;
        }
        else {
            *cierre_me = total;
            *dif = PAGOS_ME_A_ML( *dif_me, h );
            *cierre = PAGOS_ME_A_ML( total, h );
        }
        #ifdef _DEBUG
        //Que no se pierda mas de un centavo
        tot_real = ROUND( total * medios[h].cotizacion - *dif_me * medios[h].cotizacion, 10,
                          DECIMALES );
        tot_real = total;
        assert( tot_real - tot <= 0.01 );
        #endif
    }
    else {
        *dif = total - tot;
        *cierre = total;
    }
    // Se agrego un digito mas para poder mostrar la cantidad maxima de digitos
    // cuando se declara el pago con dolares.
    FTOA( *cierre, stringData, ENTEROS + 3, DECIMALES, 1, 1 );////
    //sprintf( stringData3, stringData );
    if( MOSTRAR_DIFERENCIAS ) {
        MOSTRAR_FLOTANTE( 27 - ancho_panel / 2, 6 + h, ENTEROS + 1, DECIMALES, 1, 1,
                          ( medios[h].moneda_extranjera ) ? *dif_me : *dif );
        // Se agrego un digito mas para poder mostrar la cantidad maxima de digitos
        // cuando se declara el pago con dolares.
        FTOA( ( *cierre - tot ), stringData2, ENTEROS + 3, DECIMALES, 1, 1 );
        //strcat(stringData3, "$" );
        //strcat(stringData3, stringData2);
    }
    ExecuteSSAction( A_MOSTRAR_IMPO_EN_CIERRE, h, medios[h].nombre, stringData, stringData2, " " );
}
/*****************************************************************************/
double PEDIR_CIERRE_LOTE_TARJ( char *desc, int *lote, int *terminal )
/*****************************************************************************/
{
    char cad_aux[255],title[200];
    double importe = 0;
    int continuar = 1;
    _snprintf(cad_aux, sizeof(cad_aux)-1, "Numero de Terminal:~Cierre de Lote:~Importe:" );
    _snprintf(title, sizeof(title)-1, "Cierre de Lote: %s", desc );
    ExecuteSSAction( A_PEDIR, _CONCATENADO, title, cad_aux, NULL, " " );
    ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL, " " );
    continuar = 1;
    while( continuar ) {
        *terminal = ( int )PEDIR_ENTERO( 0, 1, 4, NO );
        //Se obtiene la cadena ingresada para luego verificar que se haya ingresado
        //al menos un caracter.
        OBTENER_INPUT_CADENA( cad_aux );
        if( 27 == LASTKEY() ) {
            ExecuteAction( A_FIN_PEDIR );
            return -1;
        }
        LTRIM( cad_aux );
        if( *terminal < 0 || strlen( cad_aux ) < 1 ) {
            continuar = 1;
            MENSAJE( "DATO INVALIDO" );
        }
        else {
            continuar = 0;
        }
        BORRAR_MENSAJE();
    }
    continuar = 1;
    ExecuteSSAction( A_PEDIR_DATO, 1, " ", " ", NULL, " " );
    while( continuar ) {
        *lote = ( int )PEDIR_ENTERO( 0, 1, 4, NO );
        OBTENER_INPUT_CADENA( cad_aux );
        if( 27 == LASTKEY() ) {
            ExecuteAction( A_FIN_PEDIR );
            return -1;
        }
        LTRIM( cad_aux );
        if( *lote < 1 || strlen( cad_aux ) < 1 ) {
            continuar = 1;
            MENSAJE( "DATO INVALIDO" );
        }
        else {
            continuar = 0;
        }
        BORRAR_MENSAJE();
    }
    continuar = 1;
    while( continuar ) {
        ExecuteSSAction( A_PEDIR_DATO, 2, " ", " ", NULL, " " );
        importe = PEDIR_FLOTANTE( 0, 0, ENTEROS, DECIMALES );
        OBTENER_INPUT_CADENA( cad_aux );
        if( 27 == LASTKEY() ) {
            ExecuteAction( A_FIN_PEDIR );
            return -1;
        }
        LTRIM( cad_aux );
        if( cad_aux[0] < '0' || cad_aux[0] > '9' ) {
            continuar = 1;
            MENSAJE( "DATO INVALIDO" );
        }
        else {
            continuar = 0;
        }
        BORRAR_MENSAJE();
    }
    ExecuteAction( A_FIN_PEDIR );
    return ( importe <= 0 ) ? 0 : importe;
}
/*****************************************************************************/
double PEDIR_IMPORTE_CIERRE( int *reproceso, double monto, double monto_retiro, int i )
/*****************************************************************************/
{
    int salir = 0;
	char restar = 0;
    double importe, total = monto_retiro;
	double retiro = monto_retiro;
    #if defined(INVEL_W) || defined(INVEL_L)
    char stringData[17];
    #endif

	*reproceso = 0;

    while( !salir ) {
        
		FTOA( total, stringData, 8, 2, 0, 0 );
                ExecuteSSAction( A_MOSTRAR_SUMA_SUB_CIERR, i, "", stringData, NULL, " " );

		importe = PEDIR_GRAN_FLOTANTE_CON_VALOR( 29 - ancho_panel / 2,
			20, ENTEROS + 1, DECIMALES, 0 );

        switch( LASTKEY() ) {
            case 27:
                *reproceso = SALIR;
				CALCULAR_IMPORTES_ARQUEO( 27,&restar, monto_retiro, &total, &importe);
                salir = 1;
			break;
			case 13:
				salir = 1;
				CALCULAR_IMPORTES_ARQUEO( 13,&restar, monto_retiro, &total, &importe);
				break;
            case '+':
				CALCULAR_IMPORTES_ARQUEO( '+',&restar, monto_retiro, &total, &importe);
                _ATRIBUTO = 52;
                MOSTRAR_FLOTANTE( 55, 6 + i, ENTEROS + 1, DECIMALES, 0, 1, total );
                if( MOSTRAR_DIFERENCIAS ) {
                    MOSTRAR_FLOTANTE( 65, 6 + i, ENTEROS + 1, DECIMALES, 0, 1, total - monto );
                }
                #if defined(INVEL_W) || defined(INVEL_L)
                FTOA( total, stringData, 8, 2, 0, 0 );
                ExecuteSSAction( A_MOSTRAR_SUMA_SUB_CIERR, i, "", stringData, NULL, " " );
                #endif
                break;
            case '-'://-62
                CALCULAR_IMPORTES_ARQUEO( '-' ,&restar ,monto_retiro , &total , &importe );
				_ATRIBUTO = 52;
                MOSTRAR_FLOTANTE( 55, 6 + i, ENTEROS + 1, DECIMALES, 0, 1, total );
				FTOA( total, stringData, 8, 2, 0, 0 );
				ExecuteSSAction( A_MOSTRAR_SUMA_SUB_CIERR, i, "", stringData, NULL, " " );
                if( MOSTRAR_DIFERENCIAS ) {
                    MOSTRAR_FLOTANTE( 65, 6 + i, ENTEROS + 1, DECIMALES, 0, 1, total - monto );
					
                }
                break;
            case -80:
                // flecha abajo
                if( !SELEC_SUBMEDIOS_CIERRE_POR_NUMER ) {
                    *reproceso = ADELANTE;
                    salir = 1;
                }
                break;
            case -72:
                // flecha arriba
                if( !SELEC_SUBMEDIOS_CIERRE_POR_NUMER ) {
                    *reproceso = ATRAS;
                    salir = 1;
                }
                break;
            case -73:
                // -------- PAGINA ARRIBA ---------
                if( !SELEC_SUBMEDIOS_CIERRE_POR_NUMER ) {
                    *reproceso = P_ATRAS;
                    salir = 1;
                }
                break;
            case -81:
                // -------- PAGINA ABAJO -----------
                if( !SELEC_SUBMEDIOS_CIERRE_POR_NUMER ) {
                    *reproceso = P_ADELANTE;
                    salir = 1;
                }
                break;
           
        }
        PAGOS_MUESTRA_IMPORTE_TEMPORAL( total, NO );
    }
    return total;
}
/*****************************************************************************/
void MOSTRAR_ITEM_RENDICION( struct rendicion_en_memoria *mem, int pant_pos )
/*****************************************************************************/
{
    _ATRIBUTO = 52;
    MOSTRAR_CADENA( 41, 6 + pant_pos, "           " );
    MOSTRAR_CADENA( 41, 6 + pant_pos, mem->desc );
    MOSTRAR_FLOTANTE( 55, 6 + pant_pos, ENTEROS + 1, DECIMALES, 0, 1, mem->monto_retiro );
    if( MOSTRAR_DIFERENCIAS ) {
        MOSTRAR_FLOTANTE( 65, 6 + pant_pos, ENTEROS + 1, DECIMALES, 0, 1,
                          mem->monto_retiro - mem->monto );
    }
    if( SELEC_SUBMEDIOS_CIERRE_POR_NUMER ) {
        MOSTRAR_ENTERO( 39, 6 + pant_pos, "9", pant_pos + 1 );
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    MOSTRAR_ITEM_RENDICION_EN_JAVA( pant_pos, mem->desc, mem->monto_retiro,
                                    mem->monto_retiro - mem->monto );
    #endif
}
/*****************************************************************************/
void SELECCIONA_ITEM_RENDICION( char *nombre, struct rendicion_en_memoria *mem, int pant_pos )
/*****************************************************************************/
{
    char desc[100];
    #ifdef INVEL_W
    MOSTRAR_ITEM_RENDICION( mem, pant_pos );
    #endif
    // RESALTAR LA SELECCION
    _ATRIBUTO = 120;
    MOSTRAR_CADENA( 41, 6 + pant_pos, mem->desc );
    //------------------------------------
    _ATRIBUTO = 7;
    _snprintf(desc, sizeof(desc)-1, "%s: %s     ", nombre, mem->desc );
    MOSTRAR_CADENA( 30, 18, desc );
}
/*****************************************************************************/
void DISCRIMINAR_CIERRE_MENSAJE_PAGINA( int atras, int adelante )
/*****************************************************************************/
{
    int at = _ATRIBUTO;
    _ATRIBUTO = 52;
    if( atras ) {
        MOSTRAR_CADENA( 60, 15, ST( S_____PAG__ANTERIOR ) );
    }
    else {
        MOSTRAR_CADENA( 60, 15, "?????????" );
    }
    if( adelante ) {
        MOSTRAR_CADENA( 42, 15, ST( S_PAG__SIGUIENTE____ ) );
    }
    else {
        MOSTRAR_CADENA( 42, 15, "?????????" );
    }
    _ATRIBUTO = at;
    #if defined(INVEL_W) || defined(INVEL_L)
    if( atras ) {
        ExecuteSSAction( A_MENSAJE_PAGINA_CIERRE, 0, "", "", NULL, " " );
    }
    if( adelante ) {
        ExecuteSSAction( A_MENSAJE_PAGINA_CIERRE, 1, "", "", NULL, " " );
    }
    #endif
}
/*****************************************************************************/
double ACUMULAR_CIERRE_LOTE( int medio, int sub_medio )
/*****************************************************************************/
{
    double importe = 0;
   // SELECT( AREA_CIE_LOTE );
   // SET_ORDER( 1 );
   // GET_FIRST();

	SELECT_TABLE( T_CIERRE_LOTE, tipo_t_cierre_lote );
	SET_ORDER2( "MEDIO, SUB_MEDIO, TERMINA, LOTE" );
	RUN_QUERY(NO);
    while( !dbEOF() ) {
        if( ( event_cie_lote->medio == medio ) && ( event_cie_lote->sub_medio == sub_medio ) ) {
            importe += event_cie_lote->imp;
        }
        SKIP2( 1 );
    }
    return importe;
}
/*****************************************************************************/
int GRABAR_CIERRE_DE_LOTE( double importe, int terminal, int lote, int medio, int sub_medio,
                           long *registro )
/*****************************************************************************/
{
    int aux,seguir,ok,encontrado;
    char k;
    ind1_cie_lote ind_cie_lote;
    //SELECT( AREA_CIE_LOTE );
    ind_cie_lote.medio = (short)medio;
    ind_cie_lote.sub_medio = (short)sub_medio;
    ind_cie_lote.terminal = (short)terminal;
    ind_cie_lote.lote = (short)lote;
    //SET_ORDER( 1 );
   // GET_EQUAL( ( char* )&ind_cie_lote );
	SELECT_TABLE( T_CIERRE_LOTE, tipo_t_cierre_lote );
	SET_ORDER2( "MEDIO, SUB_MEDIO, TERMINA, LOTE" );
	SET_WHERE("MEDIO  = '%d' and SUB_MEDIO = '%d' and TERMINA = '%d' and LOTE = '%d'", ind_cie_lote.medio, ind_cie_lote.sub_medio, ind_cie_lote.terminal, ind_cie_lote.lote );
	RUN_QUERY(NO);
    encontrado = FOUND2();
    event_cie_lote->imp = importe;
    event_cie_lote->terminal = terminal;
    event_cie_lote->lote = lote;
    event_cie_lote->medio = medio;
    event_cie_lote->sub_medio = sub_medio;
    event_cie_lote->caja_z = event_ticket.caja_z;
    event_cie_lote->id_evento = RAM_ID_EVENTO;
    event_cie_lote->sucursal = config.sucursal;
    event_cie_lote->fecha = GET_FECHA_DOS();
    if( encontrado ) {
        if( importe > 0 ) {
            MENSAJE( "CIERRE DE LOTE EXISTENTE, <S> PARA MODIFICAR, OTRA TECLA CONTINUA" );
        }
        else {
            MENSAJE( "CIERRE DE LOTE EXISTENTE, <S> PARA ELIMINAR, OTRA TECLA CONTINUA" );
        }
        aux = TECLADO_ALFABETICO;
        TECLADO_ALFABETICO = 1;
        seguir = 1;
        while( seguir ) {
            k = GETCH();
            switch( k ) {
                case 'S':
                case 's':
                    seguir = 0;
                    ok = 1;
                    if( importe > 0 ) {
                        UPDATE2();
                    }
                    else {
                        DELETE2();
                    }
                    break;
                default:
                    ok = 0;
                    seguir = 0;
                    break;
            }
        }
        TECLADO_ALFABETICO = aux;
        BORRAR_MENSAJE();
    }
    else {
        if( importe > 0 ) {
            INSERT();
        }
        ok = 1;
    }
    *registro = GET_POSITION_REG();
    return ok;
}
/*****************************************************************************/
void VACIAR_CIERRE_DE_LOTE( void )
/*****************************************************************************/
{
    //int selected;
	int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SELECT_TABLE( T_CIERRE_LOTE, tipo_t_cierre_lote );
    //SELECT( AREA_CIE_LOTE );
    ZAP2();
    //SELECT( selected );
	SELECT_TABLE( area_ant, tipo_ant );
}
/*****************************************************************************/
void ELIM_MEDIO_CIERRE_DE_LOTE( int medio )
/*****************************************************************************/
{
		int  area_ant, tipo_ant;

	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	SELECT_TABLE( T_CIERRE_LOTE, tipo_t_cierre_lote );
    SET_ORDER2( "MEDIO, SUB_MEDIO, TERMINA, LOTE" );
	SET_WHERE("MEDIO  >= '%d'", medio );
	RUN_QUERY(NO);
    //no uso el indice completo porque solo estoy interesado en el medio.
//    GET_GREATER_E( ( char* )&medio );
    while( event_cie_lote->medio == medio && !( dbBOF() || dbEOF() ) ) {
        DELETE2();
    }
    //SELECT( selected );
	SELECT_TABLE( area_ant, tipo_ant );
}
/*****************************************************************************/
double DISCRIMINAR_CIERRE( struct _medios *medio, struct rendicion_en_memoria *mem, int cant )
/*****************************************************************************/
{
    int i = 0, j = 0, p = 0;
    double total = 0, total_mem = 0;
    //float pru;
    int proximo = 0, proximo_ant = -1, paginas = 0, punt_pag = 0;
    int salir = 0, letra, j_temp, dejar_al_principio, pedir_importe;
    char stringData[20],stringData3[40],stringData4[35];
    struct rendicion_en_memoria mem1 = {0,"",0,0,0};
    char nombre[10];

	memset( stringData, 0, sizeof( stringData ) );
	memset( stringData3, 0, sizeof( stringData3 ) );
	memset( stringData4, 0, sizeof( stringData4 ) );

    if( MODO_NEGOCIO != SERVICIOS ) {
        // Se borra lo cargado anteriormente
        /*for( i = 0;i < cant;i++ ) {
           mem[i].monto_retiro = 0;
			;
        }*/
    }
    /*controla si tiene que recuperar*/
    if( CANTIDAD_REGISTROS_REC( ARCH_REC_DIFERENCIAS, sizeof( diferencias ) ) == 0 ) {
        /*guarda el vector en blanco*/
        memset( diferencias, 0, sizeof( diferencias ) );
        GUARDAR_ARCHIVO_REC( ARCH_REC_DIFERENCIAS, ( char* )diferencias, sizeof( diferencias ) );
    }
    else {
        RECUPERAR_ARCHIVO_REC( ARCH_REC_DIFERENCIAS, ( char* )diferencias, sizeof( diferencias ),
                               0 );
    }
    //INIT_VEC_DIFERENCIAS( medio->nro );
    //if( medio->tarjeta ) {
    //ELIM_MEDIO_CIERRE_DE_LOTE( medio->nro );
    //}
    // Initial Page.
    ExecuteAction( A_CEREAR_ITEM_RE_CIERRE );
    strcpy( nombre, medio->nombre );
    for( i = 0, j = 0;j < cant && i < 9;j++, i++ ) {
	 if( mem[j].codigo != 0 ){
        MOSTRAR_ITEM_RENDICION( &mem[j], i );
        if( strlen( diferencias[medio->nro][j].strDif ) > 0 ) {
            memset( stringData4, 0, sizeof( stringData4 ) );
            //          FTOA( ( mem[j].monto_retiro - mem[j].monto ), stringData4, ENTEROS + 1, DECIMALES, 1, 1 ); // Cargo en strimgData4 la diferencia entre lo declarado y lo informado
            if( MOSTRAR_DIFERENCIAS ) {
				FTOA( ( diferencias[medio->nro][j].importe - mem[j].monto ), stringData4, ENTEROS + 2,
					  DECIMALES, 1, 1 ); // Cargo en strimgData4 la diferencia entre lo declarado y lo informado
			}
            ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc,
                             diferencias[medio->nro][j].strDif, stringData4, " ");
        }
	 }
    }
    if( cant > 9 ) {
        DISCRIMINAR_CIERRE_MENSAJE_PAGINA( NO, SI );
        paginas = ( int )ceil( ( ( double )cant / 9 ) );
        punt_pag = 1;
    }
    i = j = salir = 0;
    while( !salir ) {
		switch( proximo ) {
            case SALIR:
                salir = 1;
                continue;
            case P_ATRAS:
                dejar_al_principio = ( j >= 9 ) ? 0 : 1;
                for( ;i != 0 && j != 0;i--, j-- ) {
                    ;
                }
                // VUELVE PANTALLA
                for( p = 0;p < 9;p++ ) {
                    MOSTRAR_CADENA( 39, 6 + p, "" );
                }
                j -= 9;
                if( j < 0 ) {
                    j = 0;
                }
                ExecuteAction( A_CEREAR_ITEM_RE_CIERRE );
                // Llenar la pantalla
                for( i = 0;i < 9 && j < cant &&( mem[j].codigo != 0 );i++, j++ ) {
                    MOSTRAR_ITEM_RENDICION( &mem[j], i );
                    if( strlen( diferencias[medio->nro][j].strDif ) > 0 ) {
						if( MOSTRAR_DIFERENCIAS ) {
							FTOA( ( diferencias[medio->nro][j].importe - mem[j].monto ), stringData4,
								  ENTEROS + 1, DECIMALES, 1, 1 ); // Cargo en strimgData4 la diferencia entre lo declarado y lo informado
						}
                        ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc,
                                         diferencias[medio->nro][j].strDif, stringData4, " " );
                        //                        ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc,
                        //                                         diferencias[medio->nro][j].strDif, NULL );
                    }
                }
                if( dejar_al_principio ) {
                    i = 0;
                    j = 0;
                }
                else {
                    i = 8;
                    j--;
                }
                break;
            case P_ADELANTE:
                if( j + ( 9 - ( i + 1 ) ) >= cant ) {
                    for( ;j < cant - 1;i++, j++ ) {
                        ;
                    }
                }
                else {
                    for( p = 0;p < 9;p++ ) {
                        MOSTRAR_CADENA( 39, 6 + p, "" );
                    }
                    for( ;i < 9;i++, j++ ) {
                        ;
                    }   // Lo deja listo para el proximo
                    j_temp = j;
                    ExecuteAction( A_CEREAR_ITEM_RE_CIERRE );
                    // Llenar
                    for( i = 0;j < cant && i < 9&&( mem[j].codigo != 0 );j++, i++ ) {
                        MOSTRAR_ITEM_RENDICION( &mem[j], i );
                        if( strlen( diferencias[medio->nro][j].strDif ) > 0 ) {
							if( MOSTRAR_DIFERENCIAS ) {
								FTOA( ( diferencias[medio->nro][j].importe - mem[j].monto ),
									  stringData4, ENTEROS + 1, DECIMALES, 1, 1 ); // Cargo en strimgData4 la diferencia entre lo declarado y lo informado
							}
                            ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc,
                                             diferencias[medio->nro][j].strDif, stringData4, " " );
                        }
                    }
                    i = 0;
                    j = j_temp;
                }
                //MOSTRAR_ITEM_RENDICION( &"  PROXIMA PAGINA >> ", 9 );
                break;
            case ATRAS:
                if( i == 0 ) {
                    if( j > 0 ) {
                        // Vuelve Pantalla
                        for( p = 0;p < 9;p++ ) {
                            MOSTRAR_CADENA( 39, 6 + p, "" );
                        }
                        j -= 9;
                        ExecuteAction( A_CEREAR_ITEM_RE_CIERRE );
                        // Llenar la pantalla
                        for( i = 0;i < 8 && (mem[j].codigo != 0 );i++, j++ ) {
                            MOSTRAR_ITEM_RENDICION( &mem[j], i );
                            if( strlen( diferencias[medio->nro][j].strDif ) > 0 ) {
                                ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc,
                                                 diferencias[medio->nro][j].strDif, NULL, " " );
                            }
                        }
                        _snprintf(mem1.desc, sizeof(mem1.desc)-1, "%s", "  << REGRESAR " );
                        MOSTRAR_ITEM_RENDICION( &mem1, 9 );
                    }
                }
                else {
                    j--;
                    i--;
                }
                break;
            case ADELANTE:
				/* se controla hasta cant - 1 sino se pasa del vector */
                if( j < cant - 1 ) {
                    i++;
                    if( i == 9 ) {
                        int j_temp = j;
                        for( p = 0;p <= 9;p++ ) {
                            MOSTRAR_CADENA( 39, 6 + p, "" );
                        }
                        //MOSTRAR_ITEM_RENDICION( &"  PROXIMA PAGINA >> ", 9 );
                        ExecuteAction( A_CEREAR_ITEM_RE_CIERRE );
                        // Llenar
                        for( j += 1, i = 0;j < cant && i < 9 && ( mem[j].codigo != 0 );j++, i++ ) {
                            MOSTRAR_ITEM_RENDICION( &mem[j], i );
                            if( strlen( diferencias[medio->nro][j].strDif ) > 0 ) {
                                ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc,
                                                 diferencias[medio->nro][j].strDif, NULL, " " );
                            }
                        }
                        i = 0;
                        j = j_temp;
                    }
                    if( j + 1 == cant ) {
                        i--;
                    }
                    else {
                        j++;
                    }
                }
                break;
        }
        if( paginas != 0 ) {
            if( punt_pag < paginas ) {
                _snprintf(mem1.desc, sizeof(mem1.desc)-1, "%s", "SIGUIENTE >>" );
                MOSTRAR_ITEM_RENDICION( &mem1, 9 );
            }
            else {
                _snprintf(mem1.desc, sizeof(mem1.desc)-1, "%s", "<< VOLVER" );
                MOSTRAR_ITEM_RENDICION( &mem1, 9 );
            }
        }
        DISCRIMINAR_CIERRE_MENSAJE_PAGINA( ( j > 8 ),
			( j + ( 9 - i + 1 ) < cant ) ? 1 : 0 );

		if( SELEC_SUBMEDIOS_CIERRE_POR_NUMER ) {
            int j_ant = j;
            int i_ant = i;
            int k;
            MENSAJE_SIN_SONIDO( ST(S_SELECCIONE_SUB_MEDIO),NO );
            while ( ( letra = GETCH() ) == -999 );
            BORRAR_MENSAJE();
            pedir_importe = 0;
            switch( letra ) {
                case 27:
                    proximo = SALIR;
                    break;
                    //   case -73:              // -------- PAGINA ARRIBA ---------
                    //proximo = P_ATRAS;
                    //break;
                    //case -81:              // -------- PAGINA ABAJO -----------
                    //proximo = P_ADELANTE;
                    //break;
                case 48:
                    // -------- PAGINA ABAJO -----------
                    if( punt_pag < paginas ) {
                        proximo = P_ADELANTE;
                        punt_pag++;
                    }
                    else {
                        proximo = P_ATRAS;
                        punt_pag = 1;
						//punt_pag--;
                        j = 0;
                    }
                    break;
                default:
                    k = ( letra - 48 );
					if( k > 0 && k < 10 ) {
                        k--;
                        if( k > i ) {
                            for( ;k != i;i++, j++ ) {
                                ;
                            }
                        }
                        else {
                            for( ;k != i;i--, j-- ) {
                                ;
                            }
                        }
                    }
                    else {
                        i = -1;
                    }
                    if( j >= cant || i < 0 || i > 9 ) {
                        i = i_ant;
                        j = j_ant;
                    }else {
						if(mem[j].codigo!=0 )
							pedir_importe = 1;
						else{
							pedir_importe = 0;
						}
                    }
                    break;
            };
		}
        else {
			if(mem[j].codigo!=0 )
				pedir_importe = 1;
			else{
				pedir_importe = 0;
				proximo = P_ATRAS;
			}
        }
        if( pedir_importe ) {
			/**/{
            SELECCIONA_ITEM_RENDICION( nombre, &mem[j], i );
            //LOTE TARJETA
			if( MODO_NEGOCIO != SERVICIOS ) {
                #if defined(INVEL_W) || defined(INVEL_L)
                ExecuteSSAction( A_RESALTAR_SUBMEDIO_CIER, i, mem[j].desc, "", NULL, " " );
                #endif
				 
                total = PEDIR_IMPORTE_CIERRE( &proximo, mem[j].monto,
					((mem[j].monto_retiro> 0.0 )?mem[j].monto_retiro:diferencias[medio->nro][j].importe)/* original mente estaba  mem[j].monto_retiro pero no tiene importe alguno todavia*/,
					i );
                if( medio->mutual && MUTUAL_MONEDA_EXTRANJERA( mem[j].codigo ) ) {
                    total = MUTUALES_ME_A_ML( total, mem[j].codigo );
                }
            }
			else {
                if( medio->tarjeta ) {
                    char seguir,k;
                    int aux,lote,terminal;
                    double importe = ACUMULAR_CIERRE_LOTE( medio->nro, j + 1 ), imp;
                    long registro = 0;
                    seguir = 1;
                    while( seguir ) {
                        imp = PEDIR_CIERRE_LOTE_TARJ( mem[j].desc, &lote, &terminal );
                        if( ( imp >= 0 ) && ( lote > 0 ) && ( terminal >= 0 )
                         && GRABAR_CIERRE_DE_LOTE( imp, terminal, lote, medio->nro, j + 1,
                                                   &registro ) ) {
                            importe = ACUMULAR_CIERRE_LOTE( medio->nro, j + 1 );
                        }
                        //Se verifica que no se reporte en un submedio de tarjeta,
                        //mas de lo que se cobro con ese submedio.
                        //Se elimina el control Segun inc 24337
                        /*if( importe > mem[j].monto ) {
                            MENSAJE_STRING( S_SE_EXCEDIO_EL_MONTO_REGISTRADO );
                            if( registro > 0 ) {
                                GET_DIRECT( registro );
                                _DELETE();
                                registro = 0;
                            }
                            importe = ACUMULAR_CIERRE_LOTE( medio->nro, j + 1 );
                        }*/
                        //memset( stringData, 0, 20 );
                        //memset( stringData4, 0, 35 );
                        FTOA( importe, stringData, ENTEROS + 2, DECIMALES, 1, 1 );//cargo el importe en stringData
						if( MOSTRAR_DIFERENCIAS ) {
							FTOA( (importe - mem[j].monto), stringData4, ENTEROS + 2, DECIMALES, 1, 1 );//cargo la diferencia en stringData4
						}
                        ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc, stringData,
                                         stringData4, " " );
                        MENSAJE( "PRESIONE <S> PARA SEGUIR, OTRA TECLA TERMINA" );
                        aux = TECLADO_ALFABETICO;
                        TECLADO_ALFABETICO = 1;
                        k = GETCH();
                        switch( k ) {
                            case 'S':
                            case 's':
                                seguir = 1;
                                break;
                            default:
                                seguir = 0;
                                break;
                        }
                        TECLADO_ALFABETICO = aux;
                        BORRAR_MENSAJE();
                    }
                    total = importe;
                }
                else {
                    #if defined(INVEL_W) || defined(INVEL_L)
                    ExecuteSSAction( A_RESALTAR_SUBMEDIO_CIER, i, mem[j].desc, "", NULL, " " );
                    #endif
					ExecuteSSAction( A_OCULTA_DIF_SUBMEDIO, i, NULL, NULL, NULL, " " );
                    total = PEDIR_IMPORTE_CIERRE( &proximo, mem[j].monto, mem[j].monto_retiro, i );
                }
			}
			}
			proximo_ant = -1;
            if( proximo == 0 ) {
				proximo_ant = proximo;
				mem[j].monto_retiro = total;
                proximo = ADELANTE;
            }

			if( proximo != SALIR ) {
				FTOA( mem[j].monto_retiro/*total*/, stringData, ENTEROS + 1, DECIMALES, 1, 1 );
				memset( stringData3, 0, sizeof( stringData3 ) );
				//memset( stringData2, 0, sizeof( stringData2 ) );
				FTOA( total, stringData3, ENTEROS + 1, DECIMALES, 1, 1 ); //sprintf( stringData3, stringData );
				/*if( MOSTRAR_DIFERENCIAS && ( MODO_NEGOCIO != SERVICIOS ) ) {
					strcat( stringData3, "  $" );
					FTOA( mem[j].monto_retiro - mem[j].monto, stringData2, 8, 2, 1, 1 );
					if ( mem[j].monto_retiro - mem[j].monto < 0)
						strcat( stringData3, " ");
					if ( (mem[j].monto_retiro - mem[j].monto) >= 1000 || (mem[j].monto_retiro - mem[j].monto) <= -1000){
						if ( (mem[j].monto_retiro - mem[j].monto) >= 1000000 || (mem[j].monto_retiro - mem[j].monto) <= -1000000){
							strcat( stringData3, "  ");
						}else strcat( stringData3, " ");
					}
					for ( d = 0 ; stringData2[d]==' ' ; d++)
						 strcat( stringData3, " " );
					strcat( stringData3, stringData2 );
				}*/
				//ATRAS = 2
				//ADELANTE = 3
				if( proximo != ATRAS 
						&& ( proximo != ADELANTE || ( proximo_ant == 0 && proximo == ADELANTE ) ) ) {
					sprintf( diferencias[medio->nro][j].strDif, stringData3 );
					diferencias[medio->nro][j].importe = total;
				}
				if( MOSTRAR_DIFERENCIAS ) {
					FTOA( ( mem[j].monto_retiro/*total*/ - mem[j].monto ), stringData4, ENTEROS + 2, DECIMALES, 1, 1 );
				}
			}
			if(i<0 ||j<0)
				i=0;
			if( mem[j].codigo != 0 )//existe o esta vacio??
				ExecuteSSAction( A_MOSTRAR_IMPO_SUB_CIERR, i, mem[j].desc, stringData, stringData4, " " );
            _ATRIBUTO = 7;
            MOSTRAR_CADENA( 30, 18, "                       " );
            _ATRIBUTO = 52;
        }
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteAction( A_CEREAR_ITEM_RE_CIERRE );
    #endif
    /*Actualiza el vector diferencias por si hay que recuperar*/
    ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DIFERENCIAS, ( char* )diferencias, sizeof( diferencias ), 0 );
    // Suma todo total_mem
    total_mem = 0;
    for( i = 0;i < cant;i++ ) {
        total_mem += diferencias[medio->nro][i].importe;//mem[i].monto_retiro;
        if( mem[i].monto_retiro ) {
            //Guardo lo declarado para cada Submedio
            GUARDAR_DECLARACION_SUBMEDIO( medio->nro, mem[i].codigo, mem[i].monto_retiro );
        }
    }

    if( SELEC_SUBMEDIOS_CIERRE_POR_NUMER ) {
        for( i = 0;i < 9;i++ ) {
            MOSTRAR_CADENA( 39, 6 + i, "" );
        }
    }
    DISCRIMINAR_CIERRE_MENSAJE_PAGINA( NO, NO );
    return total_mem;
}
/*****************************************************************************/
void CIERRE_RETIRO_DISCRIMINA_SUBMEDIOS( double *cierre )
/*****************************************************************************/
{
    int h,j;
    /*------------------ Imprime y actualiza acumuladores --------------------*/
    IMPRIME_CABECERA_RETIROS( 1, SI );
    for( h = 0;h < 9;h++ ) {
        if( medios[h].nro ) {
            IMPRIME_CUERPO_RETIROS( 0, medios[h].nombre, cierre[h], 0L, 0, 0.0, 0 );
            if( medios[h].mutual || medios[h].ticket_compra || medios[h].tarjeta ) {
                for( j = 0;j < cant_mem_rendicion_medio[h];j++ ) {
                    IMPRIME_DISC_RETIROS( RENDICION_MEDIO( h, j )->desc,
                                          RENDICION_MEDIO( h, j )->monto_retiro );
                }
            }
            _ADD_MEMORY_DOUBLE( __x_retiros_importe, h, cierre[h] );
            if( cierre[h] ) {
                _ADD_MEMORY_INT( __x_retiros_cantidad, h, 1 );
            }
            if( medios[h].efectivo ) {
                ADD_MEMORY_DOUBLE( __x_tot_dinero, -cierre[h] );
            }
        }
    }
    IMPRIME_PIE_RETIROS( NULL, 0, SI, NULL, SI );
}
/*****************************************************************************/
void CIERRE_RETIRO( double *cierre, double *cierre_me )
/*****************************************************************************/
{
    int h;
    char cadaux[20];
    /*------------------ Imprime y actualiza acumuladores --------------------*/
	if( !TKT_CONFIGURABLE ) {
		IMPRIME_CABECERA_RETIROS( 1, SI );
		for( h = 0;h < 9;h++ ) {
			if( medios[h].nro ) {
				if( MODO_NEGOCIO == SERVICIOS ) {
					_snprintf(cadaux, sizeof(cadaux)-1, "%d#%d", 40, 20 + h * 3 );
					setData( ACTION_SET_POSITION, cadaux, 0 );
					memset( cadaux, 0, 20 );
				}
				IMPRIME_CUERPO_RETIROS( 0, medios[h].nombre, cierre[h], 0L, 0, cierre_me[h], 0 );
				_ADD_MEMORY_DOUBLE( __x_retiros_importe, h, cierre[h] );
				_ADD_MEMORY_DOUBLE( __x_retiros_importe_me, h, cierre_me[h] );
				if( cierre[h] ) {
					_ADD_MEMORY_INT( __x_retiros_cantidad, h, 1 );
				}
				if( medios[h].efectivo ) {
					ADD_MEMORY_DOUBLE( __x_tot_dinero, -cierre[h] );
				}
			}
		}
		IMPRIME_PIE_RETIROS( NULL, 0, SI, NULL, SI );
	} else 
	{
		IMPRIME_TKT_RETIRO( SI );
	}
}
/*****************************************************************************/
void GRABAR_CABECERA_EVENTO_INFORMES( int reintento, int incrementa_evento )
/*****************************************************************************/
{
    
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = _EVENT_INFORME;//8
	event_ticket.nro_comprobante_new = 0;
    event_ticket.nro_ticket = 0;
    event_ticket.bultos = reintento;
    GRABAR_EVENT_TICKET( NO );
	
	/* Cuando se realiza solo el informe por tecla debe incrementar el evento,
	sino lo debe incrementar posterior a haber grabado el detalle, 
	de lo contrario queda desincronizada la cabecera de los eventos */

	if(incrementa_evento)
		ACTUALIZA_ID_EVENTO_NEW();
}
/*****************************************************************************/
void GRABAR_CABECERA_EVENTO_REIMPRESION( int valor )
/*****************************************************************************/
{
  
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = _EVENT_REIMPRESION_COMPROBANTE;//37
	event_ticket.nro_comprobante_new = 0;
    event_ticket.nro_ticket = 0;
    event_ticket.bultos = valor;
    GRABAR_EVENT_TICKET( NO );
	ACTUALIZA_ID_EVENTO_NEW();
}
/*****************************************************************************/
void GRABAR_EVENTO_DIFERENCIAS( int modo, double cierre, double tot, double cierre_me,
                                double tot_me, int submedio )
/*****************************************************************************/
{
    int informar = 0;
    //en caso de servicios puede haber manejo de cheques pero puede
    //quedar el total en cero porque hubo retiros. Para que funcionen los reportes
    //de instore, se necesita enviar una grabacion del medio en 0 en caso de
    // que se hubiese trabajado con cheques en el pos.
    if( MODO_NEGOCIO == SERVICIOS && MODO_CHEQUE_COMO_UN_MEDIO && medios[modo - 1].cheque ) {
        if( _X_COBROS_IMPORTE( modo - 1 ) || _X_RETIROS_IMPORTE( modo - 1 )
         || _X_DEP_IMP( modo - 1 ) ) {
            informar = 1;
        }
    }
	if( submedio != 0 ) {
		if( MOSTRAR_SUBMEDIO( modo, submedio ) == 1 ) {
            event_pago.importe = importe_dec;  //informado
			
            event_pago.vuelto_efectivo = importe_inf;    //calculado
		
            event_pago.imp_pago = ( importe_dec - importe_inf );
            event_pago.cod_sub_medio = ( char )sub_medios;
            event_pago.modo_pago = modo;
            event_pago.importe_me = cierre_me;   /*----- informado ------*/
            event_pago.vuelto_propio = tot_me;   /*----- calculado ------*/
            event_pago.posicion = c;
            c++;	importe_inf = 0;importe_dec = 0;
            GRABAR_EVENT_PAGO( NO );
        }
    }
    else if( cierre != 0 || tot != 0 || informar ) {
        //Para Medios s/submedio
        event_pago.importe = cierre;         /*----- informado ------*/
        event_pago.vuelto_efectivo = tot;   /*----- calculado ------*/
        event_pago.imp_pago = ( cierre - tot );
        event_pago.modo_pago = modo;
        event_pago.importe_me = cierre_me;   /*----- informado ------*/
        event_pago.vuelto_propio = tot_me;   /*----- calculado ------*/
        event_pago.posicion = c;
        c++;
        GRABAR_EVENT_PAGO( NO );
    }
}
/*****************************************************************************/
void ACTUALIZA_CABECERA_EVENTO()
/*****************************************************************************/
{
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
    //event_ticket.ila = c;
    UPDATE2();
}
/*****************************************************************************/
int GRABAR_ENTRADA_CAJERO()
/*****************************************************************************/
{
    char aux[50];
    int ok = 0, caja = 0;
    //struct _rend_caj rend_caj;
    struct _ind
    {
        unsigned short cod_cajero;
        unsigned short caja;
        unsigned long nro_y;
    } ind;
    if( OPEN_TABLE(T_REND_CAJ, TT_ORIG , ( char* )&rend_caj, sizeof( rend_caj ))== 0 ) {
		//SELECT_TABLE( T_REND_CAJ, TT_ORIG );
        ind.cod_cajero = (unsigned short)NRO_CAJERO;
        ind.caja = 0;
        ind.nro_y = 0;
        SET_WHERE("COD_CAJERO >= '%i'",ind.cod_cajero);
        SET_ORDER2("COD_CAJERO");
		RUN_QUERY(NO);
        while( rend_caj.cod_cajero == (unsigned short)ind.cod_cajero && !dbEOF() && !caja ) {
            if( rend_caj.estado == 0 ) {
                caja = rend_caj.caja;
            }
            SKIP2( 1 );
        }
        if( caja == 0 ) {
            memset( &rend_caj, 0, sizeof( rend_caj ) );
            ind.cod_cajero = (unsigned short)NRO_CAJERO;
            ind.caja = (unsigned short)NRO_CAJA;
            ind.nro_y = NRO_Y + 1;
			SET_WHERE("COD_CAJERO = '%i' and CAJA = '%i' and NUMERO_Y = '%ld'",
					ind.cod_cajero,ind.caja,ind.nro_y);
			RUN_QUERY(NO);
            if( FOUND2() ) {
                ok = 1;
            }
            else {
                rend_caj.cod_cajero = NRO_CAJERO;
                rend_caj.caja = NRO_CAJA;
                rend_caj.nro_y = NRO_Y + 1;
                rend_caj.fecha_ing = GET_FECHA_DOS();
                rend_caj.hora_ing = GET_HORA_DOS();
                if( INSERT2(SI) == 0 ) {
                    ok = 1;
                }
                else {
                    MENSAJE_TECLA_STRING( S_NO_PUDO_INSERTAR_EN_RENDICION_DE_CAJA );
                }
            }
        }
        else if( caja == NRO_CAJA ) {
            ok = 1;
        }
        else {
            _snprintf(aux, sizeof(aux)-1, ST( S_EL_CAJERO_YA_ESTA_HAB_EN_LA_CAJA___I ), caja );
            MENSAJE( aux );
        }
        CLOSE_TABLE( T_REND_CAJ, TT_ORIG );
    }
    else {
        ok = 1;                 // TOLEDO: Permite ingresar aunque no tenga red
    }
    return ( ok );
}
/*****************************************************************************/
int GRABAR_SALIDA_CAJERO()
/*****************************************************************************/
{
    int ok = 0;
    struct _ind
    {
        unsigned short cod_cajero;
        unsigned short caja;
        unsigned long nro_y;
    } ind;
    //struct _rend_caj rend_caj;
	if( CAJEROS_FLOTANTES ) {
        if(OPEN_TABLE(T_REND_CAJ, TT_ORIG ,( char* )&rend_caj, sizeof( rend_caj ))== 0 ) {
			//SELECT_TABLE( T_REND_CAJ, TT_ORIG );
            ind.cod_cajero = (unsigned short)NRO_CAJERO;
            ind.caja = (unsigned short)NRO_CAJA;
            ind.nro_y = (unsigned long)(NRO_Y + 1);
			SET_WHERE("COD_CAJERO = '%i' and CAJA = '%i' and NUMERO_Y = '%ld'",
					ind.cod_cajero,ind.caja,ind.nro_y);
            //GET_EQUAL( ( char* )( &ind ) );
			RUN_QUERY(NO);
            if( FOUND2() ) {
                rend_caj.fecha_cie = GET_FECHA_DOS();
                rend_caj.hora_cie = GET_HORA_DOS();
                rend_caj.estado = 1;
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                if( UPDATE2() == 0 ) {
                    ok = 1;
                }
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
            else {
                rend_caj.cod_cajero = NRO_CAJERO;
                rend_caj.caja = NRO_CAJA;
                rend_caj.nro_y = NRO_Y + 1;
                rend_caj.fecha_ing = GET_FECHA_DOS();
                rend_caj.hora_ing = GET_HORA_DOS();
                rend_caj.fecha_cie = GET_FECHA_DOS();
                rend_caj.hora_cie = GET_HORA_DOS();
                rend_caj.estado = 1;
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                INSERT2(SI);
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
                ok = 1;
            }
            CLOSE_TABLE( T_REND_CAJ, TT_ORIG );
        }
        else {
            if( PERMITE_CIERRE_SIN_RED ) {
                ok = 1;
            }
        }
    }
    else {
        ok = 1;
    }
    return ( ok );
}
/*****************************************************************************/
int ARQUEO()
/*****************************************************************************/
{
    double tot[10],dif[10],cierre[10],tot_me[10],dif_me[10],cierre_me[10];
    int h;
    char cadena[100];
    char salir = 0;
    int arqueo_confirmado = 1;
    #if defined(INVEL_W) || defined(INVEL_L)
    char aux[10];
    _itoa( TIT_ARQUEO, aux, 10 );
    #endif
    memset( tot, 0, sizeof( tot ) );
    memset( cierre, 0, sizeof( cierre ) );
    memset( dif, 0, sizeof( dif ) );
    memset( tot_me, 0, sizeof( tot_me ) );
    memset( cierre_me, 0, sizeof( cierre_me ) );
    memset( dif_me, 0, sizeof( dif_me ) );
    VACIAR_CIERRE_DE_LOTE();
    MENSAJE_SIN_SONIDO_STRING( S_ARQUEO_DE_CAJERO );
    Orden = NUMERO_PANEL_BILLETES;
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
    if( !CAJEROS_FLOTANTES ) {
        while( !salir ) {
            /*---------------------- prepara pantalla --------------------------------*/
			if( PEDIR_DIFERENCIAS ){
                CREAR_ARRAY_DIFERENCIAS();
                if( !LLENAR_ARRAY_DIFERENCIAS() ) {
                    MENSAJE_STRING( S_ERROR_LLENAR_ARRAY );
                    return 0;
                }
				if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ ) {
					/*CREAR_ARRAY_DIFERENCIAS();
					if( !LLENAR_ARRAY_DIFERENCIAS() ) {
						MENSAJE_STRING( S_ERROR_LLENAR_ARRAY );
						return 0;
					}*/

                #if defined(INVEL_W) || defined(INVEL_L)
                ExecuteSSAction( A_DES_VTAS_ACT_CIERRE, T_CIERRE_SUBMEDIOS, aux, "", NULL, " " );
                #endif
                CIERRE_SUBMEDIOS_CONFECCIONA_PANTALLA( NO );
            }
            else {
					/*#debe arreglar el tema de la  pantalla de para solo medios..
					if defined(INVEL_W) || defined(INVEL_L)
					ExecuteSSAction( A_DES_VTAS_ACT_CIERRE, T_CIERRE_MEDIOS, aux, "", NULL, " " );
					#endif

					CIERRE_CONFECCIONA_PANTALLA( NO );*/
                #if defined(INVEL_W) || defined(INVEL_L)
						ExecuteSSAction( A_DES_VTAS_ACT_CIERRE, T_CIERRE_SUBMEDIOS, aux, "", NULL, " " );
                #endif
					CIERRE_SUBMEDIOS_CONFECCIONA_PANTALLA( NO );
            }
			}
            cajero_operando = 0;
            /*--------------------- calcula lo que hay -------------------------------*/
            for( h = 0;h < 9;h++ ) {
                if( medios[h].nro ) {
                    if( medios[h].efectivo ) {
                        tot[h] = X_TOT_DINERO;
                    }
                    else {
                        tot[h] = _X_VENTAS_IMPORTE( h ) + _X_COBROS_IMPORTE( h )
                               - _X_RETIROS_IMPORTE( h ) + _X_FONDO_FIJO_MULTIPLE_IMPORTE( h );
                        tot_me[h] = _X_VENTAS_IMPORTE_ME( h ) + _X_COBROS_IMPORTE_ME( h )
                                  - _X_RETIROS_IMPORTE_ME( h ) + _X_FONDO_FIJO_MULTIPLE_IMP_ME( h );
                    }
                }
           }
            /*---------------------- Pide los importes ------------------------------*/
            if(/* DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ */PEDIR_DIFERENCIAS) {
                while( 1 ) {
                    int tecla = GETCH();
                    if( tecla == 32 ) {
                        // espacio
                        break;
                    }
					if( tecla == 137 && SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI,ST(S__ABRIR_CAJON_), NULL, RAM_COD_FUNCION, SI ) ){ //la configurada para abrir el cajon en el teclado
						ABRIR_CAJON();
					}
                    else if( tecla == 27 ) {
                        // escape
                        if( ABORTA_CIERRE() ) {
                            arqueo_confirmado = 0;
                            salir = 1;
                            MENSAJE_STRING( S_ARQUEO_ABORTADO );
                            break;
                        }
                    }
                    tecla -= 48;     // obtener tecla en int
                    if( tecla > 0 && tecla < 10 ) {
                        h = tecla - 1;
                        if( medios[h].nro ) {
                            INGRESA_IMPORTE_DISC_CIERRE( h, tot[h], &cierre[h], &dif[h], tot_me[h],
                                                         &cierre_me[h], &dif_me[h] );
                            MOSTRAR_ENTERO( 5, 6 + h, "9", h + 1 );
                        }
                    }
                }
            }else {
                for( h = 0;h < 9;h++ ) {
                    if( medios[h].nro ) {
                        INGRESA_IMPORTE_CIERRE( h, tot[h], &cierre[h], &dif[h], tot_me[h],
                                                &cierre_me[h], &dif_me[h] );
                    }
                }
            }
            if( arqueo_confirmado && REINTENTA_RENDICION() ) {
                PRN_SELECT( JOURNAL );
                _snprintf(cadena, sizeof(cadena)-1, ST( S_REINTENTO_ARQUEO___LI_ ), USUARIO_CLAVE_ANTERIOR() );
                MOSTRAR_CADENA( 1, PRN, cadena );
                LF( 1 );
                #if defined(INVEL_W) || defined(INVEL_L)
                ExecuteAction( A_REINTENT_ARQUEO_CIERRE );
                #endif
                PRN_HABILITA_AVANCES();
            }
            else {
                salir = 1;
            }
        }
        if( arqueo_confirmado ) {
            if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ && EMITIR_INFORME_DIFERENCIAS_SUBME ) {
                INFORME_DIFERENCIAS_SUBMEDIOS( cierre, tot, NO, cierre_me, tot_me );
            }
            else {
                INFORME_DIFERENCIAS( dif, NO, dif_me );
            }
            if( DISCRIMINAR_SUBMEDIOS_CIERRE_CAJ ) {
                FREE_ARRAY_DIFERENCIAS();
            }
        }
        cajero_operando = 1;
    }
    ClearPanel( pPanel );

    ExecuteAction( A_DES_CIERRE_ACT_VENTAS );
    return 1;
}
/*-------------------------------------*
 *                                     *
 * ESTRUCTURA DE RENDICION EN MEMORIA  *
 *                                     *
 *-------------------------------------*/
/*****************************************************************************/
void FREE_ARRAY_DIFERENCIAS( void )
/*****************************************************************************/
{
    int h;
    for( h = 0;h < 10 ;h++ ) {
         if( mem_rendicion_medio[h] )
        free( mem_rendicion_medio[h] );
        mem_rendicion_medio[h] = NULL;
        cant_mem_rendicion_medio[h] = 0;
    }
}
/*****************************************************************************/
void CREAR_ARRAY_DIFERENCIAS( void )
/*****************************************************************************/
{
    int i = 0, btrv_rta, h;
    int tam_buffer_tarjetas = 0, tam_buffer_t_compra = 0, tam_buffer_mutuales = 0;
	char straux[80];
    struct _tarjetas tarjeta_sic;
    struct _t_compra  t_compra;
    struct _t_mutuales mutuales;
    struct rendicion_en_memoria *rend_tmp;
    FREE_ARRAY_DIFERENCIAS();
    for( h = 0;h < 10;h++ ) {
        if( medios[h].mutual ) {
            /* LLENAR MUTUALES */
            btrv_rta = /*USE_DB( AREA_TEMPORAL, mutuales, ( char* )&mutuales, sizeof( mutuales ),
                               NULL, 0, 0 );*/
					   OPEN_TABLE(T_MUTUALES, TT_ORIG ,( char* )&mutuales,sizeof( mutuales ));
            if( btrv_rta == 0 ) {
				//SELECT_TABLE( T_MUTUALES, TT_ORIG );
				SET_WHERE("");
				RUN_QUERY(NO);
                i = 0;
                cant_mem_rendicion_medio[h] = RECCOUNT2(NULL,NULL);
				//GO2(TOP);
                //GET_FIRST();
                while( !dbEOF() ) {
                    rend_tmp = RENDICION_MEDIO( h, i );
                    if ( mutuales.cod_medio_padre == medios[h].nro ) {
                        strcpy( rend_tmp->desc, mutuales.des_mutual );
                        rend_tmp->codigo = mutuales.cod_mutual;
                        //aca deberia copiarse mmancuso
                        memcpy(mem_rendicion_medio[h] + i,rend_tmp, sizeof(rend_tmp));
                        i++;
                    }
                    //GET_NEXT();
					SKIP2(1);
                }
                CLOSE_TABLE( T_MUTUALES, TT_ORIG );
            }
            else {
                //se actualiza a la cantidad real de submedios
                cant_mem_rendicion_medio[h] = ( i < 0 )? 0 : i;
                _snprintf(straux, sizeof(straux)-1, "ERROR carga rendicion medio: %", medios[h].nombre );
                GRABAR_LOG_SISTEMA( straux ,LOG_ERRORES,1);
            }
        }
        else if( medios[h].ticket_compra ) {
            /* LLENAR T_COMPRA */
            btrv_rta = /*USE_DB( AREA_TEMPORAL, _t_compra , ( char* )&t_compra, sizeof( t_compra ),
                               NULL, 0, 0 );*/
			OPEN_TABLE(T_COMPRA, TT_ORIG ,( char* )&t_compra,sizeof( t_compra ));
			SELECT_TABLE( T_COMPRA, TT_ORIG );
            if( btrv_rta == 0 ) {
               SET_WHERE("");
				   RUN_QUERY(NO);
                i = 0;
                cant_mem_rendicion_medio[h] = RECCOUNT2(NULL,NULL);
                //GET_FIRST();
				     GO2(TOP);
                while( !dbEOF() ) {
                    rend_tmp = RENDICION_MEDIO( h, i );
                    if ( t_compra.cod_medio_padre == medios[h].nro ) {
                        strcpy( rend_tmp->desc, t_compra.des_t_compra );
                        rend_tmp->codigo = t_compra.cod_t_compra;
                        //aca deberia copiarse mmancuso
                        memcpy(mem_rendicion_medio[h] + i,rend_tmp, sizeof(rend_tmp));
                        i++;
                    }
                    SKIP2(1);
                }
                CLOSE_TABLE( T_COMPRA, TT_ORIG );
            }
            else {
                //se actualiza a la cantidad real de submedios
                cant_mem_rendicion_medio[h] = ( i < 0 )? 0 : i;
                _snprintf(straux, sizeof(straux)-1, "ERROR carga rendicion medio: %", medios[h].nombre );
                GRABAR_LOG_SISTEMA( straux ,LOG_ERRORES,1);
            }
        }
        else if( medios[h].tarjeta ) {
            /* LLENAR TARJETAS */
            btrv_rta = /*USE_DB( AREA_TEMPORAL, _tarjetas, ( char* )&tarjeta_sic,
                               sizeof( tarjeta_sic ), NULL, 0, 0 );*/
						   OPEN_TABLE(T_TARJETAS, TT_ORIG ,( char* )&tarjeta_sic,sizeof( tarjeta_sic ));
            if( btrv_rta == 0 ) {
				SELECT_TABLE( T_TARJETAS, TT_ORIG );
                i = 0;
                cant_mem_rendicion_medio[h] = RECCOUNT2(NULL,NULL);
                //GET_FIRST();
				SET_WHERE("");
				RUN_QUERY(NO);
                while( !dbEOF() ) {
                    rend_tmp = RENDICION_MEDIO( h, i );
                    strcpy( rend_tmp->desc, tarjeta_sic.nombre );
                    rend_tmp->codigo = tarjeta_sic.cod_tarjeta;
                    //aca deberia copiarse mmancuso
                    memcpy(mem_rendicion_medio[h] + i,rend_tmp, sizeof(rend_tmp));
                    i++;
                    //GET_NEXT();
					SKIP2(1);
                }
                CLOSE_TABLE( T_TARJETAS, TT_ORIG );
            }
            else {
                char straux[80];
                _snprintf(straux, sizeof(straux)-1, "ERROR carga rendicion medio: %", medios[h].nombre );
                GRABAR_LOG_SISTEMA( straux ,LOG_ERRORES,1);
            }
        }
        //CLOSE_DATABASE( AREA_TEMPORAL );
    }//for
}
/*****************************************************************************/
int LLENAR_ARRAY_DIFERENCIAS( void )
/****************************************************************************/
{
    #ifdef INVEL_W
    struct rendicion_en_memoria *str_tmp;
	int count = 0;
    /* REGRESA HASTA LA ULTIMA Y */
    //SELECT( AREA_TICKET );
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	SET_WHERE("");
	SET_ORDER2("ID_EVENTO");
    //GET_LAST();
	RUN_QUERY(NO);
	GO2(BOTTOM);
	count = RECCOUNT2( NULL, NULL);
    while( count && !dbBOF() && CONTINUA_BUSCA_EVENTO() ) {
        //GET_PREVIOUS();
		SKIP2(-1);
    }
    while( !dbEOF() ) {
        if( event_ticket.cajero == NRO_CAJERO && ( event_ticket.tipo_evento == 0 ||   // ticket
            event_ticket.tipo_evento == 7 ||   // retiro
            event_ticket.tipo_evento == 1 ||   // cobro
            event_ticket.tipo_evento == 2 ||   // pago
            event_ticket.tipo_evento == 3 ||   // factura A
            event_ticket.tipo_evento == 4 ||   // factura B
            event_ticket.tipo_evento == 5 ||   // nota de credito A
            event_ticket.tipo_evento == 6 ||   // nota de credito B
            event_ticket.tipo_evento == 14 ||   // facturas A de remitos
            event_ticket.tipo_evento == 15 ||   // facturas B de remitos
            event_ticket.tipo_evento == 16 ||   // factura remota A
            event_ticket.tipo_evento == 17 ||   // factura remota B
            event_ticket.tipo_evento == 18 ||   // remito remoto
            event_ticket.tipo_evento == 19 ||   // NC remota A
            event_ticket.tipo_evento == 25 ||   // fondo fijo
            event_ticket.tipo_evento == 20 ||   // NC remota B
            event_ticket.tipo_evento == 21 ||   // Devolucion
            event_ticket.tipo_evento == 35 ||   // Ticket Factura A
            event_ticket.tipo_evento == 36 ||   // Ticket Factura B
            event_ticket.tipo_evento == 40 ||   // Cambio de medios
            event_ticket.tipo_evento == 70 ||   // Cobros Especiales
            event_ticket.tipo_evento == 71 ||   // Cobros Especiales
            event_ticket.tipo_evento == 72 ) ) {
            // Pagos Especiales
            //SELECT( AREA_PAGO );
            //SET_ORDER( 1 );
            //GET_EQUAL( ( char* )&event_ticket.id_evento );
			SELECT_TABLE( T_PAGOS, tipo_t_pagos );
			if(event_ticket.tipo_evento == 40 && config_tps.ActivarCashPlus == 1)
				SET_WHERE("ID_EVENTO = '%ld' and POSICION = 0", event_ticket.id_evento);
			else
				SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            while( !dbEOF() && event_ticket.id_evento == event_pago.id_evento ) {
                if( !event_pago.reproceso ) {
                    if( event_pago.modo_pago ) {
                        event_pago.modo_pago--;
                    }
                    else {
                        event_pago.modo_pago = _medio_efectivo;
                    }
                    if( medios[event_pago.modo_pago].mutual || medios[event_pago.modo_pago].tarjeta
                     || medios[event_pago.modo_pago].ticket_compra ) {
                        str_tmp = BUSCAR_ESTRUCTURA_EN_MEMORIA( ( char )event_pago.modo_pago,
                                                                event_pago.cod_sub_medio );
                        if( str_tmp != NULL ) {
                            str_tmp->cantidad++;
                            if( event_ticket.tipo_evento == 7 ) {
                                str_tmp->monto -= event_pago.importe;
                            }
                            else {
                                str_tmp->monto += event_pago.importe;
                            }
                            if( event_pago.vuelto_medio ) {
                                if( medios[event_pago.cod_medio_vuelto - 1].mutual
                                 || medios[event_pago.cod_medio_vuelto - 1].ticket_compra ) {
                                    str_tmp = BUSCAR_ESTRUCTURA_EN_MEMORIA( ( char )( event_pago.
                                 cod_medio_vuelto - 1 ), event_pago.cod_submedio_vuelto );
                                    if( str_tmp != NULL ) {
                                        str_tmp->monto -= event_pago.vuelto_medio;
                                    }
                                }
                            }
                        }
                        else {
                            char debug[150];
                            char tipo_medio[30];
                            if( medios[event_pago.modo_pago].mutual ) {
                                strncpy( tipo_medio, ST( S_MUTUAL ), sizeof( tipo_medio ) );
                            }
                            else if( medios[event_pago.modo_pago].tarjeta ) {
                                strncpy( tipo_medio, ST( S_TARJETA ), sizeof( tipo_medio ) );
                            }
                            else if( medios[event_pago.modo_pago].ticket_compra ) {
                                strncpy( tipo_medio, ST( S_TICKET_COMPRA ), sizeof( tipo_medio ) );
                            }
                            else {
                                tipo_medio[0] = '\0';
                            }
                            _snprintf(debug, sizeof(debug)-1, ST( S_NO_ENCONTRADO_MEDIO__S___D__D__DE___2F__ENTER_ ),
                                     tipo_medio, event_pago.modo_pago, event_pago.id_registro,
                                     event_pago.importe );
                            GRABAR_LOG_SISTEMA( debug,LOG_ERRORES,1 );
                            MENSAJE_CON_TECLA( debug, 13 );
                        }
                    }
                }
                SKIP2(1);
            }
        }
        //SELECT( AREA_TICKET );
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
        //GET_NEXT();
		SKIP2(1);
    }
    #endif
    return 1;
}
/*****************************************************************************/
struct rendicion_en_memoria * BUSCAR_ESTRUCTURA_EN_MEMORIA( char medio, char id_registro )
/*****************************************************************************/
{
    int h,i;
    struct rendicion_en_memoria *ret = NULL;
    h = medio;
    if( h < 9 ) {
        if( medios[h].mutual || medios[h].ticket_compra || medios[h].tarjeta ) {
            for( i = 0;
                 i < cant_mem_rendicion_medio[h] && RENDICION_MEDIO( h, i )->codigo != id_registro;
                 i++ ) {
                ;
            }
            if( i < cant_mem_rendicion_medio[h] ) {
                ret = RENDICION_MEDIO( h, i );
            }
        }
    }
    return ret;
}
/***************************************************************************/
struct rendicion_en_memoria * RENDICION_MEDIO( int medio, int posicion )
/***************************************************************************/
{
    int cant;
    if( mem_rendicion_medio[medio] ) {
        return mem_rendicion_medio[medio] + posicion;
    }
    else {
        cant = cant_mem_rendicion_medio[medio];
        mem_rendicion_medio[medio] = ( struct rendicion_en_memoria *)calloc( cant, sizeof( struct rendicion_en_memoria ) );
        return ( mem_rendicion_medio[medio] + posicion );
    }
    return NULL;
}
/*****************************************************************************/
void IMPRIME_DISC_RETIROS( char *desc, double monto )
/*****************************************************************************/
{
    if( IMPRIME_RETIROS ) {
        MOSTRAR_CADENA( 4, PRN, desc );
        MOSTRAR_FLOTANTE( 16, PRN, ENTEROS, DECIMALES, 0, 0, monto );
        LF( 1 );
    }
}
/*****************************************************************************/
int ABORTA_CIERRE()
/*****************************************************************************/
{
    int aborta = 0;
    int salir = 0;
    char k;
    if( !PERMITIR_ABORTAR_CIERRE ) {
        return 0;
    }
    MENSAJE_STRING( S_ENTER_PARA_ABORTAR_ARQUEO_ESC_RETORNAR_ARQUEO );
    while( !salir ) {
        k = GETCH();
        switch( k ) {
            case 13:
                aborta = 1;
                salir = 1;
                break;
            case 27:
                salir = 1;
        }
    }
    BORRAR_MENSAJE();
    if( aborta ) {
        if( !SOLICITAR_CLAVES( 50, 20, NO, SI, SI, ST( S_ABORTAR_CIERRE ),
                               ST( S_ABORTAR_CIERRE_DE_CAJERO ), _F_SUP_ABORTA_CIERRE, SI )
         || !SOLICITAR_CLAVES( 50, 20, SI, NO, SI, ST( S_ABORTAR_CIERRE ),
                               ST( S_ABORTAR_CIERRE_DE_CAJERO ), _F_SUP_ABORTA_CIERRE, SI ) ) {
            aborta = 0;
        }
    }
    if( aborta ) {
        VACIAR_CIERRE_DE_LOTE();
        VACIAR_DECLARACION_SUBMEDIO();
    }
    BORRAR_MENSAJE();
    return aborta;
}
/*****************************************************************************/
int REINTENTA_RENDICION()
/*****************************************************************************/
{
    int salir = 0;
    int reintenta = 0;
    int k;
    if( !PERMITIR_REINTENTO_DE_ARQUEO ) {
        return( 0 );
    }
    MENSAJE_STRING( S_PRES_ENTER_ACEPTAR_EL_ARQUEO );
    while( !salir ) {
        k = GETCH();
        switch( k ) {
            case 13:
                salir = 1;
                break;
        }

        if( !salir && k != -999 ) {
            if( SOLICITAR_CLAVES( 50, 20, NO, SI, SI, ST( S_ARQUEO ),
                                  ST( S_REINTENTO_ARQUEO_CAJERO_EN_CIERRE ),
                                  _F_SUP_REINTENTO_RENDICION, SI ) ) {
                reintenta = 1;
                salir = 1;
            }
        }
    }
    BORRAR_MENSAJE();
    return( reintenta );
}
/*****************************************************************************/
void IMPRIME_VALE_DIFERENCIA( int medio, int submedio, double importe, int duplicado )
/*****************************************************************************/
{
    //char cadaux[20];
    int hora, fecha, i = 0, j = 0;
	char cadena[30];
	char caracter_sig[2];
	caracter_sig[0]='-';
	caracter_sig[1]='\0';

    hora = GET_HORA_DOS();
    fecha = GET_FECHA_DOS();
    if( MODO_NEGOCIO == SERVICIOS ) {
        setData( ACTION_SET_POSITION, "10#20", 0 );
        MOSTRAR_CADENA( 1, PRN, "CAJERO:    " );
		memset(cadena, 0, sizeof(cadena));
        if( IMPRIMIR_LEGAJO_CAJERO ) {
            _snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
			MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
        }
        else {
			_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
			MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
        }
        MOSTRAR_CADENA( 1, PRN, caracter_sig );
        MOSTRAR_CADENA( 1, PRN, datos_cajero_actual.nombre );
        setData( ACTION_SET_POSITION, "150#(20 + i)", 0 );
        MOSTRAR_CADENA( 1, PRN, "CAJA: " );
        //MOSTRAR_ENTERO( 1, PRN, "0000", NRO_CAJA );
		MOSTRAR_ENTERO( 1, PRN, ( USAR_NRO_CAJA_DGI )?
				"0000":"000", NUMERO_CAJA( ) );
    }
    //IMPRIME_CABECERA_TICKET(  );
    setData( ACTION_SET_POSITION, "75#30", 0 );
    if( duplicado ) {
        MOSTRAR_CADENA( 1, PRN, "DUPLICADO" );
    }
    else {
        MOSTRAR_CADENA( 1, PRN, "ORIGINAL" );
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
        setData( ACTION_SET_POSITION, "10#50", 0 );
    }
    MOSTRAR_CADENA( 1, PRN, "VALE POR   " );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, medios[medio].nombre );
    /*if (medios[medio].tarjeta)
          MOSTRAR_CADENA( 1, PRN, NOMBRE_TARJETA(submedio) );*/
    setData( ACTION_SET_POSITION, "70#50", 0 );
    MOSTRAR_FLOTANTE( 1, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
    LF( 1 );
    setData( ACTION_SET_POSITION, "10#70", 0 );
    MOSTRAR_CADENA( 1, PRN, "    ----------------                          ------------" );
    setData( ACTION_SET_POSITION, "10#73", 0 );
    MOSTRAR_CADENA( 1, PRN, "    FIRMA SUPERVISOR                          FIRMA CAJERO" );
    setData( ACTION_SET_POSITION, "45#83", 0 );
    MOSTRAR_FECHA_LETRAS( 1, PRN, fecha );
    setData( ACTION_SET_POSITION, "103#83", 0 );
    MOSTRAR_HORA( 1, PRN, hora );
    setData( ACTION_SET_POSITION, "117#83", 0 );
    MOSTRAR_CADENA( 1, PRN, "Hs." );
    setData( ACTION_SET_POSITION, "10#93", 0 );
    MOSTRAR_CADENA(
    1, PRN, "----------------------------------------------------------------------------------" );
    setData( ACTION_PRINT, NULL, 0 );
    //smg solicitò imprimir los vales en una misma pagina.
    PRN_FIN_TICKET();
    setData( ACTION_DOPRINT, NULL, 0 );
}
/*************************************************************************************************************************/
void IMPRIME_VALE_DIFERENCIA_CONTINUO( struct _datosCierre *datosCierre, int duplicado )
/*************************************************************************************************************************/
{
    int hora, fecha, medio = 0, i = 0, k = 0, j = 0, t = 0, x = 0, y = 0 , p = 0 , q = 0;
    int pagina = 0, tot_paginas = 0;
    char posinicial[11][10] = {"10#20","150#20","75#30","10#50","70#50","10#70","10#73","45#83",
                               "103#83","117#83","10#93"};
    char posactual[11][10];
	char cadena[30];
    char stringData[20],stringDatax[5],stringDatay[5];
	char caracter_sig[2];
	caracter_sig[0]='-';
	caracter_sig[1]='\0';
	memset( cadena, 0, sizeof( cadena ) );
	memset( stringData, 0, sizeof( stringData ) );
	memset( stringDatax, 0, sizeof( stringDatax ) );
	memset( stringDatay, 0, sizeof( stringDatay ) );

    hora = GET_HORA_DOS();
    fecha = GET_FECHA_DOS();
    for( p = 0 ;p < 11 ;p++ ) {
        for( q = 0 ;q < 10 ;q++ ) {
            posactual[p][q] = posinicial[p][q];
        } // inicializo la posicion actual
    }
    for( i = 0 ;i < 9 ;i++ ) {
        if( fabs( datosCierre->tot[i] - datosCierre->cierre[i] ) > 0.00001 ) {
            k++;
        } //Cuento la cantidad de vales a imprimir
    }
    if( k != 1 ) {
        tot_paginas = ( ( k % 2 ) + ( k / 2 ) - 1 ); // calcula la cantidad de paginas a imprimir
    }
    for( i = 0 ;i < 9 ;i++ ) {
        if( fabs( datosCierre->tot[i] - datosCierre->cierre[i] ) > 0.00001 ) {
            j++; // cuento cuantos vales imprimi
            medio = i;
            memset( stringData, 0, sizeof( stringData ) );
            FTOA( ( datosCierre->tot[i] - datosCierre->cierre[i] ), stringData, ENTEROS + 4,
                  DECIMALES, 1, 1 );
            if( MODO_NEGOCIO == SERVICIOS ) {
                setData( ACTION_SET_POSITION, posactual[0], 0 );
                MOSTRAR_CADENA( 1, PRN, "CAJERO:    " );
				memset(cadena, 0, sizeof(cadena));
                if( IMPRIMIR_LEGAJO_CAJERO ) {
					_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
					MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
                }
                else {
					_snprintf( 1, sizeof(cadena)-1,"%d", NRO_CAJERO );
					MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );

                }
                MOSTRAR_CADENA( 1, PRN, caracter_sig );
                MOSTRAR_CADENA( 1, PRN, datos_cajero_actual.nombre );
                setData( ACTION_SET_POSITION, posactual[1], 0 );
				//MOSTRAR_CADENA( 1, PRN, "CAJA: " );
				MOSTRAR_ENTERO( 1, PRN, ( USAR_NRO_CAJA_DGI )?
						"0000":"000", NUMERO_CAJA( ) );

            }
            //IMPRIME_CABECERA_TICKET(  );
            setData( ACTION_SET_POSITION, posactual[2], 0 );
            if( duplicado ) {
                MOSTRAR_CADENA( 1, PRN, "DUPLICADO" );
            }
            else {
                MOSTRAR_CADENA( 1, PRN, "ORIGINAL" );
            }
            if( MODO_NEGOCIO == SERVICIOS ) {
                setData( ACTION_SET_POSITION, posactual[3], 0 );
            }
            MOSTRAR_CADENA( 1, PRN, "VALE POR   " );
            LF( 1 );
            MOSTRAR_CADENA( 1, PRN, medios[medio].nombre );
            /*if (medios[medio].tarjeta)
                          MOSTRAR_CADENA( 1, PRN, NOMBRE_TARJETA(submedio) );*/
            setData( ACTION_SET_POSITION, posactual[4], 0 );
            MOSTRAR_CADENA( 1, PRN, stringData );
            LF( 1 );
            setData( ACTION_SET_POSITION, posactual[5], 0 );
            MOSTRAR_CADENA( 1, PRN, "    ----------------                          ------------" );
            setData( ACTION_SET_POSITION, posactual[6], 0 );
            MOSTRAR_CADENA( 1, PRN, "    FIRMA SUPERVISOR                          FIRMA CAJERO" );
            setData( ACTION_SET_POSITION, posactual[7], 0 );
            MOSTRAR_FECHA_LETRAS( 1, PRN, fecha );
            setData( ACTION_SET_POSITION, posactual[8], 0 );
            MOSTRAR_HORA( 1, PRN, hora );
            setData( ACTION_SET_POSITION, posactual[9], 0 );
            MOSTRAR_CADENA( 1, PRN, "Hs." );
            setData( ACTION_SET_POSITION, posactual[10], 0 );
			MOSTRAR_CADENA( 1, PRN, "--------------------------------------------------------------------------------------------------" );
            for( t = 0 ;t < 11 ;t++ ) {
                sscanf( posactual[t], "%d#%d", &x, &y );
                y += 83;//Incremento la coordenada y de la posicion para imprimir el siguiente vale.
                _itoa( x, stringDatax, 10 );
                _itoa( y, stringDatay, 10 );
                _snprintf(posactual[t], sizeof(posactual[t])-1, "%s#%s", stringDatax, stringDatay );
            }
            if( j == 2 ) {
                setData( ACTION_PRINT, NULL, 0 );
                PRN_FIN_TICKET_CONTINUO( pagina, tot_paginas );
                setData( ACTION_DOPRINT, NULL, 0 );
                for( p = 0 ;p < 11 ;p++ ) {
                    for( q = 0 ;q < 10 ;q++ ) {
                        posactual[p][q] = posinicial[p][q];
                    } // inicializo la posicion actual
                }
                j = 0;
                pagina++;
            }
        }
    }
    if( j == 1 ) {
        setData( ACTION_PRINT, NULL, 0 );
        PRN_FIN_TICKET_CONTINUO( pagina, tot_paginas );
        setData( ACTION_DOPRINT, NULL, 0 );
    }
}
#if defined(INVEL_W) || defined(INVEL_L)
/*****************************************************************************/
void MOSTRAR_ITEM_RENDICION_EN_JAVA( int pant_pos, char *nombre, double importe, double diferencia )
/*****************************************************************************/
{
    char stringData[16],stringData2[16],stringData3[35];

	memset( stringData, 0, sizeof( stringData ) );
    memset( stringData2, 0, sizeof( stringData2 ) );
	memset( stringData3, 0, sizeof( stringData3 ) );
    FTOA( importe, stringData, ENTEROS + 1, DECIMALES, 1, 1 );
    _snprintf(stringData3, sizeof(stringData3)-1, stringData );
    if( MOSTRAR_DIFERENCIAS ) {
        strcat( stringData3, "$" );
        FTOA( diferencia, stringData2, ENTEROS + 1, DECIMALES, 1, 1 );
        strcat( stringData3, stringData2 );
    }
    ExecuteSSAction( A_MOSTRAR_ITEM_RE_CIERRE, pant_pos, nombre, stringData3, NULL, " " );
}
#endif
/*****************************************************************************/
void INIT_VEC_DIFERENCIAS( int medio )
/*****************************************************************************/
{
    int i,e;
    int comienzo = 0, fin = 10;
    if( medio >= 0 ) {
        //si solo se inicializa un solo medio
        comienzo = medio;
        fin = medio + 1;
    }
    for( i = comienzo;i < fin;i++ ) {
        for( e = 0;e < 100;e++ ) {
            sprintf( diferencias[i][e].strDif, "" );
        }
    }
}


/***************************************************************************/
int MOSTRAR_SUBMEDIO( int medio, int submedio )
/***************************************************************************/
{
    sub_medios = submedio;
    //Se almacenan los datos del submedio solicitado en las varables globales 'importe_inf' e 'importe_dec'
    return OBTENER_IMPORTE_Y_DECLARACION_SUBMEDIO( medio, submedio, &importe_inf, &importe_dec );
}

//***************************************************************************/
/* tipo calculadora...*/
/*
tecla = tecla actual ingresada  '+' '-' 'ENTER' 'ESC'
operacion = la operación ingresada previamente , para completar operaciones 
importe_orig = importe del medio ingresado apenas entras al arqueo
importe calculado = acumula los resultados de las operaciones  que se van aplicando sobre el importe_orig
importe_ingresado = el valor que ingreso el cual debe ser operado o ingresado directamente .
*/
void CALCULAR_IMPORTES_ARQUEO( char tecla,char *operacion, double importe_orig, double *importe_calculado, double *importe_ingresado)
/***************************************************************************/

{
switch (tecla){
				case 27:
					*importe_calculado = importe_orig;
					*operacion =0;
					break;
				
				case 13:
					if( *operacion == 0)
						*importe_calculado = *importe_ingresado;
					
					if( *operacion  == '+')
						*importe_calculado += *importe_ingresado;
					
					if( ( *operacion  == '-')&&(( *importe_calculado - *importe_ingresado ) >= 0.0 ))
						*importe_calculado -= *importe_ingresado;
					
					*importe_ingresado=0;
					*operacion =0;
					break;
				
				case '+':
                    if( *importe_calculado == 0.0 && *importe_ingresado == 0.0 ) {
                        *importe_calculado += importe_orig;
                    }else{
						if( *importe_calculado == 0.0)
							*importe_calculado += importe_orig;
						if( *operacion  == '+'||*operacion  == 0){
							*importe_calculado += *importe_ingresado;
							if( ( *operacion  == '-')&&(( *importe_calculado - *importe_ingresado ) >= 0.0 ))
								*importe_calculado -= *importe_ingresado;
							*importe_ingresado=0;
						}
					}
					*operacion ='+';
					break;
				
				case '-':
					//restar=	SI;//para estar seguro
					if( *importe_calculado == 0.0 && *importe_ingresado == 0.0 ) {
						if(( *importe_calculado - importe_orig ) >= 0.0 )
							*importe_calculado -= importe_orig;
					}else {
						//cargo el existente
						if( *importe_calculado == 0.0 )
							*importe_calculado += importe_orig;
						
						if( *operacion  == '+'||*operacion  == 0)
								*importe_calculado += *importe_ingresado;
						
						if( ( *operacion  == '-')&&(( *importe_calculado - *importe_ingresado ) >= 0.0 ))
							*importe_calculado -= *importe_ingresado;
						*importe_ingresado=0;
					}
					*operacion ='-';
                    break;
}
                    
                    
}