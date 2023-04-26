#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <afinidad.h> 
#include <asc.h> 
#include <baucher.h> 
#include <b_mens.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <comtar.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <cr_pant.h> 
#include <cupones.h> 
#include <dbrouter.h> 
#include <dec_mes.h> 
#include <dtoai.h> 
#include <envios.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <lista.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <netcom2.h> 
#include <pago_tar.h> 
#include <pant.h> 
#include <pausa.h> 
#include <pinpad.h> 
#include <print.h> 
#include <tarjetas.h> 
#include <tcp_ip.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <actions.h>
#ifdef INVEL_W
#include <io.h>
#include <ini.h>
#else
#include <sys/io.h>
#endif
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#else
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#endif
/*---------- Tipos de autorizacin ------------*/
#define _NUNCA                0
#define _SIEMPRE              1
#define _SOLO_SI_PASA_LIMITE  2
struct _rta_terminal_afinidad
{
    char cod_rta; //1
    char autorizacion[6]; //7
    char nombre[29]; //36
    UINT16 lote;
    float anticipo;
    float saldo;
    float cuota;
    char cuotas;
    UINT16 primer_vencimiento;
    char moneda;                             /* 0-Pesos , 1-Dolar  2-Cecor */
    char nro_referencia[12];
    char mensaje[37];
    char working_key[8];
		//char nousado[50];
	char nousado[8]; 
	char nro_cuenta[20]; //los 50 los reparto
	char nousado2[22];
};
extern long OPERACION_SOLICITADA_PARA_REVERSAR(int nro_cupon, int nodo);
extern int _OPEN_O_CREAT( char *nombre, int tipo, int linea, char *file );
extern int LSEEK( int handle, long pos, int tipo, int linea, char *file );
extern void LTRIM( char *cadena );
int _WRITE( int handle, char *buffer, int cant, int linea, char *file );
int _OPEN( char *nombre, int tipo, int linea, char *file );
int _READ( int handle, char *buffer, int cant, int linea, char *file );
extern void ABRIR_DOCUMENTO_NO_FISCAL( char slip );
extern void CERRAR_DOCUMENTO_NO_FISCAL();
//dnc
int PIDE_TARJETA_PARA_ANULAR( int *cancela_anula );
extern int ES_LA_MISMA_TARJETA(char * tarjetaOrig,char*tarjeta2);
int CARGA_VERIFICA_ANULA_TARJETAS(int handle, char *buffer,int h, int posiciontarjeta);
void MODIFICAR_VOUCHER_ENCOLADOS( void );
void BORRAR_VOUCHER_ENCOLADOS( int cant );
extern int VALIDAR_VOUCHER_TARJETA( int cod_formato, double importe, int h );
//
/********************************************************************************/
int EXIGE_AUTORIZACION( double importe )
/********************************************************************************/
{
    int rta = 0;
    if( importe > VAR_TARJETAS_LIMITE || RAM_SOLICITAR_AUTORIZACION ) {
        rta = 1;
    }
    if( VAR_TARJETAS_PEDIR_AUTO ) {
        rta = 1;
    }
    //----- Esto debiera ser configurable
    /* if( VAR_TARJETAS_TARJETA == _VISA && !VAR_TARJETAS_LECTURA_BANDA ) {
     * rta = 1; } */
    if( config.pais == ARGENTINA ) {
        if( EN_CUOTAS() ) {
            rta = 1;
        }
    }
    return ( rta );
}
/********************************************************************************/
int PIDE_AUTORIZACION_SI_NECESITA( int h, int pedir_aut )
/********************************************************************************/
{
    char aux[11];
    int ok = 1;
    if( pedir_aut || EXIGE_AUTORIZACION( _PAGO_IMPORTE( h ) ) ) {
        if( PIDE_AUTORIZACION_TARJETA( _PAGO_IMPORTE( h ) ) ) {
            _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
        }
        else {
            ok = 0;
        }
    }
    else {
        memset( aux, 0, sizeof( aux ) );
        NETCOM2_FORMATEAR_AUTORIZACION( aux, 1 );
        SET_MEMORY( __var_tarjetas_autorizacion, aux );
        SET_MEMORY_INT( __var_tarjetas_fecha_host, _GET_FECHA_DOS() );
        SET_MEMORY_LONG( __var_tarjetas_hora_host, GET_HORA_COMPLETA_DOS() );
    }
    return ( ok );
}
/********************************************************************************/
int EXIGE_AUTORIZACION_ON_LINE()
/********************************************************************************/
{
    int rta = 0;
    if( AUTORIZACION_ON_LINE_HABILITADA ) {
        rta = TARJETA_ON_LINE( VAR_TARJETAS_TARJETA );
        if( VAR_TARJETAS_UNICAMENTE_OL ) {
            rta = 1;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int ON_LINE( int h, int dev_cobro, int posipago )
/*****************************************************************************/
{
    int ok = 1, tarjeta = 0, i = 0;
    struct _rta_terminal rta_terminal;
	char marca[50], aux[50],*n, cadena[100], cad_aut[100], mensaje[40];
	double diff = 0.00, importe_caja= 0.0, importe_auton = 0.0;
    	
	memset(aux,0, sizeof(aux));
	memset(cadena,0, sizeof(cadena));
	memset(cad_aut,0, sizeof(cad_aut));
	memset(mensaje,0, sizeof(mensaje));
	memset(marca,0, sizeof(marca));
	memset(&rta_terminal,0, sizeof( struct _rta_terminal));
   

	if( AUTORIZACION_ON_LINE( ( MODO_DEVOLUCION || RAM_NOTA_CR || dev_cobro ) ? _ON_LINE_DEVOLUCION
                            : _ON_LINE_COMPRA, _PAGO_IMPORTE( h ), ( char* )&rta_terminal,
                              sizeof( rta_terminal ) ) ) {
       
		if(config_tps.NapseModalidad ==0) {
			#ifdef COMPILAR_PINPAD
			PINPAD_ACTUALIZAR_WORKING_KEY( VAR_TARJETAS_CELDA_PINPAD, rta_terminal.working_key );
			#endif
		}
    }
    else {
        /*------------- incrementa nro de cupon --------------*/
        if( PROTOCOLO_AUTORIZACION_TARJETA != _TCP_IP ) {
            SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
                             CALCULAR_NRO_CUPON( VAR_TARJETAS_TARJETA ) );
        }
        _SET_MEMORY_LONG( __pago_nro_cupon_int1, h, VAR_TARJETAS_NRO_CUPON_INT );
        CR1_MUESTRA_TARJETA();
        /*------------- pide autorizacion manual --------------*/
        rta_terminal.cod_rta = 1;
    }
	/*Verifico si se pago con mas de una tarjeta.
	  El mensaje solo se muestra cuando hay más de una tarjeta*/
	for( i = 0; i < RAM_P_PAGO; i++ ) {
		if( medios[( int )_PAGO_MODO( i )].tarjeta ) {
			tarjeta ++;
		}
	}
	if( tarjeta > 1 ) {
		strncpy( marca, TARJ[VAR_TARJETAS_TARJETA].nombre, sizeof( marca ) ); 
		n = GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/;
		memcpy( aux, &n[strlen( n ) - 4], 4 );
		aux[4] = 0;
		_snprintf( cad_aut, 100, ST( S_AUTORIZAR_TARJETA___S__ULTIMOS_4_DIG__S__ENTER ), marca, aux );
        cad_aut[99] = '\0';
		_snprintf( cadena, 100, ST( S_TARJETA___S__ULTIMOS_4_DIG__S__RECHAZADA_ENTER ), marca, aux );
		cadena[99] = '\0';
	}
    /*-------------- Procesa la respuesta ---------------*/
	CEREAR_MEMORY( __var_tarjetas_autorizacion );
	SET_MEMORY_CHAR( __var_tarjetas_datafono , 99);
    switch( rta_terminal.cod_rta ) {
        case 0:
            /* Aprobada */
            rta_terminal.autorizacion[10] = 0;
            SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal.autorizacion );
            _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
            SET_MEMORY_CHAR( __var_tarjetas_on_line, SI );
			{
				char auxil[13];
				
				 memset(auxil,0, sizeof(auxil));
				_snprintf(auxil, sizeof(auxil) -1 ,rta_terminal.nro_referencia);
				//rta_terminal.nro_referencia;
            //SET_MEMORY( __var_tarjetas_nro_ref, rta_terminal.nro_referencia ); //guarda aca que no pegue junto el mensaje y este campo
				SET_MEMORY( __var_tarjetas_nro_ref, auxil ); //guarda aca que no pegue junto el mensaje y este campo
				if(config_tps.NapseModalidad ==1 ){
					char nrotarj[21];
					memset(nrotarj,0,21);
					GET_NUMERO_CUENTA_TARJETA( nrotarj);
					SET_NUMERO_TARJETA_PRIVADO(nrotarj);
					_SET_MEMORY( __pago_nro_tarjeta, h, GET_NUMERO_TARJETA_PRIVADO( ));
				}
            //---- Esto debiera ser configurable
			}
            SET_MEMORY_INT( __var_tarjetas_lote, rta_terminal.lote );
            SET_MEMORY_CHAR( __var_tarjetas_aut_on_line, SI );
//			if( strcmp( rta_terminal.mensaje, "NO" ) != 0)
            SET_MEMORY( __var_tarjetas_mensaje, rta_terminal.mensaje );
            SET_MEMORY_FLOAT( __var_tarjetas_saldo_cliente, rta_terminal.saldo );
            if( rta_terminal.cuota
             && !( ( config.pais == CHILE || config.pais == BRASIL ) && RAM_NOTA_CR ) ) {
                SET_MEMORY_FLOAT( __var_tarjetas_importe_cuota, rta_terminal.cuota );
            }
            SET_MEMORY_INT( __var_tarjetas_fecha_contable, rta_terminal.fecha_contable );
            SET_MEMORY_INT( __var_tarjetas_fecha_host, rta_terminal.fecha_host );
            SET_MEMORY_LONG( __var_tarjetas_hora_host, rta_terminal.hora_host );
            SET_MEMORY( __var_tarjetas_nro_cuenta, rta_terminal.nro_cuenta );
            if( !RAM_NOTA_CR ) {
                SET_MEMORY_FLOAT( __var_tarjetas_tasa_aplicada, rta_terminal.tasa_aplicada );
            }
			//GRABAR_TARJETAS_REC(h);
			if(config_tps.ActivarCashPlus == 1 && VAR_TARJETAS_ANTICIPO > 0.005 && RAM_MODO_EJECUCION != CAMBIO_MEDIO){
				importe_caja= _PAGO_IMPORTE( h ) + VAR_TARJETAS_ANTICIPO ; 
			}else {
				importe_caja= _PAGO_IMPORTE( h ); 
			}
			importe_auton = rta_terminal.importe;
			diff = importe_caja - importe_auton ;
			if( diff < 0.01)
				diff = diff * -1;
            if( rta_terminal.importe > 0.001 
                    && ( diff >= 0.01) ) {
						if(config_tps.NapseModalidad == 0) { //ojo revisar si no afecta cashplus
							_SET_MEMORY_DOUBLE( __pago_importe_descuento, h, 
								_PAGO_IMPORTE( h ) - (rta_terminal.importe) );
						}
                _SET_MEMORY_DOUBLE( __pago_importe, h, rta_terminal.importe );             //xxx
            }
            break;
        case 1:
            /* Error en la comunicacion */
			if(config_tps.NapseModalidad ==1) {
				strncpy( rta_terminal.mensaje, "OPERACION CON ERROR DESDE PINPAD",
                     sizeof( rta_terminal.mensaje ) );
			}else {
            strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),
                     sizeof( rta_terminal.mensaje ) );
			}
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            if( VAR_TARJETAS_UNICAMENTE_OL ) {
                //MENSAJE_STRING( S_LA_OPERACION_SOLO_PUEDE_SER_ON_LINE ); mensaje muy largo al combinar con mensaje de tecla
				_snprintf( mensaje, sizeof( mensaje ),"%s, %s", ST( S_UNICAMENTE_ON_LINE ), ST( S_PRES_UNA_TECLA ) );
				mensaje[sizeof( mensaje )-1] = '\0';
				MENSAJE_TECLA( mensaje );
                ok = 0;
            }
            else {
                if( NUMERO_ARCHIVO( VAR_TARJETAS_TARJETA )> 0 && BUSCAR_BOLETIN == _SOLO_ERROR_COMUNICACION ) {
                    MENSAJE_SIN_SONIDO_STRING( S_BUSCANDO_EN_BOLETIN );
                    ok = BUSCAR_EN_BOLETIN( VAR_TARJETAS_TARJETA );
                    BORRAR_MENSAJE();
                }
                if( ok ) {
                    ok = COMPRA_OFF_LINE( h, NO, dev_cobro );
                }
            }
            break;
        case 2:
            /* Denegada => Pedir autorizacion */
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            ok = 0;
            if( !VAR_TARJETAS_UNICAMENTE_OL ) {
				if( tarjeta > 1 ) {
					MENSAJE_CON_TECLA( cad_aut, 13 );
				}
                if( PIDE_AUTORIZACION_TARJETA( _PAGO_IMPORTE( h ) ) ) {
                    _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
                    ok = GRABAR_OFF_LINE( ( MODO_DEVOLUCION || RAM_NOTA_CR || dev_cobro ) ? _OFF_LINE_DEVOLUCION
                                        : _OFF_LINE_COMPRA, _PAGO_IMPORTE( h ) );
                }
            }
            break;
        case 3:
            /* Denegada => No puede comprar */
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            ok = 0;
            break;
        case 4:
            /* Denegada => Sin saldo */
            if( EN_CUOTAS() || LIMITE2_TARJETA( VAR_TARJETAS_TARJETA ) == 0
             || VAR_TARJETAS_UNICAMENTE_OL ) {
                MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
                ok = 0;
            }
            else if( SUPERA_LIMITE_LIBERTAD( h ) ) {
                sprintf( rta_terminal.mensaje, ST( S_TARJETA_AUTORIZADA_HASTA__7_2F__ ),
                         LIMITE2_TARJETA( VAR_TARJETAS_TARJETA ) );
                MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
                ok = 0;
            }
            else {
                ok = GRABAR_OFF_LINE( ( MODO_DEVOLUCION || RAM_NOTA_CR || dev_cobro) ? _OFF_LINE_DEVOLUCION
                                    : _OFF_LINE_COMPRA, _PAGO_IMPORTE( h ) );
            }
            break;
        case 5:
            /* Denegada => Pedir autorizacion si presiona
             * una tecla */
            if( MOSTRAR_RESULTADO_ON_LINE( SI, rta_terminal.mensaje ) == 27 ) {
                ;
                ok = 0;
            }
            else {
                ok = 0;
                if( !VAR_TARJETAS_UNICAMENTE_OL ) {
                    if( tarjeta > 1 ) {
						MENSAJE_CON_TECLA( cad_aut, 13 );
					}
                    if( PIDE_AUTORIZACION_TARJETA( _PAGO_IMPORTE( h ) ) ) {
                        _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
                        ok = GRABAR_OFF_LINE( ( MODO_DEVOLUCION || RAM_NOTA_CR || dev_cobro )
                                            ? _OFF_LINE_DEVOLUCION : _OFF_LINE_COMPRA,
                                              _PAGO_IMPORTE( h ) );
                    }
                }
                else {
                    //MENSAJE_STRING( S_LA_OPERACION_SOLO_PUEDE_SER_ON_LINE );
					_snprintf( mensaje, sizeof( mensaje ),"%s, %s", ST( S_UNICAMENTE_ON_LINE ), ST( S_PRES_UNA_TECLA ) );
					mensaje[sizeof( mensaje )-1] = '\0';
					MENSAJE_TECLA( mensaje );
                }
            }
            break;
        case 6:
            /* Denegada => PIN invalido */
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            #ifdef COMPILAR_PINPAD
			SETEAR_DEBITO_POS( -1) ;
            ok =PIDE_CLAVE_EN_PINPAD( SI, SI, 0.0 );
            #endif
            ok = 0;
            break;
		case 7:{/* Denegada => Transacciones Pendientes para el Nodo solicitado por el Jauton */
			long operacionAux = 0;
				//el mensaje debe indicar que se pide reverso y que reintente la operacion realizada
				MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
				operacionAux = OPERACION_SOLICITADA_PARA_REVERSAR(rta_terminal.nro_ticket, 
						NODO_TARJETA( VAR_TARJETAS_TARJETA ));
				
				if( operacionAux > 0 )
					T__REVERSAR_TRANSACCION( operacionAux );
				ok = 0;
			 }
            break;
    }
	if( rta_terminal.cod_rta != 0 && rta_terminal.cod_rta != 1 && !ok && tarjeta > 1 ) {
		//MENSAJE_CON_TECLA( cadena, 13 );
		int k;
		ALERTA_EN_VENTANA(cadena,NULL,5,"tps - v" ,0);
		k = EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 

	}
	if( rta_terminal.cod_rta != 0 && rta_terminal.cod_rta != 1 ) {
		int k;
		_snprintf( mensaje, sizeof( mensaje ),"TARJETA Nro %i FUE RECHAZADA",posipago + 1);
		mensaje[sizeof( mensaje )-1] = '\0';
		MOSTRAR_MENSAJE_UBUNTU( 0, mensaje, 20 );
		
		ALERTA_EN_VENTANA("Recuerde anular el medio de pago con la tecla < - >( Presione el boton)" ,NULL,5,"tps - v" ,0);
		k = EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 
		
	}

	SET_MEMORY_CHAR( __var_tarjetas_datafono , rta_terminal.cod_rta);
    return ( ok );
}
/********************************************************************************/
int COMPRA_OFF_LINE( int h, int pedir_aut, int dev_cobro )
/********************************************************************************/
{
    int ok = 0;
    if( PIDE_AUTORIZACION_SI_NECESITA( h, pedir_aut ) ) {
        ok = GRABAR_OFF_LINE( ( MODO_DEVOLUCION || RAM_NOTA_CR || dev_cobro ) ? _OFF_LINE_DEVOLUCION
                            : _OFF_LINE_COMPRA, _PAGO_IMPORTE( h ) );
    }
    return ( ok );
}
/*****************************************************************************/
int MOSTRAR_RESULTADO_ON_LINE( int autoriza, char *mensaje )
/*****************************************************************************/
{
    int k = 0;
	char men[80];
	memset(men,0,sizeof(men));
    #if defined(INVEL_W) || defined(INVEL_L)   
    //MENSAJE( mensaje );
	if( autoriza ) {
        sprintf(men, "%s - %s", mensaje, ST( S__ESC__RECHAZA_O_ENTER__AUTORIZA_ ) );
		MENSAJE( men );
        k = GETCH();
        while( k != 27 && k != 13 ) {
            k = GETCH();
        }
    }
    else {
        _snprintf(men, sizeof(men)-1,"%s - %s", mensaje, " PRESIONE ACEPTAR" );
		MENSAJE( men );
		ALERTA_EN_VENTANA(men,NULL,5,"tps - v" ,0);
		k = EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 
		k=27;
        /*k = GETCH();
        while( k != 27 ) {
            k = GETCH();
        }*/
    }
    #else
    _ATRIBUTO = 15;
    MOSTRAR_CADENA( 1, 25, mensaje );
    if( autoriza ) {
        MOSTRAR_CADENA( 46 - ancho_panel / 2, 25, ST( S__ESC__RECHAZA_O_ENTER__AUTORIZA_ ) );        
        while( k != 27 && k != 13 ) {
            k = GETCH();
        }
    }
    else {
        MOSTRAR_CADENA( 64 - ancho_panel / 2, 25, ST( S__PRESIONE_ESC_ ) );        
        while( k != 27 ) {
            k = GETCH();
        }
    }
    #endif
    return ( k );
}
/*****************************************************************************/
void CAMBIA_TARJETA_CREDITO()
/*****************************************************************************/
{
    /*---- Guarda los datos de la tarjeta anterior y cerea la estructura -----*/
    COPY_STRUCT( _var_tarjetas_ant, _var_tarjetas );
    CEREAR_STRUCT( _var_tarjetas );
    /*--------------------- Pasa la nueva tarjeta --------------------*/
    if( VALIDA_TARJETA_NUEVA() ) {
        MENSAJE_STRING( S_TARJETA_OK );
        PAUSA( 9 );
    }
    else {
        /*------------- Recupera los datos de la tarjeta anterior -------------*/
        COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
        MENSAJE_STRING( S_CAMBIO_DE_TARJETA_NO_ACEPTADO );
    }
    BORRAR_MENSAJE();
}
/*****************************************************************************/
void GRABAR_RESULTADO_ON_LINE( int ok )
/*****************************************************************************/
{
    /*--------------- Graba resultado en el LOG -------------*/
    switch( ok ) {
        case 0:
            GRABAR_LOG_SISTEMA_STRING( S_AUTORIZACION_ON_LINE_ERROR, LOG_VENTAS,2 );
            break;
        case 1:
            GRABAR_LOG_SISTEMA_STRING( S_AUTORIZACION_ON_LINE_OK, LOG_VENTAS,2 );
            break;
        case 2:
            GRABAR_LOG_SISTEMA_STRING( S_AUTORIZACION_OFF_LINE_OK, LOG_VENTAS,2 );
            break;
    }
}
/*****************************************************************************/
int PIDE_AUTORIZACION_TARJETA( double importe )
/*****************************************************************************/
{
    int ok, rta = 0, len, alfa;
    char aux[11],aux2[11];
    long num;
    if( ( config.pais == CHILE || config.pais == BRASIL ) && RAM_NOTA_CR ) {
        rta = 1;
        SET_MEMORY( __var_tarjetas_autorizacion, VAR_TARJETAS_AUT_ORIGINAL );
    }
    else {
        VALIDACION_TELEFONICA( importe );
        ExecuteSSAction( A_PEDIR, 1, ST( S_AUTORIZACION ), " ", NULL, " " );
        len = ( config.pais == ECUADOR || config.pais == CHILE || config.pais == BRASIL  || config.pais == ARGENTINA ) ? 6 : 10;
        alfa = ( config.pais == ECUADOR || config.pais == CHILE || config.pais == BRASIL ) ? SI
             : NO;
        if( alfa ) {
            TECLADO_ALFABETICO = SI;
            ok = PEDIR_GRAN_CADENA( ( char )( 25 - ancho_panel / 2 ), ( char )22, ( char )len, aux,
                                    NULL );
            TECLADO_ALFABETICO = 0;
        }
        else {
			MENSAJE( "INGRESE COD. AUTORIZACION DE LA TARJETA DE CREDITO");
            ok = PEDIR_GRAN_CADENA_NUMERICA( 25 - ancho_panel / 2, 22, len, aux );
			BORRAR_MENSAJE();
        }
        memset( aux2, 0, sizeof( aux2 ) );
        memset( aux2, ' ', len );
        if( strcmp( aux, aux2 ) == 0 ) {
            ok = 0;
        }
        num = atol( aux );
        if( num == 0 && !alfa ) {
            MENSAJE_STRING( S_NUMERO_DE_AUTORIZACION_INVALIDO );
            ok = 0;
        }
        if( ok ) {
            NETCOM2_FORMATEAR_AUTORIZACION( aux, 0 );
        }
        ExecuteAction( A_FIN_PEDIR );
        if( ok ) {
            SET_MEMORY( __var_tarjetas_autorizacion, aux );
            SET_MEMORY_INT( __var_tarjetas_fecha_host, _GET_FECHA_DOS() );
            SET_MEMORY_LONG( __var_tarjetas_hora_host, GET_HORA_COMPLETA_DOS() );
            rta = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 19, NO, SI,
                                    PEDIR_CLAVE_AUTORIZACION_TARJETA, "INGRESAR AUTORIZACION",
                                    ST( S_INGRESO_N__AUTORIZACION ), _F_SUP_NRO_AUT_TARJETA, SI );
        }
    }
    return ( rta );
}
#ifdef COMPILAR_TECLA_REPROCESAR
/*****************************************************************************/
void REPROCESAR_TARJETAS( char tipo_pago, int actualizar )
/*****************************************************************************/
{
    int h, cant, posicion = 0;
    long anulado;
    struct _rta_terminal rta_terminal;
    for( h = 0, cant = 0;h < RAM_P_PAGO;h++ ) {
        if( medios[_PAGO_MODO( h )].tarjeta ) {
            if( VARIAS_TARJETAS_POR_PAGO ) {
                LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
            }
            anulado = VAR_TARJETAS_CUPON_ORIGINAL;//<-- este tiene el anterior _PAGO_NRO_CUPON_INT1( h )-->este es el comprobante actual
			
            if( PROTOCOLO_AUTORIZACION_TARJETA != _TCP_IP ) {
                SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
                                 CALCULAR_NRO_CUPON( VAR_TARJETAS_TARJETA ) );//<-- calcula el nuevo nro de cupon ! ! ! 
            }
            SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int, _PAGO_NRO_CUPON_INT1( h ) );
            SET_MEMORY_INT( __var_tarjetas_fecha_original, VAR_TARJETAS_FECHA_HOST );
            SET_MEMORY( __var_tarjetas_aut_original, VAR_TARJETAS_AUTORIZACION );
            SET_MEMORY_LONG( __var_tarjetas_hora_original, VAR_TARJETAS_HORA_HOST );
			if( !actualizar ) {
			   //SET_MEMORY_INT( __var_tarjetas_tipo_voucher, 1 ); 
			   ENCOLAR_VOUCHER_TARJETA(  );
			}
            /*-------------- Envia on-line o graba off-line -------------*/
            if( EXIGE_AUTORIZACION_ON_LINE() ) {
                if( _VAR_TARJETAS_NRO_REF( 0 ) ) {
                    /*---------------- Anulacin remota ------------------*/
                    if( !AUTORIZACION_ON_LINE( _ON_LINE_ANULACION, _PAGO_IMPORTE( h ),
                                               ( char* )&rta_terminal, sizeof( rta_terminal ) ) ) {
                        rta_terminal.cod_rta = 1;
                    }
                    if( rta_terminal.cod_rta == 0 ) {
                        SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal.autorizacion );
                        SET_MEMORY_STR_CHAR( __var_tarjetas_autorizacion, 10, 0 );
                        _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
                    }
                    /*-------------- Si hay error de comunicacion ---------------*/
                    else if( rta_terminal.cod_rta == 1 ) {
                        GRABAR_OFF_LINE( _OFF_LINE_ANULACION, _PAGO_IMPORTE( h ) );
                    }
                }
                else {//numero de referencia vacio?¿
                    GRABAR_OFF_LINE( _OPERACION_ANULADA, _PAGO_IMPORTE( h ) );
                }
				if( !actualizar ) {
				   ENCOLAR_VOUCHER_TARJETA(  ); //aca genera voucher de anulacion ojo con caida de caja UAA-38
            }
            }
            /*---------- VOUCHER de anulacion -----------*/
            if( actualizar ) {
                IMPRIME_VOUCHER( h, 1, anulado );
                IMPRIME_VOUCHER( h, 0, anulado );
                if( SINCRONIZAR_PRINTER() ) {
                    IMPRIME_COPIA_VOUCHER( SI );
                }
                /*---------- actualiza archivos -------------*/
                cant++;
                #ifdef COMPILAR_ASC
                ANULA_TARJETAS_ASC( -cant );
                #endif
                ANULAR_CUPON( -cant );
                if( tipo_pago == 1 ) {
                    _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE( h ) ) );
                    _ADD_MEMORY_INT( __x_cobros_cantidad, _PAGO_MODO( h ), -1 );
                }
                else {
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE( h ) ) );
                    _ADD_MEMORY_INT( __x_ventas_cantidad, _PAGO_MODO( h ), -1 );
                }
                /*-------- actualiza COMTAR ---------*/
                GRABAR_COMTAR( VAR_TARJETAS_TARJETA, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/, 1 );
            }
            if( VARIAS_TARJETAS_POR_PAGO ) {
                GRABA_MODIFICACION_TARJETAS( posicion - 1, TARJETA_TRADICIONAL );
            }
        }
    }
}
#endif
/*****************************************************************************/
long PIDE_NRO_CUPON( int digitos )
/*****************************************************************************/
{
    long numero = 0;
    ExecuteSSAction( A_PEDIR, 1, ST( S__NRO_CUPON_ ), "", NULL, NULL );
    while( numero == 0 ) {
        numero = PEDIR_GRAN_ENTERO( 29 - ancho_panel / 2, 22, digitos );
    }
    ExecuteAction( A_FIN_PEDIR );
    return ( numero );
}
/*****************************************************************************/
void IMPRIME_DATOS_TARJETA( long cupon )
/*****************************************************************************/
{
    MOSTRAR_CADENA( 21, PRN, ST( S_CUPON_N_ ) );
    MOSTRAR_ENTERO( 31, PRN, "000000000", cupon );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_TARJETA_N_ ) );
    MOSTRAR_CADENA( 21, PRN, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
    // if( IMPRIMIR_MARCA_TARJETA_EN_TICKET && VAR_TARJETAS_TARJETA != _CMR )
    // {
    if( IMPRIMIR_MARCA_TARJETA_EN_TICKET ) {
        LF( 1 );
        MOSTRAR_CADENA( 1, PRN, NOMBRE_TARJETA( VAR_TARJETAS_TARJETA ) );
        MOSTRAR_CADENA( 21, PRN, ST( S_CUOTAS_ ) );
        MOSTRAR_ENTERO( 29, PRN, "00", VAR_TARJETAS_CUOTAS );
    }
    /* if( VAR_TARJETAS_TARJETA == _CMR ) { LF(1); MINI_VOUCHER_CMR(); } */
}
/*****************************************************************************/
int EN_CUOTAS()
/*****************************************************************************/
{
    //return( VAR_TARJETAS_CUOTAS > 1 && VAR_TARJETAS_TARJETA != _PROVENCRED && VAR_TARJETAS_TARJETA != _PROVENCRED_NUEVA && VAR_TARJETAS_TARJETA != _KADICARD );
    // Esto debiera ser configurable
    return ( VAR_TARJETAS_CUOTAS > 1 );
}
/*****************************************************************************/
int SUPERA_LIMITE_LIBERTAD( int h )
/*****************************************************************************/
{
    return ( _PAGO_IMPORTE( h ) > LIMITE2_TARJETA( VAR_TARJETAS_TARJETA ) );
}
#ifdef COMPILAR_PINPAD
/***************************************************************************/
int PIDE_CLAVE_EN_PINPAD( int cartel, int confirmar_importe, double importe )
/***************************************************************************/
{
    int rta = 1;
    char pin[9];
    char nro_tarjeta[21];
	memset(pin,0,9);
	memset(nro_tarjeta,0,21);
    if( VAR_TARJETAS_CELDA_PINPAD && UTILIZA_PINPAD ) {
        strncpy( nro_tarjeta, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/, sizeof( nro_tarjeta ) );
        rta = PINPAD_AUTORIZAR_COMPRA( VAR_TARJETAS_CELDA_PINPAD, nro_tarjeta,
                                       VAR_TARJETAS_TIPO_CUENTA, pin, VAR_TARJETAS_L_PIN,
                                       confirmar_importe, importe );
        if( rta ) {
			pin[8]= '\0';
            SET_PIN_PRIVADO( pin );
			//guardemos el pin del usuario
			PINPAD_ACTUALIZAR_pin_WORKING_KEY( pin );
        }
        if( cartel ) {
            PINPAD_CARTEL();
        }
    }
    return ( rta );
}
#endif
/*****************************************************************************/
void CONSULTA_SALDO_TARJETA()
/*****************************************************************************/
{
    #ifdef COMPILAR_PINPAD
    int ok;
    if( UTILIZA_PINPAD ) {
        /*------------------ Guarda datos tarjeta y pone a cero -----------------*/
        COPY_STRUCT( _var_tarjetas_ant, _var_tarjetas );
        CEREAR_STRUCT( _var_tarjetas );
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        /*------------------------- Pide nuevos datos ---------------------------*/
        ok = VALIDA_TARJETA( SI, NO, CONSULTA_SALDO, NO, NULL, NO, -1/*NO*/ );
        if( ok && VAR_TARJETAS_STATUS == 1 ) {
			ok = PIDE_DATOS_TARJETA( SI, 0 );
        }
        /*---------------------------- Pide PIN ---------------------------------*/
        if( ok && PIDE_CLAVE_EN_PINPAD( NO, NO, 0.0 ) ) {
            PINPAD_MOSTRAR( ST( S_CONECTANDO___ ) );
            PEDIR_SALDO_ON_LINE();
        }
        /*---------------------- Recupera datos anteriores ----------------------*/
        COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
        PINPAD_CARTEL();
    }
    #endif
}
/*****************************************************************************/
void PEDIR_SALDO_ON_LINE()
/*****************************************************************************/
{
    struct _rta_terminal rta_terminal;
    if( AUTORIZACION_ON_LINE( _ON_LINE_SALDO, 0, ( char* )&rta_terminal, sizeof( rta_terminal ) ) ) {
        #ifdef COMPILAR_PINPAD
        PINPAD_ACTUALIZAR_WORKING_KEY( VAR_TARJETAS_CELDA_PINPAD, rta_terminal.working_key );
        #endif
    }
    else {
        rta_terminal.cod_rta = 1;
        strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),
                 sizeof( rta_terminal.mensaje ) );
    }
    /*-------------- Procesa la respuesta ---------------*/
    switch( rta_terminal.cod_rta ) {
        case 0:
            /* Aprobada */
            #ifdef COMPILAR_PINPAD
            MOSTRAR_SALDO_EN_PINPAD( rta_terminal.saldo );
            #endif
            break;
        default:
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            break;
    }
}
#ifdef COMPILAR_CLIENTE_AFINIDAD_ON_LINE
/*****************************************************************************/
double ENVIAR_PAQUETE_AFINIDAD( char tipo_operacion, long cliente, double puntos )
/*****************************************************************************/
{
    char _cliente[12];
    double rta = -1;
    struct _rta_terminal_afinidad rta_terminal;
		memset(_cliente,0, sizeof(_cliente));
    /*------------------ Guarda datos tarjeta y pone a cero -----------------*/
	memset(&rta_terminal,0, sizeof( struct _rta_terminal_afinidad ));
    COPY_STRUCT( _var_tarjetas_ant, _var_tarjetas );
    CEREAR_STRUCT( _var_tarjetas );
    SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
    /*---------------------- Prepara nuevos datos ------------------------*/
    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, _AFINIDAD );
    SET_MEMORY_DOUBLE( __var_tarjetas_importe_entrega, RAM_TOTAL );
	if ( VAR_TARJETAS_TARJETA != _AFINIDAD ) { //para el raptor necesito el nro de tarjeta no el cliente
	SET_NUMERO_TARJETA_PRIVADO( _cliente );//    SET_MEMORY( __var_tarjetas_numero, _cliente );
		sprintf( _cliente, "%li", cliente );
		SET_MEMORY( __var_tarjetas_numero, _cliente );
	} else {
	   SET_MEMORY( __var_tarjetas_numero, RAM_STRING_CLIENTE_AFINIDAD );
	}
    if( !AUTORIZACION_ON_LINE( tipo_operacion, puntos - RAM_TOTAL, ( char* )&rta_terminal,
                               sizeof( struct _rta_terminal ) ) ) {
        rta_terminal.cod_rta = 1;
        strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),
                 sizeof( rta_terminal.mensaje ) );
    }
    /*-------------- Procesa la respuesta ---------------*/
    switch( rta_terminal.cod_rta ) {
        case 0:
            /* Aprobada */
            rta = rta_terminal.saldo;
            SET_MEMORY_DOUBLE( __ram_puntos_afinidad, ( double )rta_terminal.saldo );
            rta_terminal.nombre[28] = 0;
            SET_MEMORY( __ram_nombre_cliente_afinidad, rta_terminal.nombre );
            SET_MEMORY_INT( __ram_fecha_puntos_afinidad,
                            OBTENER_FECHA_PUNTOS( rta_terminal.mensaje ) );
            if( rta_terminal.mensaje[0] && tipo_operacion != _ON_LINE_COMPRA ) {
                MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            }
			SET_MEMORY( __var_tarjetas_ant_nro_cuenta, rta_terminal.nro_cuenta );
            break;
        default:
            SET_MEMORY_DOUBLE( __ram_puntos_afinidad, ( double ) - 1 );
            rta_terminal.nombre[0] = 0;
            SET_MEMORY( __ram_nombre_cliente_afinidad, rta_terminal.nombre );
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            if( tipo_operacion == _ON_LINE_COMPRA ) {
                GRABAR_OFF_LINE( ( MODO_DEVOLUCION ) ? _OFF_LINE_DEVOLUCION : _OFF_LINE_COMPRA,
                                 puntos );
            }
            break;
    }
    /*---------------------- Recupera datos anteriores ----------------------*/
    COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
    return ( rta );
}
#endif
/*****************************************************************************/
int ON_LINE___TARJETA( char tipo_operacion, struct _rta_terminal *rta_terminal, double importe )
/*****************************************************************************/
{
    int rta = 0;
    if( AUTORIZACION_ON_LINE( tipo_operacion, importe, ( char* )rta_terminal,
                              sizeof( struct _rta_terminal ) ) ) {
    }
    else {
        rta_terminal->cod_rta = 1;
        strncpy( rta_terminal->mensaje, ST( S_ERROR_DE_COMUNICACION ),
                 sizeof( rta_terminal->mensaje ) );
    }
    /*-------------- Procesa la respuesta ---------------*/
    switch( rta_terminal->cod_rta ) {
        case 0:
            /* Aprobada */
            SET_MEMORY_CHAR( __var_tarjetas_on_line, SI );
			LTRIM(rta_terminal->nro_referencia);
            SET_MEMORY( __var_tarjetas_nro_ref, rta_terminal->nro_referencia );
            SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal->autorizacion );
            SET_MEMORY_INT( __var_tarjetas_lote, rta_terminal->lote );
            SET_MEMORY_CHAR( __var_tarjetas_aut_on_line, SI );
            SET_MEMORY( __var_tarjetas_mensaje, rta_terminal->mensaje );
            rta = 1;
            break;
        case 1:
            /* Error en la comunicacion */
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal->mensaje );
            if( VAR_TARJETAS_UNICAMENTE_OL ) {
                MENSAJE_STRING( S_UNICAMENTE_ON_LINE );
            }
            switch( tipo_operacion ) {
                case _ON_LINE_ANUL_COBRO:
                    if( VAR_TARJETAS_UNICAMENTE_OL ) {
                        rta = 0;
                    }
                    else {
                        GRABAR_OFF_LINE( _OFF_LINE_ANUL_COBRO, importe );
                        rta = 2;
                    }
                    break;
                    /*  case   _ON_LINE_ANUL_DEVOLUCION_COBRO:
                     * if( VAR_TARJETAS_UNICAMENTE_OL ) rta = 0;
                     * else {
                     * GRABAR_OFF_LINE( _OFF_LINE_ANUL_DEVOLUCION_COBRO ,importe );
                     * rta = 2;
                     * }
                     * break;
                     * case _ON_LINE_DEVOLUCION_COBRO:
                     * if( VAR_TARJETAS_UNICAMENTE_OL ) rta = 0;
                     * else {
                     * GRABAR_OFF_LINE( _OFF_LINE_DEVOLUCION_COBRO ,importe );
                     * rta = 2;
                     * }
                     * break;
                     */
                case _ON_LINE_COBRO:
                    if( VAR_TARJETAS_UNICAMENTE_OL ) {
                        rta = 0;
                    }
                    else {
                        if( GRABAR_OFF_LINE( _OFF_LINE_COBRO, importe ) ) {
                            rta = 2;
                        }
                        else {
                            rta = 0;
                        }
                    }
                    break;
                default:
                    if( VAR_TARJETAS_UNICAMENTE_OL ) {
                        rta = 0;
                    }
                    else {
                        rta = 2;
                    }
                    break;
            }
            BORRAR_MENSAJE();
            break;
        default:
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal->mensaje );
            break;
    }
    BORRAR_MENSAJE();
    return rta;
}
/*****************************************************************************/
int _ANULAR_TARJETAS( int cuantas, int AnulacionIndividual, int posiciontarjeta )
/*****************************************************************************/
{
    int h, handle, posicion = 0, rtaa= 1 ;
    char *buffer = 0;
   
    #ifdef INVEL_L
    handle = open( "tarjetas.$$$", O_RDONLY );
    #else
    handle = _open( "tarjetas.$$$", O_RDONLY );    
    #endif
    if( !buffer ) {
        buffer = ( char* )MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
    }
    //ESTA MODIFICACION PERMITE ANULAR UNA TARJETA  ESPECIFICA Y NO BARRER TODAS!! 
	//ver despues de hacer una sola funcion con los parametros correctos del for
	if( AnulacionIndividual == SI ){
		h = cuantas+1;
		rtaa = CARGA_VERIFICA_ANULA_TARJETAS(handle, buffer,h, posiciontarjeta +1 );
	} else{
		for( h = 0;h <= cuantas /*RAM_P_PAGO */ ;h++ ) {
            CARGA_VERIFICA_ANULA_TARJETAS(handle, buffer,h,-1);
         }
    }
    if( handle != -1 )
#ifdef INVEL_L
    {
        close( handle );
    }
    #else
    _close( handle );
    #endif
    if( buffer ) {
        FREEMEM( buffer );
        buffer = 0;
    }
	return ( rtaa) ;
}

