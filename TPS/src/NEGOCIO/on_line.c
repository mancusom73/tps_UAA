/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <config.h>
#include <stdlib.h>
#include <math.h>
#include <netcom.h>
#include <string.h>
#include <aarch.h>
#include <cmemory.h>
#include <b_mens.h>
#include <tarjetas.h>
#include <on_line.h>
#include <pago_tar.h>
#include <lista.h>
#include <on_line.h>
#include <envios.h>
#include <cupones.h>
#include <_cr1.h>
#include <perfiles.h>
#include <netcom2.h>
#include <pagos.h>
#include <round.h>
#include <cadafech.h>
#include <DB.h>
#include <DefTables.h>
#include <path.h>
#include <ini.h>
#define _NUNCA                0
#define _SIEMPRE              1
#define _SOLO_SI_PASA_LIMITE  2
extern int calcular_nro_cupon_en_netcom2;
//static struct _datos {
//  char      valor[31];
//  char      boletin_consultado;   // 0 Ninguno, 1 Local, 2 Remoto
//  int       cod_supervisor;
//};
extern int PAGOS_CONSTRUYE_MASCARA( struct _dmedios *datos_medio, char *mascara_in, int tam_mascara,
                                    char *mascara, int cant_datos, double importe,
                                    struct _datos *datos );
extern int ENVIAR_TRANSACCION( int nodo, int operacion, double importe, long comprobante, long *nro,
                               long *autorizacion, long anulacion );
extern int PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos );
int ON_LINE_MEDIO( struct _olmedios *olmedios, int h, int tipo_operacion_on,
                   int tipo_operacion_off );
int ON_LINE_MEDIO_PREPRARAR_DATOS( int tipo_operacion_on, int tipo_operacion_off,
                                   struct _olmedios *olmedios, struct _dmedios *datos_medio,
                                   int cant_datos, int h, struct _datos *datos, double importe );
