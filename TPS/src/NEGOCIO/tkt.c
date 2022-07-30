/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <cr_disco.h>
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <aarch.h>
#include <string.h>
#include <math.h>
#include <btrv.h>
#include <uti-comp.h>
#include "actions.h"
#include <_dd.h>
#include <myodbc.h>
#include <inf_dif.h>
#include <ini.h>
#include <tkt.h>
#include <db.h>
#include <deftables.h>
#include <tkt.h>
#include <cmemory.h>
#include <trim.h>
#include <ftoc.h>
#include <stack.h>
#include <fiscal.h>
#include <_cr1.h>
#include <m_cadena.h>
#include <get_fech.h>
#include <comprob.h>
#include <get_hora.h>
#include <m_fecha.h>
#include <cobros_esp.h>
#include <nume.h>
#include <imp_tick.h>
#include <lista.h>
#include <pago_tc.h>
#include <pago_mut.h>
#include <round.h>
#include <clientes.h>
#include <pagos.h>
#include <dd.h>
#include <valida.h>
#include <pausa.h>
#include <calcular.h>
#include <mensaje.h>
#include <log.h>
#include <actual.h>
#include <inf_tar.h>
#include <path.h>
#include <tarjetas.h>
#include <b_mens.h>
#include <recupero.h>
#include <name_mon.h>
#include <day.h>
#include <cont_sub.h>
#include <baucher.h>
#include <p_fecha.h>
#include <clave.h>
#include <imp_comp.h>
#include <dtoa.h>
/*--------------------------------- Globales -----------------------------*/
#define CANT_SUB_RET 50
int IMPRIMIR_TKT_INF_Y( int ini_est,int inicio_tkt,int fin_tkt );
void OBTENER_DATO_ARCHIVO_REC( char *ruta,int tam_struct, char *tipores, char offset[200]/*, int linea*/ );
static int ind_config = 0;
static int ind_comienzo_ticket = 0,ind_comienzo_cuerpo = 0,ind_comienzo_total = 0;
static int ind_comienzo_medios = 0,ind_comienzo_pie = 0,ind_comienzo_iva = 0,ind_comienzo_vuelto = 0,
           ind_comienzo_afinidad = 0,ind_comienzo_no_afinidad = 0,ind_comienzo_donacion = 0,ind_comienzo_ticket_espera = 0;
static int ind_comienzo_id_con_cliente = 0,ind_comienzo_id_sin_cliente = 0,ind_comienzo_titulo_ticket = 0;
static int ind_comienzo_comando_x = 0,ind_comienzo_comando_z = 0,ind_comienzo_comando_y = 0,
           ind_comienzo_impresora_fiscal = 0,ind_comienzo_comando_b = 0,ind_informe_diferencia = 0,
           ind_comienzo_corte_papel = 0,ind_comienzo_subtotal = 0,ind_comienzo_inf_00 = 0, ind_comienzo_inf_05 = 0,
		   ind_comienzo_cabecera_inf_10 = 0,ind_comienzo_inf_10 = 0,
		   ind_comienzo_cabecera_inf_15 = 0,ind_comienzo_inf_15 = 0,
		   ind_comienzo_tarjetas = 0,
           ind_comienzo_f_fijo = 0,ind_comienzo_timbradora = 0,ind_comienzo_inf_sup = 0,ind_comienzo_cab_inf_sup = 0,
           ind_comienzo_pago_reintegro = 0,ind_comienzo_timbrado_reintegro = 0,ind_informe_diferencia_submedios = 0,ind_comienzo_inf_y = 0;
static int ind_comienzo_cuerpo_cantidad = 0, ind_comienzo_cab_inf_dif_sub = 0, ind_comienzo_descripcion_cuerpo = 0,
		   ind_comienzo_descripcion_tarjeta = 0, ind_comienzo_importe_total = 0, ind_comienzo_franqueo = 0, ind_comienzo_franqueo_cobros = 0;
static int ind_comienzo_cuerpo_informacion_comp = 0, ind_comienzo_fin_ticket = 0;
static int ind_comienzo_duplicado_tarjeta =0, ind_comienzo_retiro = 0, ind_comienzo_ing_cajero = 0, ind_comienzo_retiro_cierre = 0; //Seba

static int ticks_b = 0;
static int med_tarj = 0;
static INT16 tkt_fila;
int tkt_columna = 0;
static int fiscal_b = 0;
static INT16 TKT_OFFSET_COLUMNA = 0;
static char SI_MODO_ENTRENAMIENTO,SI_MODO_INVENTARIO,SI_CODIGO_MEDIO,PRECIO_SIN_IVA_EN_TICKET;
static char SI_REIMPRESION,SI_ANULACION,SI_DEVOLUCION, NO_DEVOLUCION;
static int SI_DATO_ARTICULO;
static INT16 _TKT_FOR_I = 0 ,_TKT_INI_EST;
static int _TKT_FOR_LIMITE = 0,_TKT_FOR_IND_CONFIG;
static int _TKT_WHILE_IND_CONFIG;
static struct _tkt_tmp *tkt_tmp = NULL;
extern struct _ConfTkt *CONFIG_TKT1;
extern char LARGO_CADENA,OFFSET_CADENA;
long nro_comp;
extern UINT16 _COD_VARIABLE_RNV,_SKIP_BLOCK;
extern INT16 _SUB_VARIABLE_RNV;
extern UINT16 _VALOR_RNV;
extern char _COMPARACION_IGUAL;
#ifdef COMPILAR_MUTUALES
#endif
static char SI_TOTAL_PROMOCION;
static double total_promo;
static char HAY_T_GRAVADO;
static char HAY_T_EXENTO;
static char SI_T_EXENTO;
static char SI_T_GRAVADO;
static char *_SI_P_FORMULA = NULL;
int _SI_CANT_IMPRESION = 0;
char _SI_CAD_IMPRESION[50];
int *_SI_REPETIR_CADENA = NULL;
int LINEA_IMPRESA = 0;
extern char imprimiendo_factura_journal;
char ultimaImagen[50] = {""};    // guarda la ultima imagen enviada al java server
int cantMaxRenglones = 230;      // cantidad maxima de renglones (milimetros) para pasar a otra hoja
int calcHojas = 0;  //indica si tiene que calcular la cantidad de hojas o no
int cantCopias = 0;
int cantTotalCopias = 0;
static int interlineado, carPorLinea;
void ARMAR_MENSAJE( char *buffer, char *buffer2 );
void RESOLVER_FUNCIONES( char *funciones, char *RtaConsulta, int linea );
long ULTIMO_ID_EVENTO( int tipo_evento );
long CANTIDAD_EVENTOS_ORIGINAL( int tipo_evento );
long CANTIDAD_EVENTOS( int tipo_evento , int evento_bak);
long ID_EVENTO_NRO( int nro_evento, int tipo_evento );
extern int GET_TYPE( int cod );
extern void FORMATO_CAB_COMPROBANTE_FISCAL( char *texto, int linea, char *dobleancho );
extern void FORMATO_FIN_COMPROBANTE_FISCAL( char *texto, int linea, char *dobleancho );
extern int IMPRIME_MENSAJE_FISCAL( char *mensaje );
void derecha_alinear( int largo, char *Consulta );
void RESOLVER_TKT_CONSULTA_SQL(char *Consulta, char *Funciones, char *TipoRes, int linea);
extern struct _dif_submedios dif_submedios[10];
extern int MUTUAL_MONEDA_EXTRANJERA( int h );
extern struct ImpresionDatosAdicionales ImpDatosAdi;
extern int tipo_t_eventos, tipo_t_ev_cont, tipo_t_pagos;
//Voucher Tarjeta - Fiscal - Seba
void IMPRIME_VOUCHER_FISCAL(char *buffer);//Seba
int IMPRIME_MENSAJE_FISCAL( char *mensaje ); //Seba
void ClearCfg(void);
static char cardName[20]="NO";
static char cardNum[21]="NO";
static char cardUsr[20]="NO";
static char expDate[20]="000000";
static int estNum=0;
static int cupNum=0;
static int intNum=0;
static long autCod=0;
static char opType[20]="NO";
static double _total=0;
static int _cuotas=0;
static char moneda[20]="NO";
static int termNum=0;
static int lotNum=0;
static int electTermNum=0;
static int sucNum=0;
static char Operator[20]="NO";
static int docFiscal=0;
static char _firma[20]="NO";
static char _aclaracion[20]="NO";
static char _telefono[20]="NO";
static char __total[20];
#define MAX_VAR 44
static char nombre_variable[MAX_VAR][10];
int dec,sig;
char buffer[350];//Seba
char memoria_comun_tkt[200];
extern char linea_descripcion[4][100];
extern struct _tarjetas_fisc tarjetas_fisc_rec[10];
static double total = 0.00, total_ff = 0.00;
extern double acu_ret[10][CANT_SUB_RET];
extern double acu_retirado[10],acu_retirado_me[10];
static char SI_MEDIO_SALIENTE, SI_CIERRE_Z, SI_CIERRE_X;
extern int medio_saliente;
extern double importe_retiro_ff;
int copia =0;
extern long _secuencia_ant;
extern int cierre_x, cierre_z;
extern int RECUPERAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes, int indice );
void INFORME_05_CUERPO( int x, int hora, int minutos, int segundos );
extern struct _emision_informes informes;
void RESOLVER_ASIGNA_NOMBRES( char *formula,  char *cadena );
int DEVUELVE_POSICION_NOMBRE( char* nombreVariable );
int RESOLVER_VAR_NOMBRES( char *cadena );
int ASIGNAMES( double nombre, char* nombreVariable );
int TRADUCE_LINEA_CON_VARIABLES(char * cadaux ,char *cadaux3,struct _valores_calculo val_calc);
extern char * DEVOLVER_TKT_FUNCION( int posicion );
/***************************************************************************/
void IMPRIMIR_TKT_CABECERA( int indice )
/***************************************************************************/
{
    int ind_config_ant;

    /*------------------ Inicializaciones ---------------*/
    tkt_fila = 1;
    tkt_columna = 1;

    if( MODO_NEGOCIO != SERVICIOS ) {
		//int local_ind_config = 0;

		/**/glog("IMPRIMIR_TKT_CABECERA en MODO_NEGOCIO != SERVICIOS ",LOG_VENTAS,4);
        PRN_SELECT( RECEIPT );

        ind_config = ind_comienzo_ticket;
		//local_ind_config = ind_comienzo_ticket;

		/**/glog("IMPRIMIR_TKT_CABECERA: entrando al while ...",LOG_VENTAS,4);
//com_thread->mutex_com->Lock();
          while(ind_config && CONFIG_TKT1[ind_config].codigo != _COMIENZO_DESCRIPCION_CUERPO ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
          }
//com_thread->mutex_com->Unlock();
 		/**/glog("IMPRIMIR_TKT_CABECERA: while finalizado",LOG_VENTAS,4);

		//ind_config = local_ind_config;

    } else {
		/**/glog("IMPRIMIR_TKT_CABECERA en MODO_NEGOCIO == SERVICIOS ",LOG_VENTAS,4);
        // esto no debería haberse agregado. Esta no es la forma.

        ind_config_ant = ind_config;
        ind_config = ind_comienzo_cuerpo_cantidad;

        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_CANTIDAD ) {
            TKT_ejecutar_comando( &ind_config, 1, indice, 0 );
        }

        ind_config = ind_config_ant;
    }
    //LF( 1 );
}
/***************************************************************************/
void IMPRIMIR_TKT_CABECERA_SUP()
/***************************************************************************/
{
    int ind_config_ant;
    /*------------------ Inicializaciones ---------------*/
    tkt_fila = 1;
    tkt_columna = 1;
    if( MODO_NEGOCIO == SERVICIOS ) {
        //PRN_SELECT( SLIP );
    }
    else {
        PRN_SELECT( RECEIPT );
    }
    ind_config_ant = ind_config;
    ind_config = ind_comienzo_cab_inf_sup;
    while( ind_config &&  CONFIG_TKT1[ind_config].codigo != _FIN_CAB_INF_SUP ) {
        TKT_ejecutar_comando( &ind_config, 1, 1, 0 );
    }
    ind_config = ind_config_ant;
    //LF( 1 );
}
/***************************************************************************/
void IMPRIMIR_TKT_CABECERA_DIF_SUB()
/***************************************************************************/
{
 int ind_config_ant;
    /*------------------ Inicializaciones ---------------*/
    tkt_fila = 1;
    tkt_columna = 1;
    ind_config_ant = ind_config;
    ind_config = ind_comienzo_cab_inf_dif_sub;
    while(ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CAB_INF_DIF_SUB ) {
        TKT_ejecutar_comando( &ind_config, 1, 1, 0 );
    }
	ind_config = ind_config_ant;

}
/****************************************************************************/
void IMPRIMIR_TKT_CUERPO( double precio, int indice )
/****************************************************************************/
{
    int lf = 1;
    int tot_paginas = 1;
    if( PRECIO_SIN_IVA_EN_TICKET ) {
        precio = fabs( ITEM_PRECIO_SIN_IVA ) * ( ( ITEM_PRECIO < 0.0 ) ? -1.0 : 1.0 );
    }
    #if !defined(INVEL_L) && !defined(INVEL_W)
    if( RAM_ANULACION ) {
        precio = -precio;
    }
    #endif
    LINEA_IMPRESA ++;
    if( RAM_CANTIDAD == 1 || MODO_NEGOCIO == SERVICIOS ) {
        // imprime el cuerpo
        ind_config = ind_comienzo_cuerpo;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_UNITARIO ) {
            TKT_ejecutar_comando( &ind_config, precio, indice, 0 );
        }
    }
    else {
        ind_config = ind_comienzo_cuerpo_cantidad;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_CANTIDAD ) {
            TKT_ejecutar_comando( &ind_config, precio, indice, 0 );
        }
    }
	setData( ACTION_DOPRINT, NULL, 0 ); //para obligar a imprimir el texto en las no fiscales
                                        // frente a un corte de luz que no imprimia la cantidad x precio
            //temporal para la epson 950**********************
            //GRABAR_JOURNAL_ELECTRONICO2( "\n" );
            //GRABAR_TICKET_PRN((char *)"}");
            //************************************************
    //LF( 1 );
}
/****************************************************************************/
int IMPRIMIR_TKT_TOTAL( double importe, int modo )
/****************************************************************************/
{
    double total = 0;
    int /*_estado,*/ ok = 1;
    tkt_fila = 1;
    tkt_columna = 1;
	if( ind_comienzo_total ) {
        ind_config = ind_comienzo_total;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_TOTAL ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
		//Configurar en la medida que sea posible en el los achivos CFG (TKT-VAL-FAC)  
        //LF( 1 );
    }

    return( ok );
}
/****************************************************************************/
void IMPRIMIR_TKT_PIE()
/****************************************************************************/
{
    if( ind_comienzo_pie ) {
        //tkt_fila = 1;
        //tkt_columna = 1;
        ind_config = ind_comienzo_pie;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_PIE ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }
}

/****************************************************************************/
void IMPRIMIR_TKT_TICKET_ESPERA( void )
/****************************************************************************/
{
	if( MODO_NEGOCIO != SERVICIOS ) {
        ind_config = ind_comienzo_ticket_espera;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _TKT_FIN_TICKET_ESPERA ) {
          TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
	}
}

