/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Cambio de medios
 * ------------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <stdio.h>
#if defined(INVEL_L)
#include <unistd.h>
#endif
#undef EOF
#include <ctype.h>
#include <math.h>
#pragma pack(1)
#include <cr.h> 
#include <panel.h>
#include <dbrouter.h>
#include <aarch.h>
#include <config.h>
#include <path.h>
#include "cont_sub.h"
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#include <typeacti.h>
#include <pagos.h>
#include <deftables.h>
#include <stack.h>
#include <cmemory.h>
#include <pant.h>
#include <clave.h>
#include <_cr1.h>
#include <cajon.h>
#include <log.h>
#include <db.h>
#include <cambio_m.h>
#include <on_line.h>
#include <tarjetas.h>
#include <xyz.h>
#include <m_cadena.h>
#include <valida.h>
#include <mensaje.h>
#include <pago_tar.h>
#include <pagos.h>
#include <b_mens.h>
#include <retiros.h>
#include <cr_pant.h>
#include <pago_che.h>
#include <pago_tc.h>
#include <pago_mut.h>
#include <ftoc.h>
#include <malloc.h>
#include <recupero.h>
#include <imp_ret.h>
#include <dec_mes.h>
#include <envios.h>
#include <cobros.h>
#include <db.h>
#include <deftables.h>
#include <imp_ret.h>
#include <_cr0.h>
#include <ini.h>
#endif
/*................................. Funciones ............................... */
/*............................. Variables Globales .......................... */
static struct
{
    char modo;
    double importe;
    long numero;
    int id_registro;
    char cod_sub_medio;
    short banco;
	double importe_me;
} retiro;
static char anulacion;
static double acu_retirado[10];
static int _en_cambio_de_medios;
//static unsigned aux_fecha;
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
struct _cam_val_hab camb_val_hab;
struct _transac2 *transac2;
extern void CARGAR_DATOS_BASICOS( struct _datos_transaccion *tran, int cashback );
extern int ANULACION_TARJETA_TECLA( int cashback);
extern int PINPAD_ACTUALIZAR_WORKING_KEY( char celda_pinpad, char *working_key );
extern void SETEAR_MODO_EJECUCION( int modo );
extern int T_ABRIR_TRANSAC2();

/*****************************************************************************/
void CAMBIO_MEDIOS( int cambio_medio_automatico )
/*****************************************************************************/
{
    int pant = 0,i = 0, h = 0, cod = 0, rta = 0;
    int _ptr = 0,_handle = 0,_estado = 0, _h = 0, _error_m_saliente = 0;
    char abrir_cajon = 0;
	static int modo_ant;
	int PedirAutorizacionCambioMedioAutomatico = NO;

	modo_ant = RAM_MODO_EJECUCION;
	SETEAR_MODO_EJECUCION( CAMBIO_MEDIO );
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );
	_h = STACK_INS( sizeof( short ) );
	_error_m_saliente = STACK_INS( sizeof( short ) );


    if( _ESTADO == 0 ) {
        SET_MEMORY_INT( _h, 1 );
		SET_MEMORY_INT( _error_m_saliente, 0 );
		if( cambio_medio_automatico ) { 
			if( RAM_CIERRE_Y && !CAJEROS_FLOTANTES ){
				PedirAutorizacionCambioMedioAutomatico = 0;
			} else {
				PedirAutorizacionCambioMedioAutomatico = config_tps.PedirAutorizacionCambioMedioAutomatico;
			}
		}

		if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, SI, SI,
				( cambio_medio_automatico )? PedirAutorizacionCambioMedioAutomatico:PEDIR_SUPERVISOR_PARA_CAMBIO_MED, 
                ( cambio_medio_automatico )?"          CAMBIO DE MEDIO AUTOMATICO":ST( S_CAMBIO_MEDIOS ), 
				( cambio_medio_automatico )?"          CAMBIO DE MEDIO AUTOMATICO":ST( S_CAMBIO_MEDIOS )
				, RAM_COD_FUNCION, SI ) ) {

            if( !USUARIO_PUEDE_RETIRAR( USUARIO_CLAVE_ANTERIOR() ) ) {
                MENSAJE_STRING( S_NO_ESTA_AUTORIZADO_PARA_HACER_RETIROS );
            } else {
				if( !cambio_medio_automatico ) {
					if( config.abrir_cajon_ultimo_camb || ABRIR_CAJON() ) {
						SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
						GRABAR_LOG_SUPERVISOR_STRING( S_CAMBIO_MEDIOS, USUARIO_CLAVE_ANTERIOR(),LOG_VENTAS,4 );
						_SET_ESTADO( 1 );
					}
				} else {
					_SET_ESTADO( 1 );
				}
			}
        }
    }

    if( _ESTADO == 1 ) {
        /*-------------- Actualiza estado ----------------------*/
        SETEAR_TIEMPO( _ENTRADA_NO_VENTA );
        /*----------- ingresa los medios que entran ------------*/
        anulacion = 0;
        SET_MEMORY_DOUBLE( __ram_total, 0.0 );
        CEREAR_STRUCT( _pago );
        SET_MEMORY_CHAR( __ram_p_pago, 0 );
		LIMPIAR_VARIAS_TARJETAS( );
		BORRAR_BIN_DATOS_ADICIONALES( D_MEDIOS_PRE_IMPORTE );
		ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
        _SET_ESTADO( 2 );
    }

    if( _ESTADO == 2 ) {		
		if( !cambio_medio_automatico ) {
			CAMBIO_CONFECCIONA_PANTALLA( ENTRAN_MEDIOS );
			rta = CAMBIO_PIDE_MEDIOS( ENTRAN_MEDIOS ); 
		} else {
			if( !( RAM_CIERRE_Y && !CAJEROS_FLOTANTES ) ) {//No visualiza box en el Cierre Y
				rta = PROCESAR_CAMBIO_MEDIOS_AUTOMATICO( );
			} else {
				rta = 1;
			}
		}
        if( rta ) {
            _SET_ESTADO( 3 );
        }
        else {
            _SET_ESTADO( 6 );
        }
    }

    if( _ESTADO == 3 ) {	

		if( !cambio_medio_automatico ) {
			int cashplusactivado = 0;
			if(config_tps.ActivarCashPlus == 1 
				&&  IsCashActivo()
				&& config_tps.HabilitarCashPlusSolo == 1 ){ //esto si en el cambio de medio hago la extraccion
				cashplusactivado = 1;
				ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
			}
			CAMBIO_CONFECCIONA_PANTALLA( SALEN_MEDIOS );
			if( cashplusactivado == 1  ){
				ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_AGREGAR, "RETIRO ACTIVADO", " ", NULL, " " ); 
			}		
			rta = CAMBIO_PIDE_MEDIOS( SALEN_MEDIOS );
			if( rta ) 
				_SET_ESTADO( 4 );
			else
				_SET_ESTADO( 6 );

		} else {
			rta = CARGAR_CAMBIO_MEDIO_AUTOMATICO( ); 
			 _SET_ESTADO( 7 );
		}

    }
	
	if( _ESTADO == 4 ) {  //enviar transacciones al autorizador!!!!
		if( config_tps.HabilitarCashPlusSolo == 1 
			&& (medios[( int )_PAGO_MODO( 0 )].tarjeta && config_tps.ActivarCashPlus == 0)
				|| (medios[( int )_PAGO_MODO( 0 )].tarjeta && config_tps.ActivarCashPlus == 1 &&  IsCashActivo() ) ) {

			int autorizada =0;
			if(config_tps.HabilitarCashPlusSolo == 0 )
				autorizada = 1 ;
			else
				autorizada = PEDIR_AUTORIZACION_ON_LINE( 1, NO ) ;

			if( autorizada ) {
				_SET_ESTADO( 41 );
			} else {
				_SET_ESTADO( 6 );//Anula ingreso
			}
		} else {
			_SET_ESTADO( 41 );
		}
	}

	if( _ESTADO == 41 ) {
		int h = 0, pos = 0;
		
		for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO; ADD_MEMORY_INT( _h, 1 ) ) {
			h = GET_MEMORY_INT( _h );
			/*if( medios[( int )_PAGO_MODO( h )].tarjeta ) {
				LEE_TARJETAS( pos++, TARJETA_COBRO );
				importe = _PAGO_IMPORTE( h );
				if( !ENVIAR_IMPORTE_MONEDERO( importe, TARJETA_COBRO ) ) {//TARJETAS A LAS QUE SE LE ASIGNARIA EL MONTO
					//Cancelo la tarjeta q enviamos antes
					REPROCESAR_TARJETAS( 0, NO );
					ADD_MEMORY_INT( _error_m_saliente, 1 );
				} else {
					_SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
				}
			}*/
            if(medios[( int )_PAGO_MODO( h )].abrir_cajon == 'S' || medios[( int )_PAGO_MODO( 0 )].abrir_cajon == 'S' ){
                abrir_cajon = 1;
            }
		}

		if( GET_MEMORY_INT( _error_m_saliente ) > 0 ) {
			_SET_ESTADO( 6 );
		} else {
			//_SET_ESTADO( 42 );
			_SET_ESTADO( 7 );
		}
	}
	
	if( _ESTADO == 7 ) {
        if( VALIDAR_MEDIOS_EN_CAMBIOS_DE_MED ) {
			int i = 0;
		//	if(IMPRESORA_FISCAL ==FISCAL)
		//		ABRIR_DOCUMENTO_NO_FISCAL('1');
			//do {
			if( !cambio_medio_automatico ) {	
				cod = VALIDAR_MEDIOS_CAMBIO_M( _PAGO_MODO( i ) + 1, _PAGO_COD_SUBMEDIO_VUELTO( i ) );
					if( !cod ) {
						_SET_ESTADO( 100 );
					} else {
						_SET_ESTADO( 42 );
					}
				//	i++;
				//} while( i <= RAM_P_PAGO && ( _PAGO_CUENTA( i ) == ENTRAN_MEDIOS && cod ) );
			//	if(IMPRESORA_FISCAL == FISCAL)
			//		CERRAR_DOCUMENTO_NO_FISCAL();
			} else {
				VALIDAR_CAMBIO_MEDIO( config_tps.ValidacionCambioMedioAutomatico );
				_SET_ESTADO( 5 );
			}
        }		
	}
 
    if( _ESTADO == 42 ) {
        if( config.abrir_cajon_ultimo_camb && abrir_cajon ) {
            ABRIR_CAJON();
        }
        
        SETEAR_TIEMPO( _ENTRADA_OCIO );
        GRABA_EVENTO_CABECERA_CAMBIO();
        for( i = 0;i < RAM_P_PAGO;i++ ) {
            _GRABA_EVENTO_CAMBIO( _PAGO_MODO( i ) + 1, _PAGO_IMPORTE( i ), _PAGO_ID_REGISTRO( i ),
                                  ( char )_PAGO_COD_SUBMEDIO_VUELTO( i ), _PAGO_CUENTA( i ), i );
            CAMBIO_ACUMULAR_CAMBIO_EN_X( _PAGO_MODO( i ), fabs( _PAGO_IMPORTE( i ) ),
                                         ( _PAGO_IMPORTE( i ) < 0 ) ? 1 : 0, _PAGO_CUENTA( i ) );
            CAMBIO_GUARDAR_SUBMEDIOS_REC( _PAGO_MODO( i ) + 1, _PAGO_COD_SUBMEDIO_VUELTO( i ), fabs( _PAGO_IMPORTE( i ) ),
                                         ( _PAGO_IMPORTE( i ) < 0 ) ? 1 : 0, _PAGO_CUENTA( i ) );
        }
        ACTUALIZA_ID_EVENTO_NEW();
        _SET_ESTADO( 5 );
        GRABAR_XYZ( 'X' );
    }
	
    if( _ESTADO == 5 ) {
		_SET_ESTADO( 100 );
    }
    if( _ESTADO == 6 ) {
        if( RAM_P_PAGO ) {
			MENSAJE_TECLA( ST( S_CAMBIO_DE_MEDIO_CANCELADO_Pres_Tecla ) );
            MOSTRAR_CADENA( 1, PRN, ST( S_CAMBIO_ANULADO ) );
			ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
            LF( 1 );
            if( IMPRIME_RETIROS == 3 ) {
                PRN_INHABILITA_SLIP();
            }
            else if( IMPRIME_RETIROS ) {
                PRN_FIN_TICKET();
            }
            CLOSE_COPIA_PRN();
			CEREAR_STRUCT( _var_tarjetas );
			SETEAR_MODO_EJECUCION( modo_ant );
        }
		_SET_ESTADO( 100 );
	}

	/*if( _ESTADO == 7 ) {
        if( VALIDAR_MEDIOS_EN_CAMBIOS_DE_MED ) {
			int i = 0;
			
			do {
				VALIDAR_MEDIOS_CAMBIO_M( _PAGO_MODO( i ) + 1, _PAGO_COD_SUBMEDIO_VUELTO( i ) );
				i++;
			} while( i <= RAM_P_PAGO && ( _PAGO_CUENTA( i ) == ENTRAN_MEDIOS ) );
        }
		_SET_ESTADO( 100 );
	}*/

	/*if( _ESTADO == 100 ) {
			debe devolvar las tarjestas
	}*/
	if( !cambio_medio_automatico )
		CERRAR_CAJON( );

	SETEAR_MODO_EJECUCION( modo_ant );
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
    /*--------------- fin ----------------*/
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    SET_MEMORY_DOUBLE( __ram_total, 0.0 );
    SET_MEMORY_CHAR( __ram_p_pago, 0 );
	SET_MEMORY_LONG( __ram_cod_premio, 0);
    CEREAR_STRUCT( _var_tarjetas );
    MUESTRA_SUBTOTAL( NO );
    ExecuteAction( A_DES_CAMBIOMEDIO_ACT_VENTAS );
}

