#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <abort.h> 
#include <archivos.h> 
#include <aarch.h>
#include <btrv.h> 
#include <b_mens.h> 
#include <cio.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <day.h> 
#include <dbrouter.h> 
#include <dd.h> 
#include <dec_mes.h> 
#include <driver.h> 
#include <dtoa.h> 
#include <dtoai.h> 
#include <export.h> 
#include <fecha.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <lista.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mon.h> 
#include <mstring.h> 
#include <netcom.h> 
#include <netcom2.h> 
#include <pagos.h> 
#include <pausa.h> 
#include <power.h> 
#include <rnv.h> 
#include <round.h> 
#include <stack.h> 
#include <tarjetas.h> 
#include <tkt.h> 
#include <trim.h> 
#include <year.h>
#include <ctype.h>
#include <db.h>
#include <deftables.h>
#include <config.h>
#include <path.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#include <sys/io.h>
#else
#endif
#define ES_TARJETA (nodo < 9)
//#define _CANTIDAD_OFF_LINE_A_ENVIAR 5 /*1  no se porque estaba limitado a sola 1 operacion*/
int  _CANTIDAD_OFF_LINE_A_ENVIAR = 5; /*1  no se porque estaba limitado a sola 1 operacion*/
struct _nodos 
{
	BINARY status;				//0
	BINARY cod_nodo;			//1
	char nombre[21];			//2
	INT16 time_out_disponible;  //23
	BINARY fin;			//25
	//char reserva[5];			//25
};
struct _transac2 *transac2;
struct _respuesta_napse  *respuestaNapse;
struct _ind2_transac2 ind2_transac2;
struct _codigos_operacion *tabla_operaciones;
static char *buffer_out,*buffer_in;
//struct _ind3_transac ind3_transac;
static char canal_tarjetas_abierto = 0, protocolo;
static char clave_inicializada = 0;
#define CANTIDAD_OPERACIONES 30
//int  ABRIR_CANAL_AL_PRINCIPIO = 0;
//int  ABRIR_CANAL_AL_PRINCIPIO = 2;
#define LEN_BUFFER_TCP 4096
//Estas variables se debe agregar en el CONFTPV
//#define MODELO_TEF       1
//#define GENERAR_REVERSOS 0
//Agregar a ARCHIVOS.h
//#define _RTA_SWITCH_BTR 0
int calcular_nro_cupon_en_netcom2 = SI;
int yacalculo_cupon= NO;
//#define  COMPILA_CAMBIO_CARACTER
#ifdef COMPILA_CAMBIO_CARACTER
	#define CARACTER_CAMBIO_NULL  '_'
#endif
#define INTENTAR_UNA_SOLA_VEZ 1
#define INTENTAR_VARIAS_VECES 0
#define ESPERA_RESPUESTA 1
#define NO_ESPERA_RESPUESTA 0
#define  OPERACION_ON_LINE(op)                       ( tabla_operaciones[ (op)].tipo_de_mensaje == 200 )
#define  OPERACION_OFF_LINE(op)                      ( tabla_operaciones[ (op)].tipo_de_mensaje == 220 )
#define  OPERACION_COBRO(op)                         ( tabla_operaciones[ (op)].tipo_de_mensaje & 800000 )
#define  OPERACION_REVERSABLE(op)                    ( tabla_operaciones[( op)].reversable && GENERAR_REVERSOS )
#define  OPERACION_UN_SOLO_INTENTO(op)               ( tabla_operaciones[( op)].un_solo_intento)
#define  OPERACION_PRIORIDAD(op)                     ( tabla_operaciones[( op)].prioridad)
#define  OPERACION_ESPERA_RTA(op)                    ( tabla_operaciones[( op)].espera_rta)
static int T_ENVIAR_TRANSACCIONES_PENDIENTES( int nodo, int enviar_todas_las_operaciones_off_line,
                                              int reintentos );
int PROCESA_CONTROL_DATOS_ENCRIPTADOS( int encripta  );

struct _datos_transaccion *tran_rta;
struct _datos_transaccion *tran_temp = NULL;
struct _rta_switch_ncr *rta_switch_ncr;
int cant_rta_switch_ncr;
extern int TARJETA_DEBITO ;

int CONVERSION_ASCII_EXT_A_TO_HEXA( int operacion,char *bloque_string,int *long_bloq_string, char* bloque_hexa,int *long_bloq_hexa);
int ES_OPERACION_OFF( int operacion1 );
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES
	#define CEREAR_DATOS_TARJETA 1
	//#define _TRAN_CRP_SIC "TRAN_CRP.SIC" 
#endif
void OCULTA_DATOS_ENCRIPTADOS( struct _datos_transaccion * tran );
void CARGAR_DATOS_BASICOS( struct _datos_transaccion * tran, int cashback );
long OBTENER_NRO_CUPON( char _marca );
void MOSTRAR_MENSAJE_UBUNTU( int icono, char *pMsg, int segTimeout );
long PEDIR_ENTERO_EN_RECUADRO( char *titulo, int salvar_pantalla, int digitos );
void ABRIR_DOCUMENTO_NO_FISCAL( char slip );
int  reverso_inmediato = 0;
void GUARDAR_OPERACION_NAPSE( void );
void LEER_OPERACION_NAPSE( void );
void SETEOS_VALORES_RESPUESTA_NAPSE(int lecturaTabla);
void GUARDAR_OPERACION_NAPSE_DAT( void );
void COPIAR_RTA_NAPSE( void );
long GET_ID_TRANSACCION( void) ;

int T_ANULAR_TRANSACCION( long nro_ticket, double *importe_t, int *nodo_t, UINT16 *modo_pago, int *cuota, long *fecha, int cashback );
unsigned char RC5key[CANTIDAD_DE_VALORES_DE_CLAVE] =    // claves alternativas
{0x91,0x5f,0x46,0x19,0xbe,0x41,0xb2,0x51,0x63,0x55,0xa5,0x01,0x10,0xa9,0xce,0x91,0x78,0x33,0x48,
 0xe7,0x5a,0xeb,0x0f,0x2f,0xd7,0xb1,0x69,0xbb,0x8d,0xc1,0x67,0x87,0xdc,0x49,0xdb,0x13,0x75,0xa5,
 0x58,0x4f,0x64,0x85,0xb4,0x13,0xb5,0xf1,0x2b,0xaf,0x78,0x33,0x48,0xe7,0x5a,0xeb,0x0f,0x2f,0xd7,
 0xb1,0x69,0xbb,0x8d,0xc1,0x67,0x87,0x52,0x69,0xf1,0x49,0xd4,0x1b,0xa0,0x15,0x24,0x97,0x57,0x4d,
 0x7f,0x15,0x31,0x25,0xdc,0x49,0xdb,0x13,0x75,0xa5,0x58,0x4f,0x64,0x85,0xb4,0x13,0xb5,0xf1,0x2b,0xaf};
/****************************************************************************/
int ENVIAR_TRANSACCION_AL_AUT_ON( int operacion, double importe, struct _rta_terminal *rta_terminal,
                                  int nodo )
/****************************************************************************/
{
    int ok = 0;
    int ok1;
    FIN_BUSCA_TARJETA();
    // se hace aca para compatibilidad con el protocolo via archivo
    if( operacion == _OPERACION_ANULADA ) {
        operacion = _OFF_LINE_ANULACION;
    }
    switch( operacion ) {
        case _ON_LINE_ANULACION:
            if( MODO_DEVOLUCION || RAM_NOTA_CR > 0 ) {
                operacion = _ON_LINE_ANULACION_DEVOLUCION;
            }
            break;
        case _OFF_LINE_ANULACION:
            if( MODO_DEVOLUCION || RAM_NOTA_CR > 0) {
                operacion = _OFF_LINE_ANULACION_DEVOLUCION;
            }
            break;
    }
    memset( tran_rta, 0, sizeof( struct _datos_transaccion ) );
	SET_MEMORY_INT( __var_tarjetas_operacion, operacion );
	SET_MEMORY_DOUBLE( __var_tarjetas_importe, importe );
	CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
    if( T_ABRIR_TRANSAC2() ) {
        MENSAJE_STRING( S_AREA_TRANSAC2_CERRADA );
        return 0;
    }
    ok1 = T_ENVIAR_TRANSACCION( nodo, operacion, importe );
    if( RAM_TRANSACCION_ENVIADA ) {
        if( rta_terminal ) {
            rta_terminal->cod_rta = ( char )tran_rta->codigo_de_respuesta;
            memcpy( rta_terminal->nro_referencia, tran_rta->retrieval_reference_number, 12 );
            if( tran_rta->autorizacion == 0L && tran_rta->autorizacion_alfa[0] ) {
                strncpy( rta_terminal->autorizacion, tran_rta->autorizacion_alfa,
                         sizeof( rta_terminal->autorizacion ) );
            }
            else {
                sprintf( rta_terminal->autorizacion, "%ld", tran_rta->autorizacion );
            }
            if( config.pais == ECUADOR && tran_rta->codigo_de_respuesta == 0
             && tran_rta->autorizacion == 0 && strcmp( tran_rta->mensaje, "APROBADA" ) ) {
                TRIM( tran_rta->mensaje );
                strncpy( rta_terminal->autorizacion, tran_rta->mensaje,
                         sizeof( rta_terminal->autorizacion ) );
                rta_terminal->autorizacion[sizeof( rta_terminal->autorizacion ) - 1] = 0;
            }
            sprintf( rta_terminal->cupon, "%ld", tran_rta->ticket );
            rta_terminal->lote = tran_rta->lote;
            rta_terminal->saldo = tran_rta->saldo;
            rta_terminal->cuota = tran_rta->importe_cuota;
            memcpy( rta_terminal->mensaje, tran_rta->mensaje, 37 );
            memcpy( rta_terminal->working_key, tran_rta->pin_working_key, 8 );
            rta_terminal->fecha_contable = tran_rta->fecha_contable;
            rta_terminal->fecha_host = tran_rta->fecha_host;
            rta_terminal->hora_host = tran_rta->hora_host;

			memcpy( rta_terminal->nro_cuenta, tran_rta->nro_cuenta2,
                    sizeof( rta_terminal->nro_cuenta ) );
            rta_terminal->tasa_aplicada = tran_rta->tasa_aplicada;
			rta_terminal->monto_compra = tran_rta->monto_compras ;
			rta_terminal->nro_ticket = tran_rta->ticket ;
			
			if( strlen(tran_temp->bloque_encriptado.buffer) > 2) {
				 char mensa2[202];
				 memset(mensa2, 0, sizeof(mensa2));
				 _snprintf(mensa2,sizeof(mensa2)-1, tran_temp->bloque_encriptado.buffer);
				 SET_MEMORY( __ram_buffer, mensa2 );
			  //	 memset( rta_terminal->mensaje, 0, sizeof(rta_terminal->mensaje) );
				// memcpy( rta_terminal->mensaje, "NO", 2 );


				//_SET_MEMORY( __var_tarjetas_mensaje, 0, tran_temp->bloque_encriptado.buffer );
			}
				//memcpy( rta_terminal->mensaje, tran_rta->mensaje, 37 );

		    /*
                     * No los asigna, porque anteriormente no los asignaba en ningun lado.
                     * La estructura rta_terminal estaba definida solamente en PAGO_TAR.C
                     * y a estas variables no eran asignadas.
                     *
                     * char     adherente[9];
                     * float    anticipo;
                     * char     cuotas;
                     * unsigned primer_vencimiento;
                     * char     moneda;
                     * //  0-Pesos , 1-Dolar  2-Cecor
                     */
            if( operacion == _ON_LINE_COMPRA ) {
			   rta_terminal->importe = tran_rta->importe;
            } else {
			   rta_terminal->importe = 0.0;
            }
			
			//if(&tran_rta->bloque_encriptado.buffer[0]!=NULL){				MENSAJE_SIN_SONIDO( &tran_rta->bloque_encriptado.buffer[1],NO );			}
        }
        ok = 1;
    }
    if( OPERACION_OFF_LINE( operacion ) && ok1 ) {
        ok = 1;
    }
    if( OPERACION_OFF_LINE( operacion )  ) {
		if( nodo < 10 && !VAR_TARJETAS_LOTE  ) { //limitados hasta 10 nodos y menores de 10 solo para los offline
			int nro_lote = 0;
			nro_lote = (int) _PAGO_DATAFONO(nodo);
		//la posicion la da el nodo
			SET_MEMORY_INT( __var_tarjetas_lote, nro_lote);
		}
	}
    if( OPERACION_OFF_LINE( operacion ) && RAM_TRANSACCION_ENVIADA ) {
        ok = 2;
    }
    if( ( ok == 1 && rta_terminal ) || ( ok == 2 ) ) {
        if( ( ok == 2 ) || ( rta_terminal->cod_rta == 0 ) ) {
            if( yacalculo_cupon == NO)
            CALCULAR_NRO_CUPON( VAR_TARJETAS_TARJETA );
			yacalculo_cupon = NO;
        }
    }
	if( operacion == _CIERRE_DE_LOTE && ok1 == 1 ) {
		ok = 1; //aca guardar el nro de lote
		if( nodo < 10 ) { 
			int nro_lote = 0;
			nro_lote = (int) _PAGO_DATAFONO(nodo);
			SET_WHERE("NODO = '%d' AND LOTE = '%d'", nodo, nro_lote);
			RUN_QUERY(NO);
			if( RECCOUNT2(NULL,NULL) > 0 ) {
				_ADD_MEMORY_LONG( __pago_datafono, nodo, 1L ); ///aca solo incrementar si hubo operaciones con ese lote un select por nro de lote. > 0 distinto de cierre de lote
			}
	}
	}
    //SET_MEMORY_LONG( __ram_nro_transaccion_pedida, 0 );
    SET_MEMORY_CHAR( __ram_transaccion_enviada, 0 );
    //CLOSE_DATABASE( AREA_TRANSAC2 );
	CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
    CLOSE_RNV_VIRTUAL();
    OPEN_RNV_VIRTUAL();
    INICIALIZAR_BUSCA_TARJETA();
    return ok;
}
/****************************************************************************/
int T_ENVIAR_TRANSACCION( int nodo, int operacion, double importe )
/****************************************************************************/
{
    long nro_tran;
    int ok = 0, cant, fecha_aux =0, cant2 = 0;
    char aux_track2[50],aux_track1[80],aux[50];
    struct _datos_transaccion tran;
    char auxi[100],auxi1[100];
    int _ptr,_handle,_estado, track_aux = 0 ;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );
    /*----------------- Define el protocolo a utilizar -----------------*/
	if( config_tps.NapseModalidad == 1)
		protocolo = NAPSE;
	else
		protocolo = TCP_IP;
	if( _ESTADO == 0 ) {
		memset( aux_track2, 0, sizeof( aux_track2 ) );
        memset( aux_track1, 0, sizeof( aux_track1 ) );
        //char nro_nom_caja[4];
        SET_MEMORY_LONG( __ram_nro_transaccion_pedida, 0 );
        SET_MEMORY_CHAR( __ram_transaccion_enviada, 0 );
        nro_tran = T_CALCULA_NRO_TRANSACCION();
        /*----------------- Graba la transaccion -----------------*/
        memset( &tran, 0, sizeof( tran ) );
       /* if( ( NRO_CAJA + config.offset_tarjetas ) <= 999 ) {
            sprintf( nro_nom_caja, "%.3i", NRO_CAJA + config.offset_tarjetas );
        }
        else {
            MENSAJE_STRING( S_OFFSET_INVALIDO );
            return 0;
        }
        memcpy( tra.nnom_caja, nro_nom_caja, 4 );
		*/
		//VERSIONADO DE DATOS TRANSACCION
		//Producto 3
		tran.nom_caja[0] = 3;
		tran.nom_caja[1] = 0;
		//Version  1
		tran.nom_caja[2] = 1;
		tran.nom_caja[3] = 0;
        //    memcpy( tran.nom_caja, NRO_NOM_CAJA, 4 );
        tran.id_terminal.tipo = 1;
        tran.id_terminal.version = VERSION.version;
        tran.id_terminal.sub_version = VERSION.sub_version;
        tran.id_terminal.revision = VERSION.release;
        //tran.id_terminal.numero      = VAR_TARJETAS_TERMINAL;
        tran.id_terminal.numero = NRO_CAJA + config.offset_tarjetas;
        tran.nodo = nodo;
        tran.tipo_de_tarjeta = VAR_TARJETAS_TIPO_CUENTA ? 1 : 0;    // debito 1   credito 0
        // tipo de cuenta  1 caja de ahorro 2 cta cte
        // modena  0 pesos  1 dolar
        if( VAR_TARJETAS_TIPO_CUENTA ) {
			tran.tipo_de_cuenta = ( ( VAR_TARJETAS_MONEDA == DOLARES ) ? 7 : 0 )
                                + VAR_TARJETAS_TIPO_CUENTA;
			//SE CAMBIA LA CONSULTA POR EL TIPO DE MONEDA QUEDANDO IGUAL A LIBERTAD DOS, CAMBIADO POR PEDIDO DE Seba por jauton, indicaba que era en dolares, cuando en realidad era en pesos
        }
        // ojo no mover esta funcion necesita tran.tipo_de_cuenta
        //si cashplus esta habilitado la operacion deja de ser 0 
		if(config_tps.ActivarCashPlus == 1 && operacion == _ON_LINE_COMPRA && VAR_TARJETAS_ANTICIPO > 0.005 ){
			operacion = _ON_LINE_CASHPLUS;
			SET_MEMORY_INT( __var_tarjetas_operacion, operacion);
		}
		if(config_tps.ActivarCashPlus == 1 && operacion == _ON_LINE_COMPRA && RAM_MODO_EJECUCION == CAMBIO_MEDIO 
			&& TARJ[VAR_TARJETAS_TARJETA ].tipo_tarjeta & _TIPO_CASH_ADVANCE){
			operacion = _ON_LINE_CASHPLUS;//ESTO es para cashplus solo
			SET_MEMORY_INT( __var_tarjetas_operacion, operacion);
		}

		if(config_tps.ActivarCashPlus == 1 && operacion == _ON_LINE_ANULACION && VAR_TARJETAS_ANTICIPO > 0.005 ){
			operacion = _ON_LINE_ANULACION_CASHPLUS;
		}

		BUSCA_MENSAJE_EN_TABLA( operacion, ( int* )&tran.tipo_de_mensaje,
                                ( long* )&( tran.codigo_de_procesamiento ), tran.tipo_de_cuenta );
        tran.codigo_marca = VAR_TARJETAS_TARJETA;
        tran.modo_de_ingreso = VAR_TARJETAS_LECTURA_BANDA;
		memcpy( tran.numero_cuenta_tarjeta, GET_NUMERO_TARJETA_PRIVADO( ), sizeof( tran.numero_cuenta_tarjeta ) );
        fecha_aux = DECREMENTAR_MES( GET_FECHA_VTO_PRIVADA(  )  );
        memset( &aux, 0, sizeof( aux ) );
        if( fecha_aux > 0 )
        DTOA_INVERSA( fecha_aux, aux );

        memcpy( tran.fecha_expiracion, aux, sizeof( tran.fecha_expiracion ) );
        memset( tran.track_1, 0, sizeof( tran.track_1 ) );
        memset( tran.track_2, 0, sizeof( tran.track_2 ) );

        if( VAR_TARJETAS_LECTURA_BANDA == 1 ) {
			/* Se hace esto para identificar la siguiente secuencia:
			1- Abonar con Tarjeta "Aprobada"
			2- Abonar con Tarjeta "Denegada"
			3- Anular Tarjeta "Denegada" con tecla '-'
			4- Abonar importe de la Tarjeta "Denegada" con efectivo
			5- Totalizar*/
			if( operacion == 0 
					&& strlen( GET_TRACK_2_PRIVADO()) < 2 ) {
				if( strlen( GET_TRACK_1_PRIVADO()) < 2 ) {
				/*Bandera para copiar directamente el valor de transac2->dt.track_2 sin procesar*/
					track_aux = 1;
					SELECT_TABLE( T_TRANSAC2, TT_ORIG );
					SET_WHERE("ID_EVENTO = '%ld'and AUTORIZACION_ALFA = '%s'", RAM_ID_EVENTO, VAR_TARJETAS_AUT_ORIGINAL );
					RUN_QUERY(NO);// me posiciono en la tarjeta que se va a aprobar
				}
			}
			
			/* seteo  los var_.. tracks para operar estos controles , luego los vacio */
			//en una anulacion online necesito el track_2
			if( operacion == 2 || operacion == 5 ) { //anulacion on, necesitamos el track1
				if(strlen( GET_TRACK_2_PRIVADO()) < 2 ) {
					if( strlen( GET_TRACK_1_PRIVADO()) < 2 ) {
				SELECT_TABLE( T_TRANSAC2, TT_ORIG );
				SET_WHERE("ID_EVENTO = '%ld'and AUTORIZACION_ALFA = '%s'", RAM_ID_EVENTO, VAR_TARJETAS_AUT_ORIGINAL );
				RUN_QUERY(NO);// me posiciono en la tarjeta que se va a anular
				
				/* traer del original el bloque encriptado y los datos de los tracks */
#ifdef COMPILAR_ENCRIPTACION_TARJETAS
			  if( ENCRIPTAR_DATOS_TARJETA ){
				memcpy(tran.bloque_encriptado.buffer,transac2->dt.bloque_encriptado.buffer,transac2->dt.bloque_encriptado.len);
				tran.bloque_encriptado.len = transac2->dt.bloque_encriptado.len;
				tran.bloque_encriptado.id_clave = transac2->dt.bloque_encriptado.id_clave;
				tran.bloque_encriptado.version_bloque = transac2->dt.bloque_encriptado.version_bloque;
				tran.datos_tarjetas_encriptados=transac2->dt.datos_tarjetas_encriptados;
			}
#endif

				/********************************************************************/
				SET_TRACK_2_PRIVADO( transac2->dt.track_2 );
				SET_TRACK_1_PRIVADO( transac2->dt.track_1 );
					}
				}
			}

			if( operacion == 0 &&  strlen( GET_TRACK_2_PRIVADO()) < 2  ) { //compra ON pero sin trac_2, necesitamos el track1
				SET_TRACK_2_PRIVADO( transac2->dt.track_2 );
				SET_TRACK_1_PRIVADO( transac2->dt.track_1 );
			}

			SET_MEMORY( __var_tarjetas_track1 , GET_TRACK_1_PRIVADO());
			SET_MEMORY( __var_tarjetas_track2 , GET_TRACK_2_PRIVADO());

			if( !track_aux ) {
            cant = _VAR_TARJETAS_TRACK2( 0 );
            if( _VAR_TARJETAS_TRACK2( cant ) == '?' ||  _VAR_TARJETAS_TRACK2( cant ) == '_' ) {
                cant--;
            }
            cant2 = _VAR_TARJETAS_TRACK1( 0 );
            if( _VAR_TARJETAS_TRACK1( cant2 ) == '?' ||  _VAR_TARJETAS_TRACK1( cant2 ) == '_'  ) {
                cant2--;
            }
            memcpy( aux_track2, VAR_TARJETAS_TRACK2, sizeof( aux_track2 ) );
            memcpy( aux_track1, VAR_TARJETAS_TRACK1, sizeof( aux_track1 ) );
			
			/* Se acceden  a traves  de GET_TRAC_n_PRIVADOS() */
			CEREAR_MEMORY( __var_tarjetas_track1 );
			CEREAR_MEMORY( __var_tarjetas_track2 );

			//reemplacemos los caracteres 
			 //track2
			/*4546400012049396¡06121014430000000000_
			  4507990000004905=10031012370000000000" */
			//track1
			//4546400012049396&KLDJALDJDSKLJFLKDSKL &061210110155        00443000000_ñ
			//4507990000004905^PRUEBA/IMPRE         ^100310199999        00237000000

			{ int ii; 
			for(ii=0; aux_track1[ii] != '\0' && ii < cant2 &&ii < sizeof(aux_track1); ii++)  {
					if(aux_track1[ii] == '&' ) {
					aux_track1[ii] = '^';
					}
					if(aux_track1[ii] == '-' ) {
					aux_track1[ii] = '/';
			}
			}
				}

			{ int ii, salir = 0; 
			for(ii=0; aux_track2[ii] != '\0' && ii < cant && salir == 0; ii++) { 
				if(aux_track2[ii] == '¡' ) {
					aux_track2[ii] = '=';
					salir = 1;
				}
			}
			}

            if( cant <= 0 ) {
                cant = 0;
            }
            if( cant > 38 ) {
                cant = 38;
            }
/*				if( operacion == 2 ) { //esto se comenta porque en la anulacion ya pedimos de nuevo el track y no solo lo copiamos
					cant = strlen( aux_track2 );
					memcpy( tran.track_2, &aux_track2[0], cant );
				} else {*/
            memcpy( tran.track_2, &aux_track2[1], cant );
//				}
			tran.track_2[37] = '\0';
            
			if( cant2 <= 0 ) {
                cant2 = 1;       //no pudo leer la tarjeta
                memset( &aux_track1[2], 1, cant2 );
            }
            if( cant2 > 80 ) {
                cant2 = 80;
            }
            //fabiana probando
				//memcpy( tran.track_1, &aux_track1[2], cant2 );
/*			if( operacion == 2 ) { //esto se comenta porque en la anulacion ya pedimos de nuevo el track y no solo lo copiamos
					cant2 = strlen( aux_track1 );
					memcpy( tran.track_1, &aux_track1[0], cant2 );
				} else {*/
            memcpy( tran.track_1, &aux_track1[2], cant2 );
				//}

		   tran.track_1[79] = '\0';
			} else {
				memcpy( aux_track2, VAR_TARJETAS_TRACK2, sizeof( aux_track2 ) );
				memcpy( aux_track1, VAR_TARJETAS_TRACK1, sizeof( aux_track1 ) );
				cant = strlen( aux_track2 );
				cant2 = strlen( aux_track1 );
				memcpy( tran.track_2, &aux_track2[0], cant );
				memcpy( tran.track_1, &aux_track1[0], cant2 );
				/* Se acceden  a traves  de GET_TRAC_n_PRIVADOS() */
				CEREAR_MEMORY( __var_tarjetas_track1 );
				CEREAR_MEMORY( __var_tarjetas_track2 );
        }
		}
        else {
			if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET )
				sprintf( tran.track_2, "%s", "" );
			else {
				char mensa[39];
				memset(mensa,0, sizeof(mensa));
				_snprintf(mensa, sizeof( tran.track_2 ) ,"%s",GET_NUMERO_TARJETA_PRIVADO( ));
				strncpy( &tran.track_2[1], mensa, sizeof( tran.track_2 ) - 1 );
				tran.track_2[0]='\0'; // pisaba el campo anterior fecha y se producia error, aca el trac_2 no es importante con esta configuracio
			//	strncpy( tran.track_2, GET_NUMERO_TARJETA_PRIVADO( )/*VAR_TARJETAS_NUMERO*/, sizeof( tran.track_2 ) );
			}
				

            cant2 = 1;
         

			sprintf( tran.track_1, "%s", "" );

        }
        tran.cvv = VAR_TARJETAS_CVV;        
        if( ( OPERACION_REVERSABLE( operacion ) || !GENERAR_REVERSOS )
         && calcular_nro_cupon_en_netcom2 ) {
			 SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
					OBTENER_NRO_CUPON( VAR_TARJETAS_TARJETA ) );