/****************************************************************************/
void TIMBRADO_TKT( int indice )
/****************************************************************************/
{
    ind_config = ind_comienzo_timbradora;
    while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_TIMBRADORA ) {
        TKT_ejecutar_comando( &ind_config, 1, indice, 0 );
    }
    setData( ACTION_TIMBRADORA, NULL, 0 );
}
/****************************************************************************/
void IMPRIMIR_TKT_PAGO_REINTEGRO()
/****************************************************************************/
{
    ind_config = ind_comienzo_pago_reintegro;
    while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_PAGO_REINTEGRO ) {
        TKT_ejecutar_comando( &ind_config, 1, 1, 0 );
    }
    PRN_FIN_TICKET();
    setData( ACTION_DOPRINT, NULL, 0 );
}
/****************************************************************************/
void TIMBRADO_TKT_REINTEGRO()
/****************************************************************************/
{
    ind_config = ind_comienzo_timbrado_reintegro;
    while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_TIMBRADO_REINTEGRO ) {
        TKT_ejecutar_comando( &ind_config, 1, 1, 0 );
    }
    setData( ACTION_TIMBRADORA, NULL, 0 );
}
/***************************************************************************/
void ARMAR_STRING_FOLIO( char *cad ) {
/***************************************************************************/
	char cadena2[9];
	//char cad[80];
	int ie, je;
	unsigned fecha;
	memset( cad, 0, 80 );
	memset( cadena2, 0, sizeof( cadena2 ) );
	fecha = GET_FECHA_DOS(  );
	DTOC( fecha, cadena2 );
	for( ie = 0, je = 0; ie < 8; ie++ ){
		if( cadena2[ie] != '/' ) {
			cad[je] = cadena2[ie];
			je++;
		}
	}
	memset( cadena2, 0, sizeof( cadena2 ) );
	sprintf( cadena2, "%02ld" ,config.sucursal );
	strcat( cad, cadena2 );
	memset( cadena2, 0, sizeof( cadena2 ) );
	sprintf( cadena2, "%03ld" ,NRO_CAJA );
	strcat( cad, cadena2 );
	memset( cadena2, 0, sizeof( cadena2 ) );
	/*La Copia del Ticket Factura(TF) tiene que tener la misma secuencia que el TF*/
	if( RAM_COPIA ) {
		sprintf( cadena2, "%04ld", NRO_EVENTO - 1);
	} else {
		sprintf( cadena2, "%04ld", NRO_EVENTO );
	}
	strcat( cad, cadena2 );
}
/***************************************************************************/
void TKT_ejecutar_comando( int *ind_config, double precio, int h, int h2 )
/***************************************************************************/
{
    char caracter,xx[161],cadaux[350],cadaux2[350],cadaux3[350] /*,buff_val[200]*/, cadena_aux[100],cadena_aux2[100];
    char letra[10], str_tmp[10];
    unsigned fecha,hora /*, cajero*/;
    INT16 codigo,_fila/*,area*/,tabla_ant, tipo_ant, cod_medio,i, j /*, quedan*/;
    INT16 enteros, decimales, cod_var, cod_sub, cod_sol = 0, nro_orden = 0;
    INT16 ancho,modalidad,p1,p2,p3,p4,p5,codtabla,orden,font_size,posicion;
    int ind_configTmp=0, /*cod_var, cod_sub,*/ cantidad, area_ant = 0 /*, posicion*/;
    double precio_total = 0, importe_total, puntos, tot_imp[40], cod_iva[40], importe_tmp;
	int signo = 0;
    struct _valores_calculo val_calc;

	//int je , ie;
	int nroBitmap;
	unsigned char cadena[80];
	int rta = 0, seguir = 1;
	//unsigned char cadena2[9];
	//unsigned fecha;
    struct {
        long caja_z;
        long id_evento;
    } ind_imp;
	struct _datos_cobro datosTmp;
	
	memset(xx,0,sizeof(xx));
	memset(cadaux,0,sizeof(cadaux));
	memset(cadaux2,0,sizeof(cadaux2));
	memset(cadaux3,0,sizeof(cadaux3));
	memset(cadena_aux,0,sizeof(cadena_aux));
	memset(cadena_aux2,0,sizeof(cadena_aux2));
	memset(&datosTmp,0,sizeof(struct _datos_cobro));
	
	memset(letra,0,sizeof(letra));
	memset(str_tmp,0,sizeof(str_tmp));
	memset(cadena,0,sizeof(cadena));

	memset(&val_calc,0,sizeof(struct _valores_calculo));

    ( *ind_config )++;
    codigo = ( INT16 )CONFIG_TKT1[( *ind_config )].codigo;
    //( *ind_config )++;
    codigo = ( INT16 )TKT_evaluar_condiciones( ( int )codigo, /*ind_config,*/ h );
    switch( codigo ) {
		case _TKT_MES_ANIO_VENCIMIENTO:
			{
			char fecha_vto[30];
			int marca_tarjeta = VAR_TARJETAS_TARJETA;
			memset( fecha_vto, 0, sizeof( fecha_vto ) );
			
			if( TARJ[marca_tarjeta].enmasc_fecha == 's'
				|| TARJ[marca_tarjeta].enmasc_fecha == 'S' ) {
				strcpy( fecha_vto, "**/**" );
				MOSTRAR_CADENA( tkt_columna, PRN, fecha_vto );
			} else 
			MOSTRAR_MES_ANIO( tkt_columna, PRN, GET_FECHA_VTO_PRIVADA(  )  );
			}
			break;
        case _COMIENZO_SI:
            // Chequeo que la formula sea correcta
            _SI_P_FORMULA = CONFIG_TKT1[( *ind_config )].parametros;
            codigo = ( INT16 )CONFIG_TKT1[( *ind_config )].codigo;
            codigo = ( INT16 )TKT_evaluar_condiciones( ( int )codigo, /*ind_config,*/ h );
            while( CONFIG_TKT1[( *ind_config )].codigo != _FIN_SI ) {
                if( codigo )
                                // Si la formula es correcta ejecuto hasta [FIN SI]
                {
                    TKT_ejecutar_comando( ind_config, 1, h, h2/*0*/ );
                }
                else
                                // si la formula no es correcta, saltea el comando
                {
                    ( *ind_config )++;
                }
            }
            break;
        case _FECHA:
            fecha = GET_FECHA_DOS();
            if( MODO_NEGOCIO != SERVICIOS ) {
                MOSTRAR_FECHA( tkt_columna, PRN, fecha );
            }
            else {
                MOSTRAR_FECHA_LETRAS( tkt_columna, PRN, fecha );
            }
            break;
        case _TKT_FECHA_ALINEADA:
            ind_configTmp = 0;
            ancho = TKT_tomar_entero( ind_config, &ind_configTmp );
            caracter = CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp];
            fecha = GET_FECHA_DOS();
            // Alineacion a la derecha
            if( caracter == 'D' ) {
                // copio la fecha en letras al string cadaux
                OBTENER_FECHA_LETRAS( fecha, cadaux2 );
                derecha_alinear( ancho, (char *) cadaux2 );
                IMPRIMIR_CADENA( tkt_columna, cadaux2 );
            }
            else {
                OBTENER_FECHA_LETRAS( fecha, cadaux );
                IMPRIMIR_CADENA( tkt_columna, cadaux );
            }
            break;
        case _HORATKT:
            //if(MODO_NEGOCIO != SERVICIOS){
            hora = GET_HORA_DOS();
            MOSTRAR_HORA( tkt_columna, PRN, hora );
            //}
            break;
        case _FECHA_CUATRO_DIGITOS:
            fecha = GET_FECHA_DOS();
            MOSTRAR_FECHA_2000( ( char )tkt_columna, PRN, fecha );
            break;
        case _CODIGO_BARRA:
			if( articulo.imp_cod_barra == 'S' ) {
				IMPRIMIR_CADENA( tkt_columna, ITEM_COD_BARRA );
			}
            break;
        case _CODIGO_ARTICULO:
            MOSTRAR_ENTERO( tkt_columna, PRN, "000000000", ITEM_COD_ESPECIAL );
            break;
        case _SECUENCIA:
            if( MODO_NEGOCIO != SERVICIOS )
			{
				memset(cadena,0,sizeof(cadena));
				if( IMPRESORA_FISCAL == FISCAL ) 
				{
					_snprintf(cadena, sizeof(cadena)-1, "%08li ", NRO_EVENTO - 1);
					IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena, NO );
				}
				else
				{
					if( LIMITE_MAXIMO_NUMERO_DE_EVENTO ) 
					{
						_ltoa( LIMITE_MAXIMO_NUMERO_DE_EVENTO, cadena, 10 );
						MOSTRAR_ENTERO( tkt_columna, PRN, MASCARA( '0', strlen( cadena ), "" ),
							( ( NRO_EVENTO )? NRO_EVENTO : 1 ) ); 						
					} 
					else 
					{
						MOSTRAR_ENTERO( tkt_columna, PRN, ( config.pais != ECUADOR ) ? "00000000" : "0000000",
							( ( NRO_EVENTO )? NRO_EVENTO : 1 ) ); 
					}
				}
            } 
			else 
			{
                MOSTRAR_ENTERO( tkt_columna, PRN, ( config.pais != ECUADOR ) ? "000000000" : "0000000",
                                NRO_ULTIMO_EVENTO_ANALIZADO - 1 );
            }
            break;
        case _NUMERO_Z:
            MOSTRAR_ENTERO( tkt_columna, PRN, "000000000", NRO_Z );
            break;
        case _DESC_ARTICULO:
            if( MODO_NEGOCIO != SERVICIOS ) {
                if( LARGO_CADENA ) {
                    char aux[31];
                    memset( aux, 0, sizeof( aux ) );
                    memcpy( aux, ITEM_NOMBRE_LARGO, sizeof( aux ) );
                    aux[LARGO_CADENA] = 0;
                    TRIM( aux );
                    aux[LARGO_CADENA] = 0;
                    IMPRIMIR_CADENA( tkt_columna, aux );
                    LARGO_CADENA = 0;
                }
                else {
                    IMPRIMIR_CADENA( tkt_columna, ITEM_NOMBRE_LARGO );
                }
            }
            else {
                MOSTRAR_CADENA( tkt_columna, PRN, cobros.nombre );
            }
            break;
        case _MARCA:
            if( MODO_NEGOCIO != SERVICIOS ) {
                MOSTRAR_CADENA( tkt_columna, PRN, ITEM_MARCA );
            }
            else {
                MOSTRAR_DATO_COBRO( 1, tkt_columna );
            }
            break;
        case _PRECIO_TOTAL:
            ind_configTmp = 0;
            enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
            decimales = TKT_tomar_entero( ind_config, &ind_configTmp );
            precio_total = ROUND(  precio * RAM_CANTIDAD, enteros,
                                   decimales ) /** RAM_CANTIDAD*/;
        case _PRECIO_UNITARIO:
            if( !precio_total ) {
            ind_configTmp = 0;
            enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
            decimales = TKT_tomar_entero( ind_config, &ind_configTmp );
            }
			if( RAM_ANULACION && _TICKET_ORIGEN( h ) != _ORIGEN_PROMOCION ) {
                //MOSTRAR_CADENA( tkt_columna - 1, PRN, "-" );
				precio_total*= -1;
				precio *= -1;
            }
			/*------Cobros Venta-------*/
			if( _TICKET_COD_COBRO( h ) && !precio_total ){
				precio_total = ROUND(  precio * RAM_CANTIDAD, enteros,
                                   decimales ) /** RAM_CANTIDAD*/;
			}
			/*------------------------*/
            if( RAM_CANTIDAD == 1 ) {
                //MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1, precio );
				if(_TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION && RAM_ANULACION)
					MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1, precio*(-1) );
				else
                MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1, precio );
            }
            else if( ITEM_PEDIR_PRECIO && RAM_MODO_EJECUCION != INVENTARIO ) {
                //MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1,
                MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 0,
                                  ( precio_total ) ? precio_total : precio );
            }
            else {
                //MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1,
                if(_TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION)
					MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1,( precio_total ) ? (precio_total*(-1)) : (precio*(-1)) );
				else
					MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1,( precio_total ) ? precio_total : precio );
            }
            break;
        case _TOTAL_TICKET:
            {
                INT16 alineado = 0;
                ind_configTmp = 0;
                enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
                decimales = TKT_tomar_entero( ind_config, &ind_configTmp );
                alineado = TKT_tomar_entero( ind_config, &ind_configTmp );
                importe_total = RAM_TOTAL;
                //MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1, importe_total );
                FTOC( importe_total, xx, enteros, decimales, SI, SI );
                LTRIM( xx );
                if( alineado ) {
                    MOSTRAR_CADENA_ALD( tkt_columna, PRN, xx, alineado );
                }
                else {
                    MOSTRAR_CADENA( tkt_columna, PRN, xx );
                }
            }
            break;
		case _CANTIDAD_ARTICULO:
            /*MOSTRAR_FLOTANTE_ALINEADO( tkt_columna, PRN, ENTEROS - 3, ( ITEM_PESABLE ) ? 3 : 0, 1, 0,
                              ( ( ITEM_PESABLE && !ITEM_PEDIR_PRECIO && RAM_CANTIDAD_ORIGINAL )
                               ? RAM_CANTIDAD_ORIGINAL : RAM_CANTIDAD ) );*/
			MOSTRAR_FLOTANTE_ALINEADO( tkt_columna, PRN, ENTEROS + DECIMALES - (( ITEM_PESABLE ) ? 3:3 ), ( ITEM_PESABLE ) ? 3 : 3, 1, 0,
					( _TICKET_COD_COBRO( h ) ) ? 1 : ( ( ITEM_PESABLE && !ITEM_PEDIR_PRECIO && RAM_CANTIDAD_ORIGINAL )
                               ? RAM_CANTIDAD_ORIGINAL : RAM_CANTIDAD ) );

            break;
        case _CONTENIDO_ARTICULO:
            IMPRIMIR_CADENA( tkt_columna, ITEM_CONTENIDO );
            break;
        case _UNIDAD_ARTICULO:
            IMPRIMIR_CADENA( tkt_columna, ITEM_UNIDAD );
            break;
		case _CADENA_FISCAL:
			memset(cadaux,0,sizeof(cadaux));
            ARMAR_MENSAJE( /*&*/CONFIG_TKT1[( *ind_config )].parametros, cadaux );
            IMPRIME_MENSAJE_FISCAL( cadaux );
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            break;
		case _CADENA_NO_FISCAL:
			memset(cadena_aux,0,sizeof(cadena_aux));
			memset(cadena_aux2,0,sizeof(cadena_aux2));
			_snprintf(cadena_aux2, sizeof(cadena_aux2)-1,&CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp]);
            if ( IMPRESORA_FISCAL == FISCAL )//Seba
			{
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux2, NO );//Seba
			}
            break;
		case _CADENA:
        case _TKT_COMANDO_FISCAL:
			memset(cadaux2,0,sizeof(cadaux2));
            ARMAR_MENSAJE(CONFIG_TKT1[( *ind_config )].parametros, cadaux2 );
			if ( IMPRESORA_FISCAL == FISCAL ) 
				IMPRIMIR_TEXTO_NO_FISCAL(1, TIPO_DE_LETRA, cadaux2, NO );
			else
				IMPRIMIR_CADENA( tkt_columna, cadaux2 );
            break;
        case _POSICION:
            ind_configTmp = 0;
            tkt_columna = TKT_tomar_entero( ind_config, &ind_configTmp ) + TKT_OFFSET_COLUMNA;
            _fila = TKT_tomar_entero( ind_config, &ind_configTmp );
            if( MODO_NEGOCIO != SERVICIOS ) {
				TKT_SET_FILA( _fila );
            }
            else {
                sprintf( cadaux, "%d#%d", tkt_columna, _fila );
                tkt_fila = _fila;
                setData( ACTION_SET_POSITION, cadaux, 0 );
            }
            break;
		case _POS_X:
            ind_configTmp = 0;
            tkt_columna = TKT_tomar_entero( ind_config, &ind_configTmp ) + TKT_OFFSET_COLUMNA;
            break;
        case _POS_Y:
            ind_configTmp = 0;
            _fila = TKT_tomar_entero( ind_config, &ind_configTmp );
			TKT_SET_FILA( _fila );
            break;
		case _TKT_LF:
			{
			int lines_to_feed = 0;
			ind_configTmp = 0;
			lines_to_feed = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            TKT_SET_FILA( tkt_fila + lines_to_feed );
			if( MODO_NEGOCIO == SERVICIOS ) {
                if( tkt_fila > cantMaxRenglones ) {
                    char pagina[31];
                    //Imprime numero de hoja
                    if(!calcHojas){
                        sprintf( pagina, "pagina %d de %d", cantCopias + 1,cantTotalCopias + 1 );
                        setData( ACTION_SET_POSITION, "150#235", 0 );
                        IMPRIMIR_CADENA( 0, pagina );
                        cantCopias += 1;
                        setData( ACTION_DOPRINT, NULL, 0 );
                        SET_MEMORY_CHAR( __ram_empieza_ticket, 0 );
                    }else{
                        cantCopias += 1;
                    }
                    switch( RAM_IMPRIMIR_CABECERA ) {
                        case 0:
                            IMPRIMIR_TKT_CABECERA_SUP();
                            tkt_fila = 65;
							tkt_columna = 11;
							break;
                        case 1:
                            IMPRIMIR_TKT_CABECERA( 1 );
                            tkt_fila = 65;
							tkt_columna = 11;
							break;
						case 2:
							IMPRIMIR_TKT_CABECERA_DIF_SUB();
							tkt_fila = 50;
							tkt_columna = 11;
                            break;
                    }
//                    tkt_fila = 65;
//                    tkt_columna = 11;
                }
                sprintf( cadaux, "%d#%d", tkt_columna, tkt_fila );
                setData( ACTION_SET_POSITION, cadaux, 0 );
            }
            else {
                tkt_columna = 1 + TKT_OFFSET_COLUMNA;
            }
			}
            break;
        case _TKT_TICKS_B:
            ind_configTmp = 0;
            ticks_b = TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _CORTE_TICKET:
            PRN_CORTE_TOTAL();
            LINEA_IMPRESA = _SI_CANT_IMPRESION;
            break;
        case _LARGO_CADENA:
            ind_configTmp = 0;
            LARGO_CADENA = (char)TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_RESET:
            //      RESET_FLAG_ERROR_IBM4610( ticks_b );

            break;
        case _LETRA_NORMAL:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_NORMAL;
			else
            PRN_LETRA_CHICA();
            break;
            /*  case _LETRA_CALIDAD:
                    break;*/
            /*  case _LETRA_DOBLE_PASADA:
                    PRN_LETRA_DOBLE_PASADA(  );
                    break;*/
        case _LETRA_DOBLE_ANCHO:
			if( IMPRESORA_FISCAL == FISCAL )
				TIPO_DE_LETRA = LETRA_DOBLE_ANCHO ;
			else
				PRN_LETRA_GRANDE();
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
		case _LETRA_SUBRAYADO_NEGRITA:
			if(IMPRESORA_FISCAL == FISCAL)
				TIPO_DE_LETRA = LETRA_SUBRAYADO_NEGRITA ;
			break;
        case _TOTAL_EN_LETRAS:
            if( MODO_NEGOCIO != SERVICIOS ) {
                TKT_total_en_letras( ind_config );
            }
            else {
                NUM_A_LETRAS( RAM_TOTAL, cadaux2 );
                MOSTRAR_CADENA( 1, PRN, cadaux2 );
            }
            break;
        case _NUMERO_CAJA:
			memset(cadena, 0, sizeof(cadena));
			if(IMPRESORA_FISCAL == FISCAL)
			{
				_snprintf( cadena,sizeof(cadena)-1, "%04li", NRO_CAJA);
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena, NO );
			}
			else {
				if( config.pais == ELSALVADOR ) {
					MOSTRAR_ENTERO( tkt_columna, PRN, "0000", NRO_CAJA );
				} else {
					MOSTRAR_ENTERO( tkt_columna, PRN, "000", NRO_CAJA );
				}
			}

            break;
        case _TKT_NRO_CAJA:
            MOSTRAR_ENTERO( tkt_columna, PRN, /*&*/CONFIG_TKT1[( *ind_config )].parametros,
                            NRO_CAJA );
            /*( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;*/
            break;
        case _NOMBRE_CAJERO:
			memset( cadena, 0, sizeof( cadena ) );
			if( IMPRESORA_FISCAL == FISCAL ) //Seba
			{
				_snprintf( cadena,sizeof(cadena)-1, "%s", datos_cajero_actual.nombre);
				IMPRIMIR_TEXTO_NO_FISCAL(1, TIPO_DE_LETRA, cadena, NO );//Seba
			}
			else
				MOSTRAR_CADENA( tkt_columna, PRN, datos_cajero_actual.nombre );
            break;
        case _NUMERO_CAJERO:
			memset( cadena, 0, sizeof( cadena ) );
            if( IMPRIMIR_LEGAJO_CAJERO ) 
			{
				memset(cadena, 0, sizeof(cadena));
				if( IMPRESORA_FISCAL == FISCAL )
				{					
					_snprintf( cadena,sizeof(cadena)-1, "%08li", NRO_LEGAJO_CAJERO );
					IMPRIMIR_TEXTO_NO_FISCAL(1, TIPO_DE_LETRA, cadena, NO );
				}
				else {
					_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
					MOSTRAR_ENTERO( tkt_columna, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
				}
            }
            else 
			{
				if( IMPRESORA_FISCAL == FISCAL ) 
				{
					_snprintf( cadena,sizeof(cadena)-1, "%04li", /*cajero.cod_cajero*/NRO_CAJERO );
					IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena, NO );
				}
				else {
					_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
					MOSTRAR_ENTERO( tkt_columna, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
				}
            }
            break;
        case _TKT_TOTAL:
            importe_total = RAM_TOTAL;
            MOSTRAR_ENTERO( tkt_columna, PRN, /*&*/CONFIG_TKT1[( *ind_config )].parametros,
                            ( long )importe_total );
            /*( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;*/
            break;
        case _TKT_NRO_TICKET:
         
            MOSTRAR_ENTERO( tkt_columna, PRN, /*&*/CONFIG_TKT1[( *ind_config )].parametros,
                            NRO_TICKET );
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            break;
        case _NRO_TICKET:
            
            if( MODO_NEGOCIO != SERVICIOS ) {
                if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
                    MOSTRAR_ENTERO( tkt_columna, PRN, "000000", NRO_TICKET );
                }
                else {
                    if( !fiscal_b ) {
                        MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", NRO_TICKET );
                    }
                    else {
                        MOSTRAR_ENTERO( tkt_columna, PRN, "000000000000", NRO_TICKET );
                    }
                }
            }
            //MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", GET_MEMORY_LONG( h ) );
            if( MODO_NEGOCIO == SERVICIOS/* && modo == _MODO_COBRANZA*/ ) {
                //OBTENER_DATOS_COMPROBANTE( _FACTURA, comp.subtipo, 1, &comp.nro_suc, &comp.nro_comp,
                //   NULL, NO, NO );
                MOSTRAR_ENTERO( tkt_columna, PRN, "0000", comp.nro_suc );
                MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", comp.nro_comp );
            }
            break;
        case _VUELTO:
            IMPRIME_VUELTO( RAM_VUELTO - RAM_TOTAL_DONACION );
            break;
        case _NOMBRE_SUCURSAL:
			if (IMPRESORA_FISCAL == FISCAL){
				memset (cadaux,0,sizeof(cadaux));
				_snprintf(cadaux,sizeof(cadaux)-1,"Sucursal: %d", config.sucursal );
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadaux, NO );
			}
			else
				MOSTRAR_ENTERO( tkt_columna, PRN, "000", config.sucursal );
            break;
        case _TKT_FIN_INICIALIZACION:
        case _FIN_CUERPO:
        case _FIN_CUERPO_UNITARIO:
        case _FIN_CUERPO_CANTIDAD:
		case _FIN_DESCRIPCION_CUERPO:
		case _FIN_DESCRIPCION_TARJETA:
        case _COMIENZO_CUERPO:
        case _COMIENZO_CUERPO_UNITARIO:
        case _COMIENZO_CUERPO_CANTIDAD:
		case _COMIENZO_DESCRIPCION_CUERPO:
		case _COMIENZO_DESCRIPCION_TARJETA:
		case _COMIENZO_DUPLICADO_TARJETA:	//Seba
		case _FIN_DUPLICADO_TARJETA:		//Seba
            break;
        case _CABECERA_FISCAL:
			{
			int linea = 0;
			
			ind_configTmp = 0;
			memset(cadaux,0,sizeof(cadaux));
			memset(cadaux2,0,sizeof(cadaux2));
			memset(cadaux3,0,sizeof(cadaux3));
			linea = TKT_tomar_entero( ind_config, &ind_configTmp );
			
			strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp], sizeof( cadaux2 ) - 1 );
			for(i=0; i<tkt_columna; i++)
				cadaux[i]= ' ';
			strcat(cadaux, cadaux2);

			memset(cadaux2,0,sizeof(cadaux2));
			( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
			strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp], sizeof( cadaux2 ) - 1 );

			TRADUCE_LINEA_CON_VARIABLES(cadaux ,cadaux3,val_calc);
			
			FORMATO_CAB_COMPROBANTE_FISCAL (cadaux,linea,cadaux2);
			}
            break;
        case _CABECERA_FISCAL_2:
            FORMATO_CAB_COMPROBANTE_FISCAL( /*&*/CONFIG_TKT1[( *ind_config )].parametros, 2, "" );
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            break;
        case _PIE_FISCAL:
			{
				int linea = 0, maximaSec = -999, modificada=0;
			
			ind_configTmp = 0;
			memset(cadaux,0,sizeof(cadaux));
			memset(cadaux2,0,sizeof(cadaux2));
			linea = TKT_tomar_entero( ind_config, &ind_configTmp );
			
			strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp], sizeof( cadaux2 ) - 1 );
			for(i=0; i<tkt_columna; i++)
				cadaux[i]= ' ';
			strcat(cadaux, cadaux2);

			memset(cadaux2,0,sizeof(cadaux2));
			memset(cadaux3,0,sizeof(cadaux3));
			( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
			strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp], sizeof( cadaux2 ) - 1 );
            
			if (!modificada && !strcmp(cadaux,"SECUENCIA")) {
				modificada=1;
				if( NRO_EVENTO && DIRECT_DB_QUERY( "SELECT max( NRO_SECUENCIA ) FROM eventos" ) > 0 ) {
					GET_DATO(1,C_INT,(char *)&maximaSec,4);
					//Cuando la consulta no devuelve un registo, GET_DATO no setea maximaSec sino que 
					//conserva su valor original para nuestro caso -999. Esto hace que setee el nro de 
					//secuencia con el valor de NRO_EVENTO ultimo. Para el caso de 1er evento tipo 26 
					//con incremento de secuencia, la secuencia asumiria el valor del NRO_EVENTO ultimo
					//(Tarea 5682), por esa razon se corrige la situacion.			
					if( maximaSec == -999 )
						maximaSec = 0;//No hay registro por consecuencia la secuencia es cero.
				}
				_snprintf(cadaux, sizeof(cadaux)-1, "Nro de Secuencia: %08li", 
					( maximaSec != -999 )? maximaSec + 1: NRO_EVENTO + 1 ); 
			}
			if (!modificada && !strcmp(cadaux,"CAJA")){
				modificada=1;
				_snprintf( cadaux,sizeof(cadaux)-1, "Caja: %04li", NRO_CAJA);
			}
			if (!modificada && !strcmp(cadaux,"CAJA_PUNTOS")) {
				modificada=1;
				if( fabs(RAM_ACU_PUNTOS )> 0.01 ) {
					if( RAM_REFERENCIA_NC )
						_snprintf( cadaux,sizeof(cadaux)-1, "Caja:%04li Puntos Devueltos:%.2lf", NRO_CAJA, RAM_ACU_PUNTOS);
					else
						_snprintf( cadaux,sizeof(cadaux)-1, "Caja:%04li Puntos Obtenidos:%.2lf", NRO_CAJA, RAM_ACU_PUNTOS);
				}else {
					_snprintf( cadaux,sizeof(cadaux)-1, "Caja: %04li", NRO_CAJA);
				}
			}
			if (!modificada && !strcmp(cadaux,"ARTICULOS")){
				modificada=1;
				_snprintf( cadaux,sizeof(cadaux)-1, "Articulos: %04li", RAM_BULTOS);}
			if (!modificada && !strcmp(cadaux,"CAJERO")){
				modificada=1;
				_snprintf( cadaux,sizeof(cadaux)-1, "Cajero: %04li %s", NRO_CAJERO,datos_cajero_actual.nombre);
			}
			if(!modificada &&!strcmp(cadaux,"DATOSCARGADOS")){
				//DEVUELVE_LINEA_DESCRIPCION(cadaux, 0, 40 );
				_snprintf( cadaux,sizeof(cadaux)-1, "%s", memoria_comun_tkt);
			}

			FORMATO_FIN_COMPROBANTE_FISCAL( cadaux,linea,cadaux2);

			}
            break;
        case _PIE_FISCAL_2:
            FORMATO_FIN_COMPROBANTE_FISCAL( /*&*/CONFIG_TKT1[( *ind_config )].parametros, 2, "" );
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            break;
        case _ITEM_FISCAL:
            if( RAM_ANULACION ) {
                IMPRIME_ITEM_FISCAL( ITEM_NOMBRE_LARGO, (precio+ITEM_IMP_INT), precio, -RAM_CANTIDAD );
            }
            else {
                IMPRIME_ITEM_FISCAL( ITEM_NOMBRE_LARGO, (precio+ITEM_IMP_INT), precio, RAM_CANTIDAD );
            }
            break;
            /*   case _FISCAL_X:
                  FISCAL_X();
                  break;*/
        case _FISCAL_Z:
			FISCAL_Z();
			break;
		case _IMAGEN:
			nroBitmap = SET_BITMAP( CONFIG_TKT1[( *ind_config )].parametros );
			memset(cadena,0,sizeof(cadena));
			sprintf(cadena,"%d",nroBitmap);
			PRINT_BITMAP( cadena );
			break;
		case _TKT_CODIGO_BARRA_FOLIO:
			ARMAR_STRING_FOLIO( cadena );
			SET_BAR_CODE( cadena );
			BAR_CODE(  );
            break;
		case _TKT_CODIGO_FOLIO:
			{
			unsigned char cad[40];
			int l_buffer = 0;

			memset( cad, 0, sizeof( cad ) );
			ARMAR_STRING_FOLIO( cadena );
			l_buffer = strlen( cadena );
			memcpy( cad, cadena, l_buffer );
			IMPRIMIR_CADENA( tkt_columna, cad );
			}
            break;
            /*case _TKT_INICIO_COMANDO_X:
            case _TKT_FIN_COMANDO_X:
            case _TKT_INICIO_COMANDO_Z:
            case _TKT_FIN_COMANDO_Z:
            case _TKT_INICIO_IMPRESORA_FISCAL:
            case _TKT_FIN_IMPRESORA_FISCAL:
            case _TKT_INICIO_COMANDO_Y:
            case _TKT_FIN_COMANDO_Y:
            case _TKT_INICIO_COMANDO_B:
            case _TKT_FIN_COMANDO_B:
            case _TKT_INICIO_COMANDO_T:
            case _TKT_FIN_COMANDO_T:*/
        case _TKT_INICIO_CORTE_PAPEL:
        case _TKT_FIN_CORTE_PAPEL:
            break;
        case _IVA_ARTICULO:
            MOSTRAR_ENTERO( tkt_columna, PRN, "00", ITEM_COD_IVA );
            break;
        case _NOMBRE_MEDIO:
            if( MODO_NEGOCIO == SERVICIOS ) {
                if(IMPRESORA_FISCAL == FISCAL)
					IMPRIMIR_TEXTO_NO_FISCAL(1, TIPO_DE_LETRA, medios[_PAGO_MODO( h )].nombre, NO );//Seba
				else
                MOSTRAR_CADENA( tkt_columna, PRN, medios[_PAGO_MODO( h )].nombre );
            }
            else {
                //area = SELECTED();
				tabla_ant = SELECTED2();
				tipo_ant = SELECTED_TIPO();

                if( medios[_PAGO_MODO( h )].tarjeta ) {
                    if(IMPRESORA_FISCAL == FISCAL)
						IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA, NOMBRE_TARJETA( VAR_TARJETAS_TARJETA ), NO );//Seba
					else
                    MOSTRAR_CADENA( tkt_columna, PRN, NOMBRE_TARJETA( VAR_TARJETAS_TARJETA ) );
                    #ifdef COMPILAR_MUTUALES
                }
                else if( medios[_PAGO_MODO( h )].mutual ) {
                    NOMBRE_MUTUAL( ( char )_PAGO_COD_SUBMEDIO( h ), xx );
                    if(IMPRESORA_FISCAL == FISCAL)
						IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA, xx, NO );//Seba
					else
                    MOSTRAR_CADENA( tkt_columna, PRN, xx );
                }
                    #endif
                else if( medios[_PAGO_MODO( h )].ticket_compra ) {
                    NOMBRE_TICKET_COMPRA( _PAGO_COD_SUBMEDIO( h ), xx );
                    if(IMPRESORA_FISCAL == FISCAL)
						IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA, xx, NO );//Seba
					else
                    MOSTRAR_CADENA( tkt_columna, PRN, xx );
                }
                else {
                    if(IMPRESORA_FISCAL == FISCAL)
						IMPRIMIR_TEXTO_NO_FISCAL(1,LETRA_NORMAL, medios[_PAGO_MODO( h )].nombre, NO );//Seba
					else
                    MOSTRAR_CADENA( tkt_columna, PRN, medios[_PAGO_MODO( h )].nombre );
                }
                //SELECT( area );
				SELECT_TABLE( tabla_ant, tipo_ant );
            }
            break;
       case _IMPORTE_MEDIO:
            ind_configTmp = 0;
            enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
            decimales = TKT_tomar_entero( ind_config, &ind_configTmp );
            if(IMPRESORA_FISCAL == FISCAL)
			{
				double importe = ROUND( _PAGO_IMPORTE( h ), enteros, decimales );
				memset(cadena_aux,0,sizeof(cadena_aux));
				_snprintf(cadena_aux,sizeof(cadena_aux)-1,"$ %f",importe);
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA, cadena_aux, NO );//Seba
            }
			else
			{
            if( MODO_NEGOCIO == SERVICIOS ) {
                FTOC( ROUND( _PAGO_IMPORTE( h ), ENTEROS, DECIMALES ), xx, ENTEROS + 1, DECIMALES, SI,
                      SI );
                MOSTRAR_CADENA_ALD( tkt_columna, PRN, xx, 14 );
            }
            else {
                importe_tmp = ROUND( _PAGO_IMPORTE( h ), enteros, decimales);
				
				if( TOT_TICK_IMPORTE_SIN_IVA < 0 )
				{
					if(MODO_DEVOLUCION || RAM_NOTA_CR)
						importe_tmp -= ROUND(RAM_DIFERENCIA_REDONDEO_GRAV, enteros, decimales);
					else
						importe_tmp += ROUND(RAM_DIFERENCIA_REDONDEO_GRAV, enteros, decimales);
				}
				if( TOT_TICK_IMPORTE_SIN_IVA > 0 )
				{
					if(MODO_DEVOLUCION || RAM_NOTA_CR)
						importe_tmp += ROUND(RAM_DIFERENCIA_REDONDEO_GRAV, enteros, decimales);
					else
						importe_tmp -= ROUND(RAM_DIFERENCIA_REDONDEO_GRAV, enteros, decimales);
				}
				if( TOT_TICK_EXENTO < 0 )
				{
					if(MODO_DEVOLUCION || RAM_NOTA_CR)
						importe_tmp -= ROUND(RAM_DIFERENCIA_REDONDEO_EXENT, enteros, decimales);
					else
						importe_tmp += ROUND(RAM_DIFERENCIA_REDONDEO_EXENT, enteros, decimales);
				}
				if( TOT_TICK_EXENTO > 0 )
				{
					if(MODO_DEVOLUCION || RAM_NOTA_CR)
						importe_tmp += ROUND(RAM_DIFERENCIA_REDONDEO_EXENT, enteros, decimales);
					else
						importe_tmp -= ROUND(RAM_DIFERENCIA_REDONDEO_EXENT, enteros, decimales);
				}
				if( RAM_FACTURA == 1 && TOT_TICK_IMPORTE_SIN_IVA < 0 && FACT_INDEP_DEL_TIPO_DE_CLIENTE == 1)
					importe_tmp += ROUND( RAM_DIFERENCIA_REDONDEO_GRAV, ENTEROS, DECIMALES ); 
				if(RAM_FACTURA == 1 && TOT_TICK_IMPORTE_SIN_IVA > 0 && FACT_INDEP_DEL_TIPO_DE_CLIENTE == 1)
					importe_tmp +=ROUND( RAM_DIFERENCIA_REDONDEO_GRAV, ENTEROS, DECIMALES );
				if(MODO_DEVOLUCION || RAM_NOTA_CR)
					importe_tmp = importe_tmp * -1;
					//MOSTRAR_CADENA( tkt_columna, PRN, "-" );
				MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1,importe_tmp);
				}
            }

            break;
       case _TKT_COTIZACION:
            IMPRIME_COTIZACION( tkt_columna, PRN, h );
            break;
		case _TKT_IMPORTE_INGRESADO:
            IMPRIME_IMPORTE_INGRESADO( tkt_columna, PRN, h );
			break;
        case _TKT_IMPORTE_MEDIO_ME:
            ind_configTmp = 0;
            enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
            decimales = TKT_tomar_entero( ind_config, &ind_configTmp );
            MOSTRAR_FLOTANTE( tkt_columna, PRN,	ENTEROS, DECIMALES, 1, 0,
                              ROUND( _PAGO_IMPORTE_ME( h ), ENTEROS, DECIMALES ) );
            break;
        case _SELECT:
            ind_configTmp = 0;
            enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
            if( MODO_NEGOCIO != SERVICIOS ) {
                if( !imprimiendo_factura_journal ) {
                    switch( enteros ) {
                        case 1:
                            PRN_SELECT( RECEIPT );
                            break;
                        case 2:
                            PRN_SELECT( JOURNAL );
                            break;
                        case 3:
                            //PRN_HABILITA_AVANCES();
                            PRN_SELECT( RECEIPT + JOURNAL );
                            break;
						case 4:
                            PRN_SELECT( SLIP );
                            break;
                        case 31:
                            PRN_SELECT( RECEIPT_PART );
                            break;
                        case 32:
                            PRN_SELECT( JOURNAL_PART );
                            break;
                    }
                }
            }
            else {
                PRN_SELECT( enteros );
            }
            break;
        case _IVA1:
            MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              ( iva[h].porc_iva1 )
                            ? ROUND( precio * iva[h].porc_iva1 / 100.0, ENTEROS, DECIMALES ) : 0 );
            break;
        case _TASA_IVA1:
            MOSTRAR_FLOTANTE( tkt_columna, PRN, 3, 2, 1, 1, iva[h].porc_iva1 );
            break;
        case _BASE_IMPONIBLE:
            MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              ROUND( precio, ENTEROS, DECIMALES ) );
            break;
        case _COD_IVA:
            MOSTRAR_ENTERO( tkt_columna, PRN, "00", h );
            break;
        case _BULTOS:
            MOSTRAR_ENTERO( tkt_columna, PRN, "00000", RAM_BULTOS );
            break;
        case _EAN_CLIENTE_AFINIDAD:
            MOSTRAR_CADENA( tkt_columna, PRN, RAM_STRING_CLIENTE_AFINIDAD );
            break;
        case _SI_MODO_ENTRENAMIENTO:
            #ifdef COMPILAR_MODO_ENTRENAMIENTO
            SI_MODO_ENTRENAMIENTO = 1;
            #endif
            break;
        case _SI_REIMPRESION:
            SI_REIMPRESION = 1;
            break;
        case _TRANSACCION:
            MOSTRAR_ENTERO( tkt_columna, PRN, "0000000", RAM_ID_EVENTO + 1 );
            break;
        case _CAJA_ANTERIOR:
			memset(cadena_aux,0, sizeof(cadena_aux));
			_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%04d", RAM_CAJA_ANTERIOR);
	 		MOSTRAR_CADENA(tkt_columna, PRN, cadena_aux );
            break;
        case _FECHA_ANTERIOR:
            if( RAM_EN_IMPORTACION_DE_TICKET == 2 ) {
                MOSTRAR_FECHA( tkt_columna, PRN, RAM_SUCURSAL_ANTERIOR );
            }
            break;
        case _SI_ANULACION:
            SI_ANULACION = 1;
            break;
        case _SI_MODO_INVENTARIO:
            SI_MODO_INVENTARIO = 1;
            break;
        case _SI_DEVOLUCION:
            SI_DEVOLUCION = 1;
            break;
		case _NO_DEVOLUCION:
            NO_DEVOLUCION = 1;
            break;
        case _TKT_PUNTAJE_VENTA:
            MOSTRAR_FLOTANTE( tkt_columna, PRN, 6, 2, 1, 0, RAM_ACU_PUNTOS );
            break;
        case _TKT_NUMERO_CLIENTE_AFINIDAD:
            if( MODO_NEGOCIO != SERVICIOS ) {
                MOSTRAR_ENTERO( tkt_columna, PRN, "000000000", RAM_CLIENTE_AFINIDAD );
            }
            else {
                MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", cli_cobro.nro_socio );
            }
            break;
        case _TKT_PUNTAJE_VENTA_TRUNCADO:
            puntos = RAM_ACU_PUNTOS;
            MOSTRAR_FLOTANTE( tkt_columna, PRN, 6, 0, 1, 0, puntos );
            break;
        case _TKT_NOMBRE_CLIENTE_AFINIDAD:
            MOSTRAR_CADENA( tkt_columna, PRN, RAM_NOMBRE_CLIENTE_AFINIDAD );
            break;
        case _TKT_PUNTOS_CLIENTE:
			{	long cliente_aux = clientes.cod_cliente;
				if( RAM_CLIENTE_AFINIDAD  && clientes.cod_cliente != RAM_CLIENTE_AFINIDAD   ) {			
					if( !EXISTE_CLIENTE( RAM_CLIENTE_AFINIDAD ) ) {
					}
				}
            if( clientes.puntos > 0 ) {
                MOSTRAR_FLOTANTE( tkt_columna, PRN, 6, 2, 1, 0, clientes.puntos );
            }
				EXISTE_CLIENTE( cliente_aux ) ;
			}
            break;
        case _TKT_BUSCA_CLIENTE:
            EXISTE_CLIENTE( RAM_CLIENTE_AFINIDAD );
            break;
        case _TKT_FECHA_ACTUALIZACION:
            MOSTRAR_FECHA( tkt_columna, PRN, RAM_FECHA_PUNTOS_AFINIDAD );
            break;
        case _TKT_PUNTAJE_ACUMULADO:
            if( RAM_PUNTOS_AFINIDAD > 0 ) {
                MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS + 2, DECIMALES, 1, 0,
                                  RAM_PUNTOS_AFINIDAD );
            }
            break;
        case _TKT_DIFERENCIA_AFINIDAD:
            MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 0, RAM_DIFERENCIA_AFINIDAD );
            break;
        case _TKT_CANT_ARTICULOS_AFINIDAD:
            MOSTRAR_ENTERO( tkt_columna, PRN, "0000", RAM_CANT_ARTICULOS_AFINIDAD );
            break;
        case _SI_MEDIO:
            ind_configTmp = 0;
            SI_CODIGO_MEDIO = (char)TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_NUMERO_DOCUMENTO:
            MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", _PAGO_NUMERO( h ) );
            break;
        case _TKT_DATO_MEDIO:
            ind_configTmp = 0;
            if( PAGOS_OBTENER_VALOR( h, TKT_tomar_entero( ind_config, &ind_configTmp ), xx, NO ) ) {
                MOSTRAR_CADENA( tkt_columna, PRN, xx );
            }
            break;
        case _TKT_PRECIO_SIN_IVA_EN_TICKET:
            PRECIO_SIN_IVA_EN_TICKET = SI;
            break;
        case _NOMBRE_CLIENTE:
            if( MODO_NEGOCIO == SERVICIOS ) {
                IMPRIMIR_CADENA( tkt_columna, cli_cobro.apellynom );
            }
            else {
                IMPRIMIR_CADENA( tkt_columna, clientes.nombre );
            }
            break;
        case _CUIT_CLIENTE:
            if( MODO_NEGOCIO == SERVICIOS ) {
                IMPRIMIR_CADENA( tkt_columna, cli_cobro.cuit );
            }
            else {
                IMPRIMIR_CADENA( tkt_columna, clientes.cuit );
            }
            break;
        case _DOMICILIO_CLIENTE:
            if( MODO_NEGOCIO == SERVICIOS ) {
                IMPRIMIR_CADENA( tkt_columna, cli_cobro.domicilio );
            }
            else {
                IMPRIMIR_CADENA( tkt_columna, clientes.domicilio );
            }
            break;
        case _LOCALIDAD_CLIENTE:
            if( MODO_NEGOCIO == SERVICIOS ) {
                IMPRIMIR_CADENA( tkt_columna, cli_cobro.localidad );
            }
            else {
                IMPRIMIR_CADENA( tkt_columna, clientes.localidad );
            }
            break;
        case _PROVINCIA_CLIENTE:
            if( MODO_NEGOCIO == SERVICIOS ) {
                IMPRIMIR_CADENA( tkt_columna, cli_cobro.provincia );
            }
            else {
                IMPRIMIR_CADENA( tkt_columna, clientes.provincia );
            }
            break;
        case _OFFSET_CADENA:
            ind_configTmp = 0;
            OFFSET_CADENA = (char)TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_VARIABLE_RNV:
            ind_configTmp = 0;
            cod_var = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            cod_sub = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            /*      strncpy( xx , &CONFIG_TKT[( *ind_config )], sizeof(xx)-1 );
                    ( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
                    MOSTRAR_VARIABLE_RNV( cod_var, cod_sub, tkt_columna, " " );*/
            if( DD_TIPO( cod_var ) == _DOUBLE ) {
                char aux[20];
                memset( aux, 0, 20 );
                CARGAR_VARIABLE_RNV( cod_var, aux, 20, cod_sub, NULL );
                FTOC( strtod( aux, NULL ), aux, ENTEROS + 1, DECIMALES, SI, SI );
                //MOSTRAR_CADENA_ALD( tkt_columna, PRN, aux, 14 );
                MOSTRAR_CADENA( tkt_columna, PRN, aux );
            }
            else {
                MOSTRAR_VARIABLE_RNV( cod_var, cod_sub, tkt_columna, NULL );
            }
            break;
		case _TKT_OFF_VARIABLE_RNV:
            memset(cadena_aux,0,sizeof(cadena_aux));
            ind_configTmp = 0;
            cod_var = ( INT16 )EST_VAR( ( INT16 )_TKT_INI_EST
                                      + ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp ) );
            cod_sub = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            if( cod_sub == 254 ) {
                cod_sub = ( INT16 )_TKT_FOR_I;
            }
            if( DD_TIPO( cod_var ) == _DOUBLE ) {
                char aux[20];
                memset( aux, 0, 20 );
                CARGAR_VARIABLE_RNV( cod_var, aux, 20, cod_sub, NULL );
                //FTOC( strtod( aux, NULL ), aux, ENTEROS + 1, DECIMALES, SI, SI );
                //MOSTRAR_CADENA_ALD( tkt_columna, PRN, aux, 14 );
                _snprintf(cadena_aux,sizeof(cadena_aux)-1,"%.2lf",strtod( aux, NULL ));
				MOSTRAR_CADENA( tkt_columna, PRN, cadena_aux );
            }
            else {
                MOSTRAR_VARIABLE_RNV( cod_var, cod_sub, tkt_columna, NULL );
            }
            break;
        case _TKT_SLIP_ON:
            PRN_HABILITA_SLIP( 1, NO );
			#if defined( INVEL_W) || defined( INVEL_L)
				LF( 0 );
				setData( ACTION_PRINT, NULL, 0 );
            #endif
            break;
        case _TKT_SLIP_OFF:
            /*EJECT();
            PRN_INHABILITA_SLIP();*/
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
        case _TKT_IMPORTE_DONACION:
            memset(cadena_aux,0,sizeof(cadena_aux));
			memset(cadena_aux2,0,sizeof(cadena_aux2));
			//_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "IMPORTE DONADO:  %.2f", RAM_TOTAL_DONACION);
			_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "%.2f", RAM_TOTAL_DONACION);
			if(IMPRESORA_FISCAL == FISCAL)
			{		
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux2, NO );
			}
			else
				MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 0, RAM_TOTAL_DONACION );

            break;
        case _MAXIMO_RENGLONES_TRANSFERENCIA:
            ind_configTmp = 0;
            _LARGO_TRANSFERENCIA = TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_DATO_ARTICULO:
            ind_configTmp = 0;
            if( PAGOS_OBTENER_VALOR( h, TKT_tomar_entero( ind_config, &ind_configTmp ), xx, 1 ) ) {
                MOSTRAR_CADENA( tkt_columna, PRN, xx );
            }
            break;
        case _SI_DATO_ARTICULO:
            ind_configTmp = 0;
            SI_DATO_ARTICULO = TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_OFFSET_COLUMNA:
            ind_configTmp = 0;
            TKT_OFFSET_COLUMNA = TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_LF_RECEIPT:
            ind_configTmp = 0;
            enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
            LF( enteros );
            break;
        case _TKT_MAX_LIN_COMENTARIOS:
            ind_configTmp = 0;
            MOSTRAR_ENTERO( tkt_columna, PRN, "0000",
                            TKT_tomar_entero( ind_config, &ind_configTmp ) );
            break;
        case _TKT_CARACTERES_X_LIN:
            ind_configTmp = 0;
            MOSTRAR_ENTERO( tkt_columna, PRN, "00",
                            TKT_tomar_entero( ind_config, &ind_configTmp ) );
            break;
        case _TKT_NRO_SERIE:
            MOSTRAR_CADENA( tkt_columna, PRN, numero_serie );
            break;
        case _TKT_DISCRIMINACION_IVA:
            MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              _PAGO_IVA_PROPORCIONAL( h ) );
            break;
        case _TKT_TOTAL_PROMOCION:
            precio_total = 0.0;
            for( i = 0 ;i < RAM_P_TICKET ;i++ ) {
                if( _TICKET_ORIGEN( i ) == _ORIGEN_PROMOCION ) {
                    precio_total += _TICKET_PRECIO( i ) * _TICKET_CANTIDAD( i )
                                  * ( ( _TICKET_TIPO( i ) & _RESTA ) ? -1.00 : 1.00 );
                }
            }
            MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 0,
                              ROUND( -precio_total, ENTEROS, DECIMALES ) );
            break;
        case _TKT_SI_TOTAL_PROMOCION:
            total_promo = 0.0;
            for( i = 0 ;i < RAM_P_TICKET ;i++ ) {
                if( _TICKET_ORIGEN( i ) == _ORIGEN_PROMOCION ) {
                    total_promo += _TICKET_PRECIO( i ) * _TICKET_CANTIDAD( i )
                                 * ( ( _TICKET_TIPO( i ) & _RESTA ) ? -1.00 : 1.00 );
                }
            }
            if( fabs( total_promo ) <= 0.0 ) {
                total_promo = 0.0;
            }
            SI_TOTAL_PROMOCION = 1;
            break;
        case _TKT_PAUSA:
            ind_configTmp = 0;
            PAUSA( TKT_tomar_entero( ind_config, &ind_configTmp ) );
            break;
            /*   case _TKT_BEGIN:
                  break;*/
        case _TKT_END:
            _SKIP_BLOCK = 0;
            break;
        case _TKT_FOR:
            ind_configTmp = 0;
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, /*&*/CONFIG_TKT1[( *ind_config )].parametros, sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( /*&*/CONFIG_TKT1[( *ind_config )].parametros ) + 1;
            _TKT_FOR_I = ( INT16 )RESOLVER( cadaux, NULL );
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;
            _TKT_FOR_LIMITE = ( INT16 )RESOLVER( cadaux, NULL );
            _TKT_FOR_IND_CONFIG = *ind_config;
            break;
        case _TKT_END_FOR:
            ind_configTmp = 0;
            _TKT_FOR_I += TKT_tomar_entero( ind_config, &ind_configTmp ); // Incremento de I
            if( _TKT_FOR_I <= _TKT_FOR_LIMITE ) {
                *ind_config = _TKT_FOR_IND_CONFIG;
            }
            break;
        case _TKT_TOTALIZAR_TMP:
            ind_configTmp = 0;
            p1 = TKT_tomar_entero( ind_config, &ind_configTmp );
            p2 = TKT_tomar_entero( ind_config, &ind_configTmp );
            p3 = TKT_tomar_entero( ind_config, &ind_configTmp );
            p4 = TKT_tomar_entero( ind_config, &ind_configTmp );
            p5 = TKT_tomar_entero( ind_config, &ind_configTmp );
            TKT_TOTALIZAR_TEMPORAL( ( int )p1, ( int )p2, ( int )p3, ( int )p4, ( int )p5 );
            break;
        case _TKT_IMPORTE_TOTAL_TMP:
            ind_configTmp = 0;
            p1 = TKT_tomar_entero( ind_config, &ind_configTmp );
            importe_total = 0.0;
            //SELECT( AREA_AUX6 );
    			SELECT_TABLE( T_TKT_TMP, TT_ORIG );
            GO2( TOP );
            while( /*!BtrvEOF()*/ !dbEOF() ) {
                importe_total += tkt_tmp->importe;
                SKIP2( 1 );
            }
            if( p1 != 0 ) {
                importe_total *= -1;
            }
            MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 1,
                              ROUND( importe_total, ENTEROS, DECIMALES ) );
            break;
        case _TKT_CANTIDAD_TOTAL_TMP:
            cantidad = 0;
            //SELECT( AREA_AUX6 );
			SELECT_TABLE( T_TKT_TMP, TT_ORIG );
            GO2( TOP );
            while( /*!BtrvEOF()*/ !dbEOF() ) {
                cantidad += tkt_tmp->cantidad;
                SKIP2( 1 );
            }
            MOSTRAR_ENTERO( tkt_columna, PRN, "0000", cantidad );
            break;
        case _TKT_WHILE_TMP:
            //SELECT( AREA_AUX6 );
			SELECT_TABLE( T_TKT_TMP, TT_ORIG );
            GO2( TOP );
            _TKT_WHILE_IND_CONFIG = *ind_config;
            if( /*BtrvEOF()*/ dbEOF() ) {
                /*codigo = CONFIG_TKT1[( *ind_config )].codigo;
                ( *ind_config )++;
                while( codigo != _TKT_END_WHILE_TMP && codigo != _FIN ) {
                    //TKT_incrementar_puntero( codigo, ind_config );
                    codigo = CONFIG_TKT1[( *ind_config )-1].codigo;
                    ( *ind_config )++;
                }*/
                ( *ind_config )++;
                while( CONFIG_TKT1[( *ind_config )].codigo != _TKT_END_WHILE_TMP
                    && CONFIG_TKT1[( *ind_config )].codigo != _FIN) {
                    ( *ind_config )++;
                }
                //CLOSE_DATABASE( AREA_AUX6 );
				CLOSE_TABLE( T_TKT_TMP, TT_ORIG );
            }
            break;
        case _TKT_END_WHILE_TMP:
            //SELECT( AREA_AUX6 );
			SELECT_TABLE( T_TKT_TMP, TT_ORIG );
            if( /*!BtrvEOF()*/ !dbEOF() ) {
                SKIP2( 1 );
            }
            if( /*!BtrvEOF()*/ !dbEOF() ) {
                *ind_config = _TKT_WHILE_IND_CONFIG;
            }
            else {
                //CLOSE_DATABASE( AREA_AUX6 );
				CLOSE_TABLE( T_TKT_TMP, TT_ORIG );
            }
            break;
        case _TKT_IMPORTE_TMP:
            MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, 1, 1,
                              ROUND( tkt_tmp->importe, ENTEROS, DECIMALES ) );
            break;
        case _TKT_CANTIDAD_TMP:
            MOSTRAR_ENTERO( tkt_columna, PRN, "0000", tkt_tmp->cantidad );
            break;
        case _TKT_DESCRIPCION_TMP:
            MOSTRAR_CADENA( tkt_columna, PRN, tkt_tmp->descripcion );
            break;
        case _TKT_DESC_MEDIO:
            ind_configTmp = 0;
            cod_medio = TKT_tomar_entero( ind_config, &ind_configTmp );
            if( cod_medio == 254 ) {
                cod_medio = _TKT_FOR_I + 1;
            }
            if( cod_medio > 0 && cod_medio < 10 ) {
                MOSTRAR_CADENA( tkt_columna, PRN, medios[cod_medio - 1].nombre );
            }
            break;  
		case _TKT_SI_VALOR_VARIABLE_RNV:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV:
            ind_configTmp = 0;
            _COMPARACION_IGUAL = ( codigo == _TKT_SI_VALOR_VARIABLE_RNV ) ? SI : NO;
            _COD_VARIABLE_RNV = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            _SUB_VARIABLE_RNV = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            if( _SUB_VARIABLE_RNV == 254 ) {
                _SUB_VARIABLE_RNV = _TKT_FOR_I;
            }
            _VALOR_RNV = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_SI_VALOR_VARIABLE_RNV_OFF:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV_OFF:
            ind_configTmp = 0;
            _COMPARACION_IGUAL = ( codigo == _TKT_SI_VALOR_VARIABLE_RNV ) ? SI : NO;    //mec
            _COD_VARIABLE_RNV = EST_VAR( _TKT_INI_EST
                                       + ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp ) );
            _SUB_VARIABLE_RNV = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            if( _SUB_VARIABLE_RNV == 254 ) {
                _SUB_VARIABLE_RNV = _TKT_FOR_I;
            }
            _VALOR_RNV = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _TKT_SI_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
            ind_configTmp = 0;
            _COMPARACION_IGUAL = ( codigo == _TKT_SI_VALOR_VARIABLE_RNV_Y_SI_MEDIO ) ? SI : NO;
            _COD_VARIABLE_RNV = TKT_tomar_entero( ind_config, &ind_configTmp );
            _SUB_VARIABLE_RNV = TKT_tomar_entero( ind_config, &ind_configTmp );
            if( _SUB_VARIABLE_RNV == 254 ) {
                _SUB_VARIABLE_RNV = _TKT_FOR_I;
            }
            _VALOR_RNV = TKT_tomar_entero( ind_config, &ind_configTmp );
            SI_CODIGO_MEDIO = (char)TKT_tomar_entero( ind_config, &ind_configTmp );
            break;
        case _SI_EXENTO:
            SI_T_EXENTO = 1;
            HAY_T_EXENTO = 0;
            if( OBTENER_COD_GRAVADO()!=1 /*|| EXENTO_COMO_GRAVADO( OBTENER_COD_GRAVADO()) || !EXENTO_PARA_DIPLOMATICOS()*/) {
                HAY_T_EXENTO = 1;
            }
            break;
        case _SI_GRAVADO:
            SI_T_GRAVADO = 1;
            HAY_T_GRAVADO = 0;
            if( OBTENER_COD_GRAVADO()==1 /*|| EXENTO_PARA_DIPLOMATICOS()*/) {
                HAY_T_GRAVADO = 1;
            }
            break;
        case _TKT_LETRA_NEGRITA:
            PRN_LETRA_NEGRITA(  );
            break;
        case _TKT_DESHABILITAR_FORMATOS_LETRAS:
            PRN_DESHABILITA_FORMATOS_LETRAS(  );
            break;
        case _TKT_REPETIR_CADENA:
            {
            char text[200];

            ind_configTmp = 0;
            _SI_CANT_IMPRESION = 0;

            memset( _SI_CAD_IMPRESION, 0, sizeof( _SI_CAD_IMPRESION ) );
            memset( text, 0 ,100);

            strncpy( _SI_CAD_IMPRESION, CONFIG_TKT1[( *ind_config )].parametros, sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( CONFIG_TKT1[( *ind_config )].parametros ) + 1;

             _SI_CANT_IMPRESION = TKT_tomar_entero( ind_config, &ind_configTmp );

             _SI_REPETIR_CADENA = &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp];

             //Para que imprima la primer linea con la cadena ingresada
             LINEA_IMPRESA = _SI_CANT_IMPRESION;

            }
            break;
        case _TKT_SI_FORMULA:
            _SI_P_FORMULA = /*&*/CONFIG_TKT1[( *ind_config )].parametros;
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            break;
        case _TKT_FORMULA:
            ind_configTmp = 0;
            strncpy( xx, /*&*/CONFIG_TKT1[( *ind_config )].parametros, sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( /*&*/CONFIG_TKT1[( *ind_config )].parametros ) + 1;
            strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
            strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;

			/*Para imprimir una variable de la rnv del tipo string, solo hay que agregar
			 *al inicio de la variable, la palabra "string", así se puede devolver el
			 * valor de la variable en un tipo de dato string */
			if( strncmp( cadaux, "escadena", 8 )== 0 ) {
				//el dato a devolver será un 0 en importe total
				//Dato para calcular variables rnv off
				val_calc.tkt_ini_est = _TKT_INI_EST;
				importe_total = RESOLVER( xx, &val_calc );
			} else {
				//Dato para calcular variables rnv off
				val_calc.tkt_ini_est = _TKT_INI_EST;
				importe_total = RESOLVER( xx, &val_calc );

				//       if( cadaux2[0] ) {
				if( cadaux[0] ) {
					FORMAT_FLOTANTE( cadaux, importe_total, xx );
					MOSTRAR_CADENA( tkt_columna, PRN, xx );
				}
				else {
					RESOLVER_ASIGNA_NOMBRES( cadaux2,  xx );
					sprintf( xx, "%f", importe_total );
					RESOLVER_FUNCIONES( cadaux2, xx, CONFIG_TKT1[( *ind_config )].linea );
					//MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, SI, NO, importe_total );
				}
			}
            break;
        case _PROCESAR_DATOS_ADICIONALES:
			{
			long id_evento = 0;
            ind_configTmp = 0;
			
            cod_sol = TKT_tomar_entero( ind_config, &ind_configTmp );
            nro_orden = TKT_tomar_entero( ind_config, &ind_configTmp );
            modalidad = TKT_tomar_entero( ind_config, &ind_configTmp ); // archivo binario o tabla sic

            //si el medio es cheque, si es modo cheque como un medio y modalidad por archivo
            //entonces calcula el indice del archivo
            if(medios[_PAGO_MODO( h )].cheque && MODO_CHEQUE_COMO_UN_MEDIO && modalidad )
                h2 = ( _PAGO_MODO( h ) * 100 ) + h2;
            //Marcos  D_MEDIOS_PRE_IMPORTE -> 0
			if( RAM_IMPORTACION_ACTIVA ) {
				//Si el ticket es una importacion con datos adicionales los busca por tabla
				modalidad = 0;//Buscar por tabla
				id_evento = RAM_ID_EVENTO; //Buscar evento en curso
			} else {
				id_evento = ( MODO_NEGOCIO != SERVICIOS )?RAM_ID_EVENTO - 1 : NRO_ULTIMO_EVENTO_ANALIZADO;
			}
            CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( ( int )cod_sol, ( int )nro_orden, id_evento,
				( int )modalidad, D_MEDIOS_PRE_IMPORTE, RAM_P_PAGO, h2 );   //tipo deberia ser 1   
			}
            break;
        case _DATO_ADICIONAL_MEDIO_PAGO:
            ind_configTmp = 0;
            memset( xx, 0, sizeof( xx ) );
            posicion = TKT_tomar_entero( ind_config, &ind_configTmp );
            strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            if( posicion == 254 ) {
                posicion = _TKT_FOR_I;
            }
            if( BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( posicion, xx ) ) {
                //MOSTRAR_CADENA(tkt_columna, PRN, xx );
                RESOLVER_FUNCIONES( cadaux2, xx, CONFIG_TKT1[( *ind_config )].linea );
            }
            break;
        case _DNI_CLIENTE:
            if( MODO_NEGOCIO != SERVICIOS ) {
                MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", clientes.nro_documento );
            }
            else {
                MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", cli_cobro.nro_documento );
            }
            break;
        case _TKT_CONSULTA_TABLA:
            ind_configTmp = 0;
            codtabla = TKT_tomar_entero( ind_config, &ind_configTmp );
            orden = TKT_tomar_entero( ind_config, &ind_configTmp );

            strncpy( xx, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;

            strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;

            strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;

            strncpy( cadaux3, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux3 ) - 1 );
            //( ind_configTmp ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            {
                unsigned int cnt;
                char strTemp[100] = {""};
                char strTemp2[5] = {""};
                /* primero se fija si tiene que asignar h que es el pago actual */
                for( cnt = 0;cnt < strlen( xx );cnt++ ) {
                    if( xx[cnt] == 'H' ) {
                        sprintf( strTemp2, "%i", h + 1 );
                    }
                    if( xx[cnt] == 'h' ) {
                        sprintf( strTemp2, "%i", h );
                    }
                    if( strlen( strTemp2 ) > 0 ) {
                        strcat( strTemp, strTemp2 );
                        //strTemp2[0]=0;
                        cnt = cnt + strlen( strTemp2 );
                    }
                    else {
                        strTemp[cnt] = xx[cnt];
                    }
                    strTemp[cnt + 1] = 0;
                }
                strcpy( xx, strTemp );
            }
            OBTENER_DATO_TABLA( ( int )codtabla, ( int )orden, xx, cadaux, cadaux2/*,CONFIG_TKT1[( *ind_config )].linea*/ );
            RESOLVER_FUNCIONES( cadaux3, cadaux2, CONFIG_TKT1[( *ind_config )].linea );
            /*if(strlen(cadaux2) > 0) //si se encontro el dato lo muestra
              MOSTRAR_CADENA( tkt_columna, PRN, cadaux2);
            else    // sino muestra el dato por defecto
              MOSTRAR_CADENA( tkt_columna, PRN, cadaux3);*/
            break;
        case _DATO_SUBMEDIO_TAR:
            //if( MODO_NEGOCIO != SERVICIOS ) {
                //INFORME_TARJETAS_CUERPO( h );
            //}
			{
			int cant = 0, i = 0;
			int	tabla_ant = SELECTED2();
			int tipo_ant = SELECTED_TIPO();
			struct _tarjetas tarjeta_sic;

			if( OPEN_TABLE(T_TARJETAS, TT_ORIG,( char* )&tarjeta_sic,sizeof( struct _tarjetas )) == 0 ) {
				SET_ORDER2("COD_TARJETA");
				SET_WHERE("");
				RUN_QUERY(NO); 

				while( !dbEOF() ) {
					if( MODO_NEGOCIO != SERVICIOS ) {
						INFORME_TARJETAS_CUERPO( tarjeta_sic.cod_tarjeta - 1);
					}
					SKIP2( 1 );
				}

				CLOSE_TABLE( T_TARJETAS, TT_ORIG );
				SELECT_TABLE(tabla_ant, tipo_ant);
				}
			}
            break;
            //SIN_LOTE_TARJETA
            /* case _DATO_LOTE_TAR:
                    if(MODO_NEGOCIO != SERVICIOS)
                       INFORME_TARJETAS_LOTE( h );
                   break;*/
        case _ENVIAR_BMP:
            if(!calcHojas){
                ind_configTmp = 0;
                strncpy( ultimaImagen, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                         sizeof( ultimaImagen ) - 1 );
                ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
              + 1;
                p1 = TKT_tomar_entero( ind_config, &ind_configTmp );
                p2 = TKT_tomar_entero( ind_config, &ind_configTmp );
                sprintf(xx,"%d#%d",p1,p2);
                setData( ACTION_SET_TAM_IMAGE, xx, 0 );
                //sprintf( ultimaImagen, /*&*/CONFIG_TKT1[( *ind_config )].parametros );
                setData( ACTION_IMAGE, ultimaImagen, 0 );
            }
            //( *ind_config ) += strlen( &CONFIG_TKT[( *ind_config )] ) + 1;
            break;
        case _SET_FONT:
            ind_configTmp = 0;
            strncpy( xx, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;
            font_size = TKT_tomar_entero( ind_config, &ind_configTmp );
            setData( ACTION_SET_FONT, xx, font_size );
            break;
        case _TKT_VERSION:
            ind_configTmp = 0;
            TKT_tomar_entero( ind_config, &ind_configTmp );
            TKT_tomar_entero( ind_config, &ind_configTmp );
            strncpy( xx, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;
            break;
        case _TKT_ULTIMO_ID:
            ind_configTmp = 0;
            {
            long id_return;
            INT16 tipo, variable;
            char strAsignar[20];
            tipo = TKT_tomar_entero( ind_config, &ind_configTmp );
            strncpy( xx, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
            id_return = ULTIMO_ID_EVENTO( tipo );
            variable = atoi( xx );
            sprintf( strAsignar, "asignar(%d,%d)", variable, id_return );
            RESOLVER( strAsignar, NULL );
			//Esto biene a ser una operacion sobre la ticket o la ticket_bak2 abierta en ULTIMO_ID_EVENTO
			SET_ORDER( 1 );
            GET_EQUAL( ( char* )&id_return );
            }
            break;
        case _TKT_CANT_EVENTOS:
            ind_configTmp = 0;
            {
            long cant_eventos_return;
            INT16 tipo, variable;
            char strAsignar[20];
            tipo = TKT_tomar_entero( ind_config, &ind_configTmp );
            strncpy( xx, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( xx ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
            cant_eventos_return = CANTIDAD_EVENTOS( tipo, NO );
            variable = atoi( xx );
            sprintf( strAsignar, "asignar(%d,%d)", variable, cant_eventos_return );
            RESOLVER( strAsignar, NULL );
            }
            break;
        case _TKT_ID_EVENTO_NRO:
            ind_configTmp = 0;
            {
            long id_evento_return;
            INT16 nro_evento, tipo_evento, variable;
            char strAsignar[20];
            memset( xx, 0, sizeof( xx ) );
            strncpy( xx, /*&*/CONFIG_TKT1[( *ind_config )].parametros, sizeof( xx ) - 1 );
            nro_evento = ( INT16 )RESOLVER( xx, NULL );
            ( ind_configTmp ) += strlen( /*&*/CONFIG_TKT1[( *ind_config )].parametros ) + 1;
            tipo_evento = TKT_tomar_entero( ind_config, &ind_configTmp );
            variable = TKT_tomar_entero( ind_config, &ind_configTmp );
            id_evento_return = ID_EVENTO_NRO( nro_evento, tipo_evento );
            sprintf( strAsignar, "asignar(%d,%d)", variable, id_evento_return );
            RESOLVER( strAsignar, NULL );
            }
            break;
        case _TKT_CONSULTA_TABLA_SQL:
            ind_configTmp = 0;
            memset( cadaux, 0, sizeof( cadaux ) );
            strncpy( cadaux, /*&*/CONFIG_TKT1[( *ind_config )].parametros, sizeof( cadaux ) - 1 );
            ( ind_configTmp ) += strlen( /*&*/CONFIG_TKT1[( *ind_config )].parametros ) + 1;
            memset( cadaux2, 0, sizeof( cadaux2 ) );
            strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;
            memset( cadaux3, 0, sizeof( cadaux3 ) );
            strncpy( cadaux3, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux3 ) - 1 );
            ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
          + 1;
            RESOLVER_TKT_CONSULTA_SQL(cadaux2, cadaux3, cadaux, CONFIG_TKT1[( *ind_config )].linea);
            break;
		case _NUMERO_TARJETA:
			{
				int marca_tarjeta = 0;
				char temp[21], numero_tarjeta[21];
				memset( temp, 0, sizeof( temp ) );
				memset( numero_tarjeta, 0, sizeof( numero_tarjeta ) );

				if( ENCRIPTAR_DATOS_DE_TARJETAS_PRIV ) {
					marca_tarjeta = VAR_TARJETAS_TARJETA;
					_snprintf( numero_tarjeta, sizeof( numero_tarjeta ), "%s",
							GET_NUMERO_TARJETA_PRIVADO( ));
					if( TARJ[marca_tarjeta].enmasc_nro_tarjeta == 's'
						|| TARJ[marca_tarjeta].enmasc_nro_tarjeta == 'S' )
					{
						strncpy( &numero_tarjeta[2], MASCARA( '*', 6, "" ), 6 );
					}
					strncpy( temp, numero_tarjeta, sizeof( temp ) -1 );
				} else {
					if( IMPRESORA_FISCAL == FISCAL ) {
						marca_tarjeta = tarjetas_fisc_rec[h2].marca_tarjeta;
						_snprintf( numero_tarjeta, sizeof( numero_tarjeta ), "%s",
								tarjetas_fisc_rec[h2].numero_tarjeta );
					} else {
						marca_tarjeta = _PAGO_TARJETA( h );
						_snprintf( numero_tarjeta, sizeof( numero_tarjeta ), "%s",
								_PAGO_NRO_TARJETA( h ) );
					}
					ENMASCARA_NUMERO_TARJETA( marca_tarjeta, numero_tarjeta, temp );
				}
				MOSTRAR_CADENA( tkt_columna, PRN, temp );
				strncpy( cardNum, temp, sizeof( cardNum ) );
				//strcpy( cardNum, temp );
			}
			break;
		case _TKT_SALDO_TARJETA:
			ind_configTmp = 0;
            enteros = TKT_tomar_entero( ind_config, &ind_configTmp );
            decimales = TKT_tomar_entero( ind_config, &ind_configTmp );
			MOSTRAR_FLOTANTE( tkt_columna, PRN, enteros, decimales, 1, 1,
				ROUND( VAR_TARJETAS_SALDO_CLIENTE, enteros, decimales ) );
            break;
		case _NRO_AUTO_TARJETA:
			{
				char nro_auto[30];
				int l_buffer = 0;
				char buffer1[30];
				memset( buffer1, 0, sizeof(buffer1) );
				memset( nro_auto, 0, sizeof(nro_auto) );
				//sprintf( nro_auto, VAR_TARJETAS_AUTORIZACION, strlen( VAR_TARJETAS_AUTORIZACION ) );
				sprintf( buffer1, VAR_TARJETAS_AUTORIZACION, strlen( VAR_TARJETAS_AUTORIZACION ) );
				//l_buffer = strlen( nro_auto );
				l_buffer = strlen( buffer1 );
				//memcpy( buffer1, nro_auto, l_buffer );
				memcpy( nro_auto, buffer1, l_buffer );
				nro_auto[l_buffer] = 0;
				MOSTRAR_CADENA( tkt_columna, PRN, nro_auto );
			}
			break;
        case _TKT_SI_VALOR_MED :
            {
                int medio,submedio;
                INT16 resultado;
                char aresolver[50];
                ind_configTmp = 0;
                strncpy( xx, /*&*/CONFIG_TKT1[( *ind_config )].parametros, sizeof( xx ) - 1 );
                ( ind_configTmp ) += strlen( /*&*/CONFIG_TKT1[( *ind_config )].parametros ) + 1;
                strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                        sizeof( cadaux ) - 1 );
                ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
            + 1;
                resultado = TKT_tomar_entero( ind_config, &ind_configTmp );
                medio = ( INT16 )RESOLVER( xx, NULL );
                submedio = ( INT16 )RESOLVER( cadaux, NULL );
                sprintf( aresolver, "asignar(%d,0)", resultado );
                RESOLVER( aresolver, NULL );
                if( submedio == 0 ) {
                    for( i = 0;i < 30;i++ ) {
                        if( dif_submedios[medio].imp_declarado[i] != 0
                        || dif_submedios[medio].imp_real[i] != 0 ) {
                            sprintf( aresolver, "asignar(%d,1)", resultado );
                            RESOLVER( aresolver, NULL );
                            break;
                        }
                    }
                }
                else {
                    if( ( dif_submedios[medio].imp_declarado[submedio - 1] != 0
                    || dif_submedios[medio].imp_real[submedio - 1] != 0 )
                    && dif_submedios[medio].cod_submedio[submedio - 1] != 0 ) {
                        sprintf( aresolver, "asignar(%d,1)", resultado );
                        RESOLVER( aresolver, NULL );
                        i = 30;
                    }
                }
            }
            break;
        case _TKT_DATO_MEDIO_SUBMEDIO :
            {
                //char cadena[22];
                int medio,submedio,i,j;
                double total;
                INT16 resultado;
                ind_configTmp = 0;
				memset(cadena,0,sizeof(cadena));
                strncpy( xx, /*&*/CONFIG_TKT1[( *ind_config )].parametros, sizeof( xx ) - 1 );
                ( ind_configTmp ) += strlen( /*&*/CONFIG_TKT1[( *ind_config )].parametros ) + 1;
                strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                        sizeof( cadaux ) - 1 );
                ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
                resultado = TKT_tomar_entero( ind_config, &ind_configTmp );
                strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                        sizeof( cadaux ) - 1 );
                ( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
                medio = ( INT16 )RESOLVER( xx, NULL );
                submedio = ( INT16 )RESOLVER( cadaux, NULL );
                switch( resultado ) {
                    case 1:
                        if( submedio == 0 ) {
                            RESOLVER_FUNCIONES( cadaux2, dif_submedios[medio].nombre_medio,
                                                CONFIG_TKT1[( *ind_config )].linea );
                        }
                        else {
                            RESOLVER_FUNCIONES( cadaux2,
                                                dif_submedios[medio].nombre_submedio[submedio - 1],
                                                CONFIG_TKT1[( *ind_config )].linea );
                        }
                        break;
                    case 2:
                        if( medio == 10 ) {
                            total = 0;
                            for( i = 0;i < 10;i++ ) {
                                for( j = 0;j < 30;j++ ) {
                                    if( dif_submedios[i].cod_submedio[j] != 0 || j == 0 ) {
                                        total += dif_submedios[i].imp_declarado[j];
                                    }
                                }
                            }
                            //sprintf( cadena, "%f", total );
							_snprintf( cadena, sizeof( cadena ),"%f", total );
							cadena[sizeof( cadena ) - 1] = '\0';
                            RESOLVER_FUNCIONES( cadaux2, cadena, CONFIG_TKT1[( *ind_config )].linea );
                        }
                        else {
                            if( submedio == 0 ) {
                                i = 0;
                                total = dif_submedios[medio].imp_declarado[i];
                                i++;
                                do {
                                    if( dif_submedios[medio].cod_submedio[i] != 0 ) {
                                        total = total + dif_submedios[medio].imp_declarado[i];
                                    }
                                    i++;
                                }
                                while( i < 30 );
                                //sprintf( cadena, "%f", total );
								_snprintf( cadena, sizeof( cadena ),"%f", total );
								cadena[sizeof( cadena ) - 1] = '\0';
                                RESOLVER_FUNCIONES( cadaux2, cadena,
                                                    CONFIG_TKT1[( *ind_config )].linea );
                            }
                            else {
                                //sprintf( cadena, "%f", dif_submedios[medio].imp_declarado[submedio - 1] );
								_snprintf( cadena, sizeof( cadena ),"%f", dif_submedios[medio].imp_declarado[submedio - 1]);
								cadena[sizeof( cadena ) - 1] = '\0';
                                RESOLVER_FUNCIONES( cadaux2, cadena,
                                                    CONFIG_TKT1[( *ind_config )].linea );
                            }
                        }
                        break;
                    case 3:
                        if( medio == 10 ) {
                            total = 0;
                            for( i = 0;i < 10;i++ ) {
                                for( j = 0;j < 30;j++ ) {
                                    if( dif_submedios[i].cod_submedio[j] != 0 || j == 0 ) {
                                        total += dif_submedios[i].imp_real[j];
                                    }
                                }
                            }
                            //sprintf( cadena, "%f", total );
							_snprintf( cadena, sizeof( cadena ),"%f", total );
							cadena[sizeof( cadena ) - 1] = '\0';
                            RESOLVER_FUNCIONES( cadaux2, cadena, CONFIG_TKT1[( *ind_config )].linea );
                        }
                        else {
                            if( submedio == 0 ) {
                                i = 0;
                                total = dif_submedios[medio].imp_real[i];
                                i++;
                                do {
                                    if( dif_submedios[medio].cod_submedio[i] != 0 ) {
                                        total = total + dif_submedios[medio].imp_real[i];
                                    }
                                    i++;
                                }
                                while( i < 30 );
                                //sprintf( cadena, "%f", total );
								_snprintf( cadena, sizeof( cadena ),"%f", total );
								cadena[sizeof( cadena ) - 1] = '\0';
                                RESOLVER_FUNCIONES( cadaux2, cadena,
                                                    CONFIG_TKT1[( *ind_config )].linea );
                            }
                            else {
                                //sprintf( cadena, "%f", dif_submedios[medio].imp_real[submedio - 1] );
								_snprintf( cadena, sizeof( cadena ),"%f", dif_submedios[medio].imp_real[submedio - 1] );
								cadena[sizeof( cadena ) - 1] = '\0';
                                RESOLVER_FUNCIONES( cadaux2, cadena,
                                                    CONFIG_TKT1[( *ind_config )].linea );
                            }
                        }
                        break;
                }
            }
            break;
		case _TKT_TOTAL_IMPUESTOS:
			memset( tot_imp, 0, sizeof( tot_imp ) );
			memset( cod_iva, 0, sizeof( cod_iva ) );
            ind_imp.caja_z = event_e_ticket.caja_z;
            ind_imp.id_evento = event_e_ticket.id_evento;
			//area_ant = SELECTED(  );
			tabla_ant = SELECTED2();
			tipo_ant = SELECTED_TIPO();
			//SELECT( AREA_E_TICKET );
			//SET_ORDER( 2 );
			SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
			SET_WHERE("ID_EVENTO = '%ld' and CAJA_Z = '%ld'",
				ind_imp.id_evento,
				ind_imp.caja_z);
			SET_ORDER2("CAJA_Z,ID_EVENTO");
			for( i = 1; i < 40 && RAM_COD_FUNCION != 16
					&& RAM_COD_FUNCION != 82 && RAM_COD_FUNCION != 106; i++ ) {
				//GET_EQUAL( ( char* ) &ind_imp );
				RUN_QUERY(NO);
				/*if(FOUND2()){
					//SET_WHERE("CONCAT(CAJA_Z,ID_EVENTO) >= CONCAT(%ld,%ld)"
					SET_WHERE("(CAJA_Z = %ld and ID_EVENTO >= %ld) or CAJA_Z > %ld",
							ind_imp.caja_z,
							ind_imp.id_evento,
							ind_imp.caja_z);
					RUN_QUERY(NO);
				}*/
                while( !dbEOF() ) {
					signo = ( event_e_ticket.tipo & _RESTA ) ? -1 : 1;
					if( impuestos[i].cod_impuesto == event_e_ticket.cod_impuesto[0] ) {
						tot_imp[i] += event_e_ticket.monto_impuesto[0] * event_e_ticket.cantidad * signo;
						cod_iva[i]  = event_e_ticket.cod_iva;
					}
					SKIP2( 1 );
				}
			}
			for( i = 1; i < 40 && RAM_COD_FUNCION != 16
					&& RAM_COD_FUNCION != 82 && RAM_COD_FUNCION != 106; i++ ) {
				if( fabs( tot_imp[i] ) > 0.0001 ) {
					char aux[60];
					memset( letra, 0, sizeof( letra ) );
					memset( aux, 0, sizeof( aux ) );
					LETRA_IMPUESTO( impuestos[i].cod_impuesto, (int)cod_iva[i], event_e_ticket.exento, letra );
					strcpy( aux, impuestos[i].nom_impuesto );
					strcat( aux, letra );
					strcat( aux, " :" );
					sprintf( aux, "%-20s        %10.2lf", aux, ROUND( tot_imp[i],
						ENTEROS, DECIMALES ) );
					MOSTRAR_CADENA( tkt_columna, PRN, aux );
					LF( 1 );
				}
			}
			//SELECT( area_ant );
			SELECT_TABLE( tabla_ant, tipo_ant );
			break;
			case _TKT_LETRA_IMPUESTO:
				memset( cod_iva, 0, sizeof( cod_iva ) );
				ind_imp.caja_z = event_e_ticket.caja_z;
				ind_imp.id_evento = event_e_ticket.id_evento;
				//area_ant = SELECTED(  );
				tabla_ant = SELECTED2();
				tipo_ant = SELECTED_TIPO();
				//SELECT( AREA_E_TICKET );
				//SET_ORDER( 2 );
				//GET_EQUAL( ( char* ) &ind_imp );
				SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
				SET_WHERE("ID_EVENTO = '%ld' and CAJA_Z = '%ld'",
						ind_imp.id_evento,
						ind_imp.caja_z);
				SET_ORDER2("CAJA_Z,ID_EVENTO");
				RUN_QUERY(NO);
				/*if(FOUND2()){
					//SET_WHERE(/*"CONCAT(CAJA_Z,ID_EVENTO) >= CONCAT(%ld,%ld)"
					SET_WHERE("(CAJA_Z = %ld and ID_EVENTO >= %ld) or CAJA_Z > %ld",
							ind_imp.caja_z,
							ind_imp.id_evento,
							ind_imp.caja_z);
					RUN_QUERY(NO);
				}*/
				while( !dbEOF( ) && seguir ) {
					if( event_e_ticket.posicion == h + 1 ) {
						memset( letra, 0, sizeof( letra ) );
						LETRA_IMPUESTO( event_e_ticket.cod_impuesto[0], event_e_ticket.cod_iva , event_e_ticket.exento, letra);
						IMPRIMIR_CADENA( tkt_columna, letra );
						seguir = 0;
					}
					 SKIP2( 1 );
				}
				//SELECT( area_ant );
				SELECT_TABLE( tabla_ant, tipo_ant );
				break;
			case _TKT_MENSAJE_ESPECIAL:
				MENSAJE( CONFIG_TKT1[( *ind_config )].parametros );
				 break;
			case _TKT_ESPERAR_ENTER:
				while( GETCH() != 13 );
				BORRAR_MENSAJE();
				break;
			case _TKT_DETALLE_SUBMEDIOS:
				IMPRIMIR_INFORME_MEDIOS( );
				break;
			case _TKT_CODIGO_BARRA_COBROS:
				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * )&datosTmp,
							sizeof( struct _datos_cobro ), 1 );
				MOSTRAR_CADENA( tkt_columna, PRN, datosTmp.cod_barra );
				break;
			case _TKT_CODIGO_BARRA_ARTICULO_COBROS:

				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char * )&datosTmp,
							sizeof( struct _datos_cobro ), 1 );

				MOSTRAR_CADENA( tkt_columna, PRN, datosTmp.cod_barra_articulo );
				break;
            case _TKT_INTERLINEADO:
                ind_configTmp = 0;
                _snprintf( str_tmp, sizeof( str_tmp ), "%i", TKT_tomar_entero( ind_config, &ind_configTmp ) );
                setData( ACTION_SET_PRINT_LINE_SPACE, str_tmp, sizeof( str_tmp ) );
                break;
            case _TKT_CARACTERES_LINEA:
                ind_configTmp = 0;
                _snprintf( str_tmp, sizeof( str_tmp ), "%i", TKT_tomar_entero( ind_config, &ind_configTmp ) );
                setData( ACTION_SET_PRINT_LINE_CHARS, str_tmp, sizeof( str_tmp ) );
                break;
			case _TKT_IMPORTE_DE_RETIROS:
				if( acu_retirado != NULL )
				{
					memset(cadena_aux,0, sizeof(cadena_aux));
					memset(cadena_aux2, 0, sizeof(cadena_aux2));
					if(importe_retiro_ff > 0.0)
					{
						_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "%.2f", importe_retiro_ff);
						if(IMPRESORA_FISCAL == FISCAL)
						{
							_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%s", "FONDO FIJO ");
							//Agregamos espacios
							for(i = strlen(cadena_aux); i < 31; i++)
								cadena_aux[i]= ' ';
							//Alineación a la derecha
							for(i = 0; i <= strlen(cadena_aux2); i++)
								cadena_aux[30 - i]= cadena_aux2[strlen(cadena_aux2)- i];
							cadena_aux[31]=0;
							//strncat( cadena_aux, cadena_aux2, sizeof( cadena_aux ) - strlen( cadena_aux ) - 1 );
							IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux, NO );
							LF(1);
						}
						else
						{
							MOSTRAR_CADENA( tkt_columna, PRN, ST( S_FONDO_FIJO ));
							MOSTRAR_CADENA( (tkt_columna+21), PRN, cadena_aux2);
							LF(1);
						}
						total += importe_retiro_ff;
						memset(cadena_aux,0, sizeof(cadena_aux));
						copia ++;
					}
					for( h = 0;h < 10;h++ ) 
					{
						if( medios[h].nro && ( fabs( acu_retirado[h] ) >  0.00 ) ) 
						{
							tabla_ant = SELECTED2();
							tipo_ant = SELECTED_TIPO();
							if( medios[h].tarjeta ) {
								for( j = 0; j <= CANT_SUB_RET; j++ )
								{
									if( acu_ret[h][j] >  0.00 )
									{
										_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "%.2f", acu_ret[h][j]);
										if( IMPRESORA_FISCAL == FISCAL ) 
										{
											_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%s ", NOMBRE_TARJETA( j - 1 ));
											//Agregamos espacios
											for(i = strlen(cadena_aux); i < 31; i++)
												cadena_aux[i]= ' ';
											//Alineación a la derecha
											for(i = 0; i <= strlen(cadena_aux2); i++)
												cadena_aux[30 - i]= cadena_aux2[strlen(cadena_aux2)- i];
											cadena_aux[31]=0;											
											//strncat( cadena_aux, cadena_aux2, sizeof( cadena_aux ) - strlen( cadena_aux ) - 1 );
											IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA, cadena_aux, NO );
										}
										else
										{
											MOSTRAR_CADENA( tkt_columna, PRN, NOMBRE_TARJETA( j - 1 ) );
											MOSTRAR_CADENA( tkt_columna, PRN, cadena_aux2 );
										}
										LF(1);
										memset(cadena_aux,0, sizeof(cadena_aux));
										memset(cadena_aux2,0, sizeof(cadena_aux2));
									}
								}
							}
							else if( medios[h].mutual ) {
								for( j = 0; j <= CANT_SUB_RET; j++ )
								{
									if( acu_ret[h][j] >  0.00 )
									{
										NOMBRE_MUTUAL( j, xx );										
										_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "%.2f", acu_ret[h][j]);
										if(IMPRESORA_FISCAL == FISCAL)
										{
											_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%s ", xx);
											//Agregamos espacios
											for(i = strlen(cadena_aux); i < 31; i++)
												cadena_aux[i]= ' ';
											//Alineación a la derecha
											for(i = 0; i <= strlen(cadena_aux2); i++)
												cadena_aux[30 - i]= cadena_aux2[strlen(cadena_aux2)- i];
											cadena_aux[31]=0;
											//strncat( cadena_aux, cadena_aux2, sizeof( cadena_aux ) - strlen( cadena_aux ) - 1 );
											IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA, cadena_aux, NO );
										}
										else
										{
											MOSTRAR_CADENA( tkt_columna, PRN, xx );
											MOSTRAR_CADENA( tkt_columna, PRN, cadena_aux2 );
										}
										LF(1);
										memset(xx,0, sizeof(xx));
										memset(cadena_aux,0, sizeof(cadena_aux));
										memset(cadena_aux2,0, sizeof(cadena_aux2));
									}
								}
							}
							else if( medios[h].ticket_compra ) {
								for( j = 0; j <= CANT_SUB_RET; j++ )
								{
									if( acu_ret[h][j] > 0.00 )
									{
										NOMBRE_TICKET_COMPRA( j, xx );
										_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "%.2f", acu_ret[h][j]);
										TRIM(cadena_aux2);
										if(IMPRESORA_FISCAL == FISCAL)
										{
											_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%s", xx);
											//Agregamos espacios
											for(i = strlen(cadena_aux); i < 31; i++)
												cadena_aux[i]= ' ';
											//Alineación a la derecha
											for(i = 0; i <= strlen(cadena_aux2); i++)
												cadena_aux[30 - i]= cadena_aux2[strlen(cadena_aux2)- i];
											cadena_aux[31]=0;
											//strncat( cadena_aux, cadena_aux2, sizeof( cadena_aux ) - strlen( cadena_aux ) - 1 );
											IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux, NO );
										}
										else
										{
											MOSTRAR_CADENA( tkt_columna, PRN, xx );
											MOSTRAR_CADENA( tkt_columna, PRN, cadena_aux2 );
										}
										LF(1);
										memset(xx,0, sizeof(xx));
										memset(cadena_aux,0, sizeof(cadena_aux));
										memset(cadena_aux2,0, sizeof(cadena_aux2));
									}
								}
							}
							else 
							{
								_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "%.2f", acu_retirado[h]);
								if(IMPRESORA_FISCAL == FISCAL) 
								{
									_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%s ", medios[h].nombre);
									//Agregamos espacios
									for(i = strlen(cadena_aux); i < 31; i++)
										cadena_aux[i]= ' ';
									//Alineación a la derecha
									for(i = 0; i <= strlen(cadena_aux2); i++)
										cadena_aux[30 - i]= cadena_aux2[strlen(cadena_aux2)- i];
									cadena_aux[31]=0;
									//strncat( cadena_aux, cadena_aux2, sizeof( cadena_aux ) - strlen( cadena_aux ) - 1 );
									IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux, NO );
									LF(1);
								}
								else
								{
									MOSTRAR_CADENA( tkt_columna, PRN, medios[h].nombre);
									MOSTRAR_CADENA( (tkt_columna+21), PRN, cadena_aux2);
									LF(1);
								}
							}
							total += acu_retirado[h];
							if( config.pais == ELSALVADOR ) {
								long id_evento;
								id_evento = RAM_ID_EVENTO;
								area_ant = SELECTED(  );
								SELECT( AREA_PAGO );
								GO( TOP );
								SET_ORDER( 1 );	
								GET_EQUAL( ( char* ) &id_evento );
								while( !BtrvEOF( ) && seguir ) {
									if( event_pago.id_evento == id_evento && event_pago.modo_pago == h + 1
										&& event_pago.importe_me ) {
										MOSTRAR_CADENA( 3, PRN, "[" );
										MOSTRAR_CADENA( 5, PRN, medios[_medio_moneda_extranjera].simbolo_moneda );
										MOSTRAR_FLOTANTE( 9, PRN, ENTEROS, DECIMALES, 0, 0, event_pago.importe_me / ( ( RAM_CANTIDAD )? RAM_CANTIDAD :1 ) );
										MOSTRAR_CADENA( 20, PRN, "x" );
										MOSTRAR_ENTERO( 21, PRN, "999", RAM_CANTIDAD );
										MOSTRAR_FLOTANTE( 25, PRN, ENTEROS, DECIMALES, SI, 0, event_pago.importe_me );
										MOSTRAR_CADENA( 33, PRN, "]" );
										LF(1);
										seguir = 0;
									} 
									SKIP( 1 );
								}
								SELECT( area_ant );
							}
							SELECT_TABLE( tabla_ant, tipo_ant );
							memset(xx,0, sizeof(xx));
							memset(cadena_aux,0, sizeof(cadena_aux));
							memset(cadena_aux2,0, sizeof(cadena_aux2));
						}
					}
				}
				break;
			case _TKT_TOTAL_DE_RETIROS:
				memset(cadena_aux,0, sizeof(cadena_aux));
				memset(cadena_aux2, 0, sizeof(cadena_aux2));
				if(IMPRESORA_FISCAL == FISCAL)
				{
					_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%s", ST( S_TOTAL_RETIROS ) );
					_snprintf(cadena_aux2, sizeof(cadena_aux2)-1,"%.2f", total);

					//Agregamos espacios
					for(i = strlen(cadena_aux); i < 31; i++)
						cadena_aux[i]= ' ';
					//Alineación a la derecha
					for(i = 0; i <= strlen(cadena_aux2); i++)
						cadena_aux[30 - i]= cadena_aux2[strlen(cadena_aux2)- i];
					cadena_aux[31]=0;

					IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux, NO );
				}
				else{
					_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%.2f", total);
					MOSTRAR_CADENA( tkt_columna, PRN, ST( S_TOTAL_RETIROS ));
					MOSTRAR_CADENA( (tkt_columna+21), PRN, cadena_aux);
				}
				total = 0.00;
				if (copia ==2)
				{
					importe_retiro_ff = 0.0;
					copia = 0;
				}
				break;
			case _TKT_COD_AUTORIZACION:
				MOSTRAR_CADENA(tkt_columna, PRN, VAR_TARJETAS_AUTORIZACION );
				break;
			case _TICKET_FINAL:
				{
				long ticket_final = 0; 
				if(cierre_z)
					ticket_final = NRO_TICKET; /*Z_NRO_PRIMER_TICKET + (Z_CANTIDAD_TICKETS - 1);*/
				if(cierre_x)
					ticket_final = NRO_TICKET; /*X_NRO_PRIMER_TICKET + (X_CANTIDAD_TICKETS - 1);*/
				memset(cadena_aux,0, sizeof(cadena_aux));
				_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%08ld", ticket_final);
				MOSTRAR_CADENA(tkt_columna, PRN, cadena_aux );
				}
				break;
			case _TRANSACCION_FINAL:
				{
				int cant_tans;
				long tans_final;
				if(cierre_z)
				{
					cant_tans = Z_FACTURAS_A_CANTIDAD + Z_CONS_FINAL_CANTIDAD_FACT + Z_CONS_FINAL_CANTIDAD_TKT +
								Z_ANULACIONES_CANTIDAD + /*Z_DEV_ENVASES_CANTIDAD +*/ Z_DEVOLUCIONES_CANTIDAD +
								Z_PAGOS_POR_CAJA_CANTIDAD /*+ Z_FACTURAS_B_CANTIDAD + Z_NC_A_CANTIDAD*/ +
								Z_NC_B_CANTIDAD + Z_CANT_CANCELADOS  + Z_DONACIONES_CANTIDAD;
					tans_final = Z_NRO_TRANSACCION_INICIAL + cant_tans;
				}
				if(cierre_x)
				{
					cant_tans = X_FACTURAS_A_CANTIDAD + X_CONS_FINAL_CANTIDAD_FACT + X_CONS_FINAL_CANTIDAD_TKT +
								X_ANULACIONES_CANTIDAD + /*X_DEV_ENVASES_CANTIDAD +*/ X_DEVOLUCIONES_CANTIDAD +
								X_PAGOS_POR_CAJA_CANTIDAD /*+ X_FACTURAS_B_CANTIDAD + X_NC_A_CANTIDAD*/ +
								X_NC_B_CANTIDAD + X_CANT_CANCELADOS  + X_DONACIONES_CANTIDAD;
					tans_final = X_NRO_TRANSACCION_INICIAL + cant_tans;
				}
				memset(cadena_aux,0, sizeof(cadena_aux));
				_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%08ld", tans_final);
				MOSTRAR_CADENA(tkt_columna, PRN, cadena_aux );
				}
				break;
		   case _SECUENCIA_ANTERIOR:
				memset(cadena_aux,0, sizeof(cadena_aux));
				_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%04ld", _secuencia_ant);
	 		    MOSTRAR_CADENA(tkt_columna, PRN, cadena_aux );
				break;
		   case _SI_CIERRE_Z:
			   //SI_CIERRE_Z = 1;

			   while( *ind_config && CONFIG_TKT1[( *ind_config )].codigo != _FIN_CIERRE_Z )
			   {
				    if( cierre_z )
						TKT_ejecutar_comando( ind_config, 1, h, 0 );
					else
						( *ind_config )++;
               }
			   break;
		   case _SI_CIERRE_X:
			   //SI_CIERRE_X = 1;

			   while( *ind_config && CONFIG_TKT1[( *ind_config )].codigo != _FIN_CIERRE_X )
			   {
					if(cierre_x )
						TKT_ejecutar_comando( ind_config, 1, h, 0 );
					 else
						( *ind_config )++;
			   }
			   break;
		   case _NUMERO_DE_X:
			   memset(cadena_aux,0, sizeof(cadena_aux));
			   if( NRO_MODO == 'Y' )
					{
						_snprintf(cadena_aux, sizeof(cadena_aux)-1,"Cierre de Caja");
						MOSTRAR_CADENA(tkt_columna, PRN, cadena_aux );
						LF(1);
						memset(cadena_aux,0, sizeof(cadena_aux));
						_snprintf(cadena_aux, sizeof(cadena_aux)-1,"Y%08ld", NRO_Y);
						MOSTRAR_CADENA(tkt_columna, PRN, cadena_aux );
					}
			   if( NRO_MODO == 'X' )
				   {
						_snprintf(cadena_aux, sizeof(cadena_aux)-1,"X%08ld", NRO_X);
						MOSTRAR_CADENA(tkt_columna, PRN, cadena_aux );
				   }
			   break;

					//----------Voucher Tarjeta de Credito IMP Fiscal---------Seba
			case _TIPO_TARJETA://Seba
				memset( cardName, 0, sizeof( cardName ) );
				_snprintf( cardName, sizeof(cardName), tarjetas_fisc_rec[h2].nombre);
				cardName[sizeof(cardName)-1]='\0';
				break;
			case _CLIENTE://Seba
				memset( cardUsr, 0, sizeof( cardUsr ) );
				OBTIENE_NOMBRE_CLIENTE( cardUsr, h );
				cardUsr[sizeof(cardUsr)-1]='\0';
				break;
			case _VENCIMIENTO://Seba
				memset( expDate, 0, sizeof( expDate ) );
				if(TARJ[VAR_TARJETAS_TARJETA].usar_fecha_venc_defecto == 'S'|| TARJ[VAR_TARJETAS_TARJETA].usar_fecha_venc_defecto == 's' )
				{
					DTOA( TARJ[VAR_TARJETAS_TARJETA].fecha_vencimiento, expDate, NO );
				}
				else {
					if( TARJ[VAR_TARJETAS_TARJETA].pedir_fecha_vto == 'S'
						|| TARJ[VAR_TARJETAS_TARJETA].pedir_fecha_vto == 's' ) {
						OBTIENE_FECHA_VENCIMIENTO(expDate);
					}
				}
				expDate[sizeof(expDate)-1]='\0';
				break;
			case _NUMERO_ESTABLECIMIENTO://Seba
				estNum = (int)TARJ[VAR_TARJETAS_TARJETA].nro_comercio_pesos_cuotas;
				break;
			case _NUMERO_CUPON://Seba
				//cupNum = tarjetas_fisc_rec[h2].nro_cupon; //esta variable no se setea con VAR_TARJETAS_NRO_CUPON que siempre es 0
				cupNum = tarjetas_fisc_rec[h2].nro_cupon_int;
				break;
			case _COMPROBANTE_NUMERO://Seba
				intNum = tarjetas_fisc_rec[h2].nro_cupon_int; //deberia ser número interno del comprobante que se esta emitiendo  ?¿
				break;
			case _CODIGO_AUTORIZACION://Seba
				autCod = tarjetas_fisc_rec[h2].cod_autorizacion;
				break;
			case _OPERACION: {
				char *c_op[] = {"Compra","On-line Compra","Off-line Compra","Anulacion","On-line Anulacion",
                    "Off-line Anulacion","Devolucion","On-line Devolucion","Off-line Devolucion", "On-Line Extraccion","On-Line Anul Extracc"};
				int op;
//				char aux[20];
				int devolucion = 0, anulacion = 0;
				if( RAM_NOTA_CR || MODO_DEVOLUCION ) {
					devolucion = 1;
				}

				op = VAR_TARJETAS_ON_LINE;
				if( anulacion ) {
					op += 3;
				}
				else if( devolucion ) {
					op += 6;
				 }
				memset( opType, 0, sizeof( opType ) );
				strncpy( opType, c_op[op] , sizeof( opType ) );
				//sprintf( opType,"Online", strlen("Online") );
				
							 
				
				}
				
