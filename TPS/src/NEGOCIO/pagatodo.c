/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * PagaTodo - 2007 - Marcos Carranza
 * ------------------------------------------------------------------------------
 */
#include <config.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <fcntl.h>
#include <path.h>
#include <_cr0.h>
#pragma pack(1)
#include <cr.h>
#include <aarch.h>
#include <dbrouter.h>
#include <memory.h>
#include <btrv.h>
#include <stdio.h>
#include <sys/stat.h>
#if defined(INVEL_L)
#include <string.h>
#include <actions.h>
#include <sys/io.h>
#endif
#if defined(INVEL_W)
#include <io.h>
#include <dos.h>
#include <string.h>
#include <actions.h>
#include <tpv_dat.h>
#endif
#include <typeacti.h>
#include "pagatodo.h"
#include <recupero.h>
#include <stack.h>
#include <cmemory.h>
#include <pagos.h>
#include <mensaje.h>
#include <round.h>
#include <cobros.h>
#include <clave.h>
#include <get_fech.h>
#include <get_hora.h>
#include <actual.h>
#include <_cr1.h>
#include <b_mens.h>
#include <p_float.h>
#include <p_fecha.h>
#include <deftables.h>
#include <db.h>
#include <valida.h>
#include <trim.h>
//*****************************************************************************
int COBROS_PAGATODO( char modo, char devolucion )
//PAGATODO_RECARGA    1 
//PAGATODO_VENTA_PIN  2
//PAGATODO_SERVICIOS  3
//*****************************************************************************
{
    int       _ptr, _handle, _estado, panel_activado = 0, ok = 0;
    struct    DatosCobroPagaTodo datosCobros;
	
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );
    
    if( _ESTADO != 0 ) {
		RECUPERAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros, 
		    sizeof( struct DatosCobroPagaTodo ), 0 );
	}
	if( _ESTADO == 1 ) {
		PANTALLA_COBROS(  );
		/*ExecuteAction( A_ACTIVAR_PANEL_PAGATODO );
		ExecuteSAction( A_MUESTRA_TIT_IMPORTE_PAGATODO, ST( S_Importe ) );
        panel_activado = 1;*/
		UNGETCH( 13 );
		ELEGIR_COBRO_BROWSE( _MODO_INTERACTIVO, NO );
        MOSTRAR_DATOS_SOLICITADOS_EN_COBROS( cobros.datos_pedir, COBRO_PAGATODO );
	}
	if( _ESTADO == 0 ) {
		POSICIONAR_COBRO_SELECCIONADO(  );
		BORRAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO );
		memset( &datosCobros, 0x00, sizeof( struct DatosCobroPagaTodo ) );
		datosCobros._modo = modo;
		datosCobros.__devolucion = devolucion;
		strncpy( datosCobros._cod_producto, cobros.cod_item_compania, 
				sizeof( datosCobros._cod_producto ) /*- 1*/ );
		datosCobros._importe = cobros.importe;
		datosCobros._costo_servicio = cobros.costo_servicio;
		datosCobros._iva_costo_servicio = cobros.iva_costo_servicio;
		datosCobros._codigo = cobros.codigo;
		datosCobros._cod_valida = cobros.cod_valida;
		strncpy( datosCobros._nombre_cobro, cobros.nombre, 
				sizeof( datosCobros._nombre_cobro ) /*- 1*/ );
        datosCobros._datos_pedir = cobros.datos_pedir;
		GUARDAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros, 
			sizeof( struct DatosCobroPagaTodo ) );
		_SET_ESTADO( 1 );
	}
	if( _ESTADO == 1 ) {
		int  rta;

		/*if( panel_activado == 0 ) {
			ExecuteAction( A_ACTIVAR_PANEL_PAGATODO );
			ExecuteSAction( A_MUESTRA_TIT_IMPORTE_PAGATODO, ST( S_Importe ) );
		}*/
		rta = PEDIR_DATOS_PAGATODO( &datosCobros );
	    //ExecuteAction( A_DESACTIVAR_PANEL_PAGATODO );
		if( devolucion ) {
			ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_DEV_COBRO_BORRAR, " ",
				" ", NULL, " " );
		} else {
			ExecuteSSAction( A_MUESTRA_ICON_COBRO, T_ICON_COBRO_BORRAR, " ",
				" ", NULL, " " );
		}
		if( rta ) {
			if( !RAM_FLAG_COBRO_VENTA )
			SET_MEMORY_DOUBLE( __ram_total, datosCobros._importe 
				+ datosCobros._costo_servicio + datosCobros._iva_costo_servicio );
			ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
				sizeof( struct DatosCobroPagaTodo ), 0 );
			
			if( RAM_FLAG_COBRO_VENTA && !RAM_ANULACION )
				GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros, 
					sizeof( struct DatosCobroPagaTodo ) );
		   
			_SET_ESTADO( 2 );       
		} else {
			datosCobros._anulacion = 1;
			ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
				sizeof( struct DatosCobroPagaTodo ), 0 );
			_SET_ESTADO( 3 );
		}
	}
    if( _ESTADO == 2 ) {
		if( !RAM_FLAG_COBRO_VENTA ){
			if( !PAGOS( _MODO_COBRANZA, F_PAGATODO_COBRO, devolucion ) ) {
				RECUPERAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros, 
					sizeof( struct DatosCobroPagaTodo ), 0 );
				datosCobros._anulacion = 1;
				ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
					sizeof( struct DatosCobroPagaTodo ), 0 );
			} else {
				RECUPERAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros, 
					sizeof( struct DatosCobroPagaTodo ), 0 );
			}
			_SET_ESTADO( 3 ); //Todo ok.
		}else{
			int rta = 0;
			if( rta = AGREGAR_ARTICULO_COBRO_VENTA( cobros.cod_barra, cobros.cod_interno,
					datosCobros._importe + datosCobros._costo_servicio + datosCobros._iva_costo_servicio, 
						datosCobros._codigo ) ){
				SET_MEMORY_CHAR( __ram_hay_cobro_venta, 1 );	
				//GRABAR_MEDIOS_COBRO( datosCobros._codigo , datosCobros._importe );
			}
			if( RAM_ANULACION || !rta ){
				CANCELAR_COBRO_VENTA( (void*)&datosCobros, COBRO_PAGATODO );
				SET_MEMORY_INT( __ram_anulacion, 0 );
			}
			CR1_INICIALIZAR_VAR_TICKET( SI );
			SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
			//ExecuteAction( A_DES_COBROS_ACT_VENTAS );
			CLOSE_RECUPERO( _ptr, _handle );
			return 1;
		}
	}
	if( _ESTADO == 3 ) {
		if( datosCobros._anulacion == 0 ) {
			char    nip[17];
		
			strcpy( nip, datosCobros._nip );
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
			if( RAM_FLAG_COBRO_VENTA ){
				SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
				CLOSE_RECUPERO( _ptr, _handle );
				//ExecuteAction( A_DES_COBROS_ACT_VENTAS );
				return ok;
			}
		}
		 _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        //Graba el evento
        GRABAR_EVENTO_PAGATODO( &datosCobros );
		_SET_ESTADO( 5 );
    }
	
	if( _ESTADO == 5 ) { //si se dio escape y se anula el cobro datosCobros._anulacion == 1
        ExecuteAction( A_DES_COBROS_ACT_VENTAS );
		ExecuteAction( A_DES_PAGOS_ACT_VENTAS ); 
		_SET_ESTADO( 6 );
    }
	CLOSE_RECUPERO( _ptr, _handle );
	return( ok );
}
//-- El parametro es para mantener la compatibilidad - No sacar
/*****************************************************************************/
int F_PAGATODO_COBRO( double importe )
/*****************************************************************************/
{
    int      rta = 1, operacionNro;
    struct   DatosCobroPagaTodo datosCobros;
	char     *nombreOperacion[] = { "PAGO", "REVERSA", "DEVOLUCION" };
	char     mensajeAux[81];
    //--------Obtengo estructura para enviarla
	memset( &datosCobros, 0x00, sizeof( struct DatosCobroPagaTodo ) );
	RECUPERAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros, 
			sizeof( struct DatosCobroPagaTodo ), 0 );
	if( datosCobros._rta_envio != OPERACION_FINALIZADA ) {
		//--------Modifico estado de envio a enviando
		//TRANSACCION_SIN_ENVIAR 0 - TRANSACCION_INICIAL_ENVIANDO 1 - TRANSACCION_INICIAL_ENVIADA 2
		//TRANSACCION_REVERSO_ENVIANDO 3 - TRANSACCION_REVERSO_ENVIADA 4
		//Estados por corte de luz. 
		//1 y 2 sirven para transacciones de pago y devolucion (primera iteracion)
		//3 y 4 sirven para transacciones de reverso y devolucion (desde la 2da. iteracion).
		if( datosCobros._estado_envio == TRANSACCION_SIN_ENVIAR ) {
			datosCobros._estado_envio = TRANSACCION_INICIAL_ENVIANDO;
			datosCobros._rta_envio = 0;
			ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
				sizeof( datosCobros ), 0 );
		}
		operacionNro = ( datosCobros.__devolucion ) ? PAGATODO_DEVOLUCION : PAGATODO_PAGO;
		sprintf( mensajeAux, "ENVIANDO TRANSACCION DE %s", nombreOperacion[operacionNro-1] );
		MENSAJE( mensajeAux );
		rta = ENVIAR_TRANSACCION_PAGATODO( operacionNro, &datosCobros );
		datosCobros._estado_envio = TRANSACCION_INICIAL_ENVIADA;
		datosCobros._rta_envio = ( char ) rta;
		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
			sizeof( datosCobros ), 0 );
		if( rta == OPERACION_NECESITA_REVERSO ) {
			int   intentos = REINTENTOS_PAGATODO_REVERSA/*REINTENTOS_PAGATODO*/;

			operacionNro = ( datosCobros.__devolucion ) ? PAGATODO_DEVOLUCION : PAGATODO_REVERSA;
			sprintf( mensajeAux, "ENVIANDO TRANSACCION DE %s", nombreOperacion[operacionNro-1] );
			do {
				MENSAJE( mensajeAux );
				datosCobros._estado_envio = TRANSACCION_REVERSO_ENVIANDO;
				ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
					sizeof( datosCobros ), 0 );
				rta = ENVIAR_TRANSACCION_PAGATODO( operacionNro, &datosCobros );
				datosCobros._estado_envio = TRANSACCION_REVERSO_ENVIADA;
				datosCobros._rta_envio = ( char ) rta;
				ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
					sizeof( datosCobros ), 0 );
				intentos--;
			} while( rta < 0 && intentos );
			if( rta ) {
				sprintf( mensajeAux, "ERROR AL ENVIAR %s", nombreOperacion[operacionNro-1] );
				MENSAJE( mensajeAux );
			}
			if( !intentos ) {
				while( !( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI,
						SI, mensajeAux, "AUTORIZA CONTINUAR",
						_F_SUP_REVERSO_PAGATODO_ERROR, SI ) ) );
				UNGETCH( 27 );
				rta = -1;
			} else {
				rta = -1;  //para que permita reenviar en pagos.
			}
		}
		if( rta == 0 ) {
			datosCobros._rta_envio = OPERACION_FINALIZADA;   //envio de transacciones finalizado
		} else {
			datosCobros._estado_envio = TRANSACCION_SIN_ENVIAR;
			datosCobros._rta_envio = 0;
		}
		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobros,
			sizeof( datosCobros ), 0 );
	} else {
		rta = 0;
	}
    return( !rta );
}
int F_PAGATODO_COBRO_VENTA( double importe )
/*****************************************************************************/
{
    int      rta = 1, operacionNro;
    struct   DatosCobroPagaTodo datosCobros;
	char     *nombreOperacion[] = { "PAGO", "REVERSA", "DEVOLUCION" };
	char     mensajeAux[81];

    //--------Obtengo estructura para enviarla
	memset( &datosCobros, 0x00, sizeof( struct DatosCobroPagaTodo ) );
	RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros, 
			sizeof( struct DatosCobroPagaTodo ), 0 );
	if( datosCobros._rta_envio != OPERACION_FINALIZADA ) {
		//--------Modifico estado de envío a enviando
		//TRANSACCION_SIN_ENVIAR 0 - TRANSACCION_INICIAL_ENVIANDO 1 - TRANSACCION_INICIAL_ENVIADA 2
		//TRANSACCION_REVERSO_ENVIANDO 3 - TRANSACCION_REVERSO_ENVIADA 4
		//Estados por corte de luz. 
		//1 y 2 sirven para transacciones de pago y devolución (primera iteración)
		//3 y 4 sirven para transacciones de reverso y devolución (desde la 2da. iteración).
		if( datosCobros._estado_envio == TRANSACCION_SIN_ENVIAR ) {
			datosCobros._estado_envio = TRANSACCION_INICIAL_ENVIANDO;
			datosCobros._rta_envio = 0;
			ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros,
				sizeof( datosCobros ), 0 );
		}
		operacionNro = ( datosCobros.__devolucion ) ? PAGATODO_DEVOLUCION : PAGATODO_PAGO;
		sprintf( mensajeAux, "ENVIANDO TRANSACCION DE %s", nombreOperacion[operacionNro-1] );
		MENSAJE( mensajeAux );
		rta = ENVIAR_TRANSACCION_PAGATODO( operacionNro, &datosCobros );
		datosCobros._estado_envio = TRANSACCION_INICIAL_ENVIADA;
		datosCobros._rta_envio = ( char ) rta;
		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros,
			sizeof( datosCobros ), 0 );
		if( rta == OPERACION_NECESITA_REVERSO ) {
			int   intentos = REINTENTOS_PAGATODO_REVERSA;

			operacionNro = ( datosCobros.__devolucion ) ? PAGATODO_DEVOLUCION : PAGATODO_REVERSA;
			sprintf( mensajeAux, "ENVIANDO TRANSACCION DE %s", nombreOperacion[operacionNro-1] );
			do {
				MENSAJE( mensajeAux );
				datosCobros._estado_envio = TRANSACCION_REVERSO_ENVIANDO;
				ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros,
					sizeof( datosCobros ), 0 );
				rta = ENVIAR_TRANSACCION_PAGATODO( operacionNro, &datosCobros );
				datosCobros._estado_envio = TRANSACCION_REVERSO_ENVIADA;
				datosCobros._rta_envio = ( char ) rta;
				ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros,
					sizeof( datosCobros ), 0 );
				intentos--;
			} while( rta < 0 && intentos );
			if( rta ) {
				sprintf( mensajeAux, "ERROR AL ENVIAR %s", nombreOperacion[operacionNro-1] );
				MENSAJE( mensajeAux );
			}
			if( !intentos ) {
				while( !( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI,
						SI, mensajeAux, "AUTORIZA CONTINUAR",
						_F_SUP_REVERSO_PAGATODO_ERROR, SI ) ) );
				UNGETCH( 27 );
				rta = -1;
			} else {
				rta = -1;  //para que permita reenviar en pagos.
			}
		}
		if( rta == 0 ) {
			datosCobros._rta_envio = OPERACION_FINALIZADA;   //envío de transacciones finalizado
		} else {
			datosCobros._estado_envio = TRANSACCION_SIN_ENVIAR;
			datosCobros._rta_envio = 0;
		}
		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_PAGATODO, ( char * ) &datosCobros,
			sizeof( datosCobros ), 0 );
	} else {
		rta = 0;
	}

    return( !rta );
}