/*****************************************************************************/
int PROCESAR_CAMBIO_MEDIOS_AUTOMATICO( void )
/*****************************************************************************/
{
	int pant_activa = 0, tecla = 0, lineas = 0, ok = NO;
	char linea1[70], linea2[70], linea3[70], linea4[70];
	char *descripcion=NULL;
	char sql[500];
	int CantIdAProcesar = 0; 
	double importe = 0.00;
	int espacio_extra = 100;
	long id_transaccion = 0L, _caja_z = 0L;

	memset( linea1, 0, sizeof(linea1) );
	memset( linea2, 0 ,sizeof(linea2) );
	memset( linea3, 0 ,sizeof(linea3) );
	memset( linea4, 0 ,sizeof(linea4) );
	memset(sql,0,sizeof(sql));

	id_transaccion = PROXIMO_TRANSACCION_CAMBIO_MEDIO_A_PROCESAR( 0, 0, &CantIdAProcesar );
	
	if( CantIdAProcesar ) {
		_caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
		_snprintf(sql,sizeof(sql)-1,"SELECT SUM(interes) FROM transac2 where enviado = 1 and error = 0 and operacion = 22 and autorizacion > 0 and codigo_de_respuesta = 0 and id_transaccion >= '%ld' and caja_z = '%ld' and ticket NOT IN (SELECT ticket_original FROM transac2 where enviado = 1 and error = 0 and operacion = 23 and codigo_de_respuesta = 0 and caja_z = '%ld')" 
		, id_transaccion, _caja_z, _caja_z );
		if( DIRECT_DB_QUERY( sql) > 0 ) {
			GET_DATO( 1, C_DOUBLE, (char *)&importe, 8 );

			if( importe > 0 ) {
				lineas = strlen("        CAMBIO DE MEDIOS AUTOMATICO")
					+ strlen("&")
					+ strlen("                         CANTIDAD       IMPORTE")
					+ strlen("                                  %d                      %.2lf")
					+ strlen("&")
					+ strlen("&")
					+ strlen("Aceptar <ENTER> - Cancelar <ESCAPE>")
					+ espacio_extra;

				//total de lineas a mostrar en ventana
				if( lineas > 0 )
					descripcion =(char*) malloc(lineas + 100);	

				if( descripcion && lineas ) {
					memset( descripcion, 0 ,lineas);
					//limpio las lineas para solo mostrar las posibles, con valores != 0
					lineas=0;
					MENSAJE("CONFIRME CAMBIO DE MEDIOS AUTOMATICO");	

					//TITULO y centrado
					_snprintf( linea1, sizeof(linea1)-1,"%s", "        CAMBIO DE MEDIOS AUTOMATICO");
					linea1[strlen(linea1)]=0;

					//DESCRIPCION COLUMNAS
					linea2[0]='&';
					_snprintf( &linea2[1], sizeof(linea2)-1,"                         CANTIDAD       IMPORTE");
					linea2[strlen(linea2)]=0;

					//DATOS DE LAS COLUMNAS
					linea3[0]='&';
					linea3[1]='&';

					_snprintf( &linea3[2], sizeof(linea3)-1,"                                  %d                      %.2lf", CantIdAProcesar, importe );
					linea3[strlen(linea3)+1]='&';
					linea3[strlen(linea3)]=0;

					strcat(descripcion,linea2);
					strcat(descripcion,linea3);

					memset( linea4,' ',sizeof(linea4));
					linea4[0]='&';
					linea4[1]='&';
					_snprintf( &linea4[2],sizeof(linea4)-1," %s", "            Procesar <ENTER> - Cancelar <ESCAPE>");
					linea4[strlen(linea4)]=0;
					strcat(descripcion,linea4);

					ExecuteSAction( A_GET_PANEL_ACTIVO, ( char* )&pant_activa );
					ExecuteSSAction( A_PEDIR, _HIBRIDO, linea1, descripcion, NULL, " " );
								
					while( tecla != 13 && tecla != 27 
							/*&& !disable_standby_automatico
							&& !disable_ssaver*/ ) {
						tecla = GETCH();
					}

					//disable_standby_automatico = NO;
					//disable_ssaver = NO;

					ExecuteSAction( A_FOCO_A_PANEL_ACTIVO, ( char* )&pant_activa );
					ExecuteAction( A_FIN_PEDIR );
					BORRAR_MENSAJE();
				}
			
				if( descripcion  ) {
					FREEMEM( ( char* )descripcion );
				}
			} else {
				CantIdAProcesar = 0;
				glog("Importe = 0 en Cambio Medio Automatico",LOG_ERRORES,1);
			} 
		} else {
			CantIdAProcesar = 0;
			glog("Error consulta Cambio Medio Automatico",LOG_ERRORES,1);
		} 
	} 

	if( CantIdAProcesar == 0 ) {
		MENSAJE_TECLA( "No hay Operaciones Pendientes ( Presione una tecla )"  );
	}

	if( tecla == 13 ) {
		MENSAJE_CON_PAUSA("CAMBIO DE MEDIO AUTOMATICO FINALIZADO...", 50 );
		ok = SI;	
	} 

	return ok;

}

/*****************************************************************************/
int CARGAR_CAMBIO_MEDIO_AUTOMATICO( void )
/*****************************************************************************/
{
	int j = 0, h = 0, i = 0, nro = 0;
	int CantIdAProcesar = 0;
	long rta = 0L;
	//Guardamos el id_evento como bandera de impresion.
	if( !RAM_COD_PREMIO || RAM_COD_PREMIO > RAM_ID_EVENTO ) {
		SET_MEMORY_LONG( __ram_cod_premio, RAM_ID_EVENTO );
	} 

	for( j = 0; j < 10; j++ ) {//Como maximo 10 configuraciones de medios en el conftps.xml
		
		if( config_tps.det_CambioMedio[j].MedioEntrante && config_tps.det_CambioMedio[j].MedioSaliente ) {
			nro = config_tps.det_CambioMedio[j].MedioEntrante - 1;
			if( medios[nro].efectivo ) {
				CantIdAProcesar = 0;
			} else if( medios[nro].cheque ) {
				CantIdAProcesar = 0;
			} else if( medios[nro].tarjeta ) {
				CantIdAProcesar = 0;
				rta = PROXIMO_TRANSACCION_CAMBIO_MEDIO_A_PROCESAR( config_tps.det_CambioMedio[j].MedioEntrante,
					config_tps.det_CambioMedio[j].SubmedioEntrante, &CantIdAProcesar );
				rta = _CARGAR_CAMBIO_MEDIO_AUTOMATICO( CantIdAProcesar, j );
			} else if( medios[nro].mutual ) {
				CantIdAProcesar = 0;
			} else if( medios[nro].cta_cte ) {
				CantIdAProcesar = 0;
			} 
		}
	}
	//No se grabo ningun tipo_evento 40
	if( RAM_ID_EVENTO == RAM_COD_PREMIO )
		SET_MEMORY_LONG( __ram_cod_premio, 0);

	return 1;
}

/*****************************************************************************/
int _CARGAR_CAMBIO_MEDIO_AUTOMATICO( int CantIdAProcesar, int j  )
/*****************************************************************************/
{

	int h = 0, i = 0;

	for( h = 0; h < CantIdAProcesar; h++ ) {	

		INGRESAR_CAMBIO_MEDIO_AUTOMATICO( config_tps.det_CambioMedio[j].MedioEntrante - 1,
			config_tps.det_CambioMedio[j].SubmedioEntrante, 
			ENTRAN_MEDIOS );
			
		INGRESAR_CAMBIO_MEDIO_AUTOMATICO( config_tps.det_CambioMedio[j].MedioSaliente - 1,
			config_tps.det_CambioMedio[j].SubmedioSaliente,
			SALEN_MEDIOS );
		
		GRABA_EVENTO_CABECERA_CAMBIO();
		for( i = 0;i < RAM_P_PAGO;i++ ) {
			_GRABA_EVENTO_CAMBIO( _PAGO_MODO( i ) + 1, _PAGO_IMPORTE( i ), _PAGO_ID_REGISTRO( i ),
								  ( char )_PAGO_COD_SUBMEDIO_VUELTO( i ), _PAGO_CUENTA( i ), i );
			CAMBIO_ACUMULAR_CAMBIO_EN_X( _PAGO_MODO( i ), fabs( _PAGO_IMPORTE( i ) ),
										 ( _PAGO_IMPORTE( i ) < 0 ) ? 1 : 0, _PAGO_CUENTA( i ) );
			CAMBIO_GUARDAR_SUBMEDIOS_REC( _PAGO_MODO( i ) + 1, _PAGO_COD_SUBMEDIO_VUELTO( i ), fabs( _PAGO_IMPORTE( i ) ),
										 ( _PAGO_IMPORTE( i ) < 0 ) ? 1 : 0, _PAGO_CUENTA( i ) );
		}

		ACTUALIZA_ID_EVENTO_NEW();

		/*--------------- Inicializar Variables ----------------*/
		//retiro.importe = 0;
		retiro.cod_sub_medio = 0;
		retiro.importe = 0;
		retiro.numero = 0;
		retiro.id_registro =  0;
		i = 0;
		SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
		SET_MEMORY_DOUBLE( __ram_total, 0.0 );
		SET_MEMORY_CHAR( __ram_p_pago, 0 );
		CEREAR_STRUCT( _var_tarjetas );
	}

	return 1;
}