int _ON_LINE_MEDIO( int tipo_operacion_on, int tipo_operacion_off, double importe, int nodo );
int OL_MEDIO_OP_HABILITADA( int cod_op, char *mask_op );
extern int TARJETA_DEBITO ;
int TARJETA_APROBADA_ONLINE(int posicionPago );
void MENSAJE_TECLA( char *cadena );
double CASHPLUS_INGRESA_IMPORTE(  );
/********************************************************************************/
int PEDIR_AUTORIZACION_ON_LINE( char origen, int dev_cobro )
/********************************************************************************/
{
    int     ok = 1, h, posicion = 0, envio_medios = -1;
    long    envio_cta_cte = -1;
    struct  _olmedios olmedios;
	char    cantidad_tarjetas = 0;
	int     Es_lamismatarjeta = NO;
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        return ok;
    }
    #endif
    /*------------------ Envia transacciones de Cta Cte --------------------*/
    if( CTA_CTE_ON_LINE ) {
        for( h = 0;h < RAM_P_PAGO && ok;h++ ) {
            if( medios[( int )_PAGO_MODO( h )].cta_cte && CTA_CTE_ON_LINE ) {
                ok = ON_LINE_CTA_CTE( h, &envio_cta_cte );
                if( !ok ) {
                    envio_cta_cte = -1;
                }
                BORRAR_MENSAJE();
            }
        }
    }
    if( ok && MEDIOS_DE_PAGO_ONLINE ) {
        COPY_STRUCT( _var_tarjetas_ant, _var_tarjetas );
        CEREAR_STRUCT( _var_tarjetas );
        /*if( USE_DB( AREA_AUX6, _OLMEDIOS_SIC, ( char* )&olmedios, sizeof( struct _olmedios ), NULL,
                    0, 0 ) == 0 ) {*/
		if( OPEN_TABLE( T_OLMEDIOS, TT_ORIG, ( char* )&olmedios,
				sizeof( struct _olmedios))== 0 ) {

            for( h = 0;h < RAM_P_PAGO && ok;h++ ) {
                ok = ON_LINE_MEDIO( &olmedios, h,
                                    ( MODO_DEVOLUCION || RAM_NOTA_CR ) ? _ON_LINE_DEVOLUCION
                                  : _ON_LINE_COMPRA,
                                    ( MODO_DEVOLUCION || RAM_NOTA_CR ) ? _OFF_LINE_DEVOLUCION
                                  : _OFF_LINE_COMPRA );
                if( ok == 1 ) {
                    envio_medios = h;
                }
            }
            if( !ok && envio_medios != -1 ) {
                for( h = 0;h <= envio_medios;h++ ) {
                    ON_LINE_MEDIO( &olmedios, h,
                                   ( MODO_DEVOLUCION || RAM_NOTA_CR )
                                 ? _ON_LINE_ANULACION_DEVOLUCION : _ON_LINE_ANULACION,
                                   ( MODO_DEVOLUCION || RAM_NOTA_CR )
                                 ? _OFF_LINE_ANULACION_DEVOLUCION : _OFF_LINE_ANULACION );
                }
            }
            //CLOSE_DATABASE( AREA_AUX6 );
			CLOSE_TABLE( T_OLMEDIOS, TT_ORIG );
        }
        else {
            MENSAJE_STRING( S_ERROR_AL_ABRIR_CONFIGURACION_ON_LINE );
            ok = 0;
        }
        COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
    }
    /*------------------ Envia transacciones de Tarjetas -------------------*/
	if( origen ) {
		cantidad_tarjetas = origen;
	} else {
	    cantidad_tarjetas = RAM_P_PAGO;
	}
	TARJETA_DEBITO= 0;
	for( h = 0;h < cantidad_tarjetas && ok; h++ ) {

		//solo se procesan tarjetas tarjetas
		if( medios[( int )_PAGO_MODO( h )].tarjeta ){
/************************************************************************************************************/
		/*
		la modificacion esta realizada para que solo anule o frene las autorizaciones 
		si encuentra una tarjeta no autorizada, si se encontraba una previamente autorizada no se anula, 
		se verifica en el posterior pedido de autorizaciones
		*/
		int estabaAprobada = NO;
		int versionAnulacionCompleta = SI;
		//char *auxiliar = NULL, *str2 = "      ", *ptr;

		//auxiliar=malloc(SIZEOF_VAR( __pago_auto_tarjeta )+2);
		
		/*if( auxiliar!= NULL ){
			versionAnulacionCompleta = NO;
			memset( auxiliar,0,(SIZEOF_VAR( __pago_auto_tarjeta )+2));
			memcpy( auxiliar , _PAGO_AUTO_TARJETA( h ),SIZEOF_VAR( __pago_auto_tarjeta ) );
			strlwr( auxiliar );
			// Sólo las tarjeta aprobadas tiene codigo de autorización..
			ptr = strstr(auxiliar, str2);
			if( !ptr )
				estabaAprobada= SI;
		}*/
		estabaAprobada =  TARJETA_APROBADA_ONLINE( h );
		if( estabaAprobada < 0 ){
			//quiere decir que no se pudo chequera cual, entonces realizo el modo tradicional.
			versionAnulacionCompleta = SI;
			estabaAprobada = 0;
		}else
			versionAnulacionCompleta = NO;
		

/************************************************************************************************************/
		if( //VERSION NUEVA SOLO LAS QUE ESTAN SIN APROBAR, SINO TODAS
			 estabaAprobada == NO ||versionAnulacionCompleta == SI 
			) {

            if( VARIAS_TARJETAS_POR_PAGO ) {
                ok = LEE_TARJETAS( posicion, TARJETA_TRADICIONAL );
            }
            if( ok && EXIGE_AUTORIZACION_ON_LINE() ) {
                char num_tarj[21];
                int opcion = 0;
                double importe;
                char cuotas;
				Es_lamismatarjeta = NO;
                strncpy( num_tarj, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/, 21 );
                num_tarj[20] = 0;
                importe = _PAGO_IMPORTE( h );
                cuotas = VAR_TARJETAS_CUOTAS;
                if( strcmp( RAM_TARJ_NUMERO, num_tarj ) == 0 && RAM_TARJ_IMPORTE == importe
                  && RAM_TARJ_CUOTAS == cuotas && config.pais != ECUADOR)
				   Es_lamismatarjeta = SI;

                if( Es_lamismatarjeta == SI
                 && !VAR_TARJETAS_UNICAMENTE_OL ) {
                    MENSAJE_STRING( S_1_AUTORIZACION_OFF_LINE_2_ENVIAR_TRANSACCION_ON_LINE );
                    while( !opcion ) {
                        char c = ( char )GETCH();
                        if( c == '1' ) {
                            opcion = 1;
                        }
                        else if( c == '2' ) {
                            opcion = 2;
                        }
                    }
                }
                else {
                    SET_MEMORY( __ram_tarj_numero, num_tarj );
                    SET_MEMORY_DOUBLE( __ram_tarj_importe, importe );
                    SET_MEMORY_CHAR( __ram_tarj_cuotas, cuotas );
                }
                if( ( opcion != 2 )
					&& ( opcion == 1 || ( RAM_ENVIAR_OFFLINE && !VAR_TARJETAS_UNICAMENTE_OL ) ) ) {
                    ok = COMPRA_OFF_LINE( h, SI, dev_cobro );
                }
				else {
                    if( ( opcion == 2 )
                     || ( VAR_TARJETAS_UNICAMENTE_OL || EN_CUOTAS()
                       || TARJETA_ON_LINE( VAR_TARJETAS_TARJETA ) == _SIEMPRE
					   || SUPERA_LIMITE_LIBERTAD( h ) ) ) {
						   //va lo de cashplus
						   if(  medios[( int )_PAGO_MODO( h )].tarjeta 
							   && config_tps.ActivarCashPlus == 1
							   && TARJ[VAR_TARJETAS_TARJETA].tipo_tarjeta & _TIPO_CASH_ADVANCE 
							   && IsCashActivo() 
							   && !RAM_VENTA_FRACCION ) { //aqui no pedir si estoy en cambio de medios sino copiar directamente
								   double rtaa =0.00;
								   if( RAM_MODO_EJECUCION != CAMBIO_MEDIO) {
										rtaa = CASHPLUS_INGRESA_IMPORTE(); //a modo de prueba
										if(rtaa > 0.00) {
											SET_MEMORY_FLOAT( __var_tarjetas_anticipo, (float) rtaa );											
											SET_MEMORY_CHAR( __ram_venta_fraccion, 1 );//Variable reutilizada como bandera para activar el retiro de tarjeta
										}
								   }

						   }
                        ok = ON_LINE( h, dev_cobro, posicion );
                        GRABAR_RESULTADO_ON_LINE( ok );
					}
                    else {
                        ok = GRABAR_OFF_LINE( ( MODO_DEVOLUCION || RAM_NOTA_CR || dev_cobro )
                                            ? _OFF_LINE_DEVOLUCION : _OFF_LINE_COMPRA,
                                              _PAGO_IMPORTE( h ) );
                        GRABAR_RESULTADO_ON_LINE( ok );
                    }
                }
                BORRAR_MENSAJE();
            }
            if( PROTOCOLO_AUTORIZACION_TARJETA == _TCP_IP && ok ) {
                _SET_MEMORY_LONG( __pago_nro_cupon_int1, h, VAR_TARJETAS_NRO_CUPON_INT );
            }
            _SET_MEMORY_CHAR( __pago_status_tarjeta, h, CUPON_CALCULAR_STATUS() );
			
			//LA VERSION NUEVA NO DEBE ANULAR SINO ESTA AUTORIZADA, Y SI ESTA AUTORIZADA LA DEJA ASI.
			//SINO ESTA MODIFICADO PARA QUE  USE EL SISTEMA TRADICIONAL Y ANULE TODAS
            if( ( versionAnulacionCompleta == SI ) && !ok && VARIAS_TARJETAS_POR_PAGO ) //No pudo enviar alguna tarjeta
            {
                _ANULAR_TARJETAS( h - 1, NO,-1 );//TRADICIONAL AL PEDIR AUTORIZACION, REVERSA TODAS
            }
			if( ok ) {
				GRABAR_TARJETAS_REC( h, posicion );
            }
            if( ok && VARIAS_TARJETAS_POR_PAGO ) {
                GRABA_MODIFICACION_TARJETAS( posicion, TARJETA_TRADICIONAL );
            }
			
		}
		
		//navega por cada tarjeta este aprobada o no!!!!!
		posicion++;
		}	
    }
	//borrar el achivo de pines de tarjetas
	if( cantidad_tarjetas > 0 && ok !=0)
		unlink( _ARCH_PWK );
    #ifdef COMPILAR_CLIENTE_AFINIDAD_ON_LINE
    /*---------- Cliente Afinidad ON-LINE --------*/
    if( ok && RAM_CLIENTE_AFINIDAD && COMPRA_AFINIDAD_ON_LINE ) {
        ENVIAR_PAQUETE_AFINIDAD( _ON_LINE_COMPRA, RAM_CLIENTE_AFINIDAD, RAM_ACU_PUNTOS );
    }
    #endif
    if( !ok && envio_medios != -1 ) {
        COPY_STRUCT( _var_tarjetas_ant, _var_tarjetas );
        CEREAR_STRUCT( _var_tarjetas );
        /*if( USE_DB( AREA_AUX6, _OLMEDIOS_SIC, ( char* )&olmedios, sizeof( struct _olmedios ), NULL,
                    0, 0 ) == 0 ) {*/
		if( OPEN_TABLE(T_OLMEDIOS, TT_ORIG, ( char* )&olmedios,
				sizeof( struct _olmedios))== 0 ) {
            for( h = 0;h < RAM_P_PAGO;h++ ) {
                ON_LINE_MEDIO( &olmedios, h,
                               ( MODO_DEVOLUCION || RAM_NOTA_CR ) ? _ON_LINE_ANULACION_DEVOLUCION
                             : _ON_LINE_ANULACION,
                               ( MODO_DEVOLUCION || RAM_NOTA_CR ) ? _OFF_LINE_ANULACION_DEVOLUCION
                             : _OFF_LINE_ANULACION );
            }
            //CLOSE_DATABASE( AREA_AUX6 );
			CLOSE_TABLE( T_OLMEDIOS, TT_ORIG );
        }
        else {
            MENSAJE_STRING( S_ERROR_AL_ABRIR_CONFIGURACION_ON_LINE );
            ok = 0;
        }
        COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
    }
    /*-------------- Reversa la ctacte si no autorizo ------------*/
    if( !ok && envio_cta_cte != -1 ) {
        REVERSAR_TRANSACCION( 9, envio_cta_cte );
    }
    TARJETA_DEBITO= 0 ;
    return ( ok );
}
/********************************************************************************/
int ON_LINE_CTA_CTE( int h, long *nro )
/********************************************************************************/
{
    int ok = 0;
    //long autorizacion;
	//Se comenta provisoriamente hasta verificar si se continua utilizando o no
    /*ok = ENVIAR_TRANSACCION( NODO_CTA_CTE, Compra, _PAGO_IMPORTE( h ), NRO_TICKET, nro,
                             &autorizacion, 0L );
    if( ok ) {
        _SET_MEMORY_LONG( __pago_buffer1_long, h, autorizacion );
    }*/

    return ( ok );
}
//Se comenta porque no se esta usando la funcion
/********************************************************************************/
//int ON_LINE_TARJETA( int h, long *nro, long anulacion )
/********************************************************************************/
/*{
    int ok = 1, operacion;
    long autorizacion;
    operacion = Compra;
    if( RAM_CAMBIO ) {
        operacion = Cambio;
    }
    if( RAM_NOTA_CR ) {
        operacion = Devolucion;
    }
    if( anulacion ) {
        operacion = Anulacion;
    }
    ok = ENVIAR_TRANSACCION( NODO_TARJETA( VAR_TARJETAS_TARJETA ), operacion, _PAGO_IMPORTE( h ),
                             VAR_TARJETAS_NRO_CUPON_INT, nro, &autorizacion, anulacion );
    _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
    _SET_MEMORY_LONG( __pago_nro_cupon_int1, h, VAR_TARJETAS_NRO_CUPON_INT );
    return ( ok );
}
*/
/********************************************************************************/
int CONSULTA_ESTADO_TARJETA()
/********************************************************************************/
{
    int ok = 1;
    CR1_VALIDA_TARJETA( NO, 0 );
    // Esto debiera ser configurable
    /* if( VAR_TARJETAS_TARJETA == _CMR && VAR_TARJETAS_STATUS ) { ok =
     * _CONSULTA_ESTADO_TARJETA();
     * if( ok ) ACTUALIZA_NOM_PERFIL(); } else
     * SET_MEMORY_INT( __ram_perfil, 0 ); */
    SET_MEMORY_INT( __ram_perfil, 0 );
    MUESTRA_PERFIL();
    return ( ok );
}
//Se comenta porque no se esta usando la funcion
/********************************************************************************/
//int _CONSULTA_ESTADO_TARJETA()
/********************************************************************************/
/*{
    long nro,autorizacion;
    return ( ENVIAR_TRANSACCION( NODO_TARJETA( VAR_TARJETAS_TARJETA ), Consulta_de_Estado, 0, 0,
                                 &nro, &autorizacion, 0L ) );
}*/
//Se comenta porque no se esta usando la funcion
/********************************************************************************/
//int _CONSULTA_ESTADO_TARJETA_COBRO()
/********************************************************************************/
/*{
    long nro,autorizacion;
    return ( ENVIAR_TRANSACCION( NODO_TARJETA( VAR_TARJETAS_TARJETA ), Consulta_de_Estado_Cobro, 0,
                                 0, &nro, &autorizacion, 0L ) );
}*/
/********************************************************************************/
int ON_LINE_PAGO_TARJETA( double importe, int anulacion, int tipo_cobro_tarjeta )
/********************************************************************************/
{
    int ok = 1;
    struct _rta_terminal rta_terminal;
    memset(&rta_terminal, 0, sizeof(struct _rta_terminal));
    ok = ENVIAR_TRANSACCION_AL_AUT_ON( ( anulacion ) ? _ON_LINE_ANUL_COBRO : _ON_LINE_COBRO,
            importe, &rta_terminal, NODO_TARJETA( VAR_TARJETAS_TARJETA ) );
    if( !ok && tipo_cobro_tarjeta == T_NORMAL ) {
        ok = ENVIAR_TRANSACCION_AL_AUT_ON( ( anulacion ) ? _OFF_LINE_ANUL_COBRO : _OFF_LINE_COBRO,
                importe, &rta_terminal, NODO_TARJETA( VAR_TARJETAS_TARJETA ) );
        if( !ok ){
            strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),sizeof( rta_terminal.mensaje ) );
        }
        MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
	} else {
        if( ok ) {
			SET_MEMORY_FLOAT( __var_tarjetas_saldo_cliente, rta_terminal.saldo );
			SET_MEMORY_FLOAT( __var_tarjetas_importe_cuota, rta_terminal.cuota );
			SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal.autorizacion );
			SET_MEMORY_CHAR( __var_tarjetas_on_line, SI );
			SET_MEMORY( __var_tarjetas_mensaje, rta_terminal.mensaje );
			ok = ( rta_terminal.cod_rta == 0 ) ? 1 : 0;
        }else{
            strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),sizeof( rta_terminal.mensaje ) );
        }
        if( MOSTRAR_MENSAJE_EN_COBROS_ONLINE && !ok) {
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
        }
    }
    return ( ok );
}
//Se comenta porque no se esta usando la funcion
/********************************************************************************/
//int _CONSULTA_ESTADO_TARJETA_ADELANTO()
/********************************************************************************/
/*{
    long nro,autorizacion;
    return ( ENVIAR_TRANSACCION( NODO_TARJETA( VAR_TARJETAS_TARJETA ), Consulta_de_Estado_Adelanto,
                                 0, 0, &nro, &autorizacion, 0L ) );
}*/
/********************************************************************************/
int ON_LINE_ADELANTO_TARJETA( double importe )
/********************************************************************************/
{
    int ok = 1;
    // Esto debiera ser configurable
    /* if( VAR_TARJETAS_TARJETA == _CMR ) { ok = ENVIAR_TRANSACCION(
     * NODO_TARJETA(VAR_TARJETAS_TARJETA), Adelanto, importe, 0, &nro,
     * &autorizacion, 0L ); } */
    importe = 0;
    return ( ok );
}
/********************************************************************************/
int ON_LINE_MEDIO( struct _olmedios *olmedios, int h, int tipo_operacion_on,
                   int tipo_operacion_off )