/*****************************************************************************/
void ENCOLAR_VOUCHER_TARJETA( void )
/*****************************************************************************/
{
   int       handle_voucher;
   char     *buffer_voucher = NULL;

   handle_voucher = _OPEN_O_CREAT( "voucher.$$$", O_RDWR, __LINE__, __FILE__ );
   if( handle_voucher > 0 ) {
      buffer_voucher = MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
      if( buffer_voucher ) {
		  PASAR_PRIVADOS_A_DATOS_TARJETA();
         COPY_STRUCT_TO_BUFFER( buffer_voucher, _var_tarjetas );
         LSEEK( handle_voucher, 0L, SEEK_END, __LINE__, __FILE__ );
         _WRITE( handle_voucher, ( char * ) buffer_voucher, SIZEOF_STRUCT( _var_tarjetas ),__LINE__, __FILE__ );
		 FREEMEM( buffer_voucher );
         buffer_voucher = 0;
      }
	  _close( handle_voucher );
   }
}

/*****************************************************************************/
void VALIDAR_VOUCHER_ENCOLADOS( void )
/*****************************************************************************/
{
   int       handle_voucher;
   char     *buffer_voucher = NULL;
   int       cant_voucher, i;
   long      anulado;
   char      num[50];
   int       cod_val;
   unsigned  aux_fecha;

	memset(num,0, sizeof(num));
   if( access( "voucher.$$$", 0 ) == 0 ) {
      handle_voucher = _OPEN( "voucher.$$$", O_RDONLY, __LINE__, __FILE__ );
      if( handle_voucher > 0 ) {
         buffer_voucher = ( char * ) MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
         if( buffer_voucher ) {
            cant_voucher = filelength( handle_voucher ) / SIZEOF_STRUCT( _var_tarjetas );
            for( i = 0; i < cant_voucher; i++ ) {
               _READ( handle_voucher, buffer_voucher, SIZEOF_STRUCT( _var_tarjetas ), __LINE__, __FILE__ );
               COPY_BUFFER_TO_STRUCT( buffer_voucher, _var_tarjetas );
//-- Acá hay que imprimir el voucher con los datos de la estructura Tarjeta.
			   PASAR_DATOS_TARJETA_A_PRIVADOS();
               anulado = VAR_TARJETAS_CUPON_ORIGINAL;//VAR_TARJETAS_NRO_CUPON_INT;
			   if( anulado == 0 && ( VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION ||VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION  || VAR_TARJETAS_OPERACION == _ON_LINE_ANUL_COBRO || VAR_TARJETAS_OPERACION == _OFF_LINE_ANUL_COBRO  || VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION_CASHPLUS))
				   anulado = SI;
			   // deberia ser anulado = VAR_TARJETAS_OPERACION
			   sprintf(num,"%s",GET_NUMERO_TARJETA_PRIVADO());
               if( strlen(num) < 12){ //control por si leee basura o 0 en el voucher$$$ sobre todo si se usa la funcion borrar_voucher_encolado
                 GRABAR_LOG_SISTEMA_STRING( "Warning Voucher vacio", LOG_ERRORES,2 );
			   } else {
               #ifdef COMPILAR_IBM_3FA
               if( IMPRESORA_FISCAL == FISCAL_IBM_3FA ) {
                  SET_MEMORY_INT( __ram_variable_dnfh, 11 );
                  if( DNFH_VOUCHER_TARJETA ) {
                     VALIDAR_DNFH_IBM( DNFH_VOUCHER_TARJETA, 0 );
                  }
                  if( TARJ[LOCALIZA_TARJETA(VAR_TARJETAS_TARJETA)].val_voucher_pesos ) {
                     VALIDAR_DNFH_IBM( TARJ[LOCALIZA_TARJETA(VAR_TARJETAS_TARJETA)].val_voucher_pesos, 0 );
                  }
                  if( DNF_VOUCHER_TARJETA ) {
                     SET_MEMORY_INT( __ram_variable_dnfh, 11 );
                     VALIDAR_DNFH_IBM( DNF_VOUCHER_TARJETA, 0 );
                  }
                  SET_MEMORY_INT( __ram_variable_dnfh, 12 );
                  if( DNFH_VOUCHER_TARJETA ) {
                     VALIDAR_DNFH_IBM( DNFH_VOUCHER_TARJETA, 0 );
                  }
                  if( TARJ[LOCALIZA_TARJETA(VAR_TARJETAS_TARJETA)].val_voucher_pesos ) {
                     VALIDAR_DNFH_IBM( TARJ[LOCALIZA_TARJETA(VAR_TARJETAS_TARJETA)].val_voucher_pesos, 0 );
                  }
                  if( DNF_VOUCHER_TARJETA ) {
                     SET_MEMORY_INT( __ram_variable_dnfh, 11 );
                     VALIDAR_DNFH_IBM( DNF_VOUCHER_TARJETA, 0 );
                  }
               } else {
                  #endif
				cod_val = ( VAR_TARJETAS_MONEDA == PESOS ) ? TARJ[VAR_TARJETAS_TARJETA].
							val_voucher_pesos : TARJ[VAR_TARJETAS_TARJETA].val_voucher_dolares;
				if( !cod_val ) {

   					if( IMPRESORA_FISCAL == FISCAL )
   						ABRIR_DOCUMENTO_NO_FISCAL ( 0);
					/*if( VOUCHER_TARJETA_FORMATO_POSNET ) {
						IMPRIME_VOUCHER_IDEM_POSNET( -1, 1, anulado );
						IMPRIME_VOUCHER_IDEM_POSNET( -1, 0, anulado );
						} else {*/
					{
					   char mensa[100];
					    memset(mensa,0, sizeof(mensa));
						if( VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION || VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION 
							|| VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION_DEVOLUCION  || VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION_DEVOLUCION ) {
								  _snprintf(mensa, sizeof(mensa)-1,"IMPRIMIENDO VOUCHER DE ANULACION TARJETA" );
						} else
							_snprintf(mensa, sizeof(mensa)-1,"IMPRIMIENDO VOUCHER TARJETA");

						MENSAJE(mensa);
						IMPRIME_VOUCHER( -1, 1, anulado );
						if( IMPRESORA_FISCAL == FISCAL )
							ABRIR_DOCUMENTO_NO_FISCAL ( 0);
						IMPRIME_VOUCHER( -1, 0, anulado );
					}   					
					if( IMPRESORA_FISCAL == FISCAL )
   						CERRAR_DOCUMENTO_NO_FISCAL();
					BORRAR_MENSAJE();
				} else { //con val-caja
					_SET_MEMORY_DOUBLE( __pago_importe, 0, VAR_TARJETAS_IMPORTE ); //para el val-caja
					aux_fecha = GET_FECHA_VTO_PRIVADA( );
					SET_FECHA_VTO_PRIVADA( DECREMENTAR_MES( aux_fecha ));
					{
					   char mensa[100];
					    memset(mensa,0, sizeof(mensa));
						if( VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION || VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION 
							|| VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION_DEVOLUCION  || VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION_DEVOLUCION ) {
								  _snprintf(mensa, sizeof(mensa)-1,"IMPRIMIENDO VOUCHER DE ANULACION TARJETA" );
						} else
							_snprintf(mensa, sizeof(mensa)-1,"IMPRIMIENDO VOUCHER TARJETA");

						MENSAJE(mensa);
					}
					VALIDAR_VOUCHER_TARJETA( cod_val, _PAGO_IMPORTE( 0 ), 0 );
					SET_FECHA_VTO_PRIVADA( aux_fecha );
					_SET_MEMORY_DOUBLE( __pago_importe, 0, 0.00 ); //para el val-caja
					BORRAR_MENSAJE();
				}
               #ifdef COMPILAR_IBM_3FA
               }
               #endif
            }
            } //fin for
			FREEMEM( buffer_voucher );
			buffer_voucher = 0;
         }
		 _close( handle_voucher );
      }
   }
   CEREAR_STRUCT( _var_tarjetas );
   unlink("voucher.$$$");
}
/*****************************************************************************/
void VALIDACION_TELEFONICA( double importe )
/*****************************************************************************/
{
    char ip[20],aux[50];
    int error = 1, fecha_aux;
    struct
    {
        int sucursal;
        int caja;
        int operacion;
        int codigo_marca;
        char tipo_de_tarjeta;                // 0-credito 1-debito
        char tipo_de_cuenta;
        char modo_de_ingreso;
        // 0 manual
        // 1 banda magnetica
        char tarjeta[21];
        char fecha_expiracion[4];
        double importe;
        char moneda;
        int cuotas;
        char plan[6];
    } validacion_te;
    if( VALIDACION_TELEFONICA_HABILITADA ) {
        memset( ip, 0, sizeof( ip ) );
        strncpy( ip, IP_HOST_VALIDACION_TELEFONICA, sizeof( ip ) );
        if( ABRIR_CANAL_TCP_IP( TCP_canal_puerto( _CANAL_VAL_TELE,
                                                  PUERTO_HOST_VALIDACION_TELEFONIC ), ip,
                                _FORMATO_MENSAJE_INVEL ) ) {
            memset( &validacion_te, 0, sizeof( validacion_te ) );
            validacion_te.sucursal = config.sucursal;
            validacion_te.caja = NRO_CAJA;
            validacion_te.operacion = ( MODO_DEVOLUCION ) ? _ON_LINE_DEVOLUCION : _ON_LINE_COMPRA;
            validacion_te.codigo_marca = VAR_TARJETAS_TARJETA;
            // debito 1   credito 0
            validacion_te.tipo_de_tarjeta = VAR_TARJETAS_TIPO_CUENTA ? 1 : 0;
            if( VAR_TARJETAS_TIPO_CUENTA ) {
                validacion_te.tipo_de_cuenta = ( ( VAR_TARJETAS_MONEDA == PESOS ) ? 7 : 0 )
                                             + VAR_TARJETAS_TIPO_CUENTA;
            }
            validacion_te.modo_de_ingreso = VAR_TARJETAS_LECTURA_BANDA;
            strcpy( validacion_te.tarjeta, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/ );
            fecha_aux = DECREMENTAR_MES( GET_FECHA_VTO_PRIVADA(  )  );
            DTOA_INVERSA( fecha_aux, aux );
            memcpy( validacion_te.fecha_expiracion, aux, 4 );
            validacion_te.importe = importe;
            validacion_te.moneda = VAR_TARJETAS_MONEDA;
            validacion_te.cuotas = VAR_TARJETAS_CUOTAS;
            memcpy( &validacion_te.plan, VAR_TARJETAS_TABLA, 6 );
            error = _ENVIAR_PAQUETE_TCP_IP( ( char* )&validacion_te, sizeof( validacion_te ), NULL,
                                            0, 0, NO, _CANAL_VAL_TELE );
            CERRAR_CANAL_TCP_IP( _CANAL_VAL_TELE );
        }
        if( !error ) {
            MENSAJE_TECLA_STRING( S_VALIDACION_TELEFONICA_ENVIADA_PRESIONE_ENTER );
        }
        else {
            MENSAJE_TECLA_STRING( S_VALIDACION_TELEFONICA_ERROR_PRESIONE_ENTER );
        }
    }
}
/*****************************************************************************/
void CONSULTA_SALDO_TARJETA_CREDITO()
/*****************************************************************************/
{
    struct _rta_terminal rta_terminal;
	char men[80];
    /*------------------ Guarda datos tarjeta y pone a cero -----------------*/
    COPY_STRUCT( _var_tarjetas_ant, _var_tarjetas );
    CEREAR_STRUCT( _var_tarjetas );
	
	if( VAR_TARJETAS_STATUS == 0 ) {
        VALIDA_TARJETA( SI, NO, CONSULTA_SALDO, NO, NULL, NO, -1/*NO*/ );
    }
   
	SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
    /*------------------------- Pide nuevos datos ---------------------------*/
    NUMERO_DE_COMERCIO_TARJETA(); 
	
	//ok = VALIDA_TARJETA( SI, NO, CONSULTA_SALDO, NO, NULL, -1/*NO*/ );
    //if( ok && VAR_TARJETAS_STATUS == 1 ) {
    if( VAR_TARJETAS_STATUS > 0 ) {
        if( AUTORIZACION_ON_LINE( _ON_LINE_SALDO, 0, ( char* )&rta_terminal,
                                  sizeof( rta_terminal ) ) ) {
            #ifdef COMPILAR_PINPAD
            PINPAD_ACTUALIZAR_WORKING_KEY( VAR_TARJETAS_CELDA_PINPAD, rta_terminal.working_key );
            #endif
        }
        else {
            rta_terminal.cod_rta = 1;
            strncpy( rta_terminal.mensaje, ST( S_ERROR_DE_COMUNICACION ),
                     sizeof( rta_terminal.mensaje ) );
        }
        /*-------------- Procesa la respuesta ---------------*/
		CEREAR_STRUCT( _var_tarjetas );
		switch( rta_terminal.cod_rta ) {
        case 0:
            /* Aprobada */
			sprintf( men, "%s Saldo: %.2lf", rta_terminal.mensaje, rta_terminal.monto_compra );
			SET_MEMORY_FLOAT( __var_tarjetas_saldo_cliente, rta_terminal.monto_compra );
			MOSTRAR_RESULTADO_ON_LINE( NO, men );     
            break;
        default:
            MOSTRAR_RESULTADO_ON_LINE( NO, rta_terminal.mensaje );
            break;
    }
		//CR1_MUESTRA_TARJETA( );4
		
    }
    /*---------------------- Recupera datos anteriores ----------------------*/
    //COPY_STRUCT( _var_tarjetas, _var_tarjetas_ant );
}
/*************************************************************************************************/
int PIDE_TARJETA_PARA_ANULAR( int *cancela_anula  ){
/*************************************************************************************************/
	int limite=5;
	int h = 0;
////aqui despues de un corte de caja y se quiere anular no se cargó la tar_med
//carguemos la tar_med
	for( h = 0;h < RAM_P_PAGO;h++ ) {
		if( medios[( int )_PAGO_MODO( h )].tarjeta ) {
			CARGAR_CONFIGURACION_TAR_MED( ( int )_PAGO_MODO( h ) ) ;
		}
	}

//


	if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ){
		char *buffer = NULL;
		char mensaje[100];
		char num_tarj[21];
		int ok = 0, tecla = 0;
	//	int limite=NO;

		buffer = ( char* )MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
		//por seguridad respaldo la tarjeta como estaba.. 
		//sino es la respuesta baja la anulación, si es sigue todo igual
		COPY_STRUCT_TO_BUFFER( buffer, _var_tarjetas );

		memset(num_tarj,0,sizeof(num_tarj));
		strncpy( num_tarj, GET_NUMERO_TARJETA_PRIVADO(), 21 );
		memset(&num_tarj[6],'*',5);
		num_tarj[20] = 0;
		memset(mensaje,0,sizeof(mensaje));
		sprintf(mensaje," PASE LA TARJETA (%s) NUEVAMENTE PARA ANULAR LA COMPRA",num_tarj);

		while( limite!= -1 && (!VALIDA_TARJETA( ANULACION, NO, VENTAS, NO,mensaje, NO, -1) ||  
			!ES_LA_MISMA_TARJETA(num_tarj,GET_NUMERO_TARJETA_PRIVADO()))  ){
				
				
				if( LASTKEY() ==27 || limite < 1 ) {
					int k=0;
					VACIAR_KEY();
					MOSTRAR_MENSAJE_UBUNTU( 0, "CUIDADO!!!SE ANULARAN TODAS LAS TARJETAS", 8 );
					ALERTA_EN_VENTANA("SE PRESIONO ESC POR LO TANTO SE PEDIRA PASAR TODAS LAS TARJETAS PARA ANULAR, RECORDAR LA TECLA ANULACION",NULL,5,"tps - v" ,0);
					k = EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 
					tecla = MENSAJE_CON_TECLA2_S_N("DESEA CANCELAR LA ANULACION DE TARJETA <ENTER>=SI <ESC>=NO",13,27);

				  if( tecla == 13 ) {
				     limite = -1;
				  } else 
					  limite = 5;
				} else {
					MENSAJE_STRING( S_TARJETA_INCORRECTA );
					MOSTRAR_MENSAJE_UBUNTU( 0, ST(S_TARJETA_INCORRECTA), 4 );
				}
		}
		if( config_tps.NapseModalidad == 1) {
			MENSAJE( mensaje );
			MOSTRAR_MENSAJE_UBUNTU( 0, mensaje, 8 );
		}
				
		if(limite<=0 || LASTKEY() == 27){
			//GRABAR_LOG_SISTEMA_STRING( S_TARJETA_INCORRECTA, LOG_ERRORES,2 );
			GRABAR_LOG_SISTEMA_STRING( S_NO_SE_PUEDE_REALIZAR_LA_ANULACION,LOG_ERRORES,2 );
			//MENSAJE_STRING( S_TARJETA_INCORRECTA );
			MENSAJE_STRING( S_NO_SE_PUEDE_REALIZAR_LA_ANULACION );
			MOSTRAR_MENSAJE_UBUNTU( 2, ST(S_NO_SE_PUEDE_REALIZAR_LA_ANULACION), 3 );
			*cancela_anula = SI;
		}else {
			 #ifdef COMPILAR_PINPAD
			    SETEAR_DEBITO_POS( 0) ;
                ok =PIDE_CLAVE_EN_PINPAD( SI, SI, 0.0 );//no interesa el importe aca porque no lo mostramos en el pinpad. Si hay que mostrarlo aca debe ir el importe a pagar
				if( ok == 1)
					SETEAR_DEBITO_POS( -1) ;
            #endif
			COPY_BUFFER_TO_STRUCT( buffer, _var_tarjetas );
		}
		 FREEMEM( buffer );
	
	if( LASTKEY() == 27) 
		limite = -1;
	} //corro el fin del if ya el lastkey no debe tenerse en cuenta si no se encripta la tarjeta

	return (limite>0)?SI:NO;
}

