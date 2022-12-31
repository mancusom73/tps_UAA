#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <bancos.h>
#include <baucher.h>
#include <btrv.h>
#include <b_mens.h>
#include <cadafech.h>
#include <calcular.h>
#include <cio.h>
#include <clave.h>
#include <clientes.h>
#include <cmemory.h>
#include <comprob.h>
#include <cr.h>
#include <cr_disco.h>
#include <day.h>
#include <ini.h>
#include <dbrouter.h>
#include <dd.h>
#include <dec_mes.h>
#include <d_entry.h>
#include <fiscal.h>
#include <getch.h>
#include <get_fech.h>
#include <get_hora.h>
#include <imp_tick.h>
#include <log.h>
#include <mensaje.h>
#include <menu.h>
#include <mon.h>
#include <mstring.h>
#include <m_cadena.h>
#include <m_entero.h>
#include <m_fecha.h>
#include <name_mon.h>
#include <nume.h>
#include <pagos.h>
#include <pago_che.h>
#include <pago_mut.h>
#include <pago_tc.h>
#include <print.h>
#include <round.h>
#include <scanner.h>
#include <stringt.h>
#include <tkt.h>
#include <tomra.h>
#include <trim.h>
#include <t_reproc.h>
#include <uti-val.h>
#include <valida.h>
#include <year.h>
#include <_cr0.h>
#include <_cr1.h>
#include <path.h>
#include <recupero.h>
#include <tarjetas.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <archivos.h>
#include <string.h>
#include <math.h>
#include <aarch.h>
#include "_dd.h"
#include "actions.h"
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
#include "pagatodo.h"
#include <deftables.h>
#include <db.h>
#include <ftoc.h>
#pragma pack(1)
extern int TIPO_EVENTO( INT16 codigo, INT16 tipo_comprobante );
extern int GET_HANDLE_VOUCHER ();
extern char _COPIA_VOUCHER_PRN;
extern struct _aux_envases aux_envases;
extern void ABRIR_NOTA_DE_CREDITO(char formtype, int IvaEmi, int IvaComp, char *name, char *lastname,int docType, char *DocNum, char *address1, char *address2, char *address3, char *remito, char *remito1);
extern void FREEMEM( char *puntero );
extern void CARGA_TOTAL_IMPUESTOS( double *total, char cod_ext );
extern int VALIDACION_DEL_COMPROBANTE( int tipo_comprobante,int tipo_evento_comp,int bloque ) ;
extern int COD_VALIDACION_VOUCHER( int marca );
extern int IMPRIME_MENSAJE_TARJETA( int marca );
extern char * NOMBRE_TARJETA( int marca );
extern int OBTENER_DESCRIPCION_COND_IVA( char *c_iva, char con_iva, int largo );
float VALIDA_tomar_float();
extern int PRN_CAMBIA_SLIP_LANDSCAPE( int landscape );
void CORTAR_PAPEL_ANTES_DE_VALIDACION( int ind, int tipo );
int VAL_evaluar_condiciones( int codigo, int h );
/*--------------------------------- Globales -----------------------------*/
int _VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo  , int * cantidad_pagos, int*  cant_items );
char imprimiendo_factura_journal_val = 0;
extern struct _ConfTkt *CONFIG_VALIDA;
extern unsigned *TABLA_VALIDA;
static int ind_config;
static /*int*/ INT16  fila, columna, _NRO_IMPRESION_CHEQUE;
static char COMP_INDEP = 0, *P_CAD_CONT_VAL = NULL , REIMPRESION, CONFIRMAR_IMPRESION = 0;
static int val_cant_pagos,renglon,cant_renglones, v_cant_pagos,_tipo,_hoja_actual;
extern void EJECT();
extern void PRN_INHABILITA_SLIP();
extern int imprime_copia_voucher ;
_NRO_REIMPRESION_FACTURA = 0, _SELECCION_PAPEL, _NRO_DOCUMENTO_FISCAL , cant_columnas, cant_filas;
static char SI_CODIGO_MEDIO, destino_cupon = 0, SI_CODIGO_MEDIO, *_SI_P_FORMULA = NULL;
int _NRO_COMANDO_RETIRO,_NRO_COMANDO_FONDO_FIJO,_NRO_COMANDO_INVENTARIO;
static int _VALIDA_FOR_I,_VALIDA_FOR_LIMITE,_VALIDA_FOR_IND_CONFIG;
static char SI_DEVOLUCION, en_reimpresion = 0;
static double imp_ing_medios = 0.0 ;
static double precio = 0.0,importe,cantidad,precio_final,percep_212_iva1,percep_212_iva2,cant_total,costo;
static double total,subtotal,exento,imp_int,iva1,iva2,neto,total_descuento;
static double total_backup, subtotal_backup, exento_backup, imp_int_backup, iva1_backup;
static double percep_212_iva1_backup, percep_212_iva2_backup;
static double iva2_backup, neto_backup, backup_totales = 0;
static double imp_monto_impuesto_back[4];
static double percepcion_dgi, retencion_dgr_backup, percepcion_dgi_backup, total_impuestos_backup[20];
static double imp[4];
int NRO_DOC_FISCAL_COMP;
char SI_HAY_MEDIO[10] = {0,0,0,0,0,0,0,0,0,0};
static int orden,ind_comienzo[5],ind_comienzo_cuerpo,FILA_CONT,COLUMNA_CONT;
char *TIPO_COMP_VAL[] = {"FACTURA","NOTA CRED","REMITO","FAC REM","TKT",
                                "TICKET","CREDITO FISCAL","FACT CONS FINAL"};
static char *ORDEN[] = {"ORIGINAL","DUPLICADO","TRIPLIC.","CUADRUP."};
char articulo_exento, articulo_gravado;
/*extern struct _ConfTkt *CONFIG_FACTURA;
extern struct _ConfTkt *CONFIG_REMITO;
extern struct _ConfTkt *CONFIG_TKT1;*/
static struct _ConfTkt *CONFIG_COMPROBANTE1;
static int CANT_RENGLONES[5],LARGO_COMPROBANTE[5];
static char AGREGAR_IVA_NO_INSC, PEDIR_FORMULARIO_CON_TECLA = 1;
static double nro_comp;
static float porc_retenc_dgr,porc_percep_dgi, porc_retenc_dgr2;
static int signo_nc = 1;
static char SI_FACTURA_A;
static char SI_FACTURA_B;
static char SI_NOTA_CREDITO;
static char SI_BIEN_DE_USO;
static char SI_EXENTO;
static char SI_GRAVADO;
static char HAY_GRAVADO;
static char HAY_EXENTO;
extern int nro_cuerpo;
extern int tipo_devolucion;

static int _COMP_FOR_I,_COMP_FOR_LIMITE,_COMP_FOR_IND_CONFIG;
long _secuencia_ant;
char LARGO_CADENA,OFFSET_CADENA;
UINT16 _COD_VARIABLE_RNV,_SKIP_BLOCK;
static double total_impuestos[40],min_retenc_dgr,min_percep_dgi,retencion_dgr,percepcion_dgi,
              total_impuestos_factura;
INT16 _SUB_VARIABLE_RNV;
UINT16 _VALOR_RNV;
char _COMPARACION_IGUAL;
char cheque_a_fecha = 0;
extern char *PRINT_BUFFER;
char *bmp_printer_fiscal;
extern long valida_kinacu_id_transaccion;
int CANT_VOUCHER = 0;
char indice_pos = 0;
int INDICE_IMPRESION = 0;
char SI_DESCUENTO_TARJETA = 0, VAL_COMPARACION_IGUAL, VAL_SI_REIMPRESION = 0;
struct institut_donacion
{
    BINARY status;
    int cod_institucion;
    char desc_institucion[31];
    //char reserva[50];
};
struct _art_fact
{
    long cod_articulo;
    char cod_barra[17];
    float cantidad;
    double precio;
    double neto_gravado;
    double imp_int;
    double exento;
    double iva1;
    double iva2;
    unsigned long pos;
    char cod_iva;
    char gravado;
    double costo;
    INT16 pos_ticket;
    double monto_impuesto[4];
    char cod_movimiento;
} *art_fact_val;

struct _art_fact *art_desc;

struct _art_fact_ivax
{
    double neto_gravado;
    double iva1;
    double iva2;
} ;
struct _art_fact_ivax* art_fact_iva = NULL;
struct _art_fact_ivax *art_fact_iva_bac = NULL;

extern double SUMAR_TOTAL_FAC_CAJA( int recalcular );
struct _valida valida;
struct _ev_dmedios dpago_boleta_plata;
struct ImpresionDatosAdicionales ImpDatosAdi;
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
extern void RESOLVER_TKT_CONSULTA_SQL(char *Consulta, char *Funciones, char *TipoRes, int linea);
#define _V_INICIALIZA		0
#define _V_CABECERA		1
#define _V_CUERPO			2
#define _V_PIE			3
int para_comercio = 1;
/***************************************************************************/
void VALIDAR_REIMPRESION_FACTURA( double comprobante, long cuenta )
/***************************************************************************/
{
    _SELECCION_PAPEL = JOURNAL;
    if( _NRO_REIMPRESION_FACTURA ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.nro_cliente = cuenta;
        valida.comprobante = comprobante;
        strncpy( valida.mensaje, ST( S_REIMPRESION_DE_FACTURA ), sizeof( valida.mensaje ) );
        VALIDAR( _NRO_REIMPRESION_FACTURA, _VALREIMPFAC, 0, 0, 0 );
    }
}
/***************************************************************************/
void VALIDAR_MEDIOS( int modo )
/***************************************************************************/
{
    int h,cod;
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        cod = medios[_PAGO_MODO( h )].cod_valida;

		//verificar c¢digo de validaci¢n al nivel del submedio mutuales
		/*if( cod == 0 && medios[_PAGO_MODO( h )].mutual ) {
			cod = COD_VALIDA_MUTUAL( _PAGO_CUENTA( h ) );
		}*/

		//verificar c¢digo de validaci¢n al nivel del submedio T_COMPRA
		/*if( cod == 0 && medios[_PAGO_MODO( h )].ticket_compra ) {
			cod = COD_VALIDA_T_COMPRA( _PAGO_CUENTA( h ) );
		}*/
		if( cod ) {
            VALIDAR_UN_MEDIO( h, cod, modo );
        }
    }
}
/***************************************************************************/
void VALIDAR_VUELTO( )
/***************************************************************************/
{
    int  cod = 0, tarjeta_con_vuelto = 0;
	int vuelto_medio = 0, vuelto_submedio = 0;
	unsigned aux_fecha;

    _SELECCION_PAPEL = RECEIPT | JOURNAL;
	/*El vuelto es para el ultimo medio ingresado*/
	if( medios[_PAGO_MODO( RAM_P_PAGO - 1 )].tarjeta ) {
		aux_fecha = GET_FECHA_VTO_PRIVADA(  ) ;
		SET_FECHA_VTO_PRIVADA( DECREMENTAR_MES( aux_fecha ));

		cod = COD_VALIDA_VUELTO( _PAGO_MODO( RAM_P_PAGO - 1 )+ 1, _PAGO_TARJETA( RAM_P_PAGO - 1 ) + 1 );
		COD_VUELTO_MEDIO( _PAGO_MODO( RAM_P_PAGO - 1 )+ 1, _PAGO_TARJETA( RAM_P_PAGO - 1 ) + 1, &vuelto_medio, &vuelto_submedio );
		tarjeta_con_vuelto = ( VAR_TARJETAS_VUELTO ) ? 1: 0;
	}
	/*Impresion de voucher de vuelto cuando hay validacion configurada*/
	if( cod && tarjeta_con_vuelto && vuelto_medio == _EFECTIVO  ) {
        VALIDAR_UN_MEDIO( RAM_P_PAGO - 1, cod, _MODO_VENTA );
    }
	if( medios[_PAGO_MODO( RAM_P_PAGO - 1 )].tarjeta ) {
		SET_FECHA_VTO_PRIVADA(  aux_fecha );
	}

}
/***************************************************************************/
void VALIDAR_COBROS_POR_ARTICULO()
/***************************************************************************/
{
    int h,j,cod,anulado = 0;
    long id_evento,cod_articulo;
    double importe;
	int cobro_kinacu = 0;
	int posicionTransaccionKinacu = 0;
	int fin_recorrido = 0;
    if( !MODO_DEVOLUCION
		&& /*USE_DB( AREA_AUX4, _COBROS_SIC, ( char* )&cobros, sizeof( cobros ), NULL, 0, 0 )*/
		OPEN_TABLE(T_COBROS, TT_ORIG,( char* )&cobros, sizeof( cobros ))== 0 ) {
		//---Se busca por articulos-cobros.
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            if( _TICKET_COD_COBRO( h ) && !( _TICKET_TIPO( h ) & _RESTA ) ) {
                cod_articulo = _TICKET_COD_ESPECIAL( h );
                importe = _TICKET_PRECIO( h );
                //---Verificamos que no este anulado.
                for( j = h, anulado = 0;j < RAM_P_TICKET && !anulado;j++ ) {
                    if( cod_articulo == _TICKET_COD_ESPECIAL( j ) && importe == _TICKET_PRECIO( j )
                     && ( _TICKET_TIPO( j ) & _RESTA ) ) {
                        anulado = 1;
                    }
                }
                if( !anulado ) {
                    //SELECT( AREA_AUX4 );
					SELECT_TABLE( T_COBROS, TT_ORIG );
                    cod = _TICKET_COD_COBRO( h );
					SET_WHERE("CODIGO = '%i'",cod);
					RUN_QUERY(NO);
                    //GET_EQUAL( ( char* )&cod );
                    /*---------------- Obtenemos cantidad de renglones ---------------*/
                    //SELECT( AREA_E_TICKET );
					SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
                    id_evento = RAM_ID_EVENTO;
					SET_WHERE("ID_EVENTO = '%ld'",id_evento);
					SET_ORDER2("ID_EVENTO");
					RUN_QUERY(NO);
                    //GET_EQUAL( ( char* )&id_evento );
                    SKIP2( h );
                    /*------------------ Inicializaciones ---------------*/
					//Cobros KINACU
					if( RAM_NRO_PEDIDO_PIN ) {
						fin_recorrido = 0;
						if( cobros.cod_especial ) {
							while( !fin_recorrido && RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&datos_cobro_kinacu,
									sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {
								if( datos_cobro_kinacu.id_transaccion ) {
									fin_recorrido = 1;
                                    //El ultimo parametro se agrego por el merge con Selectos btrieve. Revisar
									VALIDAR_COBROS_KINACU( &datos_cobro_kinacu, 0, posicionTransaccionKinacu - 1 );
								}
								posicionTransaccionKinacu ++;
							}
						}
					} else {
						valida.fecha = GET_FECHA_DOS();
						valida.hora = GET_HORA_DOS();
						valida.importe = event_e_ticket.total;
						valida.nro_cliente = RAM_NRO_CLIENTE;
						valida.numero = NRO_EVENTO - 1;
						valida.cod_medio = 0;
						valida.autorizacion = 0L;
						strncpy( valida.mensaje, cobros.nombre, sizeof( valida.mensaje ) );
						strcpy( valida.telefono, "" );
						strcpy( valida.cuit, "" );
						valida.comprobante = event_e_ticket.comprobante;//Comprobante dejo de ser long para ser double
						TRIM( valida.mensaje );
						VALIDAR( cobros.cod_valida, _VALCOBROS, 0, 0, 0 );
					}
				}
			}
        }
        CLOSE_TABLE( T_COBROS, TT_ORIG );
    }
}
/*************************************************************************************/
void VALIDAR_PAGOS( int cod, double importe, long cuenta, double comprobante, char *mensaje )
/*************************************************************************************/
{
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( cod ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = importe;
        valida.nro_cliente = cuenta;
        valida.cod_medio = 0;
        valida.comprobante = comprobante;
		strncpy( valida.mensaje, mensaje, sizeof( valida.mensaje ) );
        VALIDAR( cod, _VALPAGOS, 0, 0, 0 );
    }
}

/***************************************************************************/
void VALIDAR_COMPROBANTE(  )
/***************************************************************************/
{
    /*------------------ Inicializaciones ---------------*/
	valida.fecha = GET_FECHA_DOS();
	valida.hora = GET_HORA_DOS();
	valida.importe = 0;
	valida.nro_cliente = 0;
	valida.cod_medio = 0;
	strncpy( valida.mensaje, ST( S_DEVOLUCION ), sizeof( valida.mensaje ) );
	//Para una sola validacion configurada
	VALIDAR( COD_VALIDACION_AL_FINAL, 0, 0, 0, 0 );
}

/***************************************************************************/
void VALIDAR_DEVOLUCIONES( double importe )
/***************************************************************************/
{
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( config.val_devoluciones ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = importe;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strncpy( valida.mensaje, ST( S_DEVOLUCION ), sizeof( valida.mensaje ) );
        VALIDAR( config.val_devoluciones, _VALDEVOLUC, 0, 0, 0 );
    }
}
/***************************************************************************/
void VALIDAR_COBROS( int cod, struct _datos_cobro *datos_cobro, char *mensaje,
	int devolucion, int modo )
/***************************************************************************/
{
    int cant_dig = 0;

	_SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( cod ) {
        /*------------------ Inicializaciones ---------------*/
        memset( &valida, 0, sizeof( valida ) );
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = datos_cobro->importe;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strncpy( valida.cuenta, datos_cobro->cuenta, sizeof( valida.cuenta ) );
		valida.cuenta[sizeof( valida.cuenta )- 1] = '\0';
        valida.comprobante = datos_cobro->comprobante;
        strncpy( valida.mensaje, mensaje, sizeof( valida.mensaje ) - 1 );
		if( strlen( datos_cobro->cod_barra ) > 0 ) {
			strncpy( valida.cod_barra, datos_cobro->cod_barra,
				sizeof( datos_cobro->cod_barra ) - 1 );
		}
		strncpy( valida.dato_adicional, datos_cobro->dato_adicional,
			sizeof( datos_cobro->dato_adicional ) );
		/*sprintf( valida.mensaje_largo, "%s", ( devolucion ) ? "  ************ DEVOLUCION ************  DEVOLUCION COBRO" :
			"COBRO" );*/
		_snprintf(valida.mensaje_largo, sizeof(valida.mensaje_largo)-1,"%s", ( devolucion ) ? "  DEVOLUCION COBRO" :"       COBRO" );
		strncpy( valida.mensaje2, datos_cobro->mensaje, sizeof( valida.mensaje2 ) );
        VALIDAR( cod, _VALCOBROS, 0, 0, modo );
    }
}
/***************************************************************************/
void IMPRIMIR_CHEQUE( double importe, int fecha, int cod_valida_cheque )
/***************************************************************************/
{
    char aux[80];
    int nro, tecla = 0;
    nro = ( cod_valida_cheque ) ? cod_valida_cheque : _NRO_IMPRESION_CHEQUE;
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( nro && IMPRESION_OPCIONAL_DEL_CHEQUE ) {
        sprintf( aux, ST( S__S__10_2LF___ENTER__SI ), ST( S_CONFIRMA_LA_IMPRESION_DEL_CHEQUE_DE ),
                 importe );
        MENSAJE_SIN_SONIDO( aux,NO );
        while( tecla != 13 && tecla != 27 ) {
            tecla = GETCH();
        }
        if( tecla == 27 ) {
            nro = 0;
        }
    }
    if( nro ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = fecha;
        valida.hora = GET_HORA_DOS();
        valida.importe = importe;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        //valida.cuenta = 0;
        valida.comprobante = 0;
        strncpy( valida.mensaje, ST( S_CHEQUE ), sizeof( valida.mensaje ) );
        VALIDAR( nro, _VALIMPCHEQ, 0, 0, 0 );
    }
}
/***************************************************************************/
void IMPRIME_CUPON_PROMOCION( int cod_formato, double valor, int pos, int cantidad, int h, char *pto_str, int vigencia )
/***************************************************************************/
{
    _SELECCION_PAPEL = RECEIPT;
    if( cod_formato ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = valor;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        //valida.cuenta = 0;
        valida.comprobante = cantidad;
        valida.mensaje[0] = 0;
        valida.numero = ( long )pos;
        valida.puntero_msg = pto_str;
        valida.vigencia = vigencia; //vigencia de validez del bono/cupon.. eje: 5 dias
        VALIDAR( cod_formato, _VALPROMO, h, 0, 0 );
    }
}
/***************************************************************************/
void VALIDAR_VOUCHER( int cod_formato )
/***************************************************************************/
{
    _SELECCION_PAPEL = 0;         // RECEIPT;
    if( cod_formato ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = 0;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strcpy( valida.mensaje, "" );
        VALIDAR( cod_formato, _VALVOUCHER, 0, 0, 0 );
    }
}

