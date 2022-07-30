#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <archivos.h> 
#include <asc.h> 
//#include <browse.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cajon.h> 
#include <cio.h>  
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <cobros.h> 
#include <coop.h> 
#include <cr.h>  
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <fecha.h> 
#include <ftoa.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <imp_comp.h> 
#include <imp_fond.h> 
#include <log.h> 
#include <l_valid.h> 
#include <mensaje.h>
#include <malloc.h> 
#include <menu.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <on_line.h> 
#include <pagos.h> 
#include <pant.h> 
#include <print.h> 
#include <p_float.h> 
#include <stack.h> 
#include <stringt.h> 
#include <tar_mag.h> 
#include <tkt.h> 
#include <trim.h> 
#include <valida.h> 
#include <xyz.h> 
#include <_cr1.h> 
#include <aarch.h>
#include <path.h>
#include <interpr.h>
#include <motor.h>
#include <_cr0.h>
#include <pago_tar.h>
#include <valida.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#include <math.h>
#endif
#include <tarjetas.h>
#include <cr_disco.h>
#include <deftables.h>
#include <db.h>
#include <round.h>
#include <recupero.h>
#include <dec_mes.h>
#include <pagatodo.h>
#include <pago_mut.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_L)
#include <string.h>
#include <actions.h>
#include <sys/io.h>
#include <cr_disco.h>
#endif
#if defined(INVEL_W)
//
#include <dos.h>
#include <math.h>
#include <string.h>
#include <actions.h>
#include <tpv_dat.h>
#endif
#include <typeacti.h>
int cobro_unipago = 0;
#define _CMR               25
struct varios
{
    //char codigo;
    int codigo;
    long id_cobro;
    long comprobante;
    long cuenta;
    double importe;
};
struct _cuo_cmr
{
    BINARY status;
    char cuotas;
    char descripcion[25];
    char reserva[30];
};
struct _cuo_cmr cuo_cmr;
int _elegir_repactacion( int comando, char *cadena, int dato, struct _browse *b );
struct _datos_cobro *datos_cobro;
#define _X1   6
#define _X2  30
#define _Y1   7
#define _Y2  15
extern int impresion_fiscal;
static int label_on = 0;
static int browse = 0;
extern int cobro_on_tick;
extern int cobro_marginal;
extern int cobro_unico;
#define limiteNiveles               9
int cod_cobro_ant[limiteNiveles+1] = {0,0,0,0,0,0,0,0,0,0};

int indice_cobro_venta = 0;
extern int posicionTransaccionKinacu;
long valida_kinacu_id_transaccion = 0;
int devolucion_cobro = 0;
extern int grabar_copia_ticket;

/*
 * Variables Globales Motor
 */
extern promo pr, gpr;
extern states ss, ss_bak;
extern recover_info state_venta, state_pago;
extern double PRECIO_UNITARIO_PARCIAL( int h, double* cant );
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_ev_dpago;
extern void SETEAR_MODO_EJECUCION( int modo );
//**********************************************************************/
int BUSCA_ARTICULO_COBRO( char *cod_barra , long cod_especial )
//**********************************************************************/
{
	int rta = 0;
	if( strlen( cod_barra ) ){
		if( cod_especial > 0 ){
			rta = BUSCA_COD_INTERNO_COD_BARRA( cod_especial, cod_barra );
		}else{
			rta = BUSCA_CODIGO_BARRA( cod_barra );
		}
	}else{
		rta = BUSCA_CODIGO_ESPECIAL( cod_especial );
	}
    return rta;
}

/************************************************************************************/
int AGREGAR_ARTICULO_COBRO_VENTA( char *cod_barra, long cod_especial, double importe, INT16 cod_cobro )
/************************************************************************************/
{	
	int rta = 1;
	
	if( strlen( cod_barra ) ){
		if( cod_especial > 0 ){
			rta = ASIGNA_COD_ESPECIAL_COD_BARRA( cod_especial, cod_barra );
		}else{
			rta = ASIGNA_CODIGO_BARRA( cod_barra );
		}
	}else{
		rta = ASIGNA_CODIGO_ESPECIAL( cod_especial );
	}

	if( rta ){
		//RECALCULA_ITEM_POR_PRECIO( (MODO_DEVOLUCION) ? -1 : 1 );
		//SET_MEMORY_DOUBLE( __ram_cantidad, importe );
		//Si el importe proviene del articulo -> la cantidad es 1 porque va a dividir el 
		//importe por el mismo item_precio (que obtiene de la busqueda de art.)
		//Si el importe es el configurado en cobros o ingresado del art. por importe
		//(Ej. kinacu Saldo ) -> la cantidad no es 1
		//porque va a dividir el importe por el mismo item_precio(que obtiene de la busqueda de art.)
		float cantidad = 0;
		cantidad =(float)( importe / ( (MODO_DEVOLUCION) ? -ITEM_PRECIO : ITEM_PRECIO ));
		SET_MEMORY_DOUBLE( __ram_cantidad, ( cantidad )? cantidad : 1 );
		SET_MEMORY_INT( __item_cod_cobro, cod_cobro );
		rta = CR1_AGREGA_ITEM( NO, _ORIGEN_TICKET, 0, 0 );

		/*----------------- Motor Promociones ----------------------------*/
		if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ) {
			/**/glog( "INICIO PROMOCIONES EN COBROS VENTA",LOG_VENTAS,2 );
			if( !RAM_ANULACION ) {
				/**/char buf[256];
				/**/sprintf( buf, "PROMO: ---> Agregar Articulo: i%li - cantidad: %lf [Promoc:%i] ...", ITEM_COD_ESPECIAL, RAM_CANTIDAD, ITEM_HABILITADO_EN_PROMOCION );
				/**/glog( buf ,LOG_MOTOR_PROMO,4);
				if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
						// Se agrega un artiuclo.
						add_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO,pr, &ss );
				} else {
						// Se agrega un artiuclo.
						add_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO,pr, &ss );
				}
			}
			else {
				double precUnit, cant;
				/**/char buf[256];
				/**/sprintf( buf, "PROMO: ---> Anular Articulo: i%li - cantidad: %lf [Promoc:%i] ...", ITEM_COD_ESPECIAL, RAM_CANTIDAD, ITEM_HABILITADO_EN_PROMOCION );
				/**/glog( buf ,LOG_MOTOR_PROMO,4);

				precUnit = PRECIO_UNITARIO_PARCIAL( RAM_P_TICKET - 1, &cant );

				if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
					remove_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
					if (  cant == 0 ) {
						update_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
					}
				} else {
					remove_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
					if ( cant == 0 ) {
						update_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
					}
				}
			}
			/**/glog( "FIN PROMOCIONES EN COBROS VENTA" ,LOG_VENTAS,2);
			MUESTRA_SUBTOTAL(NO);
			/**/glog( "PROMO: LISTO.",LOG_VENTAS,4 );
		}
		/*----------------------------------------------------------------*/		
	}
	return rta;
}

/*****************************************************************************/
int TIPO_COBRO_VENTA( INT16 cod_cobro )
/*****************************************************************************/
{	
	int rta = 0;
	//USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
	OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ));
    //SELECT( AREA_AUX4 );
    //SELECT_TABLE( T_COBROS, TT_ORIG );
    //GET_EQUAL( ( char* )&cod_cobro );
    SET_WHERE("CODIGO='%d'", cod_cobro);
    RUN_QUERY(NO);
    //CLOSE_DATABASE( AREA_AUX4 );
	switch( cobros.cod_funcion )
	{
        case 250: /*Cobros Kinacu*/       
        case 109: /*PagaTodo Recarga*/
        case 110: /*PagaTodo Venta PIN*/
        case 111: /*PagaTodo Servicios*/
            rta = COBRO_PAGATODO;
            break;
        case 113: /* Cobro Garantia*/
            rta = COBRO_GARANTIA;
            break;
        default:
            rta = COBRO_NORMAL;
			break;
	}
	CLOSE_TABLE( T_COBROS, TT_ORIG );
	return rta;
}

/*****************************************************************************/
void COBROS( int devolucion, int modo_usuario )
/*****************************************************************************/
{
    int		cobro = 0, _ptr, _handle, _estado, rta = 0;
	struct	_datos_cobro d_cobro;
	unsigned aux_fecha;
    int prn = 0, i = 0;
	
	devolucion_cobro = devolucion;
	
	if( modo_usuario == _MODO_INTERACTIVO 
			&& !( RAM_MODO_EJECUCION == COBRO ) ) {
		SET_MEMORY_CHAR( __ram_modo_ejecucion_ant, RAM_MODO_EJECUCION );
		SETEAR_MODO_EJECUCION( COBRO );
	}
	
	/*--------------------- Prepara recupero de memoria ----------------------*/
	OPEN_RECUPERO( &_ptr, &_handle );
	datos_cobro = &d_cobro;
	memset( &d_cobro, 0, sizeof( d_cobro ) );
    _estado = STACK_INS( sizeof( short ) );	
	
	//Verifica que no hay items en el ticket que ya hayan abierto los arch. PRN 
	for( i = 0; i < RAM_P_TICKET && !prn; i++ ) {
		prn = 1;
	}
	if( !prn ) {
		CLOSE_COPIA_TICKET_PRN(  );
		OPEN_COPIA_TICKET_PRN(  );
		CLOSE_COPIA_VOUCHER_PRN(  );
		OPEN_COPIA_VOUCHER_PRN(  );
	}
	if( _ESTADO == 0 ) {
		BORRAR_ARCHIVO_REC( ARCH_REC_COBROS );
		BORRAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO );
        if( SOLICITAR_CLAVES( 30 - ancho_panel, 21, SI, SI, 
			PEDIR_CLAVE_EN_COBROS, ST( S_COBROS ), NULL, RAM_COD_FUNCION, SI ) ) {
            SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
            INICIALIZAR_BINARIO_COD_BARRA();
            if( devolucion ) {
				ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_DEV_COBRO_AGREGAR, " ",
					" ", NULL, " " );
			} else {
				ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_AGREGAR, " ",
					" ", NULL, " " );
			}
            if( /**USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )*/
				  
             OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros )) == 0  && RECCOUNT2( NO, NULL ) > 0 ) {
				//SELECT_TABLE( T_COBROS, TT_ORIG );
				GO2( TOP );

                if( modo_usuario == _MODO_INTERACTIVO ) {
                    PANTALLA_COBROS();
                }
                //Se verifica si hay cobros para mostrar
				
				//if( ELEGIR_COBRO_BROWSE( _MODO_AUTOMATICO, SI ) ) {
                    //PANTALLA_COBROS();
                    /*--------------------------- Actualiza estado -------------------------*/
					SETEAR_TIEMPO( _ENTRADA_NO_VENTA );
					/*----------- ingresa cobros ------------*/
					cobro = PIDE_COBRO( datos_cobro, 0, devolucion, modo_usuario );
				//} else {
                    //cobro = 0;
                //}
				if( cobro != 4 ) {  //para evitar que borre icono en cobro no tradicional
					if( devolucion ) {
						ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_DEV_COBRO_BORRAR, " ",
							" ", NULL, " " );
					} else {
						ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_BORRAR, " ",
							" ", NULL, " " );
					}
				}
                if( cobro ) {
                    if ( cobro == 4 ) {    //Cobro no tradicional.
                        datos_cobro->devolucion = devolucion;
						_SET_ESTADO( 99 );
                    } else {	           //Cobro tradicional.
                        _SET_ESTADO( 1 );	
						if( RAM_FLAG_COBRO_VENTA && !RAM_ANULACION ){
							datos_cobro->devolucion = devolucion;
							GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA, ( char* )datos_cobro,
								sizeof( struct _datos_cobro ) );
						}
                    }
					GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )datos_cobro,
                        sizeof( struct _datos_cobro ) );
				}else {
					if( cobro == 0 && PEDIR_CLAVE_EN_COBROS ){
						GRABA_EVENTO_SALIDA_CAJERO(  );
					}
				}
			} else {
					SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
				}
				CLOSE_TABLE( T_COBROS, TT_ORIG );
            }
//	}
    }else{  //si no es el ESTADO 0 es porque es un recupero, entonces obtiene de nuevo los datos
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )datos_cobro,
	        sizeof( struct _datos_cobro ), 0 );
        //USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
        //SELECT( AREA_AUX4 );
        //GET_EQUAL( ( char* )&datos_cobro->codigo );
		OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ));
		//SELECT_TABLE( T_COBROS, TT_ORIG );
		SET_WHERE("CODIGO = '%i'",datos_cobro->codigo);
		RUN_QUERY(NO);
        CLOSE_TABLE( T_COBROS, TT_ORIG );
        //CLOSE_DATABASE( AREA_AUX4 );
    }
    if( _ESTADO == 1 ) {
        if( cobros.tarjeta ) {
			SET_MEMORY_CHAR( __var_tarjetas_status, 2 );
			GRABA_MODIFICACION_TARJETAS( 0, TARJETA_COBRO );
			if( !COBROS_TARJETA_PERMITIDA( cobros.codigo, VAR_TARJETAS_TARJETA + 1 ) ) {
				MENSAJE( "LA TARJETA INGRESADA NO CORRESPONDE AL COBRO" );
				_SET_ESTADO( 0 );
			}
			//SET_MEMORY( __ram_cadena2, NOMBRE_TARJETA( VAR_TARJETAS_TARJETA ) );
        }
		/* Variable RAM usada para la impresiones de datos de tarjetas on-line*/
		if( cobros.tarjeta > T_NO && cobros.tarjeta != T_OFF_LINE ) {
			SET_MEMORY_INT( __ram_cobro_servicios, 1 );
		}
		if( IMPRIMIR_COBROS && !RAM_FLAG_COBRO_VENTA ) {
			TICKET_COBRANZA( cobros.nombre, datos_cobro, NULL, NO );
		}
		if( !RAM_FLAG_COBRO_VENTA ){ 
			SET_MEMORY_DOUBLE( __ram_total, datos_cobro->importe );
			SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
			ExecuteSAction( A_MUESTRA_COBRO_IMPORTE_ME, "" );
		}
		if( !RAM_FLAG_COBRO_VENTA ){
			if( PAGOS( _MODO_COBRANZA, COBROS_F_ONLINE, devolucion ) ) {
				_SET_ESTADO( 2 );
			}else {
				if( IMPRIMIR_COBROS ) {
					MOSTRAR_CADENA( 1, PRN, ST( S_COBRO_ANULADO ) );
				}
				SET_MEMORY_DOUBLE( __ram_total, 0 );
				_SET_ESTADO( 90 );
			}
		}else{
			if(cobro){
				if( cobro = AGREGAR_ARTICULO_COBRO_VENTA( cobros.cod_barra, cobros.cod_interno, 
						datos_cobro->importe, datos_cobro->codigo ) ){
					SET_MEMORY_CHAR( __ram_hay_cobro_venta, 1 );
					//GRABAR_MEDIOS_COBRO( datos_cobro->codigo, datos_cobro->importe );
					GRABAR_EVENTO_COBRO( datos_cobro, NULL, devolucion );
					//SET_MEMORY_DOUBLE( __item_precio, datos_cobro->importe );
				}
				if( RAM_ANULACION || !cobro ){
						CANCELAR_COBRO_VENTA( (void*)datos_cobro, COBRO_NORMAL );
						SET_MEMORY_INT( __ram_anulacion, 0 );
				}
				CR1_INICIALIZAR_VAR_TICKET( SI );
			}
			SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
			CLOSE_RECUPERO( _ptr, _handle );
            LIMPIAR_PANEL_COBROS( );
			ExecuteAction( A_DES_COBROS_ACT_VENTAS );
			return;
		}
    }
    if( _ESTADO == 2 ) {
		if( cobros.tarjeta ) {
			LEE_TARJETAS( 0, TARJETA_COBRO );
		}
        SETEAR_TIEMPO( _ENTRADA_OCIO );
		GRABAR_EVENTO_COBRO( datos_cobro, NULL, devolucion );
        ADD_MEMORY_INT( __x_cobros_cantidad_servicios, 1 );
		ADD_MEMORY_DOUBLE( __x_cobros_importes_servicios,datos_cobro->importe );
        _SET_ESTADO( 3 );
    }
    if( _ESTADO == 3 ) {          
            /*----------------- Actualiza saldo en ctacte ---------------------*/
            if( ( cobros.ctacte == 1 || cobros.ctacte == 'S' ) && UTILIZA_SOCIOS ) {
                ACTUALIZA_SALDO_CLIENTE( -( datos_cobro->importe ) );
                ACTUALIZA_SOCIOS( -( datos_cobro->importe ) );
            }
			if( !IMPRIMIR_COBROS ) {
				/*Se decrementa el mes solo para la impresion, luego se le devuelve el valor original*/
				if( cobros.tarjeta ) {
			    	aux_fecha = GET_FECHA_VTO_PRIVADA(  ) ;
					SET_FECHA_VTO_PRIVADA( DECREMENTAR_MES( aux_fecha ));
				}
				/* Para la impresion de Cuenta */
 				if( datos_cobro->cuenta ) {
					//SET_MEMORY_INT( __ram_cobro_servicios, 2 );
					strcpy( ImpDatosAdi.nro_cuenta, datos_cobro->cuenta );

				}
				VALIDAR_COBROS( cobros.cod_valida, datos_cobro, cobros.nombre,
					devolucion, _MODO_COBRANZA );
				if( cobros.tarjeta ) {
					SET_FECHA_VTO_PRIVADA(aux_fecha );
				}
			}
            if( cobros.validacion_pie ) {
                VALIDAR( cobros.validacion_pie, _VALCOBROS, 0, 0, 0 );
            }
        if( IMPRIMIR_COBROS ) {
            VALIDAR_COBROS( cobros.cod_valida, datos_cobro, cobros.nombre, devolucion, _MODO_COBRANZA );
			IMPRIME_PIE_COMPROBANTE( COBRO, NO );
		}
        _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        GRABAR_LOG_SUPERVISOR( ST( S_COBROS ), USUARIO_CLAVE_ANTERIOR() ,LOG_COBROS,4);
        GRABAR_XYZ( 'X' );
        _SET_ESTADO( 5 );
    }
	if( _ESTADO == 90 ) {  //en caso de anular el cobro con ESC.
		if( cobros.tarjeta ) {
			GRABA_EVENTO_ANULADO(  );
		}
		_SET_ESTADO( 999 );
    }
	//Cobros Especiales que van por codigo de funcion
    if( _ESTADO ==  99 ) {
		POSICIONAR_COBRO_SELECCIONADO(  );
		if( !( cobros.devolucion == 'N' && devolucion ) ) {
			rta = CR1_EJECUTA_FUNCION( cobros.cod_funcion );
			/*******Si es cobro venta*******/
			if( cobros.venta ){
				LIMPIAR_PANEL_COBROS( );
				ExecuteAction( A_DES_COBROS_ACT_VENTAS );
				CLOSE_RECUPERO( _ptr, _handle );
				return;
			}
			
			if( rta ) {
				_SET_ESTADO( 5 );
			}else {
				_SET_ESTADO( 6 );
			}   
		} else {
			MENSAJE( "NO SE PUEDE DEVOLVER EL COBRO SELECCIONADO" );
			_SET_ESTADO( 999 );
		}
		//_SET_ESTADO( 999 );
    }
	if( _ESTADO == 5 ) {
		PAGOS_IMPRIME_PAGOS_ESPECIALES( devolucion, NO );
		_SET_ESTADO( 6 );
	}
	if( _ESTADO == 6 ) {
		ACTUALIZA_MEMORIA( );
		ACTUALIZAR_X();
		_SET_ESTADO( 7 );
	}
    
	/*--------------- fin ----------------*/

    if( modo_usuario == _MODO_INTERACTIVO ) {
		LIMPIAR_PANEL_COBROS( );
		//Se comenta por la accion A_DES_TODOS_ACT_VENTAS
        //ExecuteAction( A_DES_COBROS_ACT_VENTAS );
    }
	if( !RAM_P_TICKET )
		FIN_DE_TICKET( NO );
	
	if( modo_usuario == _MODO_INTERACTIVO ) {
		SETEAR_MODO_EJECUCION( RAM_MODO_EJECUCION_ANT );
	}

    SET_MEMORY_DOUBLE( __ram_no_reprocesar, 1 );
    CERRAR_CAJON();
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
/***********************************************************************/
void LIMPIAR_PANEL_COBROS( )
/***********************************************************************/
{
    /*---------------------- borra la pantalla ----------------------------*/
    
    //Borramos titulos y desabilitamos input
    ExecuteSAction( A_MUESTRA_TIT_COD_BARRA_COBRO, " " );
    ExecuteSAction( A_MUESTRA_TIT_COMPROB_COBRO, " " );
    ExecuteSAction( A_MUESTRA_TIT_CUENTA_COBRO, " " );
    ExecuteSAction( A_MUESTRA_TIT_IMPORTE_COBRO, " " );
    ExecuteSAction( A_MUESTRA_TIT_COD_FOLIO_G, " " );
    ExecuteSAction( A_MUESTRA_TIT_COD_ARTICULO_G, " " );
    ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_COD_BARRA_BORRAR, " ",
	    " ", NULL, " " );
    ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_INPUT_COMPROB_BORRAR, " ",
	    " ", NULL, " " );
    ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_INPUT_CUENTA_BORRAR, " ",
	    " ", NULL, " " );
    ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_INPUT_IMPORTE_BORRAR, " ",
	    " ", NULL, " " );

}

/*****************************************************************************/
void MOSTRAR_DATOS_SOLICITADOS_EN_COBROS( int datos_pedir, int tipo_cobro )
/*****************************************************************************/
{
    if( datos_pedir & _COBRO_DATOS_CODIGO_BARRA ) {
        ExecuteSAction( A_MUESTRA_TIT_COD_BARRA_COBRO, ST( S_Codigo_Barra ) );
        ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_COD_BARRA_AGREGAR, " ",
					" ", NULL, " " );
    } 
    if( datos_pedir & _COBRO_DATOS_NRO_COMPROBANTE ) {
        if( tipo_cobro == COBRO_GARANTIA ) {
            ExecuteSAction( A_MUESTRA_TIT_COD_FOLIO_G, ST( S_Nro_Folio ) );
        } else {
            ExecuteSAction( A_MUESTRA_TIT_COMPROB_COBRO, ST( S_Comprobante ) );
        }
        ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_INPUT_COMPROB_AGREGAR, " ",
					" ", NULL, " " );
    } 
    if( datos_pedir & _COBRO_DATOS_NRO_CUENTA ) {
        if( tipo_cobro == COBRO_GARANTIA ) {
            ExecuteSAction( A_MUESTRA_TIT_COD_ARTICULO_G, ST( S_Articulo ) );
        } else {
            ExecuteSAction( A_MUESTRA_TIT_CUENTA_COBRO, ST( S_N_Cuenta ) );
        }
        ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_INPUT_CUENTA_AGREGAR, " ",
					" ", NULL, " " );
    }
    if( datos_pedir & _COBRO_DATOS_IMPORTE ) {
        ExecuteSAction( A_MUESTRA_TIT_IMPORTE_COBRO, ST( S_Importe ) );
        ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_INPUT_IMPORTE_AGREGAR, " ",
					" ", NULL, " " );
    }

}