/*		  if( operacion != _ON_LINE_ANULACION )
            SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
                             CALCULAR_NRO_CUPON( ( char )VAR_TARJETAS_TARJETA ) );
			yacalculo_cupon = SI;*/
        }
        tran.ticket = ( short )( VAR_TARJETAS_NRO_CUPON_INT % 10000L );     
        tran.moneda = MONEDA_EXTENDIDA();

		 if( tran.codigo_marca == 99 && RAM_PEDIR_CLIENTE_AFINIDAD == 2 ){  //AFINIDAD/consulta saldo para que el raptor no la rechace
			tran.cuotas = 0x30;
			memcpy( &tran.plan, "0", 1 );
		} else {
			memcpy( &tran.plan, VAR_TARJETAS_TABLA, 1 );
			tran.cuotas = VAR_TARJETAS_CUOTAS;
		}
         memset( &aux, 0, sizeof( aux ) );
         sprintf(aux,"%01s",VAR_TARJETAS_TABLA);
         strncpy( tran.tabla,aux, sizeof( tran.tabla ) );
        //strncpy( tran.tabla, VAR_TARJETAS_TABLA, sizeof( tran.tabla ) );
		//tran.cuotas = VAR_TARJETAS_CUOTAS;
        tran.importe = ROUND(importe,ENTEROS + 1,2);
        //fabiana.. como es auton dinosaurio reemplazo este codigo para probar si hay error
        //tran.entrega = ( float )VAR_TARJETAS_IMPORTE_ENTREGA;
        //tran.entrega =
        //  ( float ) ROUND( PAGOS_IVA_PROPORCIONAL( -1, importe ), ENTEROS, DECIMALES );
        tran.vigencia = VAR_TARJETAS_COD_VIGENCIA;
        tran.hora = GET_HORA_DOS_BYTE();
        tran.fecha = DTOL( GET_FECHA_DOS() );
		//para las operaciones off se debe cargar la fecha de captura
		if( ES_OPERACION_OFF(operacion))
			tran.fecha_de_captura = DTOL( GET_FECHA_DOS() );
        tran.autorizacion = atol( VAR_TARJETAS_AUTORIZACION );
        strcpy( tran.mensaje, VAR_TARJETAS_AUTORIZACION );
        strncpy( tran.autorizacion_alfa, VAR_TARJETAS_AUTORIZACION,
                 sizeof( tran.autorizacion_alfa ) - 1 );
        tran.numero_de_terminal = VAR_TARJETAS_TERMINAL;
        strcpy( tran.numero_de_comercio, VAR_TARJETAS_NRO_COMERCIO );
        memcpy( tran.retrieval_reference_number, VAR_TARJETAS_NRO_REF,
                sizeof( tran.retrieval_reference_number ) );
        memset( auxi1, 0, sizeof( auxi1 ) );
        memcpy( auxi1, tran.retrieval_reference_number, 12 );
        sprintf( auxi, "envio %s", auxi1 );
		memset( auxi1, 0, sizeof( auxi1 ) );
// no puedo grabar el pin encriptado aca 
//		memcpy( tran.pin_working_key, GET_PIN_PRIVADO( ), sizeof( tran.pin_working_key ) );
		if(GET_PIN_PRIVADO( ) && VAR_TARJETAS_CELDA_PINPAD ) {
			memset( tran.pin_working_key, 0, sizeof( tran.pin_working_key ) ); // esto no lo graba bien
		}
        tran.ticket_original = ( unsigned short ) ( VAR_TARJETAS_CUPON_ORIGINAL % 10000L );
		if( VAR_TARJETAS_FECHA_ORIGINAL > 0)
        tran.fecha_original = DTOL( VAR_TARJETAS_FECHA_ORIGINAL );
		else
			tran.fecha_original = 0;
        tran.nro_boletin = VAR_TARJETAS_NRO_BOLETIN;
        tran.hora_original = VAR_TARJETAS_HORA_ORIGINAL;
        memcpy( tran.autorizacion_original, VAR_TARJETAS_AUT_ORIGINAL,
                sizeof( tran.autorizacion_original ) );
        {
            sprintf( auxi, "ti or %d fec or %ld", tran.ticket_original, tran.fecha_original );
        }
		memset( auxi1, 0, sizeof( auxi1 ) );
        tran.tasa = VAR_TARJETAS_TASA;
		tran.vuelto = VAR_TARJETAS_VUELTO;
        tran.importe_cuota = VAR_TARJETAS_IMPORTE_CUOTA;
        tran.nro_comp_transaccion = ( RAM_FACTURA || RAM_NOTA_CR ) ? RAM_NRO_COMP + 1 :  NRO_TICKET+1/*+1 es el que esta en curso!!*/;
		//tran.nro_comp_transaccion = RAM_NRO_COMP ;//__ram_nro_comp tiene el numero de comprobante actual indpendiente del tipo de comprobante
        tran.fecha_host = VAR_TARJETAS_FECHA_HOST;
        tran.hora_host = VAR_TARJETAS_HORA_HOST;
        tran.l_pin = VAR_TARJETAS_L_PIN;
        if( RAM_NOTA_CR ) {
            tran.nro_comp_transaccion_original = VAR_TARJETAS_CUPON_ORIGINAL;
        }
        else {
            tran.nro_comp_transaccion_original = ( RAM_FACTURA ) ? RAM_NRO_COMP +1: NRO_TICKET+1/*+1 es el que esta en curso!!*/;
        }
		tran.cod_sucursal = config.sucursal;
		tran.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
		tran.id_evento = (	RAM_ANULAR_TICKET > 100 )? RAM_ID_EVENTO - 1: RAM_ID_EVENTO;
		if( config_tps.ActivarCashPlus == 1 && VAR_TARJETAS_ANTICIPO >0.0){
			float var_tarjetas_interes=VAR_TARJETAS_ANTICIPO;
			var_tarjetas_interes = ROUND(var_tarjetas_interes,ENTEROS + 1,2);
			tran.interes = var_tarjetas_interes; //reuso este campo para cashplus
			if (( /*operacion == _ON_LINE_ANULACION_CASHPLUS ||*/ operacion == _ON_LINE_CASHPLUS) &&  RAM_MODO_EJECUCION != CAMBIO_MEDIO)  {
				tran.importe += ROUND(tran.interes,ENTEROS + 1,2); //en cashplus en el campo importe va el acumulado 
				SET_MEMORY_DOUBLE( __var_tarjetas_importe, tran.importe);

			}
		} else
			tran.interes = importe - ( importe / ( 1 + ( VAR_TARJETAS_RECARGO / 100 ) ) ); //MSI
		
        // fecha_de_captura
        // tipo_mendsaje_original
        // numero_de_trace_original
        // lote
        // compras
        // devoluciones
        // anulaciones
        // monto compras
        // monto devoluciones
        // monto anulaciones
		if( T_AGREGAR_TRANSACCION( &tran, nro_tran, operacion ) ) {
            SET_MEMORY_LONG( __ram_nro_transaccion_pedida, nro_tran );
            ok = 1;
            if( OPERACION_OFF_LINE( transac2->operacion ) ) {
                SET_MEMORY_CHAR( __ram_transaccion_enviada, 1 );
            }
            else {
                _SET_ESTADO( 1 );
            }
        }
    }
    if( _ESTADO == 1 ) {
        CARGA_TRANSACCION( RAM_NRO_TRANSACCION_PEDIDA );
        _SET_ESTADO( 2 );
    }
    // ver si se puede sacar
    // para sacar, hay que asegurarse que el si se trabaja con rnv_virtual
    // no se apague (POWER_DOWN ACTIVADO) hasta enviar todas las transacciones,
    // (salvo el caso de que se reciba una operacion = _CIERRE_DE_LOTE)
    CLOSE_RNV_VIRTUAL();
    OPEN_RNV_VIRTUAL();
    if( _ESTADO == 2 ) {
        ok = 1;
        if( MODALIDAD_CANAL_TARJETAS && !canal_tarjetas_abierto ) {
            if( !ABRIR_CANAL_NETCOM2( protocolo, 2 ) ) {
                MENSAJE_STRING( S_ERROR_ABRIENDO_CANAL );
            }
            else {
                T_ENVIAR( nodo, operacion );
                if( MODALIDAD_CANAL_TARJETAS == CANAL_POR_GRUPO ) {
                    if( CERRAR_CANAL_NETCOM2() ) {
                        MENSAJE_STRING( S_ERROR_AL_CERRAR_CANAL );
                    }
                }
            }
        }
        else {
			int ok_aux = 0;
            ok_aux = T_ENVIAR( nodo, operacion );
			if( operacion == _CIERRE_DE_LOTE ) {
				ok = ok_aux;
			}
        }
        MARCAR_ENVIADA_SI_HUBO_ERROR( RAM_NRO_TRANSACCION_PEDIDA, operacion );
    }
    else {
        ok = 0;
    }
    /*-------------------- Termina recupero de memoria -----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
    return ( ok );
}
/****************************************************************************/
int T_ENVIAR( int nodo, int operacion )
/****************************************************************************/
{
    int h;
    int ok = 1;
	if( operacion == _CIERRE_DE_LOTE && config_tps.NapseModalidad == 0) {
        if( CIERRE_DE_LOTE_POR_NODO ) {
            ok = T_ENVIAR_TRANSACCIONES_PENDIENTES( nodo, SI, 3 );
        }
        else {
            // envia offline para todos los nodos
            for( h = 0;h < 9 && ok;h++ ) {
                ok = T_ENVIAR_TRANSACCIONES_PENDIENTES( h, SI, 3 );
            }
            if( ok ) {
                T_ENVIAR_TRANSACCIONES_PENDIENTES( nodo, SI, 3 );
            }
            // envia el cierre de lote
        }
    }
    else {
        T_ENVIAR_TRANSACCIONES_PENDIENTES( nodo, NO, 0 );
    }
	return( ok );
}
/****************************************************************************/
int CARGA_TRANSACCION( long nro_transaccion )
/****************************************************************************/
{
    int rta = 0;
    //SELECT( AREA_TRANSAC2 );
    //SET_ORDER( 1 );
    //GET_EQUAL( ( char* )&nro_transaccion );
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE("ID_TRANSACCION = '%ld'", nro_transaccion );
	RUN_QUERY(NO);
    if( FOUND2() ) {
        if( transac2->enviado ) {
            rta = 1;
            memcpy( tran_rta, &transac2->dt, sizeof( struct _datos_transaccion ) );

        }
    }
    return rta;
}
/****************************************************************************/
void MARCAR_ENVIADA_SI_HUBO_ERROR( long nro_tran, int operacion )
/****************************************************************************/
{
    if( !RAM_TRANSACCION_ENVIADA && OPERACION_UN_SOLO_INTENTO( operacion ) ) {
       // SELECT( AREA_TRANSAC2 );
       // SET_ORDER( 1 );
       // GET_EQUAL( ( char* )&nro_tran );
		SELECT_TABLE( T_TRANSAC2, TT_ORIG );
		SET_WHERE("ID_TRANSACCION = '%ld'", nro_tran );
		RUN_QUERY(NO);	
        if( FOUND2() ) {
            // si se intento enviar y la operacion es en un solo intento
            // ya fue marcada como enviada y no hay que sobreescribir
            // el codigo de error
            if( !transac2->enviado ) {
                transac2->enviado = SI;
                transac2->error = ERROR_MARCADA_COMO_ENVIADA;
                transac2->ultima_fecha_transmision = GET_FECHA_DOS();
                transac2->ultima_hora_transmision = GET_HORA_COMPLETA_DOS();;
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                UPDATE2();
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
        }
    }
}
/****************************************************************************/
void T_REVERSAR_TRANSACCION_SI_ES_NECESARIO()
/****************************************************************************/
{
	if(config_tps.NapseModalidad ==0) {
		if( OPERACION_REVERSABLE( transac2->operacion ) ) {
			/*---------------- Genera el reverso si es necesario ---------------*/
			/*
				 * va a reversar si hay problemas de comunicacion con el auton,
				 * esto es transac2->error != 0
				 * o cuando el auton contesta error de comunicacion, esto es
				 * tran_rta->codigo_de_respuesta == 1
				 *
				 */
			if( transac2->error > 0 ) {
				T__REVERSAR_TRANSACCION( transac2->id_transaccion );
			}
		}
	}
}
/****************************************************************************/
int T__REVERSAR_TRANSACCION( long nro )
/****************************************************************************/
{
    int ok = 0;
    long nro_new;
    struct _datos_transaccion tran;
	char timeout_ = 97;
    nro_new = T_CALCULA_NRO_TRANSACCION();
	
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE("ID_TRANSACCION ='%ld'", nro );
	RUN_QUERY(NO);
    //SELECT( AREA_TRANSAC2 );
    //SET_ORDER( 1 );
    //GET_EQUAL( ( char* )&nro );
    if( FOUND2() ) {
        transac2->dt.tipo_de_mensaje_original = transac2->dt.tipo_de_mensaje;
        transac2->dt.tipo_de_mensaje = TIPO_MENSAJE_REVERSO;
        memcpy( &tran, &transac2->dt, sizeof( struct _datos_transaccion ) );
		if( (transac2->dt.tipo_de_mensaje_original == 200 || transac2->dt.tipo_de_mensaje_original == 220 )&& transac2->dt.codigo_de_procesamiento == 20000 ) {
			int ticket_ori = transac2->dt.ticket_original;
			int nodo = transac2->nodo;
			//SET_WHERE("ID_EVENTO='%li'", transac2->id_evento);
			
			SET_WHERE("id_evento='%li' AND tipo_de_mensaje in( 200,220) AND codigo_de_procesamiento = 0 AND ticket = %li AND error = 0 AND nodo = %i", transac2->id_evento,ticket_ori, nodo );
			RUN_QUERY(NO);

			if( FOUND2()) {
					memcpy(tran.bloque_encriptado.buffer,transac2->dt.bloque_encriptado.buffer,transac2->dt.bloque_encriptado.len);
					tran.bloque_encriptado.len = transac2->dt.bloque_encriptado.len;
					tran.bloque_encriptado.id_clave = transac2->dt.bloque_encriptado.id_clave;
					tran.bloque_encriptado.version_bloque = transac2->dt.bloque_encriptado.version_bloque;
					tran.datos_tarjetas_encriptados=transac2->dt.datos_tarjetas_encriptados;

			}

		}


        if( !ENCRIPTAR_DATOS_TARJETA ) {
            DESENCRIPTA_UN_TRACK( 38, transac2->fecha_generacion, tran.track_2, tran.track_2 );
            DESENCRIPTA_UN_TRACK( 80, transac2->fecha_generacion, tran.track_1, tran.track_1 );
            DESENCRIPTA_UN_TRACK( 2, transac2->fecha_generacion, ( char* )&tran.cvv,
                                  ( char* )&tran.cvv );
        }
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES
		  if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET ){
				PROCESA_DATOS_ENCRIPT_TRANSACCION( NO , nro, &tran );

				if( MODO_DEBUG == 75 || MODO_DEBUG == 80 ){
					
				 char encrip[70];
				 _snprintf(encrip, sizeof(encrip),"#Rev a Desencrip %2i - %3li %s", NO, nro,&tran.numero_cuenta_tarjeta[5]); 
				 encrip[sizeof( encrip)-1]=0;
				 GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,2);
				 if( MODO_DEBUG == 80 )
					MENSAJE(encrip);
				}
		   }
#endif

        //tran.tipo_de_mensaje = TIPO_MENSAJE_REVERSO;
		  //para forzar el reverso inemediato cambio la prioridad, le agrego que sea de una compra para que no reverse reversos inmediatos
		if((transac2->error == timeout_ || transac2->dt.tipo_de_mensaje_original ==TIPO_MENSAJE_COMPRA  ) && transac2->dt.codigo_de_respuesta == 1)
			reverso_inmediato = 1;
        if( T_AGREGAR_TRANSACCION( &tran, nro_new, _REVERSO ) ) {
            // no reevia porque el reverso se genera ante un error en
            // la comunicacion y a esta funcion entra cuando falla la comuni
            // cacion. En la proxima comunicacion va a ser la primer transaccion
            // a enviar
            //T__ENVIAR_TRANSACCIONES_PENDIENTES( nodo );
            ok = 1;
        }
		reverso_inmediato = 0;
    }
    return ( ok );
}

/****************************************************************************/
long OPERACION_SOLICITADA_PARA_REVERSAR(int nro_cupon, int nodo){
/****************************************************************************/
	long operacion = 0;
	
	CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
    
	if( T_ABRIR_TRANSAC2() ) {
        MENSAJE_STRING( S_AREA_TRANSAC2_CERRADA );
        return 0;
    }

	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE("");
	/* podria controlarse la operacion original aunque 
		el Jauton deb conocer exactamente que operacion 
		le esta solicitando
	_ON_LINE_COMPRA                 0
	_ON_LINE_DEVOLUCION             1
	_ON_LINE_ANULACION              2
	*/
	SET_WHERE( " TICKET = '%i' AND NODO = '%i'", nro_cupon, nodo );
	RUN_QUERY( NO );
    
	if( FOUND2() ) {
		operacion = transac2->id_transaccion;
	}
	return operacion;
	
}