/********************************************************************************/
{
    //  char         cod_auto[20];
    int ok = 2, cant_datos, cod_solicitud, cant, i;
    struct
    {
        int cod_medio;
        int cod_submedio;
    } rel_olmedios;
    double importe,total;
    struct _dmedios *datos_medio = NULL;
    struct _datos *datos = NULL;
	char mensaje[40];
    calcular_nro_cupon_en_netcom2 = NO;
    if( medios[_PAGO_MODO( h )].ticket_compra ) {
        rel_olmedios.cod_submedio = _PAGO_CUENTA( h );
    }
    else if( medios[_PAGO_MODO( h )].mutual ) {
        rel_olmedios.cod_submedio = _PAGO_CUENTA( h );
    }
    else {
        rel_olmedios.cod_submedio = 0;
    }
    cod_solicitud = _PAGO_COD_SOLICITUD( h );
    rel_olmedios.cod_medio = _PAGO_MODO( h ) + 1;
    SELECT( AREA_AUX6 );
    GET_EQUAL( ( char* )&rel_olmedios );
    if( FOUND() && olmedios->on_line_habilitado == 'S' 
       //02/12/2004-- fabiana -- se comenta para poder igualar la base de datos   
       //&& OL_MEDIO_OP_HABILITADA( tipo_operacion_on, olmedios->mask_operaciones_habilitadas ) 
      ) {
        ok = 0;
        cant_datos = PAGOS_CARGAR_DATOS_MEDIO( &datos_medio, cod_solicitud, D_MEDIOS );
        if( cant_datos > 0 ) {
            datos = calloc( CANT_DATOS_MEDIOS, sizeof( struct _datos ) );
            if( datos ) {
                cant = PAGOS_CANTIDAD_DATOS( h ) / cant_datos;          
                ok = 1;
                total = 0.0;
                for( i = 0 ;i < cant && ok ;i++ ) {
                    if( PAGOS_OBTENER_DATOS( h, i, datos ) ) {
                        if( i == cant - 1 ) {
                            importe = ROUND( _PAGO_IMPORTE( h ) - total, ENTEROS, DECIMALES );
                        }
                        else {
                            importe = ROUND( _PAGO_IMPORTE( h ) / cant, ENTEROS, DECIMALES );
                        }
                        total += importe;
                        ok = ON_LINE_MEDIO_PREPRARAR_DATOS( tipo_operacion_on, tipo_operacion_off,
                                                            olmedios, datos_medio, cant_datos, h,
                                                            datos, importe );
                        /*
                                    02/12/2004-- fabiana -- se comenta para poder igualar la base de datos   
                                    if( ok ) {
                                            if( olmedios->id_dato_autorizacion ) {
                                                strcpy( cod_auto, VAR_TARJETAS_AUTORIZACION );
                                                SET_DATO_ADICIONAL( h, i, 0, olmedios->id_dato_autorizacion, cod_auto );
                                            }
                                        }
                                    */
                    }
                    else {
                        ok = 0;
                        //MENSAJE_STRING( S_ERROR_AL_OBTENER_DATOS_MEDIOS );
						_snprintf( mensaje, sizeof( mensaje ),"%s, %s", ST( S_ERROR_AL_OBTENER_DATOS_MEDIOS ), ST( S_PRES_UNA_TECLA ) );
						mensaje[sizeof( mensaje )-1] = '\0';
						MENSAJE_TECLA( mensaje );
                    }
                }
            }
            else {
                //MENSAJE_STRING( S_NO_HAY_MEMORIA_SUFICIENTE_PARA_ON_LINE_MEDIOS );
				_snprintf( mensaje, sizeof( mensaje ),"%s, %s", ST( S_NO_HAY_MEMORIA_SUFICIENTE_PARA_ON_LINE_MEDIOS ), ST( S_PRES_UNA_TECLA ) );
				mensaje[sizeof( mensaje )-1] = '\0';
				MENSAJE_TECLA( mensaje );
            }
        }
        else {
            //MENSAJE_STRING( S_ERROR_AL_CARGAR_DATOS_MEDIOS );
			_snprintf( mensaje, sizeof( mensaje ),"%s, %s", ST( S_ERROR_AL_CARGAR_DATOS_MEDIOS ), ST( S_PRES_UNA_TECLA ) );
			mensaje[sizeof( mensaje )-1] = '\0';
			MENSAJE_TECLA( mensaje );
        }
        if( datos_medio ) {
            free( datos_medio );
        }
        if( datos ) {
            free( datos );
        }
    }
    calcular_nro_cupon_en_netcom2 = SI;
    return ( ok );
}
/********************************************************************************/
int ON_LINE_MEDIO_PREPRARAR_DATOS( int tipo_operacion_on, int tipo_operacion_off,
                                   struct _olmedios *olmedios, struct _dmedios *datos_medio,
                                   int cant_datos, int h, struct _datos *datos, double importe )