/*****************************************************************************/
void PANTALLA_COBROS( void )
/*****************************************************************************/
{
    char cad_modo[15];
    ExecuteAction( A_DES_VENTAS_ACT_COBROS );   
    strncpy( cad_modo, ST( S_COBROS ), sizeof( cad_modo ) );
    ExecuteSAction( A_MUESTRA_MODO_COBROS, cad_modo );
}
/*****************************************************************************/
int PIDE_COBRO( struct _datos_cobro *datos_cobro, int pos, int devolucion, int modo_usuario )
/*****************************************************************************/
{
    int seguir, rta = 0, tipo_cobro = 0, pedir_datos = 1;
	//char nro_tar[21];

	SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );

    seguir = ELEGIR_COBRO_BROWSE( modo_usuario, NO );
	/*---------------------- Dibuja la Pantalla Cobros----------------------------*/
    datos_cobro->datos_pedir = cobros.datos_pedir;
    datos_cobro->tipo_cobro = TIPO_COBRO_VENTA( cobros.codigo );

    MOSTRAR_DATOS_SOLICITADOS_EN_COBROS( datos_cobro->datos_pedir, datos_cobro->tipo_cobro );
	
	//------------------------- Cobro Venta -------------------------//
	if( seguir && cobros.venta ) {
		SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );
		if( modo_usuario == _MODO_AUTOMATICO 
				&& !cobros.cod_interno 
				&& strcmp( cobros.cod_barra, "" ) == 0 ) {
			strcpy( cobros.cod_barra, ITEM_COD_BARRA );
			cobros.cod_interno = ITEM_COD_ESPECIAL;
		}
		if( !BUSCA_ARTICULO_COBRO( cobros.cod_barra , cobros.cod_interno ) ){
			SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
			MENSAJE_TECLA( "NO HAY ARTICULO ASOCIADO AL COBRO, PRES. UNA TECLA PARA CONTINUAR" );
			seguir = 0;
			rta = 0;
		} else {
			strncpy( datos_cobro->cod_barra_art_venta, cobros.cod_barra,
				sizeof( datos_cobro->cod_barra_art_venta ));
			datos_cobro->cod_interno_art_venta = cobros.cod_interno;

			//Se toma el importe ingresado del articulo ( Cobros kinacu Recarga - art por importe) 
			//o el importe del articulo (Cobros kinacu Pines u otro Cobro Venta ).
			datos_cobro->importe = cobros.importe;
			if( modo_usuario == _MODO_AUTOMATICO ) {
				pedir_datos = 0;
				if( !datos_cobro->importe ) {
					datos_cobro->importe = ITEM_PRECIO;
				}
			} else {
				datos_cobro->importe = 0;
			}
		}
	}

    if( seguir && devolucion && cobros.devolucion == 'N' ) {
        seguir = 0;
		MENSAJE_TECLA( "COBRO NO HABILITADO PARA DEVOLUCION, PRES. UNA TECLA" );
    }
	if( seguir && cobros.cod_funcion && cobros.cod_funcion != 250 ) {
        datos_cobro->codigo = cobros.codigo;
		datos_cobro->hab_todos_medios = cobros.hab_todos_medios;
		datos_cobro->hab_todos_medios_dev = cobros.hab_todos_medios_dev;
		seguir = 0;
        rta = 4; //Cobro no tradicional.
    }
    //Funcion = 250 asignada a Cobros Kinacu
    if( seguir && cobros.cod_funcion == 250 ) {
        datos_cobro->codigo = cobros.codigo;
		datos_cobro->tarjeta = cobros.tarjeta;
		datos_cobro->hab_todos_medios = cobros.hab_todos_medios;
		datos_cobro->hab_todos_medios_dev = cobros.hab_todos_medios_dev;
		rta = PEDIR_DATOS_COBROS_KINACU( datos_cobro, pedir_datos,0);
        seguir = 0;
    }
    if( seguir ) {
        rta = 2;
        if( cobros.tarjeta && !VAR_TARJETAS_STATUS ) {
            if( !VALIDA_TARJETA( NO, NO, ABONO_TARJETA, NO, NULL, SI, -1/*NO*/ ) ) {
                CEREAR_STRUCT( _var_tarjetas );
            }
        }
        if( cobros.tarjeta && !VAR_TARJETAS_STATUS ) {
            MENSAJE_STRING( S_INGRESE_TARJETA_PRIMERO );
            rta = 0;
		} else if( cobros.tarjeta && 
				!COBROS_TARJETA_PERMITIDA( cobros.codigo, VAR_TARJETAS_TARJETA + 1 ) ) {
      		MENSAJE( "LA TARJETA INGRESADA NO CORRESPONDE AL COBRO" );
         	rta = 0;
        }
		else {
			strncpy( datos_cobro->cuenta, GET_NUMERO_TARJETA_PRIVADO( ), sizeof( datos_cobro->cuenta ) );
			if( pedir_datos ) 
				MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
			if( !pedir_datos || PEDIR_DATOS_COBRO( datos_cobro, pos ) ) {
				datos_cobro->codigo = cobros.codigo;
				datos_cobro->tarjeta = cobros.tarjeta;
				datos_cobro->hab_todos_medios = cobros.hab_todos_medios;
				datos_cobro->hab_todos_medios_dev = cobros.hab_todos_medios_dev;
				rta = 1;
			} else {
				rta = 0;
			}
		}
    }
    BORRAR_MENSAJE();
    return ( rta );
}

/*****************************************************************************/
int PEDIR_DATOS_COBROS_KINACU( struct _datos_cobro *datos_cobro, int activar, int origen )
/*****************************************************************************/
{
    int		rta = 0, sig = 0, fin = 0, salir = 0;
    double	importe = 0;
    //int  A_ACTIVAR_PANEL_KINACU;

    if( activar ) {
        /*A_ACTIVAR_PANEL_KINACU = A_ACTIVAR_PANEL_PAGATODO;
	    ExecuteAction( A_ACTIVAR_PANEL_KINACU );
	    ExecuteSAction( A_MUESTRA_TIT_IMPORTE_PAGATODO, ST( S_Importe ) );*/
    } else {
        salir = 1;
		rta = 1;
    }
	while( !salir ) {
            /*------------------------------Pedir Importe---------------------------------*/
        if( datos_cobro->importe == 0.00 ) {			
	        double	importe = 0.0;

            datos_cobro->importe = cobros.importe;
			//Los cobros kinacu Saldo tiene que tener configurado los datos a pedir a diferencia de los 
			//cobros kinacu Pines que son cobros con importes fijos
			if( datos_cobro->importe < 0.001 && !( datos_cobro->datos_pedir & _COBRO_DATOS_IMPORTE ) ) {
                return 1;
            }
			while( sig == 0 && fin == 0 && importe == 0.0 &&  datos_cobro->importe <= 0.00 ) {
                ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
				importe = PEDIR_FLOTANTE_COD_BARRA( ( char )( 51 - ancho_panel / 2 ), 
				    ( char ) 11, ( char )ENTEROS, ( char )DECIMALES, importe, NULL );
					if( importe > 0.0 ) {
					    sig = 1;
                        EVALUAR_COBROS_MONEDA_EXTRANJERA( importe, datos_cobro );
                        MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
                        rta = 1;
					} else if( importe == -1 ) {
					    fin = 1;
					}
            }
			    /*------------ Cobro con Importe configurado ------------*/
			if( datos_cobro->importe > 0.00 && importe == 0.0 && fin == 0 && sig == 0 ) {
                MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
                //MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( datos_cobro );
                rta = 1;
			}
        }
        if( rta ) {
            datos_cobro->codigo = cobros.codigo;
	        datos_cobro->tarjeta = cobros.tarjeta;
	        datos_cobro->hab_todos_medios = cobros.hab_todos_medios;
	        datos_cobro->hab_todos_medios_dev = cobros.hab_todos_medios_dev;
        }
        if( fin ) {
	        int ok = 0, k;

		    MENSAJE( "DESEA SALIR DE COBROS? <ENTER> PARA SALIR - <ESC> PARA CONTINUAR" );
            while( !ok ) {
		        k = GETCH();
			    switch( k ) {
			        case 13:
				        ok = 1;
					    salir = 1;
                        activar = 0;
					    break;
				    case 27:
				        ok = 1;
					    fin = 0;
					    //memset( datos_cobro, 0, sizeof( struct _datos_cobro ) );
                        MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
					    importe = 0;
					    break;
			    }
		    }
		    BORRAR_MENSAJE(  );
	    } else {
            salir = 1;
            if( cobros.datos_pedir ) {
                MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );
            }
        }
    }
	if( cobros.datos_pedir )
		BORRAR_MENSAJE();
    if( !activar ) {
        if( origen != _ORIGEN_DESC_TARJETA )
			if( datos_cobro != NULL) 
        ExecuteAction( A_DES_COBROS_ACT_VENTAS ); //forray, ver que esto refresca la pantalla de ventas cuando salis de un browser y puede afectar los videos
	    //ExecuteAction( A_DESACTIVAR_PANEL_PAGATODO );
    }
	 
	return ( rta );

}

/*****************************************************************************
int ELEGIR_COBRO()
/*****************************************************************************
{
    int seguir = 1, k, h, rta = 0;
    int cod;
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 27:
                //-------- ESC ---------
                seguir = 0;
                break;
            case 32:
                //-------- TOTAL ---------
                seguir = 0;
                break;
            default:
                if( k >= '0' && k <= '9' ) {
                    cod = k - '0';
                    SELECT( AREA_AUX4 );
                    GO( TOP );
                    for( h = 0;h < 10 && !BtrvEOF() && seguir;h++ ) {
                        if( cobros.codigo == cod ) {
                            rta = 1;
                            seguir = 0;
                        }
                        else {
                            SKIP( 1 );
                        }
                    }
                }
                break;
        }
    }
    return ( rta );
}*/

/*****************************************************************************/
int _consulta_cobros( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y/*, cod*/;
    y = _Y1 + dato;
    switch( comando ) {
//(Nota Unific MySQL-Linux): Lo siguiente estaba comentado en la version MySQL y no en 01.002
        /*case _BUSCAR:
           MOSTRAR_CADENA( _X1 + 1, _Y2 + 2, cadena );
            cod = atoi( cadena );
			SET_WHERE("CODIGO >= '%i'",cod);
			RUN_QUERY(NO);
            //GET_GREATER_E( ( char* )&cod );
            break;*/
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( cobros.codigo, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, cobros.nombre );
            }
            #endif
            MOSTRAR_ENTERO( _X1, y, "999", cobros.codigo );
            MOSTRAR_CADENA( _X1 + 4, y, cobros.nombre );
			//Cobros diparados por articulos
			/*if( ITEM_COD_COBRO ){
				UNGETCH( 13 );
			}*/

            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
			//Si aun no es fin de la tabla y el Cobro que se esta analizando no es hijo del Cobro Padre Seleccionado
			//Si el Cobro Padre es Marginal suponemos que sus hijos tambien son marginales.
			//Ordenado por Codigo Padre + Hijo, al primir cambio de padre lo filtra
            if( dato != 0 && !rta && cobros.codigo_cobro_padre != cod_cobro_ant[browse] ) {
                SKIP2( -dato );
                rta = 1;
            }
            if( !rta ) {
				int skip = ( dato >= 0 ) ? 1 : -1;
				int jump = dato;
				//Recorro la tabla, Ordenado por Codigo Padre + Hijo. 

					while( /*!BtrvEOF() && !BOF()*/ !dbEOF() && !dbBOF()
					//El cobro es hijo del padre seleccionado
					//cobros.codigo_cobro_padre: Cobro ACTUAL seleccionado
					//browse: Niveles de anidamiento
					//cod_cobro_ant: Cobro ANTERIOR seleccionado
					//El Cobro que se esta analizando ES HIJO del Cobro Padre Seleccionado
					&& cobros.codigo_cobro_padre == cod_cobro_ant[browse]
					&& ( //Si hay Cobro Marginal y El Cobro que se esta analizando es Miscelaneo
						( cobro_marginal && !cobros.venta )
							//Si hay Cobro Marginal y El Cobro que se esta analizando es Unico y no es una anulacion
							|| ( cobro_marginal && cobros.cobro_unico && !RAM_ANULACION )
							//???
							|| ( cobros.venta && devolucion_cobro && !MODO_DEVOLUCION 
									&& ( strlen( cobros.cod_barra ) || cobros.cod_especial ) 
								)
						)
				) {//Iguales
					//Salta registro para que no se visualize en el browser
						SKIP2( skip );
					//Acumulo la cantidad de saltos que doy (sumo o resto) para saber mi posicion
						jump += skip;
                        if( dbEOF() || /*BOF()*/ dbBOF() ) {
						//Si me encuentro en el ultimo registro y salto, es el Fin de la tabla entonces
						//retrocedo el JUMP para tener el nro de saltos correcto
						//que luego se analizara en el siguiente IF
							jump -= skip;
					}

				}
				//Si no es fin de tabla, se analiza desde la posicion( inicio, medio, o fin )
				//que quedo del while anterior
					if( /*BtrvEOF( )*/ dbEOF()
						 //El Cobro que se esta analizando NO ES HIJO del Cobro Padre Seleccionado
						 || cobros.codigo_cobro_padre != cod_cobro_ant[browse]
						 || ( //Si hay Cobro Marginal y El Cobro que se esta analizando es Miscelaneo
							   ( cobro_marginal && !cobros.venta )
							   //Si hay Cobro Marginal y El Cobro que se esta analizando es Unico y no es una anulacion
							   || ( cobro_marginal && cobros.cobro_unico && !RAM_ANULACION )
							   //???
							   || ( cobros.venta && devolucion_cobro && !MODO_DEVOLUCION 
										&& ( strlen( cobros.cod_barra ) 
											|| cobros.cod_especial ) 
								  )
							) 
				) {//Distintos
					//Desde donde estoy posicionado salto segun la cantidad de jump acumulado
					//Vuelvo al registro anterior al registro que realizo el 1er salto
					SKIP2( -jump );
					rta = 1;
                }
				
				if(	( cobro_marginal && !cobros.venta )
					|| ( cobro_marginal && cobros.cobro_unico && !RAM_ANULACION )
					|| ( cobros.venta && devolucion_cobro && !MODO_DEVOLUCION 
					       && ( strlen( cobros.cod_barra ) || cobros.cod_especial ) 
						)
					) {

						mostrar_cobro = 0;
						rta = 1;
				}

			}//fin if(!rta)
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
			
			if( limiteNiveles < ++browse ){
				browse=0;
				rta=0;
			}else{
				cod_cobro_ant[browse] = cobros.codigo;
				rta = 1;            
			}
            break;
        case _COMPARAR:
            rta = 2;
            break;
        case _TECLA:
            if( dato == 32 || dato == -73 || dato == -81 || ( dato >= 48 && dato <= 57 ) ) {
                b->seguir = 0;
			} else if( dato == 27 ) {
				if( browse > 0 )
					browse --;
				mostrar_cobro = 1;
            }
						/*veamos q si */
			if( dato == -72 && b->linea == 0 ) {
                //b->seguir = 0;
				b->posicionar=0;
				b->mostrar=1;
				//b->tecla=-999;
				 rta =  dbBOF() ;
				 rta =  dbEOF();
			} 
            break;
        case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
		case _LIMIT:
            if( dato > 0 ) {
                SET_LIMIT( dato, SI );
            } else {
                SET_LIMIT( NO, NO );
            }
            break;
    }

    return ( rta );
}
/**
/* (22/06/09)
/* @parameter solo_contar indica si solo se valida que haya algun cobro
/* pero no se realiza ninguna accion.
/*****************************************************************************/
int ELEGIR_COBRO_BROWSE( int modo_usuario, int solo_contar )
/*****************************************************************************/
{
    int rta = 1, ok = 1, CobroOnline = 0, CobroUnico = 0, h = 0;
    int registros = 0, venta=0;
	int cobro_online = 0, cob_unico = 0;
    browse = 1;
	memset(cod_cobro_ant,0,sizeof(cod_cobro_ant));
    _ATRIBUTO = 120;
    MOSTRAR_CADENA( _X1 + 1, _Y2 + 2, "    " );
    //SELECT( AREA_AUX4 );
	//SELECT_TABLE( T_COBROS, TT_ORIG );
    //Se verifica si hay Cobros Marginales en el ticket para definir indice
    //if( RAM_P_TICKET ) 
		//venta = 1;

	cobro_marginal = COBROS_VENTA_EN_TICKET( &cobro_on_tick , &cobro_unico);

	OPEN_TABLE( T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( struct _cobros ));

	if( cobro_marginal && !RAM_ANULACION ) {
		if( cobro_unico >= 0 ) {
			if( !( RAM_P_TICKET == 1 && !_TICKET_COD_COBRO( 0 ) ) ) {
				MENSAJE_TECLA("HAY COBRO UNICO EN TICKET. NO SE PUEDE CONTINUAR. PRES. UNA TECLA");   
			} else {
				MENSAJE_TECLA("EL COBRO ES UNICO, NO PUEDE HABER OTRO ITEMS EN EL TICKET. PRES. UNA TECLA");
			}
			return 0;
		}
	}
	
	//------- Cobros diparados por articulos -------//

	if( ITEM_COD_COBRO && modo_usuario == _MODO_AUTOMATICO ) {
		int cod_cobro;
		cod_cobro = ITEM_COD_COBRO;
		//SET_ORDER( 1 );
		//GET_EQUAL( ( char* )&cod_cobro );
		SET_WHERE( "CODIGO = '%d'", cod_cobro );
		RUN_QUERY(NO);
		if( FOUND2() && cobros.venta ){
			browse = 2;
			cod_cobro_ant[browse] = cobros.codigo_cobro_padre;
		}else{
			MENSAJE_TECLA("NO HAY COBRO ASOCIADO AL ARTICULO. PRES. UNA TECLA");
			return 0;
		}
	}

    if( modo_usuario == _MODO_INTERACTIVO 
		/*|| ( !ITEM_COD_COBRO && modo_usuario == _MODO_AUTOMATICO )*/ ) {
    do {
        if( browse > 1 && ( cod_cobro_ant[browse] > 0||cod_cobro_ant[--browse] > 0 ) ) {
       	SET_WHERE("CODIGO_COBRO_PADRE = '%i' AND CODIGO >= '%d'", cod_cobro_ant[browse], 0);
			SET_ORDER2("CODIGO_COBRO_PADRE, CODIGO");
			RUN_QUERY(NO);
            ////Si la cond de busqueda no es encontrada, la funcion GET_GREATER_E 
            ////se posiciona en algun registro. 
            //if( cod_cobro_ant[browse] != cobros.codigo_cobro_padre ) {
			//Lo anterior no es cierto en MySQL, dejamos el FOUND2
            if( !FOUND2() ) {
                ok = 0;
                //Se posiciona en el cobro antes de GET_GREATER_E, que modif la posicion
             	GO2( TOP );
				SET_ORDER2("CODIGO");
				//SET_WHERE("CODIGO_COBRO_PADRE = '%i'", cod_cobro_ant[browse] );
				SET_WHERE("CODIGO = '%i'", cod_cobro_ant[browse] );
				RUN_QUERY(NO);
            }
		} else {
			memset(cod_cobro_ant,0,sizeof(cod_cobro_ant));
			browse=1;
			GO2( TOP );
			SET_WHERE("");
			//SET_ORDER2("CODIGO_COBRO_PADRE,CODIGO");
			SET_ORDER2("CODIGO");
			RUN_QUERY(NO);

       
        }
		registros = RECCOUNT2( NO, NULL );
        if( ok && registros > 0 && !solo_contar ) {
            //rta = Consulta_cobros( _consulta_cobros );
			rta = Consulta_cobros( registros,_consulta_cobros );
        }
		if( registros == 0 && rta == 1 && browse == 1){
			ok = 0;
			rta = 0;
		}
	} while( browse && ok && !solo_contar );

    }
    return ( rta );
}
/*****************************************************************************/
int ELEGIR_COBRO_BROWSE_2()
/*****************************************************************************/
{
    int rta = 1, ok = 1;
    browse = 1;
    _ATRIBUTO = 120;
    MOSTRAR_CADENA( _X1 + 1, _Y2 + 2, "    " );
    SELECT_TABLE( T_COBROS, TT_ORIG );
    
	
	//Se verifica si hay Cobros Marginales en el ticket para definir indice
    //if( RAM_P_TICKET ) {
        cobro_marginal = COBROS_VENTA_EN_TICKET( &cobro_on_tick , &cobro_unico);

      //  if( !cobro_marginal ) {
      //    cobro_marginal =  1;
      //  }
    //}


	//Cobros diparados por articulos
	if( ITEM_COD_COBRO ){
		int cod_cobro;
		cod_cobro = ITEM_COD_COBRO;
		//SET_ORDER( 1 );
		//GET_EQUAL( ( char* )&cod_cobro );
		SET_WHERE( "CODIGO = '%d'", cod_cobro );
		RUN_QUERY(NO);
		if( FOUND2() && cobros.venta ){
			browse = 2;
			cod_cobro_ant[browse] = cobros.codigo_cobro_padre;
		}else{
			MENSAJE_TECLA("NO HAY COBRO ASOCIADO AL ARTICULO. PRES. UNA TECLA");
			return 0;
		}
	}

    do {
        if( browse > 1 ) {
            /*SET_ORDER( 3 );
            GET_EQUAL( ( char* )&cod_cobro_ant[browse] );*/
            //ind.codigo_cobro_padre = cod_cobro_ant[browse];
            //ind.codigo = 0;
            //SET_ORDER( 4 );
            //GET_GREATER_E( ( char* )&ind );
			SET_WHERE("CODIGO_COBRO_PADRE = '%i' AND CODIGO >= '%d'", cod_cobro_ant[browse], 0);
			SET_ORDER2("CODIGO_COBRO_PADRE, CODIGO");
			RUN_QUERY(NO);
            ////Si la cond de busqueda no es encontrada, la funcion GET_GREATER_E 
            ////se posiciona en algun registro. 
            //if( cod_cobro_ant[browse] != cobros.codigo_cobro_padre ) {
			//Lo anterior no es cierto en MySQL, dejamos el FOUND2
            if( !FOUND2() ) {
                ok = 0;
                //Se posiciona en el cobro antes de GET_GREATER_E, que modif la posicion
                //GO( TOP );
                //SET_ORDER( 1 );
                //GET_EQUAL( ( char* )&cod_cobro_ant[browse] );
				GO2( TOP );
				SET_ORDER2("CODIGO");
				SET_WHERE("CODIGO_COBRO_PADRE = '%i'", cod_cobro_ant[browse] );
				RUN_QUERY(NO);
            }
        } else {


            if( cobro_marginal ) {//Solo para los cobros Marginales y los cobros de venta no marginales
				SET_WHERE( "VENTA != 0" );
            /*    ind.venta = 1;
                ind.codigo_cobro_padre = 0;
                SET_ORDER( 5 );//VENTA + CODIGO_COBRO_PADRE
				GO( TOP );
                GET_GREATER_E( ( char* )&ind );*/
            } else {// Para todos los cobros tanto Marginales como Miscelaneos*/
				SET_WHERE("");
			}
			SET_ORDER2("CODIGO_COBRO_PADRE,CODIGO");
			RUN_QUERY(NO);
            
        }
		ok = RECCOUNT2( NO, NULL );
        if( ok) {
            rta = Consulta_cobros( 0,_consulta_cobros );
		}else {
			MENSAJE_TECLA("NO HAY COBRO ASOCIADOS");
			rta = 0;
		}
	} while( browse && ok );

    return ( rta );
}