/*****************************************************************************/
void INGRESAR_CAMBIO_MEDIO_AUTOMATICO( int nro, int submedio, int tipo )
/*****************************************************************************/
{
    long numero = 0;
    unsigned fecha_vto = 0;
    int banco = 0, tarjeta = 0;
    char numero_tar[21],cuit[12],telefono[12],cuenta_banco[20];
    int cod_post_cheq = 0;
    int cod_hora_acre = 0;

	retiro.modo = nro;
    retiro.id_registro = medios[nro].id_registro;
    //retiro.importe = 0;
    retiro.numero = 0;
    numero_tar[0] = 0;
    cuit[0] = 0;
    telefono[0] = 0;
    cuenta_banco[0] = 0;
    _en_cambio_de_medios = 1;
	SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
	
    if( medios[nro].efectivo ) {
        /*-------------------- Si es efectivo ----------------------*/
		if( tipo == ENTRAN_MEDIOS ) {
			//Hay de definir de donde van ha salir los importes entrantes de los medios distintos de tarjeta
			retiro.importe = 0;
		} else {
			retiro.importe = retiro.importe;//El importe que entra es el que sale...Hay que verificar esto
		}
		retiro.cod_sub_medio = 0;
    } else if( medios[nro].cheque ) {
        /*-------------------- Si es cheque -----------------------*/
		if( tipo == ENTRAN_MEDIOS ) {
			retiro.importe = 0;
		} else {
			retiro.importe = 0;//retiro.importe;
		}
        retiro.cod_sub_medio = 0;
	} else if( medios[nro].tarjeta ) {
        /*---------------- Si es tarjeta de credito ----------------*/
		if( tipo == ENTRAN_MEDIOS ) {//En PROCESAR_CAMBIO_MEDIOS_AUTOMATICO se valido Cambio de Medios Pendientes
			CARGAR_DATOS_CAMBIO_MEDIO( nro, submedio );
		} else {
			SET_MEMORY_CHAR( __var_tarjetas_tarjeta, submedio );
			SET_MEMORY_FLOAT( __var_tarjetas_anticipo, retiro.importe );
		}
		retiro.cod_sub_medio = VAR_TARJETAS_TARJETA;
		retiro.importe = VAR_TARJETAS_ANTICIPO;
		retiro.numero = VAR_TARJETAS_TARJETA;
		if( DIRECT_DB_QUERY( "SELECT id_registro FROM tarjetas WHERE cod_tarjeta = '%d'", retiro.cod_sub_medio ) > 0 ) {
			GET_DATO( 1, C_INT, (char *)&retiro.id_registro, 4 );
		}
		tarjeta = VAR_TARJETAS_TARJETA;
        strcpy( numero_tar, GET_NUMERO_TARJETA_PRIVADO());
    } else if( medios[nro].cta_cte ) {
        /*--------------------- Si es cuenta corriente -----------------*/
		if( tipo == ENTRAN_MEDIOS ) {
			retiro.importe = 0;
		} else {
			retiro.importe = 0;//retiro.importe;
		}
        retiro.cod_sub_medio = 0;

    } else if( medios[nro].ticket_compra ) {
        /*--------------------- Si es ticket de compra -----------------*/
		if( tipo == ENTRAN_MEDIOS ) {
			retiro.importe = 0;
		} else {
			retiro.importe = 0;//retiro.importe;
		}
		retiro.numero = submedio;
		if( DIRECT_DB_QUERY( "SELECT id_registro FROM t_compra WHERE cod_t_compra = '%d'", submedio ) > 0 ) {
			GET_DATO( 1, C_INT, (char *)&retiro.id_registro, 4 );
		}
		retiro.cod_sub_medio = ( char )retiro.numero;

	} else if( medios[nro].mutual ) {
        /*------------------------ Si es mutual ------------------------*/
		if( tipo == ENTRAN_MEDIOS ) {
			retiro.importe = 0;
		} else {
			retiro.importe = 0;//retiro.importe;
		}		
		retiro.numero = submedio;
		if( DIRECT_DB_QUERY( "SELECT id_registro FROM mutuales WHERE cod_mutual = '%d'", submedio ) > 0 ) {
			GET_DATO( 1, C_INT, (char *)&retiro.id_registro, 4 );
		}
		retiro.cod_sub_medio = ( char )retiro.numero;
    } 

    /*------------------ si todo OK agrega el retiro --------------------*/
	 if( retiro.importe > 0 ) {	
		 		CAMBIO_AGREGAR_MEDIO( 1/*AUTOMATICO*/, tipo, numero/*NRO CHEQUE O NRO TICKET COMPRA*/,
				banco/*CHEQUE*/, fecha_vto/*CHEQUE*/, cod_post_cheq/*CHEQUE*/, cod_hora_acre/*CHEQUE*/, cuit/*CHEQUE*/,
				telefono/*CHEQUE*/, cuenta_banco/*CHEQUE*/, tarjeta/*TARJETA*/, numero_tar/*TARJETA*/, 
				retiro.numero /*TARJETA*/);
    }

    _en_cambio_de_medios = 0;
    //BORRAR_MENSAJE_(SI);

}

/*****************************************************************************/
int CARGAR_DATOS_CAMBIO_MEDIO( int medio, int submedio )
/*****************************************************************************/
{
    int ok = 0;
	long id_evento = 0L, _caja_z = 0L, id_transaccion = 0L;
    struct _datos_transaccion tran;
	int marca_tarjeta = 0, cantticket = 0;
    int area_ant = 0, tipo_ant = 0, nro_ticket = 0;
	char sql[500];

	memset(sql,0,sizeof(sql));

	area_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();

	CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
    
	if( T_ABRIR_TRANSAC2() ) {
	}
	
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );

	id_transaccion = PROXIMO_TRANSACCION_CAMBIO_MEDIO_A_PROCESAR( medio, submedio, &cantticket );

	_caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
	_snprintf(sql,sizeof(sql)-1,
	"enviado = 1 and error = 0 and operacion = 22 and autorizacion > 0 and codigo_de_respuesta = 0 and id_transaccion = '%d' and caja_z = '%ld' and ticket NOT IN (SELECT ticket_original FROM transac2 where enviado = 1 and error = 0 and operacion = 23 and codigo_de_respuesta = 0 and caja_z = '%ld')"
	, id_transaccion, _caja_z, _caja_z  );
	SET_WHERE( sql );

	RUN_QUERY(NO);

	if( FOUND2() ) { //encontramos la operacion 
		long nro = transac2->id_transaccion;
		//*importe_t = transac2->dt.importe;
		//*nodo_t = transac2->nodo;
        memcpy( &tran, &transac2->dt, sizeof( struct _datos_transaccion ) );
		    
		CARGAR_DATOS_BASICOS( &tran, SI );
		//cargar datos transaccion
/*		tran.ticket_original = transac2->dt.ticket;
		tran.tipo_de_mensaje = TIPO_MENSAJE_COMPRA;
		tran.tipo_de_mensaje_original = transac2->dt.tipo_de_mensaje;*/
		id_evento = transac2->dt.id_evento; 
		marca_tarjeta = transac2->dt.codigo_marca + 1;
		//*cuota = transac2->dt.cuotas;
		//*fecha = transac2->fecha_generacion;
		//SET_MEMORY_DOUBLE( __var_tarjetas_importe, transac2->dt.importe );
		SET_MEMORY_INT( __var_tarjetas_operacion, transac2->operacion);
		if( transac2->operacion == _ON_LINE_CASHPLUS ) {//la anulacion de cashplus tiene que mandar el importe de retiro
			SET_MEMORY_FLOAT( __var_tarjetas_anticipo, (float) tran.interes );
		}
		ok = 1;
	}

	SELECT_TABLE(area_ant, tipo_ant);

	return ok;
}

/*****************************************************************************/
long PROXIMO_TRANSACCION_CAMBIO_MEDIO_A_PROCESAR( int medio, int submedio, int *CantIdSinProcesar )
/*****************************************************************************/
{
	int id_evento = -999, nro_cupon = -999;
	int area_ant = 0, tipo_ant = 0, nro_ticket =  0;
	char sql[500];
	long id_transaccion = 0L, _caja_z = 0L;

	memset(sql,0,sizeof(sql));

	area_ant = SELECTED2();
    tipo_ant = SELECTED_TIPO();

	//Busco el maximo id_evento tipo 40 que ya fue procesado
	_snprintf(sql,sizeof(sql)-1,
		"SELECT max(id_evento) FROM eventos where tipo_evento = 40" );
		DIRECT_DB_QUERY( sql);
		GET_DATO(1,C_INT,(char *)&id_evento,4);

	if( id_evento != -999 ) {
		//Busco el numero_id (nro de ticket al que se le hace el cambio de medio)
		//y verifico que exista ese ticket en otra operacion distinta de 40
		memset(sql,0,sizeof(sql));
		_snprintf(sql,sizeof(sql)-1,
			"SELECT numero_id FROM ev_medios where id_evento='%ld' and modo_pago = 2 and posicion =0", id_evento );
			DIRECT_DB_QUERY( sql);
			GET_DATO(1,C_INT,(char *)&nro_cupon,4);
		if( nro_cupon != -999 ) {
			//Este nro_cupon es el ultimo nro ticket correspondiente a una extraccion procesada.
			//Apartir de este nro de ticket los posteriores extracciones no fueron procesadas.
			//Busco el id_evento para procesar los proximos.
			_caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
			id_evento = 0;
			memset(sql,0,sizeof(sql));
			_snprintf(sql,sizeof(sql)-1,
				"SELECT id_evento FROM transac2 where ticket ='%ld' and enviado = 1 and error = 0 and operacion = 22 and autorizacion > 0 and codigo_de_respuesta = 0 and caja_z = '%ld' and ticket NOT IN (SELECT ticket_original FROM transac2 where enviado = 1 and error = 0 and operacion = 23 and codigo_de_respuesta = 0 and caja_z = '%ld')" 
				, nro_cupon, _caja_z, _caja_z );
				DIRECT_DB_QUERY( sql);
				GET_DATO(1,C_INT,(char *)&id_evento,4);
		} //Si no existe el cupon dejo el id_evento con el valor de la eventos.
	} else {
		id_evento = 0;
	}

	CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
		    
	if( T_ABRIR_TRANSAC2() ) {
	}
	_caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
	_snprintf(sql,sizeof(sql)-1,
		"id_evento > '%ld' and enviado = 1 and error = 0 and operacion = 22 and autorizacion > 0 and codigo_de_respuesta = 0 and caja_z = '%ld' and ticket NOT IN (SELECT ticket_original FROM transac2 where enviado = 1 and error = 0 and operacion = 23 and codigo_de_respuesta = 0 and caja_z = '%ld')",
		id_evento, _caja_z, _caja_z );

	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE( sql );
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
	*CantIdSinProcesar = RECCOUNT2(NULL, NULL);
	if( CantIdSinProcesar )
		//nro_ticket = transac2->dt.ticket;//El primero de la consulta.
		id_transaccion = transac2->id_transaccion;//Clave primaria no se repite


	SELECT_TABLE(area_ant, tipo_ant);

	return( id_transaccion );
}