/****************************************************************************/
int EXISTEN_REVERSOS_PENDIENTES(int nodo)
/****************************************************************************/
{
    int ok = NO;
    
	if( !OPENED(T_TRANSAC2, TT_ORIG) && T_ABRIR_TRANSAC2() ) {
		    MENSAJE_STRING( S_AREA_TRANSAC2_CERRADA );
    }
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE("NODO1 = '%i ' AND TIPO_DE_MENSAJE ='%i'AND ENVIADO = 0",nodo, TIPO_MENSAJE_REVERSO );
	RUN_QUERY(NO);
	if( FOUND2() ) {
		ok = (int)RECCOUNT2( NULL, NULL );// al menos 1 por eso int...
	}
    return ( ok );
}
/*************************************************************************/
/****************************************************************************/
int T_DUPLICAR_TRANSACCION( long nro )
/****************************************************************************/
{
    int ok = 0;
    long nro_new;
    struct _datos_transaccion tran;
    nro_new = T_CALCULA_NRO_TRANSACCION();
    //SELECT( AREA_TRANSAC2 );
    //SET_ORDER( 1 );
    //GET_EQUAL( ( char* )&nro );
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE("ID_TRANSACCION = '%ld'", nro );
	RUN_QUERY(NO);
    
	if( FOUND2() ) {
        transac2->enviando = 0;
        transac2->enviado = NO;
        transac2->error = 0;
        SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
                         CALCULAR_NRO_CUPON( ( char )transac2->dt.codigo_marca ) );
        transac2->dt.ticket = ( short )( VAR_TARJETAS_NRO_CUPON_INT % 10000L );
        memcpy( &tran, &transac2->dt, sizeof( struct _datos_transaccion ) );


#ifdef DEFINE_ENCRIPTA_TRANSACCIONES
		  if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET ){
				PROCESA_DATOS_ENCRIPT_TRANSACCION( NO , nro, &tran );
				if( MODO_DEBUG == 75 || MODO_DEBUG == 80 ){
					
				 char encrip[70];
				 _snprintf(encrip, sizeof(encrip),"#Rev a Desencrip %2i - %3li %s", NO, nro,&tran.numero_cuenta_tarjeta[5]);
				 encrip[sizeof( encrip)-1]=0;
				 GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,2);
				 if( MODO_DEBUG == 80 )
					MENSAJE(encrip);
				}
		   }
#endif        
        
        
        if( T_AGREGAR_TRANSACCION( &tran, nro_new, transac2->operacion ) ) {
            SET_MEMORY_LONG( __ram_nro_transaccion_pedida, transac2->id_transaccion );
            ok = 1;
        }
    }
    return ( ok );
}
/****************************************************************************/
int T_ABRIR_TRANSAC2()
/****************************************************************************/
{
	int ok = 1;
    //return ( USE_DB( AREA_TRANSAC2, _TRANSAC2_SIC, ( char* )transac2,
    //                 sizeof( struct _transac2_sic ), NULL, 0, 0 ) );
	memset( transac2, 0, sizeof( struct _transac2 ) );
	if( OPEN_TABLE( T_TRANSAC2, TT_ORIG, ( char* )transac2, 
					sizeof( struct _transac2 ) ) == 0 ) {
		SET_WHERE("");
		RUN_QUERY(NO);
		return( 0 );
}
	return( ok );

}
/****************************************************************************/
long T_CALCULA_NRO_TRANSACCION()
/****************************************************************************/
{
    long nro = 0,actual = 0;
//	char wheres[100];


	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
//	strcpy(wheres, GET_WHERE ());
	SET_ORDER2( "ID_TRANSACCION" );
	SET_WHERE("");
	RUN_QUERY(NO);

    if( RECCOUNT2( NULL, NULL ) == 0 ) {
        nro = 1;
    }
    else {
        actual = transac2->id_transaccion;
        GO2( BOTTOM );
        nro = transac2->id_transaccion + 1;
		SET_WHERE("ID_TRANSACCION = '%ld'", actual );
		SET_ORDER2( "" );//Esto es para borrar el order by anterior y no se concatene con esta sentencia
		RUN_QUERY(NO);
    }

    return ( nro );
}
/****************************************************************************/
int T_AGREGAR_TRANSACCION( struct _datos_transaccion *tran, long nro, int operacion )
/****************************************************************************/
{
    int rta = 0;
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
    memset( transac2, 0, sizeof( struct _transac2 ) );
    transac2->id_transaccion = nro;
    transac2->operacion = operacion;
    transac2->fecha_generacion = GET_FECHA_DOS();
    transac2->hora_generacion = GET_HORA_DOS();
	if( reverso_inmediato ==1 && operacion == _REVERSO )
		transac2->prioridad = (char) 11; 
	else
		transac2->prioridad = OPERACION_PRIORIDAD( operacion );
    transac2->cod_sucursal = config.sucursal;
    if( operacion == _CIERRE_DE_LOTE ) {
        // Ya se emitio la Z y se incremento en nro de Z, por eso se resta 1.
        transac2->caja_z = ( long )( NRO_CAJA )*100000L + ( ( NRO_Z - 1 ) % 100000L );
    }
    else {
        transac2->caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    }
    {
        char aux[50];
        sprintf( aux, "NRO_Z_EMITIDA %d tipo_evento %d", NRO_Z_EMITIDA, event_ticket.tipo_evento );
        glog( aux ,LOG_VENTAS,4);
    }
    if( event_ticket.tipo_evento == 10 ) {
        // si estoy sobre el evento de cierre
        // tomar los datos del evento de cierre
        transac2->id_evento = event_ticket.id_evento;
        transac2->fecha_ticket = event_ticket.fecha;
    }
    else {
        //if( NRO_FECHA_OPERACION == 0 )
        /* SEBA 18/12/02 Si no entre al if anterior y la fecha es 0, entonces puede ser
           que alguien haya movido la ubicacion del event_ticket, que normalmente
           est  sobre el evento de cierre cuando la fecha es 0 */
        transac2->id_evento = (	RAM_ANULAR_TICKET > 100 )? RAM_ID_EVENTO - 1: RAM_ID_EVENTO;
        transac2->fecha_ticket = ( NRO_FECHA_OPERACION ) ? NRO_FECHA_OPERACION : _GET_FECHA_DOS();
    }
    memcpy( &transac2->dt, tran, sizeof( struct _datos_transaccion ) );
    transac2->nodo = transac2->dt.nodo;
	if( transac2->dt.modo_de_ingreso == 1 && ES_OPERACION_OFF(operacion) && transac2->dt.track_2 != NULL &&  strlen(transac2->dt.track_2) < 1 ) {
		transac2->dt.modo_de_ingreso = 0;
	}
    if( ENCRIPTAR_DATOS_TARJETA ) {
        //--- Encriptacion con claves configurables.
        if(operacion != _REVERSO &&  operacion != _ON_LINE_ANULACION/* && operacion != _OFF_LINE_ANULACION*/) {
            T_ENCRIPTAR_DATOS_TARJETA();
        }
    }
    else {
        //--- Encriptacion normal
        ENCRIPTA_UN_TRACK( 38, transac2->fecha_generacion, ( char* )transac2->dt.track_2,
                           ( char* )transac2->dt.track_2 );
        ENCRIPTA_UN_TRACK( 80, transac2->fecha_generacion, ( char* )transac2->dt.track_1,
                           ( char* )transac2->dt.track_1 );
        ENCRIPTA_UN_TRACK( 2, transac2->fecha_generacion, ( char* )&transac2->dt.cvv,
                           ( char* )&transac2->dt.cvv );
    }
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES
	//ver aca por que quedan los datos!!!!!
	if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET ){
		 //if( CEREAR_DATOS_TARJETA && OPERACION_OFF_LINE( operacion ))
		  /*if( MODO_DEBUG == 75 || MODO_DEBUG == 80 )*/{
			 char encrip[70];
			 _snprintf(encrip, sizeof(encrip),"#Llam a encrip %2i - %3li %s", SI, nro,&transac2->dt.numero_cuenta_tarjeta[5]);
			 encrip[sizeof( encrip)-1]=0;
			 GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,2);
			 if( MODO_DEBUG == 80 )
				MENSAJE(encrip);
		  }
		  PROCESA_DATOS_ENCRIPT_TRANSACCION( SI , nro, &transac2->dt );
		  //prueba----------->dnc
		  //PROCESA_DATOS_ENCRIPT_TRANSACCION( NO , nro, &transac2->dt );

	   }
#endif
    #ifdef INVEL_L
  //  BEGIN_TRANSACTION();
    #endif
    if( INSERT2(NO) == 0 ) {//aca no tienen que estar los datos respaldados en la tran
        rta = 1;
    }else
		GRABAR_LOG_SISTEMA("ERR;:GRAB:TRASC2",LOG_DEBUG,2);
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    return ( rta );
}
/****************************************************************************/
void T_CIERRE( int tipo_de_cierre ) // envios.c
/****************************************************************************/
{
    int ok = 0, ok_backup, ok_nodo, intentos, h, nodo = 0;
    struct _nodos nodos;
    if( CIERRE_DE_LOTE_POR_NODO ) {
        if( /*USE_DB( AREA_AUX, _NODOS_SIC, ( char* )&nodos, sizeof( nodos ), NULL, 0, 0 )*/
			OPEN_TABLE(T_NODOS, TT_ORIG,( char* )&nodos, sizeof( nodos ))== 0 ) {
			SET_ORDER2("COD_NODO");
			SET_WHERE("");
			RUN_QUERY(NO);
            ok = 1;
            while( !dbEOF() ) {
                intentos = 0;
                ok_nodo = 0;
				nodo = nodos.cod_nodo;
                while( !ok_nodo && intentos < 1 ) {
                    ok_nodo = ENVIAR_TRANSACCION_AL_AUT_ON( _CIERRE_DE_LOTE, 0, NULL,
                                                            nodos.cod_nodo );
                    intentos++;
                }
                if( !ok_nodo ) {
                    ok = 0;
                }
				if( !OPENED( T_NODOS, TT_ORIG ) ) {
					OPEN_TABLE(T_NODOS, TT_ORIG,( char* )&nodos, sizeof( nodos ));
					SET_ORDER2("COD_NODO");
					SET_WHERE("COD_NODO >= '%i'",nodo);
					RUN_QUERY(NO);
				} else {
					SELECT_TABLE( T_NODOS, TT_ORIG );
				}
                SKIP2( 1 );
            }
            CLOSE_TABLE( T_NODOS, TT_ORIG );
        }
    }
    else {
        if( GENERAR_CIERRE_DE_LOTE ) {
            // el nodo es 10 , para que no se confunda con ningun otro.
            // la idea es que sea un numero de nodo que no se use
            ok = 0;
            intentos = 0;
            while( !ok && intentos < 3 ) {
                ok = ENVIAR_TRANSACCION_AL_AUT_ON( _CIERRE_DE_LOTE, 0, NULL, 10 );
                intentos++;
            }
        }
        else {
            //CLOSE_DATABASE( AREA_TRANSAC2 );
			CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
            if( T_ABRIR_TRANSAC2() == 0 ) {
                ok = 1;
                for( h = 0;h < 9 && ok;h++ ) {
                    ok = T_ENVIAR_TRANSACCIONES_PENDIENTES( h, SI, 3 );
                }
                //CLOSE_DATABASE( AREA_TRANSAC2 );
				CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
            }
            else {
                MENSAJE_STRING( S_AREA_TRANSAC2_CERRADA );
            }
        }
    }
    if( tipo_de_cierre == _CIERRE_DE_LOTE_EN_Z ) {
        // el cierre manual no debe hacer back up
        ok_backup = BACKUP_TRANSAC2();
        if( ok && ok_backup ) {
			SET_EMPTY_TABLE(T_TRANSAC2,TT_ORIG);
            //COPY( _TRANSAC2_000, _TRANSAC2_SIC );
        }
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES        
		if( ok && ok_backup){
				SET_EMPTY_TABLE(T_TRAN_CRP,TT_ORIG); 
				//COPY( _TRAN_CRP_000, _TRAN_CRP_SIC );
        }
#endif        
        
    }
}
/****************************************************************************/
void T_BUSCAR_PRIMERA_TRANSACCION_A_ENVIAR( int nodo )
/****************************************************************************/
{
    //SET_ORDER( 2 );
    ind2_transac2.nodo = ( char ) nodo;
    ind2_transac2.enviado = ( char ) NO;
    ind2_transac2.enviando = ( char ) NO;
   // ind2_transac2.prioridad = ( char ) 0;
   // ind2_transac2.id_transaccion = 0;
   // GET_GREATER_E( ( char* )&ind2_transac2 );
    /*SET_WHERE("NODO >= '%d' AND ENVIADO >= '%d' AND ENVIANDO >= '%d'",
			ind2_transac2.nodo, ind2_transac2.enviado , ind2_transac2.enviando );*/

	SET_WHERE("NODO >= '%d' AND ENVIADO = '%d' AND ENVIANDO >= '%d'",
			ind2_transac2.nodo, ind2_transac2.enviado , ind2_transac2.enviando );
	//SET_ORDER2( "NODO, ENVIADO, ENVIANDO, PRIORIDAD, ID_TRANSACCION" );
	//SET_ORDER2( "NODO, ENVIADO, TICKET ,ENVIANDO ,PRIORIDAD, ID_TRANSACCION" );
	SET_ORDER2( "NODO, ENVIADO,ENVIANDO,PRIORIDAD desc, TICKET , ID_TRANSACCION" );

}
/****************************************************************************/
static int T_ENVIAR_TRANSACCIONES_PENDIENTES( int nodo, int enviar_todas_las_operaciones_off_line,
                                              int reintentos )