//*****************************************************************************
int ENVIAR_TRANSACCION_PAGATODO( int modo, struct DatosCobroPagaTodo *datosCobros )
//*****************************************************************************
{
    int    ok = 0, procesar = 1, l_buffer = 0;
    char   buffer[SIZE_BUFFER_TCP];
    memset( &buffer,  0x00, sizeof( buffer ) );  //Inicializo el buffer
    if( ABRIR_CANAL_JGATEWAY(  ) ) {
        //Preparo el paquete a comunicar.
        PREPARAR_ENVIO_PAGATODO( modo, datosCobros, buffer, &l_buffer );
		if( ENVIAR_COMANDO_JGATEWAY( buffer, l_buffer, REINTENTOS_PAGATODO, 0 ) ) {
            ok = PROCESAR_RESPUESTA_PAGATODO( modo, buffer, datosCobros );
        } else {
		    ok = -2;
        }
    } else {
        MENSAJE( "ERROR AL CONECTAR CON JGATEWAY" );
		ok = -1; //Abortar operacion por falta de comunicacion - no necesita reverso
	}
  
    return ok;
}
//*****************************************************************************
int PREPARAR_ENVIO_PAGATODO( int comando, struct DatosCobroPagaTodo *datosCobros, 
		char *buffer, int *l_buffer )