/*************************************************************************************/
int EVALUAR_MASCARA_COD_BARRA( int cod_cobro, char *scanner_cod_barra,
                               struct _datos_cobro *datos_cobro )
/*************************************************************************************/
{
    struct  _mascara_scanner masc_scanner;
    int rta = 1;
    if( OPEN_TABLE( T_MASCARA_SCANNER, TT_ORIG, ( char* )&masc_scanner, sizeof( masc_scanner ) ) == 0 ) {
        SELECT_TABLE( T_MASCARA_SCANNER, TT_ORIG );
        SET_ORDER2( "COD_COBRO" );
		SET_WHERE( "COD_COBRO = '%i'", cod_cobro );
		RUN_QUERY( NO );
        while( masc_scanner.cod_cobro == cod_cobro && rta == 1 && !dbEOF(  ) && !dbBOF(  ) ) {
            rta = ANALIZAR_CODIGO_BARRA_COBROS( scanner_cod_barra, datos_cobro,
                                                masc_scanner.mascara );
			datos_cobro->codigo = cod_cobro;
            SKIP2( 1 );
        }
		if( rta && rta != 2 ) {
			//Si cereo aqui datos_cobro no me deja continuar (al presionar ENTER) en la 
			//funcionalidad ya que cereo el cobro cargado. 
			//memset( datos_cobro, 0, sizeof( struct _datos_cobro ) );
			MENSAJE_STRING( S_CODIGO_INVALIDO );
			BORRAR_MENSAJE( );
		} else if( !EJECUTAR_L_VALID( scanner_cod_barra, cobros.nro_funcion_valid ) ) {
			//memset( datos_cobro, 0, sizeof( struct _datos_cobro ) );
			MENSAJE_STRING( S_DIGITO_VERIFICADOR_INVALIDO );
			BORRAR_MENSAJE(  );
			rta = 1;
		}
        CLOSE_TABLE( T_MASCARA_SCANNER, TT_ORIG );
    }
    return !rta;
}
/********************************************************************************/
struct _obtenido_codigo_barra {
	// Datos comprobante.
	char      C[41];			  // NRO CUENTA
	char      N[13];			  // NRO COMPROBANTE
	// Importe primer Vencimiento
	char      E[11];			  // PARTE ENTERA
	char      K[6];				  // PARTE DECIMAL
	// Fecha Primer Vencimiento
	char      G[5];				  // DIF. EN DIAS DESDE 1/1/1998
	char      A[5];				  // ANIO
	char      M[3];				  // MES
	char      D[3];				  // DIA
	// Fecha Segundo Vencimiento
	char      S[4];				  // DIAS
	char      Z[4];				  // Cantidad de dias partiendo del 01/01/anio corriente hasta el 2do vto
	char      Y[2];				  // Anio de la decada del aÃ±o en curso para el 2er vto
	// Importe Segundo VTO
	char      Q[11];			  // PARTE ENTERA
	char      P[6];				  // PARTE DECIMAL
	char      H[3];				  // COEFICIENTE RECARGO ENTERO
	char      J[3];				  // COEFICIENTE RECARGO DECIMAL
	// Recargo 1 a sumar al importe
	char      I[6];				  // PARTE ENTERA
	char      F[3];				  // PARTE DECIMAL
	// Fecha Tercer Vencimiento
	char      T[4];				  // DIAS
	char      U[4];               // Cantidad de dias partiendo del 01/01/anio corriente hasta el 3er vto
	char      R[2];				  // Anio de la decada del anio en curso para el 3er vto
	// Importe Tercer VTO
	char      a[11];			  // PARTE ENTERA
	char      b[6];				  // PARTE DECIMAL
	// Recargo 2 a sumar al importe
	char      V[6];				  // PARTE ENTERA
	char      W[3];				  // PARTE DECIMAL
	//Anio Garantia
	char      B[2];
} obtenido_cod_barra;
/*****************************************************************************/
int ANALIZAR_CODIGO_BARRA_COBROS( char *scanner_cod_barra,
	struct _datos_cobro *datos_cobro, char *mascara )
/*****************************************************************************/
{
    char *letra_mascara = mascara;   // NEW  cobros.patron_codigo_barra
    char *letra_scanner = scanner_cod_barra;
	double primer_importe = 0.0;
    int iError = 0;
    int i = 0, seguir = 0;
	int FECHA_DOS = 0;
	int annio_1_venc = 0, 	fecha_1_venc = 0;
	int annio_2_venc = 0, 	fecha_2_venc = 0, dif_dias_2_venc = 0;
	int annio_3_venc = 0, 	fecha_3_venc = 0, dif_dias_3_venc = 0;

	memset( &obtenido_cod_barra, 0, sizeof( obtenido_cod_barra ) );

	// Controla el largo de la barra
    for( i = 0, seguir = 1;scanner_cod_barra[i] && seguir;i++ ) {
        if( scanner_cod_barra[i] < '0' || scanner_cod_barra[i] > '9' ) {
		//&& scanner_cod_barra[i] != '-' ) {
            scanner_cod_barra[i] = 0;
            seguir = 0;
        }
    }
    TRIM( letra_mascara );
    if( strlen( scanner_cod_barra ) != strlen( letra_mascara ) ) {
        iError = 1;
    }
    while( *letra_mascara && !iError ) {
        if( *letra_mascara >= '0' && *letra_mascara <= '9' ) {
            if( *letra_mascara != *letra_scanner ) {
                iError = 1;
                break;
            }
        }
        else {
            switch( *letra_mascara ) {
                    // Importe primer Vencimiento
                case 'E':
                    // PARTE ENTERA
                    if( strlen( obtenido_cod_barra.E ) + 1 < sizeof( obtenido_cod_barra.E ) ) {
                        obtenido_cod_barra.E[strlen( obtenido_cod_barra.E )] = *letra_scanner;
                    }
                    break;
                case 'K':
                    // PARTE DECIMAL
                    if( strlen( obtenido_cod_barra.K ) + 1 < sizeof( obtenido_cod_barra.K ) ) {
                        obtenido_cod_barra.K[strlen( obtenido_cod_barra.K )] = *letra_scanner;
                    }
                    break;
                case 'C':
                    // NRO CUENTA
                    if( strlen( obtenido_cod_barra.C ) + 1 < sizeof( obtenido_cod_barra.C ) ) {
                        obtenido_cod_barra.C[strlen( obtenido_cod_barra.C )] = *letra_scanner;
                    }
                    break;
                case 'N':
                    // NRO COMPROBANTE
                    if( strlen( obtenido_cod_barra.N ) + 1 < sizeof( obtenido_cod_barra.N ) ) {
                        obtenido_cod_barra.N[strlen( obtenido_cod_barra.N )] = *letra_scanner;
                    }
                    // Fecha Primer Vencimiento
                    break;
                case 'G':
                    // DIF. EN DIAS DESDE 1/1/1998
                    if( strlen( obtenido_cod_barra.G ) + 1 < sizeof( obtenido_cod_barra.G ) ) {
                        obtenido_cod_barra.G[strlen( obtenido_cod_barra.G )] = *letra_scanner;
                    }
                    break;
                case 'A':
                    // ANIO
                    if( strlen( obtenido_cod_barra.A ) + 1 < sizeof( obtenido_cod_barra.A ) ) {
                        obtenido_cod_barra.A[strlen( obtenido_cod_barra.A )] = *letra_scanner;
                    }
                    break;
                case 'M':
                    // MES
                    if( strlen( obtenido_cod_barra.M ) + 1 < sizeof( obtenido_cod_barra.M ) ) {
                        obtenido_cod_barra.M[strlen( obtenido_cod_barra.M )] = *letra_scanner;
                    }
                    break;
                case 'D':
                    // DIA
                    if( strlen( obtenido_cod_barra.D ) + 1 < sizeof( obtenido_cod_barra.D ) ) {
                        obtenido_cod_barra.D[strlen( obtenido_cod_barra.D )] = *letra_scanner;
                    }
                    break;
                    // Fecha Segundo Vencimiento
                case 'S':
                    // DIAS
                    if( strlen( obtenido_cod_barra.S ) + 1 < sizeof( obtenido_cod_barra.S ) ) {
                        obtenido_cod_barra.S[strlen( obtenido_cod_barra.S )] = *letra_scanner;
                    }
                    break;
				case 'Z':
                    // Cantidad de dias partiendo del 01/01/anio corriente hasta el 2do vto
                    if( strlen( obtenido_cod_barra.Z ) + 1 < sizeof( obtenido_cod_barra.Z ) ) {
                        obtenido_cod_barra.Z[strlen( obtenido_cod_barra.Z )] = *letra_scanner;
                    }
                    break;
				case 'Y':
                    // Anio de la decada del anio en curso para el 2er vto
                    if( strlen( obtenido_cod_barra.Y ) + 1 < sizeof( obtenido_cod_barra.Y ) ) {
                        obtenido_cod_barra.Y[strlen( obtenido_cod_barra.Y )] = *letra_scanner;
                    }
                    break;
                    // Importe Segundo VTO
                case 'Q':
                    // PARTE ENTERA
                    if( strlen( obtenido_cod_barra.Q ) + 1 < sizeof( obtenido_cod_barra.Q ) ) {
                        obtenido_cod_barra.Q[strlen( obtenido_cod_barra.Q )] = *letra_scanner;
                    }
                    break;
                case 'P':
                    // PARTE DECIMAL
                    if( strlen( obtenido_cod_barra.P ) + 1 < sizeof( obtenido_cod_barra.P ) ) {
                        obtenido_cod_barra.P[strlen( obtenido_cod_barra.P )] = *letra_scanner;
                    }
                    break;
                case 'H':
                    // COEFICIENTE RECARGO ENTERO
                    if( strlen( obtenido_cod_barra.H ) + 1 < sizeof( obtenido_cod_barra.H ) ) {
                        obtenido_cod_barra.H[strlen( obtenido_cod_barra.H )] = *letra_scanner;
                    }
                    break;
                case 'J':
                    // COEFICIENTE RECARGO DECIMAL
                    if( strlen( obtenido_cod_barra.J ) + 1 < sizeof( obtenido_cod_barra.J ) ) {
                        obtenido_cod_barra.J[strlen( obtenido_cod_barra.J )] = *letra_scanner;
                    }
                    break;
				case 'I':				  
					// IMPORTE RECARGO ENTERO A SUMAR
					if( strlen( obtenido_cod_barra.I ) + 1 < sizeof( obtenido_cod_barra.I ) )
						obtenido_cod_barra.I[strlen( obtenido_cod_barra.I )] = *letra_scanner;
					break;
				case 'F':				  
					// IMPORTE RECARGO DECIMAL A SUMAR
					if( strlen( obtenido_cod_barra.F ) + 1 < sizeof( obtenido_cod_barra.F ) )
						obtenido_cod_barra.F[strlen( obtenido_cod_barra.F )] = *letra_scanner;
					break;
					// Fecha Tercer Vencimiento
				case 'T':				 
					// DIAS
					if( strlen( obtenido_cod_barra.T ) + 1 < sizeof( obtenido_cod_barra.T ) )
						obtenido_cod_barra.T[strlen( obtenido_cod_barra.T )] = *letra_scanner;
					break;
				case 'U':				 
					// Cantidad de dias partiendo del 01/01/aÃ±o corriente hasta el 3er vto
					if( strlen( obtenido_cod_barra.U ) + 1 < sizeof( obtenido_cod_barra.U ) )
						obtenido_cod_barra.U[strlen( obtenido_cod_barra.U )] = *letra_scanner;
					break;
				case 'R':				 
					// AÃ±o de la decada del aÃ±o en curso para el 3er vto
					if( strlen( obtenido_cod_barra.R ) + 1 < sizeof( obtenido_cod_barra.R ) )
						obtenido_cod_barra.R[strlen( obtenido_cod_barra.R )] = *letra_scanner;
					break;					
				case 'a':				  
					// IMPORTE ENTERO Tercer VTO
					if( strlen( obtenido_cod_barra.a ) + 1 < sizeof( obtenido_cod_barra.a ) )
						obtenido_cod_barra.a[strlen( obtenido_cod_barra.a )] = *letra_scanner;
					break;
				case 'b':				  
					// IMPORTE DECIMAL Tercer VTO
					if( strlen( obtenido_cod_barra.b ) + 1 < sizeof( obtenido_cod_barra.b ) )
						obtenido_cod_barra.b[strlen( obtenido_cod_barra.b )] = *letra_scanner;
					break;
				case 'V':				  
					// IMPORTE RECARGO 2 ENTERO A SUMAR
					if( strlen( obtenido_cod_barra.V ) + 1 < sizeof( obtenido_cod_barra.V ) )
						obtenido_cod_barra.V[strlen( obtenido_cod_barra.V )] = *letra_scanner;
					break;
				case 'W':				  
					// IMPORTE RECARGO 2 DECIMAL A SUMAR
					if( strlen( obtenido_cod_barra.W ) + 1 < sizeof( obtenido_cod_barra.W ) )
						obtenido_cod_barra.W[strlen( obtenido_cod_barra.W )] = *letra_scanner;
					break;
				 case 'B':
                    // Años Garantia
                    if( strlen( obtenido_cod_barra.B ) + 1 < sizeof( obtenido_cod_barra.B ) ) {
                        obtenido_cod_barra.B[strlen( obtenido_cod_barra.B )] = *letra_scanner;
                    }
                    break;
            }
        };
        letra_mascara++;
        letra_scanner++;
    }
	
	//Analizar lo Obtenido
	if( !iError ) { 
		datos_cobro->comprobante = atof( obtenido_cod_barra.N );
		strncpy( datos_cobro->cuenta, obtenido_cod_barra.C, sizeof( datos_cobro->cuenta ) );
		datos_cobro->importe = 0;
		FECHA_DOS = _GET_FECHA_DOS(  );
		// -----------------------------------------------------
		// Primero trata de Obtener el primer vto en formato 'G'
		// -----------------------------------------------------

		if( strlen( obtenido_cod_barra.G ) > 0 ) {//G: Cantidad de dias partiendo del 01/01/anio en curso (maximo 3 caracteres) hasta el 1er vto
			if( strlen( obtenido_cod_barra.G ) <= 3 ) {
				if( strlen( obtenido_cod_barra.A ) == 1//A: Anio del 1er vto
						|| strlen( obtenido_cod_barra.A ) == 2 ) {

					annio_1_venc = atoi (obtenido_cod_barra.A);
					//Le resto uno (-1) porque ya estoy incluyendo ese uno (1) 
					//cuando parto del dia 1/1/annio_1_venc
					fecha_1_venc = INC_DIA( FECHA( 1, 1, annio_1_venc ), ( atoi( obtenido_cod_barra.G ) - 1 ) );
				} //Nose para que es esto? else {
					//?fecha_1_venc = INC_DIA( FECHA( 1, 1, 98 ), ( atoi( obtenido_cod_barra.G ) - 1 ) );
				//?}
				else {
					fecha_1_venc = 0;	
				}
			} else {
				fecha_1_venc = 0;
			}
		} else {
			int dia = 0, mes = 0, anio = 0;

			if( strlen( obtenido_cod_barra.D )//D: Dia del 1er vto
					|| strlen( obtenido_cod_barra.M )//M: Mes del 1er vto
					|| strlen( obtenido_cod_barra.A ) ) {
				dia = atoi( obtenido_cod_barra.D );
				mes = atoi( obtenido_cod_barra.M );
				if( strlen( obtenido_cod_barra.A ) == 4 ) {
					//Anio 4 digitos
					anio = atoi( &obtenido_cod_barra.A[2] );	
				} else {
					if( strlen( obtenido_cod_barra.A ) == 1 
						|| strlen( obtenido_cod_barra.A ) == 2 ) {
						anio = atoi( obtenido_cod_barra.A );
					} else {
						dia = 0, mes = 0;
					}
				}
				fecha_1_venc = FECHA( dia, mes, anio );
			}
		}
        //E: PARTE ENTERA del 1er vto, K: PARTE DECIMAL del 1er vto
		primer_importe =
			atoi( obtenido_cod_barra.E ) + ( atoi( obtenido_cod_barra.K ) / 100.0 );

		if( fecha_1_venc >= FECHA_DOS || !fecha_1_venc ) {
			datos_cobro->importe = primer_importe;
		} else {
			// -----------------------------------------------------
			// Vencido controlar si existe fecha 2
			// -----------------------------------------------------

			// -----------------------------------------------------------------
			// Segundo Vencimiento por recargo
			// -----------------------------------------------------------------
			if( strlen( obtenido_cod_barra.H ) > 0 ||//H: COEFICIENTE RECARGO ENTERO del 1er vto
					strlen( obtenido_cod_barra.J ) > 0 ) {//J: COEFICIENTE RECARGO DECIMAL del 1er vto
				double recargo = 0.0;

				recargo = atoi( obtenido_cod_barra.H ) + atoi( obtenido_cod_barra.J ) / 100.0;
				dif_dias_2_venc = atoi( obtenido_cod_barra.S );
				datos_cobro->importe =
					primer_importe + ( primer_importe * ( recargo ) * dif_dias_2_venc ) / 36500.0;

			} else if( strlen( obtenido_cod_barra.Q ) ||//Q: Importe del Segundo VTO (PARTE ENTERA)
					strlen( obtenido_cod_barra.P ) ) {	//P: Importe del Segundo VTO (PARTE DECIMAL)
				datos_cobro->importe =
					atoi( obtenido_cod_barra.Q ) + atoi( obtenido_cod_barra.P ) / 100.0;

			} else if( strlen( obtenido_cod_barra.I ) ||//I: IMPORTE RECARGO ENTERO A SUMAR al importe del 1er Vto
				strlen( obtenido_cod_barra.F ) ) {	    //F: IMPORTE RECARGO DECIMAL A SUMAR al importe del 1er Vto
				datos_cobro->importe = primer_importe +
					atoi( obtenido_cod_barra.I ) + ( atoi( obtenido_cod_barra.F ) / 100.0);

			} else {
				datos_cobro->importe = primer_importe;
			}

			// -----------------------------------------------------------------
			// controla que este entre fecha vencimiento 1 y fecha vencimiento 2
			// -----------------------------------------------------------------
			if( strlen( obtenido_cod_barra.S ) > 0 ) {//S: Cantidad dias despues del 1er vto hasta el 2do vto
				dif_dias_2_venc = atoi( obtenido_cod_barra.S );
			} else {
				if( strlen( obtenido_cod_barra.Z ) > 0 ) {//Z: Cantidad de dias partiendo del 01/01/anio en curso hasta el 2do vto
					if( strlen( obtenido_cod_barra.Y ) == 1 ) {//Y: AÃ±o de la decada del anio en curso para el 2do vto (maximo 1 caracteres)
						annio_2_venc = atoi( obtenido_cod_barra.Y );
						//Le resto uno (-1) porque ya estoy incluyendo ese uno (1) 
						//cuando parto del dia 1/1/annio_2_venc
						fecha_2_venc = INC_DIA( FECHA( 1, 1, annio_2_venc ), 
							( atoi( obtenido_cod_barra.Z ) - 1 ) );
					} else {
						dif_dias_2_venc = 0;
					}
				} else {
					dif_dias_2_venc = 0;
				}
			}
			fecha_2_venc = ( fecha_2_venc )? 
				fecha_2_venc : INC_DIA( fecha_1_venc, dif_dias_2_venc );

			if( !( fecha_2_venc >= FECHA_DOS ) ) {
				// -----------------------------------------------------------------
				// controla que este entre fecha vencimiento 2 y fecha vencimiento 3
				// -----------------------------------------------------------------
				if( strlen( obtenido_cod_barra.T ) > 0 ) {//T: Cantidad dias despues del 2er vto hasta el 3ro vto
					dif_dias_3_venc = atoi( obtenido_cod_barra.T );
				} else {
					if( strlen( obtenido_cod_barra.U ) > 0 ) {//U: Cantidad de dias partiendo del 01/01/anio en curso hasta el 3er vto
						if( strlen( obtenido_cod_barra.R ) == 1 ) {//R: Anio de la decada del anio en curso para el 3er vto
							annio_3_venc = atoi( obtenido_cod_barra.R );
							//Le resto uno (-1) porque ya estoy incluyendo ese uno (1) 
							//porque parto del dia 1/1/annio_3_venc
							fecha_3_venc = INC_DIA( FECHA( 1, 1, annio_3_venc ), 
									( atoi( obtenido_cod_barra.U ) - 1 ) );
						} else {
							dif_dias_3_venc = 0;
						}
					} else {
						dif_dias_3_venc = 0;
					}
				}

				// -----------------------------------------------------------------
				// controla que este entre fecha vencimiento 2 y fecha vencimiento 3
				// -----------------------------------------------------------------
   				dif_dias_3_venc = dif_dias_2_venc + dif_dias_3_venc;

				fecha_3_venc = ( fecha_3_venc )? 
					fecha_3_venc : INC_DIA( fecha_1_venc, dif_dias_3_venc );

				if( !( fecha_3_venc >= FECHA_DOS ) ) {
					MENSAJE( ST( S_COMPROBANTE_VENCIDO ) );
					BORRAR_MENSAJE(  );

					if( cobros.permite_pago_venci != 'S' ) {
						datos_cobro->comprobante = 0;
						strncpy( datos_cobro->cuenta, "", sizeof( datos_cobro->cuenta ) );
						datos_cobro->importe = 0;
						iError = 2;
					} else {
						if( ( strlen( obtenido_cod_barra.V ) > 0  ||
								strlen( obtenido_cod_barra.W ) > 0 )) {

          					datos_cobro->importe = primer_importe +
	      			               atoi( obtenido_cod_barra.V ) + ( atoi( obtenido_cod_barra.W ) / 100.0);
			            }
					}
				} else { 
					if( strlen( obtenido_cod_barra.a ) ||
							strlen( obtenido_cod_barra.b ) ) {	    
						datos_cobro->importe =
							atoi( obtenido_cod_barra.a ) + atoi( obtenido_cod_barra.b ) / 100.0;
					} else {
						//importe a sumar de 2do vencimiento
						if( ( strlen( obtenido_cod_barra.T ) > 0 
								|| strlen( obtenido_cod_barra.S ) > 0 ) 
								&& ( strlen( obtenido_cod_barra.V ) > 0  
								|| strlen( obtenido_cod_barra.W ) > 0 ) ) {
          					datos_cobro->importe = primer_importe +
	      							   atoi( obtenido_cod_barra.V ) + ( atoi( obtenido_cod_barra.W ) / 100.0);
						}
					}
				}
			}
		}
	}

    if( !iError ) {
        strcpy( datos_cobro->cod_barra, scanner_cod_barra );
		datos_cobro->anios_garantia = atoi( obtenido_cod_barra.B );
    }
    return ( iError );
}
/*****************************************************************************/
int PEDIR_DATOS_COBRO( struct _datos_cobro *datos_cobro, int pos )
/*****************************************************************************/
{
    int		rta = 0, salir = 0, sig = 0, fin = 0, cant_dig = 0;
    char	*cuenta = NULL;
	char	cod_barra_aux[81], cuenta_aux[41], comprobante_aux[11];
	long	codigo_barra = 0, comprobante = 0, _cuenta = 0;
    double	importe = 0;
	int solicitar_importe = 1;

	/*------------------------ Pide los datos -----------------------------*/
	while( !salir ) {
		/*----------------------- Pide Codigo Barra ----------------------------*/
		_PERMITIR_SCANNER = SI;

		while( sig == 0 && fin == 0 && datos_cobro->datos_pedir & _COBRO_DATOS_CODIGO_BARRA ) {
			memset( cod_barra_aux, 0, sizeof( cod_barra_aux ) );
            ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
			codigo_barra = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
				( char )0, ( char )80, cod_barra_aux, NULL, 0, SI, NO, NULL );
            if( codigo_barra > 0 && strlen( cod_barra_aux ) > 0 ) {
				if( EVALUAR_MASCARA_COD_BARRA( cobros.codigo,
					cod_barra_aux, datos_cobro ) ) {
					sig = 1;
					MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
				} else {
					fin = 1;
				}
			} else if( codigo_barra == 0 ) {
				fin = 1;
			} else if( strlen( cod_barra_aux ) == 0 ) {
				if( cobros.barra_obligatoria == 'N' )
					sig = 1;
				codigo_barra = 0;
            }
        }
		_PERMITIR_SCANNER = NO;
        sig = 0;
		/*------------------------------Pedir Comprobante-----------------------------*/
		if( datos_cobro->comprobante == 0 && datos_cobro->datos_pedir & _COBRO_DATOS_NRO_COMPROBANTE) {
			while( sig == 0 && fin == 0 && comprobante == 0 ) {
				memset( comprobante_aux, 0, sizeof( comprobante_aux ) );
				ExecuteAction( A_MUESTRA_DATO_COMPROB_COBRO );
				comprobante = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
					( char )0, ( char )10, comprobante_aux, NULL, 0, SI, NO , NULL);
				if( comprobante > 0 && strlen( comprobante_aux ) > 0 ) {
					sig = 1;
					datos_cobro->comprobante = atof( comprobante_aux );
				} else if( comprobante == 0 ) {
					fin = 1;
				} else if( strlen( comprobante_aux ) == 0 ) {
					comprobante = 0;
				}
			}
		}
		sig = 0;
		/*------------------------------Pedir Cuenta----------------------------------*/
		if( strlen( datos_cobro->cuenta ) == 0 && datos_cobro->datos_pedir & _COBRO_DATOS_NRO_CUENTA ) {
			while( sig == 0 && fin == 0 && _cuenta == 0 ) {
				memset( cuenta_aux, 0, sizeof( cuenta_aux ) );
				ExecuteAction( A_MUESTRA_DATO_CUENTA_COBRO );
				_cuenta = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
					( char )9, ( char )40, cuenta_aux, NULL, 0, SI, NO, NULL );
				TRIM( cuenta_aux );
				cant_dig = strlen( cuenta_aux );
				cuenta = cuenta_aux;
				cuenta[cant_dig] = '\0';
				if( _cuenta > 0 && strlen( cuenta_aux ) > 0 ) {
					sig = 1;
					strncpy( datos_cobro->cuenta, cuenta, cant_dig );
					datos_cobro->cuenta[cant_dig] = '\0';
				} else if( _cuenta == 0 ){
					fin = 1;
				} else if( strlen( cuenta_aux ) == 0 ) {
					_cuenta = 0;
				}
			}
		}
        sig = 0;
		/*------------------------------Pedir Importe---------------------------------*/
		if( datos_cobro->importe == 0.00 ) {
			datos_cobro->importe = cobros.importe;
		
			if( cobros.venta ) {
				if( datos_cobro->importe <= 0.00 ) {
					solicitar_importe = 0;
				}
			} else {
				if(	!( datos_cobro->datos_pedir & _COBRO_DATOS_IMPORTE ) ) {
					solicitar_importe = 0;
				}//configurar el importe o los datos a solicitar
			}
			while( sig == 0 && fin == 0 && importe == 0.0 && datos_cobro->importe <= 0.00 
				&& solicitar_importe ) {
				ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
				importe = PEDIR_FLOTANTE_COD_BARRA( ( char )( 51 - ancho_panel / 2 ), ( char )11,
					( char )ENTEROS, ( char )DECIMALES, importe, NULL );
				if( importe > 0 ) {
					sig = 1;
					EVALUAR_COBROS_MONEDA_EXTRANJERA( importe, datos_cobro );
					MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
				} else if( importe == -1 ) {
					fin = 1;
				}
			} 
			/*------------ Cobro con Importe configurado ------------*/
			if( datos_cobro->importe > 0.00 && importe == 0.0 && fin == 0 && sig == 0 ) {
				MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
			}
		}
		sig = 0;
		/*------------------------Pedir Datos Adicionales------------------------------*/
		if( cobros.datos_especiales == 3 && sig == 0 && fin == 0 ) {
			if( PAGOS_PEDIR_DATOS_ADICIONALES( ( RAM_FLAG_COBRO_VENTA ) ? RAM_P_TICKET : 0, 0, cobros.cod_solicitud,
				0, D_COBROS, NULL, 1, NULL, NULL , '\0', '\0') ) {
				struct _datos datos;

				if( _PAGOS_OBTENER_DATOS( ( RAM_FLAG_COBRO_VENTA ) ? RAM_P_TICKET : 0, 0, &datos, D_COBROS ) ) {
					strncpy( datos_cobro->dato_adicional, datos.valor,
						sizeof( datos_cobro->dato_adicional ) );
					//Cobros venta
					if( RAM_FLAG_COBRO_VENTA ){
						AGREGA_BIN_EV_DMEDIO_A_BTRV( RAM_P_TICKET, D_COBROS );
					}

					sig = 1;
				}
			} else {
				fin = 1;
			}
		}
		if( fin ) {
			int ok = 0, k;
			MENSAJE( "DESEA SALIR DE COBROS? <ENTER> PARA SALIR - <ESC> PARA CONTINUAR" );
			while( !ok ) {
				k = GETCH();
				switch( k ) {
					case 13:
						ok = 1;
						salir = 1;
						break;
					case 27:
						ok = 1;
						fin = 0;						
						strncpy( datos_cobro->cod_barra, "", sizeof( datos_cobro->cod_barra ) );
						datos_cobro->comprobante = 0;
						strncpy( datos_cobro->cuenta, "", sizeof( datos_cobro->cuenta ) );
						datos_cobro->importe = 0;
                        MOSTRAR_DATOS_CODIGO_BARRA( datos_cobro );
                        codigo_barra = 0;
                        comprobante = 0;
                        _cuenta = 0;
                        importe = 0;
                        break;
                }
            }
            BORRAR_MENSAJE();
		} else {
            salir = 1;
            MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );
        }
	}
	BORRAR_MENSAJE();
    if( fin == 0 ) {
        if( datos_cobro->comprobante >= 0 && VALIDAR_NRO_COMPROBANTE( datos_cobro->comprobante ) ) {
            if( datos_cobro->cuenta && VALIDAR_CUENTA( datos_cobro->cuenta, NULL ) ) {
                if( datos_cobro->importe >= 0 ) {
                    if( VERIFICA_CUPO_CLIENTE( datos_cobro->importe, 0.0, SI, SI ) ) {
                        //if( strlen( cod_barra_aux ) != 0 ) {
                            GRABAR_COD_BARRA_EN_BINARIO( cod_barra_aux, pos );
                        //}
                        rta = 1;
                    }
                }
            }
        }
    }
	
    return ( rta );
}