int HAY_QUE_ANULAR () {
	// 0, 2,5
	int rrta = NO;
	//VAR_TARJETAS_ANT_DATAFONO;
	switch ( VAR_TARJETAS_DATAFONO ) {
		case 0:
		case 2:
		case 5:
		    rrta = SI;
			break;
	}
	if( rrta == NO && VAR_TARJETAS_ON_LINE == 2)
		rrta = SI;
  return rrta;
}


/****************************************************************************************/
int CARGA_VERIFICA_ANULA_TARJETAS(int handle, char *buffer,int h, int posiciontarjeta){
/****************************************************************************************/
    int posicion=0;
    struct _rta_terminal rta_terminal;
	int rta = 1;
    int falla_anulacion =NO;
	int cancela_anula= NO;
	if( posiciontarjeta == -1)
		posicion = h;
	else
		posicion = posiciontarjeta;
	
    if( medios[( int )_PAGO_MODO( h )].tarjeta ) {
        if( handle < 0 || !buffer ) {
            MENSAJE_STRING( S_NO_SE_PUEDEN_HACER_LAS_ANULACIONES );
            rta = 0;
        }
        else {
            int auxiliar=0;
            _lseek( handle, posicion * ( SIZEOF_STRUCT( _var_tarjetas ) ), 0 );
    #ifdef INVEL_L
            read( handle, buffer, SIZEOF_STRUCT( _var_tarjetas ) );
    #else
            _read( handle, buffer, SIZEOF_STRUCT( _var_tarjetas ) );
    #endif
            posicion++;
            COPY_BUFFER_TO_STRUCT( buffer, _var_tarjetas );
    #ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
            PASAR_DATOS_TARJETA_A_PRIVADOS();
    #endif
			
			/*--------------------- Encolamos el voucher Inicial de la operacion de venta------------------------*/
			/*se encola ahora y no con el ok, porque tiene los datos para el voucher 
			antes de modificarlos por la anulacion*/
			
			ENCOLAR_VOUCHER_TARJETA(  );
			
			/*--------------------------------------------------------------------------------------------------*/


            auxiliar=VAR_TARJETAS_TARJETA; 
            SET_MEMORY_LONG( __var_tarjetas_cupon_original,_PAGO_NRO_CUPON_INT1( h ) );
            _SET_MEMORY_LONG( __pago_nro_cupon_int1, h, OBTENER_NRO_CUPON( VAR_TARJETAS_TARJETA ));
               //CALCULAR_NRO_CUPON( VAR_TARJETAS_TARJETA ) );
			
			//controlemos que el nro de cupon original de la compra  se corresponda con el nro de cupon de la compra
			//VAR_TARJETAS_NRO_CUPON_INT nro de cupon de compra
			//VAR_TARJETAS_CUPON_ORIGINAL nro de cupon original puesto en la anulacin
			if( VAR_TARJETAS_NRO_CUPON_INT < VAR_TARJETAS_CUPON_ORIGINAL )
				SET_MEMORY_LONG( __var_tarjetas_cupon_original, VAR_TARJETAS_NRO_CUPON_INT);
			

			SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int, _PAGO_NRO_CUPON_INT1( h ) );
			
            /*-----CARGAMOS LOS DATOS PARA LA ANULACION ----------------------------------------*/
			if( VAR_TARJETAS_FECHA_HOST) 
				SET_MEMORY_INT( __var_tarjetas_fecha_original, VAR_TARJETAS_FECHA_HOST );
            SET_MEMORY_LONG( __var_tarjetas_hora_original, VAR_TARJETAS_HORA_HOST );
            SET_MEMORY( __var_tarjetas_aut_original, VAR_TARJETAS_AUTORIZACION );
            SET_MEMORY_DOUBLE( __var_tarjetas_importe, _PAGO_IMPORTE( h ));

			
			/*--------- Envia on-line o graba off-line ----------------------------------------*/
			if( EXIGE_AUTORIZACION_ON_LINE()) {
				int opcion = 0;
				int tecla = 0;
				int condicion_anulacion = 0;
				//VAR_TARJETAS_ON_LINE en 1 es online y en 2 es offline;
				if( _VAR_TARJETAS_NRO_REF( 0 ) || config_tps.NapseModalidad == 1)
					condicion_anulacion = 1;
				else
					condicion_anulacion = 2;

			/*------------ Tenemos que anular y pedimos la tarjeta ------------------------*/
				if( condicion_anulacion > 0 && HAY_QUE_ANULAR() == SI  && 
						PIDE_TARJETA_PARA_ANULAR( &cancela_anula) ) {
				
             
							
							
			/*------------- Anulacin remota ------------------*/
					if(condicion_anulacion == 1 &&  !AUTORIZACION_ON_LINE( _ON_LINE_ANULACION, _PAGO_IMPORTE( h ),
                    			( char* )&rta_terminal,sizeof( rta_terminal ) ) ) {
                    	rta_terminal.cod_rta = 1;
						falla_anulacion = SI;
                    }

					// while de Debito  
					do {                  
						if( condicion_anulacion == 2 || 
							( condicion_anulacion == 1 && rta_terminal.cod_rta == 1) ) { //reintentar la anulacion 
						
								if( !VAR_TARJETAS_UNICAMENTE_OL ) {
									char mensajes[200];
									memset(mensajes,0,sizeof(mensajes));
									MENSAJE_TECLA( "FALLO ANULACION ONLINE - PRES. TECLA PARA CONTINUAR" );
									VACIAR_KEY();
									_snprintf(mensajes, sizeof(mensajes) -1, "%s %s",ST(S_1_AUTORIZACION_OFF_LINE_2_ENVIAR_TRANSACCION_ON_LINE),"(ESC) CANCELA");
									MENSAJE( mensajes );
									while( !opcion ) {
										char c = ( char )GETCH();
										if( c == '1' ) {
											opcion = 1;
											SET_MEMORY_CHAR( __var_tarjetas_on_line, 2 );
											falla_anulacion = NO;
										}else
											if( c == '2' ) {
												opcion = 2;
												SET_MEMORY_CHAR( __var_tarjetas_on_line, 1 );
												falla_anulacion = NO;
											}else 
												if( c == 27 ) {
													opcion = 3;
													rta = 0; //abajo deberia borrar el voucher encolado?
													falla_anulacion = SI;
												}
									}
								} else { ////si es de debito dar la opcin de mandar online nuevamente
									if( VAR_TARJETAS_CELDA_PINPAD > 0 //ES DE DEBITO
										|| VAR_TARJETAS_UNICAMENTE_OL ) { 
											VACIAR_KEY();
											MENSAJE_TECLA( "FALLO ANULACION ONLINE Y TARJETA ES EXCLUSIVA ONLINE - PRES. TECLA" );
											VACIAR_KEY();
											falla_anulacion =SI;
											tecla = MENSAJE_CON_TECLA_S_N("DESEA REINTENTAR ONLINE <ENTER>=SI <ESC>=NO",13,27);
											if( tecla == 13 ) {
												opcion = 2;
												falla_anulacion =NO;
											} else {
												falla_anulacion = NO; //ojo con este que agregue
												cancela_anula = SI;//ojo
												opcion = 0;
											}
									}
								}
								
								if( opcion == 0 || ( opcion == 2 && 
									!AUTORIZACION_ON_LINE( _ON_LINE_ANULACION, _PAGO_IMPORTE( h ),
									( char* )&rta_terminal, sizeof( rta_terminal ) ) ) ){
										int reintentar = 0;

										MENSAJE_TECLA( "DEBE LLAMAR AL SUPERVISOR PARA ANULAR LA TARJETA - PRES. TECLA" );
										while( reintentar < 3 ) {
											if( SOLICITAR_CLAVES( 50, 20, NO, SI, SI, "Anulacion de Tarjeta",
												"Anulacion de Tarjeta", NO, SI ) ) {
													reintentar = 3;
													rta_terminal.cod_rta = 1;
													if (cancela_anula != SI)
														falla_anulacion = SI;
													rta = 1;
											} else {
												reintentar ++;
												rta = 0;
											}
										}	
								}
								if( ( opcion != 2 )
									&& ( opcion == 1 || ( RAM_ENVIAR_OFFLINE && !VAR_TARJETAS_UNICAMENTE_OL ) ) ) {
										rta_terminal.cod_rta = 1; 
								}
						}
						BORRAR_MENSAJE();
					} while( opcion == 2 && rta_terminal.cod_rta == 1 && VAR_TARJETAS_CELDA_PINPAD > 0 && VAR_TARJETAS_UNICAMENTE_OL);
					if( rta_terminal.cod_rta == 0 ) {
						SET_MEMORY( __var_tarjetas_autorizacion, rta_terminal.autorizacion );
                        SET_MEMORY_STR_CHAR( __var_tarjetas_autorizacion, 10, 0 );
                        _SET_MEMORY( __pago_auto_tarjeta, h, VAR_TARJETAS_AUTORIZACION );
						falla_anulacion = NO;

                    }else {/*-------- Si hay error de comunicacion ---------------
						   si o si indicar rta = 0 sino permite quedar la operacion
						   de compra sin anular y	no queda el medio de pago en pantalla */
						rta = 0;
						/*rta_terminal.cod_rta == 1 --> ERROR DE COMUNICACION*/
						if (rta_terminal.cod_rta == 1 && condicion_anulacion > 0 &&
							!VAR_TARJETAS_UNICAMENTE_OL && rta == 1){
								//mmancuso aca la manda off si o si y no pregunta is quiere mandar
								if( PIDE_AUTORIZACION_SI_NECESITA( h, NO) ) {
									rta = 1;
									GRABAR_OFF_LINE( _OFF_LINE_ANULACION, _PAGO_IMPORTE( h ) );
									falla_anulacion = NO;
								}
						}
					}
					if( rta_terminal.cod_rta == 1 && tecla == 27 && VAR_TARJETAS_CELDA_PINPAD > 0) {
						/*tarjeta de debito que fallo la anulacion
						deberiamos reversar la compra original??*/
						VACIAR_KEY();
						MENSAJE_TECLA( "ERROR- NO SE PUDO ANULAR LA COMPRA TARJETA DEBITO - PRES. TECLA" );
						rta = 0;
						falla_anulacion =SI;
					}
				}else {
					
					/* "Tenemos que anular y pedimos la tarjeta" como
					no tengo retrieval reference number por error de comunicacion
					para anulaciones off tambien debe debe pedir la tarjeta a menos que sea de debito o unicamente Ol*/
					if( cancela_anula = SI )
						rta=0;

					if(( VAR_TARJETAS_DATAFONO == 1 ) && 
						(!( VAR_TARJETAS_CELDA_PINPAD > 0 || VAR_TARJETAS_UNICAMENTE_OL))) {
							PIDE_TARJETA_PARA_ANULAR( &cancela_anula );
							rta = 1;
							falla_anulacion = NO;
							GRABAR_OFF_LINE( _OPERACION_ANULADA, _PAGO_IMPORTE( h ) );
					}
				}//fin else 
			}//fin del exige autorizacion online
		}
	}
	// cuando genera la anulacion incrementa de a 2 nro de ticket
    //SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int, CALCULAR_NRO_CUPON( VAR_TARJETAS_TARJETA ) );
	if( !cancela_anula ) {
		_SET_MEMORY_LONG( __pago_nro_cupon_int1, h, VAR_TARJETAS_NRO_CUPON_INT );
	} else {
		rta = 0;
	}

	if( VARIAS_TARJETAS_POR_PAGO ) {
		
		if ( rta != 0 )
			GRABA_MODIFICACION_TARJETAS( posicion - 1, TARJETA_TRADICIONAL );
		
		/*Sino esta todo ok,encolamos la anulacion, 
			SINO (es decir no se realiza la anulacion) --> se desencola voucher agregado inicialmente*/
		
		if( falla_anulacion == NO && cancela_anula == NO){
			ENCOLAR_VOUCHER_TARJETA(  );
		} else {
			/*siempre que no se haya realizar la anulacion ON u OFF desencolar voucher
			teoricamente borra el ultimo agregado, entonces no haria falta revisar si corresponde a la tarjeta
			en operacion..... o si?*/
			BORRAR_VOUCHER_ENCOLADOS(1); 
		}
	}
	return (rta);
}