/*				if ( strcmp(tarjetas_fisc_rec[h2].cod_autorizacion,"") != 0 ) {
					sprintf( opType,"Online", strlen("Online") );
				} else {
					sprintf( opType,"Offline", strlen("Offline") );
				}*/
				break;
			case _IMPORTE_TARJETA://Seba
                memset( __total, 0, sizeof( __total ) );
				//_total = RAM_TOTAL; //aca solo deberia ser el importe de la tarjeta
				if( RAM_COD_FUNCION == 57 ) //reimpresion de voucher
					_total = tarjetas_fisc_rec[h2].importe ;
				else
				_total = _PAGO_IMPORTE(h);
				_snprintf( __total, sizeof(__total), _fcvt( _total, DECIMALES, &dec, &sig ) );
				break;
			case _NUMERO_CUOTAS://Seba
				_cuotas = tarjetas_fisc_rec[h2].cuotas;
				break;
			case _MONEDA://Seba
				memset( moneda, 0, sizeof( moneda ) );
				if(tarjetas_fisc_rec[h2].moneda == PESOS)
                    sprintf( moneda, "PESOS", strlen("PESOS") );
				if(tarjetas_fisc_rec[h2].moneda == DOLARES)
                    sprintf( moneda, "DOLARES", strlen("DOLARES") );
				moneda[sizeof(moneda)-1]='\0';
				break;
			case _NUMERO_TERMINAL://Seba
				termNum = tarjetas_fisc_rec[h2].nro_terminal;
				break;
			case _NUMERO_LOTE://Seba
				lotNum = tarjetas_fisc_rec[h2].nro_lote;
				break;
			case _NUMERO_TPV://Seba
				electTermNum = NRO_CAJA;
				break;
			case _NUMERO_SUCURSAL://Seba
				sucNum=config.sucursal;
				break;
			case _OPERADOR://Seba
				_itoa(NRO_CAJERO, Operator, 10);
				Operator[sizeof(Operator)-1]='\0';
				break;
			case _TIQUE_REFERENCIA://Seba
				docFiscal = NRO_TICKET;
				break;
			case _FIRMA://Seba
				sprintf( _firma, "SI", strlen("SI") );
				break;
			case _ACLARACION://Seba
				sprintf( _aclaracion, "SI", strlen("SI") );
				break;
			case _PHONE://Seba
				sprintf( _telefono, "SI", strlen("SI") );
				break;
			case _IMPRIME_VOUCHER_FISCAL://Seba
				memset( buffer, 0, sizeof(buffer) );
				if(!strcmp(expDate,"")){
					_snprintf( expDate, sizeof(expDate), "000000");
					expDate[sizeof(expDate)-1]='\0';
				}
				if(!strcmp(cardName,"")){
					_snprintf( cardName, sizeof(cardName), "NO");
					cardName[sizeof(cardName)-1]='\0';
				}
				if(strcmp(cardUsr,"")){//CABAL TIENE ERRORES CON LA Ñ, LA REEMPLAZA POR CARACTERES
					char *caracter =NULL;
					if(caracter = strchr( cardUsr, '&' )){
                    *caracter = 'Ñ';
					}
				}
				
				if(!strcmp(cardNum,"")){
					_snprintf( cardNum, sizeof(cardNum), "NO");
					cardNum[sizeof(cardNum)-1]='\0';
				}
				
                _snprintf( buffer, sizeof(buffer), "%s&%s&%s&%s&%d&%d&%d&%ld&%s&%s&%d&%s&%d&%d&%d&%d&%s&%d&%s&%s&%s&",cardName,
							cardNum, cardUsr, expDate, (int)estNum, (int)cupNum, (int)intNum, (long)autCod,
							opType, __total, (int)_cuotas, moneda,(int)termNum, (int)lotNum, (int)electTermNum,
							(int)sucNum, Operator, (int)docFiscal,_firma, _aclaracion, _telefono);
				buffer[sizeof(buffer)-1]='\0';
				IMPRIME_VOUCHER_FISCAL(buffer);
				ClearCfg();
				break;

			//-----------Documentos NO Fiscales
			case _ABRIR_DOC_NO_FISCAL:				//Seba
				ABRIR_DOCUMENTO_NO_FISCAL('a');
				break;
			case _CERRAR_DOC_NO_FISCAL:				//Seba
				CERRAR_DOCUMENTO_NO_FISCAL();
				break;
			case _IMPORTE_MEDIO_FF:					//Seba
			    //IMPRIMIR_IMPORTE_FF(p, tkt_columna);
				{
				int sta=1,medio=0, salir=0,i=0, pos=0;
				char cadena_aux[100], cadena_aux2[100];
				ind_configTmp = 0;
				pos = TKT_tomar_entero( ind_config, &ind_configTmp ) ;
				memset( cadena_aux, 0, sizeof(cadena_aux) );
				memset( cadena_aux2, 0, sizeof(cadena_aux2) );
				//Busco el nombre del medio en la posicion
				//SELECT( AREA_PAGO );
				SELECT_TABLE( T_PAGOS, tipo_t_pagos );
				//A esta altura RAM_ID_EVENTO ya fue actualizado
				SET_WHERE("ID_EVENTO = '%ld' AND POSICION ='%i'", RAM_ID_EVENTO - 1, pos );
				RUN_QUERY( NO );
				if( FOUND2( ) ){
					medio = event_pago.modo_pago;
				}
				/*while(  !dbEOF() && !salir)
				{
					if( event_pago.posicion == pos)
					{
						medio=event_pago.modo_pago;
						salir=1;
					}
					SKIP2( -1 );
				}*/

				if( medio!= 0 )
					{
						_snprintf( cadena_aux, sizeof(cadena_aux)-1,"%s : ",medios[medio-1].nombre);
					/*for(i=0; i<tkt_columna; i++)
						cadaux[i]= ' ';
					strcat(cadaux, cadaux2);*/
						_snprintf( cadena_aux2, sizeof(cadena_aux2),"%.2lf", _PAGO_IMPORTE( pos ));
						strncat( cadena_aux, cadena_aux2, sizeof( cadena_aux ) - strlen( cadena_aux ) - 1 );
						IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux, NO );//Seba
						total_ff += _PAGO_IMPORTE( pos );
					}
				}
				break;
			case _TOTAL_FF:
				memset(cadena_aux,0, sizeof(cadena_aux));
				memset(cadena_aux2, 0, sizeof(cadena_aux2));
				_snprintf(cadena_aux, sizeof(cadena_aux)-1,"%s", "TOT.. FONDO FIJO");
					/*for(i=0; i<tkt_columna; i++)
						cadaux[i]= ' ';
					strcat(cadaux, cadaux2);*/
				_snprintf(cadena_aux2, sizeof(cadena_aux2)-1,"%.2f", total_ff);
				strncat( cadena_aux, cadena_aux2, sizeof( cadena_aux ) - strlen( cadena_aux ) - 1 );
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux, NO );
				total_ff = 0.00;
				break;

		   case _SI_MEDIO_SALIENTE:
				SI_MEDIO_SALIENTE = 1;
				break;
			case _TKT_FRANQUE0_AL_FINAL:
				SI_FRANQUE0_AL_FINAL = 1;
				break;
		 case _SUPERVISOR:
			{
				struct _cajeros sup_cajero;
				int rta; 
				LEE_CAJERO( USUARIO_CLAVE_ANTERIOR(), &sup_cajero );
				rta = ( sup_cajero.tipo == SUPERVISOR ) ? SI : NO;
				if( rta ) 
				{
					if( IMPRIMIR_LEGAJO_CAJERO ) 
					{
						MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", sup_cajero.nro_legajo );	
					}
					else 
					{
						MOSTRAR_ENTERO( tkt_columna, PRN, "00000000", sup_cajero.cod_cajero );	
					}
				}
			}
            break;
			case _NOMBRE_SUPERVISOR:
			{
				struct _cajeros sup_cajero;
				int rta;
				LEE_CAJERO( USUARIO_CLAVE_ANTERIOR(), &sup_cajero );
				rta = ( sup_cajero.tipo == SUPERVISOR ) ? SI : NO;
				if( rta ) 
					if ( IMPRESORA_FISCAL == FISCAL ) 
						IMPRIMIR_TEXTO_NO_FISCAL(1, TIPO_DE_LETRA, sup_cajero.nombre, NO );
					else
						MOSTRAR_CADENA( tkt_columna, PRN, sup_cajero.nombre );
			}
			break;
			case _TKT_FOR_CONTADOR:
				ind_configTmp = 0;
				sprintf(cadaux2,"%i",_TKT_FOR_I );
				strncpy( cadaux3, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux3 ) - 1 );
					( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
				RESOLVER_FUNCIONES( cadaux3, cadaux2, CONFIG_TKT1[( *ind_config )].linea );
				break;
			case _TKT_CAMPO_ARCH_REC:
				{
				char ruta[100];
				int tamanio=0;
				ind_configTmp = 0;
					//1 ruta				
					strncpy( ruta, /*&*/CONFIG_TKT1[( *ind_config )].parametros, sizeof( ruta ) - 1 );
					( ind_configTmp ) += strlen( /*&*/CONFIG_TKT1[( *ind_config )].parametros ) + 1;
					
					// 2 tamaño de la estructura
					tamanio = TKT_tomar_entero( ind_config, &ind_configTmp );
				
					//3 tipo rta
					strncpy( xx, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
						sizeof( xx ) - 1 );
					( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )
						+ 1;	

					//4 offset
					strncpy( cadaux2, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux2 ) - 1 );
					( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
					
					//RESOLVER_FUNCIONES( cadaux2, cadaux2, CONFIG_TKT1[( *ind_config )].linea );

					//5 funciones
					strncpy( cadaux3, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],
                     sizeof( cadaux3 ) - 1 );
					( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
				
					OBTENER_DATO_ARCHIVO_REC( ruta, tamanio, xx, cadaux2);

					RESOLVER_FUNCIONES( cadaux3, cadaux2, CONFIG_TKT1[( *ind_config )].linea );
				}
				break;
				case _TKT_VALIDACION:
				{
					int validacion = 0;
					ind_configTmp = 0;
					validacion = TKT_tomar_entero( ind_config, &ind_configTmp );
					VALIDAR( validacion, 0, 0, 0, 0 );
				}
				break;
				case _TKT_PRODUCTIVIDAD_CAJERO:
				{
					int promedio = 0,min = 0,seg = 0;
					unsigned segundos = 0,ventas = 0;
					//char cad_aux[50];
					segundos = _GET_MEMORY_UNSIGNED( informes.prod_cod_seg, _TKT_FOR_I );
					if( segundos ) {
						ventas = _GET_MEMORY_UNSIGNED( informes.prod_cod_vta, _TKT_FOR_I );
						promedio = segundos / ventas;
						min = promedio / 60;
						seg = promedio - min * 60;
						INFORME_05_CUERPO( tkt_columna, _TKT_FOR_I, min, seg ); 
					}
				}
				break;
				case _TKT_DIA_OPERATIVO:
					{
					int anterior = 0;
					ind_configTmp = 0;
					anterior = ( INT16 )TKT_tomar_entero( ind_config, &ind_configTmp );
					IMPRIME_DIA_OPERATIVO( anterior );
					}
				break;
				case _LINEA_DE_DESCRIPCION	:
				{
					int linea_nro = 0, operacion = 0, longitud=0;
					int ind_configTmp = 0;
					double resultado=0.0;
					char *ptr;
					memset( cadaux, 0, sizeof( cadaux ) );
					
					// linea de descripcion, 
					linea_nro = TKT_tomar_entero( ind_config, &ind_configTmp );
					linea_nro--;
					
					// 0 borra e inicializa, 1 = append,....
					operacion = TKT_tomar_entero( ind_config, &ind_configTmp );
					/*
					//verifico si tiene una variable del Rnv
					ARMAR_MENSAJE( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp], cadaux);
					//strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],sizeof( cadaux ) - 1 );
					( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )+ 1;	
					*/

					strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp], sizeof( cadaux ) - 1 );
					( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] ) + 1;
					
					ptr = strstr(cadaux, "({" );

					if( ptr ){
						//detecto variable de RNV Ej.[({#294})];
						memset( cadaux2, 0, sizeof( cadaux2 ) );
						strcpy(cadaux2,cadaux);
						//ESTE USA  "({" 
						ARMAR_MENSAJE( cadaux2, cadaux);
					}else{
						ptr = strstr(cadaux, "var(" );
						if( ptr ){
							//detecto variable de RNV Ej.[var(4)];
							resultado = RESOLVER( cadaux, &val_calc );
							memset( cadaux, 0, sizeof( cadaux ) );
							//sprintf(cadaux,"%*.*lf",ENTEROS, DECIMALES,resultado);
							_snprintf( cadaux, sizeof( cadaux) - 1,"%*.*lf",ENTEROS, DECIMALES,resultado);
						}else{
							//cadenas directas..
						}
					}

					
					if( linea_nro >= 0 && linea_nro < 4  ){
						
						//if( strlen(cadaux) > 0 )CARACTERES_ESPECIALES(cadaux, _IMPRIMIR_CADENA );

						longitud=strlen(linea_descripcion[linea_nro]);

						switch(operacion){
						case 0:{
							memset( &linea_descripcion[linea_nro], 0, longitud);
							if( strlen(cadaux) > 0 )
							_snprintf( &linea_descripcion[linea_nro], sizeof( linea_descripcion[linea_nro] ) - 1,"%s", cadaux);
							break;
							   }
						case 1:{
							_snprintf( &linea_descripcion[linea_nro][longitud],( sizeof( linea_descripcion[linea_nro] ) - longitud ) - 1,"%s", cadaux);
							break;
							  }
						}
					}
				}
				break;
				case _IMPRIME_LINEA_DE_DESCRIPCION:
					{
 					int linea_nro = 0;
					int ind_configTmp = 0;
						memset( cadaux, 0, sizeof( cadaux ) );
						
						linea_nro = TKT_tomar_entero( ind_config, &ind_configTmp );
						linea_nro--;
						
						if(linea_nro >= 0 && strlen (linea_descripcion[linea_nro]) > 0 ){ 
						 IMPRIME_MENSAJE_FISCAL( linea_descripcion[linea_nro]);
						 memset( &linea_descripcion[linea_nro], 0, strlen(linea_descripcion[linea_nro]));
						 //sino lo borro queda cargado la rta de la impresora
						}
					}
					break;
				case _PREFERENCIA_IMPRESION:{
					int ind_configTmp = 0, prefer_sobre = 0;
					double resultado=0.0;
					memset( cadaux, 0, sizeof( cadaux ) );

					prefer_sobre = TKT_tomar_entero( ind_config, &ind_configTmp );

					strncpy( cadaux, &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp],sizeof( cadaux ) - 1 );
					( ind_configTmp ) += strlen( &CONFIG_TKT1[( *ind_config )].parametros[ind_configTmp] )+ 1;	

					if( IMPRESORA_FISCAL == FISCAL ) {
					
						switch(prefer_sobre){
					case 1:{//ACTION_SET_PRINTER_PREF preferencias de la impresora
						   PREFERENCIAS_IMPRESORA(&cadaux[0]);
						   }break;
					case 2:{//ACTION_SET_PAPER_PREF preferencias de papel
						   PREFERENCIAS_PAPEL(&cadaux[0]);
						   }break;	
					case 3:{//ACTION_SET_COMPR_PREF preferencias de comprobante
							PREFERENCIAS_COMPROBANTE(&cadaux[0]);
						   }break;
						}
					}
					break;
				}
    }
}