/*****************************************************************************/
void GRABAR_EVENTO_COBRO( struct _datos_cobro *datos_cobro, char *cta_cmr, int anulacion )
/*****************************************************************************/
{
    char _cta_cmr[11], *cod_barra = NULL;

    if( cta_cmr ) {
        strcpy( _cta_cmr, cta_cmr );    /* provisorio */
    }
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    //event_ticket.nro_ticket = ( long )cobros.codigo;
	event_ticket.nro_ticket = 0;
    event_ticket.cod_cobro = cobros.codigo;
    event_ticket.bultos = cobros.id;
	if ( cobros.ctacte == 1 || cobros.ctacte == 'S' ) {
		event_ticket.nro_cliente = strtol( datos_cobro->cuenta, NULL, 10 ); 
	}
    event_ticket.nro_comprobante_new = datos_cobro->comprobante;
	event_ticket.importe_sin_iva = ( anulacion ) ? -datos_cobro->importe 
		: datos_cobro->importe;
    event_ticket.tipo_evento = ( anulacion ) ? 50 : 1;
    event_ticket.nro_vendedorl = NRO_VENDEDOR_LONG;
    event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;
    if( cobros.tarjeta ) {
        SET_MEMORY( __ram_string_cliente_afinidad, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
    }
    SETEAR_TIEMPO( _INDEFINIDO );
    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
	if( !RAM_FLAG_COBRO_VENTA )
	    GRABAR_EVENT_TICKET( NO );
    ACUMULAR_TIEMPOS( );
    CEREAR_TIEMPOS( );
    SETEAR_TIEMPO( _ENTRADA_OCIO );
	if( !RAM_FLAG_COBRO_VENTA )
    GRABA_EVENTOS_PAGO( 0, anulacion );
    cod_barra = LEER_COD_BARRA_EN_BINARIO( 0 );
    if( !cobros.tarjeta ) {
		if( cod_barra != NULL ) {
			//int area = SELECTED();
			int tabla_ant = SELECTED2();
			int tipo_ant = SELECTED_TIPO();
			//SELECT( AREA_COD_BARRA );
			SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
			memset( &event_cod_barra, 0, sizeof( event_cod_barra ) );
			event_cod_barra.caja = NRO_CAJA;
			event_cod_barra.nro_z = NRO_Z;
			event_cod_barra.id_evento = RAM_ID_EVENTO;
			event_cod_barra.posicion_medio = ( RAM_FLAG_COBRO_VENTA ) ? RAM_P_TICKET : 0;
			event_cod_barra.posicion_cod_barra = 1;
			event_cod_barra.cuenta = cobros.codigo;
			event_cod_barra.importe = ( anulacion ) ? -datos_cobro->importe : 
				datos_cobro->importe;
			event_cod_barra.nro_comprobante_new = datos_cobro->comprobante;
			{//Controlar el tamaño del campo nro_comprobante_new
				char nro[12];//No debe superar los 10 digitos porque es lo maximo que el campo del servidor acepta
				memset(nro,0,sizeof(nro));
				_snprintf( nro, sizeof(nro)-2,"%lf",event_ticket.nro_comprobante_new );
				event_ticket.nro_comprobante_new = atof(nro);
			}
			strncpy( event_cod_barra.nro_cuenta_ext, datos_cobro->cuenta,
					sizeof( event_cod_barra.nro_cuenta_ext ) );
			event_cod_barra.cod_sucursal = config.sucursal;
			event_cod_barra.fecha_ticket = NRO_FECHA_OPERACION;/* ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS()*/;
			event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
			memcpy( event_cod_barra.codigo_barra, cod_barra,
					sizeof( event_cod_barra.codigo_barra ) );
			#ifdef INVEL_L
			//BEGIN_TRANSACTION();
			#endif
			//INSERT();
			INSERT2(NO);
			#ifdef INVEL_L
			//END_TRANSACTION();
			#endif
			SELECT_TABLE( tabla_ant, tipo_ant );
			//SELECT( area );
		}
	} else {
		//int area = SELECTED();
		int tabla_ant = SELECTED2();
		int tipo_ant = SELECTED_TIPO();
		//SELECT( AREA_COD_BARRA );
		SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
		memset( &event_cod_barra, 0, sizeof( event_cod_barra ) );
		event_cod_barra.caja = NRO_CAJA;
		event_cod_barra.nro_z = NRO_Z;
		event_cod_barra.id_evento = RAM_ID_EVENTO;
		event_cod_barra.cuenta = VAR_TARJETAS_TARJETA + 1;
		memcpy( event_cod_barra.codigo_barra, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/,
			sizeof( event_cod_barra.codigo_barra ) );
		event_cod_barra.importe = datos_cobro->importe;
		event_cod_barra.cod_sucursal = config.sucursal;
		event_cod_barra.fecha_ticket = NRO_FECHA_OPERACION/*( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS()*/;
		event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
		event_cod_barra.posicion_medio = ( RAM_FLAG_COBRO_VENTA ) ? RAM_P_TICKET : 0;
		#ifdef INVEL_L
		//BEGIN_TRANSACTION();
		#endif
		INSERT2(NO);
		//INSERT();
		#ifdef INVEL_L
		//END_TRANSACTION();
		#endif
		//SELECT( area );
		SELECT_TABLE( tabla_ant, tipo_ant );
	}
    ACTUALIZA_ID_EVENTO_NEW();
}

/**************************************************************************/
void TICKET_COBRANZA( char *concepto, struct _datos_cobro *datos_cobro, char *cta_cmr,
                      int anulacion )
/**************************************************************************/
{
    int comas = 0;
    int doc_no_fiscal = NO;
    if( config.pais == COLOMBIA ) {
        comas = 1;
    }
	

	if( IMPRESORA_FISCAL == FISCAL )
		doc_no_fiscal = SI;
    
    IMPRIME_CABECERA_COMPROBANTE( ( anulacion ) ? ST( S_ANUL_COBRANZA ) : ST( S_COBRANZA ),
                                  ( int )IMPRIMIR_COBROS, 0, doc_no_fiscal );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CONCEPTO_____ ) );
    MOSTRAR_CADENA( 15, PRN, concepto );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_COMPROBANTE__ ) );
    MOSTRAR_ENTERO( 16, PRN, "9999999999", datos_cobro->comprobante );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CUENTA_______ ) );
    if( cta_cmr ) {
        MOSTRAR_CADENA( 15, PRN, cta_cmr );
    }
    else {
        MOSTRAR_CADENA( 15, PRN, datos_cobro->cuenta );
    }
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE______ ) );
    MOSTRAR_FLOTANTE( 17, PRN, ENTEROS, DECIMALES, 0, comas, datos_cobro->importe );
    LF( 1 );
    setData( ACTION_PRINT, NULL, 0 );
	if( IMPRESORA_FISCAL == FISCAL )
		CERRAR_DOCUMENTO_NO_FISCAL();

}
/*****************************************************************************/
int VALIDAR_NRO_COMPROBANTE( double _comprobante )
/*****************************************************************************/
{
    int rta = 1;
    switch( cobros.valid_comprobante ) {
        case 1:
            /*---------- Valida numeros de LIBERTAD --------------*/
            if( _comprobante != 9430 && _comprobante != 9420 ) {
                rta = 0;
            }
            break;
    }
    if( !rta ) {
        MENSAJE_STRING( S_COMPROBANTE_INVALIDO );
    }
    return ( rta );
}
/*****************************************************************************/
int VALIDAR_CUENTA( char *_cuenta, char *cta_cmr )
/*****************************************************************************/
{
    int rta = 1;
    //char cadena[41];
    /*if( cta_cmr ) {
        strcpy( cadena, cta_cmr );
    }
    else {
        strcpy( cadena, _cuenta );
    }
    rta = EJECUTAR_L_VALID( cadena, cobros.nro_funcion_valid );*/
    /*---------------- Verifica cliente si es ctacte --------------*/
    if( cobros.ctacte == 1 || cobros.ctacte == 'S' ) {
        rta = EXISTE_CLIENTE( atol( _cuenta ) );
    }
    if( !rta ) {
        MENSAJE_STRING( S_NRO_CTA_INVALIDO );
    }
    return ( rta );
}
//#define COMPILAR_CMR
#ifdef COMPILAR_CMR
/*****************************************************************************/
int COBRO_CMR()
/*****************************************************************************/
{
    int pant,nro;
    int _ptr,_handle,_estado,_cta_cmr,_importe;
    double minimo_pago;
    char mensaje[60];
    if( VENDEDOR_OBLIGATORIO && !NRO_VENDEDOR_LONG ) {
        MENSAJE_STRING( S_FAVOR_INGRESE_VENDEDOR );
        return ( 0 );
    }
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    _cta_cmr = STACK_INS( 11 );
    _importe = STACK_INS( sizeof( double ) );
    /*---------- prepara pantalla ----------*/

    _ATRIBUTO = 120;
    if( _ESTADO == 0 ) {
        if( SOLICITAR_CLAVES( 30, 21, SI, SI, PEDIR_CLAVE_EN_COBROS, ST( S__COBRO_CMR_ ), NULL,
                              RAM_COD_FUNCION, SI ) ) {
            SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
            /*------------- Solo carga en memoria el primer registro ---------*/
            if( /*USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )*/
             OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros )) == 0 ) {
                _SET_ESTADO( 1 );
                CLOSE_TABLE( T_COBROS, TT_ORIG );
            }
        }
    }
    SET_MEMORY_INT( __permite_repactar_tarjeta_cmr, 0 );
    if( _ESTADO == 1 ) {
        if( PEDIR_DATOS_COBRO_CMR( _cta_cmr, _importe ) ) {
            minimo_pago = ( PORCENTAJE_MINIMO_DE_PAGO_CMR / 100 ) * VAR_TARJETAS_IMPORTE_CUOTA;
            if( PERMITE_REPACTAR_TARJETA_CMR && ( _IMPORTE < VAR_TARJETAS_IMPORTE_CUOTA )
             && ( _IMPORTE > minimo_pago ) ) {
                nro = ELEGIR_REPACTACION();
                SET_MEMORY_INT( __nro_cod_cuota, nro );
            }
            _SET_ESTADO( 2 );
        }
    }
    /*----------------- Imprime el ticket e ingresa el pago -----------------*/
    if( _ESTADO == 2 ) {
        TICKET_COBRANZA( cobros.nombre, &datos_cobro, _CTA_CMR, NO );
        SET_MEMORY_DOUBLE( __ram_total, _IMPORTE );
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        if( PAGOS( _MODO_COBRANZA, NULL, NO ) ) {
            _SET_ESTADO( 3 );
        }
        else if( IMPRIMIR_COBROS ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_COBRO_ANULADO ) );
            SET_MEMORY_DOUBLE( __ram_total, 0 );
            IMPRIME_PIE_COMPROBANTE( IMPRIMIR_COBROS, NO );
            GRABA_EVENTO_ANULADO();
        }
    }
    /*------------------------- Envia el paquete on-line --------------------*/
    if( _ESTADO == 3 ) {
        SET_MEMORY_CHAR( __var_tarjetas_tarjeta, _CMR );
        SET_NUMERO_TARJETA_PRIVADO(_CTA_CMR);//SET_MEMORY( __var_tarjetas_numero, _CTA_CMR );
        ON_LINE_PAGO_TARJETA( _IMPORTE, NO, 0 );
        if( PERMITE_REPACTAR_TARJETA_CMR ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_COBRO_REPACTADO ) );
            LF( 1 );
            sprintf( mensaje, ST( S__D_CUOTAS_DE__7_2F__ ), NRO_COD_CUOTA,
                     VAR_TARJETAS_IMPORTE_CUOTA );
            MOSTRAR_CADENA( 5, PRN, mensaje );
        }
        /*------------------------*/
        _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        #ifdef COMPILAR_ASC
        GENERA_COBROS_ASC( cobros.id, &datos_cobro );
        #endif
        _SET_ESTADO( 5 );
    }
    if( _ESTADO == 5 ) {
        GRABAR_EVENTO_COBRO( &datos_cobro, _CTA_CMR, NO );
        CERRAR_ARCHIVOS_EVENTOS();
        ABRIR_ARCHIVOS_EVENTOS();
        _SET_ESTADO( 6 );
    }
    if( _ESTADO == 6 ) {
        VALIDAR_COBROS( cobros.cod_valida, &datos_cobro, _CTA_CMR, NO, _MODO_COBRANZA );
        _SET_ESTADO( 7 );
    }
    if( _ESTADO == 7 ) {
        GRABAR_LOG_SUPERVISOR( ST( S_COBRO_CMR ), USUARIO_CLAVE_ANTERIOR() );
        GRABAR_XYZ( 'X' );
    }
    /*--------------- fin ----------------*/
    CERRAR_CAJON();
    FIN_DE_TICKET( NO );
    SET_MEMORY_DOUBLE( __ram_no_reprocesar, 1 );
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
    return 1;
}
/*****************************************************************************/
int PEDIR_DATOS_COBRO_CMR( int _cta, int _importe )
/*****************************************************************************/
{
    int pant, rta = 0;
    char cta[15];
    double importe;

    do {
        rta = PEDIR_CTA_CMR( cta, CONSULTA_CUENTA_ON_LINE_COBROS, VALIDAR_CUENTA );
    }
    while( rta == -1 );
    if( rta == 1 ) {
        rta = 0;
        importe = PEDIR_FLOAT_EN_RECUADRO( ST( S__IMPORTE_A_PAGAR_ ), NO, ENTEROS, DECIMALES );
        if( importe > 0.009 ) {
            SET_MEMORY( _cta, cta );
            SET_MEMORY_DOUBLE( _importe, importe );
            rta = 1;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int PEDIR_CTA_CMR( char *cta, void f_online( char *cta ), int f_valid( int cod, char *cta ) )
/*****************************************************************************/
{
    char buffer2[50],buffer1[80];
    int rta = 0;
    _PERMITIR_SCANNER = SI;
    _PERMITIR_BANDA = SI;
    rta = PEDIR_CADENA_EN_RECUADRO( ST( S__NUMERO_DE_CUENTA_ ), NO, 13, cta, NULL );
    if( rta ) {
        if( rta == -2 ) {
            if( LEER_TARJETA_BANDA( buffer2, buffer1 ) ) {
                memcpy( cta, &buffer2[6], 10 );
                cta[10] = 0;
            }
        }
        TRIM( cta );
        if( strlen( cta ) == 13 ) {
            memmove( &cta[2], cta, 10 );
            cta[10] = 0;
        }
        if( f_valid( 0, cta ) ) {
            // CONSULTA_CUENTA_ON_LINE( cta );
            f_online( cta );
            rta = 1;
            if( RAM_RESULTADO_TRANSAC == 1 ) {
                rta = -1;
                MENSAJE_STRING( S_CUENTA_DENEGADA );
            }
        }
        else {
            rta = -1;
        }
    }
    _PERMITIR_SCANNER = NO;
    _PERMITIR_BANDA = NO;
    return ( rta );
}
/*****************************************************************************/
void CONSULTA_CUENTA_ON_LINE( char *cta )
/*****************************************************************************/
{
    CEREAR_STRUCT( _var_tarjetas );
    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, _CMR );
    SET_NUMERO_TARJETA_PRIVADO(cta);//SET_MEMORY( __var_tarjetas_numero, cta );
    _CONSULTA_ESTADO_TARJETA();
    CEREAR_STRUCT( _var_tarjetas );
}
/*****************************************************************************/
void CONSULTA_CUENTA_ON_LINE_COBROS( char *cta )
/*****************************************************************************/
{
    CEREAR_STRUCT( _var_tarjetas );
    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, _CMR );
    SET_NUMERO_TARJETA_PRIVADO(cta);//SET_MEMORY( __var_tarjetas_numero, cta );
    _CONSULTA_ESTADO_TARJETA_COBRO();
    CEREAR_STRUCT( _var_tarjetas );
}
#endif
#ifdef COMPILAR_REVALIDAR_COBROS
/*****************************************************************************/
//void REVALIDAR_COBROS()
/*****************************************************************************/
/*{
    if( SOLICITAR_CLAVES( 27 - ancho_panel, 21, NO, SI, PEDIR_SUPERVISOR_REIMP_VALIDACIO,
                          ST( S_REIMPRESION_VALIDACION_COBRO ), NULL, RAM_COD_FUNCION, SI ) ) {
        SELECT( AREA_TICKET );
        SET_ORDER( 1 );
        GO( BOTTOM );
        if( event_ticket.tipo_evento != 1 ) {
            MENSAJE_STRING( S_ULTIMA_OPERACION_NO_FUE_COBRO );
        }
        else {
            if( USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )
             == 0 ) {
                // busca cobros.id  que esta en bultos
                SET_ORDER( 1 );
                GET_EQUAL( ( char* )&event_ticket.nro_ticket );
                if( FOUND() ) {
                    VALIDAR_COBROS( cobros.cod_valida, event_ticket.nro_comprobante,
                                    event_ticket.nro_cliente, event_ticket.importe_sin_iva,
                                    cobros.nombre );
                }
                else {
                    MENSAJE_STRING( S_IDENTIF_COBRO_NO_ENCONTRADO );
                }
                CLOSE_DATABASE( AREA_AUX4 );
            }
        }
    }
}*/
#endif
/*****************************************************************************/
void COBROS_VARIOS()
/*****************************************************************************/
{
}
/*{
    char nro_cuenta[21];
    int codigo;
    int pant,j;
    double tot_imp = 0;
    int _ptr, _handle, _estado, seguir, *_comprobante = NULL, *_cuenta = NULL, *_importe = NULL,
                                                      *_codigo = NULL;
    int _cantidad;
    int _reprocesar;
    int cantidad;
    //--------------------- Prepara recupero de memoria ----------------------
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    //  _cantidad = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    _cantidad = STACK_INS( sizeof( short ) );
    _reprocesar = STACK_INS( sizeof( char ) );
    SET_MEMORY_INT( __ram_hora_inicio_evento, GET_HORA_DOS() );   
    if( !_comprobante ) {
        _comprobante = ( int* )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO + 1, sizeof( int ) );
    }
    if( !_cuenta ) {
        _cuenta = ( int* )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO + 1, sizeof( int ) );
    }
    if( !_importe ) {
        _importe = ( int* )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO + 1, sizeof( int ) );
    }
    if( !_codigo ) {
        _codigo = ( int* )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO + 1, sizeof( int ) );
    }
    for( cantidad = 0;cantidad <= CANTIDAD_DE_SERVICIOS_POR_COBRO ;cantidad++ ) {
        //      _codigo[cantidad] = STACK_INS( sizeof( int ) );
        _codigo[cantidad] = STACK_INS( sizeof( short ) );
        _comprobante[cantidad] = STACK_INS( sizeof( long ) );
        _cuenta[cantidad] = STACK_INS( sizeof( long ) );
        _importe[cantidad] = STACK_INS( sizeof( double ) );
    }
    //---------- prepara pantalla ----------

    if( _ESTADO == 0 ) {
        if( SOLICITAR_CLAVES( 30 - ancho_panel, 21, SI, SI, PEDIR_CLAVE_EN_COBROS, ST( S_COBROS ),
                              NULL, RAM_COD_FUNCION, SI ) ) {
            INICIALIZAR_BINARIO_COD_BARRA();
            SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
            if( USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )
             == 0 ) {
                PANTALLA_COBROS();
                _ATRIBUTO = 112;
                //---------------------- Dibuja la pantalla ----------------------------
                ExecuteSAction( A_MUESTRA_TIT_COMPROB_COBRO, ST( S_Comprobante ) );
                ExecuteSAction( A_MUESTRA_TIT_CUENTA_COBRO, ST( S_N_Cuenta ) );
                ExecuteSAction( A_MUESTRA_TIT_IMPORTE_COBRO, ST( S_Importe ) );
                ExecuteSAction( A_MUESTRA_TIT_TOTAL_COBRO, "TOTAL: " );
                //-----------ingresa cobros ------------
                SET_MEMORY_INT( _cantidad, 0 );
                cantidad = _CANTIDAD;
                seguir = 1;
                while( seguir ) {
                    if( cantidad < CANTIDAD_DE_SERVICIOS_POR_COBRO ) {
                        seguir = PIDE_COBRO( _comprobante[cantidad], _cuenta[cantidad],
                                             _importe[cantidad], _codigo[cantidad], cantidad );
                        if( seguir == 1 ) {
                            _SET_ESTADO( 1 );
                            tot_imp += GET_MEMORY_DOUBLE( _importe[cantidad] );
                            ADD_MEMORY_INT( _cantidad, 1 );
                            cantidad = _CANTIDAD;
                            SELECT( AREA_AUX4 );
                            PANTALLA_COBROS_VARIOS( cantidad, tot_imp );
                        }
                    }
                    else {
                        MENSAJE_STRING( S_NO_MAS_SERVICIOS );
                        SELECT( AREA_AUX4 );
                        PANTALLA_COBROS_VARIOS( cantidad, tot_imp );
                        BORRAR_MENSAJE();
                        seguir = 0;
                    }
                    //*****viejo*******
                    //seguir =
                    //        PIDE_COBRO( _comprobante[cantidad], _cuenta[cantidad],
                    //        _importe[cantidad], _codigo[cantidad], cantidad );
                    //   if( seguir == 1 ) {
                    //        if( cantidad < CANTIDAD_DE_SERVICIOS_POR_COBRO ) {
                    //            _SET_ESTADO( 1 );
                    //            tot_imp += GET_MEMORY_DOUBLE( _importe[cantidad] );
                    //            ADD_MEMORY_INT( _cantidad, 1 );
                    //            cantidad = _CANTIDAD;
                    //            SELECT( AREA_AUX4 );
                    //            PANTALLA_COBROS_VARIOS( cantidad, tot_imp );
                    //        } else {
                    //      MENSAJE_STRING( S_NO_MAS_SERVICIOS );
                    //      SELECT( AREA_AUX4 );
                    //            PANTALLA_COBROS_VARIOS( cantidad, tot_imp );
                    //            BORRAR_MENSAJE(  );
                    //        }
                    //    }
                    //**************
                    SET_MEMORY_DOUBLE( __ram_total, tot_imp );
                }
                //---------------------- borra la pantalla ----------------------------
                ExecuteSAction( A_MUESTRA_TIT_COMPROB_COBRO, " " );
                ExecuteSAction( A_MUESTRA_TIT_CUENTA_COBRO, " " );
                ExecuteSAction( A_MUESTRA_TIT_IMPORTE_COBRO, " " );
                ExecuteSAction( A_MUESTRA_TIT_TOTAL_COBRO, " " );
                if( LASTKEY() == 27 ) {
                    CLOSE_DATABASE( AREA_AUX4 );
                    _SET_ESTADO( 0 );
                }
                #if defined(INVEL_W) || defined(INVEL_L)
                if( _ESTADO == 0 ) {
                    if( label_on ) {
                        ExecuteSAction( A_MUESTRA_TIT_CANTIDAD_COBRO, " " );
                        label_on = 0;
                    }
                    ExecuteSAction( A_MUESTRA_CANTIDAD_COBRO, " " );
                    ExecuteSAction( A_MUESTRA_TOTAL_COBRO, " " );
                    ExecuteAction( A_DES_COBROS_ACT_VENTAS );
                    ExecuteSAction( A_MUESTRA_TIT_TOTAL_COBRO, " " );
                }
                #endif               
            }
        }
    }
    if( _ESTADO == 1 ) {
        #if defined(INVEL_W) || defined(INVEL_L)
        if( label_on ) {
            ExecuteSAction( A_MUESTRA_TIT_CANTIDAD_COBRO, " " );
            ExecuteSAction( A_MUESTRA_TIT_TOTAL_COBRO, " " );
            label_on = 0;
        }
        ExecuteSAction( A_MUESTRA_CANTIDAD_COBRO, " " );
        ExecuteSAction( A_MUESTRA_TOTAL_COBRO, " " );
        #endif               
        if( !USED( AREA_AUX4 ) ) {
            USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
            tot_imp = GET_MEMORY_DOUBLE( __ram_total );
        }
        SELECT( AREA_AUX4 );
        for( j = 0;j < _CANTIDAD;j++ ) {
            codigo = GET_MEMORY_INT( _codigo[j] );
            GET_EQUAL( ( char* )&codigo );
            if( cobros.tarjeta ) {
                strcpy( nro_cuenta, VAR_TARJETAS_NUMERO );
            }
            else {
                nro_cuenta[0] = 0;
            }
            //TICKET_COBRANZA_VARIOS( cobros.nombre, GET_MEMORY_LONG( _comprobante[j] ),
            //  GET_MEMORY_LONG( _cuenta[j] ), GET_MEMORY_DOUBLE( _importe[j] ), NULL, NO );
            TICKET_COBRANZA_VARIOS( cobros.nombre, GET_MEMORY_LONG( _comprobante[j] ),
                                    GET_MEMORY_LONG( _cuenta[j] ), GET_MEMORY_DOUBLE( _importe[j] ),
                                    NULL, ( char )( j == 0 ? SI : NO ) );
        }
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        if( PAGOS( _MODO_COBRANZA, NULL ) ) {
            ADD_MEMORY_INT( __x_cobros_cantidad_servicios, _CANTIDAD );
            _SET_ESTADO( 2 );
            //----------------- Actualiza saldo en ctacte ---------------------
            for( j = 0;j < _CANTIDAD;j++ ) {
                SELECT( AREA_AUX4 );
                codigo = GET_MEMORY_INT( _codigo[j] );
                GET_EQUAL( ( char* )&codigo );
                if( ( cobros.ctacte == 1 || cobros.ctacte == 'S' ) && UTILIZA_SOCIOS ) {
                    ACTUALIZA_SALDO_CLIENTE( -( GET_MEMORY_DOUBLE( _importe[j] ) ) );
                    ACTUALIZA_SOCIOS( -GET_MEMORY_DOUBLE( _importe[j] ) );
                }
                if( cobros.tarjeta ) {
                    ON_LINE_PAGO_TARJETA( GET_MEMORY_DOUBLE( _importe[j] ), NO, 0 );
                }
                if( cobros.validacion_pie ) {
                    VALIDAR( cobros.validacion_pie, _VALCOBROS, 0, 0 );
                }
            }
        }
        else if( IMPRIMIR_COBROS ) {
            MOSTRAR_CADENA( 1, PRN, ST( S_COBRO_ANULADO ) );
            CLOSE_DATABASE( AREA_AUX4 );
            SET_MEMORY_DOUBLE( __ram_total, 0 );
        }
        IMPRIME_PIE_COMPROBANTE( IMPRIMIR_COBROS, NO );
    }
    if( _ESTADO == 2 ) {
        for( j = 0;j < _CANTIDAD;j++ ) {
            SELECT( AREA_AUX4 );
            codigo = GET_MEMORY_INT( _codigo[j] );
            GET_EQUAL( ( char* )&codigo );
            VALIDAR_COBROS( cobros.cod_valida, GET_MEMORY_LONG( _comprobante[j] ),
                            GET_MEMORY_LONG( _cuenta[j] ), GET_MEMORY_DOUBLE( _importe[j] ),
                            cobros.nombre );
        }
        _SET_ESTADO( 3 );
    }
    if( _ESTADO == 3 ) {
        #ifdef COMPILAR_ASC
        for( j = 0;j < _CANTIDAD;j++ ) {
            SELECT( AREA_AUX4 );
            codigo = GET_MEMORY_INT( _codigo[j] );
            GET_EQUAL( &codigo );
            GENERA_COBROS_ASC( cobros.id, GET_MEMORY_LONG( _comprobante[j] ),
                               GET_MEMORY_LONG( _cuenta[j] ), GET_MEMORY_DOUBLE( _importe[j] ) );
        }
        #endif
        _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        int area = SELECTED();
        
        for( j = 0;j < _CANTIDAD;j++ ) {
            SELECT( AREA_AUX4 );
            codigo = GET_MEMORY_INT( _codigo[j] );
            GET_EQUAL( ( char* )&codigo );
            GRABAR_EVENTO_COBROS_VARIOS( cobros.codigo, cobros.id,
                                         GET_MEMORY_LONG( _comprobante[j] ),
                                         GET_MEMORY_LONG( _cuenta[j] ),
                                         GET_MEMORY_DOUBLE( _importe[j] ), NULL );
        }
        _GRABAR_EVENTO_COBROS_TOTAL( _CANTIDAD, tot_imp, _importe, _comprobante, _cuenta, _codigo );
        SET_MEMORY_CHAR( __ram_no_reprocesar, 0 );  // New
        CLOSE_DATABASE( AREA_AUX4 );
        SELECT( area );
        _SET_ESTADO( 5 );
    }
    if( _ESTADO == 5 ) {
        GRABAR_LOG_SUPERVISOR( ST( S_COBROS ), USUARIO_CLAVE_ANTERIOR() );
        GRABAR_XYZ( 'X' );
    }
    SET_MEMORY_CHAR( _reprocesar, RAM_NO_REPROCESAR );
    FIN_DE_TICKET( SI );
    SET_MEMORY_CHAR( __ram_no_reprocesar, GET_MEMORY_CHAR( _reprocesar ) );
    CERRAR_CAJON();
    //--------------------- Fin de recupero de memoria ----------------------
    CLOSE_RECUPERO( _ptr, _handle );
    if( ( char* )_comprobante != NULL ) {
        FREEMEM( ( char* )_comprobante );
    }
    if( ( char* )_cuenta != NULL ) {
        FREEMEM( ( char* )_cuenta );
    }
    if( ( char* )_importe != NULL ) {
        FREEMEM( ( char* )_importe );
    }
    if( ( char* )_codigo != NULL ) {
        FREEMEM( ( char* )_codigo );
    }
    ENABLE_STANDBY_AUTOMATICO();
}*/
/**********************************************************************/
//void PANTALLA_COBROS_VARIOS( int i, double importe )
/**********************************************************************/
/*{
    #if defined(INVEL_W) || defined(INVEL_L)
    char cad[10];
    if( !label_on ) {
        ExecuteSAction( A_MUESTRA_TIT_CANTIDAD_COBRO, "CANTIDAD: " );
        ExecuteSAction( A_MUESTRA_TIT_TOTAL_COBRO, "TOTAL: " );
        label_on = 1;
    }
    _itoa( i, cad, 10 );
    ExecuteSAction( A_MUESTRA_CANTIDAD_COBRO, cad );
    FTOA( importe, cad, ENTEROS, DECIMALES, 0, 0 );
    ExecuteSAction( A_MUESTRA_TOTAL_COBRO, cad );
    #else
    _ATRIBUTO = 103;
    BOX( 5, 20, 31, 23 );
    MOSTRAR_CADENA( 6, 21, ST( S_CANTIDAD__ ) );
    MOSTRAR_ENTERO( 16, 21, "9", i );
    MOSTRAR_CADENA( 6, 22, ST( S_TOTAL__ ) );
    MOSTRAR_FLOTANTE( 16, 22, ENTEROS, DECIMALES, 0, 0, importe );
    #endif
}*/
/**************************************************************************/
/*void TICKET_COBRANZA_VARIOS( char *concepto, long comprobante, long cuenta, double importe,
                             char *cta_cmr, char cabecera )*/
/**************************************************************************/
/*{
    int comas = 0;
    if( IMPRIMIR_COBROS ) {
        if( config.pais == COLOMBIA ) {
            comas = 1;
        }
        if( cabecera ) {
            IMPRIME_CABECERA_COMPROBANTE( ST( S_COBRANZA ), IMPRIMIR_COBROS, 0 );
            LF( 1 );
            if( PEDIR_CLAVE_EN_COBROS ) {
                IMPRIMIR_NOMBRE_USUARIO( USUARIO_CLAVE_ANTERIOR() );
            }
            IMPRIMIR_NOMBRE_USUARIO( NRO_CAJERO );
            LF( 1 );
        }
        MOSTRAR_CADENA( 1, PRN, ST( S_CONCEPTO_____ ) );
        MOSTRAR_CADENA( 15, PRN, concepto );
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_COMPROBANTE__ ) );
        MOSTRAR_ENTERO( 15, PRN, "999999999", comprobante );
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_CUENTA_______ ) );
        if( cta_cmr ) {
            MOSTRAR_CADENA( 15, PRN, cta_cmr );
        }
        else {
            MOSTRAR_ENTERO( 15, PRN, "999999999", cuenta );
        }
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, ST( S_IMPORTE______ ) );
        MOSTRAR_FLOTANTE( 17, PRN, ENTEROS, DECIMALES, 0, comas, importe );
        LF( 1 );
    }
}*/
/**************************************************************************************/
/*void GRABAR_EVENTO_COBROS_VARIOS( int codigo, int id, long comprobante, long cuenta, double importe,
                                  char *cta_cmr )*/
/**************************************************************************************/
/*{
    char _cta_cmr[11];
    struct varios *pvarios = NULL;
    int area;
    area = SELECTED();
    SELECT( AREA_E_TICKET );
    memset( &event_e_ticket, 0, sizeof( event_e_ticket ) );
    event_e_ticket.cod_articulo = ( long )id;
    event_e_ticket.id_evento = RAM_ID_EVENTO;
    event_e_ticket.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    event_e_ticket.importe_sin_iva = importe;
    event_e_ticket.cod_clasificacion = comprobante;
    event_e_ticket.nro_z = NRO_Z;
    event_e_ticket.caja = NRO_CAJA;
    event_e_ticket.cod_depto = codigo;
    event_e_ticket.fecha_ticket = _GET_FECHA_DOS();
    event_e_ticket.cod_sucursal = config.sucursal;
    if( cobros.tarjeta ) {
        strncpy( event_e_ticket.cod_barra, VAR_TARJETAS_NUMERO,
                 sizeof( event_e_ticket.cod_barra ) - 1 );
    }
    #ifdef INVEL_L
    BEGIN_TRANSACTION();
    #endif
    INSERT();
    #ifdef INVEL_L
    END_TRANSACTION();
    #endif
    SELECT( area );
    if( !pvarios ) {
        pvarios = ( struct varios * )MALLOC( sizeof( struct varios ) );
    }
    if( cta_cmr ) {
        strcpy( _cta_cmr, cta_cmr );
    }
    pvarios->codigo = codigo;
    pvarios->id_cobro = ( long )id;
    pvarios->comprobante = comprobante;
    pvarios->cuenta = cuenta;
    pvarios->importe = importe;
    GRABAR_EVENT_VARIOS( ( char* )pvarios, sizeof( struct varios ), NO );
    if( cta_cmr ) {
        GRABAR_EVENT_VARIOS( _cta_cmr, strlen( _cta_cmr ), NO );
    }
    FREEMEM( (char *)pvarios );
}*/
/*****************************************************************************/
/*void _GRABAR_EVENTO_COBROS_TOTAL( int cantidad, double importe, int *_importe, int *_comprobante,
                                  int *_cuenta, int *_codigo )*/
/*****************************************************************************/
/*{
    int area,j;
    
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.bultos = cantidad;
    event_ticket.importe_sin_iva = importe;
    event_ticket.tipo_evento = 1;   //cobros
    event_ticket.nro_vendedorl = NRO_VENDEDOR_LONG;
    event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;
    GRABAR_EVENT_TICKET( NO );
    GRABA_EVENTOS_PAGO( 0 );
    area = SELECTED();
    SELECT( AREA_COD_BARRA );
    for( j = 0;j < cantidad;j++ ) {
        char *cod_barra;
        cod_barra = LEER_COD_BARRA_EN_BINARIO( j );
        memset( &event_cod_barra, 0, sizeof( event_cod_barra ) );
        event_cod_barra.caja = NRO_CAJA;
        event_cod_barra.nro_z = NRO_Z;
        event_cod_barra.id_evento = RAM_ID_EVENTO;
        event_cod_barra.posicion_medio = ( char )128;
        event_cod_barra.posicion_cod_barra = j + 1;
        event_cod_barra.cuenta = GET_MEMORY_INT( _codigo[j] );
        event_cod_barra.importe = GET_MEMORY_DOUBLE( _importe[j] );
        event_cod_barra.nro_comprobante = GET_MEMORY_LONG( _comprobante[j] );
        event_cod_barra.nro_cuenta_comprob = GET_MEMORY_LONG( _cuenta[j] );
        if( cod_barra != NULL ) {
            memcpy( event_cod_barra.codigo_barra, cod_barra,
                    sizeof( event_cod_barra.codigo_barra ) );
        }
        else {
            memset( event_cod_barra.codigo_barra, 0, sizeof( event_cod_barra.codigo_barra ) );
        }
        event_cod_barra.cod_sucursal = config.sucursal;
        event_cod_barra.fecha_ticket = _GET_FECHA_DOS();
        event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
        #ifdef INVEL_L
        BEGIN_TRANSACTION();
        #endif
        INSERT();
        #ifdef INVEL_L
        END_TRANSACTION();
        #endif
    }
    ACTUALIZA_ID_EVENTO_NEW();
    SELECT( area );

}*/
#ifdef COMPILAR_REVALIDAR_COBROS
/*****************************************************************************/
//void REVALIDAR_COBROS_VARIOS()
/*****************************************************************************/
/*{
    if( SOLICITAR_CLAVES( 27 - ancho_panel, 21, NO, SI, PEDIR_SUPERVISOR_REIMP_VALIDACIO,
                          ST( S_REIMPRESION_VALIDACION_COBRO ), NULL, RAM_COD_FUNCION, SI ) ) {
        SELECT( AREA_TICKET );
        SET_ORDER( 1 );
        GO( BOTTOM );
        if( event_ticket.tipo_evento != 1 ) {
            MENSAJE_STRING( S_ULTIMA_OPERACION_NO_FUE_COBRO );
        }
        else {
            if( SELECCIONA_COBROS_EN_REVALIDACIO ) {
                int codigo;
                double importe;
                while( MENU_SELECCIONA_COBRO( &codigo, &importe ) != -1 ) {
                    SELECT( AREA_VARIOS );
                    GO( BOTTOM );
                    SKIP( ( -1 )*( event_ticket.bultos - 1 ) );
                    while( !BtrvEOF()
                        && ( ( ( struct varios * )event_varios->buffer )->codigo != codigo
                          || ( ( struct varios * )event_varios->buffer )->importe != importe ) ) {
                        SKIP( 1 );
                    }
                    if( ( ( struct varios * )event_varios->buffer )->codigo == codigo ) {
                        if( USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ),
                                    NULL, 0, 0 ) == 0 ) {
                            // busca cobros.id  que esta en bultos
                            SET_ORDER( 1 );
                            GET_EQUAL( ( char* )&codigo );
                            if( FOUND() ) {
                                VALIDAR_COBROS( cobros.cod_valida,
                                                (
                                ( struct varios * )event_varios->buffer )->comprobante,
                                                ( ( struct varios * )event_varios->buffer )->cuenta,
                                                (
                                ( struct varios * )event_varios->buffer )->importe, cobros.nombre );
                            }
                            else {
                                MENSAJE_STRING( S_IDENTIF_COBRO_NO_ENCONTRADO );
                            }
                            CLOSE_DATABASE( AREA_AUX4 );
                        }
                        else {
                            MENSAJE_STRING( S_ERROR_AL_OBTENER_TABLA_EN_DBROUTER );
                        }
                    }
                    else {
                        MENSAJE_STRING( S_IDENTIF_COBROS_VARIOS_NO_ENC );
                    }
                }
            }
            else {
                SELECT( AREA_VARIOS );
                GO( BOTTOM );
                SKIP( ( -1 )*( event_ticket.bultos - 1 ) );
                while( !BtrvEOF() ) {
                    if( USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0,
                                0 ) == 0 ) {
                        // busca cobros.id  que esta en bultos
                        SET_ORDER( 1 );
                        GET_EQUAL( ( char* )&( ( struct varios * )event_varios->buffer )->
                            codigo );
                        if( FOUND() ) {
                            VALIDAR_COBROS( cobros.cod_valida,
                                            (
                            ( struct varios * )event_varios->buffer )->comprobante,
                                            ( ( struct varios * )event_varios->buffer )->cuenta,
                                            ( ( struct varios * )event_varios->buffer )->importe,
                                            cobros.nombre );
                        }
                        else {
                            MENSAJE_STRING( S_IDENTIF_COBRO_NO_ENCONTRADO );
                        }
                        CLOSE_DATABASE( AREA_AUX4 );
                    }
                    SELECT( AREA_VARIOS );
                    SKIP( 1 );
                }
            }
        }
    }
}*/
#endif
/*****************************************************************************/
//int _elegir_repactacion( int comando, char *cadena, int dato, struct _browse *b )
/*****************************************************************************/
/*{
    int rta = 0;
    switch( comando ) {
        case _BUSCAR:
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[10];
                _itoa( cuo_cmr.cuotas, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, cuo_cmr.descripcion );
            }
            #endif
            MOSTRAR_ENTERO( 30 / 2, 8 + dato, "99", cuo_cmr.cuotas );
            MOSTRAR_CADENA( 38 / 2, 8 + dato, cuo_cmr.descripcion );
            MOSTRAR_CADENA( 30 / 2, 15, cadena );
            break;
        case _VACIO:
            LLENAR( 30 / 2, 8 + dato, 76 / 2, 8 + dato, ' ' );
            break;
        case _RESALTAR:
            LLENAR_ATRIBUTO( 30 / 2, 8 + dato, 76 / 2, 8 + dato, 7 );
            break;
        case _SKIP:
            SKIP( dato );
            rta = BtrvEOF() || BOF();
            break;
        case _VALIDAR:
            if( dato >= '1' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? BOF() : BtrvEOF();
            break;
        case _ENTER:
        case _COMPARAR:
            rta = 1;
            break;
        case _TECLA:
            if( dato == -73 || dato == -81 ) {
                b->tecla = 0;
            }
            break;
    }
    return ( rta );
}*/
/*****************************************************************************/
//int ELEGIR_REPACTACION()
/*****************************************************************************/
/*{
    int rta = 0, pant, ok = 0;

    //---------------------------- Abre el archivo ---------------------------
    USE_DB( AREA_AUX2, _CUO_CMR, ( char* )&cuo_cmr, sizeof( cuo_cmr ), NULL, 0, 0 );
    GO( TOP );
    //-------------------------- Prepara la pantalla -------------------------
    _ATRIBUTO = 120;
    BOX( 28 / 2, 7, 78 / 2, 16 );
    MOSTRAR_CADENA( 28 / 2, 14, "������������Ĵ" );
    ok = Consulta_repactacion( _elegir_repactacion );
    if( ok ) {
        rta = cuo_cmr.cuotas;
    }
    CLOSE_DATABASE( AREA_AUX2 );
    return ( rta );
}*/
/*****************************************************************************/
//int MENU_SELECCIONA_COBRO( int *cobro, double *importe )
/*****************************************************************************/
/*{
    struct _menu *items = NULL;
    char *cobros_nombre = NULL;
    int *cobros_codigo = NULL;
    double *cobros_importe = NULL;
    int cant_items_a_revalidar = 0;
    int rta = -1;
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    #endif
    if( CANTIDAD_DE_SERVICIOS_POR_COBRO < 1 ) {
        return -1;
    }
    items = ( struct _menu * )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO, sizeof( struct _menu ) );
    cobros_nombre = ( char* )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO, sizeof( char[50] ) );
    cobros_codigo = ( int* )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO, sizeof( int ) );
    cobros_importe = ( double* )calloc( CANTIDAD_DE_SERVICIOS_POR_COBRO, sizeof( double ) );
    SELECT( AREA_VARIOS );
    GO( BOTTOM );
    SKIP( ( -1 )*( event_ticket.bultos - 1 ) );
    while( !BtrvEOF() ) {
        if( USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 ) == 0 ) {
            // busca cobros.id  que esta en bultos
            SET_ORDER( 1 );
            GET_EQUAL( ( char* )&( ( struct varios * )event_varios->buffer )->codigo );
            if( FOUND() ) {
                TRIM( cobros.nombre );
                sprintf( ( char* )&cobros_nombre[cant_items_a_revalidar * 50], " %d) %s de %.2f ",
                         cant_items_a_revalidar, cobros.nombre,
                         ( ( struct varios * )event_varios->buffer )->importe );
                cobros_codigo[cant_items_a_revalidar] = (
                ( struct varios * )event_varios->buffer )->codigo;
                cobros_importe[cant_items_a_revalidar] = (
                ( struct varios * )event_varios->buffer )->importe;
                items[cant_items_a_revalidar].nom = ( char* )
                                                   &cobros_nombre[cant_items_a_revalidar * 50];
                items[cant_items_a_revalidar].tecla = cant_items_a_revalidar + 48;  // Para obtener el Ascii de la Tecla
                items[cant_items_a_revalidar].rta = cant_items_a_revalidar;
                cant_items_a_revalidar++;
            }
            else {
                MENSAJE_STRING( S_IDENTIF_COBRO_NO_ENCONTRADO );
            }
            CLOSE_DATABASE( AREA_AUX4 );
        }
        SELECT( AREA_VARIOS );
        SKIP( 1 );
    }
    if( cant_items_a_revalidar ) {
        #if defined(INVEL_W) || defined(INVEL_L)
        columnas = 2;
        cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
        if( cab != NULL ) {
            cab[0].nombre = "COD";
            cab[0].align = ALIGN_LEFT;
            cab[0].width = 8;
            cab[1].nombre = "DESCRIPCION";
            cab[1].align = ALIGN_LEFT;
            cab[1].width = 20;
            rta = MENU( ST( S_COBROS_A_REVALIDAR ), items, cant_items_a_revalidar, cab, columnas );
            free( cab );
        }
        #else
        rta = MENU( 0, 17, ST( S_COBROS_A_REVALIDAR ), items, cant_items_a_revalidar, 120, 52 );
        #endif
        if( rta != -1 ) {
            *cobro = cobros_codigo[rta];
            *importe = cobros_importe[rta];
            rta = cobros_codigo[rta];
        }
    }
    else {
        rta = -1;
        *cobro = -1;
        *importe = -1;
    }
    free( items );
    free( cobros_nombre );
    free( cobros_codigo );
    free( cobros_importe );
    return rta;
}*/
/*****************************************************************************/
int GRABAR_COD_BARRA_EN_BINARIO( char *cod_barra, int pos )
/*****************************************************************************/
{
    char nom_aux[13];
    int handle=0;
    int error = 0;
    char codigo[80];
    char archivo[30];
    memset( codigo, 0, sizeof( codigo ) );
    strncpy( codigo, cod_barra, sizeof( codigo ) - 1 );
    sprintf( nom_aux, "ev_codba.%d", pos );
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( cod_barra != NULL && pos <= CANTIDAD_DE_SERVICIOS_POR_COBRO ) {
        handle = _CREAT( archivo, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
        if( handle > 0 ) {
            error = _WRITE( handle, codigo, sizeof( codigo ), __LINE__, __FILE__ );
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return !error;
}
/*****************************************************************************/
void INICIALIZAR_BINARIO_COD_BARRA()
/*****************************************************************************/
{
    int i = 0;
    char nom_aux[13];
    char archivo[30];
    for( i = 0;i == 0 || i <= CANTIDAD_DE_SERVICIOS_POR_COBRO;i++ ) {
        //        sprintf( archivo, "ev_codba.%d", i );
        sprintf( nom_aux, "ev_codba.%d", i );
        sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
        if( access( archivo, 0 ) == 0 ) {
            _unlink( archivo );
        }
    }
}
/*****************************************************************************/
char * LEER_COD_BARRA_EN_BINARIO( int posicion )
/*****************************************************************************/
{
    int handle;
    static char buffer[80];
    char nom_aux[13];
    char archivo[30];
    sprintf( nom_aux, "ev_codba.%d", posicion );
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        if( handle > 0 ) {
            if( !_READ( handle, buffer, sizeof( buffer ), __LINE__, __FILE__ ) ) {
                return buffer;
            }
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    return NULL;
}
/*****************************************************************************/
void ANULACION_COBROS()
/*****************************************************************************/
{
    char nro_cuenta[21];
    int _ptr,_handle,_estado;
    struct  _datos_cobro d_cobro;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    datos_cobro = &d_cobro;
    memset( &d_cobro, 0, sizeof( d_cobro ) );
    _estado = STACK_INS( sizeof( short ) );
    BORRAR_ARCHIVO_REC( ARCH_REC_COBROS );
    /*---------- prepara pantalla ----------*/

    if( _ESTADO == 0 ) {
        //SELECT( AREA_TICKET );
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
        //GET_LAST();
		SET_WHERE("");
		SET_ORDER2("ID_EVENTO");
		RUN_QUERY(NO);
		GO2(BOTTOM);
        if( event_ticket.tipo_evento == 1 ) {
            _SET_ESTADO( 1 );
        }
        else {
            MENSAJE_STRING( S_LA_ULTIMA_TRANSACCION_NO_FUE_COBRO );
        }
    }
    if( _ESTADO == 1 ) {
        if( SOLICITAR_CLAVES( 30 - ancho_panel, 21, NO, SI, SI, ST( S_ANUL_COBROS ), NULL,
                              RAM_COD_FUNCION, SI ) ) {
            SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
            //SELECT( AREA_TICKET );
            //GET_LAST();
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
			GO2(BOTTOM);
            if( /*USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )*/
             OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ))== 0 ) {
				 //SELECT_TABLE( T_COBROS, TT_ORIG );
                datos_cobro->codigo = ( INT16 )event_ticket.nro_ticket;
                datos_cobro->comprobante = event_ticket.nro_comprobante_new;
                datos_cobro->importe = event_ticket.importe_sin_iva;
                sprintf( datos_cobro->cuenta, "%ld", event_ticket.nro_cliente );
                /*SET_MEMORY_LONG( _comprobante, event_ticket.nro_comprobante );
                         SET_MEMORY_LONG( _cuenta, event_ticket.nro_cliente );
                         SET_MEMORY_DOUBLE( _importe, event_ticket.importe_sin_iva );
                         SET_MEMORY_INT( _codigo, event_ticket.nro_ticket );
                         cod_cobro = event_ticket.nro_ticket;*/
                //GET_EQUAL( ( char* )&datos_cobro->codigo );
				SET_WHERE("CODIGO = '%i'",datos_cobro->codigo);
                RUN_QUERY(NO);
                CLOSE_TABLE( T_COBROS, TT_ORIG );
                _SET_ESTADO( 2 );
            }
        }
    }
    //---Hacer la confirmacion del cobro.
    if( _ESTADO == 2 ) {
        if( ANULACION_COBROS_CONFIRMACION( datos_cobro ) ) {
            _SET_ESTADO( 3 );
        }
    }
    if( _ESTADO == 3 ) {
        if( cobros.tarjeta ) {
            COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
            strcpy( nro_cuenta, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO */);
        }
        else {
            nro_cuenta[0] = 0;
        }
        if( DEBE_ABRIR_CAJON() ) {
            ABRIR_CAJON();
        }
        TICKET_COBRANZA( cobros.nombre, datos_cobro, ( cobros.tarjeta ) ? nro_cuenta : NULL,
			SI );
        SET_MEMORY_DOUBLE( __ram_total, datos_cobro->importe );
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        #ifdef COMPILAR_TECLA_REPROCESAR
        PAGOS_REPROCESAR( 1, NO );
        #endif
        ADD_MEMORY_INT( __x_cobros_cantidad_servicios, -1 );
		ADD_MEMORY_DOUBLE( __x_cobros_importes_servicios,datos_cobro->importe );
        _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        /*----------------- Actualiza saldo en ctacte ---------------------*/
        if( ( cobros.ctacte == 1 || cobros.ctacte == 'S' ) && UTILIZA_SOCIOS ) {
            ACTUALIZA_SALDO_CLIENTE( datos_cobro->importe );
            ACTUALIZA_SOCIOS( datos_cobro->importe );
        }
        if( cobros.tarjeta ) {
            ON_LINE_PAGO_TARJETA( datos_cobro->importe, SI, 0 );
        }
        if( cobros.validacion_pie ) {
            VALIDAR( cobros.validacion_pie, _VALCOBROS, 0, 0, 0 );
        }
        IMPRIME_PIE_COMPROBANTE( IMPRIMIR_COBROS, NO );
        _SET_ESTADO( 5 );
    }
    if( _ESTADO == 5 ) {
        VALIDAR_COBROS( cobros.cod_valida, datos_cobro, cobros.nombre, NO, _MODO_COBRANZA );
        _SET_ESTADO( 6 );
    }
    if( _ESTADO == 6 ) {
        #ifdef COMPILAR_ASC
        GENERA_COBROS_ASC( cobros.id, &datos_cobro );
        #endif
        _SET_ESTADO( 7 );
    }
    if( _ESTADO == 7 ) {
        GRABAR_EVENTO_COBRO( datos_cobro, NULL, SI );
        _SET_ESTADO( 8 );
    }
    if( _ESTADO == 8 ) {
        GRABAR_LOG_SUPERVISOR( ST( S_ANUL_COBROS ), USUARIO_CLAVE_ANTERIOR(),LOG_COBROS,4);
        GRABAR_XYZ( 'X' );
        FIN_DE_TICKET( NO );
        _SET_ESTADO( 9 );
    }
    SET_MEMORY_DOUBLE( __ram_no_reprocesar, 1 );
    CERRAR_CAJON();
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
/*****************************************************************************/
int ANULACION_COBROS_CONFIRMACION( struct _datos_cobro *datos_cobros )
/*****************************************************************************/
{
    int ok = 1;

	if( MENSAJE_CON_TECLA_S_N( ST( S_CONFIRMA_LA_ANULACION_DEL_COBRO_ENTER_ESC ), 13, 27 ) == 27 ) {
		ok = 0;	
	}

    return ( ok );

}
/*****************************************************************************/
int COBROS_TARJETA_PERMITIDA( INT16 cod_cobro, INT16 cod_tarjeta )
/*****************************************************************************/
{
    int rta = 0;
    struct _cob_tar cob_tar;
    struct
    {
        INT16 cod_cobro;
        INT16 cod_tarjeta;
    } rel_cob_tar;

    if( OPEN_TABLE( T_COB_TAR, TT_ORIG, ( char* )&cob_tar, sizeof( cob_tar ) ) == 0 ) {
	//if( USE_DATABASE( AREA_COB_TAR, "c:cob_tar.sic", 
		//( char * ) &cob_tar, sizeof( cob_tar ), NULL, 0, 0 ) == 0 ) {
	//if( USE_DB( AREA_COB_TAR, _COB_TAR_SIC,( char * ) &cob_tar, 
	//		sizeof( cob_tar ), NULL, 0, 0 ) == 0 ) {
        rel_cob_tar.cod_cobro = cod_cobro;
        rel_cob_tar.cod_tarjeta = cod_tarjeta;
		SET_WHERE( "COD_COBRO = '%i' AND COD_TARJETA = '%i'", rel_cob_tar.cod_cobro,
			rel_cob_tar.cod_tarjeta );
		RUN_QUERY( NO );
        rta = FOUND2();
        CLOSE_TABLE( T_COB_TAR, TT_ORIG );
    }
    else {
        MENSAJE( "ERROR AL ABRIR COB TAR" );
    }
    return( rta );
}
/*****************************************************************************/
COBROS_F_ONLINE( double importe )
/*****************************************************************************/
{
	int 	  rta = 1;
	struct	  _datos_cobro datos_cobro;
	/*solo sirve para un cobro, si se empieza a usar cobros varios se debera 
	reveer esta funcion para buscar el reg. recuperado*/ 
	if( !RAM_FLAG_COBRO_VENTA ){
	RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )&datos_cobro,
	    sizeof( struct _datos_cobro ), 0 );
	}else{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA, ( char* )&datos_cobro,
	    sizeof( struct _datos_cobro ), indice_cobro_venta );
	}
	if( datos_cobro.tarjeta ) {
		LEE_TARJETAS( 0, TARJETA_COBRO );
		NUMERO_DE_COMERCIO_TARJETA(  );
		if( datos_cobro.tarjeta != T_OFF_LINE ){
			rta = ON_LINE_PAGO_TARJETA( datos_cobro.importe, RAM_ANULACION, datos_cobro.tarjeta );
		}
		GRABA_MODIFICACION_TARJETAS( 0, TARJETA_COBRO );
	}
	return( rta );
}
/*******************************************************************************/
int VALIDAR_MONEDERO_ELECTRONICO( double importe )
/*******************************************************************************/
{
	int rta = 0;
	if( fabs( importe ) > 0.0001 ) {
		if( ( _PAGO_TIPO_VUELTO( RAM_P_PAGO - 1 ) == _VUELTO_OTRO_MEDIO && 
				_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ) > 0 && 
				medios[_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO -1 ) - 1].tarjeta ) ||
				( _PAGO_TIPO_VUELTO( RAM_P_PAGO - 1 ) == _VUELTO_PROPIO && 
				medios[_PAGO_MODO(RAM_P_PAGO - 1 )].tarjeta ) ) {	
			if( strlen( RAM_CUENTA_CLIENTE ) ) {
				if( ENVIAR_IMPORTE_MONEDERO( importe,  TARJETA_MONEDERO ) ) {
					_SET_MEMORY( __pago_auto_tarjeta, RAM_P_PAGO - 1, VAR_TARJETAS_AUTORIZACION );
					_SET_MEMORY( __pago_nro_tarjeta, RAM_P_PAGO - 1, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/);
					rta = 1;
				}
			}
		} else {
			rta = 1;
		}
	} else {
		rta = 1;
	}
	return( rta );
}
/*****************************************************************************/
int ENVIAR_IMPORTE_MONEDERO( double importe, char archivo )
/*****************************************************************************/
{
    int rta = 1;
	LEE_TARJETAS( 0, archivo );
	NUMERO_DE_COMERCIO_TARJETA(  );
	rta = ON_LINE_PAGO_TARJETA( importe, 0, T_ON_LINE );
	GRABA_MODIFICACION_TARJETAS( 0, archivo );
	
	return( rta );
} 
/*****************************************************************************/
void MOSTRAR_DATOS_CODIGO_BARRA( struct _datos_cobro *datos_cobro )
/*****************************************************************************/
{
	char buffer[20];
	/*----------------Mostrar Codigo Barra---------------*/	
    if( datos_cobro->datos_pedir & _COBRO_DATOS_CODIGO_BARRA ) {
    ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
    if( strlen( datos_cobro->cod_barra ) == 0 ) {
		MOSTRAR_INPUT_CADENA( "", 0 );
    }else {
        MOSTRAR_INPUT_CADENA( datos_cobro->cod_barra, 0 );
    }
    }
	/*-----------------Mostrar Comprobante---------------*/
    if( datos_cobro->datos_pedir & _COBRO_DATOS_NRO_COMPROBANTE ) {
	ExecuteAction( A_MUESTRA_DATO_COMPROB_COBRO );
	if( datos_cobro->comprobante ) {
		sprintf( buffer, "%.0lf", datos_cobro->comprobante );		
		MOSTRAR_INPUT_CADENA( buffer, 0 );
	} else {
		MOSTRAR_INPUT_CADENA( "", 0 );
	}
    }
	/*-----------------Mostrar Cuenta--------------------*/
    if( datos_cobro->datos_pedir & _COBRO_DATOS_NRO_CUENTA ) {
	ExecuteAction( A_MUESTRA_DATO_CUENTA_COBRO );
	if( strlen( datos_cobro->cuenta ) > 0 ) {		
		MOSTRAR_INPUT_CADENA( datos_cobro->cuenta, 0 );
	} else {
		MOSTRAR_INPUT_CADENA( "", 0 );
	}
    }
	/*-----------------Mostrar Importe-------------------*/
    if( datos_cobro->datos_pedir & _COBRO_DATOS_IMPORTE ) {
	ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
	if( datos_cobro->importe ) {
		sprintf( buffer, "%.2lf", datos_cobro->importe );
		MOSTRAR_INPUT_CADENA( buffer, 0 );
	} else {
		MOSTRAR_INPUT_CADENA( "", 0 );
	}
}
}
/******************************************************************************/
int COBROS_GARANTIA( char devolucion )
/******************************************************************************/
{
	int		  rta = 0;
    int       _ptr, _handle, _estado, panel_activado = 0;
    struct    _datos_cobro datosCobros;
	
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );
    RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * ) &datosCobros, 
		    sizeof( struct _datos_cobro ), 0 );
    if( _ESTADO == 1 ) {
		PANTALLA_COBROS(  );
		//ExecuteAction( A_ACTIVAR_PANEL_GARANTIA );
		//panel_activado = 1;
		UNGETCH( 13 );
		ELEGIR_COBRO_BROWSE( _MODO_INTERACTIVO, NO );
        MOSTRAR_DATOS_SOLICITADOS_EN_COBROS( cobros.datos_pedir, COBRO_GARANTIA );
	}
	if( _ESTADO == 0 ) {
		POSICIONAR_COBRO_SELECCIONADO(  );
		memset( &datosCobros, 0x00, sizeof( struct _datos_cobro ) );
		_SET_ESTADO( 1 );
	}
	if( _ESTADO == 1 ) { //Obtengo Datos
		//int  rta;
		//ExecuteAction( A_ACTIVAR_PANEL_GARANTIA );
		//ExecuteSAction( A_MUESTRA_TIT_COD_FOLIO_G, "Nro Folio :" );
		//ExecuteSAction( A_MUESTRA_TIT_COD_ARTICULO_G, "Articulo :" );
        //ExecuteSAction( A_MUESTRA_TIT_IMPORTE_G, ST( S_Importe ) );
		rta = PEDIR_DATOS_COBRO_GARANTIA( &datosCobros );		
        datosCobros.devolucion = devolucion;
		//ExecuteAction( A_DESACTIVAR_PANEL_GARANTIA );
        ExecuteAction( A_DES_COBROS_ACT_VENTAS );
		if( devolucion ) {
			ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_DEV_COBRO_BORRAR, " ",
				" ", NULL, " " );
		} else {
			ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_BORRAR, " ",
				" ", NULL, " " );
		}
		GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * ) &datosCobros, 
			sizeof( struct _datos_cobro ) );
		if( rta ){
			_SET_ESTADO( 2 );       
			
			if( RAM_FLAG_COBRO_VENTA && !RAM_ANULACION ){
				GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_GARANTIA , ( char * ) &datosCobros, 
					sizeof( struct _datos_cobro ) );
			}

		}else{
			_SET_ESTADO( 6 );
		}
	}

    if( _ESTADO == 2 ) { //Calculo Monto!!
		//Buscar articulo
        if( RAM_TOTAL || RAM_FLAG_COBRO_VENTA ) {
       		if( cobro_marginal || cobros.venta) { 
                _SET_ESTADO( 6 );//Desactiva Pantalla Cobro y Activa Pantalla Venta
            } else {
                _SET_ESTADO( 3 );       
            }

		} else { //error
			MENSAJE("Cobro cancelado, importe cero");
			_SET_ESTADO( 6 );
		}
	}

	if( _ESTADO == 3 ){
		if( !PAGOS( _MODO_COBRANZA, NULL, devolucion ) ) {
			MENSAJE("COBRO ANULADO");
			_SET_ESTADO( 6 );
			rta = 0;
		} else {
			rta = 1;
			_SET_ESTADO( 4 ); //Todo ok.
		}
    }
	if( _ESTADO == 4 ) {   //Validacion!!
		VALIDAR_COBROS_GARANTIA( &datosCobros, _MODO_COBRANZA ); 
		_SET_ESTADO( 5 );
    }
    if( _ESTADO == 5 ) { 		//Graba el evento
        GRABAR_EVENTO_COBRO_GARANTIA( &datosCobros, devolucion );
		_SET_ESTADO( 6 );
    }
	if( _ESTADO == 6 ) { //
		if( rta && RAM_FLAG_COBRO_VENTA ){
			if( rta = AGREGAR_ARTICULO_COBRO_VENTA( cobros.cod_barra, cobros.cod_interno, 
					datosCobros.importe, datosCobros.codigo ) ){
				GRABAR_EVENTO_COBRO_GARANTIA( &datosCobros, devolucion );
				SET_MEMORY_CHAR( __ram_hay_cobro_venta, 1 );
				//GRABAR_MEDIOS_COBRO( datosCobros.codigo , datosCobros.importe );
				//SET_MEMORY_DOUBLE( __item_precio, datos_cobro->importe );
			}
			if( RAM_ANULACION || !rta){
				CANCELAR_COBRO_VENTA( (void*)&datosCobros, COBRO_GARANTIA );
				SET_MEMORY_INT( __ram_anulacion, 0 );
			}
			CR1_INICIALIZAR_VAR_TICKET( SI );
		}
		SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
		//_SET_ESTADO( 7 );
    }

	CLOSE_RECUPERO( _ptr, _handle );
	return rta;
}
/*****************************************************************************/
int PEDIR_DATOS_COBRO_GARANTIA( struct _datos_cobro *datos_cobro )
/*****************************************************************************/
{
    int		rta = 0, salir = 0, sig = 0, fin = 0;
    char	*cod_barra = NULL, *cuenta = NULL;
	char	cod_barra_aux[81];
	long	codigo_barra = 0, comprobante = 0, _cuenta = 0;
    double	importe = 0;
    /*------------------------ Pide los datos -----------------------------*/
    while( !salir ) {
		TECLADO_ALFABETICO = 0;
		_PERMITIR_SCANNER = SI;
		/*----------------------- Pide Folio ----------------------------*/
        while( sig == 0 && fin == 0 ) {
			memset( cod_barra_aux, 0, sizeof( cod_barra_aux ) );
            ExecuteAction( A_MUESTRA_DATO_FOLIO_G );
			MOSTRAR_INPUT_CADENA( "", 0 );
			codigo_barra = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
				( char )0, ( char )13, cod_barra_aux, NULL, 0, SI, NO , NULL);
            if( codigo_barra > 0 ) {
				cod_barra = cod_barra_aux;
				if( strlen( cod_barra ) > 0 ) {
					memcpy( datos_cobro->cod_barra, cod_barra_aux, 13 );
					if( EVALUAR_MASCARA_COD_BARRA( cobros.codigo,
						cod_barra, datos_cobro ) ) {
						sig = 1;
					} else {
						fin = 1;
					}
				} else {
					//fin = 1;
					codigo_barra = 0;
				}
			} else {
               fin = 1;
			   codigo_barra = 0;
            }
        }
        sig = 0;
		/*------------------------------Pedir Comprobante-----------------------------*/
		
		//if( datos_cobro->cod_barra_articulo == 0 ) {
			while( sig == 0 && fin == 0 ) {
				memset( cod_barra_aux, 0, sizeof( cod_barra_aux ) );
			    BORRAR_MENSAJE( );
				ExecuteAction( A_MUESTRA_COD_ARTICULO_G );
				comprobante = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
				( char )0, ( char )13, cod_barra_aux, NULL, 0, SI, NO , NULL);
				if( comprobante > 0 ) {
					memcpy( datos_cobro->cod_barra_articulo, cod_barra_aux, 13 );
					if( BUSCA_CODIGO_BARRA( datos_cobro->cod_barra_articulo ) ){
						if( articulo.precio == 'X' ) { //Articulo Garantizable
							sig = 1;
						} else {
							ExecuteAction( A_MUESTRA_COD_ARTICULO_G );
							MOSTRAR_INPUT_CADENA( "", 0 );
							MENSAJE("ARTICULO NO GARANTIZABLE");
						}
					} else {
                        ExecuteAction( A_MUESTRA_COD_ARTICULO_G );
						MOSTRAR_INPUT_CADENA( "", 0 );
						MENSAJE("ARTICULO NO ENCONTRADO");
					}
				} else {
					fin = 1;
					comprobante = 0;
				}
			}
		//}
		sig = 0;
		
		_PERMITIR_SCANNER = NO;
		
		if( sig == 0 && fin == 0 ) {
			double importe;
			char buffer[21];
			//Buscar articulo
			if ( ASIGNA_CODIGO_BARRA( datos_cobro->cod_barra_articulo ) ) {
				//Buscar Folio Y CALCULO MONTO DE GARANTIA
				importe = CALCULAR_MONTO_GARANTIA( fabs(ITEM_PRECIO), datos_cobro->anios_garantia );

				if( importe ) {
					if( !RAM_FLAG_COBRO_VENTA )
					SET_MEMORY_DOUBLE( __ram_total, importe );
					datos_cobro->importe = importe;
					//ExecuteAction( A_MUESTRA_DATO_IMPORTE_G );
                    ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
					sprintf( buffer, "%.2lf", importe );		
					MOSTRAR_INPUT_CADENA( buffer, 0 );
				} else {
					fin = 1;
				}
			}   
		}

		/*------------------------Pedir Datos Adicionales------------------------------*/
		if( cobros.datos_especiales == 3 && sig == 0 && fin == 0 ) {
			if( PAGOS_PEDIR_DATOS_ADICIONALES( ( RAM_FLAG_COBRO_VENTA ) ? RAM_P_TICKET : 0, 0, cobros.cod_solicitud,
				0, D_COBROS, NULL, 1, NULL, NULL , '\0', '\0') ) {
				struct _datos datos;

				if( _PAGOS_OBTENER_DATOS( ( RAM_FLAG_COBRO_VENTA ) ? RAM_P_TICKET : 0, 0, &datos, D_COBROS ) ) {
					strncpy( datos_cobro->dato_adicional, datos.valor,
						sizeof( datos_cobro->dato_adicional ) );
					//Cobros venta
					if( RAM_FLAG_COBRO_VENTA ){
						AGREGA_BIN_EV_DMEDIO_A_BTRV( RAM_P_TICKET, D_COBROS );
					}

					sig = 1;
				}
			} else {
				fin = 1;
			}
		}
		
		if( fin ) {
			int ok = 0, k;

			while( !ok ) {
				MENSAJE( "DESEA SALIR DE COBROS? <ENTER> PARA SALIR - <ESC> PARA CONTINUAR" );
				k = GETCH();
				switch( k ) {
					case 13:
						ok = 1;
						salir = 1;
						break;
					case 27:
						ok = 1;
						fin = 0;
						memset( datos_cobro, 0, sizeof( struct _datos_cobro ) );
						//MOSTRAR_DATOS_CODIGO_GARANTIA( datos_cobro );
						codigo_barra = 0;
						comprobante = 0;
						_cuenta = 0;
						importe = 0;
						break;
				}
			}
			BORRAR_MENSAJE(  );
		} else if( cod_barra != NULL ) {
			int k = 0;
            salir = 1;
			MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );			
		}
	}
	BORRAR_MENSAJE();
	if( fin == 0 ) {
		if( datos_cobro->importe > 0 ) {
			rta = 1;
		}
    }
	
    return ( rta );
}
/**********************************************************************************/
double CALCULAR_MONTO_GARANTIA( double precio, INT16 tipo )
/**********************************************************************************/
{
	int  rango_ok = 0;
	int  area_ant, tipo_ant;
	float porc = 0.00;
	double monto;
	struct _garantias_porc garantia;
	
	struct {
		char   tipo;
		double limite;
	} indice;

 
	area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	//Setear Indice
	indice.tipo = (char)tipo;
	indice.limite = 0;//precio;
	//Buscar
	//area_ant = SELECTED( );
	
   // if( USE_DB( AREA_GARANTIA, _GARANTIA, ( char* )&garantia,
   //             sizeof( struct _garantias_porc ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_GARANTIA, TT_ORIG, ( char* )&garantia, sizeof( struct _garantias_porc ) ) == 0 ) {
		SELECT_TABLE( T_GARANTIA, TT_ORIG );
		//SET_ORDER( 1 );
		//GET_GREATER_E( (char*)( &indice ) );
		SET_WHERE("TIPO = '%d' and LIMITE_INFERIOR >= '%i'", tipo, 0.00 ); 
		RUN_QUERY(NO);
		SET_ORDER2("TIPO, LIMITE_INFERIOR");
		//if( FOUND( ) ) {
		if( FOUND2( ) ) {
			do {
				if( fabs(precio) >= garantia.limite_inferior  && fabs(precio) < garantia.limite_superior  ){
					porc = garantia.porcentaje;
					if( porc <= 0.00 ) {
						MENSAJE( "PORCENTAJE INVALIDO" );
						break;
					} else {
						rango_ok = 1;
					}
				} else {
					//SKIP( 1 );
					SKIP2( 1 );
				}
			} while( garantia.tipo == (char)tipo && !dbEOF() && porc == 0.00 );
			
			if( !rango_ok ){
				MENSAJE( "NO SE ENCONTRO RANGO DE IMPORTE VALIDO" );
			}
		} else {
			MENSAJE( "PERIODO DE GARANTIA NO ENCONTRADO" );
		}
        //CLOSE_DATABASE( AREA_GARANTIA );
		CLOSE_TABLE( T_GARANTIA, TT_ORIG );
    }

	//Calcular porc!
	monto = ( precio * porc ) / 100;

	if( monto <= 0 && rango_ok ) {
		MENSAJE( "MONTO DE GARNTIA INVALIDO" );
	}
	
	//SELECT( area_ant );
	SELECT_TABLE( area_ant, tipo_ant );
	return monto;
}
/*****************************************************************************/
void GRABAR_EVENTO_COBRO_GARANTIA( struct _datos_cobro *datos_cobro, int anulacion )
/*****************************************************************************/
{
    char *cod_barra = NULL;
	int /*area,*/ tabla_ant, tipo_ant, bandera = 0;
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    //event_ticket.nro_ticket = ( long )cobros.codigo;
	 event_ticket.nro_ticket = 0;
	event_ticket.cod_cobro = cobros.codigo;
    event_ticket.bultos = cobros.id;
    //event_ticket.nro_cliente = datos_cobro.cuenta;
    event_ticket.nro_comprobante_new = datos_cobro->comprobante;
	event_ticket.importe_sin_iva = ( anulacion ) ? -datos_cobro->importe 
		: datos_cobro->importe;
    event_ticket.tipo_evento = ( anulacion ) ? 50 : 1;
    event_ticket.nro_vendedorl = NRO_VENDEDOR_LONG;
    event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;
	if( !RAM_FLAG_COBRO_VENTA ){
    GRABAR_EVENT_TICKET( NO );
    GRABA_EVENTOS_PAGO( 0 , anulacion );
	}

	//area = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	//SELECT( AREA_COD_BARRA );
	SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
	do {
		memset( &event_cod_barra, 0, sizeof( event_cod_barra ) );
		event_cod_barra.caja = NRO_CAJA;
		event_cod_barra.nro_z = NRO_Z;
		event_cod_barra.id_evento = RAM_ID_EVENTO;
		event_cod_barra.posicion_cod_barra = ( RAM_FLAG_COBRO_VENTA ) ? bandera + 1 : 1;
		event_cod_barra.cuenta = cobros.codigo;
		event_cod_barra.posicion_medio = ( RAM_FLAG_COBRO_VENTA ) ? RAM_P_TICKET : bandera;
		if( bandera == 1 ) {
			event_cod_barra.importe = ( anulacion && !RAM_FLAG_COBRO_VENTA ) ? -ITEM_PRECIO : 
				ITEM_PRECIO; 
		}
		event_cod_barra.nro_comprobante_new = datos_cobro->comprobante;
		{//Controlar el tamaño del campo nro_comprobante_new
			char nro[12];//No debe superar los 10 digitos porque es lo maximo que el campo del servidor acepta
			memset(nro,0,sizeof(nro));
			_snprintf( nro, sizeof(nro)-2,"%lf",event_ticket.nro_comprobante_new );
			event_ticket.nro_comprobante_new = atof(nro);
		}		
		strncpy( event_cod_barra.nro_cuenta_ext, datos_cobro->cuenta,
				sizeof( event_cod_barra.nro_cuenta_ext ) );
		event_cod_barra.cod_sucursal = config.sucursal;
		event_cod_barra.fecha_ticket = NRO_FECHA_OPERACION/*( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS()*/;
		event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
		if( bandera == 0 ) {		
			memcpy( event_cod_barra.codigo_barra, datos_cobro->cod_barra,
				sizeof( event_cod_barra.codigo_barra ) );
		} else if( bandera == 1 ) {
			memcpy( event_cod_barra.codigo_barra, datos_cobro->cod_barra_articulo,
				sizeof( event_cod_barra.codigo_barra ) );
		}
		INSERT2(NO);
		bandera++;
	} while( bandera < 2 );

	//SELECT( area );
	SELECT_TABLE( tabla_ant, tipo_ant );

	if( !RAM_FLAG_COBRO_VENTA )
    ACTUALIZA_ID_EVENTO_NEW();
}