/****************************************************************************/
{
    int error = 0, ok = 1, rtaupdate=0;
    int cantidad_off_line = 0;
    long pos1,pos2;
    int error_al_abrir_el_canal = 0, l_buffer_out;
    int salir = 0;
    char mensa[] = "ESTABLECIENDO COMUNICACION";
    //char      mensa2[] = "abrio CANAL";
    char mensa3[] = "TERMINANDO COMUNICACION";
    char wheres[100];

	char pin_working_key_temp[8];
    //long ultimo_id=0;
    /*----------------- Define el protocolo a utilizar -----------------*/
    /*---------------- Recorre los registros pendientes ----------------*/
    //SELECT( AREA_TRANSAC2 );
	memset(pin_working_key_temp,0, sizeof(pin_working_key_temp));
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	//mmancuso
	strcpy(wheres, GET_WHERE ());
	T_BUSCAR_PRIMERA_TRANSACCION_A_ENVIAR( nodo );
	RUN_QUERY(NO);
    ok = 0;
	while( 1 ) {
	     if( dbEOF()) { //no hay para posicionarse
			  if( !error_al_abrir_el_canal && error ) {
				  if( ES_OPERACION_OFF( transac2->operacion ) && OPERACION_REVERSABLE( transac2->operacion ) )
					  SKIP2(-1); //quedaba posicionado eof por el ultimo skip y no analizaba si la operacion era reversable y generar reverso
			  }	else { SET_WHERE("%s",wheres);
			 RUN_QUERY(NO);
		  }
		  }

        pos1 = GET_POSITION_REG();
       // se lo comente  SKIP2( 1 );
        pos2 = GET_POSITION_REG();
 		GET_DIRECT_REG( pos1 , GET_PAGINA_REG());
        if( transac2->nodo == ind2_transac2.nodo && !transac2->enviado && !dbEOF() && !error
         && !salir
         && ( cantidad_off_line < _CANTIDAD_OFF_LINE_A_ENVIAR
           || enviar_todas_las_operaciones_off_line ) ) {
        }
        else {
			if( !error_al_abrir_el_canal && error ) { //hubo error y me movi con el ultimo skip2, vuelvo
				if( !dbEOF() || !dbBOF()) {
					SKIP2(-1);
					transac2->error = error; //por alguna razon el error se pierde cuando se hace el SKIP2(-1)
				}
			}
            break;
        }

        if( transac2->enviando && rtaupdate == 0) {
			//el update de enviando = 0 tiene qu haber funcionado sino no hay que corregir la transaccion
            // si se apaga la POS en la mitad de la transaccion
            T_CORREGIR_ENVIO_FALLIDO();
            pos2 = GET_POSITION_REG();
        }
		else {
			if( MODALIDAD_CANAL_TARJETAS == CANAL_TRANSACCION ) {
                //#if defined(INVEL_W) || defined(INVEL_L) 
                //MENSAJE( mensa );
                //#else
                MENSAJE_SIN_SONIDO( mensa,NO );
                //#endif
                if( !ABRIR_CANAL_NETCOM2( protocolo, reintentos ) ) { 
                    error_al_abrir_el_canal = 1;
                    error = 1;
                    break;
                }
            }
            transac2->enviando = 1;
            #ifdef INVEL_L
           // BEGIN_TRANSACTION();
            #endif
            UPDATE2();
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            {
                char aa[100];
				sprintf( aa, "VA A ENVIAR PAQUETE Opera:%d id: %ld", transac2->operacion,
                         transac2->id_transaccion );
                GRABAR_LOG_SISTEMA( aa ,LOG_DEBUG,2);
            }
            memset( tran_temp, 0, sizeof( struct _datos_transaccion ) );
            memset( buffer_out, 0, LEN_BUFFER_TCP );
			memset( buffer_in, 0, LEN_BUFFER_TCP );   
            //          memcpy( tran_temp, &transac2->dt, sizeof( struct _datos_transaccion ) );
	
			

#ifdef DEFINE_ENCRIPTA_TRANSACCIONES
			if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET ){		
				 //if( CEREAR_DATOS_TARJETA && OPERACION_OFF_LINE( transac2->operacion ))
				 {
					 PROCESA_DATOS_ENCRIPT_TRANSACCION( NO , transac2->id_transaccion, tran_temp );
					 memcpy( transac2->dt.numero_cuenta_tarjeta,tran_temp->numero_cuenta_tarjeta,sizeof( transac2->dt.numero_cuenta_tarjeta ) );//esto deja el numero para cuando chequeo la integridad
					 memcpy( transac2->dt.numero_de_comercio,tran_temp->numero_de_comercio, sizeof(transac2->dt.numero_de_comercio));
					 memcpy( transac2->dt.retrieval_reference_number,tran_temp->retrieval_reference_number, sizeof( transac2->dt.retrieval_reference_number) );

					/**if( MODO_DEBUG == 75 || MODO_DEBUG == 80 )*/{
						  char encrip[70];
						  _snprintf(encrip, sizeof(encrip),"#Llam a desencrip %i - %li - %s", NO, transac2->id_transaccion, &transac2->dt.numero_cuenta_tarjeta[5]);
			 			  encrip[sizeof( encrip)-1]=0;
						  GRABAR_LOG_SISTEMA(encrip, LOG_DEBUG,2);
						  if( MODO_DEBUG == 80 )
							 MENSAJE(encrip);
					}
				 }
			}//dnc
#endif
#ifdef COMPILAR_ENCRIPTACION_TARJETAS
			  if( ENCRIPTAR_DATOS_TARJETA ){
				int tam=transac2->dt.bloque_encriptado.len,tam2=0;
				
				
				memcpy(buffer_in,&transac2->dt.bloque_encriptado.buffer[0],transac2->dt.bloque_encriptado.len);

				CONVERSION_ASCII_EXT_A_TO_HEXA( 1,
				transac2->dt.bloque_encriptado.buffer,
				&tam2,
				buffer_in,
				&tam);
					transac2->dt.bloque_encriptado.len = tam2;
			
			
			}
#endif
  	// CARGO SI TENGO PINWORKING
			  if( transac2->dt.tipo_de_cuenta  > 0 && VAR_TARJETAS_CELDA_PINPAD ) { //debito
				  //cargo el pin para mandarlo al juton
					  PINPAD_LEER_pin_WORKING_KEY( transac2->dt.pin_working_key);
					  if( transac2->dt.tipo_de_mensaje == 200 && transac2->dt.codigo_de_procesamiento!= 20000 )//compra -anulacion
						  TARJETA_DEBITO++;
			  }


			NETCOM2_ARMAR_PAQUETE_REQ( transac2, buffer_out, &l_buffer_out );
			memset( buffer_in, 0, LEN_BUFFER_TCP ); 
			reverso_inmediato = 0;

//aca cambiar para napse
            error = _ENVIAR_PAQUETE( ( int )protocolo, ( char* )buffer_out, l_buffer_out,   //envio
                                     ( char* )buffer_in,
                                     /*LEN_BUFFER_TCP*/( sizeof( struct _datos_transaccion )
                                                       - sizeof( transac2->dt.reserva1 ) ),    //rta
                                                         ( int )TIMEOUT_NODO( ( char )nodo ),
                                     ( char )OPERACION_ESPERA_RTA( transac2->operacion ),
                                     ( int )_CANAL_TARJETAS );        
            //         error = _ENVIAR_PAQUETE( ( int ) protocolo, ( char * ) tran_temp, ( sizeof( struct _datos_transaccion ) - sizeof( transac2->dt.reserva1 ) ), //envio
            //            ( char * ) tran_temp, ( sizeof( struct _datos_transaccion ) - sizeof( transac2->dt.reserva1 ) ),   //rta
            //            ( int ) TIMEOUT_NODO( nodo ),
            //            ( char ) OPERACION_ESPERA_RTA( transac2->operacion ),
            //            ( int ) _CANAL_TARJETAS );
            if( !error ) {
                error = NETCOM2_PROCESAR_PAQUETE_RTA( buffer_in, tran_temp, transac2 );
            }
            else {
				if(error == 4 && OPERACION_REVERSABLE( transac2->operacion)  )
					reverso_inmediato = 1;
                
				transac2->dt.codigo_de_respuesta = 1;
                error = 1;
            }
		//aca deberiabos borrar el archivo ver que pasa en las anulaciones del pinworkingkey
			//if( transac2->dt.tipo_de_cuenta  > 0 && VAR_TARJETAS_CELDA_PINPAD )
				//unlink( _ARCH_PWK );
			//	haydebitos = SI ;
			//
			if( !error && NETCOM2_ENVIAR_CONFIRMACION( transac2, tran_temp ) ) {
                NETCOM2_ARMAR_PAQUETE_CONFIRMACION( transac2, buffer_out, &l_buffer_out,
                                                    tran_temp );
                CERRAR_CANAL_NETCOM2();
                if( ABRIR_CANAL_NETCOM2( protocolo, reintentos ) ) {
                    error = _ENVIAR_PAQUETE( ( int )protocolo, ( char* )buffer_out, l_buffer_out,
                                             ( char* )buffer_in, LEN_BUFFER_TCP,
                                             ( int )TIMEOUT_NODO( ( char )nodo ), SI,
                                             ( int )_CANAL_TARJETAS );
                    //--Que recibe de respuesta del completion??
				}
                else {
                    transac2->dt.codigo_de_respuesta = 1;
                    error = 1;
                }
            }
			if( MODALIDAD_CANAL_TARJETAS == CANAL_TRANSACCION ) {
                //#if !defined(INVEL_W) && !defined(INVEL_L)
                MENSAJE_SIN_SONIDO( mensa3,NO );
                //#else
                //MENSAJE( mensa3 );
                //#endif
                if( !CERRAR_CANAL_NETCOM2() ) {
                    salir = 1;
                }
                BORRAR_MENSAJE();
            }
            transac2->enviando = 0;
            if( !error ) {
                // verificar integridad
				if( transac2->operacion != _CIERRE_DE_LOTE ) { //cierre de lote no devuelve rta.
					error = CHEQUEA_INTEGRIDAD(  );
				}
				if( !error ) {
                    // el aut-on me contesta que tubo error de comunicacion
                    COPIAR_RTA();
                    if( transac2->dt.codigo_de_respuesta == 1
                     || (( transac2->operacion == _REVERSO
                         || OPERACION_OFF_LINE( transac2->operacion ) )
                       && transac2->dt.codigo_de_respuesta == 3 ) ) {
                        error = ERROR_COMUNICACION_AUT_ON_CON_NODO;
                    }
                }
            }
            {
				char mens[80];
                sprintf( mens, "Error: %i Respuesta: %i", error, transac2->dt.codigo_de_respuesta );
                if( error ==0 && transac2->dt.codigo_de_respuesta == 0 &&
					(transac2->operacion == _ON_LINE_ANULACION || transac2->operacion == _ON_LINE_ANULACION_CASHPLUS)) {
						EJECUTAR_SCRIPT_AUTOIT("killscript.exe","notificaciones", NO );//cierra cualquier notificacion 
						MOSTRAR_MENSAJE_UBUNTU( 2, "TARJETA ANULADA CORRECTAMENTE", 4 );

				}


				GRABAR_LOG_SISTEMA( mens ,LOG_DEBUG,2);
            }
            transac2->error = error;
            if( !error || ( error && OPERACION_UN_SOLO_INTENTO( transac2->operacion ) ) ) {
                transac2->enviado = SI;
            }
			//agregar a este if los campos de respuesta que falten copiar a la transac2
			if( !error ){ 
				transac2->dt.monto_compras = tran_temp->monto_compras;
			}
			
			transac2->ultima_fecha_transmision = GET_FECHA_DOS();
            transac2->ultima_hora_transmision = GET_HORA_COMPLETA_DOS();
			{
				int tam=transac2->dt.bloque_encriptado.len,tam2=0;
				
				
				memcpy(buffer_in,&transac2->dt.bloque_encriptado.buffer[0],transac2->dt.bloque_encriptado.len);

				CONVERSION_ASCII_EXT_A_TO_HEXA( 0,
				buffer_in,
				&tam,
				transac2->dt.bloque_encriptado.buffer,
				&tam2);
					transac2->dt.bloque_encriptado.len = tam2;
			   // OCULTA_DATOS_ENCRIPTADOS( &transac2->dt ); //ya recibi la respuesta
			
			}
			if( transac2->dt.tipo_de_cuenta  > 0 ) { //debito
				 //aca como tengo que actualiza la respuesta del jauton y si viene con working key tengo que "modificar" el campo
				//controla el retrieve reference number
  				memcpy(pin_working_key_temp,transac2->dt.pin_working_key , sizeof(pin_working_key_temp));   
				//memcpy( transac2->dt.pin_working_key, 0, sizeof( transac2->dt.pin_working_key ) ); // esto no lo graba bien
				memset( transac2->dt.pin_working_key, 0, sizeof( transac2->dt.pin_working_key ) ); // esto no lo graba bien
				//transac2->dt.pin_working_key[0]='\0'; // esto no lo graba bien
			
			}
            SELECT_TABLE( T_TRANSAC2, TT_ORIG );
			#ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
			//ojo con que falle este update si es 1 es error!!
			{
				int algo = strlen(transac2->dt.retrieval_reference_number);
				if(algo > 12 && transac2->dt.codigo_de_respuesta == 3 )
					transac2->dt.retrieval_reference_number[11]='\0';

			}
            rtaupdate = UPDATE2();
			if(rtaupdate == 1)
				GRABAR_LOG_SISTEMA( "ERROR AL UPDATEAR transac2 con la respuesta" ,LOG_ERRORES,3);

            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            if( !error && transac2->id_transaccion == RAM_NRO_TRANSACCION_PEDIDA ) {
				int ticket_pedido=0;
				if( transac2->dt.codigo_de_respuesta ==7 ){
					//cuando la respuesta es 7, indica que nos piden un ticket en particular para reversar
					ticket_pedido=transac2->dt.ticket_original;
				}

                SET_MEMORY_CHAR( __ram_transaccion_enviada, 1 );
                memcpy( tran_rta, &transac2->dt, sizeof( struct _datos_transaccion ) );
				
				if( transac2->dt.codigo_de_respuesta ==7 ){
					tran_rta->ticket = ticket_pedido;
				}
				
				if( transac2->dt.tipo_de_cuenta  > 0 )  //debito necesito para acualizar workinkey
					memcpy( tran_rta->pin_working_key, pin_working_key_temp, 8);

                CLOSE_RNV_VIRTUAL();
                OPEN_RNV_VIRTUAL();
            }
            if( OPERACION_OFF_LINE( transac2->operacion ) ) {
                cantidad_off_line++;
            }
        }
        // si hay error tengo que seguir posicionado en el mismo
        // registro para poder reversar la operacion
        //ultimo_id = transac2->id_transaccion;
      //  if( !error ) {
			SKIP2( 1 ); //no hubo error pasemos al siguiente registro si hay
			//GET_DIRECT_REG( pos2 , GET_PAGINA_REG());
       // }
    }
    if( !error ) {
        ok = 1;
    }
    // tambien hay que preguntar por error por que si las transacciones
    // que se envian ninguna da error, no es necesario reversarlas
    // es mas, si no se pregunta, va a reversar una transaccion, que
    // ya fue enviada y dio error. (se reversa algo que no hace falta reversar)
    // SOLO HAY QUE REVERSAR LA ULTIMA TRANSACCION ENVIADA QUE FALLO
    // O SEA QUE DIO ERROR
	if( !error_al_abrir_el_canal && error ) {
		SET_WHERE("");
		SET_ORDER2( "" );//Esto es para borrar el order by anterior y no se concatene con esta sentencia
		RUN_QUERY(NO);
		GO2(BOTTOM);
		T_REVERSAR_TRANSACCION_SI_ES_NECESARIO();
		reverso_inmediato = 0;
    }
	OCULTA_DATOS_ENCRIPTADOS( &transac2->dt ); //ya recibi la respuesta
    if( error && MODALIDAD_CANAL_TARJETAS == CANAL_PERMANENTE ) {
        CERRAR_CANAL_NETCOM2();
    }
	
    return ( ok );
}
/****************************************************************************/
int ABRIR_CANAL_NETCOM2( int protocolo, int reintentos )
/****************************************************************************/
{
    int cantidad = 0;
    int abrio_canal = 0, canal;
    char nomip[20];
    //char      mensa1[] = "ERROR AL ABRIR CANAL";
    char auxi[100];
    int rta = 1;
    //canal = ( PUERTO_CANAL_TCP_IP << 3 ) + _CANAL_TARJETAS; // Puerto + Canal
    canal = _CANAL_TARJETAS;
    strcpy( nomip, NOM_CANAL_TCP_IP );
    do {
        rta = ABRIR_CANAL( protocolo, canal, nomip, NETCOM2_FORMATO_MENSAJE_TCP() );
        if( !rta ) {
            #if defined(INVEL_W) || defined(INVEL_L) 
            MENSAJE_SIN_SONIDO( "ERROR ABRIENDO CANAL",NO );
            #else
            MENSAJE_STRING( S_ERROR_ABRIENDO_CANAL );
            #endif
            PAUSA( 36 );
            if( reintentos ) {
                sprintf( auxi, "error al abrir canal %i intento %d", canal, cantidad + 1 );
                GRABAR_LOG_SISTEMA( auxi ,LOG_COMUNICACIONES,3);
            }
            cantidad++;
        }
        else {
            abrio_canal = 1;
        }
    }
    while( cantidad < reintentos && !abrio_canal );
    canal_tarjetas_abierto = abrio_canal;
    return rta;
}
/****************************************************************************/
int CERRAR_CANAL_NETCOM2()
/****************************************************************************/
{
    int rta;
    rta = CERRAR_CANAL( protocolo, _CANAL_TARJETAS );
    canal_tarjetas_abierto = 0;
    return ( rta );
}
/****************************************************************************/
void T_CORREGIR_ENVIO_FALLIDO()
/****************************************************************************/
{
    long pos1;
   // pos1 = GET_POSITION();
	pos1 = GET_POSITION_REG();
    if( OPERACION_REVERSABLE( transac2->operacion ) ) {
        transac2->enviando = 0;
    }
    transac2->enviando = 0;
    transac2->error = ERROR_FALLO_AL_ENVIAR;
    if( OPERACION_UN_SOLO_INTENTO( transac2->operacion ) ) {
        transac2->enviado = SI;
    }
    #ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
    UPDATE2();
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    T_REVERSAR_TRANSACCION_SI_ES_NECESARIO();
    //GET_DIRECT( pos1 );
	GET_DIRECT_REG( pos1 , GET_PAGINA_REG());
    if( OPERACION_UN_SOLO_INTENTO( transac2->operacion ) ) {
        T_DUPLICAR_TRANSACCION( transac2->id_transaccion );
    }
    T_BUSCAR_PRIMERA_TRANSACCION_A_ENVIAR( transac2->nodo );
    CLOSE_RNV_VIRTUAL();
    OPEN_RNV_VIRTUAL();
}
/****************************************************************************/
void T_COMIENZO_NETCOM2()
/****************************************************************************/
{
    transac2 = NULL;
    tran_rta = NULL;
    tran_temp = NULL;
    buffer_out = NULL;
    buffer_in = NULL;
    if( PROTOCOLO_AUTORIZACION_TARJETA == _TCP_IP ) {
        transac2 = (struct _transac2 *)MALLOC( sizeof( struct _transac2 ) );
        tran_rta = (struct _datos_transaccion *)MALLOC( sizeof( struct _datos_transaccion ) );
        tran_temp = (struct _datos_transaccion *)MALLOC( sizeof( struct _datos_transaccion ) );
        buffer_out = (char *)MALLOC( LEN_BUFFER_TCP );
        buffer_in = (char *)MALLOC( LEN_BUFFER_TCP );
		respuestaNapse =  (struct _respuesta_napse *)MALLOC( sizeof( struct _respuesta_napse) );
		
        if( !tran_rta || !transac2 || !buffer_out || !buffer_in ) {
            ABORT( "ERROR AL CARGAR NETCOM2" );
        }
        CARGA_TABLA_OPERACIONES();
        if( MODELO_TEF == _TEF_SWITCH_NCR ) {
            if( !SWITCH_NCR_CARGAR_RESPUESTAS() ) {
                ABORT( "ERROR AL CARGAR SWITCH" );
            }
        }
    }
}
/****************************************************************************/
void LIBERA_MEMORIA_NETCOM2()
/****************************************************************************/
{
    if( transac2 ) {
        free( transac2 );
    }
    if( tran_rta ) {
        free( tran_rta );
    }
    if( tran_rta ) {
        free( tran_temp );
    }
}
/****************************************************************************/
void CARGA_TABLA_OPERACIONES()
/*las prioridades son inversas, es decir a menor numero mayor prioridad
REVERSOS
OFFLINE
ONLINE
LOTE
ECHO TEST
*/
/****************************************************************************/
{
    tabla_operaciones = (struct _codigos_operacion *)calloc( CANTIDAD_OPERACIONES, sizeof( struct _codigos_operacion ) );
    if( tabla_operaciones ) {
        memset( tabla_operaciones, 0, CANTIDAD_OPERACIONES * sizeof( struct _codigos_operacion ) );
        CARGA_TABLA( _ON_LINE_COMPRA, 200, 0L, REVERSABLE, 8, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _OFF_LINE_COMPRA, 220, 0L, REVERSABLE, 3, INTENTAR_VARIAS_VECES,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_ANULACION, 200, 20000L, REVERSABLE, 9, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _OFF_LINE_ANULACION, 220, 20000L, REVERSABLE, 3, INTENTAR_VARIAS_VECES,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_DEVOLUCION, 200, 200000L, REVERSABLE, 8, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _OFF_LINE_DEVOLUCION, 220, 200000L, REVERSABLE, 3, INTENTAR_VARIAS_VECES,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_ANULACION_DEVOLUCION, 200, 220000L, REVERSABLE, 8,
                     INTENTAR_UNA_SOLA_VEZ, ESPERA_RESPUESTA );
        CARGA_TABLA( _OFF_LINE_ANULACION_DEVOLUCION, 220, 220000L, REVERSABLE, 3,
                     INTENTAR_VARIAS_VECES, ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_COBRO, 200, 800000L, REVERSABLE, 8, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _OFF_LINE_COBRO, 220, 800000L, REVERSABLE, 3, INTENTAR_VARIAS_VECES,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_ANULACION_COBRO, 200, 820000L, REVERSABLE, 8, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _OFF_LINE_ANULACION_COBRO, 220, 820000L, REVERSABLE, 3, INTENTAR_VARIAS_VECES,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_ANUL_COBRO, 200, 820000L, REVERSABLE, 8, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _OFF_LINE_ANUL_COBRO, 220, 820000L, REVERSABLE, 3, INTENTAR_VARIAS_VECES,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_CONSULTA_DEUDA, 200, 1100L, NO_REVERSABLE, 3, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_SALDO, 100, 310000L, NO_REVERSABLE, 3, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        CARGA_TABLA( _CIERRE_DE_LOTE, 500, 920000L, NO_REVERSABLE, 12, INTENTAR_UNA_SOLA_VEZ,
                     NO_ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_CONSULTA_CLIENTE, 100, 320000L, NO_REVERSABLE, 3,
                     INTENTAR_UNA_SOLA_VEZ, ESPERA_RESPUESTA );
        CARGA_TABLA( _ON_LINE_ACTUALIZA_WK, 200, 1200L, NO_REVERSABLE, 3, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
        //  NO OLVIDARSE DE PONER PRIORIDAD EN 0 EN VEZ DE 11 que
        //  los reversos no molesten
        //CARGA_TABLA(_REVERSO                      ,400,     0L,NO_REVERSABLE,14,INTENTAR_VARIAS_VECES);
        CARGA_TABLA( _REVERSO, 400, 0L, NO_REVERSABLE, 0, INTENTAR_VARIAS_VECES, ESPERA_RESPUESTA );
		//las operaciones de cashplus 
		CARGA_TABLA( _ON_LINE_CASHPLUS, 200, 90000L, REVERSABLE, 8, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
		CARGA_TABLA( _ON_LINE_ANULACION_CASHPLUS, 200, 140000L, REVERSABLE, 8, INTENTAR_UNA_SOLA_VEZ,
                     ESPERA_RESPUESTA );
		CARGA_TABLA( _ON_LINE_ECHO_TEST, 800, 990000L, NO_REVERSABLE, 12, INTENTAR_UNA_SOLA_VEZ,
                    NO_ESPERA_RESPUESTA );
    }
    else {
        ABORT_STRING( S_ERROR_OPERACIONES );
    }
}
/****************************************************************************/
void CARGA_TABLA( int operacion, int tipo_de_mensaje, long codigo_de_procesamiento, int reversable,
                  char prioridad, char un_solo_intento, char espera_rta )
/****************************************************************************/
{
    if( operacion < CANTIDAD_OPERACIONES ) {
        tabla_operaciones[operacion].tipo_de_mensaje = tipo_de_mensaje;
        tabla_operaciones[operacion].codigo_de_procesamiento = codigo_de_procesamiento;
        tabla_operaciones[operacion].reversable = reversable;
        tabla_operaciones[operacion].prioridad = prioridad;
        tabla_operaciones[operacion].un_solo_intento = un_solo_intento;
        tabla_operaciones[operacion].espera_rta = espera_rta;
        // prioridad: mientras mayor sea su valor menor va a ser la prioridad
        // ya que el indice es ascendente y se van a procesar primero aquellas
        // transacciones que tengan un valor mas bajo
    }
    else {
        ABORT_STRING( S_ERROR_OPERACIONES );
    }
}
/****************************************************************************/
void BUSCA_MENSAJE_EN_TABLA( int operacion, int *tipo_de_mensaje, long *codigo_de_procesamiento,
                             int tipo_cuenta )
/****************************************************************************/
{
    *tipo_de_mensaje = tabla_operaciones[operacion].tipo_de_mensaje;
    *codigo_de_procesamiento = tabla_operaciones[operacion].codigo_de_procesamiento;
    if( *codigo_de_procesamiento == COD_PROC_CONSULTA_SALDO ) {
        *codigo_de_procesamiento += tipo_cuenta * 1000;
    }
}
/****************************************************************************/
int BACKUP_TRANSAC2()
/****************************************************************************/
{
    //int handle;
    int ok = 0;
    GRABAR_LOG_SISTEMA_STRING( S_BACKUP_TRANSAC2, LOG_VENTAS,2 );
	BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(), T_TRAN_CRP, _TRAN_CRP_BAK );
    BACKUP_ARCHIVO_DE_LA_SEMANA_SQL( GET_FECHA_BACKUP(),T_TRANSAC2, _TRANSAC2_BAK );
	if(config_tps.NapseModalidad == 1) {
		char aux[10];
		char nombre_tabla_dest_tmp[80];
		memset(nombre_tabla_dest_tmp,0,80);
		CDAY( _FECHA_ARCHIVO, aux );
		strcat(nombre_tabla_dest_tmp,"C:..\\BAK\\respuestaNapse_");
		strcat(nombre_tabla_dest_tmp,aux);
		strcat(nombre_tabla_dest_tmp,".bak");
		BACKUP_ARCHIVO_DE_LA_SEMANA_SQL_NOMBRE( GET_FECHA_BACKUP(),T_RESPUESTA_NAPSE, nombre_tabla_dest_tmp );

	}
	
        ok = 1;
    return ok;
}
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES        
/****************************************************************************/
int BACKUP_TRAN_CRP( )
/****************************************************************************/
{
    //int handle;
    int ok = 0;

    GRABAR_LOG_SISTEMA( "BACKUP_TRAN_CRP" ,LOG_ERRORES,4);
    if( BACKUP_ARCHIVO_DE_LA_SEMANA( GET_FECHA_BACKUP(), _TRAN_CRP_SIC, _TRAN_CRP_BAK ) ) {
        ok = 1;
    };
        
    return ok;
}
#endif
int CHEQUEA_INTEGRIDAD()
/****************************************************************************/
{
    int error = 0;
	if(protocolo == NAPSE) {
		return error = 0;
	}
    if( MODELO_TEF == _TEF_SWITCH_NCR ) {
        error = 0;
    }
    else {
        if( strcmp( transac2->dt.nom_caja, tran_temp->nom_caja ) != 0 ) {
            error = ERROR_FALLA_INTEGRIDAD;
        }
        if( transac2->dt.id_terminal.numero != tran_temp->id_terminal.numero ) {
            error = ERROR_FALLA_INTEGRIDAD;
        }
        if( transac2->dt.nodo != tran_temp->nodo ) {
            error = ERROR_FALLA_INTEGRIDAD;
        }
        if( strcmp( transac2->dt.numero_cuenta_tarjeta, tran_temp->numero_cuenta_tarjeta ) != 0 ) {
            error = ERROR_FALLA_INTEGRIDAD;
        }
        if( transac2->dt.tipo_de_mensaje != tran_temp->tipo_de_mensaje ) {
            error = ERROR_FALLA_INTEGRIDAD;
        }
        if( transac2->dt.codigo_de_procesamiento != tran_temp->codigo_de_procesamiento ) {
            error = ERROR_FALLA_INTEGRIDAD;
        }
		if( transac2->dt.ticket != tran_temp->ticket && tran_temp->codigo_de_respuesta != 7 ) {//si es 7 se pide otro nro ticket
            error = ERROR_FALLA_INTEGRIDAD;
        }
    }
    if( error ) {
        GRABA_LOG_FALLA_INTEGRIDAD();
    }
    return error;
}
/****************************************************************************/
void GRABA_LOG_FALLA_INTEGRIDAD()
/****************************************************************************/
{
    GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_TRANSACCION , LOG_ERRORES,2);
    _GRABA_LOG_FALLA_INTEGRIDAD( "transaccion enviada", &transac2->dt );
    _GRABA_LOG_FALLA_INTEGRIDAD( "transaccion recibida", tran_temp );
}
/****************************************************************************/
void _GRABA_LOG_FALLA_INTEGRIDAD( char *men, struct _datos_transaccion *tran )
/****************************************************************************/
{
    char men_temp[200];
    GRABAR_LOG_SISTEMA( men ,LOG_VENTAS,1);
    sprintf( men_temp, "caja: %s", tran->nom_caja );
    GRABAR_LOG_SISTEMA( men_temp ,LOG_VENTAS,1);
    sprintf( men_temp, "nodo: %d", tran->nodo );
    GRABAR_LOG_SISTEMA( men_temp ,LOG_VENTAS,1);
    sprintf( men_temp, "tarjeta: %s", tran->numero_cuenta_tarjeta );
    GRABAR_LOG_SISTEMA( men_temp ,LOG_VENTAS,1);
    sprintf( men_temp, "importe: %8.2f", tran->importe );
    GRABAR_LOG_SISTEMA( men_temp ,LOG_VENTAS,1);
    sprintf( men_temp, "tipo mensaje: %i", tran->tipo_de_mensaje );
    GRABAR_LOG_SISTEMA( men_temp ,LOG_VENTAS,1);
    sprintf( men_temp, "codigo pocesamiento: %ld", tran->codigo_de_procesamiento );
    GRABAR_LOG_SISTEMA( men_temp ,LOG_VENTAS,1);
}
/****************************************************************************/
void COPIAR_RTA()
/****************************************************************************/
{
    memcpy( transac2->dt.pin_working_key, tran_temp->pin_working_key, 8 );
    memcpy( transac2->dt.retrieval_reference_number, tran_temp->retrieval_reference_number, 12 );
	//aca deberiamos tener 	tran_temp->bloque_encriptado.buffer el mensaje promocional
   /* if( strelen( tran_temp->bloque_encriptado.buffer) > 5 )
		memcpy( transac2->dt.mensaje, tran_temp->mensaje, 37 );
	else*/
    memcpy( transac2->dt.mensaje, tran_temp->mensaje, 37 );
    transac2->dt.codigo_de_respuesta = tran_temp->codigo_de_respuesta;
	if( tran_temp->codigo_de_respuesta == 7 ){
		transac2->dt.ticket_original = tran_temp->ticket;
	}
    transac2->dt.importe_cuota = tran_temp->importe_cuota;
    transac2->dt.saldo = tran_temp->saldo;
    transac2->dt.lote = tran_temp->lote;
    transac2->dt.autorizacion = tran_temp->autorizacion;
    memcpy( transac2->dt.autorizacion_alfa, tran_temp->autorizacion_alfa,
            sizeof( transac2->dt.autorizacion_alfa ) );
    if( config.pais == ECUADOR && transac2->dt.tipo_de_mensaje == 100 ) {
        memcpy( transac2->dt.numero_de_comercio, tran_temp->numero_de_comercio,
                sizeof( transac2->dt.numero_de_comercio ) );
        memcpy( transac2->dt.track_2, tran_temp->track_2, sizeof( transac2->dt.track_2 ) );
    }
    transac2->dt.fecha_contable = tran_temp->fecha_contable;
    transac2->dt.fecha_host = tran_temp->fecha_host;
    transac2->dt.hora_host = tran_temp->hora_host;
    memcpy( transac2->dt.nro_cuenta, tran_temp->nro_cuenta, sizeof( transac2->dt.nro_cuenta ) );
    transac2->dt.tasa_aplicada = tran_temp->tasa_aplicada;
	memcpy( transac2->dt.nro_cuenta2, tran_temp->nro_cuenta2,
		sizeof( transac2->dt.nro_cuenta2 ) );
//if(&tran_temp->bloque_encriptado.buffer[0]!= &transac2->dt.bloque_encriptado.buffer[0] )//||&tran_temp->bloque_encriptado.buffer[0]==',')
//		memcpy( transac2->dt.bloque_encriptado.buffer, tran_temp->bloque_encriptado.buffer, sizeof( transac2->dt.bloque_encriptado.buffer ) );
	
	if( transac2->operacion == _ON_LINE_COMPRA ) 
		transac2->dt.importe = tran_temp->importe;

	//aca guardemos el nro de lote por si en algun momento queda offline y tengamos el ultimo
	if( tran_temp->lote > 0 && tran_temp->nodo < 10 ) { //limitados hasta 10 nodos y menores de 10 solo para los offline
		//la posicion la da el nodo
		_SET_MEMORY_LONG( __pago_datafono, tran_temp->nodo, tran_temp->lote );
	}
	if(config_tps.NapseModalidad ==1)
		COPIAR_RTA_NAPSE();

}
/****************************************************************************/
void ABRIR_CANAL_TARJETAS()
/****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L) 
    int rta = 0;
    DRIVER( _DRV_tcp, _TCP_inicializacion, _CANAL_TARJETAS, &rta, sizeof( rta ) );
    #endif
    if( MODALIDAD_CANAL_TARJETAS == CANAL_PERMANENTE ) {
        ABRIR_CANAL_NETCOM2( TCP_IP, 2 );
    }
}
/****************************************************************************/
void NETCOM2_ARMAR_PAQUETE_REQ( struct _transac2 *transac2, char *buffer_out,
                                int *l_buffer_out )
/****************************************************************************/
{
    switch( MODELO_TEF ) {
        case _TEF_AUTON:

			if( transac2->dt.codigo_de_procesamiento == 20000 && transac2->dt.tipo_de_mensaje == TIPO_MENSAJE_REVERSO ) {
					char auxiliar[100];
					memset(auxiliar,0,sizeof(auxiliar));
					strncpy( auxiliar,GET_TRACK_2_PRIVADO(), 38);
					memcpy( transac2->dt.track_2, &auxiliar[1], sizeof( transac2->dt.track_2 ) - 1);
					transac2->dt.track_2[37] = '\0';
			}
            memcpy( buffer_out, &transac2->dt,
                    ( sizeof( struct _datos_transaccion ) - sizeof( transac2->dt.reserva1 ) ) );
            *l_buffer_out = ( sizeof( struct _datos_transaccion )
                            - sizeof( transac2->dt.reserva1 ) );
            //aca controlaremos los campos "problematicos" en los insert y update del transac2
			//FECHA_EXPIRACION 
				{
	
					int dd= strlen( transac2->dt.fecha_expiracion); 
					if( dd > 4 ) { //es mas grande la fecha que el espacio de 4, viene pegado el track2
						char mensa2[39];
			   			memset(mensa2,0, sizeof(mensa2));
						_snprintf(mensa2, sizeof( transac2->dt.track_2 ) ,"%s",GET_NUMERO_TARJETA_PRIVADO( ));
						strncpy( &transac2->dt.track_2[1], mensa2, sizeof( transac2->dt.track_2 ) - 1 );
						transac2->dt.track_2[0]='\0'; // pisaba el campo anterior fecha y se producia error, aca el trac_2 no es importante con esta configuracio
					}
				}
            if( !ENCRIPTAR_DATOS_TARJETA ) {
                DESENCRIPTA_UN_TRACK( 38, transac2->fecha_generacion, transac2->dt.track_2,
                                      transac2->dt.track_2 );
                DESENCRIPTA_UN_TRACK( 80, transac2->fecha_generacion, transac2->dt.track_1,
                                      transac2->dt.track_1 );
                DESENCRIPTA_UN_TRACK( 2, transac2->fecha_generacion, ( char* )&transac2->dt.cvv,
                                      ( char* )&transac2->dt.cvv );
            }
            break;
        case _TEF_SWITCH_NCR:
            SWITCH_NCR_ARMAR_PAQUETE_REQ( transac2, buffer_out, l_buffer_out );
            break;
    }
}
/****************************************************************************/
int NETCOM2_PROCESAR_PAQUETE_RTA( char *buffer_in, struct _datos_transaccion *tran_temp,
                                  struct _transac2 *transac2 )
/****************************************************************************/
{
    int error = 0;
	if(config_tps.NapseModalidad ==0 ) {
		switch( MODELO_TEF ) {
			case _TEF_AUTON:
				memcpy( tran_temp, buffer_in, sizeof( struct _datos_transaccion ) );
				break;
			case _TEF_SWITCH_NCR:
				memcpy( tran_temp, &transac2->dt, sizeof( struct _datos_transaccion ) );
				error = SWITCH_NCR_PROCESAR_PAQUETE_RTA( buffer_in, tran_temp, transac2 );
				break;
		}
		return( error );
	} else {
		SETEOS_VALORES_RESPUESTA_NAPSE(SI);
		if(tran_temp->codigo_de_respuesta !=0){
			error = 1;
			return error ;
		}
		COPIAR_RTA();
		GUARDAR_OPERACION_NAPSE_DAT();
		LEER_OPERACION_NAPSE();
		return error ;
	}
}
/****************************************************************************/
int NETCOM2_FORMATO_MENSAJE_TCP()
/****************************************************************************/
{
    int rta;
    switch( MODELO_TEF ) {
        case _TEF_AUTON:
            rta = _FORMATO_MENSAJE_INVEL;
            break;
        case _TEF_SWITCH_NCR:
            rta = _FORMATO_MENSAJE_SWITCH_NCR;
            break;
    }
    return( rta );
}
/****************************************************************************/
void SWITCH_NCR_ARMAR_PAQUETE_REQ( struct _transac2 *transac2, char *buffer_out,
                                   int *l_buffer_out )
/****************************************************************************/
{
    char tipo_paquete[3],fecha_hora[20],buffer[50],documento_cajero[10],plan;
    unsigned char byte;
    int tipo_paquete_int,i,fecha,fecha_ori,fecha_vto,cuotas;
    long hora, boleta = 0;
    strncpy( tipo_paquete, SWITCH_NCR_TIPO_PAQUETE( transac2 ), 3 );
    tipo_paquete_int = tipo_paquete[0] * 0x100 + tipo_paquete[1];
    fecha = _GET_FECHA_DOS();
    hora = GET_HORA_SEGUNDOS();
    hora = ( hora / 3600 ) * 10000L + ( ( hora % 3600 ) / 60 ) * 100L + ( hora % 60 );
    sprintf( fecha_hora, "%.02i%.02i%.02i%.06li", YEAR( fecha ) % 100, ( int )MON( fecha ),
             ( int )DAY( fecha ), hora );
    memset( documento_cajero, 0, sizeof( documento_cajero ) );
    strncpy( documento_cajero, datos_cajero.documento, 8 );
    TRIM( documento_cajero );
    if( documento_cajero[0] == 0 ) {
        sprintf( documento_cajero, "%.08li", NRO_LEGAJO_CAJERO );
    }
    //boleta = ( RAM_FACTURA || RAM_NOTA_CR ) ? RAM_NRO_COMP : NRO_TICKET + 1;
    boleta = transac2->dt.nro_comp_transaccion;
    boleta = boleta % 1000000L;
    sprintf( buffer_out, "%.04i%.03i%.02i%-8s%.04i%s%s%.06li", 0, config.sucursal, NRO_CAJA,
             documento_cajero, transac2->dt.ticket, fecha_hora, tipo_paquete, boleta );
    switch( tipo_paquete_int ) {
        case SWITCH_NCR_T1:
        case SWITCH_NCR_T2:
        case SWITCH_NCR_T3:
        case SWITCH_NCR_T4:
        case SWITCH_NCR_T5:
        case SWITCH_NCR_T8:
        case SWITCH_NCR_TR:
            SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 1, 10,
                                               ( long )ROUND( transac2->dt.importe * 100, 10, 0 ) );
            if( tipo_paquete_int == SWITCH_NCR_T4 || tipo_paquete_int == SWITCH_NCR_T5
             || tipo_paquete_int == SWITCH_NCR_TR ) {
                SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 2, 6,
                                                   transac2->dt.nro_comp_transaccion_original
                                                 % 1000000L );
            }
            else {
                SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 2, 6, boleta );
            }
            if( tipo_paquete_int == SWITCH_NCR_T1 || tipo_paquete_int == SWITCH_NCR_T3
             || tipo_paquete_int == SWITCH_NCR_T4 || tipo_paquete_int == SWITCH_NCR_TR
             || tipo_paquete_int == SWITCH_NCR_T5 ) {
                plan = ( transac2->dt.plan >= '0' && transac2->dt.plan <= '9' )
                     ? ( transac2->dt.plan - '0' ) : 0;
                cuotas = ( plan > 0 ) ? transac2->dt.cuotas : 0;
                SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 3, 3, plan * 100 + cuotas );
                //SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 23, 10, (long) ROUND( transac2->dt.importe_cuota * 100.0, 10, 0 ) );
                SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 23, 10,
                                                   ( long )ROUND( transac2->dt.importe_cuota, 10,
                                                                  0 ) * 100 );
                SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 27, 4,
                                                   ( long )ROUND( transac2->dt.tasa * 100.0, 4,
                                                                  0 ) );
                //SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 3, 3, plan  * 100 + transac2->dt.cuotas );
            }
            if( transac2->dt.modo_de_ingreso == 0 ) {
                SWITCH_NCR_VERIFICAR_CAMPO( transac2->dt.numero_cuenta_tarjeta, 20 );
                SWITCH_NCR_VERIFICAR_CAMPO( transac2->dt.fecha_expiracion, 4 );
                sprintf( buffer, "M%s=%4s?", transac2->dt.numero_cuenta_tarjeta,
                         transac2->dt.fecha_expiracion );
            }
            else {
                SWITCH_NCR_VERIFICAR_CAMPO( transac2->dt.track_2, 40 );
                for( i = 0 ;i < 40 ;i++ ) {
                    if( ( transac2->dt.track_2[i] < '0' || transac2->dt.track_2[i] > '9' )
                     && transac2->dt.track_2[i] != '=' ) {
                        transac2->dt.track_2[i] = 0;
                    }
                }
                sprintf( buffer, ";%s?", transac2->dt.track_2 );
            }
            SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 4, strlen( buffer ), buffer );
            if( tipo_paquete_int == SWITCH_NCR_T3 || tipo_paquete_int == SWITCH_NCR_T5 ) {
                //if( transac2->dt.autorizacion ) sprintf( buffer, "%.06li T", transac2->dt.autorizacion );
                //else sprintf( buffer, "%.02i%.02i%.02i L", transac2->dt.nro_boletin % 100,
                //  DAY(DLTOD(transac2->dt.fecha)), MON(DLTOD(transac2->dt.fecha) ) );
                //SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 6, 8, buffer );
                SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 6, 8, transac2->dt.autorizacion_alfa );
                fecha_ori = transac2->dt.fecha_host;
                sprintf( buffer, "%.02i%.02i%.02i%.06li", YEAR( fecha_ori ) % 100, MON( fecha_ori ),
                         DAY( fecha_ori ), transac2->dt.hora_host );
                SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 24, 12, buffer );
            }
            if( tipo_paquete_int == SWITCH_NCR_T2 || tipo_paquete_int == SWITCH_NCR_T8 ) {
                for( i = 0 ;i < 8 ;i++ ) {
                    byte = ( ( unsigned char )transac2->dt.pin_working_key[i] ) >> 4;
                    buffer[i * 2] = ( byte <= 9 ) ? byte + '0' : byte - 10 + 'A';
                    byte = transac2->dt.pin_working_key[i] & 0xf;
                    buffer[i * 2 + 1] = ( byte <= 9 ) ? byte + '0' : byte - 10 + 'A';
                }
                buffer[16] = 0;
                SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 8, 16, buffer );
                //SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 9, 2, TARJ[transac2->dt.codigo_marca].largo_pin );
                SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 9, 2, transac2->dt.l_pin );
                if( tipo_paquete_int == SWITCH_NCR_T8 ) {
                    SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 23, 10,
                                                       ( long )ROUND( transac2->dt.vuelto * 100, 10,
                                                                      0 ) );
                }
            }
            if( tipo_paquete_int == SWITCH_NCR_T4 || tipo_paquete_int == SWITCH_NCR_T5
             || tipo_paquete_int == SWITCH_NCR_TR ) {
                fecha_ori = DLTOD( transac2->dt.fecha_original );
                sprintf( buffer, "%.02i%.02i%.02i%.06li", YEAR( fecha_ori ) % 100, MON( fecha_ori ),
                         DAY( fecha_ori ), transac2->dt.hora_original );
                SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 11, 12, buffer );
                SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 12, 8,
                                                 transac2->dt.autorizacion_original );
            }
            break;
        case SWITCH_NCR_C1:
            SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 1, 10,
                                               ( long )ROUND( transac2->dt.importe * 100, 10, 0 ) );
            SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 2, 6, boleta );
            fecha_vto = GET_FECHA_VTO_PRIVADA(  ) ;
            if( !fecha_vto ) {
                fecha_vto = _GET_FECHA_DOS();
            }
            sprintf( buffer, "%.02i%.02i%.02i%.06li", YEAR( fecha_vto ) % 100, MON( fecha_vto ),
                     DAY( fecha_vto ), GET_HORA_COMPLETA_DOS() );
            SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 11, 12, buffer );
            SWITCH_NCR_OBTENER_CAMPO( 1, ';', buffer, 7, transac2->dt.track_2 );
            SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 13, 7, atol( buffer ) ); // Nro Cheque
            SWITCH_NCR_OBTENER_CAMPO( 2, ';', buffer, 7, transac2->dt.track_2 );
            SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 14, 7, atol( buffer ) ); // Banco Plaza
            SWITCH_NCR_OBTENER_CAMPO( 3, ';', buffer, 11, transac2->dt.track_2 );
            buffer[11] = 0;
            SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 15, 11, buffer );        // Numero Cuenta
            //SWITCH_NCR_OBTENER_CAMPO( 4, ';', buffer, 10, transac2->dt.track_2 );
            //SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( buffer_out, 16, 10, atol( buffer ) ); // RUT
            SWITCH_NCR_OBTENER_CAMPO( 4, ';', buffer, 9, transac2->dt.track_2 );
            SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 28, 9, buffer ); // RUT
            break;
    }
    sprintf( buffer, "%.04i", strlen( buffer_out ) );
    memcpy( buffer_out, buffer, 4 );    
    *l_buffer_out = strlen( buffer_out );
}
/****************************************************************************/
void SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( char *buffer_out, int id_campo, int len, long dato )
/****************************************************************************/
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    char aux[40];
    sprintf( aux, "%.0*li", len, dato );
    if( strlen( aux ) > ( unsigned )len ) {
        movmem( &aux[strlen( aux ) - len], aux, len );
    }
    sprintf( &buffer_out[strlen( buffer_out )], "%.02i%.03i%s", id_campo, len, aux );
    #endif
}
/****************************************************************************/
void SWITCH_NCR_AGREGAR_CAMPO_CADENA( char *buffer_out, int id_campo, int len, char *dato )
/****************************************************************************/
{
    int offset;
    offset = ( strlen( dato ) > ( unsigned )len ) ? strlen( dato ) - len : 0;
    sprintf( &buffer_out[strlen( buffer_out )], "%.02i%.03i%*s", id_campo, len, len,
             &dato[offset] );
}
/****************************************************************************/
char * SWITCH_NCR_TIPO_PAQUETE( struct _transac2 *transac2 )
/****************************************************************************/
{
    static char t_paquete[3];
    switch( transac2->dt.nodo ) {
        case 1:
            // TransBank
            switch( transac2->operacion ) {
                case _ON_LINE_COMPRA:
                    if( transac2->dt.tipo_de_tarjeta == 0 ) {
                        strcpy( t_paquete, "T1" );
                    }
                    else {
                        if( transac2->dt.vuelto > 0.01 ) {
                            strcpy( t_paquete, "T8" );
                        }
                        else {
                            strcpy( t_paquete, "T2" );
                        }
                    }
                    break;
                case _OFF_LINE_COMPRA:
                    strcpy( t_paquete, "T3" );
                    break;
                case _ON_LINE_ANULACION:
                case _OFF_LINE_ANULACION:
                    strcpy( t_paquete, "TR" );
                    break;
                case _ON_LINE_DEVOLUCION:
                    strcpy( t_paquete, "T4" );
                    break;
                case _OFF_LINE_DEVOLUCION:
                    strcpy( t_paquete, "T5" );
                    break;
                case _ON_LINE_ACTUALIZA_WK:
                    strcpy( t_paquete, "T7" );
                    break;
                default:
                    strcpy( t_paquete, "XX" );
                    break;
            }
            break;
        case 2:
            // Consulta de Cheques
            if( transac2->operacion == _ON_LINE_COMPRA ) {
                strcpy( t_paquete, "C1" );
            }
            break;
        default:
            strcpy( t_paquete, "XX" );
            break;
    }
    return( t_paquete );
}
/****************************************************************************/
int SWITCH_NCR_TIPO_PAQUETE_INT( struct _transac2 *transac2 )
/****************************************************************************/
{
    char tipo_paquete[3];
    int tipo_paquete_int;
    strncpy( tipo_paquete, SWITCH_NCR_TIPO_PAQUETE( transac2 ), 3 );
    tipo_paquete_int = tipo_paquete[0] * 0x100 + tipo_paquete[1];
    return( tipo_paquete_int );
}
/****************************************************************************/
int SWITCH_NCR_PROCESAR_PAQUETE_RTA( char *buffer_in, struct _datos_transaccion *tran_temp,
                                     struct _transac2 *transac2 )