/****************************************************************************************/
void MODIFICAR_VOUCHER_ENCOLADOS( void )
/*MODIFICAR UNOS DATOS DE LOS VOUCHERS ENCOLADOS DESPUES DE UNA RESPUESTA DEL JAUTON
*/
/****************************************************************************************/
{
  int       handle_voucher;
   char     *buffer_voucher = NULL;
//SE PODRIA CONTROLAR EL NRO DE LA TARJETA PARA VERIFICAR SI ES ESA
   handle_voucher = _OPEN_O_CREAT( "voucher.$$$", O_RDWR, __LINE__, __FILE__ );
   if( handle_voucher > 0 ) {
      buffer_voucher = MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
      if( buffer_voucher ) {
		  PASAR_PRIVADOS_A_DATOS_TARJETA();
		  COPY_STRUCT_TO_BUFFER( buffer_voucher, _var_tarjetas );
		 //_lseek( handle, posicion * ( SIZEOF_STRUCT( _var_tarjetas ) ), 0 );
         LSEEK( handle_voucher, -1 * ( SIZEOF_STRUCT( _var_tarjetas ) ), SEEK_END, __LINE__, __FILE__ );
         _WRITE( handle_voucher, ( char * ) buffer_voucher, SIZEOF_STRUCT( _var_tarjetas ),__LINE__, __FILE__ );
		 FREEMEM( buffer_voucher );
         buffer_voucher = 0;
      }
	  _close( handle_voucher );
   }
}