/*****************************************************************************/
void EVALUAR_COBROS_MONEDA_EXTRANJERA( double importe, struct _datos_cobro *datos_cobro )
/*****************************************************************************/
{
	int		rta = 0;
	char    aux[30], simb_me[4];
	double	cotizacion = 0.0;
	if( cobros.cod_medio_me && medios[cobros.cod_medio_me - 1].mutual ) {
		rta = COTIZACION_MUTUAL( cobros.cod_submedio_me, &cotizacion, simb_me );
	} else if( cobros.cod_medio_me && medios[cobros.cod_medio_me - 1].cotizacion != 0.00 ) {
		cotizacion = medios[cobros.cod_medio_me - 1].cotizacion;
		strncpy( simb_me, medios[cobros.cod_medio_me - 1].simbolo_moneda, sizeof( simb_me ) );
	}
	if( cotizacion ) {
		sprintf( aux, "%s %.2lf", simb_me, ROUND( importe, ENTEROS, DECIMALES ) );
		strncpy( datos_cobro->mensaje, aux, sizeof( datos_cobro->mensaje ) );
		ExecuteSAction( A_MUESTRA_COBRO_IMPORTE_ME, aux );
		datos_cobro->importe = importe * cotizacion;
	} else {
		datos_cobro->importe = importe;
	}
}