/****************************************************************************/
{
    char buffer[40];
    int l_rta, offset, id_campo, l_campo, byte_l, byte_h, dia, mes, anio, l, i,
    rta, error = 0;
    memset( tran_temp->pin_working_key, 0, sizeof( tran_temp->pin_working_key ) );
    l_rta = SWITCH_NCR_OBTENER_ENTERO( buffer_in, 0, 4 );
    if( l_rta >= 41 ) {
        tran_temp->id_terminal.numero = SWITCH_NCR_OBTENER_ENTERO( buffer_in, 7, 2 );
        tran_temp->ticket = ( short )SWITCH_NCR_OBTENER_ENTERO( buffer_in, 17, 4 );
        offset = 41;
        while( offset < l_rta ) {
            id_campo = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset, 2 );
            l_campo = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 2, 3 );
            offset += 5;
            switch( id_campo ) {
                case 5:
                    // Respuesta
                    rta = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset, l_campo );
                    tran_temp->codigo_de_respuesta = SWITCH_NCR_TRANSFORMAR_RTA( rta );
                    if( tran_temp->codigo_de_respuesta < 0 ) {
                        //error = 1;
                        //tran_temp->codigo_de_respuesta = 1;
                        tran_temp->codigo_de_respuesta = 2; // Denegada Pedir Autorizacion.
                    }
                    //--- Si se aprueba registra un RRN ficticio
                    if( tran_temp->codigo_de_respuesta == 0 ) {
                        tran_temp->retrieval_reference_number[0] = '1';
                    }
                    break;
                case 6:
                    // Codigo de Autorizacion.
                    l = sizeof( tran_temp->autorizacion_alfa );
                    memset( tran_temp->autorizacion_alfa, 0, l );
                    strncpy( tran_temp->autorizacion_alfa, &buffer_in[offset],
                             ( l_campo < l ) ? l_campo : l );
                    tran_temp->autorizacion = 0L;
                    //tran_temp->autorizacion = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset, l_campo );
                    break;
                case 29:
                    // Tasa aplicada
                    rta = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset, l_campo );
                    tran_temp->tasa_aplicada = ( float )rta / 100;
                    break;
                case 47:
                    // Mensaje.
                    l = sizeof( tran_temp->mensaje );
                    memset( tran_temp->mensaje, 0, l );
                    strncpy( tran_temp->mensaje, &buffer_in[offset],
                             ( l_campo < l ) ? l_campo : l );
                    break;
                case 48:
                    // Working Key
                    memcpy( buffer, &buffer_in[offset], 16 );
                    for( i = 0 ;i < 8 ;i++ ) {
                        byte_h = ( buffer[i * 2] <= '9' ) ? buffer[i * 2] - '0'
                               : buffer[i * 2] - 'A' + 10;
                        byte_l = ( buffer[i * 2 + 1] <= '9' ) ? buffer[i * 2 + 1] - '0'
                               : buffer[i * 2 + 1] - 'A' + 10;
                        tran_temp->pin_working_key[i] = ( byte_h << 4 ) + byte_l;
                    }
                    break;
                case 49:
                    // Fecha Contable.
                    anio = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset, 2 );
                    mes = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 2, 2 );
                    dia = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 4, 2 );
                    tran_temp->fecha_contable = FECHA( ( char )dia, ( char )mes, ( char )anio );
                    break;
                case 50:
                    // Nro de Cuenta.
                    l = sizeof( tran_temp->nro_cuenta );
                    memset( tran_temp->nro_cuenta, 0, l );
                    //strncpy( tran_temp->nro_cuenta, buffer_in, (l_campo < l)? l_campo : l );
                    strncpy( tran_temp->nro_cuenta, &buffer_in[offset],
                             ( l_campo < l ) ? l_campo : l );                    
                    break;
                case 51:
                    // Fecha y Hora Transbank
                    anio = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset, 2 );
                    mes = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 2, 2 );
                    dia = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 4, 2 );
                    tran_temp->fecha_host = FECHA( ( char )dia, ( char )mes, ( char )anio );
                    tran_temp->hora_host = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 6, 2 )
                                         * 10000L;
                    tran_temp->hora_host += SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 8, 2 )
                                          * 100L;
                    tran_temp->hora_host += SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset + 10, 2 );
                    //transac2->ultima_fecha_transmision = FECHA( dia, mes, anio );
                    //transac2->ultima_hora_transmision  = SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset+6, 2 ) * 10000L;
                    //transac2->ultima_hora_transmision += SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset+8, 2 ) * 100L;
                    //transac2->ultima_hora_transmision += SWITCH_NCR_OBTENER_ENTERO( buffer_in, offset+10, 2 );
                    break;
            }
            offset += l_campo;
        }
    }
    return( error );
}
/****************************************************************************/
long SWITCH_NCR_OBTENER_ENTERO( char *buffer_in, int offset, int len )
/****************************************************************************/
{
    char buffer[20];
    long rta;
    memcpy( buffer, &buffer_in[offset], len );
    buffer[len] = 0;
    rta = atol( buffer );
    return( rta );
}
/****************************************************************************/
void SWITCH_NCR_VERIFICAR_CAMPO( char *cadena, int l )
/****************************************************************************/
{
    int i, salir = 0;
    for( i = 0 ;i < l && !salir ;i++ ) {
        if( ( cadena[i] < '0' || cadena[i] > '9' ) && cadena[i] != '=' ) {
            cadena[i] = 0;
            salir = 1;
        }
    }
    if( !salir ) {
        cadena[i] = 0;
    }
}
/****************************************************************************/
int NETCOM2_ENVIAR_CONFIRMACION( struct _transac2 *transac2,
                                 struct _datos_transaccion *tran_temp )