//*****************************************************************************
{
    int ok = 1;
    
	switch( comando ) {
        case PAGATODO_PAGO:
            {
		        struct _cons_pagatodo_pago_req pagatodo_req;
				memset( &pagatodo_req, 0x00, sizeof( struct _cons_pagatodo_pago_req ) );
				pagatodo_req.cab.producto = PAGATODO;
				pagatodo_req.cab.servicio = PAGATODO_PAGO;
				pagatodo_req.cab.version  = V_PAGATODO_PAGO_REQ;
				pagatodo_req.sucursal     = config.sucursal;
				pagatodo_req.caja         = NRO_CAJA;
                pagatodo_req.id_evento    = RAM_ID_EVENTO;
				pagatodo_req.caja_z       = ( long ) ( NRO_CAJA ) * 100000L + ( NRO_Z % 100000L );
				pagatodo_req.fecha        = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
				pagatodo_req.hora         = GET_HORA_COMPLETA_DOS(  );
				strncpy( pagatodo_req.codigo_producto, datosCobros->_cod_producto, sizeof( pagatodo_req.codigo_producto ) /*- 1*/ );
				if( datosCobros->_modo == PAGATODO_VENTA_PIN ) {
				    sprintf( pagatodo_req.cuenta_tel, "%40.40s", datosCobros->_cod_producto );		
				} else {
					strncpy( pagatodo_req.cuenta_tel, datosCobros->_cuenta, sizeof( pagatodo_req.cuenta_tel ) /*- 1*/ );
				}
				strncpy( pagatodo_req.llavero, datosCobros->_codClientePagatodo, sizeof( pagatodo_req.llavero ) /*- 1*/ );
				pagatodo_req.importe      = ROUND( datosCobros->_importe, ENTEROS, DECIMALES );
				pagatodo_req.importe_costo_servicio      = ROUND( datosCobros->_costo_servicio, ENTEROS, DECIMALES );
				pagatodo_req.importe_IVA_costo_servicio  = ROUND( datosCobros->_iva_costo_servicio, ENTEROS, DECIMALES );
				datosCobros->_fecha = pagatodo_req.fecha;
				datosCobros->_hora = pagatodo_req.hora;
				datosCobros->_caja_original = NRO_CAJA;
				datosCobros->_sucursal_original = config.sucursal; 
				datosCobros->_fecha_original = datosCobros->_fecha;
				datosCobros->_hora_original = datosCobros->_hora;
		  		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros, sizeof( struct DatosCobroPagaTodo ), 0 );
				
				*l_buffer = sizeof( struct _cons_pagatodo_pago_req );
				memcpy( buffer, &pagatodo_req, *l_buffer );
			}
			break;
		case PAGATODO_REVERSA:
		    {
		        struct _cons_pagatodo_reversa_req pagatodo_req;
				memset( &pagatodo_req, 0x00, sizeof( struct _cons_pagatodo_reversa_req ) );
				pagatodo_req.cab.producto = PAGATODO;
				pagatodo_req.cab.servicio = PAGATODO_REVERSA;
				pagatodo_req.cab.version  = V_PAGATODO_REVERSA_REQ;
				pagatodo_req.sucursal     = config.sucursal;
				pagatodo_req.caja         = NRO_CAJA;
                pagatodo_req.id_evento    = RAM_ID_EVENTO;
				pagatodo_req.caja_z       = ( long ) ( NRO_CAJA ) * 100000L + ( NRO_Z % 100000L );
				pagatodo_req.fecha        = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
				pagatodo_req.hora         = GET_HORA_COMPLETA_DOS(  );
				strncpy( pagatodo_req.nip, datosCobros->_nip, sizeof( pagatodo_req.nip ) /*- 1*/ );
				strncpy( pagatodo_req.confirmacion, datosCobros->_confirmacion_original, 
						sizeof( pagatodo_req.confirmacion ) /*- 1*/ );
				pagatodo_req.sucursal_original = datosCobros->_sucursal_original;
				pagatodo_req.caja_original = datosCobros->_caja_original;
				pagatodo_req.fecha_original = datosCobros->_fecha_original;
				pagatodo_req.hora_original = datosCobros->_hora_original;
				pagatodo_req.numero_transaccion = datosCobros->_numero_transaccion;
				
				*l_buffer = sizeof( struct _cons_pagatodo_reversa_req );
				memcpy( buffer, &pagatodo_req, *l_buffer );
			}
			break;
		case PAGATODO_DEVOLUCION:
            {
		        struct _cons_pagatodo_reversa_req pagatodo_req;
				memset( &pagatodo_req, 0x00, sizeof( struct _cons_pagatodo_reversa_req ) );
				pagatodo_req.cab.producto = PAGATODO;
				pagatodo_req.cab.servicio = PAGATODO_DEVOLUCION;
				pagatodo_req.cab.version  = V_PAGATODO_DEVOLUCION_REQ;
				pagatodo_req.sucursal     = config.sucursal;
				pagatodo_req.caja         = NRO_CAJA;
                pagatodo_req.id_evento    = RAM_ID_EVENTO;
				pagatodo_req.caja_z       = ( long ) ( NRO_CAJA ) * 100000L + ( NRO_Z % 100000L );
				pagatodo_req.fecha        = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
				pagatodo_req.hora         = GET_HORA_COMPLETA_DOS(  );
				pagatodo_req.sucursal_original = datosCobros->_sucursal_original;
				pagatodo_req.caja_original     = datosCobros->_caja_original;
				pagatodo_req.fecha_original    = datosCobros->_fecha_original;
				pagatodo_req.hora_original     = datosCobros->_hora_original;
				pagatodo_req.importe      = ROUND( datosCobros->_importe, ENTEROS, DECIMALES );
				pagatodo_req.importe_costo_servicio      = ROUND( datosCobros->_costo_servicio, ENTEROS, DECIMALES );
				pagatodo_req.importe_IVA_costo_servicio  = ROUND( datosCobros->_iva_costo_servicio, ENTEROS, DECIMALES );
				datosCobros->_fecha = pagatodo_req.fecha;
				datosCobros->_hora = pagatodo_req.hora;
		  		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros,
					sizeof( datosCobros ), 0 );
				
				*l_buffer = sizeof( struct _cons_pagatodo_reversa_req );
				memcpy( buffer, &pagatodo_req, *l_buffer );
			}
			break;
		default:
			ok = 0;
			break;
   }
   return( ok );
}
//***************************************************************************************************
int PROCESAR_RESPUESTA_PAGATODO( int comando, char *buffer, 
	struct DatosCobroPagaTodo *datosCobros )