/***************************************************************************/
void VALIDAR_CAMBIO_MEDIO( int cod_formato )
/***************************************************************************/
{
    _SELECCION_PAPEL = 0;         // RECEIPT;
    if( cod_formato ) {
		MENSAJE("IMPRIMIENDO CAMBIO DE MEDIOS...");	
		MOSTRAR_MENSAJE_UBUNTU( 0,"IMPRIMIENDO CAMBIO DE MEDIOS", 2 );

        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = 0;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strcpy( valida.mensaje, "" );
        VALIDAR( cod_formato, _VALCAMBIOMEDIO, 0, 0, _MODO_CAMBIO_MEDIO );
		BORRAR_MENSAJE( );
    }
}
/***************************************************************************/
VALIDAR_VOUCHER_TARJETA( int cod_formato, double importe, int h )
/***************************************************************************/
{
    double total_cuotas;

    _SELECCION_PAPEL = RECEIPT;
    if( cod_formato ) {
		if( IMPRESORA_FISCAL != FISCAL)
			setData( ACTION_SET_HEADER, BEGIN_VOUCHER, 0 );
		else{
			_COPIA_VOUCHER_PRN = 1;
			//OPEN_COPIA_VOUCHER_PRN();LO ABRE AL ABRIR COMPROBANTE FISCAL EN LA VALIDACION
		}
		CANT_VOUCHER++;
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = importe;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strcpy( valida.mensaje, "" );
        if( VAR_TARJETAS_IMPORTE_CUOTA ) {
            importe = ROUND( importe, ENTEROS, DECIMALES );
            total_cuotas = ROUND( ( VAR_TARJETAS_CUOTAS - 1 )
                                * ROUND( VAR_TARJETAS_IMPORTE_CUOTA, ENTEROS, DECIMALES ), ENTEROS,
                                  DECIMALES );
            SET_MEMORY_DOUBLE( __ram_importe, ROUND( importe - total_cuotas, ENTEROS, DECIMALES ) );
        }
        VALIDAR( cod_formato, _VALVOUCHER, h, 0, 0 );
	//setData( ACTION_SET_HEADER, FINISH_VOUCHER, 0 );
	//Se envia LF para que se envie los corte de ticket cuando hay mas de 2 voucher a imprimir porque no lo envia.
	LF( 0 );
	setData( ACTION_PRINT, NULL, 0 );
	if( IMPRESORA_FISCAL == FISCAL){
	//	CLOSE_COPIA_VOUCHER_PRN();LO CIERRA AL CERRAR COMPROBANTE FISCAL EN LA VALIDACION
		_COPIA_VOUCHER_PRN = 0;
	}

	}
}
/***************************************************************************/
void VALIDAR_VOUCHER_MUTUAL( int h, int cod_formato, double importe )
/***************************************************************************/
{
	_SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( cod_formato ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = importe;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strcpy( valida.mensaje, "" );
		//VALIDAR_UN_MEDIO( h, cod, _MODO_VENTA );
        VALIDAR( cod_formato, _VALVOUCHER, h, 0, 0 );
    }
}
/***************************************************************************/
void VALIDAR_VOUCHER_TICKET_COMPRA( int h, int cod_formato, double importe )
/***************************************************************************/
{
	_SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( cod_formato ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = importe;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strcpy( valida.mensaje, "" );
		valida.comprobante = 0;
        VALIDAR( cod_formato, _VALVOUCHER, h, 0, 0 );
    }
}
/***************************************************************************/
void VALIDA_CIERRE_CAJERO( int cod_formato )
/***************************************************************************/
{

    if( cod_formato ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = 0;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        strcpy( valida.mensaje, "" );
        VALIDAR( cod_formato, _VALCAJERO, 0, 0, 0 );
    }
}
/***************************************************************************/
int VALIDAR_CLIENTE_AFINIDAD()
/***************************************************************************/
{
    int rta = 0;
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( RAM_CLIENTE_AFINIDAD && config.val_afinidad ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.nro_cliente = RAM_CLIENTE_AFINIDAD;
        valida.cod_medio = 0;
        //valida.cuenta = RAM_CLIENTE_AFINIDAD;
        valida.comprobante = 0;
        valida.importe = RAM_DIFERENCIA_AFINIDAD;
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
            EXISTE_CLIENTE( RAM_CLIENTE_AFINIDAD );
        }
        if( RAM_CLIENTE_AFINIDAD_ADICIONAL ) {
            VALIDAR( config.val_afinidad_adicional, _VALAFINID, 0, 0 , 0 );
        }
        else {
            VALIDAR( config.val_afinidad, _VALAFINID, 0, 0, 0 );
        }
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
int VALIDAR_LIMITE_DGI()
/***************************************************************************/
{
    int rta = 0;
    double imp;
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    imp = PAGOS_TOTAL_A_PAGAR();
    if( imp >= config.limite && CODIGO_VALIDACION_LIMITE_DGI && !RAM_REPROCESANDO ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = imp;
        valida.nro_cliente = 0;
        valida.cod_medio = 0;
        //valida.cuenta = 0;
        valida.comprobante = 0;
        VALIDAR( CODIGO_VALIDACION_LIMITE_DGI, _VALDGI, 0, 0, 0 );
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
void INIT_VALIDA()
/***************************************************************************/
{
    /*----------- Tomamos los parametros iniciales ---------*/
    if( INICIALIZAR_VALDACIONES ) {
        ind_config = 0;
        while( CONFIG_VALIDA[ind_config].codigo != _VAL_FIN_INICIALIZACION
            && CONFIG_VALIDA[ind_config].codigo != _VAL_FIN ) {
            VALIDA_ejecutar_comando( ( int* )&ind_config, 0, 0, 0, NO );
        }
    }
}
/***************************************************************************/
void VALIDA_ejecutar_comando( int *ind_config, int h, int ind_posicion, int modo, int reimpresion )
/***************************************************************************/
{
    char mensaje[100], desc[20],buff_val[41],dato_adic[35],xx[150],cadaux[200],cadaux2[200], buffer[210];
    INT16 _fila,anio,quedan = 0 ,/*area_ant,*/cod,sub,cod_sub,fecha_dm,codtabla,orden = 0 /*area = 0*/;
	UINT16 codigo;
    double imp = 0.0,puntos = 0.0,importe_total =0.0;
    int i, suma = 0, mod = 0, len = 0, j, offset, tabla_ant = 0, tipo_ant = 0, rta = 0;
    unsigned char cadena[80];
    char digito, c_mod, nro_cuerpo;
    char _234567[8] = {2,3,4,5,6,7,2,3};
    long codigo_verificacion = -1;
    int cod_sol = 0, nro_orden = 0, modalidad, posicion,/* _nro_dato = 0,*/ nroBitmap = 0;
	char cadena_aux[100], cadena_aux2[100];//Seba
	int ind_configTmp =0, cod_var= 0;
	char decimales = DECIMALES;
	int borra_arch_impresion = 0;
	en_reimpresion = reimpresion;
    ( *ind_config )++;
    codigo = ( UINT16 )CONFIG_VALIDA[( *ind_config )].codigo;
    codigo = ( UINT16 )VAL_evaluar_condiciones( ( int )codigo, h );



	if( codigo > _VAL_FIN ){
	 memset( mensaje, 0, sizeof( mensaje ) );
	}


    memset( mensaje, 0, sizeof( mensaje ) );
    switch( codigo ) {
		case _VALIDA_COMIENZO_SI:
            // Chequeo que la formula sea correcta
            _SI_P_FORMULA = CONFIG_VALIDA[( *ind_config )].parametros;
            codigo = ( INT16 )CONFIG_VALIDA[( *ind_config )].codigo;
            codigo = ( INT16 )VAL_evaluar_condiciones( ( int )codigo, h );
            while( CONFIG_VALIDA[( *ind_config )].codigo != _VALIDA_FIN_SI ) {
                if( codigo )
                                // Si la formula es correcta ejecuto hasta [FIN SI]
                {
                    VALIDA_ejecutar_comando( ind_config, h, 0, modo, reimpresion );
                }
                else
                                // si la formula no es correcta, saltea el comando
                {
                    ( *ind_config )++;
                }
            }
            break;
        case _FECHA:
            if( cheque_a_fecha ) {
                cheque_a_fecha = 0;
                MOSTRAR_FECHA( columna, PRN, valida.fecha );
            }
            else {
                MOSTRAR_FECHA( columna, PRN, valida.fecha );
            }
            break;
		case _VAL_MES_ANIO_VENCIMIENTO:
{
			char fecha_vto[30];
			int marca_tarjeta = VAR_TARJETAS_TARJETA;
			memset( fecha_vto, 0, sizeof( fecha_vto ) );
			
			if( TARJ[marca_tarjeta].enmasc_fecha == 's'
				|| TARJ[marca_tarjeta].enmasc_fecha == 'S' ) {
				strcpy( fecha_vto, "**/**" );
				MOSTRAR_CADENA( columna, PRN, fecha_vto );
			} else 
			MOSTRAR_MES_ANIO( columna, PRN, GET_FECHA_VTO_PRIVADA(  )  );
			}

            break;
        case _VALIDA_HORA:
            MOSTRAR_HORA( columna, PRN, valida.hora );
            break;
        case _IMPORTE:
			MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 1, valida.importe );
            break;
		case _IMP_EXT_CASH_BACK:
			{
				double imp_a_extraer = 0.0, imp_ing_tarjetas = 0.0, importe_restante = 0.0;
				for( i = 0;i < RAM_P_PAGO; i++ ) {
					if( !medios[_PAGO_MODO( i )].tarjeta ) {
						imp_ing_medios += _PAGO_IMPORTE( i );
					}
					if( medios[_PAGO_MODO( i )].tarjeta ) {
						cod = COD_VALIDA_VUELTO( _PAGO_MODO( i )+ 1, _PAGO_TARJETA( i ) + 1 );
						/*El vuelto es para el ultimo medio ingresado*/
						if( cod && i == RAM_P_PAGO - 1 ){
							imp_ing_tarjetas += _PAGO_IMPORTE( i );
						} else {
							/*Para tarjetas que no tienen configurada una validacion*/
							imp_ing_medios += _PAGO_IMPORTE( i );
						}
					}
				}
				importe_restante = RAM_IMPORTE_TOTAL - imp_ing_medios;
				imp_a_extraer  = imp_ing_tarjetas - importe_restante;

				MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0, imp_a_extraer );
				imp_a_extraer = 0.0;
			}
			break;
		case _VENTA:
			{
				double imp_vta_tarjetas = 0.0;

				imp_vta_tarjetas = RAM_IMPORTE_TOTAL - imp_ing_medios;
				MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0, imp_vta_tarjetas );
				imp_ing_medios = 0.0;
			}
			break;
        case _NUMERO_CLIENTE:
            MOSTRAR_CLIENTE( columna, PRN, valida.nro_cliente );
		/* del comprob.. ver de cargarloscase _NUMERO_CLIENTE:
            MOSTRAR_CLIENTE( columna, PRN, event_ticket.nro_cliente );
            break;*/

            break;
        case _NOMBRE_MEDIO:
			if( valida.cod_medio ) {
				MOSTRAR_CADENA( columna, PRN, medios[valida.cod_medio].nombre );
			} else {
				//area = SELECTED();
				tabla_ant = SELECTED2();
				tipo_ant = SELECTED_TIPO();
				if( medios[_PAGO_MODO( h )].mutual ) {
                    NOMBRE_TICKET_COMPRA( _PAGO_COD_SUBMEDIO( h ), xx );
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
				else if( medios[_PAGO_MODO( h )].ticket_compra ) {
                    NOMBRE_TICKET_COMPRA( _PAGO_COD_SUBMEDIO( h ), xx );
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
				//SELECT( area );
				SELECT_TABLE( tabla_ant, tipo_ant );
			}
            break;
        case _CAJA:
			MOSTRAR_ENTERO( columna, PRN, ( USAR_NRO_CAJA_DGI )?
					"0000":"000", NUMERO_CAJA( ) );
            break;
        case _CAJERO:
			memset(cadena, 0, sizeof(cadena));
			if( IMPRIMIR_LEGAJO_CAJERO ) {
				_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
				MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
            }
            else {
				_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
				MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
            }
            break;
		case _SUPERVISOR:
			{
		    struct _cajeros sup_cajero;
			int rta;
		    LEE_CAJERO( USUARIO_CLAVE_ANTERIOR(), &sup_cajero );
			rta = ( sup_cajero.tipo == SUPERVISOR ) ? SI : NO;
            if( rta )
                MOSTRAR_ENTERO( columna, PRN, "00000000", USUARIO_CLAVE_ANTERIOR() );
			}
            break;
		case _NOMBRE_SUPERVISOR:
			{
			struct _cajeros sup_cajero;
			int rta;
		    LEE_CAJERO( USUARIO_CLAVE_ANTERIOR(), &sup_cajero );
			rta = ( sup_cajero.tipo == SUPERVISOR ) ? SI : NO;
            if( rta )
				MOSTRAR_CADENA( columna, PRN, sup_cajero.nombre );
			}
			break;
        case _SUCURSAL:
			MOSTRAR_ENTERO( columna, PRN, "000", config.sucursal );
			break;
        case _CUENTA:
			MOSTRAR_CADENA( columna, PRN, valida.cuenta );
            break;
        case _COMPROBANTE:
			//MOSTRAR_ENTERO( columna, PRN, "0000000000", valida.comprobante );
            MOSTRAR_FLOTANTE( columna, PRN, 10, 0, 0, 0, valida.comprobante );
            break;
        case _NOMBRE_COMERCIO:
			MOSTRAR_CADENA( columna, PRN, config.nom_comercio );
            break;
        case _FECHA_CHEQUE:
            MOSTRAR_ENTERO( columna, PRN, "99", DAY( valida.fecha ) );
            columna += 3;
            MOSTRAR_CADENA( columna, PRN, "DE" );
            columna += 3;
            NAME_MON( valida.fecha, mensaje, NO );
            MOSTRAR_CADENA( columna, PRN, mensaje );
            break;
        case _DIA:
            MOSTRAR_ENTERO( columna, PRN, "99", DAY( valida.fecha ) );
            break;
        case _MES:
            MOSTRAR_ENTERO( columna, PRN, "99", MON( valida.fecha ) );
            break;
        case _NOMBRE_MES:
            NAME_MON( valida.fecha, mensaje, NO );
            MOSTRAR_CADENA( columna, PRN, mensaje );
            break;
        case _ANIO:
            //anio = YEAR( valida.fecha ) % 100;
            //MOSTRAR_ENTERO( columna, PRN, "00", anio );
            MOSTRAR_ENTERO( columna, PRN, "0000", YEAR( valida.fecha ) );
            break;
        case _TELEFONO:
            MOSTRAR_CADENA( columna, PRN, valida.telefono );
            break;
        case _TELEFONO_COMPRADOR:
            MOSTRAR_CADENA( columna, PRN, valida.telefono_comprador );
            break;
        case _CUIT:
            MOSTRAR_CADENA( columna, PRN, valida.cuit );
            break;
        case _CUIT_CLIENTE_VERIF:
            strcpy( (char *)cadena, clientes.cuit );
            len = strlen( (char *)cadena );
            /*--- Se invierte la cadena ---*/
            _strrev( cadena );
            /*--- Se multiplican los digitos por [23456723] y se suman ---*/
            for( i = 0;cadena[i] && len - 1;i++ ) {
                digito = cadena[i] - '0';
                suma += ( digito * _234567[i] );
            }
            /*--- Se saca el MOD11 al resultado---*/
            mod = MOD11( suma );
            if( mod == 10 ) {
                c_mod = 'K';
            }
            else if( mod == 11 ) {
                c_mod = '0';
            }
            else {
                c_mod = ( mod + '0' );
            }
            /*--- Se invierte la cadena de nuevo---*/
            _strrev( cadena );
            cadena[len] = c_mod;
            cadena[len + 1] = 0;
            MOSTRAR_CADENA( columna, PRN, cadena );
            break;
        case _NOMBRE_CLIENTE:
            MOSTRAR_CADENA( columna, PRN, clientes.nombre );
            break;
        case _NUMERO_EVENTO:
			memset(cadena, 0, sizeof(cadena));
			if( LIMITE_MAXIMO_NUMERO_DE_EVENTO ) {
				_ltoa( LIMITE_MAXIMO_NUMERO_DE_EVENTO, cadena, 10 );
				MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', strlen( cadena ), "" ), NRO_EVENTO );
			} else {
				_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_EVENTO );
				MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 3, cadena ), NRO_EVENTO );
			}
            break;
        case _NUMERO_TARJETA:
			{	int marca_tarjeta = 0;
				char temp[21], numero_tarjeta[21];
				memset( temp, 0, sizeof( temp ) );
				memset( numero_tarjeta, 0, sizeof( numero_tarjeta ) );
				
				marca_tarjeta = VAR_TARJETAS_TARJETA;
				_snprintf( numero_tarjeta, sizeof( numero_tarjeta ), "%s",GET_NUMERO_TARJETA_PRIVADO( ));

/*
				if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ) {
					if( TARJ[marca_tarjeta].enmasc_nro_tarjeta == 's'
						|| TARJ[marca_tarjeta].enmasc_nro_tarjeta == 'S' )
					{
						strncpy( &numero_tarjeta[2], MASCARA( '*', 6, "" ), 6 );
					}
					strncpy( temp, numero_tarjeta, sizeof( temp ) -1 );
				} else {*/
					ENMASCARA_NUMERO_TARJETA( marca_tarjeta, numero_tarjeta, temp );
				//}
				MOSTRAR_CADENA( columna, PRN, temp );
			break;
			}
		case _VAL_SALDO_TARJETA:
			if( VAR_TARJETAS_STATUS == 2 ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0, VAR_TARJETAS_SALDO_CLIENTE );
            }
            break;
		case _VAL_CUOTA_TARJETA:
			if( VAR_TARJETAS_STATUS == 2 ) {
               MOSTRAR_ENTERO( columna, PRN, "00", VAR_TARJETAS_CUOTAS );
            }
            break;
		case _VALIDA_MENSAJE2:
            MOSTRAR_CADENA( columna, PRN, valida.mensaje2 );
            break;
        case _NUMERO:
            MOSTRAR_ENTERO( columna, PRN, "00000000", valida.numero );
            break;
        case _AUTORIZACION:
            MOSTRAR_ENTERO( columna, PRN, "00000000", valida.autorizacion );
            break;
		case _POSICION:
			{
				int col_aux,fila_aux;
				ind_configTmp = 0;
				col_aux = VALIDA_tomar_entero( ind_config, &ind_configTmp );
				fila_aux = VALIDA_tomar_entero( ind_config, &ind_configTmp );
				if( !imprimiendo_factura_journal_val ) {
						columna = col_aux;
						_fila = fila_aux;
						VALIDA_SET_FILA( _fila );
				}
			}
            break;
		case _POS_X:
        {
            int col_aux;
            ind_configTmp = 0;
            col_aux = VALIDA_tomar_entero( ind_config, &ind_configTmp );
				if( !imprimiendo_factura_journal_val ) {
				columna = col_aux;
				}
        }
        break;
        case _POS_Y:
		{
            int fila_aux;
            ind_configTmp = 0;
            fila_aux =  VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( !imprimiendo_factura_journal_val ) {
				_fila = fila_aux;
                VALIDA_SET_FILA( _fila );
			}
        }
        break;
		case _CORTE_TICKET:
            PRN_CORTE_TOTAL();
            //LINEA_IMPRESA = _SI_CANT_IMPRESION;
            break;
        case _CADENA:
            MOSTRAR_CADENA( columna, PRN, CONFIG_VALIDA[( *ind_config )].parametros );
			break;
		case _VAL_CODIGO_BARRA_FOLIO:
            ARMAR_STRING_FOLIO( (char *)cadena );
            SET_BAR_CODE( (char *)cadena );
            BAR_CODE();
            break;
		case _VAL_CODIGO_FOLIO:
            ARMAR_STRING_FOLIO( (char *)cadena );
            MOSTRAR_CADENA( columna, PRN, (char *)cadena );
            break;
        case _CODIGO_BARRA:
            MOSTRAR_CADENA( columna, PRN, valida.cod_barra );
		/*del comprob .. cargar el cod de barra dnc
		case _CODIGO_BARRA:
            IMPRIMIR_CADENA( columna, event_e_ticket.cod_barra );*/
            break;
        case _DATO_ADICIONAL_COBRO:
            MOSTRAR_CADENA( columna, PRN, valida.dato_adicional );
            break;
		case _CUERPO_MEDIO:
			if( !RAM_FLAG_COBRO_VENTA )
				PAGOS_IMPRIME_PAGO( modo );
				if( RAM_ANULAR_TICKET > 100 )
					FIN_DE_TICKET( NO );
			else
				LF( 1 );

            break;
		case _NOMBRE_COMPROBANTE:
			MOSTRAR_CADENA( columna, PRN, valida.mensaje_largo );
            break;
        case _LETRA_GRANDE:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_DOBLE_ANCHO ;
			else
				PRN_LETRA_GRANDE();
            break;
        case _LETRA_NORMAL:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_NORMAL;
			else
            PRN_LETRA_CHICA();
            break;
		case _LETRA_RESALTADA:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_RESALTADA ;
			break;
		case _LETRA_DOBLE_ALTO:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_DOBLE_ALTO ;
			break;
		case _LETRA_DOBLE_ALTO_NEGRITA:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_DOBLE_ALTO_NEGRITA ;
			break;
		case _LETRA_DOBLE_ANCHO_NEGRITA:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_DOBLE_ANCHO_NEGRITA ;
			break;
		case _LETRA_DOBLE_ANCHO_ALTO:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_DOBLE_ANCHO_ALTO ;
			break;
		case _LETRA_DOBLE_ANCHO_ALTO_NEGRITA:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_DOBLE_ANCHO_ALTO_NEGRITA ;
			break;
		case _LETRA_SUBRAYADO:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_SUBRAYADO ;
            break;
        case _SLIP_ON:
            if( IMPRESORA_FISCAL != FISCAL_IBM_4610 && IMPRESORA_FISCAL != FISCAL_TMT88 ) {
                PRN_HABILITA_SLIP( 1, NO );
                #if defined( INVEL_W ) || defined( INVEL_L )
                LF( 0 );
                setData( ACTION_PRINT, NULL, 0 );
                #endif
            }
            fila = 1;
            break;
        case _SLIP_OFF:
            #if defined( INVEL_W) || defined( INVEL_L)
            LF( 0 );
            setData( ACTION_PRINT, NULL, 0 );
            #endif
            EJECT();
            #if defined( INVEL_W) || defined( INVEL_L)
            LF( 0 );
            setData( ACTION_PRINT, NULL, 0 );
            #endif
            #if !defined(INVEL_W) && !defined(INVEL_L)
            delay( 1000 );
            #endif
            PRN_INHABILITA_SLIP();
            break;
        case _MENSAJE_CON_ESPERA:
            if( !valida.comprobante ) {
                sprintf( mensaje, ST( S__S__S_A_VALIDAR_Y_PRESIONE__ENTER_ ),
                         ST( S_INGRESE_EL_COMPROBANTE_DE ), valida.mensaje );
            }
            else {
                sprintf( mensaje, ST( S__S__S__LI_Y_PRESIONE__ENTER_ ),
                         ST( S_INGRESE_EL_COMPROBANTE_DE ), valida.mensaje, valida.comprobante );
            }
            MENSAJE( mensaje );
            while( GETCH() != 13 );
            break;
        case _MENSAJE_SIN_ESPERA:
            if( !valida.comprobante ) {
                sprintf( mensaje, ST( S__S__S_A_VALIDAR ), ST( S_INGRESE_EL_COMPROBANTE_DE ),
                         valida.mensaje );
            }
            else {
                sprintf( mensaje, ST( S__S__S__LI_A_VALIDAR ), ST( S_INGRESE_EL_COMPROBANTE_DE ),
                         valida.mensaje, valida.comprobante );
            }
            MENSAJE( mensaje );
            break;
        case _MENSAJE_ESPECIAL:
            MENSAJE( CONFIG_VALIDA[( *ind_config )].parametros );
            break;
        case _ESPERAR_ENTER:
            while( GETCH() != 13 );
            break;
        case _IMPORTE_EN_LETRAS:
            VALIDA_importe_en_letras( ind_config );
            break;
        case _CORTE_RECEIPT:
            if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
                PRN_FIN_TICKET();
            }
            break;
        case _IMPRESION_CHEQUE:
            ind_configTmp = 0;
            _NRO_IMPRESION_CHEQUE = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _REIMPRESION_FACTURA:
            ind_configTmp = 0;
            _NRO_REIMPRESION_FACTURA = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _VAL_FIN_INICIALIZACION :
            break;
        case _PORCENTAJE_DEL_TOTAL:
            imp = ( RAM_IMPORTE_TOTAL + RAM_VARIACION ) * VALIDA_tomar_float( ind_config ) / 100;
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0, imp );
            break;
        case _PORCENTAJE_TRUNCADO_DEL_TOTAL:
            imp = floor( ( RAM_IMPORTE_TOTAL + RAM_VARIACION ) * VALIDA_tomar_float( ind_config )
                       / 100 );
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0, imp );
            break;
        case _NRO_TICKET:
			memset(cadena, 0, sizeof(cadena));
			_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_TICKET );
			MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 7, cadena ), NRO_TICKET );
            break;
        case _IMPORTE_CUPON_ENVASES:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0, RAM_CUPON_ENVASES );
            break;
        case _CUENTA_CMR:
        case _NOMBRE_SERVICIO:
			MOSTRAR_CADENA( columna, PRN, valida.mensaje );
            break;
        case _NUMERO_SECUENCIA:
			MOSTRAR_ENTERO( columna, PRN, "000", NRO_EVENTO );
            break;
        case _PUNTAJE_VENTA:
            MOSTRAR_FLOTANTE( columna, PRN, 6, 2, 1, 0, RAM_ACU_PUNTOS );
            break;
        case _NUMERO_CLIENTE_AFINIDAD:
            MOSTRAR_ENTERO( columna, PRN, "00000000", RAM_CLIENTE_AFINIDAD );
            break;
        case _NUMERO_CLI_AFI_ADICIONAL:
            MOSTRAR_ENTERO( columna, PRN, "00000000", RAM_CLIENTE_AFINIDAD_ADICIONAL );
            break;
        case _PUNTAJE_VENTA_TRUNCADO:
            puntos = RAM_ACU_PUNTOS;
            MOSTRAR_FLOTANTE( columna, PRN, 6, 0, 1, 0, puntos );
            break;
        case _NOMBRE_CLIENTE_AFINIDAD:
            MOSTRAR_CADENA( columna, PRN, RAM_NOMBRE_CLIENTE_AFINIDAD );
            break;
        case _NOMBRE_CLI_AFI_ADICIONAL:
            MOSTRAR_CADENA( columna, PRN, RAM_NOM_CLI_AFINIDAD_ADICIONAL );
            break;
        case _FECHA_ACTUALIZACION:
            MOSTRAR_FECHA( columna, PRN, RAM_FECHA_PUNTOS_AFINIDAD );
            break;
        case _PUNTAJE_ACUMULADO:
            if( RAM_PUNTOS_AFINIDAD > 0 ) {
                if( config.pais != CHILE && config.pais != BRASIL ) {
                    MOSTRAR_FLOTANTE( columna, PRN, 6, 2, 1, 0, RAM_PUNTOS_AFINIDAD );
                }
                else {
                    MOSTRAR_FLOTANTE( columna, PRN, 6, 0, 1, 0, RAM_PUNTOS_AFINIDAD );
                }
            }
            break;
        case _SALDO_CHEQUE_CLIENTE:
            MOSTRAR_FLOTANTE( columna, PRN, 6, 2, 1, 0, valida.saldo );
            break;
        case _CABECERA_GENERAL:
            IMPRIME_CABECERA_TICKET( SI );
            break;
        case _CABECERA_CAJERO:
            IMPRIME_CAJERO();
            break;
        case _VALIDA_PIE_TICKET:
            IMPRIME_PIE();
            break;
        case _NRO_BANCO:
            MOSTRAR_ENTERO( columna, PRN, "0000", valida.comprobante );
            break;
        case _NOMBRE_BANCO:
            EXISTE_BANCO( valida.comprobante );
            MOSTRAR_CADENA( columna, PRN, bancos.nom_banco );
            break;
        case _VARIABLE_RNV:
            ind_configTmp = 0;
            cod = ( INT16 )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            sub = ( INT16 )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( sub == -3 ) {
                sub = h;
            }
            strncpy( buff_val, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp], sizeof( buff_val ) - 1 );

            MOSTRAR_VARIABLE_RNV( cod, sub, columna, buff_val );
            break;
        case _MARCA_TARJETA:
				MOSTRAR_CADENA( columna, PRN, TARJ[VAR_TARJETAS_TARJETA].nombre );
            break;
        case _NETO_GRAVADO_PROPORCIONAL:
            ind_configTmp = 0;
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              PAGOS_NETO_PROPORCIONAL( VALIDA_tomar_entero( ind_config, &ind_configTmp ),
                                                       valida.importe ) );
            break;
        case _IVA_PROPORCIONAL:
            ind_configTmp = 0;
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              PAGOS_IVA_PROPORCIONAL( VALIDA_tomar_entero( ind_config, &ind_configTmp ),
                                                      valida.importe ) );
            break;
        case _EXENTO_PROPORCIONAL:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              PAGOS_EXENTO_PROPORCIONAL( valida.importe ) );
            break;
        case _SI_VALOR_VARIABLE_RNV:
        case _SI_NO_VALOR_VARIABLE_RNV:
            ind_configTmp = 0;
            _COMPARACION_IGUAL = ( codigo == _SI_VALOR_VARIABLE_RNV ) ? SI : NO;
            _COD_VARIABLE_RNV = ( INT16 )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            _SUB_VARIABLE_RNV = ( INT16 )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( _SUB_VARIABLE_RNV == -3 ) {
                _SUB_VARIABLE_RNV = h;
            }
            _VALOR_RNV = ( INT16 )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _SI_MEDIO:
            ind_configTmp = 0;
            SI_CODIGO_MEDIO = (char)VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
		case _VAL_SI_HAY_MEDIO:
			{
				int i;
                ind_configTmp = 0;
				for( i = 0; i < 10; i++ ){
					SI_HAY_MEDIO[i] = (char)VALIDA_tomar_entero( ind_config, &ind_configTmp );
					if( !SI_HAY_MEDIO[i] )
						break;
				}
			}
            break;
        case _NOMBRE_CLIENTE_TRACK_I:
			if(config_tps.NapseModalidad == 0) {
				OBTIENE_NOMBRE_CLIENTE( mensaje, h );
				if( strcmp( mensaje, "NO" ) != 0 ) {
				   MOSTRAR_CADENA( columna, PRN, mensaje );
				}
			}else {
				OBTIENE_NOMBRE_CLIENTE_NAPSE(mensaje, h);
				MOSTRAR_CADENA( columna, PRN, mensaje );
			}
            break;
        case _INSTITUCION:
            MOSTRAR_CADENA( columna, PRN, valida.mensaje );
            break;
        case _PUNTOS_CLIENTE:
			{	long cliente_aux = clientes.cod_cliente;
				if( RAM_CLIENTE_AFINIDAD  && clientes.cod_cliente != RAM_CLIENTE_AFINIDAD   ) {			
					if( !EXISTE_CLIENTE( RAM_CLIENTE_AFINIDAD ) ) {
					}
				}
            if( clientes.puntos > 0 ) {
                if( config.pais != CHILE && config.pais != BRASIL ) {
                    MOSTRAR_FLOTANTE( columna, PRN, 6, 2, 1, 0, clientes.puntos );
                }
                else {
                    MOSTRAR_FLOTANTE( columna, PRN, 6, 0, 1, 0, clientes.puntos );
                }
            }
			EXISTE_CLIENTE( cliente_aux ) ;
			}
        case _VAL_ANIO_CUATRO_DIGITOS:
            anio = YEAR( valida.fecha );
            MOSTRAR_ENTERO( columna, PRN, "0000", anio );
            break;
        case _FECHA_CUATRO_DIGITOS:
            MOSTRAR_FECHA_2000( ( char )columna, PRN, valida.fecha );
            break;
        case _NRO_DOC_FISCAL:
            ind_configTmp = 0;
            _NRO_DOCUMENTO_FISCAL = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( !VALIDA_COD_COMANDO() ) {
                _NRO_DOCUMENTO_FISCAL = -1;
            }
            break;
        case _NRO_COM_RETIRO:
            ind_configTmp = 0;
            _NRO_COMANDO_RETIRO = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _NRO_COM_FONDO_FIJO:
            ind_configTmp = 0;
            _NRO_COMANDO_FONDO_FIJO = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _COD_AUTORIZACION:
            MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_AUTORIZACION );
            break;
        case _NRO_COM_INVENTARIO:
            ind_configTmp = 0;
            _NRO_COMANDO_INVENTARIO = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _MENSAJE_CUMPLEANIO:
            if( RAM_HOY_CUMPLE_ANIOS ) {
                MOSTRAR_CADENA( columna, PRN, CONFIG_VALIDA[( *ind_config )].parametros );
            }
            else {
                MOSTRAR_CADENA( columna, PRN, " " );
            }
            break;
        case _LF_RECEIPT:
            ind_configTmp = 0;
            LF( VALIDA_tomar_entero( ind_config, &ind_configTmp ) );
            break;
        case _CODIGO_TOMRA:
            MOSTRAR_ENTERO( columna, PRN, "000000000", aux_envases.cod_interno );
            break;
        case _DESCRIPCION_TOMRA:
			tabla_ant = SELECTED2();
			tipo_ant = SELECTED_TIPO();
			//area_ant = SELECTED();
            memset( desc, 0, 20 );
            //SELECT( GET_AREA_ARTICULOS() );
            //SET_ORDER( 3 );
            //GO( TOP );
            //GET_EQUAL( ( char* )&aux_envases.cod_interno );
			SELECT_TABLE( T_ARTICULO, TT_ORIG );
			SET_WHERE("COD_INTERNO = '%l'",aux_envases.cod_interno);
			RUN_QUERY(NO);
            if( FOUND2() ) {
                strcpy( desc, articulo.nombre );
            }
            desc[20] = 0;
            MOSTRAR_CADENA( columna, PRN, desc );
            //SELECT( area_ant );
			SELECT_TABLE( tabla_ant, tipo_ant );
            break;
        case _CANTIDAD_TOMRA:
            //cant = ( aux_envases.cantidad_vouchedit er - aux_envases.cantidad_facturados );
            //MOSTRAR_ENTERO( columna, PRN, "000", cant );
            MOSTRAR_ENTERO( columna, PRN, "000", aux_envases.cantidad_voucher );
            break;
        case _TIPO_TOMRA:
            MOSTRAR_ENTERO( columna, PRN, "00", ( int )aux_envases.tipo );
            break;
        case _COMIENZO_ENV_TOMRA:
        case _FIN_ENV_TOMRA:
            break;
        case _VAL_SELECT:
            ind_configTmp = 0;
            codigo = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            switch( codigo ) {
                case 1:
                    PRN_SELECT( RECEIPT );
					_SELECCION_PAPEL=RECEIPT;
                break;
                case 2:
                    PRN_SELECT( JOURNAL );
					_SELECCION_PAPEL=JOURNAL;
                    break;
                case 3:
					//PRN_HABILITA_AVANCES();
                    PRN_SELECT( RECEIPT + JOURNAL );
					_SELECCION_PAPEL=RECEIPT | JOURNAL ;
                break;
				case 4:
                    PRN_SELECT( SLIP );
					_SELECCION_PAPEL=SLIP ;
				break;
				case 31:
                    PRN_SELECT( RECEIPT_PART );
				break;
				case 32:
                    PRN_SELECT( JOURNAL_PART );
                break;

            }
            break;
        case _COD_BOLETA_PLATA:
            MOSTRAR_CADENA( columna, PRN, RAM_COD_BOLETA_PLATA );
            /*codigo_verificacion =
                    CALCULA_CODIGO_VERIFICACION( NRO_CAJA, _GET_FECHA_DOS(  ), NRO_TICKET,
                    RAM_COD_BOLETA_PLATA[20], config.sucursal );
                MOSTRAR_ENTERO( columna, PRN, "0000000000", codigo_verificacion );
                */
            break;
        case _CALCULAR_COD_BOLPLATA:
            CALCULAR_CODIGO_VOUCHER_BOLETA_PLATA();
            break;
        case _INSERTAR_BOLETIN:
            ind_configTmp = 0;
            dpago_boleta_plata.cod_solicitud = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            dpago_boleta_plata.posicion = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            dpago_boleta_plata.indice_posicion = ( char )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            dpago_boleta_plata.id_dato = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            dpago_boleta_plata.boletin_consultado = ( char )VALIDA_tomar_entero( ind_config, &ind_configTmp );

            VALIDA_tomar_cadena_compuesta( ind_config, buff_val, h );
            strncpy( dpago_boleta_plata.valor, buff_val, sizeof( dpago_boleta_plata.valor ) );
            VALIDA_tomar_cadena_compuesta( ind_config, buff_val, h );
            dpago_boleta_plata.importe = OBTENER_FLOTANTE_DE_CADENA( buff_val );
            VALIDA_tomar_cadena_compuesta( ind_config, buff_val, h );
            dpago_boleta_plata.fecha_vigencia = ( int )CADENA_A_FECHA( buff_val );
            dpago_boleta_plata.nro_acumulador = ( int )valida.numero;
            dpago_boleta_plata.caja = NRO_CAJA;
            dpago_boleta_plata.nro_z = NRO_Z;
            dpago_boleta_plata.id_evento = RAM_ID_EVENTO;
            dpago_boleta_plata.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
            dpago_boleta_plata.cod_sucursal = config.sucursal;
            dpago_boleta_plata.accion = _GENERAR_BOLETIN;
            INSERTAR_EN_DPAGO( dpago_boleta_plata );
            break;
        case _VALIDA_DATO_MEDIO:
            ind_configTmp = 0;
            if( PAGOS_OBTENER_VALOR_POS( h, VALIDA_tomar_entero( ind_config, &ind_configTmp ), ind_posicion,
                                         mensaje, 0 ) ) {
                MOSTRAR_CADENA( columna, PRN, mensaje );
            }
            break;
        case _VALIDA_DIA_DATO_MEDIO:
        case _VALIDA_MES_DATO_MEDIO:
        case _VALIDA_ANIO_DATO_MEDIO:
            ind_configTmp = 0;
			cod = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( PAGOS_OBTENER_VALOR_POS( h, cod, ind_posicion, mensaje, 0 ) ) {
                fecha_dm = CADENA_A_FECHA( mensaje );
                if( fecha_dm ) {
                    if( codigo == _VALIDA_DIA_DATO_MEDIO ) {
                        sprintf( mensaje, "%i", DAY( fecha_dm ) );
                    }
                    else if( codigo == _VALIDA_MES_DATO_MEDIO ) {
                        NAME_MON( fecha_dm, mensaje, NO );
                    }
                    else {
                        sprintf( mensaje, "%i", YEAR( fecha_dm ) );
                    }
                    MOSTRAR_CADENA( columna, PRN, mensaje );
                }
            }
            break;
        case _VALIDA_REDIRECCIONAR_A_BUFFER:
            ind_configTmp = 0;
            cant_columnas = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            cant_filas = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            REDIRECCION_PRINTER_A_BUFFER( cant_columnas, cant_filas );
            break;
        case _VALIDA_FIN_REDIRECCIONAR_A_BUFFER:
            FIN_REDIRECCION_PRINTER_A_BUFFER();
            break;
        case _VALIDA_IMPRIMIR_COLUMNAS_BUFFER:
            //cant_columnas = VALIDA_tomar_entero( ind_config );
            //cant_filas    = VALIDA_tomar_entero( ind_config );
            ind_configTmp = 0;
            offset = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            mod = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( PRINT_BUFFER ) {
                if( mod == 0 ) {
                    for( i = 0 ;i < cant_columnas ;i++ ) {
                        memset( mensaje, 0, sizeof( mensaje ) );
                        for( j = 0 ;j < cant_filas ;j++ ) {
                            mensaje[cant_filas - j - 1] = PRINT_BUFFER[j * cant_columnas + i];
                            if( mensaje[cant_filas - j - 1] == 0 ) {
                                mensaje[cant_filas - j - 1] = ' ';
                            }
                        }
                        mensaje[j] = 0;
                        MOSTRAR_CADENA( 1 + offset, PRN, mensaje );
                        LF( 1 );
                    }
                }
                else {
                    for( i = cant_columnas - 1 ;i >= 0 ;i++ ) {
                        memset( mensaje, 0, sizeof( mensaje ) );
                        for( j = 0 ;j < cant_filas ;j++ ) {
                            mensaje[cant_filas - j - 1] = PRINT_BUFFER[j * cant_columnas + i];
                            if( mensaje[cant_filas - j - 1] == 0 ) {
                                mensaje[cant_filas - j - 1] = ' ';
                            }
                        }
                        mensaje[j] = 0;
                        MOSTRAR_CADENA( 1 + offset, PRN, mensaje );
                        LF( 1 );
                    }
                }
            }
            break;
            /*   case _VALIDA_BEGIN:
                  break;*/
        case _VALIDA_END:
            _SKIP_BLOCK = 0;
            break;
        case _VALIDA_GRUPO_CUOTA:
            if( VALIDA_OBTENER_GRUPO_CUOTA( mensaje ) ) {
                MOSTRAR_CADENA( columna, PRN, mensaje );
            }
            break;
        case _IMPORTE_SIN_VUELTO:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              valida.importe - VAR_TARJETAS_VUELTO );
            break;
        case _ENVIAR_BMP:
            IMPRIME_BMP_LOGO( CONFIG_VALIDA[( *ind_config )].parametros );
            //mmmt( "pausa" );
            break;
        case _VALIDA_VTO_TARJETA:
            DTOC( DECREMENTAR_MES( GET_FECHA_VTO_PRIVADA(  )  ), mensaje );
            MOSTRAR_CADENA( columna, PRN, &mensaje[3] );
            break;
        case _VALIDA_NOMBRE_CAJERO:
            MOSTRAR_CADENA( columna, PRN, datos_cajero_actual.nombre );
			break;
        case _DATO_ADICIONAL_D_ENTRY:
            ind_configTmp = 0;
            memset( dato_adic, 0, sizeof( dato_adic ) );
            BUSCAR_DATO_ADICIONAL_D_ENTRY( VALIDA_tomar_entero( ind_config, &ind_configTmp ), dato_adic,
                                           sizeof( dato_adic ) );
            MOSTRAR_CADENA( columna, PRN, dato_adic );
            break;
        case _PEDIR_DATO_ADICIONAL:
            ind_configTmp = 0;
            cod_sol = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( !reimpresion ) {
				PAGOS_PEDIR_DATOS_ADICIONALES( 0, ( char )/*_nro_dato++*/( char )indice_pos++, cod_sol, NULL, D_VALIDACIONES,
                                           NULL, 1, NULL, NULL , '\0', '\0');
            }
            break;
        case _PROC_DATOS_ADIC_MEDIO_P:
            ind_configTmp = 0;
            /*cod_sol = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            nro_orden = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            modalidad = VALIDA_tomar_entero( ind_config, &ind_configTmp );   //ver si es antes de confirmar la impresion o despu�
            CARGAR_DATOS_D_IMPRESION( valida.orden_medio, cod_sol, nro_orden, modalidad );*/
            cod_sol = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            nro_orden = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            modalidad = VALIDA_tomar_entero( ind_config, &ind_configTmp );   //ver si es antes de confirmar la impresion o despu�
			borra_arch_impresion = VALIDA_tomar_entero( ind_config, &ind_configTmp );
			CARGAR_DATOS_D_IMPRESION( valida.orden_medio, cod_sol, nro_orden,
					modalidad, borra_arch_impresion, reimpresion);
			/*CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( cod_sol, nro_orden, RAM_ID_ULTIMO_EVENTO,
                                                         modalidad, D_MEDIOS_PRE_IMPORTE, 3,
                                                         0 );*/
            break;
        case _IMPRIMIR_DATO_ADICIONAL:
            ind_configTmp = 0;
            memset( dato_adic, 0, sizeof( dato_adic ) );
            posicion = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( posicion == 254 ) {
                posicion = _VALIDA_FOR_I;
            }
			//Buscamos la posicion que guardo la variable PROC_DATOS_ADIC_MEDIO_P
			if( posicion == 255 ) {
				posicion = INDICE_IMPRESION - 1;
			}
            if( BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( posicion, dato_adic ) ) {
                MOSTRAR_CADENA( columna, PRN, dato_adic );
            }
            break;
        case _VALIDA_FOR:
            ind_configTmp = 0;
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, CONFIG_VALIDA[( *ind_config )].parametros, sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_VALIDA[( *ind_config )].parametros ) + 1;
            _VALIDA_FOR_I = /*( int )*/( INT16 )RESOLVER( cadaux, NULL );
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp] )
            + 1;
            _VALIDA_FOR_LIMITE = /*( int )*/( INT16 )RESOLVER( cadaux, NULL );
            _VALIDA_FOR_IND_CONFIG = *ind_config;
            break;
        case _VALIDA_END_FOR:
            ind_configTmp = 0;
            _VALIDA_FOR_I += VALIDA_tomar_entero( ind_config, &ind_configTmp );// Incremento de I
            if( _VALIDA_FOR_I <= _VALIDA_FOR_LIMITE ) {
                *ind_config = _VALIDA_FOR_IND_CONFIG;
            }
            break;
        case _VALIDA_LF:
            ind_configTmp = 0;
            VALIDA_SET_FILA( fila + VALIDA_tomar_entero( ind_config, &ind_configTmp ) );
            break;
        case _VALIDA_SI_FORMULA:
            _SI_P_FORMULA = CONFIG_VALIDA[( *ind_config )].parametros;
            break;
        case _VALIDA_FORMULA:
            ind_configTmp = 0;
             strncpy( xx, CONFIG_VALIDA[( *ind_config )].parametros, sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_VALIDA[( *ind_config )].parametros ) + 1;

            strncpy( cadaux, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp] ) + 1;

            strncpy( cadaux2, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );

            importe_total = RESOLVER( xx, NULL );
           if( cadaux[0] ) {
                FORMAT_FLOTANTE( cadaux, importe_total, xx );
                MOSTRAR_CADENA( columna, PRN, xx );
            }
            else {
                sprintf( xx, "%f", importe_total );
                RESOLVER_FUNCIONES( cadaux2, xx, CONFIG_VALIDA[( *ind_config )].linea );
                //MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, SI, NO, importe_total );
            }
            break;

		case _VAL_CONSULTA_TABLA_SQL:
			ind_configTmp = 0;

			memset( xx, 0, sizeof( xx ) );
            strncpy( xx, CONFIG_VALIDA[( *ind_config )].parametros, sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_VALIDA[( *ind_config )].parametros ) + 1;

            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp] ) + 1;

			memset( cadaux2, 0, sizeof( cadaux2 ) );
            strncpy( cadaux2, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            RESOLVER_TKT_CONSULTA_SQL(cadaux, cadaux2, xx, CONFIG_VALIDA[( *ind_config )].linea );
            break;

        case _VALIDA_CONSULTA_TABLA:
            ind_configTmp = 0;
            codtabla = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            orden = VALIDA_tomar_entero( ind_config, &ind_configTmp );

               strncpy( xx, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp] )
            + 1;
            strncpy( cadaux, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp] )
            + 1;
            strncpy( cadaux2, &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp] )
            + 1;

            OBTENER_DATO_TABLA( ( int )codtabla, ( int )orden, xx, cadaux, cadaux2 );
            MOSTRAR_CADENA( columna, PRN, cadaux2 );
            break;
        case _LINEA_MENSAJE:
            ind_configTmp = 0;
			nro_cuerpo = ( char ) VALIDA_tomar_entero( ind_config, &ind_configTmp );
            IMPRIMIR_LINEAS_MENSAJE( valida_kinacu_id_transaccion, nro_cuerpo );
			break;
        case _VAL_ID_TRANSAC:
            MOSTRAR_ENTERO( columna, PRN, "00000000", valida_kinacu_id_transaccion );
			break;
		case _COSTO_SERVICIO:
			MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 0,
                valida.costo_servicio );
			break;
		case _CUENTA_LARGA:
			MOSTRAR_CADENA( columna, PRN, valida.cuenta_ext );
			break;
		case _CONFIRMACION:
			MOSTRAR_CADENA( columna, PRN, valida.confirmacion );
			break;
		case _HORA_LARGA:
			memset( cadaux, 0, sizeof( cadaux ) );
			sprintf( cadaux, "%02ld:%02ld:%02ld", valida.hora_larga / 10000,
				( ( valida.hora_larga - ( ( valida.hora_larga / 10000 ) * 10000 ) ) / 100 ),
				( valida.hora_larga - ( ( valida.hora_larga / 100 ) * 100 ) ) );
			MOSTRAR_CADENA( columna, PRN, cadaux );
			break;
		case _LLAVERO:
			MOSTRAR_CADENA( columna, PRN, valida.codigo_llavero );
			break;
		case _FOLIO_PAGATODO:
			MOSTRAR_CADENA( columna, PRN, valida.folio );
			break;
		case _NIP_PAGATODO:
			MOSTRAR_CADENA( columna, PRN, valida.NIP );
			break;
		case _IMAGEN:
			nroBitmap = SET_BITMAP( CONFIG_VALIDA[( *ind_config )].parametros );
			memset(cadena,0,sizeof(cadena));
			sprintf(cadena,"%d",nroBitmap);
			PRINT_BITMAP( cadena );
			break;
        case _VALIDA_CANTIDAD:
            MOSTRAR_ENTERO( columna, PRN, "000", valida.comprobante );
            break;
		 case _CANTIDAD2: //CANTIDAD MASCARA
		//
			
	        strncpy( xx, CONFIG_VALIDA[( *ind_config )].parametros, sizeof( xx ) - 1 );
			 
			 sprintf( cadaux, "%li", valida.comprobante );
             RESOLVER_FUNCIONES( xx,cadaux , CONFIG_VALIDA[( *ind_config )].linea );

            //MOSTRAR_ENTERO( columna, PRN, "000", valida.comprobante );
            break;

        case _VALIDA_STRING_PROMO:
            if(valida.puntero_msg){
                MOSTRAR_CADENA( columna, PRN, valida.puntero_msg );
            }
            break;
		case _SETEA_TOTAL:
			strncpy( xx, CONFIG_VALIDA[( *ind_config )].parametros, sizeof( xx ) - 1 );
            importe_total = RESOLVER( xx, NULL );
			valida.importe = importe_total;
			break;
		case _VIGENCIA:
			 MOSTRAR_ENTERO( columna, PRN, "00", valida.vigencia );
			break;
		case _VIGENCIA_EN_FECHA:
            {
            unsigned int vigencia;
            vigencia = _GET_FECHA_DOS() + valida.vigencia;
            MOSTRAR_ENTERO( columna, PRN, "99", DAY( (short)vigencia ) );
            columna += 3;
            MOSTRAR_CADENA( columna, PRN, " de " );
            columna += 3;
            NAME_MON( vigencia, mensaje, NO );
            MOSTRAR_CADENA( columna, PRN, mensaje );
            columna += 3;
            MOSTRAR_CADENA( columna, PRN, " del " );
            columna += 4;
            MOSTRAR_ENTERO( columna, PRN, "0000", YEAR( vigencia ) );
            }
			break;
		case _VAL_SI_DEVOLUCION:
            SI_DEVOLUCION = 1;
			break;
		case _VAL_DATO_ADICCIONAL_NRO_TARJETA:
            {
				int cant = 0;
				MOSTRAR_CADENA( columna, PRN, ImpDatosAdi.nro_tarjeta );
				cant = strlen( ImpDatosAdi.nro_tarjeta );
				memset( ImpDatosAdi.nro_tarjeta, ' ', cant);
			}
			break;
		case _VAL_DATO_ADICCIONAL_FECHA_VENCIM:
			{
				int cant = 0;
				MOSTRAR_CADENA( columna, PRN, ImpDatosAdi.fecha );
				cant = strlen( ImpDatosAdi.fecha );
				memset( ImpDatosAdi.fecha, ' ', cant);
			}
			break;
		case _VAL_DATO_ADICCIONAL_AUTORIZACION:
			{
				int cant = 0;
				MOSTRAR_CADENA( columna, PRN, ImpDatosAdi.autorizacion );
				cant = strlen( ImpDatosAdi.autorizacion );
				memset( ImpDatosAdi.autorizacion, ' ', cant);
			}
			break;
		case _VAL_DATO_ADICCIONAL_NRO_CUENTA:
			{
				int cant = 0;
				MOSTRAR_CADENA( columna, PRN, ImpDatosAdi.nro_cuenta );
				cant = strlen( ImpDatosAdi.nro_cuenta );
				memset( ImpDatosAdi.nro_cuenta, ' ', cant);
			}
			break;
        case _VAL_INTERLINEADO:
            {
                char str_tmp[10];
                ind_configTmp = 0;
                _snprintf( str_tmp, sizeof( str_tmp ), "%i", VALIDA_tomar_entero( ind_config, &ind_configTmp ) );
                setData( ACTION_SET_PRINT_LINE_SPACE, str_tmp, sizeof( str_tmp ) );
            }
            break;
        case _VAL_CARACTERES_LINEA:
            {
                char str_tmp[10];
				ind_configTmp = 0;
                _snprintf( str_tmp, sizeof( str_tmp ), "%i", VALIDA_tomar_entero( ind_config, &ind_configTmp ) );
                setData( ACTION_SET_PRINT_LINE_CHARS, str_tmp, sizeof( str_tmp ) );
            }
            break;

		case _IMPORTE_MEDIO:
			MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 1,
			ROUND( _PAGO_IMPORTE( h ), ENTEROS, DECIMALES ) );
			break;

		case _IMPORTE_DESCUENTO_TARJETA:
			MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 1,
			ROUND( _PAGO_IMPORTE_DESCUENTO( h ), ENTEROS, DECIMALES ) );
			break;

		case _IMPORTE_ORIGINAL_MEDIO:
			{
			double importe_original_medio =0.0;
			if(  VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION_CASHPLUS  ) 
				importe_original_medio= ROUND( _PAGO_IMPORTE( h ) + _PAGO_IMPORTE_DESCUENTO( h ), ENTEROS, DECIMALES ) - VAR_TARJETAS_ANTICIPO;
			else
				importe_original_medio= ROUND( _PAGO_IMPORTE( h ) + _PAGO_IMPORTE_DESCUENTO( h ), ENTEROS, DECIMALES ) ;

			MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 1,ROUND(importe_original_medio, ENTEROS, DECIMALES ) );
			}
			break;
		case _SI_DESCUENTO_TARJETA:
		case _SI_NO_DESCUENTO_TARJETA:
			_COMPARACION_IGUAL = ( codigo == _SI_DESCUENTO_TARJETA ) ? SI : NO;
			SI_DESCUENTO_TARJETA = 1;
			break;
		//-----------Documentos NO Fiscales
		case _ABRIR_DOC_NO_FISCAL:				//Seba
		case _ABRIR_DOC_NO_FISCAL_2:				//Seba
				ABRIR_DOCUMENTO_NO_FISCAL('a');
				if(HABILITA_COPIA_VOUCHER_PRN && _COPIA_VOUCHER_PRN ){//HACE FALTA _COPIA_VOUCHER_PRN
					OPEN_COPIA_VOUCHER_PRN();
				}
				break;
		case _CERRAR_DOC_NO_FISCAL_2:
		case _CERRAR_DOC_NO_FISCAL:				//Seba
				CERRAR_DOCUMENTO_NO_FISCAL();
				if( GET_HANDLE_VOUCHER ()){
					CLOSE_COPIA_VOUCHER_PRN();
				}
				break;
		case _VAL_CADENA_NO_FISCAL:	//Seba
				{
				ind_configTmp = 0;
				memset(cadena_aux,0,sizeof(cadena_aux));
				memset(cadena_aux2,0,sizeof(cadena_aux2));
				TIPO_DE_LETRA= VALIDA_tomar_entero( ind_config, &ind_configTmp );
				_snprintf(cadena_aux2, sizeof(cadena_aux2)-1,&CONFIG_VALIDA[( *ind_config )]);
				IMPRIMIR_CADENA( columna, cadena_aux2 );
				}
				break;
		case _LETRA_FACTURA:
            IMPRIMIR_CADENA( columna, ( ES_FACTURA_A( valida.cond_cli ) ) ? "A" : "B" );
            break;
        case _TIPO_COMPROBANTE:
            IMPRIMIR_CADENA( columna, TIPO_COMP_VAL[valida.tipo_comp] );
            break;
		case _COMPROBANTE_NUMERO:
        case _NUMERO_COMPROBANTE:
            MOSTRAR_ENTERO( columna, PRN, "0000", valida.nro_suc );
            IMPRIMIR_CADENA( columna + 4, "-" );
            MOSTRAR_ENTERO( columna + 5, PRN, "00000000", nro_comp );
            break;
        case _NRO_ORDEN_COMPROBANTE:
            IMPRIMIR_CADENA( columna, ORDEN[orden] );
            break;
        // del comprob case _FECHA:            MOSTRAR_FECHA( columna, PRN, valida.fecha );            break;
        case _HORATKT:
            MOSTRAR_HORA( columna, PRN, valida.hora );
            break;
        case _NOMBRE_SUCURSAL:
			MOSTRAR_ENTERO( columna, PRN, "000", config.sucursal );
            break;
        case _SECUENCIA:
            MOSTRAR_ENTERO( columna, PRN, ( config.pais != ECUADOR ) ? "000000000" : "0000000",
                            ( config.pais == ELSALVADOR && RAM_FACTURA ) ? NRO_EVENTO - 1
                          : NRO_EVENTO - 1 );
            break;
        //del comprob - case _NOMBRE_CLIENTE:            IMPRIMIR_CADENA( columna, clientes.nombre/*CLIENTE_AFINIDAD_NOMBRE*/ );            break;
        case _DOMICILIO_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.domicilio );
            break;
        case _LOCALIDAD_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.localidad );
            break;
        case _PROVINCIA_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.provincia );
            break;
        case _TIPO_IVA_CLIENTE:
            {
                char cond_iva[31];

                memset( cond_iva, 0, sizeof( cond_iva ) );

		        if( OBTENER_DESCRIPCION_COND_IVA( cond_iva, clientes.cond_iva, sizeof( cond_iva ) ) ) {
                    TRIM( cond_iva );
                    IMPRIMIR_CADENA( columna, cond_iva );
                }
            }
            //IMPRIMIR_CADENA( columna, TIPOS_IVA[valida.cond_cli - 1] );
            break;
        case _CUIT_CLIENTE:
            IMPRIMIR_CADENA( columna, FORMATEA_CUIT( clientes.cuit ) );
            break;
        case _CONDICION_VENTA:
            if( config.pais == ELSALVADOR ) {
                //SELECT( AREA_PAGO );
                //GET_EQUAL( ( char* )&event_ticket.id_evento );
				SELECT_TABLE( T_PAGOS, tipo_t_pagos );
				SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
				SET_ORDER2("ID_EVENTO");
				RUN_QUERY(NO);
                if( FOUND2() ) {
                    int credito = 0;
                    for( i = 0;i < val_cant_pagos;i++ ) {
                        if( medios[event_pago.modo_pago - 1].credito ) {
                            credito = 1;
                        }
                        SKIP2( 1 );
                    }
                    if( credito ) {
                        IMPRIMIR_CADENA( columna, "CREDITO" );
                    }
                    else {
                        IMPRIMIR_CADENA( columna, "CONTADO" );
                    }
                    // rox -- esto se hizo exclusivamente para El Salvador porque lo necesitaban asi..
                    // lo que sigue es la funcionalidad como estaba anteriormente.
                }
            }
            else {
                if( !RAM_NOTA_CR || !MEDIO_PAGO_NC ) {
                    buffer[0] = 0;
                    quedan = sizeof( buffer ) - 1;
                    //SELECT( AREA_PAGO );
                    //GET_EQUAL( ( char* )&event_ticket.id_evento );
					SELECT_TABLE( T_PAGOS, tipo_t_pagos );
					SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
					SET_ORDER2("ID_EVENTO");
					RUN_QUERY(NO);
                    for( i = 0;i < val_cant_pagos;i++ ) {
                        if( medios[event_pago.modo_pago - 1].tarjeta ) {
                            if( quedan > 0 ) {
                                strncat( buffer, NOMBRE_TARJETA( event_pago.tarjeta ), quedan );
                            }
                            quedan -= strlen( NOMBRE_TARJETA( event_pago.tarjeta ) );
                            TRIM( buffer );
                            if( quedan > 0 ) {
                                strncat( buffer, " CUPON: ", quedan );
                            }
                            quedan -= 8;
                            if( quedan > 9 ) {
                                sprintf( &buffer[strlen( buffer )], "%09li", event_pago.numero_id );
                            }
                            quedan -= 9;
                        }
                        else {
                            if( quedan > 0 ) {
                                strncat( buffer, medios[event_pago.modo_pago - 1].nombre, quedan );
                            }
                            quedan -= strlen( medios[event_pago.modo_pago - 1].nombre );
                            TRIM( buffer );
                        }
						memset(cadena_aux,0,sizeof(cadena_aux));
                        FTOC( signo_nc * event_pago.importe, cadena_aux, ENTEROS, DECIMALES, 0, 0 );
                        if( quedan > ENTEROS + DECIMALES + 3 ) {
                            IMPRIMIR_CADENA( columna, buffer );
							columna = 35;
                            IMPRIMIR_CADENA( columna, cadena_aux );
                            /*strcat( buffer, " " );
                            strcat( buffer, aux );
                            strcat( buffer, " " );*/
                            quedan -= ENTEROS + DECIMALES + 3;
                            columna = 1;
                            COMP_SET_FILA( fila + 1 );
                            memset( buffer, 0, sizeof( buffer ) );
                        }
                        SKIP2( 1 );
                    }
                    //IMPRIMIR_CADENA( columna, buffer );
                    //SELECT( AREA_E_TICKET );
					SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
                }
            }
            break;
        case _CODIGO_BARRA_12:
            memcpy( buffer, event_e_ticket.cod_barra, 12 );
            buffer[12] = 0;
            IMPRIMIR_CADENA( columna, buffer );
            break;
        case _CODIGO_ARTICULO:
            MOSTRAR_ENTERO( columna, PRN, "000000000", event_e_ticket.cod_articulo );
            break;
        case _DESC_ARTICULO:
            IMPRIMIR_CADENA( columna, articulo.nombre );
            break;
		case _IVA_ARTICULO:
			{
			float cod_iva = 0.0 ;
				if( art_fact_val[renglon].exento || articulo.gravado == 'N'
				 || ( articulo.gravado == 'E' && !EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO() ) ) ) {
					cod_iva = 0;
				}
				else {
					cod_iva = iva[art_fact_val[renglon].cod_iva].porc_iva1;
				}
				MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, cod_iva );
			}
            break;
        case _PRECIO_ARTICULO:
            //fabiana 15-02-05 ** nuevo manejo de redondeos-Se redondea a 4 decimales y no es configurable
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, decimales, 1, 0, precio );
            break;
        case _IMPORTE_ARTICULO:
            //fabiana 15-02-05 ** nuevo manejo de redondeos- el importe del
            //articulo cuando es factura a, se redondea por cdigo a los decimales configurables.
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, importe );
            break;
        case _PRECIO_FINAL:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, signo_nc * precio_final );
            break;
        case _SUBTOTAL:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                ( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) ? neto + iva1 : signo_nc * subtotal );
            break;
        case _NETO_GRAVADO:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, ( signo_nc * neto ) );
            break;
        case _EXENTO:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( exento ) ? ( signo_nc * exento ) : exento ) );
            break;
        case _IVA1:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( iva1 ) ? ( signo_nc * iva1 ) : iva1 ) );
            break;
        case _IVA2:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( iva2 ) ? ( signo_nc * iva2 ) : iva2 ) );
            break;
        case _ILA:
            //MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, ( fabs( imp[0] ) ? ( signo_nc * imp[0] ) : imp[0] ) );
            break;
        case _IHA:
            //MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, ( fabs( iha ) ? ( signo_nc * iha ) : iha ) );
            break;
        case _GIRO_CLIENTE:
            IMPRIMIR_CADENA( columna, clientes.giro );
            break;
        case _NRI_CLIENTE:
            IMPRIMIR_CADENA( columna, FORMATEA_NRI( clientes.nri ) );
            break;
        case _OC_GD_CLIENTE:
            IMPRIMIR_CADENA( columna, RAM_OC_GD );
            break;
        case _TOTAL:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0, signo_nc * total );
            break;
        case _PERCEP_212_IVA1:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( percep_212_iva1 ) ? ( signo_nc * percep_212_iva1 )
                              : percep_212_iva1 ) );
            break;
        case _PERCEP_212_IVA2:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( percep_212_iva2 ) ? ( signo_nc * percep_212_iva2 )
                              : percep_212_iva2 ) );
            break;
        case _LUGAR_DE_EMISION:
            IMPRIMIR_CADENA( columna, valida.emision );
            break;
        case _CANTIDAD_ARTICULO:
            MOSTRAR_FLOTANTE( columna, PRN, 3, 3, 1, 0, cantidad );
            break;
        case _CANTIDAD_SIN_DECIMALES:
            if( cantidad < 1 ) {
                cantidad = 1;
            }
            MOSTRAR_FLOTANTE( columna, PRN, 3, 0, 1, 0, cantidad );
            break;
        case _CONTENIDO_ARTICULO:
            IMPRIMIR_CADENA( columna, articulo.contenido );
            break;
        case _UNIDAD_ARTICULO:
            IMPRIMIR_CADENA( columna, articulo.unidad );
            break;
        case _IMPUESTO_INTERNO:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( imp_int ) ? ( signo_nc * imp_int ) : imp_int ) );
            break;
        case _TASA_IVA1:
            // MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, config.porc_iva1 );
            MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, iva[INDICE_IVA_ARTICULO()].porc_iva1 );
            break;
        case _TASA_IVA2:
            // MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, config.porc_iva2 );
            MOSTRAR_FLOTANTE( columna, PRN, 2, 2, 0, 0, iva[INDICE_IVA_ARTICULO()].porc_iva2 );
            break;
        case _LARGO_FACTURA:
        case _LARGO_FACTURA_A:
        case _LARGO_FACTURA_B:
        case _LARGO_REMITO:
            ind_configTmp = 0;
            LARGO_COMPROBANTE[_tipo] = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _RENGLONES_CUERPO:
        case _RENGLONES_CUERPO_A:
            ind_configTmp = 0;
            CANT_RENGLONES[_FACTURA] = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _RENGLONES_CUERPO_B:
            ind_configTmp = 0;
            CANT_RENGLONES[_NOTA_CREDITO] = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _NRO_DOC_FISCAL_COMP:
            ind_configTmp = 0;
            NRO_DOC_FISCAL_COMP = COMP_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _LETRA_DOBLE_ANCHO:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_DOBLE_ANCHO ;
			else
            PRN_LETRA_GRANDE();
            break;
		case _TKT_FIN_INICIALIZACION:
            break;
        case _INDEPENDIENTES:
            COMP_INDEP = 1;
            break;
        case _CADENA_CONTINUA_EN_PROXIMA:
            ind_configTmp = 0;
            COLUMNA_CONT = VALIDA_tomar_entero( ind_config, &ind_configTmp );//COMP_tomar_entero( ind_config, &ind_configTmp );
            FILA_CONT = VALIDA_tomar_entero( ind_config, &ind_configTmp );//COMP_tomar_entero( ind_config, &ind_configTmp );
			P_CAD_CONT_VAL = &CONFIG_VALIDA[( *ind_config )].parametros[ind_configTmp];
			break;
        case _AGREGAR_IVA_NO_INSCRIPTO:
            AGREGAR_IVA_NO_INSC = 1;
            break;
        case _SI_FACTURA_A:
            SI_FACTURA_A = 1;
            break;
        case _SI_FACTURA_B:
            SI_FACTURA_B = 1;
            break;
        case _LARGO_CADENA:
            ind_configTmp = 0;
            LARGO_CADENA = (char)VALIDA_tomar_entero( ind_config, &ind_configTmp );;
            break;
        case _TOTAL_EN_LETRAS:
			VALIDA_importe_en_letras( ind_config );
            //COMP_total_en_letras( ind_config );
            break;
        case _REIMPRESION_COMPROBANTE:
            if( REIMPRESION ) {
                IMPRIMIR_CADENA( columna, ST( S_REIMPRESION ) );
            }
            break;
        case _TOTAL_IMPUESTO:
            ind_configTmp = 0;
            codigo = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( codigo <= 20 ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  ( fabs( total_impuestos[codigo - 1] )
                                  ? ( signo_nc * total_impuestos[codigo - 1] )
                                  : total_impuestos[codigo - 1] ) );
            }
            break;
        case _TOTAL_IMPUESTO_FACTURA:
            total_impuestos_factura = 0.0;
            for( i = 0;i <= 19;i++ )
                                                                     //total_impuestos_factura += total_impuestos[i];
            {
                total_impuestos_factura += ROUND( total_impuestos[i], ENTEROS, DECIMALES );
            }
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( total_impuestos_factura )
                              ? ( signo_nc * total_impuestos_factura )
                              : total_impuestos_factura ) );
            break;
        case _EJECT:
            EJECT();
            break;
        case _CANTIDAD_PRESENTACION:
            MOSTRAR_ENTERO( columna, PRN, "999", event_e_ticket.cant_presentacion );
            break;
        case _CODIGO_PRESENTACION:
            MOSTRAR_ENTERO( columna, PRN, "9", event_e_ticket.cod_presentacion );
            break;
        case _SUC_REMITO_A_FACTURAR:
            MOSTRAR_ENTERO( columna, PRN, "0000", event_e_ticket.cant_presentacion );
            break;
        case _NRO_REMITO_A_FACTURAR:
            MOSTRAR_ENTERO( columna, PRN, "000000000", event_e_ticket.cod_articulo );
            break;
        case _NRO_VENDEDOR:
            MOSTRAR_ENTERO( columna, PRN, "00000", event_ticket.nro_vendedorl );
            break;
        case _NUMERO_CAJA:
            MOSTRAR_ENTERO( columna, PRN, "000", NRO_CAJA );
            break;
        case _NUMERO_CAJERO:
			memset(cadena, 0, sizeof(cadena));
            if( IMPRIMIR_LEGAJO_CAJERO ) {
				_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
				MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
            }
            else {
				_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
                MOSTRAR_ENTERO( columna, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
            }
            break;
        case _RETENCION_DGR:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( retencion_dgr ) ? ( signo_nc * retencion_dgr )
                              : retencion_dgr ) );
            break;
        case _PERCEPCION_DGI:
            MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                              ( fabs( percepcion_dgi ) ? ( signo_nc * percepcion_dgi )
                              : percepcion_dgi ) );
            break;
        case _NRO_BOLETA_ANTERIOR:
            MOSTRAR_ENTERO( columna, PRN, "00000000", RAM_BOLETA_ANTERIOR );
            break;
        case _NRO_EVENTO:
            MOSTRAR_ENTERO( columna, PRN, "00000000", NRO_EVENTO - 1 );
            break;
        case _MENSAJE_TARJETA:
            if( IMPRIME_MENSAJE_TARJETA( VAR_TARJETAS_TARJETA ) && _VAR_TARJETAS_MENSAJE( 0 ) ) {
				IMPRIMIR_MENSAJES_PROMOCIONALES( para_comercio);
				if( para_comercio == 1)
					para_comercio = 0;
				else
					para_comercio = 1;
				/*char mensaje1[41];
				memset(mensaje1,0,sizeof(mensaje1));
				_snprintf(mensaje1,sizeof( mensaje1) -1, VAR_TARJETAS_MENSAJE);
				if( mensaje1[0] == '.'){ //se debe imprimir
                MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_MENSAJE );
				}*/
            }
            break;
        case _VIGENCIA_TARJETA:
            if( VAR_TARJETAS_COD_VIGENCIA ) {
                if( BUSCAR_VIGENCIA( VAR_TARJETAS_TARJETA, VAR_TARJETAS_COD_VIGENCIA, buffer ) ) {
                    MOSTRAR_CADENA( columna, PRN, buffer );
                }
            }
            break;
        case _PRIMER_VTO_TARJETA:
            if( VAR_TARJETAS_PRIMER_VTO ) {
                MOSTRAR_FECHA( columna, PRN, VAR_TARJETAS_PRIMER_VTO );
            }
            break;
        case _CANT_CUOTAS_TARJETA:
            if( VAR_TARJETAS_CUOTAS ) {
                MOSTRAR_ENTERO( columna, PRN, "00", VAR_TARJETAS_CUOTAS );
            }
            break;
        case _VALIDACION_TARJETA:
            if( VAR_TARJETAS_STATUS == 2 ) {
                VALIDAR_VOUCHER( COD_VALIDACION_VOUCHER( VAR_TARJETAS_TARJETA ) );
            }
            break;
        case _HOJA_ACTUAL:
            MOSTRAR_ENTERO( columna, PRN, "99", _hoja_actual );
            break;
        case _TOTAL_HOJAS_COMPROBANTE:
            MOSTRAR_ENTERO( columna, PRN, "99", valida.cant_formularios );
            break;
        case _PEDIR_FORMULARIO_CON_TECLA:
            PEDIR_FORMULARIO_CON_TECLA = 1;
            break;
        case _NETO_GRAVADO_COD:
            ind_configTmp = 0;
            codigo = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( art_fact_iva != NULL && codigo < MAX_COD_IVA ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  signo_nc * art_fact_iva[codigo].neto_gravado );
            }
            else {
                MOSTRAR_CADENA( columna, PRN, ST( S_____PARAM_INVALIDO____ ) );
            }
            break;
        case _IVA1_COD:
            ind_configTmp = 0;
            codigo = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( art_fact_iva != NULL && codigo < MAX_COD_IVA ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  art_fact_iva[codigo].iva1 );
            }
            else {
                MOSTRAR_CADENA( columna, PRN, ST( S_____PARAM_INVALIDO____ ) );
            }
            break;
        case _IVA2_COD:
            ind_configTmp = 0;
            codigo = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( art_fact_iva != NULL && codigo < MAX_COD_IVA ) {
                MOSTRAR_FLOTANTE( columna, PRN, ENTEROS + 1, DECIMALES, 1, 0,
                                  art_fact_iva[codigo].iva2 );
            }
            else {
                MOSTRAR_CADENA( columna, PRN, ST( S_____PARAM_INVALIDO____ ) );
            }
            break;
        case _NC_DATOS_CAJA:
            MOSTRAR_ENTERO( columna, PRN, "999", RAM_CAJA_ANTERIOR );
			if(IMPRESORA_FISCAL == FISCAL)
			{
				memset( cadaux, 0, sizeof( cadaux ) );
				_snprintf(cadaux,sizeof(cadaux),"Caja: %d",RAM_CAJA_ANTERIOR);
				 rta = IMPRIME_MENSAJE_FISCAL(cadaux);
			}
            break;
        case _NC_DATOS_SUCURSAL:
            MOSTRAR_ENTERO( columna, PRN, "9999", RAM_SUCURSAL_ANTERIOR );
			if(IMPRESORA_FISCAL == FISCAL)
			{
				memset( cadaux, 0, sizeof( cadaux ) );
				_snprintf(cadaux,sizeof(cadaux), "Sucursal: %d",RAM_SUCURSAL_ANTERIOR);
				rta = IMPRIME_MENSAJE_FISCAL(cadaux);
			}
            break;
        case _NC_DATOS_COMPROBANTE:
            MOSTRAR_ENTERO( columna, PRN, "000000000", /*RAM_BOLETA_ANTERIOR*/_secuencia_ant );
			if(IMPRESORA_FISCAL == FISCAL)
			{
				memset( cadaux, 0, sizeof( cadaux ) );
				_snprintf(cadaux,sizeof(cadaux),"Comprobante: %d",RAM_BOLETA_ANTERIOR);
				rta = IMPRIME_MENSAJE_FISCAL(cadaux);
			}
            break;
        case _NC_DATOS_FECHA:
            MOSTRAR_FECHA( columna, PRN, RAM_FECHA_ANTERIOR );
			if(IMPRESORA_FISCAL == FISCAL)
			{
				char fecha[20];
				memset( cadaux, 0, sizeof( cadaux ) );
                DTOC( RAM_FECHA_ANTERIOR, cadaux );
				_snprintf(fecha, sizeof(fecha),"Fecha Anterior: %s",cadaux);
				rta = IMPRIME_MENSAJE_FISCAL(fecha);
			}
            break;
            #define NC_ORIGEN_TICKET 1
            #define NC_ORIGEN_FACT_A 2
            #define NC_ORIGEN_FACT_B 3
        case _NC_DATOS_TIPO_COMP:
            switch( RAM_TIPO_COMP_ANTERIOR ) {
                case NC_ORIGEN_TICKET:
                    MOSTRAR_CADENA( columna, PRN, ST( S_TICKET ) );
					if(IMPRESORA_FISCAL == FISCAL)
						IMPRIME_MENSAJE_FISCAL(ST( S_TICKET ));
                    break;
                case NC_ORIGEN_FACT_A:
                    MOSTRAR_CADENA( columna, PRN, ST( S_FACTURA_A ) );
					if(IMPRESORA_FISCAL == FISCAL)
						IMPRIME_MENSAJE_FISCAL(ST( S_FACTURA_A ));
                    break;
                case NC_ORIGEN_FACT_B:
                    MOSTRAR_CADENA( columna, PRN, ST( S_FACTURA_B ) );
					if(IMPRESORA_FISCAL == FISCAL)
						IMPRIME_MENSAJE_FISCAL(ST( S_FACTURA_B ));
                    break;
            }
            break;
        case _NC_DATOS_AUTORIZACION:
            {
                struct _ev_medios ev_pago;
                //if( USE_DATABASE( AREA_BOTON, EVM_TMP, ( char* )&ev_pago,
				//sizeof( struct _ev_medios ), NULL, 0, 0 ) == 0 ) {
				if( OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&ev_pago, sizeof( struct _ev_medios )) == 0 ) {
                    //SELECT( AREA_BOTON );
                    //SET_ORDER( 1 );
                    //GO( TOP );
					SELECT_TABLE( T_EVM_TMP, TT_ORIG );
					SET_WHERE("");
					RUN_QUERY(NO);
					GO2( TOP );
                    while( /*!BtrvEOF( )*/ !dbEOF( ) ) {
                        if( ev_pago.tarjeta ) {
                            MOSTRAR_CADENA( columna, PRN, NOMBRE_TARJETA( ev_pago.tarjeta ) );
                            MOSTRAR_CADENA( columna, PRN, " " );
                            MOSTRAR_CADENA( columna, PRN, ev_pago.auto_tarjeta );
                            LF( 1 );
                        }
                        SKIP2( 1 );
                    }
					CLOSE_TABLE( T_EVM_TMP, TT_ORIG );
                    //CLOSE_DATABASE( AREA_BOTON );
                }
            }
            break;
        case _SI_NOTA_CREDITO:
            SI_NOTA_CREDITO = 1;
            break;
        case _SI_BIEN_DE_USO:
            SI_BIEN_DE_USO = 1;
            break;
        case _SI_EXENTO:
            SI_EXENTO = 1;
            HAY_EXENTO = 0;
			articulo_exento = NO;
            if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
                if( art_fact_val[renglon].exento && !EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO() )
                 || EXENTO_PARA_DIPLOMATICOS() ) {
                    HAY_EXENTO = 1;
					articulo_exento = SI;
                }
            }
            else {
                if( articulo.gravado == 'N'
                 || ( articulo.gravado == 'E' && !EXENTO_COMO_GRAVADO( 2 ) ) ) {
                    HAY_EXENTO = 1;
					articulo_exento = SI;
                }
            }
            break;
        case _SI_GRAVADO:
            SI_GRAVADO = 1;
            HAY_GRAVADO = 0;
			articulo_gravado = NO;
            if( !FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
                if( !( art_fact_val[renglon].exento && !EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO() )
                    || EXENTO_PARA_DIPLOMATICOS() ) ) {
                    HAY_GRAVADO = 1;
					articulo_gravado = SI;
                }
            }
            else {
                if( ( articulo.gravado == 'E' && EXENTO_COMO_GRAVADO( 2 ) )
                 || articulo.gravado == 'S' ) {
                    HAY_GRAVADO = 1;
					articulo_gravado = SI;
                }
            }
            break;
        case _TKT_LF:
            {
                int fila_aux;
                ind_configTmp = 0;
                fila_aux = COMP_tomar_entero( ind_config, &ind_configTmp );
                if( !imprimiendo_factura_journal_val ) {
                    COMP_SET_FILA( fila + fila_aux );
                }
				//imprimir = 10;
            }
            break;
        case _TKT_FOR:
            ind_configTmp = 0;
            memset( cadaux, 0, sizeof( cadaux ) );
			strncpy( cadaux, CONFIG_COMPROBANTE1[( *ind_config )].parametros, sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_COMPROBANTE1[( *ind_config )].parametros ) + 1;
            _COMP_FOR_I = ( int )RESOLVER( cadaux, NULL );
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            (
            ind_configTmp ) += strlen(
            &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp] ) + 1;
            _COMP_FOR_LIMITE = ( int )RESOLVER( cadaux, NULL );
            _COMP_FOR_IND_CONFIG = *ind_config;
            break;
        case _TKT_END_FOR:
            ind_configTmp = 0;
            _COMP_FOR_I += VALIDA_tomar_entero( ind_config, &ind_configTmp ); // Incremento de I
            if( _COMP_FOR_I <= _COMP_FOR_LIMITE ) {
                *ind_config = _COMP_FOR_IND_CONFIG;
            }
            break;
        case _TKT_SI_FORMULA:
            _SI_P_FORMULA = CONFIG_COMPROBANTE1[( *ind_config )].parametros;
            //( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;
            break;
        case _TKT_FORMULA:
            ind_configTmp = 0;
            strncpy( xx, CONFIG_COMPROBANTE1[( *ind_config )].parametros, sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_COMPROBANTE1[( *ind_config )].parametros ) + 1;
            strncpy( cadaux, &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            (
            ind_configTmp ) += strlen(
            &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp] ) + 1;
            strncpy( cadaux2, &CONFIG_COMPROBANTE1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            //( *ind_config ) += strlen( &CONFIG_COMPROBANTE[( *ind_config )] ) + 1;

            importe_total = RESOLVER( xx, NULL );

            //if( cadaux2[0] ) {
                if( cadaux[0] ) {
                    FORMAT_FLOTANTE( cadaux, importe_total, xx );
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
                else {
                    MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, SI, NO, importe_total );
                }
            //}
            break;
        case _TKT_VARIABLE_RNV:
            ind_configTmp = 0;
            cod_var = ( INT16 )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            cod_sub = ( INT16 )VALIDA_tomar_entero( ind_config, &ind_configTmp );
            // esto todavia no estaria implementado, serviria solo para imprimir una variable comun.
            //if( cod_sub == 254 ) cod_sub = _TKT_FOR_I;
            if( DD_TIPO( cod_var ) == 4/*_DOUBLE*/ ) {
                char aux[20];
                memset( aux, 0, 20 );
                CARGAR_VARIABLE_RNV( cod_var, aux, 20, cod_sub, NULL );
                FTOC( strtod( aux, NULL ), aux, ENTEROS + 1, DECIMALES, SI, SI );
                //MOSTRAR_CADENA_ALD( tkt_columna, PRN, aux, 14 );
                MOSTRAR_CADENA( columna, PRN, aux );
            }
            else {
                MOSTRAR_VARIABLE_RNV( cod_var, cod_sub, columna, NULL );
            }
            break;
        case _COD_CLIENTE_GENERICO:
            IMPRIMIR_CADENA( columna, clientes.valor_grupo_cliente );
            break;
        case _DNI_CLIENTE:
            {
                char aux[50];
                sprintf( aux, "%ld", clientes.nro_documento );
                IMPRIMIR_CADENA( columna, aux );
            }
            break;

		case _TKT_MES_ANIO_VENCIMIENTO:
			{
			char fecha_vto[30];
			int marca_tarjeta = VAR_TARJETAS_TARJETA;
			memset( fecha_vto, 0, sizeof( fecha_vto ) );
			
			if( TARJ[marca_tarjeta].enmasc_fecha == 's'
				|| TARJ[marca_tarjeta].enmasc_fecha == 'S' ) {
				strcpy( fecha_vto, "**/**" );
				MOSTRAR_CADENA( columna, PRN, fecha_vto );
			} else 
			MOSTRAR_MES_ANIO( columna, PRN, GET_FECHA_VTO_PRIVADA(  )  );
			}
			break;

		case _NUMERO_CUPON:
			{
			char aux[9];
			if( VAR_TARJETAS_NRO_CUPON > 0)
			sprintf( aux, "%li", VAR_TARJETAS_NRO_CUPON );
			else
				sprintf( aux, "%li", VAR_TARJETAS_NRO_CUPON_INT );

			MOSTRAR_CADENA( columna,PRN,aux);
			}
			break;
		case _MONEDA2:
			{
				char aux[8];
				if(VAR_TARJETAS_MONEDA == PESOS) {
					sprintf( aux, "%s", "PESOS" );
                    MOSTRAR_CADENA( columna,PRN,aux);
				}
				if(VAR_TARJETAS_MONEDA == DOLARES) {
					sprintf( aux, "%s", "DOLARES" );
                    MOSTRAR_CADENA( columna,PRN,aux);
				}
			}
				break;
        case _CONFIRMAR_IMPRESION:
            CONFIRMAR_IMPRESION = 1;
            break;
        case _DATOS_ADICIONALES_COMPROB:
            ind_configTmp = 0;
            cod_sol = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( !imprimiendo_factura_journal_val ) {
                //PAGOS_PEDIR_DATOS_ADICIONALES( _hoja_actual, _nro_dato++, ( int ) cod_sol , NULL, 1,  NULL, 0, D_VALIDACIONES );
                PAGOS_PEDIR_DATOS_ADICIONALES( _hoja_actual, ( char )/*_nro_dato++*/( char )indice_pos++, ( int )cod_sol, NULL, D_VALIDACIONES,
                                               NULL, 0, NULL, NULL , '\0', '\0');
            }
            break;
        case _PROCESAR_DATOS_ADICIONALES:
            ind_configTmp = 0;
            cod_sol = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            nro_orden = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            modalidad = VALIDA_tomar_entero( ind_config, &ind_configTmp );   //ver si es antes de confirmar la impresion o despuï¿½
            if( !imprimiendo_factura_journal_val ) {
                CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( cod_sol, nro_orden, RAM_ID_ULTIMO_EVENTO,
                                                         modalidad, D_VALIDACIONES, 0,
                                                         _hoja_actual );
            }
            else {
                CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( cod_sol, nro_orden, RAM_ID_ULTIMO_EVENTO,
                                                         0, D_VALIDACIONES, 0, _hoja_actual + 1 );
            }
            break;
        case _DATO_ADICIONAL_MEDIO_PAGO:
            ind_configTmp = 0;
            memset( xx, 0, sizeof( xx ) );
            posicion = VALIDA_tomar_entero( ind_config, &ind_configTmp );
            if( posicion == 254 ) {
                posicion = _COMP_FOR_I;
            }
            if( !imprimiendo_factura_journal_val ) {
                if( BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( posicion, xx ) ) {
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
            }
            else {
                if( BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( _hoja_actual - 1, xx ) ) {
                    MOSTRAR_CADENA( columna, PRN, xx );
                }
            }
            break;
		case _TKT_SLIP_ON:
            PRN_HABILITA_SLIP( 1, NO );
			#if defined( INVEL_W) || defined( INVEL_L)
				LF( 1 );
				setData( ACTION_PRINT, NULL, 0 );
			#endif
            break;
        case _TKT_SLIP_OFF:
            #if defined( INVEL_W) || defined( INVEL_L)
            LF( 1 );
            setData( ACTION_PRINT, NULL, 0 );
            #endif
            EJECT();
            #if defined( INVEL_W) || defined( INVEL_L)
            LF( 1 );
            setData( ACTION_PRINT, NULL, 0 );
            #endif
            #if !defined(INVEL_W) && !defined(INVEL_L)
            delay( 1000 );
            #endif
            PRN_INHABILITA_SLIP();
            break;
        case _VAL_SI_REIMPRESION:
            VAL_SI_REIMPRESION = 1;
            break;
        case _CUIT_ESTABLECIMIENTO:
            MOSTRAR_CADENA( columna, PRN, config.cuit );
            break;
        case _NRO_ESTABLECIMIENTO:
            MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_NRO_COMERCIO);
            break;
        case _TIPO_TARJETA:
            if( VAR_TARJETAS_TIPO_CUENTA || VAR_TARJETAS_L_PIN > 0) {
		         MOSTRAR_CADENA( columna, PRN, ST(S_TARJETA_DE_DEBITO) );
	         } else {
		         MOSTRAR_CADENA( columna, PRN, ST(S_TARJETA_DE_CREDITO) );
	         }
         break;
       case _NRO_CUPON:
            MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_NRO_COMERCIO);
            break;
		 case _NRO_COMPROBANTE:
			 {
				char aux[20];
				sprintf(aux,"%li",VAR_TARJETAS_NRO_CUPON_INT);
				MOSTRAR_CADENA( columna, PRN,aux );
			 }
            break;
       case _CODIGO_AUTORIZACION:
            MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_AUTORIZACION);
            break;
		 case _TARJETA_TABLA:
		      MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_TABLA);
		      break;
		 case _OPERACION: {
				char *c_op[] = {"Compra","On-line Compra","Off-line Compra","Anulacion","On-line Anulacion",
                    "Off-line Anulacion","Devolucion","On-line Devolucion","Off-line Devolucion","On-line Anulacion Devo.","Off-line Anulacion Dev.", "On-Line Extraccion","On-Line Anul Extracc"};
   
				int op;
				char aux[24];
				int devolucion = 0, anulacion = 0;
				if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
					devolucion = 1;
				}

				op = VAR_TARJETAS_ON_LINE;
				if(VAR_TARJETAS_OPERACION ==_ON_LINE_ANULACION 
					|| VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION   )
					anulacion = 1;
				if(VAR_TARJETAS_OPERACION ==_ON_LINE_ANULACION_DEVOLUCION 
					|| VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION_DEVOLUCION   )
					anulacion = 1;
				
				if(VAR_TARJETAS_OPERACION ==_ON_LINE_CASHPLUS ||  RAM_MODO_EJECUCION == CAMBIO_MEDIO)
					op = 11 ;
				 
				if(VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION_CASHPLUS  )
					 op = 12;

				if( anulacion ) {
					op += 3;
					if( VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION && devolucion == 0)
						op += 1;

					if( RAM_NOTA_CR > 0 &&  devolucion)
						op += 5;
					if( VAR_TARJETAS_OPERACION == _OFF_LINE_ANULACION && devolucion == 1)
						op += 1;
				}
				else if( devolucion ) {
					op += 6;
				 }
				strncpy( aux, c_op[op] , sizeof( aux ) );
				MOSTRAR_CADENA( columna, PRN, aux );
				}
				break;

        case _CUOTA_COEFICIENTE : {
            char aux[12];
            double dd;

            dd = _PAGO_IMPORTE( h ) * VAR_TARJETAS_COEFICIENTE ;
            sprintf(aux,"%lf",dd);
            MOSTRAR_CADENA( columna, PRN, aux );
            }
         	break;
		case _NUMERO_TERMINAL:
			{
			char aux[10];
			sprintf(aux,"%li", VAR_TARJETAS_TERMINAL);
			aux[9]= 0;
			MOSTRAR_CADENA( columna, PRN, aux );
			}
			break;
		case _FECHA_ORIGINAL:
			if(VAR_TARJETAS_FECHA_ORIGINAL > 0)
				MOSTRAR_FECHA( columna , PRN, VAR_TARJETAS_FECHA_ORIGINAL );
			break;
		case _CUPON_ORIGINAL:
			{
			char aux[10];
			memset(aux,0,sizeof(aux));
			sprintf(aux,"%li", VAR_TARJETAS_CUPON_ORIGINAL);
			MOSTRAR_CADENA( columna, PRN, aux );
			}
			break;
		case _NUMERO_LOTE:
			{
			char aux[8];
			sprintf(aux,"%i", VAR_TARJETAS_LOTE);
			MOSTRAR_CADENA( columna, PRN, aux );
			}
			break;
		case  _NRO_CUENTA_TARJETA:
			{
				if( VAR_TARJETAS_NRO_CUENTA[0] != 0 ) 
					MOSTRAR_CADENA( columna, PRN, VAR_TARJETAS_NRO_CUENTA);
			}
			break;
		 case _FIN_VALIDACION:
		      glog("FIN VALIDACION",LOG_VENTAS,4);
		      break;
		case _IMPORTE_EXTRACCION:
			MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 1,ROUND( VAR_TARJETAS_ANTICIPO  , ENTEROS, DECIMALES )) ;
			break;
		 case _IMPORTE_TOTALCASHPLUS:
			 {
			double importe_total = 0.00;			
			if(  VAR_TARJETAS_OPERACION == _ON_LINE_ANULACION_CASHPLUS  ) 
				importe_total = ROUND( _PAGO_IMPORTE( h ) + _PAGO_IMPORTE_DESCUENTO( h ), ENTEROS, DECIMALES );
			else
				importe_total = ROUND( VAR_TARJETAS_ANTICIPO  , ENTEROS, DECIMALES ) +  ROUND( _PAGO_IMPORTE( h ) + _PAGO_IMPORTE_DESCUENTO( h ), ENTEROS, DECIMALES );
			 
			MOSTRAR_FLOTANTE( columna, PRN, ENTEROS, DECIMALES, 1, 1,importe_total) ;
			 }
			break;
		case  _VALIDA_FIN_SI:
		      glog("_VALIDA FIN SI",LOG_VENTAS,4);
		      break;
		default:
            if( codigo >0 ) {
				 MOSTRAR_CADENA( columna, PRN, "VALIDACION NO REG" );
				 MOSTRAR_ENTERO( columna, PRN, "9999", codigo );
				 LF(1);
                //GRABAR_LOG_SISTEMA_STRING( S_ERROR_COMANDO_VALIDACION );
            }

            break;
    }
}
/***************************************************************************/
int VAL_evaluar_condiciones( int codigo, /*int *ind_config,*/ int h )
/***************************************************************************/
{
    int rta, i, j;
    rta = codigo;

	if( !_SKIP_BLOCK && SI_DESCUENTO_TARJETA ) {
		SI_DESCUENTO_TARJETA = 0;
		if( _COMPARACION_IGUAL )
		{
   			if( fabs( _PAGO_IMPORTE_DESCUENTO( h ) ) < 0.005 )
   				rta = 0;
		}
		else
		{
   			if( fabs( _PAGO_IMPORTE_DESCUENTO( h ) ) >= 0.005 )
   				rta = 0;
		}
	}
    if( !_SKIP_BLOCK && _COD_VARIABLE_RNV ) {
        if( _COMPARACION_IGUAL ) {
            if( OBTENER_VALOR_RNV( _COD_VARIABLE_RNV, _SUB_VARIABLE_RNV ) != ( int )_VALOR_RNV ) {
                rta = 0;
            }
        }
        else if( OBTENER_VALOR_RNV( _COD_VARIABLE_RNV, _SUB_VARIABLE_RNV ) == ( int )_VALOR_RNV ) {
            rta = 0;
        }
        _COD_VARIABLE_RNV = 0;
    }
    if( !rta && _VALIDA_BEGIN == codigo ) {
        _SKIP_BLOCK = 1;
    }
    if( _VALIDA_END == codigo ) {
        _SKIP_BLOCK = 0;
    }
	if( !_SKIP_BLOCK && SI_DEVOLUCION ) {
        SI_DEVOLUCION = 0;
        if( !MODO_DEVOLUCION ) {
            rta = 0;
        }
    }
    if( !_SKIP_BLOCK && SI_CODIGO_MEDIO ) {
        if( SI_CODIGO_MEDIO != _PAGO_MODO( h ) + 1 ) {
            rta = 0;
        }
        SI_CODIGO_MEDIO = 0;
    }

    if( !_SKIP_BLOCK && VAL_SI_REIMPRESION ) {
        VAL_SI_REIMPRESION = 0;
        if( en_reimpresion == 0 ) {
            rta = 0;
        }
    }

    for( j = 0; j < 10 && !_SKIP_BLOCK && SI_HAY_MEDIO[j]; j++ ) {
		for( i = 0; i < RAM_P_PAGO
			&& ( SI_HAY_MEDIO[j] != _PAGO_MODO( i ) + 1 ); i++ ){}
		if( i < RAM_P_PAGO ){
			rta = codigo;
			break;
		}else{
			rta = 0;
		}
    }
	SI_HAY_MEDIO[0] = 0;

    if( _SI_P_FORMULA ) {
        if( fabs( RESOLVER( _SI_P_FORMULA, NULL ) ) < 0.005 ) {
            rta = 0;
        }
        _SI_P_FORMULA = NULL;
    }
    /*-------- Si no se realiza la accion incrementamos el indice -------*/
    if( !rta || _SKIP_BLOCK ) {
//(Nota Unific MySQL-Linux):Lo siguiente queda comentado como en la version Linux
        /*switch( codigo ) {
            case _POS_X:
            case _POS_Y:
            case _IMPRESION_CHEQUE:
            case _NRO_DOC_FISCAL:
            case _NRO_COM_RETIRO:
            case _NRO_COM_FONDO_FIJO:
            case _NRO_COM_INVENTARIO:
            case _REIMPRESION_FACTURA:
            case _NETO_GRAVADO_PROPORCIONAL:
            case _IVA_PROPORCIONAL:
            case _LF_RECEIPT:
            case _VALIDA_DATO_MEDIO:
            case _VALIDA_DIA_DATO_MEDIO:
            case _VALIDA_MES_DATO_MEDIO:
            case _VALIDA_ANIO_DATO_MEDIO:
            case _VALIDA_IMPRIMIR_COLUMNAS_BUFFER:
			case _LINEA_MENSAJE:
                ( *ind_config ) += 2;
                break;
            case _POSICION:
                //case _IMPORTE_EN_LETRAS:
            case _VALIDA_REDIRECCIONAR_A_BUFFER:
                //case _VARIABLE_RNV:
                //case _SI_VALOR_VARIABLE_RNV:
                //case _SI_NO_VALOR_VARIABLE_RNV:
                ( *ind_config ) += 4;
                break;
            case _VARIABLE_RNV:
                ( *ind_config ) += 4;
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                break;
            case _SI_VALOR_VARIABLE_RNV:
            case _SI_NO_VALOR_VARIABLE_RNV:
                ( *ind_config ) += 6;
                break;
            case _IMPORTE_EN_LETRAS:
                ( *ind_config ) += 8;
                break;
            case _VALIDA_CONSULTA_TABLA:
                ( *ind_config ) += 4;
                //el break falta intencionalmente
            case _VALIDA_FORMULA:
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                break;
            case _VALIDA_SI_FORMULA:
            case _CADENA:
            case _MENSAJE_ESPECIAL:
            case _MENSAJE_CUMPLEANIO:
            case _ENVIAR_BMP:
			case _IMAGEN:
			case _SETEA_TOTAL:
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                break;
            case _INSERTAR_BOLETIN:
                ( *ind_config ) += 10;
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                ( *ind_config ) += strlen( &CONFIG_VALIDA[( *ind_config )] ) + 1;
                break;
        }
        rta = 0;*/
    }
    return ( rta );
}
/***************************************************************************/
void VALIDA_importe_en_letras( int *ind_config )
/***************************************************************************/
{
    char p_cad[200],*p_cad2;
    int largo,col_siguiente,l_cad,i/*, relleno, largo_con_relleno*/;
    int  ind_configTmp;

    /*---------------------- Inicializaciones ---------------------*/
    ind_configTmp = 0;
    largo = VALIDA_tomar_entero( ind_config, &ind_configTmp );
    col_siguiente = VALIDA_tomar_entero( ind_config, &ind_configTmp );
    /*  relleno = VALIDA_tomar_entero( ind_config );
        largo_con_relleno = VALIDA_tomar_entero( ind_config ); */
    /*------------------ Transformacion a letras ------------------*/
    NUM_A_LETRAS( valida.importe, p_cad );
    l_cad = strlen( p_cad );
    /*  if( relleno && relleno != 32 && largo_con_relleno && l_cad < largo_con_relleno ) {
            memset( &p_cad[l_cad], relleno, largo_con_relleno - l_cad );
            p_cad[largo_con_relleno] = 0;
            l_cad = largo_con_relleno;
            if( l_cad > largo ){
                p_cad[largo - 1] = ' ';
            }
        }*/
    if( l_cad <= largo ) {
        MOSTRAR_CADENA( columna, PRN, p_cad );
		/*if(IMPRESORA_FISCAL==FISCAL)
		{
			memset(cadena_aux,0,sizeof(cadena_aux));
			for(i=0;i<columna;i++)
				cadena_aux[i]=' ';
			strcat(cadena_aux,p_cad);
			IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux);//Seba
		}*/
    }
    else {
        for( i = largo - 1;i > 0 && p_cad[i] != ' ';i-- ) {
            ;
        }
        p_cad2 = &p_cad[i + 1];
        if( i >= 0 )
			p_cad[i] = 0;
        MOSTRAR_CADENA( columna, PRN, p_cad );
        VALIDA_SET_FILA( fila + 1 );
        MOSTRAR_CADENA( col_siguiente, PRN, p_cad2 );
		/*if(IMPRESORA_FISCAL==FISCAL)
		{
			memset(cadena_aux,0,sizeof(cadena_aux));
			for(i=0;i<columna;i++)
				cadena_aux[i]=' ';
			strcat(cadena_aux,p_cad);
			IMPRIMIR_TEXTO_NO_FISCAL(1,tipo_letra,cadena_aux);//Seba
			memset(cadena_aux,0,sizeof(cadena_aux));
			for(i=0;i<columna;i++)
				cadena_aux[i]=' ';
			strcat(cadena_aux,p_cad2);
			IMPRIMIR_TEXTO_NO_FISCAL(1,tipo_letra,cadena_aux);//Seba
		}*/
    }
}