/****************************************************************************/
{
    int rta = 0, tipo_paquete_int;
    if( MODELO_TEF == _TEF_SWITCH_NCR ) {
        tipo_paquete_int = SWITCH_NCR_TIPO_PAQUETE_INT( transac2 );
        switch( tipo_paquete_int ) {
            case SWITCH_NCR_T1:
            case SWITCH_NCR_T2:
                //case SWITCH_NCR_T3:
            case SWITCH_NCR_T4:
            case SWITCH_NCR_TR:
                //case SWITCH_NCR_T5:
            case SWITCH_NCR_T8:
                if( tran_temp->codigo_de_respuesta == 0 ) {
                    rta = 1;
                }
                break;
        }
    }
    return( rta );
}
/****************************************************************************/
void NETCOM2_ARMAR_PAQUETE_CONFIRMACION( struct _transac2 *transac2, char *buffer_out,
                                         int *l_buffer_out, struct _datos_transaccion *tran_temp )
/****************************************************************************/
{
    char buffer[10];
    if( MODELO_TEF == _TEF_SWITCH_NCR ) {
        buffer_out[33] = 'T';
        buffer_out[34] = 'C';
        SWITCH_NCR_AGREGAR_CAMPO_CADENA( buffer_out, 6, 8, tran_temp->autorizacion_alfa );
        sprintf( buffer, "%.04i", strlen( buffer_out ) );
        memcpy( buffer_out, buffer, 4 );
        *l_buffer_out = strlen( buffer_out );
    }
}

/****************************************************************************/
int SWITCH_NCR_CARGAR_RESPUESTAS()
/****************************************************************************/
{
    int rta = 0, h = 0;
//(Nota Unific MySQL-Linux): comentado en la version linux. No se usa?
    struct _rta_switch_ncr __rta_switch_ncr;

    /*if( USE_DB( AREA_TRANSAC2, _RTA_SWITCH_BTR, ( char* )&__rta_switch_ncr,
                sizeof( struct _rta_switch_ncr ), NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_RTA_SWITCH_BTR, TT_ORIG,( char* )&__rta_switch_ncr, sizeof( struct _rta_switch_ncr ))== 0 ) {
        cant_rta_switch_ncr = RECCOUNT2( NULL, NULL );/*RECCOUNT()*/
        rta_switch_ncr = MALLOC( cant_rta_switch_ncr * sizeof( struct _rta_switch_ncr ) );
        if( rta_switch_ncr ) {
            while( /*!BtrvEOF()*/ !dbEOF() ) {
                memcpy( &rta_switch_ncr[h++], &__rta_switch_ncr, sizeof( struct _rta_switch_ncr ) );
                //SKIP( 1 );
				SKIP2( 1 );
            }
            rta = 1;
        }
        //CLOSE_DATABASE( AREA_TRANSAC2 );
		CLOSE_TABLE( T_RTA_SWITCH_BTR, TT_ORIG );
    }

    return( rta );
}

/****************************************************************************/
int SWITCH_NCR_TRANSFORMAR_RTA( int rta_switch )
/****************************************************************************/
{
    int rta_pos = -1, i, salir = 0 ;
    for( i = 0 ;i < cant_rta_switch_ncr && !salir ;i++ ) {
        if( rta_switch_ncr[i].rta_switch == rta_switch ) {
            rta_pos = rta_switch_ncr[i].rta_pos;
            salir = 1;
        }
    }
    return( rta_pos );
}
/****************************************************************************/
int SWITCH_NCR_OBTENER_CAMPO( int orden, char separador, char *buffer_out, int l_buffer_out,
                              char *buffer_in )
/****************************************************************************/
{
    char *p1,*p2,*p;
    int rta = 0, i;
    p = p1 = p2 = buffer_in;
    for( i = 0 ;i < orden - 1 && p2 ;i++ ) {
        p2 = strchr( p1, separador );
        if( p2 ) {
            p1 = &p2[1];
            p = p1;
        }
    }
    if( p2 ) {
        rta = 1;
        memset( buffer_out, 0, l_buffer_out + 1 );
        strncpy( buffer_out, p, l_buffer_out );
        p2 = strchr( buffer_out, separador );
        if( p2 ) {
            p2[0] = 0;
        }
    }
    return( rta );
}
/****************************************************************************/
void NETCOM2_FORMATEAR_AUTORIZACION( char *cod_autorizacion, int modo )
/****************************************************************************/
{
    char aux[10];
    int fecha;
    if( MODELO_TEF == _TEF_SWITCH_NCR ) {
        if( modo == 0 ) {
            // Autorizacion ingresada a mano
            //sprintf( aux, "%.06li T", atol( cod_autorizacion ) );
            memset( aux, 0, sizeof( aux ) );
            strncpy( aux, cod_autorizacion, 6 );
            strcat( aux, " T" );
        }
        else {
            fecha = _GET_FECHA_DOS();
            sprintf( aux, "%.02i%.02i%.02i L", VAR_TARJETAS_NRO_BOLETIN % 100, DAY( fecha ),
                     MON( fecha ) );
        }
        strcpy( cod_autorizacion, aux );
    }
}
/****************************************************************************/
int TEF_HABILITADO( void )
/****************************************************************************/
{
    int rta = 1;
    #ifdef COMPILAR_ENCRIPTACION_TARJETAS
    if( ENCRIPTAR_DATOS_TARJETA && !clave_inicializada ) {
        if( !NETCOM2_CARGAR_CLAVES() ) {
            rta = 0;
            MENSAJE( "S_ERROR_AL_CARGAR_CLAVES_PARA_TARJETA" );
        }
    }
    #endif
    return( rta );
}
/****************************************************************************/
void ENCRIPTA_UN_TRACK( int lo, unsigned fecha, char *track_normal, char *track_enciptado )
/****************************************************************************/
{
    #ifdef COMPILAR_ENCRIPTACION_TARJETAS
    CRIPTER_DE_UN_TRACK( lo, RC5key, fecha, track_normal, track_enciptado );
    #endif
}
/****************************************************************************/
void DESENCRIPTA_UN_TRACK( int lo, unsigned fecha, char *track_enciptado, char *track_normal )
/****************************************************************************/
{
    #ifdef COMPILAR_ENCRIPTACION_TARJETAS
    CRIPTER_DE_UN_TRACK( lo, RC5key, fecha, track_enciptado, track_normal );
    #endif
}
/****************************************************************************/
void CRIPTER_DE_UN_TRACK( int lo, char *charKey1, unsigned key2, char *charCipherText,
                          char *charNewText )
/****************************************************************************/
{
    #ifdef COMPILAR_ENCRIPTACION_TARJETAS
    int iCipherTextLength = 0;
    int a = 0;
    int i;
    int Pos_Key2;
    Pos_Key2 = key2 % CANTIDAD_DE_VALORES_DE_CLAVE;
    // iCipherTextLength = strlen(&charCipherText[0]);
    iCipherTextLength = lo;
    //charNewText[0] = NULL;
    for( i = 0;i < iCipherTextLength;i++ ) {
        charNewText[i] = charCipherText[i] ^ charKey1[a] ^ charKey1[Pos_Key2];
        //charNewText[i + 1] = NULL;
        a++;
        if( a >= CANTIDAD_DE_VALORES_DE_CLAVE ) {
            a = 0;
        }
    }
    #endif
}
/****************************************************************************/
void T_ENCRIPTAR_DATOS_TARJETA( void )
/****************************************************************************/
{
    #ifdef COMPILAR_ENCRIPTACION_TARJETAS
    transac2->dt.datos_tarjetas_encriptados = 1;
	ACTIVAR_PUBLIC_KEY( );
    switch( clave_tef->id_algoritmo ) {
        case ALGORITMO_RSA:
            T_ENCRIPTAR_DATOS_TARJETA_RSA();
            break;
    }
	DESACTIVAR_PUBLIC_KEY( );
    #endif
}
/****************************************************************************/
int NETCOM2_CARGAR_CLAVES( void )
/****************************************************************************/
{int rta = 1;
#ifdef COMPILAR_ENCRIPTACION
/*	el llamado para inicializar las dos claves al mismo tiempo falla, por separado y 
en las funciones de encriptacion y desencriptacion funcionan ok
	rta = InitDesencriptacion( );
		*/
#endif    
	return rta;
}

/****************************************************************************/
void T_ENCRIPTAR_DATOS_TARJETA_RSA( void )
/****************************************************************************/
#define DEBUG_TRANSITORIO_DATOS_SENSIBLES
{int Iauz=0;
    #ifdef COMPILAR_ENCRIPTACION_TARJETAS
    struct _bloque_a_encriptar bloque_in;
	char auxiliar[1024];
	int longAuxiliar1=0,longAuxiliar2=0;

	memset(&bloque_in, 0, sizeof(struct _bloque_a_encriptar));
	memcpy( bloque_in.track_1, transac2->dt.track_1, sizeof( bloque_in.track_1 ) );
    memcpy( bloque_in.track_2, transac2->dt.track_2, sizeof( bloque_in.track_2 ) );
	
    memset( bloque_in.fecha_expiracion, 0, 4/*sizeof( bloque_in.fecha_expiracion )*/ );
	{
    char mensa[5];
	memset(mensa,0, sizeof(mensa));
	_snprintf(mensa,4,"%s",transac2->dt.fecha_expiracion);
	mensa[4]='\0';
	memcpy( bloque_in.fecha_expiracion, mensa, 4/*sizeof( bloque_in.fecha_expiracion )*/ );
//	memcpy( bloque_in.fecha_expiracion, transac2->dt.fecha_expiracion, 4/*sizeof( bloque_in.fecha_expiracion )*/ );

	}
    bloque_in.cvv = transac2->dt.cvv;
    #ifdef DEBUG_TRANSITORIO_DATOS_SENSIBLES
    memset( transac2->dt.track_1, 0, sizeof( transac2->dt.track_1 ) );
    memset( transac2->dt.track_2, 0, sizeof( transac2->dt.track_2 ) );
    memset( transac2->dt.fecha_expiracion, 0, sizeof( transac2->dt.fecha_expiracion ) );
    transac2->dt.cvv = 0;
    #endif
    transac2->dt.bloque_encriptado.len = sizeof( transac2->dt.bloque_encriptado.buffer );
	
//	RSA_ENCRIPTAR( (char*)&bloque_in, sizeof( bloque_in ), (char*)transac2->dt.bloque_encriptado.buffer,&transac2->dt.bloque_encriptado.len );
	if( RSA_ENCRIPTAR( (char*)&bloque_in, sizeof( bloque_in ), (char*)auxiliar, &longAuxiliar1) &&
		 CONVERSION_ASCII_EXT_A_TO_HEXA( 0,auxiliar,&longAuxiliar1, &transac2->dt.bloque_encriptado.buffer[0],&longAuxiliar2) ){
	
	
	transac2->dt.bloque_encriptado.len = longAuxiliar2;
	transac2->dt.bloque_encriptado.id_clave = clave_tef->id_clave;
    transac2->dt.bloque_encriptado.version_bloque = _VERSION_BLOQUE_A_ENCRIPTAR;
	

	}
    #endif
}
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES
/****************************************************************************/
int ACTIVAR_PUBLIC_KEY( void ){
/****************************************************************************/
int handle, clave_inicializada = 0 ;
   if( clave_tef == NULL ) {
         clave_tef = MALLOC( sizeof( struct _clave_tef ) );
      }
    /*----------- Lee el archivo -------------*/

   handle = OPEN( _KEY_POS_DAT, O_RDONLY );
	if( handle > 0 ) {
   		if( _READ( handle, ( char * ) clave_tef, sizeof( struct _clave_tef ),
   			__LINE__, __FILE__ ) == 0 ) {
			if( RSA_INIT( ) ) {
               clave_inicializada = 1;
               glog("ACT.pbl.k",LOG_DEBUG,4);
            }
   	   }
   	}
   	_CLOSE( handle, __LINE__, __FILE__ );


   return clave_inicializada;
}
/****************************************************************************/
int InitEncryDesencriptacion( void ){
/****************************************************************************/
int rta = 0;
   
   if( ACTIVAR_PUBLIC_KEY() ){
		rta = ACTIVAR_PRIVATE_KEY( ) ;
   }
   if(!rta){
	   DESACTIVAR_PRIVATE_KEY( );
	   DESACTIVAR_PUBLIC_KEY( );
   }
   return rta;
}

/****************************************************************************/
int PROCESA_DATOS_ENCRIPT_TRANSACCION( int encripta , long nro_transaccion,struct _datos_transaccion * tran )
/****************************************************************************/
{

   int       rta = 0,/*area_disponible = 0 ,*/ln_in = 0,ln_out = 0/*,area_usada =0*/, tabla_ant, tipo_ant;
   char encrip[100];
   struct _tran_datos_encrip datos_encriptados_reg,datos_encriptados;
   struct _datos_desencriptados datos_desencriptados;


// levanta las llaves de encriptacion y hace una prueba con caracteres.si es ok, sigo
	if( InitEncryDesencriptacion( ) &&	PROCESA_CONTROL_DATOS_ENCRIPTADOS( 0 )) {
		rta = 1;
	}

   //area_usada = SELECTED();
   //area_disponible = AREA_DISPONIBLE( );
   tabla_ant = SELECTED2();
   tipo_ant = SELECTED_TIPO();
   /*if( MODO_DEBUG == 75 || MODO_DEBUG == 80 )*/{
	   memset(encrip,0,sizeof(encrip));
	   _snprintf(encrip, sizeof(encrip),"#PROC_ENCRIPT %i - %li"/*[A.u%3i][A.d%3i]"*/, encripta, nro_transaccion/*, area_usada, area_disponible*/);
		encrip[sizeof( encrip)-1]=0;
	   GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,4);
      if( MODO_DEBUG == 80 )
         MENSAJE(encrip);
	}
   
									
   //if( rta && USE_DB(area_disponible  , _TRAN_CRP_SIC, &datos_encriptados_reg,sizeof( struct _tran_datos_encrip ), NULL, 0, 0 )== 0 ){
   //COMENTO HASTA QUE SE ARREGLE	
   if( rta && OPEN_TABLE( T_TRAN_CRP, TT_ORIG, &datos_encriptados_reg, 
							sizeof( struct _tran_datos_encrip ) ) == 0 ) {
	int longString = 0, longHexa = 0;
	/********************************* cereo las estructuras  **********************/
      memset( &datos_desencriptados, 0, sizeof( struct _datos_desencriptados ));

      switch(encripta){
      case NO:// 0 - desencripta
      {

/*****************  BUSCO LOS DATOS ENCRIPTADOS DE LA OPERACION  **************/
         //SELECT( area_disponible );
         //SET_ORDER( 1 );
         //GET_EQUAL( ( char * ) &nro_transaccion );
		 SELECT_TABLE( T_TRAN_CRP, TT_ORIG );
		 SET_WHERE( "ID_TRANSACCION = '%ld'", nro_transaccion );
		 RUN_QUERY(NO);

         if( FOUND2(  ) && datos_encriptados_reg.enviado != SI ) {

            ln_in = datos_encriptados_reg.datos.len;

			//del bloque que tiene los caracteres de hexa..al que puede tener nulos
			longHexa = ln_in*2;
			CONVERSION_ASCII_EXT_A_TO_HEXA( 1,datos_encriptados.datos.buffer,&longString, datos_encriptados_reg.datos.buffer,&longHexa);


			ACTIVAR_PRIVATE_KEY();
/********************************* DESENCRIPTA  *******************************/
            if( RSA_DESENCRIPTAR( (char*)datos_encriptados.datos.buffer/*datos_encriptados_reg.datos.buffer*/, ln_in ,
                                           (char*)&datos_desencriptados , &ln_out  )){
/************************** DATOS DESENCRIPTADOS A LA DATOS TRANSACCION ********/
               strncpy( tran->numero_de_comercio,datos_desencriptados.numero_de_comercio,sizeof(tran->numero_de_comercio));
               tran->lote = datos_desencriptados.lote;
               strncpy( tran->numero_cuenta_tarjeta,datos_desencriptados.numero_cuenta_tarjeta,sizeof(tran->numero_cuenta_tarjeta));
               tran->autorizacion = datos_desencriptados.autorizacion;
               tran->numero_de_trace = datos_desencriptados.numero_de_trace;
               strncpy( tran->retrieval_reference_number,datos_desencriptados.retrieval_reference_number,sizeof(tran->retrieval_reference_number));
               tran->plan = datos_desencriptados.plan;
               tran->cuotas =datos_desencriptados.cuotas;
			   if( tran->modo_de_ingreso != 1  ) {
				   strncpy( tran->track_2, tran->numero_cuenta_tarjeta, sizeof( tran->numero_cuenta_tarjeta ) );
			   }
               if( UPDATE2() == 0){//solo si esta completa la operacion
                  //memcpy( &datos_encriptados_reg.reserva,"DESCRIPTADO", 11 );
                  rta = 3;
               }
               //if( MODO_DEBUG == 75 || MODO_DEBUG == 80 )
               {
                  char encrip[100];
                  _snprintf(encrip, sizeof(encrip),"#DESENCRIPTA %s - rta=[%2i]",&datos_desencriptados.numero_cuenta_tarjeta[5], rta );
				 		encrip[sizeof( encrip)-1]=0;
                  
                  GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,4);
                  if( MODO_DEBUG == 80 )
                     MENSAJE(encrip);
               }
            }else{
               GRABAR_LOG_SISTEMA("S_ERROR_EN_DATOS" ,LOG_DEBUG,2);
            }
         }else{
            GRABAR_LOG_SISTEMA("S_ERROR_AL_BUSCAR_REFERENCIAS_DE_TARJETAS ",LOG_DEBUG,2);
         }
      }
      break;
      case SI://1 - encripta
      {

            /******************* COPIO LOS DATOS A ENCRIPTAR ********************/
            {
               strncpy( datos_desencriptados.numero_de_comercio, tran->numero_de_comercio, sizeof( datos_desencriptados.numero_de_comercio ));
               datos_desencriptados.lote = tran->lote ;
               strncpy( datos_desencriptados.numero_cuenta_tarjeta, tran->numero_cuenta_tarjeta, sizeof( datos_desencriptados.numero_cuenta_tarjeta ));
               datos_desencriptados.autorizacion = tran->autorizacion;
               datos_desencriptados.numero_de_trace = tran->numero_de_trace ;
               strncpy( datos_desencriptados.retrieval_reference_number, tran->retrieval_reference_number , sizeof( datos_desencriptados.retrieval_reference_number ));
               datos_desencriptados.plan = tran->plan ;
               datos_desencriptados.cuotas = tran->cuotas ;
            }

            ln_in = sizeof( datos_desencriptados );
            /******************* ENCRIPTAS DATOS ******************************/
			ACTIVAR_PUBLIC_KEY();
            if( RSA_ENCRIPTAR((char*)&datos_desencriptados, ln_in,
                                  (char*) datos_encriptados.datos.buffer, &ln_out  )){
               
			

			   SET_WHERE( "ID_TRANSACCION = '%ld'", nro_transaccion );
			   RUN_QUERY(NO);
               if( FOUND2(  ) && datos_encriptados_reg.enviado != SI ){
                  rta = 2;
               }

               //memset( &datos_encriptados_reg.reserva, 0, sizeof(datos_encriptados.reserva) );

			   //datos_encriptados_reg.datos.buffer[ln_out]=0;
			   
               datos_encriptados_reg.datos.len = ln_out;//

			   //memcpy( &datos_encriptados_reg.reserva,"ENCRIPTADOS", 11 );
               
			   datos_encriptados_reg.enviado = NO;
               datos_encriptados_reg.datos.id_clave = 0;
               datos_encriptados_reg.datos.version_bloque = 0;
               datos_encriptados_reg.datos.len = ln_out;
			   
			   
				
			   
				//del bloque que puede contener nulos al que tiene los caracteres de hexa..
			   longString = ln_out;
			   CONVERSION_ASCII_EXT_A_TO_HEXA( 0,datos_encriptados.datos.buffer,&longString, datos_encriptados_reg.datos.buffer,&longHexa);
               //memcpy( &datos_encriptados_reg.datos.buffer,datos_encriptados.datos.buffer, ln_out );
				   
				   
				   if( rta == 2 && UPDATE2() == 0 ){
                  rta = 2;
               }else{
                  datos_encriptados_reg.id_transaccion = nro_transaccion;
                  if( INSERT2(NO) == 0 )
                     rta = 2;
               }
               //if( MODO_DEBUG == 75 || MODO_DEBUG == 80 )
               {
                  char encrip[100];
                  _snprintf(encrip, sizeof(encrip),"#ENCRIPTA rta=[%2i]- %5s", rta, &tran->numero_cuenta_tarjeta[5] );
                  encrip[sizeof( encrip)-1]=0;
                  GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,4);
                  if( MODO_DEBUG == 80 )
                     MENSAJE(encrip);
               }
			   
/******************* BORRO LOS DATOS ENCRIPTADOS  ******************************/
               if( rta == 2 ){
                  OCULTA_DATOS_ENCRIPTADOS( tran); //aca me parece que no deberia borrar el track2
                  rta = 3;

               }
			   
				
            }else{
               GRABAR_LOG_SISTEMA("S_ERROR_EN_DATOS",LOG_DEBUG,2);
            }
         }
      break;
      }

      if( rta != 3 )
         rta = 0;

      //if( area_disponible )
      //   CLOSE_DATABASE( area_disponible );
	  CLOSE_TABLE( T_TRAN_CRP, TT_ORIG );
   }else
      GRABAR_LOG_SISTEMA("S_ERROR_AL_ABRIR_LA_TABLA " ,LOG_ERRORES,2);

   //if( area_usada )
      //SELECT(area_usada);
	SELECT_TABLE( tabla_ant, tipo_ant );	

	DESACTIVAR_PRIVATE_KEY();
	DESACTIVAR_PUBLIC_KEY();


  /*if( MODO_DEBUG == 75 || MODO_DEBUG == 80 )*/{
  	
      char encrip[100];
      _snprintf(encrip, sizeof(encrip),"#PROC_ENCRIPT.%i-%li-Rta=%2i]"/*[A.u%3i-A.d%3i]"*/, encripta,
         nro_transaccion, rta /*, area_usada, area_disponible*/);
      encrip[sizeof( encrip)-1]=0;
      GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,4);
      if( MODO_DEBUG == 80 )
         MENSAJE(encrip);
   }

   return rta;
}
/****************************************************************************/
int PROCESA_CONTROL_DATOS_ENCRIPTADOS( int encripta  )
//utilizado para pruebas de encriptacion y desencriptacion
/****************************************************************************/
{

   int       rta = 0,ln_in = 0,ln_out = 0;
   char entrada[60], salida[150],extra[60];

//   rta = InitEncryDesencriptacion( );
   

/********************************* cereo las estructuras  **********************/
      
	   {	
			memset(salida,0,sizeof( salida ) );
			memset(entrada,0,sizeof( entrada ) );
			memset(extra,0,sizeof( extra ) );
			_snprintf( entrada, sizeof( entrada ),"abcdefghijklmnñopqrstuvwxyz=ABCDEFGHIJKLMNÑOPQRSTUVWXYZ");	
			_snprintf( extra, sizeof( extra ),"abcdefghijklmnñopqrstuvwxyz=ABCDEFGHIJKLMNÑOPQRSTUVWXYZ");	
            ln_in = sizeof( entrada );

            /******************* COPIO LOS DATOS A ENCRIPTAR ********************/
            
            ln_in = sizeof( entrada );
            /******************* ENCRIPTAS DATOS ******************************/
       //     ACTIVAR_PUBLIC_KEY( );
			
            if(RSA_ENCRIPTAR(&entrada, ln_in, &salida, &ln_out  )){

			memset(entrada,0,sizeof(entrada));
			
			//int RSA_DESENCRIPTAR( char  *bloque_in, int len_in , char *bloque_out, int *len_out )
		//	ACTIVAR_PRIVATE_KEY();
			   RSA_DESENCRIPTAR( &salida, &ln_out, &entrada , &ln_in  );
			   if(strcmp(entrada,extra) == 0 )
				   rta = 1;

               
			}

	//DESACTIVAR_PUBLIC_KEY();
  
		}

  
   return rta;
}

