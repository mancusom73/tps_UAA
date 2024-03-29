#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <cio.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <dbrouter.h> 
#include <dec_mes.h> 
#include <dtoai.h> 
#include <envios.h> 
#include <export.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <lista.h> 
#include <mstring.h> 
#include <netcom2.h> 
#include <pausa.h> 
#include <tarjetas.h> 
#include <tkt.h> 
#include <aarch.h>
#include <ini.h>
#include <deftables.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
#ifdef INVEL_L
#else
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#endif
/*------------- Banderas en AUTOR.DAT ---------------*/
#define _NO_COMUNICACION   -1
#define _SERVER_DISPONIBLE  0
#define _ESPERA_ATENCION    1
#define _ATENDIDO           2
#define _HAY_RESPUESTA      3
#define _RESPUESTA_TOMADA   4
#define _CAJA_OFF_LINE      5
#ifdef COMPILAR_ON_LINE
int _AUTORIZACION_ON_LINE( char tipo_operacion, double importe, char *buf_rta, int l_buf_rta );
#endif
/*****************************************************************************/
int AUTORIZACION_ON_LINE( int tipo_operacion, double importe, char *buf_rta, int l_buf_rta )
/*****************************************************************************/
{
    int rta;

    if( PROTOCOLO_AUTORIZACION_TARJETA == _TCP_IP ) {
		if( RAM_PEDIR_CLIENTE_AFINIDAD == 2) {
			char marcat= VAR_TARJETAS_ANT_CONFIRMAR_NUMERO; //REHUSO ESTA VARIABLE

			rta = ENVIAR_TRANSACCION_AL_AUT_ON( tipo_operacion, importe,
                                            ( struct _rta_terminal * )buf_rta,
                                             marcat  );
		}else
        rta = ENVIAR_TRANSACCION_AL_AUT_ON( tipo_operacion, importe,
                                            ( struct _rta_terminal * )buf_rta,
                                            NODO_TARJETA( VAR_TARJETAS_TARJETA ) );
    }
    else {
        #ifdef COMPILAR_ON_LINE
        rta = _AUTORIZACION_ON_LINE( ( char )tipo_operacion, importe, buf_rta, l_buf_rta );
        #endif
    }
    return rta;
}
#ifdef COMPILAR_ON_LINE
/*****************************************************************************/
int _AUTORIZACION_ON_LINE( char tipo_operacion, double importe, char *buf_rta, int l_buf_rta )
/*****************************************************************************/
{
    int rta = 0;
    int handle;
    struct _datos_terminal datos_terminal;
    /*-------------------- Carga la estructura de envio ----------------------*/
    PREPARAR_DATOS_TERMINAL( tipo_operacion, importe, &datos_terminal );
    /*---------------------- Crea el archivo de envio ------------------------*/
    #if defined(INVEL_W) || defined(INVEL_L)
    handle = CREAT_( _ENVIO, S_IFREG | S_IWRITE | S_IREAD );
    #else
    handle = CREAT( _ENVIO, FA_ARCH );
    #endif
    /*-------------- Verifica condiciones de la comunicacion ----------------*/
    if( handle < 4 ) {
        MENSAJE_STRING( S_PROBLEMA_ENVIAR_DATOS );
    }
    else if( LEE_AUTOR_DAT() == _NO_COMUNICACION && tipo_operacion != _CIERRE_DE_LOTE ) {
        MENSAJE_STRING( S_SERVER_FUERA_LINEA );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    else if( LEE_AUTOR_DAT() == _CAJA_OFF_LINE && tipo_operacion != _CIERRE_DE_LOTE ) {
        MENSAJE_STRING( S_TPV_FUERA_LINEA );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    else {
        /*---------------------- Envia el paquete ------------------------*/
        _WRITE( handle, ( char* )&datos_terminal, sizeof( datos_terminal ), __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
        /*-------------------- Establece protocolo -----------------------*/
        if( tipo_operacion == _CIERRE_DE_LOTE ) {
            GRABA_AUTOR_DAT( _ESPERA_ATENCION );
        }
        else {
            //*--------------- Verifica disponibilidad del server --------------*/
            MENSAJE_SIN_SONIDO_STRING( S_ESPERANDO_ATENCION_DEL_SERVER );
            if( !ESPERAR_BANDERA( _SERVER_DISPONIBLE, TIME_OUT_SERVER_NO_DISPONIBLE ) ) {
                MENSAJE_STRING( S_SERVER_NO_ATIENDE_DISP );
            }
            else {
                GRABA_AUTOR_DAT( _ESPERA_ATENCION );
                //*---------------------- Espera que lo atiendan -----------------------*/
                if( !ESPERAR_BANDERA( _ATENDIDO, TIME_OUT_LLAMADO_NO_ATENDIDO ) ) {
                    MENSAJE_STRING( S_SERVER_NO_ATIENDE_ATEN );
                }
                else {
                    MENSAJE_SIN_SONIDO_STRING( S_ATENDIDO_ESPERANDO_RESPUESTA );
                    if( !ESPERAR_BANDERA( _HAY_RESPUESTA, TIME_OUT_SERVER_NO_RESPONDIO ) ) {
                        MENSAJE_STRING( S_SERVER_NO_RESPONDIO );
                    }
                    else {
                        /*--------------------- Lee la respuesta ----------------------*/
                        #if defined(INVEL_W) || defined(INVEL_L)
                        handle = OPEN( _RESP, O_RDWR );
                        #else
                        handle = OPEN( _RESP, O_RDWR | O_DENYNONE );
                        #endif
                        if( handle > 3 ) {
                            _READ( handle, buf_rta, l_buf_rta, __LINE__, __FILE__ );
                            rta = 1;
                            _CLOSE( handle, __LINE__, __FILE__ );
                            GRABA_AUTOR_DAT( _RESPUESTA_TOMADA );
                        }
                        else {
                            MENSAJE_STRING( S_PROBLEMA_LEER_RESPUEST );
                        }
                    }
                }
            }
        }
    }
    BORRAR_MENSAJE();
    return ( rta );
}
#endif
/*****************************************************************************/
int ESPERAR_BANDERA( int bandera, int segundos )
/*****************************************************************************/
{
    int h,atendido;
    int aux;
    for( h = 0, atendido = 0;h < segundos && !atendido;h++ ) {
        aux = LEE_AUTOR_DAT();
        if( bandera == _ATENDIDO ) {
            if( aux == _HAY_RESPUESTA ) {
                atendido = 1;
            }
        }
        if( aux == bandera ) {
            atendido = 1;
        }
        if( !atendido ) {
            PAUSA( 18 );
        }
    }
    return ( atendido );
}
/*****************************************************************************/
void PREPARAR_DATOS_TERMINAL( char tipo_operacion, double importe,
                              struct _datos_terminal *datos_terminal )
/*****************************************************************************/
{
    int cant;
    char aux[10],aux_track2[50];
    unsigned fecha_aux;
    datos_terminal->tipo_de_operacion = tipo_operacion;
    datos_terminal->lectura_banda = VAR_TARJETAS_LECTURA_BANDA;
    datos_terminal->cupon = VAR_TARJETAS_NRO_CUPON_INT;
    datos_terminal->moneda = MONEDA_EXTENDIDA();
    datos_terminal->cuotas = VAR_TARJETAS_CUOTAS;
    datos_terminal->importe_total = importe + VAR_TARJETAS_IMPORTE_ENTREGA;
    datos_terminal->monto_entrega = VAR_TARJETAS_IMPORTE_ENTREGA;
    datos_terminal->primer_vencimiento = VAR_TARJETAS_PRIMER_VTO;
    datos_terminal->vigencia = VAR_TARJETAS_COD_VIGENCIA;
    datos_terminal->marca_tarjeta = VAR_TARJETAS_TARJETA;
    datos_terminal->cajero = NRO_CAJERO;
    datos_terminal->hora = GET_HORA_COMPLETA_DOS();
    datos_terminal->boletin = VAR_TARJETAS_NRO_BOLETIN;
    datos_terminal->fecha = GET_FECHA_DOS();
    datos_terminal->terminal = VAR_TARJETAS_TERMINAL;
    datos_terminal->cvv = VAR_TARJETAS_CVV;
    datos_terminal->tipo_cuenta = VAR_TARJETAS_TIPO_CUENTA;
    datos_terminal->cupon_original = VAR_TARJETAS_CUPON_ORIGINAL;
    datos_terminal->fecha_original = VAR_TARJETAS_FECHA_ORIGINAL;
    strcpy( datos_terminal->plan_de_pagos, VAR_TARJETAS_TABLA );
    strcpy( datos_terminal->numero_comercio, VAR_TARJETAS_NRO_COMERCIO );
    memcpy( datos_terminal->nro_referencia, VAR_TARJETAS_NRO_REF,
            sizeof( datos_terminal->nro_referencia ) );
    memcpy( datos_terminal->autorizacion, VAR_TARJETAS_AUTORIZACION,
            sizeof( datos_terminal->autorizacion ) );
    memcpy( datos_terminal->pin, GET_PIN_PRIVADO( ), sizeof( datos_terminal->pin ) );
    if( VAR_TARJETAS_LECTURA_BANDA == 1 ) {
        memset( datos_terminal->datos_tarjeta, 0, 40 );
        cant = _VAR_TARJETAS_TRACK2( 0 ) - 1;
        memcpy( aux_track2, GET_TRACK_2_PRIVADO( ), sizeof( aux_track2 ) );
        if( cant <= 0 ) {
            cant = 0;
        }
        if( cant > 40 ) {
            cant = 40;
        }
        memcpy( datos_terminal->datos_tarjeta, &aux_track2[1], cant );
    }
    else {
        // Esto debiera ser configurable
        /* if( VAR_TARJETAS_TARJETA == _CABAL_VIEJA ) { strcpy(
         * datos_terminal->datos_tarjeta, "3121" ); strcat(
         * datos_terminal->datos_tarjeta, VAR_TARJETAS_NUMERO ); } else {
         * strcpy( datos_terminal->datos_tarjeta, VAR_TARJETAS_NUMERO ); } */
        strncpy( datos_terminal->datos_tarjeta, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/,sizeof( datos_terminal->datos_tarjeta ) );
        fecha_aux = DECREMENTAR_MES( GET_FECHA_VTO_PRIVADA(  )  );
        DTOA_INVERSA( fecha_aux, aux );
        memcpy( datos_terminal->vto_tarjeta, aux, 4 );
    }
}
/*****************************************************************************/
int LEE_AUTOR_DAT()
/*****************************************************************************/
{
    int bandera = -1, handle;
    handle = OPEN( _AUTOR_DAT, O_RDWR );
    if( handle > 3 ) {
        /*-------------- Lee bandera general ----------------*/
        _READ( handle, ( char* )&bandera, 2, __LINE__, __FILE__ );
        if( bandera == 0 ) {
            /*-------------- Lee bandera de la caja ----------------*/
            LSEEK( handle, ( long )( NRO_CAJA * 2 ), SEEK_SET, __LINE__, __FILE__ );
            _READ( handle, ( char* )&bandera, 2, __LINE__, __FILE__ );
        }
        else {
            bandera = _NO_COMUNICACION;
        }
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return ( bandera );
}
/*****************************************************************************/
int GRABA_AUTOR_DAT( int bandera )
/*****************************************************************************/
{
    int rta = -1, handle;
    //modo = _MODO_EJECUCION;
    //_MODO_EJECUCION = OPERACION;
    handle = OPEN( _AUTOR_DAT, O_RDWR );
    if( handle > 3 ) {
        LSEEK( handle, ( long )( NRO_CAJA * 2 ), SEEK_SET, __LINE__, __FILE__ );
        rta = _WRITE( handle, ( char* )&bandera, 2, __LINE__, __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    //_MODO_EJECUCION = modo;
    return ( rta );
}
/*****************************************************************************/
int GRABAR_OFF_LINE( int tipo_operacion, double importe )
/*****************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_ON_LINE
    int handle;
    //int       modo;
    long pos;
    struct _datos_terminal datos_terminal;
    SET_MEMORY_CHAR( __var_tarjetas_on_line, 2 );
    //modo = _MODO_EJECUCION;
    //_MODO_EJECUCION = OPERACION;
	memset(&datos_terminal,0,sizeof(datos_terminal));
    if( PROTOCOLO_AUTORIZACION_TARJETA == _TCP_IP ) {
        rta = ENVIAR_TRANSACCION_AL_AUT_ON( tipo_operacion, importe, NULL,
                                            NODO_TARJETA( VAR_TARJETAS_TARJETA ) );
    }
    else {
        /*-------------------- Carga la estructura de envio ----------------------*/
        PREPARAR_DATOS_TERMINAL( ( char )tipo_operacion, importe, &datos_terminal );
        /*--------------------- Graba el archivo de off line ---------------------*/
        handle = OPEN_O_CREAT( _OFF_LINE_ZZZ, O_RDWR );
        if( handle > 3 ) {
            LSEEK( handle, 0, SEEK_END, __LINE__, __FILE__ );
            if( tipo_operacion == _OPERACION_ANULADA ) {
                pos = sizeof( datos_terminal );
                LSEEK( handle, -pos, SEEK_END, __LINE__, __FILE__ );
            }
            rta = !_WRITE( handle, ( char* )&datos_terminal, sizeof( datos_terminal ), __LINE__,
                           __FILE__ );
            _CLOSE( handle, __LINE__, __FILE__ );
        }
        else {
            MENSAJE_STRING( S_NO_PUDO_ABRIR_TARJ_OFF );
        }
    }
    //_MODO_EJECUCION = modo;
    #endif
    return ( ( rta ) ? 2 : 0 );
}
#ifdef COMPILAR_ON_LINE
/***********************************************************************/
void CIERRE_DE_LOTE( int tipo_de_cierre )
/***********************************************************************/
{
	if(config_tps.NapseModalidad ==0) {
		int error;
		if( PROTOCOLO_AUTORIZACION_TARJETA == _TCP_IP ) {
			T_CIERRE( tipo_de_cierre );
		}
		else {
			error = APPEND( _OFF_LINE_ZZZ, _OFF_LINE_REMOTO );
			LOG_EXISTE_ARCHIVO_COD( _OFF_LINE_REMOTO );
			AUTORIZACION_ON_LINE( _CIERRE_DE_LOTE, 0, NULL, 0 );
			CREAR_OFF_LINE( !error );
		}
	} else {
		int ok = 1, ok_backup = 0;
		if( tipo_de_cierre == _CIERRE_DE_LOTE_EN_Z ) {
        // el cierre manual no debe hacer back up
        ok_backup = BACKUP_TRANSAC2();
        if( ok && ok_backup ) {
			SET_EMPTY_TABLE(T_TRANSAC2,TT_ORIG);
        }
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES        
		if( ok && ok_backup){
				SET_EMPTY_TABLE(T_TRAN_CRP,TT_ORIG); 
        }
#endif  
		if( ok && ok_backup){
				SET_EMPTY_TABLE(T_RESPUESTA_NAPSE,TT_ORIG); 
        }
        
		}
	}
}

/***********************************************************************/
void ECHO_TEST( )
/***********************************************************************/
{
   ENVIAR_TRANSACCION_AL_AUT_ON( _ON_LINE_ECHO_TEST, 0, NULL, 2 );

}
/***********************************************************************/
void CREAR_OFF_LINE( int cerear )
/***********************************************************************/
{
    BACKUP_ARCHIVO_DE_LA_SEMANA( GET_FECHA_BACKUP(), _OFF_LINE_ZZZ, _OFF_LINE_BAK );
    if( cerear ) {
        CEREAR_ARCHIVO_BINARIO( _OFF_LINE_ZZZ );
    }
}
#endif