/*******************************Para procesar cobros marginales****************************/
int VALIDAR_COBROS_VENTA( void )
/*****************************************************************************/
{
    int		cobro = 0, _ptr, _handle, _estado, rta = 0, ok = 1;
	struct	_datos_cobro *datos_cobros;
	struct	_datos_cobro d_cobro;
	struct    DatosCobroPagaTodo datosCobros;
	/*unsigned aux_fecha;*/

	/*--------------------- Prepara recupero de memoria ----------------------*/
	OPEN_RECUPERO( &_ptr, &_handle );
	datos_cobros = &d_cobro;
	memset( &d_cobro, 0, sizeof( d_cobro ) );
    _estado = STACK_INS( sizeof( short ) );	

	if( !RAM_HAY_COBRO_VENTA )
		return 1;
	//Si hay cobros venta
	SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );
	
	/***********************************Cobros Tradicionales OnLine******************************************/
	for( indice_cobro_venta = 0; 
			indice_cobro_venta < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA, sizeof( struct _datos_cobro ) );
			indice_cobro_venta++ )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA, ( char* )datos_cobros,
			sizeof( struct _datos_cobro ), indice_cobro_venta );
		//USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
		//SELECT( AREA_AUX4 );
		//GET_EQUAL( ( char* )&datos_cobros->codigo );
		//CLOSE_DATABASE( AREA_AUX4 );
		OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ));
	    //SELECT_TABLE( T_COBROS, TT_ORIG );
	    SET_WHERE("CODIGO='%d'", datos_cobros->codigo );
    	RUN_QUERY(NO);		
		CLOSE_TABLE( T_COBROS, TT_ORIG );
		if( cobros.online ){
			if( _ESTADO == 5 ){
				//Hay mas de un cobro
				_SET_ESTADO( 0 );
			}
			if( _ESTADO == 0 ){
				if( !COBROS_F_ONLINE( datos_cobros->importe ) ){
					_SET_ESTADO( 2 );
					break;
				}else{
					_SET_ESTADO( 3 );
				}
			}
			if( _ESTADO == 3 ){
				if( cobros.tarjeta ) {
					LEE_TARJETAS( 0, TARJETA_COBRO );
				}
				ADD_MEMORY_INT( __x_cobros_cantidad_servicios, 1 );
				ADD_MEMORY_DOUBLE( __x_cobros_importes_servicios,datos_cobros->importe );
				_SET_ESTADO( 4 );
			}
			/*----------------- Actualiza saldo en ctacte ---------------------*/
			if( _ESTADO == 4 ){
				if( ( cobros.ctacte == 1 || cobros.ctacte == 'S' ) && UTILIZA_SOCIOS ) {
					ACTUALIZA_SALDO_CLIENTE( -( datos_cobros->importe ) );
					ACTUALIZA_SOCIOS( -( datos_cobros->importe ) );
				}
				_SET_ESTADO( 5 );
				//break;
			}
		}
	}
	if( indice_cobro_venta == CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA, sizeof( struct _datos_cobro ) ) ){
		_SET_ESTADO( 1 );
	}
	/***********************************Cobros PAGATODO******************************************/
	if( _ESTADO == 1 ){
		if( !RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros, 
			sizeof( struct DatosCobroPagaTodo ), 0 ) ){
			if( !F_PAGATODO_COBRO_VENTA( datosCobros._importe ) ){
				_SET_ESTADO( 2 );
			}
		}
	}
	/***********************************REVERSAS******************************************/
	if( _ESTADO == 2 ){
		/******************************Para cobros tarjetas****************************/
		REPROCESAR_TARJETAS( _MODO_VENTA, NO );
		/******************************Para PagaTodo***********************************/
		if( !RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros, 
			sizeof( struct DatosCobroPagaTodo ), 0 ) ){
				int   rta,intentos = REINTENTOS_PAGATODO_REVERSA;
				
				datosCobros._estado_envio = TRANSACCION_REVERSO_ENVIANDO;
				ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
					sizeof( datosCobros ), 0 );

				do{
					rta = ENVIAR_TRANSACCION_PAGATODO( PAGATODO_REVERSA , &datosCobros );
					intentos--;
				}while( intentos && rta < 0);
		}
		ok = 0;
	}

	SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );

	CLOSE_RECUPERO( _ptr, _handle );
	return ok;
}