/****************************************************************************/
int MARCAR_ENVIADOS_DATOS_ENCRIPT( long nro_transaccion )
/****************************************************************************/
{

   int       rta = 0 /*,area_disponible = 0, area_usada =0*/, tabla_ant, tipo_ant ;

   //area_usada = SELECTED();
   //area_disponible = AREA_DISPONIBLE( );
   tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
   

   //if( USE_DB( area_disponible, _TRAN_CRP_SIC, &datos_encriptados,
   //      sizeof( struct _tran_datos_encrip ), NULL, 0, 0 ) == 0 ){
   //COMENTO HASTA QUE SE ARREGLE
    /*if( OPEN_TABLE( T_TRAN_CRP, TT_ORIG, &datos_encriptados, sizeof( struct _tran_datos_encrip ) ) == 0){
	
      //SELECT( area_disponible );
      //SET_ORDER( 1 );
      //GET_EQUAL( ( char * ) &nro_transaccion );
	 SET_WHERE( "NRO_TRANSACCION = '%ld'", nro_transaccion );
	 RUN_QUERY(NO);
	  
      if( FOUND2(  ) && datos_encriptados.id_transaccion == nro_transaccion ) {
         datos_encriptados.enviado = SI;
         rta = !UPDATE2();
      }
      //if( MODO_DEBUG == 75 || MODO_DEBUG == 80 )
      {
         char encrip[70];
         _snprintf(encrip, sizeof(encrip),"#MRC_ENVDOS.%4li-Rta=2%i",  //[A.u%3i-A.d%3i]"
         nro_transaccion, rta ); //, area_usada, area_disponible);
         encrip[sizeof( encrip)-1]=0;
         GRABAR_LOG_SISTEMA(encrip);
         if( MODO_DEBUG == 80 )
            MENSAJE(encrip);
      }

      //if( area_disponible )
      //CLOSE_DATABASE( area_disponible );
	  CLOSE_TABLE( T_TRAN_CRP, TT_ORIG );

   }else*/
      GRABAR_LOG_SISTEMA("S_ERROR_AL_ABRIR_LA_TABLA",LOG_ERRORES,2 );

   //if( area_usada )
      //SELECT(area_usada);
	  SELECT_TABLE( tabla_ant, tipo_ant );

   return rta;
}
/******************************************************************************/
void CEREA_DATOS_ENCRIPT( void )
/******************************************************************************/
{
 char nom_destino[80];

 NOM_ARCHIVO( _TRAN_CRP_SIC, nom_destino );
 //COPIAR_ARCHIVO( "TRAN_CRP.000", _TRAN_CRP_SIC, __LINE__, __FILE__ );
 COPIAR_ARCHIVO( "TRAN_CRP.000", nom_destino, __LINE__, __FILE__  ) ;
}
/****************************************************************************/
void REVISAR_TRANSACCIONES_ENCRIPT( void )
/****************************************************************************/
{
    int ok = 0,x = 0,y = 0,opcion = 0;
    long nro=0;
	char encrip[60];
   	unlink("encrip2.txt");
	 //CLOSE_DATABASE( AREA_TRANSAC2 );
	 CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
    if( T_ABRIR_TRANSAC2() ) {
        MENSAJE_STRING( S_AREA_TRANSAC2_CERRADA );
        return 0;
    }
	 while((nro = PEDIR_ENTERO_EN_RECUADRO( " iNGRESE NRO OPER. - 0 SALE", NO, 5 )) != 0 ){
		
		//GO(TOP);
		//SET_ORDER( 1 );
		//GET_EQUAL( ( char* )&nro );
		 SET_WHERE( "ID_TRANSACCION = '%ld'", nro );
		 RUN_QUERY(NO);
		
		if( FOUND2( ) ) {
			x = 10;
			y = 10;	
			{
				if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET )
					PROCESA_DATOS_ENCRIPT_TRANSACCION( NO , nro, &transac2->dt );

				_snprintf(encrip, sizeof(encrip),"#Operacion Nro %3li Tarjeta %s", nro ,transac2->dt.numero_cuenta_tarjeta);
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#Comercio Nro %s Lote %i",transac2->dt.numero_de_comercio, transac2->dt.lote );
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#Autorizacion %li",transac2->dt.autorizacion);
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#trace %li Ref Numnber %s",transac2->dt.numero_de_trace,transac2->dt.retrieval_reference_number);
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#PLan %i Cuotas %i",transac2->dt.plan, transac2->dt.cuotas);
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);

				/* FALTA SACAR DEL BLOQUE ENCIRPTADO LOS DATOS DEL TRACK Y DEMAS*/
			}
		}
	 }
  	 //CLOSE_DATABASE( AREA_TRANSAC2 );}
	 CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
}
/***************************************************************************************/
int CONVERSION_ASCII_EXT_A_TO_HEXA( int operacion,char *bloque_string,int *long_bloq_string, char* bloque_hexa,int *long_bloq_hexa){
/***************************************************************************************/

int rta =0, i = 0, longitud_bloque_string= *long_bloq_string, longitud_bloque_hexa= *long_bloq_hexa;
char auxi;

if( operacion == 0 && *long_bloq_string > 0 ){
	//string con ascii extendido a hexa
	memset(bloque_hexa,0,(*long_bloq_string * 2 ));

	for( i = 0; i < longitud_bloque_string   ; i++ ) {
		auxi = ( bloque_string[i] & 0xf0 ) >> 4;
		bloque_hexa[i * 2] = ( auxi < 0xa ) ? '0' + auxi : 'A' + auxi - 0xa;
		auxi = bloque_string[i] & 0x0f;
		bloque_hexa[i * 2 + 1] = ( auxi < 0xa ) ? '0' + auxi : 'A' + auxi - 0xa;
	}
	
	rta = SI;
	*long_bloq_hexa =(int)( i * 2);

	//memcpy( bloque_hexa, c, ( ( long_bloq_string ) * 2 ) );
}else{
	//string chexa a ascii con ascii extendido ..
	if( operacion == 1 && longitud_bloque_hexa > 0 ){
		memset(bloque_string,0,(int)(longitud_bloque_hexa / 2 ));
		for( i = 0; i < longitud_bloque_hexa ; i++ ) {
			auxi = ( bloque_hexa[i * 2] < 'A' ) ? bloque_hexa[i * 2] - '0' : bloque_hexa[i * 2] - 'A' + 0xa;
			bloque_string[i] = auxi << 4;
			auxi =( bloque_hexa[i * 2 + 1] < 'A' ) ? bloque_hexa[i * 2 + 1] - '0' : bloque_hexa[i * 2 + 1] - 'A' + 0xa;
			bloque_string[i] += auxi;
		}
		*long_bloq_string = i/2;
		rta = SI;
		printf( "%s\n", bloque_string );
	}
}

return rta;
}

/***************************************************************************************/
int ES_OPERACION_OFF(int operacion1 )
/***************************************************************************************/
{
	int rta = 0;
	switch( operacion1 ) {
	case _OFF_LINE_COMPRA:                
	case _OFF_LINE_DEVOLUCION:            
	case _OFF_LINE_ANULACION:             
	case _OFF_LINE_COBRO:                
	case _OFF_LINE_ANUL_COBRO:           
	case _OFF_LINE_ANULACION_DEVOLUCION:
	case _OFF_LINE_ANULACION_COBRO:
		 rta = 1;
		break;
	}
	return rta;
}

/***********************************************************************/
void ENVIAR_OPERACION_OFFLINE(  )
/***********************************************************************/
{
	 int ok = 0, ok_nodo, intentos,  nodo = 0;
    struct _nodos nodos;
    if( CIERRE_DE_LOTE_POR_NODO ) {
        if( /*USE_DB( AREA_AUX, _NODOS_SIC, ( char* )&nodos, sizeof( nodos ), NULL, 0, 0 )*/
			OPEN_TABLE(T_NODOS, TT_ORIG,( char* )&nodos, sizeof( nodos ))== 0 ) {
			SET_ORDER2("COD_NODO");
			SET_WHERE("");
			RUN_QUERY(NO);
            ok = 1;
			while( !dbEOF() ) {
                intentos = 0;
                ok_nodo = 0;
				nodo = nodos.cod_nodo;
				 protocolo = TCP_IP;
				 _CANTIDAD_OFF_LINE_A_ENVIAR = 1; //ojo con cambiarle el valor que en otro lado pregunta por el valor 1 OJOOO
				 CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
				if( T_ABRIR_TRANSAC2() ) {
					MENSAJE_STRING( S_AREA_TRANSAC2_CERRADA );
					return 0;
				}
                while( !ok_nodo && intentos < 1 ) {
                    ok_nodo = T_ENVIAR_TRANSACCIONES_PENDIENTES( nodo, NO, 3 );
                    intentos++;
				}
			
			_CANTIDAD_OFF_LINE_A_ENVIAR = 5;
			if( !ok_nodo ) {
                    ok = 0;
                }
				if( !OPENED( T_NODOS, TT_ORIG ) ) {
					OPEN_TABLE(T_NODOS, TT_ORIG,( char* )&nodos, sizeof( nodos ));
					SET_ORDER2("COD_NODO");
					SET_WHERE("COD_NODO >= '%i'",nodo);
					RUN_QUERY(NO);
				} else {
					SELECT_TABLE( T_NODOS, TT_ORIG );
				}
                SKIP2( 1 );
		}
         CLOSE_TABLE( T_NODOS, TT_ORIG );
		}
	}
	BORRAR_MENSAJE();
}
/***************************************************************************************/
void OCULTA_DATOS_ENCRIPTADOS( struct _datos_transaccion * tran )
/*
Aqui enmascaramos los datos sensibles del transac2 que no se pueden mostrar
*/
/***************************************************************************************/
{
//	memcpy( tran->numero_de_comercio,"*********", 9 );
    //tran->numero_de_comercio[9] = '\0';
//    tran->lote = 0;
	if( config_tps.NapseModalidad ==0) {
		memcpy( tran->numero_cuenta_tarjeta,"*********", 9 );
		tran->numero_cuenta_tarjeta[9] = '\0';
	}
    
//    tran->autorizacion = 0L;
//    tran->numero_de_trace = 0L;
    //memcpy( tran->retrieval_reference_number,"*********", 9 );
    //tran->retrieval_reference_number[9] = '\0';
//    tran->plan = 0;
 //   tran->cuotas = 0;
	if( tran->modo_de_ingreso != 1  ) {
		sprintf( tran->track_2, "%s", "" );
	}
}

#endif

/****************************************************************************/
int T_ANULAR_TRANSACCION( long nro_ticket, double *importe_t, int *nodo_t, UINT16 *modo_pago, int *cuota, long *fecha, int cashback )
/****************************************************************************/
{
    int ok = 0, rta=0, cant_cupones = 0, eleccion = 0;
    long nro_new;
    struct _datos_transaccion tran;
	long id_evento = 0L;
	int marca_tarjeta = 0;
	CLOSE_TABLE( T_TRANSAC2, TT_ORIG );
    
	if( T_ABRIR_TRANSAC2() ) {
	}
	/*veamos si existen nros de ticket repetidos*/ 

	rta =DIRECT_DB_QUERY("SELECT count(ticket) FROM transac2 t where  tipo_de_mensaje in( 200,220) AND enviado = 1 AND codigo_de_procesamiento in (0,90000, 200000) AND ticket = %li AND error = 0  AND codigo_de_respuesta = 0;",nro_ticket);
	if( rta )
		GET_DATO(1,C_INT,(char *)&cant_cupones,sizeof(cant_cupones));

	if( cant_cupones > 1) {
		//hay mas de 1 cupon por ende o hubo error o coincde la numeracion entre nodos justo ahora
		//mostremos cupon importe hora?
		struct _menu items[5];
        long hora1 = 0;
		double importe =0.00;
		char nombres[5][30];
		int posicion =0, i=-1;
		char hora[20];
		 int columnas, id_transaccion = 0;
		 struct _cabmenu *cab;
		

		rta =DIRECT_DB_QUERY("SELECT importe, hora_generacion, id_transaccion FROM transac2 t where  tipo_de_mensaje in( 200,220) AND enviado = 1 AND codigo_de_procesamiento in (0,90000, 200000) AND ticket = %li AND error = 0  AND codigo_de_respuesta = 0;",nro_ticket);
		if( rta ) {
			do {
				GET_DATO( 1, C_DOUBLE, (char *)&importe, 8 );
				GET_DATO(2,C_INT,(char *)&hora1,sizeof(hora1));
				GET_DATO(3,C_INT,(char *)&id_transaccion,sizeof(id_transaccion));
				memset(hora,0, sizeof(hora));
				HTOAL( hora1, hora );
				i++;
				sprintf( ( char* )&nombres[i][0], " %d) %s %.2lf", i, hora, importe );

				items[i].nom = ( char* )&nombres[i][0];
                items[i].tecla = posicion + 48;   // para obtener el ascii
                // '1',...'n'
                items[i].rta = id_transaccion;
				posicion++;
			
		
			} while ( rta = DIRECT_DB_SKIP(1) );
            columnas = 2;
            cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
            if( cab != NULL ) {
                cab[0].nombre = "   HORA";
                cab[0].align = ALIGN_CENTER;
                cab[0].width = 8;
                cab[1].nombre = "IMPORTE";
                cab[1].align = ALIGN_LEFT;
                cab[1].width = 20;
                eleccion = MENU( "ELIJA CUPON CORRECTO", items, i + 1, cab, columnas );
                //             rta = Menu( ST( S_TICKETS_EN_ESPERA ), items, i + 1, cab, columnas );
				 if( eleccion != -1 ) {
				 }
                free( cab );
            }
		}
        
	}

    nro_new = T_CALCULA_NRO_TRANSACCION();
	
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	if( cant_cupones > 0 && eleccion > 0 )
		SET_WHERE("tipo_de_mensaje in( 200,220) AND enviado = 1 AND codigo_de_procesamiento in(0,90000, 200000) AND ticket = %li AND error = 0  AND codigo_de_respuesta = 0 and id_transaccion = %i", nro_ticket,eleccion);
	else
		SET_WHERE("tipo_de_mensaje in( 200,220) AND enviado = 1 AND codigo_de_procesamiento in(0,90000, 200000) AND ticket = %li AND error = 0  AND codigo_de_respuesta = 0", nro_ticket);
	
	RUN_QUERY(NO);
    //ojo buscar que no se haya anulado despues y se duplique la anulacion!!!!!
    if( FOUND2() ) { //encontramos la operacion 
		long nro = transac2->id_transaccion;
		*importe_t = transac2->dt.importe;
		*nodo_t = transac2->nodo;
        memcpy( &tran, &transac2->dt, sizeof( struct _datos_transaccion ) );
		    
		CARGAR_DATOS_BASICOS( &tran, cashback );
		//cargar datos transaccion
/*		tran.ticket_original = transac2->dt.ticket;
		tran.tipo_de_mensaje = TIPO_MENSAJE_COMPRA;
		tran.tipo_de_mensaje_original = transac2->dt.tipo_de_mensaje;*/
		id_evento = transac2->dt.id_evento; 
		marca_tarjeta = transac2->dt.codigo_marca + 1;
		*cuota = transac2->dt.cuotas;
		*fecha = transac2->fecha_generacion;
		SET_MEMORY_INT( __var_tarjetas_operacion, transac2->operacion);
		if( transac2->operacion == _ON_LINE_CASHPLUS ) {//la anulacion de cashplus tiene que mandar el importe de retiro
			SET_MEMORY_FLOAT( __var_tarjetas_anticipo, (float) tran.interes );
		}
		ok = 1;


	/*	if( FOUND2()) {
					memcpy(tran.bloque_encriptado.buffer,transac2->dt.bloque_encriptado.buffer,transac2->dt.bloque_encriptado.len);
					tran.bloque_encriptado.len = transac2->dt.bloque_encriptado.len;
					tran.bloque_encriptado.id_clave = transac2->dt.bloque_encriptado.id_clave;
					tran.bloque_encriptado.version_bloque = transac2->dt.bloque_encriptado.version_bloque;
					tran.datos_tarjetas_encriptados=transac2->dt.datos_tarjetas_encriptados;
		}
*/
/*        if( !ENCRIPTAR_DATOS_TARJETA ) {
            DESENCRIPTA_UN_TRACK( 38, transac2->fecha_generacion, tran.track_2, tran.track_2 );
            DESENCRIPTA_UN_TRACK( 80, transac2->fecha_generacion, tran.track_1, tran.track_1 );
            DESENCRIPTA_UN_TRACK( 2, transac2->fecha_generacion, ( char* )&tran.cvv,
                                  ( char* )&tran.cvv );
        }*/
#ifdef DEFINE_ENCRIPTA_TRANSACCIONES
		  if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET ){
				PROCESA_DATOS_ENCRIPT_TRANSACCION( NO , nro, &tran );
				if( MODO_DEBUG == 75 || MODO_DEBUG == 80 ){
					
				 char encrip[70];
				 _snprintf(encrip, sizeof(encrip),"#Rev a Desencrip %2i - %3li %s", NO, nro,&tran.numero_cuenta_tarjeta[5]); 
				 encrip[sizeof( encrip)-1]=0;
				 GRABAR_LOG_SISTEMA(encrip,LOG_DEBUG,2);
				 if( MODO_DEBUG == 80 )
					MENSAJE(encrip);
				}
		   }
#endif

    }
	//ahora busquemos que no haya sido anulada
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE("tipo_de_mensaje in( 200,220) AND enviado = 1 AND codigo_de_procesamiento in( 20000, 140000) AND ticket_original = %li AND error = 0  AND codigo_de_respuesta = 0 and ROUND(importe, 2) = ROUND(%lf, 2)", nro_ticket,transac2->dt.importe);
	RUN_QUERY(NO);
    //ojo buscar que no se haya anulado despues y se duplique la anulacion!!!!!
    if( FOUND2() ) { //encontramos la operacion de anulacion ya hecha no se puede
		ok = -2;
		return (ok);
	}

	
	//busquemos en la ev_medios
	SELECT_TABLE( T_PAGOS, TT_ORIG );
	SET_WHERE("ID_EVENTO = '%ld' and tarjeta = %i and nro_cupon_int1 = %li",id_evento, marca_tarjeta, nro_ticket  ); //*importe_t 
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);

	if( FOUND2() ) {
		*modo_pago = event_pago.modo_pago;

	} else
		ok = -1;

	return ( ok );
}