/********************************************************************************/
{
    char buffer[50],nro[40],aux[80];
    int ok = 0;
    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, olmedios->cod_marca );
    memset( buffer, 0, sizeof( buffer ) );
    PAGOS_CONSTRUYE_MASCARA( datos_medio, olmedios->mascara_track2,
                             sizeof( olmedios->mascara_track2 ), buffer, cant_datos, importe,
                             datos );
    memset( aux, 0, sizeof( aux ) );
    aux[0] = strlen( buffer ) + 1;
    memcpy( &aux[1], buffer, aux[0] );
    SET_MEMORY( __var_tarjetas_track2, aux );
    CADENA_NRO_TARJETA( buffer, nro, 0 );
    SET_NUMERO_TARJETA_PRIVADO( nro );//SET_MEMORY( __var_tarjetas_numero, nro );
    memset( buffer, 0, sizeof( buffer ) );
    PAGOS_CONSTRUYE_MASCARA( datos_medio, olmedios->mascara_comercio,
                             sizeof( olmedios->mascara_comercio ), buffer, cant_datos, importe,
                             datos );
    SET_MEMORY( __var_tarjetas_nro_comercio, buffer );
    memset( buffer, 0, sizeof( buffer ) );
    PAGOS_CONSTRUYE_MASCARA( datos_medio, olmedios->mascara_terminal,
                             sizeof( olmedios->mascara_terminal ), buffer, cant_datos, importe,
                             datos );
    SET_MEMORY_LONG( __var_tarjetas_terminal, atol( buffer ) );
    memset( buffer, 0, sizeof( buffer ) );
    PAGOS_CONSTRUYE_MASCARA( datos_medio, olmedios->mascara_fecha_vto,
                             sizeof( olmedios->mascara_fecha_vto ), buffer, cant_datos, importe,
                             datos );
    SET_FECHA_VTO_PRIVADA( CADENA_A_FECHA( buffer ) );
    memset( buffer, 0, sizeof( buffer ) );
    PAGOS_CONSTRUYE_MASCARA( datos_medio, olmedios->mascara_cuota,
                             sizeof( olmedios->mascara_cuota ), buffer, cant_datos, importe,
                             datos );
    SET_MEMORY_CHAR( __var_tarjetas_cuotas, atoi( buffer ) );
    SET_MEMORY_CHAR( __var_tarjetas_lectura_banda, 1 );
    if( olmedios->numerador_cupon < 20 ) {
        SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
                         _CALCULAR_NRO_CUPON( ( char )olmedios->numerador_cupon ) );
    }
    SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, ( olmedios->admite_offline ) ? 0 : 1 );
    //02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
    //if( !OL_MEDIO_OP_HABILITADA( tipo_operacion_off, olmedios->mask_operaciones_habilitadas ) ){
    //  tipo_operacion_off = -1;
    //}
    ok = _ON_LINE_MEDIO( tipo_operacion_on, tipo_operacion_off, importe, olmedios->nodo );
    if( ok ) {
        _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
    }
    return( ok );
}
/********************************************************************************/
int _ON_LINE_MEDIO( int tipo_operacion_on, int tipo_operacion_off, double importe, int nodo )
/********************************************************************************/
{
    int rta = 0;
    struct _rta_terminal rta_terminal;
    if( !ENVIAR_TRANSACCION_AL_AUT_ON( tipo_operacion_on, importe, &rta_terminal, nodo ) ) {
        rta_terminal.cod_rta = 1;
    }
    /*-------------- Procesa la respuesta ---------------*/
    switch( rta_terminal.cod_rta ) {
        case 0:
            /* Aprobada */
            rta_terminal.autorizacion[10] = 0;
            SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal.autorizacion );
            SET_MEMORY( __var_tarjetas_nro_ref, rta_terminal.nro_referencia );
            SET_MEMORY_INT( __var_tarjetas_lote, rta_terminal.lote );
            SET_MEMORY( __var_tarjetas_mensaje, rta_terminal.mensaje );
            SET_MEMORY_INT( __var_tarjetas_fecha_contable, rta_terminal.fecha_contable );
            SET_MEMORY_INT( __var_tarjetas_fecha_host, rta_terminal.fecha_host );
            SET_MEMORY_LONG( __var_tarjetas_hora_host, rta_terminal.hora_host );
            SET_MEMORY( __var_tarjetas_nro_cuenta, rta_terminal.nro_cuenta );
            SET_MEMORY_FLOAT( __var_tarjetas_tasa_aplicada, rta_terminal.tasa_aplicada );
            rta = 1;
            break;
        case 1:
            /* Error en la comunicacion */
            //strcpy(rta_terminal.mensaje, "ERROR DE COMUNICACION");
            strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),
                     sizeof( rta_terminal.mensaje ) );
        case 2:
            /* Denegada => Pedir autorizacion */
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            if( !VAR_TARJETAS_UNICAMENTE_OL && PIDE_AUTORIZACION_TARJETA( importe ) ) {
                if( tipo_operacion_off >= 0 ) {
                    rta = ENVIAR_TRANSACCION_AL_AUT_ON( tipo_operacion_off, importe, &rta_terminal,
                                                        nodo );
                }
                else {
                    rta = 1;
                }
            }
            break;
        case 3:
            /* Denegada => No puede comprar */
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            break;
    }
    return ( rta );
}
/********************************************************************************/
int OL_MEDIO_OP_HABILITADA( int cod_op, char *mask_op )
/********************************************************************************/
{
    int rta,byte,mask;
    byte = cod_op / 8;
    mask = 1 << ( cod_op % 8 );
    if( byte < 8 ) {
        rta = mask_op[7 - byte] & mask;
    }
    else {
        rta = 0;
    }
    return( rta );
}