/*****************************************************************************/
void CAMBIO_CONFECCIONA_PANTALLA( int tipo )
/*****************************************************************************/
{
    int h;
    char tt[30];
    ExecuteAction( A_DES_VENTAS_ACT_CAMBIOMEDIO );
    ExecuteSAction( A_MUESTRA_TITULO_FFIJO, ST( S_CAMBIO_MEDIOS ) );
	ExecuteAction( A_BORRAR_MEDIOS );

    for( h = 0;h < RAM_P_PAGO;h++ ) {
        if( _PAGO_CUENTA( h ) == tipo ) {
            CAMBIO_MOSTRAR_MEDIO( _PAGO_MODO( h ), _PAGO_IMPORTE( h ), _PAGO_NUMERO( h ),
                ( _PAGO_IMPORTE( h ) < 0 ) ? 1 : 0, _PAGO_IMPORTE_ME( h ) );
        }
    }
    if( !ancho_panel ) {
        /*------------ muestra medios de retiro ------------*/
		
        for( h = 0;h < 9;h++ ) {
            if( medios[h].nro /*&& !medios[h].moneda_extranjera*/ ) {
                if( PERMITIR_MEDIO( h, tipo ) ) {
                    #if defined(INVEL_W) || defined(INVEL_L)
                    #ifdef ICONO_MEDIO
                    memset( tt, ' ', sizeof( tt ) );               
                    strcpy( tt, medios[h].nombre );
                    switch( medios[h].nro ) {
                        case MEDIO_1:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_1_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_2:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_2_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_3:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_3_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_4:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_4_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_5:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_5_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_6:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_6_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_7:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_7_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_8:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_8_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_9:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_9_AGREGAR, " ", tt, NULL, " " );
                            break;
                        case MEDIO_10:
                            ExecuteSSAction( A_AGREGAR_MEDIO, T_MEDIO_10_AGREGAR, " ", tt, NULL, " " );
                            break;
                        default:
                            GRABAR_LOG_SISTEMA( "MEDIO NO DEFINIDO" ,LOG_VENTAS,2);
                    }
                    #else
                    sprintf( tt, "%2d  ", medios[h].nro );
                    strcat( tt, medios[h].nombre );
                    ExecuteSAction( A_AGREGAR_MEDIO, tt );
                    #endif
                    #endif
                    MOSTRAR_ENTERO( 7, 9 + h, "9", medios[h].nro );
                    MOSTRAR_CADENA( 9, 9 + h, medios[h].nombre );
                }
				else {
					
				}
			} else {
			
			}
        }
    }
    CAMBIO_MUESTRA_TOTAL( tipo, 0);
    _ATRIBUTO = 112;
}
/*****************************************************************************/
int CAMBIO_PIDE_MEDIOS( int tipo )
/*****************************************************************************/
{
    int seguir = 1, k, ok = 1;
    Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
	
    while( seguir ) {
		BORRAR_MENSAJE();
        k = GETCH();
        switch( k ) {
            case 32:
                /*-------- TOTAL -------*/
                if( tipo == SALEN_MEDIOS ) {
                    if( fabs( RAM_TOTAL ) < 0.01 ) {
                        seguir = 0;
                        CLOSE_COPIA_PRN();
                        if( IMPRIME_RETIROS == _ESTACION_JOURNAL_RECEIPT
                         || IMPRIME_RETIROS == _ESTACION_SLIP ) {
                            CAMBIO_SET_ACU_RETIRADO();
                            //IMPRIME_PIE_RETIROS( acu_retirado, 0.0, NO, NULL, NO );
                            //IMPRIME_CABECERA_RETIROS( 3, NO );
                            //IMPRIME_COPIA_PRN();
                            //IMPRIME_PIE_RETIROS( acu_retirado, 0.0, NO, NULL, NO );
                        }
                        ADD_MEMORY_UINT( __nro_nro_retiro, 1 );
                        ACTUALIZAR_X();
                    } else {
                        char cad[100];
                        _snprintf(cad, sizeof(cad)-1, "Resto: $%0.2lf (Presione <ESC> para Cancelar)", RAM_TOTAL );
                        MENSAJE( cad );
                    }
                }
                else {
                    if( RAM_TOTAL > 0.005 ) {
                        seguir = 0;
                    }
                }
                break;
            case 27:
                /*-------- ESC -------*/
                seguir = 0;
                ok = 0;
				anulacion = 1;
				CAMBIO_ACTIVA_ANULACION();
				CAMBIO_MOSTRAR_ANULACION();
				ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
                break;
            case 45:
                /*-------- '-' : Anulacion ------*/
                CAMBIO_ACTIVA_ANULACION();
                CAMBIO_MOSTRAR_ANULACION();
                break;
			case 172: //ojo esta tecla se deberia solo activar si hay un medio de pago cashplus
				if( IsCashActivo() )
					ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
				else{ 
					ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_AGREGAR, "RETIRO ACTIVADO", " ", NULL, NULL ); 
				}
				
				break;
            case FLECHA_ARRIBA:
                if( Orden > NUMERO_PANEL_MINIMO_EN_RETIROS ) {
                    Orden--;
                    DestroyPanel( pPanel );
                    PreparePanel( &pPanel, Orden );
                    ShowPanel( pPanel );
                };
                break;
            case FLECHA_ABAJO:
                if( Orden < NUMERO_PANEL_MAXIMO_EN_RETIROS ) {
                    Orden++;
                    DestroyPanel( pPanel );
                    Response = PreparePanel( &pPanel, Orden );
                    if( Response == FALSE ) {
                        Orden--;
                        PreparePanel( &pPanel, Orden );
                    };
                    ShowPanel( pPanel );
                }
                break;
            default:
                if( k >= '1' && k <= '9' ) {
                    if( medios[k - '1'].nro /*&& !medios[k - '1'].moneda_extranjera*/
                     && PERMITIR_MEDIO( ( k - '1' ), tipo ) ) {
                        if( RAM_P_PAGO < 9
                         && ( tipo == ENTRAN_MEDIOS
                           || ( ( tipo == SALEN_MEDIOS && RAM_TOTAL > 0 ) || anulacion ) ) ) {
                            #if defined(INVEL_W) || defined(INVEL_L)
                            ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__IMPORTE_ ), "", NULL, " " );
                            ExecuteSSAction( A_RESALTAR_MEDIO_PCM, k - '1', "", "", NULL, " " );
                            #endif
                            CAMBIO_INGRESA_MEDIO( k - '1', tipo );
                            #if defined(INVEL_W) || defined(INVEL_L)
                            ExecuteSSAction( A_MUESTRA_TITULO_BOX, 0, ST( S__IMPORTE_ ), "", NULL, " " );
                            ExecuteAction( A_DESELECCIONA_MEDIO );
                            #endif
                        }
                    }
                }
                break;
        }
    }
    if( Response ) {
        DestroyPanel( pPanel );
    }
    return ( ok );
}