/****************************************************************************/
void CARGAR_DATOS_BASICOS( struct _datos_transaccion *tran, int cashback )
/****************************************************************************/
{
			
	char marca = tran->codigo_marca;
//	char* plan; 
	char tabla[6];
	char auto_original[11];

//	SET_MEMORY_LONG( __nro_evento, tran.nro_transaccion );
	SET_MEMORY_LONG( __var_tarjetas_terminal, NRO_TERMINAL( marca ) );
//	SET_MEMORY_DOUBLE( __var_tarjetas_importe_entrega, (double) tran->importe_entrega );
	SET_MEMORY_CHAR( __var_tarjetas_moneda, tran->moneda );
	SET_MEMORY_CHAR( __var_tarjetas_cuotas, tran->cuotas );
//	SET_MEMORY_INT( __var_tarjetas_primer_vto, tran->primer_vto );
//            tran.modo_ingreso = VAR_TARJETAS_LECTURA_BANDA;
//            tran.fecha_expiracion = DECREMENTAR_MES( VAR_TARJETAS_FECHA_VTO );

	SET_MEMORY_INT( __var_tarjetas_fecha_vto, tran->fecha_expiracion ); //ver si hay que incrementar mes
    SET_MEMORY_CHAR( __var_tarjetas_tarjeta, marca );
//	SET_MEMORY_INT( __var_tarjetas_nro_boletin,tran->boletin);
  //          tran.cvv = VAR_TARJETAS_CVV;
	SET_MEMORY_INT( __var_tarjetas_cvv, tran->cvv );
	SET_MEMORY_CHAR( __var_tarjetas_tipo_cuenta,(int) tran->tipo_de_cuenta );
	glog("codigo de autorizacion es ",1,1);
	if(VAR_TARJETAS_AUTORIZACION != NULL)
		glog(VAR_TARJETAS_AUTORIZACION,1,1);
/*	SET_MEMORY_CHAR( __var_tarjetas_cod_vigencia, tran->cod_vigencia );*/
	SET_MEMORY( __var_tarjetas_nro_comercio, tran->numero_de_comercio );
	memset( tabla, 0, sizeof( tabla ) );
	memcpy( tabla, tran->tabla, sizeof( tran->tabla ) );

	SET_MEMORY( __var_tarjetas_tabla, tabla );

	memset(auto_original,0,11);
    _ltoa( transac2->dt.autorizacion, auto_original, 10 );

	SET_MEMORY_LONG( __var_tarjetas_cupon_original, transac2->dt.ticket );
	SET_MEMORY( __var_tarjetas_aut_original, auto_original );
	if( cashback == 1 &&  RAM_MODO_EJECUCION == CAMBIO_MEDIO )
		//cashback toma numero de cupon de la manera normal no como anulacion
		if( config_tps.HabilitarCashPlusSolo == 0) //aqui el nro de cupon es de la compra
			SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,transac2->dt.ticket );
		else
			SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
					OBTENER_NRO_CUPON( VAR_TARJETAS_TARJETA ) );
	/*else
		SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,
                        CALCULAR_NRO_CUPON( marca ) );*/
	else { //la anulacion no debe incrementar el nro de cupon porque ya se incremento en la operacion original, ver si no es reverso
		int marca = TARJ[VAR_TARJETAS_TARJETA].cod_numeracion - 1;	
		SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int, ( long )_NRO_CUPONES( marca ));
	}

	
	SET_MEMORY_CHAR( __var_tarjetas_lectura_banda, transac2->dt.modo_de_ingreso ); //0 manual, 1 banda, 2 scanner
	
	//truchada porque esto se setea en la respuesta
	//SET_MEMORY_CHAR( __var_tarjetas_on_line, SI );

	//valores anteriores revisar!!!

	SET_MEMORY_INT( __var_tarjetas_fecha_original,tran->fecha_host );
    SET_MEMORY_LONG( __var_tarjetas_hora_original, tran->hora_host );

	
	SET_NUMERO_TARJETA_PRIVADO( tran->numero_cuenta_tarjeta );
	

}

/****************************************************************************/
void IMPRESION_BAUCHER_ANULACION_TECLA( double pago_importe, long anulacion  )
/****************************************************************************/
{
	int cod_val;
	unsigned aux_fecha;
	double auxi = _PAGO_IMPORTE( 0 );
	_SET_MEMORY_DOUBLE( __pago_importe, 0, pago_importe ); //para el val-caja

	if( EMITIR_VOUCHER_TARJETA && VOUCHER_TARJETA_FISCAL == NO ) {
		cod_val = ( VAR_TARJETAS_MONEDA == PESOS ) ? TARJ[VAR_TARJETAS_TARJETA].
        val_voucher_pesos : TARJ[VAR_TARJETAS_TARJETA].val_voucher_dolares;
        if( !cod_val ) {
   			if( IMPRESORA_FISCAL == FISCAL )
   				ABRIR_DOCUMENTO_NO_FISCAL ( 0);
			OPEN_COPIA_VOUCHER_PRN(  );//EL OPEN LO BORRA
			IMPRIME_VOUCHER( 0, 1, anulacion );
			CLOSE_COPIA_VOUCHER_PRN(  );
   			if( IMPRESORA_FISCAL == FISCAL )
   				CERRAR_DOCUMENTO_NO_FISCAL();

           }
			else {
						/*Se decrementa el mes solo para la impresion, luego se le devuelve el valor original*/
						
						aux_fecha = GET_FECHA_VTO_PRIVADA( );
						SET_FECHA_VTO_PRIVADA( DECREMENTAR_MES( aux_fecha ));
						
						VALIDAR_VOUCHER_TARJETA( cod_val, pago_importe, 0 );
						
						SET_FECHA_VTO_PRIVADA( aux_fecha );
             }
             
       }
	_SET_MEMORY_DOUBLE( __pago_importe, 0, auxi );
}

/*****************************************************************************/
int DESENCRIPTAR_BLOQUE()
/*****************************************************************************/

{
	long nro_ticket = 0L;
	 int       rta = 0,ln_in = 0,ln_out = 0;
   char entrada[60], salida[150],extra[60];
//	struct _bloque_a_encriptar bloque_out;
	char auxiliar[1024];
	int longAuxiliar1=0,longAuxiliar2=0;
	char encrip[60];

	
		memset(auxiliar,0,sizeof( auxiliar ) );
      
			memset(salida,0,sizeof( salida ) );
			memset(entrada,0,sizeof( entrada ) );
			memset(extra,0,sizeof( extra ) );
			_snprintf( entrada, sizeof( entrada ),"abcdefghijklmnñopqrstuvwxyz=ABCDEFGHIJKLMNÑOPQRSTUVWXYZ");	
			_snprintf( extra, sizeof( extra ),"abcdefghijklmnñopqrstuvwxyz=ABCDEFGHIJKLMNÑOPQRSTUVWXYZ");	
            ln_in = sizeof( entrada );

            
            ln_in = sizeof( entrada );
	
	MENSAJE("INGRESE EL ID_TRANSACCION A DESENCRIPTAS");
	nro_ticket = PEDIR_ENTERO_EN_RECUADRO( "ID?TRANSACCION?DESCEN", NO, 6 );
	BORRAR_MENSAJE();
	SELECT_TABLE( T_TRANSAC2, TT_ORIG );
	SET_WHERE("ID_TRANSACCION ='%ld'", nro_ticket);
	RUN_QUERY(NO);
	

//
	//if( RSA_ENCRIPTAR( (char*)&bloque_in, sizeof( bloque_in ), ) &&

		
	
   //entrada el bloque encriptado, salida desencriptado
/*	PROCESA_DATOS_ENCRIPT_TRANSACCION( 0, nro_ticket,&transac2->dt );
	InitEncryDesencriptacion( );
	RSA_DESENCRIPTAR( &transac2->dt.bloque_encriptado , sizeof( transac2->dt.bloque_encriptado ),&auxiliar, &longAuxiliar1   );
	DESACTIVAR_PRIVATE_KEY();
*/
   	unlink("encrip2.txt");
	if( ENCRIPTA_DATOS_DE_TRANSAC_TARJET )
			PROCESA_DATOS_ENCRIPT_TRANSACCION2( NO , nro_ticket , &transac2->dt );

	_snprintf(encrip, sizeof(encrip),"#Operacion Nro %3li Tarjeta %s", nro_ticket  ,transac2->dt.numero_cuenta_tarjeta);
	MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#Comercio Nro %s Lote %i",transac2->dt.numero_de_comercio, transac2->dt.lote );
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#Autorizacion %li",transac2->dt.autorizacion);
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#trace %li Ref Numnber %s",transac2->dt.numero_de_trace,transac2->dt.retrieval_reference_number);
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);
				_snprintf(encrip, sizeof(encrip),"#PLan %i Cuotas %i",transac2->dt.plan, transac2->dt.cuotas);
				MENSAJE_SIN_SONIDO(encrip,0);
				glog( encrip ,LOG_INCONDICIONAL,1);

				/* FALTA SACAR DEL BLOQUE ENCIRPTADO LOS DATOS DEL TRACK Y DEMAS*/
  	 //CLOSE_DATABASE( AREA_TRANSAC2 );}
	 CLOSE_TABLE( T_TRANSAC2, TT_ORIG );


	return 1;
}


/****************************************************************************/
int PROCESA_DATOS_ENCRIPT_TRANSACCION2( int encripta , long nro_transaccion,struct _datos_transaccion * tran )
/****************************************************************************/
{

   int       rta = 0,/*area_disponible = 0 ,*/ln_in = 0,ln_out = 0/*,area_usada =0*/, tabla_ant, tipo_ant;
 //  char encrip[100];
//  struct _tran_datos_encrip datos_encriptados_reg,datos_encriptados;
   struct _datos_desencriptados datos_desencriptados;
   int longString = 0, longHexa = 0;

   struct _bloque_encriptado datos_encriptados_reg2 ,datos_encriptados2 ;
   struct _bloque_a_encriptar datos_desencriptados2;


// levanta las llaves de encriptacion y hace una prueba con caracteres.si es ok, sigo
	if( InitEncryDesencriptacion( ) &&	PROCESA_CONTROL_DATOS_ENCRIPTADOS( 0 )) {
		rta = 1;
	}

   //area_usada = SELECTED();
   //area_disponible = AREA_DISPONIBLE( );
   tabla_ant = SELECTED2();
   tipo_ant = SELECTED_TIPO();
 
									
 /*  if( rta && OPEN_TABLE( T_TRAN_CRP, TT_ORIG, &datos_encriptados_reg, 
							sizeof( struct _tran_datos_encrip ) ) == 0 ) {
	
      memset( &datos_desencriptados, 0, sizeof( struct _datos_desencriptados ));

		 SELECT_TABLE( T_TRAN_CRP, TT_ORIG );
		 SET_WHERE( "ID_TRANSACCION = '%ld'", nro_transaccion );
		 RUN_QUERY(NO);

         if( FOUND2(  ) && datos_encriptados_reg.enviado != SI ) {*/

//            ln_in = datos_encriptados_reg.datos.len;
		   strncpy( datos_encriptados_reg2.buffer, tran->bloque_encriptado.buffer,sizeof(tran->bloque_encriptado.buffer));
		   datos_encriptados_reg2.id_clave = tran->bloque_encriptado.id_clave;
		   datos_encriptados_reg2.len = tran->bloque_encriptado.len;
		   datos_encriptados_reg2.version_bloque = tran->bloque_encriptado.version_bloque;
		ln_in = datos_encriptados_reg2.len;

			//del bloque que tiene los caracteres de hexa..al que puede tener nulos
			longHexa = ln_in*2;
//			CONVERSION_ASCII_EXT_A_TO_HEXA( 1,datos_encriptados.datos.buffer,&longString, datos_encriptados_reg.datos.buffer,&longHexa);
			CONVERSION_ASCII_EXT_A_TO_HEXA( 1,datos_encriptados2.buffer,&longString, datos_encriptados_reg2.buffer,&longHexa);


			ACTIVAR_PRIVATE_KEY();
/********************************* DESENCRIPTA  *******************************/
            if( RSA_DESENCRIPTAR( (char*)datos_encriptados2.buffer/*datos_encriptados_reg.datos.buffer*/, ln_in ,
                                           (char*)&datos_desencriptados2 , &ln_out  )){
/************************** DATOS DESENCRIPTADOS A LA DATOS TRANSACCION ********/
               strncpy( tran->numero_de_comercio,datos_desencriptados.numero_de_comercio,sizeof(tran->numero_de_comercio));
               tran->lote = datos_desencriptados.lote;
               strncpy( tran->numero_cuenta_tarjeta,datos_desencriptados.numero_cuenta_tarjeta,sizeof(tran->numero_cuenta_tarjeta));
               tran->autorizacion = datos_desencriptados.autorizacion;
               tran->numero_de_trace = datos_desencriptados.numero_de_trace;
               strncpy( tran->retrieval_reference_number,datos_desencriptados.retrieval_reference_number,sizeof(tran->retrieval_reference_number));
               tran->plan = datos_desencriptados.plan;
               tran->cuotas =datos_desencriptados.cuotas;
			   
			}
			return 0;
}

/*****************************************************************************/
void GUARDAR_OPERACION_NAPSE_DAT( void )
/*****************************************************************************/
{
   int       handle_napse;
   char     *buffer_voucher = NULL;

   handle_napse = _CREAT( "napse.dat", /*S_IFREG |*/ S_IWRITE | S_IREAD /*| O_BINARY*/, __LINE__, __FILE__ );
   if( handle_napse > 0 ) {
  //    buffer_voucher = MALLOC( SIZEOF_STRUCT( _var_tarjetas ) );
	   memcpy( buffer_in, &transac2->dt, sizeof( struct _datos_transaccion ) );
      if( buffer_in ) {
	     LSEEK( handle_napse, 0L, SEEK_SET, __LINE__, __FILE__ );
         _WRITE( handle_napse, ( char * ) buffer_in, sizeof( struct _datos_transaccion ) - sizeof( transac2->dt.reserva1 ),__LINE__, __FILE__ );
		// _WRITE( handle_voucher, ( char * ) buffer_voucher, SIZEOF_STRUCT( _var_tarjetas ),__LINE__, __FILE__ );
	//	 FREEMEM( buffer_voucher );
    //     buffer_voucher = 0;
      }
	  _close( handle_napse );
   }
}

/*****************************************************************************/
void SETEOS_VALORES_RESPUESTA_NAPSE( int lecturaTabla)
/*****************************************************************************/
//aqui deberia ir la lectura de napse.dat que nos mandaron, aca lo simulo
{
	
	memcpy( tran_temp->pin_working_key, "", 8 );
	tran_temp->fecha_contable = 0;
	tran_temp->fecha_host = 0;
	tran_temp->hora_host = 0;

	if( lecturaTabla ==SI ) {
		int area_ant = 0, tipo_ant = 0;
		char sql[500];
		CLOSE_TABLE( T_RESPUESTA_NAPSE , TT_ORIG );
		if( T_ABRIR_NAPSE() ) {
			MENSAJE_STRING( S_AREA_TRANSAC2_CERRADA );
			return 0;
		}
		memset(sql,0,sizeof(sql));

		area_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();
		SELECT_TABLE( T_RESPUESTA_NAPSE, TT_ORIG );
		_snprintf(sql,sizeof(sql)-1,"id_transaccion = '%d' and id_evento = '%ld' and caja_z = '%ld'",transac2->id_transaccion,transac2->id_evento,transac2->caja_z);
		SET_WHERE( sql );
		RUN_QUERY(NO);
		if( FOUND2() ) { //encon
			char autoalfa[11];

			tran_temp->importe = transac2->dt.importe; //no deberia ser el total de la tarjeta?
			/*if( transac2->operacion == _ON_LINE_COMPRA ) {
				transac2->dt.importe = tran_temp->importe;
			}*/
	
			
			
			if(respuestaNapse->estado == 1)
				tran_temp->codigo_de_respuesta = 0;  //el 0 es ok para nosotros
			else {
				tran_temp->codigo_de_respuesta = 2;//respuestaNapse->estado; //chequear los codigos aqui que sirvan 
				return 1;
			}

			sprintf(autoalfa,"%li",respuestaNapse->autorizacion);
			memcpy( tran_temp->autorizacion_alfa, autoalfa,sizeof( transac2->dt.autorizacion_alfa ) );
	/*		0 0 "APROBADA "
	1 2 "PEDIR AUTORIZACION"
	2 2 "PEDIR AUTORIZACION"
	3 3 "COMERCIO INVALIDO"
	4 3 "CAPTURAR TARJETA"
	5 3 "DENEGADA"
	7 3 "RETENGA Y LLAME"
	*/
			if( tran_temp->codigo_de_respuesta == 7 ){
				tran_temp->ticket_original = tran_temp->ticket;
			}
			tran_temp->importe_cuota = RAM_TOTAL;
			tran_temp->saldo = 0;//tran_temp->saldo;
			tran_temp->lote = respuestaNapse->lote;

			tran_temp->autorizacion =respuestaNapse->autorizacion;
			memcpy( tran_temp->nro_cuenta, respuestaNapse->nro_cuenta, sizeof( respuestaNapse->nro_cuenta ) );
			tran_temp->tasa_aplicada = 0;
		
			memcpy( tran_temp->nro_cuenta2, respuestaNapse->nro_cuenta,	sizeof( transac2->dt.nro_cuenta2 ) );
	
		
			tran_temp->ticket = respuestaNapse->ticket;//nro de cupon
			SET_MEMORY_LONG( __var_tarjetas_nro_cupon_int,	tran_temp->ticket);
		
			memcpy( tran_temp->numero_cuenta_tarjeta,respuestaNapse->tarjeta , sizeof( tran_temp->numero_cuenta_tarjeta ) );


			tran_temp->tipo_de_cuenta = respuestaNapse->tipo_cta;

			memcpy( tran_temp->bloque_encriptado.buffer,respuestaNapse->nombre_titular , sizeof( tran_temp->bloque_encriptado.buffer) );

			SET_NUMERO_TARJETA_PRIVADO( tran_temp->numero_cuenta_tarjeta);
			//aca guardemos el nro de lote por si en algun momento queda offline y tengamos el ultimo
			if( tran_temp->lote > 0 && tran_temp->nodo < 10 ) { //limitados hasta 10 nodos y menores de 10 solo para los offline
				//la posicion la da el nodo
				_SET_MEMORY_LONG( __pago_datafono, tran_temp->nodo, tran_temp->lote );
			}
		}
	
	}
	
}
/*****************************************************************************/
void LEER_OPERACION_NAPSE( void )
/*****************************************************************************/
{
	int       handle_voucher;
   char     *buffer_voucher = NULL;
   struct _datos_transaccion *tran_temp = NULL;

   tran_temp = (struct _datos_transaccion *)MALLOC( sizeof( struct _datos_transaccion ) );
	 memset( tran_temp, 0, sizeof( struct _datos_transaccion ) );
	if( access( "napse.dat", 0 ) == 0 ) {
      handle_voucher = _OPEN( "napse.dat", O_RDONLY, __LINE__, __FILE__ );
      if( handle_voucher > 0 ) {
         buffer_voucher = ( char * ) MALLOC( sizeof( struct  _datos_transaccion));
         if( buffer_voucher ) {
               _READ( handle_voucher, buffer_voucher, sizeof( struct _datos_transaccion ) , __LINE__, __FILE__ );
                memcpy( tran_temp, buffer_voucher, sizeof( struct _datos_transaccion ) );
//-- Acá hay que imprimir el voucher con los datos de la estructura Tarjeta.
           
            }
//fin for
			FREEMEM( buffer_voucher );
			buffer_voucher = 0;
         }
		 _close( handle_voucher );
	}
   unlink("napse.dat");

}

/****************************************************************************/
int T_ABRIR_NAPSE()
/****************************************************************************/
{
	int ok = 1;
    //return ( USE_DB( AREA_TRANSAC2, _TRANSAC2_SIC, ( char* )transac2,
    //                 sizeof( struct _transac2_sic ), NULL, 0, 0 ) );
	memset( respuestaNapse, 0, sizeof( struct _respuesta_napse ) );
	if( OPEN_TABLE( T_RESPUESTA_NAPSE, TT_ORIG, ( char* )respuestaNapse, 
					sizeof( struct _respuesta_napse ) ) == 0 ) {
		SET_WHERE("");
		RUN_QUERY(NO);
		return( 0 );
}
	return( ok );

}

/*****************************************************************************/
void OBTIENE_NOMBRE_CLIENTE_NAPSE( char *nombre, int p_pago )
/*****************************************************************************/
{
	sprintf(nombre, tran_temp->bloque_encriptado.buffer, sizeof(tran_temp->bloque_encriptado.buffer));//Seba
}

/*****************************************************************************/
void COPIAR_RTA_NAPSE( void )
/*****************************************************************************/
{
	memcpy( transac2->dt.pin_working_key, tran_temp->pin_working_key, 8 );
	transac2->dt.ticket = tran_temp->ticket;

	memcpy(transac2->dt.numero_cuenta_tarjeta,tran_temp->numero_cuenta_tarjeta , sizeof( tran_temp->numero_cuenta_tarjeta ) );
	transac2->dt.tipo_de_cuenta = tran_temp->tipo_de_cuenta;

}

/*****************************************************************************/
long GET_ID_TRANSACCION( void) 
/*****************************************************************************/
{
	
	return transac2->id_transaccion;
}

/*****************************************************************************/
void GET_NUMERO_CUENTA_TARJETA( char cadena[21]) 
/*****************************************************************************/
{
	memcpy(cadena,transac2->dt.numero_cuenta_tarjeta, sizeof( transac2->dt.numero_cuenta_tarjeta ) );
	cadena[20]=0;
}