/***************************************************************************/
void ClearCfg()//Seba
/***************************************************************************/
{
//Limpia todas la variables qu forman el buffer para la generacion del vouche para impresoras fiscales
memset( cardName, 0, sizeof( cardName ) );
memset( cardNum, 0, sizeof( cardNum ) );
memset( cardUsr, 0, sizeof( cardUsr ) );
memset( expDate, 0, sizeof( expDate ) );
memset( opType, 0, sizeof( opType ) );
memset( moneda, 0, sizeof( moneda ) );
memset( Operator, 0, sizeof( Operator ) );
memset( _firma, 0, sizeof( _firma ) );
memset( _aclaracion, 0, sizeof( _aclaracion ) );
memset( _telefono, 0, sizeof( _telefono ) );

_snprintf( cardName, sizeof(cardName), "NO");
cardName[sizeof(cardName)-1]='\0';
_snprintf( cardNum, sizeof(cardNum), "NO");
cardNum[sizeof(cardNum)-1]='\0';
_snprintf( cardUsr, sizeof(cardUsr), "NO");
cardUsr[sizeof(cardUsr)-1]='\0';
_snprintf( expDate, sizeof(expDate), "000000");
expDate[sizeof(expDate)-1]='\0';
estNum=0;
cupNum=0;
intNum=0;
autCod=0;
_snprintf(opType, sizeof(opType), "NO");
opType[sizeof(opType)-1]='\0';
_total=0;
_cuotas=0;
_snprintf( moneda, sizeof(moneda), "NO");
moneda[sizeof(moneda)-1]='\0';
termNum=0;
lotNum=0;
electTermNum=0;
sucNum=0;
_snprintf(Operator, sizeof(Operator), "NO");
Operator[sizeof(Operator)-1]='\0';
docFiscal=0;
_snprintf( _firma, sizeof(_firma), "NO");
_firma[sizeof(_firma)-1]='\0';
_snprintf( _aclaracion, sizeof(_aclaracion), "NO");
_aclaracion[sizeof(_aclaracion)-1]='\0';
_snprintf( _telefono, sizeof(_telefono), "NO");
_telefono[sizeof(_telefono)-1]='\0';
}