/*****************************************************************************/
void CAMBIO_INGRESA_MEDIO( int nro, int tipo )
/*****************************************************************************/
{
    long numero = 0;
    unsigned fecha_vto = 0;
    int banco = 0, tarjeta = 0;
    char numero_tar[21],cuit[12],telefono[12],cuenta_banco[20];
    int cod_post_cheq = 0;
    int cod_hora_acre = 0;
	int datos_adicionales = 0;
    int plaza = 0;
    int cod_valida_cheque = 0;
    retiro.modo = nro;
    retiro.importe = 0;
    retiro.numero = 0;
    retiro.id_registro = medios[nro].id_registro;
    numero_tar[0] = 0;
    cuit[0] = 0;
    telefono[0] = 0;
    cuenta_banco[0] = 0;
    _en_cambio_de_medios = 1;
    DestroyPanel( pPanel );
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
	
	ExecuteSAction( A_MUESTRA_TITULO_BROWSE, medios[nro].nombre );
    if( medios[nro].efectivo ) {
        /*-------------------- Si es efectivo ----------------------*/
        Orden = NUMERO_PANEL_BILLETES;
        PreparePanel( &pPanel, Orden );
        ShowPanel( pPanel );
		retiro.importe = CAMBIO_PIDE_IMPORTE( medios[nro].cod_solicitud, RAM_P_PAGO );	// Ahv
        DestroyPanel( pPanel );
        if( retiro.importe > 0 ) {
            retiro.importe *= RAM_CANTIDAD;
            if( !anulacion ) {
                if( tipo == SALEN_MEDIOS && CONTROL_DE_IMPORTES_EN_RETIROS
                 && ( retiro.importe
                    > ( X_TOT_DINERO /*+ X_FONDO_FIJO_IMPORTE - X_RETIRO_FONDO_IMPORTE*/ ) ) ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
            }
        }
        retiro.cod_sub_medio = 0;
    } else if( medios[nro].cheque ) {
        /*-------------------- Si es cheque -----------------------*/
		retiro.importe = CAMBIO_PIDE_IMPORTE( medios[nro].cod_solicitud, RAM_P_PAGO ) * RAM_CANTIDAD;
        retiro.cod_sub_medio = 0;
        if( retiro.importe > 0 && !anulacion ) {
            if( !PIDE_DATOS_CHEQUE( medios[nro].al_dia, &numero, &banco, &fecha_vto, &cod_post_cheq,
                                    &cod_hora_acre, cuit, telefono, cuenta_banco,
                                    &cod_valida_cheque, &plaza, NO ) ) {
                retiro.importe = -1;
            }
        }
	}  else if( medios[nro].tarjeta ) {
        /*---------------- Si es tarjeta de credito ----------------*/
        retiro.cod_sub_medio = 0;
		if( anulacion ) {
            retiro.numero = PIDE_TARJETA( &retiro.id_registro, SI );
			if ( !PERMITE_SUBMEDIO( medios[nro].nro, retiro.numero, tipo ) ) {
				MENSAJE("SUBMEDIO NO HABILITADO");			
				Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
				PreparePanel( &pPanel, Orden );
				ShowPanel( pPanel );
				return;
			}
            retiro.cod_sub_medio = ( char )retiro.numero;
            if( retiro.numero > 0 ) {
				retiro.importe = CAMBIO_PIDE_IMPORTE( medios[nro].cod_solicitud, RAM_P_PAGO ) * RAM_CANTIDAD;
            }
        } else {
			if(PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI && config_tps.ActivarCashPlus == 1 &&  IsCashActivo() ) {
				int resultado;
				retiro.importe = CAMBIO_PIDE_IMPORTE( medios[nro].cod_solicitud, RAM_P_PAGO ) * RAM_CANTIDAD;
				if( retiro.importe >0.00) {
					resultado = ANULACION_TARJETA_TECLA(SI);
					if( resultado == 0) {
						Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
						PreparePanel( &pPanel, Orden );
						ShowPanel( pPanel );
						return;
					}
					retiro.numero = VAR_TARJETAS_TARJETA + 1;
					SET_MEMORY_CHAR( __var_tarjetas_status, 2 );
					retiro.cod_sub_medio = ( char )retiro.numero;
                    tarjeta = VAR_TARJETAS_TARJETA;
                    strcpy( numero_tar, GET_NUMERO_TARJETA_PRIVADO());
					if( retiro.importe >0.00 && tipo == ENTRAN_MEDIOS   ) {
						if ( !PERMITE_SUBMEDIO( medios[nro].nro, retiro.numero, tipo ) ) {
							MENSAJE("SUBMEDIO NO HABILITADO");			
							Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
							PreparePanel( &pPanel, Orden );
							ShowPanel( pPanel );
							return;
						}
						if( retiro.importe >0.00 && TARJ[retiro.numero - 1].tipo_tarjeta & _TIPO_CASH_ADVANCE && VAR_TARJETAS_STATUS  ) {

							SET_MEMORY_DOUBLE( __var_tarjetas_vuelto, retiro.importe );
							SET_MEMORY_FLOAT( __var_tarjetas_anticipo, (float) retiro.importe );
					//		MENSAJE("INGRESE EL MONTO DEL CAMBIO");			

						} else {
							MENSAJE("TARJETA NO ADMITE RETIROS O IMPORTE 0");			
							Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
							PreparePanel( &pPanel, Orden );
							ShowPanel( pPanel );
							return;
						}

					}
				}
			}


			if(PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI && config_tps.ActivarCashPlus == 0 ) {
				//|| (PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI && config_tps.ActivarCashPlus == 1 &&  GetNumerRowsCash() > 0 )) {
                if( VALIDA_TARJETA( NO, NO, CAMB_MEDIOS, NO, NULL, NO, nro/*, NO*/) ) {
					NUMERO_DE_COMERCIO_TARJETA( );
                    retiro.numero = VAR_TARJETAS_TARJETA + 1;
					SET_MEMORY_CHAR( __var_tarjetas_status, 2 );
					/*Solo para impresion luego se vuelve a su valor original*/
					/*if( VAR_TARJETAS_FECHA_VTO ) {
						aux_fecha = VAR_TARJETAS_FECHA_VTO;
						SET_MEMORY_INT( __var_tarjetas_fecha_vto, DECREMENTAR_MES( aux_fecha ));
					}*/
					if ( !PERMITE_SUBMEDIO( medios[nro].nro, retiro.numero, tipo ) ) {
						MENSAJE("SUBMEDIO NO HABILITADO");			
						Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
						PreparePanel( &pPanel, Orden );
						ShowPanel( pPanel );
						return;
					}
                    retiro.cod_sub_medio = ( char )retiro.numero;
                    tarjeta = VAR_TARJETAS_TARJETA;
                    strcpy( numero_tar, GET_NUMERO_TARJETA_PRIVADO());
                    if( retiro.numero > 0 ) {
						if( tipo == ENTRAN_MEDIOS  ) {
							if( TARJ[retiro.numero - 1].tipo_tarjeta & _TIPO_CASH_ADVANCE ) {
								int resultado;
								SET_MEMORY_DOUBLE( __var_tarjetas_vuelto, retiro.importe );
								MENSAJE("INGRESE EL MONTO DEL CAMBIO");			
								resultado = ANULACION_TARJETA_TECLA(NO);
							} 
						}
						

						retiro.importe = CAMBIO_PIDE_IMPORTE( medios[nro].cod_solicitud, RAM_P_PAGO ) * RAM_CANTIDAD;
                    }
			
					
					if( tipo == ENTRAN_MEDIOS && TARJ[retiro.numero - 1].permite_consulta_saldo == 'S' ) {
						if( !CONSULTA_SALDO_TARJETAS( retiro.importe ) ) {
						//borrar todas las cosas!!!
							CEREAR_STRUCT( _var_tarjetas );
							MENSAJE("SALDO INSUFICIENTE");			
							Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
							PreparePanel( &pPanel, Orden );
							ShowPanel( pPanel );
							return;
						}
					}
					
					if( tipo == SALEN_MEDIOS){
                        if(!medios[nro].tarjeta){
						    double importeTmp;
						    importeTmp = OBTENER_IMPORTE_SUBMEDIO( nro + 1, retiro.cod_sub_medio, 0 );
						    if( retiro.importe > importeTmp ) {
							    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
							    //retiro.importe = -1;
							    return;
						    }
                        }
					}
					if( tipo == ENTRAN_MEDIOS && VAR_TARJETAS_STATUS ) {
						if( TARJ[retiro.numero - 1].tipo_tarjeta & _TIPO_CASH_ADVANCE ) {
							SET_MEMORY_DOUBLE( __var_tarjetas_vuelto, retiro.importe );
							SET_MEMORY_FLOAT( __var_tarjetas_anticipo, (float) retiro.importe );
						}
					}
                } else {
                    retiro.importe = -1;
                }
			} else {
					if(!(PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI 
						&& config_tps.ActivarCashPlus == 1 
						&&  IsCashActivo() )) 
						retiro.importe = CAMBIO_PIDE_IMPORTE( medios[nro].cod_solicitud, RAM_P_PAGO ) * RAM_CANTIDAD;
            }
		}
    } else if( medios[nro].cta_cte ) {
        /*--------------------- Si es cuenta corriente -----------------*/
		retiro.importe = CAMBIO_PIDE_IMPORTE( medios[nro].cod_solicitud, RAM_P_PAGO ) * RAM_CANTIDAD;
        retiro.cod_sub_medio = 0;
    } else if( medios[nro].ticket_compra ) {
        /*--------------------- Si es ticket de compra -----------------*/
        retiro.cod_sub_medio = 0;
        if( PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI ) {
            retiro.numero = PIDE_TICKET_COMPRA( &retiro.id_registro, NULL, &numero, NULL, NULL,
                                                NULL, &datos_adicionales, medios[nro].nro, NO,
                                                NULL, SI );
            retiro.cod_sub_medio = ( char )retiro.numero;
			if ( !PERMITE_SUBMEDIO( medios[nro].nro, retiro.numero, tipo ) ) {
				MENSAJE("SUBMEDIO NO HABILITADO");			
				Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
				PreparePanel( &pPanel, Orden );
				ShowPanel( pPanel );
				return;
			}
        }
        if( retiro.numero > 0 || !PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI ) {
			retiro.importe = CAMBIO_PIDE_IMPORTE( datos_adicionales, RAM_P_PAGO ) * RAM_CANTIDAD;
        }
        #ifdef COMPILAR_MUTUALES
    } else if( medios[nro].mutual ) {
        /*------------------------ Si es mutual ------------------------*/
        retiro.cod_sub_medio = 0;
        if( PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI ) {
			double monto = 0.00;

			if( tipo == SALEN_MEDIOS ) {
				monto = RAM_TOTAL;
			}           
			retiro.numero = PIDE_MUTUAL( &retiro.id_registro, NULL, NULL, NULL, NULL, NULL,
				&datos_adicionales, medios[nro].nro, monto, NO, NULL, SI );
            retiro.cod_sub_medio = ( char )retiro.numero;
			if ( !PERMITE_SUBMEDIO( medios[nro].nro, retiro.numero, tipo ) ) {
				MENSAJE("SUBMEDIO NO HABILITADO");
				Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
				PreparePanel( &pPanel, Orden );
				ShowPanel( pPanel );
				return;
			}
        }

		if( retiro.numero > 0 || !PEDIR_SUBMEDIO_EN_CAMBIO_DE_MEDI ) {
			double cot = 1.0;
			COTIZACION_MUTUAL( retiro.cod_sub_medio, &cot, NULL );
			retiro.importe = ( CAMBIO_PIDE_IMPORTE( datos_adicionales, RAM_P_PAGO ) * RAM_CANTIDAD ) * cot;
		}  
		#endif
    } else {
        /*--------------------- Si es otro medio -----------------------*/
		datos_adicionales = medios[nro].cod_solicitud;
        retiro.importe = CAMBIO_PIDE_IMPORTE( datos_adicionales, RAM_P_PAGO ) * RAM_CANTIDAD;
        retiro.cod_sub_medio = 0;
        if( retiro.importe > 0 ) {
			if( medios[nro].moneda_extranjera ){//lo convierto al medio definido como efectivo
				retiro.importe_me = retiro.importe;
				retiro.importe = PAGOS_ME_A_ML( retiro.importe_me, nro );
			}
            retiro.importe *= RAM_CANTIDAD;
            if( !anulacion ) {
                if( tipo == SALEN_MEDIOS && CONTROL_DE_IMPORTES_EN_RETIROS
                 && ( retiro.importe > _X_VENTAS_IMPORTE( nro ) + _X_COBROS_IMPORTE( nro )
                    - _X_RETIROS_IMPORTE( nro ) + 0.02 ) ) {
                    MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                    retiro.importe = 0;
                }
            }
        }
    }
    if( retiro.importe > 0 && anulacion
     && !VALIDA_ANULACION_MEDIO( retiro.modo + 1, retiro.cod_sub_medio, retiro.importe, tipo ) ) {
        MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
        retiro.importe = 0;
    }
    /*------------------ si todo OK agrega el retiro --------------------*/
	 if( retiro.importe > 0 ) {
		 if( SOLICITAR_SUPERVISOR_CAMBIO_M( medios[nro].nro, retiro.numero, tipo ) ) {
			
			CAMBIO_AGREGAR_MEDIO( 0, tipo, numero, banco, fecha_vto, cod_post_cheq, cod_hora_acre, cuit,
								telefono, cuenta_banco, tarjeta, numero_tar, retiro.numero );
			CAMBIO_MUESTRA_TOTAL( tipo, nro);
		 } else {
			Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
			PreparePanel( &pPanel, Orden );
			ShowPanel( pPanel );
			return;
		 }
    }
    if( anulacion ) {
        CAMBIO_ACTIVA_ANULACION();
    }
    _en_cambio_de_medios = 0;
    BORRAR_MENSAJE_(SI);
    CAMBIO_MOSTRAR_ANULACION();
    Orden = NUMERO_PANEL_MINIMO_EN_RETIROS;
    PreparePanel( &pPanel, Orden );
    ShowPanel( pPanel );
}

/*****************************************************************************/
void CAMBIO_AGREGAR_MEDIO( int modalidad, int tipo, long numero, int banco, int fecha_vto, int c_postal_c,
                           int c_hora_c, char *cuit, char *telefono, char *cuenta_banco,
                           int tarjeta, char *numero_tar, long cod_sub_medio )
/*****************************************************************************/
{
    int signo = 0, pos = 0;
	pos = RAM_P_PAGO;
	
	if( medios[retiro.modo].moneda_extranjera ){//lo convierto al medio definido como efectivo
		_SET_MEMORY_DOUBLE( __pago_importe_me, pos, retiro.importe_me );
	}

    /*---------- muestra datos ---------*/
	if( modalidad == 0 /*MANUAL*/) {
		CAMBIO_MOSTRAR_MEDIO( retiro.modo, retiro.importe, retiro.numero, anulacion, retiro.importe_me);
	}

   
    _SET_MEMORY_CHAR( __pago_modo, pos, retiro.modo );
    _SET_MEMORY_DOUBLE( __pago_importe, pos, ( anulacion ) ? -retiro.importe : retiro.importe );
    _SET_MEMORY_LONG( __pago_numero, pos, retiro.numero );
    _SET_MEMORY_CHAR( __pago_cod_submedio_vuelto, pos, retiro.cod_sub_medio );
    _SET_MEMORY_CHAR( __pago_id_registro, pos, retiro.id_registro );
    _SET_MEMORY_INT( __pago_cuenta, pos, tipo );
	_SET_MEMORY_INT( __pago_cod_submedio, pos, cod_sub_medio );
    _SET_MEMORY_CHAR( __pago_tarjeta, pos, tarjeta );
    //_SET_MEMORY_INT(__pago_cuenta, pos, cuenta);
    //_SET_MEMORY_LONG(__pago_buffer1_long, pos, autorizacion);
    //_SET_MEMORY_INT(__pago_banco, pos, (banco) ? banco : comprobante);
    _SET_MEMORY_INT( __pago_banco, pos, banco );
    _SET_MEMORY_LONG( __pago_numero, pos, numero );
    _SET_MEMORY_INT( __pago_fecha_vto, pos, fecha_vto );
    //_SET_MEMORY_INT(__pago_cantidad_cupones, pos, cantidad_cupones);
    //_SET_MEMORY_DOUBLE(__pago_valor_cupones, pos, valor_cupones);
    _SET_MEMORY_INT( __pago_cod_post_cheq, pos, c_postal_c );
    _SET_MEMORY_INT( __pago_cod_hora_acre, pos, c_hora_c );
    _SET_MEMORY( __pago_nro_tarjeta, pos, numero_tar );
    _SET_MEMORY( __pago_cuit, pos, cuit );
    _SET_MEMORY( __pago_telefono, pos, telefono );
    if( medios[retiro.modo].cheque ) {
        _SET_MEMORY( __pago_buffer, pos, cuenta_banco );
    }    
	
	if( modalidad == 0 /*MANUAL*/) {
		if( medios[retiro.modo].tarjeta ) {	
			if( tipo == ENTRAN_MEDIOS ) {
				GRABA_MODIFICACION_TARJETAS( 0, TARJETA_TRADICIONAL );
			} else if( tipo == SALEN_MEDIOS ) {		
				GRABA_MODIFICACION_TARJETAS( VARIOS_TARJETAS_POR_PAGO, TARJETA_COBRO );
				ADD_MEMORY_INT( __varios_tarjetas_por_pago, 1 );
			}
		}
	} else {
		_SET_MEMORY_LONG( __pago_nro_cupon_int1, pos, VAR_TARJETAS_NRO_CUPON_INT );
	}

	ADD_MEMORY_CHAR( __ram_p_pago, 1 );
	signo = ( anulacion ) ? -1 : 1;
	if( tipo == SALEN_MEDIOS ) {
		signo *= -1;
	}
	ADD_MEMORY_DOUBLE( __ram_total, retiro.importe * signo );
	//CAMBIO_IMPRIME_MEDIO( tipo );

}
/*****************************************************************************/
void CAMBIO_MOSTRAR_MEDIO( int modo, double importe, long numero, int anulacion, double importe_me )
/*****************************************************************************/
{
    int larray = ENTEROS + 1 + DECIMALES + 5, ok = 1;
    char *pr,*temp;
    pr = ( char* )calloc( 1, larray );
    temp = ( char* )calloc( 1, sizeof( medios[modo].nombre ) + 2 );
    if( pr != NULL && temp != NULL ) {
        memset( pr, 0, larray );
        memset( temp, 0, sizeof( medios[modo].nombre ) + 2 );
    }
    else {
        GRABAR_LOG_SISTEMA( "Sin memoria para pantallas" ,LOG_ERRORES,1);
        ok = 0;
    }
    if( ok ) {
        //      SCROLL( 28 - ancho_panel / 2, 9, 58 - ancho_panel / 2, 18, -1 );
        FTOC( importe, pr, ENTEROS + 1, DECIMALES, 1, 1 );
        if( anulacion ) {
            strcat( temp, "-" );
        }
        strcat( temp, medios[modo].nombre );
        agregarPago( /*( int )importe*/0, temp, pr, "", "" );
        FREEMEM( pr );
        FREEMEM( temp );
    }
}
/*****************************************************************************/
void CAMBIO_ACUMULAR_CAMBIO_EN_X( int modo, double importe, int anulacion, int tipo )
/*****************************************************************************/
{
    int signo;
    /*--------- actualiza acumuladores -----------*/
    signo = ( tipo == SALEN_MEDIOS ) ? -1 : 1;
    if( anulacion ) {
        _ADD_MEMORY_DOUBLE( __x_cambios_importe, modo, -importe * signo );
        _ADD_MEMORY_INT( __x_cambios_cantidad, modo, -1 );
        if( medios[modo].efectivo ) {
            ADD_MEMORY_DOUBLE( __x_tot_dinero, -importe * signo );
        }
    }
    else {
        _ADD_MEMORY_DOUBLE( __x_cambios_importe, modo, importe * signo );
        _ADD_MEMORY_INT( __x_cambios_cantidad, modo, 1 );
        if( medios[modo].efectivo ) {
            ADD_MEMORY_DOUBLE( __x_tot_dinero, importe * signo );
        }
    }
}
/*****************************************************************************/
void CAMBIO_GUARDAR_SUBMEDIOS_REC( int medio, int submedio, double importe, int anulacion, int tipo )
/*****************************************************************************/
{
    int signo;

    /*--------- actualiza acumuladores -----------*/
    signo = ( tipo == SALEN_MEDIOS ) ? -1 : 1;
    if ( anulacion ) {
        signo *= -1;
    }

    SUMAR_IMPORTE_SUBMEDIO( medio, submedio, importe * signo );
}

/*****************************************************************************/
void CAMBIO_ACTIVA_ANULACION()
/*****************************************************************************/
{
    if( anulacion ) {
        anulacion = 0;
    } else {
        anulacion = 1;
    }
}
/*****************************************************************************/
void CAMBIO_MOSTRAR_ANULACION()
/*****************************************************************************/
{
    if( anulacion ) {
        ExecuteSSAction( A_MUESTRA_FLAG_CAMBIO_MEDIO, T_FLAGS_DEV_ITEM_AGREGAR, " ", " ", NULL, " " );
    }
    else{
        ExecuteSSAction( A_MUESTRA_FLAG_CAMBIO_MEDIO, T_FLAGS_DEV_ITEM_BORRAR, " ", " ", NULL, " " );
    }
}

/*****************************************************************************/
double CAMBIO_PIDE_IMPORTE( int datos_adicionales, int pos )
/*****************************************************************************/
{
    double importe = 0.00;
    long mult = 1;
    int j, posx = 56, rta = 0, enteros;
	
	enteros = ENTEROS;
	
   if( datos_adicionales ) {
      char      archivo[24];
      _snprintf(archivo, sizeof(archivo)-1, "d_tmp.%d", pos );
      _unlink( archivo );
   }
    if( config.pais == CHILE || config.pais == BRASIL ) {
		enteros += 2;
        posx = 58;
    }
    if( config.pais == COLOMBIA || config.pais == ELSALVADOR ) {
        posx = 60;
    }
    for( j = 1;j <= DECIMALES;j++ ) {
        mult *= 10;
    }
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S_IMPORTE ), ST( S_IMPORTE ), NULL, " " );
    #endif
   	
	if( datos_adicionales ) {
		int cant_datos;
		struct _dmedios *datos_medio = NULL;
   		rta = PAGOS_PEDIR_DATOS_ADICIONALES( ( int ) pos, ( char ) 0, datos_adicionales,
   			&importe, D_MEDIOS_PRE_IMPORTE, NULL ,0, NULL, NULL,'\0', '\0' );
		 cant_datos = PAGOS_CARGAR_DATOS_MEDIO( &datos_medio, datos_adicionales, D_MEDIOS_PRE_IMPORTE);
		 if( cant_datos == 0)
			 rta =1; //no habia datos a cargar
	} else {
		rta = 1;
	}
	if (rta  && importe == 0.00 )
		importe = PEDIR_GRAN_FLOTANTE( 34 - ancho_panel / 2, 22, enteros/*ENTEROS*/, DECIMALES );
	
	if( importe == 0.0 ) {
        importe = RAM_TOTAL;
        MOSTRAR_GRAN_FLOTANTE( posx - ancho_panel / 2, 21, enteros, DECIMALES, 1, 0, importe );
    }
	if( importe > 0.0 && datos_adicionales ) {
   		rta = PAGOS_PEDIR_DATOS_ADICIONALES( ( int ) pos, ( char ) 0, datos_adicionales,
   			&importe, D_MEDIOS , "DATOS     " ,0, NULL, NULL,'\0', '\0' );
	} else {
		rta = 1;
	}
    return ( importe );
}
/*****************************************************************************/
void CAMBIO_IMPRIME_MEDIO( int tipo )
/*****************************************************************************/
{
    char *p_nombre;
    int m,signo;
    m = retiro.modo;
    signo = ( tipo == SALEN_MEDIOS ) ? -1 : 1;
    if( RAM_P_PAGO == 1 ) {
        IMPRIME_CABECERA_RETIROS( 2, NO );
        OPEN_COPIA_PRN();
    }
    if( medios[m].cheque || medios[m].tarjeta || medios[m].cta_cte || medios[m].ticket_compra
     || medios[m].mutual ) {
        p_nombre = PAGOS_OBTENER_NOMBRE( ( char )( m + 1 ), retiro.cod_sub_medio );
        IMPRIME_CUERPO_RETIROS( anulacion, p_nombre, ( retiro.importe * signo ), 0, 0,
                                0.0, 0 );
    }
    else {
        IMPRIME_CUERPO_RETIROS( anulacion, medios[retiro.modo].nombre,
                                ( retiro.importe * signo ), retiro.numero,
                                ( int )RAM_CANTIDAD, 0.0, 0 );
    }
}
/*****************************************************************************/
void _GRABA_EVENTO_CAMBIO( int medio, double importe, int id_registro, char cod_sub_medio, int tipo,
                           int h )