//***************************************************************************************************
{
    int    rta = 0;
	char   mensaje[73];
    switch( comando ) {
    case PAGATODO_PAGO:
	    {
			struct _cons_pagatodo_pago_rta pagatodo_pago_rta;
		    memcpy( &pagatodo_pago_rta, buffer, sizeof( struct _cons_pagatodo_pago_rta ) );
			strncpy( datosCobros->_confirmacion_original, pagatodo_pago_rta.confirmacion, 
					sizeof( datosCobros->_confirmacion_original ) /*- 1*/ );
			datosCobros->_numero_transaccion = pagatodo_pago_rta.numero_transaccion;
			strncpy( datosCobros->_transaccion_pagatodo, pagatodo_pago_rta.transaccion_pagatodo, 
					sizeof( datosCobros->_transaccion_pagatodo ) /*- 1*/ );
			strncpy( datosCobros->_nip, pagatodo_pago_rta.nip, 
					sizeof( datosCobros->_nip ) /*- 1*/ );
			strncpy( datosCobros->_folio, pagatodo_pago_rta.folio, 
					sizeof( datosCobros->_folio ) /*- 1*/ );
			//si cuenta estaba vacia pero viene en la respuesta la grabo en datosCobro.
			//habra problema si se reintenta un envio y se envia llavero y cuenta de tel?
			if( strlen( datosCobros->_cuenta ) == 0 ) {
				INT16  posicion;
				
				for( posicion = 0; posicion <= sizeof( pagatodo_pago_rta.cuenta_tel ) 
						&& pagatodo_pago_rta.cuenta_tel[posicion] == '0'; posicion++ );
				strncpy( datosCobros->_cuenta, &pagatodo_pago_rta.cuenta_tel[posicion], 
						sizeof( pagatodo_pago_rta.cuenta_tel ) - posicion - 1 );
			}
			rta = pagatodo_pago_rta.codigo_error;
			ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros,
					sizeof( struct DatosCobroPagaTodo ), 0 );
			strncpy( mensaje, pagatodo_pago_rta.mensaje_error, sizeof( mensaje ) );
			if( rta == 0 ) {
				int   i, /*a_ant*/ tabla_ant, tipo_ant;
				struct _lin_mens lin_mens;
				//a_ant = SELECTED(  );
				tabla_ant = SELECTED2();
				tipo_ant = SELECTED_TIPO();
				//if( USE_DATABASE( AREA_LIN_MENS, "lin_mens.tmp", ( char * ) &lin_mens, sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {
				if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens, sizeof( struct _lin_mens))== 0 ) {
					char  cantidad_total_lineas = 0;
					char  nro_cuerpo = 0;
					//memcpy( &rta_confirmacion, buffer, sizeof( rta_confirmacion ) );
					cantidad_total_lineas = pagatodo_pago_rta.cant_1 +
						pagatodo_pago_rta.cant_2;
					for( i = 0; i < cantidad_total_lineas; i++ ) {
						if( ( nro_cuerpo == 0 && ( i > ( pagatodo_pago_rta.cant_1 - 1 ) ) )
								/*|| ( nro_cuerpo == 1 && ( i > ( pagatodo_pago_rta.cant_2 - 1 ) ) )*/ )
							nro_cuerpo++;
						lin_mens.nro_cuerpo = nro_cuerpo;
						lin_mens.id_transaccion = 0;//datosCobros->_numero_transaccion;
						lin_mens.nro_linea = i;
						memcpy( lin_mens.descripcion, &buffer[sizeof( struct _cons_pagatodo_pago_rta ) +
							( i * sizeof( lin_mens.descripcion ) )],
							sizeof( lin_mens.descripcion ) );
						INSERT(  );
					}
					//CLOSE_DATABASE( AREA_LIN_MENS );
					CLOSE_TABLE( T_LIN_MENS_TMP, TT_TEMP );
				}
				//SELECT( a_ant );
				SELECT_TABLE( tabla_ant, tipo_ant );
			}
		}
        break;
    case PAGATODO_REVERSA:
        {
			struct _cons_pagatodo_reversa_rta pagatodo_reversa_rta;
			memcpy( &pagatodo_reversa_rta, buffer, sizeof( struct _cons_pagatodo_reversa_rta ) );
			//ver si me hace falta algun dato de la respuesta para imprimir!!!!
			strncpy( mensaje, pagatodo_reversa_rta.mensaje_error, sizeof( mensaje ) );
			rta = pagatodo_reversa_rta.codigo_error;
		}
        break;
	case PAGATODO_DEVOLUCION:
		{
			struct _cons_pagatodo_reversa_rta pagatodo_reversa_rta;
		    memcpy( &pagatodo_reversa_rta, buffer, sizeof( struct _cons_pagatodo_reversa_rta ) );
			//ver si me hace falta algun dato de la respuesta para imprimir!!!!
			strncpy( datosCobros->_confirmacion_original, pagatodo_reversa_rta.confirmacion, 
					sizeof( datosCobros->_confirmacion_original ) /*- 1*/ );
			strncpy( datosCobros->_transaccion_pagatodo, pagatodo_reversa_rta.transaccion_pagatodo, 
					sizeof( datosCobros->_transaccion_pagatodo ) /*- 1*/ );
			strncpy( datosCobros->_cuenta, pagatodo_reversa_rta.cuenta_tel, 
					sizeof( datosCobros->_cuenta ) );
			strncpy( datosCobros->_folio, pagatodo_reversa_rta.folio, 
					sizeof( datosCobros->_folio ) );
			ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros,
					sizeof( struct DatosCobroPagaTodo ), 0 );
			strncpy( mensaje, pagatodo_reversa_rta.mensaje_error, sizeof( mensaje ) );
			rta = pagatodo_reversa_rta.codigo_error;
			if( rta == 0 ) {
				int   i, /*a_ant,*/ tabla_ant, tipo_ant;
				struct _lin_mens lin_mens;
				//a_ant = SELECTED(  );
				tabla_ant = SELECTED2();
				tipo_ant = SELECTED_TIPO();
				//if( USE_DATABASE( AREA_LIN_MENS, "lin_mens.tmp", ( char * ) &lin_mens, sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {
				if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens, sizeof( struct _lin_mens))== 0 ) {
					char  cantidad_total_lineas = 0;
					char  nro_cuerpo = 0;
					
					cantidad_total_lineas = pagatodo_reversa_rta.cant_1 +
						pagatodo_reversa_rta.cant_2;
					for( i = 0; i < cantidad_total_lineas; i++ ) {
						if( ( nro_cuerpo == 0 && ( i > ( pagatodo_reversa_rta.cant_1 - 1 ) ) )
								/*|| ( nro_cuerpo == 1 && ( i > ( pagatodo_reversa_rta.cant_2 - 1 ) ) )*/ )
							nro_cuerpo++;
						lin_mens.nro_cuerpo = nro_cuerpo;
						lin_mens.id_transaccion = 0/*datosCobros->_numero_transaccion*/;
						lin_mens.nro_linea = i;
						memcpy( lin_mens.descripcion, &buffer[sizeof( struct _cons_pagatodo_reversa_rta ) +
							( i * sizeof( lin_mens.descripcion ) )],
							sizeof( lin_mens.descripcion ) );
						INSERT(  );
					}
					//CLOSE_DATABASE( AREA_LIN_MENS );
					CLOSE_TABLE( T_LIN_MENS_TMP, TT_TEMP );
				}
				//SELECT( a_ant );
				SELECT_TABLE( tabla_ant, tipo_ant);
			}
		}
	    break;
    }
	if( rta ) {
		MENSAJE( mensaje );
	}
	return rta;
}
//*****************************************************************************
void GRABAR_EVENTO_PAGATODO( struct DatosCobroPagaTodo *datos_cobros )
//*****************************************************************************
{
	char *cod_barra = NULL;
	memset( &event_ticket, 0, sizeof( struct _eventos ) );
	event_ticket.cod_cobro = datos_cobros->_codigo;
	event_ticket.importe_sin_iva = datos_cobros->_importe + datos_cobros->_costo_servicio 
		+ datos_cobros->_iva_costo_servicio;
	if( datos_cobros->__devolucion ) {
		event_ticket.importe_sin_iva *= -1;
	}
	event_ticket.tipo_evento = 
		( datos_cobros->_anulacion ) ? 99 : ( datos_cobros->__devolucion ) ? 61 : 60;
	event_ticket.hora_inicio = RAM_HORA_INICIO_EVENTO;
	event_ticket.nro_comprobante_new = 0;
    event_ticket.nro_ticket = 0;
    SETEAR_TIEMPO( _INDEFINIDO );
    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
	if(!RAM_FLAG_COBRO_VENTA)
		GRABAR_EVENT_TICKET( NO );
    ACUMULAR_TIEMPOS( );
    CEREAR_TIEMPOS( );
    SETEAR_TIEMPO( _ENTRADA_OCIO );
	if(!RAM_FLAG_COBRO_VENTA)
		GRABA_EVENTOS_PAGO( 0, datos_cobros->__devolucion  );
	/*cod_barra = LEER_COD_BARRA_EN_BINARIO( 0 );
	if( cod_barra != NULL ) {
		int area = SELECTED(  );
		SELECT( AREA_COD_BARRA );
		memset( &event_cod_barra, 0, sizeof( struct _event_cod_barra ) );
		event_cod_barra.caja = NRO_CAJA;
		event_cod_barra.nro_z = NRO_Z;
		event_cod_barra.id_evento = RAM_ID_EVENTO;
		event_cod_barra.posicion_cod_barra = 1;
		event_cod_barra.cuenta = cobros.codigo;
		event_cod_barra.importe = datos_cobros->_importe + datos_cobros->_costo_servicio 
			+ datos_cobros->_iva_costo_servicio;
		event_cod_barra.cod_sucursal = config.sucursal;
		event_cod_barra.fecha_ticket = _GET_FECHA_DOS();
		event_cod_barra.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
		memcpy( event_cod_barra.codigo_barra, cod_barra,
			sizeof( event_cod_barra.codigo_barra ) );
		#ifdef INVEL_L
			BEGIN_TRANSACTION();
		#endif
		INSERT();
		#ifdef INVEL_L
			END_TRANSACTION();
		#endif
		SELECT( area );
	}*/
	if( !RAM_FLAG_COBRO_VENTA )
		ACTUALIZA_ID_EVENTO_NEW();
}