/***************************************************************************/
void RESOLVER_TKT_CONSULTA_SQL(char *Consulta, char *Funciones, char *TipoRes, int linea)
/***************************************************************************/
{
    char *token;
    double resultado;
    char StrQuery[350];
    char rta[100];
    double rta2 = 0;
    struct _valores_calculo val_calc;
	char sep[]="$&,";
	char bytetemporal;
	memset(&val_calc,0,sizeof(struct _valores_calculo));
    TRIM(TipoRes);
    token = strtok( Consulta, sep );
    strcpy(StrQuery, token);
    token = strtok( NULL, sep );

    while(token != NULL){
        val_calc.tkt_ini_est = _TKT_INI_EST;
        resultado = RESOLVER( token, &val_calc );
        sprintf(StrQuery,"%s%lf",StrQuery,resultado);
        token = strtok( NULL, sep );
        if(token != NULL){
            sprintf(StrQuery,"%s%s",StrQuery,token);
        }
        token = strtok( NULL, sep );
    }
	memset(rta, 0, sizeof(rta));
    if(DIRECT_DB_QUERY(StrQuery)> 0){
        if(strcmp(TipoRes,"numerico") == 0){
            GET_DATO(1,C_DOUBLE,(char *)&rta2,8);
            _snprintf( rta, sizeof( rta )-1,"%lf",rta2);
        }else if(strcmp(TipoRes,"cadena") == 0){
            GET_DATO(1,C_CHAR,rta,sizeof(rta));
        }else if(strcmp(TipoRes,"byte") == 0){
            GET_DATO(1,C_CHAR,rta,sizeof(rta));
			bytetemporal  = *( INT16 * )( rta );
			_snprintf( rta, sizeof( rta )-1, "%d", ( int )bytetemporal );
        }else{
            char strTmp[100];
            sprintf(strTmp,"ERROR CONSULTA SQL: no existe el tipo de respuesta linea tkt: %d",linea);
            GRABAR_LOG_SISTEMA( strTmp ,LOG_ODBC,1);
            MENSAJE("ERROR CONSULTA SQL TKT");
            return;
        }
    }
    RESOLVER_FUNCIONES( Funciones, rta, linea );
}
/***************************************************************************/
void INIT_COMPROBANTE_TKT()
/***************************************************************************/
{
    ind_config = 0;
    while( CONFIG_TKT1[ind_config].codigo !=_TKT_FIN_INICIALIZACION
        && CONFIG_TKT1[ind_config].codigo != _FIN ) {
        TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
    }
    ind_comienzo_ticket = ind_config + 1;
    ind_comienzo_cuerpo = _COMPROBANTE_TKT( _COMIENZO_CUERPO_UNITARIO, _FIN_CUERPO_UNITARIO );
    ind_comienzo_cuerpo_cantidad = _COMPROBANTE_TKT( _COMIENZO_CUERPO_CANTIDAD,
                                                     _FIN_CUERPO_CANTIDAD );
    ind_comienzo_total = _COMPROBANTE_TKT( _COMIENZO_CUERPO_TOTAL, _FIN_CUERPO_TOTAL );
    ind_comienzo_medios = _COMPROBANTE_TKT( _COMIENZO_CUERPO_MEDIOS, _FIN_CUERPO_MEDIOS );
    ind_comienzo_pie = _COMPROBANTE_TKT( _COMIENZO_PIE, _FIN_PIE );
	ind_comienzo_ticket_espera = _COMPROBANTE_TKT( _TKT_COMIENZO_TICKET_ESPERA, _TKT_FIN_TICKET_ESPERA );
    ind_comienzo_iva = _COMPROBANTE_TKT( _COMIENZO_CUERPO_IVA, _FIN_CUERPO_IVA );
    ind_comienzo_vuelto = _COMPROBANTE_TKT( _COMIENZO_CUERPO_VUELTO, _FIN_CUERPO_VUELTO );
    ind_comienzo_afinidad = _COMPROBANTE_TKT( _COMIENZO_CUERPO_AFINIDAD, _FIN_CUERPO_AFINIDAD );
    ind_comienzo_no_afinidad = _COMPROBANTE_TKT( _COMIENZO_CUERPO_NO_AFINIDAD,
                                                 _FIN_CUERPO_NO_AFINIDAD );
    ind_comienzo_donacion = _COMPROBANTE_TKT( _COMIENZO_CUERPO_DONACION, _FIN_CUERPO_DONACION );
    ind_comienzo_id_con_cliente = _COMPROBANTE_TKT( _COMIENZO_CUERPO_ID_CON_CLIENTE,
                                                    _FIN_CUERPO_ID_CON_CLIENTE );
    ind_comienzo_id_sin_cliente = _COMPROBANTE_TKT( _COMIENZO_CUERPO_ID_SIN_CLIENTE,
                                                    _FIN_CUERPO_ID_SIN_CLIENTE );
    ind_informe_diferencia = _COMPROBANTE_TKT( _TKT_INICIO_INF_DIFERENCIA,
                                               _TKT_FIN_INF_DIFERENCIA );
	ind_informe_diferencia_submedios = _COMPROBANTE_TKT( _TKT_INICIO_INF_DIFERENCIA_SUBMEDIOS,
                                               _TKT_FIN_INF_DIFERENCIA_SUBMEDIOS );
    ind_comienzo_f_fijo = _COMPROBANTE_TKT( _TKT_COMIENZO_CUERPO_F_FIJO, _TKT_FIN_CUERPO_F_FIJO );
    #if !defined(INVEL_L) && !defined(INVEL_W)
    ind_comienzo_comando_x = _COMPROBANTE_TKT( _TKT_INICIO_COMANDO_X, _TKT_FIN_COMANDO_X );
    ind_comienzo_comando_z = _COMPROBANTE_TKT( _TKT_INICIO_COMANDO_Z, _TKT_FIN_COMANDO_Z );
    ind_comienzo_comando_y = _COMPROBANTE_TKT( _TKT_INICIO_COMANDO_Y, _TKT_FIN_COMANDO_Y );
    ind_comienzo_comando_b = _COMPROBANTE_TKT( _TKT_INICIO_COMANDO_B, _TKT_FIN_COMANDO_B );
    ind_comienzo_impresora_fiscal = _COMPROBANTE_TKT( _TKT_INICIO_IMPRESORA_FISCAL,
                                                      _TKT_FIN_IMPRESORA_FISCAL );
    #endif
    ind_comienzo_corte_papel = _COMPROBANTE_TKT( _TKT_INICIO_CORTE_PAPEL, _TKT_FIN_CORTE_PAPEL );
    ind_comienzo_subtotal = _COMPROBANTE_TKT( _TKT_INICIO_CUERPO_SUBTOTAL,
                                              _TKT_FIN_CUERPO_SUBTOTAL );
    ind_comienzo_inf_00 = _COMPROBANTE_TKT( _COMIENZO_INFORME_00, _FIN_INFORME_00 );
	ind_comienzo_inf_05 = _COMPROBANTE_TKT( _COMIENZO_INFORME_05, _FIN_INFORME_05 );
	ind_comienzo_cabecera_inf_10 = _COMPROBANTE_TKT( _COMIENZO_CABECERA_INFORME_10, _FIN_CABECERA_INFORME_10 );
ind_comienzo_inf_15= _COMPROBANTE_TKT( _COMIENZO_INFORME_15, _FIN_INFORME_15 );
ind_comienzo_cabecera_inf_15 = _COMPROBANTE_TKT( _COMIENZO_CABECERA_INFORME_15, _FIN_CABECERA_INFORME_15 );
	ind_comienzo_inf_10= _COMPROBANTE_TKT( _COMIENZO_INFORME_10, _FIN_INFORME_10 );
	ind_comienzo_tarjetas = _COMPROBANTE_TKT( _COMIENZO_CUERPO_TARJETAS, _FIN_CUERPO_TARJETAS );
    ind_comienzo_timbradora = _COMPROBANTE_TKT( _COMIENZO_TIMBRADORA, _FIN_TIMBRADORA );
    ind_comienzo_inf_sup = _COMPROBANTE_TKT( _INICIO_INF_SUP, _FIN_INF_SUP );
    ind_comienzo_cab_inf_sup = _COMPROBANTE_TKT( _INICIO_CAB_INF_SUP, _FIN_CAB_INF_SUP );
	ind_comienzo_cab_inf_dif_sub = _COMPROBANTE_TKT( _INICIO_CAB_INF_DIF_SUB, _FIN_CAB_INF_DIF_SUB );
    ind_comienzo_pago_reintegro = _COMPROBANTE_TKT( _COMIENZO_PAGO_REINTEGRO, _FIN_PAGO_REINTEGRO );
    ind_comienzo_timbrado_reintegro = _COMPROBANTE_TKT( _COMIENZO_TIMBRADO_REINTEGRO, _FIN_TIMBRADO_REINTEGRO );
	ind_comienzo_descripcion_cuerpo = _COMPROBANTE_TKT( _COMIENZO_DESCRIPCION_CUERPO, _FIN_DESCRIPCION_CUERPO );
	ind_comienzo_descripcion_tarjeta = _COMPROBANTE_TKT( _COMIENZO_DESCRIPCION_TARJETA, _FIN_DESCRIPCION_TARJETA );
	ind_comienzo_importe_total = _COMPROBANTE_TKT( _COMIENZO_IMPORTE_TOTAL, _FIN_IMPORTE_TOTAL );
	ind_comienzo_franqueo = _COMPROBANTE_TKT( _TKT_COMIENZO_FRANQUEO, _TKT_FIN_FRANQUEO);
	ind_comienzo_cuerpo_informacion_comp = _COMPROBANTE_TKT( _TKT_COMIENZO_INFCOMPARATIVA, _TKT_FIN_INFCOMPARATIVA );
	ind_comienzo_fin_ticket = _COMPROBANTE_TKT( _COMIENZO_FIN_TICKET, _FIN_TICKET );
	ind_comienzo_franqueo_cobros = _COMPROBANTE_TKT( _TKT_COMIENZO_FRANQUEO_COBROS, _TKT_FIN_FRANQUEO_COBROS );
	ind_comienzo_duplicado_tarjeta = _COMPROBANTE_TKT( _COMIENZO_DUPLICADO_TARJETA, _FIN_DUPLICADO_TARJETA ); //Seba
	ind_comienzo_retiro = _COMPROBANTE_TKT(_COMIENZO_CUERPO_RETIRO, _FIN_CUERPO_RETIRO);//Seba
	ind_comienzo_retiro_cierre = _COMPROBANTE_TKT(_COMIENZO_CUERPO_RETIRO_CIERRE, _FIN_CUERPO_RETIRO_CIERRE);//Seba
	ind_comienzo_inf_y  = _COMPROBANTE_TKT( _TKT_COMIENZO_INFO_Y, _TKT_FIN_INFO_Y );
	ind_comienzo_ing_cajero = _COMPROBANTE_TKT(_COMIENZO_CUERPO_ING_CAJERO, _FIN_CUERPO_ING_CAJERO);//Seba


    if( IMPRESORA_FISCAL == FISCAL ) {
        if( !ind_comienzo_comando_b /*|| !ind_comienzo_comando_t */|| !ind_comienzo_comando_z ) {
            {
                char aux[100];
                sprintf( aux, "B: %i T %i Z: %i", ind_comienzo_comando_b,
                         /*ind_comienzo_comando_t,*/ ind_comienzo_comando_z );
                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,4);
            }
            ABORT_STRING( S_ABORTAR_CARGA_TKTCFG );
        }
    }
}