/***************************************************************************/
INT16 VALIDA_tomar_entero( int *ind_config, int *indice )
/***************************************************************************/
{
    INT16 valor;

    valor = *( INT16* )&CONFIG_VALIDA[*ind_config].parametros[*indice];
    ( *indice ) += 2;
    return ( valor );
}
/***************************************************************************/
float VALIDA_tomar_float( int *ind_config )
/***************************************************************************/
{
    float valor;
    valor = *( float* )&CONFIG_VALIDA[*ind_config];
    ( *ind_config ) += 4;
    return ( valor );
}
/***************************************************************************/
void VALIDA_SET_FILA( int _fila )
/***************************************************************************/
{
    if( _fila > fila ) {
        LF( _fila - fila );
        fila = _fila;
    }
    fila = _fila;
}
/*****************************************************************************/
void VALIDA_CHEQUE_CLIENTE( double importe, double saldo )
/*****************************************************************************/
{
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( VALIDACION_CLIENTE_PARA_CHEQUE ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = importe;
        valida.nro_cliente = RAM_NRO_CLIENTE;
        valida.saldo = saldo;
        strncpy( valida.mensaje, ST( S_CHEQUES_CLIENTE ), sizeof( valida.mensaje ) );
        VALIDAR( VALIDACION_CLIENTE_PARA_CHEQUE, _VALCLICHEQ, 0, 0, 0 );
    }
}
/*****************************************************************************/
void REIMPRIMIR_VALIDACIONES_MEDIOS_DE_PAGO()
/*****************************************************************************/
{
    int indice;
    if( SOLICITAR_CLAVES( 27 - ancho_panel, 21, NO, SI, PEDIR_SUPERVISOR_REIMP_VALID_MED,
                          ST( S_REIMPRESION_VALIDACION_MEDIO_DE_PAGO ), NULL, RAM_COD_FUNCION,
                          SI ) ) {
        //SELECT( AREA_TICKET );
        //SET_ORDER( 1 );
	    SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
		SET_ORDER2("ID_EVENTO");
		SET_WHERE("");
		RUN_QUERY(NO);
        GO2( BOTTOM );
    }
    while( ( indice = MENU_SELECCIONA_MEDIO_A_REIMPRIMIR() ) != -1 ) {
        VALIDAR_UN_MEDIO( indice, NO, _MODO_VENTA );
    }
}
/***************************************************************************/
int MENU_SELECCIONA_MEDIO_A_REIMPRIMIR()
/***************************************************************************/
{
    struct _menu *items = NULL;
    char *medio_nombre = NULL;
    char *posicion = NULL;
    int *medio_codigo = NULL;
    int cant_medios_a_reimprimir = 0;
    int rta = -1, h, cod;
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    #endif
    items = ( struct _menu * )calloc( RAM_P_PAGO, sizeof( struct _menu ) );
    medio_nombre = ( char* )calloc( RAM_P_PAGO, sizeof( char[50] ) );
    medio_codigo = ( int* )calloc( RAM_P_PAGO, sizeof( int ) );
    posicion = ( char* )calloc( RAM_P_PAGO, sizeof( char ) );
    for( h = 0;h < RAM_P_PAGO;h++ ) {
        cod = medios[_PAGO_MODO( h )].cod_valida;
        if( cod ) {
            sprintf( ( char* )&medio_nombre[cant_medios_a_reimprimir * 50], " %d) %s de %.2f ",
                     cant_medios_a_reimprimir, medios[_PAGO_MODO( h )].nombre, _PAGO_IMPORTE( h ) );
            medio_codigo[h] = medios[_PAGO_MODO( h )].nro;
            items[cant_medios_a_reimprimir].nom = ( char* )
                                                 &medio_nombre[cant_medios_a_reimprimir * 50];
            items[cant_medios_a_reimprimir].tecla = cant_medios_a_reimprimir + 48;  // Para obtener el Ascii de la Tecla
            items[cant_medios_a_reimprimir].rta = cant_medios_a_reimprimir;
            posicion[cant_medios_a_reimprimir] = h;
            cant_medios_a_reimprimir++;
        }
    }
    if( cant_medios_a_reimprimir ) {
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
            rta = MENU( ST( S_MEDIOS_DE_PAGO_A_REIMPRIMIR ), items, cant_medios_a_reimprimir, cab,
                        columnas );
            free( cab );
        }
        #else
        rta = MENU( 0, 17, ST( S_MEDIOS_DE_PAGO_A_REIMPRIMIR ), items, cant_medios_a_reimprimir,
                    120, 52 );
        #endif
        if( rta >= 0 ) {
            rta = posicion[rta];
        }
    }
    if(items)
    free( items );
    if(medio_nombre)
    free( medio_nombre );
    if(medio_codigo)
    free( medio_codigo );
    if(posicion)
    free( posicion );
    return rta;
}
/***************************************************************************/
void VALIDAR_UN_MEDIO( int h, int cod, int modo )
/***************************************************************************/
{
    int cant,i;
    double total;
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( !cod ) {
        cod = medios[_PAGO_MODO( h )].cod_valida;
    }
	if( cod ) {
        /*------------------ Inicializaciones ---------------*/
		valida.nro_suc = config.sucursal;
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = _PAGO_IMPORTE( h );
        valida.nro_cliente = RAM_NRO_CLIENTE;
        valida.numero = _PAGO_NUMERO( h );
		_snprintf(valida.cuenta, sizeof(valida.cuenta),"%i", _PAGO_CUENTA( h ));//estaba comentado por el tipo de dato...
        valida.cuenta[sizeof( valida.cuenta )- 1] = '\0';
		valida.cod_medio = _PAGO_MODO( h );
        valida.autorizacion = _PAGO_BUFFER1_LONG( h );
		valida.orden_medio = h;
		valida.nro_comp=RAM_NRO_COMP;
		nro_comp = valida.nro_comp;
		strncpy( valida.telefono_comprador, _PAGO_TELEFONO_OTRO( h ), sizeof( valida.telefono_comprador ) );
		strncpy( valida.mensaje, medios[_PAGO_MODO( h )].nombre, sizeof( valida.mensaje ) );
        strncpy( valida.telefono, _PAGO_TELEFONO( h ), sizeof( valida.telefono ) );
        strncpy( valida.telefono_comprador, _PAGO_TELEFONO_OTRO( h ), sizeof( valida.telefono_comprador ) );
        strncpy( valida.cuit, _PAGO_CUIT( h ), sizeof( valida.cuit ) );
		if( medios[_PAGO_MODO( h )].cheque ) {
            if( !medios[_PAGO_MODO( h )].al_dia ) {
                valida.fecha = _PAGO_FECHA_VTO( h );
                valida.fecha1 = GET_FECHA_DOS();
                cheque_a_fecha = 1;
            }
            valida.comprobante = _PAGO_BANCO( h );
        }
        else {
			valida.comprobante = _PAGO_NRO_CUPON_INT1(h);//Comprobante dejo de ser long para ser double
        }
        TRIM( valida.mensaje );
        cant = PAGOS_CANTIDAD_DATOS_ADICIONALES( h );
		if( cant ) {
            total = 0.0;
            for( i = 0 ;i < cant ;i++ ) {
				_snprintf(valida.mensaje, sizeof(valida.mensaje)-1,"%s (%i/%i)", medios[_PAGO_MODO( h )].nombre, i + 1,
                         cant );
                if( i == cant - 1 ) {
                    valida.importe = ROUND( _PAGO_IMPORTE( h ) - total, ENTEROS, DECIMALES );
                }
                else {
                    valida.importe = ROUND( _PAGO_IMPORTE( h ) / cant, ENTEROS, DECIMALES );
                }
                total += valida.importe;
                VALIDAR( cod, _VALMEDIOS, h, i, modo );
            }
        }
        else {
            VALIDAR( cod, _VALMEDIOS, h, 0, modo );
        }
    }
}
/**************************************************************/
void MOSTRAR_VARIABLE_RNV( int cod_variable, int subindice, int columna, char *picture )
/**************************************************************/
{
    char *aux = ( char * )malloc( 80 );
    memset( aux, 0, 80 );
    CARGAR_VARIABLE_RNV( cod_variable, aux, 80, subindice,
                         ( picture && picture[0] ) ? picture : NULL );
    MOSTRAR_CADENA( columna, PRN, aux );
    free( aux );
}
/**************************************************************/
int OBTENER_VALOR_RNV( int cod_variable, int subindice )
/**************************************************************/
{
    int tipo_variable,valor_int;
    tipo_variable = DD_TIPO( cod_variable );
    switch( tipo_variable ) {
        case _SHORT_INT:
            if( subindice >= 0 ) {
                valor_int = _GET_MEMORY_CHAR( cod_variable, subindice );
            }
            else {
                valor_int = GET_MEMORY_CHAR( cod_variable );
            }
            break;
        case _INT:
        case _UNSIGNED:
        case _HORA:
            if( subindice >= 0 ) {
                valor_int = _GET_MEMORY_INT( cod_variable, subindice );
            }
            else {
                valor_int = GET_MEMORY_INT( cod_variable );
            }
            break;
        case _DOUBLE:
            if( subindice >= 0 ) {
                valor_int = ( int )_GET_MEMORY_DOUBLE( cod_variable, subindice ) * 100;
            }
            else {
                valor_int = ( int )GET_MEMORY_DOUBLE( cod_variable ) * 100;
            }
            break;
        case _STRING:
            valor_int = GET_MEMORY_STR_CHAR( cod_variable, 0 );
            break;
    }
    return ( valor_int );
}
/***************************************************************************/
void VALIDAR_DONACION()
/***************************************************************************/
{
    int codigo;
    struct institut_donacion instituciones;
    if( DONACION_DE_VUELTO ) {
        _SELECCION_PAPEL = RECEIPT | JOURNAL;
        if( CODIGO_VALIDACION_PARA_DONACION && RAM_TOTAL_DONACION ) {
            /*------------------ Inicializaciones ---------------*/
            valida.fecha = GET_FECHA_DOS();
            valida.hora = GET_HORA_DOS();
            valida.importe = RAM_TOTAL_DONACION;
            valida.nro_cliente = RAM_NRO_CLIENTE;
            codigo = RAM_INSTITUCION_BENEFICIARIA;
            //USE_DB( AREA_AUX, _INSTIT, ( char* )&instituciones, sizeof( instituciones ), NULL, 0,
            //        0 );
            //SET_ORDER( 1 );
            //GET_EQUAL( ( char* )&codigo );
			OPEN_TABLE( T_INSTITUT, TT_ORIG, ( char* )&instituciones, sizeof( instituciones ) );
			SELECT_TABLE( T_INSTITUT, TT_ORIG );
			SET_WHERE("COD_INSTITUCION = '%d'", codigo );
			RUN_QUERY(NO);
            if( FOUND2() ) {
				strncpy( ( char* )valida.mensaje, ( char* )&instituciones.desc_institucion , sizeof( valida.mensaje ) );
            }
            else {
                strncpy( valida.mensaje, ST( S_ENTIDAD_DESCONOCIDA ), sizeof( valida.mensaje ) );
            }
            //CLOSE_DB( _INSTIT );
			CLOSE_TABLE( T_INSTITUT, TT_ORIG );
            TRIM( valida.mensaje );
            VALIDAR( CODIGO_VALIDACION_PARA_DONACION, _VALDONA, 0, 0, 0 );
        }
    }
}
/***************************************************************************/
int VALIDA_COD_COMANDO()
/***************************************************************************/
{
    int seguir = 1;
    char cadena[80];
    if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
        if( !( _NRO_DOCUMENTO_FISCAL > 3 && _NRO_DOCUMENTO_FISCAL != 49
            && _NRO_DOCUMENTO_FISCAL < 30 && _NRO_DOCUMENTO_FISCAL != 0 ) ) {
            seguir = 0;
            GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_NRO_DOC_FISCAL ,LOG_VENTAS,1);
        }
        if( MODO_DEBUG == 50 || MODO_DEBUG == 15 ) {
            sprintf( cadena, "nrodocfiscal: %d", _NRO_DOCUMENTO_FISCAL );
            GRABAR_LOG_SISTEMA( cadena,LOG_JAVA_SERVER,3 );
        }
    }
    return seguir;
}
/***************************************************************************/
void VALIDAR( int cod, int tipo, int h, int ind_posicion, int modo )
/***************************************************************************/
{
    char mens[40],cadena[80];
    int impreso_ok = NO, por_slip = NO, k, cnt = 0;

    while( !impreso_ok ) {
    if( cod && cod <= ( int )CANTIDAD_VALIDACIONES ) {
        ind_config = TABLA_VALIDA[cod - 1];
        if( ind_config > 0 ) {
            if( MODO_DEBUG == 50 || MODO_DEBUG == 15 ) {
                sprintf( cadena, "codval: %d, tipoval: %d", cod, tipo );
                GRABAR_LOG_SISTEMA( cadena,LOG_VENTAS,4 );
            }
            if( _SELECCION_PAPEL ) {
                PRN_SELECT( _SELECCION_PAPEL );
            }
            CORTAR_PAPEL_ANTES_DE_VALIDACION( ind_config, tipo );
            while( CONFIG_VALIDA[ind_config].codigo != _FIN_VALIDACION
                && CONFIG_VALIDA[ind_config].codigo != _VAL_FIN ) {
                    if( CONFIG_VALIDA[ind_config].codigo == _SLIP_ON ) {
                        por_slip = SI;
                    }
                if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && CONFIG_VALIDA[ind_config++].codigo == _SLIP_ON ) {
                    if( _NRO_IMPRESION_CHEQUE == cod ) {
                        PRN_HABILITA_SLIP( _LANDSCAPE, SI );
                    }
                    else {
                        PRN_HABILITA_SLIP( 1, SI );
                    }
                    ind_config++;
                }
                else {
                    VALIDA_ejecutar_comando( ( int* )&ind_config, h, ind_posicion, modo, NO );
                }
            }
            /*
                        if( IMPRESORA_FISCAL == FISCAL ) {
                            int mens = 0;
                            while( STATUS_PRINTER_FISCAL( ) ) {
                                if ( mens == 20000 ) {
                                    mens = 0;
                                    MENSAJE_STRING( S_IMPRESORA_ERROR );
                                }
                                mens++;
                            }
                        }
            */
			indice_pos = 0;
			INDICE_IMPRESION = 0;
            BORRAR_MENSAJE();
            SET_COLUMNA( 1 );
        }
        else {
            sprintf( mens, ST( S_VALIDACION__I_NO_DEFINIDA ), cod );
            MENSAJE( mens );
        }
    }
    setData( ACTION_PRINT, NULL, 0 );
        impreso_ok = SI;
        if( por_slip ) {
            ESPERAR_IMPRESION_SLIP_DRIVER( 'c' );
            VACIAR_KEY();
            MENSAJE( ST( S_SE_IMPRIMIO_CORRECTAMENTE_VALIDACION_ENTER_ESC ) );
            k = GETCH(  );
            while( k != 13 && k != 27 ) {
                k = GETCH();
            }
            if( k == 27 ) {
                impreso_ok = NO;
                cnt++;
            }
        }
    }
}
/***************************************************************************/
void CORTAR_PAPEL_ANTES_DE_VALIDACION( int ind, int tipo )
/***************************************************************************/
{
    int cortar = 1;
    if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && tipo == _VALPROMO ) {
        while( CONFIG_VALIDA[ind].codigo != _FIN_VALIDACION
            && CONFIG_VALIDA[ind].codigo != _FIN_VALIDACION && cortar ) {
            if( CONFIG_VALIDA[ind].codigo == _SLIP_ON ) {
                cortar = 0;
            }
            else {
                ind++;
            }
        }
        if( cortar ) {
            TKT_CORTE_PAPEL();
        }
    }
}
/*****************************************************************************/
void VALIDA_ENVASES_TOMRA()
/*****************************************************************************/
{
    int       /*diferencia, */ indice;
    _SELECCION_PAPEL = RECEIPT | JOURNAL;
    if( CODIGO_VALIDACION_TICKET_TOMRA ) {
        /*------------------ Inicializaciones ---------------*/
        valida.fecha = GET_FECHA_DOS();
        valida.hora = GET_HORA_DOS();
        valida.importe = 0.0;
        valida.nro_cliente = RAM_NRO_CLIENTE;
        strncpy( valida.mensaje, ST( S_DEVOLUCION_DE_ENVASES ), sizeof( valida.mensaje ) );
        ind_config = TABLA_VALIDA[CODIGO_VALIDACION_TICKET_TOMRA - 1];
        if( RECORRER_ENVASES( 2 ) && ind_config ) {
            CORTAR_PAPEL_ANTES_DE_VALIDACION( ind_config, _VALENVASES );
            while( CONFIG_VALIDA[ind_config].codigo != _FIN_VALIDACION
                && CONFIG_VALIDA[ind_config].codigo != _VAL_FIN  ) {
                if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && CONFIG_VALIDA[ind_config].codigo == _SLIP_ON ) {
                    PRN_HABILITA_SLIP( 1, SI );
                    ind_config++;
                }
                else {
                    if( CONFIG_VALIDA[ind_config].codigo != ( unsigned short )_COMIENZO_ENV_TOMRA ) {
                        VALIDA_ejecutar_comando( ( int* )&ind_config, 0, 0, 0, NO );
                    }
                    else {
                        indice = ind_config;
                        /*if( USE_DB( AREA_AUX_ENV, _AUX_ENV_SIC, ( char* )&aux_envases,
												sizeof( struct _aux_envases ), NULL, 0, 0 ) == 0 ) {*/
						if( OPEN_TABLE(T_AUX_ENVASE, TT_ORIG,( char* )&aux_envases, sizeof( struct _aux_envases))== 0 ) {
                            //GO( TOP );
							GO2( TOP );
                            while( /*!BtrvEOF()*/ !dbEOF() ) {
                                ind_config = indice;
                                //diferencia =
                                //  ( aux_envases.cantidad_voucher -
                                //  aux_envases.cantidad_facturados );
                                if( aux_envases.cantidad_voucher > 0 ) {
                                    // caso imprimir cantidad sobrante en voucher
                                    while( CONFIG_VALIDA[ind_config].codigo != _FIN_ENV_TOMRA ) {
                                        VALIDA_ejecutar_comando( ( int* )&ind_config, 0, 0, 0,NO );
                                    }
                                }
                                //SELECT( AREA_AUX_ENV );
								SELECT_TABLE( T_AUX_ENVASE, TT_ORIG );
                                //SKIP( 1 );
								SKIP2( 1 );
                            }
                            //CLOSE_DATABASE( AREA_AUX_ENV );
							CLOSE_TABLE( T_AUX_ENVASE, TT_ORIG );
                           if( indice == ind_config ) {
                                while( CONFIG_VALIDA[ind_config].codigo != _FIN_ENV_TOMRA ) {
                                    ind_config++;
                                }
                            }
                        }
                        else {
                            MENSAJE_STRING( S_ERROR_EN_AREA_AUXILIAR_ENVASES );
                            ind_config++;
                        }
                    }
                }
            }
            VACIAR_TABLA_AUXILIAR();
        }
    }
}
/***************************************************************************/
void VALIDAR_FACTURA_REMOTA( double total, char *nombre_cliente, int nro_cliente )
/***************************************************************************/
{
    /*------------------ Inicializaciones ---------------*/
    valida.fecha = GET_FECHA_DOS();
    valida.hora = GET_HORA_DOS();
    valida.importe = total;
    valida.nro_cliente = nro_cliente;
	strncpy( valida.mensaje, nombre_cliente, sizeof( valida.mensaje ) );
    VALIDAR( CODIGO_VALIDACION_FACTURA_REMOTA, _VALFACREM, 0, 0, 0 );
}
/***************************************************************************/
void CALCULAR_CODIGO_VOUCHER_BOLETA_PLATA()
/***************************************************************************/
{
    char ss[21] = "";
    long nro;
    //sprintf( ss, "29%.03i%.03i%.04li0", config.sucursal, NRO_CAJA, NRO_TICKET );
    nro = ( RAM_FACTURA || RAM_NOTA_CR ) ? RAM_NRO_COMP : NRO_TICKET;
    sprintf( ss, "29%.010li0",
             CALCULA_CODIGO_VERIFICACION( NRO_CAJA, _GET_FECHA_DOS(), nro, 0, config.sucursal ) );
    CALCULA_CRC_EAN13( ss );
    SET_MEMORY( __ram_cod_boleta_plata, ss );
}
/*****************************************************************************/
void VALIDA_tomar_cadena_compuesta( int *ind_config, char *val_cad, int h )
/*****************************************************************************/
{
    char *pos,*pos1,*pos2,*aux,cadena[35],_cad[35];
    int cod_variable, var_rnv = 0, indice = 0, lb;
    memset( cadena, 0, strlen( cadena ) );
    strncpy( cadena, CONFIG_VALIDA[*ind_config].parametros, sizeof( cadena ) + 1 );
    pos = strchr( cadena, '@' );
    if( pos ) {
        var_rnv = 1;
        pos1 = strchr( cadena, '(' );
        pos2 = strchr( cadena, ')' );
        cod_variable = atoi( &pos[1] );
        if( pos1 && pos2 ) {
            //*pos1 = 0;
            aux = &pos1[1];
            *pos2 = 0;
            if( aux[0] == 'H' ) {
                indice = h;
            }
            else {
                indice = atoi( aux );
            }
        }
    }
    else {
        sprintf( val_cad, CONFIG_VALIDA[*ind_config].parametros );
    }
    if( var_rnv ) {
        //if( OBTENER_VALOR_RNV( cod_variable, indice ) ) {
        //OBTENER_VALOR_RNV( cod_variable, indice );
        lb = sizeof( _cad );
        memset( _cad, 0, lb );
        CARGAR_VARIABLE_RNV( cod_variable, _cad, lb, indice, NULL );
        sprintf( val_cad, _cad );
        //}
    }
}
/**************************************************************/
void CARGAR_VARIABLE_RNV( int cod_variable, char *buffer, int l_buffer, int indice, char *picture )
/**************************************************************/
{
    char aux[80];
    int tipo_variable,valor_int,array,i,sumar,fecha;
    long valor_long;
    double valor_double;
    tipo_variable = DD_TIPO( cod_variable );
    array = DD_ELEMENTOS( cod_variable );
    sumar = ( ( indice == -3 ) || ( indice == 254 ) ) ? SI : NO;
    if( ( indice == -3 ) || ( indice == 254 ) ) {
        indice = 0;
    }
	switch( tipo_variable ) {
        case _SHORT_INT:
            if( array > 1 && indice >= 0 ) {
                if( sumar ) {
                    for( i = 0, valor_int = 0 ;i < array ;i++ ) {
                        valor_int += _GET_MEMORY_CHAR( cod_variable, i );
                    }
                }
                else {
                    if( indice < array ) {
                        valor_int = _GET_MEMORY_CHAR( cod_variable, indice );
                    }
                    else {
                        GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE ,LOG_ERRORES,2);
                    }
                }
            }
            else {
                valor_int = GET_MEMORY_CHAR( cod_variable );
            }
            if( picture ) {
                FORMAT_ENTERO( picture, ( long )valor_int, buffer );
            }
            else {
                sprintf( buffer, "%.2i", valor_int );
            }
            break;
        case _INT:
        case _UNSIGNED:
            if( array > 1 && indice >= 0 ) {
                if( sumar ) {
                    for( i = 0, valor_int = 0 ;i < array ;i++ ) {
                        valor_int += _GET_MEMORY_INT( cod_variable, i );
                    }
                }
                else {
                    if( indice < array ) {
                        valor_int = _GET_MEMORY_INT( cod_variable, indice );
                    }
                    else {
                        GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE ,LOG_ERRORES,2);
                    }
                }
            }
            else {
                valor_int = GET_MEMORY_INT( cod_variable );
            }
            if( picture ) {
                FORMAT_ENTERO( picture, ( long )valor_int, buffer );
            }
            else {
                sprintf( buffer, "%.4i", valor_int );
            }
            break;
        case _HORA:
            if( array > 1 && indice >= 0 ) {
                if( indice < array ) {
                    valor_int = _GET_MEMORY_INT( cod_variable, indice );
                }
                else {
                    GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE, LOG_ERRORES,2 );
                }
            }
            else {
                valor_int = GET_MEMORY_INT( cod_variable );
            }
            if( picture ) {
                FORMAT_ENTERO( picture, ( long )valor_int, buffer );
            }
            else {
                sprintf( buffer, "%.4i", valor_int );
            }
            break;
        case _LONG:
            if( array > 1 && indice >= 0 ) {
                if( sumar ) {
                    for( i = 0, valor_long = 0L ;i < array ;i++ ) {
                        valor_long += _GET_MEMORY_LONG( cod_variable, i );
                    }
                }
                else {
                    if( indice < array ) {
                        valor_long = _GET_MEMORY_LONG( cod_variable, indice );
                    }
                    else {
                        GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE, LOG_ERRORES,2  );
                    }
                }
            }
            else {
                valor_long = GET_MEMORY_LONG( cod_variable );
            }
            if( picture ) {
                FORMAT_ENTERO( picture, valor_long, buffer );
            }
            else {
                sprintf( buffer, "%.8li", valor_long );
            }
            break;
        case _FLOAT:
            if( array > 1 && indice >= 0 ) {
                if( sumar ) {
                    for( i = 0, valor_double = 0.0 ;i < array ;i++ ) {
                        valor_double += _GET_MEMORY_FLOAT( cod_variable, i );
                    }
                }
                else {
                    if( indice < array ) {
                        valor_double = _GET_MEMORY_FLOAT( cod_variable, indice );
                    }
                    else {
                        GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE, LOG_ERRORES,2  );
                    }
                }
            }
            else {
                valor_double = GET_MEMORY_FLOAT( cod_variable );
            }
            if( picture ) {
                FORMAT_FLOTANTE( picture, valor_double, buffer );
            }
            else {
                sprintf( buffer, "%*.*lf", ENTEROS + DECIMALES, DECIMALES, valor_double );
            }
            break;
        case _DOUBLE:
            if( array > 1 && indice >= 0 ) {
                if( sumar ) {
                    for( i = 0, valor_double = 0.0 ;i < array ;i++ ) {
                        valor_double += _GET_MEMORY_DOUBLE( cod_variable, i );
                    }
                }
                else {
                    if( indice < array ) {
                        valor_double = _GET_MEMORY_DOUBLE( cod_variable, indice );
                    }
                    else {
                        GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE, LOG_ERRORES,2  );
                    }
                }
            }
            else {
                //valor_double = GET_MEMORY_DOUBLE( cod_variable );
                valor_double = _GET_MEMORY_DOUBLE( cod_variable, 0 );
            }
            if( picture ) {
                FORMAT_FLOTANTE( picture, valor_double, buffer );
            }
            else {
                sprintf( buffer,"%lf",/*ENTEROS+DECIMALES,DECIMALES,*/valor_double );
            }
            break;
        case _STRING:
            memset( aux, 0, sizeof( aux ) );
            if( array > 1 ) {
                if( indice < array ) {
                    strncpy( aux, _GET_MEMORY_STR( cod_variable, indice ), sizeof( aux ) );
                }
                else {
                    GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE, LOG_ERRORES,2  );
                }
            }
            else {
                strncpy( aux, GET_MEMORY_STR( cod_variable ), sizeof( aux ) );
            }
            if( picture ) {
                FORMAT_CADENA( picture, aux, buffer );
            }
            else {
                strncpy( buffer, aux, sizeof( aux ) );
            }
            break;
        case _DATE:
            memset( aux, 0, sizeof( aux ) );
            fecha = 0;
            if( array > 1 ) {
                if( indice < array ) {
                    fecha = _GET_MEMORY_INT( cod_variable, indice );
                    DTOC4( fecha, aux );
                }
                else {
                    GRABAR_LOG_SISTEMA_STRING( S_ERROR_EN_SUBINDICE, LOG_ERRORES,2  );
                }
            }
            else {
                fecha = GET_MEMORY_INT( cod_variable );
                DTOC4( fecha, aux );
            }
            if( picture ) {
                FORMAT_FECHA( picture, fecha, aux );
				strncpy( buffer, aux, sizeof( aux ) );
            } else {
                strncpy( buffer, aux, sizeof( aux ) );
            }
            break;
    }
}
/***************************************************************************/
void VALIDA_SET_SALIDA_CUPON( int destino )
/***************************************************************************/
{
    destino_cupon = destino;
}
/***************************************************************************/
int VALIDA_GET_SALIDA_CUPON()
/***************************************************************************/
{
    return( destino_cupon );
}
/***************************************************************************/
void VALIDAR_T_ESPERA( int cod )
/***************************************************************************/
{
    /*------------------ Inicializaciones ---------------*/
    valida.fecha = GET_FECHA_DOS();
    valida.hora = GET_HORA_DOS();
    VALIDAR( cod, _VAL_T_ESPER, 0, 0, 0 );
}
/***************************************************************************/
int VALIDA_OBTENER_GRUPO_CUOTA( char *mensaje )
/***************************************************************************/
{
    int rta = 0;
    struct _cuo_gru cuo_gru;
    struct
    {
        unsigned short cod_tarjeta;
        int cod_grupo;
    }ind_cuo_gru;
    /*---------------------------- Abre el archivo ---------------------------*/
    if( OPEN_TABLE( T_CUO_GRU, TT_ORIG, ( char* )&cuo_gru, sizeof( struct _cuo_gru ) )
     == 0 ) {
        ind_cuo_gru.cod_tarjeta = (unsigned short)VAR_TARJETAS_TARJETA + 1;
        ind_cuo_gru.cod_grupo = (short)VAR_TARJETAS_GRUPO_CUOTA;
        //GET_EQUAL( ( char* )&ind_cuo_gru );
		SET_WHERE( "COD_TARJETA = '%i' AND COD_GRUPO = '%i'", ind_cuo_gru.cod_tarjeta,
			ind_cuo_gru.cod_grupo );
		RUN_QUERY( NO );
        if( FOUND2() ) {
            rta = 1;
            strncpy( mensaje, cuo_gru.descripcion, sizeof( cuo_gru.descripcion ) );
        }
        CLOSE_TABLE( T_CUO_GRU, TT_ORIG );
    }
    return( rta );
}
/*****************************************************************************/
long CALCULA_CODIGO_VERIFICACION( int caja, int fecha, long ticket, int digito, int sucursal )
/*****************************************************************************/
{
    long codigo;
    long *p,temp;
    float t;
    //char      tmp[100];
    t = 0;
    t += ( float )( sucursal * 47.07 );
    t += ( float )( fecha * .4 );
    t += ( float )( caja * .89 );
    t += ( float )( ticket * 3.573 );
    t += ( float )( digito * 13.34 );
    p = ( long* )&t;
    codigo = *p;
    codigo += ( ( long )sucursal ) << 17;
    codigo += 1007L * ( caja * 407l );
    codigo += ( fecha * 11L ) << 13;
    codigo += ( ticket * 37 ) << 15;
    // bajo el bit mas significativo para que hayan numeros negativos
    temp = ( 0xffffffff >> 1 );
    codigo &= temp;
    return codigo;
}
/******************************************************************************/
void CARGAR_DATOS_D_IMPRESION( int posic, int cod_sol, int nro_orden,
								int modalidad, int borra_arch_impresion, int reimpresion )