/**********************************************************************/
void GRABAR_MEDIOS_COBRO( int cod_cobro , double importe )
/**********************************************************************/
{
	//int area_ant;
	struct _cob_med cob_med;
	struct requerir_medios medios_requeridos;
	//area_ant = SELECTED( );
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();

	//if( USE_DB( AREA_COB_MED, _COB_MED_SIC, ( char* )&cob_med, sizeof( cob_med ), NULL, 0, 0 ) == 0 ) {
	if( cod_cobro && OPEN_TABLE( T_COB_MED, TT_ORIG ,( char* )&cob_med, sizeof( cob_med )) == 0 ) {
		//SELECT( AREA_COB_MED );
		//SELECT_TABLE( T_COB_MED, TT_ORIG );
		//SET_ORDER( 5 );
        //GET_EQUAL( (char*)&cod_cobro );
        SET_WHERE("COD_COBRO='%d'", cod_cobro );
        RUN_QUERY(NO);
		if( FOUND2( ) ){
			while( /*!BtrvEOF()*/ !dbEOF() && cob_med.cod_cobro == cod_cobro ){
				if( cob_med.habilitado ){
					medios_requeridos.origen = ORIGEN_COBRO;
					medios_requeridos.cod_grupo = RAM_P_TICKET;
					medios_requeridos.medio = cob_med.cod_medio;
					medios_requeridos.submedio = ( cob_med.cod_submedio ) ? cob_med.cod_submedio : -1;
					medios_requeridos.plan  = -1;
					medios_requeridos.monto = importe;
					medios_requeridos.monto_compartido = SI;
					medios_requeridos.condicion_monto = CONDICION_MAYOR_IGUAL;
					GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios));
				}
				SKIP2( 1 );
			}
		}
	}
	//SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
}