//***************************************************************************/
//int _COMPROBANTE_TKT( int comienzo, int final )
///***************************************************************************/
//{
//    int indice = 0;
//    while( CONFIG_TKT1[indice].codigo != comienzo && CONFIG_TKT1[indice].codigo != final
//        && CONFIG_TKT1[indice].codigo != _FIN ) {
//        //if( CONFIG_TKT1[indice].codigo > _FIN_CAMPOS ) {
//        //indice += TKT_desplazar_config( CONFIG_TKT[indice], &indice );
//        //} else {
//        indice++;
//        //}
//    }
//    if( CONFIG_TKT1[indice].codigo == _FIN || CONFIG_TKT1[indice].codigo == final ) {
//        indice = 0;
//    } //else {
//    //indice++;
//    //}
//    return( indice );
//}
/***************************************************************************/
int _COMPROBANTE_TKT( int comienzo, int final )
/***************************************************************************/
{
	//int indice = 0;
	char mensaje[100];
	int indice = 0, encuentra_inicio = NO, encuentra_fin = NO;
	memset(mensaje,0,sizeof(mensaje));

	//modificación para controlar que tiene inicio y fin, sino sigue hasta el final
	
	while( ( encuentra_inicio == NO || encuentra_fin == NO )

        && CONFIG_TKT1[indice].codigo != _FIN ) {
			
			//dejo indicado el inicio
			if(CONFIG_TKT1[indice].codigo == comienzo ){
				encuentra_inicio = indice;
			}
			//controlo que termina el bloque
			if(CONFIG_TKT1[indice].codigo == final)
				encuentra_fin = indice;
			
	        
        indice++;
        
    }
	//	aca incremento 1 asi que puede estar en el final
	//verifica si esta o no 
	if( encuentra_inicio && encuentra_fin ){
		indice = encuentra_inicio;
	}else{
		if( encuentra_inicio || encuentra_fin ){
			_snprintf(mensaje,sizeof(mensaje)-1,"TKT:FALTA COMPLEMENTO (%s)",DEVOLVER_TKT_FUNCION( CONFIG_TKT1[((encuentra_inicio)? encuentra_inicio:encuentra_fin)].codigo - 1 - _FIN_CAMPOS));
			GRABAR_LOG_SISTEMA( mensaje ,LOG_ERRORES,2);
		}else{
			_snprintf(mensaje,sizeof(mensaje)-1,"TKT- Sin declarar[%i - %i]", comienzo,  final);
			GRABAR_LOG_SISTEMA( mensaje ,LOG_COMPROBANTES,5);
		}
        indice = 0;
	}
	
    return( indice );
}
/***************************************************************************/
INT16 TKT_tomar_entero( int *ind_config, int *indice )
/***************************************************************************/
{
    INT16 valor;
    valor = *( INT16* )&CONFIG_TKT1[*ind_config].parametros[*indice];
    ( *indice ) += 2;
    return ( valor );
}
/***************************************************************************/
void TKT_SET_FILA( int _fila )
/***************************************************************************/
{
    if( _fila > tkt_fila ) {
        LF( _fila - tkt_fila );
        tkt_fila = _fila;
        LINEA_IMPRESA ++;
    }
    tkt_fila = _fila;
}
/****************************************************************************/
void IMPRIMIR_TKT_MEDIOS( int h, int h2 )
/****************************************************************************/
{
	static int first_mc = 1;
	tkt_fila = 2;
	if(first_mc){
		first_mc = 0;
	}else{
		//LF(1);
	}
    tkt_columna = 1;
	if( ind_comienzo_medios ) {
        ind_config = ind_comienzo_medios;
 		while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_MEDIOS ) {
			TKT_ejecutar_comando( &ind_config, 0, h, h2 );
        }
    }
	med_tarj = 0;
}
/****************************************************************************/
void IMPRIMIR_TKT_VUELTO( int h )
/****************************************************************************/
{
    if( ind_comienzo_vuelto && RAM_VUELTO > 0.005 ) {
        ind_config = ind_comienzo_vuelto;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_VUELTO ) {
            TKT_ejecutar_comando( &ind_config, 0, h, 0 );
        }
        LF( 1 );
    }
}
/****************************************************************************/
int IMPRIMIR_TKT_INFORME_SUPERVISOR()
/****************************************************************************/
{
    int i;
    /*primero calcula la cantidad total de hojas*/
    for( i = 1 ;i >= 0 ;i-- ) {
        calcHojas = i;                   //cuando i es 1 indica que tiene que calcular las hojas
        IMPRIMIR_TKT_CABECERA_SUP();
        if( ind_comienzo_inf_sup ) {
            ind_config = ind_comienzo_inf_sup;
            while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_INF_SUP ) {
                TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
            }
            //LF( 1 );
        }
        PRN_FIN_TICKET();
        cantTotalCopias = cantCopias;
        cantCopias = 0;
    }
    return( ind_comienzo_inf_sup );
}
/****************************************************************************/
void IMPRIMIR_TKT_IVAS()
/****************************************************************************/
{
    int h,cod_iva,comienzo_rep,signo,acumular;
    double importe_tasa[MAX_COD_IVA];
    double *base_total = NULL, diferencia;
    base_total = calloc( _LARGO_TICKET, sizeof( double ) );
    if( base_total && ind_comienzo_iva ) {
        ind_config = ind_comienzo_iva;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_PARTE_FIJA ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
        comienzo_rep = ind_config;
        if( config.pais != ECUADOR ) {
            ACUMULAR_BASE_DTO_EVENTO( base_total );
        }
        for( h = 0;h < MAX_COD_IVA;h++ ) {
            importe_tasa[h] = 0;
        }
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            cod_iva = _TICKET_COD_IVA( h );
            //if( _TICKET_GRAVADO( h ) && _TICKET_ORIGEN( h ) != _ORIGEN_DESC_PORC && _TICKET_ORIGEN( h ) != _ORIGEN_PROMOCION && _TICKET_ORIGEN( h ) != _ORIGEN_DESC_PESOS ){
            acumular = ( config.pais == ECUADOR ) ? SI
                     : ( _TICKET_GRAVADO( h ) && _TICKET_ORIGEN( h ) != _ORIGEN_DESC_PORC
                      && _TICKET_ORIGEN( h ) != _ORIGEN_PROMOCION
                      && _TICKET_ORIGEN( h ) != _ORIGEN_DESC_PESOS );
            if( acumular ) {
                if( ( _TICKET_TIPO2( h ) & _CORRECCION ) || ( _TICKET_TIPO( h ) & _RESTA ) ) {
                    signo = -1;
                }
                else {
                    signo = 1;
                }
                if( config.pais != ECUADOR ) {
                    if( iva[cod_iva].porc_iva1 ) {
                        diferencia = ( base_total[h] / ( 1 + iva[cod_iva].porc_iva1 / 100.0 ) );
                    }
                    else {
                        diferencia = base_total[h];
                    }
                }
                else {
                    diferencia = 0;
                }
                importe_tasa[cod_iva] += signo * ( ( ( iva[cod_iva].porc_iva1 )
                                                   ? _TICKET_IMPORTE_SIN_IVA( h )
                                                   - _TICKET_IMP_INT( h ) : _TICKET_PRECIO( h ) )
                                                 * _TICKET_CANTIDAD( h ) - diferencia );
            }
        }
        for( h = 0;h < MAX_COD_IVA;h++ ) {
            if( importe_tasa[h] ) {
                ind_config = comienzo_rep;
                while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_IVA ) {
                    TKT_ejecutar_comando( &ind_config, importe_tasa[h], h, 0 );
                }
                LF( 1 );
            }
        }
        LF( 1 );
    }
    if( base_total ) {
        FREEMEM( ( char* )base_total );
    }
}
/****************************************************************************/
void IMPRIMIR_TKT_AFINIDAD()
/****************************************************************************/
{
    if( ind_comienzo_afinidad && RAM_CLIENTE_AFINIDAD ) {
        //Validacion para clientes afinidad
        ind_config = ind_comienzo_afinidad;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_AFINIDAD ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
        LF( 1 );
    }
}
/****************************************************************************/
void IMPRIMIR_TKT_NO_AFINIDAD()
/****************************************************************************/
{
    if( ind_comienzo_no_afinidad && !RAM_CLIENTE_AFINIDAD ) {
        //Validacion para clientes no afinidad
        ind_config = ind_comienzo_no_afinidad;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_NO_AFINIDAD ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
        LF( 1 );
    }
}
/***************************************************************************/
int TKT_evaluar_condiciones( int codigo, /*int *ind_config,*/ int h )
/***************************************************************************/
{
    int rta;
    struct _valores_calculo val_calc;
	memset(&val_calc,0,sizeof(struct _valores_calculo));

    rta = codigo;
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    if( !_SKIP_BLOCK && SI_MODO_ENTRENAMIENTO ) {
        SI_MODO_ENTRENAMIENTO = 0;
        if( RAM_MODO_EJECUCION != ENTRENAMIENTO ) {
            rta = 0;
        }
    }
    #endif
    if( !_SKIP_BLOCK && SI_MODO_INVENTARIO ) {
        SI_MODO_INVENTARIO = 0;
        if( RAM_MODO_EJECUCION != INVENTARIO ) {
            rta = 0;
        }
    }
    if( !_SKIP_BLOCK && SI_REIMPRESION ) {
        SI_REIMPRESION = 0;
        if( RAM_EN_IMPORTACION_DE_TICKET != 2
            || RAM_EN_IMPORTACION_DE_TICKET != 3 ) {
            rta = 0;
        }
    }
    if( !_SKIP_BLOCK && SI_ANULACION ) {
        SI_ANULACION = 0;
        if( !RAM_ANULACION ) {
            rta = 0;
        }
    }
    if( !_SKIP_BLOCK && SI_DEVOLUCION ) {
        SI_DEVOLUCION = 0;
        if( !MODO_DEVOLUCION ) {
            rta = 0;
        }
    }
	if( !_SKIP_BLOCK && NO_DEVOLUCION ) {
        NO_DEVOLUCION = 0;
		if( MODO_DEVOLUCION ) {
            rta = 0;
        }
    }
    if( !_SKIP_BLOCK && SI_CODIGO_MEDIO ) {
        if( SI_CODIGO_MEDIO != _PAGO_MODO( h ) + 1 ) {
            rta = 0;
        }
        SI_CODIGO_MEDIO = 0;
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
    if( !_SKIP_BLOCK && SI_DATO_ARTICULO ) {
        if( SI_DATO_ARTICULO != _TICKET_COD_SOLICITUD( h ) ) {
            rta = 0;
        }
        SI_DATO_ARTICULO = 0;
    }
    if( !_SKIP_BLOCK && SI_TOTAL_PROMOCION ) {
        if( fabs( total_promo ) <= 0.0 ) {
            rta = 0;
        }
        SI_TOTAL_PROMOCION = 0;
    }

	if( !_SKIP_BLOCK && SI_MEDIO_SALIENTE ) {
        if( !medio_saliente ) {
            rta = 0;
        }
        SI_MEDIO_SALIENTE = 0;
    }
	/*if( !_SKIP_BLOCK && SI_CIERRE_Z ) {
        if( !cierre_z ) {
            rta = 0;
        }
        SI_CIERRE_Z = 0;
		_SKIP_BLOCK = 1;
    }
	if( !_SKIP_BLOCK && SI_CIERRE_X ) {
        if( !cierre_x ) {
            rta = 0;
        }
        SI_CIERRE_X = 0;
		_SKIP_BLOCK = 1;
    }*/
    if( !rta && _TKT_BEGIN == codigo ) {
        _SKIP_BLOCK = 1;
    }
    if( _TKT_END == codigo ) {
        _SKIP_BLOCK = 0;
    }
    if( SI_T_EXENTO ) {
        SI_T_EXENTO = 0;
        if( !HAY_T_EXENTO ) {
            rta = 0;
        }
    }
    if( SI_T_GRAVADO ) {
        SI_T_GRAVADO = 0;
        if( !HAY_T_GRAVADO ) {
            rta = 0;
        }
    }
    if( _SI_P_FORMULA ) {
        //Dato para calcular variables rnv off
        val_calc.tkt_ini_est = _TKT_INI_EST;

        if( fabs( RESOLVER( _SI_P_FORMULA, &val_calc ) ) < 0.005 ) {
            rta = 0;
        }
        _SI_P_FORMULA = NULL;
    }
    /*-------- Si no se realiza la accion incrementamos el indice -------*/
    if( !rta || _SKIP_BLOCK ) {
        //TKT_incrementar_puntero( codigo, ind_config );
        //(*ind_config) ++;
    }
    return ( rta );
}
/****************************************************************************/
void IMPRIMIR_TKT_IDENTIFICACION()
/****************************************************************************/
{
    if( IDENTIFICACION_CLIENTES_HABILITA && RAM_TOTAL > config.piso_identificacion_clientes ) {
        ind_config = ( RAM_NRO_CLIENTE ) ? ind_comienzo_id_con_cliente
                   : ind_comienzo_id_sin_cliente;
        if( ind_config ) {
            while( CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_ID_CON_CLIENTE
                && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_ID_SIN_CLIENTE ) {
                TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
            }
            LF( 1 );
        }
    }
}
/****************************************************************************/
void IMPRIMIR_TKT_DONACION()
/****************************************************************************/
{
    if( ind_comienzo_donacion && RAM_TOTAL_DONACION > 0.005 ) {
        /*if( IMPRESORA_FISCAL == FISCAL ) {
			ABRIR_DOCUMENTO_NO_FISCAL('1');
			IMPRIME_CABECERA_GENERAL( NRO_EVENTO + 1 );
		}*/
        ind_config = ind_comienzo_donacion;
        while( CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_DONACION ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
		SET_MEMORY_INT( __ram_impresion_comprobante, _DONACION_IMPRESA );
        /*LF( 1 );
		if( IMPRESORA_FISCAL == FISCAL ) {
			IMPRIME_PIE_COMPROBANTE( COBRO, NO );
		}*/
    }
}
/***************************************************************************/
void IMPRIMIR_TKT_TITULO_TICKET()
/***************************************************************************/
{
    /*   ind_comienzo_titulo_ticket = _COMPROBANTE_TKT( _COMIENZO_CUERPO_TITULO_TICKET,
             _FIN_CUERPO_TITULO_TICKET );
       if( ind_comienzo_titulo_ticket ) {
          ind_config = ind_comienzo_titulo_ticket;
          while( CONFIG_TKT[ind_config] != _FIN_CUERPO_TITULO_TICKET ) {
             TKT_ejecutar_comando( &ind_config, 0, 0 );
            }
        }
    */
}
/****************************************************************************/
TKT_FISCAL_X()
/****************************************************************************/
{
    #if !defined(INVEL_L) && !defined(INVEL_W)
    int indice = 0;
    indice = ind_comienzo_comando_x;
    if( indice ) {
        while( CONFIG_TKT1[indice].codigo != _TKT_FIN_COMANDO_X ) {
            TKT_ejecutar_comando( &indice, 0, 0 );
        }
    }
    #endif
}
/****************************************************************************/
TKT_FISCAL_Z()
/****************************************************************************/
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    int indice = 0;
    indice = ind_comienzo_comando_z;
    if( indice ) {
        while( CONFIG_TKT1[indice].codigo != _TKT_FIN_COMANDO_Z ) {
            TKT_ejecutar_comando( &indice, 0, 0, 0 );
        }
        if( IMPRESORA_FISCAL == FISCAL ) {
            delay( 5000 );
        }
    }
    #endif
}
/****************************************************************************/
TKT_FISCAL_Y()
/****************************************************************************/
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    int indice = 0;
    indice = ind_comienzo_comando_y;
    if( indice ) {
        while( CONFIG_TKT1[indice].codigo != _TKT_FIN_COMANDO_Y ) {
            TKT_ejecutar_comando( &indice, 0, 0, 0 );
        }
    }
    #endif
}
/****************************************************************************/
TKT_INICIALIZAR_FISCAL()
/****************************************************************************/
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    int indice = 0;
    indice = ind_comienzo_impresora_fiscal;
    if( indice ) {
        if( IMPRESORA_FISCAL ) {
            MENSAJE_TECLA_STRING( S_INICIALIZANDO_IMPRESORA_FISCAL );
        }
        while( CONFIG_TKT1[indice].codigo != _TKT_FIN_IMPRESORA_FISCAL ) {
            TKT_ejecutar_comando( &indice, 0, 0, 0 );
        }
    }
    #endif
}
/****************************************************************************/
//void TKT_FISCAL_B(  )
/****************************************************************************/
/*{
    int       indice = 0;
   fiscal_b = 1;
    indice = ind_comienzo_comando_b;
   if ( indice ) {
      while( CONFIG_TKT[indice] != _TKT_FIN_COMANDO_B ) {
         TKT_ejecutar_comando( &indice, 0, 0 );
      }
      if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
         PAUSA( ticks_b );
      }
   }
}*/
/****************************************************************************/
//TKT_FISCAL_T( double importe )
/****************************************************************************/
/*{
    int       indice = 0;
    indice = ind_comienzo_comando_t;
   if ( indice ) {
      while( CONFIG_TKT[indice] != _TKT_FIN_COMANDO_T ) {
         TKT_ejecutar_comando( &indice, importe, 0 );
      }
   }
}*/
/****************************************************************************/
void TKT_CORTE_PAPEL()
/****************************************************************************/
{
    int indice = 0;
    indice = ind_comienzo_corte_papel;
    if( indice ) {
        while( CONFIG_TKT1[indice].codigo != _TKT_FIN_CORTE_PAPEL ) {
            TKT_ejecutar_comando( &indice, 0, 0, 0 );
        }
    }
}
/***************************************************************************/
int TKT_desplazar_config( int codigo, int *indice )
/***************************************************************************/
{
    /*  int       rta = 1;
      switch ( codigo ) {
         case _POS_X:
         case _POS_Y:
         case _TKT_TICKS_B:
            case _TKT_PAUSA:
         case _IVA1_COD:
         case _IVA2_COD:
         case _TKT_DATO_MEDIO:
         case _OFFSET_CADENA:
         case _SELECT:
         case _MAXIMO_RENGLONES_TRANSFERENCIA:
         case _TKT_DATO_ARTICULO:
         case _TKT_OFFSET_COLUMNA:
         case _DATO_ADICIONAL_MEDIO_PAGO:
            rta = 2;
            rta += strlen( &CONFIG_TKT[( *indice )] ) + 1;
            break;
         case _TKT_LF:
            rta = 2;
            break;
         case _IMPORTE_MEDIO:
         case _POSICION:
         case _TKT_IMPORTE_MEDIO_ME:
         case _TOTAL_TICKET:
         case _TKT_VARIABLE_RNV:
         case _TKT_OFF_VARIABLE_RNV:
            rta = 4;
            break;
        case _TKT_SI_VALOR_VARIABLE_RNV:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV:
        case _TKT_SI_VALOR_VARIABLE_RNV_OFF:
        case _TKT_SI_NO_VALOR_VARIABLE_RNV_OFF:
        case _PROCESAR_DATOS_ADICIONALES:
            rta = 6;
            break;
         case _TKT_SI_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
         case _TKT_SI_NO_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
            rta = 8;
            break;
         case _TKT_CONSULTA_TABLA:
            rta = 4;
                rta += strlen( &CONFIG_TKT[( *indice )] ) + 1;
            rta += strlen( &CONFIG_TKT[( *indice )] ) + 1;
            rta += strlen( &CONFIG_TKT[( *indice )] ) + 1;
            rta += strlen( &CONFIG_TKT[( *indice )] ) + 1;
            break;
         case _TKT_FORMULA:
            rta = strlen( &CONFIG_TKT[( *indice )] ) + 1;
            rta += strlen( &CONFIG_TKT[( *indice )] ) + 1;
            rta += strlen( &CONFIG_TKT[( *indice )] ) + 1;
            break;
         case _CADENA:
         case _TKT_COMANDO_FISCAL:
         case _TKT_NRO_CAJA:
         case _TKT_NRO_TICKET:
         case _TKT_TOTAL:
            rta = strlen( &CONFIG_TKT[( *indice )] ) + 1;
            break;
      }*/
    return ( 0 );
}
/****************************************************************************/
void IMPRIMIR_TKT_SUBTOTAL()
/****************************************************************************/
{
    if( ind_comienzo_subtotal ) {
        ind_config = ind_comienzo_subtotal;
        while( CONFIG_TKT1[ind_config].codigo != _TKT_FIN_CUERPO_SUBTOTAL ) {
            TKT_ejecutar_comando( &ind_config, 0.0, 0, 0 );
        }
    }
}
/***************************************************************************/
void TKT_incrementar_puntero( int codigo, int *ind_config )
/***************************************************************************/
{
   switch ( codigo ) {
      case _POS_X:
      case _POS_Y:
      case _TKT_TICKS_B:
      case _TKT_PAUSA:
      case _IVA1_COD:
      case _IVA2_COD:
      case _TKT_DATO_MEDIO:
      case _OFFSET_CADENA:
      case _SELECT:
      case _MAXIMO_RENGLONES_TRANSFERENCIA:
      case _TKT_DATO_ARTICULO:
      case _TKT_OFFSET_COLUMNA:
      case _TKT_LF:
      case _DATO_ADICIONAL_MEDIO_PAGO:
      case _IMPORTE_MEDIO:
      case _POSICION:
      case _TKT_IMPORTE_MEDIO_ME:
      case _TOTAL_TICKET:
      case _TKT_VARIABLE_RNV:
      case _TKT_OFF_VARIABLE_RNV:
      case _TKT_SI_VALOR_VARIABLE_RNV:
      case _TKT_SI_NO_VALOR_VARIABLE_RNV:
      case _TKT_SI_VALOR_VARIABLE_RNV_OFF:
      case _TKT_SI_NO_VALOR_VARIABLE_RNV_OFF:
      case _PROCESAR_DATOS_ADICIONALES:
      case _TKT_SI_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
      case _TKT_SI_NO_VALOR_VARIABLE_RNV_Y_SI_MEDIO:
      case _TKT_CONSULTA_TABLA:
      case _TKT_FORMULA:
      case _TKT_SI_FORMULA:
      case _CADENA:
	  case _TKT_MENSAJE_ESPECIAL:
      case _TKT_COMANDO_FISCAL:
      case _TKT_NRO_CAJA:
      case _TKT_NRO_TICKET:
      case _TKT_TOTAL:
      case _ENVIAR_BMP:
      case _TKT_CONSULTA_TABLA_SQL:
	  case _TKT_SI_VALOR_MED:
	  case _TKT_DATO_MEDIO_SUBMEDIO:
	  case _TKT_CAMPO_ARCH_REC:
	  case _TKT_FOR_CONTADOR:
	  case _IMAGEN:
      case _TKT_REPETIR_CADENA:
	  case _TKT_VALIDACION:
	  case _TKT_DIA_OPERATIVO:
	  case	_LINEA_DE_DESCRIPCION:
	  case _IMPRIME_LINEA_DE_DESCRIPCION:
	  case _PREFERENCIA_IMPRESION :
            ( *ind_config ) ++;
            break;
    }
}

/***************************************************************************/
void TKT_TOTALIZAR_TEMPORAL( int tipo_evento, int sub_evento, int disc_medios, int filtra_medios,
                             int cod_medio_sub_medio )