/******************************************************************************/
{
    /*
    MODALIDAD: Si es 1 quiere decir que el dato adicional se pidio en la misma
               validacion,
               Si es 0 quiere decir que se pidio la carga del dato adicional
               cuando se ingreso el medio de pago.
    */
    int pos = 0;
    char archivo[30];
    struct _datos datos_aux;
    struct _datos_en_bin datos_en_bin;
    int handle = -1, salir = 0, codigos_en_archivo = 0;
    SET_MEMORY_INT( __ram_cant_datos_adic_medio, 0 );
    BIN_DATOS_ADICIONALES( modalidad ? D_VALIDACIONES : D_MEDIOS_PRE_IMPORTE/*D_MEDIOS*/, modalidad ? 0 : posic, archivo );
    if( reimpresion ) {
        strncat(archivo,".bak",(sizeof(archivo) - (strlen(archivo) - 1)));
    }
    if( access( archivo, 0 ) == 0 ) {
        handle = _OPEN( archivo, O_RDONLY, __LINE__, __FILE__ );
        if( handle >= 0 ) {
            codigos_en_archivo = filelength( handle ) / sizeof( datos_en_bin );
			if( !salir && codigos_en_archivo > 0 && !borra_arch_impresion ) {
				pos = INDICE_IMPRESION;
			}
            while( !salir && codigos_en_archivo > 0 ) {
                if( !_READ( handle, ( char* )&datos_en_bin, sizeof( datos_en_bin ), __LINE__,
                            __FILE__ ) ) {
                    if( datos_en_bin.cod_solicitud == cod_sol
                     && datos_en_bin.nro_orden == nro_orden ) {
                        memcpy( &datos_aux.valor, datos_en_bin.valor,
                                sizeof( datos_en_bin.valor ) );
                        AGREGA_DATOS_MEDIO( 0, ( char )pos, nro_orden, 0, &datos_aux, cod_sol,
                                            D_IMPRESION, ( pos == 0 ), 0.0 );
                        INDICE_IMPRESION++;
                        pos++;
                        ADD_MEMORY_INT( __ram_cant_datos_adic_medio, 1 );
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
    //#if defined(INVEL_L)
    if(!reimpresion){
        char archivo_bak[30];
        memcpy(archivo_bak,archivo,sizeof(archivo_bak));
        //hacemos un bakup del archivo por si hay que reimprimir
        strncat(archivo_bak,".bak",(sizeof(archivo_bak) - (strlen(archivo_bak) - 1)));
        unlink( archivo_bak );
        COPIAR_ARCHIVO( archivo, archivo_bak, __LINE__, __FILE__ );
        AGREGA_BIN_EV_DMEDIO_A_BTRV( 0, D_VALIDACIONES );
    }
    /*#else
        AGREGA_BIN_EV_DMEDIO_A_BTRV( 0 , D_VALIDACIONES, NO, NO );
    #endif*/
}
/***************************************************************************/
void VALIDAR_COBROS_PAGATODO( struct DatosCobroPagaTodo *datos_cobros, int modo )
/***************************************************************************/
{
	int cant_dig = 0;

	_SELECCION_PAPEL = RECEIPT;
	if( datos_cobros->_cod_valida ) {
		memset( &valida, 0, sizeof( valida ) );
		valida.fecha = GET_FECHA_DOS();
		valida.hora = GET_HORA_DOS();
		valida.importe = datos_cobros->_importe;
		valida.nro_cliente = 0;
        valida.cod_medio = 0;
        /*sprintf( valida.mensaje_largo, "%s", ( datos_cobros->__devolucion )
			? "  ************ DEVOLUCION ************  DEVOLUCION COBRO PAGATODO" :	"COBRO PAGATODO" );*/
		_snprintf(valida.mensaje_largo, sizeof(valida.mensaje_largo)-1,"%s", ( datos_cobros->__devolucion )
			? "     DEVOLUCION        COBRO PAGATODO" :	"   COBRO PAGATODO" );
		valida.costo_servicio = datos_cobros->_costo_servicio +
			datos_cobros->_iva_costo_servicio;
		strncpy( valida.cuenta_ext, datos_cobros->_cuenta,
			sizeof( valida.cuenta_ext ) );
		TRIM(datos_cobros->_cod_producto);
		cant_dig = strlen( datos_cobros->_cod_producto );
		strncpy( valida.cuenta, datos_cobros->_cod_producto, cant_dig );
		valida.cuenta[cant_dig] = '\0';
		strncpy( valida.confirmacion, datos_cobros->_confirmacion_original,
			sizeof( valida.confirmacion ) );
		strncpy( valida.folio, datos_cobros->_folio, sizeof( valida.folio ) );
		strncpy( valida.NIP, datos_cobros->_nip, sizeof( valida.NIP ) );
		strncpy( valida.mensaje, datos_cobros->_nombre_cobro,
			sizeof( valida.mensaje ) );
		if( datos_cobros->__devolucion ) {
			valida.hora_larga = datos_cobros->_hora_original;
		} else {
			valida.hora_larga = datos_cobros->_hora;
		}
		strncpy( valida.codigo_llavero, datos_cobros->_codClientePagatodo,
			sizeof( valida.codigo_llavero ) );
		VALIDAR( datos_cobros->_cod_valida, _VALCOBROS, 0, 0, modo );
	}
}
/******************************************************************************/
void IMPRIMIR_LINEAS_MENSAJE( long id_transaccion, char nro_cuerpo )
/******************************************************************************/
{
	int   /*a_ant*/ tabla_ant, tipo_ant;
	struct _lin_mens lin_mens;
	/*struct {
		long id_transaccion;
		char nro_cuerpo;
		char nro_linea;
	} indice_lin;*/

	//a_ant = SELECTED(  );
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	/*if( USE_DATABASE( AREA_LIN_MENS, "lin_mens.tmp", ( char * ) &lin_mens,
			sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens, sizeof( struct _lin_mens))== 0 ) {
		//SET_ORDER( 2 );
		SET_ORDER2("ID_TRANSACCION, NRO_CUERPO, NRO_LINEA");
		/*indice_lin.id_transaccion = id_transaccion;
		indice_lin.nro_cuerpo = 0;
		indice_lin.nro_linea = 0;
		GET_EQUAL( ( char * ) &indice_lin );*/
		SET_WHERE("ID_TRANSACCION = '%ld' AND NRO_CUERPO = '%i' AND NRO_LINEA = '%i' ",  id_transaccion, 0, 0 );
		RUN_QUERY(NO);
		while( !BtrvEOF(  ) && lin_mens.id_transaccion == /*indice_lin.*/id_transaccion ) {
			if( lin_mens.nro_cuerpo == nro_cuerpo ) {
				MOSTRAR_CADENA( columna, PRN, lin_mens.descripcion );
				LF( 1 );
                //_DELETE(  );
			}
			SKIP( 1 );
		}
		GO( TOP );
		/*while( !BtrvEOF(  ) ) {
			_DELETE(  );
		}*/
		CLOSE_DATABASE( AREA_LIN_MENS );
	}
	//SELECT( a_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/***************************************************************************/
void VALIDAR_COBROS_GARANTIA( struct _datos_cobro *datos_cobros, int modo )
/***************************************************************************/
{ //terminar
	_SELECCION_PAPEL = RECEIPT | JOURNAL;
	if( datos_cobros->codigo ) {
		/*------------------ Inicializaciones ---------------*/
		memset( &valida, 0, sizeof( valida ) );
		valida.fecha = GET_FECHA_DOS();
		valida.hora = GET_HORA_DOS();
		valida.importe = datos_cobros->importe;
		valida.nro_cliente = 0;
        valida.cod_medio = 0;
		_snprintf(valida.cuenta, sizeof(valida.cuenta),"%d", datos_cobros->anios_garantia );
		valida.cuenta[sizeof( valida.cuenta )- 1] = '\0';
       /* sprintf( valida.mensaje_largo, "%s", ( datos_cobros->devolucion )
			? "  ************ DEVOLUCION ************  DEVOLUCION COBRO GARANTIA" :	"COBRO GARANTIA" );*/
		_snprintf(valida.mensaje_largo, sizeof(valida.mensaje_largo)-1,"%s", ( datos_cobros->devolucion )
			? "     DEVOLUCION        COBRO GARANTIA" :	"   COBRO GARANTIA" );
		/*--------------------------Para Cobros Venta----------------------------*/
		if( RAM_FLAG_COBRO_VENTA )
			ASIGNA_CODIGO_BARRA( datos_cobros->cod_barra_articulo );
		/*-----------------------------------------------------------------------*/
		strncpy( valida.mensaje, ITEM_NOMBRE_LARGO, sizeof( valida.mensaje ) );
		strncpy( valida.folio, datos_cobros->cod_barra, sizeof( valida.folio ) );
		strncpy( valida.cod_barra, datos_cobros->cod_barra_articulo, sizeof( datos_cobros->cod_barra_articulo ) );
		VALIDAR( cobros.cod_valida, _VALCOBROS, 0, 0, modo );  //xxxxxxxxxxxxxxx  es el cod_validacion aca y no el del cobro
	}
}
/******************************************************************************/
void VALIDAR_COBROS_KINACU( struct _datos_cobro_kinacu *datos_cobro_kinacu, int modo, int posicion )
/******************************************************************************/
{
    int  /*a_ant,*/ a_aux, tabla_ant, tipo_ant;
    struct _lin_mens lin_mens;
	struct _datos_cobro_kinacu dck;
    struct {
        long id_transaccion;
        char nro_linea;
    } indice_lin;

    if( datos_cobro_kinacu->cod_valida ) {
		_SELECCION_PAPEL = RECEIPT;
        memset( &valida, 0, sizeof( valida ) );
        valida.fecha = GET_FECHA_DOS(  );
	    valida.hora = GET_HORA_DOS(  );
		strncpy( valida.telefono, datos_cobro_kinacu->nro_linea, sizeof( valida.telefono ) );
	    valida.importe = datos_cobro_kinacu->monto;
        valida_kinacu_id_transaccion = datos_cobro_kinacu->id_transaccion;
		strncpy( valida.mensaje2, datos_cobro_kinacu->mensaje1, sizeof( valida.mensaje2 ) );
        VALIDAR( datos_cobro_kinacu->cod_valida, _VALCOBROS, 0, 0, modo );
        //a_ant = SELECTED(  );
		tabla_ant = SELECTED2();
		tipo_ant = SELECTED_TIPO();

        a_aux = AREA_DISPONIBLE(  );
        /*if( USE_DB( a_aux, _LIN_MENS_TMP, ( char * ) &lin_mens,
                sizeof( struct _lin_mens ), NULL, 0, 0 ) == 0 ) {*/
		if( OPEN_TABLE(T_LIN_MENS_TMP, TT_TEMP,( char* )&lin_mens, sizeof( struct _lin_mens))== 0 ) {
            //SET_ORDER( 1 );
			SET_ORDER2("ID_TRANSACCION, NRO_LINEA");
            indice_lin.id_transaccion = datos_cobro_kinacu->id_transaccion;
            indice_lin.nro_linea = 0;
            //GET_EQUAL( ( char * ) &indice_lin );
			SET_WHERE("ID_TRANSACCION = '%ld' AND NRO_LINEA = '%i' ",  indice_lin.id_transaccion, indice_lin.nro_linea );
			RUN_QUERY(NO);
            while( /*!BtrvEOF()*/ !dbEOF() && lin_mens.id_transaccion == indice_lin.id_transaccion ) {
		        //_DELETE(  );
				DELETE2(  );
		    }
		    //CLOSE_DATABASE( a_aux );
			CLOSE_TABLE( T_LIN_MENS_TMP, TT_TEMP );
        }
	    //SELECT( a_ant );
		SELECT_TABLE( tabla_ant, tipo_ant );
        /*------------------------------- DELETE -------------------------------*/
		memset( (char*)&dck, 0, sizeof( struct _datos_cobro_kinacu ) );
	    ACTUALIZAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU, (char*)&dck,
	            sizeof( struct _datos_cobro_kinacu ), posicion );
        ADD_MEMORY_LONG( __ram_nro_pedido_pin, - 1L );
    }

}
/***************************************************************************/
int IMPRIMIR_COMPROBANTE_VAL( int tipo, int reimpresion )
/***************************************************************************/
{
    char init_acu;
    int j = 0, rta, h, cant, tipo_evento_comp = 0;
    double _neto = 0.00, control_total = 0.00, tot_imp = 0.00;
    float _neto_percep_iva1 = 0.00, _neto_percep_iva2 = 0.00;
    int renglones_configuracion, i = 0;
    int subtipo, tipo_comprobante = 0, cond_IVA, reimpresion_tmp = 0;
    int reglon_backup = 0, k = 0, salir = 0,cant_renglones = 0;
    double importe_sin_redondear = 0.00, importe_redondeado = 0.00, diferencia_redondeo=0.00, iva_sin_redondear = 0.00;
		int validacion = 0;

		valida.fecha = GET_FECHA_DOS();
	valida.hora = GET_HORA_DOS();
	valida.nro_cliente = RAM_NRO_CLIENTE;
    valida.numero = NRO_EVENTO - 1;
    valida.cod_medio = 0;
    valida.autorizacion = 0L;
    strcpy( valida.mensaje, cobros.nombre );
    strcpy( valida.telefono, "" );
    strcpy( valida.cuit, "" );
    valida.comprobante = event_e_ticket.comprobante;//Comprobante dejo de ser long para ser double
	strncpy( valida.cod_barra, event_e_ticket.cod_barra,
				sizeof( event_e_ticket.cod_barra ) - 1 );

    fila = 1;
    columna = 1;
       /*------------------ Tipo de comprobante -------------------*/
   	tipo_comprobante = TIPO_COMPROBANTE( &subtipo );
	tipo_evento_comp = TIPO_EVENTO( NO, tipo_comprobante );
	cond_IVA = OBTENER_COND_IVA_CLIENTE( 0 );
	/*
	tipo_comprobante=
	#define _FACTURA        0
	#define _NOTA_CREDITO   1
	#define _REMITO         2
	#define _FAC_REM        3
	#define _TKT            4
	#define _TICKET         5
	#define _TICKET_FACTURA 6
	#define _CREDITO_FISCAL 7
	#define _FACTURA_CONS_F 8
	*/

    /*------------------ Inicializaciones ---------------*/
    //REIMPRESION = reimpresion;

    if( !reimpresion ) {
        if( !_VERIFICAR_EVENTOS_DEL_COMPROBANTE( tipo , &v_cant_pagos,&cant_renglones ) || !EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
            return ( 0 );
        }
		valida.tipo_comp = tipo_comprobante;
		valida.cond_cli = cond_IVA;
        valida.subtipo = subtipo;
        valida.nro_suc = event_ticket.suc_comprobante;
        valida.nro_comp = event_ticket.nro_comprobante_new;
    }

    if( CUERPOS == NULL ) {
        CUERPOS = calloc( MAX_CUERPOS, sizeof( struct _cuerpo ) );
        if( CUERPOS == NULL ) {
            MENSAJE_STRING( S_MEMORIA_INSUFICIENTE_PARA_EMITIR_COMPROBANTE );
            return ( 0 );
        }
    }

	//struct _art_fact *art_desc;



	if( !AGRUPAR_ARTICULOS(&cant_renglones, &art_fact_val, &art_fact_iva ,&art_fact_iva_bac, &art_desc)){
	//if( !AGRUPAR_ARTICULOS( &cant_renglones ) ) {
        MENSAJE_STRING( S_MEMORIA_INSUFICIENTE_PARA_EMITIR_COMPROBANTE );
        return ( 0 );
    }


	valida.fecha = event_ticket.fecha;
	valida.hora = event_ticket.hora;
	valida.nro_cliente = event_ticket.nro_cliente;
	valida.cant_formularios = _CANTIDAD_FORMULARIOS( cant_renglones, tipo_comprobante );

	cant = ( COMP_INDEP ) ? valida.cant_formularios : 1;
    SET_MEMORY_INT( __ram_cant_comp, cant );
    signo_nc = 1;
    rta = OBTENER_DATOS_COMPROBANTE( tipo, valida.subtipo, cant, &valida.nro_suc, &valida.nro_comp,
                                     &valida.copias, SI, NO );
	strcpy( valida.telefono_comprador, clientes.telefono );
	strcpy( valida.telefono_comprador, clientes.telefono );
	strcpy( valida.cuit, clientes.cuit );

	if( rta ) {
        if( IMPRESORA_FISCAL && ( tipo != _REMITO ) && !PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
            if( config.pais != ELSALVADOR ) {
                IMPRIMIR_TICKET_FACTURA( ( COMP_INDEP )
                    ? valida.cant_formularios : 1 );
            }
		if( IMPRESORA_FISCAL == FISCAL)//seba
			{
			ABRIR_NOTA_DE_CREDITO('P',RESP_INSC,cond_IVA,CLIENTE_AFINIDAD_NOMBRE, clientes.nombre, __CUIT,clientes.cuit,clientes.domicilio,clientes.localidad,clientes.provincia,"abc"/*clientes.empresa*/,"abc"/*clientes.telefono*/ );//Seba
			}
		}

        if( PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
            PRN_HABILITAR_IMPRESORA_OPCIONAL();
        }
		for( orden = 0;orden < valida.copias; orden++ ) {
            init_acu = SI;
            nro_comp = valida.nro_comp;
            renglon = 0;
            nro_cuerpo = 0;
            subtotal_backup = 0.00;
   			exento_backup = 0.00;
   			imp_int_backup = 0.00;
   			iva1_backup = 0.00;
   			iva2_backup = 0.00;
   			for( i = 0;i < 4;i++ ) {
                imp_monto_impuesto_back[i] = 0;
            }
   			neto_backup = 0.00;
   			total_backup = 0.00;
   			retencion_dgr_backup = 0.00;
   			percepcion_dgi_backup = 0.00;
   			percep_212_iva1_backup = 0.00;
   			percep_212_iva2_backup = 0.00;
   			memset( total_impuestos_backup, 0, sizeof(total_impuestos_backup));
   			memset( art_fact_iva_bac, 0, sizeof( struct _art_fact_ivax ) * MAX_COD_IVA );

            /*----------------------- imprime formulario ----------------------*/
			for( j = 0;j < valida.cant_formularios;j++ ) {
                /*if( IMPRESORA_FISCAL || TIPO_IMPRESORA ) {
                    //Luciano recoradar no se lee TIPO_IMPRESORA RNV
                    #if defined(INVEL_W) || defined(INVEL_L)
                    int rta = 0;
                    /*getData( ACTION_SLP_EMPTY, ( char* )&rta, 0 );
                    while( !rta ) {
                        MENSAJE_TECLA( "RETIRE EL PAPEL DE LA IMPRESORA Y PRESIONE ENTER" );
                        getData( ACTION_SLP_EMPTY, ( char* )&rta, 0 );
                    }*/
                    MENSAJE_CON_TECLA( ST( S_COLOQUE_FORMULARIO_EN_IMPRESORA_PRES_ENTER ), 13 );

					/*rta = 1;
                    while( rta ) {
                        getData( ACTION_SLP_EMPTY, ( char* )&rta, 0 );
                    }*/
                    //#else

                    if( PEDIR_FORMULARIO_CON_TECLA ) {
                         MENSAJE_CON_TECLA( ST( S_COLOQUE_FORMULARIO_EN_IMPRESORA_PRES_ENTER ), 13 );
                    }
                    else {
                        MENSAJE_STRING( S_COLOQUE_EL_FORMULARIO_EN_LA_IMPRESORA );
                    }

					//#endif
                //}

                _hoja_actual = j + 1;

                /* if( !PUERTO_IMPRESORA_OPCIONAL_FACTUR )  // Se comenta, para que la habilitacion del slip se por archivo de confg
				{
                    //PRN_HABILITA_SLIP( 1, NO );
                }*/

				if( COMP_INDEP || init_acu ) {
                    if( !backup_totales ) {
                        subtotal = 0.0;
                        exento = 0.0;
                        imp_int = 0.0;
                        iva1 = 0.0;
                        iva2 = 0.0;
                        neto = 0.0;
                        total = 0.0;
                        for( i = 0;i < 4;i++ ) {
                            imp[i] = 0.0;
                        }
                        retencion_dgr = 0.0;
                        percepcion_dgi = 0.0;

                        percep_212_iva1 = 0.0;
                        percep_212_iva2 = 0.0;

                        cant_total = 0.0;
                        costo = 0.0;

                        memset( total_impuestos, 0, sizeof( total_impuestos ) );
                        memset( art_fact_iva, 0, sizeof( struct _art_fact_ivax ) * MAX_COD_IVA );
                    }
                    init_acu = NO;
                }

                validacion = VALIDACION_DEL_COMPROBANTE(tipo_comprobante, tipo_evento_comp, _V_CABECERA);
                VALIDAR( validacion, _VAL_COMPROBANTES, 0, 0, 0 );
                /*----------------------- Cuerpo de la factura ---------------------*/
                h = 0;
                //art_descuento.precio = 0;
                renglones_configuracion = ( DESCUENTOS_POR_CUERPO )? CANT_RENGLONES[_tipo] - 1 : CANT_RENGLONES[_tipo];
                if( tipo == _FACTURA )
					renglones_configuracion = CANT_RENGLONES[_FACTURA];
				if( tipo == _NOTA_CREDITO )
					renglones_configuracion = CANT_RENGLONES[_NOTA_CREDITO];
                reglon_backup = renglon;

				while( renglon < cant_renglones && h < renglones_configuracion ) {
                    if( fabs( art_fact_val[renglon].cantidad ) > 0.001 ) {
                        /*----------------- Resta descuento  ----------------------------*/
                        if( DESCUENTOS_POR_CUERPO ) {
                            CALCULAR_VALOR_CADA_ARTICULO_RESTANDO_DESCUENTOS( renglon );
                        }

                        /*----------------- Barro el art_fact  ----------------------------*/
                        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                            BUSCA_COD_INTERNO_COD_BARRA( art_fact_val[renglon].cod_articulo,
                                                         art_fact_val[renglon].cod_barra );
                        } else {
                            BUSCA_CODIGO_ESPECIAL( art_fact_val[renglon].cod_articulo );
                        }
                        //SELECT( AREA_E_TICKET );
                        SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
                        //GET_DIRECT( art_fact_val[renglon].pos );
                        GET_DIRECT_REG( art_fact_val[renglon].pos, 0 );

                        cantidad = art_fact_val[renglon].cantidad;
                        precio = art_fact_val[renglon].precio;

						if( ES_FACTURA_A( valida.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                                for( i = 0;i < 4;i++ ) {
                                    tot_imp = art_fact_val[renglon].monto_impuesto[i];
                                }
                                precio_final =
                                        ROUND( art_fact_val[renglon].neto_gravado
                                        + art_fact_val[renglon].exento
                                        + art_fact_val[renglon].imp_int, ENTEROS, DECIMALES )
                                        + ROUND( art_fact_val[renglon].iva1
                                        + art_fact_val[renglon].iva2 + tot_imp, ENTEROS, DECIMALES );
						} else {
                                precio_final =
                                        ROUND( art_fact_val[renglon].neto_gravado
                                        + art_fact_val[renglon].exento
                                        + art_fact_val[renglon].imp_int + art_fact_val[renglon].iva1
                                        + art_fact_val[renglon].iva2, ENTEROS, DECIMALES );
                        }
						if( ES_FACTURA_A( valida.cond_cli ) && !FACTURA_CON_PRECIO_FINAL ) {
                            importe =
                                    art_fact_val[renglon].neto_gravado
                                    + art_fact_val[renglon].exento
                                    + art_fact_val[renglon].imp_int;
							iva_sin_redondear = art_fact_val[renglon].iva1;

                            #ifdef DEBUG_R
                            if( MODO_DEBUG == _DEBUG_REDONDEO ) {
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                _snprintf( cadena_debug, 80, "14.1.Comprob, art=%li,pr=%lf,cant=%f,imp=%lf",
                                         art_fact_val[renglon].cod_articulo, art_fact_val[renglon].precio,
                                         art_fact_val[renglon].cantidad, importe );
                                cadena_debug[79] = '\0';
                                //GRABAR_LOG_SISTEMA( cadena_debug );
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                _snprintf( cadena_debug, 80, "14.2.Comprob, %lf, pr=%lf,iva=%lf,imp=%lf, dif=%lf",
                                         precio_final, art_fact_val[renglon].neto_gravado,
                                         art_fact_val[renglon].iva1,
                                         ROUND( importe, ENTEROS, DECIMALES ),
                                         ROUND( importe, ENTEROS, DECIMALES ) - importe );
                                cadena_debug[79] = '\0';

                                GRABAR_LOG_SISTEMA( cadena_debug );
                            }
                            #endif
                            if(!reimpresion_tmp){
						    importe_sin_redondear = importe;
                            importe = ROUND( importe, ENTEROS, DECIMALES );
							importe_redondeado = importe;
							diferencia_redondeo = ( fabs( importe_sin_redondear ) ) - ( fabs( importe_redondeado ) );
							/*if(RAM_NOTA_CR)
								diferencia_redondeo *= -1;*/
							if(art_fact_val[renglon].neto_gravado != 0) // articulo gravado
								ADD_MEMORY_DOUBLE( __ram_diferencia_redondeo_grav, diferencia_redondeo );
							if(art_fact_val[renglon].exento != 0) // articulo exento
								ADD_MEMORY_DOUBLE( __ram_diferencia_redondeo_exent, diferencia_redondeo );

							    diferencia_redondeo = ( fabs( iva_sin_redondear ) ) - ( fabs( ROUND( iva_sin_redondear, ENTEROS, DECIMALES ) ) );
							    ADD_MEMORY_DOUBLE( __ram_diferencia_redondeo_iva, diferencia_redondeo );
                            }

						} else {
                            importe = precio_final;

                            #ifdef DEBUG_R
                            if( MODO_DEBUG == _DEBUG_REDONDEO ) {
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                sprintf( cadena_debug,
                                         "14.3.Comprob, art=%li,pr=%lf,cant=%f,imp=%lf",
                                         art_fact_val[renglon].cod_articulo, art_fact_val[renglon].precio,
                                         art_fact_val[renglon].cantidad, importe );
                                //GRABAR_LOG_SISTEMA( cadena_debug );
                                memset( cadena_debug, 0, strlen( cadena_debug ) );
                                sprintf( cadena_debug, "14.4.Comprob, pr=%lf,imp=%lf, dif=%lf",
                                         art_fact_val[renglon].neto_gravado,
                                         ROUND( importe, ENTEROS, DECIMALES ),
                                         ROUND( importe, ENTEROS, DECIMALES ) - importe );
                                GRABAR_LOG_SISTEMA( cadena_debug );
                            }
                            #endif
                        }
                        /*------------------------- Acumula ----------------------------*/
                        cant_total += cantidad;
                        subtotal += importe;
                        exento += art_fact_val[renglon].exento;
                        costo += art_fact_val[renglon].costo;
                        imp_int += art_fact_val[renglon].imp_int;
                        iva1 += art_fact_val[renglon].iva1;
                        iva2 += art_fact_val[renglon].iva2;
                        for( i = 0;i < 4;i++ ) {
                            imp[i] += art_fact_val[renglon].monto_impuesto[i];
                        }
                        neto += art_fact_val[renglon].neto_gravado;
                        //total += precio_t;
                        total += precio_final;

						if( art_fact_val[renglon].cod_iva < MAX_COD_IVA ) {
                            art_fact_iva[art_fact_val[renglon].cod_iva].neto_gravado +=
                                art_fact_val[renglon].neto_gravado;
                            art_fact_iva[art_fact_val[renglon].cod_iva].iva1 +=
                                art_fact_val[renglon].iva1;
                            art_fact_iva[art_fact_val[renglon].cod_iva].iva2 +=
                                art_fact_val[renglon].iva2;
                        } else {
                            MENSAJE_STRING( S_COD_IVA_INVALIDO );
                        }

                        /*------------ Acumula ILA e IHA ------------*/
                        if( event_e_ticket.cod_ext ) {
                                CARGA_TOTAL_IMPUESTOS( total_impuestos, event_e_ticket.cod_ext );
                        } else {
                            for( i = 0;i < 4;i++ ) {
                                if( articulo.cod_impuesto[i] ) {
                                    total_impuestos[articulo.cod_impuesto[i] - 1] +=
                                    art_fact_val[renglon].monto_impuesto[i];
                                }
                            }
                        }

						/*-----------------Se deben imprimir positivos ------------*/
                        if( DESCUENTOS_POR_CUERPO
                         && ( config.pais == CHILE || config.pais == BRASIL )
                         && ( RAM_NOTA_CR || RAM_REFERENCIA_NC || RAM_MODO_DEVOLUCION ) ) {
                            signo_nc = -1;
                        }

						/*-------------------------- Cabecera ---------------------*/
						validacion = VALIDACION_DEL_COMPROBANTE(tipo_comprobante, tipo_evento_comp,_V_CUERPO);
						VALIDAR( validacion , _VAL_COMPROBANTES, 0, 0, 0 );


                        COMP_SET_FILA( fila + 1 );
                        h++;
                    }
                    renglon++;
                }
				/*------------------------- Imprime totales ------------------------*/
				if( j == valida.cant_formularios - 1 || COMP_INDEP ) {
                    /*----- Calculamos retenciones --------*/
                    if( COMP_INDEP ) {
                        _neto += neto;
                    }

					if( j == valida.cant_formularios - 1 ) {
                        if( !COMP_INDEP ) {
                            _neto = neto;
                        }
                        if( CALCULAR_RETENCION_DGR
                                && !RAM_BIEN_DE_USO
                                && ( valida.tipo_comp == _FACTURA
                                    || valida.tipo_comp == _NOTA_CREDITO )
                                //Salvador aplica la retencion a clientes Gubernamentales tanto
                                //al comprobante A como al B.
                                && ( valida.subtipo == TIPO_A
                                    || clientes.cond_iva == INST_GUBERNAMENTAL )
                                && fabs( _neto ) >= min_retenc_dgr
                                && ( clientes.exento_ing_brutos == 'N'
                                    || clientes.exento_ing_brutos == 'n' ) ) {
                            retencion_dgr =
                                ROUND( _neto * porc_retenc_dgr / 100, 10, DECIMALES );
                            total += retencion_dgr;
                        }
                        /*Percepcion DGI- Control para cliente dgi = 'N' -----*/
                        /*-----Modalidad bien de Uso -- No calcula Percepcion */
                        if( CALCULAR_PERCEPCION_DGI && !RAM_BIEN_DE_USO
                                && ( valida.tipo_comp == _FACTURA || valida.tipo_comp == _NOTA_CREDITO )
                                && valida.subtipo == TIPO_A
                                && fabs( _neto ) >= min_percep_dgi
                                && clientes.realizar_percepcion_dgi != 'N' ) {
                                percepcion_dgi =
                                ROUND( _neto * porc_percep_dgi / 100, 10, DECIMALES );
                                total += percepcion_dgi;
                        }
                        //---Actualizacion monotributo
                        _neto_percep_iva1 =
                            ( float )( art_fact_iva[0].neto_gravado + art_fact_iva[0].iva1 )
                            * ( ( iva[0].porc_iva1 / 2 ) / 100 );
                        _neto_percep_iva2 =
                            ( float )( art_fact_iva[1].neto_gravado + art_fact_iva[1].iva1 )
                            * ( ( iva[1].porc_iva1 / 2 ) / 100 );
                        if( valida.cond_cli == RESP_NO_IDEN && valida.tipo_comp == _FACTURA ) {
                            percep_212_iva1 = ROUND( _neto_percep_iva1, 10, DECIMALES );
                            percep_212_iva2 = ROUND( _neto_percep_iva2, 10, DECIMALES );
                            total += ( percep_212_iva1 + percep_212_iva2 );
                        }
                            total = ROUND( total, 10, DECIMALES );
                    }

                    if( config.pais == CHILE /*&& valida.cant_formularios > 1*/
                            || config.pais == BRASIL ) {
                        control_total = total - ROUND( neto, ENTEROS, DECIMALES );
                        for( i = 0 ;i < 20 ;i++ ) {
                            control_total -= ROUND( total_impuestos[i], ENTEROS, DECIMALES );
                        }
                        iva1 = control_total;
                        CUERPOS[nro_cuerpo].nro_evento = event_ticket.nro_evento;
                        CUERPOS[nro_cuerpo].nro_sucursal = valida.nro_suc;
                        CUERPOS[nro_cuerpo].nro_comprobante = nro_comp;
                        CUERPOS[nro_cuerpo].exento = exento;
                        CUERPOS[nro_cuerpo].imp_int = imp_int;
                        CUERPOS[nro_cuerpo].iva1 = iva1;
                        CUERPOS[nro_cuerpo].iva2 = iva2;
                        for( i = 0;i < 0;i++ ) {
                            CUERPOS[nro_cuerpo].monto_impuesto[i] = imp[i];
                        }
                        CUERPOS[nro_cuerpo].retencion_dgr = retencion_dgr;
                        CUERPOS[nro_cuerpo].percepcion_dgi = percepcion_dgi;
                        CUERPOS[nro_cuerpo].neto = neto;
                        CUERPOS[nro_cuerpo].subtotal = subtotal;
                        CUERPOS[nro_cuerpo].total = total;
                        CUERPOS[nro_cuerpo].total_descuento = total_descuento;
                        CUERPOS[nro_cuerpo].bultos = cant_total;
                        CUERPOS[nro_cuerpo].costo = costo;
                        CUERPOS[nro_cuerpo].tipo = valida.subtipo;
                        CUERPOS[nro_cuerpo].nro_orden++;
                        for( i = 0 ;i < 20 ;i++ ) {
                            CUERPOS[nro_cuerpo].total_impuestos[i] = total_impuestos[i];
                        }
                        nro_cuerpo++;
                    }
					//validacion = VALIDACION_DEL_COMPROBANTE(tipo_comprobante,_V_CUERPO);
                    //VALIDAR( /*112*/ validacion , _VAL_COMPROBANTES, 0, 0, 0 );

					validacion = VALIDACION_DEL_COMPROBANTE(tipo_comprobante, tipo_evento_comp, _V_PIE);
					valida.importe = SUMAR_TOTAL_FAC_CAJA( NO) + RAM_RETENCION_DGR ;
                    VALIDAR( validacion , _VAL_COMPROBANTES, 0, 0, 0 );

				} else {
                    COMP_SET_FILA( FILA_CONT );
                    columna = COLUMNA_CONT;
					if( P_CAD_CONT_VAL!= NULL )
						MOSTRAR_CADENA( columna, PRN, P_CAD_CONT_VAL );
					//Expulso el papel
						LF( 1 );
						setData( ACTION_PRINT, NULL, 0 );
						EJECT();
						LF( 1 );
						setData( ACTION_PRINT, NULL, 0 );
						PRN_INHABILITA_SLIP();

					fila = 1;
					columna = 1;
			    }

                if( COMP_INDEP ) {
                    nro_comp++;
                }

                if( HARD_OMRON && !PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
                    #ifdef COMPILAR_OMRON
                    PRN_INHABILITA_SLIP();
                    OMRON_ESPERAR_QUE_SAQUEN_LA_HOJA();
                    #endif
                }
                SET_COLUMNA( 1 );
					 COMP_SET_FILA( 1 ); // seba

                #if defined( INVEL_W ) || defined( INVEL_L )
                LF( 0 );
                setData( ACTION_PRINT, NULL, 0 );
                #endif
                ///   rox aca preguntar si confirma o no el comprobante...
                if( CONFIRMAR_IMPRESION ) {
                    salir = 0;
                    MENSAJE( "Se imprimio correctamente? (Enter/Esc)" );
                    while( !salir ) {
                        k = GETCH(  );
                        while( k != 13 && k != 27 ) {
                            k = GETCH();
                        }
                        if( k == 27 || k == 13 ) {
                            salir = 1;
                        }
						if( k == 27 ) {
							fila = 1;
							columna = 1;
                            reimpresion_tmp++;
                            AGREGA_BIN_A_EXTENDIDO( _hoja_actual, D_VALIDACIONES, SI );
                            /* agrego esto para que reinicie los contadores del
                            comprobante cuando se imprime mal.*/
                            if( j == valida.cant_formularios - 1 || COMP_INDEP )
                                init_acu = SI;
                            j--;
                            renglon = reglon_backup;
                            if( !COMP_INDEP ) {
                                subtotal = subtotal_backup;
                                exento = exento_backup;
                                imp_int = imp_int_backup;
                                iva1 = iva1_backup;
                                iva2 = iva2_backup;
                                for( i = 0;i < 4;i++ ) {
                                    imp[i] += imp_monto_impuesto_back[i];
                                }
                                neto = neto_backup;
                                total = total_backup;
                                retencion_dgr = retencion_dgr_backup;
                                percepcion_dgi = percepcion_dgi_backup;
                                percep_212_iva1 = percep_212_iva1_backup;
                                percep_212_iva2 = percep_212_iva2_backup;
                                memcpy( art_fact_iva, art_fact_iva_bac,
                                        sizeof( struct _art_fact_ivax ) );
                                memcpy( &total_impuestos, total_impuestos_backup,
                                        sizeof( double ) * 20 );
                                backup_totales = 1;
                                }
                        } else {
                            //poner funcion nueva que pasa de bin a bin_extendido
                            AGREGA_BIN_A_EXTENDIDO(_hoja_actual,D_VALIDACIONES,NO);
                            if( !COMP_INDEP ) {
                                subtotal_backup = subtotal;
                                exento_backup = exento;
                                imp_int_backup = imp_int;
                                iva1_backup = iva1;
                                iva2_backup = iva2;
                                for( i = 0;i < 4;i++ ) {
                                    imp[i] += imp_monto_impuesto_back[i];
                                }
                                neto_backup = neto;
                                total_backup = total;
                                retencion_dgr_backup = retencion_dgr;
                                percepcion_dgi_backup = percepcion_dgi;
                                percep_212_iva1_backup = percep_212_iva1;
                                percep_212_iva2_backup = percep_212_iva2;
                                memcpy( art_fact_iva_bac, art_fact_iva,
                                sizeof( struct _art_fact_ivax ) );
                                memcpy( &total_impuestos_backup, total_impuestos,
                                sizeof( double ) * 20 );
                                backup_totales = 0;
                            }
                        }
                    }
                    BORRAR_MENSAJE( );
                }
            }
        }
        /*----------------- actualizo factu.mem ---------*/
        cant = nro_comp - valida.nro_comp - 1;
        OBTENER_DATOS_COMPROBANTE( tipo, valida.subtipo, cant, &valida.nro_suc, &valida.nro_comp,
                                   &valida.copias, SI, NO );
        /*--------------- Vuelve ---------------*/
        if( PUERTO_IMPRESORA_OPCIONAL_FACTUR ) {
            PRN_DESHABILITAR_IMPRESORA_OP();
            //PRN_INHABILITA_SLIP(  );
        } else if( !HARD_OMRON ) {
            if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
                if( valida.tipo_comp == _REMITO ) {
                    CERRAR_DOCUMENTO( 0, NO, 0 );
                }
                else {
                    CERRAR_DOCUMENTO( total, NO, 0 );
                }
            }
            PRN_INHABILITA_SLIP();
        }
        if( art_fact_val ) {
            FREEMEM( ( void* )art_fact_val );
        }
        if( art_fact_iva ) {
            FREEMEM( ( void* )art_fact_iva );
        }
        if( art_fact_iva_bac ) {
            FREEMEM( ( void * ) art_fact_iva_bac );
        }
        if( art_desc ) {
            FREEMEM( ( void* )art_desc );
        }
    } else {
        MENSAJE_STRING( S_ERROR_LEER_DATOS_PARA_EMITIR_COMPROBANTE );
    }

    SET_COLUMNA( 1 );

    if( config.pais == ELSALVADOR ) {
        //fabiana---lo comento hasta que funcione
        IMPRIMIR_COMPROBANTE_JOURNAL( tipo, reimpresion );
    }

    return ( rta );
}