/*****************************************************************************/
{
    int posicion = 0;
    long id = 0;
    // Calcula Posicion
    //SELECT( AREA_PAGO );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
	SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    //GET_EQUAL( ( char* )&event_ticket.id_evento );
    while( !dbEOF() && event_ticket.id_evento == event_pago.id_evento ) {
        posicion++;
        //GET_NEXT();
		SKIP2(1);
    }
    if( importe ) {
        if( medios[_PAGO_MODO( h )].cheque ) {
            id = _PAGO_NUMERO( h );
        }
        if( medios[_PAGO_MODO( h )].tarjeta ) {
            id = _PAGO_NRO_CUPON_INT1( h );
        }
        if( medios[_PAGO_MODO( h )].ticket_compra ) {
            id = _PAGO_CUENTA( h );
        }
        if( medios[_PAGO_MODO( h )].mutual ) {
            id = _PAGO_CUENTA( h );
        }
        memset( &event_pago, 0, sizeof( event_pago ) );
        event_pago.modo_pago = medio;
        event_pago.importe = importe * ( ( tipo == ENTRAN_MEDIOS ) ? 1 : -1 );
		event_pago.importe_me = _PAGO_IMPORTE_ME( h );
        event_pago.id_registro = id_registro;
        event_pago.posicion = posicion;
        event_pago.cod_sub_medio = cod_sub_medio;
        event_pago.tarjeta = _PAGO_TARJETA( h ) + 1;  //cashplus
        memcpy( event_pago.auto_tarjeta, _PAGO_AUTO_TARJETA( h ) ,
                SIZEOF_VAR( __pago_auto_tarjeta ) );
		event_pago.numero_id = id;
        event_pago.banco = _PAGO_BANCO( h );
        event_pago.fecha_vto = _PAGO_FECHA_VTO( h );
        event_pago.cod_post_cheq = _PAGO_COD_POST_CHEQ( h );
        event_pago.cod_hora_acre = _PAGO_COD_HORA_ACRE( h );
        strncpy( event_pago.nro_tarjeta, _PAGO_NRO_TARJETA( h ), SIZEOF_VAR( __pago_nro_tarjeta ) ); //cashplus
		strncpy( &event_pago.nro_tarjeta[2], MASCARA( '*', 6, "" ), 6 );
        strcpy( event_pago.cuit, _PAGO_CUIT( h ) );
        strcpy( event_pago.telefono, _PAGO_TELEFONO( h ) );

		//cashpplus
		if(config_tps.ActivarCashPlus == 1 
			&&  medios[_PAGO_MODO( h )].tarjeta 
			&& TARJ[VAR_TARJETAS_TARJETA ].tipo_tarjeta & _TIPO_CASH_ADVANCE ){
			strcpy( event_pago.nro_comercio, VAR_TARJETAS_NRO_COMERCIO  );
			memcpy( event_pago.tabla, VAR_TARJETAS_TABLA , SIZEOF_VAR( __pago_tabla ) );
			event_pago.nro_cupon_int1 = _PAGO_NRO_CUPON_INT1( h );
		}
				
        GRABAR_EVENT_PAGO( NO );
		AGREGA_BIN_EV_DMEDIO_A_BTRV( h, D_MEDIOS_PRE_IMPORTE );
		AGREGA_BIN_EV_DMEDIO_A_BTRV( h, D_MEDIOS   );
    }
}
/*****************************************************************************/
void CAMBIO_SET_ACU_RETIRADO()
/*****************************************************************************/
{
    int i,signo;
    memset( acu_retirado, 0, sizeof( acu_retirado ) );
    for( i = 0;i < RAM_P_PAGO;i++ ) {
        signo = ( _PAGO_CUENTA( i ) == SALEN_MEDIOS ) ? -1 : 1;
        acu_retirado[_PAGO_MODO( i )] += _PAGO_IMPORTE( i ) * signo;
    }
}
/*****************************************************************************/
void GRABA_EVENTO_CABECERA_CAMBIO()
/*****************************************************************************/
{
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = 40;
    event_ticket.nro_ticket = NRO_NRO_RETIRO;
	event_ticket.nro_comprobante_new = 0;
    SETEAR_TIEMPO( _INDEFINIDO );
    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
    GRABAR_EVENT_TICKET( NO );
    ACUMULAR_TIEMPOS( );
    CEREAR_TIEMPOS( );
    SETEAR_TIEMPO( _ENTRADA_OCIO );
}
/*****************************************************************************/
void CAMBIO_MUESTRA_TOTAL( int tipo, int nro )
/*****************************************************************************/
{
    char *tot = 0;
    int larray = ENTEROS + 1 + DECIMALES + 5;
    _ATRIBUTO = 112;
    #if defined(INVEL_W) || defined(INVEL_L)   
    tot = ( char* )calloc( 1, larray );
    if( tot != NULL ) {
        memset( tot, 0, larray );
    }
    else {
        GRABAR_LOG_SISTEMA( "Sin memoria para pantallas" ,LOG_ERRORES,1);
        return;
    }
    ExecuteSAction( A_MUESTRA_TIT_TOTAL_FFIJO,
                    ( tipo == ENTRAN_MEDIOS ) ? ST( S_RECIBE ) : ST( S_FALTA ) );
	if(nro!=0)
	{
	if(medios[nro].moneda_extranjera)
	{
		//SET_MEMORY_DOUBLE(__ram_total, RAM_TOTAL * medios[nro].cotizacion);
		FTOC( RAM_TOTAL, tot, ENTEROS + 1, DECIMALES, 1, 1 );  
	}
	else
		FTOC( RAM_TOTAL, tot, ENTEROS + 1, DECIMALES, 1, 1 );
	}
	else
		FTOC( RAM_TOTAL, tot, ENTEROS + 1, DECIMALES, 1, 1 );
    ExecuteSAction( A_PAGO_MUESTRA_TOTAL, tot );
    FREEMEM( tot );
    #else
    MOSTRAR_GRAN_CADENA( 23 - ancho_panel / 2, 3,
                         ( tipo == ENTRAN_MEDIOS ) ? ST( S_RECIBE ) : ST( S_FALTA ) );
    if( config.pais == COLOMBIA || config.pais == CHILE || config.pais == BRASIL ) {
        MOSTRAR_GRAN_FLOTANTE( 43 - ancho_panel / 2, 3, ENTEROS, DECIMALES, 1, 1, RAM_TOTAL );
    }
    else {
        MOSTRAR_GRAN_FLOTANTE( 40 - ancho_panel / 2, 3, ENTEROS, DECIMALES, 1, 1, RAM_TOTAL );
    }
    #endif
}
/*****************************************************************************/
int VALIDA_ANULACION_MEDIO( int medio, int submedio, double importe, int tipo )
/*****************************************************************************/
{
    int h,rta;
    double total = 0.0;
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        if( _PAGO_CUENTA( h ) == tipo && _PAGO_MODO( h ) + 1 == medio
         && _PAGO_COD_SUBMEDIO_VUELTO( h ) == submedio ) {
            total += _PAGO_IMPORTE( h );
        }
    }
    rta = ( total - importe > -0.01 ) ? 1 : 0;
    return ( rta );
}
/*****************************************************************************/
int EN_CAMBIO_DE_MEDIOS()
/*****************************************************************************/
{
    return( _en_cambio_de_medios );
}
/*****************************************************************************/
int PERMITIR_MEDIO( int medio, int tipo )
/*****************************************************************************/
{
    int rta = 1;
   // int area = SELECTED();
    int  area_ant, tipo_ant;
	//struct _cam_val_hab camb_val_hab;
    struct _indice_camb_val_hab
    {
        char cod_medioing;
        char cod_medioret;
    } indice;
	
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    
	indice.cod_medioing = (char)(_PAGO_MODO( 0 ) + 1);
    indice.cod_medioret = (char)(medio + 1);
	
    if( RESTRINGIR_CAMBIO_DE_VALORES ) {
		BORRAR_MENSAJE();
        if( tipo == SALEN_MEDIOS ) {
          //  if( !USED( AREA_AUX6 ) ) {
				
               // if( USE_DB( AREA_AUX6, _CAMB_VAL_HAB_SIC, ( char* )&camb_val_hab,
               //             sizeof( struct _cam_val_hab ), NULL, 0, 0 ) == 0 ) {
			  if( OPEN_TABLE( T_CAMB_VAL_HAB, TT_ORIG, ( char* )&camb_val_hab, sizeof( struct _cam_val_hab ) ) == 0 ) {
					//SET_ORDER( 2 );                
					//GET_EQUAL( ( char* )( &indice ) );
					//SELECT_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
					SET_WHERE("COD_MEDIOING = '%d' and COD_MEDIORET = '%d'", indice.cod_medioing, indice.cod_medioret );
					SET_ORDER2( "COD_MEDIOING, COD_MEDIORET" );
					RUN_QUERY(NO);
                    
					if( !FOUND2() ) {
                        rta = 0;
					} else {
						int encontro = 0;
						while( indice.cod_medioing == camb_val_hab.cod_medioing && 
							indice.cod_medioret == camb_val_hab.cod_medioret && /*!BtrvEOF()*/ !dbEOF() && encontro == 0 ) {
							if(  _PAGO_COD_SUBMEDIO_VUELTO( 0 ) != camb_val_hab.cod_submedioing ) {
								SKIP2( 1 );
							} else {
								encontro = 1;
							}
						}
						if( encontro == 0 )
							rta = 0;
					}
					//CLOSE_DATABASE( AREA_AUX6 );
					CLOSE_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
                }
         //   }
           // SELECT( area );
        } else {
            //Permite solo un medio de pago para ingreso de valores.
            if( RAM_P_PAGO > 0 ) {
                rta = 0;
				MENSAJE( "SOLO SE PERMITE UN MEDIO DE INGRESO" );
            }
        }
    }
	SELECT_TABLE( area_ant, tipo_ant );

    return rta;
}
/***************************************************************************/
/*	Esta funcion fue modificada para que imprima una sola validacion correspondiente 
al primer medio entrante , sino duplica tantas como medios ingresados, ENTRANTES O SALIENTES
la funcion original esta a continuación renombrada.	*/
int VALIDAR_MEDIOS_CAMBIO_M( char cod_medio_ing, int cod_submedio_ing )
/***************************************************************************/
{
    int cod = -1, med_sale = 0;
	int  area_ant = 0, tipo_ant = 0, h=0;

	unsigned aux_fecha;

	struct _indice_camb_val_hab
    {
        char cod_medioing;
        INT16 cod_submedioing;
        char cod_medioret;
        INT16 cod_submedioret;
    } indice;
	 
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
	
	indice.cod_medioing = cod_medio_ing;
    indice.cod_submedioing = (short)cod_submedio_ing;


	BORRAR_MENSAJE();
    
	if( OPEN_TABLE( T_CAMB_VAL_HAB, TT_ORIG, ( char* )&camb_val_hab, sizeof( struct _cam_val_hab ) ) == 0 ) {

		/* solo hace falta posicionar en el primero de los salientes, que puede ser el segundo u otro 
		dependiendo de RESTRINGIR_CAMBIO_DE_VALORES...*/
		for( h = 0 ; h < RAM_P_PAGO && ( _PAGO_CUENTA( h ) == ENTRAN_MEDIOS )  ; h++ );//{med_sale = h ;		}
			
			
		/*******************************************************************************************
		No hace falta recorrer para validar por que ya estan validados, 
		busco la primera combinación con el entrante y saliente que tenga validacion configurada .
		******************************************************************************************/
			
		for( /*h = med_sale */; h < RAM_P_PAGO && cod == -1 ; h++ ) {
			
			indice.cod_medioret = (char)(_PAGO_MODO( h ) + 1) ;
			indice.cod_submedioret =(short) _PAGO_COD_SUBMEDIO_VUELTO( h );
			
			SET_WHERE("COD_MEDIOING = '%d' and COD_SUBMEDIOING = '%d' and COD_MEDIORET = '%d' and COD_SUBMEDIORET = '%d'", 
						indice.cod_medioing, indice.cod_submedioing, indice.cod_medioret, indice.cod_submedioret );
			SET_ORDER2( "COD_MEDIOING, COD_SUBMEDIOING, COD_MEDIORET, COD_SUBMEDIORET" );
				RUN_QUERY(NO);
			
				/**************************************************************************************************
				En el ingreso de medios se controla si esta habilitado siempre cuando  este configurado 
				RESTRINGIR_CAMBIO_DE_VALORES, de lo contrario permite ingresar mas de un medio como entrante!!! 
				**************************************************************************************************/

			if( FOUND2() ) {
				
				/******************************************************************************************
				Solo para imprimir la fecha ingresada para el caso del 
				CASH ADVANCE,luego retoma su valor original	
				*****************************************************************************************/
				if( medios[camb_val_hab.cod_medioing - 1].tarjeta && medios[camb_val_hab.cod_medioret - 1].efectivo ) {
					
					aux_fecha = GET_FECHA_VTO_PRIVADA(  ) ;
					SET_FECHA_VTO_PRIVADA(DECREMENTAR_MES( aux_fecha ));
				}
						
				cod = camb_val_hab.cod_val_entrante;
						
				if( medios[camb_val_hab.cod_medioing - 1].tarjeta && medios[camb_val_hab.cod_medioret - 1].efectivo ) {
					SET_FECHA_VTO_PRIVADA( aux_fecha );
					aux_fecha = 0;
				}
			} 
		}
		/******************************************************************************************
				existe la validacion entrante y esta habilitada alguna saliente
		******************************************************************************************/
		if( cod > 0 ) {
			VALIDAR_UN_MEDIO( /*h valida el primero unicamente con el detalle de todo*/ 0 , cod, _MODO_CAMBIO_MEDIO );
		}else {
			if( cod == 0 )
				MENSAJE_TECLA( ST( S_FALTA_CONFIG_CANCELA_CAMBIO_MEDIOS_Pres_Tecla ) );
			else 
				MENSAJE_TECLA( ST( S_CAMBIO_DE_MEDIO_NO_HABILITADO_CANCELA_CAMBIO_MEDIOS_Pres_Tecla ) );
		}
		VALIDAR_MEDIOS( _MODO_CAMBIO_MEDIO );
		if(medios[( int )_PAGO_MODO( 0 )].tarjeta 
			&& config_tps.ActivarCashPlus == 1  
			&&  TARJ[VAR_TARJETAS_TARJETA ].tipo_tarjeta & _TIPO_CASH_ADVANCE 
			&& config_tps.HabilitarCashPlusSolo == 0
			&&  IsCashActivo() ) {
			//aqui va la validacion de extraccion de dinero
			VALIDAR_UN_MEDIO( /*h valida el primero unicamente con el detalle de todo*/ 0 ,config_tps.ValidacionCambioMediosCash, _MODO_CAMBIO_MEDIO );
			 
		} 
		CLOSE_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
    } 
	SELECT_TABLE( area_ant, tipo_ant );
	return( cod );
}