//*****************************************************************************
int PEDIR_DATOS_PAGATODO( struct DatosCobroPagaTodo *datosCobros )
//*****************************************************************************
{
	int  rta = 0, car, salir = 0, fin = 0, rta_pedir = 0;
	struct _datos_cobro datos_cobro_aux;
	double importe;
	int sig=1 ,fin1 = 0;
	
	memset( &datos_cobro_aux, 0, sizeof( struct _datos_cobro ) );		
	if( !datosCobros->__devolucion ) {
		switch( datosCobros->_modo ) {
		//solicitud de llavero u otros datos segun modo de ingreso
		case PAGATODO_RECARGA:
			while( !salir ) {
				datos_cobro_aux.importe = datosCobros->_importe;
                datos_cobro_aux.datos_pedir = datosCobros->_datos_pedir;
	//si no viene el importe lo pedimos
				importe = datos_cobro_aux.importe;
				if( importe < 0.01) {
					sig = 0;
					fin1 = 0;
				}
				
				while( sig == 0 && fin1 == 0 && importe == 0.0 ) {
					ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
					importe = PEDIR_FLOTANTE_COD_BARRA( ( char )( 51 - ancho_panel / 2 ), ( char )11,
					( char )ENTEROS, ( char )DECIMALES, importe, NULL );
					if( importe > 0 ) {
						sig = 1;
						//datos_cobro_aux.importe = importe;
						datosCobros->_importe = importe;
						EVALUAR_COBROS_MONEDA_EXTRANJERA( importe, &datos_cobro_aux );
						MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
					} else if( importe == -1 ) {
						//rta = 0;
						fin1 = 1;
				}
			} 
			if( fin1 == 1)
				rta = 0;
			else {
//

				//MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( &datos_cobro_aux );
                MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
				//ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA_PT );
                //ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
				//MOSTRAR_INPUT_CADENA( "", 0 );
				MENSAJE( "DESEA REALIZAR LECTURA DE LLAVERO? S/N" );
				TECLADO_ALFABETICO = 1;
				do {
					car = GETCH(  );
				} while( car != 'S' && car != 'N' && car != 's' && car != 'n' && car != 27 );                 
				BORRAR_MENSAJE(  );
				TECLADO_ALFABETICO = 0;
				if( car == 'S' || car == 's' ) {
					char cadena_aux[14];
					MENSAJE( "PASE EL LLAVERO POR EL SCANNER" );
					_PERMITIR_SCANNER = SI;
					//ExecuteSSAction( A_PEDIR, _SIMPLE, "LLAVERO", NULL, NULL );
                    ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
					rta = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
						( char )0, ( char )13, cadena_aux, NULL, 0, SI, NO , NULL);
					//ExecuteAction( A_FIN_PEDIR );
					if( rta > 0 ) {
						memcpy( datosCobros->_codClientePagatodo, cadena_aux, 
							sizeof( datosCobros->_codClientePagatodo ) );
					}
					_PERMITIR_SCANNER = NO;
					BORRAR_MENSAJE(  );
				} else if( car != 27 ) {
					char cuenta_aux[41];
					int  seguir = 1;
					memset( cuenta_aux, 0x00, sizeof( cuenta_aux ) );
					//ExecuteSSAction( A_PEDIR_DATO, pos_actual, " ", " ", NULL );
					do {
						ExecuteSSAction( A_PEDIR, _SIMPLE, "TELEFONO", "NUMERO DE TELEFONO", NULL, " " );
						//ExecuteSSAction( A_PEDIR_DATO, 0, " ", " ", NULL );
						//el titulo "TELEFONO" debera ir en la ventana
						rta_pedir = PEDIR_CADENA_NUMERICA( 1, 1, 40, datosCobros->_cuenta, NULL, 0, SI, NO, NULL );
						ExecuteAction( A_FIN_PEDIR );
						
						//el titulo "REINGRESE TELEFONO" debera ir en la ventana
						if( rta_pedir ) {
							ExecuteSSAction( A_PEDIR, _SIMPLE, "TELEFONO", "REINGRESE TELEFONO", NULL, " " );
							rta_pedir = PEDIR_CADENA_NUMERICA( 1, 1, 40, cuenta_aux, NULL, 0, SI, NO, NULL );
							ExecuteAction( A_FIN_PEDIR );
						}
						if( ( rta_pedir && strcmp( datosCobros->_cuenta, cuenta_aux ) == 0 ) || !rta_pedir ) {
							seguir = 0;     //ok
						} else {
							MENSAJE( "NO COINCIDEN LOS NUMEROS TELEFONICOS. REINGRESAR" );
							//BORRAR_MENSAJE(  );
							memset( datosCobros->_cuenta, 0x00, sizeof( datosCobros->_cuenta ) );
						}
					} while( seguir );                 
				        
					if( rta_pedir ) {
						MENSAJE( "SE ENTREGA LLAVERO? S/N" );
						TECLADO_ALFABETICO = 1;
						do {
							car = GETCH(  );
						} while( car != 'S' && car != 'N' && car != 's' && car != 'n' && car != 27 );                 
						BORRAR_MENSAJE(  );
						TECLADO_ALFABETICO = 0;
						if( car == 'S' || car == 's' ) {
							char cadena_aux[14];
							MENSAJE( "PASE EL LLAVERO POR EL SCANNER" );
							_PERMITIR_SCANNER = SI;
							//ExecuteSSAction( A_PEDIR, _SIMPLE, "LLAVERO", "", NULL );
                            ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
							rta = PEDIR_CADENA_NUMERICA( ( char )( 51 - ancho_panel / 2 ),
								( char )0, ( char )13, cadena_aux, NULL, 0, SI, NO , NULL);
							//ExecuteAction( A_FIN_PEDIR );
							if( rta > 0 ) {
								memcpy( datosCobros->_codClientePagatodo, cadena_aux, 
									sizeof( datosCobros->_codClientePagatodo ) );
							}
							_PERMITIR_SCANNER = NO;
							BORRAR_MENSAJE(  );
    					} 
						if( car != 27 ) {
							rta = 1;
						} else {
							rta = 0;
						}
					} else {
						rta = 0;
					}
				}
			}//
				if( !rta ) {
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
								memset( &datos_cobro_aux, 0, sizeof( struct _datos_cobro ) );
								datos_cobro_aux.importe = datosCobros->_importe;
								//MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( &datos_cobro_aux );
                                MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
								//ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA_PT );
                                ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
								MOSTRAR_INPUT_CADENA( "", 0 );
								break;
						}
					}
					BORRAR_MENSAJE(  );
				} else {
					salir = 1;
					MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );
				}
			}
			break;
		case PAGATODO_SERVICIOS:
			{
				char	cod_barra_aux[81];
				long	codigo_barra = 0;
				int		sig = 0;
				
				while( !salir ) {
					while( sig == 0 && fin == 0 ) {
						memset( cod_barra_aux, 0, sizeof( cod_barra_aux ) );
                        ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA );
						TECLADO_ALFABETICO = 1;
                        codigo_barra = PEDIR_CADENA( ( char )( 23 ), ( char )( 6 ), ( char )80,
                            cod_barra_aux, NULL, SI, NO );
						TECLADO_ALFABETICO = 0;
						if( codigo_barra > 0 && strlen( cod_barra_aux ) > 0 ) {
							if( strlen( cod_barra_aux ) > 0 ) {
								memset( &datos_cobro_aux, 0, sizeof( struct _datos_cobro ) );		
								strncpy( datos_cobro_aux.cod_barra, cod_barra_aux, sizeof( datos_cobro_aux.cod_barra ) );
								datos_cobro_aux.codigo = datosCobros->_codigo;
								if( EVALUAR_MASCARA_COD_BARRA( cobros.codigo,
				   						cod_barra_aux, &datos_cobro_aux ) ) {
									sig = 1;
									strncpy( datosCobros->_codigoBarra, datos_cobro_aux.cod_barra, 
										sizeof( datosCobros->_codigoBarra ) );
									strncpy( datosCobros->_cuenta, datos_cobro_aux.cuenta, 
										sizeof( datosCobros->_cuenta ) );
									datosCobros->_importe = datos_cobro_aux.importe;
									ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros,
										sizeof( struct DatosCobroPagaTodo ), 0 ); 
									//MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( &datos_cobro_aux );
                                    MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
								}
							} else {
								sig = 1;
							}
						} else if( !codigo_barra ) {
							fin = 1;
							codigo_barra = 0;
						}
					}
					sig = 0;
					/*------------------------------Pedir Importe---------------------------------*/
					if( datosCobros->_importe == 0.00 ) {			
						double	importe = 0.0;

						datosCobros->_importe = cobros.importe;
						datos_cobro_aux.importe = datosCobros->_importe;
						
						while( sig == 0 && fin == 0 && importe == 0.0 && datos_cobro_aux.importe <= 0.00 ) {
							//ExecuteAction( A_MUESTRA_DATO_IMPORTE_PT );
                            ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
							importe = PEDIR_FLOTANTE_COD_BARRA( ( char )( 51 - ancho_panel / 2 ), 
								( char ) 11, ( char )ENTEROS, ( char )DECIMALES, importe, NULL );
							if( importe > 0.0 ) {
								sig = 1;
								datosCobros->_importe = importe;
								ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros,
										sizeof( struct DatosCobroPagaTodo ), 0 ); 
							} else if( importe == -1 ) {
								fin = 1;
							}
						}
						/*------------ Cobro con Importe configurado ------------*/
						if( datos_cobro_aux.importe > 0.00 && importe == 0.0 && fin == 0 && sig == 0 ) {
							//MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( &datos_cobro_aux );
                            MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
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
									memset( &datos_cobro_aux, 0, sizeof( struct _datos_cobro ) );
                                    MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
									break;
							}
						}
						BORRAR_MENSAJE(  );
					} else {
						salir = 1;
						MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );
					}
				}
				if( fin == 0 ) {
					rta = 1;
					ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros,
						sizeof( struct DatosCobroPagaTodo ), 0 );
				}
			}
			break;
		case PAGATODO_VENTA_PIN:
			datos_cobro_aux.importe = datosCobros->_importe;
            datos_cobro_aux.datos_pedir = datosCobros->_datos_pedir;
			//MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( &datos_cobro_aux );
            MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
			MENSAJE_CON_TECLA_STRING( S_PRESIONE_ENTER_PARA_CONTINUAR, 13 );
			rta = 1;
			break;
		}
	} else {
	    long   numero_aux = 0;
		int    ok = 0, k;
		//pido dato por dato:
        
		while( !salir ) {
			MENSAJE( "INGRESE LOS DATOS DEL COMPROBANTE A DEVOLVER" );
			ExecuteSSAction( A_PEDIR, _SIMPLE, "DEVOLUCION", "SUCURSAL", NULL, " " );
			rta_pedir = (INT16) PEDIR_ENTERO( 1, 1, 4, NO );
			ExecuteAction( A_FIN_PEDIR );
			if( rta_pedir > 0 ) {
				datosCobros->_sucursal_original = rta_pedir;
				ExecuteSSAction( A_PEDIR, _SIMPLE, "DEVOLUCION", "CAJA", NULL, " " );
				rta_pedir = (INT16) PEDIR_ENTERO( 1, 1, 4, NO );
				ExecuteAction( A_FIN_PEDIR );
			}
			if( rta_pedir > 0 ) {
				datosCobros->_caja_original = rta_pedir;
				ExecuteSSAction( A_PEDIR, _SIMPLE, "DEVOLUCION", "FECHA", NULL, " " );
				rta_pedir = (INT16) PEDIR_FECHA( 1, 1, NULL );
				ExecuteAction( A_FIN_PEDIR );
			}
			if( rta_pedir > 0 ) {
				datosCobros->_fecha_original = rta_pedir;
				ExecuteSSAction( A_PEDIR, _SIMPLE, "DEVOLUCION", "HORA", NULL, " " );
				rta_pedir = PEDIR_HORA_LARGA( 1, 1, NULL );
				ExecuteAction( A_FIN_PEDIR );
			}
			if( rta_pedir > 0 ) {
				double  importeAux = 0.0;
				datosCobros->_hora_original = rta_pedir;
                ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
				importeAux = PEDIR_FLOTANTE_COD_BARRA( 1, 1, ( char )ENTEROS, ( char )DECIMALES, 
					importeAux, NULL );
				if( importeAux > 0.0 ) {
					datosCobros->_importe = ROUND( importeAux - datosCobros->_costo_servicio - 
						datosCobros->_iva_costo_servicio, ENTEROS, DECIMALES );
					ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) datosCobros,
						sizeof( struct DatosCobroPagaTodo ), 0 ); 
					rta = 1;
				}
			}
			BORRAR_MENSAJE(  );
			if( !rta ) {
				MENSAJE( "DESEA SALIR DE COBROS? <ENTER> PARA SALIR - <ESC> PARA CONTINUAR" );
				ok = 0;
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
							memset( &datos_cobro_aux, 0, sizeof( struct _datos_cobro ) );
                            MOSTRAR_DATOS_CODIGO_BARRA( &datos_cobro_aux );
							break;
					}
				}
				BORRAR_MENSAJE(  );
			} else {
				salir = 1;
			}
		}
	}
	return( rta );
}
/*****************************************************************************/
void MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( struct _datos_cobro *datos_cobro )
/*****************************************************************************/
{
	char buffer[20];
	//----------------Mostrar Codigo Barra---------------
	if( strlen( datos_cobro->cod_barra ) == 0 ) {
		ExecuteAction( A_MUESTRA_DATO_CODIGO_BARRA_PT );
		MOSTRAR_INPUT_CADENA( "", 0 );
	}
	//-----------------Mostrar Importe-------------------
	//ExecuteAction( A_MUESTRA_DATO_IMPORTE_PT );
    ExecuteAction( A_MUESTRA_DATO_IMPORTE_COBRO );
	if( datos_cobro->importe ) {
		sprintf( buffer, "%.2lf", datos_cobro->importe );		
		MOSTRAR_INPUT_CADENA( buffer, 0 );
	} else {
		MOSTRAR_INPUT_CADENA( "", 0 );
	}
}
/*****************************************************************************/
//void COPIAR_RNV_A_MEMORIA_PARA_DEBUG( void )
/*****************************************************************************/
/*{
	int aa;
	struct _struct_var_tarjetas {
			char     c_var_tarjetas_tarjeta                ;
			char     c_var_tarjetas_numero[21]                ;
			UINT16   c_var_tarjetas_fecha_vto              ;
			float    c_var_tarjetas_limite                 ;
			char     c_var_tarjetas_cuotas                 ;
			char     c_var_tarjetas_financia_tarjeta       ;
			char     c_var_tarjetas_autorizacion[11]           ;
			long     c_var_tarjetas_nro_cupon              ;
			long     c_var_tarjetas_nro_cupon_int          ;
			char     c_var_tarjetas_lectura_banda          ;
			char     c_var_tarjetas_boletin_vencido        ;
			float    c_var_tarjetas_recargo                ;
			UINT16   c_var_tarjetas_nro_boletin            ;
			char     c_var_tarjetas_tabla[6]                  ;
			char     c_var_tarjetas_on_line                ;
			char     c_var_tarjetas_adherente[9]              ;
			float    c_var_tarjetas_anticipo               ;
			float    c_var_tarjetas_saldo_cliente          ;
			float    c_var_tarjetas_importe_cuota          ;
			UINT16   c_var_tarjetas_primer_vto             ;
			char     c_var_tarjetas_moneda                 ;
			double   c_var_tarjetas_importe_entrega        ;
			float    c_var_tarjetas_porc_entrega           ;
			char     c_var_tarjetas_nro_ref[12]                ;
			char     c_var_tarjetas_aut_en_boletin         ;
			INT16    c_var_tarjetas_cvv                    ;
			char     c_var_tarjetas_nro_comercio[21]           ;
			char     c_var_tarjetas_cod_servicio[3]           ;
			char     c_var_tarjetas_unicamente_ol          ;
			char     c_var_tarjetas_necesita_pin           ;
			char     c_var_tarjetas_pedir_auto             ;
			UINT16   c_var_tarjetas_lote                   ;
			char     c_var_tarjetas_captura_on_line        ;
			float    c_var_tarjetas_coeficiente            ;
			char     c_var_tarjetas_aut_on_line            ;
			long     c_var_tarjetas_terminal               ;
			char     c_var_tarjetas_status                 ;
			char     c_var_tarjetas_tipo_cuenta            ;
			char     c_var_tarjetas_pin[8]                    ;
			char     c_var_tarjetas_celda_pinpad           ;
			char     c_var_tarjetas_l_pin                  ;
			char     c_var_tarjetas_mensaje[37]                ;
			char     c_var_tarjetas_cod_vigencia           ;
			char     c_var_tarjetas_pide_entrega           ;
			char     c_var_tarjetas_nro_orden_cuota        ;
			char     c_var_tarjetas_control_cuotas         ;
			long     c_var_tarjetas_cupon_original         ;
			UINT16   c_var_tarjetas_fecha_original         ;
			float    c_var_tarjetas_recargo_diferim        ;
			char     c_var_tarjetas_track1[80]                 ;
			char     c_var_tarjetas_track2[50]                 ;
			UINT16   c_var_tarjetas_fecha_contable         ;
			UINT16   c_var_tarjetas_fecha_host             ;
			long     c_var_tarjetas_hora_host              ;
			char     c_var_tarjetas_nro_cuenta[20]             ;
			long     c_var_tarjetas_hora_original          ;
			char     c_var_tarjetas_aut_original[11]           ;
			float    c_var_tarjetas_tasa                   ;
			double   c_var_tarjetas_vuelto                 ;
			INT16    c_var_tarjetas_grupo_cuota            ;
			float    c_var_tarjetas_tasa_aplicada          ;
			INT16    c_var_tarjetas_red                    ;
			char     c_var_tarjetas_confirmar_numero       ;
			INT16    c_var_tarjetas_ultimos_digitos        ;
			char     c_var_tarjetas_min_cuotas             ;
			char     c_var_tarjetas_max_cuotas             ;
			UINT16   c_var_tarjetas_fecha_tope_predatada   ;
			UINT16   c_var_tarjetas_fecha_tope_1_cuota     ;
			char     c_var_tarjetas_pedir_cvv              ;
			char     c_var_tarjetas_pedir_garantia         ;
			long     c_var_tarjetas_op_habilitadas         ;
			INT16    c_var_tarjetas_caja_original          ;
			INT16    c_var_tarjetas_suc_original           ;
			char     c_var_tarjetas_pin_ascii[21]          ;
			char     c_var_tarjetas_nom_arch_voucher[13]   ;
			char     c_var_tarjetas_datafono               ;
			long     c_var_tarjetas_nro_trace              ;
			char	 reserva[100];
	} copia_rnv_var_tarjetas;
	
    
	COPY_STRUCT_TO_BUFFER( (char*) &copia_rnv_var_tarjetas, _var_tarjetas );
	int h, pos = 0, v, l;
    for( h = _APUNTAR_STRUCT( _var_tarjetas );EST_COD( h ) == _var_tarjetas;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v ) * DD_ELEMENTOS( v );
        _DD_EXEC( _DM_GET, DD_CLASE( v ), DD_OFFSET( v ), &buffer[pos], l );
        pos += l;
    }
	DD_LEER_COD_ESPECIAL(  );
	dd_var.nom_variable
	aa = 1;
}*/
/*****************************************************************************/
void LIMPIAR_ARCHIVO_LINEAS_MENSAJE( void )
/*****************************************************************************/
{
	struct _lin_mens lin_mens;
	//int    a_ant;
	//a_ant = SELECTED(  );
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
	/*if( USE_DATABASE( AREA_LIN_MENS, "lin_mens.tmp", ( char * ) &lin_mens,
			sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens, sizeof( struct _lin_mens))== 0 ) {
		//GO( TOP );
		GO2(TOP);
		while(  /*!BtrvEOF()*/ !dbEOF()) {
			//_DELETE(  );
			DELETE2(  );
		}
		//CLOSE_DATABASE( AREA_LIN_MENS );
		CLOSE_TABLE( T_LIN_MENS_TMP, TT_TEMP );
	}
	//SELECT( a_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/*****************************************************************************/
