#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h>
#include <interpr.h>
#include <asc.h>
#include <aut_cheq.h>
#include <bancos.h>
#include <baucher.h>
#include <boletin.h>
//#include <browse.h>
#include <btrv.h>
#include <b_mens.h>
#include <cajon.h>
#include <cierre.h>
#include <cio.h>
#include <clave.h>
#include <clientes.h>
#include <cmemory.h>
#include <comprob.h>
#include <comtar.h>
#include <coop.h>
#include <cr.h>
#include <_cr1.h>
#include <cr_disco.h>
#include <cr_pant.h>
#include <cupones.h>
#include <cus.h>
#include <dbrouter.h>
#include <dd.h>
#include <display.h>
#include <driver.h>
#include <dtoa.h>
#include <eventos.h>
#include <ext_tick.h>
#include <fecha.h>
#include <ftoc.h>
#include <func.h>
#include <getch.h>
#include <get_fech.h>
#include <imp_item.h>
#include <imp_tick.h>
#include <inc_mes.h>
#include <lista.h>
#include <log.h>
#include <malloc.h>
#include <mensaje.h>
#include <mstring.h>
#include <m_cadena.h>
#include <m_fecha.h>
#include <on_line.h>
#include <pagos.h>
#include <pago_che.h>
#include <pago_cta.h>
#include <pago_efe.h>
#include <pago_mut.h>
#include <pago_tar.h>
#include <pago_tc.h>
#include <panel.h>
#include <pant.h>
#include <pan_pago.h>
#include <pausa.h>
#include <print.h>
#include <promo.h>
#include <p_fecha.h>
#include <p_float.h>
#include <recup.h>
#include <recupero.h>
#include <redondeo.h>
#include <rnv.h>
#include <round.h>
#include <stack.h>
#include <stringt.h>
#include <tarjetas.h>
#include <tkt.h>
#include <tomra.h>
#include <trim.h>
#include <valida.h>
#include <ventas.h>
#include <_cr0.h>
#include <cont_sub.h>
#include <import_t.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <assert.h>
#include <motor.h>
#include <cobros.h>
#include <deftables.h>
#include <db.h>
#include <cambio_m.h>
#include <dec_mes.h>
#include <str.h>
#include <devol_am.h>
#include <scanner.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

//#define _DEBUG_RNV_REC
#pragma pack(1)
#include <cr.h>
#include <cr_disco.h>
#include <recupero.h>
#include <config.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <aarch.h>
#include <panel.h>
#include <dbrouter.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <path.h>
#ifdef INVEL_L
#else
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#endif
#include <assert.h>
#include <motor.h>
#include <pagatodo.h>
#include <ini.h>
#include <t_pago.h>
#include <fiscal.h>
#include <cadafech.h>
#ifdef COMPILAR_MUTUALES
extern    int CONTROLA_IMPORTE_MUTUAL( char marca, char *numero_bono, double importe );
#endif
extern int imprime_copia_voucher ;
double REDONDEAR( double importe, int tipo, double minima_denominacion );
double MAXIMO_VUELTO_PERMITIDO( int h, int i );
double PORCENTAJE_VUELTO_PERMITIDO( int h, int i );
double TOTAL_X_MEDIO_EN_TICKET( int medio );
extern    int CONTROLA_IMPORTE_TICKET_COMPRA( char marca, char *numero_bono, double importe );
extern void GRABA_MODIFICACION_TARJETAS( int posicion, int id );
extern    int PIDE_DATOS_CHEQUE( int al_dia, long *numero, int *banco, unsigned * fecha_vto,
                                 int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono,
                                 char *cuenta_banco, int *cod_valida_cheque, int *plaza,
                                 int pedir_fecha_vto );
extern int VALIDAR_CHEQUE( int ind_medio, int sucursal, int nro_caja, long cajero, long nro_boleta,
                           char *nro_rut_comprador, char *nro_rut_girador, int nro_banco,
                           char *nro_cuenta, long nro_cheque, int nro_plaza, int tipo_credito,
                           double monto, unsigned fecha_vencimiento, int habilitado );
extern    int VALIDAR_VOUCHER_TARJETA( int cod_formato, double importe, int h );
extern    int MOSTRAR_OPCIONES_VUELTO( char cod_medio, char cod_submedio, int *cod_medio_vuelto, int *cod_submedio_vuelto );
extern	  int VALIDAR_MONEDERO_ELECTRONICO( double importe );
extern	  int CARGAR_MONEDERO_ELECTRONICO( int origen_promo, char *mensaje );
extern		void RECUPERAR_CLIENTE_AFINIDAD( void  );
void PAGOS_ANULAR_PAGO();
double VARIACION_DE_SUBMEDIOS( int modo , int submedio );
void PAGOS_INTERESES();
void PAGOS_PEDIR_INTERESES();
void PAGOS_DISCRIMINA_IVA();
void PAGOS_MARCA_AUTORIZACION();
void PAGOS_MUESTRA_IMPORTE( double importe );
//void PAGOS_IMPRIME_PAGOS_ESPECIALES( int devolucion );
//int PAGOS_GRABA_PAGO( int modo, int dev_cobro );
void PAGOS_AGREGAR_VARIACIONES();
void PAGOS_PREPARA_ITEM( int h, int anulacion );
void PAGOS_AGREGA_ITEM_INTERESES();
void PAGOS_ELIMINA_ITEM_INTERESES();
void _PAGOS_PREPARA_ITEM( double importe, double porc, int anulacion, int gravado, int origen,
                          int tipo );
void PAGOS_ENVIAR_OFFLINE();
void PAGOS_VALIDA_CHEQUE_CLIENTE();
extern double GET_PORC_FROM_VUELTOS_SIC( char medio, char submedio,char medio_vuelto, char submedio_vuelto );
extern double GET_MAX_VUELTO_FROM_VUELTOS_SIC( char medio, char submedio,char medio_vuelto, char submedio_vuelto );
#ifdef COMPILAR_COMPROMISO_200
void PAGOS_VALIDA_200();
#endif
int CONTROLA_IMPORTE_T_COMPRA( int nro, int submedio, float importe );
int PAGOS_CONTROL_COMPROB_IMPORTADO( int *medio, int *submedio );
/*................................. Funciones ............................... */
void IDENTIFICAR_BILLETES();
double PAGOS_PIDE_IMPORTE_CON_T_COMPRA( char *titulo, int enteros, int decimales, int signo,
                                        char cuenta, char posicion, int tipo_vuelto,
                                        int datos_adicionales, char cod_medio, char submedio,
                                        int cant_cuotas );
double PAGOS_PIDE_IMPORTE( char *titulo, int enteros, int decimales, int signo );
extern   int BUSCAR_CLIENTE_EN_BOLETIN( char *banco, char *cuenta, char *cuit, char *telefono,
                                        int *habilitado, long nro_cheque, int lector, char *serie,
                                        int plaza );
int LEER_CHEQUE( char *cod_cuenta, char *nr_banco, long *nro_cheq,
                 int *plaza /*, char *nro_serie */ );
void LIMPIAR_VARIAS_TARJETAS();
double CALCULA_VARIACION( double importe, int modo,int submedio );
double CALCULA_VARIACION_TARJETA( double importe, int modo, int modificar );
int _PAGOS_VALIDAR_LARGO( char *valor, int largo_min, INT16 cod_referencia, int tipo_dato );
int PAGOS_VALIDA_EN_BOLETIN( struct _dmedios *datos_medio, int pos, int cant_datos, double *importe,
                             struct _datos *datos );
int PAGOS_F_DATOS_ADICIONALES( int pos, int ind_pos, int nro, int pos_actual, double importe,
                               struct _dmedios *datos_medio, char *buffer );
int CANTIDAD_MAXIMA_PAGOS = 10;
extern char importando_ticket_devolucion;
extern int posicionTransaccionKinacu;
extern int CANT_VOUCHER;
int VALIDA_CODIGO( int validar, char *dato );
int CHEQUES_CARGADOS( int modoPago, int cantidad, int pos, double *importe, int dato_adic, double totCheque, int *cnt, int *_pos );
int ManipularCheques( int cant, int *nroCheque );
void SACAR_CHEQUE( int pos, int arch, int totCheques );
double MUTUALES_ML_A_ME_COT( double importe_me, double cotizacion );
double MUTUALES_ME_A_ML_COT( double importe_me, double cotizacion );
double    MUTUALES_ML_A_ME( double importe_me, int h );
double    MUTUALES_ME_A_ML( double importe_me, int h );
void ELIMINA_PAGO_PROMO( unsigned short medio, unsigned short submedio );
int MEDIOS_LIMITADOS_POR_PROMOCION();
int COD_SUBMEDIO(int pos_pago);
int MOSTRAR_MEDIOS_OBLIGATORIOS(int grupo);
int MEDIOS_HABILITADOS_COBROS( int n, int dev_cobro, struct    _datos_cobro *datosCobros );
int MEDIOS_LIMITADOS_POR_COBRO();
int _PAGOS_PEDIR_DESCRIPTOR( struct _dmedios *dmedios, char *valor );
extern struct _tarjetas_fisc tarjetas_fisc_rec[10];//Seba
int PAGOS_CARGAR_TARJETAS_IMPORTACION( );
void PAGOS_AGREGAR_DESCUENTO_TARJETA( );
int OBTENER_DATOS_PINES_VIRTUALES( void );
extern double RETORNAR_TOTAL_PRINTER( double variacion );
extern double RETORNAR_IMPORTES_IMPRESORA( double precio_final, double *iva_1 );
extern void SETEA_FOCUS_TPS( );
int MEDIOS_AGRUPADOS( int sumar, int modo, int cod_submedio );
void MOSTRAR_SALDO_EFECTIVO();
int MENU( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum );
void SETEAR_DEBITO_POS( int nro) ;
void MOSTRAR_SALDO_PROMOCIONES( );
void MENSAJE_ID_CON_PAUSA_CONF( int id );
void ABORT( char *mensaje );
void PAUSA( int ticks );
void  SET_STATIC_REINTENTO_QR( );
int  GET_STATIC_REINTENTO_QR( );
void  CEREAR_STATIC_REINTENTO_QR( );
static int  _static_reintento_qr = 0;
/*............................. Variables Globales .......................... */
//tipo_dato:
#define _TIPO_NUMERICO 1
#define _TIPO_CADENA   2
#define _TIPO_FECHA    3
#define _TIPO_DOUBLE   4
static int _modo;
/*static struct _datos {
    char      valor[31];
    char      boletin_consultado;   // 0 Ninguno, 1 Local, 2 Remoto
    int       cod_supervisor;
    char      valor_en_boletin[20];
};*/
struct id_billete
{
    BINARY status;
    int caja;
    char fecha[6];
    long nro_ticket;
    int denominacion;
    char cliente[12];
    char numero[16];
    int cajero;
    char reserva[38];
};
struct
{
    UINT16 cod_medio;
    UINT16 cod_submedio;
    UINT16 nro_cuota;
} indice_cuo_med;
/*struct sub_medios
{
    int medios;
    double submedios;
    double importe;
    double importe_dec;
};
*/
//struct institut_donacion
struct institut_donacion
{
    BINARY status;
	int cod_institucion;
    char desc_institucion[31];
    BINARY fin;
};
struct pagosPromo
{
    int eliminado;
    int cod_promo;
    int pos_pago;
    double monto_usado;
};
int CANT_TARJETAS_REC();
int RECUPERAR_POSICION_TARJETA_FISCAL( int h );
extern double VARIACION_RETENCION( double importe_neto, double iva );
extern double VARIACION_PERCEPCION( double importe_neto );
extern double VARIACION_PERCEPCION_212( double importe_neto );
//extern void GRABA_MODIFICACION_TARJETAS( int posicion, int id );
extern int LEE_TARJETAS( int posicion, int id );
struct institut_donacion instituciones;
char id_cliente[12];
long cliente_factura = 0;
void LIMPIAR_VARIAS_TARJETAS();
double MAXIMO_VUELTO_PERMITIDO( int h, int i );
int PAGOS_CONSTRUYE_MASCARA( struct _dmedios *datos_medio, char *mascara_in, int tam_mascara,
                             char *mascara, int cant_datos, double importe, struct _datos *datos );
int PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos );
static float porc_interes = 0.0;
static int muestra_interes = 0;
static struct _cuotas_medio cuo_med_sic;
int imprimiendo = 0;
extern int pant_activa_cliente;
extern struct _tarjetas_fisc tarjetas_fisc;
struct    _t_mutuales mutuales;
extern int tipo_t_eventos, tipo_t_eventos, tipo_ev_dpago;
int medio_saliente = 0;
int tomar_variacion = NO;
double variacion_printer = 0.0;
extern double IMPORTE_MEDIO_PROMOCIONADO( int medio, int submedio, double *importe_ingresado );
extern int PROCESAR_PROMOCION( int medio, int submedio );
/***********Consulta Descrip**********/
#define _X1  32
#define _X2  53
#define _Y1   5
#define _Y2  20
/************************************/

extern void SET_TARJERTAS_ON( int valor);
extern int mostrarmensaje;
void RECUPERAR_EVENTO_IMPORTADO( long *caja_z, long *id_evento, int *caja, int *pos);
/*****************************************************************************/
int _consulta_descrip( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y;

    y = _Y1 + dato;
    switch( comando ) {
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                setFieldValuepChar( b->index, 0, d_descrip.valor );
                setFieldValuepChar( b->index, 1, d_descrip.descripcion );
            }
            #endif
            MOSTRAR_CADENA( _X1, y, d_descrip.descripcion );
            break;
        case _SKIP:
            SKIP2( dato );
			rta = dbEOF() || dbBOF();
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _COMPARAR:
            rta = 2;
            break;
    }

    return ( rta );
}

/*****************************************************************************/
int PAGOS( int modo, int ( funcion_val ) ( double importe ), int dev_cobro )
/*****************************************************************************/
{
    int rta = 0;
    int _ptr,_handle,_estado;
	int _h, h;

    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );
	_h = STACK_INS( sizeof( short ) );
    _modo = modo;

    if( _ESTADO == 3 ) { //por corte de luz.
		ExecuteAction( A_DES_VENTAS_ACT_PAGOS );
        rta = 1; //se pone respuesta = 1 porque si estaba en el estado 3 rta estaba en 1
		if( NRO_PAGARE >0 ) //LO pongo por la promo postpago
			SET_MEMORY_DOUBLE( __ram_resto, PAGOS_TOTAL_A_PAGAR() );
	}
	if( _ESTADO == 0 ) {
        //Se necesita que el archivo .rec de submedios contenga la informacion
        //de los medios y submedios utilizados para los cobros. Esta informacion
        //es posteriormente usada en el cierre Y.
        //Si se presiona ESC mientras se piden los medios de pago, la informacion
        //almacenada en el archivo .rec de submendios referida a este cobro debe
        //ser descartada para poder ser cargada nuevamente.
        /*CREAR_COPIA_ARCHIVO_REC( ARCH_REC_SUBMEDIOS, NO );*/
        rta = ( modo == _MODO_REPROCESAMIENTO ) || ( modo == _MODO_COBRANZA )
           || ( RAM_P_TICKET
             && ( ( RAM_TOTAL > 0.009 && !MODO_DEVOLUCION && !NC_PERMANENTE )
               || ( RAM_NOTA_CR && !NC_PERMANENTE ) || ( RAM_CAMBIO && RAM_TOTAL > -0.009 )
               || RAM_MODO_EJECUCION == INVENTARIO
               || ( ( MODO_DEVOLUCION || NC_PERMANENTE )
                 && ( RAM_TOTAL < -0.009 || ( config.pais == ECUADOR && RAM_TOTAL <= 0.0 ) ) ) ) );
        _SET_ESTADO( 1 );
    }
    /*----------------------- Setea variables globales -----------------------*/
    if( _ESTADO == 1 ) {
        if( modo != _MODO_REPROCESAMIENTO ) {
            SET_MEMORY_DOUBLE( __ram_importe_total, ROUND( RAM_TOTAL, 10, DECIMALES ) );
            SET_MEMORY_CHAR( __ram_discriminar, 0 );
            SET_MEMORY_CHAR( __ram_solicitar_autorizacion, 0 );
        }
        if( modo == _MODO_NC || ( MODO_DEVOLUCION && modo != _MODO_COBRANZA ) ) {
            SET_MEMORY_DOUBLE( __ram_importe_total, -RAM_IMPORTE_TOTAL );
        }
        //if( !MODO_DEVOLUCION ) {
            SET_MEMORY_CHAR( __ram_p_pago, 0 );
        //}
        SET_MEMORY_DOUBLE( __ram_variacion, 0 );
        SET_MEMORY_DOUBLE( __ram_intereses, 0 );
        CALCULAR_RETENCIONES();
		if( modo != _MODO_COBRANZA ) {
			if( !CR1_VERIFICAR_LIMITES_FACTURA( PAGOS_TOTAL_A_PAGAR(  ), 0 ) ) {
				CLOSE_RECUPERO( _ptr, _handle );
				return ( 0 );
			}
		}
        SET_MEMORY_DOUBLE( __ram_resto, PAGOS_TOTAL_A_PAGAR() );
        SET_MEMORY_DOUBLE( __ram_resto_me, 0.0 );
        SET_MEMORY_CHAR( __ram_mostrar_faltante_en_me, 0 );
        SET_MEMORY_CHAR( __ram_pedir_diferimiento, 0 );
        SET_MEMORY_DOUBLE( __ram_diferencia_redondeo, 0 );
        if( RAM_NRO_CLIENTE && RAM_FACTURA && EMITIR_FACTURA_REMOTA() ) {
            cliente_factura = RAM_NRO_CLIENTE;
        }
		if(! (RAM_IMPORTACION_PEDIDOS && importando_ticket_devolucion == 2)) //para la NC automatica en la anulacion por tarjeta
        LIMPIAR_VARIAS_TARJETAS();//alejo
        if( rta ) {
            _SET_ESTADO( 2 );
        }
    }
    if( _ESTADO == 2 ) {
        /*-------------------------- Pide Datos Adicionales ----------------------------*/
        if( CODIGO_SOLICITUD_DATOS_ADIC_CLTE && modo == _MODO_VENTA ) {
            PAGOS_PEDIR_DATOS_ADICIONALES( 0, 0, CODIGO_SOLICITUD_DATOS_ADIC_CLTE, 0, D_CLIENTES,
                                           NULL, 1, NULL, NULL , '\0', '\0');
        }
        _SET_ESTADO( 3 );
    }
    /*-------------------------- Prepara pantalla ----------------------------*/
	if( modo == _MODO_COBRANZA ) {
        ExecuteAction( A_DES_COBROS_ACT_PAGOS );
    }
    else {
        if( MODO_NEGOCIO == SERVICIOS ) {
            ExecuteAction( A_DES_COBROS_ACT_PAGOS );
        }
        else {
            ExecuteAction( A_DES_VENTAS_ACT_PAGOS );
        }
    }
	#ifdef INVEL_W
		//SETEA_FOCUS_TPS( );
	#endif INVEL_W

    PAGOS_CONFECCIONA_PANTALLA( ( int )modo, dev_cobro );
    SETEAR_TIEMPO( _ENTRADA_PAGO );
    PAGOS_MUESTRA_TOTAL( RAM_RESTO );
    if( modo != _MODO_COBRANZA && modo != _MODO_FAC_REM && modo != _MODO_NC
     && modo != _MODO_REPROCESAMIENTO ) {
			 MENSAJE( ST(S_GRACIAS_POR_SU_COMPRA) );
    }
    MOSTRAR_RETENCIONES();
    /*----------------------------- ingresa pagos ----------------------------*/
    if( _ESTADO == 3 ) {
        if( PAGOS_PIDE_PAGO( modo, funcion_val, dev_cobro ) ) {
			if( VALIDAR_COBROS_VENTA( ) ){
            SET_MEMORY_DOUBLE( __ram_vuelto, -RAM_RESTO );
            ELIMINA_BASES_DTOS_POR_TECLA();
            _SET_ESTADO( 4 );
            #ifdef COMPILAR_DISPLAY
            DISPLAY_LINEA( ST( S_______GRACIAS_______ ), 1 );
            DISPLAY_LINEA( ST( S____POR_SU_COMPRA____ ), 2 );
            #endif
				/*----------- Imprime el ticket si debe hacerlo al final -----------*/
				//if( modo != _MODO_COBRANZA && modo != _MODO_NC && !EMITIR_FACTURA(  )
				//  && IMPRIMIR_TICKET_AL_FINAL ) {
				//  IMPRIMIR_TICKET_COMPLETO(  );
				//}
			}else{
				rta = 0;
			}
        }
        else {
            rta = 0;
        }
    }

    /*-------------------------- Imprimir ticket completo --------------------*/
    if( _ESTADO == 4 ) {
		//if( config.pais != ELSALVADOR || ( config.pais == ELSALVADOR && IMPRIMIR_TICKET_AL_FINAL ) ) {
		if( ORDEN_VOUCHER_DE_TARJETA == _VOUCHER_PRETICKET ) {
			//se imprime el voucher de tarjeta antes del ticket x un problema de lentitud.
			//pero esto solo funciona si la impresi�n del ticket es al final
			if( IMPRIMIR_TICKET_AL_FINAL ) {
				PAGOS_IMPRIME_PAGOS_ESPECIALES( NO, SI );
			} else {
				MENSAJE( "ERROR EN CONFIGURACION FORMATOS IMPRESION" );
				GRABAR_LOG_SISTEMA( "ERROR EN CONFIGURACION FORMATOS IMPRESION" ,LOG_VENTAS,2);
			}
		}
        CLOSE_RNV_VIRTUAL();
        OPEN_RNV_VIRTUAL();
		/**/GRABAR_LOG_SISTEMA("INICIO IMPRESION TICKET",LOG_VENTAS,4);//guille
        PAGOS_IMPRIMIR_TICKET_COMPLETO( modo );
		/**/GRABAR_LOG_SISTEMA("FIN IMPRESION TICKET",LOG_VENTAS,4);
        imprimiendo = 1;
		//while( imprimiendo )  // se queda en este while hasta que la funcion entry setee la variable en 0 cuando termine de imprimir
			//SLEEP(100);
		_SET_ESTADO( 5 );
    }

    /*-------------------------- Agrega intereses ----------------------------*/
    if( _ESTADO == 5 ) {
        PAGOS_AGREGA_ITEM_INTERESES();
        _SET_ESTADO( 6 );
    }
    /*-------------------------- Agrega variaciones --------------------------*/
    if( _ESTADO == 6 ) {
        PAGOS_AGREGAR_VARIACIONES();
        _SET_ESTADO( 71 );
    }
	if( _ESTADO == 71 ) {                  //xxx
      if( modo != _MODO_COBRANZA ) {
         PAGOS_AGREGAR_DESCUENTO_TARJETA(  );
      }
        _SET_ESTADO( 7 );
    }
    /*------------------------- Imprime modos de pago ------------------------*/
    if( _ESTADO == 7 ) {
        //ESTO VA PARA LAS FISCALES? YA SE COBRO LA RETENCION, VOLVER A CALCULARLA NO PARECE LOGICO
		//queda a analisis futuros
		if( IMPRESORA_FISCAL != FISCAL )
        CALCULAR_RETENCIONES_TOTALES();
		CALCULA_AHORRO_OBTENIDO();
  		

		/* esta pregunta hay que analizarla y probar para definir mejor las consultas por cada modo y
		agruparlas para merjorar la comprensi�n, existia un error an utilizar nota de credito y 
		no imprimir cobros en el CONFTPV(?�)*/

		if( ((IMPRESORA_FISCAL )&&
			
				(  !EMITIR_FACTURA() || 
					( modo == _MODO_NC  &&  
					( !EMITIR_FACTURA_REMOTA() || IMPRIMIR_TICKET_SI_ES_REMOTA ) &&  //no emitir remota
					!( modo == _MODO_COBRANZA && IMPRESORA_FISCAL ) &&               // nos es fiscal y es modo cobranza
					!(  modo == _MODO_COBRANZA  && MODO_NEGOCIO == SERVICIOS )&&	//modo servicios
					( ( modo != _MODO_COBRANZA ||( modo == _MODO_COBRANZA && IMPRIMIR_COBROS ) ) // cobranza--cambio para que no controle sino es modo cobranza
						|| modo == _MODO_VENTA ) 
					)
				)//
			 )||
		
			((!IMPRESORA_FISCAL )&&( ( !EMITIR_FACTURA(  ) ) && ( !EMITIR_FACTURA_REMOTA() || IMPRIMIR_TICKET_SI_ES_REMOTA )
				&& !( MODO_NEGOCIO == SERVICIOS && modo == _MODO_COBRANZA )
				&& ( IMPRIMIR_COBROS || modo == _MODO_VENTA ) )
			)){

			if( TKT_CONFIGURABLE && !IMPRESORA_FISCAL) {
				IMPRIME_TKT_IMPORTE_TOTAL( 1 );
			}
			rta = PAGOS_IMPRIME_PAGO( modo );
            #ifdef COMPILAR_TECLA_REPROCESAR
            if( !rta ) {
                REPROCESAR_TARJETAS( ( char )modo, NO );
				if( RAM_ANULAR_TICKET > 100 )//Tarea: UAA-59
					FIN_DE_TICKET( NO );
			}
            #endif
        }
        if( rta ) {
            _SET_ESTADO( 8 );
			if(IMPRESORA_FISCAL && !VAR_TARJETAS_STATUS)
				_SET_ESTADO( 17 );//Seba
        }
    }
    if( _ESTADO == 8 ) {
        PAGOS_VALIDA_CHEQUE_CLIENTE();
        _SET_ESTADO( 9 );
    }
    if( _ESTADO == 9 ) {
        /* codigo obsoleto
        *if( modo != _MODO_COBRANZA && UTILIZA_PROMOCIONES && !RAM_NOTA_CR ) {
        *    if( UTILIZA_MODULO_PROMOCIONES ) {
        *    }
        *    else {
        *        //glog( "Calculando promociones PieTicket TRADICIONALES" );
        *        //CALCULAR_PROMOCIONES( _PIE_TICKET );
        *    }
        *}
        */
        _SET_ESTADO( 10 );
    }

    if( _ESTADO == 10 ) {
        //IMPRIMIR_CUPONES_DE_PROMOCION( _PIE_TICKET );
        _SET_ESTADO( 11 );
    }

    /*----------------------------- Imprime pie ------------------------------*/
    if( _ESTADO == 11 ) {

		if(IMPRESORA_FISCAL == FISCAL && RAM_TICKET_FACTURA)//Seba
		{
			CERRAR_TIQUE_FACTURA(RESP_INSC, clientes.cond_iva);
		}
		if(IMPRESORA_FISCAL == FISCAL && RAM_NOTA_CR)
		{
			CERRAR_NOTA_DE_CREDITO(RESP_INSC, clientes.cond_iva);
		}
        /*if( !EMITIR_FACTURA() && modo != _MODO_COBRANZA
         && ( !EMITIR_FACTURA_REMOTA() || IMPRIMIR_TICKET_SI_ES_REMOTA ) ) {
            IMPRIME_PIE();
        }
        else {
            if( VALIDAR_CLIENTE_AFINIDAD() ) {
                VALIDAR_LIMITE_DGI();
                PRN_FIN_TICKET();
            }
        }*/
        _SET_ESTADO( 12 );
    }
    /*--------------------------- Imprime vouchers ---------------------------*/
     
    if( _ESTADO == 12 ) {
		int posicion = 0, posicion2 =0;

		for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO;ADD_MEMORY_INT( _h, 1 ) ) {
			h = GET_MEMORY_INT( _h );
        // si entra por reproceso de pago no imprime la copia
			if( medios[_PAGO_MODO( h )].tarjeta
					&& IMPRESORA_FISCAL == FISCAL && EMITIR_VOUCHER_TARJETA && VOUCHER_TARJETA_FISCAL == SI ) {// Imprime voucher de Tarjeta
				if( VARIAS_TARJETAS_POR_PAGO ) {
					LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
				}
				IMPRIME_TKT_DESCRIPCION_TARJETA( h, posicion2 );
			}
        if( VARIOS_COPIA_TICKET_PRN ) {
//            CLOSE_COPIA_TICKET_PRN();
            if( SINCRONIZAR_PRINTER() ) {
                IMPRIME_COPIA_TICKET( SI );
            }
        }
        	if( medios[_PAGO_MODO( h )].tarjeta
				&& IMPRESORA_FISCAL == FISCAL && EMITIR_VOUCHER_TARJETA == 2  && VOUCHER_TARJETA_FISCAL == SI ) { // Imprime para comercio y cliente
					if( h >= 0) {
						IMPRIME_TKT_DUPLICADO_TARJETA(h, posicion2); //que pasa si hay mas de 1 tarjeta
				   }
					posicion2++;


			}
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && VAR_TARJETAS_STATUS ) {
            VALIDAR_VOUCHER( COD_VALIDACION_VOUCHER( VAR_TARJETAS_TARJETA ) );
        }
        if( SINCRONIZAR_PRINTER() ) {
            IMPRIME_COPIA_VOUCHER( SI );
			}
        }
        _SET_ESTADO( 14 );
    }
    /*-------------------------- Valida comprobantes --------------------------*/
    if( _ESTADO == 14 ) {
		if( !SI_FRANQUE0_AL_FINAL ) {
			VALIDAR_MEDIOS( modo );
		}
        _SET_ESTADO( 15 );
    }
    if( _ESTADO == 15 ) {
        #ifdef COMPILAR_COMPROMISO_200
        PAGOS_VALIDA_200();
        #endif
        _SET_ESTADO( 16 );
    }
    if( _ESTADO == 16 ) {
        if( _RAM_NUMERO_TICKET_TOMRA( 0 ) ) {
            if( MODALIDAD_TOMRA == _POR_TIPO || !GET_ERROR_EN_RED() ) {
                VALIDA_ENVASES_TOMRA();
                if( MODALIDAD_TOMRA == _POR_ARCHIVO ) {
                    if( !MARCAR_TABLA_ENVASES() ) {
                        MENSAJE_STRING( S_ERROR_AL_ACTUALIZAR_TABLA_ENVASES );
                    }
                }
            }
            else {
                MENSAJE_STRING( S_SIN_RED_NO_SE_PROCESARON_TICKETS_DEV_ENVASES );
            }
            VACIAR_TABLA_AUXILIAR();
            //while ( _RAM_NUMERO_TICKET_TOMRA(i) ) {
            //  _SET_MEMORY_LONG( __ram_numero_ticket_tomra, i, 0 );
            //  i++;
            //}
        }
        _SET_ESTADO( 17 );
    }
    /*----------------------------- Graba archivos ---------------------------*/
    if( _ESTADO == 17 ) {
        if( PAGOS_GRABA_PAGO( modo, dev_cobro ) ) {
#ifdef _DEBUG_RNV_REC
             CONTROL_CONSISTENCA_RNV_REC();
#endif
            _SET_ESTADO( 18 );
        }
        else {
            _SET_ESTADO( 19 );
        }
    }
    /*--------------------- Actualiza transacciones del CUS ------------------*/
    if( _ESTADO == 18 ) {
        CUS_GRABA_PAGO();
        _SET_ESTADO( 19 );
    }
    /*------------------- Recupera pantalla y muestra vuelto -----------------*/
    ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TARJETA_BORRAR, " ", " ", NULL, " " );
    /*if( MODO_NEGOCIO == SERVICIOS ) {
        ExecuteAction( A_DES_PAGOS_ACT_COBROS_ESP );
    }
    else {
        ExecuteAction( A_DES_PAGOS_ACT_VENTAS );
    }                                  */
    if( _ESTADO == 19 ) {
        if( DONACION_DE_VUELTO ) {
            MOSTRAR_VUELTO_EN_GRANDE( RAM_RESTO + RAM_TOTAL_DONACION );
        }
        else {
            MOSTRAR_VUELTO_EN_GRANDE( RAM_RESTO );
        }
        if( modo != _MODO_REPROCESAMIENTO ) {
            SET_MEMORY_DOUBLE( __ram_total_anterior, PAGOS_TOTAL( 1 ) );
        }
        rta = 1;
    }
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
    return ( rta );
}
/*****************************************************************************/
PAGOS_PIDE_PAGO( int modo, int ( funcion_val ) ( double importe ), int dev_cobro )
/*****************************************************************************/
{
    int seguir = 1, k = 0, _k = 0, rta = 0, control_import_dev = 1, debe_anular = NO;
    int j = 0,ok = 0, anular_medio = SI, tarjetayaanulada =0;
    char *tmp;
	/*char referenciada = ( RAM_MODO_DEVOLUCION == _DEVOL_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL ||
        RAM_MODO_DEVOLUCION == _DEVOL_INTERME || RAM_MODO_DEVOLUCION == _NC_TOTAL ||
        RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME );*/

	char mediosReferenciados = ( RAM_MODO_DEVOLUCION == _DEVOL_TOTAL ||RAM_MODO_DEVOLUCION == _NC_TOTAL );

    glog("PIDE PAGO", LOG_PAGOS, 1); //este log es para saber si llega hasta aca cuando se caia la caja con promos
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
    /*--------- Si pasa la tarjeta va directamente a tarjeta ----------*/
    if( VAR_TARJETAS_STATUS && MEDIO_DE_PAGO_HABILITADO( modo, _medio_tarjeta, dev_cobro )
			&& PRIMER_MEDIO_DE_PAGO_TARJETA ) {
		#if defined(INVEL_W) || defined(INVEL_L)
			ExecuteSSAction( A_RESALTAR_MEDIO_PP, _medio_tarjeta, "", "", NULL, " " );
        #endif
		PAGOS_INGRESA_PAGO( _medio_tarjeta, modo, dev_cobro );
    }
	if( importando_ticket_devolucion > 0 ) {
        if( PAGOS_CARGAR_MEDIOS_IMPORTACION(  ) ) {
            UNGETCH( 32 );
        }
        if( importando_ticket_devolucion != 2) //para la NC de anulacion de tarjeta no cereamos esta variable
        importando_ticket_devolucion = 0;
    }
    if( ( RAM_NOTA_CR || MODO_DEVOLUCION ) && mediosReferenciados  ) {
		int haytarjeta = NO;
        //Se cargan los submedios usados en MSI
        CARGAR_MEDIOS_AM( );
        //Se cargan las tarjetas, ya que es el medio preferido para devolucion
        haytarjeta = PAGOS_CARGAR_TARJETAS_IMPORTACION( );
        //Pantalla
        PAGOS_MOSTRAR_RESTO( RAM_RESTO );
        PAGOS_MUESTRA_TOTAL( RAM_RESTO );
        
		if( haytarjeta == SI)
			CR1_MUESTRA_TARJETA();
    }

	if( modo == _MODO_COBRANZA && CANTIDAD_REGISTROS_REC( ARCH_REC_DATOS_PAGATODO,
			sizeof( struct DatosCobroPagaTodo ) ) ) {
		struct   DatosCobroPagaTodo datosCobrosAux;

		memset( &datosCobrosAux, 0x00, sizeof( struct DatosCobroPagaTodo ) );
		RECUPERAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobrosAux,
			sizeof( struct DatosCobroPagaTodo ), 0 );
		if( datosCobrosAux._rta_envio != OPERACION_FINALIZADA ) {
			if( datosCobrosAux._estado_envio == TRANSACCION_INICIAL_ENVIANDO ||
					( datosCobrosAux._estado_envio == TRANSACCION_INICIAL_ENVIADA
					&& datosCobrosAux._rta_envio == OPERACION_NECESITA_REVERSO ) ||
					( datosCobrosAux._estado_envio == TRANSACCION_REVERSO_ENVIANDO ) ||
					( datosCobrosAux._estado_envio == TRANSACCION_REVERSO_ENVIADA
					&& datosCobrosAux._rta_envio != 0 ) ) {
				char *nombreOperacion[] = { "PAGO", "REVERSA", "DEVOLUCION" };
				int   intentos = REINTENTOS_PAGATODO, rta2 = 1, operacionNro;
				char  mensajeAux[81];
				operacionNro = ( datosCobrosAux.__devolucion ) ? PAGATODO_DEVOLUCION : PAGATODO_REVERSA;
				sprintf( mensajeAux, "ENVIANDO TRANSACCION DE %s", nombreOperacion[operacionNro-1] );
				do {
					MENSAJE( mensajeAux );
					rta2 = ENVIAR_TRANSACCION_PAGATODO( operacionNro, &datosCobrosAux );
					intentos--;
				} while( rta < 0 && intentos );
				if( rta2 ) {
					sprintf( mensajeAux, "ERROR AL ENVIAR %s", nombreOperacion[operacionNro-1] );
					MENSAJE( mensajeAux );
				}
				if( !intentos ) {
					while( !( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI,
							SI, mensajeAux, "AUTORIZA CONTINUAR",
							_F_SUP_REVERSO_PAGATODO_ERROR, SI ) ) );
				}
				UNGETCH( 27 );
			}
			else if( ( datosCobrosAux._estado_envio == TRANSACCION_INICIAL_ENVIADA
					&& datosCobrosAux._rta_envio == OPERACION_OK ) ||
					( datosCobrosAux._estado_envio == TRANSACCION_REVERSO_ENVIADA
					&& datosCobrosAux._rta_envio == OPERACION_OK ) ) {
				datosCobrosAux._rta_envio = OPERACION_FINALIZADA;
				ACTUALIZAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char * ) &datosCobrosAux,
					sizeof( datosCobrosAux ), 0 );
				UNGETCH( 32 );
			}
			else if( ( datosCobrosAux._estado_envio == TRANSACCION_INICIAL_ENVIADA
					&& datosCobrosAux._rta_envio == OPERACION_NO_NECESITA_REVERSO ) ) {
				UNGETCH( 32 );
			}
		}
		else {
			UNGETCH( 32 );
		}
	}
	MUESTRA_CLIENTE( 1 );
	glog("INGRESE MEDIO PAGO", LOG_PAGOS, 1); //este log es para saber si llega hasta aca cuando se caia la caja con promos
	ExecuteAction( A_PAGOS_FOCUS );
	while( seguir ) {
        //MUESTRA_CLIENTE( 1 );
        DISABLE_PAD_NUMERICO();
        if( _k ) {
            k = _k;
            _k = 0;
        }
        else {
            k = GETCH();
        }
		//BORRAR_MENSAJE();
        if( k != -999 && k != 999 ) {
            if( GRABAR_LOG_ACTIONS() == 'S' ) {
                char buff[100];
                sprintf( buff, "TECLA %d", k );
                GRABAR_LOG_SISTEMA( buff ,LOG_DEBUG,4);
            }
        }
        ENABLE_PAD_NUMERICO();
        switch( k ) {
            case 27:
                /*-------- ESC ---------*/
				if( 1 ) {
					/// esto es igual que la tecla -
					int j = RAM_P_PAGO ;
					int positarjeta[20];
					int hh, h1=0, auxi = 0;
					int anulotodaslastarjetas =SI;
					

					for( hh = 0;RAM_P_PAGO> 0 && hh < RAM_P_PAGO; hh++ ){
						_PAGO_AUTO_TARJETA(hh);
						if( medios[_PAGO_MODO( hh )].tarjeta ) {
							if( VARIAS_TARJETAS_POR_PAGO ) {
								if( strlen(_PAGO_AUTO_TARJETA(hh)) > 1 ){
									positarjeta[hh]= h1;
									h1++;
								}
							}
						}
					}
					//ojo aca el voucher se debe borrar si no hubo operaciones anuladas autorizadas
					if( RAM_P_PAGO > 0 && tarjetayaanulada == 0) //LE PUSE LA CONDICION QUE BORRE SI HAY PAGOS
						unlink("voucher.$$$");
					//aca anulamos las tarjetas pendientes que no se usaron la tecla - sino que se hizo ESC de una
					for( j = RAM_P_PAGO-1 ; j >= 0 ; j-- ){
						
						if( TARJETA_APROBADA_ONLINE( j ) != 0 && TARJETA_APROBADA_ONLINE( j ) != -2 && TARJETA_APROBADA_ONLINE( j ) != -1  
							|| (VAR_TARJETAS_ON_LINE == 2 && !VAR_TARJETAS_UNICAMENTE_OL ) ) {
								anular_medio = _ANULAR_TARJETAS( j-1 , SI, positarjeta[j]-1 );//TECLA ESCAPE
								if(anular_medio == 0){
									anulotodaslastarjetas =NO;
									tarjetayaanulada =1;
								}
						} else {
									glog("TARJETA no aprobada online",1,1);
						}


						//anular_tarjeta = 1; ANULAR la tarjeta
						//anular_tarjeta = 0; CANCELAR la anulacion de la tarjeta
					}
					
					if( anulotodaslastarjetas == NO) {
						anular_medio =0;  //no deja pasar a ventas
					}

                }
                if( anular_medio
						&& ( modo == _MODO_VENTA || modo == _MODO_FAC_REM || modo == _MODO_COBRANZA || modo == _MODO_NC ) 
						&& !RAM_REPROCESANDO
						&& !VARIOS_RESTRINGIR_VOLVER_DE_PAGOS
						&& ( !( RAM_MODO_DEVOLUCION == _NC_RF_INTERME && RESTRINGIR_VOLVER_DE_PAGOS ) ||
						!( RAM_MODO_DEVOLUCION == _DEVOL_INTERME && RESTRINGIR_VOLVER_DE_PAGOS ) ) ) {
                    seguir = 0;
                    rta = 0;
                    SETEAR_TIEMPO( _ENTRADA_REG );
                    LIMPIAR_VARIAS_TARJETAS();
					if( MODO_NEGOCIO == SERVICIOS ) {
							if( BORRAR_TARJETA_AL_SALIR_DE_PAGOS ) {
								CEREAR_STRUCT( _var_tarjetas );
                            CR1_MUESTRA_TARJETA();
                        }
                        CANCELA_CHEQUES();
                    }
                    BORRAR_MENSAJE();
                }
				
				if( anular_medio  ) {
					if(RAM_CLIENTE_AFINIDAD ){
						 RECUPERAR_CLIENTE_AFINIDAD(  );
					}
					BORRAR_BIN_DATOS_ADICIONALES( D_MEDIOS_PRE_IMPORTE );
					SET_MEMORY_INT( __ram_calcular_retenc_nc , NO );
					//Se cerea var_tarjetas_status para evitar lo especificado en tarea 4581 cuando PRIMER_MEDIO_DE_PAGO_TARJETA = SI
					SET_MEMORY_CHAR( __var_tarjetas_status, 0 );
					/*ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_CUPO_CHEQUE_BORRAR, " ", " ", NULL, " " );
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TOTAL_ANT_BORRAR, " ", " ", NULL, " " );
					ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_RETENCION_DGR_BORRAR_PAGO, " ", " ", NULL, " " );*/
					if( RAM_NRO_CLIENTE && pant_activa_cliente == PANTALLA_PAGO ) {
						SET_MEMORY_LONG( __ram_nro_cliente, 0 );
						CEREAR_PERFIL_CLIENTE();
						MOSTRAR_CLIENTE_EN_PANTALLA( );
					}
					SET_MEMORY_DOUBLE( __ram_puntos_esp_3, 0.00);//cereo el total de la impresora en el esc
					SETEAR_DEBITO_POS( 0) ;
					//por ahora no conviente usar este, conviene que de inconsistencia fiscal if( debe_anular == SI ) SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_ERROR_IMPRESORA ); 
					if( debe_anular == SI ){
						MENSAJE_STRING( S_IMPRESORA_ERROR );
						PAUSA( 18 );
						BORRAR_MENSAJE();
					}
				}
				if( IsCashActivo() ) {
					SET_MEMORY_CHAR( __ram_venta_fraccion, 0 );
					ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " ");
				}
                break;
            case 32:
                /*-------- TOTAL -------*/
				GRABAR_LOG_SISTEMA_CONFIG("ENTRANDO FUNCION VALIDAR PAGO",LOG_VARIOS,4);
				
				
				
				if( modo != _MODO_COBRANZA ) {
					rta = CR1_VERIFICAR_LIMITES_FACTURA( PAGOS_TOTAL_A_PAGAR(  ), 0 );
				} else
					rta = SI;

                if( MODO_DEVOLUCION ) {
                    control_import_dev = CONTROL_IMPORTE_DEVOLUCION( );
                }

				if( rta && IMPRESORA_FISCAL == FISCAL && DOCUMENTO_ABIERTO( )== 2 ) { 
					rta = NO;
					debe_anular =SI;
					_k= 27 ; //SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_ERROR_IMPRESORA ); 
				}
				/* por ahora no controlamos el tema de las diferencias... 
				if( rta && ! FISCAL_TOTAL_COMPROBANTE( SI ) ){
					rta = NO;
					_k= 27 ; 
				}*/
				//saquemos el ping porque puede producir bloqueo con otro comando al jserver (mutex)
				setHabPing(0);

				if( rta && control_import_dev ){
                if( rta && VALIDAR_PAGO( modo ) ) {
                    GRABAR_LOG_SISTEMA_CONFIG("ENTRANDO FNCION PROCESAR DONACIONES",LOG_VARIOS,4);
                    if( PROCESAR_DONACIONES() ) {
						GRABAR_LOG_SISTEMA_CONFIG("ENTRANDO FNCION PEDIR_AUTORIZACION_ON_LINE",LOG_VARIOS,4);
						if( PEDIR_AUTORIZACION_ON_LINE( 0, dev_cobro ) ) {
                            if( ( funcion_val == NULL || funcion_val( RAM_TOTAL ) ) && VALIDAR_MONEDERO_ELECTRONICO( -RAM_RESTO ) ) {
								GRABAR_LOG_SISTEMA_CONFIG("ENTRANDO FNCION DEBE_ABRIR_CAJON",LOG_VARIOS,4);
								if( DEBE_ABRIR_CAJON(  ) ){
									mostrarmensaje=0;
									ABRIR_CAJON(  );
								}
								rta = 1;
								seguir = 0;
								SETEAR_TIEMPO( _ENTRADA_OCIO );
							} else {
								SET_LASTKEY( 0 );
								rta = 0;
								REPROCESAR_TARJETAS( modo, NO );
							}
							ACTUALIZA_REDONDEO_DE_VUELTO( modo );
                        }
                    }
                }
				GRABAR_LOG_SISTEMA_CONFIG("FIN TOTAL......",LOG_VARIOS,4);
                if( RAM_NRO_CLIENTE && pant_activa_cliente == PANTALLA_PAGO ) {
                    long nro_cliente;
                    //Para limpiar la informacion del cliente en la pantalla pago
                    nro_cliente = RAM_NRO_CLIENTE;
                    SET_MEMORY_LONG( __ram_nro_cliente, 0 );
                    //CEREAR_PERFIL_CLIENTE();
                    MOSTRAR_CLIENTE_EN_PANTALLA( );
                    SET_MEMORY_LONG( __ram_nro_cliente, nro_cliente );
                }
                BORRAR_MENSAJE();
                }
				LEER_TARJETAS_REC();//Seba

				if( rta && IMPRESORA_FISCAL == FISCAL && DOCUMENTO_ABIERTO( )== 2 ) { 
					rta = NO;
					_k = 27 ;  
					debe_anular =SI;
					seguir = SI;
				}
				if( IsCashActivo() ) {
					ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " ");
				}
				setHabPing(1); //habilitamos el ping
                break;
            case 45:
                /*-------- '-' : Anulacion ------*/
                if( MODO_DEVOLUCION ) {
                    ACUMULAR_IMPORTE_DEVOLUCION( ( int )( _PAGO_MODO( RAM_P_PAGO - 1 ) + 1 ),
                        -( _PAGO_IMPORTE( RAM_P_PAGO - 1 ) ) );
                }
                PAGOS_ANULAR_PAGO();
                break;
            case '~':
                /*-------- '~' : Intereses ------*/
                if( RAM_P_PAGO == 0 ) {
                    PAGOS_INTERESES();
                }
                break;
            case '@':
                /*-------- '@' : Discrimina IVA ------*/
                if( RAM_P_PAGO == 0 ) {
                    PAGOS_DISCRIMINA_IVA();
                }
                break;
            case '+':
                /*-------- '+' : Pide autorizacion ------*/
                if( RAM_P_PAGO == 0 ) {
                    PAGOS_MARCA_AUTORIZACION();
                }
                break;
            case -65:
                /*---- F7 ----*//*--- Validacion tarjeta ---*/
                if( RAM_P_PAGO == 0 && !PASAR_TARJETA_SOLO_AL_INICIO ) {
                    ClearPanel( pPanel );
                    if( VAR_TARJETAS_STATUS == 2 && config.momento_recargo == AL_RENGLON ) {
                        CAMBIA_TARJETA_CREDITO();
                    }
                    else if( config.momento_recargo != AL_RENGLON ) {
						VALIDA_TARJETA( NO, NO, VENTAS, NO, NULL, SI, -1/*NO*/ );
                    }
                    CR1_MUESTRA_TARJETA();
                    Orden = NUMERO_PANEL_MINIMO_EN_PAGOS;
                    PreparePanel( &pPanel, Orden );
                    ShowPanel( pPanel );
                }
                break;
            case 110:
                IDENTIFICAR_BILLETES();
                break;
			case 115:
				MOSTRAR_SALDO_PROMOCIONES( );
				break;
            case 132:
                /*---- Menu de funciones ---*/
                _k = SELECCIONA_FUNCION( 2 );
                break;
            case 148:
                /*--- Seleccionar offline ---*/
                PAGOS_ENVIAR_OFFLINE();
                break;
            case 149:
                /*-------- saldo cheque --------*/
                if( PEDIR_CLIENTE_EN_CHEQUES ) {
                    ok = 1;
                    for( j = 0;j < RAM_P_PAGO;j++ ) {
                        if( medios[_PAGO_MODO( j )].cheque ) {
                            ok = 0;
                        }
                    }
                    if( ok ) {
                        ELEGIR_CLIENTE_NUMERICO( 1, 0 );
                    }
                    else {
                        MENSAJE_STRING( S_ANULE_CHEQUES_CAMBIAR_CLIENTE );
                        PAUSA( 18 );
                        BORRAR_MENSAJE();
                    }
                }
                else {
                    MENSAJE_STRING( S_CLIENTE_EN_CHEQUES_NO_CONF );
                }
                break;
			case 150: //mostrar saldo en efectivo de ventas para cashplus
				MOSTRAR_SALDO_EFECTIVO();
				
				break;
            case 159:
                /*------ Pedir Diferimiento ----*/
                SET_MEMORY_CHAR( __ram_pedir_diferimiento, !RAM_PEDIR_DIFERIMIENTO );
                MOSTRAR_PEDIR_DIFERIMIENTO();
                break;
			case 172: //ojo esta tecla se deberia solo activar si hay un medio de pago cashplus
				if( config_tps.ActivarCashPlus == 1 
					&& (config_tps.MinimoaHabilitar == 0 
						|| (config_tps.MinimoaHabilitar >0 && config_tps.MinimoaHabilitar <= RAM_IMPORTE_TOTAL )) 
					&&TARJ[VAR_TARJETAS_TARJETA].tipo_tarjeta & _TIPO_CASH_ADVANCE) {
					SET_MEMORY_CHAR( __ram_venta_fraccion, 0 );
					if( IsCashActivo() )
						ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
					else{
						ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_AGREGAR, "RETIRO ACTIVADO", " ", NULL, NULL ); 
					}
				} else
						ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
				
				break;
            case 180:
                if( RAM_RESTO < 0 && _medio_moneda_extranjera >= 0
                 && !_PAGO_VUELTO( RAM_P_PAGO - 1 ) && !_PAGO_VUELTO_MEDIO( RAM_P_PAGO - 1 ) ) {
                    if( !RAM_RESTO_ME ) {
                        if( !PEDIR_SUPERVISOR_VUELTO_MONEDA_E
                         || SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_VUELTO_ME_ ),
                                              ST( S_AUTORIZ__VUELTO_MONEDA_EXT_ ),
                                              _F_SUP_VUELTO_ME, SI ) ) {
                            SET_MEMORY_DOUBLE( __ram_resto_me,
                                               PAGOS_ML_A_ME( RAM_RESTO,
                                                              _medio_moneda_extranjera ) );
                        }
                    }
                    else {
                        SET_MEMORY_DOUBLE( __ram_resto_me, 0.0 );
                    }
                    PAGOS_MUESTRA_TOTAL( RAM_RESTO );
                }
                else if( RAM_RESTO > 0 && _medio_moneda_extranjera >= 0 ) {
                    SET_MEMORY_CHAR( __ram_mostrar_faltante_en_me, !RAM_MOSTRAR_FALTANTE_EN_ME );
                    PAGOS_MOSTRAR_RESTO( RAM_RESTO );
                }
                break;
            case 182:
                if( !RAM_NRO_CLIENTE ) {
                    VERIFICA_EMPADRONAMIENTO_CLIENTES( NO );
                }
                break;
			case 183:
				//Franqueo Cobros
				if( modo == _MODO_COBRANZA ) {
					CR1_FRANQUEO( RAM_TOTAL, modo );
				}
                break;
			
            case FLECHA_ARRIBA:
                if( Orden > NUMERO_PANEL_MINIMO_EN_PAGOS ) {
                    Orden--;
                    DestroyPanel( pPanel );
                    PreparePanel( &pPanel, Orden );
                    ShowPanel( pPanel );
                };
                break;
            case FLECHA_ABAJO:
                if( Orden < NUMERO_PANEL_MAXIMO_EN_PAGOS ) {
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
			case 137:{//abrir cajon por tecla;
					if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S__ABRIR_CAJON_ ),
						ST( S__ABRIR_CAJON_ ), RAM_COD_FUNCION, SI ) ) {
							ABRIR_CAJON() ;
						}
					 }



            default:
                // Continua solo si el nro de pago actual es menor o igual
                // a CANTIDAD_MAXIMA_PAGOS
                if( RAM_P_PAGO >= CANTIDAD_MAXIMA_PAGOS ) {
                    tmp = ST( S_HA_ALCANZADO_LIMITE_MEDIOS_PAGO );
                    sprintf( tmp, tmp, CANTIDAD_MAXIMA_PAGOS );
                    MENSAJE( tmp );
                    BORRAR_MENSAJE();
                }
                else
                if( k >= '1' && k <= '9' ) {
                    if( (medios[k - '1'].nro && MEDIO_DE_PAGO_HABILITADO( modo, k - '1', dev_cobro )
                     && RAM_RESTO > 0) || (MODO_CHEQUE_COMO_UN_MEDIO && medios[k - '1'].cheque) ) {
                        #if defined(INVEL_W) || defined(INVEL_L)
                        ExecuteSSAction( A_RESALTAR_MEDIO_PP, k - '1', "", "", NULL, " " );
                        #endif
                        if( GetConfig( CONF_TOUCH_SCREEN ) ) {
                            /* Informo al touch screen un cambio de pantalla */
                            ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "20" );
                            PAGOS_INGRESA_PAGO( k - '1', modo, dev_cobro );
                            /* Vuelvo a la pantalla anterior del touch */
                            ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "03" );
                        }
                        else {
                            PAGOS_INGRESA_PAGO( k - '1', modo, dev_cobro );
                        }
                    }
                }
                break;
        }
        if( RAM_ANULAR_TICKET ) {
            rta = 0;
            seguir = 0;
        }
    }
    if( MODO_DEVOLUCION ) {
        BORRAR_ARCHIVO_REC( ARCH_REC_IMPORTE_DEVOLUCION );
    }
    if( Response ) {
        DestroyPanel( pPanel );
    }

    return ( rta );
}
/*****************************************************************************/
int PAGOS_INGRESA_PAGO( int nro, int modo, int dev_cobro  )
/*****************************************************************************/
{
    int       /*pant,*/ ok = 1, h;
    long autorizacion = 0, numero = 0;
    unsigned fecha_vto = 0;
    int cuenta = 0,
        banco = 0, cantidad_cupones = 0, comprobante = 0, datos_adicionales, indice_pos = 0;
    int plaza = 0;
    char tarjeta = 0, numero_tar[21], cuit[21], telefono[12], aux[50],
         cuenta_banco[20], serie[8], cod_medio_vuelto = 0, cod_submedio_vuelto = 0, tipo_vuelto;
    double importe = 0, valor_cupones = 0, importe_aux = 0, importe_ingresado = 0, resto,
           valor_redondeado=0.0, dif=0.0, importe_me = 0.0;
    int cod_post_cheq = 0, pos = -1;
    int cod_hora_acre = 0, salir = 0;
    int error = 1, k;
    int habilitado = 0, cheque_al_dia = 0;
    char cod_cuenta[15],nro_banco[16],nro_cheque[12],datosche[100],dche[150];
    int id_registro = 0, cod_valida_cheque = 0;
    #ifdef COMPILAR_MUTUALES
    char num_bono[20];
    #endif
    char cuit_comprador[12],telefono_comprador[12];
    int cant_veces = 0, cuotas, nro_cuota = 0;
    int rta = 0;
    int _h, _ptr, _handle;
	double    cotizacion;
	float		 variacion = 0.0, variacion_submedio = 0.0;
	int 		 cod_restriccion_art;
	int tarmed_abierta = NO;
	int area_ant = 0, tipo_ant = 0;

	OPEN_RECUPERO( &_ptr, &_handle );
	_h = STACK_INS( sizeof( short ) );

	memset( cuit, 0, sizeof( cuit ) );
    memset( telefono, 0, sizeof( telefono ) );
    memset( cuenta_banco, 0, sizeof( cuenta_banco ) );
    memset( serie, 0, sizeof( serie ) );
    memset( cod_cuenta, 0, sizeof( cod_cuenta ) );
    memset( nro_banco, 0, sizeof( nro_banco ) );
    memset( nro_cheque, 0, sizeof( nro_cheque ) );
    memset( numero_tar, ' ', sizeof( numero_tar ) );
    numero_tar[sizeof( numero_tar ) - 1] = 0;
    memset( telefono_comprador, 0, sizeof( telefono_comprador ) );
    memset( cuit_comprador, 0, sizeof( cuit_comprador ) );
    memset( aux, 0, sizeof( aux ) );
    memset( cuenta_banco, 0, sizeof( cuenta_banco ) );
    memset( serie, 0, sizeof( serie ) );
    memset( datosche, 0, sizeof( datosche ) );
    memset( dche, 0, sizeof( dche ) );
    memset( num_bono, 0, sizeof( num_bono ) );
	cod_restriccion_art = medios[nro].cod_restriccion_art;
	cotizacion = medios[nro].cotizacion;
    /*-------------- valida que no repita efectivo ---------------*/
    if( medios[nro].efectivo ) {
		//Log para robot
		GRABAR_LOG_SISTEMA( "Medio Seleccionado Efectivo" ,LOG_ROBOT,1);
        for( h = 0;h < RAM_P_PAGO;h++ ) {
            if( _PAGO_MODO( h ) == nro && IMPORTE_CONTADO_ADITIVO_EN_PAGOS ) {
                pos = h;
                // ok = 0;
            }
        }
    }
    /*-------------- valida que no repita cheque ---------------*/
    if( medios[nro].cheque ) {
		//Log para robot
		GRABAR_LOG_SISTEMA( "Medio Seleccionado Cheque" ,LOG_ROBOT,1);
        for( h = 0;h < RAM_P_PAGO;h++ ) {
        //    if( _PAGO_MODO( h ) == nro ) {
            if( _PAGO_MODO( h ) == nro && MODO_CHEQUE_COMO_UN_MEDIO ) {
                pos = h;
                // ok = 0;
            }
        }
    }
    /*-------------- valida que no repita tarjetas ---------------*/
    if( medios[nro].tarjeta ) {
		//Log para robot
		GRABAR_LOG_SISTEMA( "Medio Seleccionado Tarjeta" ,LOG_ROBOT,1);
        for( h = 0;h < RAM_P_PAGO;h++ ) {
            if( _PAGO_MODO( h ) == nro ) {
                ok = 0;
            }
        }
        if( !ok && VARIAS_TARJETAS_POR_PAGO == NO ) {
            MENSAJE_STRING( S_SOLO_UN_PAGO_TARJETA );
            PAUSA( 36 );
            BORRAR_MENSAJE();
            return ( 0 );
        }
        /*---------- Controla que pueda pasarse si no lo hizo ----------*/
        else if( PASAR_TARJETA_SOLO_AL_INICIO && VAR_TARJETAS_STATUS == 0 ) {
            MENSAJE_STRING( S_TARJETA_NO_PASADA );
            PAUSA( 36 );
            BORRAR_MENSAJE();
            return ( 0 );
        }
        if( VARIAS_TARJETAS_POR_PAGO ) {
            ok = 1;
        }
        if( MODO_NEGOCIO == SERVICIOS ) {
            PEDIR_MARCAS_TARJETA( nro + 1 );
        }
    }
    /*----------- prepara pantalla -----------*/
    tipo_vuelto = medios[nro].vuelto;
    datos_adicionales = medios[nro].cod_solicitud;
    if( !MODO_CHEQUE_COMO_UN_MEDIO ) {
        cuotas = CANT_CUOTAS_MEDIO( nro, ( unsigned * )&datos_adicionales, ( int* )&cant_veces,
                                    ( int* )&nro_cuota );
    }
    else {
        cuotas = 0;
    }
    numero = cuotas;
    ExecuteSAction( A_MUESTRA_TITULO_BROWSE, medios[nro].nombre );
    /*---------------- Si es ticket de compra ----------------*/


 	if( medios[nro].ticket_compra ) {
		//Log para robot
		GRABAR_LOG_SISTEMA( "Medio Seleccionado Ticket Compra" ,LOG_ROBOT,1);
        /*-------------- pide tipo de ticket ----------------*/
        cuenta = PIDE_TICKET_COMPRA( ( int* )&id_registro, ( long* )&autorizacion, ( long* )&numero,
                                     ( char* )&cod_medio_vuelto, ( char* )&cod_submedio_vuelto,
									 ( char* )&tipo_vuelto, ( int* )&datos_adicionales, medios[nro].nro, SI,
									 &cod_restriccion_art, SI );
        if( cuenta <= 0 ) {
            ok = 0;
        }else
			SET_MEMORY_INT( __cantidad_eventos_enviar_offline,cuenta);

		if( ok && modo == _MODO_COBRANZA ) {
			struct    _datos_cobro datosCobros;
			RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * ) &datosCobros,
				sizeof( struct _datos_cobro ), 0 );
			if( ( ( !dev_cobro && !datosCobros.hab_todos_medios ) ||
				( dev_cobro && !datosCobros.hab_todos_medios_dev ) )
				&& !PAGOS_MEDIO_HABILITADO_COBRO( medios[nro].nro, cuenta, dev_cobro  ) ) {
				MENSAJE("SUBMEDIO NO HABILITADO");
				ok = 0;
			}
		}
    }
    #ifdef COMPILAR_MUTUALES
    /*---------------- Si es mutual ----------------*/
    if( medios[nro].mutual && ok ) {
		//Log para robot
		GRABAR_LOG_SISTEMA( "Medio Seleccionado Mutual" ,LOG_ROBOT,1);
        /*-------------- pide mutual ----------------*/
        if( MUTUAL_CODIGO ) {
            cuenta = MUTUAL_CODIGO;
        }
        else {
            cuenta = PIDE_MUTUAL( &id_registro, &autorizacion, &numero, &cod_medio_vuelto,
                          &cod_submedio_vuelto, &tipo_vuelto, &datos_adicionales, medios[nro].nro,
						  RAM_RESTO, ( modo == _MODO_VENTA ) ? 1 : 0, &cod_restriccion_art, SI );
        }
        if( cuenta <= 0 ) {
            ok = 0;
        }else
			SET_MEMORY_INT( __cantidad_eventos_enviar_offline,cuenta);

		if( ok && modo == _MODO_COBRANZA ) {
			struct    _datos_cobro datosCobros;
			RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * ) &datosCobros,
				sizeof( struct _datos_cobro ), 0 );
			if( ( ( !dev_cobro && !datosCobros.hab_todos_medios ) ||
				( dev_cobro && !datosCobros.hab_todos_medios_dev ) )
				&& !PAGOS_MEDIO_HABILITADO_COBRO( medios[nro].nro, cuenta, dev_cobro  ) ) {
				MENSAJE("SUBMEDIO NO HABILITADO");
				ok = 0;
			}
		}
		if( ok ) {
            if( config.pais != CHILE && config.pais != BRASIL ) {
                if( !PIDE_CUIT( cuit, NO, ST( S__RUT_ ), 38 ) ) {
                    ok = 0;
                }
            }
			else {
				if( MODALIDAD_VALES_INSTITUCIONALES ) {
                    if( !RAM_NRO_CLIENTE && !RAM_CLIENTE_AFINIDAD ) {
                        if( !PEDIR_NRO_CLIENTE( 0, NO ) ) {
                            ok = 0;
                        }
                    }
                    if( ok ) {
                        if( RAM_CLIENTE_AFINIDAD_ADICIONAL ) {
                            if( cuenta != RAM_COD_MUTUAL_AFINIDAD_ADICIONAL ) {
                                MENSAJE_STRING( S_EL_CLIENTE_NO_PERTENECE_A_ESA_INSTITUCION );
                                ok = 0;
                            }
                        }
                        else {
                            if( cuenta != clientes.cod_mutual ) {
                                MENSAJE_STRING( S_EL_CLIENTE_NO_PERTENECE_A_ESA_INSTITUCION );
                                ok = 0;
                            }
                        }
                    }
                    MUESTRA_CLIENTE( 0 );
                }
                if( ok ) {
                    if( PEDIR_NUMERO_DE_VALE ) {
                        numero = PIDE_NRO_VALE_MUTUAL();
                        if( numero < 0 ) {
                            ok = 0;
                        }
                    }
                }
            }
        }
    }
    #endif
    /*------------- pide datos ----------------*/
    if( ok ) {
        if( pos == -1 ) {
            _SET_MEMORY_LONG( __pago_numero, RAM_P_PAGO, 0 );
        }
        importe_ingresado = PAGOS_PIDE_IMPORTE_CON_T_COMPRA( ST( S__IMPORTE_ ),
			( config.pais == ECUADOR ) ? 6 : ENTEROS, DECIMALES, NO,
			( char )( ( medios[nro].ticket_compra ) ? cuenta : 0 ), ( char )( ( medios[nro].cheque
            && MODO_CHEQUE_COMO_UN_MEDIO && pos != -1 ) ? pos : RAM_P_PAGO ),
			( int )tipo_vuelto, datos_adicionales, ( char )nro, ( char )cuenta,
			( int )cant_veces );
		//if( cuenta > 0 /*&& _medio_moneda_extranjera && medios[nro].oc_online*/ ){
 		if( cuenta > 0 && medios[nro].mutual && MUTUAL_MONEDA_EXTRANJERA( cuenta ) ){
            if( importe_ingresado != RAM_RESTO ){
               importe_me = ROUND( importe_ingresado, ENTEROS, DECIMALES );
               importe_ingresado =  MUTUALES_ME_A_ML_COT( importe_ingresado, cotizacion );
            } else {
               importe_me = MUTUALES_ML_A_ME_COT ( importe_ingresado,cotizacion);//2.0299999
            }
        }
    }

	if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && !RAM_NOTA_CR ) {
        if( cuenta > 0 && ( medios[nro].mutual || medios[nro].ticket_compra ) ) {
		    if( ( medios[nro].mutual && VALIDA_MUTUAL( cuenta ) )
			    || ( medios[nro].ticket_compra && VALIDA_T_COMPRA( cuenta ) ) ) {
			    double monto_2 = 0.0;
			    if( ( monto_2 = ROUND( MONTO_MEDIO_ACTIVADO_PROMO( cuenta, 0, medios[nro].nro ),
                        ENTEROS + 1, DECIMALES ) ) < importe_ingresado ){
				    importe_ingresado = monto_2;
			    }
			    if( fabs( importe_ingresado ) > 0 ) {
				    ACT_MONTO_MEDIO_ACTIVADO_PROMO( cuenta, 0, importe_ingresado, medios[nro].nro );
			    }
		    }
	    }
    }

    BORRAR_MENSAJE();
    if( MODO_CHEQUE_COMO_UN_MEDIO ) {
        numero = ( pos != -1 ) ? _PAGO_NUMERO( pos ) : _PAGO_NUMERO( RAM_P_PAGO );
    }else{
        numero = 0;
    }
    DestroyPanel( pPanel );
	/*------------- Controla restricciones de art�culos --------------*/
	if( cod_restriccion_art != 0 ) {
		double sum, signo;
		int h;

		sum = 0.0;
		// sumo art�culos que pueden ser pagados con este medio
   		for( h = 0; h < RAM_P_TICKET; h++ ) {
   			//if( ARTICULO_EN_GRUPO( cod_restriccion_art, "", _TICKET_COD_BARRA( h ),
			//	_TICKET_COD_CLASIFICACION( h ), _TICKET_COD_ESPECIAL( h ) ) ) {
			if( ARTICULO_EN_GRUPO( cod_restriccion_art, "0", _TICKET_COD_BARRA( h ),
				_GET_MEMORY_STR( __ticket_cod_clasificacion_str, h ), _TICKET_COD_ESPECIAL( h ) ) ) {
				signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;
           		if( MODO_DEVOLUCION && ( _TICKET_TIPO( h ) & _DEV_ENVASE ) )
           			signo *= -1;
				if( _TICKET_ORIGEN( h ) != _ORIGEN_DESC_PESOS  &&
					_TICKET_ORIGEN( h ) != _ORIGEN_DESC_PORC  &&
					_TICKET_ORIGEN( h ) !=_ORIGEN_PROMOCION ) {
					sum += ROUND( ( _TICKET_PRECIO( h ) + _TICKET_RECARGO( h ) )
						* fabs( _TICKET_CANTIDAD( h ) ) * signo, ENTEROS, DECIMALES );
				}
			}
   		}
   		// resto montos de pagos anteriores con el mismo c�digo de restricci�n
		for( h = 0; h < RAM_P_PAGO; h++ ) {
			if( _PAGO_COD_RESTRICCION_ART( h ) == cod_restriccion_art ) {
				sum -= _PAGO_IMPORTE( h );
			}
		}
		// validar importe
   		if( ( sum + 0.001 ) < importe_ingresado ) {
			MENSAJE_TECLA( ST( S_EL_IMP_EXCEDE_TOTAL_ART_REST_X_MEDIO ) );
   			ok = 0;
   		}
   	}
    if( importe_ingresado > 0.0 || (medios[nro].cheque && numero > 1 )
			|| (MODO_CHEQUE_COMO_UN_MEDIO && importe_ingresado != 0.0) ) {
        /*---------------------- Calcula importe -----------------------*/
        double resto_me_aux = 0.0;
        importe = importe_ingresado;
        variacion = variacion_submedio;
        if( medios[nro].tipo_variacion == SOBRE_IMPORTE ) {
            importe += CALCULA_VARIACION( importe_ingresado, nro,cuenta );
        }
        PAGOS_MUESTRA_IMPORTE( importe );
        if( !id_registro ) {
            id_registro = medios[nro].id_registro;
        }
        if( medios[nro].mutual && ok && cuenta > 0 && MUTUAL_MONEDA_EXTRANJERA( cuenta ) ) {
            if( importe_ingresado != RAM_RESTO ) {
                importe_me = importe_ingresado;
                importe_ingresado = MUTUALES_ME_A_ML( importe_ingresado, cuenta );
            } else {
              importe_me = MUTUALES_ML_A_ME( importe_ingresado, cuenta );
            }
            resto_me_aux = MUTUALES_ML_A_ME( RAM_RESTO, cuenta );
         } else {
            resto_me_aux = RAM_RESTO;
        }
        /*#ifdef COMPILAR_MUTUALES
         if( ok && medios[nro].mutual ) {
             if( config.pais == COLOMBIA ) {
                 ok = CONTROLA_IMPORTE_MUTUAL( ( char )cuenta, num_bono, importe );
                 strcpy( numero_tar, num_bono );
             }
             else if( CONTROLA_IMPORTE_MEDIOS ) {
                 ok = CONTROLA_IMPORTE_MUTUAL( ( char )cuenta, NULL, importe );
             }
         }
         #endif*/
        /*---------------- Si es ticket de compra ----------------*/
        if( medios[nro].ticket_compra ) {
            if( ok && CONTROLA_IMPORTE_MEDIOS ) {
                if( !CONTROLA_IMPORTE_T_COMPRA( nro, cuenta, ( float )importe ) ) {
                    ok = 0;
                }
            }
        }
        /*---------------- Verifica Importe por Medio ------------------*/
        if( ok && CONTROLA_IMPORTE_MEDIOS ) {
            if( !medios[nro].mutual && !medios[nro].ticket_compra && !medios[nro].tarjeta ) {
                if( importe > medios[nro].pago_imp_maximo ) {
                    ok = 0;
                    MENSAJE_STRING( S_EL_IMPORTE_EXCEDE_MAXIMO_PERMITIDO );
                }
                if( importe < medios[nro].pago_imp_minimo ) {
                    ok = 0;
                    MENSAJE_STRING( S_EL_IMPORTE_NO_LLEGA_MINIMO_EXIGIDO );
                }
            }
        }
		/*----------- Control Importe maximo para devoluciones ---------*/
		if( ok && dev_cobro && medios[nro].monto_maximo_devolucion != 0.00 ){
			double total_medio = importe_ingresado;
			total_medio += TOTAL_X_MEDIO_EN_TICKET( medios[nro].nro );
				if( total_medio > medios[nro].monto_maximo_devolucion ){
					ok = 0;
					MENSAJE("TOTAL DE IMPORTE EXCEDIDO EN DEVOLUCION");
				}

		}
        /*------------------------ Si es cheque ------------------------*/
        if( medios[nro].cheque && ok ) {
            // valida que el cheque no haya sido usado para otro pago
            //if (GRABA_CHEQUE_CLIENTE( GET_MEMORY_INT( _h ) )) {
            if( !MODO_CHEQUE_COMO_UN_MEDIO ) {
                if( !GRABA_CHEQUE_CLIENTE( h, h, NO ) ) {
                    ok = 0;
                }
            }
            if( medios[nro].al_dia ) {
                cheque_al_dia = 1;
            }
            else {
                cheque_al_dia = 0;
            }
            if( PEDIR_DATOS_COMPRADOR_EN_CHEQUES ) {
                ok = PIDE_DATOS_COMPRADOR_EN_CHEQUE( cuit_comprador, telefono_comprador,
                                                     cheque_al_dia, &fecha_vto,
                                                     ( ( PASAR_CHEQUE_POR_LECTOR == LECTOR_IBM ) ? 1
                                                     : 0 ) );
            }
            if( ok && PASAR_CHEQUE_POR_LECTOR == LECTOR_IBM ) {
                while( !salir ) {
                    MENSAJE_TECLA_STRING( S_INGRESE_CHEQUE_A_LEER_Y_PRESS_ENTER );
                    error = LEER_CHEQUE( cod_cuenta, nro_banco, &numero, &plaza );
                    sprintf( datosche, ST( S_CH_LEIDO___S__S__LI__D ), cod_cuenta, nro_banco,
                             numero, plaza );
                    GRABAR_LOG_SISTEMA( datosche ,LOG_VENTAS,4);
                    if( !error ) {
                        salir = 1;
                    }
                    else {
                        MENSAJE_STRING( S_ERROR_DE_LECTURA_ENTER_REINTENTAR_ESC_CANCELAR );
                        k = GETCH();
                        while( k != 13 && k != 27 ) {
                            k = GETCH();
                        }
                        if( GETCH() == 27 ) {
                            salir = 1;
                            MENSAJE_STRING( S_DEBE_INGRESAR_COMPROBANTE_EN_ESTACION_DOCUMEN );
                            k = GETCH();
                            while( k != 13 ) {
                                k = GETCH();
                            }
                        }
                    }
                }
                if( !error ) {
                    BUSCAR_CLIENTE_EN_BOLETIN( nro_banco, cod_cuenta, cuit, telefono, &habilitado,
                                               numero, 1, serie, plaza );
                    banco = atoi( nro_banco );
                    strcpy( cuenta_banco, cod_cuenta );
                    if( habilitado ) {
                        ok = PIDE_DATOS_CHEQUE_FALTANTES( NO, cheque_al_dia, &numero, nro_banco,
                                                          &fecha_vto, &cod_post_cheq,
                                                          &cod_hora_acre, cuit, telefono, NO );
                    }
                    else {
                        ok = 0;
                    }
				}
                else {
                    if( ok ) {
                        if( !fecha_vto ) {
                            ok = CARGAR_CHEQUE_MANUAL( cheque_al_dia, &numero, &banco, &fecha_vto,
                                                       &cod_post_cheq, &cod_hora_acre, cuit,
                                                       telefono, cuenta_banco, &plaza, nro_banco,
                                                       habilitado, serie, &cod_valida_cheque, SI );
                        }
                        else {
                            ok = CARGAR_CHEQUE_MANUAL( cheque_al_dia, &numero, &banco, &fecha_vto,
                                                       &cod_post_cheq, &cod_hora_acre, cuit,
                                                       telefono, cuenta_banco, &plaza, nro_banco,
                                                       habilitado, serie, &cod_valida_cheque, NO );
                        }
                    }
                }
            }
            else {
                if( ok && !MODO_CHEQUE_COMO_UN_MEDIO ) {
                    ok = CARGAR_CHEQUE_MANUAL( cheque_al_dia, &numero, &banco, &fecha_vto,
                                               &cod_post_cheq, &cod_hora_acre, cuit, telefono,
                                               cuenta_banco, &plaza, nro_banco, habilitado, serie,
                                               &cod_valida_cheque, SI );
                }
                /*if( MODO_CHEQUE_COMO_UN_MEDIO ) {
                    numero = ( pos != -1 ) ? _PAGO_NUMERO( pos ) : _PAGO_NUMERO( RAM_P_PAGO );
                }*/
            }
            sprintf( dche, ST( S_CH__S__S__LI__D__S ), cuenta_banco, nro_banco, numero, plaza,
                     cuit );
            GRABAR_LOG_SISTEMA( dche ,LOG_VENTAS,4);
            if( !ok ) {
                CEREAR_DATOS_CHEQUE( &numero, nro_banco, &fecha_vto, &cod_post_cheq, &cod_hora_acre,
                                     cuit, telefono );
            }
            if( UTILIZA_AUTORIZADOR_CHEQUES && ok ) {
                ok = VALIDAR_CHEQUE( nro, config.sucursal, NRO_CAJA, NRO_CAJERO, NRO_TICKET + 1,
                                     cuit, cuit_comprador, banco, cuenta_banco, numero, plaza, 0,
                                     importe, fecha_vto, habilitado );
            }
            if( PEDIR_DATOS_COMPRADOR_EN_CHEQUES ) {
                strcpy( cuit, cuit_comprador );
            } //para endoso cheque rut_comprador
            /*-------------- pide autorizacion ----------------*/
            if( ( medios[nro].autorizacion && ok ) || ( medios[nro].autorizacion == 2 ) ) {
                int aut = 1;
                if( medios[nro].autorizacion == 2 && medios[nro].cheque ) {
                    if( !SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI,
                                           ST( S_AUTORIZAR_PAGO ), ST( S_AUTORIZAR_PAGO ),
                                           RAM_COD_FUNCION, SI ) ) {
                        aut = 0;
                    }
                }
                autorizacion = aut ? PAGOS_PIDE_AUTORIZACION( ST( S_AUTORIZACION ) ) : 0;
                if( autorizacion <= 0 ) {
                    ok = 0;
                }
            }
            if( !ok ) {
                // Para que limpie la pantalla si no esta OK.
                CEREAR_DATOS_CHEQUE( &numero, nro_banco, &fecha_vto, &cod_post_cheq, &cod_hora_acre,
                                     cuit, telefono );
                MOSTRAR_CLIENTE_EN_PANTALLA();
            }
            else {
                id_registro = nro_cuota;
            }
        }
        /*---------------- Si es tarjeta de credito ----------------*/
        if( medios[nro].tarjeta && ok ) {
			int numero_cuotas_nc_ref = 0;
			SET_MEMORY_DOUBLE( __ram_importe, importe );
            if( VARIAS_TARJETAS_POR_PAGO ) {
                CEREAR_STRUCT( _var_tarjetas );
            }
            /*---- si no ha sido pasada y puede hacerlo al final, la pide ----*/
            if( VAR_TARJETAS_STATUS == 0 ) {
                if( !VALIDA_TARJETA( NO, NO, VENTAS, NO, NULL, SI, nro ) ) {
                    CEREAR_STRUCT( _var_tarjetas );
                    ok = 0;
                }
            }


			if( ok && ( RAM_NOTA_CR || MODO_DEVOLUCION ) && RAM_REFERENCIA_NC && LIMITAR_NOTAS_DE_CREDITO_CON_TAR ) {
                int msg_error = 1;
                struct _eventos eventos;
                struct _ev_cont ev_cont;
                struct _ev_medios ev_pago;
                struct _ev_despro ev_despro;
                struct _ev_dmedios ev_dpago;
                struct _ev_cod_barra ev_cod_barra;
                struct _ev_bpromo ev_b_promo;

                ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                    &ev_cod_barra, &ev_b_promo );
				
				SELECT_TABLE( T_EVM_TMP, TT_ORIG );
				SET_WHERE("");
				RUN_QUERY(NO);
                while( /*!BtrvEOF( )*/ !dbEOF() ) {
					if( medios[  ev_pago.modo_pago - 1 ].tarjeta &&  ES_LA_MISMA_TARJETA(ev_pago.nro_tarjeta,GET_NUMERO_TARJETA_PRIVADO( ))){
                        int h;
                        double importe_en_tarj = 0;

                        msg_error = 0;
                        //La tarjeta fue usada en el comp original, se verifica
                        //que no se intente pagar por mayor importe al original

                        for( h = 0; h < RAM_P_PAGO; h++ ) {
							if( medios[_PAGO_MODO( h )].tarjeta && ES_LA_MISMA_TARJETA(ev_pago.nro_tarjeta,_PAGO_NRO_TARJETA( h ))){
                                importe_en_tarj += _PAGO_IMPORTE( h ); //no hay vuelto
                            }
                        }
                        if( ( ev_pago.importe - importe_en_tarj - RAM_IMPORTE ) < -0.001 ) {
                            msg_error = 2;
						}
                    
						//La tarjeta que entra esta en la compra original, cargo los datos para no pedirlos
						if( !msg_error 
							&& ( RAM_MODO_DEVOLUCION == _DEVOL_TOTAL 
								|| RAM_MODO_DEVOLUCION == _DEVOL_INTERME 
								|| RAM_MODO_DEVOLUCION == _NC_TOTAL 
								|| RAM_MODO_DEVOLUCION == _NC_RF_INTERME ) ) {
							SET_MEMORY_CHAR( __var_tarjetas_status, 2 );
							NUMERO_DE_COMERCIO_TARJETA(  );
							SET_MEMORY( __var_tarjetas_nro_comercio, ev_pago.nro_comercio );
							SET_MEMORY( __var_tarjetas_tabla, ev_pago.tabla );
							SET_MEMORY_CHAR( __var_tarjetas_cuotas, ev_pago.cuotas );
							SET_MEMORY_LONG( __var_tarjetas_cupon_original, ev_pago.nro_cupon_int1);
							SET_MEMORY_INT( __var_tarjetas_fecha_original, ev_pago.fecha_ticket);
							SET_MEMORY( __var_tarjetas_aut_original, ev_pago.auto_tarjeta );
							SET_MEMORY_INT( __var_tarjetas_caja_original, ev_pago.caja );
							//Averiguramos si la cuota es ONLINE, esto se esta seteando de tar_med.cuotas_online
							//Tarea: 4588
							area_ant = SELECTED2();
							tipo_ant = SELECTED_TIPO();
							if( OPENED( T_TAR_MED, TT_ORIG )) {
								tarmed_abierta = SI;
							}
							if( tarmed_abierta || (!OPEN_TABLE( T_TAR_MED, TT_ORIG, ( char* )&tar_med, sizeof( tar_med ) ) ) ) {
								SELECT_TABLE( T_TAR_MED, TT_ORIG );
								SET_WHERE(" COD_MEDIO = '%i' AND COD_TARJETA = '%i'", ev_pago.modo_pago, ev_pago.tarjeta );
								RUN_QUERY(NO);
								if( FOUND2( ) && (tar_med.cuotas_online == 's'|| tar_med.cuotas_online == 'S')){
									SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 
										( tar_med.cuotas_online == 's'|| tar_med.cuotas_online == 'S' || VAR_TARJETAS_UNICAMENTE_OL )? 1: 0 );
								}
								CLOSE_TABLE( T_TAR_MED, TT_ORIG );
							}
							SELECT_TABLE( area_ant, tipo_ant );
						}
						numero_cuotas_nc_ref = ev_pago.cuotas;
					}
					SKIP2( 1 );
                }
				if( msg_error ) {
                    if( msg_error == 1 ) {
                        MENSAJE( "LA TARJETA NO SE UTILIZO EN EL PAGO ORIGINAL" );
                        CEREAR_STRUCT( _var_tarjetas );
                    }
                    if( msg_error == 2 ) {
                        MENSAJE( "NO SE PUEDE SUPERAR EL IMPORTE DEL PAGO ORIGINAL" );
                    }
                    PAUSA( 36 );
                    BORRAR_MENSAJE();
                    ok = 0;
				}else{
				
				}
                CERRAR_EVENTOS_TEMPORALES();
			}

            /*------------ Si fue pasada pero no pidio los datos -----------*/
			if( ok && VAR_TARJETAS_STATUS == 1 ) {
                if( !PIDE_DATOS_TARJETA( NO, medios[nro].nro ) ) {
                    CEREAR_STRUCT( _var_tarjetas );
                    ok = 0;
                }else{
                    /*Si no se aplica variacion en el medio*/
                    if( CALCULA_VARIACION( RAM_IMPORTE, nro ,cuenta) == 0.0 ) {
                        if(RAM_IMPORTE != importe){
                            if( RAM_IMPORTE < medios[nro].pago_imp_minimo ) {
                                ok = 0;
                                MENSAJE_STRING( S_EL_IMPORTE_NO_LLEGA_MINIMO_EXIGIDO );
                            }else{
                                importe = RAM_IMPORTE; // Por si se cambio el importe
                                importe_ingresado = importe;
                            }
                        }
                    }else{
                        MENSAJE("EL MEDIO TIENE CONFIGURADA UNA VARIACION, NO SE PUEDE RECALCULAR");
                        ok = 0;
					}
					{
						char aux[200];
						sprintf(aux," %i-%i-%i-%i-%i-%i-",ok,RAM_REFERENCIA_NC,RAM_MODO_DEVOLUCION ,_NC_RF_PARCIAL,numero_cuotas_nc_ref ,VAR_TARJETAS_CUOTAS );
						glog(aux,1,1);
					}
					if( ok && RAM_REFERENCIA_NC &&( RAM_MODO_DEVOLUCION ==_NC_RF_PARCIAL|| RAM_MODO_DEVOLUCION ==_DEVOL_PARCIAL ) &&  numero_cuotas_nc_ref < VAR_TARJETAS_CUOTAS ) {
						 MENSAJE("EXCEDE EL NUMERO DE CUOTAS DEL COMPROBANTE ORIGINAL");
                        ok = 0;

					}
                }
            }

            /*-------- si fue pasada verifica que el medio la permita ------*/
            if( ok && VERIFICAR_TARJETAS_PERMITIDAS ) {
                if( !VERIFICA_MARCA_EN_MEDIO( ( char )nro, VAR_TARJETAS_TARJETA ) ) {
                    MENSAJE_STRING( S_TARJETA_NO_ACEPTADA_POR_MEDIO );
                    ok = 0;
                }
            }

			if( ok && modo == _MODO_COBRANZA ) {
				struct    _datos_cobro datosCobros;

				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * ) &datosCobros,
					sizeof( struct _datos_cobro ), 0 );
				if( ( ( !dev_cobro && !datosCobros.hab_todos_medios ) ||
				( dev_cobro && !datosCobros.hab_todos_medios_dev ) )  &&
						!PAGOS_MEDIO_HABILITADO_COBRO( medios[nro].nro, VAR_TARJETAS_TARJETA + 1, dev_cobro  ) ) {
					MENSAJE("SUBMEDIO NO HABILITADO");
					CEREAR_STRUCT( _var_tarjetas );
					ok = 0;
				}
			}
			if( ok ) {
				if(  !dev_cobro && !RAM_MODO_DEVOLUCION && TARJ[VAR_TARJETAS_TARJETA].permite_consulta_saldo == 'S' ) {
					if( !CONSULTA_SALDO_TARJETAS( importe ) ){
						ELIMINA_PAGO_PROMO( medios[nro].nro, TARJ[VAR_TARJETAS_TARJETA].cod_tarjeta );
						CEREAR_STRUCT( _var_tarjetas );
                        //eliminar monto usado por promocion
						ok = 0;
					}
                }
            }
            /*--------------------- toma los datos ------------------------*/
            if( ok ) {
                if( CONTROLAR_TOTAL_EN_CUOTAS && VAR_TARJETAS_CUOTAS > 1
					&& fabs(importe - (RAM_RETENCION_DGR + RAM_PERCEPCION_DGI)) >fabs( TOT_TICK_TOTAL_EN_CUOTAS) + .02 && VAR_TARJETAS_CONTROL_CUOTAS ) {
                    sprintf( aux, ST( S_SOLO_PUEDE_PAGAR____8_2F_EN_CUOTAS ),
                             TOT_TICK_TOTAL_EN_CUOTAS );
                    MENSAJE_CON_TECLA( aux, 27 );
                    ok = 0;
					

                    if( config.momento_recargo != AL_RENGLON ) {
                        SET_MEMORY_CHAR( __var_tarjetas_status, 1 );
                    }
                }
                if( ok ) {
					if( ok && TARJ[VAR_TARJETAS_TARJETA].cod_solicitud > 0 ) {
						PAGOS_PEDIR_DATOS_ADICIONALES( RAM_P_PAGO , 0,
								TARJ[VAR_TARJETAS_TARJETA].cod_solicitud , &importe, D_MEDIOS, NULL ,0, NULL, NULL , (char) nro + 1, '\0' ); 
					}
					
					if( ok && config_tps.ActivarCashPlus == 1 && (config_tps.MinimoaHabilitar == 0 || (config_tps.MinimoaHabilitar >0 && config_tps.MinimoaHabilitar <= RAM_IMPORTE_TOTAL )) 
						&&TARJ[VAR_TARJETAS_TARJETA].tipo_tarjeta & _TIPO_CASH_ADVANCE)
					{ //avisa que esta tarjeta tiene posibilidade de cargar el retiro de dinero
						char *msg[4];
						msg[0] = (char*)malloc(50);
						msg[1] = (char*)malloc(50);
						msg[2] = (char*)malloc(50);
						msg[3] = (char*)malloc(50);
						sprintf(msg[0],"PUEDE RETIRAR DINERO");
						sprintf(msg[1],"CON ESTA TARJETA");
						sprintf(msg[2],"                     ");
						sprintf(msg[3],"ACTIVANDO POR TECLA!!");
						MOSTRAR_MENSAJE_POP_UP("RETIRO DE DINERO",msg,4);
						free( msg[0] );
						msg[0] = NULL;
						free( msg[1] );
						msg[1] = NULL;
						free( msg[2] );
						msg[2] = NULL;
						free( msg[3] );
						msg[3] = NULL;
						

					}

                    if( config.momento_recargo == AL_FINAL ) {
                        // TOLEDO: (OK) Cambio de moneda y % de recargo si
                        // hay mezcla de cuotables y no cuotables.
                        if( !VAR_TARJETAS_CONTROL_CUOTAS && CONTROLAR_TOTAL_EN_CUOTAS
                         && RAM_TOTAL > TOT_TICK_TOTAL_EN_CUOTAS ) {
                            SET_MEMORY_CHAR( __var_tarjetas_moneda, PESOS );
                            SET_MEMORY_FLOAT( __var_tarjetas_recargo, 0 );
                            NUMERO_DE_COMERCIO_TARJETA();
                        }
                        importe += CALCULA_VARIACION_TARJETA( importe, nro, SI );
                        PAGOS_MUESTRA_IMPORTE( importe );
                        if( !EXIGE_AUTORIZACION_ON_LINE() && (importe > VAR_TARJETAS_LIMITE && VAR_TARJETAS_LIMITE > 0.01) ) {
                            MENSAJE_STRING( S_IMPORTE_EXCEDE_LIMITE_COMPRA );
							ok = 0;
                        }
                    }
                    tarjeta = VAR_TARJETAS_TARJETA;
                    //id_registro = TARJ[tarjeta].id_registro;
					cuenta = TARJ[tarjeta].cod_tarjeta;
					tipo_vuelto = TARJ[tarjeta].vuelto;

					strcpy( numero_tar, GET_NUMERO_TARJETA_PRIVADO( ) );
                    fecha_vto = GET_FECHA_VTO_PRIVADA( );
                    SET_MEMORY( __var_tarjetas_autorizacion, "          " );
                    if(config.pais != MEXICO){ // Forray se saca la autorizacion para mexico revisar esto despues del piloto
                        if( !EXIGE_AUTORIZACION_ON_LINE() ) {
                            if( EXIGE_AUTORIZACION( importe ) ) {
                                if( !PIDE_AUTORIZACION_TARJETA( importe ) ) {
                                    ok = 0;
                                }
                            }
                        }
                    }
                }
            }
            if( ok ) {
                if( VAR_TARJETAS_PORC_ENTREGA ) {
                    importe_aux = importe;
                    importe = ROUND( importe * ( 100 - VAR_TARJETAS_PORC_ENTREGA ) / 100, 10, 2 );
                    SET_MEMORY_DOUBLE( __var_tarjetas_importe_entrega, importe_aux - importe );
                    importe_ingresado = importe;
                }
                if( VAR_TARJETAS_PIDE_ENTREGA ) {
                    SET_MEMORY_DOUBLE( __var_tarjetas_importe_entrega, PIDE_ENTREGA( importe ) );
                    importe_ingresado = importe - VAR_TARJETAS_IMPORTE_ENTREGA;
                }
            }
            /*--------------- Pide clave en pinpad -----------------*/
            #ifdef COMPILAR_PINPAD
            if( ok ) {
                ok = PIDE_CLAVE_EN_PINPAD( SI, SI, importe_ingresado );
				if( ok == 0 ) {
					SET_TARJERTAS_ON( 1); //lo pongo en 1 asi cuando pase por mostrar lo cacnela
					SET_MEMORY_CHAR( __var_tarjetas_status, 0 );
				} else
					SET_TARJERTAS_ON( 0); 
            }
            #endif
        }
        /*---------------- Si es cuenta cte ----------------*/
         if( medios[nro].cta_cte && ok ) {
            char cuenta_ok = 0;
			//Log para robot
			GRABAR_LOG_SISTEMA( "Medio Seleccionado Cta. Cte.",LOG_ROBOT,1 );
            if( UTILIZA_SOCIOS ) {
                if( RAM_NRO_CLIENTE && ( NRO_COD_CUOTA || COD_FINANCIACION_POR_DEFECTO ) ) {
                    cuenta = (NRO_COD_CUOTA)?NRO_COD_CUOTA:COD_FINANCIACION_POR_DEFECTO ;
                    numero = RAM_NRO_CLIENTE;
                    if( !COOP_VERIFICA_CLIENTE_HABILITADO() ) {
                        ok = 0;
                    }
                    if( ok && !VERIFICA_CUPO_CLIENTE( importe, 0.0, NO, SI ) ) {
                        ok = 0;
                    }
                }
                else {
                    //MENSAJE_STRING( S_NO_INGRESO_CODIGO_FINANCIACION );
                    MENSAJE_STRING( S_DEBE_SELECCIONAR_EL_CLIENTE_AL_COMIENZO );
                    cuenta = -1;
                }
            }
            else {
                if( VALIDAR_CLIENTE_CTA_CTE ) {
                    //No se permite pago credito si no se esta realizando una factura
                    if( RAM_FACTURA ) {
                    if( RAM_NRO_CLIENTE ) {
                        if( EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
                            if( clientes.ctacte == 'N' ) {
                                MENSAJE_STRING( S_CLIENTE_NO_HABILITADO_PARA_CUENTA_CORRIENTE );
                            }
                            else {
                                cuenta_ok = 1;
                            }
                        }
                        else {
                            MENSAJE_STRING( S_EL_CLIENTE_NO_EXISTE );
                        }
                    }
                    else {
                        MENSAJE_STRING( S_CLIENTE_NO_ESTA_INGRESADO );
                        while( CONTROLAR_CLIENTE_EN_CTA_CTE && !rta ) {
                            rta = PEDIR_NRO_CLIENTE(  0, SI );
                        }
                        }
                    } else {
                        MENSAJE_STRING( S_TIPO_COMPROB_INVALIDO_PARA_OPERACION );
                        cuenta_ok = 0;
                    }
                }
                else {
                    if( config.pais != ELSALVADOR ) {
                    #if defined(INVEL_W) || defined(INVEL_L)
                    ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__N__CUENTA_ ), "", NULL, " " );
                    #endif
                    cuenta = ( int )PIDE_CUENTA( 4 );
                }
            }
            }
            if( PEDIR_COMPROBANTE_CTA_CTE ) {
                #if defined(INVEL_W) || defined(INVEL_L)
                ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__COMPROBANTE_ ), "", NULL, " " );
                #endif
                comprobante = PIDE_COMPROBANTE();
            }
            //if( config.pais != ELSALVADOR ) {
            if( cuenta <= 0 ) {
                if( !cuenta_ok ) {
                    ok = 0;
                }
                }
            //}
            else if( T_COMPRA_ENTRA_COMO_CTACTE
                  && ( cuenta == 25 || cuenta == 34 || cuenta == 180 || cuenta == 200
                    || cuenta == 190 || cuenta == 185 ) ) {
                /*------------------ cupones --------------------*/
                cantidad_cupones = PIDE_CANTIDAD_CUPONES();
                if( cantidad_cupones <= 0 ) {
                    ok = 0;
                }
                else {
                    valor_cupones = importe / cantidad_cupones;
                }
            }
            else {
                /*-------------- pide autorizacion ----------------*/
                if( medios[nro].autorizacion && ok ) {
                    autorizacion = PAGOS_PIDE_AUTORIZACION( ST( S_AUTORIZACION ) );
                    if( autorizacion <= 0 ) {
                        ok = 0;
                    }
                }
            }
        }
        /*---------------- Si es Moneda Extranjera ----------------*/
        if( medios[nro].moneda_extranjera == 1 && ok && !MUTUAL_MONEDA_EXTRANJERA( cuenta ) ) {
			//Log para robot
            GRABAR_LOG_SISTEMA( "Medio Seleccionado Moneda Extranjera" ,LOG_ROBOT,1);
			//Murgui decidio que esto es error.
            if( importe_ingresado != RAM_RESTO || MODO_NEGOCIO == SERVICIOS ) {
				importe_me = importe_ingresado;
				importe_ingresado = PAGOS_ME_A_ML( importe_ingresado, nro );
            }
            else {
                importe_me = PAGOS_ML_A_ME( importe_ingresado, nro );
            }
        }
        /*-------------- pide autorizacion ----------------*/
        if( ( medios[nro].autorizacion == 1 || medios[nro].autorizacion == '1' ) && ok
         && !medios[nro].cta_cte ) {
            autorizacion = PAGOS_PIDE_AUTORIZACION( ST( S_AUTORIZACION ) );
            if( autorizacion <= 0 ) {
                ok = 0;
            }
        }
        if( medios[nro].pide_supervisor == 1 || medios[nro].pide_supervisor == '1'
         || medios[nro].pide_supervisor == 'S' ) {
            if( !SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, "AUTORIZACION", NULL, 0,
                                   SI ) ) {
                ok = 0;
			}
		}
/*----------------------------------------------------------------*/
		if( ok && UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && !RAM_NOTA_CR && PROCESAR_PROMOCION ( nro, cuenta ) ) {
			int auxi = (int) VAR_TARJETAS_CUOTAS;
			/*PARA LAS PROMOS POSTPAGO SE SETEAN MEDIO SUBMEDIO Y PLAN*/
			SET_MEMORY_INT( __puerto_servidor_dbrouter, auxi );
			SET_MEMORY_INT( __ram_subindice_pago_cmr, nro + 1 );
			SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );
			SET_MEMORY_INT( __cantidad_eventos_enviar_offline,((int) VAR_TARJETAS_TARJETA) + 1 );
		
			// se guarda el estado actual de la caja y de las
			// variables de las promos para permitir revertir
			// los efectos de la evaluacion postpago.
			_SET_MEMORY_CHAR( __ram_promociones_afinidad_usadas,0,0  );//variable reutilizada descuentos postpago
			state_pago = actual_info( ss );
			
			#ifdef PROMO_FAULT_TOLERANCE
				logPanelPosPago();
			#endif //PROMO_FAULT_TOLERANCE

			if( eval( POSTPAGO, pr, &ss ) != NORM ) {
				glog( "PROMO: T_PAGO.C: eval(POSTPAGO): ERROR in PROMOS",LOG_PAGOS,4 );
			}
			SET_MEMORY_INT( __ram_subindice_pago_cmr, 0 );
			if( IMPORTE_MEDIO_PROMOCIONADO( nro, cuenta, &importe_ingresado ) == -1 ) {
				ok = 0;
			}

			SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
		} else {
			if( UTILIZA_PROMOCIONES && !RAM_NOTA_CR ) {
				//CALCULAR_PROMOCIONES( _POS_PAGO );
            }
        }
		if( config_tps.AgruparMediosEnImpresion ) {
			int ram_p_pago = 0;
			double resto = 0;
			if( ok && pos == -1 ) {
				ram_p_pago = RAM_P_PAGO + 1;//Agregamos uno porque incrementa el medio en PAGOS_AGREGAR_PAGO
				if( !config_tps.CantMediosPermitidos )  {
					config_tps.CantMediosPermitidos = CANTIDAD_MAXIMA_PAGOS;//Maxima cantidad de medios permitidos
				}
				if( ram_p_pago >= config_tps.CantMediosPermitidos ) {//limite de medios
					resto = RAM_RESTO - importe_ingresado;
					if( resto > 0 || ram_p_pago > config_tps.CantMediosPermitidos ) {//Faltante
						ram_p_pago = MEDIOS_AGRUPADOS( SI, nro, cuenta );
						if( ram_p_pago > config_tps.CantMediosPermitidos 
							|| ( ram_p_pago == config_tps.CantMediosPermitidos && resto > 0 ) ) {//la agrupacion nos da 5 medios pero aun existe resto
							if( config_tps.InformarMediosExcedidos ) {
								char mensaje[100];

								memset(mensaje,0,sizeof(mensaje));
								_snprintf(mensaje, sizeof(mensaje) - 1,"Solo puede abonar hasta con %i medios de pago ( Presione una tecla )", config_tps.CantMediosPermitidos );
								MENSAJE_TECLA( mensaje );
							}							//No Puede Ingresar mas de 5 medios de Pago ( Presione una tecla )" );
							//Solo puede abonar hasta con 5 medios de pago.
							ok = 0;
						}

					}
				}
			}
		}
        /*------------------ si todo OK agrega el pago --------------------*/
        if( ok ) {
            PAGOS_AGREGAR_PAGO( pos, ( char )nro, importe_ingresado, autorizacion, cuenta, tarjeta,
                                numero, numero_tar, banco, fecha_vto, cantidad_cupones,
                                valor_cupones, cod_post_cheq, cod_hora_acre, comprobante, cuit,
                                telefono, cuenta_banco, id_registro, cod_medio_vuelto,
                                cod_submedio_vuelto, tipo_vuelto, importe_me, cod_valida_cheque,
                                datos_adicionales, telefono_comprador, cod_restriccion_art );
            if (pos == -1) {
				pos = RAM_P_PAGO;
			}

			if( MODO_DEVOLUCION || ( RAM_NOTA_CR && RAM_REFERENCIA_NC ) ) {
                //Comienza en 1 los modos de pago
                //Acumulo los importes de los distintos medios ingresados
                ACUMULAR_IMPORTE_DEVOLUCION( ( int )( _PAGO_MODO( RAM_P_PAGO - 1 ) + 1 ),
                        _PAGO_IMPORTE( RAM_P_PAGO - 1 ) );
            }
		}
        if( ok && cliente_factura && RAM_FACTURA && EMITIR_FACTURA_REMOTA() ) {
            SET_MEMORY_LONG( __ram_nro_cliente, cliente_factura );
            EXISTE_CLIENTE( cliente_factura );
        }
    }
    // redibuja el faltante
    if( config.redondeo != 0 && ( RAM_RESTO < 0 ) 
     && _PAGO_TIPO_VUELTO( RAM_P_PAGO - 1 ) != _VUELTO_OTRO_MEDIO ) {

        resto = RAM_RESTO; 
        valor_redondeado = REDONDEAR( RAM_RESTO, config.redondeo,
                                      medios[_medio_efectivo].minima_denominacion );

		dif = FABS( resto - valor_redondeado);// FABS( valor_redondeado ) ;
		if( RAM_RESTO > -0.02 && RAM_RESTO < -0.00 && medios[_PAGO_MODO(RAM_P_PAGO - 1)].vuelto == 0 ) 
				SET_MEMORY_DOUBLE( __ram_resto, 0.00 );
		else
				SET_MEMORY_DOUBLE( __ram_resto, valor_redondeado );
        
		SET_MEMORY_DOUBLE( __ram_diferencia_redondeo, dif );
    }
    ExecuteAction( A_DESELECCIONA_MEDIO );
    #if defined(INVEL_W) || defined(INVEL_L)
    ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, "", "", NULL, " " );
    MOSTRAR_INPUT_CADENA( " ", 0 );
    BORRAR_MENSAJE();
    #endif
    PAGOS_MOSTRAR_RESTO( RAM_RESTO );
    PAGOS_MUESTRA_TOTAL( RAM_RESTO );
    CR1_MUESTRA_TARJETA();
    Orden = NUMERO_PANEL_MINIMO_EN_PAGOS;
    PreparePanel( &pPanel, Orden );
    ShowPanel( pPanel );
    CLOSE_RECUPERO( _ptr, _handle );
	return 1;
}
/*****************************************************************************/
void PAGOS_AGREGAR_PAGO( int pos, char modo, double importe, long autorizacion, unsigned cuenta,
                         char tarjeta, long numero, char *numero_tar, unsigned banco,
                         unsigned fecha_vto, unsigned cantidad_cupones, double valor_cupones,
                         int c_postal_c, int c_hora_c, unsigned comprobante, char *cuit,
                         char *telefono, char *cuenta_banco, int id_registro, char cod_medio_vuelto,
                         char cod_submedio_vuelto, char tipo_vuelto, double importe_me,
                         int cod_valida_cheque, int datos_adicionales, char *telefono_comprador,
						 int cod_restriccion_art )
/*****************************************************************************/
{
    char cadena[21], add_p_pago = 0;
    double variacion, vuelto = 0, aux_double, vuelto_medio = 0, pago_dis_iva;
    //struct sub_medios sub_medios;
    int i = 0;
    int cod_submedio_aux,cod_medio_aux;
    if( pos == -1 ) {
        pos = RAM_P_PAGO;
        add_p_pago = 1;
    }
	else {
        /*-------------- anula el recargo del viejo pago ---------------------*/
        ADD_MEMORY_DOUBLE( __ram_variacion, -( _PAGO_VARIACION( pos ) ) );

        ADD_MEMORY_DOUBLE( __ram_resto, _PAGO_IMPORTE( pos ) - _PAGO_VARIACION( pos ) );

      if( medios[modo].efectivo && IMPORTE_CONTADO_ADITIVO_EN_PAGOS ) {
            importe += _PAGO_IMPORTE( pos );
        }
        if( medios[modo].cheque && MODO_CHEQUE_COMO_UN_MEDIO ) {
            importe += _PAGO_IMPORTE( pos );
        }
    }
    if( pos < CANTIDAD_MAXIMA_PAGOS ) {
        variacion = CALCULA_VARIACION( importe, modo,cuenta );
        if( medios[modo].tarjeta && config.momento_recargo != AL_RENGLON ) {
			double importe_aux = importe;
			
			//parche para retenciones 
			/*importe = importe - ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI + RAM_PERCEPCION_TACIS + RAM_PERCEPCION_212_IVA1
                        + RAM_PERCEPCION_212_IVA2);*/
			variacion += CALCULA_VARIACION_TARJETA( importe , modo, SI ); 
			/*if( fabs(variacion> 0.01))
				variacion-= RAM_RETENCION_DGR + RAM_PERCEPCION_DGI + RAM_PERCEPCION_TACIS + RAM_PERCEPCION_212_IVA1
					        + RAM_PERCEPCION_212_IVA2;*/
			 //importe = importe_aux;
        }
        _SET_MEMORY_CHAR( __pago_modo, pos, modo );
        _SET_MEMORY_DOUBLE( __pago_importe, pos, importe );
        if( medios[modo].tipo_variacion == SOBRE_IMPORTE ) {
            _ADD_MEMORY_DOUBLE( __pago_importe, pos, variacion );
        }
        _SET_MEMORY_CHAR( __pago_tarjeta, pos, tarjeta );
        _SET_MEMORY_INT( __pago_cuenta, pos, cuenta );
        _SET_MEMORY_LONG( __pago_buffer1_long, pos, autorizacion );
        _SET_MEMORY_INT( __pago_banco, pos, ( banco ) ? banco : comprobante );
        _SET_MEMORY_LONG( __pago_numero, pos, numero );
        _SET_MEMORY_INT( __pago_fecha_vto, pos, fecha_vto );
        _SET_MEMORY_INT( __pago_cantidad_cupones, pos, cantidad_cupones );
        _SET_MEMORY_DOUBLE( __pago_valor_cupones, pos, valor_cupones );
        _SET_MEMORY_INT( __pago_cod_post_cheq, pos, c_postal_c );
        _SET_MEMORY_INT( __pago_cod_hora_acre, pos, c_hora_c );
		/*if( fabs( variacion) > 0.01 && IMPRESORA_FISCAL == FISCAL  && RAM_TICKET_FACTURA == 1) {
			variacion = CALCULA_VARIACION( importe, modo );
			variacion += CALCULA_VARIACION_TARJETA( importe , modo, NO );
		}*/
        _SET_MEMORY_DOUBLE( __pago_variacion, pos, variacion );
		//_SET_MEMORY_FLOAT( __pago_porc_variacion, pos, porc_variacion );
        _SET_MEMORY( __pago_nro_tarjeta, pos, numero_tar );
        _SET_MEMORY( __pago_cuit, pos, cuit );
        _SET_MEMORY( __pago_telefono, pos, telefono );
        _SET_MEMORY( __pago_telefono_otro, pos, telefono_comprador );
        _SET_MEMORY_CHAR( __pago_id_registro, pos, id_registro );
        _SET_MEMORY_INT( __pago_cod_submedio, pos, cuenta );
		
		
		if(cod_medio_vuelto >= 0 )
			_SET_MEMORY_CHAR( __pago_cod_medio_vuelto, pos, cod_medio_vuelto );
		else
			_SET_MEMORY_CHAR( __pago_cod_medio_vuelto, pos, 0);		


		if(cod_submedio_vuelto >= 0 )
        _SET_MEMORY_CHAR( __pago_cod_submedio_vuelto, pos, cod_submedio_vuelto );
		else
		_SET_MEMORY_CHAR( __pago_cod_submedio_vuelto, pos, 0 );

        _SET_MEMORY_CHAR( __pago_tipo_vuelto, pos, tipo_vuelto );
        _SET_MEMORY_DOUBLE( __pago_importe_me, pos, importe_me );
        _SET_MEMORY_INT( __pago_cod_solicitud, pos, datos_adicionales );
        _SET_MEMORY_CHAR( __pago_cuotas, pos, VAR_TARJETAS_CUOTAS );
{
		int auxi= 0;
		auxi = (int) VAR_TARJETAS_CUOTAS;
		SET_MEMORY_INT( __puerto_servidor_dbrouter, auxi );
		}
		_SET_MEMORY_INT( __pago_cod_restriccion_art, pos, cod_restriccion_art );
        if( medios[modo].tarjeta ) {
            if( ( PROTOCOLO_AUTORIZACION_TARJETA != _TCP_IP && EXIGE_AUTORIZACION_ON_LINE() )
             || !EXIGE_AUTORIZACION_ON_LINE() ) {
                SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
                                 CALCULAR_NRO_CUPON( VAR_TARJETAS_TARJETA ) );
            }
            if( !( ( config.pais == CHILE || config.pais == BRASIL ) && RAM_NOTA_CR
                && VAR_TARJETAS_IMPORTE_CUOTA > 0.005 ) ) {
                SET_MEMORY_FLOAT( __var_tarjetas_importe_cuota,
                                  ( long )( importe * VAR_TARJETAS_COEFICIENTE ) );
            }
            SET_MEMORY_DOUBLE( __var_tarjetas_vuelto,
                               ( ( RAM_RESTO - importe ) < 0.0 ) ? fabs( RAM_RESTO - importe )
                             : 0.0 );
            _SET_MEMORY( __pago_auto_tarjeta, pos, VAR_TARJETAS_AUTORIZACION );
            _SET_MEMORY( __pago_buffer, pos, VAR_TARJETAS_NRO_COMERCIO );
            _SET_MEMORY_DOUBLE( __pago_importe_entrega, pos, VAR_TARJETAS_IMPORTE_ENTREGA );
            strcpy( cadena, VAR_TARJETAS_NRO_COMERCIO );
            cadena[15] = 0;
            _SET_MEMORY( __pago_buffer, pos, cadena );
            _SET_MEMORY( __pago_tabla, pos, VAR_TARJETAS_TABLA );
            _SET_MEMORY_LONG( __pago_nro_cupon1, pos, VAR_TARJETAS_NRO_CUPON );
            _SET_MEMORY_LONG( __pago_nro_cupon_int1, pos, VAR_TARJETAS_NRO_CUPON_INT );
            _SET_MEMORY_CHAR( __pago_cuotas, pos, VAR_TARJETAS_CUOTAS );
            	{
			int auxi= 0;
			auxi = (int) VAR_TARJETAS_CUOTAS;
			SET_MEMORY_INT( __puerto_servidor_dbrouter, auxi );
			}

			_SET_MEMORY_INT( __pago_nro_boletin, pos, VAR_TARJETAS_NRO_BOLETIN );
            _SET_MEMORY_CHAR( __pago_status_tarjeta, pos, CUPON_CALCULAR_STATUS() );
            _SET_MEMORY_CHAR( __pago_moneda, pos, VAR_TARJETAS_MONEDA );
            _SET_MEMORY_CHAR( __pago_cod_vigencia, pos, VAR_TARJETAS_COD_VIGENCIA );
            _SET_MEMORY_INT( __pago_primer_vto, pos, VAR_TARJETAS_PRIMER_VTO );
            _SET_MEMORY_DOUBLE( __pago_importe_descuento, pos, 0.0 );
            _SET_MEMORY_INT( __pago_cuenta, pos, VAR_TARJETAS_NRO_ORDEN_CUOTA );
        }
        else {
            if( medios[modo].cheque ) {
                _SET_MEMORY( __pago_buffer, pos, cuenta_banco );
                if( PEDIR_CLIENTE_EN_CHEQUES ) {
                    _SET_MEMORY_LONG( __pago_buffer1_long, pos, RAM_NRO_CLIENTE );
                }
                _SET_MEMORY_INT( __pago_nro_boletin, pos, cod_valida_cheque );
            }
            else {
                _SET_MEMORY_INT( __pago_nro_boletin, pos, 0 );
            }
            _CEREAR_MEMORY( __pago_auto_tarjeta, pos );
            _CEREAR_MEMORY( __pago_buffer, pos );
            _CEREAR_MEMORY( __pago_tabla, pos );
            _SET_MEMORY_LONG( __pago_nro_cupon1, pos, 0 );
            _SET_MEMORY_CHAR( __pago_cuotas, pos, 0 );
            _SET_MEMORY_CHAR( __pago_status_tarjeta, pos, 0 );
            _SET_MEMORY_CHAR( __pago_moneda, pos, 0 );
            _SET_MEMORY_CHAR( __pago_cod_vigencia, pos, 0 );
            _SET_MEMORY_INT( __pago_primer_vto, pos, 0 );
            #ifdef COMPILAR_MUTUALES
            if( medios[modo].mutual ) {
                _SET_MEMORY_CHAR( __pago_cuotas, pos, MUTUAL_CUOTAS );
                _SET_MEMORY_LONG( __pago_nro_cupon_int1, pos, MUTUAL_ORDEN );
            }
            else {
                _SET_MEMORY_CHAR( __pago_cuotas, pos, 0 );
                _SET_MEMORY_LONG( __pago_nro_cupon_int1, pos, 0 );
            }
            #endif
            if( medios[modo].ticket_compra || medios[modo].mutual ) {
                _SET_MEMORY_LONG( __pago_nro_cupon1, pos, numero );
            }
        }
        if( VARIAS_TARJETAS_POR_PAGO && medios[modo].tarjeta ) {
            GRABA_MODIFICACION_TARJETAS( -1, TARJETA_TRADICIONAL );
            ADD_MEMORY_INT( __varios_tarjetas_por_pago, 1 );
        }
        /*------------------------- Muestra datos --------------------------*/
        MOSTRAR_PAGO( pos );
        /*----------------- Se calcula el iva proporcional -----------------*/
        _SET_MEMORY_DOUBLE( __pago_iva_proporcional, pos, 0 );
        if( medios[modo].discrimina_iva == 'S' || medios[modo].discrimina_iva == 's' ) {
            int atrib = _ATRIBUTO;
            if( _PAGO_IMPORTE( pos ) <= RAM_RESTO ) {
                pago_dis_iva = _PAGO_IMPORTE( pos );
            }
            else {
                pago_dis_iva = RAM_RESTO;
            }
            if( RAM_TOTAL ) {
                _SET_MEMORY_DOUBLE( __pago_iva_proporcional, pos,
                                    ( pago_dis_iva * TOT_TICK_IVA1 ) / RAM_TOTAL );
            }
            else {
                _SET_MEMORY_DOUBLE( __pago_iva_proporcional, pos,
                                    ( pago_dis_iva * TOT_TICK_IVA1 ) / RAM_TOTAL_ANT );
            }
            MOSTRAR_FLOTANTE( 52, 11, ENTEROS, DECIMALES, 1, 0, _PAGO_IVA_PROPORCIONAL( pos ) );
            MENSAJE_TECLA_STRING( S_PRESIONE_UNA_TECLA );
        }
        /*---------------------- Recalcula resto ---------------------*/
		/* Para Moneda extranjera, se analiza la versi�n, llegando a un acuerdo que es correcto el funcionamiento:
		el centavo/s faltantes surgen de la cotizaci�n y minima denominaci�n de la moneda extranjera,
		es decir si el cliente paga con moneda extranjera, seguramente no pagara con denominaciones 
		peque�as sino con valores cerrados , ej. u$s 100, 50, 20, 10,... muy rara vez intentar� 
		completar los valores minimos exactos (ej.$15.22). La diferencia de 0.01 centavo de moneda 
		local, no es cubrible por la minima denominaci�n posioble de la moneda extranjera de acuerdo
		con la cotizaci�n, por esta razon este excedente/faltante se solicita se pague con la moneda local.*/
		ADD_MEMORY_DOUBLE( __ram_resto, -importe ); //mmancuso aca puede ser la diferencia de vuelta cliente especial UAA que no de 0.01

		// para la promo post pago ojo con el vuelto si no se puede
		if( RAM_RESTO > -0.02 && RAM_RESTO < -0.004 && medios[modo].vuelto == 0 ) 
			SET_MEMORY_DOUBLE( __ram_resto, 0.00 );


		
        if( medios[modo].tipo_variacion == SOBRE_TOTAL ) {
            ADD_MEMORY_DOUBLE( __ram_resto, variacion );
        }
        SET_MEMORY_DOUBLE( __ram_subtotal, RAM_RESTO );
		ADD_MEMORY_DOUBLE( __ram_variacion, variacion );
        /*------------------ Calcula vuelto en el medio ---------------*/
        if( RAM_RESTO < 0 && _PAGO_TIPO_VUELTO( pos ) == _VUELTO_PROPIO ) {
            vuelto = floor( fabs( RAM_RESTO ) / ( ( double )medios[modo].minima_denominacion ) )
                   * ( ( double )medios[modo].minima_denominacion );
            aux_double = _PAGO_IMPORTE( pos );
            if( vuelto > aux_double ) {
                vuelto = aux_double;
            }
        }
        else if( RAM_RESTO < 0 && _PAGO_TIPO_VUELTO( pos ) == _VUELTO_OTRO_MEDIO ) {
            vuelto_medio = fabs( RAM_RESTO );
        }
        if( MODO_NEGOCIO == SERVICIOS && RAM_MODO_DEVOLUCION ) {
            //En reverso
        } else {
            _SET_MEMORY_DOUBLE( __pago_vuelto_medio, pos, vuelto_medio );
            _SET_MEMORY_DOUBLE( __pago_vuelto, pos, vuelto );
			cod_medio_aux = _PAGO_COD_MEDIO_VUELTO( pos );
 			if( _PAGO_TIPO_VUELTO( pos ) == _VUELTO_OTRO_MEDIO && RAM_RESTO < 0 ) {
				int rta = 0;

				while( !rta ) {
					rta = MOSTRAR_OPCIONES_VUELTO( _PAGO_MODO( RAM_P_PAGO  ), _PAGO_COD_SUBMEDIO( RAM_P_PAGO ),
							&cod_medio_aux, &cod_submedio_aux );
					if( rta && medios[cod_medio_aux - 1].tarjeta ) {
						if( !strlen( RAM_CUENTA_CLIENTE )  ) {
							rta = CARGAR_MONEDERO_ELECTRONICO( NO, NULL );
						}
					}
				}
				if( rta ){
					_SET_MEMORY_CHAR( __pago_cod_medio_vuelto, pos, (char)cod_medio_aux );
					_SET_MEMORY_CHAR( __pago_cod_submedio_vuelto, pos, (char)cod_submedio_aux );
				}
			}
        }
        if( add_p_pago ) {
            ADD_MEMORY_CHAR( __ram_p_pago, 1 );
        }
    }
}
/*****************************************************************************/
void PAGOS_ANULAR_PAGO()
/*****************************************************************************/
{
    int y = 0, handle = 0, i = 0, a = 0, pos = 0, h = 0, rtta = 1;
    long file_size;
    char *buffer_tarjeta = 0;
    struct _datos datos[5];
    if( RAM_P_PAGO > 0 ) {
		//Si hay Datos Adicionales para el medio anulado se borran
		BORRAR_DATOS_ADICIONALES( RAM_P_PAGO, D_MEDIOS_PRE_IMPORTE );
        ADD_MEMORY_CHAR( __ram_p_pago, -1 );
        if( medios[_PAGO_MODO( RAM_P_PAGO )].cheque ) {
            if( REGISTRAR_CHEQUES_EN_ARCHIVO && MODO_NEGOCIO == SERVICIOS ) {
                // recorro todos los dd_tmp_pr.xxx para encontrar los cheques
                // y los elimino de cliecheq.btr
                pos = RAM_P_PAGO;
                h = _PAGO_MODO( RAM_P_PAGO );
                for( a = 0;a <= h;a++ ) {
                    // con esto obtengo el xxx de dd_tmp_pr.xxx
                    i = ( h * 100 ) + a;
                    for( y = 0;y <= pos;y++ ) {
                        // obtengo los datos de los cheques
                        if( _PAGOS_OBTENER_DATOS( i, y, &datos[0], 3 ) ) {
                            /* datos[1] es el numero de cheque,
                               datos[2] es el codigo de banco,
                               uso esos valores para eliminar el cheque
                                   de la tabla cliecheq */
                                ELIMINA_CHEQUE( atoi( ( &datos[1] )->valor ), atoi( ( &datos[2] )->valor ), ( &datos[1] )->valor );
                        }
                    }
                }
            }
        }
        if( medios[_PAGO_MODO( RAM_P_PAGO )].tarjeta ) {
			if( VARIAS_TARJETAS_POR_PAGO ) {
				
				if( 1 ){
					int j = RAM_P_PAGO ;
					int positarjeta[20];
					int hh, h1=0;
					for( hh = 0; hh <= RAM_P_PAGO; hh++ ){
						if( strlen(_PAGO_AUTO_TARJETA(hh)) > 1 ){
							positarjeta[hh]= h1;
							h1++;
						}
					}
					if( TARJETA_APROBADA_ONLINE( j ) != 0 || (VAR_TARJETAS_ON_LINE == 2 && !VAR_TARJETAS_UNICAMENTE_OL )) {
						SETEAR_DEBITO_POS(0);	
						rtta = _ANULAR_TARJETAS(  j - 1 ,SI,positarjeta[RAM_P_PAGO] - 1 );//ANULA UNA EL ULTIMO MEDIO TECLA  " - "
					
                }

                }

				{

                #ifdef INVEL_L
                handle = open( "tarjetas.$$$", O_RDWR );
                #else
                handle = _open( "tarjetas.$$$", O_RDWR );
                #endif
				if( handle != -1 && rtta == 1 ) {
                    file_size = filelength( handle ) - SIZEOF_STRUCT( _var_tarjetas );
                    #ifdef INVEL_L
                    //               trunc( handle, file_size );
                    #else
                    chsize( handle, file_size );
                    #endif
                    if( file_size > 0 ) {
                        _lseek( handle, file_size - SIZEOF_STRUCT( _var_tarjetas ), SEEK_SET );
                        if( !buffer_tarjeta ) {
                            buffer_tarjeta = ( char* )MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
                        }
                        #ifdef INVEL_L
                        read( handle, buffer_tarjeta, ( SIZEOF_STRUCT( _var_tarjetas ) ) );
                        #else
                        _read( handle, buffer_tarjeta, ( SIZEOF_STRUCT( _var_tarjetas ) ) );
                        #endif
                        COPY_BUFFER_TO_STRUCT( buffer_tarjeta, _var_tarjetas );
						
                        ADD_MEMORY_INT( __varios_tarjetas_por_pago, -1 );
                    }


					

                    if( file_size == 0 ) {
                        SET_MEMORY_INT( __varios_tarjetas_por_pago, 0 );
                    }
                    FREEMEM( buffer_tarjeta );
                    #ifdef INVEL_L
                    close( handle );
                    #else
                    _close( handle );
					#endif
				} else {
					if( rtta == 0 )
						#ifdef INVEL_L
						close( handle );
						#else
						_close( handle );
						#endif
				}
                }
			}
            if( MODO_NEGOCIO == SERVICIOS ) {
                CEREAR_STRUCT( _var_tarjetas );
                CR1_MUESTRA_TARJETA();
            }
			if( rtta == 0) {
				ADD_MEMORY_CHAR( __ram_p_pago, 1 );
				return ;
			}
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TARJETA_BORRAR, " ", " ", NULL, " " );
            #endif
            if( PROTOCOLO_AUTORIZACION_TARJETA != _TCP_IP ) {
                DECREMENTAR_NRO_CUPON( _PAGO_TARJETA( RAM_P_PAGO ) );
            }
            #if defined(INVEL_W) || defined(INVEL_L)
            ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_TARJETA_BORRAR, " ", " ", NULL, " " );
            #endif
        }
        /*------------------------ Calcula variacin ------------------------*/
        ADD_MEMORY_DOUBLE( __ram_variacion, -( _PAGO_VARIACION( RAM_P_PAGO ) ) );
        /*------------------ borra en la pantalla ----------------------*/
        y = 8 + RAM_P_PAGO;
        ExecuteAction( A_BORRAR_PAGO );
        /*------------------------ Recalcula resto ----------------------------*/
        ADD_MEMORY_DOUBLE( __ram_resto,
                           _PAGO_IMPORTE( RAM_P_PAGO ) - _PAGO_VARIACION( RAM_P_PAGO ) );
        ADD_MEMORY_DOUBLE( __ram_resto, -RAM_DIFERENCIA_REDONDEO );
        SET_MEMORY_DOUBLE( __ram_diferencia_redondeo, 0.0 );
        SET_MEMORY_DOUBLE( __ram_resto_me, 0.0 );
        PAGOS_MUESTRA_TOTAL( RAM_RESTO );
        if( medios[_PAGO_MODO( RAM_P_PAGO )].autorizacion ) {
            medios[_PAGO_MODO( RAM_P_PAGO )].autorizacion = 0;
            SET_MEMORY_LONG( __ram_nro_cliente, 0 );
            CEREAR_MEMORY( __cliente_afinidad_nombre );
            MOSTRAR_CLIENTE_EN_PANTALLA();
        }
        if( medios[_PAGO_MODO( RAM_P_PAGO )].moneda_extranjera ) {
            ExecuteSSAction( A_MUESTRA_DATOS, T_FLAGS_MEDIO_EXTR_BORRAR, " ", " ", NULL, " " );
        }
        /*sub_medios2.medios = _PAGO_MODO( RAM_P_PAGO ) + 1;     //Anulo los pagos hechos por cada Submedio
        if( medios[_PAGO_MODO( RAM_P_PAGO )].tarjeta ) {
            sub_medios2.submedios = _PAGO_COD_SUBMEDIO( RAM_P_PAGO );
        }   //Pago con tarjeta
        else {
            sub_medios2.submedios = _PAGO_CUENTA( RAM_P_PAGO );
        }
        sub_medios2.importe = -_PAGO_IMPORTE( RAM_P_PAGO );
        SUMAR_IMPORTE_SUBMEDIO( sub_medios2.medios, sub_medios2.submedios, sub_medios2.importe, 0 );*/
		ELIMINA_PAGO_PROMO( _PAGO_MODO( RAM_P_PAGO ) + 1,
			( medios[_PAGO_MODO( RAM_P_PAGO )].tarjeta ? _PAGO_COD_SUBMEDIO( RAM_P_PAGO ) : _PAGO_CUENTA( RAM_P_PAGO ) ) );
		ELIMINA_PAGO_PROMO(
				_PAGO_MODO( RAM_P_PAGO ) + 1,
				(medios[_PAGO_MODO( RAM_P_PAGO )].tarjeta ? _PAGO_COD_SUBMEDIO( RAM_P_PAGO )
						: _PAGO_CUENTA( RAM_P_PAGO )));

		ELIMINA_PAGO_PROMO_POSPAGO(
				_PAGO_MODO( RAM_P_PAGO ) + 1,
				(medios[_PAGO_MODO( RAM_P_PAGO )].tarjeta ? _PAGO_COD_SUBMEDIO( RAM_P_PAGO )
						: _PAGO_CUENTA( RAM_P_PAGO )), 45 );

    }
}
/*****************************************************************************/
void PAGOS_INTERESES()
/*****************************************************************************/
{
    int signo;
    if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, PEDIR_CLAVE_EN_INTERESES,
                          ST( S__INTERESES_ ), NULL, _F_SUP_INTERESES, SI ) ) {
        signo = ( RAM_NOTA_CR ) ? -1 : 1;
        if( RAM_INTERESES ) {
            /*----------------------- Elimina intereses ---------------------------*/
            SET_MEMORY_DOUBLE( __ram_importe_total,
                               ROUND( RAM_IMPORTE_TOTAL - signo * RAM_INTERESES, 10, 2 ) );
            ADD_MEMORY_DOUBLE( __ram_retencion_dgr,
                               -RETENCION( ( TOT_TICK_SV_IMPORTE_SIN_IVA - TOT_TICK_SV_IMP_INT )
                                         * RAM_PORC_INTERES / 100, SI ) );
            ADD_MEMORY_DOUBLE( __ram_percepcion_dgi,
                               -PERCEPCION( ( TOT_TICK_SV_IMPORTE_SIN_IVA - TOT_TICK_SV_IMP_INT )
                                          * RAM_PORC_INTERES / 100, SI ) );
            ADD_MEMORY_DOUBLE( __ram_percepcion_212_iva1, -PERCEPCION_212_IVA_1() );
            ADD_MEMORY_DOUBLE( __ram_percepcion_212_iva2, -PERCEPCION_212_IVA_2() );
            SET_MEMORY_DOUBLE( __ram_intereses, 0 );
        }
        else {
            /*----------------------- Agrega intereses ----------------------------*/
            PAGOS_PEDIR_INTERESES();
            if( RAM_INTERESES ) {
                ADD_MEMORY_DOUBLE( __ram_importe_total, signo * RAM_INTERESES );
                ADD_MEMORY_DOUBLE( __ram_retencion_dgr,
                                   RETENCION( ( TOT_TICK_SV_IMPORTE_SIN_IVA - TOT_TICK_SV_IMP_INT )
                                            * RAM_PORC_INTERES / 100, SI ) );
                ADD_MEMORY_DOUBLE( __ram_percepcion_dgi,
                                   PERCEPCION( ( TOT_TICK_SV_IMPORTE_SIN_IVA - TOT_TICK_SV_IMP_INT )
                                             * RAM_PORC_INTERES / 100, SI ) );
                ADD_MEMORY_DOUBLE( __ram_percepcion_212_iva1, PERCEPCION_212_IVA_1() );
                ADD_MEMORY_DOUBLE( __ram_percepcion_212_iva2, PERCEPCION_212_IVA_2() );
                MOSTRAR_PORC_INTERES();
            }
        }
        /*--------------------------- muestra total ------------------------------*/
        SET_MEMORY_DOUBLE( __ram_resto, PAGOS_TOTAL_A_PAGAR() );
        PAGOS_MUESTRA_TOTAL( RAM_RESTO );
    }
}
/*****************************************************************************/
void PAGOS_PEDIR_INTERESES()
/*****************************************************************************/
{
    ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, ST( S__INTERESES_ ), " ", NULL, " " );
    SET_MEMORY_FLOAT( __ram_porc_interes, PAGOS_PIDE_IMPORTE( ST( S__INTERESES_ ), 3, 2, SI ) );
    ExecuteSSAction( A_MUESTRA_TITULO_BOX, 0, ST( S__INTERESES_ ), " ", NULL, " " );
    if( RAM_PORC_INTERES ) {
        if( RAM_PORC_INTERES < 0 ) {
            if( config.tope_interes > RAM_PORC_INTERES ) {
                MENSAJE_STRING( S_PORCENTAJE_INTERES_EXCEDIDO );
                SET_MEMORY_FLOAT( __ram_porc_interes, 0 );
            }
        }
        else {
            if( config.tope_interes < RAM_PORC_INTERES ) {
                MENSAJE_STRING( S_PORCENTAJE_INTERES_EXCEDIDO );
                SET_MEMORY_FLOAT( __ram_porc_interes, 0 );
            }
        }
        SET_MEMORY_DOUBLE( __ram_intereses,
                           ROUND( RAM_IMPORTE_TOTAL * RAM_PORC_INTERES / 100, 10, 2 ) );
        if( RAM_NOTA_CR ) {
            SET_MEMORY_DOUBLE( __ram_intereses, -RAM_INTERESES );
        }
    }
    else {
        SET_MEMORY_DOUBLE( __ram_intereses, 0 );
    }
}
/*****************************************************************************/
void PAGOS_DISCRIMINA_IVA()
/*****************************************************************************/
{
    SET_MEMORY_CHAR( __ram_discriminar, !RAM_DISCRIMINAR );
    MOSTRAR_DISCRIMINACION_IVA();
}
/*****************************************************************************/
void PAGOS_MARCA_AUTORIZACION()
/*****************************************************************************/
{
    SET_MEMORY_CHAR( __ram_solicitar_autorizacion, !RAM_SOLICITAR_AUTORIZACION );
    MOSTRAR_MARCA_AUTORIZACION();
}
/*****************************************************************************/
double PAGOS_PIDE_IMPORTE( char *titulo, int enteros, int decimales, int signo )
/*****************************************************************************/
{
    return PAGOS_PIDE_IMPORTE_CON_T_COMPRA( titulo, enteros, decimales, signo, 0, 0, 1, 0, 0, 0,
                                            0 );
}
/*********************************************************************************************************************/
double PAGOS_PIDE_IMPORTE_CON_T_COMPRA( char *titulo, int enteros, int decimales, int signo,
                                        char cuenta, char pos, int tipo_vuelto,
                                        int datos_adicionales, char cod_medio, char submedio,
                                        int cant_cuotas )
/*********************************************************************************************************************/
{
    double importe, total = 0, s = 1, totalCheque = 0;
    int salir = 0, posx = 32, tecla = 0, indice_pos = 0;
    char *cadena_scanner = NULL;
    char num_bono[20];
    int i = 0, excedido = 0, modif = 0;
    int repetir = MODO_CHEQUE_COMO_UN_MEDIO;
    //char k;
    int _pos = pos, cnt = 0;
    int mostrar_importe_temporal = 0;

    if( cuenta ) {
        char archivo[24];
        sprintf( archivo, "t_tmp.%d", pos );
        _unlink( archivo );
    }
    if( config.pais == COLOMBIA || config.pais == ECUADOR ) {
        posx = 28;
    }
    if( ( config.pais == CHILE || config.pais == BRASIL ) && RAM_TOTAL > 9999999.99 ) {
        posx = 30;
        enteros += 2;
    }
    while( !salir ) {
        if( signo ) {
            importe = PEDIR_GRAN_FLOTANTE_CON_SIGNO( posx - ancho_panel / 2, 21, enteros,
                                                     decimales );
        }
        else {
            if( datos_adicionales ) {
                importe = -1;
                #if defined(INVEL_W) || defined(INVEL_L)
                ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, titulo, titulo, NULL, " " );
                #endif
                _GRANDE = 1;
                importe = PIDE_IMPORTE_T_COMPRA( ( char )( posx - ancho_panel / 2 ), ( char )21,
                                                     ( char )enteros, ( char )decimales,
                                                     ( char )cuenta, ( char )pos, &cadena_scanner );
                _GRANDE = 0;
				//aca esta entrando y no tiene q entrar, no le esta dando bola a respuesta que estaba en 0!!!!
				if( FABS( importe ) < 0.001 && FABS( total ) < 0.001 && RAM_RESTO > 0.001 ) {
                    importe = RAM_RESTO;
				}
				if( importe > 0.001 && LASTKEY() != 27 ) {
					tecla = LASTKEY();
					/*if( nro_medio && config.pais == ELSALVADOR ) {
						/*for( i = 0; i < RAM_P_PAGO; i++ ) {
            				if( _PAGO_TIPO_VUELTO( i ) == _VUELTO_EN_EFECTIVO ) {
								acu_vuelto += ROUND( _PAGO_IMPORTE( i )
									* medios[_PAGO_MODO( i )].porc_vuelto / 100, 10, DECIMALES );
							}
            			}
            			if( tipo_vuelto == _VUELTO_EN_EFECTIVO ) {
            				acu_vuelto += ROUND( ( importe )
            					* medios[nro_medio].porc_vuelto / 100, 10, DECIMALES );
            			}
						if( ROUND( importe - RAM_RESTO, ENTEROS, DECIMALES )
								> ROUND( acu_vuelto, ENTEROS, DECIMALES ) && !MODO_DEVOLUCION ) {
							MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
							importe = 0;
							if( total > 0 )
   								tecla = '+';
	   						else if( tecla == 13 )
		   						tecla = 27;
	              	   		ok = 0;
						}
					}*/

					//if( ok ) {
						if( fabs( RAM_RESTO - ( total + importe * s ) ) < 0.000005 ) {
							SET_MEMORY_DOUBLE( __ram_resto, ( total + importe * s ) );
						}

						if( RAM_RESTO - ( total + importe * s ) < 0.0 && !tipo_vuelto ) {
							MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
							importe = 0;
							if( total > 0 ) {
								tecla = '+';
							}
							else if( tecla == 13 ) {
								tecla = 27;
							}
						} else {
							/*Precondicion p/ datos adicionales en Medios:
							* 1:Los datos tienen que ser pre_importe (posicion pedido = 1)
							* 2:Cuando hay datos adicionales para cuotas medios, no valen los datos
							*   adicionales asociados a medios.
							*Usos:
							* 1:Verificar Importe:
							*   Verifica que el importe sea identico al del Boletin (BOLETIN_POSITIVO y BOLETIN_SERVIDOR)
							* 2:Chequear repeticion de ticket de compra:
							*   Verifica que un ticket no haya sido usado (BOLETIN_POS_CON_IMPORTE)
							*/


							//importe = 0;
							//lipio datos de posicion actual por si ya se habia usado
							//Se comenta porque no guarda los datos adicionales del medio C. REGALO
							//cuando se presiona + y solicita los prox datos de C. REGALO
							//BORRAR_DATOS_ADICIONALES( pos, D_MEDIOS_PRE_IMPORTE );
							if( PAGOS_PEDIR_DATOS_ADICIONALES( ( int )_pos, ( char )indice_pos/*pos*/, datos_adicionales,
									&importe, D_MEDIOS_PRE_IMPORTE, NULL, 1,
									NULL, NULL , cod_medio + 1, submedio ) ) {
   								indice_pos++;
								BORRAR_MENSAJE();
   								if( RAM_RESTO - ( total + importe * s ) <= 0.005 ) {
   									tecla = 13;
   								}
   							} else {
								//Dato Adicional no cargado (Ej. ESCAPE)
   								importe = 0;
   								if( total > 0 )
   									tecla = '+';
								else if( tecla == 13 ){
									struct _dmedios dmedios,*datos_medio;
									int cant_datos = PAGOS_CARGAR_DATOS_MEDIO( &datos_medio, datos_adicionales, D_MEDIOS_PRE_IMPORTE );
   									if(cant_datos > 0)
										tecla = 27;
								}
   							}
						}
					//}
					SET_LASTKEY( tecla );
				}
		} else {
			if( cuenta ) {
				_GRANDE = 1;
                importe = PIDE_IMPORTE_T_COMPRA( ( char )( posx - ancho_panel / 2 ), ( char )21,
						( char )enteros, ( char )decimales, cuenta, pos, &cadena_scanner );
                _GRANDE = 0;
				if( importe > 0 && LASTKEY() != 27 ) {
					if( config.pais == COLOMBIA ) {
						tecla = LASTKEY();
						if( CONTROLA_IMPORTE_TICKET_COMPRA( cuenta, num_bono, importe ) ) {
							AGREGA_A_POSICION( pos, num_bono, importe );
                            if( fabs( RAM_RESTO - ( total + importe * s ) ) < 0.000005 ) {
								SET_MEMORY_DOUBLE( __ram_resto, ( total + importe * s ) );
                            }

                            if( RAM_RESTO - ( total + importe * s ) < 0.0 && !tipo_vuelto ) {
								MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                                importe = 0;
                                if( total > 0 ) {
									tecla = '+';
                                }
                                else if( tecla == 13 ) {
									tecla = 27;
                                }
                            } else if( RAM_RESTO - ( total + importe * s ) <= 0.005 ) {
								tecla = 13;
                            }
                        } else {
							importe = 0;
                            if( total > 0 ) {
								tecla = '+';
                            } else if( tecla == 13 ) {
								tecla = 27;
                            }
                        }
                        SET_LASTKEY( tecla );
                    } else {
						AGREGA_A_POSICION( pos,
								( cadena_scanner != NULL ) ? cadena_scanner : "", importe );
                    }
                }
            } else {
				#if defined(INVEL_W) || defined(INVEL_L)
                ExecuteSSAction( A_MUESTRA_TITULO_BOX, 1, titulo, titulo, NULL, " " );
                #endif
                importe = PEDIR_GRAN_FLOTANTE( posx - ancho_panel / 2, 21, enteros, decimales );
                /*#ifdef INVEL_W
					ExecuteAction( A_FIN_PEDIR );
                #endif*/
            }
        }
	}

        // ERROR DE PAGO
        switch( LASTKEY() ) {
            case 13:
                salir = 1;
                total += importe * s;
                mostrar_importe_temporal = 0;
                break;
            case 27:
			    //Si hay Datos Adicionales para el medio actual se borran
				BORRAR_DATOS_ADICIONALES( RAM_P_PAGO, D_MEDIOS_PRE_IMPORTE );
                salir = 2;
                total = 0;
                mostrar_importe_temporal = 0;
                break;
            case '+':
                MOSTRAR_CADENA( 40 - ancho_panel / 2 - strlen( titulo ) / 2, 20, titulo );
                total += importe * s;
                s = 1;
                if( REPETIR_MEDIO ) {
                    mostrar_importe_temporal = 1;
                    salir = 0;
                }
                break;
            case '-':
				//Hay que ver como borrar los datos adicionales cuando se selecciona esta tecla
				total += importe * s;
                s = -1;
                mostrar_importe_temporal = 1;
                break;
        }
        PAGOS_MUESTRA_IMPORTE_TEMPORAL( total, mostrar_importe_temporal );
    }//fin del while
    if( total == 0.0 && RAM_RESTO > 0.0 && salir != 2 ) {
        total = RAM_RESTO;
        MOSTRAR_GRAN_FLOTANTE( posx - ancho_panel / 2, 21, enteros, decimales, 1, 0, total );

        if( cuenta ) {
            AGREGA_A_POSICION( pos, ( cadena_scanner != NULL ) ? cadena_scanner : "", total );
        }
    }
    if( ( config.pais == CHILE || config.pais == BRASIL ) && RAM_TOTAL > 9999999.99 ) {
        enteros -= 2;
    }

    return ( total );
}

/*****************************************************************************/
void PAGOS_MUESTRA_IMPORTE( double importe )
/*****************************************************************************/
{
    int posx = 32;
    if( config.pais == COLOMBIA || config.pais == ECUADOR ) {
        posx = 28;
    }
    if( ( config.pais == CHILE || config.pais == BRASIL ) && importe > 9999999.99 ) {
        posx = 30;
    }
    MOSTRAR_GRAN_FLOTANTE( posx - ancho_panel / 2, 21, ( config.pais == ECUADOR ) ? 6 : ENTEROS,
                            DECIMALES, 1, 0, importe );
}
/*****************************************************************************/
long PAGOS_PIDE_AUTORIZACION( char *titulo )
/*****************************************************************************/
{
    long autorizacion;
    ExecuteSSAction( A_PEDIR, 1, titulo, "", NULL, " " );
    autorizacion = PEDIR_GRAN_ENTERO( 32 - ancho_panel / 2, 23, 6 );
    ExecuteAction( A_FIN_PEDIR );
    return ( autorizacion );
}
/*****************************************************************************/
int PAGOS_IMPRIME_PAGO( int modo )
/*****************************************************************************/
{
    int h=0, posicion = 0, ok = 1, pos = 0;
    char cadena[20];
    int _ptr,_handle,_estado,_h;
	unsigned aux_fecha;
	int msj_entrante = 0, msj_saliente = 0, tipo = 0;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //_estado = STACK_INS( sizeof( int ) );
    //_h = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    _h = STACK_INS( sizeof( short ) );

	//if(IMPRESORA_FISCAL==FISCAL && modo == _MODO_CAMBIO_MEDIO) // Seba
	//{
	/*----------------------------- Imprime IVA ------------------------------*/
	if( IMPRESORA_FISCAL == FISCAL && DOCUMENTO_ABIERTO( ) == 2 && ok == SI ){
		if( _ESTADO != 2 ){ //me parece que si el estado es 2 no deberia salir UAA-38
			CLOSE_RECUPERO( _ptr, _handle ); //siempre que se tiene un openrecupero y se sale de la funcion hay que hacer el close
			return NO;
		}
	}
	if( _ESTADO == 0 ) {
		if( RAM_DISCRIMINAR ) {
			IMPRIME_IVA();
		}
		_SET_ESTADO( 1 );
	}
	/*------------------------ Imprime medios de pago ------------------------*/
	if( _ESTADO == 1 ) {
	/*------------------------ Imprime total----- ------------------------*/
		for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO;ADD_MEMORY_INT( _h, 1 ) ) {
			h = GET_MEMORY_INT( _h );
			/*--------------------- Modo de pago e importe ---------------------*/
			if( !TKT_CONFIGURABLE ) {
				if( !medios[_PAGO_MODO( h )].tarjeta || VAR_TARJETAS_TARJETA ) {
					IMPRIME_MEDIO_DE_PAGO( _PAGO_MODO( h ), _PAGO_IMPORTE( h ) );
				}
				/*---------------------- Vuelto, si corresponde --------------------*/
				if( medios[_PAGO_MODO( h )].efectivo && POSICION_VUELTO == _DESPUES_CTDO ) {
					if( RAM_VUELTO >= 0.01 ) {
						LF( 1 );
						IMPRIME_VUELTO( RAM_VUELTO );
					}
				}
				/*---------------- Datos para cuentas corrientes -------------------*/
				if( medios[_PAGO_MODO( h )].cta_cte ) {
					if( UTILIZA_SOCIOS ) {
						IMPRIME_DATOS_CTA_CTE( _PAGO_NUMERO( h ) );
						IMPRIME_NOMBRE_CLIENTE( CLIENTE_AFINIDAD_NOMBRE );
					}
					else {
						IMPRIME_DATOS_CTA_CTE( _PAGO_CUENTA( h ) );
					}
				}
			/*--------------------- Datos para cheques -------------------------*/
			else if( medios[_PAGO_MODO( h )].cheque ) {
				BUSCA_NOMBRE_BANCO( _PAGO_BANCO( h ) );
				memcpy( cadena, bancos.nom_banco, 18 );
				cadena[18] = 0;
				IMPRIME_DATOS_CHEQUE( _PAGO_NUMERO( h ), cadena, _PAGO_FECHA_VTO( h ),
						"00000000" );
			}
			/*-------------------- Datos para tarjetas -------------------------*/
			else if( medios[_PAGO_MODO( h )].tarjeta ) {
				// Esto debiera ser configurable
				/* if( VAR_TARJETAS_TARJETA == _CMR ) { SET_MEMORY_CHAR(
				* __ram_imprimir_voucher_cmr, SI ); SET_MEMORY_INT(
				* __ram_subindice_pago_cmr, h ); } else {
				* IMPRIME_DATOS_TARJETA( _PAGO_NRO_CUPON_INT1(h) ); } */
				if( VARIAS_TARJETAS_POR_PAGO ) {
					LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
				}
				IMPRIME_DATOS_TARJETA( _PAGO_NRO_CUPON_INT1( h ) );
				if( VARIAS_TARJETAS_POR_PAGO ) {
					GRABA_MODIFICACION_TARJETAS( posicion - 1, TARJETA_TRADICIONAL );
				}
			}
			/*------------------ Datos para Tickets de compra ------------------*/
			else if( medios[_PAGO_MODO( h )].ticket_compra ) {
				IMPRIME_DATOS_TICKET_COMPRA( _PAGO_CUENTA( h ) );
			}
			#ifdef COMPILAR_MUTUALES
			/*---------------------- Datos para mutuales -----------------------*/
            else if( medios[_PAGO_MODO( h )].mutual ) {
                    IMPRIME_DATOS_MUTUAL( ( char )_PAGO_CUENTA( h ) );
                }
                #endif
            }
			else {
				if( medios[_PAGO_MODO( h )].tarjeta && VARIAS_TARJETAS_POR_PAGO ) {
					if( modo == _MODO_CAMBIO_MEDIO ) {/* ENTRAN_MEDIOS */
						if ( (_PAGO_CUENTA( h ) == ENTRAN_MEDIOS ) ) {
							LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
						} else {/* SALEN_MEDIOS */
							if( VARIOS_TARJETAS_POR_PAGO ) {
								LEE_TARJETAS( pos++, TARJETA_COBRO );
							}
						}
					} else {
						LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
					}
				}
				/*Se decrementa el mes solo para la impresion, luego se le devuelve el valor original*/
				if( medios[_PAGO_MODO( h )].tarjeta ) {
					aux_fecha = GET_FECHA_VTO_PRIVADA( );
					SET_FECHA_VTO_PRIVADA( DECREMENTAR_MES( aux_fecha ) );
					/*Para impresion de cuota en ticket cuando se activo una promo MSI*/
					if( MEDIO_ACTIVADO_PROMO( VAR_TARJETAS_TARJETA + 1, VAR_TARJETAS_CUOTAS, medios[_PAGO_MODO( h )].nro ) ) {
						SET_MEMORY_INT( __ram_cod_promo, 1 );
					}else {
						//SET_MEMORY_INT( __ram_cod_promo, 0 );
					}
				}
				if( medios[_PAGO_MODO( h )].cheque && MODO_CHEQUE_COMO_UN_MEDIO ) {
					// si es cheque imprime todos los cheques cargados
					int i;
					for( i = 0;i < _PAGO_NUMERO( h );i++ ) {
						IMPRIMIR_TKT_MEDIOS( h, i );
					}
				} else {
					if( modo == _MODO_CAMBIO_MEDIO ) {/* Permite entrar un solo medio*/

						tipo =_PAGO_CUENTA( h );// ENTRAN_MEDIOS O  SALEN_MEDIOS!!!!!
						/* lo dejo por compatibilidad si se utilizo ...*/
						medio_saliente = tipo;
						msj_saliente = tipo;
						/*
						if( tipo == ENTRAN_MEDIOS && msj_entrante ) {
							tipo = SALEN_MEDIOS;
						}
						if( tipo == ENTRAN_MEDIOS && !msj_entrante ) {
							//MOSTRAR_CADENA( 13, PRN, " MEDIO ENTRANTE " );
							msj_entrante = 1;
							medio_saliente = NO;
						} else {
							if( tipo == SALEN_MEDIOS && !msj_saliente) {
							//LF(1);
							//MOSTRAR_CADENA( 13, PRN, " MEDIO SALIENTE " );
							msj_saliente = 1;
							medio_saliente = SI;
							}
						}*/
					}
					IMPRIMIR_TKT_MEDIOS( h, h );
					if( medios[_PAGO_MODO( h )].tarjeta ) {
						SET_FECHA_VTO_PRIVADA( aux_fecha );
					}
				}
			}
		}// fin del for imprime total
	//}//Seba
		if( IMPRESORA_FISCAL == FISCAL && DOCUMENTO_ABIERTO( ) == 2 && ok == SI ){
			CLOSE_RECUPERO( _ptr, _handle );
			return NO;
	    }
		if( modo != _MODO_CAMBIO_MEDIO ) {
			_SET_ESTADO( 2 );
		} else {
			_SET_ESTADO( 5 );
		}
		if( TKT_CONFIGURABLE && modo != _MODO_CAMBIO_MEDIO && IMPRESORA_FISCAL != FISCAL ) {
			IMPRIMIR_TKT_DONACION();
		}
		if( TKT_CONFIGURABLE && modo != _MODO_CAMBIO_MEDIO ) {
			IMPRIMIR_TKT_VUELTO( h );
		}
		if( TKT_CONFIGURABLE && modo != _MODO_CAMBIO_MEDIO
				&& ( strcmp( RAM_CUENTA_CLIENTE, "" ) != 0 ) ) {
			IMPRIME_TKT_DESCRIPCION_TARJETA( 1,1 );
		}
		if( modo != _MODO_CAMBIO_MEDIO ) {
			_SET_ESTADO( 2 );
		} else {
			_SET_ESTADO( 5 );
		}
	}

    /*----------------------------- Imprime cuerpo total ----------------------------*/
	if( _ESTADO == 2 ) {
		mostrarmensaje = 0;
		ok = IMPRIME_TOTAL( RAM_TOTAL, RAM_BULTOS, modo );
        if( ok ) {
            _SET_ESTADO( 3 );
        }
    }

    /*---------------------------- Imprime vuelto ----------------------------*/
    if( _ESTADO == 3 ) {
        if( POSICION_VUELTO == _AL_FINAL && RAM_VUELTO >= 0.01 && !TKT_CONFIGURABLE ) {
            IMPRIME_VUELTO( RAM_VUELTO );
        }
        _SET_ESTADO( 4 );
    }
    /*------------------------- Imprime voucher CMR --------------------------*/
    if( _ESTADO == 4 ) {
        // Esto debiera ser configurable
        /* if( VAR_TARJETAS_TARJETA == _CMR ) { IMPRIME_MEDIO_DE_PAGO(
             * _PAGO_MODO( RAM_SUBINDICE_PAGO_CMR ), _PAGO_IMPORTE(
             * RAM_SUBINDICE_PAGO_CMR ) ); } if( RAM_IMPRIMIR_VOUCHER_CMR ) {
             * IMPRIME_DATOS_TARJETA(
             * _PAGO_NRO_CUPON_INT1(RAM_SUBINDICE_PAGO_CMR) ); } */
        _SET_ESTADO( 5 );
    }

	/*------------------ Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
	mostrarmensaje = 1;//la fiscal nueva con poco papel se clava en pagos
    return( ok );
}
/*****************************************************************************/
void PAGOS_IMPRIME_PAGOS_ESPECIALES( int devolucion, int preticket )
/*****************************************************************************/
{
    int h, k, posicion = 0, j = 0, abierto = NO;
    int _ptr,_handle,_estado,_h,cod_val =0 ;
	unsigned aux_fecha;
	//todos: con un 0 indica q solo se imprime el voucher de tarjeta. Con un 1, se imprimen todos.
	//if(!IMPRESORA_FISCAL)//Seba
	//{
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    /*_estado = STACK_INS( sizeof( int ) );
    _h = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
    _h = STACK_INS( sizeof( short ) );
    SET_MEMORY_INT( _h, 0 );
    /*-------------------------- Imprime comprobantes ------------------------*/
    for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO;ADD_MEMORY_INT( _h, 1 ) ) {
        h = GET_MEMORY_INT( _h );
        //Se reemplaza porque la RAM_POS_PAGO no funciona bien.
		//SET_MEMORY_INT( __ram_pos_pago, h );
        SET_MEMORY_INT( __ram_ptr, h );//Se utiliza en impresi�n
        /*---------------------- Vouchers de tarjetas -------------------------*/
		if( medios[_PAGO_MODO( h )].tarjeta && (preticket || (IMPRESORA_FISCAL == 0 && TIPO_IMPRESORA == IMPRESORA_DRIVER) || ( VOUCHER_TARJETA_FISCAL == NO && IMPRESORA_FISCAL == FISCAL))) {
            if( VARIAS_TARJETAS_POR_PAGO ) {
                LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
            }
            /*----------------------- Para el comercio -------------------------*/
            if( _ESTADO == 0 ) {
				if(! (RAM_IMPORTACION_PEDIDOS && importando_ticket_devolucion == 2)) { //para la NC automatica en la anulacion por tarjeta
                if( EMITIR_VOUCHER_TARJETA && VOUCHER_TARJETA_FISCAL == NO ) {
                    cod_val = ( VAR_TARJETAS_MONEDA == PESOS ) ? TARJ[VAR_TARJETAS_TARJETA].
                    val_voucher_pesos : TARJ[VAR_TARJETAS_TARJETA].val_voucher_dolares;
                    if( !cod_val ) {
   						if( IMPRESORA_FISCAL == FISCAL )
   							ABRIR_DOCUMENTO_NO_FISCAL ( 0);
						OPEN_COPIA_VOUCHER_PRN(  );//EL OPEN LO BORRA
						IMPRIME_VOUCHER( h, 1, 0L );
						CLOSE_COPIA_VOUCHER_PRN(  );
   						if( IMPRESORA_FISCAL == FISCAL )
   							CERRAR_DOCUMENTO_NO_FISCAL();

                    }
					else {
						/*Se decrementa el mes solo para la impresion, luego se le devuelve el valor original*/
						aux_fecha = GET_FECHA_VTO_PRIVADA( );
						SET_FECHA_VTO_PRIVADA( DECREMENTAR_MES( aux_fecha ));
						if( MODO_NEGOCIO == RETAIL ) {
							int submedio = 0;

							submedio = _PAGO_COD_SUBMEDIO( h )- 1;
							if( !( ( MODO_DEVOLUCION || devolucion )&& TARJ[submedio].hab_voucher_dev == 'N') ) {
								VALIDAR_VOUCHER_TARJETA( cod_val, _PAGO_IMPORTE( h ), h );
							
							}
						} else {
								VALIDAR_VOUCHER_TARJETA( cod_val, _PAGO_IMPORTE( h ), h );
							
						}
						SET_FECHA_VTO_PRIVADA( aux_fecha );
                    }
                }
                _SET_ESTADO( 1 );
				} else
					_SET_ESTADO( 0 ); //no imprimir voucher
                
            }
            /*----------------------- Para el cliente --------------------------*/
            if( _ESTADO == 1 ) { //cuando se tiene validacion en el val-caja se pone el voucher para el cliente aca no se vuelve a imprimir
				if( EMITIR_VOUCHER_TARJETA == 2 && VOUCHER_TARJETA_FISCAL == NO && !cod_val ) {
                    cod_val = ( VAR_TARJETAS_MONEDA == PESOS ) ? TARJ[VAR_TARJETAS_TARJETA].
                    val_voucher_pesos : TARJ[VAR_TARJETAS_TARJETA].val_voucher_dolares;
                    if( !cod_val ) {
						if( IMPRESORA_FISCAL == FISCAL )
							ABRIR_DOCUMENTO_NO_FISCAL ( 0);
						OPEN_COPIA_VOUCHER_PRN(  );
						IMPRIME_VOUCHER( h, 0, 0L );
						CLOSE_COPIA_VOUCHER_PRN();

						if( IMPRESORA_FISCAL == FISCAL )
							CERRAR_DOCUMENTO_NO_FISCAL();
                    }
                    else {
                        VALIDAR_VOUCHER_TARJETA( cod_val, _PAGO_IMPORTE( h ), h );
                    }
                }
            }
            if( VARIAS_TARJETAS_POR_PAGO ) {
                GRABA_MODIFICACION_TARJETAS( posicion - 1, TARJETA_TRADICIONAL );
            }
        }
        /*------------------------ Llenado de cheques -------------------------*/
		else if( medios[_PAGO_MODO( h )].cheque && !preticket ) {
            if( !RAM_NOTA_CR ) {
                if( _PAGO_NRO_BOLETIN( h ) > 0 ) {
                    MENSAJE_STRING( S_PRESIONE_ENTER_PARA_IMPRIMIR_EL_CHEQUE );
                    k = GETCH();
                    switch( k ) {
                        case 13:
                            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
                                DAR_VUELTA_CHEQUE_IBM4610();
                            }
                            IMPRIMIR_CHEQUE( _PAGO_IMPORTE( h ), _PAGO_FECHA_VTO( h ),
                                             _PAGO_NRO_BOLETIN( h ) );
                            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
                                DAR_VUELTA_CHEQUE_IBM4610();
                            }
                            break;
                    }
                    BORRAR_MENSAJE();
                }
            }
        }
            /*------------------------- Voucher de ctacte -------------------------*/
		else if( medios[_PAGO_MODO( h )].cta_cte && !preticket ) {
            if( EMITIR_VOUCHER_CTA_CTE ) {
                /*------------------- asigna nro de comprobante -----------------*/
                if( _ESTADO == 0 ) {
                    if( PEDIR_COMPROBANTE_CTA_CTE ) {
                        _SET_MEMORY_LONG( __pago_nro_cupon_int1, h, _PAGO_BANCO( h ) );
                    }
                    else {
                        ADD_MEMORY_LONG( __nro_comp_cta_cte, 1L );
                        _SET_MEMORY_LONG( __pago_nro_cupon_int1, h, NRO_COMP_CTA_CTE );
                    }
                    _SET_ESTADO( 1 );
                }
				cod_val = medios[_PAGO_MODO( h )].cod_valida;
				
				if( cod_val > 0  ) {
					VALIDAR_UN_MEDIO( h, cod_val, _MODO_VENTA );

				}
                /*----------------------- Para el comercio -------------------------*/
				if( cod_val == 0 ){
					if( _ESTADO == 1 ) {
						IMPRIME_VOUCHER_CTACTE( h, 1, 0L, _modo );
						_SET_ESTADO( 2 );
					}
					/*----------------------- Para el cliente --------------------------*/
					if( _ESTADO == 2 ) {
						IMPRIME_VOUCHER_CTACTE( h, 0, 0L, _modo );
					}
				}
            }
        }
		/*------------------------- Voucher de Mutual -------------------------*/
		else if( medios[_PAGO_MODO( h )].mutual && !preticket ) {
			cod_val = medios[_PAGO_MODO( h )].cod_valida;
			if( cod_val == 0  ) {
				cod_val = COD_VALIDA_MUTUAL( _PAGO_CUENTA( h ) );
			}
			if( cod_val ) {
				VALIDAR_VOUCHER_MUTUAL( h, cod_val, _PAGO_IMPORTE( h ) );
				//VALIDAR_UN_MEDIO( h, cod, _MODO_VENTA );
			}
		}
		/*------------------------- Voucher de Ticket Compra -------------------------*/
		else if( medios[_PAGO_MODO( h )].ticket_compra && !preticket ) {
			cod_val = medios[_PAGO_MODO( h )].cod_valida;
			if( cod_val == 0 ) {
				cod_val = COD_VALIDA_T_COMPRA( _PAGO_CUENTA( h ) );
			}
			if( cod_val ) {
				VALIDAR_VOUCHER_TICKET_COMPRA( h, cod_val, _PAGO_IMPORTE( h ) );
				//VALIDAR_UN_MEDIO( h, cod, _MODO_VENTA );
			}
		}
        _SET_ESTADO( 0 );
		if( h + 1 == RAM_P_PAGO ) {
            //Se reemplaza porque la RAM_POS_PAGO no funciona bien.
			//SET_MEMORY_INT( __ram_pos_pago, 0 );
            SET_MEMORY_INT( __ram_ptr, 0 );
		}
	    if( CANT_VOUCHER ) {
		    //Aqui se envia el fin de voucher en el caso de mas de un voucher
		    if( IMPRESORA_FISCAL != FISCAL)
				setData( ACTION_SET_HEADER, FINISH_VOUCHER, 0 );
		    //Se envia LF para que envie el cmd FINISH_VOUCHER
		    LF( 0 );
		    setData( ACTION_PRINT, NULL, 0 );
		    CANT_VOUCHER = 0;
        }
	}
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
	imprime_copia_voucher = 0;
	//}//Seba
}

/*****************************************************************************/
int PAGOS_GRABA_PAGO( int modo, int dev_cobro )
/*****************************************************************************/
{
    double tot_efectivo = 0,
           tot_vueltos_propios = 0, tot_vueltos_medios = 0, importe, vuelto, vuelto_medio, v, importe_me = 0.0;
    int h,m,i,k, signo = 1;
    int _ptr, _handle, _estado, _h, posicion = 0, codSubmedioTmp = 0;
    double tot_cheques = 0;
    double tmp_impuestos = 0, tmp_sobretasa = 0, importeTmp = 0;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    /*_estado = STACK_INS( sizeof( int ) );
    _h = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
    _h = STACK_INS( sizeof( short ) );
    if( PEDIR_CLIENTE_EN_CHEQUES && RAM_NRO_CLIENTE ) {
        for( i = 0;i < RAM_P_PAGO;i++ ) {
            if( medios[_PAGO_MODO( i )].cheque ) {
                tot_cheques += _PAGO_IMPORTE( i );
            }
        }
    }
    /*---------------------------- Graba archivos ----------------------------*/
    for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO;ADD_MEMORY_INT( _h, 1 ) ) {
        h = GET_MEMORY_INT( _h );
        m = _PAGO_MODO( h );
        importe = _PAGO_IMPORTE( h );
        vuelto = _PAGO_VUELTO( h );
        vuelto_medio = _PAGO_VUELTO_MEDIO( h );
        importe_me = _PAGO_IMPORTE_ME( h );
        //if( modo == _MODO_NC || ( MODO_DEVOLUCION && modo != _MODO_COBRANZA ) ) {
        if( modo == _MODO_NC || MODO_DEVOLUCION || dev_cobro ) {
            importe *= -1;
            importe_me *= -1;
            vuelto *= -1;
            vuelto_medio *= -1;
            signo = -1;
        }
        /*------------------ Actualiza acumuladores por medio -----------------*/
        if( _ESTADO == 0 ) {
            if( ( modo == _MODO_COBRANZA )
             || ( modo == _MODO_REPROCESAMIENTO && event_ticket.tipo_evento == _EVENT_COBRO ) ) {
                _ADD_MEMORY_DOUBLE( __x_cobros_importe, m, importe - vuelto );
                _ADD_MEMORY_DOUBLE( __x_cobros_importe_me, m, importe_me );
                _ADD_MEMORY_INT( __x_cobros_cantidad, m, signo );
                if( vuelto_medio ) {
                    _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_COD_MEDIO_VUELTO( h ) - 1,
                                        -vuelto_medio );
                }
            }
            else {
                if( modo == _MODO_REPROCESAMIENTO
                 && event_ticket.tipo_evento == _EVENTO_COBRO_ESPECIAL ) {
                    ADD_MEMORY_DOUBLE( __x_dev_envases_importe, importe - vuelto );
                    ADD_MEMORY_INT( __x_dev_envases_cantidad, 1 );
                }
                else {
                    for( k = 0 ;k < 4 ;k++ ) {
                        tmp_impuestos = _TOT_TICK_IMPUESTOS( k );
                    }
                    for( k = 0 ;k < 10 ;k++ ) {
                        tmp_sobretasa = _TOT_TICK_IMP_SOBRETASA( k );
                    }
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe_sin_redondeo, m,
                                        TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1 + TOT_TICK_IVA2
                                      + TOT_TICK_EXENTO + tmp_impuestos + tmp_sobretasa );
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe, m, importe - vuelto );
                    _ADD_MEMORY_INT( __x_ventas_cantidad, m, 1 );
                    if( vuelto_medio ) {
                        _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_COD_MEDIO_VUELTO( h ) - 1,
                                            -vuelto_medio );
                    }
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe_me, m, _PAGO_IMPORTE_ME( h ) );
                }
            }

            /*--------------- actualiza acumulador por submedio --------------*/
            if( medios[m].tarjeta ) {
                codSubmedioTmp = _PAGO_COD_SUBMEDIO( h );
            }
            else {
                codSubmedioTmp = _PAGO_CUENTA( h );
            }
            SUMAR_IMPORTE_SUBMEDIO( m + 1, codSubmedioTmp, importe - vuelto );
            /*------------------ actualiza vuelto medio ------------------------*/
            if( vuelto_medio ) {
                SUMAR_IMPORTE_SUBMEDIO( _PAGO_COD_MEDIO_VUELTO( h ), _PAGO_COD_SUBMEDIO_VUELTO( h ),
                    - vuelto_medio );
            }
            _SET_ESTADO( 1 );
        }
        /*---------------------- Si es Efectivo --------------------------*/
        if( medios[m].efectivo ) {
            ADD_MEMORY_DOUBLE( __x_tot_dinero, /*m,*/ importe );
            tot_efectivo = importe;
        }
        /*------------------- Si es Cuenta Corriente ---------------------*/
        else if( medios[m].cta_cte ) {
            if( UTILIZA_SOCIOS ) {
                if( _ESTADO == 1 ) {
                    ACTUALIZA_SALDO_CLIENTE( importe );
                    _SET_ESTADO( 2 );
                }
                if( _ESTADO == 2 ) {
                    ACTUALIZA_SOCIOS( importe );
                }
            }
        }
            /*------------------------ Si es Cheque --------------------------*/
        else if( medios[m].cheque ) {
            if( _ESTADO == 1 ) {
                #ifdef COMPILAR_ASC
                GENERA_CHEQUES_ASC( h );
                #endif
                _SET_ESTADO( 2 );
            }
        }
            /*------------------ Si es Tarjeta de Crdito --------------------*/
        else if( medios[m].tarjeta ) {
            if( VARIAS_TARJETAS_POR_PAGO ) {
                LEE_TARJETAS( posicion++, TARJETA_TRADICIONAL );
            }
            /*---------------------- Graba tarjetas.asc ------------------------*/
            if( _ESTADO == 1 ) {
                #ifdef COMPILAR_ASC
                GENERA_TARJETAS_ASC( h );
                #endif
                _SET_ESTADO( 2 );
            }
            /*------------------------ Graba el cupn --------------------------*/
            if( _ESTADO == 2 ) {
                GENERAR_CUPON( h );
                _SET_ESTADO( 3 );
            }
            /*------------------------ Actualiza COMTAR ------------------------*/
            if( _ESTADO == 3 ) {
                GRABAR_COMTAR( VAR_TARJETAS_TARJETA, GET_NUMERO_TARJETA_PRIVADO( ), 0 );
            }
            if( VARIAS_TARJETAS_POR_PAGO ) {
                GRABA_MODIFICACION_TARJETAS( posicion - 1, TARJETA_TRADICIONAL );
            }
        }
            /*---------------------- Si es ticket de compra -----------------------*/
        else if( medios[m].ticket_compra ) {
            #ifdef COMPILAR_ASC
            GENERA_TICKET_ASC( h );
            #endif
        }
            #ifdef COMPILAR_MUTUALES
            /*--------------------------- Si es mutual ----------------------------*/
        else if( medios[m].mutual ) {
            #ifdef COMPILAR_ASC
            GENERA_MUTUAL_ASC( h );
            #endif
        }
        #endif
        /*--------------- Acumula vueltos propios --------------------*/
        tot_vueltos_propios += vuelto;
        tot_vueltos_medios += vuelto_medio;
        _SET_ESTADO( 0 );
    }
    /*------------------ Actualiza acumuladores de intereses -----------------*/
    if( RAM_INTERESES ) {
        ADD_MEMORY_DOUBLE( __x_intereses_importe, RAM_INTERESES );
        ADD_MEMORY_INT( __x_intereses_cantidad, 1 );
    }
    if( tot_cheques > 0 ) {
        GRABA_SALDO_CHEQUE_CLIENTE( tot_cheques );
    }
    /*----------------- Considera el vuelto en los acumuladores --------------*/
    if( RAM_RESTO_ME ) {
        if( MODO_NEGOCIO == SERVICIOS ) {
            _ADD_MEMORY_DOUBLE( __x_cobros_importe, _medio_moneda_extranjera, RAM_RESTO );
            _ADD_MEMORY_DOUBLE( __x_cobros_importe_me, _medio_moneda_extranjera, RAM_RESTO_ME );
        }
        else {
            _ADD_MEMORY_DOUBLE( __x_ventas_importe, _medio_moneda_extranjera, RAM_RESTO );
            _ADD_MEMORY_DOUBLE( __x_ventas_importe_me, _medio_moneda_extranjera, RAM_RESTO_ME );
        }
    }
    else {
        //vuelto = ( modo == _MODO_NC || ( MODO_DEVOLUCION && modo != _MODO_COBRANZA ) ) ? -RAM_VUELTO
        //       : RAM_VUELTO;
        vuelto = ( modo == _MODO_NC || MODO_DEVOLUCION ) ? -RAM_VUELTO : RAM_VUELTO;
        ADD_MEMORY_DOUBLE( __x_tot_dinero, -( ROUND( vuelto - tot_vueltos_propios
                           - tot_vueltos_medios, 10, DECIMALES ) ) );
        v = vuelto/*RAM_VUELTO*/ -fabs( tot_efectivo ) - fabs( tot_vueltos_propios )
            - fabs( tot_vueltos_medios );
        SET_MEMORY_DOUBLE( __ram_vuelto_efectivo, RAM_VUELTO - fabs( tot_vueltos_propios )
                           - fabs( tot_vueltos_medios ) );
       // if( v > 0.009 ) {
			if( vuelto ) {
            ADD_MEMORY_DOUBLE( __x_vueltos_importe, ROUND( vuelto /*v*/, 10, DECIMALES ) );
            ADD_MEMORY_INT( __x_vueltos_cantidad, 1 );
			}
        //}

        if( ( modo == _MODO_COBRANZA ) /*||
           ( modo == _MODO_REPROCESAMIENTO && event_ticket.tipo_evento == 1 )*/ ) {
            /*if( medios[m].moneda_extranjera ) {
                _ADD_MEMORY_DOUBLE( __x_cobros_importe_me, 0,
                                    -( RAM_VUELTO - tot_vueltos_propios - tot_vueltos_medios
                                     + RAM_DIFERENCIA_REDONDEO ) );
            }*/
            if( v > ( _X_VENTAS_IMPORTE( 0 ) + _X_COBROS_IMPORTE( 0 ) - X_PAGOS_POR_CAJA_IMPORTE
                - _X_RETIROS_IMPORTE( 0 ) ) ) {
                //si no hay efectivo en caja, entonces el vuelto se da del fondo fijo
                //se sabe que hay fondo fijo porque ya se verifico importe excedido
                _ADD_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, 0, v );
            }
            else {
                if( ( v < 0 ) && ( v < -_X_RETIRO_FONDO_IMPORTE_PAGOS( 0 ) ) ) {
                    _SET_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, 0, 0 );
                }
                else if( ( v < 0 ) && ( v >= -_X_RETIRO_FONDO_IMPORTE_PAGOS( 0 ) ) ) {
                    _ADD_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, 0, v );
                }
            }
            _ADD_MEMORY_DOUBLE( __x_cobros_importe, 0,
                                -( vuelto/*RAM_VUELTO*/ -tot_vueltos_propios - tot_vueltos_medios
                                 + RAM_DIFERENCIA_REDONDEO ) );
            /*      + _X_FONDO_FIJO_MULTIPLE_IMPORTE( nro ) - X_PAGOS_POR_CAJA_IMPORTE
                        - _X_RETIROS_IMPORTE( nro ) ) + 0.02 ) {
                    _ADD_MEMORY_DOUBLE( __x_retiro_fondo_importe_pagos, nro, saldo );  */
        }
        else {
            if( !( modo == _MODO_REPROCESAMIENTO
                && event_ticket.tipo_evento == _EVENTO_COBRO_ESPECIAL ) ) {
                _ADD_MEMORY_DOUBLE( __x_ventas_importe, 0,
                                    -( vuelto - tot_vueltos_propios - tot_vueltos_medios
                                     + RAM_DIFERENCIA_REDONDEO ) );
            }
        }
        /*------------ actualiza acumulador por submedio -------------*/
        importeTmp = -( vuelto - tot_vueltos_propios - tot_vueltos_medios
                + RAM_DIFERENCIA_REDONDEO );
        SUMAR_IMPORTE_SUBMEDIO( /*(efectivo)*/ 1, 0, importeTmp );
    }

    ACTUALIZA_IVAS_MULTIPLES();
    ADD_MEMORY_DOUBLE( __x_tot_dinero, RAM_TOTAL_DONACION );
    if( modo == _MODO_REPROCESAMIENTO && event_ticket.tipo_evento == _EVENTO_COBRO_ESPECIAL ) {
        SET_MEMORY_DOUBLE( __x_tot_dinero, 0 );
        SET_MEMORY_DOUBLE( __x_intereses_importe, 0 );
        SET_MEMORY_INT( __x_intereses_cantidad, 0 );
        SET_MEMORY_DOUBLE( __x_vueltos_importe, 0 );
        SET_MEMORY_INT( __x_vueltos_cantidad, 0 );
    }
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
    /* Si llego hasta aqui es porque todo salio Ok, entonces
       retorna 1 */
    return 1;
}
#ifdef COMPILAR_TECLA_REPROCESAR
/*****************************************************************************/
void PAGOS_REPROCESAR( char tipo_pago, int reproceso )
/*****************************************************************************/
{
    int h;
    int _ptr,_handle,_estado,_h;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    /*  _estado = STACK_INS( sizeof( int ) );
        _h = STACK_INS( sizeof( int ) );*/
    _estado = STACK_INS( sizeof( short ) );
    _h = STACK_INS( sizeof( short ) );
    _modo = _MODO_REPROCESAMIENTO;
    if( _ESTADO == 0 ) {
        if( reproceso ) {
            REPROCESAR_EVENTOS( RAM_P_PAGO );
            // ---------------------------------------------------------------
            // Reproceso de Pago:
            // ---------------------------------------------------------------
            // Al Reprocesar pone el evento.reprocesar en procesado si
            // no estaba enviado y en a procesar no enviado si fue enviado
            // y no controlado.
            // CONTROLAR NO ENVIADO
            // ---------------------------------------------------------------
            //SELECT( AREA_TICKET );
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
            switch( event_ticket.status ) {
                case 0:
                    event_ticket.reproceso = EVENTO_REPROCESADO;
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    UPDATE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                    break;
                case 1:
                    event_ticket.reproceso = EVENTO_REPROCESAR_NO_ENVIADO;
                    #ifdef INVEL_L
                    //BEGIN_TRANSACTION();
                    #endif
                    UPDATE2();
                    #ifdef INVEL_L
                    //END_TRANSACTION();
                    #endif
                    break;
                case 2:
                    GRABAR_LOG_DBROUTER( ST( S_INTENTO_REPROCESO_CONFIRMADO ),LOG_DEBUG,2 );
                    break;
            }
        }
        _SET_ESTADO( 1 );
    }
    if( _ESTADO == 1 ) {
        REPROCESAR_EFECTIVO( tipo_pago );
        _SET_ESTADO( 2 );
    }
    if( _ESTADO == 2 ) {
        REPROCESAR_CTA_CTE( tipo_pago );
        _SET_ESTADO( 3 );
    }
    if( _ESTADO == 3 ) {
        REPROCESAR_CHEQUES( tipo_pago );
        _SET_ESTADO( 4 );
    }
    if( _ESTADO == 4 ) {
        #ifdef COMPILAR_TECLA_REPROCESAR
        REPROCESAR_TARJETAS( tipo_pago, SI );
        #endif
        _SET_ESTADO( 5 );
    }
    if( _ESTADO == 5 ) {
        REPROCESAR_TICKETS_DE_COMPRA( tipo_pago );
        _SET_ESTADO( 6 );
    }
    if( _ESTADO == 6 ) {
        #ifdef COMPILAR_MUTUALES
        REPROCESAR_MUTUALES( tipo_pago );
        #endif
        _SET_ESTADO( 7 );
    }
    if( _ESTADO == 7 ) {
        /*------------------ Reprocesa otros medios -------------------*/
        for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO;ADD_MEMORY_INT( _h, 1 ) ) {
            h = GET_MEMORY_INT( _h );
            if( !MEDIO_DE_PAGO_ESPECIAL( _PAGO_MODO( h ) ) ) {
                if( tipo_pago == 1 ) {
                    _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE( h ) ) );
                    _ADD_MEMORY_DOUBLE( __x_cobros_importe_me, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE_ME( h ) ) );
                    _ADD_MEMORY_INT( __x_cobros_cantidad, _PAGO_MODO( h ), -1 );
                    if( _PAGO_VUELTO_MEDIO( h ) ) {
                        _ADD_MEMORY_DOUBLE( __x_cobros_importe, _PAGO_COD_MEDIO_VUELTO( h ) - 1,
                                            _PAGO_VUELTO_MEDIO( h ) );
                    }
                }
                else {
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE( h ) ) );
                    _ADD_MEMORY_DOUBLE( __x_ventas_importe_me, _PAGO_MODO( h ),
                                        -( _PAGO_IMPORTE_ME( h ) ) );
                    _ADD_MEMORY_INT( __x_ventas_cantidad, _PAGO_MODO( h ), -1 );
                    if( _PAGO_VUELTO_MEDIO( h ) ) {
                        _ADD_MEMORY_DOUBLE( __x_ventas_importe, _PAGO_COD_MEDIO_VUELTO( h ) - 1,
                                            _PAGO_VUELTO_MEDIO( h ) );
                    }
                }
            }
        }
        SET_MEMORY_INT( _h, 0 );
        if( _medio_moneda_extranjera >= 0 ) {
            if( MODO_NEGOCIO == SERVICIOS ) {
                _ADD_MEMORY_DOUBLE( __x_cobros_importe_me, _medio_moneda_extranjera,
                                    -RAM_RESTO_ME );
            }
            else {
                _ADD_MEMORY_DOUBLE( __x_ventas_importe_me, _medio_moneda_extranjera,
                                    -RAM_RESTO_ME );
            }
        }
        _SET_ESTADO( 8 );
    }
    if( _ESTADO == 8 ) {
        if( reproceso ) {
            if( event_ticket.tipo_evento == 0 ) {
                if( config.pais != COLOMBIA ) {
                    ADD_MEMORY_LONG( __nro_ticket, -1L );
                }
                ADD_MEMORY_LONG( __x_cantidad_tickets, -1L );
            }
            IMPRIME_REPROCESAMIENTO();
        }
        _SET_ESTADO( 9 );
    }
    if( _ESTADO == 9 ) {
        if( reproceso ) {
            PAGOS_ELIMINA_ITEM_INTERESES();
        }
        _SET_ESTADO( 10 );
    }
    /*------------------------ Reprocesa variaciones -------------------------*/
    if( _ESTADO == 10 ) {
        if( reproceso ) {
            for( ;GET_MEMORY_INT( _h ) < RAM_P_PAGO;ADD_MEMORY_INT( _h, 1 ) ) {
                h = GET_MEMORY_INT( _h );
                if( _PAGO_VARIACION( h ) ) {
                    PAGOS_PREPARA_ITEM( h, 1 );
                }
            }
        }
        _SET_ESTADO( 11 );
    }
    if( _ESTADO == 11 ) {
        if( reproceso ) {
            ACTUALIZA_VENTAS();
        }
        _SET_ESTADO( 12 );
    }
    SET_MEMORY_INT( __ram_p_ticket, 0 );
    SET_MEMORY_INT( __ram_variacion, 0 );
    /*--------------------- Fin de recupero de memoria ----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}
#endif
/*****************************************************************************/
double CALCULA_VARIACION( double importe, int modo, int submedio )
/*****************************************************************************/
{
    double variacion = 0.0,variacion_aux = 0.0, ret_var = 0.0, iva2 = 0.0;

	if( submedio > 0 )
		variacion_aux = VARIACION_DE_SUBMEDIOS( modo , submedio );
	
	if( variacion_aux == 0.0 )
		variacion_aux = medios[modo].variacion;

    if( (medios[modo].variacion || variacion_aux )&& _modo != _MODO_COBRANZA ) {
        if( importe > RAM_RESTO ) {
            importe = RAM_RESTO;
        }
		if( !medios[modo].tarjeta ) {
            if( medios[modo].recargo_opcional == 's' || medios[modo].recargo_opcional == 'S' ) {
                char tecla = 0;
                MENSAJE_STRING( S_PRESIONE_ENTER_PARA_RECARGO );
                while( tecla != 13 && tecla != 27 ) {
                    tecla = GETCH();
                }
                BORRAR_MENSAJE();
                if( tecla == 13 ) {
                    variacion = ROUND( importe * variacion_aux/*medios[modo].variacion*/ / 100, 10, DECIMALES );
                }
            }
            else {
                variacion = ROUND( importe * variacion_aux/*medios[modo].variacion*/ / 100, 10, DECIMALES );
            }
        }
        else {
            variacion = _VARIACION_TARJETA( importe,variacion_aux/*medios[modo].variacion*/ , &ret_var, &iva2,
                                            medios[modo].art_variacion, NO );
        }
    }
    return ( variacion );
}
/*****************************************************************************/
double CALCULA_VARIACION_TARJETA( double importe, int modo, int modificar )
/*****************************************************************************/
{
    double variacion = 0.0, ret_var  = 0.0, iva2  = 0.0;

    if( _modo != _MODO_COBRANZA ) {
        if( importe > RAM_RESTO && modificar == NO) {
            importe = RAM_RESTO;
        }

        variacion = _VARIACION_TARJETA( importe,
                                        VAR_TARJETAS_RECARGO + VAR_TARJETAS_RECARGO_DIFERIM,
                                        &ret_var, &iva2, medios[modo].art_variacion, modificar );
    }
    return ( variacion );
}
/*****************************************************************************/
double _VARIACION_TARJETA( double importe, float porc, double *ret_var, double *iva2,
                           long art_variacion, int modificar )
/*****************************************************************************/
{
    double variacion = 0.00, variacion_neta = 0.00, imp_sin_ret = 0.00, ret_int = 0.00;
	int tabla_ant = 0, tipo_ant = 0;

    if( RAM_INTERESES ) {
        ret_int = RAM_PORC_INTERES / 100 * ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI );
    }
    imp_sin_ret = importe - ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI + ret_int )
                / ( RAM_IMPORTE_TOTAL + RAM_RETENCION_DGR + RAM_PERCEPCION_DGI ) * importe;


	#ifdef VARIACION_FINANCIERA_SIN_RETENCION
		//Se aplica variacion sobre el total del ticket incluyendo percepcion y retencion
		if( IMPRESORA_FISCAL == FISCAL && (RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1))
			variacion = importe * porc / 100;
		else
			variacion = ROUND( importe * porc / 100, 10, DECIMALES );
	#else
		if( IMPRESORA_FISCAL == FISCAL && (RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1))
			variacion = imp_sin_ret * porc / 100;
		else
			variacion = ROUND( imp_sin_ret * porc / 100, 10, DECIMALES );

	#endif


    // busqueda del articulo para saber que codigo de iva  tiene un medio de
    // pago
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    BUSCA_CODIGO_ESPECIAL( art_variacion );

	if( fabs( variacion ) >= 0.001 ){
	SELECT_TABLE( tabla_ant, tipo_ant );
	    
    // variacion_neta = ROUND( variacion / ( 1 + config.porc_iva1 / 100 ),
    // 10, DECIMALES );
    variacion_neta = ROUND( variacion / ( 1 + iva[articulo.cod_iva].porc_iva1 / 100 ), 10,
                            DECIMALES );

		//HMM vamos repartiendo la variacion revisar esto mas adelante como lo tiene libertad
		/*porc_desc = -variacion / ( TOT_TICK_SV_IMPORTE_SIN_IVA + TOT_TICK_SV_EXENTO + TOT_TICK_SV_IVA1 + TOT_TICK_SV_IVA2 ) * 100.0;
		acumulados = ACUMULAR_IMPUESTOS_TICKET( NO, 0, -1, -1, NO );
		aux.exento = ROUND( acumulados->exento * porc_desc, ENTEROS, DECIMALES );
		aux.importe_sin_iva = acu->importe_sin_iva * porc;*/
		//
	#ifdef VARIACION_FINANCIERA_SIN_RETENCION
		//Tarea 4982: Planeta no le aplica retenciones a los recargos financieros
		//*ret_var = ( RETENCION( variacion_neta, SI  ) - RETENCION( 0.0, SI ) )
					//+ ( PERCEPCION( variacion_neta, SI ) - PERCEPCION( 0.0, SI ) );
	#else
		*ret_var = ( RETENCION( variacion_neta, SI  ) - RETENCION( 0.0, SI ) )
					+ ( PERCEPCION( variacion_neta, SI ) - PERCEPCION( 0.0, SI ) );
	#endif

    variacion += *ret_var;
    if( EMITIR_FACTURA() && ( clientes.cond_iva == RESP_NO_INSC ) ) {
        *iva2 = variacion_neta * iva[articulo.cod_iva].porc_iva2 / 100.0;
        variacion += *iva2;
    }
    else {
        *iva2 = 0;
    }
	}
	if( IMPRESORA_FISCAL == FISCAL && variacion > 0.01 && modificar == SI) {
		double tatalim= 0.00, aux = 0.00, dif = 0.00;
		if( tomar_variacion == SI)
	        variacion_printer = variacion;
		else
			variacion_printer = 0.0;
		tatalim = RETORNAR_TOTAL_PRINTER( variacion );//Esto te da el total del ticket y necesito comprar con el total del ticket
		
		#ifdef VARIACION_FINANCIERA_SIN_RETENCION
			aux = RAM_TOTAL + variacion;
			dif= fabs(tatalim)  - fabs(aux);
			//Tarea 4982, dif 0.009999
			if( fabs( dif ) > 0.01 && fabs( dif) < 0.02 )
				variacion+= dif;
		#else
			aux = importe + variacion;
			dif= fabs(tatalim)  - fabs(aux);
			if( fabs( dif ) >0.0045 && fabs( dif) < 0.02 )
				variacion+= dif;
		#endif
	}

    return ( variacion );  
}
/*****************************************************************************/
void PAGOS_AGREGAR_VARIACIONES()
/*****************************************************************************/
{
    int h,posicion =  0;
    for( h = 0;h < RAM_P_PAGO;h++ ) {
		if( _PAGO_VARIACION( h ) ) {
			if( medios[_PAGO_MODO( h )].tarjeta &&VARIAS_TARJETAS_POR_PAGO){
				LEE_TARJETAS( posicion, TARJETA_TRADICIONAL );
			}
            
			PAGOS_PREPARA_ITEM( h, 0 );

			if( medios[_PAGO_MODO( h )].tarjeta &&VARIAS_TARJETAS_POR_PAGO){
				CEREAR_STRUCT( _var_tarjetas );
			}

        }
		if( medios[_PAGO_MODO( h )].tarjeta ){
				posicion++;
		}

    }
}
/*****************************************************************************/
void PAGOS_PREPARA_ITEM( int h, int anulacion )
/*****************************************************************************/
{
	double porc, variacion = 0, ret_pago, ret_var = 0, total, importe, ret_int =
			0, iva2 = 0;
	double total2[10];
	int gravado = 0, salir = 0;
    int encontro;
    char arti[100];
	int hay_exento = NO;
	double falta_acumulado, total_exento = 0.0;

    encontro = ASIGNA_CODIGO_ESPECIAL( medios[_PAGO_MODO( h )].art_variacion );
	if( !encontro ) {
        sprintf( arti, ST( S_ARTICULO_VARIACION_NO_ENCONTRADO____LD ),
                 medios[_PAGO_MODO( h )].art_variacion );
        MENSAJE( arti );
        GRABAR_LOG_SISTEMA( arti , LOG_VENTAS,1 );
    }

    {
        char nombre_ant[31];
        char base_nombre[24];
		memset(nombre_ant,0, 31);
		memset(base_nombre,0, 24);
        strcpy( nombre_ant, ITEM_NOMBRE_LARGO );
        sprintf( base_nombre, ST( S___BASE___2LF_ ), _PAGO_IMPORTE( h ) - _PAGO_VARIACION( h ) );
        TRIM( nombre_ant );
        if( ( strlen( nombre_ant ) + strlen( base_nombre ) ) < 31 ) {
            strcat( nombre_ant, base_nombre );
        }
        else {
            strcpy( base_nombre, nombre_ant + ( 31 - strlen( base_nombre ) + 1 ) );
        }
        SET_MEMORY( __item_nombre_largo, nombre_ant );
    }
    if( !medios[_PAGO_MODO( h )].tarjeta ) {
        total = RAM_IMPORTE_TOTAL + RAM_RETENCION_DGR + RAM_PERCEPCION_DGI + RAM_PERCEPCION_212_IVA1
              + RAM_PERCEPCION_212_IVA2;
        if( RAM_INTERESES ) {
            ret_int = RAM_PORC_INTERES / 100 * ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI );
        }
        ret_pago = ROUND( ( _PAGO_IMPORTE( h ) - _PAGO_VARIACION( h ) )
                        / total * ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI + RAM_PERCEPCION_212_IVA1
                                  + RAM_PERCEPCION_212_IVA2 + ret_int ), 10, DECIMALES );
        ret_var = ROUND( _PAGO_VARIACION( h ) / ( _PAGO_IMPORTE( h ) - _PAGO_VARIACION( h ) )
                       * ret_pago, 10, DECIMALES );
    } else {
        importe = _PAGO_IMPORTE( h ) - _PAGO_VARIACION( h );
        _VARIACION_TARJETA( importe,
                            VAR_TARJETAS_RECARGO + VAR_TARJETAS_RECARGO_DIFERIM
                          + medios[_PAGO_MODO( h )].variacion, &ret_var, &iva2,
                            medios[_PAGO_MODO( h )].art_variacion, NO );
        gravado = 1;
    }
    if( medios[_PAGO_MODO( h )].tarjeta)
		variacion = _PAGO_VARIACION( h ) ; //parche hasta que se haga el reparto de la percepcion en la variacion
	else
		variacion = _PAGO_VARIACION( h ) - ret_var - iva2;

    porc = ROUND(  variacion / RAM_IMPORTE_TOTAL, ENTEROS, DECIMALES + 1);
    if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
        variacion *= -1;
    }
	{ // veamos si con solo un descuento puedo cubrir por codigo de iva
		double totalxiva= 0.00;
		double vari = fabs(variacion);
		double total_ticket = 0.00;
		total_exento = fabs(TOT_TICK_EXENTO);

		if( articulo.gravado != 'S' && articulo.gravado != 's' ) {
			// es exento
			totalxiva = TOT_TICK_EXENTO ;
			hay_exento = SI; // articulo de descarga es exento

		}else
			totalxiva = _TOT_TICK_TOTAL_POR_COD_IVA( articulo.cod_iva )  - _TOT_TICK_IMP_INT_POR_COD_IVA( articulo.cod_iva );

        falta_acumulado = totalxiva - vari; // si es negativo no alcanza

		if( falta_acumulado < 0.0 ) {  // no alcanza el iva.

			//veamos primero si hay alguno acumulador por cod_iva que si cubra
			int ii, iva = 0;
			salir = -1;
			for(ii=0;ii< 10 && salir < 1; ii++){
				total2[ii] = _TOT_TICK_TOTAL_POR_COD_IVA( ii )  - _TOT_TICK_IMP_INT_POR_COD_IVA( ii );
				total_ticket += total2[ii];
				if(fabs(total2[ii]) >  vari ) {
					salir = 1;
					iva = ii;
				}
			}
			if( salir == 1 ) {
				SET_MEMORY_CHAR( __item_cod_iva, iva );
				SET_MEMORY_CHAR( __item_gravado, SI );//Fuerza el descarga a GRAVADO
			} else
				if(total_ticket > vari) // si el total del ticket es mayor que variacion prorrateamos
					salir = 0;

		} else
			salir = 1;
	}
	if( salir == 0 && porc < 0.01) { // prorratear el descuento
		int ivass = 0;
		while (/*cantidad_ivas > 0 &&*/ ivass < 10) {
			if ( total2[ivass]> 0.01 ) {
				variacion = total2[ivass] * porc;
				SET_MEMORY_CHAR( __item_cod_iva, ivass );
				SET_MEMORY_CHAR( __item_gravado, SI );//Fuerza el descarga a GRAVADO
				_PAGOS_PREPARA_ITEM(variacion, porc, anulacion, gravado,
								_ORIGEN_VARIACION_MEDIO, _PAGO_MODO( h ));
				//cantidad_ivas--;
			}
			ivass++;
		}
		if(total_exento > 0.01) {
			variacion = TOT_TICK_EXENTO  * porc;
			SET_MEMORY_CHAR( __item_gravado, NO );//Fuerza el descarga a exento
			SET_MEMORY_CHAR( __item_cod_iva, 0 );
				_PAGOS_PREPARA_ITEM(variacion, porc, anulacion, NO,
										_ORIGEN_VARIACION_MEDIO, _PAGO_MODO( h ));
		}

	} else {
		if( salir != -1 ) {
			if (hay_exento == SI && falta_acumulado> 0.0)
				gravado = NO;
			else
				gravado = SI;
			if (fabs(variacion) > 0.01)
				_PAGOS_PREPARA_ITEM(variacion, porc, anulacion, gravado,
						_ORIGEN_VARIACION_MEDIO, _PAGO_MODO( h ));
		}else {
			char mensa[150];
			_snprintf( mensa, sizeof( mensa ) -1, "%s ( Presione una tecla )", ST(S_NO_SE_PUEDE_APLICAR_EL_DESCUENTO) );
			mensa[ sizeof( mensa ) - 1]= '/0';
			MENSAJE_TECLA( mensa );
			 memset(mensa,0,sizeof(mensa));
			_snprintf( mensa, sizeof( mensa ) -1, "Descuento invalido %lf ", variacion );
			GRABAR_LOG_SISTEMA( mensa,LOG_ERRORES,1 );
			_ADD_MEMORY_DOUBLE( __pago_variacion, h, -variacion ); //revertimos la variacion en pagos
			_ADD_MEMORY_DOUBLE( __pago_importe, h, -variacion) ;


		}
	}

}
/*****************************************************************************/
void PAGOS_AGREGA_ITEM_INTERESES()
/*****************************************************************************/
{
    if( RAM_INTERESES ) {
        ASIGNA_CODIGO_ESPECIAL( config.cod_recargo_intereses );
        _PAGOS_PREPARA_ITEM( RAM_INTERESES, RAM_PORC_INTERES / 100, 0, NO, _ORIGEN_INTERES, 0 );
    }
}
/*****************************************************************************/
void PAGOS_ELIMINA_ITEM_INTERESES()
/*****************************************************************************/
{
    if( RAM_INTERESES ) {
        ASIGNA_CODIGO_ESPECIAL( config.cod_recargo_intereses );
        _PAGOS_PREPARA_ITEM( RAM_INTERESES, RAM_PORC_INTERES / 100, 1, NO, _ORIGEN_INTERES, 0 );
    }
}
/*****************************************************************************/
void _PAGOS_PREPARA_ITEM( double importe, double porc, int anulacion, int gravado, int origen,
                          int tipo )
/*****************************************************************************/
{
    double tot,neto, aux_importe; 
	int _iva =0;
	aux_importe= importe;
    SET_MEMORY_DOUBLE( __item_precio, importe );
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    SET_MEMORY_INT( __ram_anulacion, ( anulacion ) ? 1 : 0 );
    AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, anulacion, NO, NO, origen );
    _SET_MEMORY_DOUBLE( __ticket_precio, RAM_P_TICKET, ROUND( importe, 10, DECIMALES ) );
    if( gravado ) {
		if( RAM_P_PAGO == 1 && IMPRESORA_FISCAL == FISCAL && origen == 5 && porc > 0.01) { //DEBENOS CALCULAR EL RECARGO DE NUEVO
			neto = RETORNAR_TOTAL_PRINTER( 0.00 );
			tomar_variacion = SI;
			#ifdef VARIACION_FINANCIERA_SIN_RETENCION
				//Tarea 4982: Planeta no le aplica retenciones a los recargos financieros
				neto = neto + ( RAM_RETENCION_DGR + RAM_PERCEPCION_DGI );
			#endif
			importe = CALCULA_VARIACION_TARJETA( neto, 1, SI ); //45.50///neto
			aux_importe = variacion_printer;
			tomar_variacion = NO;
			_SET_MEMORY_DOUBLE( __ticket_precio, RAM_P_TICKET, ROUND( importe, 10, DECIMALES ) );
		}
		/**/
		_iva = RECUPERAR_ALICUOTA_DE_ARTICULO(NO,_TICKET_CANT_EXCL_DESC(RAM_P_TICKET),_TICKET_COD_BARRA( RAM_P_TICKET ), _TICKET_COD_ESPECIAL( RAM_P_TICKET ) );
		if( _iva != -1 ) {
					//Recupero el iva original de padron
			SET_MEMORY_CHAR( __item_cod_iva, _iva );
		}

		/* ***************/

        neto = importe / ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
        SET_MEMORY_DOUBLE( __item_precio_sin_iva, neto );
        _SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET, neto );
        _SET_MEMORY_DOUBLE( __ticket_iva1_precio, RAM_P_TICKET, importe - neto );
        _SET_MEMORY_DOUBLE( __ticket_iva2_precio, RAM_P_TICKET, 0 );
        _SET_MEMORY_DOUBLE( __ticket_exento, RAM_P_TICKET, 0 );
        //_SET_MEMORY_FLOAT( __ticket_ila_precio, RAM_P_TICKET, 0 );
        //_SET_MEMORY_FLOAT( __ticket_iha_precio, RAM_P_TICKET, 0 );
        //_SET_MEMORY_FLOAT( __ticket_alicuota_precio, RAM_P_TICKET, 0 );
        _SET_MEMORY_FLOAT( __ticket_impuesto1_precio, RAM_P_TICKET, 0 );
        _SET_MEMORY_FLOAT( __ticket_impuesto2_precio, RAM_P_TICKET, 0 );
        _SET_MEMORY_FLOAT( __ticket_impuesto3_precio, RAM_P_TICKET, 0 );
        _SET_MEMORY_FLOAT( __ticket_impuesto4_precio, RAM_P_TICKET, 0 );
        _SET_MEMORY_DOUBLE( __ticket_imp_int, RAM_P_TICKET, 0 );

        if( EMITIR_FACTURA()
         && ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva == RESP_NO_INSC ) ) {
            _SET_MEMORY_DOUBLE( __ticket_iva2_precio, RAM_P_TICKET,
                                neto * iva[INDICE_IVA_ARTICULO()].porc_iva2 / 100.0 );
        }
    }
	else {
		//El porcentaje se saca sin impuesto interno
        SET_MEMORY_DOUBLE( __item_precio_sin_iva, porc * ( TOT_TICK_SV_IMPORTE_SIN_IVA - TOT_TICK_SV_IMP_INT ) );
		_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET,
                            porc * ( TOT_TICK_SV_IMPORTE_SIN_IVA - TOT_TICK_SV_IMP_INT ) );
		_SET_MEMORY_DOUBLE( __ticket_imp_int, RAM_P_TICKET, porc * TOT_TICK_SV_IMP_INT );
		SET_MEMORY_DOUBLE( __item_imp_int, porc * TOT_TICK_SV_IMP_INT );
		//El item_precio_sin_iva y ticket_importe_sin_iva incluye impuesto interno (osea el %)
		SET_MEMORY_DOUBLE( __item_precio_sin_iva, 
			( ITEM_PRECIO_SIN_IVA + ITEM_IMP_INT ) );
		_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva,
			RAM_P_TICKET, ( ITEM_PRECIO_SIN_IVA + ITEM_IMP_INT ) );
        _SET_MEMORY_DOUBLE( __ticket_exento, RAM_P_TICKET, porc * TOT_TICK_SV_EXENTO );
        _SET_MEMORY_DOUBLE( __ticket_iva1_precio, RAM_P_TICKET, porc * TOT_TICK_SV_IVA1 );
        _SET_MEMORY_DOUBLE( __ticket_iva2_precio, RAM_P_TICKET, porc * TOT_TICK_SV_IVA2 );
        //      _SET_MEMORY_FLOAT( __ticket_ila_precio, RAM_P_TICKET, porc * TOT_TICK_SV_ILA );
        //      _SET_MEMORY_FLOAT( __ticket_iha_precio, RAM_P_TICKET, porc * TOT_TICK_SV_IHA );
        //      _SET_MEMORY_FLOAT( __ticket_alicuota_precio, RAM_P_TICKET,
        //          porc * TOT_TICK_SV_ALICUOTA );
        _SET_MEMORY_FLOAT( __ticket_impuesto1_precio, RAM_P_TICKET,
                           porc * _TOT_TICK_SV_IMPUESTOS( 0 ) );
        _SET_MEMORY_FLOAT( __ticket_impuesto2_precio, RAM_P_TICKET,
                           porc * _TOT_TICK_SV_IMPUESTOS( 1 ) );
        _SET_MEMORY_FLOAT( __ticket_impuesto3_precio, RAM_P_TICKET,
                           porc * _TOT_TICK_SV_IMPUESTOS( 2 ) );
        _SET_MEMORY_FLOAT( __ticket_impuesto4_precio, RAM_P_TICKET,
                           porc * _TOT_TICK_SV_IMPUESTOS( 3 ) );
        DISCRIMINA_IMPUESTOS_EN_ARTICULO( porc, NULL );
    }

    _SET_MEMORY_DOUBLE( __ticket_recargo, RAM_P_TICKET, 0 );
    _SET_MEMORY_DOUBLE( __ticket_iva1_rec, RAM_P_TICKET, 0 );
    _SET_MEMORY_DOUBLE( __ticket_iva2_rec, RAM_P_TICKET, 0 );
    // _SET_MEMORY_FLOAT( __ticket_ila_rec, RAM_P_TICKET, 0 );
    // _SET_MEMORY_FLOAT( __ticket_iha_rec, RAM_P_TICKET, 0 );
    /*----------- Agrega a gravado la diferencia por redondeo ------------*/
    tot = _TICKET_IMPORTE_SIN_IVA( RAM_P_TICKET ) + _TICKET_EXENTO( RAM_P_TICKET )
        + _TICKET_IVA1_PRECIO( RAM_P_TICKET ) + _TICKET_IVA2_PRECIO( RAM_P_TICKET );
    //  tot += _TICKET_ILA_PRECIO( RAM_P_TICKET ) + _TICKET_IHA_PRECIO( RAM_P_TICKET ) +
    //      _TICKET_ALICUOTA_PRECIO( RAM_P_TICKET );
    tot += _TICKET_IMPUESTO1_PRECIO( RAM_P_TICKET ) + _TICKET_IMPUESTO2_PRECIO( RAM_P_TICKET )
         + _TICKET_IMPUESTO3_PRECIO( RAM_P_TICKET ) + _TICKET_IMPUESTO4_PRECIO( RAM_P_TICKET );

    _ADD_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET,
                        _TICKET_PRECIO( RAM_P_TICKET ) - tot );

    ADD_MEMORY_INT( __ram_p_ticket, 1 );

    if( _modo != _MODO_REPROCESAMIENTO ) {
        SET_MEMORY_DOUBLE( __ram_total,
                           ROUND( RAM_TOTAL + _TICKET_PRECIO( RAM_P_TICKET - 1 ), 10, DECIMALES ) );
    }
    if( _modo != _MODO_REPROCESAMIENTO ) {
		double guarda_psiva = ITEM_PRECIO_SIN_IVA; 
		neto = aux_importe / ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
		variacion_printer = 0.0;
        SET_MEMORY_DOUBLE( __item_precio_sin_iva, neto );

        /*_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET - 1, neto );
        _SET_MEMORY_DOUBLE( __ticket_iva1_precio, RAM_P_TICKET - 1, importe - neto );
        _SET_MEMORY_DOUBLE( __ticket_iva2_precio, RAM_P_TICKET - 1, 0 );
        _SET_MEMORY_DOUBLE( __ticket_exento, RAM_P_TICKET - 1, 0 );*/

		if( DEBE_IMPRIMIR_ITEM(  ) ) {
			IMPRIME_ITEM( RAM_P_TICKET - 1 );
		}
		SET_MEMORY_DOUBLE( __item_precio_sin_iva, guarda_psiva );
		_SET_MEMORY_DOUBLE( __ticket_descuento, RAM_P_TICKET - 1, neto ); //gurado el importe que se mando a la printer realmente
		//_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, RAM_P_TICKET, neto );
		//IMPRIME_SUBTOTAL( PAGOS_TOTAL_A_PAGAR() - importe, NO );
        IMPRIME_INTERESES( importe, porc * 100 );   
    }
    ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
    GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, SI, ( char )origen, tipo, NO );
    GRABAR_TICKET( RAM_P_TICKET - 1 );
}
/*****************************************************************************/
int VALIDAR_PAGO( int modo )
/*****************************************************************************/
{
    int ok = 1, i, pago_con_tarjeta = 0, h,/* salir,*/ k, j, hay_efectivo;
    double acu_vuelto = 0,
		   acum_me = 0,
           acu_modo[10],
           porc,
           acu_sin_tarjeta = 0, acu_contado = 0, acu_vuelto_medio = 0, acu_importe_medio = 0;
    double tt, vuelto_efectivo = 0, vuelto, acu_credito = 0, vuelto_me = 0;
    char /*controlar_medio,*/mensaje[80],aux[15],submedio_ok = 0;
    double tot_cheques = 0;
    /*---------------------- Procesa los datos del pago ----------------------*/
    memset( acu_modo, 0, sizeof( acu_modo ) );
    vuelto_efectivo = -RAM_RESTO;
    for( i = 0;i < RAM_P_PAGO;i++ ) {
        if( medios[_PAGO_MODO( i )].cheque ) {
            tot_cheques += _PAGO_IMPORTE( i );
        }
        if( medios[_PAGO_MODO( i )].tarjeta ) {
            pago_con_tarjeta = 1;
        }
        else {
            acu_sin_tarjeta += _PAGO_IMPORTE( i );
        }
        //if (medios[_PAGO_MODO(i)].vuelto == _VUELTO_EN_EFECTIVO) {
		//En la importacion de un comprobante, la variable _PAGO_TIPO_VUELTO( i ) no se graba en la pago.btr
		//para recuperarse su valor, de manera que verificaba la condicion medios[_PAGO_COD_MEDIO_VUELTO( i ) - 1].efectivo ) y _PAGO_COD_MEDIO_VUELTO( i )
		//era igua la 0 por lo que producia un error ya que validaba medios[-1]
		


		//_VUELTO_EN_EFECTIVO
        if( medios[_PAGO_MODO( i )].vuelto == _VUELTO_EN_EFECTIVO
				|| ( _PAGO_COD_MEDIO_VUELTO( i ) && medios[_PAGO_COD_MEDIO_VUELTO( i ) - 1].efectivo ) ) {
            double porc_vuelto, maximo_vuelto;
			if( medios[_PAGO_MODO( i )].vuelto == _VUELTO_OTRO_MEDIO ) {

				porc_vuelto = GET_PORC_FROM_VUELTOS_SIC( medios[_PAGO_MODO( RAM_P_PAGO - 1 )].nro,
					( medios[_PAGO_MODO( i )].tarjeta ) ? _PAGO_TARJETA( RAM_P_PAGO - 1 ) + 1
					: _PAGO_CUENTA( RAM_P_PAGO - 1 ), _PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ),
					_PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ) );
				maximo_vuelto = GET_MAX_VUELTO_FROM_VUELTOS_SIC( medios[_PAGO_MODO( RAM_P_PAGO - 1 )].nro,
					( medios[_PAGO_MODO( i )].tarjeta ) ? _PAGO_TARJETA( RAM_P_PAGO - 1 ) + 1
					: _PAGO_CUENTA( RAM_P_PAGO - 1 ),_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ),
					_PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ) );
		    } else {
				if(_PAGO_COD_SUBMEDIO_VUELTO( i )){
					maximo_vuelto = MAXIMO_VUELTO_PERMITIDO( _PAGO_MODO( i ), i );
					porc_vuelto = PORCENTAJE_VUELTO_PERMITIDO( _PAGO_MODO( i ), i );
				}else{
					porc_vuelto = medios[_PAGO_MODO( i )].porc_vuelto;
					maximo_vuelto = medios[_PAGO_MODO( i )].vuelto_maximo;
				}
			}
			acu_vuelto += ROUND( _PAGO_IMPORTE( i ) * porc_vuelto / 100, 10,
                                 DECIMALES );
			if( maximo_vuelto > 0.001 && acu_vuelto > maximo_vuelto ) {
				acu_vuelto = maximo_vuelto;
			}
			acu_importe_medio += _PAGO_IMPORTE( i );
		}//_VUELTO_PROPIO
		else if( medios[_PAGO_MODO( i )].vuelto == _VUELTO_PROPIO ) {
            vuelto = _PAGO_VUELTO( i );
            vuelto_efectivo -= vuelto;
            acu_vuelto += vuelto
                        + ROUND( ( _PAGO_IMPORTE( i ) - vuelto )
                               * medios[_PAGO_MODO( i )].porc_vuelto / 100, 10, DECIMALES );
			acu_importe_medio += _PAGO_IMPORTE( i );
        }//_VUELTO_OTRO_MEDIO
		else if( medios[_PAGO_MODO( i )].vuelto == _VUELTO_OTRO_MEDIO ) {
            double porc_vuelto, maximo_vuelto;
			vuelto = _PAGO_VUELTO_MEDIO( i );
            vuelto_efectivo -= vuelto;
            acu_vuelto_medio += vuelto;

			porc_vuelto = GET_PORC_FROM_VUELTOS_SIC( medios[_PAGO_MODO( RAM_P_PAGO - 1 )].nro,
				( medios[_PAGO_MODO( i )].tarjeta ) ? _PAGO_TARJETA( RAM_P_PAGO - 1 ) + 1
				: _PAGO_CUENTA( RAM_P_PAGO - 1 ),_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ),
				_PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ) );

			maximo_vuelto = GET_MAX_VUELTO_FROM_VUELTOS_SIC( medios[_PAGO_MODO( RAM_P_PAGO - 1 )].nro,
				( medios[_PAGO_MODO( i )].tarjeta ) ? _PAGO_TARJETA( RAM_P_PAGO - 1 ) + 1
				: _PAGO_CUENTA( RAM_P_PAGO - 1 ),_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ),
				_PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ) );
			acu_vuelto += ROUND( ( vuelto + acu_vuelto ) * porc_vuelto / 100, 10,
                                 DECIMALES );
			if( maximo_vuelto > 0.001 && acu_vuelto > maximo_vuelto ) {
				acu_vuelto = maximo_vuelto;
			}
            acu_importe_medio += _PAGO_IMPORTE( i );
		} else {
			acu_importe_medio += _PAGO_IMPORTE( i );
		}
        if( medios[_PAGO_MODO( i )].credito ) {
            acu_credito += _PAGO_IMPORTE( i );
        }
        else {
            acu_contado += _PAGO_IMPORTE( i );
        }
        acu_modo[_PAGO_MODO( i )] += _PAGO_IMPORTE( i );
    }
    if( DONACION_DE_VUELTO && vuelto_efectivo > 0.005) {
        SET_MEMORY_DOUBLE( __ram_total_donacion, vuelto_efectivo );
    }

    /*------------------ Valida que se haya pagado todo ----------------------*/
    if( RAM_RESTO > 0.009 ) {
        ok = 0;
    }
    if( ok && tot_cheques > 0 && !( MODO_DEVOLUCION || RAM_NOTA_CR ) ) {
        ok = EXISTEN_CHEQUES_CLIENTE();
        if( ok ) {
            ok = VERIFICA_SALDO_CHEQUE( tot_cheques );
        }
    }
    /*------------ Valida que haya un pago en tarjeta si se la pas ----------*/
    if( ok && VALIDAR_QUE_PAGUE_CON_TARJETA && VAR_TARJETAS_STATUS && !pago_con_tarjeta ) {
        MENSAJE_STRING( S_DEBE_HABER_PAGO_TARJETA );
        ok = 0;
    }
    /*-------------------- Valida la entrega con tarjeta ---------------------*/
    if( ok && VARIAS_TARJETAS_POR_PAGO && VARIOS_TARJETAS_POR_PAGO > 1 ) {
        for( k = 0, ok = 1;ok && k < VARIOS_TARJETAS_POR_PAGO;k++ ) {
            ok = LEE_TARJETAS( k, TARJETA_TRADICIONAL );
            if( ok && pago_con_tarjeta && !VAR_TARJETAS_PIDE_ENTREGA
             && VAR_TARJETAS_IMPORTE_ENTREGA > 0.009
             && acu_sin_tarjeta < VAR_TARJETAS_IMPORTE_ENTREGA ) {
                MENSAJE_STRING( S_PAGO_NO_ALCANZA_ENTREGA_TARJETA );
                ok = 0;
            }
        }
    }
    else {
        if( ok && pago_con_tarjeta && !VAR_TARJETAS_PIDE_ENTREGA
         && VAR_TARJETAS_IMPORTE_ENTREGA > 0.009 && acu_sin_tarjeta < VAR_TARJETAS_IMPORTE_ENTREGA ) {
            MENSAJE_STRING( S_PAGO_NO_ALCANZA_ENTREGA_TARJETA );
            ok = 0;
        }
    }
    /*--------------- Validacion vuelto en otro medio -----------------------*/
	/*if( ok && acu_vuelto_medio > 0.005
     && ( acu_importe_medio - PAGOS_TOTAL( 1 ) ) < acu_vuelto_medio ) {
        MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
        ok = 0;
    }*/
    /*----------- Valida que no pague de mas salvo que haya vuelto -----------*/
    SET_MEMORY_DOUBLE( __ram_resto, ROUND( RAM_RESTO, ENTEROS, DECIMALES ) );

    acu_vuelto = ROUND( acu_vuelto + acu_vuelto_medio, ENTEROS, DECIMALES );
    if( ok && ( -RAM_RESTO ) > acu_vuelto && !MODO_DEVOLUCION && !NC_PERMANENTE ) {
        MENSAJE( "NO SE PUEDE ENTREGAR ESE MONTO DE VUELTO EN EL MEDIO SELECCIONADO" );
        ok = 0;
    }
    /*-------------------- Verifica que haya para dar vuelto -----------------*/
 	if( ok && 
			medios[_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 )].moneda_extranjera )  /*_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 )!= 2)//si no es moneda extrajera*/
	{
		if( ok && acu_vuelto_medio > 0.01 && acu_vuelto_medio >
			OBTENER_IMPORTE_SUBMEDIO( _PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ), _PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ), NO )
			&& CONTROLAR_VUELTO && !RAM_NOTA_CR && !RAM_RESTO_ME ) {
			for( i = 0;ok && i < RAM_P_PAGO;i++ )
			{
					h = _PAGO_MODO( i );
					_snprintf( aux, sizeof(aux), "%s", PAGOS_OBTENER_NOMBRE(  _PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ), _PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ) ));
					TRIM( aux );
					sprintf( mensaje, ST( S_NO_HAY__S_PARA_VUELTO ), aux );
					MENSAJE( mensaje );
					ok = 0;
            if( ok && CONTROLAR_VUELTO_MAXIMO_POR_MEDI ) {
                double vuelto_real;
                tt = MAXIMO_VUELTO_PERMITIDO( h, i );
                //vuelto_real = ( medios[h].efectivo ) ? vuelto_efectivo : fabs( _PAGO_VUELTO( i ) );
                vuelto_real = ( medios[h].efectivo ) ? vuelto_efectivo
                            : fabs( _PAGO_VUELTO( i ) + _PAGO_VUELTO_MEDIO( i ) );
                //---Si es el ultimo medio se le asigna todo el vuelto que queda
                if( i == ( RAM_P_PAGO - 1 ) && !medios[h].efectivo ) {
                    for( j = 0, hay_efectivo = 0 ;j < i ;j++ ) {
                        if( medios[j].efectivo ) {
                            hay_efectivo = 1;
                        }
                    }
                    if( !hay_efectivo ) {
                        vuelto_real = fabs( _PAGO_VUELTO( i ) + _PAGO_VUELTO_MEDIO( i )
                                          + vuelto_efectivo ) ;
                    }
                }
                if( tt && ( vuelto_real > tt ) ) {
                    sprintf( mensaje, ST( S_EL_VUELTO_EN__S_NO_PUEDE_EXCEDER__10_2F ), aux, tt );
                    MENSAJE( mensaje );
                    ok = 0;
                }
            }
        }
    }
	}

    /*----------- Valida que haya efectivo para vuelto -----------*/
    if( ok && vuelto_efectivo > 0.01 && vuelto_efectivo > X_TOT_DINERO && CONTROLAR_VUELTO
     && !RAM_NOTA_CR && !RAM_RESTO_ME ) {
        MENSAJE_STRING( S_NO_EFECTIVO_PARA_VUELTO );
        ok = 0;
    }
    /*----------- Valida que haya moneda extranjera para vuelto -----------*/
	if( ok && CONTROLAR_VUELTO && !RAM_NOTA_CR && RAM_RESTO_ME ) {
		h = _medio_moneda_extranjera;
		vuelto_me = RAM_RESTO_ME;
		vuelto_me = _X_VENTAS_IMPORTE_ME( h ) + _X_COBROS_IMPORTE_ME( h ) - _X_RETIROS_IMPORTE_ME( h )+ _X_FONDO_FIJO_MULTIPLE_IMP_ME( h );
        if( fabs( RAM_RESTO_ME )
          > ( _X_VENTAS_IMPORTE_ME( h ) + _X_COBROS_IMPORTE_ME( h ) - _X_RETIROS_IMPORTE_ME( h )
            + _X_FONDO_FIJO_MULTIPLE_IMP_ME( h ) ) ) {
            strcpy( aux, medios[h].nombre );
            TRIM( aux );
            sprintf( mensaje, ST( S_NO_HAY__S_PARA_VUELTO ), aux );
            MENSAJE( mensaje );
            ok = 0;
        }
    }
	/*if(_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 )== 2) {//si es moneda extrajera divido el vuelto acumulado por la cotizacion.
			acu_vuelto_medio = CALCULAR_RESTO( _PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 )-1,
					_PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ), acu_vuelto_medio);

	}*/
	acum_me = CALCULAR_RESTO( /*_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 )-1*/
			medios[_PAGO_MODO( RAM_P_PAGO - 1)].moneda_extranjera,
			_PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ),
			OBTENER_IMPORTE_SUBMEDIO( _PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ),
			_PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ), NO ) );

	if( ok && acu_vuelto_medio > 0.01 && acu_vuelto_medio > acum_me
		&& CONTROLAR_VUELTO && !RAM_NOTA_CR && !RAM_RESTO_ME ) {
			for( i = 0;ok && i < RAM_P_PAGO;i++ )
			{
					_snprintf( aux, sizeof(aux), "%s", PAGOS_OBTENER_NOMBRE(  _PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1 ), _PAGO_COD_SUBMEDIO_VUELTO( RAM_P_PAGO - 1 ) ));
					TRIM( aux );
					sprintf( mensaje, ST( S_NO_HAY__S_PARA_VUELTO ), aux );
					MENSAJE( mensaje );
					ok = 0;
			}
	}
    /*---------- Valida que no pague mas que el porcentaje habilitado --------*/
    for( i = 0;i < RAM_P_PAGO && ok;i++ ) {
        if( medios[_PAGO_MODO( i )].porc_del_pago < 100 ) {
            porc = ROUND( ( acu_modo[_PAGO_MODO( i )] / ( RAM_IMPORTE_TOTAL + RAM_VARIACION )
                          * 100 ), 10, DECIMALES );
            if( porc > medios[_PAGO_MODO( i )].porc_del_pago ) {
                sprintf( mensaje, ST( S_NO_PUEDE_PAGAR_MAS_DEL__2_2F____CON__S ),
                         medios[_PAGO_MODO( i )].porc_del_pago, medios[_PAGO_MODO( i )].nombre );
                MENSAJE( mensaje );
                ok = 0;
            }
        }
    }
    /*---------- Verifica cuanto paga en credito y en contado --------*/
    if( ok && RAM_TOTAL_VTA_CONTADO > acu_contado + 0.01 && CONTROLAR_PAGO_CONTADO ) {
        sprintf( mensaje, ST( S_DEBE_PAGAR_AL_MENOS__5_2LF_DE_CONTADO ), RAM_TOTAL_VTA_CONTADO );
        MENSAJE( mensaje );
        ok = 0;
    }
    /*---------- Verifica que no pague a credito si no puede --------*/
    if( ok && acu_credito && RAM_DEBE_PAGAR_CONTADO ) {
        MENSAJE_STRING( S_DEBE_PAGAR_TODO_DE_CONTADO );
        ok = 0;
    }
    /*-- Verifica que pague con los medios limitados por promociones --*/
    /*forray*/
    if( ok ) {
        /*struct requerir_medios medios_requeridos;
        BORRAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO);
        medios_requeridos.cod_grupo = 1;
        medios_requeridos.medio = 2;
        medios_requeridos.submedio = -1;
        medios_requeridos.plan  = -1;
        medios_requeridos.monto = 50;
        medios_requeridos.monto_compartido = NO;
        medios_requeridos.condicion_monto = CONDICION_MAYOR_IGUAL;
        GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios));
        medios_requeridos.cod_grupo = 1;
        medios_requeridos.medio = 2;
        medios_requeridos.submedio = 3;
        medios_requeridos.plan  = -1;
        medios_requeridos.monto = 50;
        medios_requeridos.monto_compartido = NO;
        medios_requeridos.condicion_monto = CONDICION_MAYOR_IGUAL;
        GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios));
        medios_requeridos.cod_grupo = 1;
        medios_requeridos.medio = 7;
        medios_requeridos.submedio = 10;
        medios_requeridos.plan  = -1;
        medios_requeridos.monto = 50;
        medios_requeridos.monto_compartido = NO;
        medios_requeridos.condicion_monto = CONDICION_MAYOR_IGUAL;
        GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios));
        medios_requeridos.cod_grupo = 1;
        medios_requeridos.medio = 7;
        medios_requeridos.submedio = 2;
        medios_requeridos.plan  = 1;
        medios_requeridos.monto = 50;
        medios_requeridos.monto_compartido = NO;
        medios_requeridos.condicion_monto = CONDICION_MAYOR_IGUAL;
        GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios));
        medios_requeridos.cod_grupo = 1;
        medios_requeridos.medio = 7;
        medios_requeridos.submedio =3;
        medios_requeridos.plan  = 1;
        medios_requeridos.monto = 50;
        medios_requeridos.monto_compartido = NO;
        medios_requeridos.condicion_monto = CONDICION_MAYOR_IGUAL;
        GUARDAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios));*/




        ok = MEDIOS_LIMITADOS_POR_PROMOCION();
		if( !ok ){
			MENSAJE( "MEDIOS DE PAGO NO VALIDOS" );
		}
        /*salir = 0;
        controlar_medio = 0;
        for( i = 0;i < DD_ELEMENTOS( __ram_medios_obligatorios ) && !salir;i++ ) {
            if( _RAM_MEDIOS_OBLIGATORIOS( i ) ) {
                if( !controlar_medio ) {
                    controlar_medio = i + 1;
                }
                for( h = 0;h < RAM_P_PAGO && !salir;h++ ) {
                    if( _PAGO_MODO( h ) == i ) {
                        salir = 1;
                    }
                }
                if( salir && !SUBMEDIO_EN_PROMOCION( i, h, &submedio_ok ) ) {
                    salir = 0;
                }
            }
        }
        if( controlar_medio && !salir ) {
            if( !submedio_ok ) {
                sprintf( mensaje, ST( S_DEBE_HABER_UN_PAGO_EN__S ),
                         medios[controlar_medio - 1].nombre );
            }
            else {
                if( medios[_PAGO_MODO( h - 1 )].tarjeta ) {
                    sprintf( mensaje, "DEBE HABER UN PAGO EN %s %s",
                             medios[controlar_medio - 1].nombre,
                             NOMBRE_TARJETA( submedio_ok - 1 ) );
                }
                else if( medios[_PAGO_MODO( h - 1 )].ticket_compra ) {
                    char cadena_auxiliar[150];
                    NOMBRE_TICKET_COMPRA( submedio_ok - 1, cadena_auxiliar );
                    sprintf( mensaje, "DEBE HABER UN PAGO EN %s %s",
                             medios[controlar_medio - 1].nombre, cadena_auxiliar );
                }
                else if( medios[_PAGO_MODO( h - 1 )].mutual ) {
                    char cadena_auxiliar[150];
                    NOMBRE_MUTUAL( ( char )( submedio_ok - 1 ), cadena_auxiliar );
                    sprintf( mensaje, "DEBE HABER UN PAGO EN %s %s",
                             medios[controlar_medio - 1].nombre, cadena_auxiliar );
                }
            }
            MENSAJE( mensaje );
            ok = 0;
        }*/
    }
    /*---------- Verifica que pague lo mismo si reprocesa --------*/
    if( ok && modo == _MODO_REPROCESAMIENTO ) {
        #ifdef COMPILAR_TECLA_REPROCESAR
        tt = TOT_TICK_ANT_IMPORTE_SIN_IVA + TOT_TICK_ANT_IMP_INT;
        tt += TOT_TICK_ANT_RECARGO_TARJETA + TOT_TICK_ANT_EXENTO;
        tt += TOT_TICK_ANT_IVA1 + TOT_TICK_ANT_IVA2;
        //      tt += TOT_TICK_ANT_ILA + TOT_TICK_ANT_IHA + TOT_TICK_ANT_DIFERENCIA +
        //          TOT_TICK_ANT_ALICUOTA;
        for( i = 0;i < 4;i++ ) {
            tt += _TOT_TICK_ANT_IMPUESTOS( i );
        }
        tt += TOT_TICK_ANT_DIFERENCIA;
        if( fabs( tt - RAM_IMPORTE_TOTAL - RAM_VARIACION ) > 0.03
				&& fabs( RAM_TOTAL_ANTERIOR - RAM_IMPORTE_TOTAL - RAM_VARIACION 
				- RAM_RETENCION_DGR - RAM_PERCEPCION_DGI - RAM_PERCEPCION_TACIS ) > 0.03 ) {
			MENSAJE_STRING( S_TOTAL_REPROCESAR_DEBE_SER_IGUAL_PAGO_ORIGINAL );
            if( MODO_DEBUG ) {
                sprintf( mensaje, "T1=%5.2lf, T2=%5.2lf, T3=(%5.2lf+%5.2lf) ", tt,
                         RAM_TOTAL_ANTERIOR, RAM_IMPORTE_TOTAL, RAM_VARIACION );
                MENSAJE( mensaje );
            }
            ok = 0;
        }
        #endif
    }
    if( ok ) {
		ok = OBTENER_DATOS_PINES_VIRTUALES(  );
	}
	if (ok) {
		ok = PROCESAR_QR();
	}

    /*----------- Controles para pagos de devoluciones y NC ----------*/
	if( ok && RAM_MODO_DEVOLUCION ) {
        int medio = 0, submedio = 0, controlar = 0, pos = 0;
        char msg[80], nombre[17];
		double ctrl_importe_total = 0.00, ctrl_importe = 0.00;
		double total = 0.00, total_me = 0.00;

        /*-- Se controla que no se intente devolver con dinero que no hay --*/
		//No se controla medio tarjeta, cheque y cta_cte
		for( pos = 0; ok && pos < RAM_P_PAGO; pos++ )
		{
			memset( msg, 0, sizeof( msg ) );
			memset( nombre, 0, sizeof( nombre ) );

			controlar = medios[_PAGO_MODO( pos )].efectivo
					|| medios[_PAGO_MODO( pos )].mutual
					|| medios[_PAGO_MODO( pos )].ticket_compra 
					|| medios[_PAGO_MODO( pos )].moneda_extranjera
					/*Otro medio que no sea tarjeta o cheque o cta_cte*/
					|| !( medios[_PAGO_MODO( pos )].tarjeta 
						|| medios[_PAGO_MODO( pos )].cheque 
						|| medios[_PAGO_MODO( pos )].cta_cte );
			
			if( controlar ) {
				total = _PAGO_IMPORTE( pos );
				if( _PAGO_IMPORTE_ME( pos ) ) {
					total_me = _PAGO_IMPORTE_ME( pos );
				}
				if( medios[_PAGO_MODO( pos )].efectivo ) {
					medio = _PAGO_MODO( pos ) + 1;
					submedio = 0;
				} else {
					medio = _PAGO_MODO( pos ) + 1;
					submedio = _PAGO_COD_SUBMEDIO( pos );
				}
				ctrl_importe = OBTENER_IMPORTE_SUBMEDIO( medio, submedio, NO );

				if( FONDO_FIJO_MULTIPLES_MEDIOS ) {
					ctrl_importe_total = (double)ctrl_importe + _X_FONDO_FIJO_MULTIPLE_IMPORTE( pos );
				} else {
					ctrl_importe_total = (double)ctrl_importe + X_FONDO_FIJO_IMPORTE;
				}

				if( ctrl_importe_total + 0.01 < abs( total ) ) {
					NOMBRE_MEDIO_SUBMEDIO( medio - 1, submedio, nombre, sizeof( nombre ) );
					_snprintf( msg, sizeof( msg ), "No hay suficiente Medio %s para la operacion ( Presione una tecla )", nombre );
					msg[ sizeof( msg ) - 1]= '/0';
					MENSAJE_TECLA( msg );
					ok = 0;
				}
			}
		}
		/*-- Se controla que se utilicen los mismos medios que en comprbante original --*/
		if( ok && AUTORIZAR_CAMBIO_MEDIO_EN_DEVOL ) {
			ok = PAGOS_CONTROL_COMPROB_IMPORTADO( &medio, &submedio );
		}
    }

    return ( ok );
}
/*****************************************************************************/
int OBTENER_DATOS_PINES_VIRTUALES( void )
/*****************************************************************************/
{
    int    error = 0;
    char   cod_barra_aux[17];
    double monto_aux = 0.0;

    if( RAM_NRO_PEDIDO_PIN ) {
        posicionTransaccionKinacu = 0;
        while( !error && RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&datos_cobro_kinacu,
                sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {

            /************************ Confirmar Venta de Articulo ************************/
            if( datos_cobro_kinacu.id_transaccion ) {
                MENSAJE( "ENVIANDO CONFIRMACION" );
                error = ENVIAR_TRANSACCION_KINACU( KINACU_CONFIRMACION, datos_cobro_kinacu.id_transaccion, NULL );
            }
            posicionTransaccionKinacu++;
        }
        if( error ) {
 	        char aux[80];

            memset( aux, 0, sizeof( aux ) );
            strcpy( aux, "NO SE PUDO CONFIRMAR RESERVA" );
            GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
            memset( aux, 0, sizeof( aux ) );
            strncpy( cod_barra_aux, datos_cobro_kinacu.cod_barra, sizeof( cod_barra_aux ) );
            monto_aux = datos_cobro_kinacu.monto;
            _snprintf( aux, 80, "ANULAR ARTICULO: %s  MONTO: %6.2lf", cod_barra_aux, monto_aux);
            aux[79] = '\0';
            GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
            MENSAJE_TECLA( aux );
	    }
    }

    return( !error );

}

/*****************************************************************************/
int MEDIO_DE_PAGO_ESPECIAL( int h )
/*****************************************************************************/
{
    int rta = 0;
    if( medios[h].efectivo || medios[h].cta_cte || medios[h].cheque || medios[h].tarjeta
     || medios[h].ticket_compra || medios[h].mutual ) {
        rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
int MEDIO_DE_PAGO_HABILITADO( int modo, int n, int dev_cobro )
/*****************************************************************************/
{
	int       rta;
   /*---Muestro solo los medios habilitados para cada tipo de pago---*/
	if( modo == _MODO_REPROCESAMIENTO && event_ticket.tipo_evento == 1 ) {
		modo = _MODO_COBRANZA;
	}
	if( modo == _MODO_COBRANZA ){
		struct    _datos_cobro datosCobros;

		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * ) &datosCobros,
		    sizeof( struct _datos_cobro ), 0 );
		/*if( medios[n].hab_cobro == 'S' || datosCobros.hab_todos_medios ) {
			rta = 1;
		} else if( medios[n].hab_cobro == 'M' ) {
			rta = PAGOS_MEDIO_HABILITADO_COBRO( n+1, 0, dev_cobro  );
		} else {
			rta = 0;
		}*/
		rta = MEDIOS_HABILITADOS_COBROS( n, dev_cobro, &datosCobros );
	} else {
		rta = ( modo != _MODO_FAC_REM	|| medios[n].hab_fac_rem ) &&
			( modo != _MODO_NC || medios[n].hab_nc ) && ( !RAM_MODO_DEVOLUCION || medios[n].hab_nc);
		if( rta && MODO_RESTRINGIDO_EN_MEDIOS  ) {
			rta = ( medios[n].hab_modo_restringido == 'S' ) ? 1 : 0;
		}
		if( rta && modo == _MODO_VENTA && medios[n].hab_ventas == 'N' ) {
      		rta = 0;
		}
	}
	return ( rta );
}
/*****************************************************************************/
double PAGOS_TOTAL_A_PAGAR()
/*****************************************************************************/
{
    double total = 0.0;

    if( RAM_FACTURA || RAM_TICKET_FACTURA /*|| MODO_DEVOLUCION || RAM_MODO_DEVOLUCION*/ ) {
         //Salvador --> ( RAM_TIPO_COMP_ANTERIOR == NC_ORIGEN_FACT_B && !RAM_CAMBIO )???
        total = SUMAR_TOTAL_FAC_CAJA( NO) + ROUND( RAM_VARIACION, ENTEROS, DECIMALES )
              + ROUND( RAM_RETENCION_DGR, ENTEROS, DECIMALES )
              + ROUND( RAM_PERCEPCION_DGI, ENTEROS, DECIMALES )
			  + ROUND( RAM_PERCEPCION_TACIS, ENTEROS, DECIMALES )
              + ROUND( RAM_PERCEPCION_212_IVA1, ENTEROS, DECIMALES )
              + ROUND( RAM_PERCEPCION_212_IVA2, ENTEROS, DECIMALES );
    }
    else {
        total = RAM_IMPORTE_TOTAL + RAM_VARIACION + RAM_RETENCION_DGR + RAM_PERCEPCION_DGI
              + RAM_PERCEPCION_TACIS + RAM_PERCEPCION_212_IVA1 + RAM_PERCEPCION_212_IVA2;
    }

    return ( total );
}
/*****************************************************************************/
double PAGOS_TOTAL( int redondeo )
/*****************************************************************************/
{
    int signo,i;
    double total = 0, tot_imp = 0;

    if( _modo == _MODO_COBRANZA
			|| ( _modo == _MODO_REPROCESAMIENTO && event_ticket.tipo_evento == 1 ) ) {
        total = RAM_IMPORTE_TOTAL + RAM_VARIACION;
    }
    else {
        signo = ( RAM_NOTA_CR || MODO_DEVOLUCION ) ? -1 : 1;
        for( i = 0;i < 4;i++ ) {
            tot_imp += _TOT_TICK_IMPUESTOS( i );
        }
        total = TOT_TICK_IMPORTE_SIN_IVA + TOT_TICK_IVA1 + TOT_TICK_IVA2 + tot_imp + TOT_TICK_EXENTO
              + TOT_TICK_RECARGO_TARJETA 
			  + signo * RAM_RETENCION_DGR 
			  + signo * RAM_PERCEPCION_DGI
			  + signo * RAM_PERCEPCION_TACIS 
			  + signo * RAM_PERCEPCION_212_IVA1 
			  + signo * RAM_PERCEPCION_212_IVA2;
        if( redondeo ) {
            total = ROUND( total, ENTEROS, DECIMALES );
        }
    }

    return ( total );

}
/*****************************************************************************/
int VERIFICA_MARCA_EN_MEDIO( char cod_medio, char marca )
/*****************************************************************************/
{
    int rta = 0;

    struct
    {
        char cod_medio;
        char marca;
    } clave;
    //Si el modo es servicio no se verifica ya que el cod_tarjeta fue seleccionado
    //por el usuario, y por lo tanto existe.
    if( MODO_NEGOCIO == SERVICIOS ) {
        return 1;
    }
	if( !OPEN_TABLE( T_TAR_MED, TT_ORIG, ( char* )&tar_med, sizeof( tar_med ) ) ) {
        clave.cod_medio = cod_medio + 1;
        clave.marca = marca + 1;
		SET_WHERE( "COD_MEDIO = '%d' AND COD_TARJETA = '%d'", clave.cod_medio, clave.marca );
		RUN_QUERY( NO );
        if( FOUND2() ) {
            rta = 1;
        }
        CLOSE_TABLE( T_TAR_MED, TT_ORIG );
    }
    return ( rta );
}
/*****************************************************************************/
int DEBE_ABRIR_CAJON()
/*****************************************************************************/
{
    int h, rta = 0;
    for( h = 0;h < RAM_P_PAGO && !rta;h++ ) {
        if( medios[_PAGO_MODO( h )].abrir_cajon == 'S' ) {
            rta = 1;
        }
    }
    if( !rta && RAM_RESTO < -0.001 ) {
		rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
void PAGOS_MUESTRA_IMPORTE_TEMPORAL( double importe, int mostrar_importe_temporal )
/*****************************************************************************/
{
    int at = _ATRIBUTO;
    char pr[20];
    char cadena[50];

    memset( pr, 0, sizeof( pr ) );
    memset( cadena, 0, sizeof( cadena ) );
    #if defined(INVEL_W) || defined(INVEL_L)
    if( mostrar_importe_temporal && importe ) {
        FTOC( importe, pr, ENTEROS, DECIMALES, 0, 1 );
        _snprintf( cadena, 50, "Importe:%s", pr );
        cadena[49] = '\0';
    }
    ExecuteSAction( A_PAGO_MUESTRA_IMPORTE_TEMPORAL, cadena );
    #else
    _ATRIBUTO = 15;
    BORRAR_MENSAJE();
    MOSTRAR_CADENA( 31 - ancho_panel / 2, 25, ST( S_IMPORTE_ ) );
    MOSTRAR_FLOTANTE( 40 - ancho_panel / 2, 25, ENTEROS, DECIMALES, 1, 0, importe );
    _ATRIBUTO = at;
    #endif
}
/*****************************************************************************/
void PAGOS_ENVIAR_OFFLINE()
/*****************************************************************************/
{
    if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_ENVIAR_OFFLINE ),
                          ST( S_ENVIAR_OFFLINE ), _F_SUP_ENVIAR_OFFLINE, SI ) ) {
        SET_MEMORY_CHAR( __ram_enviar_offline, !RAM_ENVIAR_OFFLINE );
        MOSTRAR_ENVIAR_OFFLINE();
    }
}
/*****************************************************************************/
void PAGOS_VALIDA_CHEQUE_CLIENTE()
/*****************************************************************************/
{
    double tot_cheques = 0;
    int i;
    double saldo;
    if( PEDIR_CLIENTE_EN_CHEQUES || RAM_NRO_CLIENTE ) {
        for( i = 0;i < RAM_P_PAGO;i++ ) {
            if( medios[_PAGO_MODO( i )].cheque ) {
                tot_cheques += _PAGO_IMPORTE( i );
            }
        }
        if( tot_cheques > 0 ) {
            saldo = clientes.limite_cheques - clientes.cupo_usado_cheques - tot_cheques;
            VALIDA_CHEQUE_CLIENTE( tot_cheques, saldo );
        }
    }
}
#ifdef COMPILAR_COMPROMISO_200
/*****************************************************************************/
void PAGOS_VALIDA_200()
/*****************************************************************************/
{
    int validar = 0;
    int i;
    if( MODO_DEVOLUCION ) {
        for( i = 0;i < RAM_P_TICKET && !validar;i++ ) {
            if( _TICKET_COD_MOVIMIENTO( i ) == _COMPROMISO_200 ) {
                validar = 1;
            }
        }
        if( validar ) {
            IMPRIME_CUPON_PROMOCION( VALIDACION_COMPROMISO_200, 0, -1, 1, 0, NULL,0 );
        }
    }
}
#endif
/*************************************************************************/
void LIMPIAR_VARIAS_TARJETAS()
/*************************************************************************/
{
    int handle;
    char aux[100];
    if( VARIAS_TARJETAS_POR_PAGO ) {
        #ifdef INVEL_L
        handle = creat( "tarjetas.$$$", 0 );
        #else
        #if defined(INVEL_W) || defined(INVEL_L)
        //00 Existence only
        //02  Write permission
        //04 Read permission
        //06 Read and write permission
        #ifdef INVEL_L
        if( ( access( "tarjetas.$$$", 0 ) ) != -1 )
                                        #else
            if( ( _access( "tarjetas.$$$", 0 ) ) != -1 )
                                                            #endif
            {
                #ifdef INVEL_L
                if( ( access( "tarjetas.$$$", 6 ) ) != -1 )
                                                                                #else
                    if( ( _access( "tarjetas.$$$", 6 ) ) != -1 )
                                                                                                    #endif
                    {
                        //printf( "File has read and write permission\n" );
                    }
                    else {
                        /* Change back to read/write: */
                        #ifdef INVEL_L
                        if( chmod( "tarjetas.$$$", S_IWRITE ) == -1 )
                                                                                                                                            #else
                            if( _chmod( "tarjetas.$$$", _S_IWRITE ) == -1 )
                                                                                                                                                                #endif
                            {
                                sprintf( aux,
                                         "No se encuentra archivo. No se pudo setear atributos %s",
                                         "tarjetas.$$$" );
                                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
                                return /*-1 (funcion void)*/;
                            }
                            else {
                                sprintf( aux, "Cambiando atributo a -r %s", "tarjetas.$$$" );
                                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,2);
                            }
                    }
            }
        #endif
        handle = _creat( "tarjetas.$$$", _S_IREAD | _S_IWRITE );
        #endif
        if( handle >= 0 ) {
            #ifdef INVEL_L
            close( handle );
            #else
            _close( handle );
            #endif
        }
        else {
            MENSAJE( "No se puede crear el tarjetas.$$$" );
        }
        SET_MEMORY_INT( __varios_tarjetas_por_pago, 0 );
    }
}
/***************************************************************************/
void ACTUALIZA_REDONDEO_DE_VUELTO( int modo )
/***************************************************************************/
{
    if( modo != _MODO_COBRANZA ) {
        ADD_MEMORY_DOUBLE( __x_diferencia_redondeo, RAM_DIFERENCIA_REDONDEO );
    }
}
/***************************************************************************/
double MAXIMO_VUELTO_PERMITIDO( int h, int i )
/***************************************************************************/
{
    double rta = 0;
    struct _t_mutuales mutual;
    struct _t_compra  t_compra;
    long cuenta;
    if( medios[h].tarjeta ) {
        rta = TARJ[VAR_TARJETAS_TARJETA].maximo_vuelto;
    }
    else if( medios[h].mutual ) {
        if( /*USE_DB( AREA_AUX, mutuales, ( char* )&mutual, sizeof( struct mutuales ), NULL,
                    0, 0 )*/
			OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutual,sizeof( struct _t_mutuales ))== 0 ) {
				SELECT_TABLE( T_MUTUALES, TT_ORIG );
            cuenta = _PAGO_CUENTA( i );
			SET_WHERE("COD_MUTUAL = '%ld'",cuenta);
			RUN_QUERY(NO);
            //GET_EQUAL( ( char* )&cuenta );
            if( FOUND2() ) {
                rta = mutual.vuelto_maximo;
            }
            CLOSE_TABLE( T_MUTUALES, TT_ORIG );
        }
    }
    else if( medios[h].ticket_compra ) {
        if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
         OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
			 SELECT_TABLE( T_COMPRA, TT_ORIG );
            cuenta = _PAGO_CUENTA( i );
			SET_WHERE("COD_T_COMPRA = '%li'",cuenta);
            //GET_EQUAL( ( char* )&cuenta );
			RUN_QUERY(NO);
            if( FOUND2() ) {
                rta = t_compra.vuelto_maximo;
            }
            CLOSE_TABLE( T_COMPRA, TT_ORIG );
        }
    }
    else {
        rta = medios[h].vuelto_maximo;
    }
    return rta;
}


/***************************************************************************/
double PORCENTAJE_VUELTO_PERMITIDO( int h, int i )
/***************************************************************************/
{
    double rta = 0;
    struct _t_mutuales mutual;
    struct _t_compra  t_compra;
    long cuenta;
    if( medios[h].tarjeta ) {
        rta = TARJ[VAR_TARJETAS_TARJETA].porc_vuelto;
    }
    else if( medios[h].mutual ) {
        if( /*USE_DB( AREA_AUX, mutuales, ( char* )&mutual, sizeof( struct mutuales ), NULL,
                    0, 0 )*/
			OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutual,sizeof( struct _t_mutuales ))== 0 ) {
				SELECT_TABLE( T_MUTUALES, TT_ORIG );
            cuenta = _PAGO_CUENTA( i );
			SET_WHERE("COD_MUTUAL = '%ld'",cuenta);
			RUN_QUERY(NO);
            //GET_EQUAL( ( char* )&cuenta );
            if( FOUND2() ) {
                rta = mutual.porc_vuelto;
            }
            CLOSE_TABLE( T_MUTUALES, TT_ORIG );
        }
    }
    else if( medios[h].ticket_compra ) {
        if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
         OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
			 SELECT_TABLE( T_COMPRA, TT_ORIG );
            cuenta = _PAGO_CUENTA( i );
			SET_WHERE("COD_T_COMPRA = '%li'",cuenta);
            //GET_EQUAL( ( char* )&cuenta );
			RUN_QUERY(NO);
            if( FOUND2() ) {
                rta = t_compra.porc_vuelto;
            }
            CLOSE_TABLE( T_COMPRA, TT_ORIG );
        }
    }
    else {
        rta = medios[h].porc_vuelto;
    }
    return rta;
}

/***************************************************************************/
char * PAGOS_OBTENER_NOMBRE( char medio, char submedio )
/***************************************************************************/
{
    static char nombre[20];
    struct _t_compra  t_compra;
    //struct    _t_mutuales mutuales;
	//int area_ant = SELECTED(  );
    int tabla_ant = SELECTED2();
    int	tipo_ant = SELECTED_TIPO();

    nombre[0] = 0;
     if( medios[medio - 1].ticket_compra ) {
        if( //USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )
         OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
			 SELECT_TABLE( T_COMPRA, TT_ORIG );
			 SET_WHERE("COD_T_COMPRA = '%i'",submedio);
            //GET_EQUAL( ( char* )&submedio );
			 RUN_QUERY(NO);
            if( FOUND2() ) {
                strcpy( nombre, t_compra.des_t_compra );
            }
            CLOSE_TABLE( T_COMPRA, TT_ORIG );
        }
    }
    else if( medios[medio - 1].mutual ) {
        if( /*USE_DB( AREA_AUX, mutuales, ( char* )&mutuales, sizeof( struct mutuales ),
                    NULL, 0, 0 )*/
			OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutuales,sizeof( struct _t_mutuales ))== 0 ) {
            //GET_EQUAL( ( char* )&submedio );
			//SELECT_TABLE( T_MUTUALES, TT_ORIG );
			SET_WHERE("COD_MUTUAL = '%i'",submedio);
			RUN_QUERY(NO);
            if( FOUND2() ) {
                strcpy( nombre, mutuales.des_mutual );
            }
            CLOSE_TABLE( T_MUTUALES, TT_ORIG );
        }
    }
    else if( medios[medio - 1].tarjeta ) {
        strcpy( nombre, TARJ[submedio - 1].nombre );
    }
    else {
        strcpy( nombre, medios[medio - 1].nombre );
    }
	//SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return ( nombre );
}

/***************************************************************************/
int PAGOS_CARGAR_MEDIO_DE_EVENTO( struct _ev_medios ev_pago, double importe, int var_tarjetas )
/***************************************************************************/
{
	int cargar = SI;
	int tarmed_abierta = NO;
	int area_ant = 0, tipo_ant = 0;
	int ok = 1;

	if( var_tarjetas ) {
		if(medios[( ev_pago.modo_pago - 1 )].tarjeta ) {
			char mensaje[100];
			do{
				
				SET_MEMORY_CHAR( __var_tarjetas_ant_cuotas, ev_pago.cuotas );
				if( RAM_IMPORTACION_PEDIDOS ) //para la NC automatica en la anulacion por tarjeta
					LEE_TARJETAS( 0, TARJETA_TRADICIONAL );
				if( ES_LA_MISMA_TARJETA(ev_pago.nro_tarjeta,GET_NUMERO_TARJETA_PRIVADO(  ))) {
						glog("ES LA MISMISIMA TARJETA",1,1 );
						ok = 0; //salteemos la valida_tarjeta
						SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 0);
				} else {
				_snprintf( mensaje, sizeof(mensaje), "Ingrese la Tarjeta %s", ev_pago.nro_tarjeta );
				MENSAJE( mensaje );
				}
	
				if( ok == 1 && !(VALIDA_TARJETA( NO, NO, VENTAS, NO, mensaje, SI, -1 /*NO*/ )&& 
					ES_LA_MISMA_TARJETA(ev_pago.nro_tarjeta,GET_NUMERO_TARJETA_PRIVADO(  )))){
						CEREAR_STRUCT( _var_tarjetas );
						if( LASTKEY()!= 27 ){
							BORRAR_MENSAJE();
							MENSAJE( "LA TARJETA NO SE UTILIZO EN EL PAGO ORIGINAL" );
						}
						cargar = NO;
				}else{
					SET_MEMORY_CHAR( __var_tarjetas_status, 2 );
					NUMERO_DE_COMERCIO_TARJETA(  );
					SET_MEMORY( __var_tarjetas_nro_comercio, ev_pago.nro_comercio );
					SET_MEMORY( __var_tarjetas_tabla, ev_pago.tabla );
					SET_MEMORY_CHAR( __var_tarjetas_cuotas, ev_pago.cuotas );
					//reseteo los valores originales
					strcpy( ev_pago.tabla, VAR_TARJETAS_TABLA) ;
					strcpy( ev_pago.nro_tarjeta, GET_NUMERO_TARJETA_PRIVADO( )) ;
					strcpy(ev_pago.nro_comercio , VAR_TARJETAS_NRO_COMERCIO );
					ev_pago.tarjeta = VAR_TARJETAS_TARJETA + 1;
					SET_MEMORY_LONG( __var_tarjetas_cupon_original, ev_pago.nro_cupon_int1);
					SET_MEMORY_INT( __var_tarjetas_fecha_original, ev_pago.fecha_ticket);
					SET_MEMORY( __var_tarjetas_aut_original, ev_pago.auto_tarjeta );
					SET_MEMORY_INT( __var_tarjetas_caja_original,ev_pago.caja );

					cargar = SI;
					//Averiguramos si la cuota es ONLINE, esto se esta seteando de tar_med.cuotas_online
					//Tarea: 4588
					area_ant = SELECTED2();
					tipo_ant = SELECTED_TIPO();
					if( OPENED( T_TAR_MED, TT_ORIG )) {
						tarmed_abierta = SI;
					}
					if( tarmed_abierta || (!OPEN_TABLE( T_TAR_MED, TT_ORIG, ( char* )&tar_med, sizeof( tar_med ) ) ) ) {
						SELECT_TABLE( T_TAR_MED, TT_ORIG );
						SET_WHERE(" COD_MEDIO = '%i' AND COD_TARJETA = '%i'", ev_pago.modo_pago, ev_pago.tarjeta );
						RUN_QUERY(NO);
						if( FOUND2( ) && (tar_med.cuotas_online == 's'|| tar_med.cuotas_online == 'S') ){
							SET_MEMORY_CHAR( __var_tarjetas_unicamente_ol, 
								( tar_med.cuotas_online == 'S' || VAR_TARJETAS_UNICAMENTE_OL )? 1: 0 );
						}
						CLOSE_TABLE( T_TAR_MED, TT_ORIG );
					}
					SELECT_TABLE( area_ant, tipo_ant );
				}
				BORRAR_MENSAJE();
			}while( cargar == NO && LASTKEY()!= 27 );
		}
		
    }
	/*PAGOS_AGREGAR_PAGO( int pos, char modo, double importe, long autorizacion, unsigned cuenta,
                         char tarjeta, long numero, char *numero_tar, unsigned banco,
                         unsigned fecha_vto, unsigned cantidad_cupones,
						 double valor_cupones,int c_postal_c, int c_hora_c, unsigned comprobante, char *cuit,
                         char *telefono, char *cuenta_banco, int id_registro, char cod_medio_vuelto,
                         char cod_submedio_vuelto, char tipo_vuelto, double importe_me,
                         int cod_valida_cheque, int datos_adicionales, char *telefono_comprador,
						 int cod_restriccion_art )*/


	if( cargar == SI )
		PAGOS_AGREGAR_PAGO( -1, ( char )( ev_pago.modo_pago - 1 ),
						importe, ev_pago.buffer1_long, /*ev_pago.numero_id*/ ev_pago.cod_sub_medio ,
                        ev_pago.tarjeta - 1, ev_pago.numero_id, ev_pago.nro_tarjeta,
                        ev_pago.banco, ev_pago.fecha_vto, ev_pago.cantidad_cupones,
                        ev_pago.valor_cupones, ev_pago.cod_post_cheq,ev_pago.cod_hora_acre, ev_pago.banco, ev_pago.cuit,
                        ev_pago.telefono, ev_pago.nro_comercio, (int)ev_pago.id_registro, 0,
                        0, 0, ev_pago.importe_me, 0, 0, "", 0 );

	return cargar;/*si cargo retorna 1  - sino cargo retorna 0*/
}

/***************************************************************************/
int PAGOS_CARGAR_MEDIOS_IMPORTACION()
/***************************************************************************/
{
    int rta = 0;
    struct _ev_medios ev_pago;
	struct _ev_cont ev_cont;
	double ev_cont_importe = 0.00, importe_cobro = 0.00, ev_medio_importe = 0.00, diferencia_imp = 0.00;
	int area_ant, tipo_ant, efectivo = 0, fin = 0;

    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	if( OPEN_TABLE( T_EVC_TMP, TT_ORIG, ( char* )&ev_cont, sizeof( struct _ev_cont ) ) == 0 ) {
		SET_WHERE("");
		SET_ORDER2(" ID_EVENTO, POSICION ");
		RUN_QUERY(NO);
		while( !dbEOF() ) {
			//Articulos Cobro Venta Kinacu no se devuelven.
			if( ev_cont.cod_cobro > 0 ) {
				importe_cobro += ev_cont.cantidad *(( ev_cont.tipo & _RESTA ) ? -1 : 1 )* (ev_cont.importe_sin_iva + ev_cont.iva1  + ev_cont.iva2 + ev_cont.exento )+ ev_cont.imp_int;
			}
			ev_cont_importe += ev_cont.cantidad 
				*(( ev_cont.tipo & _RESTA ) ? -1 : 1 )
				*(ev_cont.importe_sin_iva 
					+ ev_cont.iva1  
					+ ev_cont.iva2 
					+ ev_cont.exento)
					+ ev_cont.imp_int;
			SKIP2( 1 );
		}
		CLOSE_TABLE( T_EVC_TMP, TT_ORIG );
	}

	if( OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&ev_pago, sizeof( struct _ev_medios ) ) == 0 ) {
		SET_WHERE("");
		RUN_QUERY(NO);
		while( !dbEOF() ) {
			if( medios[ev_pago.modo_pago - 1].efectivo ) {
				efectivo = 1;
			}
			ev_medio_importe += ev_pago.importe;
			SKIP2( 1 );
		}

		diferencia_imp = ev_cont_importe - ev_medio_importe;

		SET_WHERE("");
		SET_ORDER2(" ID_EVENTO, POSICION ");
		GO2( TOP );
		RUN_QUERY(NO);
		while( !dbEOF() && !fin ) {
			BORRAR_MENSAJE();
			if( !ev_pago.reproceso ) {
                rta = 1;
				if( !importe_cobro ) {
					if( medios[ev_pago.modo_pago - 1].efectivo  ){
						if( ( fabs( diferencia_imp ) >= 0.001 && fabs( diferencia_imp ) < 0.02 ) ) {
							ev_medio_importe = ROUND( ev_pago.importe + diferencia_imp , ENTEROS, DECIMALES );
							fin != PAGOS_CARGAR_MEDIO_DE_EVENTO( ev_pago, ev_medio_importe
								- ev_pago.vuelto_efectivo
								- ev_pago.vuelto_medio,
								((medios[ev_pago.modo_pago - 1].tarjeta)? SI : NO ) );
							diferencia_imp = 0;
						} else {
							fin != PAGOS_CARGAR_MEDIO_DE_EVENTO( ev_pago, ev_pago.importe
							- ev_pago.vuelto_efectivo
							- ev_pago.vuelto_medio,
							((medios[ev_pago.modo_pago - 1].tarjeta)? SI : NO ) );
						}
					} else {
						if( !efectivo && (( fabs( diferencia_imp ) >= 0.001 && fabs( diferencia_imp ) < 0.02 ) ) )
						{
							ev_medio_importe = ROUND( ev_pago.importe + diferencia_imp , ENTEROS, DECIMALES );
							fin != PAGOS_CARGAR_MEDIO_DE_EVENTO( ev_pago, ev_medio_importe
							- ev_pago.vuelto_efectivo
							- ev_pago.vuelto_medio,
							((medios[ev_pago.modo_pago - 1].tarjeta)? SI : NO ) );
							diferencia_imp = 0;
						}
						else
						{
							fin != PAGOS_CARGAR_MEDIO_DE_EVENTO( ev_pago, ev_pago.importe
								- ev_pago.vuelto_efectivo
								- ev_pago.vuelto_medio,
								((medios[ev_pago.modo_pago - 1].tarjeta)? SI : NO ) );
						}
					}
				} else {
					SET_WHERE("MODO_PAGO = 1");
					RUN_QUERY(NO);
					SET_MEMORY_CHAR( __ram_p_pago, 0 );
					ev_medio_importe = ROUND( ev_cont_importe - importe_cobro, ENTEROS, DECIMALES );
					if( FOUND2())
					{
						fin != PAGOS_CARGAR_MEDIO_DE_EVENTO( ev_pago, ev_medio_importe
								- ev_pago.vuelto_efectivo
								- ev_pago.vuelto_medio,
								((medios[ev_pago.modo_pago - 1].tarjeta)? SI : NO ) );
					}
					else
					{
						//Descargo la diferencia de importe en el medio de pago 1
						PAGOS_AGREGAR_PAGO( -1, ( char )0, ev_medio_importe, 0, 0,
                            (char)0, 0, (char)0,
                            0, 0, 0,
                            0, 0,
                            0, 0, (char)0,
                            (char)0, (char)0, 1, (char)0,
                            (char)0, (char)0, 0, (int)0, (int)0, "", 0 );
					}
					fin = 1;
					importe_cobro = 0;
				}

			}
            SELECT_TABLE( T_EVM_TMP, TT_ORIG );
            SKIP2( 1 );
        }
        CLOSE_TABLE( T_EVM_TMP, TT_ORIG );
    }
	 SELECT_TABLE(area_ant, tipo_ant);

    return ( rta );
}
/***************************************************************************/
int PAGOS_CARGAR_TARJETAS_IMPORTACION( )
/***************************************************************************/
{
    int rta = 0, h=0, fue_cargada = 0;
    struct _ev_medios ev_pago;

    //if( USE_DATABASE( AREA_BOTON, EVM_TMP, ( char* )&ev_pago, sizeof( struct _ev_medios ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&ev_pago, sizeof( struct _ev_medios ) ) == 0 ) {
        //SELECT( AREA_BOTON );
		SET_WHERE("");
		RUN_QUERY(NO);
        while( /*!BtrvEOF()*/ !dbEOF() ) {
			
            if ( ev_pago.tarjeta && ev_pago.importe != 0.0 ) {
                /* Se verifica que la tarjeta no haya sido cargada ya */
				
				for( h = 0; h < RAM_P_PAGO && fue_cargada == 0 ; h++ ) {
					
					if ( medios[_PAGO_MODO( h )].tarjeta ){
						rta = ES_LA_MISMA_TARJETA(ev_pago.nro_tarjeta,_PAGO_NRO_TARJETA( h ));
						fue_cargada = rta;
					}
				}

                if( !fue_cargada && !ev_pago.reproceso && ev_pago.importe != 0.0 ) {
                    double importe_en_tarjeta = 0;
                    rta = 1;
                    /* Se calcula el importe a utilizar en el pago de tarjeta (el maximo posible) */
                    importe_en_tarjeta = min( RAM_RESTO, ev_pago.importe
                                                         - ev_pago.vuelto_efectivo
                                                         - ev_pago.vuelto_medio );

                    rta  = PAGOS_CARGAR_MEDIO_DE_EVENTO( ev_pago, importe_en_tarjeta, SI );
                }
            }
            //SELECT( AREA_BOTON );
			SELECT_TABLE( T_EVM_TMP, TT_ORIG );
			if( !ev_pago.tarjeta || (rta != 0 || LASTKEY()== 27 ))//siga pidiendo el mismo
				SKIP2( 1 );
			fue_cargada = 0;
		}
        //CLOSE_DATABASE( AREA_BOTON );
        CLOSE_TABLE(T_EVM_TMP, TT_ORIG);
    }
    return ( rta );
	}

double PAGOS_ME_A_ML( double importe_me, int h )
/***************************************************************************/
{
    double importe_ml;
    importe_ml = ROUND( importe_me * medios[h].cotizacion, 10, DECIMALES );
    return ( importe_ml );
}
/***************************************************************************/
double PAGOS_ML_A_ME( double importe_ml, int h )
/***************************************************************************/
{
    double importe_me = 0.0;
    if( medios[h].cotizacion ) {
        importe_me = ROUND( importe_ml / medios[h].cotizacion, 10, DECIMALES );
    }
    return ( importe_me );
}
/***************************************************************************/
int PAGOS_PEDIR_DATOS_ADICIONALES( int pos, char indice_pos, int datos_adicionales, double *importe,
                                   int origen, char *titulo, int nro, long *comprobante,
                                   int ( f_datos )( int, int, int, int, double, struct _dmedios *,
                                                    char * ) , char cod_medio, char submedio )
/***************************************************************************/
{
    int rta = 1, cant_datos = 0;
    struct _dmedios *datos_medio = NULL;
    cant_datos = PAGOS_CARGAR_DATOS_MEDIO( &datos_medio, datos_adicionales, origen );
    if( cant_datos > 0 ) {

		if( datos_medio->tipo_mensaje == 1 )
			MENSAJE_TECLA( datos_medio->mensaje );

        if( cant_datos ) {
            rta = _PAGOS_PEDIR_DATOS_ADICIONALES( pos, indice_pos, importe, datos_medio, cant_datos,
                                                  origen, titulo, nro, comprobante, f_datos , cod_medio, submedio );
        }

		if( datos_medio->tipo_mensaje == 2 && rta )
			MENSAJE_TECLA( datos_medio->mensaje );

        if( datos_medio ) {
            free( datos_medio );
        }
    }
    else {
        rta = 0;
    }
    return ( rta );
}
/***************************************************************************/
int _PAGOS_PEDIR_DATOS_ADICIONALES( int pos, char indice_pos, double *importe,
                                    struct _dmedios *datos_medio, int cant_datos, int origen,
                                    char *titulo, int nro, long *comprobante,
                                    int ( f_datos )( int, int, int, int, double, struct _dmedios *,
                                                     char * ) , char cod_medio, char submedio )
/***************************************************************************/
{
    	int rta = 1, seguir = 1, x = 0, y = 0, i = 0, j = 0, m = 0, pos_actual = 0, _rta = 0,
			pedir_dato = 1, msj_adic = 0;
	struct _datos *datos;
	int lectura_cheq = 0, ok = 0, pedir_input_manual = 0, cant_datos_pedir = 0;
	char buffer_cheq[40];
	int decimal = 0, confirmar = 0, posicion = 0;
	char *cad_aux = NULL;
	char *tam_aux = NULL;
	char size[5];
	int len = 0, len_aux = 0;
	int digitos_min = 0;
	memset(size, 0, sizeof(size));
	//struct DatosMedioImpresion DatosImpresion;
	//--- Se cuentan la cantidad de datos a pedir, excluyendo los reservados.
	for (i = 0, cant_datos_pedir = 0; i < cant_datos; i++) {
		//if( datos_medio[i].tipo_dato != 4 ) {
		cant_datos_pedir++;
		//}
	}
	for (i = 0; i < cant_datos_pedir; i++) {
		memset(size, 0, sizeof(size));
		_snprintf(size, sizeof(size), "%i", datos_medio[i].digitos);
		size[sizeof(size) - 1] = '\0';
		len += strlen(datos_medio[i].descripcion);
		len_aux += strlen(size);
	}
	len += (cant_datos_pedir /*- 1*/);
	len_aux += (cant_datos_pedir /*- 1*/);
	cad_aux = (char*) malloc(len);
	tam_aux = (char*) malloc(len_aux);

	if (cad_aux == NULL || tam_aux == NULL) {
		char mensajes[30];
		sprintf(mensajes, "Insufficient memory available");
		glog(mensajes, LOG_ERRORES, 1);
	} else {
		memset(cad_aux, 0, len);
		memset(tam_aux, 0, len_aux);
		for (i = 0; i < cant_datos_pedir; i++) {
			memset(size, 0, sizeof(size));

			strncat(cad_aux, datos_medio[i].descripcion,
					strlen(datos_medio[i].descripcion));

			_snprintf(size, sizeof(size), "%i", datos_medio[i].digitos);
			size[sizeof(size) - 1] = '\0';

			strncat(tam_aux, size, max(len_aux - strlen(tam_aux) - 1, 0));
			if (i + 1 < cant_datos_pedir) {
				strncat(cad_aux, "~", 1);
				strncat(tam_aux, "~", 1);
			}// "-" sirve de separador en Java
		}

		if (cant_datos_pedir) {
			ExecuteSSAction(A_PEDIR, _AJUSTABLE, titulo, cad_aux, NULL, tam_aux);
		}
	}
	datos = calloc(cant_datos, sizeof(struct _datos));
	if (datos == NULL) {
		rta = 0;
	} else {
		//memset( datos, ' ', sizeof( struct _datos ) * cant_datos );
		memset(datos, 0, sizeof(struct _datos) * cant_datos);
	}
	/*--------------- Se piden los datos ------------*/
	pos_actual = 0;
	CEREAR_MEMORY(__ram_cadena_cheque);
	while (seguir && rta) {
		if (datos_medio[pos_actual].lector_cheque == 'S' && !lectura_cheq
				&& PASAR_CHEQUE_POR_LECTOR) {
			//lectura_cheq = LEER_CHEQUE_IMPRESORA( );
			LEER_CHEQUE_IMPRESORA();
			lectura_cheq = 1;
		}
		_rta = 0;
		if (f_datos != NULL) {
			_rta = f_datos(pos, indice_pos, nro, pos_actual, *importe,
					datos_medio, datos[pos_actual].valor);
			if (_rta == 1) {
				MOSTRAR_CADENA((char) (x + 23), (char) (y + pos_actual),
						datos[pos_actual].valor);
			}
		}
		if (_rta == 0) {
			pedir_dato = 1;
			switch (datos_medio[pos_actual].tipo_dato) {
			case 4:
				// Flotante
				decimal = 1;
				/*falta el break para que entre en numerico*/
			case 1:
				// Numerico
				while (pedir_dato) {
					/* No se debe pedir dato en el caso de que se muestre un menu donde
					 hay que seleccionar un valor. Pero eso se desarrollo solo para SMG */
					pedir_dato = (MODO_NEGOCIO == SERVICIOS ? 0 : 1);
					pedir_input_manual = 1;
					_PERMITIR_SCANNER = SI;
					if (datos_medio[pos_actual].lector_cheque == 'S'
							&& PASAR_CHEQUE_POR_LECTOR
							&& _RAM_CADENA_CHEQUE( 0 ) != 0) {
						ok = !PEDIR_VALIDAR_MASCARA(buffer_cheq,
								datos_medio[pos_actual].mascara_scanner);
						if (ok) {
							strcpy(datos[pos_actual].valor, buffer_cheq);
							pedir_input_manual = 0;
							_rta = 1;
							MOSTRAR_CADENA((char) (x + 23),
									(char) (y + pos_actual),
									datos[pos_actual].valor);
							for (i = 0; i < (int) strlen(
									datos[pos_actual].valor); i++) {
								if (!(datos[pos_actual].valor[i] == ' '
										|| (datos[pos_actual].valor[i] >= '0'
												&& datos[pos_actual].valor[i]
														<= '9'))) {
									pedir_input_manual = 1;
								}
							}
						}
					}
					if (pedir_input_manual) {
						//#if defined(INVEL_W) || defined(INVEL_L)
						ExecuteSSAction(A_PEDIR_DATO, pos_actual, " ", " ",
								NULL, " ");
						//#endif
						if (MODO_NEGOCIO == SERVICIOS) {
							if ((VALIDA_CODIGO(
									datos_medio[pos_actual].cod_valida,
									datos[pos_actual].valor)) >= 0) {
								#if defined(INVEL_W) || defined(INVEL_L)
								ExecuteSAction(A_SET_INPUT_TEXT,
										datos[pos_actual].valor);
								#endif
								//MOSTRAR_INPUT_CADENA( datos[pos_actual].valor, 0 );
								_rta = 1;
							} else {
								//_rta < 0 (no hay que validar)
								pedir_dato = 1;
							}
						}
						if (pedir_dato == 1) {
							pedir_dato = 0;
							if (datos_medio[pos_actual].cod_descriptor
									&& datos_medio[pos_actual].modo_descriptor
											== 1) {
								char key = 0;
								memset(datos[pos_actual].valor, 0,
										sizeof(datos[pos_actual].valor));

								ExecuteAction(A_FIN_PEDIR);
								_rta = _PAGOS_PEDIR_DESCRIPTOR(
										&datos_medio[pos_actual],
										datos[pos_actual].valor);
								ExecuteSSAction(A_PEDIR, _CONCATENADO, titulo,
										cad_aux, NULL, " ");

								MOSTRAR_INPUT_CADENA(datos[pos_actual].valor, 0);
								while (key != 13 && key != 27
										&& datos[pos_actual].valor[0]) {
									key = GETCH();
									if (key == 27) {
										_rta = 0;
										pedir_dato = 1;
									}
								}
							} else {
								if( datos_medio[pos_actual].tipo_mensaje == 1 ) {
									MENSAJE(datos_medio[pos_actual].mensaje);
									msj_adic = 1;
								}
								_rta
										= PEDIR_CADENA_NUMERICA(
												(char) (x + 23),
												(char) (y + pos_actual),
												(char) datos_medio[pos_actual].digitos,
												datos[pos_actual].valor,
												datos_medio[pos_actual].mascara_scanner,
												decimal, SI, NO, ( char )0);
								if( datos_medio[pos_actual].tipo_mensaje == 2 && _rta ) {
									MENSAJE(datos_medio[pos_actual].mensaje);
									msj_adic = 1;
								}
								if( msj_adic ) {
									msj_adic = 0;
									BORRAR_MENSAJE( );
								}

							}
						}
					}
					_PERMITIR_SCANNER = NO;
					if (_rta == 1 && !_PAGOS_VALIDAR_LARGO(
							datos[pos_actual].valor,
							datos_medio[pos_actual].digitos_minimos,
							datos_medio[pos_actual].cod_referencia,
							_TIPO_NUMERICO)) {
						pedir_dato = 1;
						CEREAR_MEMORY(__ram_cadena_cheque);
					}
				}
				if (!_rta) {
					_rta = -1;
				}
				break;
			case 2:
				// Cadena
				while (pedir_dato) {
					/* No se debe pedir dato en el caso de que se muestre un menu donde
					 hay que seleccionar un valor. Pero eso se desarrollo solo para SMG */
					pedir_dato = (MODO_NEGOCIO == SERVICIOS ? 0 : 1);
					pedir_input_manual = 1;
					TECLADO_ALFABETICO = 1;
					_PERMITIR_SCANNER = SI;
					if (datos_medio[pos_actual].lector_cheque == 'S'
							&& PASAR_CHEQUE_POR_LECTOR
							&& _RAM_CADENA_CHEQUE( 0 ) != 0) {
						ok = !PEDIR_VALIDAR_MASCARA(buffer_cheq,
								datos_medio[pos_actual].mascara_scanner);
						if (ok) {
							strcpy(datos[pos_actual].valor, buffer_cheq);
							pedir_input_manual = 0;
							_rta = 1;
							MOSTRAR_CADENA((char) (x + 23),
									(char) (y + pos_actual),
									datos[pos_actual].valor);
						}
					}
					digitos_min = datos_medio[pos_actual].digitos_minimos;

					if (pedir_input_manual) {
//#if defined(INVEL_W) || defined(INVE_L)
						ExecuteSSAction( A_PEDIR_DATO, pos_actual, " ", " ", NULL, " " );
//#endif
						if (pedir_dato == 1) {
							pedir_dato = 0;
							if (datos_medio[pos_actual].cod_descriptor
									&& datos_medio[pos_actual].modo_descriptor
											== 1) {
								char key = 0;
								memset(datos[pos_actual].valor, 0,
										sizeof(datos[pos_actual].valor));

								ExecuteAction(A_FIN_PEDIR);
								_rta = _PAGOS_PEDIR_DESCRIPTOR(
										&datos_medio[pos_actual],
										datos[pos_actual].valor);
								ExecuteSSAction(A_PEDIR, _CONCATENADO, titulo,
										cad_aux, NULL, " ");

								MOSTRAR_INPUT_CADENA(datos[pos_actual].valor, 0);
								while (key != 13 && key != 27
										&& datos[pos_actual].valor[0]) {
									key = GETCH();
									if (key == 27) {
										_rta = 0;
										pedir_dato = 1;
									}
								}
							} else {
								if( datos_medio[pos_actual].tipo_mensaje == 1 ) {
									MENSAJE(datos_medio[pos_actual].mensaje);
									msj_adic = 1;
								}
								_rta
										= PEDIR_CADENA(
												(char) (x + 23),
												(char) (y + pos_actual),
												(char) datos_medio[pos_actual].digitos,
												datos[pos_actual].valor,
												datos_medio[pos_actual].mascara_scanner,
												SI, NO);
								if( datos_medio[pos_actual].tipo_mensaje == 2 && _rta ) {
									MENSAJE(datos_medio[pos_actual].mensaje);
									msj_adic = 1;
								}
								if( msj_adic ) {
									msj_adic = 0;
									BORRAR_MENSAJE( );
								}
							}
						}
					}
					_PERMITIR_SCANNER = NO;
					TECLADO_ALFABETICO = 0;
					_PERMITIR_SCANNER = NO;
					if (_rta == 1 && !_PAGOS_VALIDAR_LARGO(
							datos[pos_actual].valor, digitos_min,
							datos_medio[pos_actual].cod_referencia,
							_TIPO_CADENA)) {
						pedir_dato = 1;
						CEREAR_MEMORY(__ram_cadena_cheque);
					}
				}
				if (!_rta) {
					_rta = -1;
				} /*else {
				 TOMAR_DATO_ADICIONAL( datos[pos_actual].valor, _TIPO_CADENA, cod_medio, submedio );
				 }*/
				break;
			case 3:
				// Fecha
				_rta = 0;
				//while( _rta == 0 )
//#if defined(INVEL_W) || defined(INVEL_L)
				ExecuteSSAction(A_PEDIR_DATO, pos_actual, " ", " ", NULL, " ");
//#endif
				_PERMITIR_SCANNER = NO;
				msj_adic = 0;
				if( datos_medio[pos_actual].tipo_mensaje == 1 ) {
					MENSAJE(datos_medio[pos_actual].mensaje);	
					msj_adic = 1;
				}
				_rta = PEDIR_FECHA(x + 23, y + pos_actual,
						datos[pos_actual].valor, SI);
				if( datos_medio[pos_actual].tipo_mensaje == 2 && _rta ) {
					MENSAJE(datos_medio[pos_actual].mensaje);
					msj_adic = 1;
				}
				if( msj_adic ) {
					msj_adic = 0;
					BORRAR_MENSAJE( );
				}

				if (_rta > 0) {
					if (!_PAGOS_VALIDAR_LARGO(datos[pos_actual].valor,
							datos_medio[pos_actual].digitos_minimos,
							datos_medio[pos_actual].cod_referencia, _TIPO_FECHA)) {
						pedir_dato = 1;
						_rta = 0;
					} else {
						DTOC4(_rta, datos[pos_actual].valor);
						datos[pos_actual].valor[30] = 0;
						_rta = 1;
					}
				}
				break;
			default:
				_rta = -1;
				break;
			}
			/********* Confirmar Datos Adicionales *********/
			if (_rta == 1 && datos_medio[pos_actual].reingresar == 'S') {
				ExecuteAction(A_FIN_PEDIR);
				confirmar = CONFIRMAR_DATO_ADICIONAL(datos_medio,
						datos[pos_actual].valor, pos_actual);
				ExecuteSSAction(A_PEDIR, _AJUSTABLE, titulo, cad_aux, NULL,
						tam_aux);
				//Posici�n actual para Reconstruir INPUT original.
				posicion = pos_actual;
				if (confirmar == -1) {//ESC para funci�n CONFIRMAR_DATO_ADICIONAL
					/*Posici�n anterior para Reconstruir INPUT.
					 Solicita dato al cual se le dio ESC para volver a ingresar
					 Si es la 1era posicion no decrementa la posicion para graf el input*/
					if (posicion) {
						posicion = pos_actual - 1;
					}
					_rta = -2;
				}
				//Reconstruyo INPUT segun posicion
				for (j = 0; j <= posicion && confirmar; j++) {
					ExecuteSSAction(A_PEDIR_DATO, j, " ", " ", NULL, " ");
					MOSTRAR_INPUT_CADENA(datos[j].valor, j);
				}
			}
			/*if( _rta ) {
			 TOMAR_DATO_ADICIONAL( datos[pos_actual].valor, datos_medio[pos_actual].descripcion, datos_medio[pos_actual].nro_orden, datos_medio[pos_actual].tipo_dato, cod_medio, submedio );
			 }*/
		}
		
		
		switch (_rta) {
		case 1:
			// Seguir
			pos_actual++;
			if (pos_actual == cant_datos_pedir) {
				seguir = 0;
			}
			break;
		case -1:
			// Anular
			rta = 0;
			break;
		case -3:
			// Flecha Arriba
			if (pos_actual) {
				pos_actual--;
			}
			break;
		}
	}
	if (rta) {
		//------------- Validar en Boletin ----------//
		for (i = 0; i < cant_datos_pedir && rta; i++) {
			if (origen == D_CLIENTES) {
				datos[i].boletin_consultado = 0;
				datos[i].cod_supervisor = 0;
			} else {
				rta = PAGOS_VALIDA_EN_BOLETIN(datos_medio, i, cant_datos,
						importe, datos);
			}
			/*Comprueba si tiene referencia para asignar el valor*/
			if (MODO_NEGOCIO == SERVICIOS) {
				switch (datos_medio[i].cod_referencia) {
				case 1:
					// Importe
					if (importe != NULL) {
						*importe = atof(datos[i].valor);
					}
					break;
				case 2:
					// Comprobante
					if (comprobante != NULL) {
						*comprobante = atol(datos[i].valor);
					}
					break;
					/*if(datos_medio[i].verificar_importe == 'S')
					 *importe = atof(datos[i].valor);*/
				}
			}
		}
		//------------- Registrar los datos ingresados ------------
		for (i = 0; rta && i < cant_datos; i++) {
			AGREGA_DATOS_MEDIO( /*( _modo == _MODO_VENTA ? pos : i )*/pos,
					indice_pos, datos_medio[i].nro_orden,
					datos_medio[i].id_dato, &datos[i],
					datos_medio[i].cod_solicitud, origen, (i == 0), 0.0);
		}
	}
	if (datos) {
		free(datos);
	}

	if (cant_datos_pedir) {
		ExecuteAction(A_FIN_PEDIR);
	}
	if (cad_aux != NULL) {
		free(cad_aux);
	}
	return (rta);
}
/***************************************************************************/
int _PAGOS_VALIDAR_LARGO( char *valor, int largo_min, INT16 cod_referencia, int tipo_dato )
/***************************************************************************/
{
    char cadena[80];
    int rta = 1, j;
    strncpy( cadena, valor, sizeof( cadena ) );
    for( j = strlen( cadena ) - 1;j >= 0 ;--j ) {
        if( !isspace( cadena[j] ) ) {
            cadena[j + 1] = 0;
            // el string termina aqui, todas las iteraciones posteriores son innecesarias
            valor[j] = cadena[j];
            break;
        }
    }
    if( ( int )strlen( cadena ) < largo_min ) {
        rta = 0;
        sprintf( cadena, ST( S_DEBE_INGRESAR_AL_MENOS__D_DIGITOS__PRESIONE__ENTER_ ), largo_min );
        MENSAJE( cadena );
        while( GETCH() != 13 );
        BORRAR_MENSAJE();
    }
    // valido valor mayor que cero para aquellos datos que sean del tipo importe
    //cod_referencia == 0 nada
    //cod_referencia == 1 dato tipo numerico "importe". Suma importe al ram_total
    //cod_referencia == 2 dato tipo numerico "comprobante"
    //cod_referencia == 3 dato tipo numerico "importe". No Suma importe al ram_total
    //cod_referencia == 4 nada
    if( rta && tipo_dato == _TIPO_NUMERICO ) {
        switch( cod_referencia ) {
            case 1:
                //no acepta valores menores o iguales a cero
            case 2:
                //no acepta valores menores o iguales a cero
            case 3:
                //no acepta valores menores o iguales a cero
                if( atof( valor ) <= 0.0 ) {
                    rta = 0;
                    MENSAJE( "VALOR INCORRECTO - PRESIONE <ENTER> PARA REINTENTAR" );
                    while( GETCH() != 13 );
                    BORRAR_MENSAJE();
                }
                break;
        }
    }
    // valido valor mayor que cero para aquellos datos que sean del
    // tipo alfanumerico obligatorios donde no se acepta el blanco como unico digito
    //cod_referencia == 0 nada
    //cod_referencia == 1 nada
    //cod_referencia == 2 nada
    //cod_referencia == 3 dato tipo alfanumerico no acepta incio cadena el valor cero, no le importa el blanco
    //cod_referencia == 4 dato tipo alfanumerico no acepta incio cadena el valor blanco, no le importa el cero
    //cod_referencia == 5 dato tipo alfanumerico no acepta incio cadena el valor blanco ni el cero
    if( rta && ( tipo_dato == _TIPO_CADENA && cod_referencia ) ) {
        //blanco
        switch( cod_referencia ) {
            case 3:
                //no cero
                if( strncmp( valor, "0", 1 ) == 0 ) {
                    rta = 0;
                }
                break;
            case 4:
                //no blanco
                if( strncmp( valor, " ", 1 ) == 0 ) {
                    rta = 0;
                }
                break;
            case 5:
                //ni cero ni blanco
                if( ( strncmp( valor, " ", 1 ) == 0 ) || ( strncmp( valor, "0", 1 ) == 0 ) ) {
                    rta = 0;
                }
                break;
        }
        if( !rta ) {
            MENSAJE( "VALOR INCORRECTO - PRESIONE <ENTER> PARA REINTENTAR" );
            while( GETCH() != 13 );
            BORRAR_MENSAJE();
        }
    }
    return ( rta );
}
/*****************************************************************************/
int AGREGA_DATOS_MEDIO( int pos, char indice_pos, int nro_orden, int id_dato, struct _datos *datos,
                        int cod_solicitud, int origen, int primer_dato, double importe )
/*****************************************************************************/
{
    struct _datos_en_bin datos_en_bin;
    char archivo[30];
    int handle;
    int error = 1;
    BIN_DATOS_ADICIONALES( origen, pos, archivo );
    //Como en la grabacion de los cheques de SM solo hay un cheque por
    //archivo, hay que evitar que se anexen los datos de un cheque a los
    //de otro aunque el indice_pos sea mayor que cero.
    if( ( MODO_NEGOCIO == SERVICIOS ) && MODO_CHEQUE_COMO_UN_MEDIO && primer_dato
     && D_MEDIOS_PRE_IMPORTE == origen && ( pos >= 300 ) ) {
        _unlink( archivo );
    }
    if( indice_pos == 0 && primer_dato ) {
        _unlink( archivo );
    }
    if( origen == D_ITEMS && RAM_ANULACION ) {
        importe *= -1;
    }
    memset( &datos_en_bin, 0, sizeof( datos_en_bin ) );
    datos_en_bin.indice_posicion = indice_pos;
    datos_en_bin.nro_orden = nro_orden;
    datos_en_bin.id_dato = id_dato;
    memcpy( &datos_en_bin.valor, datos->valor, sizeof( datos_en_bin.valor ) );
    datos_en_bin.boletin_consultado = datos->boletin_consultado;
    datos_en_bin.cod_supervisor = datos->cod_supervisor;
    strncpy( datos_en_bin.valor_en_boletin, datos->valor_en_boletin,
             sizeof( datos_en_bin.valor_en_boletin ) );
    datos_en_bin.cod_solicitud = cod_solicitud;
    datos_en_bin.importe = importe;

    handle = _OPEN_O_CREAT( archivo, O_WRONLY | _O_BINARY, __LINE__, __FILE__ );

    if( handle >= 0 ) {
        LSEEK( handle, 0, SEEK_END, __LINE__, __FILE__ );
        error = _WRITE( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                        __FILE__ );
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return !error;
}
/*****************************************************************************/
int AGREGA_BIN_EV_DMEDIO_A_BTRV( int h, int origen )
/*****************************************************************************/
{
    struct _datos_en_bin datos_en_bin;
    char archivo[80];
    int handle;
    int salir = 0, tipo_dato = 0;
    int codigos_en_archivo = 0;
    memset( archivo, 0, sizeof( archivo ) );
    BIN_DATOS_ADICIONALES( origen, h, archivo );
    if( access( archivo, 0 ) == 0 ) {
        //handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        #ifdef INVEL_L
        handle = _OPEN( archivo, O_RDWR, __LINE__, __FILE__ );
        #else
        handle = _OPEN( archivo, O_RDWR | O_BINARY, __LINE__, __FILE__ );
        #endif
        //SELECT( AREA_DPAGO );
        SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );

            while( !salir && codigos_en_archivo > 0 ) {
                if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                            __FILE__ ) ) {
					memset( event_dpago, 0, sizeof( struct _ev_dmedios ) );

					if( !origen ) {
                        event_dpago->caja = event_pago.caja;
                        event_dpago->nro_z = event_pago.nro_z;
                        event_dpago->id_evento = event_pago.id_evento;
                        event_dpago->caja_z = event_pago.caja_z;
                    }
                    else {
                        event_dpago->caja = NRO_CAJA;//event_ticket.caja
                        event_dpago->nro_z = NRO_Z; //event_ticket.nro_z
						//event_dpago->id_evento        = event_ticket.id_evento;
						event_dpago->id_evento = RAM_ID_EVENTO;
                        event_dpago->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L ); //event_ticket.caja_z
                    }
                    //si h es >= 300 es porque es un cheque
                    if( MODO_CHEQUE_COMO_UN_MEDIO && h >= 300 ) {
                        event_dpago->pos_pago = h - ( ( event_pago.modo_pago - 1 ) * 100 ) + 1;
                    }
                    else {
                        event_dpago->pos_pago = h + 1;
                    }
                    event_dpago->indice_posicion = ( char )datos_en_bin.indice_posicion;
                    event_dpago->nro_orden = datos_en_bin.nro_orden;
                    event_dpago->id_dato = datos_en_bin.id_dato;
                    memcpy( event_dpago->valor, datos_en_bin.valor,
                        sizeof( event_dpago->valor ) - 1 );
                            event_dpago->valor[30] = 0;
					//Se comenta porque backoffice no necesita estos campos seteados para identificar el tipo de dato
					//que se grabo en el campo event_dpago->valor.
                    /*tipo_dato = TIPO_DATO_ADICIONAL( datos_en_bin.cod_solicitud, datos_en_bin.nro_orden );
                    GRABAR_TIPO_DATO_DEL_DATO_ADICIONAL( tipo_dato, event_dpago, datos_en_bin.valor );*/
					event_dpago->tipo_entero = 0;
					event_dpago->tipo_double = 0;
                    event_dpago->boletin_consultado = datos_en_bin.boletin_consultado;
                    event_dpago->cod_supervisor = datos_en_bin.cod_supervisor;
                    event_dpago->cod_sucursal = config.sucursal;
					if( !NRO_FECHA_OPERACION )
						SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

					event_dpago->fecha_ticket = NRO_FECHA_OPERACION;
                    strncpy( event_dpago->valor_en_boletin, datos_en_bin.valor_en_boletin,
                             sizeof( event_dpago->valor_en_boletin ) );
                    event_dpago->cod_solicitud = datos_en_bin.cod_solicitud;
                    event_dpago->posicion = NRO_MOZO;
                    event_dpago->importe = datos_en_bin.importe;
                    event_dpago->origen = origen;
                    #ifdef INVEL_L
                    BEGIN_TRANSACTION();
                    #endif
					{	char cad[200];
						_snprintf( cad, 200,
							"MSJ DPAGO.BTR --> ID_EVENTO %ld POSICION %d VALOR %s",
								event_dpago->id_evento, event_dpago->posicion, event_dpago->valor );
						cad[199] = '\0';
						glog(cad,LOG_VENTAS,4);
					}
					if( INSERT2(NO) ) {
						glog("NO INSERTO EVENTO EN DPAGO.BTR ",LOG_VENTAS,2);
					}
                    #ifdef INVEL_L
                    END_TRANSACTION();
                    #endif
                    codigos_en_archivo--;
                    /*Variable __nro_mozo utilizada para la posicion del dato adicional en un mismo evento*/
                    ADD_MEMORY_INT( __nro_mozo, 1 );
                }
                else {
                    salir = 1;
                }
            }
            _CLOSE( handle, __LINE__, __FILE__ );
            GRABAR_LOG_SISTEMA( "fin grab dpago.btr",LOG_VENTAS,4 );
        }
        _unlink( archivo );
    }
    return 1;
}
/***************************************************************************/
int PAGOS_VALIDA_EN_BOLETIN( struct _dmedios *datos_medio, int pos, int cant_datos, double *importe,
                             struct _datos *datos )
/***************************************************************************/
{
    int rta = 1, consulta_local;
    char mascara[40];
    datos[pos].boletin_consultado = 0;
    datos[pos].cod_supervisor = 0;
    if( datos_medio[pos].tipo_boletin ) {
        memset( mascara, ' ', sizeof( mascara ) );
        PAGOS_CONSTRUYE_MASCARA( datos_medio, datos_medio[pos].mascara_boletin, 40, mascara,
                                 cant_datos, *importe, datos );
        consulta_local = ( datos_medio[pos].boletin_en_servidor == 'N' ) ? SI : NO;
        //-------------- Busqueda en servidor -----------
        if( datos_medio[pos].boletin_en_servidor == 'S' ) {
            datos[pos].boletin_consultado = 2;
            rta = BUSCAR_MEDIO_EN_BOLETIN( ( char )datos_medio[pos].tipo_boletin,
                                           datos_medio[pos].nro_boletin, mascara, importe, NO,
                                           ( datos_medio[pos].verificar_importe == 'S' ) ? SI : NO,
                                           NO );
            if( rta < 0 ) {
                rta = 0;
                if( datos_medio[pos].permite_consulta_local == 'S' ) {
                    consulta_local = SI;
                }
            }
        }
        //---------------- Busqueda local -------------//
        if( consulta_local ) {
            rta = BUSCAR_MEDIO_EN_BOLETIN( ( char )datos_medio[pos].tipo_boletin,
                                           datos_medio[pos].nro_boletin, mascara, importe, NO,
                                           ( datos_medio[pos].verificar_importe == 'S' ) ? SI : NO,
                                           SI );
            datos[pos].boletin_consultado = 1;
            strncpy( datos[pos].valor_en_boletin, mascara, sizeof( datos[pos].valor_en_boletin ) );
        }
        if( !rta && datos_medio[pos].permite_boletin_sup == 'S' ) {
            rta = SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_BOLETIN_ ),
                                    ST( S_AUTORIZ__MEDIO_EN_BOLETIN ), _F_SUP_MEDIO_EN_BOL, SI );
            if( rta ) {
                datos[pos].cod_supervisor = USUARIO_CLAVE_ANTERIOR();
            }
        }
    }
    return ( rta );
}
/***************************************************************************/
int PAGOS_OBTENER_VALOR( int h, int nro_orden, char *cadena, int origen )
/***************************************************************************/
{
    int rta;
    rta = PAGOS_OBTENER_VALOR_POS( h, nro_orden, -1, cadena, origen );
    return ( rta );
}
/***********************************************************************************************************/
int CONFIRMAR_DATO_ADICIONAL( struct _dmedios *datos_medio, char *valor, int posicion )
/***********************************************************************************************************/
{
    char descripcion[50];
    char cadena_A[121];
    char cadena_B[121];
    char msj[80];
    int rta = 1, size_dato, seguir = 1;
    double tipo_double;
    long tipo_long;

    if( datos_medio[posicion].reingresar == 'S' ) {
        _snprintf( descripcion, sizeof( descripcion ), ST( S_REINGRESE__S ), strtok( datos_medio[posicion].descripcion, ":" ) );
        descripcion[sizeof( descripcion ) - 1 ] = '\0';
        //Hasta no ingresar un dato( correcto correcto ) o dar ESC no sale del while
        while( seguir ) {
            switch( datos_medio[posicion].tipo_dato ) {
                case _TIPO_DOUBLE:
                    tipo_double = PEDIR_FLOAT_EN_RECUADRO( descripcion, NO, ENTEROS, DECIMALES );

                    _snprintf( cadena_A, sizeof( cadena_A ),"%*.*f", ENTEROS, DECIMALES, tipo_double );
                    cadena_A[sizeof( cadena_A ) - 1] = '\0';
                    break;
                case _TIPO_NUMERICO:
               	    tipo_long = PEDIR_ENTERO_EN_RECUADRO( descripcion, NO, datos_medio[posicion].digitos );
                    _ltoa( tipo_long, cadena_A, 10 );
                    break;
                case _TIPO_CADENA:
                    rta = PEDIR_CADENA_EN_RECUADRO( descripcion, NO, datos_medio[posicion].digitos, cadena_A, NULL );
                    break;
                case _TIPO_FECHA:
                    rta = PEDIR_FECHA_EN_RECUADRO( descripcion, NO, cadena_A );
                    DTOC4( rta, cadena_A );
                    break;
            }

            TRIM( cadena_A );
            LTRIM( cadena_A );
            size_dato = strlen( valor );
            strncpy(  cadena_B, valor, size_dato );
            cadena_B[size_dato]= '\0';

            if( strcmp( cadena_B, cadena_A ) != 0 ) {
                if( LASTKEY(  ) != 27 ) {
		            _snprintf( msj, sizeof( msj ), ST( S_ERROR_REINGRESO_REINTENTAR_ENTER_SALIR_ESC ), strtok( datos_medio[posicion].descripcion, ":" ) );
                    msj[sizeof( msj ) - 1] = '\0';
                    if( MENSAJE_CON_TECLA_S_N( msj, 13, 27 ) == 27 ) {
                        seguir = 0;
                        rta = -1;
                    } else {
                        seguir = 1;
                        rta = 0;
                    }
                } else {//para ESC
                    seguir = 0;
                    rta = -1;
                }
            } else {
                if( LASTKEY(  ) != 27 ) {
                    rta = 1;
                } else {//para ESC
                    //Para TIPO_CADENA, si se ingreso el valor y luego se dio ESC queda seteado
                    rta = -1;
                }
                seguir = 0;
            }
        }
        ExecuteAction( A_FIN_PEDIR );

    }

    return ( rta );
}

/***************************************************************************/
int TIPO_DATO_ADICIONAL( int solicitud, int orden )
/***************************************************************************/
{
	int tipo_dato = 0, /*area_aux, area_ant*/ tabla_ant, tipo_ant, salir = 0;

    /*struct
    {
        int cod_solicitud;
        int nro_orden;
    } filtro;*/

    struct _dmedios dmedios;

    /*---------------------------- Abre el archivo ---------------------------*/
    //area_ant = SELECTED();
    //area_aux = AREA_DISPONIBLE();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    //if( USE_DB( AREA_AUX, _DMEDIOS_SIC, ( char* )&dmedios, sizeof( struct _dmedios ), NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( T_D_MEDIOS, TT_ORIG, ( char* )&dmedios, sizeof( struct _dmedios ) == 0 ) ) {
        //filtro.cod_solicitud = solicitud;
        //filtro.nro_orden = 0;
        //GET_GREATER_E( ( char* )&filtro );
		SET_ORDER2("COD_SOLICITUD, NRO_ORDEN");
		SET_WHERE("COD_SOLICITUD = '%d' AND NRO_ORDEN = '%d'", solicitud, 0 );
		RUN_QUERY(NO);
        while( /*!BtrvEOF()*/ !dbEOF() && !salir ) {
            if( dmedios.cod_solicitud == /*filtro.cod_solicitud*/ solicitud
                    && dmedios.nro_orden == orden ) {
                tipo_dato = dmedios.tipo_dato;
                salir = 1;
            }
            SKIP2( 1 );
        }
    }
    //CLOSE_DATABASE( AREA_AUX );
    //SELECT( area_ant );
	CLOSE_TABLE( T_D_MEDIOS, TT_ORIG );
	SELECT_TABLE( tabla_ant, tipo_ant );

    return ( tipo_dato );
}

/*****************************************************************************/
void GRABAR_TIPO_DATO_DEL_DATO_ADICIONAL( int tipo_dato, struct _ev_dmedios *dpago, char *valor )
/*****************************************************************************/
{

    /*Todos los tipos de datos se graban en el valor generico*/
    memcpy( event_dpago->valor, valor, sizeof( event_dpago->valor ) - 1 );
        event_dpago->valor[30] = 0;

    switch( tipo_dato ) {
        case _TIPO_DOUBLE:
            event_dpago->tipo_double = atof( event_dpago->valor );
            event_dpago->tipo_entero = 0;
            break;
        case _TIPO_NUMERICO:
            event_dpago->tipo_entero = atoi( event_dpago->valor );
            event_dpago->tipo_double = 0;
            break;
        case _TIPO_FECHA:
            /*Para tipo FECHA se utiliza el atributo "valor" de la dpago
                y tambien el atributo "tipo_entero"*/
            event_dpago->tipo_double = 0;
            event_dpago->tipo_entero = CADENA_A_FECHA( valor );
			break;
        case _TIPO_CADENA:
            /*Para tipo CADENA se utiliza directamente el atributo "valor" de la dpago*/
			event_dpago->tipo_entero = 0;
			event_dpago->tipo_double = 0;
            break;
    }

}

/***************************************************************************/
int PAGOS_CARGAR_DATOS_MEDIO( struct _dmedios **_datos_medio, int datos_adicionales, int origen )
/***************************************************************************/
{
    int cant_datos = 0, pos_pedido = 0 /*area_aux area_ant*/;

    struct
    {
        int cod_solicitud;
        int nro_orden;
    } filtro;
    struct _dmedios dmedios,*datos_medio;
    int area_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();

    pos_pedido = ( origen == D_MEDIOS_PRE_IMPORTE ) ? 1 : 0;
    /*---------------------------- Abre el archivo ---------------------------*/

    //if( USE_DB( AREA_AUX, _DMEDIOS_SIC, ( char* )&dmedios, sizeof( struct _dmedios ), NULL, 0, 0 )
    // == 0 ) {
	if( OPEN_TABLE( T_D_MEDIOS, TT_ORIG, ( char* )&dmedios, sizeof( struct _dmedios ) ) == 0 ) {
        filtro.cod_solicitud = datos_adicionales;
        filtro.nro_orden = 0;
        //GET_GREATER_E( ( char* )&filtro );
		SET_WHERE("COD_SOLICITUD = '%d' and NRO_ORDEN >= '%d'", filtro.cod_solicitud, filtro.nro_orden  );
		RUN_QUERY(NO);

        while( !dbEOF() && dmedios.cod_solicitud == filtro.cod_solicitud ) {
            if( *_datos_medio == NULL ) {
                *_datos_medio = ( struct _dmedios * )calloc( CANT_DATOS_MEDIOS,
                                                             sizeof( struct _dmedios ) );
                datos_medio = *_datos_medio;
            }
            if( dmedios.habilitado == 'S' && pos_pedido == dmedios.pos_pedido ) {
                memcpy( &datos_medio[cant_datos++], &dmedios, sizeof( struct _dmedios ) );
            }
            SKIP2( 1 );
        }
        //CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_D_MEDIOS, TT_ORIG );
    }
    else {
        cant_datos = -1;
    }
   // SELECT( area_ant );
	SELECT_TABLE( area_ant, tipo_ant );
    return ( cant_datos );
}
/***************************************************************************/
int PAGOS_CONSTRUYE_MASCARA( struct _dmedios *datos_medio, char *mascara_in, int tam_mascara,
                             char *mascara, int cant_datos, double importe, struct _datos *datos )
/***************************************************************************/
{
    int rta = 1, p_mas, i, j, k, var, nro;
    char format[20],desc[20],valor[40];
    for( i = 0, p_mas = 0;mascara_in[i] != ' ' && mascara_in[i] && i < tam_mascara;i++ ) {
        switch( mascara_in[i] ) {
            case '%':
                //---- Se obtiene el formato de conversion del string -----//
                for( j = i + 1, k = 0;mascara_in[j] != ']' && j < 40;j++ ) {
                    if( mascara_in[j] != '[' ) {
                        format[k++] = mascara_in[j];
                    }
                }
                format[k] = 0;
                //------ Se obtiene la variable y el descriptor ------//
                for( var = 0, k = 0, j++;!var && j < 40;j++ ) {
                    if( mascara_in[j] == 'v' || mascara_in[j] == 'i' || mascara_in[j] == 'c' ) {
                        var = mascara_in[j];
                    }
                    else {
                        desc[k++] = mascara_in[j];
                    }
                }
                desc[k] = 0;
                i = j - 1;
                //------------ Conversion de la variable ------------
                switch( var ) {
                    case 'v':
                        nro = atoi( desc );
                        for( j = 0;j < cant_datos;j++ ) {
                            if( datos_medio[j].nro_orden == nro ) {
                                strncpy( valor, datos[j].valor, sizeof( valor ) );
                                for( k = 0;k < sizeof( valor );k++ ) {
                                    if( valor[k] == ' ' ) {
                                        valor[k] = 0;
                                    }
                                }
                                sprintf( &mascara[p_mas], format, valor );
                                //---- Si el dato es numerico se rellenan con 0.
                                if( datos_medio[j].tipo_dato == 1 ) {
                                    for( k = p_mas;mascara[k];k++ ) {
                                        if( mascara[k] == ' ' ) {
                                            mascara[k] = '0';
                                        }
                                    }
                                }
                                p_mas = strlen( mascara );
                            }
                        }
                        break;
                    case 'i':
                        sprintf( &mascara[p_mas], format, importe );
                        p_mas = strlen( mascara );
                        break;
                    case 'c':
                        sprintf( &mascara[p_mas], format, NRO_CAJA );
                        p_mas = strlen( mascara );
                        break;
                }
                break;
            default:
                mascara[p_mas++] = mascara_in[i];
                break;
        }
    }
    mascara[p_mas] = 0;
    return ( rta );
}
/***************************************************************************/
int PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos )
/***************************************************************************/
{
    int rta;
    rta = _PAGOS_OBTENER_DATOS( h, posicion, datos, 0 );
    return ( rta );
}
/***************************************************************************/
int _PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos, int origen )
/***************************************************************************/
{
    struct _datos_en_bin datos_en_bin;
    char nom_aux[16],archivo[30];
    int handle, salir = 0, codigos_en_archivo = 0, rta = 0, pos = 0;
    switch( origen ) {
        case 0:
            sprintf( nom_aux, "d_tmp.%d", h );
            break;
        case 1:
            sprintf( nom_aux, "art_tmp.%d", h );
            break;
        case 2:
            sprintf( nom_aux, "cli_tmp.%d", h );
            break;
        case 3:
            sprintf( nom_aux, "d_tmp_pr.%d", h );
            break;
        case D_COBROS:
            sprintf( nom_aux, "d_cobros.%d", h );
            break;
    }
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
            while( !salir && codigos_en_archivo > 0 ) {
                if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                            __FILE__ ) ) {
                    if( datos_en_bin.indice_posicion == posicion ) {
                        rta = 1;
                        strncpy( datos[pos].valor, datos_en_bin.valor,
                                 sizeof( datos_en_bin.valor ) );
                        datos[pos].valor[30] = 0;
                        pos++;
                    }
                    codigos_en_archivo--;
                }
                else {
                    salir = 1;
                }
            }
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    //}
    return ( rta );
}
/***************************************************************************/
double PAGOS_NETO_PROPORCIONAL( int h, double importe )
/***************************************************************************/
{
    double neto = 0, prop;
    if( PAGOS_TOTAL( 1 ) ) {
        prop = importe / PAGOS_TOTAL( 1 );
        if( prop ) {
            if( h >= 0 && h < 10 ) {
                neto = _TOT_TICK_IMP_NETO_GRAVADO( h ) * prop;
            }
            else {
                neto = ( TOT_TICK_IMPORTE_SIN_IVA - TOT_TICK_IMP_INT ) * prop;
            }
        }
    }
    return ( neto );
}
/***************************************************************************/
double PAGOS_IVA_PROPORCIONAL( int h, double importe )
/***************************************************************************/
{
    double iva = 0, prop;
    if( PAGOS_TOTAL( 1 ) ) {
        prop = importe / PAGOS_TOTAL( 1 );
        if( prop ) {
            if( h >= 0 && h < 10 ) {
                iva = _TOT_TICK_IMP_TASA( h ) * prop;
            }
            else {
                iva = TOT_TICK_IVA1 * prop;
            }
        }
    }
    {
        char aux[80];
        if( iva <= 0.01 ) {
            sprintf( aux, "%lf %lf %lf %lf %lf %i", PAGOS_TOTAL( 1 ), importe, prop,
                     ( h >= 0 && h < 10 ) ? _TOT_TICK_IMP_TASA( h ) : TOT_TICK_IVA1,
                     ( h >= 0 && h < 10 ) ? _TOT_TICK_IMP_NETO_GRAVADO( h )
                   : TOT_TICK_IMPORTE_SIN_IVA, h );
            GRABAR_LOG_SISTEMA( aux ,LOG_DEBUG,4);
        }
    }
    return ( iva );
}
/***************************************************************************/
double PAGOS_EXENTO_PROPORCIONAL( double importe )
/***************************************************************************/
{
    double exento = 0, prop;
    if( PAGOS_TOTAL( 1 ) ) {
        prop = importe / PAGOS_TOTAL( 1 );
        if( prop ) {
            exento = TOT_TICK_EXENTO * prop;
        }
    }
    return ( exento );
}
/*****************************************************************************/
int PROCESAR_DONACIONES()
/*****************************************************************************/
{
    int k, salir = 0, ok = 0, rta = 1, institucion = 0;

    if( DONACION_DE_VUELTO && RAM_TOTAL_DONACION && !RAM_NOTA_CR && ( RAM_RESTO < 0 )
     && ( _PAGO_TIPO_VUELTO( RAM_P_PAGO - 1 ) == _VUELTO_EN_EFECTIVO
		||  medios[_PAGO_COD_MEDIO_VUELTO( RAM_P_PAGO - 1) - 1].efectivo ) ) {
			if( CONFIRMAR_DONACION ) {
            MENSAJE_SIN_SONIDO_STRING( S_DESEA_HACER_UNA_DONACION_ENTER_ESC );
            while( !salir ) {
                k = GETCH();
                switch( k ) {
                    case 27:
                        salir = 1;
                        break;
                    case 'N':
                    case 'n':
                        salir = 1;
                        break;
                    case 13:
                    case 'S':
                    case 's':
                        salir = 2;
                        break;
                }
            }
            if( salir == 2 ) {
                ok = 1;
            }
        }
        else {
            ok = 1;
        }
		if( ok ) {
            //Desea hacer una donacion

			if( DONACION_DE_VUELTO == 2 ) {
                //Si la donacion es por multiplo de la moneda mas chica
                for( k = 0;!medios[k].efectivo && k < 9;k++ ) {
                    ;
                }
                if( medios[k].efectivo ) {
                    if( medios[k].minima_denominacion ) {
                        rta = DONACION_INGRESA_MULTIPLO();
                    }
                    else {
                        MENSAJE_STRING( S_DEBE_ESPECIFICAR_LA_MINIMA_DENOMINACION );
                        SET_MEMORY_DOUBLE( __ram_total_donacion, 0.00 );
                    }
                }
            }
            else {
				rta = DONACION_INGRESA_IMPORTE( fabs( RAM_RESTO ) );
            }
        }
		if( ok && RAM_TOTAL_DONACION > 0 ) {
           // USE_DB( AREA_AUX, _INSTIT, ( char* )&instituciones, sizeof( instituciones ), NULL, 0,
            //        0 );
			OPEN_TABLE( T_INSTITUT, TT_ORIG, ( char* )&instituciones, sizeof( instituciones ) );
			SELECT_TABLE( T_INSTITUT, TT_ORIG );
			RUN_QUERY(NO);
            if( RECCOUNT2( NULL, NULL ) > 1 ) {
                //mas de una institucion -> muestro menu
				int mensaje = 0, salir = 0;
				char cadena[80];

				memset( cadena, 0, sizeof( cadena ) );
				while( !salir ) {
					institucion = ELEGIR_INSTITUCION();
					SET_MEMORY_INT( __ram_institucion_beneficiaria, 
							( institucion )? instituciones.cod_institucion: 0 );
					if( RAM_INSTITUCION_BENEFICIARIA <= 0 ) {
						if( !mensaje ) {
							mensaje = 1;
							MENSAJE_ID_CON_PAUSA_CONF( S_SI_NO_SELECCIONA_INSTITUCION_NO_SE_PUEDE_REALIZAR_DONACION );
							BORRAR_MENSAJE( );
							MENSAJE( ST( S_DESEA_SELECCIONAR_INSTITUCION_ENTER_ESC ) );
							k = GETCH(  );
							while( k != 13 && k != 27 ) {
								k = GETCH();
							}
							if( k == 27 ) {
								salir = 1;
								SET_MEMORY_DOUBLE( __ram_total_donacion, 0.00 );
								SET_MEMORY_INT( __ram_institucion_beneficiaria, 0 );
							}
						} else {
							MENSAJE_ID_CON_PAUSA_CONF( S_NO_SELECCIONO_INSTITUCION_NO_SE_REALIZARA_DONACION );
							salir = 1;
						}
						BORRAR_MENSAJE( );
					} else {
						salir = 1;
					}
				}
            }
            else {
                //tomo la institucion que existe
                GO2( TOP );
                SET_MEMORY_INT( __ram_institucion_beneficiaria, instituciones.cod_institucion );
            }
            //CLOSE_DB( _INSTIT );
			CLOSE_TABLE( T_INSTITUT, TT_ORIG );
            //VALIDAR_DONACION();
		}
        else {
            SET_MEMORY_DOUBLE( __ram_total_donacion, 0.00 );
        }
	} else {
		SET_MEMORY_DOUBLE( __ram_total_donacion, 0.00 );
		}
    return rta;
}
/*****************************************************************************/
int DONACION_INGRESA_MULTIPLO()
/*****************************************************************************/
{
    int rta = 0;
    double donacion = -1, minima_denominacion;
    char c_donacion[20],mensaje[60];
	double min_denom_round = 0;
	int min_denom_pow = 0;

    if( config.minima_denominacion_donacion ) {
        minima_denominacion = config.minima_denominacion_donacion;
    }
    else {
		minima_denominacion = 1.0;
    }

    min_denom_round = ROUND( minima_denominacion, ENTEROS, DECIMALES );
    min_denom_pow   = ( ( (int) ( min_denom_round * pow( 10, DECIMALES ) ) ) != 0 ) ?
                           ( (int) ( min_denom_round * pow( 10, DECIMALES ) ) ) : 0;

    if( fabs( RAM_RESTO ) >= min_denom_round ) {
		donacion =
				( ( (long) ROUND( fabs( RAM_RESTO ) * pow( 10, DECIMALES ) ,
				ENTEROS, DECIMALES ) ) % min_denom_pow ) / pow( 10, DECIMALES ) ;
		if( !donacion ) {
			donacion = ( min_denom_round <= fabs( RAM_RESTO ) ) ? min_denom_round : 0 ;
		}
	} else
		donacion = fabs( RAM_RESTO );

    memset( c_donacion, 0, sizeof( c_donacion ) );
    FTOC( donacion, c_donacion, ENTEROS, DECIMALES, 0, 0 );
    sprintf( mensaje, "%s %s", ST( S_MONTO_DONACION_SUGERIDO ), c_donacion );
    MENSAJE_SIN_SONIDO( mensaje,NO );
    rta = DONACION_INGRESA_IMPORTE( donacion );
    return ( rta );
}

/*****************************************************************************/
double CASHPLUS_INGRESA_IMPORTE(  )
/*****************************************************************************/
{
    int salir=0, rta = 1, i=0, ticket=0, j=0;
    double importe_retiro = -1;
	struct _cabmenu *cab;
	int columnas;
	struct _menu items[10];//Tiene que tener igual cant posicion que config_tps.MontosValidos[j]
	char nombres[10][80];//Tiene que tener igual cant posicion que config_tps.MontosValidos[j]
	int cant_pend = 0;
	int tarjeta = 0, extraccion = SI, tecla = 0;
	char *n, marca[50], aux[50], cadena[150];
	memset( nombres,0, sizeof(nombres));
	memset(marca,0,sizeof(marca));
	memset(aux,0,sizeof(aux));
	memset(cadena,0,sizeof(cadena));
	
	
	glog("Lo encontro1",1,1);
	glog("CASHPLUS_INGRESA_IMPORTE",1,1);

	/*Informo que tarjeta se va ha realizar la extraccion si son mas de una*/
	for( i = 0; i < RAM_P_PAGO; i++ ) {
		if( medios[( int )_PAGO_MODO( i )].tarjeta ) {
			tarjeta ++;
		}
	}

	if( tarjeta > 1 ) {
		
		if( !RAM_VENTA_FRACCION ) {
			_snprintf( cadena, 150, 
				"RECUERDE QUE SOLO PUEDE REALIZAR UN RETIRO DE TARJETA POR COMPRA",
				tarjeta );
			cadena[149] = '\0';
			ALERTA_EN_VENTANA(cadena, "ATENCION!!!",5,"tps - v" ,0);
			memset(cadena,0,sizeof(cadena));
		}

		strncpy( marca, TARJ[VAR_TARJETAS_TARJETA].nombre, sizeof( marca ) ); 
		n = GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/;
		memcpy( aux, &n[strlen( n ) - 4], 4 );
		aux[4] = 0;		
		_snprintf( cadena, 150, 
			"Desea realizar la extraccion de la Tarjeta ( %s ) ultimos digitos ( ---%s )? <ENTER>=SI <ESC>=NO",
			marca, aux );
		cadena[149] = '\0';
		tecla = MENSAJE_CON_TECLA2_S_N(cadena,13,27);
		if( tecla == 27 ) {
		    extraccion = NO;
		} 
	}

	while(config_tps.MontosValidos[j] > 0 && extraccion == SI && j< 10 ){
		j++;
	}
	VACIAR_KEY();

	if( extraccion == SI ) {
		if( j > 0) {
			for( i = -1, ticket = 0;ticket < j && i < 10 ;ticket++ ) {
				 i++;
				 sprintf( ( char* )&nombres[i][0], " %d)      %7.2lf", ticket, config_tps.MontosValidos[i]);
				 items[i].nom = ( char* )&nombres[i][0];
				 items[i].tecla = ticket + 48;   // para obtener el ascii '1',...'n'
				 items[i].rta = ticket;
				 items[i].index = i;
			}
		
			columnas = 2;
			cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
			if( cab != NULL ) {
				cab[0].nombre = "Tecla";
				cab[0].align = ALIGN_CENTER;
				cab[0].width = 8;
				cab[1].nombre = "Monto";
				cab[1].align = ALIGN_RIGHT;
				cab[1].width = 40;
				rta = MENU( "MONTO EXTRACCION", items, i + 1, cab, columnas );
						//             rta = Menu( ST( S_TICKETS_EN_ESPERA ), items, i + 1, cab, columnas );
				 free( cab );
				 cab = NULL;
				if( rta >=0 && rta < j) {
					importe_retiro = config_tps.MontosValidos[rta];
				}
			}
		} else {
			 while( !salir ) {
				ExecuteSSAction( A_PEDIR, 1, "MONTO RETIRO " , " ", NULL, " " );
				importe_retiro = PEDIR_GRAN_FLOTANTE( 32 - ancho_panel / 2, 22, ENTEROS, DECIMALES );
				if( importe_retiro >0.01)
					salir = -1;
				if( LASTKEY( )== 27 )  {
					salir = -1;
				}
				ExecuteAction( A_FIN_PEDIR );
			 }
		}
	}
	glog("FIN-CASHPLUS_INGRESA_IMPORTE",1,1);

    return ( importe_retiro );
}


/*****************************************************************************/
int DONACION_INGRESA_IMPORTE( double imp_sugerido )
/*****************************************************************************/
{
    int salir, rta = 1;
    double donacion = -1;
    salir = 0;
    while( !salir ) {
        ExecuteSSAction( A_PEDIR, 1, ST( S_MONTO_DONACION ), " ", NULL, " " );
        donacion = PEDIR_GRAN_FLOTANTE( 32 - ancho_panel / 2, 22, ENTEROS, DECIMALES );
        if( donacion == 0 && fabs( RAM_RESTO ) ) {
            donacion = imp_sugerido;
        }
        if( donacion <= fabs( RAM_RESTO ) ) {
            salir = 1;
        }
        else {
            MENSAJE_STRING( S_LA_DONACION_NO_PUEDE_SER_MAYOR_QUE_EL_VUELTO );
            BORRAR_MENSAJE();
        }
        ExecuteAction( A_FIN_PEDIR );
    }
    if( donacion >= 0 ) {
        SET_MEMORY_DOUBLE( __ram_total_donacion, donacion );
    }
    else {
		SET_MEMORY_DOUBLE( __ram_total_donacion, 0.00 );
		rta = 0;
    }
    return ( rta );
}
/*****************************************************************************/
int _consulta_instituciones( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0, y;
	int codigo = 0;
    y = 9 + dato;
    switch( comando ) {
		case _BUSCAR:
			codigo = atoi( cadena );
			SET_WHERE("COD_INSTITUCION >= %i", codigo );
			SET_ORDER2("COD_INSTITUCION");
			RUN_QUERY(NO);
			break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[20];
                _itoa( instituciones.cod_institucion, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, instituciones.desc_institucion );
			}
            #endif
            MOSTRAR_ENTERO( 7, y, "99", instituciones.cod_institucion );
            MOSTRAR_CADENA( 7 + 3, y, instituciones.desc_institucion );
            break;
        case _SKIP:
            SKIP2( dato );
            //rta = BtrvEOF() || BOF();
			 rta = dbEOF() || dbBOF();
            break;
		case _VALIDAR:
            if( dato >= '0' && dato <= '9' ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            //rta = ( dato == -1 ) ? BOF() : BtrvEOF();
			rta = ( dato == -1 ) ? dbBOF() : dbEOF();
            break;
        case _ENTER:
            rta = 1;
            break;
        case _COMPARAR:
            rta = 2;
            break;
    }
    return ( rta );
}
/*****************************************************************************/
int ELEGIR_INSTITUCION()
/*****************************************************************************/
{
    int rta;
    rta = Consulta_intituciones( _consulta_instituciones );

    return ( rta );
}
/*****************************************************************************/
void IDENTIFICAR_BILLETES()
/*****************************************************************************/
{
    int ok = 1;
    int denominacion;
    struct id_billete;
    char nro_billete[16];
    if( HABILITA_IDENTIFICACION_BILLETES ) {
        //Si se un medio de pago que requiere verificacion
        denominacion = PEDIR_GRAN_ENTERO( ( 30 - ancho_panel / 2 ), 23, 4 );
        if( !strlen( id_cliente ) ) {
            ok = PEDIR_GRAN_CADENA_NUMERICA( 30 - ancho_panel / 2, 23, 11, id_cliente );
            TRIM( id_cliente );
            if( !ok || strlen( id_cliente ) == 0 ) {
                MENSAJE_STRING( S_FAVOR_SELECCIONE_CLIENTE );
                ok = 0;
            }
        }
        if( ok ) {
            memset( nro_billete, 0, sizeof( nro_billete ) );
            ok = PEDIR_GRAN_CADENA( ( char )( 30 - ancho_panel / 2 ), ( char )23, ( char )15,
                                    nro_billete, NULL );
            TRIM( nro_billete );
            if( !ok || !strlen( nro_billete ) ) {
                MENSAJE_STRING( S_DEBE_INGRESAR_EL_NUMERO_DE_SERIE_DEL_BILLETE );
                ok = 0;
            }
        }
    }
    BORRAR_MENSAJE();
}
/***************************************************************************/
int LEER_CHEQUE( char *cod_cuenta, char *nr_banco, long *nro_cheq, int *plaza )
/***************************************************************************/
{
    int rta = 1;
    return rta;
}
/*****************************************************************************/
int CARGAR_CHEQUE_MANUAL( int al_dia, long *numero, int *banco, unsigned * fecha_vto,
                          int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono,
                          char *cuenta_banco, int *plaza, char *nrobanco, int habil, char *serie,
                          int *cod_valida_cheque, int pedir_fecha_vto )
/*****************************************************************************/
{
    int ok = 0;
    ok = PIDE_DATOS_CHEQUE( al_dia, numero, banco, fecha_vto, cod_post_cheq, cod_hora_acre, cuit,
                            telefono, cuenta_banco, cod_valida_cheque, plaza, pedir_fecha_vto );
    if( ok ) {
        _itoa( *banco, nrobanco, 10 );
        BUSCAR_CLIENTE_EN_BOLETIN( nrobanco, cuenta_banco, cuit, telefono, &habil, *numero, 0,
                                   serie, *plaza );
        if( !habil ) {
            ok = 0;
        }
    }
    return ( ok );
}
/***************************************************************************/
void MOSTRAR_PORC_INTERES()
{
    /***************************************************************************/
    char cad[50] ;
    if( muestra_interes && ( porc_interes != RAM_PORC_INTERES ) && ( RAM_PORC_INTERES != 0.0 ) ) {
        sprintf( cad, "INTERES: %02.02f%", RAM_PORC_INTERES );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_INTERES_BORRAR, " ", " ", NULL, " " );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_INTERES_AGREGAR, cad, " ", NULL, " " );
        porc_interes = RAM_PORC_INTERES;
    }
    else if( muestra_interes && RAM_PORC_INTERES == 0.0 ) {
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_INTERES_BORRAR, " ", " ", NULL, " " );
        porc_interes = 0.0;
        muestra_interes = 0;
    }
    else if( RAM_PORC_INTERES && !muestra_interes ) {
        sprintf( cad, "INTERES: %02.02f%", RAM_PORC_INTERES );
        ExecuteSSAction( A_MUESTRA_DATOS, T_DATOS_INTERES_AGREGAR, cad, " ", NULL, " " );
        porc_interes = RAM_PORC_INTERES;
        muestra_interes = 1;
    }
}
/******************************************************************/
int DAR_VUELTA_CHEQUE_IBM4610()
/******************************************************************/
{
    int rta = 1, error, i;
    //--- comando Esc F..
    char vuelta_cheque[20] = {27,'^','F',10,0};
    if( MODO_DEBUG == 15 || MODO_DEBUG == 50 ) {
        GRABAR_LOG_SISTEMA( vuelta_cheque ,LOG_DEBUG,4);
    }
    for( i = 0;vuelta_cheque[i];i++ ) {
        DRIVER( _DRV_printer, _PRN_enviar_byte, ( unsigned )vuelta_cheque[i], &error,
                sizeof( error ) );
    }
    //delay( 1000 );
    PAUSA( 80 );
    if( error ) {
        rta = 0;
    }
    return rta;
}
/***************************************************************************/
int CONTROLA_IMPORTE_T_COMPRA( int nro, int submedio, float importe )
/***************************************************************************/
{
    int ok = 1;
    struct _t_compra  t_compra;
    if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0) {
		SELECT_TABLE( T_COMPRA, TT_ORIG );
		SET_WHERE("COD_T_COMPRA = '%i'",submedio);
		RUN_QUERY(NO);
		//GET_EQUAL( ( char* )&submedio );
        if( FOUND2() ) {
            /*
            02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
            if( importe > t_compra.pago_imp_maximo ) {
               ok = 0;
               MENSAJE_STRING( S_EL_IMPORTE_EXCEDE_MAXIMO_PERMITIDO );
            }
            if( importe < t_compra.pago_imp_minimo ) {
               ok = 0;
               MENSAJE_STRING( S_EL_IMPORTE_NO_LLEGA_MINIMO_EXIGIDO );
            }
            */
        }
        else {
            ok = 0;
        }
        CLOSE_TABLE( T_COMPRA, TT_ORIG );
    }
    return ( ok );
}
/***************************************************************************/
void ELIMINA_BASES_DTOS_POR_TECLA()
/***************************************************************************/
{
    int i;
    char archivo[100];
    for( i = 5;i <= 6;i++ ) {
        sprintf( archivo, "dp%d.dat", i );
        if( access( archivo, 0 ) == 0 ) {
            _unlink( archivo );
        }
    }
}
/***************************************************************************/
int LEER_CHEQUE_IMPRESORA()
/***************************************************************************/
{
    int rta = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int t_espera = 0;
    unsigned long *t,t1;
    PRN_HABILITAR_LECTURA_CHEQUE();
    PRN_LEER_CHEQUE();
    t = ( void* )MK_FP_BIOS_PROTEGIDO( 0x6c );
    t1 = *t;
    while( !PRN_HAY_DATOS() && ( *t - t1 ) < ( T_ESPERA * 18 ) ) {
        //delay( 5 );
        //PAUSA( 1 );
        t_espera++;
    }
    if( ( *t - t1 ) < ( T_ESPERA * 18 ) ) {
        rta = LEER_DATOS_CHEQUE_IMPRESORA();
    }
    PRN_INHABILITA_SLIP();
    #endif
    return ( rta );
}
/*****************************************************************************/
int CANT_CUOTAS_MEDIO( int indice, unsigned * d_adicionales, int *cant_veces, int *nro_cuota )
/*****************************************************************************/
{
    int nro = 0;
	if( !OPEN_TABLE( T_CUO_MED, TT_ORIG, ( char* )&cuo_med_sic, sizeof( cuo_med_sic ) ) ) {
        if( BUSCA_CUO_MED( medios[indice].nro, 0 ) ) {
            //OK
            if( MOSTRAR_MENU_DE_CUOTAS_POR_MEDIO == SI ) {
                nro = ELEGIR_CUOTAS_MEDIO( medios[indice].nro, d_adicionales, cant_veces,
                                           nro_cuota );
            }
            BORRAR_MENSAJE();
        }
        CLOSE_TABLE( T_CUO_MED, TT_ORIG );
    }
    else {
        MENSAJE_STRING( S_AREA_INVALIDA );
    }
    return ( nro );
}
/*****************************************************************************/
int _elegir_cuotas_medio( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio )
/*****************************************************************************/
{
    int rta = 0;
    switch( comando ) {
        case _BUSCAR:
            MOSTRAR_CADENA( 30 - ancho_panel / 2, 15, cadena );
            indice_cuo_med.nro_cuota = atoi( cadena );
			SET_WHERE( "COD_MEDIO >= '%d' AND COD_SUBMEDIO >= '%d' AND ORDEN >= '%d'",
				indice_cuo_med.cod_medio, indice_cuo_med.cod_submedio, indice_cuo_med.nro_cuota );
			RUN_QUERY( NO );
            if( cuo_med_sic.cod_medio != indice_cuo_med.cod_medio ) {
                indice_cuo_med.nro_cuota = 99;
				SET_WHERE( "COD_MEDIO >= '%d' AND COD_SUBMEDIO >= '%d' AND ORDEN >= '%d'",
					indice_cuo_med.cod_medio, indice_cuo_med.cod_submedio, indice_cuo_med.nro_cuota );
				RUN_QUERY( NO );
                SKIP2( -1 );
            }
            break;
        case _MOSTRAR:
            #if defined(INVEL_W) || defined(INVEL_L)
            {
                char xx[20];
                _itoa( cuo_med_sic.nro_cuota, xx, 10 );
                setFieldValuepChar( b->index, 0, xx );
                setFieldValuepChar( b->index, 1, cuo_med_sic.descripcion );
            }
            #endif
            MOSTRAR_ENTERO( 30 - ancho_panel / 2, 8 + dato, "99", cuo_med_sic.nro_cuota );
            MOSTRAR_CADENA( 33 - ancho_panel / 2, 8 + dato, cuo_med_sic.descripcion );
            //MOSTRAR_CADENA( 30 - ancho_panel / 2, 15, cadena );
            break;
        case _SKIP:
            SKIP2( dato );
            rta = dbEOF() || dbBOF();
            if( !rta && cuo_med_sic.cod_medio != indice_cuo_med.cod_medio ) {
                if( dato > 0 ) {
                    indice_cuo_med.nro_cuota = 99;
					SET_WHERE( "COD_MEDIO >= '%d' AND COD_SUBMEDIO >= '%d' AND ORDEN >= '%d'",
						indice_cuo_med.cod_medio, indice_cuo_med.cod_submedio, indice_cuo_med.nro_cuota );
					RUN_QUERY( NO );
                    SKIP2( -1 );
                }
                else {
                    indice_cuo_med.nro_cuota = 0;
					SET_WHERE( "COD_MEDIO >= '%d' AND COD_SUBMEDIO >= '%d' AND ORDEN >= '%d'",
						indice_cuo_med.cod_medio, indice_cuo_med.cod_submedio, indice_cuo_med.nro_cuota );
					RUN_QUERY( NO );
                }
                rta = 1;
            }
            break;
        case _VALIDAR:
            if( isdigit( dato ) ) {
                rta = 1;
            }
            break;
        case _EXTREMO:
            rta = ( dato == -1 ) ? dbBOF() : dbEOF();
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
}
/*****************************************************************************/
int ELEGIR_CUOTAS_MEDIO( int medio, unsigned * d_adicionales, int *cant_veces, int *nro_cuota )
/*****************************************************************************/
{
    int rta = 0;
    /*---------------------------- Abre el archivo ---------------------------*/
    SET_ORDER2( "COD_MEDIO, COD_SUBMEDIO, ORDEN" );
    indice_cuo_med.cod_medio = (unsigned short)medio;
    indice_cuo_med.cod_submedio = 0;
    indice_cuo_med.nro_cuota = 0;
    SET_WHERE( "COD_MEDIO >= '%d' AND COD_SUBMEDIO >= '%d' AND ORDEN >= '%d'",
		indice_cuo_med.cod_medio, indice_cuo_med.cod_submedio, indice_cuo_med.nro_cuota );
	RUN_QUERY( NO );
    /*-------------------------- Prepara la pantalla -------------------------*/
    rta = Consulta_elegir_cuotas( _elegir_cuotas_medio );
    if( rta ) {
        rta = cuo_med_sic.cantidad;
        *cant_veces = cuo_med_sic.nro_datos_adicionales;
        *nro_cuota = cuo_med_sic.nro_cuota;
        if( cuo_med_sic.datos_adicionales ) {
            *d_adicionales = cuo_med_sic.datos_adicionales;
        }
    }
    return ( rta );
}
/*****************************************************************************/
int BUSCA_CUO_MED( char medio, char submedio )
/*****************************************************************************/
{
	SET_ORDER2( "COD_MEDIO, COD_SUBMEDIO, ORDEN" );
    indice_cuo_med.cod_medio = (unsigned short)medio;
    indice_cuo_med.cod_submedio = 0;
    indice_cuo_med.nro_cuota = 0;
	SET_WHERE( "COD_MEDIO >= '%d' AND COD_SUBMEDIO >= '%d' AND ORDEN >= '%d'",
		    indice_cuo_med.cod_medio, indice_cuo_med.cod_submedio, indice_cuo_med.nro_cuota );
	RUN_QUERY( NO );
    if( FOUND2() && ( char )cuo_med_sic.cod_medio == medio
     && ( char )cuo_med_sic.cod_submedio == submedio ) {
        return ( 1 );
    }
    else {
        return ( 0 );
    }
}
/***************************************************************************/
int PAGOS_CANTIDAD_DATOS( int h )
/***************************************************************************/
{
    char nom_aux[16],archivo[30];
    int handle, codigos_en_archivo = 0;
    sprintf( nom_aux, "d_tmp.%d", h );
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( struct _datos_en_bin );
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    return ( codigos_en_archivo );
}
/*****************************************************************************/
void INSERTAR_EN_DPAGO( struct _ev_dmedios dp_boletaplata )
/*****************************************************************************/
{
    int area_ant, tipo_dato = 0;
    area_ant = SELECTED();
    //SELECT( AREA_DPAGO );
    SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
    memset( event_dpago, 0, sizeof( struct _ev_dmedios ) );
    event_dpago->caja = dp_boletaplata.caja;
    event_dpago->nro_z = dp_boletaplata.nro_z;
    event_dpago->id_evento = dp_boletaplata.id_evento;
    event_dpago->caja_z = dp_boletaplata.caja_z;
    event_dpago->indice_posicion = dp_boletaplata.indice_posicion;
    event_dpago->id_dato = dp_boletaplata.id_dato;
    memcpy( event_dpago->valor, dp_boletaplata.valor, sizeof( event_dpago->valor ) - 1 );
    event_dpago->valor[30] = 0;
	//Se comenta porque backoffice no necesita estos campos seteados para identificar el tipo de dato
	//que se grabo en el campo event_dpago->valor.
    /*tipo_dato = TIPO_DATO_ADICIONAL( dp_boletaplata.cod_solicitud, dp_boletaplata.nro_orden );
    GRABAR_TIPO_DATO_DEL_DATO_ADICIONAL( tipo_dato, event_dpago, dp_boletaplata.valor );*/
	event_dpago->tipo_entero = 0;
	event_dpago->tipo_double = 0;
    event_dpago->boletin_consultado = dp_boletaplata.boletin_consultado;
    event_dpago->cod_sucursal = dp_boletaplata.cod_sucursal;
	if( !NRO_FECHA_OPERACION )
		SET_MEMORY_INT( __nro_fecha_operacion, GET_FECHA_DOS() );

	event_dpago->fecha_ticket = NRO_FECHA_OPERACION;
    event_dpago->cod_solicitud = dp_boletaplata.cod_solicitud;
    event_dpago->posicion = dp_boletaplata.posicion;
    event_dpago->importe = dp_boletaplata.importe;
    event_dpago->accion = dp_boletaplata.accion;
    event_dpago->nro_acumulador = dp_boletaplata.nro_acumulador;
    event_dpago->fecha_vigencia = dp_boletaplata.fecha_vigencia;
    #ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
    if( /*INSERT()*/ INSERT2( NO ) ) {
        GRABAR_LOG_SISTEMA_STRING( S_ERROR_GRABANDO_EV_DPAGO_BTR, LOG_ERRORES,1  );
        MENSAJE_STRING( S_ERROR_GRABANDO_EV_DPAGO_BTR );
    }
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    SELECT( area_ant );
}
/*****************************************************************************/
int PAGOS_F_DATOS_ADICIONALES( int pos, int ind_pos, int nro, int pos_actual, double importe,
                               struct _dmedios *datos_medio, char *buffer )
/*****************************************************************************/
{
    int rta = 0, fecha;
    if( indice_cuo_med.cod_medio && cuo_med_sic.cod_medio == indice_cuo_med.cod_medio ) {
        //---- Fecha de Vto
        if( datos_medio[pos_actual].tipo_dato == 3
         && datos_medio[pos_actual].id_dato == cuo_med_sic.id_dato_fecha ) {
            fecha = CALCULAR_FECHA_VTO_CUO_MED( nro );
            DTOC4( fecha, buffer );
            rta = 1;
        }
    }
    return( rta );
}
/*****************************************************************************/
int CALCULAR_FECHA_VTO_CUO_MED( int cuota )
/*****************************************************************************/
{
    int fecha;
    fecha = INC_DIA( _GET_FECHA_DOS(), cuo_med_sic.offset_dias_primera_cuota );
    fecha = INCREMENTAR_MES( fecha, cuo_med_sic.offset_meses_primera_cuota );
    fecha = INCREMENTAR_MES( fecha, cuota - 1 );
    return( fecha );
}
/***************************************************************************/
int PAGOS_CANTIDAD_DATOS_ADICIONALES( int h )
/***************************************************************************/
{
    char nom_aux[16],archivo[30];
    int handle, codigos_en_archivo = 0, id_primero = 0, cantidad = 0, salir = 0;
    struct _datos_en_bin datos_en_bin;
    sprintf( nom_aux, "d_tmp.%d", h );
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( struct _datos_en_bin );
            while( !salir && codigos_en_archivo > 0 ) {
                if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                            __FILE__ ) ) {
                    if( !id_primero ) {
                        id_primero = datos_en_bin.id_dato;
                    }
                    if( id_primero == datos_en_bin.id_dato ) {
                        cantidad++;
                    }
                    codigos_en_archivo--;
                }
                else {
                    salir = 1;
                }
            }
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    return ( cantidad );
}
/***************************************************************************/
int PAGOS_OBTENER_VALOR_POS( int h, int nro_orden, int ind_posicion, char *cadena, int origen )
/***************************************************************************/
{
    struct _datos_en_bin datos_en_bin;
    char archivo[30];
    int handle;
    int salir = 0;
    int codigos_en_archivo = 0;
    int rta = 0;
    BIN_DATOS_ADICIONALES( origen, h, archivo );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        //SELECT( AREA_DPAGO );
        SELECT_TABLE( T_EV_DMEDIOS, tipo_ev_dpago );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
            while( !salir && codigos_en_archivo > 0 ) {
                if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                            __FILE__ ) ) {
                    if( datos_en_bin.nro_orden == nro_orden
                     && ( datos_en_bin.indice_posicion == ind_posicion || ind_posicion == -1 ) ) {
                        rta = 1;
                        strncpy( cadena, datos_en_bin.valor, sizeof( datos_en_bin.valor ) );
                        salir = 1;
                    }
                    codigos_en_archivo--;
                }
                else {
                    salir = 1;
                }
            }
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    //}
    return ( rta );
}
/***************************************************************************/
int PAGOS_IMPRIMIR_TICKET_COMPLETO( int modo )
/***************************************************************************/
{
    int ok = 1, cabecera = 1;
    if( modo != _MODO_COBRANZA && modo != _MODO_NC && !EMITIR_FACTURA()
     && IMPRIMIR_TICKET_AL_FINAL ) {
		 if( IMPRIMIR_CABECERA_PRINCIPIO ) {
			cabecera = 0;
		 }
		 ok = IMPRIMIR_TICKET_COMPLETO( cabecera );
    }
    return( ok );
}
/***************************************************************************/
int SET_DATO_ADICIONAL( int h, int posicion, int origen, int id_dato, char *valor )
/***************************************************************************/
{
    int ok = 0;
    struct _datos_en_bin datos_en_bin;
    char archivo[30];
    int handle, salir = 0, codigos_en_archivo = 0, rta = 0, pos = 0;
    char nom_aux[16];
    switch( origen ) {
        case 0:
            sprintf( nom_aux, "d_tmp.%d", h );
            break;
        case 1:
            sprintf( nom_aux, "art_tmp.%d", h );
            break;
        case 2:
            sprintf( nom_aux, "cli_tmp.%d", h );
            break;
        case 3:
            sprintf( nom_aux, "d_tmp_pr.%d", h );
            break;
    }
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDWR, __LINE__, __FILE__ );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
            while( !salir && codigos_en_archivo > 0 ) {
                if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                            __FILE__ ) ) {
                    if( datos_en_bin.indice_posicion == posicion
                     && datos_en_bin.id_dato == id_dato ) {
                        rta = 1;
                        salir = 1;
                        ok = LSEEK( handle, pos * sizeof( datos_en_bin ), SEEK_SET, __LINE__,
                                    __FILE__ );
                        ok = _WRITE( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ),
                                     __LINE__, __FILE__ );
                    }
                    codigos_en_archivo--;
                    pos++;
                }
                else {
                    salir = 1;
                }
            }
            _CLOSE( handle, __LINE__, __FILE__ );
        }
    }
    return( ok );
}
/******************************************************************************/
void BORRAR_BIN_DATOS_ADICIONALES( int origen )
/******************************************************************************/
{
    int i;
    char archivo[100];
    for( i = 0;i < 1000;i++ ) {
        BIN_DATOS_ADICIONALES( origen, i, archivo );
        _unlink( archivo );
    }
}
/******************************************************************************/
void BIN_DATOS_ADICIONALES( int origen, int h, char *archivo )
/******************************************************************************/
{
    char nom_aux[30];
    memset( nom_aux, 0, sizeof( archivo ) );
    switch( origen ) {
        case D_MEDIOS:
            sprintf( nom_aux, "d_tmp.%d", h );
            break;
        case D_ITEMS:
            sprintf( nom_aux, "art_tmp.%d", h );
            break;
        case D_CLIENTES:
            sprintf( nom_aux, "cli_tmp.%d", h );
            break;
        case D_MEDIOS_PRE_IMPORTE:
            sprintf( nom_aux, "d_tmp_pr.%d", h );
            break;
        case D_COBRO_SERVICIOS:
            sprintf( nom_aux, "d_c_serv.%d", h );
            break;
        case D_ENTRY:
            sprintf( nom_aux, "d_entry.%d", h );
            break;
        case D_ENVIO_DOMICILIO:
            sprintf( nom_aux, "d_e_dom.%d", h );
            break;
        case D_CAMBIO_MEDIO:
            sprintf( nom_aux, "d_c_med.%d", h );
            break;
        case D_VALIDACIONES:
            sprintf( nom_aux, "d_valida.%d", h );
            break;
        case D_IMPRESION:
            sprintf( nom_aux, "d_impre.%d", h );
            break;
        case D_COBROS:
            sprintf( nom_aux, "d_cobros.%d", h );
            break;
    }
    sprintf( archivo, "%s%s", PATH_TMP_DADIC, nom_aux );
}
/******************************************************************************/
void BORRAR_DATOS_ADICIONALES( int pos, int origen )
/******************************************************************************/
{
    char archivo[30];
    BIN_DATOS_ADICIONALES( origen, pos, archivo );
    _unlink( archivo );
}

/******************************************************************************/
int ELEGIR_OPCION_GRILLA( int cant, char *titulo )
/******************************************************************************/
{
    int seguir = 1, k, pos = 0;
    ExecuteSSAction( A_MOSTRAR_GRILLA, PERMITE_SCROLL, titulo, " ", NULL, " " );
    ExecuteSAction( A_SELECCION_FILA_GRILLA, "0" );
    while( seguir ) {
        k = GETCH();
        switch( k ) {
            case 27:
                /*-------- ESC -----------*/
                seguir = 0;
                pos = -1;
                break;
            case 32:
                /*-------- TOTAL ---------*/
            case 13:
                /*-------- ENTER ---------*/
                seguir = 0;
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSAction( A_SELECCION_FILA_GRILLA, "-1" );
                if( pos > 0 ) {
                    pos--;
                }
                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSAction( A_SELECCION_FILA_GRILLA, "1" );
                if( pos < cant - 1 ) {
                    pos++;
                }
                break;
        }
    }
    ExecuteAction( A_FIN_GRILLA );
    return ( pos );
}
/******************************************************************************/
int VALIDA_CODIGO( int validar, char *dato )
/******************************************************************************/
 //cod_valida = 1 validar unidad de negocio
 //cod_valida = 2 validar compa�ia o codigo de cobro
 //cod_valida = 3 validar prepaga
 //cod_valida = 4 validar tipo de cliente (0:ind, 1:emp, 2:os)
{
    char *prep[] = _Des_Prepaga;
    char *tipocli[] = _Des_TipoCli;
    int cod_prep[] = _Cod_Prepaga;
    int cod_cli[] = _Cod_TipoCli;
    char aux[10] = {""};
    int /*anterior,*/ tabla_ant, tipo_ant, selected = -1, rta = -1, codigo_result = -1, cantidad = 0;
    int i,arraySize;
    char codigoString[15],tituloVentana[15];
    if( validar ) {
        rta = 0;
        //anterior = SELECTED();
		tabla_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();
        //inicializacion
        switch( validar ) {
            case 1:
            case 2:
                //SELECT( AREA_COBROS );
                //GO( TOP );
                //GET_FIRST();
				SELECT_TABLE( T_COBROS, TT_ORIG );
				SET_WHERE("");
				RUN_QUERY(NO);
                break;
        }
        //proceso
        switch( validar ) {
            case 1:
                //Modificar en caso de que sea mas de una unidad
                //while( !BtrvEOF(  ) && !BOF(  ) ){
                cantidad ++;
                if( cobros.grupo_cobro == 4 ) {
                    sprintf( aux, "PREPAGA" );
                }
                sprintf( codigoString, "%d %s", ( int )cobros.grupo_cobro, aux );
                ExecuteSAction( A_AGREGAR_FILA_GRILLA, codigoString );
                //   GET_NEXT( );
                //}
                strcpy( tituloVentana, "Unidad Neg" );
                break;
            case 2:
                while( /*!BtrvEOF()*/!dbEOF() && /*!BOF()*/ !dbBOF() ) {
                    if( !cobros.cobro_a_cuenta ) {
                        cantidad ++;
                        sprintf( codigoString, "%d %s", ( int )cobros.codigo, cobros.nombre );
                        ExecuteSAction( A_AGREGAR_FILA_GRILLA, codigoString );
                    }
                    //GET_NEXT();
					SKIP2( 1 );
                }
                strcpy( tituloVentana, "Compa�ia" );
                break;
            case 3:
                arraySize = sizeof( prep ) / sizeof( *prep );
                for( i = 0;i < arraySize;i++ ) {
                    cantidad ++;
                    ExecuteSAction( A_AGREGAR_FILA_GRILLA, prep[i] );
                }
                strcpy( tituloVentana, "Cod Prepaga" );
                break;
            case 4:
                arraySize = sizeof( tipocli ) / sizeof( *tipocli );
                for( i = 0;i < arraySize;i++ ) {
                    cantidad ++;
                    ExecuteSAction( A_AGREGAR_FILA_GRILLA, tipocli[i] );
                }
                strcpy( tituloVentana, "Tipo Cliente" );
                break;
            case 5:
                ExecuteSAction( A_OCULTAR_PAN_PEDIR, "si" );
                codigo_result = PIDE_BANCO( "" );
                ExecuteSAction( A_OCULTAR_PAN_PEDIR, " " );
                break;
        }
        //finalizacion
        switch( validar ) {
            case 1:
                selected = ELEGIR_OPCION_GRILLA( cantidad, tituloVentana );
                if( selected >= 0 ) {
                    //GO( TOP );
                    //SKIP( selected );
					GO2( TOP );
					SKIP2( selected );
                    codigo_result = ( int )cobros.grupo_cobro;
                    sprintf( dato, "%s%d", ( codigo_result < 9 ) ? "0" : "", codigo_result );
                    rta = 1;
                } else {//presion� escape
                    sprintf( dato, "0" );
                    rta = 1;
                }
                //SELECT( anterior );
				SELECT_TABLE( tabla_ant, tipo_ant );
                break;
            case 2:
                selected = ELEGIR_OPCION_GRILLA( cantidad, tituloVentana );
                if( selected >= 0 ) {
                    GO2( TOP );
                    SKIP2( selected );
                    codigo_result = ( int )cobros.codigo;
                    sprintf( dato, "%s%d", ( codigo_result < 9 ) ? "0" : "", codigo_result );
                    rta = 1;
                } else {//presion� escape
                    sprintf( dato, "0" );
                    rta = 1;
                }
                //SELECT( anterior );
				SELECT_TABLE( tabla_ant, tipo_ant );
                break;
            case 3:
                selected = ELEGIR_OPCION_GRILLA( cantidad, tituloVentana );
                if( selected >= 0 ) {
                    sprintf( dato, "%s%d", ( cod_prep[selected] < 9 ) ? "0" : "",
                             cod_prep[selected] );
                    rta = 1;
                } else {//presion� escape
                    sprintf( dato, "" );//no puede ser cero porq el cero es valido para SMG
                    rta = 1;
                }
                break;
            case 4:
                selected = ELEGIR_OPCION_GRILLA( cantidad, tituloVentana );
                if( selected >= 0 ) {
                    sprintf( dato, "%s%d", ( cod_cli[selected] < 9 ) ? "0" : "",
                             cod_cli[selected] );
                    rta = 1;
                } else {//presion� escape
                    sprintf( dato, "0" );
                    rta = 1;
                }
                break;
            case 5:
                if( codigo_result != 0 ) {
                    sprintf( dato, "%d", codigo_result );
                    rta = 1;
                }
                else {
                    rta = 0;
                }
                break;
        }
    }
    return rta;
}

//(Nota Unific Linux-MySQL): Se comenta ya que solo se usa en servicios (cobos_esp) y su uso esta comentado
/******************************************************************************
void CARGAR_EV_DMEDIOS( int cod_sol, int nro_orden, long id_evento, int origen, int area_ac,
                        struct _ev_dmedios *ev_dpago )
/******************************************************************************
{
    int tabla_ant, tipo_ant, pos = 0;
    struct _datos datos_aux;
    char archivo[30];
    memset( &datos_aux, 0, sizeof( datos_aux ) );
    SET_MEMORY_INT( __ram_cant_datos_adic_medio, 0 );
    BIN_DATOS_ADICIONALES( origen, 0, archivo );
    _unlink( archivo );
    //area = SELECTED();
    tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    //SELECT( area_ac );
    //SET_ORDER( 1 );
    //GET_EQUAL( ( char* )&id_evento );
	SELECT_TABLE( );

    while( !BtrvEOF() && id_evento == ev_dpago->id_evento ) {
        //if ( ev_dpago->cod_solicitud == cod_sol &&
        //     ev_dpago->nro_orden == nro_orden ){
        memcpy( &datos_aux.valor, ev_dpago->valor, sizeof( datos_aux.valor ) );
        AGREGA_DATOS_MEDIO( ev_dpago->pos_pago - 1, 0, ev_dpago->nro_orden, 0, &datos_aux,
                            ev_dpago->cod_solicitud, origen, ( pos == 0 ), 0.0 );
        pos++;
        ADD_MEMORY_INT( __ram_cant_datos_adic_medio, 1 );
        //}
        SKIP( 1 );
    }
    CLOSE_DATABASE( area_ac );
    //SELECT( area );
	SELECT_TABLE( tabla_ant, tipo_ant );
}*/

/***************************************************************************/
int CHEQUES_CARGADOS( int modoPago, int cantidad, int pos, double *importe, int dato_adic,
                      double totCheque, int *cnt, int *_pos )
/***************************************************************************/
{
    int y = 0, i = 0, a = 0, h = 0;
    int pos_pago_actual = RAM_P_PAGO - 1;
    double importeTmp = 0;
    struct _datos datos[5];
    int seguir = 0, nroCheque, rta = 0, k;
    char archivo[30];
    char archivo2[30];
    int anterior = 0;
	MENSAJE_SIN_SONIDO( "VERIFICACION DE CHEQUES CARGADOS",NO );
    if( REGISTRAR_CHEQUES_EN_ARCHIVO ) {
        // recorro todos los dd_tmp_pr.xxx para encontrar los cheques
        // y los elimino de cliecheq.btr
        h = modoPago;
        for( a = 0;a < cantidad;a++ ) {
            // con esto obtengo el xxx de dd_tmp_pr.xxx
            i = ( h * 100 ) + a;
            if( _PAGOS_OBTENER_DATOS( i, pos, &datos[0], D_MEDIOS_PRE_IMPORTE ) ) {
                agregarCheque( datos[0].valor, datos[1].valor, datos[2].valor, datos[3].valor,
                               datos[4].valor );
                seguir++;
            }
        }
    }
    if( seguir ) {
        seguir--;
        ExecuteAction( A_ACTIVAR_PANEL_CHEQUES );
        rta = ManipularCheques( seguir, &nroCheque );
        i = ( ( modoPago * 100 ) + nroCheque );
        if( rta == 2 ) {
            //borrar cheque
            if( cantidad > 1 ) {
                //obtiene el importe
                MENSAJE_SIN_SONIDO( "ESTA SEGURO QUE QUIERE ELIMINAR EL CHEQUE? S/N",NO );
                k = GETCH();
                if(k == 127 || k == 130){ // s o S
                    _PAGOS_OBTENER_DATOS( i, pos, &datos[0], D_MEDIOS_PRE_IMPORTE );
                    *importe = atof(( datos[4] ).valor) * -1;
                    SACAR_CHEQUE(pos, i,((modoPago * 100) + seguir));
                    *cnt -= 1;
                    *_pos -= 1;
                }else{
                    rta = 1;
                    *importe = 0;
                }
            }else{
                MENSAJE( "NO SE PUEDE ELIMINAR EL ULTIMO CHEQUE" );
                rta = 1;
                *importe = 0;
            }
        }else if(rta == 1){ //editar
            GRABA_CHEQUE_CLIENTE( pos, i, SI ); //borra el registro de la base
            //obtiene el importe
            _PAGOS_OBTENER_DATOS( i, pos, &datos[0], D_MEDIOS_PRE_IMPORTE );
            *importe = atof( ( datos[4] ).valor );
            /*Obtiene el nombre del archivo de dato adicional del cheque*/
            BIN_DATOS_ADICIONALES( D_MEDIOS_PRE_IMPORTE, i, archivo );
            sprintf( archivo2, "%s.tmp", archivo );
            /*guarda el archivo renombrandolo*/
            rename(archivo, archivo2);
            PAGOS_PEDIR_DATOS_ADICIONALES( i, ( char )pos, dato_adic,
                                           &importeTmp, D_MEDIOS_PRE_IMPORTE, NULL, 1,
                                           NULL, NULL , '\0', '\0');
            *importe = (importeTmp > 0)?(importeTmp - *importe):0;
            /*controla el importe*/
            if( ( RAM_RESTO - ( *importe + totCheque ) ) < -0.001 ) {
                MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
                anterior = 1;
            }else{
                if( importeTmp > 0 ) {
                    /*Si no excedio el importe, controla la existencia*/
                    if( !GRABA_CHEQUE_CLIENTE( pos, i, NO ) ) {
                        anterior = 1;                   //regresa al archivo anterior
                    }else{
                        _unlink( archivo2 );               //borra la copia
                    }
                }else{
                    anterior = 1;
                }
            }
            if( anterior ) {
                /*regresa al archivo anterior*/
                _unlink( archivo );
                rename( archivo2, archivo );
                GRABA_CHEQUE_CLIENTE( pos, i, NO ); //Lo graba de nuevo
                *importe = 0;
            }
        }
        ExecuteAction( A_DESACTIVAR_PANEL_CHEQUES );
    }
    BORRAR_MENSAJE();
    return rta;
}
//***************************************************************************/
void SACAR_CHEQUE( int pos, int arch, int totCheques )
//***************************************************************************/
{
    char archivo[30];
    char archivo2[30];
    int cnt;
    /*primero lo saca de la clie_cheq*/
    GRABA_CHEQUE_CLIENTE( pos, arch, SI ); //borra el registro de la base
    /*ahora el archivo*/
    BIN_DATOS_ADICIONALES( D_MEDIOS_PRE_IMPORTE, arch, archivo );
    _unlink( archivo );
    for( cnt = ( arch + 1 );cnt <= totCheques;cnt++ ) {
        BIN_DATOS_ADICIONALES( D_MEDIOS_PRE_IMPORTE, cnt, archivo2 );
        rename( archivo2, archivo );
        strcpy( archivo, archivo2 );
    }
}
/***********************************************************************/
int ManipularCheques( int cant, int *nroCheque )
/***********************************************************************/
{
    int seguir = 1, k, rta = 0, cont = 0, salir = 0;
    int grilla = A_SCROLL_CHEQUES;
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
            case 13:
                /*-------- ENTER ---------*/
                seguir = 0;
                ExecuteSAction( grilla, "-99" ); //-99 deshabilita
                rta = 1;  //editar
                break;
            case -72:
                /*----- FLECHA ARRIBA -----*/
                ExecuteSAction( grilla, "-1" );
                salir = 0;
                if( cont > 0 ) {
                    rta = 1;
                    cont--;
                }
                break;
            case -80:
                /*------ FLECHA ABAJO -----*/
                ExecuteSAction( grilla, "1" );
                salir = 0;
                if( cont < cant ) {
                    rta = 1;
                    cont++;
                }
                break;
            case 45:
                /*------ '-' -----*/
                seguir = 0;
                rta = 2;  //borrar
        }
    }
    *nroCheque = cont;
    return ( rta );
}
/***************************************************************************/
double MUTUALES_ME_A_ML_COT( double importe_me, double cotizacion )
/***************************************************************************/
{
    double importe_ml;
    importe_ml = importe_me;
    cotizacion = cotizacion ? cotizacion : 1;
    importe_ml = ROUND( importe_me * cotizacion, 10, DECIMALES );
    return ( importe_ml );
}
/***************************************************************************/
double MUTUALES_ML_A_ME_COT( double importe_me, double cotizacion )
/***************************************************************************/
{
    double importe_ml;
    importe_ml = importe_me;
    cotizacion = ROUND( cotizacion, 10, DECIMALES ) ? cotizacion : 1;
    importe_ml = ROUND( importe_me / cotizacion, 10, DECIMALES );
    return ( importe_ml );
}
/***************************************************************************/
int MUTUAL_MONEDA_EXTRANJERA( int h )
/***************************************************************************/
{
    //struct    _t_mutuales mutuales;
    int ok = 0;
    if( /*USE_DB( AREA_AUX, mutuales, ( char* )&mutuales, sizeof( mutuales ), NULL, 0, 0 )*/
		OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutuales,sizeof(  struct _t_mutuales ))== 0 ) {
        /*SELECT( AREA_AUX );
        GO( TOP );
        SET_ORDER( 1 );
        GET_EQUAL( ( char* )&h );*/
		//SELECT_TABLE( T_MUTUALES, TT_ORIG );
		SET_WHERE("COD_MUTUAL = '%i'",h);
		RUN_QUERY(NO);
        if( FOUND2() && mutuales.moneda_extranjera == SI ) {
            ok = mutuales.cod_mutual;
        }
        CLOSE_TABLE( T_MUTUALES, TT_ORIG );
    }
    return ( ok );
}
/***************************************************************************/
double MUTUALES_ME_A_ML( double importe_me, int h )
/***************************************************************************/
{
	double    importe_ml, cotizacion;
    //struct    _t_mutuales mutuales;
   importe_ml = importe_me;
   //if( USE_DB( AREA_AUX, mutuales,( char * ) &mutuales, NULL, 0, 0, sizeof( mutuales ) ) == 0 ) {
   if( /*USE_DB( AREA_AUX, mutuales, ( char* )&mutuales, sizeof( mutuales ), NULL, 0, 0 )*/
	   OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutuales,sizeof( mutuales ))== 0 ) {
     /*GO( TOP );
     SET_ORDER( 1 );
     GET_EQUAL( ( char * ) &h );*/
	//SELECT_TABLE( T_MUTUALES, TT_ORIG );
	SET_WHERE("COD_MUTUAL = '%i'",h);
	RUN_QUERY(NO);
     if( FOUND2( ) )
       cotizacion = mutuales.cotizacion ? mutuales.cotizacion : 1.0;
	    importe_ml = ROUND( importe_me * cotizacion, 10, DECIMALES );
     CLOSE_TABLE( T_MUTUALES, TT_ORIG );
   }
	return ( importe_ml );
}
/***************************************************************************/
double MUTUALES_ML_A_ME( double importe_me, int h )
/***************************************************************************/
{
    double importe_ml = 0.0,cotizacion = 0.0;
    //struct    _t_mutuales mutuales;
    importe_ml = importe_me;
    if( /*USE_DB( AREA_AUX2, mutuales, ( char* )&mutuales, sizeof( mutuales ), NULL, 0, 0 )*/
		OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutuales,sizeof( mutuales ))== 0 ) {
        /*GO( TOP );
        SET_ORDER( 1 );
        GET_EQUAL( ( char* )&h );*/
		//SELECT_TABLE( T_MUTUALES, TT_ORIG );
		SET_WHERE("COD_MUTUAL = '%i'",h);
		RUN_QUERY(NO);
        if( FOUND2() ) {
            cotizacion = mutuales.cotizacion ? mutuales.cotizacion : 1.0;
        }
        importe_ml = ROUND( importe_me / cotizacion, 10, DECIMALES );
        CLOSE_TABLE( T_MUTUALES, TT_ORIG );
    }
    return ( importe_ml );
}
/***************************************************************************/
int PAGOS_MEDIO_HABILITADO_COBRO( int cod_medio, int cod_submedio, int dev_cobro )
/***************************************************************************/
{
	int 		rta = 0, area_ant, selected_tipo;
	struct		_cob_med cob_med;

	if( ( !dev_cobro && medios[cod_medio-1].hab_cobro == 'M' ) ||
		( dev_cobro && medios[cod_medio-1].hab_cobro_dev == 'M' ) ) {

		area_ant = SELECTED2(  );
		selected_tipo = SELECTED_TIPO();
		if( OPEN_TABLE( T_COB_MED, TT_ORIG, ( char* )&cob_med, sizeof( cob_med ) ) == 0 ) {
			SELECT_TABLE(T_COB_MED, TT_ORIG);
			if( cod_submedio ) {
				struct {
					char    cod_medio;
					INT16   cod_submedio;
					INT16   cod_cobro;
				} filtro;
				SET_ORDER2( "COD_MEDIO, COD_SUBMEDIO, COD_COBRO" );
				filtro.cod_medio    = ( char ) cod_medio;
				filtro.cod_submedio = ( short ) cod_submedio;
				filtro.cod_cobro    = cobros.codigo;
				SET_WHERE( "COD_MEDIO >= '%d' AND COD_SUBMEDIO >= '%i' AND COD_COBRO >= '%i'",
					filtro.cod_medio, filtro.cod_submedio, filtro.cod_cobro );
				RUN_QUERY( NO );
				if( cob_med.cod_cobro == filtro.cod_cobro && cob_med.cod_medio == ( char ) cod_medio &&
						( cob_med.cod_submedio == filtro.cod_submedio ) ) {
					
					if( ( dev_cobro && ES_VERDADERO(cob_med.hab_devo))|| 
						( !dev_cobro && ES_VERDADERO(cob_med.habilitado))){
						rta = 1;
					}
				}
			} else {
				struct {
					char    cod_medio;
					INT16   cod_cobro;
					char    hab;
				} filtro;

				if( dev_cobro ) {
					SET_ORDER2( "COD_MEDIO, COD_COBRO, HAB_DEVO" ); //cod_medio, cod_cobro, hab_dev
				} else {
					SET_ORDER2( "COD_MEDIO, COD_COBRO, HABILITADO" ); //cod_medio, cod_cobro, habilitado
				}
				filtro.cod_medio    = (char) cod_medio;
				filtro.cod_cobro    = cobros.codigo;
				filtro.hab          = (char) 1;
				
				SET_WHERE( "COD_MEDIO >= '%d' AND COD_COBRO >= '%i' ",
					filtro.cod_medio, filtro.cod_cobro, filtro.hab );
				
				RUN_QUERY( NO );

//				CAMBIAR EL TIPO DE DATO  EN LA POS_DBMAP. A 6 PARA QUE SEA BINARY
				if( cob_med.cod_cobro == filtro.cod_cobro &&
						cob_med.cod_medio == ( char ) cod_medio ) {
					if( ( dev_cobro && ES_VERDADERO(cob_med.hab_devo))|| 
						( !dev_cobro && ES_VERDADERO(cob_med.habilitado))){
						rta = 1;
					}
				}
			}
			CLOSE_TABLE( T_COB_MED, TT_ORIG );
		}
		SELECT_TABLE( area_ant, selected_tipo );
	} else if( ( !dev_cobro && medios[cod_medio-1].hab_cobro == 'S' ) ||
		( dev_cobro && medios[cod_medio-1].hab_cobro_dev == 'S' ) ) {
		rta = 1;
	}
	return( rta );
}
/***********************************************************************/
int GET_MEDIO_ACTIVADO_PROMO(struct activacion_medios *act_medio, unsigned short medio, unsigned short sub_medio, char plan,
                             int SumarMonto, double *SumaMonto)
/***********************************************************************/
{
    int cant = 0;
    int rta = 0;
    int cnt = 0;
    cant = CANTIDAD_REGISTROS_REC( ARCH_REC_ACT_MEDIO_MOTOR, sizeof( struct activacion_medios ) );
    while(cnt < cant)
    {
        RECUPERAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)act_medio, sizeof(struct activacion_medios), cnt);
        if(act_medio->tipo == ACTIVAR_MEDIO
			&& act_medio->medio == medio
			&& act_medio->submedio == sub_medio && act_medio->plan == plan
			&& (act_medio->monto - act_medio->monto_usado) > 0){
            rta = 1;
            if(!SumarMonto){ //si no tiene que sumar los montos setea cnt para salir del while
                cnt = cant;
            }else{
                *SumaMonto += act_medio->monto - act_medio->monto_usado;
            }
        }
        cnt++;
    }
    return rta;
}
/***********************************************************************/
int MEDIO_ACTIVADO_PROMO(unsigned short sub_medio, char plan, unsigned short cod_medio )
/***********************************************************************/
{
    struct activacion_medios act_medio;
	return GET_MEDIO_ACTIVADO_PROMO(&act_medio, cod_medio, sub_medio, plan, NO, NULL);
}
/***********************************************************************/
double MONTO_MEDIO_ACTIVADO_PROMO(unsigned short sub_medio, char plan, unsigned short cod_medio )
/***********************************************************************/
{
    /*retorna el monto total para ese submedio plan, sumando los montos de las
      distintas promociones que activaron el mismo plan*/
    double rta = 0;
    struct activacion_medios act_medio;
    GET_MEDIO_ACTIVADO_PROMO(&act_medio, cod_medio, sub_medio, plan, SI, &rta);
    return rta;
}
/***********************************************************************/
int ACT_MONTO_MEDIO_ACTIVADO_PROMO( unsigned short pSub_medio, char pPlan, double pMonto_usado,
                                   unsigned short cod_medio )
/***********************************************************************/
{
    int cant = 0;
    int rta = 0;
    int cnt = 0;
    int cod_promo = 0;
    struct activacion_medios act_medio;
    double resto_monto = 0;
    resto_monto = pMonto_usado;
    cant = CANTIDAD_REGISTROS_REC( ARCH_REC_ACT_MEDIO_MOTOR, sizeof( struct activacion_medios ) );
    while(cnt < cant && resto_monto)
    {
        RECUPERAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)&act_medio, sizeof(struct activacion_medios), cnt);

		if(act_medio.tipo == ACTIVAR_MEDIO && act_medio.medio == cod_medio &&
			act_medio.submedio == pSub_medio &&
            act_medio.plan == pPlan && act_medio.monto > act_medio.monto_usado){
            if((act_medio.monto - act_medio.monto_usado) > 0){ //si tiene monto para descontar
                //si tiene monto para descontar
                if( ( act_medio.monto - act_medio.monto_usado ) >= resto_monto ) {
                    //Si le alcanza el total
                    //Actualiza el monto usado de esa promo con el total del resto
                    ACT_MONTO_PROMO(act_medio.cod_promo, resto_monto, SI, cod_medio, pSub_medio );
                    resto_monto = 0; //Ya sale del while
                }else{
                    //Actualiza el monto usado de esa promo con lo que le alcanza
                    ACT_MONTO_PROMO(act_medio.cod_promo, (act_medio.monto - act_medio.monto_usado), SI, cod_medio, pSub_medio );
                    resto_monto -= (act_medio.monto - act_medio.monto_usado); //Sigue en el while
                }
            }
        }
        cnt++;
    }
    return rta;
}
/***********************************************************************/
void ACT_MONTO_PROMO(long pCod_promo, double pMonto_promo, int pNuevo, unsigned short medio, unsigned short submedio)
/***********************************************************************/
{
    /*Actualiza los montos usados en una promocion*/
    int cant = 0;
    int cnt = 0;
    struct activacion_medios act_medio;
    struct pagosPromo pagosRealizados;
    if(pNuevo){ // si no es una eliminaci�n de medios pago realizado
        /*Guarda el monto que se uso en ese numero (posicion) de pago para esa promo
        Para que en caso que se elimine ese pago retaura los montos utilizados*/
        pagosRealizados.cod_promo = pCod_promo;
        pagosRealizados.monto_usado = pMonto_promo;
        pagosRealizados.eliminado = 0;
        pagosRealizados.pos_pago = RAM_P_PAGO;
        GUARDAR_ARCHIVO_REC(ARCH_REC_PAGOS_PROMO, (char *)&pagosRealizados, sizeof(struct pagosPromo));
    }
    cant = CANTIDAD_REGISTROS_REC( ARCH_REC_ACT_MEDIO_MOTOR, sizeof( struct activacion_medios ) );
    while(cnt < cant)
    {
        RECUPERAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)&act_medio, sizeof(struct activacion_medios), cnt);
        if(act_medio.tipo == ACTIVAR_MEDIO
			&& act_medio.medio == medio
			&& act_medio.submedio == submedio
			&& act_medio.cod_promo == pCod_promo){
            act_medio.monto_usado += pMonto_promo;
            ACTUALIZAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR, (char *)&act_medio, sizeof(struct activacion_medios), cnt);
        }
        cnt++;
    }
}

/***********************************************************************/
void ELIMINA_PAGO_PROMO( unsigned short medio, unsigned short submedio )
/***********************************************************************/
{
    /*Elimina los pagos realizados con medios activados por promocion*/
    int cant = 0;
    int cnt = 0;
    struct pagosPromo pagosRealizados;
    cant = CANTIDAD_REGISTROS_REC( ARCH_REC_PAGOS_PROMO, sizeof( struct pagosPromo ) );
    while(cnt < cant)
    {
        RECUPERAR_ARCHIVO_REC(ARCH_REC_PAGOS_PROMO, (char *)&pagosRealizados, sizeof(struct pagosPromo), cnt);
        if(pagosRealizados.pos_pago == RAM_P_PAGO && !pagosRealizados.eliminado){
            ACT_MONTO_PROMO(pagosRealizados.cod_promo, -pagosRealizados.monto_usado, NO, medio, submedio);
            pagosRealizados.eliminado = 1;
            ACTUALIZAR_ARCHIVO_REC(ARCH_REC_PAGOS_PROMO, (char *)&pagosRealizados, sizeof(struct pagosPromo), cnt);
        }
        cnt++;
    }
}

/***********************************************************************/
int COD_SUBMEDIO(int pos_pago)
/***********************************************************************/
{
    int submedio = 0;
    if( medios[_PAGO_MODO( pos_pago )].tarjeta) {
        submedio = _PAGO_TARJETA( pos_pago  );
    }
    else if( medios[_PAGO_MODO( pos_pago )].ticket_compra || medios[_PAGO_MODO( pos_pago  )].mutual) {
        submedio = _PAGO_CUENTA( pos_pago );
    }else{
        submedio = -1;
    }
    return submedio;
}
int MOSTRAR_MEDIOS_OBLIGATORIOS(int grupo)
{
   int cant=0, cnt=0, j=0, vamosok= 0;
    struct requerir_medios medios_requeridos;
    char mediosObligatorios[300];
	char nromediosObligatorios[20];
	char str[5];

    memset(mediosObligatorios,0,sizeof(mediosObligatorios));
	memset(nromediosObligatorios,0,sizeof(nromediosObligatorios));
    /*Cantidad de medios/sumedio/plan limitados por promocion*/
    cant = CANTIDAD_REGISTROS_REC(ARCH_REC_REQUERIR_MEDIO, sizeof(struct requerir_medios));
    while(cnt < cant){
        RECUPERAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios), cnt);
        /*si es un grupo distinto al anterior y el anterior no fue ok sale*/
		if(/*grupo == medios_requeridos.cod_grupo && */medios_requeridos.eliminado == 0 ){ //lo de eliminado lo agregue
            strcat(mediosObligatorios,"&");
            strcat(mediosObligatorios,medios[medios_requeridos.medio - 1].nombre);
			strcat(nromediosObligatorios,"&");
			sprintf(str, "%d", medios_requeridos.medio - 1);
            strcat(nromediosObligatorios,str);
            if(medios_requeridos.submedio > -1){
                char cadena_auxiliar[150];
                strcat(mediosObligatorios," - ");
                if(medios[medios_requeridos.medio - 1].tarjeta){
                    strcat(mediosObligatorios,NOMBRE_TARJETA( medios_requeridos.submedio ));
                }else if(medios[medios_requeridos.medio - 1].ticket_compra){
                    NOMBRE_TICKET_COMPRA( medios_requeridos.submedio, cadena_auxiliar );
                    strcat(mediosObligatorios,cadena_auxiliar);
                }else if(medios[medios_requeridos.medio - 1].mutual){
                    NOMBRE_MUTUAL( ( char )( medios_requeridos.submedio ), cadena_auxiliar );
                    strcat(mediosObligatorios,cadena_auxiliar);
                }
            }
			//
            if(medios_requeridos.plan > -1){
                char plan[50];
                sprintf(plan, "Plan: %i",medios_requeridos.plan );
                strcat(mediosObligatorios,plan);
            }

        }
        cnt++;
    }
	//nromediosObligatorios = &0&3  para medio 0 efectivo y medio 4 cta cte
	if(cant > 0 && strlen(nromediosObligatorios) >0){
		int pp;
		char * ptr;
		char ptr2[5];
		for( pp = 0; pp < RAM_P_PAGO; pp++ ) {//No tenemos en cuenta el titulo
			sprintf(ptr2,"%d", _PAGO_MODO(pp));
			ptr = strstr( nromediosObligatorios, ptr2);
			if(ptr){
				glog("Lo encontro",1,1);
				vamosok= 1;
			} else{
				glog("NO lo encontro encontro",1,1);
				vamosok = 0;
				break;
			}

		}
	

	}
	
	if(  cant > 0 && strlen(mediosObligatorios) >0 && vamosok == 0) {
		ExecuteSSAction( A_PEDIR, _HIBRIDO, " MEDIOS OBLIGATORIOS ", mediosObligatorios, NULL, " " );
		MENSAJE_TECLA("PRESIONE UNA TECLA");
		ExecuteAction( A_FIN_PEDIR );
	} else
		vamosok =1;
	return (vamosok);
}
/*******************************************************************************************************/
int TOMAR_DATO_ADICIONAL( char *valor, char *descripcion, int orden, int tipo_dato, char cod_medio, char submedio )
/*******************************************************************************************************/
{
	int       ok = 1/*, caracteres = 0, dif_fecha = 0*/, impresion = 0;
	//unsigned fecha_vto = 0;
	//struct ImpresionDatosAdicionales ImpDatosAdi;
	char  *str1 = "TARJ", *str2 = "AUTORI", *str3, *ptr;
	switch( tipo_dato ) {
		//case 4:
		//case 1:
		case 2:/*** Cadena ***/
			if( cod_medio != '\0' ) {
				switch( cod_medio ) {
					case 3: /*----Para Cheque ----*/
						switch( submedio ) {
							case 7: /*----Para Cheque Gobierno----*/
								strcpy( ImpDatosAdi.nro_cuenta, valor );
								break;
						}
						break;
					case 4: /*---- Para Tarjeta Bancaria ----*/
						switch( submedio ) {
							case 15: /*----Para Tarjeta Bancaria----*/
								str3 = _strupr( descripcion );
								ptr = strstr( str3, str1);

								if ( ptr != NULL && orden == 1 ) {
									impresion = 1;
								} else {
									ptr = strstr( str3, str2 );
									if ( ptr != NULL && orden == 2 ) {
										impresion = 2;
									}
								}
								switch( impresion ) {
									case 1:/*---- Nro Tarjeta ----*/
										strcpy( ImpDatosAdi.nro_tarjeta, valor );
										break;
									case 2:/*---- Autorizacion ----*/
										strcpy( ImpDatosAdi.autorizacion, valor );
										break;
								}
								break;
						}
						break;
					case 5: /*----Para Vales----*/
						switch( submedio ) {
							case 3: /*----Para Tran. Inter----*/
								strcpy( ImpDatosAdi.nro_cuenta, valor );
								break;
						}
						break;
					case 6: /*----Para Bonos----*/
						switch( submedio ) {
							case 7: /*----Para Bono Paguit----*/
								strcpy( ImpDatosAdi.nro_cuenta, valor );
								break;
						}
						break;
				}
			}
			break;
		case 3:/*** Fecha ***/
			if( cod_medio != '\0' ) {
				switch( cod_medio ) {
					case 4: /*----Para Tarjeta Bancaria ----*/
						switch( submedio ) {
							case 15: /*----Para Tarjeta Bancaria----*/
								strcpy( ImpDatosAdi.fecha, valor );
								break;
						}
						break;
				}
			}
			break;
	}

	return ok;
}
/**************************************************************************************/
int MEDIOS_HABILITADOS_COBROS( int n, int dev_cobro, struct    _datos_cobro *datosCobros )
/**************************************************************************************/
{
	int rta = 0;
	if( dev_cobro ){
		if( medios[n].hab_cobro_dev == 'S' || datosCobros->hab_todos_medios_dev== 'S' ) {
				rta = 1;
		} else if( medios[n].hab_cobro_dev == 'M' ) {
				rta = PAGOS_MEDIO_HABILITADO_COBRO( n+1, 0, dev_cobro );
		} else {
				rta = 0;
		}
	} else {
		if( medios[n].hab_cobro == 'S' || datosCobros->hab_todos_medios== 'S' ) {
				rta = 1;
		} else if( medios[n].hab_cobro == 'M' ) {
				rta = PAGOS_MEDIO_HABILITADO_COBRO( n+1, 0, dev_cobro );
		} else {
				rta = 0;
		}
	}
	return rta;
}
/***************************************************************************************/
double TOTAL_X_MEDIO_EN_TICKET( int medio )
/***************************************************************************************/
{
	int i;
	double total= 0;
	for( i = 0;i < RAM_P_PAGO; i++ ) {
		int mm = _PAGO_MODO( i ) + 1;
		if( mm == medio )
			total += _PAGO_IMPORTE( i );
	}
	return total;
}

/***********************************************************************/
int ARTICULO_EN_GRUPO( int grupo, char* cod_interno_alfa, char *cod_barra,
	char *cod_clasificacion, long cod_interno )
/***********************************************************************/
{
	int  rta = 0;
	struct _grupo_art grupo_art;
	/*struct _ind_grupo_art {
		INT16 cod_grupo;
		unsigned long  cod_interno;
		char  cod_barra[17];
		char cod_interno2[21];
	} ind_grupo_art;*/

	/*--------------- Busca a nivel de grupo de articulos ----------------*/
	//SELECT( AREA_GRUPO_ART );
    /*if( USE_DB( AREA_GRUPO_ART, _GRUPOART_SIC, ( char * ) &grupo_art, sizeof( grupo_art ),
		NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_GRUPO_ART, TT_ORIG ,( char* )&grupo_art,sizeof( grupo_art ))== 0 ) {
		SELECT_TABLE( T_GRUPO_ART, TT_ORIG );
		SET_WHERE("COD_GRUPO = %i COD_INTERNO = %ld CODIGO_BARRA = '%s' COD_INTERNO2 = '%s'",grupo, cod_interno, cod_barra, cod_interno_alfa);
		/*ind_grupo_art.cod_grupo = ( INT16 )grupo;
		ind_grupo_art.cod_interno = cod_interno;
		strncpy( ind_grupo_art.cod_barra, cod_barra, 17 );
		strncpy( ind_grupo_art.cod_interno2, cod_interno_alfa, 21 );*/
		RUN_QUERY(NO);
		//GET_EQUAL( ( char * ) &ind_grupo_art );
		if( FOUND2() ) {
		//if( FOUND(  ) ) {
			rta = 1;
		}
		//CLOSE_DATABASE( AREA_GRUPO_ART );
		CLOSE_TABLE( T_GRUPO_ART, TT_ORIG );
	}

    return ( rta );
}
/***********************************************************************/
int HAY_EFECTIVO( )
/***********************************************************************/
{
    int iter, hay_dinero = SI;
    double tot_dinero = 0, tot_dinero_me = 0;
    double signo = 1;

    if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
        signo = -1;
    }

    for( iter = 0; iter < 10; iter ++ ) {
        if( medios[iter].efectivo ) {
            tot_dinero = ( _X_VENTAS_IMPORTE( iter ) + _X_CAMBIOS_IMPORTE( iter )
                        - _X_RETIROS_IMPORTE( iter ) + _X_COBROS_IMPORTE( iter )
                        + _X_FONDO_FIJO_MULTIPLE_IMPORTE( iter ) );
        }

        if( medios[iter].moneda_extranjera ) {
            tot_dinero_me = ( _X_VENTAS_IMPORTE_ME( iter ) + _X_COBROS_IMPORTE_ME( iter )
                            - _X_RETIROS_IMPORTE_ME( iter )
                            + _X_FONDO_FIJO_MULTIPLE_IMP_ME( iter ) );
        }
    }
	tot_dinero += X_FONDO_FIJO_IMPORTE;// cuando no es multiple importes
	
    for( iter = 0; iter < RAM_P_PAGO; iter++ ) {
        //El importe se asume positivo y se resta si es devolucion o NC
        if( medios[_PAGO_MODO( iter )].efectivo ) {
            tot_dinero += signo * _PAGO_IMPORTE( iter );
        }
        if( medios[_PAGO_MODO( iter )].moneda_extranjera ) {
            tot_dinero_me += signo * _PAGO_IMPORTE_ME( iter );
        }
    }
    if( ( tot_dinero < -0.01 ) || ( tot_dinero_me < -0.01 ) ) {
        hay_dinero = NO;
    }

    return hay_dinero;
}

/**********************************************************************/
int PAGOS_CONTROL_COMPROB_IMPORTADO( int *medio, int *submedio )
/**********************************************************************/
{

    int msg_error = 1, h, pago = 1;
	//double comp_decimal = 0.00;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

	//comp_decimal = DECIMAL_COMPARATIVO( DECIMALES - 1 );

	if( RAM_REFERENCIA_NC ) {

        ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                            &ev_cod_barra, &ev_b_promo );

		for( h = 0 ; h < RAM_P_PAGO && pago == 1; h++ ) {
            SELECT_TABLE( T_EVM_TMP, TT_ORIG );
			RUN_QUERY(NO);
            GO2( TOP );
			pago = 0;
			while( !dbEOF() ) {
				if( medios[_PAGO_MODO( h )].nro == ev_pago.modo_pago ) {
					double aux1=0.0,aux2=0.0;
                    /* se controla submedio */
                    if( medios[_PAGO_MODO( h )].tarjeta ||
                        medios[_PAGO_MODO( h )].ticket_compra ||
                        medios[_PAGO_MODO( h )].mutual ) {
						
                        if( _PAGO_COD_SUBMEDIO( h ) == ev_pago.cod_sub_medio &&
							fabs( _PAGO_IMPORTE( h )) - fabs( ev_pago.importe ) < 0.1 ) {
                            pago = 1;
                            break;
                        }
                    }
					else{
						aux1=_PAGO_IMPORTE( h );
						aux2=ev_pago.importe ;
						if( fabs( _PAGO_IMPORTE( h )) - fabs( ev_pago.importe - ev_pago.vuelto_efectivo) < 0.1 ) {
							pago = 1;
							break;
						}
					}
                }
				SKIP2( 1 );
            }

			if( pago == 0 ) {
				char msg[80], nombre[17];

				memset( msg, 0, sizeof( msg ) );
				memset( nombre, 0, sizeof( nombre ) );

				NOMBRE_MEDIO_SUBMEDIO( _PAGO_MODO( h ), _PAGO_COD_SUBMEDIO( h ), nombre, sizeof( nombre ) );
				_snprintf( msg, sizeof( msg ), ST( S_MEDIO___S__NO_SE_UTILIZO_O_IMPORTE_MAYOR ), nombre );
				MENSAJE( msg );
				pago = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_CAMBIO_MEDIO_EN_NC ),
								  ST( S_AUTORIZO_CAMBIO_MEDIOS_NC ), RAM_COD_FUNCION, SI );
			}

        }

        if( pago == 0 ) {
            /* notar que h nunca es 0 si pago es 0, tener en cuenta
               esto al modificar esta funcion */
            *medio = _PAGO_MODO( h - 1 );
            *submedio = _PAGO_COD_SUBMEDIO( h - 1 );
        }

        CERRAR_EVENTOS_TEMPORALES();
    }

    return pago;
}

/**********************************************************************/
int MEDIO_DE_PAGO_HABILITADO_MARGINAL( int n, int dev_cobro )
/**********************************************************************/
{
	int rta = 0, i;
	struct _datos_cobro datos_cobro;
	struct DatosCobroPagaTodo datosPT;

	if( !RAM_HAY_COBRO_VENTA )
		return rta;

	//USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 );
	OPEN_TABLE( T_COBROS, TT_ORIG , ( char* )&cobros, sizeof( cobros ) );
	//Cobros tradicionales
	for( i = 0;
		i < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA, sizeof( struct _datos_cobro ) ) && !rta;
		i++ )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA, ( char* )&datos_cobro,
	        sizeof( struct _datos_cobro ), i );
	    //SELECT( AREA_AUX4 );
		SELECT_TABLE( T_COBROS, TT_ORIG );
		//GET_EQUAL( ( char* )&datos_cobro.codigo );
		SET_WHERE("CODIGO = '%ld'", datos_cobro.codigo);
		RUN_QUERY(NO);
		rta = PAGOS_MEDIO_HABILITADO_COBRO( n+1, 0, dev_cobro );
	}
	//Cobros Pagatodo
	for( i = 0;
		i < CANTIDAD_REGISTROS_REC( ARCH_REC_DATOS_PAGATODO, sizeof( struct DatosCobroPagaTodo ) ) && !rta;
		i++ )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_DATOS_PAGATODO, ( char* )&datosPT,
	        sizeof( struct DatosCobroPagaTodo ), i );
	    //SELECT( AREA_AUX4 );
		SELECT_TABLE( T_COBROS, TT_ORIG );
		//GET_EQUAL( ( char* )&datosPT._codigo );
		SET_WHERE("CODIGO = '%ld'", datosPT._codigo);
		RUN_QUERY(NO);
		rta += PAGOS_MEDIO_HABILITADO_COBRO( n+1, 0, dev_cobro );
	}
	//Cobros Garantia
	for( i = 0;
		i < CANTIDAD_REGISTROS_REC( ARCH_REC_COBROS_VENTA_GARANTIA, sizeof( struct _datos_cobro ) ) && !rta;
		i++ )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_VENTA_GARANTIA, ( char* )&datos_cobro,
	        sizeof( struct _datos_cobro ), i );
	    //SELECT( AREA_AUX4 );
		SELECT_TABLE( T_COBROS, TT_ORIG );
		//GET_EQUAL( ( char* )&datos_cobro.codigo );
		SET_WHERE("CODIGO = '%ld'", datos_cobro.codigo);
		RUN_QUERY(NO);
		rta += PAGOS_MEDIO_HABILITADO_COBRO( n+1, 0, dev_cobro );
	}
	//CLOSE_DATABASE( AREA_AUX4 );
	CLOSE_TABLE( T_COBROS, TT_ORIG );
	return rta;
}

/***************************************************************************************/
#define GRAPH_BUFFER_SIZE 4096
double maxFlow ( grafo, size );


/***********************************************************************/
int addReqsGrafo(
								int ind,		// nombre del ultimo nodo agregado
								int origen,		// codigo de origen del requerimiento a agregar
								int grupo,		// codigo de grupo de requerimientos a agregar
								char* buffer,	// buffer sobre el cual escribir los nodos
								int bufferInd,	// posicion en el buffer a partir de la cual se debe seguir escribiendo el grafo
								int n,			// cantidad hasta este momento de nodos que contiene el grafo
								int m,			// cantidad hasta este momento de aristas que contiene el grafo
								long flujoRequerido	// va registrando la suma de los requerimientos
							)
/***********************************************************************/
{
	int pos = 0; // posicion en el archivo rec
	int primeraVez = 1;
	int montoCompartido = 1;
	int cant;
    struct requerir_medios medios_requeridos;

	//Cantidad de registros en el archivo req.
    cant = CANTIDAD_REGISTROS_REC(ARCH_REC_REQUERIR_MEDIO, sizeof(struct requerir_medios));


	// Si el codigo de grupo es igual a la cantidad de registros (lo cual es una cota maxima
	// para la cantidad de grupos, se cambia al siguiente origen de restricciones.
	// Si ademas ya se encuentra en el �ltimo origen de restricciones se da por finalizada
	// la construcci�n del grafo y se calcula el flujo m�ximo sobre el mismo.
	if ( cant == grupo ) {
		if ( origen < ORIGEN_COBRO ) {
			return addReqsGrafo(
								ind,		// nombre del proximo nodo a agregar
								 origen + 1,	// codigo de origen del requerimiento a agregar
								 0,		// codigo de grupo de requerimientos a agregar
								 buffer,	// buffer sobre el cual escribir los nodos
								 bufferInd,	// posicion en el buffer a partir de la cual se debe seguir escribiendo el grafo
								 n,			// cantidad hasta este momento de nodos que contiene el grafo
								 m,			// cantidad hasta este momento de aristas que contiene el grafo
								 flujoRequerido	// va registrando la suma de los requerimientos
								);
		} else {
			// calcular el flujo maximo y terminar
			// ...
			//...return TRUE;
			char *grafo;
			int i = 0;
			grafo = (char*) malloc ( bufferInd + 100 );
			i = _snprintf( &grafo[i], ( ( bufferInd + 100 ) - (i + 1) ),"p max %d %d\n",n ,m );
			i+= _snprintf( &grafo[i], ( ( bufferInd + 100 ) - (i + 1) ),"n 1 s\n" );
			i+= _snprintf( &grafo[i], ( ( bufferInd + 100 ) - (i + 1) ),"n 2 t\n" );
			memcpy( &grafo[i], buffer, bufferInd + 1); // el '+1' es para copiar tambien el '\0'
			i += bufferInd;
			/**/glog(grafo,LOG_VENTAS,5);
			{
				char buff[256];
				long max = (long) maxFlow ( grafo, i );
				int i = 0;
				i = _snprintf(buff,255,"VALIDACION DE PAGOS: Flujo Maximo = %ld. Flujo Requerido = %ld",max,flujoRequerido);
				buff[i] = 0;
				glog(buff,LOG_VENTAS,5);
				return( max >= flujoRequerido - 10 ? 1 : 0 );
			}
		}
	}


	while ( pos <  cant ) {
		RECUPERAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_requeridos, sizeof(struct requerir_medios), pos );
		if ( medios_requeridos.origen == origen && medios_requeridos.cod_grupo == grupo ) {

			if ( primeraVez ) {
				// Se agrega la arista desde la fuente hasta este nuevo nodo con la capacidad del requerimiento.
				bufferInd += _snprintf(&buffer[bufferInd], GRAPH_BUFFER_SIZE - bufferInd,
						"a 1 %d %d\n", ++ind // nuevo nodo
						             , (long)ROUND( medios_requeridos.monto * 100, 10, 2 )  /*capacidad*/ );
				buffer[bufferInd] = 0;
				flujoRequerido += (long)ROUND( medios_requeridos.monto * 100, 10, 2 ); // incrementa el flujo requerido para satisfacer todos los requerimientos
				montoCompartido = medios_requeridos.monto_compartido;
				n++; // se agrega un nodo
				m++; // se agrega una arista
				primeraVez = 0;
			}

			if ( medios_requeridos.monto_compartido ) {
				// recorrer todos los pagos en busca de todas las ocurrencias
				// del medio requerido (notar que podr�a haber m�s de una ocurrencia)

				int i;
				for (i = 0; i < RAM_P_PAGO; i++) {
					if ( medios_requeridos.medio == _PAGO_MODO( i ) + 1
						&& ( medios_requeridos.submedio == -1
							|| COD_SUBMEDIO(i) + ( medios[_PAGO_MODO( i )].ticket_compra ? 0 : 1 )
								== medios_requeridos.submedio )
                        && ( medios_requeridos.plan == -1
							|| medios_requeridos.plan
								== _PAGO_CUOTAS( i ))) {
						// Agregar la arista entre el nuevo nodo y el nodo correspondiente a ese pago
						// con la capacidad del requerimiento
						bufferInd += _snprintf(&buffer[bufferInd], GRAPH_BUFFER_SIZE - bufferInd,
						"a %d %d %d\n", ind		/*requerimiento*/
						, 3 + i	/*pago*/
						, (long)ROUND( medios_requeridos.monto * 100, 10, 2 )  /*capacidad*/ );
						buffer[bufferInd] = 0;
						m++; // se agrega una arista
					}
				}
			} else {
				// Si el monto del requerimiento no puede ser compartido entre todos sus medios,
				// se generan un grafo distinto para cada uno de estos medios (incluyendo
				// s�lo uno de estos medios en cada grafo). A partir de cada uno de estos
				// subgrafos se continua contruyendo el grafo para el resto de los requerimientos.
				// Si alguno de todos estos grafos obtenidos se cumple que el flujo maximo
				// alcanza el flujo requerido, entonces se validan los pagos.
				//char bufferTemp[GRAPH_BUFFER_SIZE];
				char *bufferTemp;
				int bufferIndTemp = bufferInd;
				int mTemp = m;
				int i;

				bufferTemp = (char*) malloc( GRAPH_BUFFER_SIZE );
				memcpy(bufferTemp, buffer, GRAPH_BUFFER_SIZE);

				// recorrer todos los pagos en busca de todas las ocurrencias
				// del medio requerido (notar que podr�a haber m�s de una ocurrencia)
				for (i = 0; i < RAM_P_PAGO; i++) {
                    if ( medios_requeridos.medio == _PAGO_MODO( i ) + 1 && ( medios_requeridos.submedio == -1 ||
                         COD_SUBMEDIO( i ) == medios_requeridos.submedio ) && ( medios_requeridos.plan == -1 ||
                         medios_requeridos.plan == _PAGO_CUOTAS( i ))) {
							 // Agregar la arista entre el nuevo nodo y el nodo correspondiente a ese pago
							 // con la capacidad del requerimiento
							bufferIndTemp += _snprintf(&bufferTemp[bufferIndTemp], GRAPH_BUFFER_SIZE - bufferIndTemp ,
									"a %d %d %d\n", ind		/*requerimiento*/
												  , 3 + i	/*pago*/
												  , (long)ROUND( medios_requeridos.monto * 100, 10, 2 )  /*capacidad*/ );
							bufferTemp[bufferIndTemp] = 0;
							mTemp++; // se agrega una arista
						 }
				}

				// Entonces ahora, si agregando s�lo esta arista al grafo, se encuentra un flujo que satisfaga
				// el requerimiento, se da por terminado el algoritmo. Sino, se seguir� probando
				// con las otras aristas del mismo requerimiento.

				glog("VALIDAR PAGOS: Sub Rama",LOG_VENTAS,4);
				glog(bufferTemp,LOG_VENTAS,4);
				if ( addReqsGrafo(
							ind,		// nombre del ultimo nodo agregado
							origen,		// codigo de origen del requerimiento a agregar
							grupo + 1,	// codigo de grupo de requerimientos a agregar
							bufferTemp,		// buffer sobre el cual escribir los nodos
							bufferIndTemp,	// posicion en el buffer a partir de la cual se debe seguir escribiendo el grafo
							n,			// cantidad hasta este momento de nodos que contiene el grafo
							mTemp,			// cantidad hasta este momento de aristas que contiene el grafo
							flujoRequerido	// va registrando la suma de los requerimientos
							)
							) {
							   if(bufferTemp )
								free( bufferTemp );
								return 1;
							} else {
								// se sigue dentro del bucle principal.
								if( bufferTemp)
								free( bufferTemp );
							}
			}

		}
		pos++;
	}


	if ( montoCompartido ) {
		// En este caso se agregaron todas las aristas del requerimiento, y se debe ahora
		// continuar con la construccion del grafo para de ahi obtener la respuesta final.
		return addReqsGrafo(
							ind,		// nombre del ultimo nodo agregado
							origen,		// codigo de origen del requerimiento a agregar
							grupo + 1,	// codigo de grupo de requerimientos a agregar
							buffer,		// buffer sobre el cual escribir los nodos
							bufferInd,	// posicion en el buffer a partir de la cual se debe seguir escribiendo el grafo
							n,			// cantidad hasta este momento de nodos que contiene el grafo
							m,			// cantidad hasta este momento de aristas que contiene el grafo
							flujoRequerido	// va registrando la suma de los requerimientos
							);
	} else {
		return 0;
	}

}

/***********************************************************************/
int MEDIOS_LIMITADOS_POR_PROMOCION()
/***********************************************************************/
/*---CONDICION MONTO REQUERIR MEDIO - SUBMEDIO - PLAN---*/
/*Condiciones del monto requerido*/
//#define     CONDICION_MAYOR_IGUAL       1
//#define     CONDICION_MENOR_IGUAL       2
//#define     CONDICION_TOTAL_TIKET       3

/*Origen del requerimiento de medio*/
//#define     ORIGEN_PROMOCION			1
//#define     ORIGEN_COBRO				2

{
/*
 * Por el momento s�lo se implementan las restricciones de medios con condicion CONDICION_MAYOR_IGUAL.
 * Las otras restricciones ser�n tratadas del mismo modo que las CONDICION_MAYOR_IGUAL.
 */
/*
 * Se construye un grafo con capacidades en los lados, sobre el cual se calcular�
 * el flujo maximal. Si dicho flujo coincide con la suma de todos los
 * requerimientos, entonces se validan los pagos.
 * Las capacidades en el grafo se representan como enteros largos. Para contener hasta 2 decimales
 * de precision, se guarda ROUND( monto * 100, 2)
 */

	int ind = 3;	// indice del nuevo nodo del grafo. el 1 y 2 estan reservados para la fuente y el resumidero respectivamente.
	int n = 2;	// cantidad de nodos del grafo. incialmente s�lo la fuente y el resumidero.
	int m = 0;	// cantidad de aristas del grafo.
	int source_capacity = 0;	// Capacidad m�xima de la fuente (para ser comparado luego con el flujo m�ximo)
	int j;
	int rta;
	char *buffer;
	int bufferInd = 0;

	buffer = (char*) malloc( GRAPH_BUFFER_SIZE );
	memset(buffer, '\0', GRAPH_BUFFER_SIZE);


	// Se recorren todos los pagos, agregando los nodos y aristas desde �stos hasta el resumidero,
	// con capacidades iguales al monto pagado con cada pago.
	for ( j = 0; j < RAM_P_PAGO; j++ ) {
		bufferInd  += _snprintf(&buffer[bufferInd], GRAPH_BUFFER_SIZE - bufferInd,
									"a %d %d %d\n", 3 + j /*source*/
									 , 2 /*resumidero*/
									 , (long) ROUND( _PAGO_IMPORTE(j) * 100, 10, 2 )  /*capacidad*/ );
		buffer[bufferInd] = 0;
		n++; // indica que se agreg� un nodo al grafo.
		m++; // indica que se agreg� una arista al grafo.
	}
	ind += j; // actualiza el identificador del pr�ximo nodo.

	rta = addReqsGrafo(
							ind - 1,		// nombre del ultimo nodo agregado
							ORIGEN_PROMOCION,		// codigo de origen del requerimiento a agregar
							0,	// codigo de grupo de requerimientos a agregar
							buffer,		// buffer sobre el cual escribir los nodos
							bufferInd,	// posicion en el buffer a partir de la cual se debe seguir escribiendo el grafo
							n,			// cantidad hasta este momento de nodos que contiene el grafo
							m,			// cantidad hasta este momento de aristas que contiene el grafo
							0	// va registrando la suma de los requerimientos
							);

	if( buffer)
	free( buffer );

	rta = MOSTRAR_MEDIOS_OBLIGATORIOS(0); //mamncuso trucjada esta funcion puede servir para la post pago y limitar a solo esos medios
	//va de la mano de los medios requeridos por ejemplo en la funcion void reqCash
	//o en el campo detalle de la promo aqui estarian los medios restringidos. que se pague solamente con alguno de ellos
	//pero ninguno mas. lo de usar la funion regcash en muy danger solo aplica a las postpago nuevas


	return rta;
}

/****************************************************************************************/
int _PAGOS_PEDIR_DESCRIPTOR( struct _dmedios *dmedios, char *valor )
/****************************************************************************************/
{
	int rta = 0;
    //COMENTO HASTA QUE SE ARREGLE
	/*if( OPEN_TABLE( T_D_DESCR, TT_ORIG, ( char* )&d_descrip, sizeof( d_descrip ) ) == 0 ) {
		SELECT_TABLE( T_D_DESCR, TT_ORIG );
		SET_WHERE( "cod_descriptor = %i", dmedios->cod_descriptor );
		RUN_QUERY( NO );
		rta = Consulta_descrip( _consulta_descrip );
		if( rta ){
			strncpy( valor, d_descrip.valor, dmedios->digitos );
		}
		CLOSE_TABLE( T_D_DESCR, TT_ORIG );
     }*/
	return rta;
}
/*****************************************************************************/
void PAGOS_AGREGAR_DESCUENTO_TARJETA(  )
/*****************************************************************************/
{
	int      h;
	double   total_descuento = 0.0;
	long     cod_interno_art;

   for( h = 0; h < RAM_P_PAGO; h++ ) {
      if( medios[_PAGO_MODO( h )].tarjeta ) {
         total_descuento += _PAGO_IMPORTE_DESCUENTO( h );
         cod_interno_art = medios[_PAGO_MODO( h )].art_variacion;
         //no importa que pise el cod_interno_art. De todas formas trae el cod. del
         //articulo configurado en el medio y no en una tarjeta en particular.
         //Lo hago ac� como para no buscar de nuevo este dato.
      }
	}
   if( fabs( total_descuento ) > 0.001 ) {
      struct _acu_iva acu_tot;
      double    importe_gravado = 0.0, cantidad;

      memset( &acu_tot, 0, sizeof( acu_tot ) );
      for( h = 0; h < RAM_P_TICKET; h++ ) {
   		cantidad = _TICKET_CANTIDAD( h ) * ( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 );
   		ACUMULA_IVA( h, &acu_tot, cantidad );
   	}
      //suponiendo que acu_tot.importe_sin_iva + acu_tot.exento + acu_tot.iva1 +
   	//acu_tot.iva2 + acu_tot.ila + acu_tot.iha es igual a RAM_TOTAL
   	total_descuento *= -1;
      acu_tot.importe_sin_iva *= total_descuento / RAM_TOTAL;
   	acu_tot.exento *= total_descuento / RAM_TOTAL;
   	acu_tot.iva1 *= total_descuento / RAM_TOTAL;
   	acu_tot.iva2 *= total_descuento / RAM_TOTAL;
   	//acu_tot.ila *= total_descuento / RAM_TOTAL;
   	//acu_tot.iha *= total_descuento / RAM_TOTAL;

      if( fabs( acu_tot.exento ) > 0.001 ){
   	   /*if( AGREGAR_ARTICULO( cod_interno_art,
   	       acu_tot.exento, _ORIGEN_DESC_TARJETA, 0 ) ) {*/
   	   if( AGREGAR_ARTICULO_EXENTO( cod_interno_art, acu_tot.exento, 0,
   	       _ORIGEN_DESC_TARJETA ) ) {
   	   } else {
            MENSAJE_STRING( S_ARTICULO_DE_DESCARGA_NO_ENCONTRADO );
   	   }
      }
   	importe_gravado = acu_tot.importe_sin_iva + acu_tot.iva1 + acu_tot.iva2;
   	   //+ acu_tot.ila + acu_tot.iha;
   	if( fabs( importe_gravado ) > 0.001 ) {
   	   /*if( AGREGAR_ARTICULO( cod_interno_art,
   	       importe_gravado, _ORIGEN_DESC_TARJETA , _GRAVADO ) ){*/
   	   if( AGREGAR_ARTICULO_GRAVADO( cod_interno_art, importe_gravado, _GRAVADO,
		   _ORIGEN_DESC_TARJETA ) ){
   	   } else {
            MENSAJE_STRING( S_ARTICULO_DE_DESCARGA_NO_ENCONTRADO );
   	   }
      }
   }
}
/********************************************************************************/
void GRABAR_TARJETAS_REC(int h, int posicion)
/********************************************************************************/
{
	memset( &tarjetas_fisc, 0, sizeof( struct _tarjetas_fisc ) );
	strcpy( tarjetas_fisc.cod_autorizacion,VAR_TARJETAS_AUTORIZACION );
	TRIM( tarjetas_fisc.cod_autorizacion );
	tarjetas_fisc.marca_tarjeta = VAR_TARJETAS_TARJETA;
	strcpy( tarjetas_fisc.nombre, TARJ[VAR_TARJETAS_TARJETA].nombre);
	tarjetas_fisc.cuotas = VAR_TARJETAS_CUOTAS;
	tarjetas_fisc.importe = _PAGO_IMPORTE( h );;
	tarjetas_fisc.moneda = VAR_TARJETAS_MONEDA;
	tarjetas_fisc.nro_cupon = VAR_TARJETAS_NRO_CUPON;
	tarjetas_fisc.nro_cupon_int = VAR_TARJETAS_NRO_CUPON_INT;
	tarjetas_fisc.nro_lote =	VAR_TARJETAS_LOTE;
	tarjetas_fisc.nro_terminal = VAR_TARJETAS_TERMINAL;
	strcpy( tarjetas_fisc.numero_tarjeta, VAR_TARJETAS_NUMERO);
	tarjetas_fisc.p_pago = h;
		
	strcpy(tarjetas_fisc.track1, GET_TRACK_1_PRIVADO()/*VAR_TARJETAS_TRACK1*/);
	strcpy(tarjetas_fisc.track2, GET_TRACK_2_PRIVADO()/*VAR_TARJETAS_TRACK2*/);
	tarjetas_fisc.fecha_vto = VAR_TARJETAS_FECHA_VTO;
	if (posicion > 0 && (CANTIDAD_REGISTROS_REC(ARCH_REC_TARJETAS_FISC,
			sizeof(struct _tarjetas_fisc)) > 0)) {
		ACTUALIZAR_ARCHIVO_REC(ARCH_REC_TARJETAS_FISC, (char *) &tarjetas_fisc,
				sizeof(struct _tarjetas_fisc), posicion);
	} else {
		BORRAR_ARCHIVO_REC(ARCH_REC_TARJETAS_FISC);
		GUARDAR_ARCHIVO_REC(ARCH_REC_TARJETAS_FISC, (char *) &tarjetas_fisc,
				sizeof(struct _tarjetas_fisc));
	}
}
/********************************************************************************/
void LEER_TARJETAS_REC()//Seba
/********************************************************************************/
{
	int cnt = 0, cant = 0, i=0;
	cant = CANTIDAD_REGISTROS_REC( ARCH_REC_TARJETAS_FISC, sizeof( struct _tarjetas_fisc ));
	for( i=0; i<10; i++)
		memset( &tarjetas_fisc_rec[i], 0, sizeof( struct _tarjetas_fisc ) );
    while( cnt < cant )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_TARJETAS_FISC, ( char* )&tarjetas_fisc, sizeof( struct _tarjetas_fisc ), cnt );
		strcpy(tarjetas_fisc_rec[cnt].cod_autorizacion, tarjetas_fisc.cod_autorizacion);
		tarjetas_fisc_rec[cnt].cuotas = tarjetas_fisc.cuotas;
		tarjetas_fisc_rec[cnt].importe = tarjetas_fisc.importe;
		tarjetas_fisc_rec[cnt].moneda = tarjetas_fisc.moneda;
		tarjetas_fisc_rec[cnt].nro_cupon = tarjetas_fisc.nro_cupon;
		tarjetas_fisc_rec[cnt].nro_cupon_int = tarjetas_fisc.nro_cupon_int;
		tarjetas_fisc_rec[cnt].nro_lote = tarjetas_fisc.nro_lote;
		tarjetas_fisc_rec[cnt].nro_terminal = tarjetas_fisc.nro_terminal;
		tarjetas_fisc_rec[cnt].marca_tarjeta = tarjetas_fisc.marca_tarjeta;
		strcpy( tarjetas_fisc_rec[cnt].numero_tarjeta, tarjetas_fisc.numero_tarjeta);
		tarjetas_fisc_rec[cnt].p_pago = tarjetas_fisc.p_pago;
		strcpy(tarjetas_fisc_rec[cnt].track1, tarjetas_fisc.track1);
		strcpy(tarjetas_fisc_rec[cnt].track2, tarjetas_fisc.track2);
		strcpy(tarjetas_fisc_rec[cnt].nombre, tarjetas_fisc.nombre);
		tarjetas_fisc_rec[cnt].fecha_vto = tarjetas_fisc.fecha_vto;
		cnt++;
    }
	if( !( HABILITA_COPIA_VOUCHER_PRN  && IMPRESORA_FISCAL == FISCAL ) ) {
		BORRAR_ARCHIVO_REC(ARCH_REC_TARJETAS_FISC);
	}
}

/********************************************************************************/
int RECUPERAR_POSICION_TARJETA_FISCAL( int h )
/********************************************************************************/
{
	int cnt = 0, cant = 0;
	struct _tarjetas_fisc tarjetas_posicion;

	cant = CANTIDAD_REGISTROS_REC( ARCH_REC_TARJETAS_FISC, sizeof( struct _tarjetas_fisc ));
    while( cnt < cant )
	{
		RECUPERAR_ARCHIVO_REC( ARCH_REC_TARJETAS_FISC, ( char* )&tarjetas_posicion, sizeof( struct _tarjetas_fisc ), cnt );
		if( h == tarjetas_posicion.p_pago ) {
			return cnt;
		}
		cnt++;
	}
	if( !cant )
		cnt = h;

	return cnt;
}
/********************************************************************************/
int CANT_TARJETAS_REC()
/********************************************************************************/
{
	int cant = 0;
	cant = CANTIDAD_REGISTROS_REC( ARCH_REC_TARJETAS_FISC, sizeof( struct _tarjetas_fisc ));
    return cant;
}

/********************************************************************************/
int ES_VERDADERO(char opcion)
//funcion provisoria util hasta q se estabilice el tipo de dato
/********************************************************************************/
{
	int rta = NO;
	switch(opcion){
	
	case '1':
	case  1 :
	case 'S':
	case 's':
		rta = SI;
	break;
	}
	
    return rta;
}
/**************************************************************************/
double VARIACION_RETENCION( double importe_neto, double iva, double exento )
/**************************************************************************/
{
   if( RAM_NOTA_CR ) {
      importe_neto *= -1;
      iva *= -1;
      exento *= -1;
   }
   //devolvemos la variacion solo de la retencion restandola de esta manera
   return ( 1/*RETENCION( importe_neto, iva, exento,  SI, SI ) - RETENCION( 0.0, 0.0, 0.0, SI, SI )*/ );
}
/**************************************************************************/
int ES_LA_MISMA_TARJETA(char * tarjetaOrig,char*tarjeta2){
/*Se debe recurrir a esta funcion para aproximar los caracteres
no '*' de una tarjeta enmascarada y solicitarla*/
/**************************************************************************/
int rta=0;
char str1[50] ;
char busca = '*', *ptrA, *ptrB;
char aux[100];
memset(str1,0,sizeof(str1));
strcpy(str1,tarjetaOrig );//strcpy(str1,ev_pago.nro_tarjeta );
	  
	//busco el dato no encriptado
	//de adelante a atras
	ptrA = strchr(str1 , busca);
	if( ptrA ){
		//atras hacia adelante
		ptrB = strrchr(str1 , busca);
		if( ptrB ){
			memset( aux, 0, sizeof( aux ));
			//en que caracter esta el * 	
			
			if( (ptrA-str1) > 6 ){
				//cargo desde el comienzo hasta el primer *
				strncpy(aux,tarjetaOrig,((ptrA-str1)));//strncpy(aux,ev_pago.nro_tarjeta,((ptrA-str1)));
			}else{
				//cargo desde el primero sin *
				strcpy(aux,&tarjetaOrig[(ptrB-str1) + 1]);//strcpy(aux,&ev_pago.nro_tarjeta[(ptrB-str1) + 1]);
			}
			//copio el valor de la RNV, para evitar errores...
			strcpy(str1,tarjeta2);//strcpy(str1,_PAGO_NRO_TARJETA( h ));
			TRIM(aux);
			ptrA = strstr( str1, aux );
			if( ptrA ){
			//esta la cadena en el original
				rta = 1;
			}else{
			//no detecto 
			}
		}
	}else{
		if( strncmp( tarjeta2,//strncmp( _PAGO_NRO_TARJETA( h ),
			tarjetaOrig , strlen( tarjeta2 ) ) == 0 ) {
			rta = 1;
		}
	}
	return rta;
}
/***************************************************************************/
double VARIACION_DE_SUBMEDIOS( int modo , int submedio )
/***************************************************************************/
{
    double rta = 0.0;
    struct _t_mutuales mutual;
    struct _t_compra  t_compra;
    long cuenta=0;
    if( medios[modo].mutual ) {
		
		if(OPENED(T_MUTUALES, TT_ORIG))
			CLOSE_TABLE( T_MUTUALES, TT_ORIG );
        if(
			OPEN_TABLE(T_MUTUALES, TT_ORIG,( char* )&mutual,sizeof( struct _t_mutuales ))== 0 ) {
				SELECT_TABLE( T_MUTUALES, TT_ORIG );
            cuenta = submedio;
			SET_WHERE("COD_MUTUAL = '%ld'",cuenta);
			RUN_QUERY(NO);
            //GET_EQUAL( ( char* )&cuenta );
            if( FOUND2() ) {
				rta = mutual.variacion;
            }
            CLOSE_TABLE( T_MUTUALES, TT_ORIG );
        }
    }
    else if( medios[modo].ticket_compra ) {
		if(OPENED(T_COMPRA, TT_ORIG))
			CLOSE_TABLE( T_COMPRA, TT_ORIG);
        if( /*USE_DB( AREA_AUX, _t_compra , ( char* )&t_compra, sizeof( t_compra ), NULL, 0, 0 )*/
         OPEN_TABLE(T_COMPRA, TT_ORIG,( char* )&t_compra,sizeof( t_compra )) == 0 ) {
			 SELECT_TABLE( T_COMPRA, TT_ORIG );
            cuenta = submedio;
			SET_WHERE("COD_T_COMPRA = '%li'",cuenta);
            //GET_EQUAL( ( char* )&cuenta );
			RUN_QUERY(NO);
            if( FOUND2() ) {
				rta = t_compra.variacion;
            }
            CLOSE_TABLE( T_COMPRA, TT_ORIG );
        }
    }
    
    return rta;
}


/***********************************************************************/
void ELIMINA_PAGO_PROMO_POSPAGO( unsigned short medio, unsigned short submedio, int tecla )
/***********************************************************************/
{
	/*Elimina los pagos realizados con medios activados por promocion*/
	int cant = 0;
	int cnt = 0, eliminar = 0, promo_a_eliminar=0;
	struct requerir_medios medios_promocionados;
	cant = CANTIDAD_REGISTROS_REC(ARCH_REC_REQUERIR_MEDIO, sizeof(struct requerir_medios));
	while(cnt < cant)
	{
		RECUPERAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO,
				(char *) &medios_promocionados, sizeof(struct requerir_medios), cnt);
		
		if( ( medios_promocionados.medio == medio && medios_promocionados.submedio == -1 )
				|| ( medios_promocionados.medio == medio && submedio && medios_promocionados.submedio == submedio )
				|| (promo_a_eliminar == medios_promocionados.cod_promo) ) { //si es la misma promo que ya elimine elimono todos los registros
			//medios_promocionados.medio = 0;
			//medios_promocionados.submedio = 0;
			eliminar = 1;
			if( medios_promocionados.monto == 0 && medios_promocionados.eliminado ) {
				eliminar = 0;
			} 
			medios_promocionados.eliminado = tecla;
			medios_promocionados.monto = 0;
			promo_a_eliminar = medios_promocionados.cod_promo;
			ACTUALIZAR_ARCHIVO_REC(ARCH_REC_REQUERIR_MEDIO, (char *)&medios_promocionados, sizeof(struct requerir_medios), cnt);
		}
		cnt++;
	}
	SET_MEMORY_DOUBLE( __ram_subtotal, RAM_RESTO ); //mmancuso uaa lo saque de abajo
	if( eliminar && state_pago != NULL ) {
		int prepostpago, postpago, saldofinal;
		int bandera = RAM_FLAG_COBRO_VENTA;
		SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );
		prepostpago =RAM_P_TICKET;
		ss = recover( state_pago, ss );
		postpago =RAM_P_TICKET;
		//cuento la cantidad de desceuntos/reversiones de promos postpago 
		saldofinal = (postpago - prepostpago) * 2;
		_SET_MEMORY_CHAR( __ram_promociones_afinidad_usadas,0,saldofinal  );
		
		SET_MEMORY_INT( __ram_flag_cobro_venta, bandera );
		SET_MEMORY_DOUBLE(__ram_importe_total, ROUND(RAM_TOTAL, 10, DECIMALES));//Seteo para que actualice lo que muestra en PAGOS_MUESTRA_TOTAL
		//mmancuso sacado SET_MEMORY_DOUBLE(__ram_resto, ROUND(RAM_TOTAL, 10, DECIMALES));
		SET_MEMORY_DOUBLE( __ram_subtotal, RAM_RESTO ); //mmancuso uaa lo saque de abajo
		PAGOS_MOSTRAR_RESTO(RAM_RESTO);
		PAGOS_MUESTRA_TOTAL(RAM_RESTO);
		SET_MEMORY_LONG( __nro_pagare, 0 );
	}
} 
/***********************************************************************/
void MOSTRAR_SALDO_EFECTIVO()
/***********************************************************************/
{
	int k=0;
	char saldo_efectivo[50];
	double saldo=0.00;
	char *msg[1];
	memset(saldo_efectivo,0,50);
	//busquemos los medios efectivos
	saldo += X_TOT_DINERO;
	_snprintf(saldo_efectivo,50,"SALDO DISPONIBLE EFECTIVO %7.2lf" , saldo);
	msg[0] = (char*)malloc(51);
	sprintf(msg[0],"%s",saldo_efectivo);
	MOSTRAR_MENSAJE_POP_UP( "SALDO DISPONIBLE", msg, 1 );
	free( msg[0] );
	msg[0] = NULL;
}

/*****************************************************************************/
int PROCESAR_QR(void)
/*****************************************************************************/
{
	int i=0, retval=5, reintento = 0, cuenta = 0;
	char parametros[250];
	long caja_z = (long) (NRO_CAJA) * 100000L + (NRO_Z % 100000L);
	int ram_modo_devolucion =0, sucursal, nro_caja,ram_id_evento, posicion;

	if( RAM_MODO_DEVOLUCION ) {
		ram_modo_devolucion = 1;
		sucursal = config.sucursal;
		RECUPERAR_EVENTO_IMPORTADO( &caja_z , &ram_id_evento, &nro_caja, &posicion);
	} else {
		sucursal = config.sucursal;
		nro_caja =NRO_CAJA;
		ram_id_evento =RAM_ID_EVENTO;
		posicion = 0;
	}
	
	memset(parametros,0, 250);
	for (i = 0; i < RAM_P_PAGO && (retval == 5 || retval == 3); i++) {
		if( medios[_PAGO_MODO( i )].ticket_compra &&   _PAGO_BUFFER1_LONG( i )) {
			if(ram_modo_devolucion ==0)
				posicion = i;
			reintento =GET_STATIC_REINTENTO_QR();
			
			cuenta = _PAGO_CUENTA( i );
			sprintf(parametros,"%i %i %li %li %i %.2lf %i %i %i",sucursal,nro_caja ,caja_z, ram_id_evento, posicion,_PAGO_IMPORTE( i ), ram_modo_devolucion,reintento, cuenta);
			SET_STATIC_REINTENTO_QR( );		
			glog(parametros,1,1);
			
			retval = EJECUTAR_SCRIPT_AUTOIT("qrgateway.au3",parametros , SI);
					{
						char mensa[50];
						char numero_tar[21];
						memset(mensa,0,sizeof(mensa));
						memset(numero_tar,0,sizeof(numero_tar));
						sprintf(mensa,"Salio qrgateway.au3 con codigo %i",retval);

						GRABAR_LOG_SISTEMA( mensa ,LOG_VENTAS,2);
						if( retval == 3) {
							char id_interno[10];
							char trans_id[100];
							_snprintf(trans_id, sizeof(trans_id)-1, "%strans_id.txt",GET_PATH_BINARIO());
							sprintf(numero_tar,"%i%li%li%i%i",sucursal,caja_z, ram_id_evento, posicion,reintento);
							numero_tar[20]=0;
							_SET_MEMORY( __pago_nro_tarjeta, i, numero_tar ); //guardemos aqui
							if(cuenta ==2) { //aca recupero el nro interno de la operacion, la que sale en la app mobile
								
								
								if( CARGAR_ARCHIVO_QR( trans_id,id_interno, sizeof(id_interno )) == 1 
									&& strlen(id_interno)> 4)
									_SET_MEMORY( __pago_auto_tarjeta, i, id_interno );
							} else {
								if(cuenta ==1) { //mercadopago no necesita ningun codigo pero usamos si existe el archivo para tomar como correcta la operacion
									//sino existe es que fallo
									if( CARGAR_ARCHIVO_QR( trans_id,id_interno, sizeof(id_interno )) == 0) {
										//no existe el archivo quiere decir que no termino ok
										retval = 2;
										GRABAR_LOG_SISTEMA( "NO SE ENCONRO EL ARCHIVO trans_id.txt POR ESO SE DA POR ERROR" ,LOG_VENTAS,2);
									}

								}
							}
						}
					}

		}
	}
	if( retval ==2 ||retval ==1 || retval ==0 || retval < 0 )
		return 0;
	else {
		return 1;
	}
}

/*****************************************************************************/
void RECUPERAR_EVENTO_IMPORTADO( long *caja_z, long *id_evento, int *caja, int *pos)
/*****************************************************************************/
{
	struct _eventos event_ticket_recup; //estructura del ticket recuperado
	int posicion = 0;
	if( CANTIDAD_REGISTROS_REC( ARCH_REC_IMPORT, sizeof( struct _eventos ) ) ) {
        //si existe una importacion
        //recupera la estructura ticket del evento importado
        if( VERIFICAR_TIPO_EVENTO_HABILITADO_IMPORTACION( event_ticket.tipo_evento ) ) {
			RECUPERAR_ARCHIVO_REC( ARCH_REC_IMPORT, ( char* )&event_ticket_recup,
								   sizeof( struct _eventos ), 0 );
			*caja_z = event_ticket_recup.caja_z;
			*id_evento = event_ticket_recup.id_evento; 
			*caja  = event_ticket_recup.caja; 

			}
			//BORRAR_ARCHIVO_REC(ARCH_REC_IMPORT);
        }
//averiguemos la posicion
	if( DIRECT_DB_QUERY("SELECT posicion FROM evm_tmp  where id_evento = %li and modo_pago = %i;", *id_evento,6 ) > 0  ) {
		GET_DATO(1, C_INT,(char *)&posicion,4);			
		*pos = posicion;
	} else 
		*pos = 0;


}

/****************************************************************************/
void  SET_STATIC_REINTENTO_QR()
/****************************************************************************/
{
	_static_reintento_qr++;

}
/****************************************************************************/
int  GET_STATIC_REINTENTO_QR()
/****************************************************************************/
{	
	return _static_reintento_qr;
}

/****************************************************************************/
void  CEREAR_STATIC_REINTENTO_QR()
/****************************************************************************/
{	
	_static_reintento_qr = 0;
}


/***************************************************************************/
int CARGAR_ARCHIVO_QR( char *nom_archivo, char *buffer, int len )
/***************************************************************************/
{
    char *p;
    int rta = 0;
    FILE *fp;
    fp = fopen( nom_archivo, "r" );
	if( fp ) {
        rta = 1;
        //while( fgets( buffer, len, fp ) ) {
		fgets( buffer, len, fp );
            p = strchr( buffer, '\n' );
            if( p ) {
                p[0] = 0;
            }
        
        fclose( fp );
    }
    return ( rta );
}