/***************************************************************************/
int VALIDAR_MEDIOS_CAMBIO_M_original_imprime_una_validacion_completa_por_cada_medio( char cod_medio_ing, int cod_submedio_ing )
/***************************************************************************/
{
    int rta = 1, primera = 1, med_entrante = 0, cod = -1;

    //int area = SELECTED();
    //struct _cam_val_hab camb_val_hab;
	unsigned aux_fecha;
    struct _indice_camb_val_hab
    {
        char cod_medioing;
        INT16 cod_submedioing;
        char cod_medioret;
        INT16 cod_submedioret;
    } indice;
	 int  area_ant, tipo_ant, h;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	//medio 0 es el medio de ingreso
    indice.cod_medioing = cod_medio_ing;
    indice.cod_submedioing = (short)cod_submedio_ing;


	//if( RESTRINGIR_CAMBIO_DE_VALORES ) {
		BORRAR_MENSAJE();
       // if( !USED( AREA_AUX6 ) ) {
			
           // if( USE_DB( AREA_AUX6, _CAMB_VAL_HAB_SIC, ( char* )&camb_val_hab,
           //             sizeof( struct _cam_val_hab ), NULL, 0, 0 ) == 0 ) {
			if( OPEN_TABLE( T_CAMB_VAL_HAB, TT_ORIG, ( char* )&camb_val_hab, sizeof( struct _cam_val_hab ) ) == 0 ) {
				//SET_ORDER( 1 );
                for( h = 1; h < RAM_P_PAGO; h++ ) {
					indice.cod_medioret = (char)(_PAGO_MODO( h ) + 1) ;
					indice.cod_submedioret =(short) _PAGO_COD_SUBMEDIO_VUELTO( h );
					//GET_EQUAL( ( char* )( &indice ) );
					//SELECT_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
					SET_WHERE("COD_MEDIOING = '%d' and COD_SUBMEDIOING = '%d' and COD_MEDIORET = '%d' and COD_SUBMEDIORET = '%d'", 
						indice.cod_medioing, indice.cod_submedioing, indice.cod_medioret, indice.cod_submedioret );
					SET_ORDER2( "COD_MEDIOING, COD_SUBMEDIOING, COD_MEDIORET, COD_SUBMEDIORET" );
					RUN_QUERY(NO);
					if( FOUND2() ) {
						/*Solo para imprimir la fecha ingresada para el caso del 
						CASH ADVANCE,luego retoma su valor original*/
						if( medios[camb_val_hab.cod_medioing - 1].tarjeta 
								&& medios[camb_val_hab.cod_medioret - 1].efectivo ) {
							aux_fecha = GET_FECHA_VTO_PRIVADA(  ) ;
							SET_FECHA_VTO_PRIVADA(DECREMENTAR_MES( aux_fecha ));
						}
						if( primera ) {
							//Para medios entrantes solo se imprimira la primera validacion!!
							cod = camb_val_hab.cod_val_entrante;
							if( cod ) {
								//VALIDAR_UN_MEDIO( /*h*/0, cod, _MODO_CAMBIO_MEDIO );
							}
							primera = 0;
						}
						cod = camb_val_hab.cod_val_saliente;
						if( cod ) {
							VALIDAR_UN_MEDIO( h, cod, _MODO_CAMBIO_MEDIO );
						}
						if( medios[camb_val_hab.cod_medioing - 1].tarjeta 
								&& medios[camb_val_hab.cod_medioret - 1].efectivo ) {
							SET_FECHA_VTO_PRIVADA( aux_fecha );
							aux_fecha = 0;
						}
						/*med_entrante = camb_val_hab.cod_medioing;
						  if( medios[med_entrante - 1].tarjeta ) {
							SET_MEMORY_INT( __var_tarjetas_fecha_vto, aux_fecha );
							aux_fecha = 0;
						}*/
					}
					if( !cod ){
						if( cod == 0 )
							MENSAJE_TECLA( ST( S_FALTA_CONFIG_CANCELA_CAMBIO_MEDIOS_Pres_Tecla ) );
						else 
							MENSAJE_TECLA( ST( S_CAMBIO_DE_MEDIO_NO_HABILITADO_CANCELA_CAMBIO_MEDIOS_Pres_Tecla ) );
					}
				}
				VALIDAR_MEDIOS( _MODO_CAMBIO_MEDIO );
               // CLOSE_DATABASE( AREA_AUX6 );
				CLOSE_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
           // }
			//SELECT( area );
        } 
	//}
		SELECT_TABLE( area_ant, tipo_ant );
		return( cod );
}
/*************************************************************************************/
int PERMITE_SUBMEDIO( char medio, int sub_medio, char tipo )
/*************************************************************************************/
{
    int rta = 1, primera = 1;
    //int area = SELECTED();
    //struct _cam_val_hab camb_val_hab;
    struct _indice_camb_val_hab
    {
        char cod_medioing;
        INT16 cod_submedioing;
        char cod_medioret;
        INT16 cod_submedioret;
    } indice;
	 int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    indice.cod_medioing = (char)(_PAGO_MODO( 0 ) + 1) ;
    indice.cod_submedioing = (short) _PAGO_COD_SUBMEDIO_VUELTO( 0 );
	indice.cod_medioret = medio;
	indice.cod_submedioret = (short)sub_medio;
    if( RESTRINGIR_CAMBIO_DE_VALORES && tipo == SALEN_MEDIOS ) {
		BORRAR_MENSAJE();
       // if( !USED( AREA_AUX6 ) ) {
           // if( USE_DB( AREA_AUX6, _CAMB_VAL_HAB_SIC, ( char* )&camb_val_hab,
           //             sizeof( struct _cam_val_hab ), NULL, 0, 0 ) == 0 ) {
			if( OPEN_TABLE( T_CAMB_VAL_HAB, TT_ORIG, ( char* )&camb_val_hab, sizeof( struct _cam_val_hab ) ) == 0 ) {
				//SET_ORDER( 1 );
				//GET_EQUAL( ( char* )( &indice ) );
				//SELECT_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
				SET_WHERE("COD_MEDIOING = '%d' and COD_SUBMEDIOING = '%d' and COD_MEDIORET = '%d' and COD_SUBMEDIORET = '%d'", 
				indice.cod_medioing, indice.cod_submedioing, indice.cod_medioret, indice.cod_submedioret );
				SET_ORDER2( "COD_MEDIOING, COD_SUBMEDIOING, COD_MEDIORET, COD_SUBMEDIORET" );
				RUN_QUERY(NO);
				if( !FOUND2() ) {
					MENSAJE("SUBMEDIO NO HABILITADO");
					rta = 0;
				}
                //CLOSE_DATABASE( AREA_AUX6 );
            }
			//SELECT( area );
       // } 
	}
	//SELECT_TABLE( area_ant, tipo_ant );
	CLOSE_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
    return rta;
}
/*****************************************************************************/
int CONSULTA_SALDO_TARJETAS( double monto )
/*****************************************************************************/
{
    struct _rta_terminal rta_terminal;
	int rta = 0;
	char men[80];
    
	NUMERO_DE_COMERCIO_TARJETA( ); 
	
    if( VAR_TARJETAS_STATUS > 0 ) {
        if( AUTORIZACION_ON_LINE( _ON_LINE_SALDO, 0, ( char* )&rta_terminal,
                                  sizeof( rta_terminal ) ) ) {
            #ifdef COMPILAR_PINPAD
			if( strlen(rta_terminal.working_key) > 0 && VAR_TARJETAS_CELDA_PINPAD && UTILIZA_PINPAD )
            	PINPAD_ACTUALIZAR_WORKING_KEY( VAR_TARJETAS_CELDA_PINPAD, rta_terminal.working_key );
            #endif
        } else {
           rta_terminal.cod_rta = 1;
            strncpy( rta_terminal.mensaje, "ERROR DE COMUNIC. OBTENIENDO SALDO",
                     sizeof( rta_terminal.mensaje ) );
        }
		/*-------------- Procesa la respuesta ---------------*/
		if( rta_terminal.cod_rta != 1 ) {
			if( monto <= rta_terminal.monto_compra ) {
				rta = 1;
			} else { 		
				_snprintf( men, sizeof( men ), "SALDO DISPONIBLE: %.2lf, %s", rta_terminal.monto_compra, ST( S_PRES_UNA_TECLA ) );
				men[sizeof( men )-1] = '\0';
				MENSAJE_TECLA( men );
			}
		} else {
			_snprintf( men, sizeof( men ),"%s, %s", rta_terminal.mensaje, ST( S_PRES_UNA_TECLA ) );
			men[sizeof( men )-1] = '\0';
			MENSAJE_TECLA( men );
		}
    }
	return rta;
}
/*************************************************************************************/
int SOLICITAR_SUPERVISOR_CAMBIO_M( char medio, int sub_medio, char tipo )
/*************************************************************************************/
{
    int rta = 1, primera = 1, condicion;
    //int area = SELECTED();
	//struct _cam_val_hab camb_val_hab;
    struct _indice_camb_val_hab
    {
        char cod_medioing;
        INT16 cod_submedioing;
        char cod_medioret;
        INT16 cod_submedioret;
    } indice;
	int  area_ant, tipo_ant;
 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    indice.cod_medioing = (char)(_PAGO_MODO( 0 ) + 1) ;
    indice.cod_submedioing = (short) _PAGO_COD_SUBMEDIO_VUELTO( 0 );
	indice.cod_medioret = medio;
	indice.cod_submedioret = (short)sub_medio;
    if(  tipo == SALEN_MEDIOS ) {
		BORRAR_MENSAJE();
     //   if( !USED( AREA_AUX6 ) ) {
           // if( USE_DB( AREA_AUX6, _CAMB_VAL_HAB_SIC, ( char* )&camb_val_hab,
           //             sizeof( struct _cam_val_hab ), NULL, 0, 0 ) == 0 ) {
		   if( OPEN_TABLE( T_CAMB_VAL_HAB, TT_ORIG, ( char* )&camb_val_hab, sizeof( struct _cam_val_hab ) ) == 0 ) {
				//SET_ORDER( 1 );
				//GET_EQUAL( ( char* )( &indice ) );
				//SELECT_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
				SET_WHERE("COD_MEDIOING = '%d' and COD_SUBMEDIOING = '%d' and COD_MEDIORET = '%d' and COD_SUBMEDIORET = '%d'", 
						indice.cod_medioing, indice.cod_submedioing, indice.cod_medioret, indice.cod_submedioret );
				SET_ORDER2( "COD_MEDIOING, COD_SUBMEDIOING, COD_MEDIORET, COD_SUBMEDIORET" );
				RUN_QUERY(NO);
				if( FOUND2() ) {
					condicion = ( camb_val_hab.pide_supervisor == 'S' || camb_val_hab.pide_supervisor == 's' ) ? 1 : 0;
					if( !SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, condicion,
                               "Supervisor", NULL, _F_SUP_CAMBIO_MEDIO, SI ) ) {
						rta = 0;
					}
				}
                //CLOSE_DATABASE( AREA_AUX6 );
				CLOSE_TABLE( T_CAMB_VAL_HAB, TT_ORIG );
            }
			//SELECT( area );
      //  } 
	}
	SELECT_TABLE( area_ant, tipo_ant );
    return rta;
}