/*****************************************************/
int VALIDACIONES_COBROS_VENTA( )
/*****************************************************/
{
	int ok = 1, continuar = 1;
	struct	_datos_cobro d_cobro;
	struct    DatosCobroPagaTodo datosCobros;
	unsigned aux_fecha;
	struct	_datos_cobro *datos_cobros = &d_cobro;
    int cobro_kinacu = 0;
    posicionTransaccionKinacu = 0;

	if( !RAM_HAY_COBRO_VENTA )
		return 1;
	SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );

	//Si el 1er art. es un Cobro Marginal kinacu. No se graba en arch. PRN para en caso de 
	//reimprimir el ticket no lo haga ya que imprime los pines  - Solicitud Salvador 
	if( config.pais == ELSALVADOR && RAM_NRO_PEDIDO_PIN ) {
	//En Cobro Marginal kinacu no graba voucher en arch. PRN para no 
	//reimprimir el ticket por los pines que muestra - Solicitud Salvador 
		CLOSE_COPIA_TICKET_PRN(  );
		grabar_copia_ticket = 2;
	}
	/***********************************Cobro Tradicional*************************************/
	for( indice_cobro_venta = 0; 
			indice_cobro_venta < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA, sizeof( struct _datos_cobro ) );
			indice_cobro_venta++ )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA, ( char* )datos_cobros,
	        sizeof( struct _datos_cobro ), indice_cobro_venta );
        //USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
        //SELECT( AREA_AUX4 );
        //GET_EQUAL( ( char* )&datos_cobros->codigo );
        //CLOSE_DATABASE( AREA_AUX4 );
		OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ));
    	//SELECT_TABLE( T_COBROS, TT_ORIG );
    	SET_WHERE("CODIGO='%d'", datos_cobros->codigo );
    	RUN_QUERY(NO);
        continuar = FOUND2();
    	CLOSE_TABLE( T_COBROS, TT_ORIG );
		
		if( continuar ) { //Cobros KINACU
			if( RAM_NRO_PEDIDO_PIN  ) {
				cobro_kinacu = 0;
				while( cobros.cod_funcion == 250 && !cobro_kinacu 
						&& RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&datos_cobro_kinacu, 
						sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {
					if( datos_cobro_kinacu.id_transaccion 
							&& strcmp( datos_cobro_kinacu.cod_barra, datos_cobros->cod_barra_art_venta ) == 0  
							&& ( datos_cobro_kinacu.cod_interno == datos_cobros->cod_interno_art_venta )
							&& ( fabs( datos_cobros->importe - datos_cobro_kinacu.monto ) < 0.0001 ) ) {
						cobro_kinacu = 1;
					}
					posicionTransaccionKinacu ++;
				}
			}
		}
		if( continuar && !IMPRIMIR_COBROS ) {
		    /*Se decrementa el mes solo para la impresion, luego se le devuelve el valor original*/
			if( cobros.tarjeta ) {
				aux_fecha = VAR_TARJETAS_FECHA_VTO;
				SET_MEMORY_INT( __var_tarjetas_fecha_vto, DECREMENTAR_MES( aux_fecha ));
			}
 			if( datos_cobros->cuenta ) {
				memset(ImpDatosAdi.nro_cuenta, 0,  sizeof( ImpDatosAdi.nro_cuenta ) );
				strncpy( ImpDatosAdi.nro_cuenta, datos_cobros->cuenta,  sizeof( ImpDatosAdi.nro_cuenta ) - 1 );
			}
            if( !cobro_kinacu ) { 
				VALIDAR_COBROS( cobros.cod_valida, datos_cobros, cobros.nombre,
					datos_cobros->devolucion, _MODO_COBRANZA );
			} else {//Cobros KINACU
				VALIDAR_COBROS_KINACU( &datos_cobro_kinacu, _MODO_COBRANZA, posicionTransaccionKinacu - 1 );
            }
			if( cobros.tarjeta ) {
				SET_MEMORY_INT( __var_tarjetas_fecha_vto, aux_fecha );
			}
		}
		if( continuar && cobros.validacion_pie ) {
			VALIDAR( cobros.validacion_pie, _VALCOBROS, 0, 0, 0 );
		}
		if( continuar && IMPRIMIR_COBROS ) {
			if( !cobro_kinacu ) {    
				VALIDAR_COBROS( cobros.cod_valida, datos_cobros, cobros.nombre, datos_cobros->devolucion, _MODO_COBRANZA );
            } else {//Cobros KINACU
				VALIDAR_COBROS_KINACU( &datos_cobro_kinacu, _MODO_COBRANZA, posicionTransaccionKinacu - 1 );
            }
		}
	}
	/***********************************Cobro PAGATODO*************************************/
	if( !RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros, 
				sizeof( struct DatosCobroPagaTodo ), 0 ) ){
		if( datosCobros._anulacion == 0 ) {
			char    nip[17];
			memset( nip, 0,  sizeof( nip ) );
			strncpy( nip, datosCobros._nip, sizeof( nip ) - 1 );
			TRIM( nip );
			if( !datosCobros.__devolucion && strlen( nip ) > 0 && config.pais != ARGENTINA) {
				MENSAJE_SIN_SONIDO( "RECUERDE ENTREGAR NIP AL CLIENTE - PRESIONE <ENTER>",NO );
				while( GETCH() != 13 );    
				//}
				BORRAR_MENSAJE();
				//MENSAJE_CON_TECLA( "RECUERDE ENTREGAR NIP AL CLIENTE - PRESIONE <ENTER>", 13);
			}
			VALIDAR_COBROS_PAGATODO( &datosCobros, _MODO_COBRANZA ); 
			ok = 1;
		} else {
			ok = 0;
		}
	}
	/***********************************Cobro GARANTIA*************************************/
	for( indice_cobro_venta = 0; 
			indice_cobro_venta < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA_GARANTIA, sizeof( struct _datos_cobro ) );
			indice_cobro_venta++ )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_GARANTIA, ( char* )datos_cobros,
	        sizeof( struct _datos_cobro ), indice_cobro_venta );
		//USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
        //SELECT( AREA_AUX4 );
        //GET_EQUAL( ( char* )&datos_cobros->codigo );
        //CLOSE_DATABASE( AREA_AUX4 );
   		OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros ));
    	//SELECT_TABLE( T_COBROS, TT_ORIG );
    	SET_WHERE("CODIGO='%d'", datos_cobros->codigo );
    	RUN_QUERY(NO);
    	CLOSE_TABLE( T_COBROS, TT_ORIG );

		VALIDAR_COBROS_GARANTIA( datos_cobros, _MODO_COBRANZA );
	}
	
	PAGOS_IMPRIME_PAGOS_ESPECIALES( MODO_DEVOLUCION, NO );
    CEREAR_TABLA_LIN_MENS_TMP(  );
    valida_kinacu_id_transaccion = 0;
    posicionTransaccionKinacu = 0;

	SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
	return ok;
}

/*********************************************************************************/
int	TRAER_COBROS_VENTA_ESPERA( long caja_z,long id_evento, int devolucion )
/*********************************************************************************/
{
	int rta = 1, /*area_ant*/ tabla_ant, tipo_ant;
	//struct _event_cod_barra e_cod_barra;
	struct _datos_cobro datos_cobro;
	struct _ev_dmedios ev_dpago;
	
	//struct {
	//	long caja_z;
	//	long id_evento;
	//	int posicion;
	//}indice;

	//indice.caja_z = caja_z;
	//indice.id_evento = id_evento;
	//indice.posicion = 0;

	//if( USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 ) ){
	//	return 0;
	//}
	//if( USE_DB( AREA_DPAGO, _EV_DPAGO_BTR, ( char* )&ev_dpago, sizeof( struct _ev_dmedios ),
	//		NULL, 0, 0 ) ){
	//	return 0;
	//}
	if( OPEN_TABLE( T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros )) != 0 ) {
		return 0;
	}
	if( OPEN_TABLE( T_EV_DMEDIOS, tipo_ev_dpago ,( char* )&ev_dpago, sizeof( struct _ev_dmedios )) != 0 ) {
		return 0;
	}
	
	//area_ant = SELECTED( );
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	//SELECT( AREA_E_TICKET );
	//SET_ORDER( 3 );
	//GET_GREATER_E( (char*)&indice );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	SET_ORDER2( "CAJA_Z, ID_EVENTO, POSICION" );
	//(Nota Unific MySQL-Linux): la intencion del GET_GREATER_E parece ser encontrar
	//los eventos con caja_z y id_evento exactos, pero a partir de la posicion 0.
	SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION>='%d' ", caja_z, id_evento, 0 );
	RUN_QUERY(NO);

	if( FOUND2( ) ){
		while( /*!BtrvEOF()*/ !dbEOF() && id_evento == event_e_ticket.id_evento && rta ){
			
			//SELECT( AREA_COD_BARRA );
			//SET_ORDER( 3 );
			//indice.posicion = event_e_ticket.posicion;
			//GET_EQUAL( (char*)&indice );
			SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
			SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION='%d' ", caja_z, id_evento, event_e_ticket.posicion );
			RUN_QUERY(NO);
			
			if( FOUND2( ) ){
				memset( (char*)&datos_cobro, 0, sizeof( struct _datos_cobro ) );
				strncpy( datos_cobro.cod_barra, event_cod_barra.codigo_barra, sizeof( datos_cobro.cod_barra) -1 );
				datos_cobro.codigo = event_cod_barra.cuenta;
				EVALUAR_MASCARA_COD_BARRA( datos_cobro.codigo, datos_cobro.cod_barra, &datos_cobro );
				strncpy( datos_cobro.cuenta, event_cod_barra.nro_cuenta_ext, sizeof( datos_cobro.cuenta ) - 1 );
				datos_cobro.importe = event_cod_barra.importe;
				datos_cobro.comprobante = event_cod_barra.nro_comprobante_new;
				datos_cobro.devolucion = (char)devolucion;
	
				//SELECT( AREA_AUX4 );
				SELECT_TABLE( T_COBROS, TT_ORIG );
				//SET_ORDER( 1 );
				//GET_EQUAL( ( char* )&datos_cobro.codigo );
				SET_WHERE("CODIGO='%d'", datos_cobro.codigo );
				RUN_QUERY(NO);
				
				datos_cobro.tarjeta = cobros.tarjeta;
				datos_cobro.hab_todos_medios = cobros.hab_todos_medios;
				datos_cobro.hab_todos_medios_dev = cobros.hab_todos_medios_dev;
				
				//SELECT( AREA_DPAGO );
				//SET_ORDER( 2 );
				//GET_EQUAL( (char*)&indice );
				//(Nota Unific MySQL-Linux): ORDER 2 Esta compuesto por 2 campos, no 3 como el indice.
				SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
				SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' ", caja_z, id_evento );
				RUN_QUERY(NO);
				
				while( /*!BtrvEOF()*/ !dbEOF() && id_evento == ev_dpago.id_evento ){
					if( event_cod_barra.posicion_medio == ev_dpago.pos_pago ){
						strncpy( datos_cobro.dato_adicional, ev_dpago.valor, 
							sizeof( datos_cobro.dato_adicional ) );
						datos_cobro.dato_adicional[30] = 0;
						break;
					}else{
						SKIP2( 1 );
					}
				}

				if( event_e_ticket.cantidad > 0 ){
					switch( TIPO_COBRO_VENTA( datos_cobro.codigo ) ){
						case COBRO_NORMAL:
							GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA,
								(char*)&datos_cobro, sizeof( struct _datos_cobro ) );
							break;
						case COBRO_GARANTIA:
							//SELECT( AREA_COD_BARRA );
							//SKIP( 1 );
							SELECT_TABLE( T_EV_COD_BARRA, TT_ORIG );
							SKIP2( 1 );
							strncpy( datos_cobro.cod_barra_articulo, 
								event_cod_barra.codigo_barra, sizeof( datos_cobro.cod_barra_articulo ) );
							GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_GARANTIA,
								(char*)&datos_cobro, sizeof( struct _datos_cobro ) );
							break;
						default:
							break;
					}
				}else{

					CANCELAR_COBRO_VENTA( (void*)&datos_cobro, TIPO_COBRO_VENTA( datos_cobro.codigo ) );
				}
			}else if( event_e_ticket.cantidad <= 0 && event_e_ticket.cod_cobro ){
					memset( (char*)&datos_cobro, 0, sizeof( struct _datos_cobro ) );
					datos_cobro.codigo = event_e_ticket.cod_cobro;
					datos_cobro.importe = event_e_ticket.total;
					CANCELAR_COBRO_VENTA( (void*)&datos_cobro, TIPO_COBRO_VENTA( datos_cobro.codigo ) );
			}
			//SELECT( AREA_E_TICKET );
			SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
			SKIP2( 1 );
		}
	}
	//CLOSE_DATABASE( AREA_DPAGO );
	//CLOSE_DATABASE( AREA_AUX4 );
	//SELECT( area_ant );
	CLOSE_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
	CLOSE_TABLE( T_COBROS, TT_ORIG );
	SELECT_TABLE( tabla_ant, tipo_ant );	
	
	return rta;
}

/******************************************************************************************/
int COBROS_VENTA_EN_TICKET( int *cobro_on , int *unico)
/******************************************************************************************/
{
    int rta = 0, cobro_online = 0, h = 0, i, signo = 0, cob_unico = 0, cod_cobro = 0;
	double count = 0.0;
    //cobro_on_tick = 0;

	*cobro_on = -1;
	*unico = -1;

    for( h = 0; h < RAM_P_TICKET; h++ ) { 
        cobro_online = 0;
		//Caso donde ya se tickeo 1 articulo y el proximo es un cobro unico. Analizo el item actual 
		cod_cobro = ( RAM_P_TICKET == 1 && !_TICKET_COD_COBRO( h ) ) ? 
				ITEM_COD_COBRO: _TICKET_COD_COBRO( h );

		if( cod_cobro ) { 
            if( COBROS_VENTA( cod_cobro, &cobro_online , &cob_unico ) ) {
				count = 0;
				for( i = 0; i < RAM_P_TICKET; i++ ){
					if( _TICKET_COD_COBRO( i ) > 0 
							&& _TICKET_COD_COBRO( i ) == _TICKET_COD_COBRO( h )
							&& _TICKET_PRECIO( i ) == _TICKET_PRECIO( h ) ){
						signo = ( _TICKET_TIPO( i ) & _RESTA ) ? -1 : 1;
						count += _TICKET_CANTIDAD( i ) * signo;
					}
				}
				if( RAM_P_TICKET == 1 && !_TICKET_COD_COBRO( h ) ) {
					count += RAM_CANTIDAD;
				}
				if( count > 0 ){
					rta ++;//Cobros Marginales
					if( cobro_online ) {
						*cobro_on =  h;//*cobro_on + 1 ;//Cobros Marginales OnLine 
					}
					if( cob_unico ){
						*unico = h;//1;
					}
				}
			}
		}
    }
    
    return ( rta );
}

/*****************************************************************************/
int COBROS_VENTA( int codigo, int *cobro_online, int *unico)
/*****************************************************************************/
{
	int	rta = 0/*, area_ant = 0*/;

   //---------------------------- Abre el archivo ---------------------------//
    //area_ant = SELECTED();
    int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
    
    //if( USE_DB( AREA_COBROS, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 ) == 0 ) {        
		//GET_EQUAL( ( char* )&codigo );

	if( OPEN_TABLE( T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( struct _cobros )) == 0 ) {
		SET_WHERE("CODIGO='%d'", codigo );
		RUN_QUERY(NO);
		if( FOUND2(  ) ) {
            if( cobros.venta == SI ) {
				rta = 1;//Cobro Marginale
			}
            if( cobros.online ) {
                *cobro_online = 1;//Cobro Marginal OnLine
            }
			if( cobros.cobro_unico ) {
				*unico = 1;
			}
		}
		//CLOSE_DATABASE( AREA_COBROS );
		CLOSE_TABLE( T_COBROS, TT_ORIG );
	}
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );

	return ( rta );
}

/************************************************************************************/
void GENERAR_REQ_MEDIOS( )
/************************************************************************************/
{
	int i;
	for( i = 0; i < RAM_P_TICKET; i++){
		GRABAR_MEDIOS_COBRO( _TICKET_COD_COBRO(i), _TICKET_PRECIO(i) );
	}
}

/***************************************************************************************/
int CANCELAR_COBRO_VENTA( void *struct_datos, int tipo_cobro )
/***************************************************************************************/
{
	int rta = 0;
	struct _datos_cobro dc;
	struct _datos_cobro_kinacu dck;
	switch( tipo_cobro ){
		case COBRO_NORMAL:
			for( indice_cobro_venta = 0; 
				indice_cobro_venta < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA, sizeof( struct _datos_cobro ) ) && !rta;
				indice_cobro_venta++ )
			{
				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA, ( char* )&dc,
					sizeof( struct _datos_cobro ), indice_cobro_venta );
				if( dc.codigo == ((struct _datos_cobro*)struct_datos)->codigo 
						&& ( fabs( ((struct _datos_cobro*)struct_datos)->importe - dc.importe ) < 0.0001 )
						&& ( !strncmp( dc.cod_barra, ((struct _datos_cobro*)struct_datos)->cod_barra, sizeof( dc.cod_barra ) ) 
							|| !strlen( ((struct _datos_cobro*)struct_datos)->cod_barra ) ) ){
					memset( (char*)&dc, 0, sizeof( struct _datos_cobro ) );
					dc.estado = 1;
					ACTUALIZAR_ARCHIVO_REC(ARCH_REC_COBROS_VENTA, ( char* )&dc,
						sizeof( struct _datos_cobro ), indice_cobro_venta );
					
					if( RAM_NRO_PEDIDO_PIN ) {
						int found = 0;
						int posicionTransaccionKinacu = 0;
						while( !found && RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&dck, 
								sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {
							if( strcmp( dck.cod_barra, ((struct _datos_cobro*)struct_datos)->cod_barra_art_venta ) == 0  
								&& ( dck.cod_interno == ((struct _datos_cobro*)struct_datos)->cod_interno_art_venta )
								&& ( fabs( ((struct _datos_cobro*)struct_datos)->importe - dck.monto ) < 0.0001 ) 
								&& dck.estado == 1 ) {//estado = 1 -> cancelar
								memset( (char*)&dck, 0, sizeof( struct _datos_cobro_kinacu ) );
								dck.estado = 1;
								ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU, ( char * )&dck,
										sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu );
								found = 1;
								ADD_MEMORY_LONG( __ram_nro_pedido_pin, - 1L );
							}
							posicionTransaccionKinacu ++;
						}
					}
					rta = 1;
					break;
				}
			}
			break;	
		case COBRO_PAGATODO:
			BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO );
			rta = 1;
			break;
		case COBRO_GARANTIA:
			for( indice_cobro_venta = 0; 
				indice_cobro_venta < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA_GARANTIA, sizeof( struct _datos_cobro ) );
				indice_cobro_venta++ )
			{
				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_GARANTIA, ( char* )&dc,
					sizeof( struct _datos_cobro ), indice_cobro_venta );
				if( dc.codigo == ((struct _datos_cobro*)struct_datos)->codigo 
						&& ((struct _datos_cobro*)struct_datos)->importe == dc.importe 
						&& ( !strncmp( dc.cod_barra, ((struct _datos_cobro*)struct_datos)->cod_barra, sizeof( dc.cod_barra ) ) 
							|| !strlen( ((struct _datos_cobro*)struct_datos)->cod_barra ) )
						&& ( !strncmp( dc.cod_barra_articulo, ((struct _datos_cobro*)struct_datos)->cod_barra_articulo, sizeof( dc.cod_barra_articulo ) ) 
							|| !strlen( ((struct _datos_cobro*)struct_datos)->cod_barra_articulo ) ) ){
					memset( (char*)&dc, 0, sizeof( struct _datos_cobro ) );
					dc.estado = 1;
					ACTUALIZAR_ARCHIVO_REC(ARCH_REC_COBROS_VENTA_GARANTIA, ( char* )&dc,
						sizeof( struct _datos_cobro ), indice_cobro_venta );
					rta = 1;
					break;
				}
			}
			break;	
		default:
			break;
	}

	BORRAR_ARCHIVOS_COBROS_VENTA( SI );

	return rta;
}

/***************************************************************************************/
void BORRAR_ARCHIVOS_COBROS_VENTA( int verificar )
/***************************************************************************************/
{
	struct _datos_cobro dc;
	int cant_cancelado = 0;

	for( indice_cobro_venta = 0; 
			indice_cobro_venta < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA, sizeof( struct _datos_cobro ) ) && verificar;
			indice_cobro_venta++ )
	{
		//Borrar archivo en caso de que no haya ningun cobro.
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA, ( char* )&dc,
					sizeof( struct _datos_cobro ), indice_cobro_venta );
		if( dc.estado == 1 ) {//Cancelado
			cant_cancelado ++;
		}
	}
	if( indice_cobro_venta == cant_cancelado ) {
		BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA );
		BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU );
		BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_GARANTIA );
		BORRAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO );
		BORRAR_ARCHIVO_REC( ARCH_REC_REQUERIR_MEDIO );
	}
	SET_MEMORY_CHAR( __ram_hay_cobro_venta, 0 );//por anulacio o espera quedan con basura y provoca caida.
}

/*****************************************************************************/
void CEREAR_TABLA_LIN_MENS_TMP( void )
/*****************************************************************************/
{
    int error = 0;

    //int area_ant = SELECTED();
    //int a_aux = AREA_DISPONIBLE(  );
    struct _lin_mens lin_mens;

    //error = USE_DB( a_aux, _LIN_MENS_TMP, ( char * ) &lin_mens, sizeof( struct _lin_mens ), NULL, 0, 0 );
    error = OPEN_TABLE( T_LIN_MENS_TMP, TT_ORIG, ( char * ) &lin_mens, sizeof( struct _lin_mens ) );
    if( !error ) {
        ZAP2();
    }
    //CLOSE_DATABASE( a_aux  );
    CLOSE_TABLE( T_LIN_MENS_TMP, TT_ORIG );

}