/****************************************************************************************/
void BORRAR_VOUCHER_ENCOLADOS( int cant )
/*BORRAR VOUCHERS ENCOLADOS DESPUES DE UNA RESPUESTA DEL JAUTON*/

/****************************************************************************************/
{
  int       handle_voucher;
   char     *buffer_voucher = NULL;
// CONTROLAR EL POSICIONAMIENTO O  TARJETA
   handle_voucher = _OPEN_O_CREAT( "voucher.$$$", O_RDWR, __LINE__, __FILE__ );
   if( handle_voucher > 0 ) {// si el tamaño es igual al total conviene borrar el archivo
      //buffer_voucher = MALLOC( SIZEOF_STRUCT( _var_tarjetas )) ;
	  buffer_voucher = MALLOC( SIZEOF_STRUCT( _var_tarjetas )* cant ) ;
      if( buffer_voucher ) {
		 memset(buffer_voucher,0, SIZEOF_STRUCT( _var_tarjetas ) * cant );
//         LSEEK( handle_voucher, -1 * ( SIZEOF_STRUCT( _var_tarjetas ) ), SEEK_END, __LINE__, __FILE__ );
		 LSEEK( handle_voucher, -cant * ( SIZEOF_STRUCT( _var_tarjetas ) ), SEEK_END, __LINE__, __FILE__ );
//         _WRITE( handle_voucher, ( char * ) buffer_voucher, SIZEOF_STRUCT( _var_tarjetas ) ,__LINE__, __FILE__ );
		 _WRITE( handle_voucher, ( char * ) buffer_voucher, SIZEOF_STRUCT( _var_tarjetas ) * cant ,__LINE__, __FILE__ );
		 FREEMEM( buffer_voucher );
         buffer_voucher = 0;
      }//
	  _close( handle_voucher );
   }
}