/************************************************************************************************************/
int TARJETA_APROBADA_ONLINE(int posicionPago ){
/************************************************************************************************************/
	int rta= NO;
	if( medios[( int )_PAGO_MODO( posicionPago )].tarjeta ){
	/************************************************************************************************************/
			/*
			la modificacion esta realizada para que solo anule o frene las autorizaciones 
			si encuentra una tarjeta no autorizada, si se encontraba una previamente autorizada no se anula, 
			se verifica en el posterior pedido de autorizaciones
			*/
			
			char *auxiliar = NULL, *str2 = "      ", *ptr;

			auxiliar=malloc(SIZEOF_VAR( __pago_auto_tarjeta )+2);
			
			if( auxiliar!= NULL ){
				memset( auxiliar,0,(SIZEOF_VAR( __pago_auto_tarjeta )+2));
				memcpy( auxiliar , _PAGO_AUTO_TARJETA( posicionPago ),SIZEOF_VAR( __pago_auto_tarjeta ) );
				strlwr( auxiliar );
				// Sólo las tarjeta aprobadas tiene codigo de autorización..
				ptr = strstr(auxiliar, str2);
				//sino apunta no tiene datos, pero tambien debe controlar que tenga caracteres!!
				if(!ptr && strlen( auxiliar ) > 0 )
					rta= SI;
				}
				//esta esta aprobada, no esta ya anulada?
				{
					if( strcmp( auxiliar, "      " ) != 0 && strlen(auxiliar) > 1 && posicionPago>=0 && _PAGO_NRO_CUPON_INT1(posicionPago)> 0 ){
						char consulta[200];
						//// ticket_original = %li el ticket original no lo tiene bien. tengo que incluir el ticket anterior
						_snprintf(consulta,sizeof(consulta)-1,"SELECT * FROM transac2 WHERE id_evento = %li and codigo_de_procesamiento =20000 and importe = %lf  and enviado =1 and codigo_de_respuesta = 0 and autorizacion_original =\"%s\""
							,RAM_ID_EVENTO, _PAGO_IMPORTE(posicionPago), auxiliar );
							if(DIRECT_DB_QUERY(consulta) > 0){ //OBTIENE EL MAXIMO ID EVENTO
								rta= -2;
							}
					}
					
				}


				if( TARJETA_ON_LINE( VAR_TARJETAS_TARJETA ) == NO ) //ojo si es tarjeta offline, la autorizacion es del posnet y no debemos anularla
					rta= NO;
					
	}else {
				rta= -1;//no se puede hacer funcionamiento anterior.
	}
	return rta;
}