/***************************************************************************/
{
    if( tkt_tmp == NULL ) {
        tkt_tmp = (struct _tkt_tmp *)MALLOC( sizeof( struct _tkt_tmp ) );
    }
    if( tkt_tmp ) {
        //if( USE_DB( AREA_AUX6, _TKT_TMP_SIC, ( char* )tkt_tmp, sizeof( struct _tkt_tmp ), NULL, 0, 0 ) == 0 ) {
        if( OPEN_TABLE( T_TKT_TMP, TT_ORIG, ( char* )tkt_tmp, sizeof( struct _tkt_tmp ) ) == 0 ) {
            /*GO( TOP );
            while( !BtrvEOF() ) {
                _DELETE();
            }*/
			ZAP2();
            //SELECT( AREA_TICKET );
			SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
			SET_WHERE("");
			SET_ORDER2("ID_EVENTO");
			RUN_QUERY(NO);
            GO2( BOTTOM );
            while( !dbBOF()
                && ( ( event_ticket.cajero == NRO_CAJERO && event_ticket.tipo_evento != 9 )
                  || NRO_MODO == 'Z' ) ) {
                if( TKT_EVENTO_SOLICITADO( tipo_evento, sub_evento ) ) {
                    TKT_AGREGAR_EVENTO_A_TMP( disc_medios, filtra_medios, cod_medio_sub_medio );
                }
                //SELECT( AREA_TICKET );
				SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
                SKIP2( -1 );
            }
        }
        else {
            GRABAR_LOG_SISTEMA( "TKT_TOTALIZAR_TEMPORAL:ERROR,tkt_tmp" ,LOG_VENTAS,2);
        }
    }
}
/***************************************************************************/
void TKT_AGREGAR_EVENTO_A_TMP( int disc_medios, int filtra_medios, int cod_medio_sub_medio )
/***************************************************************************/
{
    int cod_medio, cod_submedio, agregar = 0, agrego = 0;
    struct
    {
        int cod_medio;
        int cod_submedio;
    }rel_tkt_tmp;
    //SELECT( AREA_PAGO );
    //GET_EQUAL( ( char* )&event_ticket.id_evento );
	SELECT_TABLE( T_PAGOS, tipo_t_pagos );
	SET_WHERE("ID_EVENTO = '%ld'",event_ticket.id_evento);
	SET_ORDER2("ID_EVENTO");
	RUN_QUERY(NO);
    while( !dbEOF() && event_pago.id_evento == event_ticket.id_evento ) {
        agrego = 1;
        switch( disc_medios ) {
            case 0:
                // Sin discriminar
                cod_medio = cod_submedio = 0;
                break;
            case 1:
                // Discriminar a nivel de medios
                cod_medio = event_pago.modo_pago;
                cod_submedio = 0;
                break;
            case 2:
                // Discriminar a nivel de sub medios
                cod_medio = event_pago.modo_pago;
                cod_submedio = event_pago.cod_sub_medio;
                break;
        }
        switch( filtra_medios ) {
            case 0:
                // No Filtra
                agregar = SI;
                break;
            case 1:
                // Filtro a nivel de medios
                agregar = ( cod_medio_sub_medio == event_pago.modo_pago ) ? SI : NO;
                break;
            case 2:
                // Filtro a nivel de Submedios
                if( ( ( cod_medio_sub_medio / 100 ) == event_pago.modo_pago )
                 && ( ( cod_medio_sub_medio % 100 ) == event_pago.cod_sub_medio ) ) {
                    agregar = SI;
                }
                break;
        }
        if( agregar ) {
            //SELECT( AREA_AUX6 );
			SELECT_TABLE( T_TKT_TMP, TT_ORIG );
            rel_tkt_tmp.cod_medio = cod_medio;
            rel_tkt_tmp.cod_submedio = cod_submedio;
            //GET_EQUAL( ( char* )&rel_tkt_tmp );
			SET_WHERE("COD_MEDIO='%d' AND COD_SUBMEDIO='%d'",cod_medio, cod_submedio );
			RUN_QUERY(NO);
            if( !FOUND2() ) {
                tkt_tmp->cod_medio = cod_medio;
                tkt_tmp->cod_submedio = cod_submedio;
                tkt_tmp->importe = event_pago.importe - event_pago.vuelto_efectivo;
                tkt_tmp->cantidad = 1;
                switch( disc_medios ) {
                    case 0:
                        memset( tkt_tmp->descripcion, 0, sizeof( tkt_tmp->descripcion ) );
                        break;
                    case 1:
                        strncpy( tkt_tmp->descripcion, medios[cod_medio - 1].nombre,
                                 sizeof( tkt_tmp->descripcion ) );
                        break;
                    case 2:
                        if( medios[cod_medio - 1].tarjeta ) {
                            strcpy( tkt_tmp->descripcion, NOMBRE_TARJETA( cod_submedio - 1 ) );
                        }
                        else if( medios[cod_medio - 1].mutual ) {
                            NOMBRE_MUTUAL( ( char )cod_submedio, tkt_tmp->descripcion );
                        }
                        else if( medios[cod_medio - 1].ticket_compra ) {
                            NOMBRE_TICKET_COMPRA( cod_submedio, tkt_tmp->descripcion );
                        }
                        else {
                            strncpy( tkt_tmp->descripcion, medios[cod_medio - 1].nombre,
                                     sizeof( tkt_tmp->descripcion ) );
                        }
                        break;
                }
                //SELECT( AREA_AUX6 );
				SELECT_TABLE( T_TKT_TMP, TT_ORIG );
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                INSERT2(NO);
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
            else {
                tkt_tmp->importe += ( event_pago.importe - event_pago.vuelto_efectivo );
                tkt_tmp->cantidad += 1;
                //SELECT( AREA_AUX6 );
				SELECT_TABLE( T_TKT_TMP, TT_ORIG );
                #ifdef INVEL_L
                //BEGIN_TRANSACTION();
                #endif
                UPDATE2();
                #ifdef INVEL_L
                //END_TRANSACTION();
                #endif
            }
        }
        //SELECT( AREA_PAGO );
		SELECT_TABLE( T_PAGOS, tipo_t_pagos );
        SKIP2( 1 );
    }
    if( !agrego && disc_medios == 0 && filtra_medios == 0 ) {
        //SELECT( AREA_AUX6 );
		SELECT_TABLE( T_TKT_TMP, TT_ORIG );
        rel_tkt_tmp.cod_medio = 0;
        rel_tkt_tmp.cod_submedio = 0;
        //GET_EQUAL( ( char* )&rel_tkt_tmp );
		SET_WHERE("COD_MEDIO='%d' AND COD_SUBMEDIO='%d'", 0, 0 );
		RUN_QUERY(NO);
        if( !FOUND2() ) {
            tkt_tmp->cod_medio = 0;
            tkt_tmp->cod_submedio = 0;
            tkt_tmp->importe = event_ticket.importe_sin_iva + event_ticket.exento
                             + event_ticket.iva1;
            tkt_tmp->cantidad = 1;
            memset( tkt_tmp->descripcion, 0, sizeof( tkt_tmp->descripcion ) );
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2(NO);
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
        }
        else {
            tkt_tmp->importe += event_ticket.importe_sin_iva + event_ticket.exento
                              + event_ticket.iva1;
            tkt_tmp->cantidad += 1;
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
/***************************************************************************/
int TKT_EVENTO_SOLICITADO( int tipo_evento, int sub_evento )
/***************************************************************************/
{
    int rta = 0;
    if( event_ticket.tipo_evento == tipo_evento ) {
        switch( tipo_evento ) {
            case 0:
                // Ticket
                switch( sub_evento ) {
                    case 1:
                        // Cambio
                        rta = event_ticket.cambio;
                        break;
                    default:
                        rta = 1;
                        break;
                }
                break;
            case 1:
                // Cobros
                //if( sub_evento == event_ticket.nro_ticket ) {
                    rta = 1;
                //}
                break;
            default:
                rta = 1;
                break;
        }
    }
    return( rta );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF00( int ini_est, double *dif, double *dif_me )
/***************************************************************************/
{
    int h;
    if( ind_comienzo_inf_00 ) {
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        for( h = 0 ;h < 10 ;h++ ) {
            _SET_MEMORY_CHAR( __pago_modo, h, h );
            _SET_MEMORY_DOUBLE( __pago_importe, h, dif[h] );
            _SET_MEMORY_DOUBLE( __pago_importe_me, h, dif_me[h] );
        }
        if( MODO_NEGOCIO != SERVICIOS ) {
            if( !IMPRIMIR_CABECERA_AL_FINAL ) {
                //IMPRIMIR_TKT_CABECERA( 1 );
            }
            else if( NRO_MODO == 'Z' ) {
                LF( 10 );
                PRN_CORTE_TOTAL();
                IMPRIMIR_TKT_CABECERA( 1 );
            }
        }
        else {
            SET_MEMORY_CHAR( __ram_imprimir_cabecera, ( char )0 );
        }
        ind_config = ind_comienzo_inf_00;
        _TKT_INI_EST = ini_est;
		//IMPRIMIR_INFORME_MEDIOS( );
		//if (IMPRESORA_FISCAL != FISCAL){
			while( CONFIG_TKT1[ind_config].codigo != _FIN_INFORME_00 ) {
				TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
			}
		//}
    }

    return( ind_comienzo_inf_00 );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF05( int ini_est )
/***************************************************************************/
{
    //int h;

	if( ind_comienzo_inf_05 ) {
		ind_config = ind_comienzo_inf_05;
        _TKT_INI_EST = ini_est;
        while( CONFIG_TKT1[ind_config].codigo != _FIN_INFORME_05 ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }

    return( ind_comienzo_inf_05 );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF10_CABECERA( )
/***************************************************************************/
{
    //int h;

	if( ind_comienzo_cabecera_inf_10 ) {
		ind_config = ind_comienzo_cabecera_inf_10;
        while( CONFIG_TKT1[ind_config].codigo != _FIN_CABECERA_INFORME_10 ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }

    return( ind_comienzo_cabecera_inf_10 );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF10( int ini_est )
/***************************************************************************/
{
    //int h;

	if( ind_comienzo_inf_10 ) {
		ind_config = ind_comienzo_inf_10;
		_TKT_INI_EST = ini_est;
        while( CONFIG_TKT1[ind_config].codigo != _FIN_INFORME_10 ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }

    return( ind_comienzo_inf_10 );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF15_CABECERA( )
/***************************************************************************/
{
    //int h;
	if( ind_comienzo_cabecera_inf_15 ) {
		ind_config = ind_comienzo_cabecera_inf_15;
        while( CONFIG_TKT1[ind_config].codigo != _FIN_CABECERA_INFORME_15 ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }

    return( ind_comienzo_cabecera_inf_15 );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF15( int ini_est )
/***************************************************************************/
{
    //int h;

	if( ind_comienzo_inf_15 ) {
		ind_config = ind_comienzo_inf_15;
	    _TKT_INI_EST = ini_est;
        while( CONFIG_TKT1[ind_config].codigo != _FIN_INFORME_15 ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }

    return( ind_comienzo_inf_15 );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF_Y( int ini_est,int inicio_tkt,int fin_tkt )
/***************************************************************************/
{
    if( inicio_tkt ) {
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        if( MODO_NEGOCIO != SERVICIOS ) {
            if( !IMPRIMIR_CABECERA_AL_FINAL ) {
                //IMPRIMIR_TKT_CABECERA( 1 );
            }
            else if( NRO_MODO == 'Z' ) {
                LF( 10 );
                PRN_CORTE_TOTAL();
                IMPRIMIR_TKT_CABECERA( 1 );
            }
        }
        else {
            SET_MEMORY_CHAR( __ram_imprimir_cabecera, ( char )0 );
        } 

        ind_config = inicio_tkt;
        _TKT_INI_EST = ini_est;
	//	IMPRIMIR_INFORME_MEDIOS( );
        while( ind_config && CONFIG_TKT1[ind_config].codigo != fin_tkt ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }
    return( inicio_tkt );
}



/***************************************************************************/
int IMPRIMIR_TKT_INF_DIFERENCIAS()
/***************************************************************************/
{
    //int       h;
    if( ind_informe_diferencia ) {
        ind_config = ind_informe_diferencia;
        //_TKT_INI_EST = ini_est;
        while( CONFIG_TKT1[ind_config].codigo != _TKT_FIN_INF_DIFERENCIA ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }
    return( ind_informe_diferencia );
}
/***************************************************************************/
int IMPRIMIR_TKT_INF_DIFERENCIAS_SUBMEDIOS_CONTINUO()
/***************************************************************************/
{
    if( ind_informe_diferencia_submedios ) {
        ind_config = ind_informe_diferencia_submedios;
        while( CONFIG_TKT1[ind_config].codigo != _TKT_FIN_INF_DIFERENCIA_SUBMEDIOS ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }
    return( ind_informe_diferencia_submedios );
}
/***************************************************************************/
int IMPRIMIR_TKT_FONDO_FIJO()
/***************************************************************************/
{
    //int       h;
    if( ind_comienzo_f_fijo ) {
        ind_config = ind_comienzo_f_fijo;
        //_TKT_INI_EST = ini_est;
        while( CONFIG_TKT1[ind_config].codigo != _TKT_FIN_CUERPO_F_FIJO ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }
    return( ind_comienzo_f_fijo );
}
/***************************************************************************/
void INFORME_TARJETAS( int est )
/***************************************************************************/
{
    int /*h,*/ cant = 0;

	if( ind_comienzo_tarjetas ) {
		if( !TKT_CONFIGURABLE ) {
			switch( est ) {
				case _x:
					EMITIR_INFORME_TARJETAS( NO, NO );
					break;
				case _z:
					EMITIR_INFORME_TARJETAS( SI, NO );
					break;
				case _z_ant:
					// cerrar eventos y abrir eventos remoto caja
					CERRAR_ARCHIVOS_EVENTOS_LOCAL();
					if( !ABRIR_ARCHIVOS_EVENTOS_BACKUP() ) {
						EMITIR_INFORME_TARJETAS( SI, SI );
						CERRAR_ARCHIVOS_EVENTOS_LOCAL();
					}
					ABRIR_ARCHIVOS_EVENTOS_LOCAL();
					break;
			}
			//ind_config = ind_comienzo_tarjetas;
			//while( CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_TARJETAS ) {
				//TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
			//}
			INFORME_TARJETAS_PIE();
		} else {
			ind_config = ind_comienzo_tarjetas;
			while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_TARJETAS ) {
				TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
			}
		}
    }
}
/* Funcion auxiliar de OBTENER_DATO_TABLA
 */
/******************************************************************************/
/* Dado el nombre de un tipo devuelve un char
 * y un int determinando si es numerico.
 */
int obtenerTipo( char *nombre, char *abrev, int *numerico )
{
    int i;
    char *tipos[] = {"int","long","double","string","char","float","uint","ulong","byte","strstr"};
    char tipos_abrev[] = "ildscfunbs";
    int tipos_nume[] = {1,1,1,0,0,1,1,1,1,2};

    int tam = sizeof( tipos ) / sizeof( *tipos );
    if( nombre ) {
        for( i = 0;i < tam;i++ ) {
            if( !strcmp( nombre, tipos[i] ) ) {
                if( abrev ) {
                    *abrev = tipos_abrev[i];
                }
                if( numerico ) {
                    *numerico = tipos_nume[i];
                }
                return 0;
            }
        }
    }
    return -1;
}
/******************************************************************************/
/******************************************************************************/
void OBTENER_DATO_TABLA( int codtabla, int orden, char *indice, char *tipores, char offset[200]/*, int linea*/ )
/******************************************************************************/
{
    int codigo_offset, tamanio = 0, dato_es_numerico;
    char estructura[1200],temp[255],tipo,tiporesCh,*token,*indice_final,res_string[250];
    struct _valores_calculo val_calc;
    INT16 inttemporal;
    UINT16 uinttemporal;
    long longtemporal;
    double doubletemporal,resultado;
    char chartemporal;
    float floattemporal;
    unsigned long ulongtemporal;
    char bytetemporal;
	memset(&val_calc,0,sizeof(struct _valores_calculo));
    /*
    Ejemplo:
    [CONSULTA TABLA]        [106] [6] [(long)var(1);(int)4;] [long] [15] [0]
    [106] codigo de la tabla, ver aarch.h
    [6]   indice
    [(long)var(1);(int)4;] valores del o los indices (para el "var" ver mas abajo)
    [long] [15] [0] valores del resultado
    para el "var" se debe utilizar por ejemplo:
    [FORMULA]               [asignar(1,{#299}-1)]
    */
    //offset del valor pedido en el struct "instanciado" en estructura[]
    codigo_offset = atoi( offset );
    strcpy( offset, "" );
    if( ( obtenerTipo( tipores, &tiporesCh, NULL ) ) == -1 ) {
        GRABAR_LOG_SISTEMA( "ERROR DATO TABLA: tipo no definido" ,LOG_ODBC,1);
        return;
    }
    //Se interpreta el indice (tipo1)dato1;(tipo2)dato2;...(tipoN)datoN;
    token = strtok( indice, "()" );
    while( token != NULL ) {
        if( ( obtenerTipo( token, &tipo, &dato_es_numerico ) ) == -1 ) {
            GRABAR_LOG_SISTEMA( "ERROR DATO TABLA: tipo no definido" ,LOG_ODBC,1);
            return;
        }
        token = strtok( NULL, ";" );
        if( token == NULL ) {
            GRABAR_LOG_SISTEMA( "ERROR DATO TABLA: indice mal formado",LOG_ODBC,1 );
            return;
        }
        //Si el valor del indice de busqueda es numerico entonces se
        //interpreta para resolver variables RNV, etc.
        if( dato_es_numerico ) {
            //Dato para calcular variables rnv off
            val_calc.tkt_ini_est = _TKT_INI_EST;
			if( dato_es_numerico == 1 ) {
            resultado = RESOLVER( token, &val_calc );
            sprintf( res_string, "%f", resultado );
			} else {
				resultado = _RESOLVER( token, &val_calc, res_string );
			}
        }
        else {
            strcpy( res_string, token );
        }
        switch( tipo ) {
            case 'i':
                //no probado
                inttemporal = ( INT16 )strtoul( res_string, NULL, 10 );
                memcpy( &temp[tamanio], &inttemporal, sizeof( INT16 ) );
                tamanio += sizeof( INT16 );
                break;
            case 'l':
                longtemporal = atol( res_string );
                memcpy( &temp[tamanio], &longtemporal, sizeof( long ) );
                tamanio += sizeof( long );
                break;
            case 'd':
                //no probado
                doubletemporal = strtod( res_string, NULL );
                memcpy( &temp[tamanio], &doubletemporal, sizeof( double ) );
                tamanio += sizeof( double );
                break;
            case 's':
                strcpy( &temp[tamanio], res_string );
                tamanio += strlen( res_string ) + 1;
                break;
            case 'c':
                chartemporal = ( char )res_string[0]; //Y si no es 1?
                memcpy( &temp[tamanio], &chartemporal, sizeof( char ) );
                tamanio += sizeof( char );
                break;
            case 'f':
                //no probado
                floattemporal = ( float )atof( res_string );
                memcpy( &temp[tamanio], &floattemporal, sizeof( float ) );
                tamanio += sizeof( float );
                break;
            case 'u':
                //no probado
                uinttemporal = ( UINT16 )strtoul( res_string, NULL, 10 );
                memcpy( &temp[tamanio], &uinttemporal, sizeof( UINT16 ) );
                tamanio += sizeof( UINT16 );
                break;
            case 'n':
                ulongtemporal = strtoul( res_string, NULL, 10 );
                memcpy( &temp[tamanio], &ulongtemporal, sizeof( unsigned long ) );
                tamanio += sizeof( unsigned long );
                break;
            case 'b':
                bytetemporal = ( char )atoi( res_string );
                memcpy( &temp[tamanio], &bytetemporal, sizeof( char ) );
                tamanio += sizeof( char );
            default:
                break;
        }
        token = strtok( NULL, "()" );
    }
    if( ( indice_final = malloc( 255 ) ) == NULL ) //Juanf
    {
        return;
    }
    memcpy( indice_final, temp, tamanio );
    if( !USE_DB( AREA_TKT_CONSULTA, codtabla, ( char* )&estructura, sizeof( estructura ), NULL, 0,
                 0 ) ) {
        SELECT( AREA_TKT_CONSULTA );
        GO( TOP );
        SET_ORDER( orden );
        GET_EQUAL( ( char* )indice_final );
        if( FOUND() ) {
            //El resultado es devuelto en forma de string en el parametro @offset
            switch( tiporesCh ) {
                case 'i':
                    inttemporal = *( INT16 * )( estructura + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%d", ( int )inttemporal );
                    #else
                    _itoa( ( int )inttemporal, offset, 10 );
                    #endif
                    break;
                case 'l':
                    longtemporal = *( long* )( estructura + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%ld", longtemporal );
                    #else
                    _ltoa( longtemporal, offset, 10 );
                    #endif
                    break;
                case 'd':
                    doubletemporal = *( double* )( estructura + codigo_offset );
                    sprintf( offset, "%f", doubletemporal );
                    //FTOC( doubletemporal, offset, ENTEROS, DECIMALES, SI, NO );
                    break;
                case 's':
                    strcpy( offset, ( char* )estructura + codigo_offset );
                    break;
                case 'c':
                    offset[0] = *( char* )( estructura + codigo_offset );
                    offset[1] = 0;
                    break;
                case 'f':
                    floattemporal = *( float* )( estructura + codigo_offset );
                    //FTOC( (double) floattemporal, offset, ENTEROS, DECIMALES, 1, 1 );
                    sprintf( offset, "%f", floattemporal );
                    break;
                case 'u':
                    uinttemporal = *( UINT16 * )( estructura + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%u", ( unsigned int ) uinttemporal );
                    #else
                    _ultoa( ( unsigned long ) uinttemporal, offset, 10 );
                    #endif
                    break;
                case 'n':
                    ulongtemporal = *( unsigned long* )( estructura + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%lu", ulongtemporal );
                    #else
                    _ultoa( ulongtemporal, offset, 10 );
                    #endif
                    break;
				case 'b':
                    bytetemporal = *( char* )( estructura + codigo_offset );
                    sprintf( offset, "%d", bytetemporal );
					break;
                default:
                    break;
            }
        }
        //Se movio el mensaje de error a la funcion RESOLVER_FUNCIONES
        /*else //!FOUND( )
        {
            char strTmp[70];
            sprintf(strTmp,"ERROR DATO TABLA: dato no encontrado linea tkt: %d",linea);
            GRABAR_LOG_SISTEMA( strTmp );
        }*/
        CLOSE_DATABASE( AREA_TKT_CONSULTA );
    }
    else //USE_DB( AREA_AUX
    {
        GRABAR_LOG_SISTEMA( "ERROR DATO TABLA: area no disponible" ,LOG_ODBC,1);
    }
    if( indice_final ) {
        free( indice_final );
    }
}
void ARMAR_MENSAJE( char *buffer, char *cad1 )
{
    int pos1 = 0, pos2 = 0, type = 0, digitos = 0;
    char varaux[200];

    pos1 = strcspn( buffer, "({" );
    pos2 = strcspn( buffer, "})" );
    memset( varaux, 0, 200 );
	if( pos2 != pos1 ) {
		strncpy(varaux,&buffer[pos1 + 3],sizeof( varaux )-1);
		//strcpy( varaux, &buffer[pos1 + 3] );
        //pos2 - 3 - pos1 -> no controla que no sea < 0 esto ocacionaba error
		//varaux[pos2 - 3 - pos1] = '\0';
		digitos = strcspn( varaux, "})" );
		varaux[digitos] = '\0';
		type = GET_TYPE( atoi( varaux ) );
	}
	if( pos2 != pos1 && type ) {
		switch( type ) {
            case 1:
				_snprintf( varaux, sizeof(varaux)-1, "%c", GET_MEMORY_CHAR( atoi( varaux ) ) );
                break;
            case 2:
				_snprintf( varaux, sizeof(varaux)-1, "%d", GET_MEMORY_INT( atoi( varaux ) ) );
                break;
            case 3:
				_snprintf( varaux, sizeof(varaux)-1, "%f", GET_MEMORY_FLOAT( atoi( varaux ) ) );
                break;
            case 4:
				_snprintf( varaux, sizeof(varaux)-1, "%f", GET_MEMORY_DOUBLE( atoi( varaux ) ) );
                break;
            case 5:
				_snprintf( varaux, sizeof(varaux)-1, "%s", GET_MEMORY_STR( atoi( varaux ) ) );
                break;
            case 8:
				_snprintf( varaux, sizeof(varaux)-1, "%ld", GET_MEMORY_LONG( atoi( varaux ) ) );
                break;
            case 9:
				_snprintf( varaux, sizeof(varaux)-1, "%ud", GET_MEMORY_UNSIGNED( atoi( varaux ) ) );
                break;
        }
        strncpy( cad1, buffer, pos1 );
        cad1[pos1] = '\0';
        strcat( cad1, varaux );
        strcat( cad1, &buffer[pos2 + 2] );
    }
    else {
        strcpy( cad1, buffer );
    }
}
void RESOLVER_FUNCIONES( char *funciones, char *RtaConsulta, int linea )
{
    char *token,*param,strDefault[50],strAsignar[100],funcion[50],strTemp[12],strImporte[20],mascara[50], cadena_aux[50],cadena_aux2[50];
    int /*cnt,*/ mostrar = 1, varAsignar = -1, convFecha = 0, importe = 0,ancho_string = 6;
	int varSiDato = -1, varNoDato = -1,alineado = 0,alineado_derecha = 0, dignro = 0, largo_str = 0, partefecha = 0, i = 0;
    double rta;
    /*
       Analiza las funciones del consultar tabla
       Funciones permitidas
          * default(string)      muestra el string en caso de no encontrar el dato en la tabla
          * asignar(variable)    asigna el valor encontrado en la consulta a la variable, en caso de no
                                 encontrar nada asigna el valor por defecto, en caso de no tener valor
                                 por defecto, asigna 0
          * noprint()            no imprime nada ni el default
          * fecha()              convierte el formato invel (int) al formato dd/mm/aaaa
          * trim()               saca los espacios
          * derecha(ancho)       alinacion derecha el parametro ancho determina el ancho que tendra el string resultante
          * mascara(mascara)     imprime un numero long con una mascara definida
		  * enmascara(dignro)    enmascara el nro de tarjeta
          * importe()            formatea la salida con los separadores de decimales y miles y cant de decimales con los que este configurada la caja
          * importe_ald()        igual que importe pero alinea a la derecha. (tamaño de la cadena resultante hardcodeada en 13)
          * sidato(variable)     aumenta en uno la variable del parametro si la consulta devolvio datos correctamente.
          * nodato(variable)     aumenta en uno la variable del parametro si no se encontro el dato.
          * log(cuando)          genera una entrada en el log del sistema dependiendo del parametro:
                                 1) cuando = SIEMPRE, loguea siempre.
                                 2) cuando = SI_VACIO, loguea si la consulta no arrojó resultados
          * enmasnro(cant)       reemplaza los caracteres del resultado por '*' exeptuando los ultimos 'cant' digitos
          * largo_str(largo)     corta la salida para que tenga a lo sumo 'largo' caracteres
          * partefecha(numero)   si numero es 1: interpreta el resultado que se asume es una fecha, y reemplaza los numeros por los nombres de los dias y meses.
                                 si numero es otro valor: imprime la fecha directamente.
		  * memoria(long)		deja los datos en un buffer para usar..., por parametro se pasa el largo a usar.
    */
    mascara[0] = 0;
    strcpy( strDefault, "" );
    /*Busca la primera funcion*/
    token = strtok( funciones, ")" );
	while( token != NULL ) {
        sprintf( funcion, "%s", token );
        LTRIM( funcion );
        param = strpbrk( funcion, "(" ) + 1;
        //token = strtok( NULL, ")" ); //parametros de la funcion
        if( strncmp( funcion, "default", /*largo de dafault*/ 7 ) == 0 ) {
            if( param && strcmp( param, "NULL" ) != 0 ) {
                sprintf( strDefault, "%s", param );
            }
        }
        else if( strncmp( funcion, "noprint", 7 ) == 0 ) {
            mostrar = 0;
        }
        else if( strncmp( funcion, "asignar", 7 ) == 0 ) {
			
			if( isalpha( (char)param[0])){
				varAsignar = DEVUELVE_POSICION_NOMBRE( param );
			}else
	            varAsignar = atoi( param );
        }
        else if( strncmp( funcion, "fecha", 5 ) == 0 ) {
            convFecha = 1;
        }
        else if( strncmp( funcion, "trim", 4 ) == 0 ) {
            if( strlen( RtaConsulta ) > 0 ) {
                //si se encontro el dato en la consulta
                LTRIM( RtaConsulta );
                TRIM( RtaConsulta );
            }
        }
        else if( strncmp( funcion, "importe", 7 ) == 0 ) {
            importe = 1;
        }
        else if( strncmp( funcion, "importe_ald", 11 ) == 0 ) {
            importe = 1;
            alineado = 1;
        }
        else if( strncmp( funcion, "sidato", 6 ) == 0 ) {
            varSiDato = atoi( param );
        }
        else if( strncmp( funcion, "nodato", 6 ) == 0 ) {
            varNoDato = atoi( param );
        }
        else if( strncmp( funcion, "derecha", 7 ) == 0 ) {
            alineado_derecha = 1;
            ancho_string = atoi( param );
        }
        else if( strncmp( funcion, "mascara", 7 ) == 0 ) {
            strcpy(mascara,param);
        }
        else if( strncmp( funcion, "log", 3 ) == 0 ) {
            char log[70];
            if( param && strcmp( param, "SIEMPRE" ) == 0 ) {
                sprintf( log, "TKT (linea %d): Consulta = %s",linea, RtaConsulta );
            }
            else if( param && strcmp( param, "SI_VACIO" ) == 0 && strlen( RtaConsulta ) <= 0 ) {
                sprintf( log, "TKT (linea %d): La consulta no tuvo resultado",linea, RtaConsulta );
            }
            GRABAR_LOG_SISTEMA( log ,LOG_VENTAS,2);
		}
		else if( strncmp( funcion, "enmasnro", 8 ) == 0 ) {
			dignro = atoi( param );
		}
		else if( strncmp( funcion, "largo_str", 9 ) == 0 ) {
            largo_str = atoi( param );
        }
		else if( strncmp( funcion, "partefecha", 10 ) == 0 ) {
            partefecha = atoi( param );
        } if( strncmp( funcion, "memoria", 7 ) == 0 ) {//memoria(long)
			int longitud = 0;
            longitud  = atoi( param );
			if(longitud==0)
				memset(memoria_comun_tkt,0,sizeof(memoria_comun_tkt));
			else{
				if(longitud>0){
					if( strlen(RtaConsulta)<longitud )
						longitud = strlen(RtaConsulta);
					RtaConsulta[longitud]=0;
					if(( strlen(memoria_comun_tkt)+longitud) < sizeof(memoria_comun_tkt))
						strcat(memoria_comun_tkt,RtaConsulta );
				}
			}
		}
        token = strtok( NULL, ")" ); // siguiente funcion
    }
    if( varAsignar >= 0) {
        // si hay una variable que asignar
        if( strlen( RtaConsulta ) > 0 ) {
            //si se encontro el dato en la consulta
            sprintf( strAsignar, "asignar(%i,%s)", varAsignar, RtaConsulta );
        }
        else if( strlen( strDefault ) > 0 ) //si hay dato por defecto
        {
            sprintf( strAsignar, "asignar(%d,%s)", varAsignar, strDefault );
        }
        else {
            sprintf( strAsignar, "asignar(%d,%s)", varAsignar, "0" );
        }
        rta = RESOLVER( strAsignar, NULL );
    }
    if( strlen( RtaConsulta ) > 0 ) {
        //si se encontro dato
        if( varSiDato >= 0 ) {
            //si hay que incrementar porque se encontro dato
            sprintf( strAsignar, "asignar(%d,var(%d)+1)", varSiDato, varSiDato );
            rta = RESOLVER( strAsignar, NULL );
        }
    }
    else {
        //no se encontro dato
        if( varNoDato >= 0 ) {
            //si hay que incrementar porque no se encontro dato
            sprintf( strAsignar, "asignar(%d,var(%d)+1)", varNoDato, varNoDato );
            rta = RESOLVER( strAsignar, NULL );
        }
    }
    if( mostrar ) {
        if( strlen( RtaConsulta ) > 0 ) {
            //si se encontro el dato en la consulta lo muestra
            if( convFecha ) {
                // se fija si tiene que convertir la fecha
                /* chequeo si se paso el parametro derecha() */
                if( 1 == alineado_derecha ) {
                    derecha_alinear( ancho_string, RtaConsulta );
                }
                DTOC_2000( atoi( RtaConsulta ), strTemp ); // formato dd/mm/aaaa
                MOSTRAR_CADENA( tkt_columna, PRN, strTemp );
            }
            else {
                if( importe ) {
                    //MOSTRAR_FLOTANTE( tkt_columna, PRN, ENTEROS, DECIMALES, SI, SI, strtod(RtaConsulta,NULL) );
                    FTOC( strtod( RtaConsulta, NULL ), strImporte, ENTEROS + 3, DECIMALES, SI, SI );
                    if( alineado == 1 ) {
                        MOSTRAR_CADENA_ALD( tkt_columna, PRN, strImporte, 13 );
                    }
                    else {
                        LTRIM( strImporte );
                        TRIM( strImporte );
                        if( 1 == alineado_derecha ) {
                            derecha_alinear( ancho_string, strImporte );
                        }
                        MOSTRAR_CADENA( tkt_columna, PRN, strImporte );
                    }
                }
                else {
					if( dignro > 0 ) {
						char nro_tar[30];
						int l_buffer = 0;
						char buffer1[30], buffer2[30];
						memset( buffer1, 0, sizeof(buffer1) );
						memset( buffer2, 0, sizeof(buffer2) );
						memset( nro_tar, 0, sizeof(nro_tar) );
						sprintf( nro_tar, RtaConsulta, strlen( RtaConsulta )  );
						l_buffer = strlen( nro_tar );
						memcpy( buffer1, nro_tar, l_buffer );
						memset( buffer2, '*', l_buffer );
						memcpy( &buffer2[0], &buffer1[0], l_buffer - dignro );
						memcpy( nro_tar, buffer2, l_buffer );
						nro_tar[l_buffer]=0;
						strncpy( RtaConsulta, nro_tar, strlen( nro_tar ) );
					}
                    if( 1 == alineado_derecha ) {
                        derecha_alinear( ancho_string, RtaConsulta );
                    }
                    if(strlen(mascara) > 0){ //si hay que mostrar con mascara
                        MOSTRAR_ENTERO( tkt_columna, PRN, mascara, atol(RtaConsulta) );
                    }else{
						if( largo_str > 0 ) {
							RtaConsulta[largo_str] = 0;
							MOSTRAR_CADENA( tkt_columna, PRN, RtaConsulta );
						} else {
							if( partefecha > 0 ) {
								if( partefecha == 1 ) { //dia+nombremes
									char cadfecha[10], strmes[50];
									NAME_MON( atoi( RtaConsulta ), strmes, SI );
									strmes[3]=0;
									sprintf( cadfecha, "%d%3s", DAY( atoi( RtaConsulta ) ), strmes );
									MOSTRAR_CADENA( tkt_columna, PRN, cadfecha );
								}
							} else {
								    memset(cadena_aux,0,sizeof(cadena_aux));
									memset(cadena_aux2,0,sizeof(cadena_aux2));
									_snprintf(cadena_aux2, sizeof(cadena_aux2)-1, "%s", RtaConsulta);
									if(IMPRESORA_FISCAL == FISCAL)
									{		
										/*for(i=strlen(cadena_aux); i<tkt_columna; i++)
											cadena_aux[i]= ' ';
										strcat(cadena_aux, cadena_aux2);*/
										IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cadena_aux2, NO );
									}
									else
										MOSTRAR_CADENA( tkt_columna, PRN, RtaConsulta );
							}
						}
                    }
                }
            }
        }
        else {
            // sino muestra el dato por defecto
			if( dignro > 0 ) {
						char nro_tar[30];
						int l_buffer = 0;
						char buffer1[30], buffer2[30];
						memset( buffer1, 0, sizeof(buffer1) );
						memset( buffer2, 0, sizeof(buffer2) );
						memset( nro_tar, 0, sizeof(nro_tar) );
						sprintf( nro_tar, RtaConsulta, strlen( RtaConsulta )  );
						l_buffer = strlen( nro_tar );
						memcpy( buffer1, nro_tar, l_buffer );
						memset( buffer2, '*', l_buffer );
						memcpy( &buffer2[0], &buffer1[0], l_buffer - dignro );
						memcpy( nro_tar, buffer2, l_buffer );
						nro_tar[l_buffer]=0;
						strncpy( RtaConsulta, nro_tar, strlen( nro_tar ) );
			}
            if( strlen( strDefault ) > 0 ) {
                if( 1 == alineado_derecha ) {
                    derecha_alinear( ancho_string, strDefault );
                }
            }
            if(strlen(mascara) > 0){ //si hay que mostrar con mascara
                MOSTRAR_ENTERO( tkt_columna, PRN, mascara, atol(strDefault) );
            }else{
                MOSTRAR_CADENA( tkt_columna, PRN, strDefault );
            }
        }
    }
}
/************************************************************************
*  Funcion: alinear_derecha.
*  Entrada: - int largo : cantidad de caracteres a mostrar alineados
*                         a la derecha.
*           - char * Consulta : esta cadena contiene la cadena que se quiere
*                         alinear a la derecha.
*  Salida:  - void.
*  Descripcion : Esta funcion convierte la cadena de entrada en el largo
*                solicitado y lo alinea a la derecha.
**************************************************************************/
void derecha_alinear( int largo, char *Consulta )
{
    char *aux;
    int Tmp,i = 0;
    /* verifico si se agrego algun parametro sino lo seteo con 6 */
    if( 0 == largo ) {
        largo = 6;
    }
    aux = ( char* )malloc( ( largo * sizeof( char ) ) + 1 );
    Tmp = largo - strlen( Consulta );
    /* chequeo que largo sea mayor a la Consulta */
    if( Tmp > 0 ) {
        /* relleno los primeros valores con espacios */
        for( i = 0;i < Tmp;i++ ) {
            aux[i] = 32;
        }
        /* Asigno al vector lo que apunta Consulta */
        /* Copia tambien el caracter nulo */
        for( i = 0;i <= ( int )strlen( Consulta );i++ ) {
            aux[i + Tmp] = Consulta[i];
        }
    }
    else {
        /* Asigno al vector el contenido de Consulta */
        /* Copia hasta el caracter nulo de Consulta inclusive */
        for( i = 0;i <= largo;i++ ) {
            aux[i] = Consulta[-Tmp + i];
        }
    }
    /* asigno el valor del vector aux a la Consulta */
    strcpy( Consulta, aux );
}
/* Funcion que calcula la cantidad de hojas que se utilizan dentro de un
 * bloque (ind_comienzo, ind_fin) de tkt, el calculo lo hace sumando todos
 * los LF y comparando con cantMaxRenglones sumando 1 a cantTotalCopias
 * cada vez que supere cantMaxRenglones. Tener en cuenta que la inicializacion
 * de cantTotalCopias no se hace en esta funcion
 */
void CALCULAR_HOJAS(int indice, int ind_fin)
{
    while( CONFIG_TKT1[indice].codigo != ind_fin ) {
        //TKT_ejecutar_comando( &ind_config, 1, 1, 0 );
    }
}
/* Toma un tipo de evento y busca en la tabla Ticket.Btr el ultimo registro
 * del tipo. Retorna el id_evento de ese registro.
 ******************************************************************************/
long ULTIMO_ID_EVENTO( int tipo_evento )
/******************************************************************************/
{
    long id_evento = -1;
	//Se verifica si es una reimpresion para saber en que tabla buscar el evento
	//Si es reipresion buscamos en TICKET.BAK
	if( _IMPRIMIENDO_Z_ANT == 1 ){
		if( /*!USED( AREA_AUX )*/ !OPENED( T_EVENTOS, TT_BAK )){
			//USE_DB( AREA_AUX, _TICKET_BAK2,( char* )&event_ticket, sizeof( struct _eventos ), NULL, 0, 0);
			OPEN_TABLE(T_EVENTOS, TT_BAK,( char* )&event_ticket,sizeof( event_ticket ));
			//SELECT( AREA_AUX );
			SELECT_TABLE( T_EVENTOS, TT_BAK );
		}
	}
	//Si no es reimpresion buscamos en TICKET.BTR
	else{
		//SELECT(AREA_TICKET);
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	}
	/*(_IMPRIMIENDO_Z_ANT) ? SET_ORDER(5) :*/ SET_WHERE( "TIPO_EVENTO = '%i'", tipo_evento);
	if( /*(_IMPRIMIENDO_Z_ANT) ? !GET_EQUAL( ( char* )&tipo_evento ) :*/ !RUN_QUERY(NO) ) {
        while ( event_ticket.tipo_evento == tipo_evento &&
			( /*(_IMPRIMIENDO_Z_ANT) ? !( BtrvEOF() || BOF() ) :*/ !( dbEOF() || dbBOF() ))) {
            id_evento = event_ticket.id_evento;
			/*(_IMPRIMIENDO_Z_ANT) ? GET_NEXT(  ) :*/ SKIP2(1);
        }
    }
	if( /*USED( AREA_AUX )*/OPENED( T_EVENTOS, TT_BAK ) && _IMPRIMIENDO_Z_ANT ) {
		//CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_EVENTOS, TT_BAK );
	}

    return id_evento;
}
/******************************************************************************/
long CANTIDAD_EVENTOS( int tipo_evento , int evento_bak)
/******************************************************************************/
{	// Esta funcion, cuenta la cantidad de eventos del tipo "tipo_evento" que hay
	int salir=SI;
    long cant_eventos = 0;
	long caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L )+(( evento_bak > 0 )?-1:0);
	
	//REVISO EN LOS EVENTOS ACUALES...(TANTO PARA LA Z ACTUAL COMO PARA LA ANTERIOR POR SI NO SE PASARON
	SELECT_TABLE( T_EVENTOS, tipo_t_eventos );

    SET_WHERE( " CAJA_Z = %ld AND TIPO_EVENTO = %i", caja_z, tipo_evento );
	SET_ORDER2("CAJA_Z, ID_EVENTO");
	RUN_QUERY(NO);
	cant_eventos += RECCOUNT2( NULL, NULL);
    
	//REVISO EN LOS EVENTOS BAK  SI ES Z ANTERIOR
	if( _IMPRIMIENDO_Z_ANT == 1 || evento_bak ){
		while( salir >= 0 ){
			if( !OPENED( T_EVENTOS, ((salir==1)? TT_BAK : TT_BAK_ULT) ) ){
				OPEN_TABLE(T_EVENTOS, ((salir==1)? TT_BAK : TT_BAK_ULT),( char* )&event_ticket,sizeof( event_ticket ));
				SELECT_TABLE( T_EVENTOS, ((salir==1)? TT_BAK : TT_BAK_ULT) );
				SET_WHERE( " CAJA_Z = %ld AND TIPO_EVENTO = %i", caja_z, tipo_evento );
				SET_ORDER2("CAJA_Z, ID_EVENTO");
				RUN_QUERY(NO);
				cant_eventos += RECCOUNT2( NULL, NULL);
				CLOSE_TABLE( T_EVENTOS, ((salir==1)? TT_BAK : TT_BAK_ULT) );
			}
		--salir;
		}
	}
	return cant_eventos;
}

/******************************************************************************/
long CANTIDAD_EVENTOS_ORIGINAL( int tipo_evento )
/******************************************************************************/
{	// Esta funcion, cuenta la cantidad de eventos del tipo "tipo_evento" que hay
	// en las tablas TICKET.BTR o TICKET.BAK
    long cant_eventos = 0;
	//Se verifica si es una reimpresion para saber en que tabla buscar el evento
	//Si es reipresion buscamos en TICKET.BAK
	if( _IMPRIMIENDO_Z_ANT == 1 ){
		if(  /*!USED( AREA_AUX )*/ !OPENED( T_EVENTOS, TT_BAK ) ){
			//USE_DB( AREA_AUX, _TICKET_BAK2,( char* )&event_ticket, sizeof( struct _eventos ), NULL, 0, 0);
			OPEN_TABLE(T_EVENTOS, TT_BAK,( char* )&event_ticket,sizeof( event_ticket ));
			//SELECT( AREA_AUX );
			SELECT_TABLE( T_EVENTOS, TT_BAK );
		}
	}
	//Si no es reimpresion buscamos en TICKET.BTR
	else{
		//SELECT(AREA_TICKET);
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	}
    /*(_IMPRIMIENDO_Z_ANT) ? SET_ORDER(5) :*/ SET_WHERE( "TIPO_EVENTO = '%i'", tipo_evento);
    if( /*(_IMPRIMIENDO_Z_ANT) ? !GET_EQUAL( ( char* )&tipo_evento ) :*/ !RUN_QUERY(NO) ) {
        while ( event_ticket.tipo_evento == tipo_evento &&
			( /*(_IMPRIMIENDO_Z_ANT) ? !( BtrvEOF() || BOF() ) :*/ !( dbEOF() || dbBOF() ) )) {
            cant_eventos++;
            /*(_IMPRIMIENDO_Z_ANT) ? GET_NEXT(  ) :*/ SKIP2(1);
        }
    }
	if( /*USED( AREA_AUX )*/OPENED( T_EVENTOS, TT_BAK ) && _IMPRIMIENDO_Z_ANT ) {
		//CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_EVENTOS, TT_BAK );
	}

    return cant_eventos;
}
/******************************************************************************/
long ID_EVENTO_NRO( int nro_evento, int tipo_evento )
/******************************************************************************/
{	// Esta funcion busca en las tablas TICKET.BTR o TICKET.BAK el ID del evento
	// de tipo "tipo_evento" con orden de ocurrencia "nro_evento".
	int i = 0;
	long id_evento = -1;
	//Se verifica si es una reimpresion para saber en que tabla buscar el evento
	//Si es reipresion buscamos en TICKET.BAK
	if( _IMPRIMIENDO_Z_ANT == 1 ){
		if( /*!USED( AREA_AUX )*/ !OPENED( T_EVENTOS, TT_BAK )){
			//USE_DB( AREA_AUX, _TICKET_BAK2,( char* )&event_ticket, sizeof( struct _eventos ), NULL, 0, 0);
			OPEN_TABLE(T_EVENTOS, TT_BAK,( char* )&event_ticket,sizeof( event_ticket ));
			//SELECT( AREA_AUX );
			SELECT_TABLE( T_EVENTOS, TT_BAK );
		}
	}
	//Si no es reimpresion buscamos en TICKET.BTR
	else{
		SELECT_TABLE( T_EVENTOS, tipo_t_eventos );
	}
    /*(_IMPRIMIENDO_Z_ANT) ? SET_ORDER(5) :*/ SET_WHERE( "TIPO_EVENTO = '%i'", tipo_evento);

	if( /*(_IMPRIMIENDO_Z_ANT) ? !GET_EQUAL( ( char* )&tipo_evento ) :*/ !RUN_QUERY(NO) ) {
		while( event_ticket.tipo_evento == tipo_evento && ( i < nro_evento ) ) {
			id_evento = event_ticket.id_evento;
			/*(_IMPRIMIENDO_Z_ANT) ? GET_NEXT(  ) :*/ SKIP2(1);
			i++;
		}
	}


	if( /*USED( AREA_AUX )*/OPENED( T_EVENTOS, TT_BAK ) && _IMPRIMIENDO_Z_ANT ) {
		//CLOSE_DATABASE( AREA_AUX );
		CLOSE_TABLE( T_EVENTOS, TT_BAK );
	}

    return id_evento;
}
/***************************************************************************/
void TKT_total_en_letras( int *ind_config )
/***************************************************************************/
{
    char p_cad[200];
    double total_factura = 0.00;
	/*------------------ Transformacion a letras ------------------*/
   /*Problemas de redondeo con esta función*/
   //total_factura = SUMAR_TOTAL_FAC_CAJA( );

   total_factura = PAGOS_TOTAL_A_PAGAR( );
   NUM_A_LETRAS( total_factura, p_cad );
   MOSTRAR_CADENA( tkt_columna, PRN, p_cad );
}
/***************************************************************************/
void LETRA_IMPUESTO( char cod_impuesto, int iva, double exento, char *letra )
/***************************************************************************/
{
	int tasa = 0, /*area_ant = 0,*/ rta = 0;
	struct _impuestos impuestos_sic;
	//area_ant = SELECTED(  );
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
	if( /*USE_DB( AREA_IMPUESTO, _IMPUESTO_SIC, ( char* )&impuestos_sic, sizeof( impuestos_sic ), NULL, 0,
		0 )*/
		OPEN_TABLE(T_IMPUESTO, TT_ORIG, ( char* )&impuestos_sic, sizeof( impuestos_sic ))== 0 ) {
		SELECT_TABLE( T_IMPUESTO, TT_ORIG );
		SET_WHERE("COD_IMPUESTO = '%i'",cod_impuesto);
		//GO(TOP);
		//SET_ORDER( 1 );
		//GET_EQUAL( ( char* ) &cod_impuesto );
		RUN_QUERY(NO);
		rta = FOUND2();
		if( iva ) {
			if ( rta ) {
				tasa = ( int )impuestos_sic.tasa;
			} else {
				tasa = 0;
			}
			switch( tasa ) {
				case 0:
					if( iva == 2 && !exento ) {
						strcpy( letra, " B" );
					}
					break;
				case 25:
					if( iva == 1 ) {
						strcpy( letra, " I" );
					} else {
						strcpy( letra, " H" );
					}
					break;
				case 30:
					if( iva == 1 ) {
						strcpy( letra, " K" );
					} else {
						strcpy( letra, " J" );
					}
					break;
				case 50:
					if( iva == 1 ) {
						strcpy( letra, " M" );
					} else {
						strcpy( letra, " L" );
					}
					break;
				}
			}
            CLOSE_TABLE( T_IMPUESTO, TT_ORIG );
		}
	//SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
	}
/***************************************************************************/
void  IMPRIME_TKT_DESCRIPCION_CUERPO( int indice )
/***************************************************************************/
{
	/*------------------ Inicializaciones ---------------*/
    tkt_fila = 1;
    tkt_columna = 1;
	if( MODO_NEGOCIO != SERVICIOS && ind_comienzo_descripcion_cuerpo > 0 ) {
        PRN_SELECT( RECEIPT );
        ind_config = ind_comienzo_descripcion_cuerpo;
		while( CONFIG_TKT1[ind_config].codigo != _FIN_DESCRIPCION_CUERPO ) {
          TKT_ejecutar_comando( &ind_config, 0, 0, 0 );

        }
	}
}
/***************************************************************************/
void IMPRIMIR_TKT_FRANQUEO( int modo )
/***************************************************************************/
{

	if( modo != _MODO_COBRANZA ) {
		if( MODO_NEGOCIO != SERVICIOS ) {

			ind_config = ind_comienzo_franqueo;
			while( ind_config && CONFIG_TKT1[ind_config].codigo != _TKT_FIN_FRANQUEO ) {
			TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
			}
		}
	} else {
		ind_config = ind_comienzo_franqueo_cobros;
		while( ind_config && CONFIG_TKT1[ind_config].codigo != _TKT_FIN_FRANQUEO_COBROS ) {
			TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
		}
	}
}

/***************************************************************************/
void IMPRIME_TKT_DESCRIPCION_TARJETA( int indice, int indice2 )
/***************************************************************************/
{
    //int ind_config_ant;
    /*------------------ Inicializaciones ---------------*/
    tkt_fila = 1;
    tkt_columna = 1;
	if( MODO_NEGOCIO != SERVICIOS ) {
        PRN_SELECT( RECEIPT );
        ind_config = ind_comienzo_descripcion_tarjeta;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_DESCRIPCION_TARJETA ) {
          TKT_ejecutar_comando( &ind_config, 0, indice, indice2 );
        }
		LF(2);
	}
}
/***************************************************************************/
void IMPRIMIR_TKT_FIN_TICKET( )
/***************************************************************************/
{

    /*------------------ Inicializaciones ---------------*/
    tkt_fila = 1;
    tkt_columna = 1;

	if( MODO_NEGOCIO != SERVICIOS ) {
        PRN_SELECT( RECEIPT );

        ind_config = ind_comienzo_fin_ticket;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_TICKET ) {
          TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
	}
}
/***************************************************************************/
void IMPRIME_TKT_IMPORTE_TOTAL( int indice )
/***************************************************************************/
{
    //int ind_config_ant;
    /*------------------ Inicializaciones ---------------*/
    tkt_fila = 1;
    tkt_columna = 1;
	if( MODO_NEGOCIO != SERVICIOS ) {
        PRN_SELECT( RECEIPT );
        ind_config = ind_comienzo_importe_total;
        while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_IMPORTE_TOTAL ) {
          TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
	}
}
/***************************************************************************/
void IMPRIME_COTIZACION( int x_loc , int y_loc , int h )
/***************************************************************************/
{
    int cod = 0;
    double cotizacion;
	char cad_aux[50];
    if( medios[_PAGO_MODO( h )].mutual ) {
        cod = MUTUAL_MONEDA_EXTRANJERA( _PAGO_CUENTA( h ) );
    }
    if( cod ) {
        COTIZACION_MUTUAL( _PAGO_CUENTA( h ), &cotizacion, NULL );
            if(IMPRESORA_FISCAL == FISCAL) //Seba
			{
				memset(cad_aux,0, sizeof(cad_aux));
				_snprintf(cad_aux,sizeof(cad_aux)-1, "%.2f",cotizacion);
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cad_aux, NO );
			}
			else
        MOSTRAR_FLOTANTE( x_loc, y_loc, 3, 2, NO, NO, cotizacion );
    } else {
		    if(IMPRESORA_FISCAL == FISCAL) //Seba
			{
				memset(cad_aux,0, sizeof(cad_aux));
				_snprintf(cad_aux,sizeof(cad_aux)-1, "%.2f",medios[_medio_moneda_extranjera].cotizacion);
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cad_aux, NO );
			}
			else
        MOSTRAR_FLOTANTE( x_loc, y_loc, 3, 4, NO, NO,
            medios[_medio_moneda_extranjera].cotizacion );
    }
}
/***************************************************************************/
void IMPRIME_IMPORTE_INGRESADO( int x_loc, int y_loc, int h )
/***************************************************************************/
{
    int cod;
    double cotizacion, importe_ingresado = 0.0;
    char letra[6];
	char cad_aux[50];

	memset(letra, 0,sizeof(letra));
    if( medios[_PAGO_MODO( h )].mutual ) {
        cod = MUTUAL_MONEDA_EXTRANJERA( _PAGO_CUENTA( h ) );
        if( cod ) {
            switch( cod ) {
                case 1:
                    strcpy( letra, " DLS." );
                break;
                case 2:
                    strcpy( letra, " QTZ." );
                break;
                case 3:
                    strcpy( letra, " DLC." );
                break;
            }
            COTIZACION_MUTUAL( _PAGO_CUENTA( h ), &cotizacion, NULL );
            if( cotizacion ) {
                importe_ingresado = _PAGO_IMPORTE( h ) / cotizacion;
            }
            if(IMPRESORA_FISCAL == FISCAL) //Seba
			{
				memset(cad_aux,0, sizeof(cad_aux));
				_snprintf(cad_aux,sizeof(cad_aux)-1, "%.2f %s",importe_ingresado, letra);
				IMPRIMIR_TEXTO_NO_FISCAL(1,TIPO_DE_LETRA,cad_aux, NO );
			}
			else
			{
            MOSTRAR_FLOTANTE( x_loc, y_loc, 3, 2, NO, NO, importe_ingresado );
            IMPRIMIR_CADENA( x_loc, letra );
        }
    }
	}
	if( medios[_PAGO_MODO( h )].moneda_extranjera){
		importe_ingresado = _PAGO_IMPORTE_ME( h );// * medios[_PAGO_MODO( h )].cotizacion;
		strncpy(letra, medios[_PAGO_MODO( h )].simbolo_moneda, sizeof(letra)-1 );
		MOSTRAR_FLOTANTE( x_loc, y_loc, 3, 2, NO, NO, importe_ingresado );
        IMPRIMIR_CADENA( x_loc, letra );
	}

}


/*************************************************************************************/
void IMPRIMIR_INFORME_MEDIOS( )
/*************************************************************************************/
{
	int nro, tipo;
	char cadena[40], medio = 0;
	double monto = 0.00;

	 for( nro = 0; nro < 9; nro++ ) {
		tipo = OBTENER_TIPO_MEDIO( nro );
		switch( tipo ) {
			case _T_COMPRA_SIC:
				RECORRER_SUBMEDIOS( _T_COMPRA_SIC, nro+1, 2 ); //2 cod_medio_padre
				break;
			case _TARJETA_SIC:
				RECORRER_SUBMEDIOS( _TARJETA_SIC, nro+1, 1 );
				break;
			case _MUTUALES_SIC:
				RECORRER_SUBMEDIOS( _MUTUALES_SIC, nro+1, 2 ); //2 cod_medio_padre
				break;
			case 10:
				if( X_TOT_DINERO != 0.00 ){
					sprintf( cadena, "%-15.15s", medios[nro].nombre );
					MOSTRAR_CADENA( 2, PRN, cadena );
					LF( 1 );
					sprintf( cadena, "%-15.15s%7.7s%10.2lf", "TOTAL", " ", X_TOT_DINERO );
					MOSTRAR_CADENA( 4, PRN, cadena );
					LF( 1 );
				}
				break;
			default:
				monto = OBTENER_IMPORTE_SUBMEDIO( nro, 0, medios[nro].modo_arqueo );
				if( monto != 0.00 ){
					sprintf( cadena, "%-15.15s", medios[nro].nombre );
					MOSTRAR_CADENA( 2, PRN, cadena );
					LF( 1 );
					sprintf( cadena, "%-15.15s%7.7s%10.2lf", "TOTAL", " ", monto );
					MOSTRAR_CADENA( 4, PRN, cadena );
					LF( 1 );
				}
				break;
		}
	}
}

/**************************************************************************/
int OBTENER_TIPO_MEDIO( int nro )
/**************************************************************************/
{
	int rta = 0;

	if( medios[nro].mutual ) {
		rta = _MUTUALES_SIC;
	} else if( medios[nro].tarjeta ) {
		rta = _TARJETA_SIC;
	} else if( medios[nro].ticket_compra ) {
		rta = _T_COMPRA_SIC;
	} else if( medios[nro].efectivo ) {
		rta = 10;
	} else {
		rta = 1;
	}
	return rta;
}

/***********************************************************************************/
void RECORRER_SUBMEDIOS( int tabla, char clave, char orden )
/***********************************************************************************/
{

	int    medio/*, seguir*/;
	void *gen_struct;
	double monto = 0.00, total = 0.00;
	/*char codigo;*/
	int tam = 0, pos_cod = 0, pos_submed = 0, pos_nombre = 0, cod_tabla_sql = 0;
    int indice_array_orden = 0;
	/*Orden de las tablas 0: T_COMPRA, 1: T_MUTUALES, 2:TARJETAS*/
	char *order_sql[3][3] = {"COD_T_COMPRA","COD_MEDIO_PADRE","COD_T_COMPRA , COD_MEDIO_PADRE",
							"COD_MUTUAL","COD_MEDIO_PADRE"," ",
			 				"COD_TARJETA","NOMBRE","PREFIJO_COD_BARRA" };

	medio = clave;

	switch( tabla ) {
		case _T_COMPRA_SIC:
			tam = sizeof( struct _t_compra  );
			pos_cod = 100;//offset
			pos_submed = 1;//offset
			pos_nombre = 2;
			cod_tabla_sql = T_COMPRA;
			indice_array_orden = 0;
			break;
		case _MUTUALES_SIC:
			tam = sizeof( struct _t_mutuales );
			pos_cod = 71;
			pos_submed = 1;
			pos_nombre = 2;
			cod_tabla_sql = T_MUTUALES;
			indice_array_orden = 1;
			break;
		case _TARJETA_SIC:
			tam = sizeof( struct _tarjetas );
			pos_cod = 1;
			pos_nombre = 3;
			cod_tabla_sql = T_TARJETAS;
			indice_array_orden = 2;
			break;
		default:
			GRABAR_LOG_SISTEMA("RECORRER_SUBMEDIOS, tabla no definida",LOG_VENTAS,2);
			return;
	}

	gen_struct = MALLOC( tam );

	//if( USE_DB( AREA_AUX3, tabla, ( char* )gen_struct, tam, NULL, 0, 0 ) == 0 ) {
	if( OPEN_TABLE( cod_tabla_sql, TT_ORIG, ( char* )gen_struct, tam ) ){
		int seguir = 1, first = 0;
		//SET_ORDER( orden );
		SET_ORDER2(order_sql[indice_array_orden][orden - 1]);
		SET_WHERE("");
		RUN_QUERY(NO);
		//if( tabla != _TARJETA_SIC ) {
		//	GET_EQUAL( &clave );
		//	seguir = FOUND( );
		//}
		while( seguir && /*!BtrvEOF( )*/ !dbEOF() ) {
			char  cadena[40], *nombre, *submed;
			INT16  *codigo;
			codigo = (INT16*)( (INT16*)(gen_struct) ) + pos_cod;
			if( *codigo == clave  || tabla == _TARJETA_SIC) {
				if( tabla == _TARJETA_SIC ) //no tiene medio padre!
					monto = OBTENER_IMPORTE_SUBMEDIO( (int)clave, *codigo, medios[clave].modo_arqueo );
				else {
					submed = ( ( char* )(gen_struct) ) + pos_submed;
					monto = OBTENER_IMPORTE_SUBMEDIO( (int)clave, *submed, medios[clave].modo_arqueo );
				}
				if( monto != 0.00 ){
					total += monto;
					if( !first ){
						first++;
						sprintf( cadena, "%s ", medios[clave-1].nombre );
						MOSTRAR_CADENA( 2, PRN, cadena );
						LF( 1 );
					}
					nombre = ( ( char* )(gen_struct) ) + pos_nombre;
					sprintf( cadena, "%-15.15s%7.7s%10.2lf", nombre, " ", monto );
					MOSTRAR_CADENA( 4, PRN, cadena );
					LF( 1 );
				}
				SKIP2( 1 );
			} else {
				seguir = 0;
			}
		}
		if( total != 0.00 ) {
			char  cadena[40];
			sprintf( cadena, "%-15.15s%7.7s%10.2lf", "TOTAL", " ", total );
			MOSTRAR_CADENA( 4, PRN, cadena );
			LF( 1 );
		}
		//CLOSE_DATABASE( AREA_AUX3 );
		CLOSE_TABLE( cod_tabla_sql, TT_ORIG );
    }
    if( gen_struct )
	free( gen_struct );
	return;
}

/****************************************************************************/
void IMPRIMIR_TKT_INFORMACION_COMPARATIVA( void )
/****************************************************************************/
{
    if( ind_comienzo_cuerpo_informacion_comp ) {
        ind_config = ind_comienzo_cuerpo_informacion_comp;
        while( CONFIG_TKT1[ind_config].codigo != _TKT_FIN_INFCOMPARATIVA ) {
            TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
        }
    }
}
/***************************************************************************/
void IMPRIME_TKT_DUPLICADO_TARJETA(int indice, int indice2)//Seba
/***************************************************************************/
{
	ind_config = ind_comienzo_duplicado_tarjeta;
    while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_DUPLICADO_TARJETA )
	{
       TKT_ejecutar_comando( &ind_config, 0, indice, indice2 );
    }
}
/***************************************************************************/
void IMPRIME_TKT_RETIRO( int cierre )//Seba
/***************************************************************************/
{
	if( !cierre ) {
		ind_config = ind_comienzo_retiro;
		while( ind_comienzo_retiro && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_RETIRO )
		{
		   TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
		}
	} else {
		ind_config = ind_comienzo_retiro_cierre;
		while( ind_comienzo_retiro_cierre && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_RETIRO_CIERRE )
		{
		   TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
		}
	}
}
/***************************************************************************/
void IMPRIME_TKT_INGRESO_EGRESO_CAJERO()//Seba
/***************************************************************************/
{
	ind_config = ind_comienzo_ing_cajero;
    while( ind_config && CONFIG_TKT1[ind_config].codigo != _FIN_CUERPO_ING_CAJERO )
	{
       TKT_ejecutar_comando( &ind_config, 0, 0, 0 );
    }
}
/***************************************************************************/
//POSICIONAMIENTO PARA EL INFORM y
/***************************************************************************/
int GET_INICIO_INFORME_Y(void )
	{return ind_comienzo_inf_y;} 
int GET_FIN_INFORME_Y(void )
	{return _TKT_FIN_INFO_Y;}
/***************************************************************************/
int GET_POSICION_DEL_FOR( int codigo_subdindice ){
/***************************************************************************/
int rta = 0;
if( codigo_subdindice == 254 && _TKT_FOR_LIMITE > 0 ) 
	rta = ( INT16 )_TKT_FOR_I;

return rta;
}
/******************************************************************************/
void OBTENER_DATO_ARCHIVO_REC( char *ruta,int tam_struct, char *tipores, char offset[200]/*, int linea*/ )
/******************************************************************************/
{
    int codigo_offset, tamanio = 0, dato_es_numerico;
	char *estruc_archivo = NULL;
    char /*estructura[1200],*/tipo,tiporesCh,*token,res_string[250];
    struct _valores_calculo val_calc;
    INT16 inttemporal;
    UINT16 uinttemporal;
    long longtemporal;
    double doubletemporal,resultado;
    float floattemporal;
    unsigned long ulongtemporal;
    char bytetemporal;
	
    if( ( obtenerTipo( tipores, &tiporesCh, NULL ) ) == -1 ) {
        GRABAR_LOG_SISTEMA( "ERROR DATO ARCHIVO: tipo no definido" ,LOG_ODBC,1);
        return;
    }
	estruc_archivo =  MALLOC( tam_struct );
	if( !estruc_archivo ) {
        GRABAR_LOG_SISTEMA( "ERROR DATO ARCHIVO: memoria no disponible",LOG_ODBC,1 );
        return;
    }
//-------------------------
//Se interpreta el indice (tipo1)dato1;(tipo2)dato2;...(tipoN)datoN;
    token = strtok( offset, "()" );
    while( token != NULL ) {
        if( ( obtenerTipo( token, &tipo, &dato_es_numerico ) ) == -1 ) {
            GRABAR_LOG_SISTEMA( "ERROR ARCHIVO: tipo no definido" ,LOG_ODBC,1);
            return;
        }

        token = strtok( NULL, ";" );
        if( token == NULL ) {
            GRABAR_LOG_SISTEMA( "ERROR ARCHIVO: offset" ,LOG_ODBC,1);
            return;
        }

        //Si el valor del indice de busqueda es numerico entonces se
        //interpreta para resolver variables RNV, etc.
        if( dato_es_numerico ) {
            //Dato para calcular variables rnv off
            val_calc.tkt_ini_est = _TKT_INI_EST;
			if( dato_es_numerico == 1 ) {
				resultado = RESOLVER( token, &val_calc );
				sprintf( res_string, "%f", resultado );
			} else {
				resultado = _RESOLVER( token, &val_calc, res_string );
			}			            
        }
        else {
            strcpy( res_string, token );
        }
        token = strtok( NULL, "()" );
    }

//------------------

	//offset del valor pedido en el struct "instanciado" en estructura[]
    codigo_offset = atoi( res_string );
    strcpy( offset, "" );
	if( tam_struct < 1 ||  tam_struct < codigo_offset ) {
        GRABAR_LOG_SISTEMA( "ERROR DATO ARCHIVO: tamaño no definido" ,LOG_ODBC,1);
        return;
    }

	if( estruc_archivo && !RECUPERAR_ARCHIVO_REC( ruta, estruc_archivo, tam_struct, 0 )) {
        {
            //El resultado es devuelto en forma de string en el parametro @offset
            switch( tiporesCh ) {
                case 'i':
                    inttemporal = *( INT16 * )( estruc_archivo + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%d", ( int )inttemporal );
                    #else
                    itoa( ( int )inttemporal, offset, 10 );
                    #endif
                    break;
                case 'l':
                    longtemporal = *( long* )( estruc_archivo + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%ld", longtemporal );
                    #else
                    ltoa( longtemporal, offset, 10 );
                    #endif
                    break;
                case 'd':
                    doubletemporal = *( double* )( estruc_archivo + codigo_offset );
                    sprintf( offset, "%f", doubletemporal );
                    //FTOC( doubletemporal, offset, ENTEROS, DECIMALES, SI, NO );
                    break;
                case 's':
                    strcpy( offset, ( char* )estruc_archivo + codigo_offset );   
                    break;
                case 'c':
                    offset[0] = *( char* )( estruc_archivo + codigo_offset );
                    offset[1] = 0;
                    break;
                case 'f':
                    floattemporal = *( float* )( estruc_archivo + codigo_offset );
                    //FTOC( (double) floattemporal, offset, ENTEROS, DECIMALES, 1, 1 );
                    sprintf( offset, "%f", floattemporal );
                    break;
                case 'u':
                    uinttemporal = *( UINT16 * )( estruc_archivo + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%u", ( unsigned int ) uinttemporal );
                    #else
                    ultoa( ( unsigned long ) uinttemporal, offset, 10 );
                    #endif
                    break;
                case 'n':
                    ulongtemporal = *( unsigned long* )( estruc_archivo + codigo_offset );
                    #if defined(INVEL_L)
                    snprintf( offset, sizeof( offset ), "%lu", ulongtemporal );
                    #else
                    ultoa( ulongtemporal, offset, 10 );
                    #endif
                    break;
				case 'b':
                    bytetemporal = *( char* )( estruc_archivo + codigo_offset );
                    sprintf( offset, "%d", bytetemporal );
					break;
                default:
                    break;
            }
        }
      
    }
    
	if( estruc_archivo ){
		FREEMEM( estruc_archivo );
	}

   
}
/*-------------------------------------------------------------------------*/
void RESOLVER_ASIGNA_NOMBRES( char *formula,  char *cadena )
/*-------------------------------------------------------------------------
 * en los parametros noprint(), trim()
 */

{
	double rta = 0.0;
	int var;
	char source[500],tiene_nombre[500],*ptr=NULL,*p=NULL;
	char *pos1,*pos2,nombre[10];
	
	strcpy( source, cadena);
	
	strcpy( tiene_nombre,formula  );
	//busca el numero de la variable
	ptr = strstr(source, "asignar(");
	if( ptr ){
		if( p = strtok(ptr, "(" ) ){
			if( p = strtok( NULL, "," ) ){
				if( isalpha( (char)p[1])){
					//var =  DEVUELVE_POSICION_NOMBRE( p );
					var = 0;
				}else{
					var = atoi(p);
					rta = 1;
				}
			}
		}
	}

	//busca el nombre de la variable
	
	if( rta ){
		ptr = strstr(tiene_nombre, "nombre(");
		rta = 0.0;	
		if( ptr ){
			if(  pos1 = strchr( ptr, '(' ) ){
				if( pos2 = strchr( pos1, ')' ) ){
					*pos2 = 0;
					strncpy( nombre, &pos1[1],sizeof(nombre));
					if(ASIGNAMES( (double)var, nombre )){
						rta = 1.0;
					}else{
					
					}
				}
			}
		}
	}
		

}
/*-------------------------------------------------------------------------*/
int RESOLVER_VAR_NOMBRES( char *cadena )
/*-------------------------------------------------------------------------
 * 
 */

{
	int rta = 0, n=0,n2=0,opcion=0;
	char source[500],*ptr=NULL,*p=NULL,nombre[10];
	char *pos1,*pos2,*pos3;
	char *lectura[]={"var(","asignar("};
	
	strncpy( source, cadena, sizeof(source));
	do
	{
		//buscamos que sea un pregunta por el valor
		ptr = strstr(source, lectura[opcion]);
	//	ptr = strstr(source, "var(");
		
		if( ptr ){
			pos1 = strchr( ptr, '(' );
			if(pos1){
				pos2 = strchr( pos1, ')' );
				//evaluamos que sea alfabetica la primer posicion
				if( pos1 && pos2 && ( isalpha( (char)pos1[1]))){
					*pos2 = 0;
					
					//"pepe"   /  "p"

					
					if(pos3 = strchr( pos1, ',' ))
						*pos3 = 0;
					strncpy( nombre, &pos1[1],sizeof(nombre));
					//la posicion  del nombre es la posicion del valor en el vector 
					rta =  DEVUELVE_POSICION_NOMBRE( nombre );
					if( rta < 0 )
						rta = 0;
					else{	
						
						ptr = strstr(cadena, lectura[opcion]);
					//	ptr = strstr(cadena, "var(");
						if(ptr){
							pos1 = strchr( ptr, '(' );
							if(pos1){
								n=pos1-ptr;
								if(opcion){
									pos2 = strchr( pos1, ',' );
									pos3 = strchr( pos2, ')' );
									*pos3=0;
								}else
									pos2 = strchr( pos1, ')' );
								
								if(pos2){
									n2=pos2-pos1 + n;
									if( opcion ){
										sprintf( cadena, "asignar(%0*i%s)",(int)(n2 - n - 1),rta,&pos2[0]);  
									}else
										sprintf( cadena, "var(%0*i)",(int)(n2 - n - 1),rta); 
								}
							}
						}
					}
                }
			}
		}
	opcion++;
	}
	while(!rta && opcion < 2 );
	return( rta );
}/*-------------------------------------------------------------------------*/
 int DEVUELVE_POSICION_NOMBRE( char* nombreVariable )
/*-------------------------------------------------------------------------*/
{
    int index = 0;
	int valor = -1;
    for(index=0;index < MAX_VAR && strcmp( nombreVariable, nombre_variable[index])!=0;index++ );

    if( index < MAX_VAR ) {
		nombre_variable[index];
		valor = index;
    }
    return valor;
}
 /*-------------------------------------------------------------------------*/
int ASIGNAMES( double nombre, char* nombreVariable )
/*-------------------------------------------------------------------------*/
{
    int index = 0;
	index = (int) nombre ;
    if( index < MAX_VAR && nombreVariable != NULL && (strlen(nombreVariable) >= 3 ) ) {//ojo no admito variable de menos de 3 caracteres..
		_snprintf( nombre_variable[index], 10, "%s", nombreVariable );  
		return	SI;
    }
	return NO;
}
 /*-------------------------------------------------------------------------*/
int DEVUELVE_LINEA_DESCRIPCION(char * texto, int linea, int tamanio ){
/*-------------------------------------------------------------------------*/
	int lon_devolver=0, rta = NO;

	if( strlen( linea_descripcion[linea] )>tamanio)
		lon_devolver =tamanio;
	else
		lon_devolver = strlen(linea_descripcion[linea] );
	if( strlen( linea_descripcion[linea] )){
		_snprintf( texto, lon_devolver, "%s", linea_descripcion[linea] );  
		rta = SI;
	}
	return rta;
}
 /*-------------------------------------------------------------------------*/
int TRADUCE_LINEA_CON_VARIABLES(char * cadaux ,char *cadaux3,struct _valores_calculo val_calc){
/*-------------------------------------------------------------------------*/
char *ptr,*ptr2;
double resultado=0.0;
int procesa =1;
do{
				
				int aux1=0,aux2=0;
				ptr = strstr(cadaux, "({" );
				memset(cadaux3,0,sizeof(cadaux3));
				/*revision de funciones o variables*/
				if( ptr ){
					//detecto variable de RNV Ej.[({#294})];
					memset( cadaux3, 0, sizeof( cadaux3 ) );
					strcpy(cadaux3,cadaux);
					//ESTE USA  "({" 
					ARMAR_MENSAJE( cadaux3, cadaux);
				}else{
					ptr = strstr(cadaux, "var(" );
					if( ptr ){
						//detecto variables de operacion  Ej.[var(4)];
						ptr2 = strstr(ptr, ")" );
						
						/*resultado de las numericas siempre el double*/
						resultado = RESOLVER( ptr, &val_calc );
						
						{//veamos si tiene resto o es entero solamente para controlar el largo de impresion
							double fraction=0.0, integer=0;
							
							fraction = modf(resultado, &integer);
						
							if( fraction!=0.0 )
								sprintf(cadaux3,"%*.*lf",ENTEROS, DECIMALES,resultado);						
							else
								sprintf(cadaux3,"%*.0lf",ENTEROS,resultado);

							TRIM(cadaux3);
							LTRIM(cadaux3);

							aux1=strlen(cadaux3) ;
							aux2=strlen(ptr) ;

							if( strlen(cadaux3) > strlen(ptr)){
								memset(ptr,0x20,strlen(ptr));
								strncpy(ptr,cadaux3,(strlen(ptr)-1));
							}else{
								//vacio el espacio que contenia la variable.
								aux2=(ptr2-ptr)+1;
								memset(ptr,0x20,(ptr2-ptr)+1);
								//Copio el Valor el espacio que contenia la variable.
								strncpy(ptr,cadaux3,(strlen(cadaux3)));
							}
						}

					}else{
//						strcat(cadaux3,cadaux);
						procesa = 0;
					}
				}
}while(procesa);

return !procesa;
}