int POSICIONAR_COBRO_SELECCIONADO( void )
/*****************************************************************************/
{
	struct	  _datos_cobro datos_cobro;
	int       rta;
	RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )&datos_cobro,
		sizeof( struct _datos_cobro ), 0 );
	//solo necesito esto para obtener el cod_cobro en caso de corte de luz.
	//USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
	OPEN_TABLE( T_COBROS, TT_ORIG, ( char* )&cobros, sizeof( cobros ) );
    //SELECT( AREA_AUX4 );
    //GET_EQUAL( ( char* )&datos_cobro.codigo );
	SET_WHERE("CODIGO = '%i'", datos_cobro.codigo );
	RUN_QUERY(NO);
	rta = FOUND2(  );
    //CLOSE_DATABASE( AREA_AUX4 );
	CLOSE_TABLE( T_COBROS, TT_ORIG );

	return( rta );
}
/*****************************************************************************/
//void COPIAR_RNV_A_MEMORIA_PARA_DEBUG( void )
/*****************************************************************************/
/*{
	int aa;
	struct _copia_rnv {
		struct _struct_pago {
			char    rnv_var_pago_modo                    ;
			double  rnv_var_pago_importe                 ;
			char    rnv_var_pago_tarjeta                 ;
			INT16   rnv_var_pago_cuenta                  ;
			long    rnv_var_pago_buffer1_long            ;
			long    rnv_var_pago_numero                  ;
			char    rnv_var_pago_status_tarjeta          ;
			INT16   rnv_var_pago_banco                   ;
			UINT16  rnv_var_pago_fecha_vto               ;
			INT16   rnv_var_pago_cantidad_cupones        ;
			double  rnv_var_pago_valor_cupones           ;
			INT16   rnv_var_pago_cod_post_cheq           ;
			INT16   rnv_var_pago_cod_hora_acre           ;
			char    rnv_var_pago_nro_tarjeta[21]         ;  
			char    rnv_var_pago_auto_tarjeta[11]        ;  
			long    rnv_var_pago_nro_cupon1              ;
			long    rnv_var_pago_nro_cupon_int1          ;
			char    rnv_var_pago_cuotas                  ;
			char    rnv_var_pago_buffer[16]              ;  
			UINT16  rnv_var_pago_nro_boletin             ;
			char    rnv_var_pago_tabla[6]                ; 
			double  rnv_var_pago_variacion               ;
			char    rnv_var_pago_viejo_cuit[12]          ;  
			char    rnv_var_pago_telefono[12]            ;  
			double  rnv_var_pago_vuelto                  ;
			double  rnv_var_pago_imp_moneda_ext          ;
			char    rnv_var_pago_moneda                  ;
			char    rnv_var_pago_cod_vigencia            ;
			UINT16  rnv_var_pago_primer_vto              ;
			char    rnv_var_pago_cuenta_bco[11]          ;  
			double  rnv_var_pago_importe_entrega         ;
			char    rnv_var_pago_id_registro             ;
			double  rnv_var_pago_vuelto_medio            ;
			char    rnv_var_pago_cod_medio_vuelto        ;
			char    rnv_var_pago_cod_submedio_vuelto     ;
			char    rnv_var_pago_tipo_vuelto             ;
			double  rnv_var_pago_importe_me              ;
			INT16   rnv_var_pago_cod_solicitud           ;
			double  rnv_var_pago_iva_proporcional        ;
			char    rnv_var_pago_telefono_otro[12]       ;  
			double  rnv_var_pago_imp_cuota               ;
			INT16   rnv_var_pago_cod_restriccion_art     ;
			long    rnv_var_pago_datafono                ;
			char    rnv_var_pago_cuit[21]                ;  
			INT16   rnv_var_pago_cod_submedio            ;
		} copia_rnv_pago[10];
	} copia_rnv;
    
	COPY_STRUCT_TO_BUFFER( (char*) &copia_rnv.copia_rnv_pago, _pago );
	aa=1;
}*/

