#pragma pack (push, 1)
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#ifndef CR_H
#define CR_H
#ifdef __cplusplus
extern    "C" {
#endif
#pragma pack(1)
#include <stringt.h>
#include <config.h>
#include <_cr_var.h>
#include <ddtpv.h>
#include "mstring.h"
#include "getch.h"
#include "print.h"
#include "string.h"
#include "stdio.h"
#include "memory.h"
#include <btrv.h>
#include <eventos.h>
//#include <cr_disco.h>


	//bam
	int mtime();

/*****************************************************************************/
	/* DEFINICIONES                                                              */
/*****************************************************************************/


/*------- Opciones de t�ulos del par�etro A_PEDIR -------------------*/
#define _SIMPLE        1
#define _DOBLE         2
#define _CONCATENADO  21
#define _HIBRIDO      31
#define _MOVIL        41
#define _AJUSTABLE    51

/*--------------------- Generales -----------------------*/
#define SI    1
#define NO    0
#define SUGERIR 2
#define ANULACION 3


#define OFF		0
#define ON		1
#define ON_OFF  2

#define PESOS    0
#define DOLARES  1
#define CECOR    2

#define PRN 100

#define   LETRA_NORMAL						240
#define   LETRA_RESALTADA					241
#define   LETRA_DOBLE_ALTO					242
#define   LETRA_DOBLE_ALTO_NEGRITA			243
#define   LETRA_DOBLE_ANCHO					244
#define   LETRA_DOBLE_ANCHO_NEGRITA			245
#define   LETRA_DOBLE_ANCHO_ALTO			246
#define   LETRA_DOBLE_ANCHO_ALTO_NEGRITA	247
#define   LETRA_SUBRAYADO					248
#define   LETRA_SUBRAYADO_NEGRITA			249


/*--------------------- Paises   -----------------------*/
#define ARGENTINA     0
#define CHILE         1
#define CUBA          2
#define COLOMBIA      3
#define PARAGUAY      4
#define ECUADOR       5
#define ELSALVADOR    6
#define BOLIVIA       7
#define BRASIL 	      8
#define MEXICO	     11

/*--------------------- Tiempos -----------------------*/
#define _INDEFINIDO		0
#define _ENTRADA_OCIO	1
#define _ENTRADA_REG    2
#define _ENTRADA_SBY    3
#define _ENTRADA_PAGO   4
#define _ENTRADA_NO_VENTA 5

/*--------------------- Tarjetas -----------------------*/
#define _CABAL_VIEJA        5
	/*
	 * #define _ARGENCARD          0
	 * #define _MASTERCARD         1
	 * #define _CARTA_FRANCA       2
	 * #define _CREDENCIAL_VIEJA   3
	 * #define _NARANJA            4
	 * #define _CABAL_VIEJA        5
	 * #define _VISA               6
	 * #define _DINERS             7
	 * #define _BANELCO            8
	 * #define _AMERICAN           9
	 * #define _MEGANET           10
	 * #define _PROVENCRED        11
	 * #define _LIDER             12
	 * #define _KADICARD          13
	 * #define _SOCIAL            14
	 * #define _CABAL_NUEVA       15
	 * #define _CREDENCIAL_NUEVA  16
	 * #define _PLUSCARD          17
	 * #define _MASTERCARD_INT    18
	 * #define _CREDENCIAL_LIM_0  19
	 * #define _PROVENCRED_NUEVA  20
	 * #define _BISEL             21
	 * #define _VALE_03200        22
	 * #define _VALE_7130         23
	 * #define _VALE_279900       10
	 * #define _MAESTRO           24
	 * #define _CMR               25
	 */
#define _AFINIDAD          99

#define _ISO7812       1
#define _MODULO9713    2
#define _MODULO2121    3
#define _MODULO4326    4
#define _MODULO11      5
#define _MODULO1397    6

#define SCANNER_INVEL      1
#define SCANNER_SPECTRA    1
#define SCANNER_METROLOGIC 1
#define SCANNER_SPAR       2
#define SCANNER_SYMBOL     3
#define SCANNER_TEC        4
#define SCANNER_DRIVER     5

#define TM930                  1
#define IMPRESORA_INVEL        2
#define IMPRESORA_DRIVER       3
#define IMPRESORA_OMRON        4

#define FISCAL_DITEMPO         1
#define FISCAL_EPSON           2
#define FISCAL_IBM_4614        3
#define FISCAL_IBM_4694        4
#define FISCAL_POS745x         5
#define FISCAL_IBM_4610        6
#define FISCAL_TMT88           7
#define FISCAL                 8

#define _TEC_3500   1
#define _TEC_5000   2
#define _TEC_5500   3

#define CAJON_INVEL            0
#define CAJON_OMRON            1
#define CAJON_TEC              2
#define CAJON_EPSON950         3
#define CAJON_DRIVER           4

#define LECTOR_BANDA_NINGUNO   0
#define LECTOR_BANDA_OMRON     1
#define LECTOR_BANDA_TEC       2
#define LECTOR_BANDA_TECLADO   3
#define LECTOR_BANDA_DRIVER    4
#define LECTOR_BANDA_INVEL     5

#define DISPLAY_INVEL          0
#define DISPLAY_OMRON          1
#define DISPLAY_TEC            2
#define DISPLAY_DRIVER         3

#define LLAVE_OMRON            1
#define LLAVE_DRIVER           2
#define LLAVE_KB78             3
#define LLAVE_CARY             4

#define TECLADO_PC             0
#define TEC_OM                 1
#define TEC_MIX                2
#define TECLADO_KB78           3
#define TECLADO_PKBST_50       4
#define TEC_3500               5
#define TECLADO_DRIVER         6
#define TECLADO_CARY           7


#define RNV_INVEL              0
#define RNV_INVEL_128          1
#define RNV_TEC_5000           2
#define RNV_DRIVER             3

#define VERSION_1              1
#define VERSION_2              2

#define BALANZA_INVEL          0
#define BALANZA_KRETZ          1
#define BALANZA_OMRON          2
#define BALANZA_DRIVER         3
#define BALANZA_TOLEDO         4

#define APAGADO_POR_TORRETA    1
#define APAGADO_POR_FLASH      2

#define JOURNAL         1
#define RECEIPT         2
#define JOURNAL_RECEIPT 3
#define SLIP            4
#define JOURNAL_PART    31
#define RECEIPT_PART    32

#define OPERACION       0
#define ENTRENAMIENTO   1
#define INVENTARIO      2
#define ETIQUETAS       3
#define TRANSFERENCIAS  4
#define CAMBIO_MEDIO	5
#define COBRO			6

#define LIBERTAD        0
#define SPAR            1
#define COOPERATIVA     2
#define AMERICANOS      3
#define TODO_PLAST      4

#define _ON_LINE_COMPRA                 0
#define _ON_LINE_DEVOLUCION             1
#define _ON_LINE_ANULACION              2
#define _OFF_LINE_COMPRA                3
#define _OFF_LINE_DEVOLUCION            4
#define _OFF_LINE_ANULACION             5
#define _CIERRE_DE_LOTE                 6
#define _ON_LINE_SALDO                  7
#define _ON_LINE_COBRO                 10
#define _OFF_LINE_COBRO                11
#define _ON_LINE_ANUL_COBRO            12
#define _OFF_LINE_ANUL_COBRO           13
#define _ON_LINE_CONSULTA_DEUDA        14
#define _OPERACION_ANULADA            100
#define _ON_LINE_ANULACION_DEVOLUCION  15
#define _OFF_LINE_ANULACION_DEVOLUCION 16
#define _ON_LINE_ANULACION_COBRO       17
#define _OFF_LINE_ANULACION_COBRO      18
#define _REVERSO                       19
#define _ON_LINE_CONSULTA_CLIENTE      20
#define _ON_LINE_ACTUALIZA_WK          21
#define _ON_LINE_CASHPLUS	           22
#define _ON_LINE_ANULACION_CASHPLUS    23
#define _ON_LINE_ECHO_TEST			   24

	//#define LOCAL   1
#define REMOTA  0

#define REMOTO          0
#define LOCAL_FAC       1
#define ALEATORIO       2

/*------------- Ingreso Cajero - USAR_LEGAJO -----------*/
#define CODIGO          0
#define LEGAJO          1
#define RUT             2

/*--------------------- Busqueda -----------------------*/
#define BUSQUEDA_CODIGO_INTERNO         0
#define BUSQUEDA_COD_INT_Y_COD_BARRA    1

/*--------------- Condiciones ante el IVA ------------*/
#define CONTRIBUYENTE           1 //RESP_INSC
#define RESP_INSC				1
#define RESP_NO_INSC  2
#define NO_RESP       3
#define EXENTO        4
#define CONS_FINAL    5
#define MONOTRIBUTO   6
#define RESP_NO_IDEN  7
#define LIBERADO                8
#define EXTERIOR                9
#define FACTURAS DE COMPRA      10
#define DIPLOMATICO_EXPORTACION 11
#define INST_GUBERNAMENTAL      12

/*------------- Tipo de Documento ----------------*/

#define _DNI    	1
#define CUIL		2
#define __CUIT		3
#define CEDULA		4
#define PASAPORTE	5
#define LIB_CIVICA	6
#define LIB_ENROLA	7

/*-----Reimpresion de Comprobantes-----*/
#define ULTIMO       1
#define SELECCIONAR  2

/*--------Modalidades TOMRA----------*/
#define _NORMAL       0
#define _POR_ARCHIVO  1
#define _POR_TIPO     2

/*--------Operacion Ingreso Tarjeta--------------*/
#define VENTAS             1
#define CAMB_MEDIOS        2
#define IDEN_CLIENTE       4
#define CONSULTA_SALDO     8
#define ABONO_TARJETA     16

#define BANDA_MAGENETICA    1
#define CODIGO_BARRA        2

/*---------------- Definicion de Area -----------------*/
#define AREA_AUX               1
#define AREA_TICKET            2
#define AREA_E_TICKET          3
#define AREA_PAGO              4
#define AREA_CLIENTES          5
#define AREA_FAC1              6
#define AREA_FAC2              7
#define AREA_ARTICULOS         8
#define AREA_ACU_VTA1          9
#define AREA_ACU_VTA2         10
#define AREA_ACU_VTA3         11
#define AREA_PRESENT          12
#define AREA_AUX2             13
#define AREA_PERFILES         14
#define AREA_AUX3             15
#define AREA_VARIOS           16
#define AREA_ART_DAT          17
#define AREA_AUX4             18
#define AREA_TICKET_ALL       19
#define AREA_E_TICKET_ALL     20
#define AREA_PAGO_ALL         21
#define AREA_VARIOS_ALL       22
#define AREA_SCRIPT           23
#define AREA_TRANSAC          24
#define AREA_PERF_DES         25
#define AREA_ART_PERF         26
#define AREA_CLAS_PER         27
#define AREA_PRO_CLAS         28
#define AREA_AUX5             29
#define AREA_TAR_MED          30
#define AREA_ARTICULOS_REMOTO 31
#define AREA_VTA_CLAS         32
#define AREA_SCRIPT_RTA       33
#define AREA_VTA_MED          34
#define AREA_CHEQUE_CLIENTE   35
#define AREA_STRING_TABLE     36
#define AREA_VTA_COB          37
#define AREA_TRANSAC2         38
#define AREA_CTRLTRAN         39
#define AREA_PANEL            41
#define AREA_BOTON  	      	42
#define AREA_COD_BARRA	 	    43
#define AREA_CIERRE            44
#define AREA_TICKET_HIST       45
#define AREA_E_TICKET_HIST     46
#define AREA_PAGO_HIST         47
#define AREA_COD_BARRA_HIST    48
#define AREA_CIERRE_HIST       49
#define AREA_COBROS            50
#define AREA_TEMPLATE          51
#define AREA_BOLETIN           52
#define AREA_AUX6              53
#define AREA_ENVASES           54
#define AREA_AUX_ENV           55
//#define AREA_DPAGO             19
//#define AREA_DPAGO_HIST        20
#define AREA_SUPER             21
#define AREA_SUPER_HIST        22
#define AREA_TRANS              6
#define AREA_TRANS2             7
#define AREA_BOLCH             37
#define AREA_BANCO             39
#define AREA_E_DES_PRO         56
#define AREA_E_DESPRO_HIST     57
#define AREA_AUX7              58
#define AREA_AUX8              59
#define AREA_FACTURA           60
#define AREA_FACTURA_HIST      61
#define AREA_DPAGO             62
#define AREA_DPAGO_HIST        63
#define AREA_GRUPO_ENV         64
#define AREA_NRO_CLI           65
#define AREA_ENTRY             66
#define AREA_CONDICIONES_IVA   67
#define AREA_PERF_DESC         68
#define AREA_OPER_CLI          69   /*SWISS MEDICAL*/
#define AREA_MASCARA           70
#define AREA_TIPO_MOV          71
#define AREA_PAGO1             72
#define AREA_PAGO2             73
#define AREA_AUX9              74
#define AREA_SOCIOS            75
#define AREA_MOTIVO2           76
#define AREA_T_COMPRA          77
#define AREA_TKT_CONSULTA      78
#define AREA_MEDREINT          79
#define AREA_CAJA_SUC          80
#define AREA_CIE_LOTE          81
#define AREA_TIPO_MOV_CAJA     82
#define AREA_SUCURSALES        83
#define AREA_DBROUTER          84
#define AREA_TEMPORAL          85
#define AREA_IMPUESTO          86
#define AREA_MODIF_ART         87
#define AREA_COB_TAR           88
#define AREA_IVA               89
#define AREA_VUELTOS           88
#define AREA_CAJEROS           89
#define AREA_EV_B_PROMO        90
#define AREA_EV_B_PROMO_HIST   91
#define AREA_LIN_MENS          92
#define AREA_GARANTIA          93
#define AREA_GRUPO_ART         94
#define AREA_TICK_TMP          95
#define AREA_COB_MED           96
#define AREA_DESCR			   97
#define	AREA_AUX_MED		   98
#define	AREA_CAJ_FLOTANTE      99
#define	AREA_CLASIF1	      100
#define AREA_AUX10             AREA_OPER_CLI /*reuso area*/

/*------------------ Funcion BROWSE --------------------*/
#define _BUSCAR    1
#define _MOSTRAR   2
#define _RESALTAR  3
#define _SKIP      4
#define _VALIDAR   5
#define _EXTREMO   6
#define _COMPARAR  7
#define _ESPECIAL  8
#define _ENTER     9
#define _VACIO     10
#define _TECLA     11
#define _SPACE_    12
#define _LIMIT     13
#define _SET_REGISTROS 14
#define _SET_MODO      15

/*--------------------- Para GO -----------------------*/
#define TOP                   -1
#define BOTTOM                -2

/*---------------- Modos de ingresar a la rutina de pagos -------------------*/
#define _MODO_VENTA                0
#define _MODO_REPROCESAMIENTO      1
#define _MODO_COBRANZA             2
#define _MODO_FAC_REM              3
#define _MODO_NC                   4
#define _MODO_ANULACION            5
#define _MODO_REPROCESAMIENTO_ESP  6
#define _MODO_CAMBIO_MEDIO         7

/*------------- Modos de ingresar a la rutina de promociones ----------------*/
#define _PRE_PAGO              0
#define _POS_PAGO              1
#define _PIE_TICKET            2

/*---------------------- Tipo fecha vigencia en promo.sic  ------------------*/
#define _VIGENCIA_FECHA        1
#define _VIGENCIA_DIAS         2
#define _VIGENCIA_MESES        3

/*---------------------- Momento de recargo de tarjetas ---------------------*/
#define AL_FINAL               0
#define AL_RENGLON             1
#define SIN_RECARGO            2

/*---------------- Tipos de variaciones -----------------*/
#define SOBRE_IMPORTE          0
#define SOBRE_TOTAL            1

/*---------------- Campo 'tipo' en la estructura del ticket -----------------*/
#define _ENVASE                1
#define _DEV_ENVASE            2
#define _ENGANCHADO            4
#define _RESTA                 8
#define _GRAVADO              16
#define _CUPON_DEV_ENV        32
#define _CREDITO              64
#define _SCANNER             128

/*----------------- Origenes del articulo del ticket ------------------------*/
#define _ORIGEN_TICKET          1
#define _ORIGEN_DESC_PESOS      2
#define _ORIGEN_DESC_PORC       3
#define _ORIGEN_PROMOCION       4
#define _ORIGEN_PROMOCION_DESC  4  //FR
#define _ORIGEN_VARIACION_MEDIO 5
#define _ORIGEN_INTERES         6
#define _ORIGEN_PERFIL          7
#define _ORIGEN_CUS             8
#define _ORIGEN_PENDIENTE       9
#define _ORIGEN_DESC_PORC_FIJO  10
#define _ORIGEN_BOLETIN_D_ADIC  11
#define _ORIGEN_DESC_TARJETA    12
#define _ORIGEN_DIF_PRECIO      22


/*---------------- Campo 'tipo2' en la estructura del ticket -----------------*/
#define _STOCKEABLE            1
#define _ACUMULAR_PRO_ACU      2
#define _ACUMULADO_PRO_ACU     4
#define _HABILITADO_CUOTAS     8
#define _CORRECCION            16
#define _PEDIR_PRECIO          32
#define _ART_EXCEPTUADO_ENVIO_A_DOMICILIO  64
#define _HABILITADO_PROMO      128

/*---------------- Campo 'tipo3' en la estructura del ticket -----------------*/
#define _PESABLE               1
#define _PROMOCION             2
#define _RECALCULAR_CANTIDAD   4
#define _GARANTIA_EXTENDIDA    8

/*---------------- Tipos de Busqueda de Cliente -----------------*/
#define _BUSQUEDA_NOMBRE        1
#define _BUSQUEDA_DNI           2
#define _BUSQUEDA_CUIT          4
#define _BUSQUEDA_CODIGO        8
#define _BUSQUEDA_BARRA        16
#define _BUSQUEDA_GENERICO     32
/*---------------- Instancias Insert Tablas -----------------*/
#define _INSERT_EV_CONT        1
#define _INSERT_EV_MEDIOS      2
#define _INSERT_EV_CIERRE      4
#define _INSERT_EV_BPROMO      8
#define _INSERT_EV_DMEDIOS     16
#define _INSERT_EV_SUPER       32
#define _INSERT_PRE_EVENTO     64
#define _INSERT_POS_EVENTO     128

/*---------------- Impresiones en ticket -----------------*/
#define _COMPROBANTE_PRE_CERRADO		1
#define _COMPROBANTE_POS_CERRADO		2
#define _DONACION_IMPRESA				3

/*---------------------------- Para encriptacin ----------------------------*/
#define __CADENA_FISCAL_X      1
#define __CADENA_FISCAL_B      2
#define __CADENA_FISCAL_Z      3
#define __CADENA_FISCAL_T1     4
#define __CADENA_FISCAL_T2     5
#define __CONVERSOR_HORA       6
#define __CONVERSOR_FECHA1     7
#define __CONVERSOR_FECHA2     8
#define __SEGMENTO_VGA         9
#define __CADENA_FISCAL_V     10

/*----------------------- Tipo de usuario para clave ------------------------*/
#define CUALQUIERA    0
#define CAJERO        1
#define SUPERVISOR    2
#define ENTRENAM      3
#define VENDEDOR      4
#define GERENTE       5
#define SUPERCAJERO   6

/*------------------------------ Tipo de tarjeta ----------------------------*/
#define PCOS          0
#define GP416         1
#define CONNAC        2

/*------------------------------- Tipo de clave -----------------------------*/
#define CL_TARJETA    0
#define CL_1903       1
#define CL_MANUAL     2

/*-------------------- Tipo de identificacion de usuarios -------------------*/
#define ID_X_RANGO    0
#define ID_X_TIPO     1

/*------------------------ Tipo de comprobante ------------------------*/
#define _FACTURA        0
#define _NOTA_CREDITO   1
#define _REMITO         2
#define _FAC_REM        3
#define _TKT            4
#define _TICKET         5
#define _TICKET_FACTURA 6
#define _CREDITO_FISCAL 7
#define _FACTURA_CONS_F 8
/*------------------------ Origen de la Nota de Credito------------------------*/
#define NC_ORIGEN_TICKET		1
#define NC_ORIGEN_FACT_A		2
#define NC_ORIGEN_FACT_B		3
#define NC_ORIGEN_TF_A          4
#define NC_ORIGEN_TF_B          5

/*------------------------ Flags de TRANSFER.BIN ----------------------*/
#define _FLAG_NOVIOS   1

/*--------------------------- Posicin vuelto -------------------------*/
#define _AL_FINAL      0
#define _DESPUES_CTDO  1

/*--------------------- Modalidades de Preempaque ---------------------*/
#define PREEMPAQUE_CODIGO_INTERNO  0
#define PREEMPAQUE_CODIGO_BARRA    1

/*----------------------------- eventos -------------------------------*/
#define EVENTO_CIERRE 			 8
#define EVENTO_Y 			 9
#define EVENTO_ZETA 			10
#define EVENTO_COBRO_TARJETA   		22
#define EVENTO_ANULA_COBRO_TARJETA   	23
#define EVENTO_TICKET_EN_ESPERA      	24
#define EVENTO_COBRO_UNIPAGO         	25
#define EVENTO_ANULA_COBRO_UNIPAGO   	26
#define EVENTO_ANULADO   		99
#define _EVENT_ANULA_COMPROBANTE            12
#define _EVENT_REIMPRESION_COMPROBANTE      37
#define _EVENT_INICIO_CAJERO					  26

/*------------------- MOTIVO DESCUENTOS DISCRIMINADOS -------------------*/
#define MOTIVO_DESC_MANUAL_PESOS  1
#define MOTIVO_DESC_MANUAL_PORC   2
#define MOTIVO_DESC_PROMOCION     3
#define MOTIVO_DESC_PERFIL        4
#define MOTIVO_REC_DESDE_PERFIL   5
#define MOTIVO_DESC_PROMO_GENERAL 6
#define MOTIVO_CONCEPTOS_SIN_DTOS 7
#define MOTIVO_DESC_BOLETIN_ART   8

/*---------------------------- Protocolos -----------------------------*/
#define _TCP_IP                 1
#define SIZE_BUFFER_TCP      6144

#define _CANAL_DBROUTER			0
#define _CANAL_AUTORIZADOR		0
#define _CANAL_TARJETAS			1
#define _CANAL_VAL_TELE			2
#define _CANAL_JGATEWAY         3
#define _CANAL_TOUCH_SCREEN     4
#define _CANAL_FTP_1            5
#define _CANAL_FTP_2            6

#define _FORMATO_MENSAJE_INVEL       0
#define _FORMATO_MENSAJE_SWITCH_NCR  1

#define _TEF_AUTON 			0
#define _TEF_SWITCH_NCR		1

//---- Destinos de cupones
#define DESTINO_CUPON_PRINTER    0
#define DESTINO_CUPON_MONITOR    1

/*---------------------------- MODALIDAD_CANAL_TARJETAS ---------------*/
#define _POR_TRANSACCION  0
#define _POR_GRUPO        1
#define _PERMANENTE       2

#define ALGORITMO_RSA   1

/*---------------------------- busquedas de cheques en caso de retiros ---------------*/
#define _BUSCAR_EN_PAGO_BTR         0
#define _BUSCAR_EN_DPAGO_BTR        1
#define _BUSCAR_EN_PAGO_BTR_ESP     2

/*-----------------------------Tipos de tarjetas (mapa de bits)-----------------------*/
#define _TIPO_CASH_ADVANCE          1

/*-----------------------------Origen para importación--------------------------------*/
#define _ORIGEN_DEVOLUCION                 0
#define _ORIGEN_TICKET_EN_ESPERA           1
#define _ORIGEN_CAJERO_FLOTANTE			   2

/*-----------------------------Origen Cadena - Validar Caracteres Especiales--------------------------------*/
#define _INGRESAR_CADENA  0
#define _IMPRIMIR_CADENA  1

/*-----------------------------Tipos de cambio--------------------------------*/
#define ENTRAN_MEDIOS 0
#define SALEN_MEDIOS  1

/*------------------------ Impresiones Especiales -----------------------------*/
#define IMPRIMIR_CUENTA  1

/*-------------- Autorizacion tarjetas en notas de Credito --------------------*/
#define _ONLINE         0
#define _OFFLINE        1
#define _NO_AUTORIZAR   2

/*------------------ Tipo procesamiento ticket en espera ----------------------*/
#define _NO_APLICA       0
#define _TE_TRADICIONAL  1
#define _TE_CON_MENU     2
#define _DATOS_CON_MENU  3

/*----------------------------- Modo ingreso Cobros -------------------------*/
#define _MODO_INTERACTIVO 0
#define _MODO_AUTOMATICO  1

/*----------------------------- Tipos ---------------------------------------*/

typedef struct {
	unsigned char rojo;
	unsigned char verde;
	unsigned char azul;
} DAC;

#pragma pack (push,8)
struct _cabmenu{
  	char *nombre;
  	int  align;
  	int  width;
};
#pragma pack (pop)

struct _CodigoBarra
{
   char     codigo[100];
   int      longitud;
};

struct DatosCobro
{
   struct   _CodigoBarra CodigoBarra;

   long     _dato;
   long     _codigo;
   long     _nroCliente;
   long     _id_cliente;
   int		_TipoCli;
   long     _Compania;
   long     _Comprobante;
   double   _Importe;
   long     _IdDeuda;
   int      _motivo;
   int      _posicion; //posicion de grabado en la e_ticket
   long     _recibo;   //numero de recibo para imprimir
   int      _tipo_mov; // tipo de movimiento (NC, NR etc.)
   char     _letra_comp;
   int      _punto_venta;

};
struct ImpresionDatosAdicionales
{
    char nro_tarjeta[31];
    char autorizacion[31];
    char fecha[31];
    char nro_cuenta[31];
};

//Movida a import_t.h
 /*struct _comprob_importados
 {
    long id_evento;
    long caja_z;
    INT16 caja;
    long secuencia;
    UINT16 fecha;
    int tipo_dev;
 };*/

/*------------------- Definiciones generales para recupero ------------------*/
#define _ESTADO          GET_MEMORY_INT( _estado )
#define _SET_ESTADO( x ) SET_MEMORY_INT( _estado, x )


#define _DM_SET  0
#define _DM_GET  1
#define _DM_CER  2
#define _DM_SIZE 3


/*definicion de nombres de archivos para el recupero*/
//Estas definiciones fueron movidas a PATH.h
//#define ARCH_REC_COBROS_ESP "REC/cobros_esp.rec"
//#define ARCH_REC_SUBMEDIOS "REC/submedios.rec"


/*------------------- Definiciones generales para recupero de la z ----------*/
#define _OK                 GET_MEMORY_INT( _ok )
#define _SET_ESTADO_OK( x ) SET_MEMORY_INT( _ok, x )

#define _SALIR                 GET_MEMORY_INT( _salir )
#define _SET_ESTADO_SALIR( x ) SET_MEMORY_INT( _salir, x )

#define _MENU_ELEGIDO          GET_MEMORY_INT( _menu_elegido )
#define _SET_EST_MENU_ELEGIDO( x ) SET_MEMORY_INT( _menu_elegido, x )

#define _ESTADO_DOS          GET_MEMORY_INT( _estado_dos )
#define _SET_ESTADO_DOS( x ) SET_MEMORY_INT( _estado_dos, x )
#define _ESTADO_Z          GET_MEMORY_INT( _estado_z )
#define _SET_ESTADO_Z( x ) SET_MEMORY_INT( _estado_z, x )
#define _ESTADO_CIERRE            GET_MEMORY_INT( _estado_cierre )
#define _SET_ESTADO_CIERRE( x )   SET_MEMORY_INT( _estado_cierre, x )


/*------------------------ Backup de eventos --------------------------*/
#define SEMANAL        0
#define MENSUAL        1

/*------------------------ Lector de Cheque --------------------------*/
#define NINGUNO        0
#define LECTOR_IBM     1
#define LECTOR_NCR     2

/*------------------------ Orientacion IBM 4610 -----------------------*/
#define _PORTRAIT       1
#define _LANDSCAPE      4

/*------------------------ Tipo validaciones --------------------------*/
#define _VALMEDIOS   1
#define _VALPAGOS    2
#define _VALDEVOLUC  3
#define _VALCOBROS   4
#define _VALIMPCHEQ  5
#define _VALPROMO    6
#define _VALVOUCHER  7
#define _VALAFINID   8
#define _VALDGI      9
#define _VALCLICHEQ  10
#define _VALDONA     11
#define _VALFACREM   12
#define _VALREIMPFAC 13
#define _VALENVASES  14
#define _VAL_T_ESPER 15
#define _VALCAJERO   16
#define _VALCAMBIOMEDIO   17
#define _VAL_COMPROBANTES 99

#define _CIERRE_DE_LOTE_EN_Z  	 	0
#define _CIERRE_DE_LOTE_MANUAL  		1

//ACTIONS
/*--------------------- Impresora Fiscal----------------*/
#define DEBE_ANULAR_TICKET 3

/*--------------------- comprobantes -------------------*/
#define _FONDO_FIJO         0
#define _ADELANTOS          1
#define _DEVOL              2
#define _NOTA_CRED          3

/*--------------------- Cheques en boletin  ------------*/
#define _NO_EN_BOLETIN      0
#define _EN_BOLETIN         1
#define _ERROR_BOLETIN      2
#define _NO_USAR_BOLETIN    3

/*--------------------- Printer estacion  ------------*/
#define _RECEIPT_STATION      0
#define _SLIP_STATION         1

#define  ALIGN_NONE 0
#define  ALIGN_LEFT 1
#define  ALIGN_CENTER 2
#define  ALIGN_RIGHT 3



#define  _CONSULTA_COBRO            1
#define  _CONSULTA_ART_CODBARRA     2
#define  _CONSULTA_ART_DESCRIPC1    3
#define  _CONSULTA_ART_DESCRIPC2    4
#define  _CONSULTA_ART_DESCRIPC3    5
#define  _CONSULTA_ART_DESCRIPC4    6
#define  _CONSULTA_ART_DESCRIPC5    7
#define  _CONSULTA_MOTIVO           8
#define  _CONSULTA_MUTUALES         9
#define  _CONSULTA_PIDE_CUOMUT      10
#define  _CONSULTA_PIDE_PERFIL      11
#define  _CONSULTA_PIDE_TARJETA     12
#define  _CONSULTA_CLIENTES         13
#define  _CONSULTA_FUNCION          14
#define  _CONSULTA_INSTITUCIONES    15
#define  _CONSULTA_T_COMPRA         16
#define  _CONSULTA_CUOTAS_TARJ      17
#define  _CONSULTA_VIGENCIA         18
#define  _CONSULTA_DIFERIMIENTO     19
#define  _CONSULTA_TARJETA          20
#define  _CONSULTA_BANCOS           21
#define  _CONSULTA_REPACTACION      22
#define  _CONSULTA_ELEGIR_CUOTAS    23
#define  _CONSULTA_ELEGIR_REMITO    24
#define  _CONSULTA_DATA_ENTRY       25
#define  _CONSULTA_TIPO_CLIENTES    26
#define  _CONSULTA_GRUPOS_CLI       27
#define  _CONSULTA_PAGOS_CAJA       28
#define  _CONSULTA_CLI_REINTEGROS   29
#define  _CONSULTA_DESCRIPT			30
/*****************************************************************************/
/* Indices para el vector de caracter�ticas del t�ulo del browse para      */
/* consultas                                                                 */
/*****************************************************************************/
#define  _INDICE_FUENTE                   0
#define  _INDICE_COLOR                    1
#define  _INDICE_POSI                     2
#define  _INDICE_ALIN                     3
#define  _INDICE_TEXT                     4
#define  _INDICE_TEXT_COL                 5
#define  _INDICE_POSI_TEXT_COL            6
#define  _INDICE_POSI_PAN_INPUT           7
#define  _INDICE_FUENTE_TIT_GRID          8
#define  _INDICE_COLOR_TIT_GRID           9
#define  _INDICE_POSI_TIT_GRID            10
#define  _INDICE_ALIN_TIT_GRID            11
#define  _INDICE_TEXT_TIT_GRID            12
#define  _INDICE_TEXT_COL_TIT_GRID        13
#define  _INDICE_POSI_TEXT_COL_TIT_GRID   14
#define  _INDICE_POSI_PAN_INPUT_TIT_GRID  15

/*****************************************************************************/
/* Indices para el vector de caracter�ticas del PANEL del browse para      */
/* consultas                                                                 */
/*****************************************************************************/
#define  _INDICE_GIF                      2
#define  _INDICE_FONT_PAN                 3
#define  _INDICE_FONT_COLOR_PAN           4
#define  _INDICE_BACK_COLOR_PAN           5
#define  _INDICE_SEL_BACK_COLOR_PAN       6
#define  _INDICE_ROW_SIZE_PAN             7
#define  _INDICE_FONT_TEXT_PAN            8
#define  _INDICE_FONT_COLOR_TEXT_PAN      9
#define  _INDICE_TEXT_POS_TEXT_PAN        10
#define  _INDICE_FRAME_SIZE_WIDTH         11
#define  _INDICE_FRAME_SIZE_HEIGHT        12


/*****************************************************************************/
/* Distintos inputs
/*****************************************************************************/
#define  _INPUT_GENERICO         0
#define  _INPUT_TARJETA          1
#define  _INPUT_ALTA_CLIENTES    2
#define  _INPUT_COBROS_ESP       3

/*****************************************************************************/
/* Tipos de Comprobantes                                                     */
/*****************************************************************************/
#define FACTURA_TIPO_A  1
#define FACTURA_TIPO_B  2

/*************************** Tipo Letra Comprobante **************************/
#define TIPO_A  1
#define TIPO_B  2

#define _F_SUP_CIERRE                   1000
#define _F_SUP_MODO_Z                   1001
#define _F_SUP_VUELTO_ME                1002
#define _F_SUP_INTERESES                1003
#define _F_SUP_ENVIAR_OFFLINE           1004
#define _F_SUP_MEDIO_EN_BOL             1005
#define _F_SUP_TAR_REP                  1006
#define _F_SUP_TAR_ESP                  1007
#define _F_SUP_DATOS_TAR                1008
#define _F_SUP_DATOS_CUPON              1009
#define _F_SUP_TARJETA_EN_BOL           1010
#define _F_SUP_BOLETIN_VENCIDO          1011
#define _F_SUP_NRO_AUT_TARJETA          1012
#define _F_SUP_CHEQUE_30_DIAS           1013
#define _F_SUP_LIMITE_TICKET            1014
#define _F_SUP_CODIGO_0                 1015
#define _F_SUP_AUT_PRECIO               1016
#define _F_SUP_CAJERO_DUPLICADO         1017
#define _F_SUP_HABILITAR_CAJERO         1018
#define _F_SUP_CAJONES_HAB              1019
#define _F_SUP_ABORTA_CIERRE            1020
#define _F_SUP_REINTENTO_RENDICION      1021
#define _F_SUP_MODO_DEVOLUCION          1022
#define _F_SUP_CAJEROS_DISTINTOS        1023
#define _F_SUP_DIF_PRECIO               1024
#define _F_SUP_PRECIO_MAS_BAJO          1025
#define _F_SUP_SALIR_INVENTARIO         1026
#define _F_SUP_CUPO_EXCEDIDO            1027
#define _F_SUP_CLIENTE_MANUAL           1028
#define _F_SUP_ROLLO_INTERNO            1029
#define _F_SUP_AUTORIZ_MEDIO_P          1030
#define _F_SUP_AUTORIZ_DESC_CLI         1031
#define _F_SUP_AUT_IMPORTAR_NC          1032
#define _F_SUP_PRECIOS_NC               1033

#define _F_SUP_ANULACION_COBRO          1034
#define _F_SUP_ANULACION_PF             1035
#define _F_SUP_AUT_POR_ACCION           1036
#define _F_SUP_CANJE_OFF_LINE           1037
#define _F_SUP_AUT_LIMITE_MAX_AVISO     1038
#define _F_SUP_INGRESAR_ENVASES         1039
#define _F_SUP_CIERRE_CAJERO_STB        1040
#define _F_SUP_AUT_DIA_OPERATIVO        1041
#define _F_SUP_DEV_ENVIO_DOMICILIO      1042
#define _F_SUP_SINDICATO_MANUAL         1043
#define _F_SUP_CLIENTE_FIEL_MANUAL      1044
#define _F_SUP_AUTORIZ_PRECIO_CRED      1045
#define _F_SUP_AUT_ARTICULO             1046
#define _F_SUP_AUT_PROMO                1047
#define _F_SUP_REPORTE_DISCOPESOS       1048
#define _F_SUP_DATOS_TARJ_ENCRIPT       1049
#define _F_SUP_DATOS_TARJ_REFERENCIA    1050
#define _F_SUP_DOSIFICACION             1051
#define _F_SUP_MEDIO_DE_PAGO            1052
#define _F_SUP_GENERAR_PAQUETE_ERROR    1053
#define _F_SUP_CAMBIO_COND_IVA          1054
#define _F_SUP_CAMBIO_MEDIO             1055
#define _F_SUP_FUTUROS_SOCIOS           1056
#define _F_SUP_CARGA_MANUAL             1057
#define _F_SUP_AUTORIZA_VENCIMIENTO     1058
#define _F_SUP_CIERRE_LOTE_Y            1059
#define _F_SUP_AUTORIZA_NC_LIBRE        1060
#define _F_SUP_CONTROL_VERSIONES        1061
#define _F_SUP_AUT_CIERRE_CON_DIF       1062
#define _F_SUP_CLIENTE_INHAB            1063
#define _F_SUP_REVERSO_COBRO            1064
#define _F_SUP_TIMBRADO_REIMPRESION     1065
#define _F_SUP_RETIMBRADO_REIMPRESION   1066
#define _F_SUP_RETIMBRADO_IMPRESION     1067
#define _F_SUP_RETIMBRADO_REINTEGRO     1068
#define _F_SUP_PRECIO_REGULADO          1069
#define _F_SUP_PERCEPCION_NC            1070
#define _F_SUP_RETENCION_NC             1071
#define _F_SUP_REINT_EFEC_OTRA_SUC      1072
#define _F_SUP_USO_F_FIJO_REINTEGRO     1073
#define _F_SUP_DEUDA_MAS_ANTIGUA        1074
#define _F_SUP_IMPORTE_MINIMO_TICKET    1075
#define _F_SUP_CAJERO_FLOTANTE_OFFLINE  1076
#define _F_SUP_REVERSO_PAGATODO_ERROR   1081
#define _F_SUP_CIERRE_CON_TICKET_ESPERA 1082
#define _F_SUP_AUTORIZACION_COMPRA_TARJ 1083
#define _F_SUP_CIERRE_CON_CAMBIO_MEDIOS 1084


/*-------------------------- Modos DEBUG ------------------------------------*/
#define _DEBUG_DRV_SEC         1  //referido drivers secundarios
#define _DEBUG_BALANZA         2  //referido a BALANZA
#define _DEBUG_INICIO          3  //referido al inicio de la caja,.._INICIANDO_POWER_DOWN,_INICIANDO_LLAVE,.._CAJON_INICIADO,
#define _DEBUG_RNV             4  //referido a RNV
#define _DEBUG_ITEM            97 //referido al item, agregar_item_al_ ticket,anular_descuento_item,,CARGAR_PROMO_MEMORIA (instantaneo para el item..)
#define _DEBUG_REDONDEO        99 //redondeo en version el salvador (por ahora)


/*----------- Origen de Datos Adicionales --------------*/
#define D_MEDIOS              0
#define D_ITEMS               1
#define D_CLIENTES            2
#define D_MEDIOS_PRE_IMPORTE  3
#define D_COBRO_SERVICIOS     4
#define D_ENTRY               5
#define D_ENVIO_DOMICILIO     6
#define D_CAMBIO_MEDIO        7
#define D_VALIDACIONES        8
#define D_IMPRESION           9
#define D_COBROS              10
#define _PRE_IMPORTE		  20
#define _POS_IMPORTE		  21


/*----------- Mostrar medios en pagos --------------*/
#define MEDIO_1               1
#define MEDIO_2               2
#define MEDIO_3               3
#define MEDIO_4               4
#define MEDIO_5               5
#define MEDIO_6               6
#define MEDIO_7               7
#define MEDIO_8               8
#define MEDIO_9               9
#define MEDIO_10              10

/*------------------------- Indices Tabla articulo ---------------------------*/
#define INDICE_COD_INT_COD_BARRA        8

/*------------------------- Modos de panelGrill ------------------------------*/
#define SIN_SCROLL      0
#define PERMITE_SCROLL  1

/*------------------------- Tipos de busquedas en la tabla cli_cobro --------*/
#define _BUSQ_CODSOCIO                  1
#define _BUSQ_CUIT                      2
#define _BUSQ_ALFABETI                  3
#define _BUSQ_ID_CLICO                  4
#define _BUSQ_CODSOCIO_UNICO            5
#define _BUSQ_ALFABETI_REINTEGROS       6
#define _BUSQ_CODSOCIO_UNICO_REINTEGROS 7


#define COD_UNIDAD_NEGOCIO 4

/*------------------------- Tipos de verificacion en la tabla motivos ---------------------------*/
#define _MOTIVO_OBTENER_CODSOLICITUD   1
#define _MOTIVO_OBTENER_SUPERVISOR     2
#define _MOTIVO_OBTENER_CODSOLYSUPER   3

// Valores del campo tarjetas de la tabla cobros
#define T_NO                   0
#define T_ON_LINE              1
#define T_NORMAL               2
#define T_OFF_LINE             3

// Valores del tipo de operacion de tarjeta guardada en archivo tarjetas.XXX
#define TARJETA_TRADICIONAL    0   //"tarjetas.$$$"
#define TARJETA_COBRO          1   //"tarjetas.cob"
#define TARJETA_MONEDERO       2   //"tarjetas.mon"

// Modos de operacion de medios acumulados
#define M_DETALLADO  0 //Retiro y Arqueo Detallado ( default )
#define M_ACUMULADO  1 //Retiro y Arqueo Acumulado
#define M_RET_DETALL 2 //Retiro Detallado y Arqueo Acumulado

/*------------------------- Pantalla  ---------------------------*/
#define PANTALLA_VENTA          1
#define PANTALLA_PAGO           2
#define PANTALLA_STANDBY        3
#define PANTALLA_PRESENTACION   4
#define PANTALLA_FONDO_FIJO     5
#define PANTALLA_RETIRO         6
#define PANTALLA_CAMBIO_MEDIO   7

/*-------------- Comienzo y Fin de Impresiones -------------------*/

#define BEGIN_VOUCHER			"|#&" //Secuencia NO IMPRIMIBLE para identificar el comienzo de un voucher
#define FINISH_VOUCHER			"|$&" //Secuencia NO IMPRIMIBLE para identificar el fin de un voucher

#define PERCEP_DGI			     1
#define RETEN_DGR			     2
#define PERCEP_TACIS		     3
#define PERCEP_212				 4

/*****************************************************************************/
/* VARIABLES GLOBALES                                                        */
/****************************************************************************/
	extern unsigned char BUFFER[256];
	extern unsigned char res1, res2;
	extern int _GRANDE;
	extern int TIPO_DE_LETRA;
	extern int ESTACION_SELECCIONADA;
	extern char BUFFER_LINEA_PRN[200];
	extern int COLUMNA ;
	extern int _LETRA_GRANDE_PEGADA;
	extern char _ATRIBUTO;
	extern char _CURSOR;
	extern char TECLADO_ESPECIAL;
	extern char _UTILIZAR_FECHA_OPERACION;
	extern int TECLADO_ALFABETICO;
	extern int error_matematico;
	extern int NRO_ARCHIVO;
	//extern char _MODO_EJECUCION;
	extern char _BANDA[50];
	extern char _LLAVE;
	extern int _medio_efectivo,
		_medio_cheque, _medio_tarjeta, _medio_ctacte, _medio_moneda_extranjera;
	extern int _medio_ticket_compra, _medio_mutual, _medio_tarjeta2;
	extern char _IMPRIMIENDO_Z_ANT;
	extern int _LARGO_TICKET;
	extern char _RNV_VIRTUAL;
	extern char _PERMITIR_SCANNER;
	extern char _PERMITIR_BANDA;
	extern int *_TECLAS_EXCEPCION;
	extern void ( *_FUNCION_EXCEPCION ) (  );
	extern char _CONTROLAR_RNV;
	extern unsigned _FECHA_ARCHIVO;
	extern int _NRO_COMANDO_INVENTARIO;
	extern int _LARGO_TRANSFERENCIA;
    extern char _TOUCH_SCREEN;


/*****************************************************************************/
/* FUNCIONES                                                                 */
/*****************************************************************************/
	//void      MOSTRAR_CADENA( int x, int y, char *cadena );
	//extern void MOSTRAR_CADENA_STRING( int x, int y, int id );
   extern void MOSTRAR_CADENA_ALD( int x, int y, char *cadena, int largo );
	//extern void MOSTRAR_CARACTER( int x, int y, char caracter );
	extern void MOSTRAR_FECHA( int x, int y, unsigned fecha );
   extern void OBTENER_FECHA_LETRAS( unsigned fecha, unsigned char *result );
   extern void MOSTRAR_FECHA_LETRAS( int x, int y, unsigned fecha );
	extern void DTOC( unsigned fecha, char *cadena );
	extern void MOSTRAR_MES_ANIO( int x, int y, unsigned fecha );
	extern void MOSTRAR_HORA( int x, int y, int hora );
	extern void MOSTRAR_ENTERO( int x, int y, char *picture, long entero );
	extern void MOSTRAR_IMPORTE( char x, char y, char largo, double importe );
	extern void MOSTRAR_IMPORTE_SIN_COMAS( char x, char y, char largo, double importe );
	extern void MOSTRAR_FLOTANTE( int x, int y, int enteros, int decimales, int signo,
		int comas, double importe );
	extern void MOSTRAR_FLOTANTE_ALINEADO( int x, int y, int enteros, int decimales, int signo,
		int comas, double importe );
	extern void FORMAT_FLOTANTE( char *picture, double importe, char *cadena );

	extern void MENSAJE_SONORO( char *cadena, char tipo );
	extern void MOSTRAR_GRAN_CADENA( int x, int y, char *cadena );
	extern void MOSTRAR_GRAN_CARACTER( int x, int y, char caracter );
	extern void MOSTRAR_GRAN_ENTERO( int x, int y, char *picture, long entero );
	extern void MOSTRAR_GRAN_IMPORTE( char x, char y, char largo, double importe );
	extern void MOSTRAR_GRAN_IMPORTE_SIN_COMAS( char x, char y, char largo, double importe );
	extern void MOSTRAR_GRAN_FLOTANTE( int x, int y, int enteros, int decimales, int signo,
		int comas, double importe );
	extern long PEDIR_ENTERO( int x, int y, int largo, int busqueda );
	//extern int GETCH(  );
	//extern void UNGETCH( int caracter );
	extern long PEDIR_GRAN_ENTERO( int x, int y, int largo );
	/*extern int PEDIR_CADENA_NUMERICA( char x, char y, char largo, char *cadena,
		char *mascara_scanner, int decimal, int cerear_cadena, int permite_flecha );*/
	extern int PEDIR_CADENA_NUMERICA( char x, char y, char largo, char *cadena, char *mascara_scanner,
                           int decimal, int cerear_cadena, int permite_flecha , char caracter_mostrar );
	extern int PEDIR_VALIDAR_MASCARA( char *cadena, char *mascara_scanner );
	extern int PEDIR_GRAN_CADENA_NUMERICA( int x, int y, int largo, char *cadena );
	extern double PEDIR_GRAN_FLOTANTE( int x, int y, int enteros, int decimales );
	extern double PEDIR_FLOTANTE_CON_SIGNO( char x, char y, char enteros, char decimales );
	extern double PEDIR_GRAN_FLOTANTE_CON_SIGNO( int x, int y, int enteros,
		int decimales );
	extern double PEDIR_GRAN_FLOTANTE_CON_VALOR( int x, int y, int enteros, int decimales,
				double valor );
	extern int PEDIR_CADENA( char x, char y, char largo, char *cadena,
		char *mascara_scanner, int cerear_cadena,int permite_flecha );
	extern int PEDIR_GRAN_CADENA( char x, char y, char largo, char *cadena,
		char *mascara_scanner );
	//extern void SET_PUERTO( int puerto );
	//extern int INICIALIZAR_PUERTO(  );
	//extern int ENVIAR_COMANDO( unsigned char *comando, unsigned char *buf_datos,
	//	unsigned char *error1, unsigned char *error2, unsigned char *error3 );

	//int         INGRESAR_CLAVE( int x, int y, char *mensaje, int tipo, int cajero, char *titulo, int funcion ) ;
	//int         SOLICITAR_CLAVES( int x, int y, int cajero, int supervisor, int condicion, char *titulo, char *log, int funcion ) ;
	//double    ROUND( double numero, int enteros, int decimales );
	//void      DTOA( unsigned fecha, char *cadena, int anio_completo ) ;
	//void      ITOA( long entero, char *cadena, int digitos );
	//void      FTOA( double numero, char *cadena, int enteros, int decimales, int signo,
	//	int comas );
	//void      FTOA_SIN_PUNTO( double numero, char *cadena, int enteros, int decimales,
	//	int signo, int comas );
	//void      CEREAR( char *puntero, int cant );
	//void      EMITIR_MENSAJES(  );
	//void      ALARMA( int tipo );

	//int       YEAR( unsigned fecha );
	//int       MON( unsigned fecha );
	//int       DAY( unsigned fecha );
	//void      GRABAR_LOG_SUPERVISOR_STRING( int id_cadena, int supervisor );
	//void      POSICIONAR_REGISTRO( int handle, int registro, int l_registro );
	//void      DTOA_INVERSA( unsigned fecha, char *cadena );
	//void      FTOC( double numero, char *cadena, int enteros, int decimales, int signo,
	//	int comas );
	//void      MOSTRAR_HORA_COMPLETA( int x, int y, long hora );
	//void      IMPRIMIR_Z_FISCAL(  );
   //void      FORMAT_CADENA( char *picture, char *cadena_in , char *cadena_out );

/*------------------------------ M_FECHA ------------------------------------*/
	//void      MOSTRAR_FECHA_2000( char x, char y, unsigned fecha );
	//void      DTOC4( unsigned fecha, char *cadena );
	//void      DTOC_2000( unsigned fecha, char *cadena );
	//void 		 FORMAT_FECHA( char *picture, unsigned fecha, char *cadena );
	//void      FECHA_DD_MMM_AAAA( unsigned fecha, unsigned char *result );

/*------------------------------ VALIDA  ------------------------------------*/
	//void		 MOSTRAR_VARIABLE_RNV( int cod_variable, int subindice, int columna,
	//	char *picture );
	//int       VALIDA_COD_COMANDO(  );
	//int       MENU_SELECCIONA_MEDIO_A_REIMPRIMIR(  );
	//void      VALIDA_ENVASES_TOMRA(  );
	//void      VALIDAR_COBROS_POR_ARTICULO(  );
	//int		 OBTENER_VALOR_RNV( int cod_variable, int subindice );
	//void		 CALCULAR_CODIGO_VOUCHER_BOLETA_PLATA( );
	//void		 VALIDA_tomar_cadena_compuesta( int *ind_config, char *val_cad, int h );
	//double	 OBTENER_FLOTANTE_DE_CADENA( char *buffer );
	//void		 CARGAR_VARIABLE_RNV( int cod_variable, char *buffer,
	//	int l_buffer, int indice, char *picture );
	//void 		 VALIDA_SET_SALIDA_CUPON( int destino );
	//int 		 VALIDA_GET_SALIDA_CUPON(  );
	//void		 VALIDAR_T_ESPERA( int cod );
	//int 		 VALIDA_OBTENER_GRUPO_CUOTA( char *mensaje );
	//long 		 CALCULA_CODIGO_VERIFICACION( int caja, int fecha, long ticket, int digito,
	//	int sucursal );
    //void         CARGAR_DATOS_D_IMPRESION( int posic, int cod_sol, int nro_orden, int modalidad );
   //void          VALIDAR_UN_MEDIO( int h, int cod, int modo );
   //void          IMPRIMIR_LINEAS_MENSAJE( long id_transaccion, char nro_cuerpo );
  /* void VALIDAR_COBROS_PAGATODO( struct DatosCobroPagaTodo *datos_cobros, int modo );
   void VALIDAR_COBROS_GARANTIA( struct _datos_cobro *datos_cobros, int modo );
   void VALIDAR_VOUCHER_MUTUAL( int h, int cod_formato, double importe );
   void VALIDAR_VOUCHER_TICKET_COMPRA( int h, int cod_formato, double importe );
   void VALIDA_CIERRE_CAJERO( int cod_formato );*/


/*------------------------------   HTOA   ------------------------------------*/
	//void      HTOAL( long hora, char *cadena );
	//void      HTOA( unsigned hora, char *cadena );

/*------------------------------   DAY   ------------------------------------*/
	//void      CDAY( unsigned fecha, char *cadena );

/*------------------------------ GET_FECH ------------------------------------*/
	//int       GET_FECHA_DOS(  );
	//unsigned  _GET_FECHA_DOS(  );

/*------------------------------ TKT ------------------------------------*/
	//void      TKT_ejecutar_comando( int *ind_config, double precio, int h, int h2);
	//void      INIT_COMPROBANTE_TKT(  );
	//void      TKT_FISCAL_B(  );
	//void      TKT_CORTE_PAPEL(  );
	//void      IMPRIMIR_TKT_CUERPO( double precio, int indice );
	//void      IMPRIMIR_TKT_CABECERA( int indice );
	/*void	  IMPRIME_TKT_DESCRIPCION_CUERPO( int indice );
	void      IMPRIME_TKT_DESCRIPCION_TARJETA( int indice );
	void      IMPRIME_TKT_IMPORTE_TOTAL( int indice );*/
	//void      IMPRIMIR_TKT_TITULO_TICKET(  );
	//void      IMPRIMIR_TKT_IVAS(  );
	//void      IMPRIMIR_TKT_AFINIDAD(  );
	//void      IMPRIMIR_TKT_NO_AFINIDAD(  );
	//void      IMPRIMIR_TKT_IDENTIFICACION(  );
	//void      IMPRIMIR_TKT_PIE(  );
	//void      IMPRIMIR_TKT_TICKET_ESPERA( void );
    //void      TIMBRADO_TKT( int indice );
    //void      IMPRIMIR_TKT_PAGO_REINTEGRO();
    //void      TIMBRADO_TKT_REINTEGRO();
	//int       IMPRIMIR_TKT_TOTAL( double importe, int modo );
	//void      IMPRIMIR_TKT_VUELTO( int h );
	//void      IMPRIMIR_TKT_DONACION(  );
	//void      IMPRIMIR_TKT_MEDIOS( int h, int h2 );
    //int       _COMPROBANTE_TKT( int comienzo, int final );
    //int       TKT_evaluar_condiciones( int codigo, /*int *ind_config,*/ int h );
    //INT16     TKT_tomar_entero( int *ind_config, int *indice );
    //void      TKT_SET_FILA( int _fila );
    //int       TKT_desplazar_config( int codigo, int *indice );
    //void 	  IMPRIMIR_TKT_SUBTOTAL(  );
    //void      TKT_incrementar_puntero( int codigo, int *ind_config );
    //void      TKT_TOTALIZAR_TEMPORAL( int tipo_evento, int sub_evento, int disc_medios,
    //int       filtra_medios, int cod_medio_sub_medio );
    //void      TKT_AGREGAR_EVENTO_A_TMP( int disc_medios, int filtra_medios, int cod_medio_sub_medio  );
    //int       TKT_EVENTO_SOLICITADO( int tipo_evento, int sub_evento );
    //int       IMPRIMIR_TKT_INF00( int ini_est, double *dif, double *dif_me );
    //void      IMPRIMIR_TKT_INF_TAR( int est );
    //void      OBTENER_DATO_TABLA( int codtabla, int orden, char *indice, char *tipores, char offset[200]/*, int linea*/);
    //int       IMPRIMIR_TKT_FONDO_FIJO( );
    //int       IMPRIMIR_TKT_INF_DIFERENCIAS( );
    //int		  IMPRIMIR_TKT_INF_DIFERENCIAS_SUBMEDIOS_CONTINUO();
    //long	  ULTIMO_ID_EVENTO( int tipo_evento );
    //long	  CANTIDAD_EVENTOS_ORIGINAL( int tipo_evento );
	//long CANTIDAD_EVENTOS( int tipo_evento , int evento_bak);
    //long	  ID_EVENTO_NRO( int nro_evento, int tipo_evento );
    //void      ARMAR_STRING_FOLIO( char *cad );
    //void      SET_BAR_CODE( char *buff );
    //void      BAR_CODE( void );
    /*void      EXISTE_PRINTER( void );
    void	  TKT_total_en_letras( int *ind_config );
    void      LETRA_IMPUESTO( char cod_impuesto, int iva, double exento, char*letra );
    void      IMPRIME_COTIZACION( int x_loc , int y_loc , int h );
    void      IMPRIME_IMPORTE_INGRESADO( int x_loc, int y_loc, int h );
    void      RESOLVER_FUNCIONES( char *funciones, char *RtaConsulta, int linea );
    void IMPRIMIR_TKT_FRANQUEO( int modo );*/
/*------------------------------ FISCAL ------------------------------------*/
	//void      CONTROLAR_COMANDO_FISCAL(  );
	//int       CAD_HEXA_2_DEC( char byte );
   //void      RESET_FLAG_ERROR_IBM4610( int ticks_b );
   //void      LEER_PRINTER_IBM4610_FLASH( int tipo );
   //int       _COMANDO_LEER_CONTADORES_PRINTER_FISCAL( int asignar, long *nro_boleta );
   //int 		 COMANDO_LEER_CONTADORES_PRINTER_FISCAL( int reintentos, int asignar, long *nro_boleta );
	//int		 STATUS_PRINTER_FISCAL( );
   //void      IMPRIME_BMP_LOGO( char *nomarch );
   //int 		 COMANDO_LEER_STATUS_PRINTER_FISCAL( int reintentos );
   //int 		 _COMANDO_LEER_STATUS_PRINTER_FISCAL(  );
   //void      RESET_FLAG_ERROR_IBM4610( int ticks_b );
   //void      CONSULTAR_DATOS_IMPRESORA_FISCAL( int *nro_caja, long *nro_z );

/*------------------------------ PRINT ------------------------------------*/
   /*void      _INICIALIZAR_PRINTER(  );
   void      PRN_LETRA_NORMAL(  );
   void      PRN_LETRA_GRANDE(  );
   void      PRN_HABILITA_AVANCES(  );
   void      PRN_INHABILITA_AVANCE_JOURNAL(  );
   void      PRN_INHABILITA_AVANCE_RECEIPT(  );
   void      ENVIAR_CODIGOS( int op );
   void      PRINTER_OFF(  );
   void      PRN_HABILITA_SLIP( int orientacion, int enviar_blanco );
   void      PRN_HABILITAR_IMPRESORA_OPCIONAL(  );
   void      PRN_DESHABILITAR_IMPRESORA_OP(  );
   void      SEND_BYTE_SERIAL( unsigned char byte );
   void      CORTE_DE_HOJA_MANUAL(  );
   void      OPEN_COPIA_VOUCHER_PRN(  );
   void      CLOSE_COPIA_VOUCHER_PRN(  );
   void      GRABA_COPIA( char comando, int param );
   void      VERIFICAR_IMPRESORA(  );
   void      ABRIR_JOURNAL_ELECTRONICO( void );
   void      CERRAR_JOURNAL_ELECTRONICO( void );
   void      GRABAR_JOURNAL_ELECTRONICO( unsigned char byte );
   void      GRABAR_JOURNAL_ELECTRONICO2( char *cadena );
   void      CALCULAR_CHECK_SUM( unsigned char *cadena, unsigned char *cadena2 );
   void      BACKUP_JOURNAL_ELECTRONICO( void );
	void      INIT_JOURNAL_ELECTRONICO( void );
	int       GET_COLUMNA( void );
	int       TM930_IS_SLIP_PRESENT(  );
	void      PRN_FIN_TICKET( );
	void      PRN_FIN_TICKET_CONTINUO( int pagina, int tot_pagina );
	void      PRN_HABILITAR_LECTURA_CHEQUE(  );
	void      PRN_LEER_CHEQUE(  );
	int       PRN_HAY_DATOS(  );
	int       LEER_DATOS_CHEQUE_IMPRESORA(  );
    void      LF( int cantidad );
    int		  REDIRECCION_PRINTER_A_BUFFER( int cant_columnas, int cant_filas );
    void	  FIN_REDIRECCION_PRINTER_A_BUFFER(  );
    void      DISK_FLUSH( void );
	void      PRINT_BITMAP(  );*/

/*------------------------------ DISPLAY ------------------------------------*/
	//void      DISPLAY_STRING( int id );

/*------------------------------ HANDLES ------------------------------------*/
	//int       AJUSTA_CANTIDAD_HANDLES( int cant_pedida );

/*------------------------------ B_MENS ------------------------------------*/
	//void      BORRAR_MENSAJE(  );

/*------------------------------ BAUCHER ------------------------------------*/
	//void          IMPRIME_BAUCHER( int p, int para_comercio, long anulacion );

/*--------------------------------- IO --------------------------------------*/
	/*#ifdef INVEL_L
		int 		filelength(int handle);
	#endif
	int       _READ( int handle, char *buffer, int cant, int linea, char *file );
	int       _WRITE( int handle, char *buffer, int cant, int linea, char *file );
	int       LSEEK( int handle, long pos, int tipo, int linea, char *file );
	int       _CLOSE( int handle, int linea, char *file );
	int       _OPEN( char *nombre, int tipo, int linea, char *file );
	int       _CREAT( char *nombre, int tipo, int linea, char *file );
	int       _OPEN_O_CREAT( char *nombre, int tipo, int linea, char *file );
	int       COPIAR_ARCHIVO( char *nom_origen, char *nom_destino, int linea, char *file );
	int       APPEND_ARCHIVO( char *nom_origen, char *nom_destino, int linea, char *file );
	void      _SET_ERROR_EN_RED( void );
	int       error_disco(  );
	int       GET_ERROR_EN_RED(  );
	void      _SET_FECHA_ARCHIVO( int handle, unsigned fecha );
	void      _VERIFICAR_REINTENTO(  );
	void      GRABAR_ERROR( int operacion, int error, int intentos, char *nombre,
		int handle );
	int       ARCH_RED( int nro );
	void      VERIFICAR_REINTENTO(  );
	int       SET_FECHA_ARCHIVO( char *nom_archivo, unsigned fecha );
	int       _COPIAR_ARCHIVO( int modo, char *nom_origen, char *nom_destino, int linea,
		char *file );*/

/*-------------------------------- IMP_RET ---------------------------------*/
	/*void      IMPRIME_CABECERA_RETIROS( unsigned tipo, int retiro );
	void IMPRIME_CUERPO_RETIROS( int anulacion, char *nombre, double importe, long numero, int cantidad,
                             double importe_me, int medio );
	void      IMPRIME_PIE_RETIROS( double *, double, int, double *, int );*/

/*-------------------------------- IMP_xxx ---------------------------------*/
	/*void      TICKET_ADELANTOS( double importe );
	void      TICKET_DEVOLUCIONES( double importe );
	void      TICKET_FONDO_FIJO( double importe, int tipo );
	void      IMPRIMIR_NOMBRE_USUARIO( int nro_caj_sup );*/

/*-------------------------------- ENVIOS ----------------------------------*/
	/*void      PREPARAR_DATOS_TERMINAL( char tipo_operacion, double importe,
      struct _datos_terminal *datos_terminal );
	int       AUTORIZACION_ON_LINE( int tipo_operacion, double importe, char *buf_rta,
      int l_buf_rta );
	int       GRABAR_OFF_LINE( int tipo_operacion, double importe );
	int       PAGOS_PEDIR_DATOS_ADICIONALES( int pos, char indice_pos,
      int datos_adicionales, double *importe, int origen, char* titulo, int nro, long *comprobante,
		int ( f_datos ) ( int, int, int, int, double,struct _dmedios *, char * ) , char cod_medio, char submedio );
	int       _PAGOS_PEDIR_DATOS_ADICIONALES( int pos, char indice_pos, double *importe,
		struct _dmedios *datos_medio, int cant_datos, int origen, char* titulo, int nro, long *comprobante,
	   int ( f_datos ) ( int, int, int, int, double,struct _dmedios *, char * ) , char cod_medio, char submedio );
	int       AGREGA_BIN_EV_DMEDIO_A_BTRV( int h, int origen );*/

/*-------------------------------- BROWSE ----------------------------------*/
	/*struct _browse {
		int       alto;
		int       seguir;
		int       mostrar;
		int       linea;
		int       pos;
		int       posicionar;
		int       tecla;
		int       index;
	};

	struct _FieldDescription {
		char      colName[20];
		int       width;
		int       alignment;
		int       type;
	};
	struct _dat {
		int       nFields;
		int       selectedField;
		struct    _FieldDescription FieldDescription[10];
	};*/
	void      setFieldValueInt( int index, int field, int value );
	void      setFieldValueFloat( int index, int field, float value );
	void      setFieldValueDouble( int index, int field, double value );
	void      setFieldValuepChar( int index, int field, char *value );
	//int       BROWSE( int x1, int y1, int x2, int y2, int l_cadena, int ( f_usuario ) (  ), double monto );
	typedef int ( *f_usu ) ( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
  	int       Consulta_articulo_codbarra( f_usu f_usuario );
   int       Consulta_elegir_motivo( f_usu f_usuario );
   int       Consulta_pide_cuotas_mutual( f_usu f_usuario );
   int       Consulta_elegir_mutuales( f_usu f_usuario, double importe, unsigned short cod_medio );
   int       Consulta_pide_perfil( f_usu f_usuario );
   int       Consulta_pide_tarjeta( f_usu f_usuario );
   int       Consulta_cliente( f_usu f_usuario );
   int       Consulta_articulo_descrip( f_usu f_usuario, int col );
   int       Consulta_elegir_funcion( f_usu f_usuario, int largo );
   int       Consulta_intituciones( f_usu f_usuario );
   int       Consulta_t_compra( f_usu f_usuario, unsigned short cod_medio );
   int		 Consulta_cuotas_tarjeta( f_usu f_usuario, unsigned short cod_medio );
   int       Consulta_vigencia( f_usu f_usuario );
   int       Consulta_diferimiento( f_usu f_usuario );
   int Consulta_cobros( int registros,f_usu f_usuario );
   int       Consulta_elegir_tarjeta( f_usu f_usuario );
   int       Consulta_bancos( f_usu f_usuario );
   int       Consulta_repactacion( f_usu f_usuario );
   int       Consulta_elegir_cuotas( f_usu f_usuario );
   int       Consulta_elegir_remito( f_usu f_usuario );
   int       Consulta_data_entry( f_usu f_usuario );
   int       Consulta_tipos_de_clientes( f_usu f_usuario );
   int       Consulta_condicion_iva( f_usu f_usuario );
   int       Consulta_Pagos_Esp( f_usu f_usuario );
   int       Consulta_Pagos_Esp2( f_usu f_usuario );
   int       Consulta_socio( f_usu f_usuario );
   int       Consulta_grupos_cli( f_usu f_usuario );
   int       Consulta_Pagos_Caja( f_usu f_usuario );
   int _elegir_motivo( int comando, char *cadena, int dato, struct _browse *b, double monto );
   /*--------------------------------- XYZ ------------------------------------*/
	/*int       GRABAR_XYZ( int modo );
#if defined(INVEL_W)
	int       _GRABAR_XYZ( char *nom, int est, struct _xyz *cab_xyz );
#endif
	void      SUMAR_X_USUARIOS( char *destino );
	void      RESETEA_ACUMULADORES_X(  );
	void      RESETEA_ACUMULADORES_Z(  );
	void      ACUMULAR_X_EN_Z(  );
	int       GRABAR_BACKUP_XYZ( int modo );
	void      GRABAR_EVENTO_YZ( int modo );
*/
/*------------------------------- GET_HORA ---------------------------------*/
	/*long      GET_HORA_COMPLETA_DOS(  );
	long      GET_HORA_SEGUNDOS(  );*/

/*-------------------------------- PAGOS -----------------------------------*/
	/*double    PAGOS_TOTAL_A_PAGAR(  );
	double    PAGOS_TOTAL( int redondeo );
	double    _VARIACION_TARJETA( double importe, float porc, double *ret_var,
		double *iva2, long );
	double    PAGOS_PIDE_IMPORTE( char *titulo, int enteros, int decimales, int signo );
	char     *PAGOS_OBTENER_NOMBRE( char medio, char submedio );
	double    PAGOS_ME_A_ML( double importe_me, int h );
	double    PAGOS_ML_A_ME( double importe_me, int h );
	int       PAGOS_OBTENER_VALOR( int h, int nro_orden, char *cadena, int origen );
	int       PAGOS_CARGAR_DATOS_MEDIO( struct _dmedios **datos_medio,
		int datos_adicionales, int origen );
	//int         PAGOS_CONSTRUYE_MASCARA( struct _dmedios *datos_medio, char *mascara_in, int tam_mascara, char *mascara, int cant_datos, double importe, struct _datos *datos ) ;
	//int         PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos ) ;
	int 		 _PAGOS_OBTENER_DATOS( int h, int posicion, struct _datos *datos, int origen );
	double    PAGOS_NETO_PROPORCIONAL( int h, double importe );
	double    PAGOS_IVA_PROPORCIONAL( int h, double importe );
	double    PAGOS_EXENTO_PROPORCIONAL( double importe );
	void      PAGOS_REPROCESAR( char tipo_pago, int reproceso );
	int       PAGOS_CARGAR_MEDIOS_IMPORTACION(  );
	int       DONACION_INGRESA_MULTIPLO(  );
   void      MOSTRAR_PORC_INTERES(  );
   void      ELIMINA_BASES_DTOS_POR_TECLA( );
	int       LEER_CHEQUE_IMPRESORA( );
	int		 DAR_VUELTA_CHEQUE_IBM4610(  );
	void		 INSERTAR_EN_DPAGO( struct _ev_dmedios dp_boletaplata );
	int 	  	 PAGOS_CANTIDAD_DATOS_ADICIONALES( int h );
	int		 PAGOS_OBTENER_VALOR_POS( int h, int nro_orden, int ind_posicion, char *cadena, int origen );
	int		 PAGOS_CANTIDAD_DATOS( int h );
	int		 CALCULAR_FECHA_VTO_CUO_MED( int cuota );
	int 		 SET_DATO_ADICIONAL( int h, int posicion, int origen, int id_dato, char *valor );
   int       PAGOS_F_DATOS_ADICIONALES(int pos, int ind_pos, int nro, int pos_actual, double importe ,struct _dmedios *datos_medio, char *buffer );
   void      BIN_DATOS_ADICIONALES( int origen, int h, char* archivo );
   void      BORRAR_BIN_DATOS_ADICIONALES( int origen );
   int       AGREGA_DATOS_MEDIO( int pos, char indice_pos, int nro_orden, int id_dato,
	      struct _datos *datos, int cod_solicitud, int origen, int primer_dato,
	      double importe );
   int       CARGAR_CHEQUE_MANUAL( int al_dia, long *numero, int *banco, unsigned *fecha_vto,
	   int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono, char *cuenta_banco,
	   int *plaza, char *nrobanco, int habil, char *serie, int *cod_valida_cheque,
	   int pedir_fecha_vto );

    void     CARGAR_EV_DMEDIOS( int cod_sol, int nro_orden, long id_evento, int origen, int area_ac,
                        struct _ev_dmedios *ev_dpago );*/
	/*int		 PAGOS_MEDIO_HABILITADO_COBRO( int cod_medio, int cod_submedio, int dev_cobro );
    int      TOMAR_DATO_ADICIONAL( char *valor, char *descripcion, int orden, int tipo_dato, char cod_medio, char submedio );*/
	int ARTICULO_EN_GRUPO( int grupo, char* cod_interno_alfa, char *cod_barra, char *cod_clasificacion, long cod_interno );

/*-------------------------------- CTA CTE -----------------------------------*/
	//long      PIDE_CUENTA( char largo );

/*---------------------------------- OMRON --------------------------------*/
	/*void      ALOJA_ESPACIO_OMRON(  );
	void      OMRON_ABRIR_CAJON(  );
	void      OMRON_BORRAR_DISPLAY(  );
	void      OMRON_BORRAR_DISPLAY_LINEA( int linea );
	int       OMRON_CAJON_CERRADO(  );
	void      OMRON_DISPLAY( char *buffer );
	void      OMRON_DISPLAY_LINEA( char *buffer, int linea );
	int       OMRON_GETKEY(  );
	int       OMRON_INIT(  );
	void      OMRON_INIT_DISPLAY(  );
	int       OMRON_IS_PRINTER(  );
	int       OMRON_KBHIT(  );
	int       OMRON_PAPER_OUT(  );
	void      OMRON_PRINT( char *buffer );
	int       OMRON_TARJETA_MAGNETICA( unsigned char *buffer );
	int       _OMRON( char tipo, char *estructura, int longitud );
*/
/*-------------------------------- ACTUAL -----------------------------------*/
	/*void      ACTUALIZA_ACUMULADORES_IVA( double signo );
	void      ACTUALIZA_ACUMULADORES_RAM( int h );
	void      ACTUALIZA_BASE_DTO_EVENTO(  );
	void      ACUMULAR_BASE_DTO_EVENTO( double *base_total );
	void		 ACTUALIZA_DESCUENTOS_BOLETIN_ARTICULOS(  );
	void		 PRORRATEA_DESCUENTO_BOLETIN_ARTICULO( int pos, struct _art *art, int cant );
   void      ACUMULA_CONSUM_FINAL_FACT( void );
   void      ACTUALIZA_CONSUM_FINAL_TKT( void );
    void      ACUMULA_DESCUENTOS( void );
    char      CALCULAR_TIPO_EVENTO( void );*/

/*----------------------------------- TIME ----------------------------------*/
	/*unsigned long GET_TIME(  );
	void      SET_TIME( unsigned long valor );*/

/*----------------------------------- TIMER ----------------------------------*/
	/*unsigned long TIEMPO_PASADO_DESDE( unsigned long *desde );
	int       YA_PASARON_SEGUNDOS( unsigned long *desde, int segundos );*/

/*---------------------------------- MALLOC ---------------------------------*/
	/*void     *MALLOC( unsigned memoria );
	void     *MALLOC_CLEAR( unsigned memoria );
	void      FREEMEM( char *puntero );*/

/*-------------------------------- Modo_pro --------------------------------*/
	//char     *MK_FP_PROTEGIDO( unsigned seg, unsigned off, unsigned long l );

/*--------------------------------- PinPad --------------------------------*/
	/*int       PINPAD_AUTORIZAR_COMPRA( char celda_pinpad, char *nro_tarjeta,
		char tipo_cuenta, char *pin, int l_pin, int confirmar_importe, double importe );
	int       PINPAD_ACTUALIZAR_WORKING_KEY( char celda_pinpad, char *working_key );
	void      PINPAD_CARTEL(  );
	int		 PINPAD_PEDIR_WORKING_KEY(  );
	int       INIT_PINPAD(  );
	void      MOSTRAR_SALDO_EN_PINPAD( double importe );
	int       _WORKING_KEY( int ( *funcion ) (  ), char celda_pinpad, char *working_key );
	int       PINPAD_LEER_WORKING_KEY( char celda_pinpad, char *working_key );
	int       PINPAD_MENSAJES_ROTATIVOS( int cant, char *mens0, char *mens1, char *mens2,
		char *mens3, char *mens4, char *mens5, char *mens6 );
	int       PINPAD_MOSTRAR( char *cadena );
	int       PINPAD_PEDIR_PIN( char *nro_tarjeta, char *working_key, char *pin,
		int l_pin );
	int       PINPAD_PEDIR_TECLA( int timeout );
	int       PINPAD_SELECCIONAR_MASTER_KEY( int master_key );
	int       PINPAD_CANCEL_SESSION(  );
	int       PINPAD_SET_TIMEOUT( int timeout );
	int       PINPAD_CONFIRMAR( char *mensaje, int espera );*/

/*--------------------------------- Pedir ---------------------------------*/
	/*void      INIT_PEDIR( int x, int y, int largo );
	void      CLOSE_PEDIR(  );
	long      BOX_PEDIR_ENTERO( int x, int y, char *titulo, int digitos,
		int salvar_pantalla, int c_tit );
	void      SET_TECLA_EXCEPCION( int *teclas, void ( *funcion ) (  ) );
	void      TESTEAR_EXCEPCION( int *k );*/

/*------------------------------ EXT_TICK ----------------------------------*/
	/*double    GET_EXT_IMPUESTOS( int sub1, int sub2 );
   void      _DISCRIMINA_IMPUESTOS_EN_ARTICULO( double porc, int h, int *incrementar, char *procesar );*/

/*-------------------------------- DD ------------------------------------*/
	//char     *DD_PTR( int var );

/*-------------------------------- RNV -----------------------------------*/
	/*void      DRIVER_MEMORY( int comando, long offset, char *valor, int largo );
	void      DRIVER_MEMORY_VIRTUAL( int comando, long offset, char *valor, int largo );
	int       SET_RNV( int estado );*/

/*------------------------------- DBMEM ----------------------------------*/
	/*void      DBMEM_RECORRER( int cod, int ( *funcion ) (  ), char *datos );
	void      _DBMEM_RECORRER( int area, int ( *funcion ) (  ), char *datos );
	int       DBMEM_LOAD( int cod );
	void      DBMEM_GO( int registro );*/

/*----------------------------- on-line ----------------------------------*/
	/*int       ON_LINE_TARJETA( int h, long *nro, long anulacion );
	int       ON_LINE_PAGO_TARJETA( double importe, int anulacion, int tipo_cobro_tarjeta );*/

/*------------------------------ NETCOM -----------------------------------*/
	/*int       ABRIR_CANAL( int protocolo, int canal, char *ip, int formato_mensaje );
	int       CERRAR_CANAL( int protocolo, int canal );
	int       _ENVIAR_PAQUETE( int protocolo, char *paquete, int l_paquete, char *rta,
		int l_rta, int time_out_disponible, char espera_rta, int canal );
	int		 AGREGAR_TRANSACCION( struct _transaccion *tran, long nro, int nodo );
	void		 DEBUG_PAQUETE( char *archivo, char *paquete, int l_paquete );
*/
/*------------------------------ UTI-COMP -----------------------------------*/
	/*long      _tomar_long( char **cadena );
	int       _tomar_fecha( char **cadena );*/

/*------------------------------ INF_DIF -----------------------------------*/
// Movido a INF_DIF.h

/*----------------------------- JGATEWAY ------------------------------------*/
int       RESERVAR_PRODUCTO_JGATEWAY( int origen );
void      ELIMINAR_ARTICULOS_RESERVADOS( void );
int       CANCELAR_PRODUCTO_JGATEWAY( char *cod_barra, double importe );
/*------------------------------ CADAFECH  -----------------------------------*/
	//int       CADENA_A_FECHA( char *cadena );

/*------------------------------ IMPORT_T  -----------------------------------*/
	/*int       OBTENER_EVENTOS_TEMPORALES( int ev_requerido, int caja, long nro_ticket, long secuencia,
		int fecha, char origen );
    int       ABRIR_EVENTOS_TEMPORALES( struct _eventos *eventos,
	   struct _ev_cont *ev_cont, struct _ev_medios *ev_pago,
	   struct _ev_despro *ev_despro, struct _ev_dmedios *ev_dpago,
      struct _event_cod_barra *ev_cod_barra );
	int       VERIFICA_DATOS_IMPORTACION( int remoto, int ev_requerido, int fecha,
		long nro_ticket, int caja, long secuencia );
*/
/*------------------------------ PAGO_TAR -----------------------------------*/
	/*int       PIDE_AUTORIZACION_TARJETA( double importe );
	void      CONSULTA_SALDO_TARJETA_CREDITO(  );
	void      VALIDACION_TELEFONICA( double importe );
*/
/*------------------------------ D_ENTRY -----------------------------------*/
   /*int       DATA_ENTRY( void );
   void      GRABAR_EVENTO_DATA_ENTRY( void );
   void      BUSCAR_DATO_ADICIONAL_D_ENTRY( int orden, char *respuesta, int tam_resp );*/

/*------------------------------ IMP_ITEM -----------------------------------*/
int  IMPRIME_ITEM( int indice );
#define MAX_COD_IVA          10
#define _VUELTO_EN_EFECTIVO  1
#define _VUELTO_PROPIO       2
#define _VUELTO_OTRO_MEDIO   3

/*------------------------------ MEDIOS -----------------------------------*/
#define _EFECTIVO            1


/*------------------------------ tipos de movimiento -----------------------------------*/
#define _VENTA_NORMAL                0
#define _DEVOLUCION                  1
#define _COMPROMISO_200              2
#define _COMPROMISO_PRECIO_MAS_BAJO  3
#define _DIFERENCIA_PRECIO           4
#define _DIFERENCIA_PRECIO_ANULADA   5
#define _CAMBIO_DE_MERCADERIA_MENOS  6
#define _CAMBIO_DE_MERCADERIA_MAS    7

   // 24 k de memoria para pasar datos
#define     LONGITUD_MEMORIA_DRIVER_INVEL 0x6000

/*------------------------------- identificacion -----------------------------*/
#define IDENTIFICACION_MANUAL  0
#define IDENTIFICACION_BANDA   1
#define IDENTIFICACION_BARRA   2

/*------------------------------- identificacion -----------------------------*/
#define VERIFICAR_LIMITE_NO    0
#define VERIFICAR_LIMITE_AL_RENGLON  1
#define VERIFICAR_LIMITE_AL_FINAL    2

/*------------------------------- busqueda en boletin -----------------------------*/
#define _AL_INICIO    0
#define _SOLO_ERROR_COMUNICACION  1

/*---------------Ticket compra y mutuales en Boletines --------------------*/
#define NO_BUSCAR						0
#define BOLETIN_POSITIVO			1
#define BOLETIN_NEGATIVO			2
#define BOLETIN_POS_SIN_ACTUALIZ	3
#define BOLETIN_NEG_SIN_ACTUALIZ	4
#define BOLETIN_POS_CON_IMPORTE	5
#define BOLETIN_POS_CON_IMP_SIN_AC  7

/*---------------Accion en Boletines --------------------*/
#define _NINGUNA					0
#define _GENERAR_BOLETIN			1
#define _USAR_BOLETIN				2

/*-------------- Tipo devoluciones ----------------------*/
#define _DEVOL_RF		    2
#define _DEVOL_PERMA	    3

#define _NC_LIBRE		    1
#define _NC_TOTAL	        2
#define _NC_RF_PARCIAL	    3
#define _NC_RF_INTERME	    4
#define _DEVOL_LIBRE        5
#define _DEVOL_TOTAL        6
#define _DEVOL_PARCIAL      7
#define _DEVOL_INTERME	    8
/*-------------- Motivo devoluciones ----------------------*/
#define _MOTIVO_DEVOL_TOTAL		1
#define _MOTIVO_DEVOL_RF	    2
#define _MOTIVO_ANUL_ITEM		3
#define _MOTIVO_ANUL_TICKET	    4
#define _MOTIVO_DESCUENTO       5
#define _MOTIVO_DEVOL_COBRO     6

/*------------ Motivo Anulacion Ticket --------------------*/
#define _ANUL_TICKET_NORMAL						1
#define _ANUL_TICKET_SUSPENDIDO					2
#define _ANUL_ERROR_IMPRESORA					3
#define _ANUL_INCONSISTENCIA_FISCAL_SUSPENDIDO	4
#define _ANUL_INCONSISTENCIA_FISCAL_ANULADO		5
#define _ANUL_TICKET_NO_SE_AUTORIZA				6  
#define _ANUL_INCONSISTENCIA_DOC_CERRADO		7
#define _ANUL_CANCELADA							8  


/*-------------- IMP. COMPROBANTES ------------------------*/
//tabla 39 del item-tab.sic
#define _ESTACION_JOURNAL_RECEIPT 1
#define _ESTACION_JOURNAL         2
#define _ESTACION_SLIP            3

/*-------------- perfiles empleados el salvador -----------*/
#define _TIPO_PERFIL_NORMAL    0
#define _TIPO_PERFIL_EMPLEADO  1

/*-------------- posicion voucher de tarjetas -----------*/
#define _VOUCHER_POSTICKET	0
#define _VOUCHER_PRETICKET	1

/*-------------------------------------------------------*/
	//void      ERROR__( int nro, char *mensaje );
	//void      DD_EXEC( int comando, int var, int subindice, char *valor );
	//void      _DD_EXEC( int comando, int clase, long offset, char *valor, unsigned largo );
	//int       GRABA_SALDO_CHEQUE_CLIENTE( double tot_cheques );
	//void      SET_RAM( int estado );
	//void      NOM_ARCHIVO( int cod, char *destino );
	//void      TRIM( char *cadena );
   //void      LTRIM( char *cadena );
	//void      SET_ERROR_EN_RED( void );
	//int       AREA_DISPONIBLE( void );
	//int       AREA_OF( int cod );
	//int       SET_AREA( int cod, int area );
	//int       DD_LARGO( int var );
	//int       EST_COD( int var );
	//int       EST_VAR( int var );
	//long      DD_OFFSET( int var );
	//int       DD_ELEMENTOS( int var );
	//char      DD_CLASE( int var );
	//int       DD_TIPO( int var );
	//void      ABORT( char *mensaje );
	//int       LEER_TARJETA_MAGNETICA( unsigned char *track2, unsigned char *track1 );
	//long      PEDIR_ENTERO_XY( char *titulo, int x, int y, int digitos );
	//int       VERIFICA_CORTE_FIN_TICKET( int lineas, int lineas_corte );
	//int       EVENTO_A_PROCESAR( struct _eventos *ticket );
	//void      FIN_DOCUMENTO( int anula, double total );
	//void      ABRIR_DOCUMENTO( int strclave, long nro_comprobante );
	//void      CERRAR_DOCUMENTO( double total, int corte_papel, int lf );
	//int       VALIDAR_CHEQUE( int ind_medio, int sucursal, int nro_caja, long cajero,
	//	long nro_boleta, char *nro_rut_comprador, char *nro_rut_girador, int nro_banco,
	//	char *nro_cuenta, long nro_cheque, int nro_plaza, int tipo_credito, double monto,
	//	unsigned fecha_vencimiento, int habilitado );

/*------------------------------ PANT ---------------------------------------*/
	/*void      SET_ESTADO_CURSOR( int estado );
	void      DIBUJA_CURSOR( char estado );
	void      SET_SCREEN( int tipo_pantalla );
	void      CAJA( int x1, int y1, int x2, int y2 );
	void      LLENAR( int x1, int y1, int x2, int y2, unsigned char caracter );
	void      LLENAR_ATRIBUTO( int x1, int y1, int x2, int y2, unsigned atributo );
	void      BOX( int x1, int y1, int x2, int y2 );
		int color_titulo );
	int       SAVE_SCREEN( int x1, int y1, int x2, int y2 );
	void      RESTORE_SCREEN( unsigned num_pantalla );
	void      CURSOR_ON(  );
	void      CURSOR_OFF(  );
	void      PUT_GRAN_CHAR( int x, int y, unsigned char caracter );
	void      PUT_CHAR( int x, int y, unsigned char caracter );
	void      PUT_CURSOR( int x, int y );
	void      SCROLL( int x1, int y1, int x2, int y2, int dir );
	void      LINEA_HORIZONTAL( int x1, int y1, int x2 );
	void      ACTUALIZA_CURSOR(  );
	void      SET_POSICION_CURSOR( int x, int y );
	void      GET_POSICION_CURSOR( int *x, int *y );
	void      INIT_FONTS(  );
	void      CLRSCR( int color );
	void      SALVA_CURSOR(  );
	void      RESTAURA_CURSOR(  );*/

/*------------------------------ P_FECHA-------------------------------------*/
	/*int       PEDIR_FECHA( int x, int y, char *str );
	int       PEDIR_FECHA_MMAA( int x, int y );
	long	  PEDIR_HORA_LARGA( int x, int y, char *str );
	long	  CADENA_A_HORA( char *cadena );
	int		  VALIDA_HORA_LARGA( char *buffer );*/

/*------------------------------ P_FLOAT ------------------------------------*/
/*#if defined(INVEL_W) || defined(INVEL_L)
	double    PEDIR_FLOTANTE( int x, int y, int enteros, int decimales );
#else
	double    PEDIR_FLOTANTE( char x, char y, char enteros, char decimales );
#endif
	double    PEDIR_FLOTANTE_COD_BARRA( char x, char y, char enteros, char decimales,
		double _default, char **cod_barra );
	double	 PIDE_IMPORTE_T_COMPRA( char x, char y, char enteros, char decimales,
		char cod_t_compra, char pos, char **output_cadena_scanner );
   double    PEDIR_FLOTANTE_CON_VALOR( int x, int y, int enteros, int decimales,
		double defecto );*/
/*------------------------------   STACK  -----------------------------------*/
	//void      CLOSE_RECUPERO( int ptr, int handle );
	//void      OPEN_RECUPERO( int *ptr, int *handle );
	//int       STACK_INS( int bytes );

/*------------------------------    RNV   -----------------------------------*/
	/*void      OPEN_RNV_VIRTUAL(  );
	void      CLOSE_RNV_VIRTUAL(  );*/
/*------------------------------ GET_DOW  -----------------------------------*/
   /*int       GET_DOW(  );
   void      CDOW( unsigned fecha, char *cadena );
   int       CARGAR_CONF_VALIDA(  );
	int       GET_DIA_DE_LA_SEMANA(  );*/
/*------------------------------ GET_HORA -----------------------------------*/
	//int       GET_HORA_DOS(  );
/*------------------------------ L_VALID  -----------------------------------*/
	//int       EJECUTAR_L_VALID( char *cadena, int f_valid );

/*------------------------------ TAR_MAG ------------------------------------*/
	/*int       INIT_TARJETA_MAGNETICA(  );
	void      LEER_TARJETA_MAGNETICA_OMRON( unsigned char *track2, unsigned char *track1 );
	int       LEER_TARJETA_MAGNETICA( unsigned char *track2, unsigned char *track1 );
	int       LECTOR_BANDA( int tecla );
	int       LECTOR_TARJETA_LEIDA(  );
	void      LIMPIA_BUFFER_TARJETA( int cerear );
	int       LEER_TARJETA_BANDA( unsigned char *buffer, unsigned char *buffer2 );
	void      CARGA_TRACKS_EN_RNV(  );
	int       STRLEN( char *men, int cant );*/

/*------------------------------  CR_PANT -----------------------------------*/
	//void      MUESTRA_ET_TOMRA(  );
//	void      LIMPIAR_RECUADRO_PETICION( char *titulo );
	//void      LIMPIAR_RECUADRO_PETICION( int mensaje );
	//void      MOSTRAR_MODO_DEVOLUCION(  );
	/*void      DIBUJAR_RECUADRO_PETICION( char *titulo, int digitos );
	void      BORRA_RECUADRO_PETICION( int x1, int x2, int xc1, int xc2 );
	long      PEDIR_ENTERO_EN_RECUADRO( char *titulo, int salvar_pantalla, int digitos );
	double    PEDIR_FLOAT_EN_RECUADRO_CS( char *titulo, int salvar_pantalla, int enteros,
		int decimales, int tipo );
	void      CR1_CONFECCIONA_PANTALLA(  );
	void      DIBUJA_RECUADRO_DEL_TICKET(  );
	void      LIMPIA_RECUADRO_DEL_TICKET(  );
	void      CR1_DISPLAY_DE_PANTALLA(  );
	void      BORRA_DISPLAY_DE_PANTALLA(  );
	void      CR1_MUESTRA_MULTIPLICACION(  );
	void      CR1_MUESTRA_ARTICULO_EN_GRANDE(  );
   void      CR1_MUESTRA_ARTICULO_EN_GRANDE( );
   void      __CR1_MUESTRA_ARTICULO_EN_GRANDE( int item );
	void      MUESTRA_BULTOS(  );
	void      MUESTRA_SUBTOTAL(  );
	void      MUESTRA_CARTEL_INICIAL(  );
	void      MUESTRA_ITEM_EN_PANTALLA( int nro );
	void      MUESTRA_DATOS_CONTADOR_PANTALLA( int nro, int MostrarDatos );
	void      CR1_MUESTRA_TARJETA(  );
	void      MUESTRA_MUTUAL(  );
	void      MUESTRA_CLIENTE( int cheque );
	void      MOSTRAR_CLIENTE_EN_PANTALLA(  );
	void      MOSTRAR_PETICION_DE_PRESENTACIONES( int presentaciones );
	void      MOSTRAR_PETICION_DE_CODIGO( int pedir_cod_especial );
	void      MUESTRA_TIPO_PRECIO(  );
	void      MOSTRAR_MODO_CAMBIO(  );
	void      MOSTRAR_MOZO(  );
	void      MOSTRAR_VENDEDOR(  );
	void      MOSTRAR_CLIENTE_AFINIDAD(  );
	void      MUESTRA_COMPROBANTE(  );
	void      REGENERAR_PANTALLA_TICKET(  );
	void      MUESTRA_HORA_EN_PANTALLA(  );
	void      MUESTRA_TIPO_DEVOLUCION(  );
	long      PEDIR_ENTERO_XY( char *titulo, int x, int y, int digitos );
	void      MUESTRA_ERROR_EN_RED( void );
	int       PEDIR_FECHA_EN_RECUADRO( char *titulo, int salvar_pantalla );
	int       PEDIR_CADENA_EN_RECUADRO( char *titulo, int salvar_pantalla, int digitos,
		char *cadena, char *mascara_scanner );
	double    PEDIR_FLOAT_EN_RECUADRO_CC( char *titulo, int salvar_pantalla, int enteros,
		int decimales, int cajero, int sup, int cond, int cajon, int controlar_efectivo,
		int funcion );
	double    PEDIR_FLOAT_EN_RECUADRO( char *titulo, int salvar_pantalla, int enteros,
		int decimales );
	int       PEDIR_CADENA_XY( char *titulo, int x, int y, int digitos, char *cadena );
	void      SET_BORRAR_RECUADRO_DE_PETICION( int borrar );
	void		 GUI_MUESTRA_TARJETA( int operacion );
//	void		 MUESTRA_REFERENCIA_NC(  );
	double 	 PEDIR_FLOAT_EN_RECUADRO_CON_VALOR( char *titulo, int salvar_pantalla, int enteros,
	int decimales, double valor );
	double 	 PEDIR_IMPORTE_XY( char *titulo, int x, int y, int enteros, int decimales );
    void VERSION_DIALOG( void );*/
    //void MOSTRAR_ANULACION( void );


/*------------------------------   IO   ------------------------------------*/
	/*int       _GET_ERROR_EN_RED(  );
	void      _VERIFICAR_REINTENTO(  );*/

/*------------------------------CLIENTES------------------------------------*/
	/*int       PEDIR_NRO_CLIENTE( int recuadro );
	void      MOSTRAR_CLIENTE( int x, int y, long nro );
	int       EXISTE_CLIENTE( long cliente );
	int       BUSCAR_CLIENTE_POR_CUIT( char *cuit );
	void      ACTUALIZA_SALDO_CLIENTE( double importe );
	int       VERIFICA_CUPO_CLIENTE( double importe, double importe_ant, char cobro, char modo );
	int       VERIFICA_SALDO_CHEQUE( double importe );
	int       VALIDA_CLIENTE(  );
	int       GRABA_SALDO_CHEQUE_CLIENTE( double tot_cheques );
	int       ACTUALIZA_CLIENTES( void );
	void      VERIFICAR_CLIENTE_HABILITADO_PARA_CHEQUES(  );
	int       VERIFICA_EMPADRONAMIENTO_CLIENTES( int recuadro );
	void      IDENTIFICAR_CLIENTE_POR_CONSULTA(  );
	int       PEDIR_DATOS_CLIENTE( void );
	int       ACEPTAR_CLIENTE(  );
   void      CEREAR_PERFIL_CLIENTE( void );
   int       ALTA_CLIENTE( void );
   void      CARGAR_PERFIL_CLIENTE( int cod_perfil );
   int       SALIR_ALTA_CLIENTE( void );
   int       ASIGNA_CONDICION_IVA( char *cadena  );*/

/*------------------------------ SCANNER  ------------------------------------*/
	/*int       INIT_SCANNER(  );
	int       LEER_SCANNER( unsigned char *buffer, int l_buffer );
	int       CRC_SCANNER( char *buffer, int largo, int empieza_impar );
	int       IS_SCANNER(  );
	void      LIMPIAR_SCANNER(  );
	void      SET_RTS_SCANNER( int estado );
	int       CALCULA_CRC_EAN( char *buffer, int largo, int empieza_impar );
	void      CALCULA_CRC_EAN13( char *buffer );
	int       LEER_CADENA_SCANNER( char *cadena, int largo );
	int       SCANNER_LIBRE(  );
	int		 SCANNER_POR_TECLADO( int tecla );*/

/*------------------------------ COMPROB  ------------------------------------*/
	/*int       ABRIR_ARCHIVOS_FACTURAS( int clase, char *buffer );
	int       AGRUPAR_ARTICULOS(  );
	void      APUNTA_CFG( int tipo );
	void      CALCULAR_RETENCIONES(  );
	int       CANTIDAD_FORMULARIOS( int renglones );
	void      COMP_SET_FILA( int _fila );
	void      COMP_ajustar_largo( char *cadena );
	void      COMP_ejecutar_comando( int *ind_config );
	int       COMP_evaluar_condiciones( int codigo );
	INT16  COMP_tomar_entero( int *ind_config, int *indice );
	void      COMP_total_en_letras( int *ind_config );
	int       DATOS_COMPROBANTE( int tipo );
	void      EJECT(  );
	void      EJECT_FISCAL( int habilita );
	int       EMITIR_FACTURA(  );
	int       EMITIR_FACTURA_REMOTA(  );
	char     *FORMATEA_NRI( char *nro_nri );
	int       ES_COMPROBANTE( int tipo );
	int       ES_FACTURA_A( int cond_cli );
	int       ES_FACTURA_B(  );
    int       TIPO_COMPROBANTE( int *subtipo, int cond_cli );
	int       OBTENER_COND_IVA_CLIENTE(  );
	void      GRABA_EVENTO_ANULACION( long nro, unsigned evento, long nuevo_nro, long act_ram_id_evento );
	void      IMPRIME_RETIRAR_FACTURA( double total );
	void      IMPRIMIR_CADENA( int x, char *cadena );
	int       IMPRIMIR_COMPROBANTE( int tipo, int reimpresion );
	void      IMPRIMIR_DETALLE_FACTURA_EN_JOURNAL(  );
	void      IMPRIMIR_TICKET_FACTURA( int cant );
	void      IMPRIMIR_TICKET_NC(  );
	void      INIT_COMPROBANTE( int tipo );
	int       MODO_FACTURACION_ESPECIAL(  );
	void      MOSTRAR_NRO_FACTURA(  );
	int       OBTENER_DATOS_COMPROBANTE( int clase, int tipo, int cant, short int *nro_suc,
	long      *nro_comp, short int *copias, int actualizar_nro, int pedir_nro );
	int       OBTENER_DATOS_RETENCIONES(  );
	double    PERCEPCION( double importe_neto, char verificar );
	double    PERCEPCION_212_IVA_1(  );
	double    PERCEPCION_212_IVA_2(  );
	void      POSICIONA_EN_COMPROBANTE( void );
	double    RETENCION( double importe_neto, char verificar );
	void      VALIDAR_REIMPRESION_FACTURA( long comprobante, long cuenta );
	double    VARIACION_PERCEPCION_212( double importe_neto );
	int       VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo );
	char     *FORMATEA_CUIT( char *nro_cuit );
	char     *OBTENER_TIPO_IVA( int cond_iva );
	void      CALCULAR_RETENCIONES_TOTALES(  );
	int		 OBTENER_FLAG_COMPROBANTE( int cond_cli, int comprob );
	void		 VALIDAR_FACTURA_REMOTA( double total, char *nombre_cliente, int nro_cliente );
	void  	 CALCULAR_VALOR_CADA_ARTICULO_RESTANDO_DESCUENTOS( int renglon );
	void		 GRABA_EVENTO_COMPROBANTE_ANULADO( long nro, long id_evento );
   int       IMPRIMIR_COMPROBANTE_JOURNAL( int tipo, int reimpresion );
   void      IMPRIMIR_CABECERA_FACTURA_JOURNAL( int bandera );
   void      IMPRIMIR_PIE_FACTURA_JOURNAL( void );
   int       BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( int posicion, char *cadena );
   void      CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( int cod_sol, int nro_orden,
         long id_evento, int bin, int origen, int cant_max, int posicion );
   int ES_CREDITO_FISCAL( int ev );

   double    SUMAR_TOTAL_FAC_CAJA(void );
   void      AGREGA_BIN_A_EXTENDIDO( int h, int origen, char anulado );
   int       INSERT_A_BTRV_EXT( struct _datos_en_bin *datos_en_bin, char anulado, int posicion );*/

/*------------------------------ TARJETA  ------------------------------------*/
	/*long      _CALCULAR_NRO_CUPON( char marca );
	void      CADENA_NRO_TARJETA( char *buffer, char *nro, char marca );
	int       OBTENER_COD_SERVICIO( char *buffer );
	void      NRO_TARJETA( char *buffer, long *n1, long *n2, char marca_especial );
	void      FECHA_VTO_TARJETA( char *buffer, unsigned *fecha );
	int       PEDIR_DATOS_TARJETA( char *cadena, char marca_especial );
	int PEDIR_CUOTAS( int marca, int financia_tarjeta, unsigned short cod_medio );
	int       PEDIR_FINANCIA_TARJETA(  );
	int       PEDIR_MONEDA( int marca, int financia_tarjeta, int consulta_saldo );
	int       PEDIR_TIPO_CUENTA( int marca );
	int       PEDIR_CVV(  );
	int _elegir_tar_esp( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
	int       PIDE_MARCA_ESPECIAL(  );
	long      PEDIR_NRO_CUPON( int marca, int cuotas, int financia_tarjeta );
	long      CALCULAR_NRO_CUPON( char _marca );
	void      DECREMENTAR_NRO_CUPON( char marca );
	int       VALIDA_TARJETA( int consulta_saldo, int consulta_boletin, int operacion, int hab_promo,
		char *mensaje, int origen_ventas );
	int PIDE_DATOS_TARJETA( int consulta_saldo, unsigned short cod_medio );
	void      NUMERO_DE_COMERCIO_TARJETA(  );
	int       AUTORIZA_TARJETA_EN_BOLETIN(  );
	int       VALIDA_TARJETA_NUEVA(  );
	int       VALIDA_FECHA_VTO( unsigned fecha_vto );
	int       MONEDA_EXTENDIDA(  );
	int       TARJETA_CON_CECOR( int marca );
	int _elegir_cuotas( int comando, char *cadena, int dato, struct _browse *b, double monto2, unsigned short cod_medio );
	int _elegir_cuotas_grupo( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
	int 		 _elegir_grupo( int comando, char *cadena, int dato, struct _browse *b );
	int ELEGIR_CUOTAS( int marca, unsigned short cod_medio );
	int _ELEGIR_CUOTAS( int marca, int grupo, unsigned short cod_medio );
	int ELEGIR_GRUPO_CUOTAS( int marca, unsigned short cod_medio );
	void GO_CUO_TAR( int modo, unsigned short cod_medio );
	void      GO_CUO_TAR_GRU( int modo );
	void 		 GO_CUO_GRU( int modo );
	int       VALIDA_LARGO_NUMERO( int marca, int largo );
	long      NRO_TERMINAL( int marca );
	int       LEE_CUO_TAR( int marca, int cuotas );
	int       CAJA_HABILITADA_PARA_CUOTAS(  );
	int _elegir_vigencia( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
	int       ELEGIR_VIGENCIA( int marca );
	void      GO_VIGENCIA( int modo );
	int       BUSCAR_VIGENCIA( int cod_tarjeta, int cod_vigencia, char *nom );
	double    PIDE_ENTREGA( double importe );
	double    _PIDE_ENTREGA( char *titulo, int enteros, int decimales, int signo );
	int       CONFIRMAR_NUMERO_TARJETA( int marca );
	int       PEDIR_CUPON_ORIGINAL(  );
	int       PEDIR_FECHA_ORIGINAL(  );
	int       BUSCAR_EN_BOLETIN( int marca );
	void      GRABA_MODIFICACION_TARJETAS( int posicion, int id );
	int       LEE_TARJETAS( int posicion, int id );
	int       PEDIR_HORA_ORIGINAL(  );
	int       PEDIR_AUT_ORIGINAL(  );
	int       CANTIDAD_TARJETAS_DEBITOS_INGRESADAS(  );
	int       PEDIR_TASA_APLICADA(  );
	int       PEDIR_IMPORTE_CUOTA(  );
	void      ENMASCARA_NUMERO_TARJETA( int marca, char *nro_cadena, char *temp );
    void      PEDIR_MARCAS_TARJETA( int medio );
	int		  VALIDAR_MODO_INGRESO_TARJETA( int marca, int operacion );*/
	//char      HABILITAR_VOUCHER_MEDIO( int submedio );

/*------------------------------ INC_MES  ------------------------------------*/
	//int       INCREMENTAR_MES( unsigned fecha, unsigned meses );

/*------------------------------ PG_FECHA  ------------------------------------*/
	/*int       PEDIR_GRAN_FECHA( int x, int y );
	int       PEDIR_GRAN_FECHA_MMAA( int x, int y );*/

/*------------------------------    MENU   ------------------------------------*/
/*#if defined(INVEL_W) || defined(INVEL_L)
//	int       MENU( int x, int y, char *titulo, struct _menu *items, int n_items,
//		int c_fondo, int c_tit );
extern int MENU( char* titulo, struct _menu *items, int nitem, struct _cabmenu * cab, int colum );
extern int MENU_PRESENT( char* titulo, struct _menu *items, int nitem, struct _cabmenu * cab, int colum );
#endif*/
/*------------------------------   LISTA   ------------------------------------*/
	/*void      INICIALIZAR_TARJETAS(  );
	void      INICIALIZAR_BUSCA_TARJETA(  );
	void      FIN_BUSCA_TARJETA(  );
	void      LISTA_CARGA_INDICE( char nro );
	int       VALIDA_NUMERO_TARJETA( char *num_tarjeta, char marca );
	int       VALIDA_BOLETIN( char nro );
	int       BUSCA_TARJETA( char *nro_tarjeta, char cod_marca );
	int       f_cmp_busca_tarjeta_cab1( const void *_p1, const void *_p2 );
	int       f_cmp_busca_tarjeta_cab2( const void *_p1, const void *_p2 );
	int       f_cmp_busca_tarjeta_lista( const void *_p1, const void *_p2 );
	int       DEBE_PEDIR_CUPON( int marca, int cuotas, int financia_tarjeta );
	int       CARGAR_TARJETAS(  );
	int       NUMERO_BOLETIN( char nro );
	int       NUMERO_ARCHIVO( int marca );
	char     *NOMBRE_TARJETA( int marca );
	float     LIMITE2_TARJETA( int marca );
	int       TARJETA_ON_LINE( int marca );
	int       IMPRIME_MENSAJE_TARJETA( int marca );
	int       DEBE_PEDIR_COD_VIGENCIA( int marca );
	int       NODO_TARJETA( int marca );
	int       COD_VALIDACION_VOUCHER( int marca );
	int       BUSCA_MARCA_TARJETA( long numero, char *cod_tarjeta, float *limite );
    int       BUSCA_MARCA_TARJETA_SERVICIOS( long numero, char *cod_tarjeta, float *limite );
	int       EXISTE_TARJETA( int marca );
	int       BOLETINES_INICIALIZADOS(  );
	void      BUSCA_MARCA_TARJETA_ESPECIAL( char *cod_tarjeta );*/
	//int		  EXISTE_PREFIJO_BARRA_TARJETA( char *bin, long prefijo, int *l_tarj );

/*------------------------------ INC_YEAR  ------------------------------------*/
	//int       INC_YEAR( unsigned fecha, unsigned anios );

/*------------------------------ PAGO_TAR  ------------------------------------*/
	/*int       EXIGE_AUTORIZACION( double importe );
	int       PIDE_AUTORIZACION_SI_NECESITA( int h, int pedir_aut );
	int       EXIGE_AUTORIZACION_ON_LINE(  );
	int       ON_LINE( int h, int dev_cobro );
	int       COMPRA_OFF_LINE( int h, int pedir_aut, int dev_cobro );
	int       MOSTRAR_RESULTADO_ON_LINE( int autoriza, char *mensaje );
	void      CAMBIA_TARJETA_CREDITO(  );
	void      GRABAR_RESULTADO_ON_LINE( int ok );
	int       PIDE_AUTORIZACION_TARJETA( double importe );
	void      REPROCESAR_TARJETAS( char tipo_pago, int actualizar );
	long      PIDE_NRO_CUPON( int digitos );
	void      IMPRIME_DATOS_TARJETA( long cupon );
	int       EN_CUOTAS(  );
	int       SUPERA_LIMITE_LIBERTAD( int h );
	int       PIDE_CLAVE_EN_PINPAD( int cartel, int confirmar_importe, double importe );
	void      CONSULTA_SALDO_TARJETA(  );
	void      PEDIR_SALDO_ON_LINE(  );
	//void      _ANULAR_TARJETAS( int cuantas );
	void _ANULAR_TARJETAS( int cuantas, int AnulacionIndividual )
	double    ENVIAR_PAQUETE_AFINIDAD( char tipo_operacion, long cliente, double puntos );
	int       ON_LINE___TARJETA( char tipo_operacion, struct _rta_terminal *rta_terminal,
		double importe );*/

/*------------------------------  COMTAR   ------------------------------------*/
	/*void      GRABAR_COMTAR( char marca, char *nro_tarjeta, char anulacion );
	int       BUSCAR_COMTAR( char marca, char *nro_tarjeta );
	int       VERIFICAR_TARJETA_EN_COMTAR( char marca );*/

/*------------------------------ PERFILES  ------------------------------------*/
	//int       RECORRER_CLASIF1( struct _clasif1 *clasif1, struct _datos_recorrer *datos ) ;
	/*int       MENU_PERFILES(  );
	void      INGRESA_PERFIL(  );
	int       ACTUALIZA_NOM_PERFIL(  );
	long      COD_DESCUENTO_PERFIL(  );
	void      MUESTRA_PERFIL(  );
	int       ABRIR_PERFILES(  );
	void      CERRAR_PERFILES(  );
	float     CALCULA_PORC_PERFIL( long cod, long cod_clasificacion );
	float     PORC_DESC( int cod );
	int       INGRESA_NRO_LEGAJO_EMPLEADO(  );
   int       ASIGNA_PERFILES( char *cadena );
   int       TIPO_PERFIL( void );*/

/*------------------------------   CLAVE   ------------------------------------*/
	/*char     *MK_FP_BIOS_PROTEGIDO( unsigned off );
	int       PIDE_SUPERVISOR( int x, int y );
	long LEER_NUMERO_USUARIO( int longitud_a_tomar );
	int       INGRESAR_CLAVE_MANUAL( int x, int y, int tipo, int cajero, int sup );
	int       INGRESAR_CLAVE_PCOS( int x, int y, int tipo );
	int       INGRESAR_CLAVE_GP416( int x, int y, int tipo, int cajero, int sup );
	void      CODIFICA_CLAVE( char *buffer, char *codigo );
	int       LEE_CLAVE_CAJERO( int nro, char *buffer );
	int       BUSCA_CAJERO_X_LEGAJO( long legajo );
	int       CLAVE_ANTERIOR(  );
	int       TIPO_USUARIO(  );
	void      CLAVE_ANTERIOR_GP( char *buffer );
	int       USUARIO_CLAVE_ANTERIOR(  );
	int       SOLICITAR_CLAVES( int x, int y, int cajero, int supervisor, int condicion,
		char *titulo, char *log, int funcion, int grabar_en_event );
	int       VERIFICAR_CLAVE_EN_ARCHIVO( int clave, int tipo, int cajero, int sup );
	int       LEER_TARJETA_CONNAC( int x, int y, int tipo, int usuario_esperado );
	int       BCD_A_INT( unsigned char *buffer );
	long      BCD_A_NRO_USUARIO( unsigned char *buffer );
	int       BCD_A_CHAR( unsigned char buffer );
	int       PREPARA_GP416(  );
	int       INGRESA_GP416(  );
	void      SACA_GP416(  );
	long      OBTENER_USUARIO( long usuario_esperado );
	int       OBTENER_TIPO( int tipo_esperado );
	int       OBTENER_CLAVE( int clave );
	int       USUARIO_PUEDE_RETIRAR( int nro );
	int       INGRESAR_CLAVE( int x, int y, char *mensaje, int tipo, int cajero,
		char *titulo, int funcion, int pant_inicial, int grabar_en_event );
	int       LEE_CAJERO( int nro, struct _cajeros *datos_cajero );
	void      MOSTRAR_CAJERO_EN_PANTALLA(  );
	long      __BCD_A_NRO_USUARIO( unsigned char *buffer );
   long      PIDE_SUPERVISOR_LONG( int x, int y );
	int		 BUSCA_CAJERO_X_DNI ( long dni );
   int       BUSCA_CAJERO_X_CODIGO( int nro );*/


/*------------------------------  DESCUENT  ------------------------------------*/
	/*int       AGREGAR_DESCUENTO_IMPORTE( int id );
	void      AGREGAR_DESCUENTO_PORCENTUAL(  );
	void      TECLA_DESCUENTO_PORCENTUAL( unsigned desc );
	void      ACUMULA_IMPUESTOS_ITEM( int h, struct _acu_iva *acu, double cantidad,
		int flags_impuestos, int cod_iva );
	void      MUESTRA_DESCUENTO_PORCENTUAL_FIJO(  );
	void      IMPRIMIR_BASE_DESCUENTO( unsigned desc );
	double    CALCULAR_BASE_DESCUENTOS( unsigned desc, double porc, int incondicional,
		int posicion, int acumulativo );
	int       _CALCULAR_BASE_DESCUENTOS( int desc, int h, int incondicional, double porc,
		double *total, double *tot_aux, double *bases_descuento, int acumulativo );
	void      APLICAR_DESCUENTO( char desc );
	void      APLICAR_DESCUENTOS_PENDIENTES( void );
	void      REVERTIR_DESCUENTOS_FINAL_TICKET( void );
	struct _acu_iva *CALCULAR_IMPORTE_IMPUESTOS( struct _acu_iva *acum, double porc );
	struct _acu_iva *ACUMULAR_IMPUESTOS_TICKET( int disc_departamentos, unsigned desc,
      int flags_impuestos, int cod_iva, int disc_articulo_habilitado );
	void      AGREGAR_DESCUENTO_ULTIMO_ARTICULO( int descuento_importe );
	int       ELEGIR_MOTIVO( int *cod_solicitud );
	void      MOSTRAR_DESCUENTOS_EN_PANTALLA( void );
	void      ELIMINA_BASE_DTO( int desc );
	void      SET_FILTRO_ORIGEN_ARTICULO( int origen );
	void		 AGREGAR_DESCUENTO_PORC_CLIENTE( float porc_desc );
	int		 CARGA_BASE_DESCUENTO( int desc, double *base_aux );
   int       ELEGIR_MOTIVO_RETIRO( int *cod_solicitud  );
   int       VERIFICAR_MOTIVO( int tipo_verificacion, int *cod_solicitud, short *supervisor, int cod_motivo );*/

/*------------------------------  CR_DISCO  ------------------------------------*/
   //Movido a CR_DISCO.h
/*------------------------------   TCP_IP   ------------------------------------*/
	//int       _ENVIAR_PAQUETE_TCP_IP( char *paquete, int l_paquete, char *rta, int l_rta,
	//	int timeout_nodo, char espera_rta, int canal );
	//int       INIT_DRV_TCP_IP( void );
	//int       CERRAR_CANAL_TCP_IP(  );
	//int       ABRIR_CANAL_TCP_IP( int canal, char *ip, int formato_mensaje );
	//int       PING_ABRIR_SOCKET( char *ip_addres );
	//int       PING_CLOSE_SOCKET(  );
	//int       TCP_ftp( char *argv[] );
	//int       FTP_ARCHIVO( char *origen, char *destino, int get_put, int modo );
	//unsigned  TCP_canal_puerto( int canal, int puerto );

/*------------------------------    _CR1    ------------------------------------*/
	/*int       CR1(  );
	void      CR1_LAZO_PRINCIPAL(  );
	void      CR1_ACTIVA_ANULACION(  );
    int       CR1_EXISTE_ITEM( long cod_especial, int presentacion, char *cod_barra );
	void      FIN_DE_TICKET( int script );
	int       CR1_AUTORIZA_ANULACIONES(  );
	void      BORRAR_CONTADOR_ARTICULO( );
	void      CR1_INFORMES(  );
	void      CR1_VALIDA_TARJETA( int mensaje );
	int       CR1_CIERRE(  );
	void      PETICION_DE_CODIGO( int status );
	void      SWITCH_PETICION_DE_CODIGO(  );
	void      CR1_PETICION_DE_PRESENTACIONES(  );
	void CR1_FACTURA( int nc, int tipo  );
    int       VERIFICA_EVENTO_NC_DEV( char tipoEvento );
	float     CR1_PEDIR_PESO(  );
	float     CR1_PEDIR_PRECIO(  );
	void      CR1_HABILITA_SCANNER( int clave );
	void      CR1_INICIALIZAR_VAR_TICKET(  );
	int       CR1_AUTORIZA_PASAR_LIMITE(  );
	int       CR1_ELEGIR_PRESENTACION(  );
	void      CR1_ADOPTA_PRESENTACION( int nro, int inicial );
	void      CR1_CALCULA_RECARGO(  );
	void      CR1_CALCULA_IVA( void );
	int       CR1_BUSCA_CODIGO( int enganche );
	int       CR1_EJECUTA_FUNCION( int funcion );
	int CR1_AGREGA_ITEM( int enganche, char origen, int tipo, char tipo_evento_orig );
	void      ANULA_ENTRADA(  );
	void      AGREGA_ITEM_AL_TICKET( int h, int resta, int enganche, int grabar,
		char origen );
	int       CR1_PIDE_CODIGO(  );
	void      SETEAR_FUNCION( int funcion, int condicion );
	int       ES_ENVASE( int h );
	void      CR1_INTERESES(  );
	void      CODIGO_INEXISTENTE(  );
	int       INGRESA_CODIGO_BARRAS(  );
	void      HABILITAR_PRECIO_CR(  );
	void      EVALUAR_CODIGOS_ESPECIALES(  );
	int       EVALUAR_CODIGOS_BALANZA( char *id_balanza );
	void      CAMBIA_ESPACIO_POR_CERO( char *m );
	void      ARMA_COD_BARRA( char *cod_barra, double precio );
	int       CONDICIONES_PARA_CIERRE(  );*/
	void GRABAR_CABECERA_EVENTO_REIMPRESION( int valor );
	/*void      RECALCULA_ITEM_POR_PRECIO( double importe );
	int       AGREGAR_ARTICULO( long cod, double importe, char origen, int tipo );
	void      TECLA_NOVIOS(  );
	int       EXENTO_COMO_GRAVADO( int tipo );
	void      CR1_ABRIR_CAJON(  );
	void      REPINTAR_PANEL(  );
	int       CR1_ITEM_GRAVADO(  );
	void      CR1_PUEDE_ANULAR_LINEA( long cod_especial, int presentacion );
	void      DESCUENTO_T_CHIP(  );
	int       CR1_EXISTE_DESCUENTO_CANTIDAD( long cod_especial, int presentacion );
	int       BUSCA_ITEM_PROMO(  );
	void      CR1_EXENTO_ESPECIAL( int tipo );
	void      CR1_TICKET_FACTURA( void );
	void      CR1_INCONSISTENCIA_FISCAL( int sumar );
	int       CR1_VERIFICAR_LIMITES_FACTURACION( double total, int modo );
	void      MODALIDAD_BIEN_DE_USO( void );
	void      MOSTRAR_MODOA_BIEN_DE_USO_EN_PANTALLA( void );
	void      CONSULTA_DE_ARTICULO( int muestra_promociones );
	char     *BUSCA_MENSAJE_STRING( int id );
	void      MENSAJE_SIN_CMR(  );
	void      EVALUAR_COD_TEMPLATE(  );
	void      TOMAR_CANTIDAD_SCANNER(  );
	void      MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA( void );
	int       BUSCA_ID_PROMO( int cod_promo );
	char     *ST( int id );
   int       CR1_BUSCAR_ITEM_ORIGINAL_POR_DATO_ADICIONAL( int datos_adicionales );
   int       MOSTRAR_SIN_IVA_ELSALVADOR( void );
   void      CR1_FACTURA_TIPO( int nc, int tipo );
   int		 CR1_EXISTE_ITEM_DIF_PRECIO( long cod_especial, int presentacion, int anulacion, char *cod_barra );*/
   /*void	     IMPRIMIR_COPIA_TICKET_FACTURA( );
   void      CR1_FRANQUEO( double importe );*/
  char     *MASCARA( char digito, int cantidad, char *valor );
  /* void      CEREAR_DATOS_COMPROB_ANTERIOR( void );
   void		CR1_CONTADOR_INIT( );
   int      CR1_CONTADOR_ARTICULO( long cod_especial, char *cod_barra, int item );
   void      GUARDAR_DATOS_PROD_RESERVADOS( long id_transaccion, char *nro_linea,
        double monto, long valor );
   int       OBTENER_COD_ESPECIAL_COBRO( int cod_cobro );
   int CR1_EXISTE_ITEM_ESTRICTO( long cod_esp, int presentacion, char *cod_barra,
        int comp_por_importe );
   void NOMBRE_MEDIO_SUBMEDIO( int medio, int submedio, char *nombre, int largo );
   void PROCESAR_PROMOCIONES_EN_TICKET_EN_ESPERA( void );*/

/*------------------------------     TRANS     ------------------------------------*/
	/*int       PEDIR_TRANSFERENCIA(  );
	void      FIN_TRANSFERENCIA( double importe, int modo );
	int       PEDIR_DATOS_TRANSFERENCIA(  );*/

/*------------------------------   CAMBIO_M    ------------------------------------*/
	/*double    CAMBIO_PIDE_IMPORTE( int datos_adicionales, int pos );
	void      CAMBIO_MOSTRAR_MEDIO( int modo, double importe, long numero, int anulacion );
	void      _GRABA_EVENTO_CAMBIO( int medio, double importe, int id_registro,
		char cod_sub_medio, int tipo, int h );
	int       VALIDA_ANULACION_MEDIO( int medio, int submedio, double importe, int tipo );
	void      CAMBIO_AGREGAR_MEDIO( int modalidad, int tipo, long numero, int banco, int fecha_vto,
		int c_postal_c, int c_hora_c, char *cuit, char *telefono, char *cuenta_banco,
		int tarjeta, char *numero_tar, long cod_sub_medio );

	void      CAMBIO_MEDIOS( int cambio_medio_automatico );
	void      CAMBIO_CONFECCIONA_PANTALLA( int tipo );
	void      CAMBIO_INGRESA_MEDIO( int nro, int tipo );
	void      CAMBIO_ACUMULAR_CAMBIO_EN_X( int modo, double importe, int anulacion,
		int tipo );
    void CAMBIO_GUARDAR_SUBMEDIOS_REC( int medio, int submedio, double importe,
        int anulacion, int tipo );
	void      CAMBIO_ACTIVA_ANULACION(  );
	void      CAMBIO_IMPRIME_MEDIO( int tipo );
	void      CAMBIO_SET_ACU_RETIRADO(  );
	void      GRABA_EVENTO_CABECERA_CAMBIO(  );
	void      CAMBIO_MUESTRA_TOTAL( int tipo );
	int       CAMBIO_PIDE_MEDIOS( int tipo );
	int		 EN_CAMBIO_DE_MEDIOS( );
	int		 PERMITIR_MEDIO( int medio, int tipo );
	void     VALIDAR_MEDIOS_CAMBIO_M( char cod_medio_ing, int cod_submedio_ing );
	int     PERMITE_SUBMEDIO( char medio, int sub_medio, char tipo );
	int SOLICITAR_SUPERVISOR_CAMBIO_M( char medio, int sub_medio, char tipo );
	int     CONSULTA_SALDO_TARJETAS( double monto );*/

/*------------------------------   RETIROS    ------------------------------------*/
	/*double    CR4_PIDE_IMPORTE(  );
	long      CR4_PIDE_NRO_CHEQUE(  );
	long      CR4_PIDE_NRO_CUPON(  );
	long      CR4_PIDE_CUENTA(  );
	double    CR4_BUSCA_CHEQUE(  );
	double    CR4_BUSCA_TARJETA(  );
	double    CR4_BUSCA_CUENTA(  );
   int       _elegir_tarjeta( int comando, char *cadena, int dato, struct _browse *b, double monto );
	int       PIDE_TARJETA( int *id_registro );
	void      CR4_CONFECCIONA_PANTALLA(  );
	void      CR4_AGREGAR_RETIRO(  );
	void      GRABA_EVENTO_CABECERA_RETIRO(  );
	void      CR4_ACTIVA_ANULACION(  );
	void      CR4_IMPRIME_RETIRO(  );
	void      CR4_RESET_ACU_RETIRADO(  );
    int       CONTROL_IMPORTES_ESP( int indice, int actualiza, long *codnivel1, long *codigo, int nro, double *importe, long nro_cheque, int banco, int motivo, short anulacion, char *nro_cheque_str );*/
    /*int       CONTROL_CONSISTENCA_RNV_REC();
    int       CONTROL_IMPORTE_SUBMEDIO( int medio, int submedio );
    int       SUMAR_IMPORTE_SUBMEDIO( int medio, int submedio, double importe );
    void      ACUMULA_RETIRADO( int medio, int submedio, double importe );
    double    OBTIENE_RETIRADO( int medio, int submedio );*/


/*------------------------------   _CR0    ------------------------------------*/
/*#ifdef  INVEL_W
	int       CR0( char *version, int _argc, char **_argv );
#else
	int       CR0( char *version );
#endif*/
/*	void      CR0_EXCEPCION_PEDIR_CAJERO( int *tecla );
	long      CR0_PEDIR_CAJERO( int x, int y, int l, int *tipo_ingreso );
	int       CR0_INGRESA_CAJERO(  );
	int       ES_SUPERVISOR( int cajero );
	void      CR0_EXIT( int status );
	int       CR0_MODO_Z( int con_menu );
	int       Z_DUPLICADA(  );
	void      CR0_GRABAR_ARCHIVOS_SPAR(  );
	int       CR0_c_b( void );
	int       CR0_CIERRE( int recupero );
	int       CR0_GRABAR_ENTRADA_CAJERO(  );
	int       CR0_VERIFICAR_CAJERO_HABILITADO( int cajero );
	int       CR0_HAY_USUARIOS_HABILITADOS(  );
	int       CR0_PEMITE_CERRAR( int con_menu );
	int       CR0_GRABAR_X( int cajon );
	int       CR0_RECUPERAR_X( int cajon );
	int       CR0_MOSTRAR_CAJONES_HABILITADOS( int elegir_cajero );
	int       NUMERO_CAJA(  );
	void      LIBERA_BUFFERS_TEMPORALES(  );
	void      GRABA_EVENTO_SALIDA_CAJERO( void );
	void      GRABA_EVENTO_INGRESO_CAJERO( void );
	void      CR0_TOMAR_DIA_OPERATIVO(  );
	void      CR0_FINAL( char *version );
	void      INICIALIZAR_PINPAD(  );
	//void      CR0_GENERAR_Z( int bandera );
	void      CR0_GENERAR_Z( );
	void      CR0_INIT_ARCHIVOS(  );
	void      CR0_LIMPIAR_RAMPAC(  );
	void      SEGURO(  );
	void      MOSTRAR_DIA_OPERATIVO(  );
	void      MOSTRAR_MEMORIA_DISPONIBLE(  );
	void      TESTEAR_CIERRE_FALLIDO(  );
	void      ALOJA_BUFFERS_TEMPORALES(  );
	void      ALOJA_STRING_TABLE(  );
	void      SETEAR_TIEMPO( int estado );
    void      SUMAR_VAR_UINT( int var, long valor );
	void      ACUMULAR_TIEMPOS( void );
    void      CEREAR_TIEMPOS( void );
	void		 ALOJA_RESPUESTAS_FISCAL( );
	int       MENU_CAJERO_SUPERVISOR( void );
	void       CONTROL_CANT_MEDIOS_SUBMEDIOS_FONDO_F_AUTO( int cajero, int *fondo_fijo_aut );
    int       ES_GERENTE( int cajero );

#if defined(INVEL_W) || defined(INVEL_L)
   char      GRABAR_LOG_ACTIONS( void );
   int       VERICAR_CONF_SERVICIOS( void );
#endif
*/
/*------------------------------   CIERRE    ------------------------------------*/
	//Trasladado a CIERRE.h
/*------------------------------  CONSULTA  ------------------------------------*/
/*	void      MOSTRAR_DATOS_ARTICULO_CON_PROMOCIONES(  );
//	void      CR1_MOSTRAR_DATOS( int y, int vacio, int que_consulta );
	int       EVALUA_TECLA( int tecla );
	void      MUESTRA_PROMOCIONES_ARTICULO( int dato );
	void      MOSTRAR_DATOS_PESO( int fila, float peso, int precio_en_me );
	void      _MOSTRAR_DATOS_ARTICULO( int refrescar, int muestra_promociones,
		int precio_en_me, double peso );
#if defined(INVEL_W) || defined(INVEL_L)
   void CR1_MOSTRAR_ITEM_WIN( int item );
#endif
	void      CR1_MOSTRAR_ITEM( int item, int y );
	//void      CR1_MOSTRAR_DATOS( int y, int vacio, int que_consulta );
   void CR1_MOSTRAR_DATOS( int y, int vacio, int que_consulta, struct _browse *b );
	int       _consulta_cod_barra( int comando, char *cadena, int dato,
		struct _browse *b, double monto );
	int       _consulta_descripcion( int comando, char *cadena, int dato, struct _browse *b, double monto );
	int       _consulta_cod_interno( int comando, char *cadena, int dato, struct _browse *b, double monto );
	int       _consulta_marca( int comando, char *cadena, int dato, struct _browse *b, double monto );
	int       CR1_CONSULTA( int indice );
	int       EVALUA_TECLA( int tecla );
	float     BUSCAR_STOCK( long cod_interno );
	void      REPORTE_STOCK(  );
	int       PIDE_CODIGO_ARTICULO( char *cadena, int consultado );
	void      MOSTRAR_DATOS_PRECIO_ME( int me );
	void      BORRAR_DATOS_PESO( int fila );
*/
/*------------------------------  IMP_ITEM  ------------------------------------*/
	/*void      CAMBIA_COMA_POR_PUNTO( char *m, int len );
	void      ITEM_FORMATO_AMERICANOS( double precio );
	void      ITEM_FORMATO_LIBERTAD( double precio );
	void      ITEM_FORMATO_OMROM_AMERICANOS( double precio );
	int       ITEM_FORMATO_FISCAL( double precio );
	void      ITEM_FORMATO_OMROM_LAR( double precio );
	void      ITEM_FORMATO_SPAR( double precio );
	void      ITEM_FORMATO_OMRON_FERNI( double precio );
	void      ITEM_FORMATO_TP( double precio );
	void      ITEM_FORMATO_VALLEJO( double precio );
	void      MOSTRAR_CANTIDAD( int x, int y, double cantidad );
	void      ITEM_FORMATO_FALABELLA( double precio );
	void      ITEM_FORMATO_BIENESTAR( double precio );
	void      ITEM_FORMATO_TOLEDO( double precio );
	void      ITEM_FORMATO_JOURNAL( double precio );
	void      ITEM_FORMATO_CARREFOUR( double precio );
	void      ITEM_FORMATO_CARREFOUR_IBM_4694( double precio );
	void      ITEM_FORMATO_CONFIGURABLE(  );
	void      MOSTRAR_CANTIDAD2( int x, int y, double cantidad );
	int       IMPRIME_ITEM( int indice );
	void      IMPRIMIR_LINEA_EN_TEXTO_FISCAL(  );
*/
/*------------------------------  CR_DISPL  ------------------------------------*/
	/*void      MOSTRAR_EN_DISPLAY( char *buffer1, double importe );
	void      MOSTRAR_ITEM_EN_DISPLAY( char *nombre, double importe );
	void      CR1_DISPLAY_CAJA_ABIERTA(  );
	void      CR1_DISPLAY( char *buff1, char *buff2 );
	void      CR1_DISPLAY_CAJA_ESPERA(  );
	void      CR1_DISPLAY_CAJA_CERRADA(  );
	void      MOSTRAR_HORA_EN_DISPLAY(  );
*/
/*------------------------------  GETCH  ------------------------------------*/
	/*int       GETCH(  );
	void      UNGETCH( int caracter );
	void      SET_LASTKEY( int tecla );
	int       LASTKEY(  );
	void      SET_ESPERA_GETCH( int ticks );
	int       GET_ESPERA_GETCH(  );
	void      DEMORA_SET(  );
	int       DEMORA( unsigned long _clock );
	int       INICIALIZAR_TECLADO(  );
	void      SET_NUM_LOCK( int prender );
	void      SET_CAPS_LOCK( int prender );
	void      SET_LOCK( unsigned char mascara, int prender );
	int       KBHIT(  );
	int       _GETCH(  );
	void      SET_MOSTRAR_HORA_DISPLAY( int status );
	int       GET_MOSTRAR_HORA_DISPLAY(  );
	void      ENABLE_VERIFICAR_CAMBIO_ARHIVOS(  );
	void      DISABLE_VERIFICAR_CAMBIO_ARHIVOS(  );
	void      ENABLE_PAD_NUMERICO(  );
	void      DISABLE_PAD_NUMERICO(  );
	void      VERIFICA_CAMBIOS(  );
	void      ENABLE_STANDBY_AUTOMATICO(  );
	void      DISABLE_STANDBY_AUTOMATICO(  );
	void      INICIALIZAR_DEMORA_SET( int valor );
*/
/*------------------------------  DISPLAY  ------------------------------------*/
	/*void      DISPLAY( char *buffer );
	void      DISPLAY_LINEA( char *buffer, int linea );
	int       INIT_DISPLAY(  );
	void      BORRAR_DISPLAY(  );
	void      BORRAR_DISPLAY_LINEA( int linea );
	int       DISPLAY_SET_XY( int x, int y );*/

/*------------------------------  M_CARACT ------------------------------------*/
	//void      MOSTRAR_CARACTER( int x, int y, char caracter );

/*------------------------------  EVENTOS  ------------------------------------*/
	/*void      ABRIR_ARCHIVOS_EVENTOS(  );
	void      ABRIR_ARCHIVOS_EVENTOS_ALL(  );
	int       ABRIR_ARCHIVOS_EVENTOS_LOCAL(  );
	int       ABRIR_ARCHIVOS_EVENTOS_BACKUP(  );
	void      CERRAR_ARCHIVOS_EVENTOS_LOCAL(  );
	void      CERRAR_ARCHIVOS_EVENTOS(  );
	void      CERRAR_ARCHIVOS_EVENTOS_ALL(  );

	int       ACTUALIZA_ID_ALL(  );
	void      GRABAR_EVENT_TICKET( int grabar_all );
	int       GRABAR_EVENT_TICKET_ALL(  );
	void      GRABAR_EVENT_E_TICKET( int h, int grabar_all, int cod_barra_art, char origen,
		int tipo );
	int       GRABAR_EVENT_E_TICKET_ALL(  );
	int       DELETE_EVENT_E_TICKET( int h, int grabar_all );
	int       DELETE_EVENT_E_TICKET_ALL( int h );
	void      GRABAR_EVENT_PAGO( int grabar_all );
	int       GRABAR_EVENT_PAGO_ALL(  );
	void      GRABAR_EVENT_VARIOS( char *buffer, int l_buffer, int grabar_all );
	int       GRABAR_EVENT_VARIOS_ALL( int l_buffer );
	void      BACKUP_EVENTOS(  );
	int       COPIAR_EVENTOS_ANTERIORES(  );
	int       COPIAR_EVENTOS_ACTUAL(  );
	void      REPROCESAR_EVENTOS( int cant );
	void      GRABA_EVENTO_ANULADO(  );
	void      RETIRAR_TODOS_LOS_COMPROBANTES(  );
	int       DEBE_GRABAR_ALL(  );
	int       BUSCAR_EVENT_TICKET( long id );
	int       EVENTO_CON_PAGOS(  );
	void      BORRAR_EVENT_TICKET( int borrar_all );
	int       SECCION_DE_CAJA(  );
	int       CARGAR_PAGO_A_RNV( long id );
	int       GRABAR_EVENTO_PROMO( struct _acu_art *acu_art, int p_art, int id );
	int       REVERTIR_EVENTO_PROMO(  );
	void      ACTUALIZA_ID_EVENTO_NEW(  );
	int       CARGAR_ARRAY_NRO_EVENTOS(  );
	void      GRABAR_EVENT_SUPERVISOR( int cod_sup, long legajo, int ok, int funcion, int grabar_en_event );
	int       INCREMENTAR_NRO_EVENTO( int tipo_evento );
    void      GRABAR_EVENTO_DESC( char tipo_motivo, int id_motivo );
    void      REVERTIR_EVENTO_DESC_PROMO( int motivo_descuento, int id_motivo );
	void      GRABAR_EVENT_FACTURA( int nro_cuerpo );
    void      GRABAR_SUPER_REC_A_EVENT(long idEvento);
    int       LIBERAR_ARRAY_NRO_EVENTOS( void );
*/
/*--------------------------------  CRYPT  ------------------------------------*/
	//void      CRYPT( int episodio, char *final );

/*------------------------------  REMITOS  ------------------------------------*/
	/*void      PEDIR_REMITO(  );
	int       EMITIR_REMITO(  );
	int       EMITIR_REMITO_REMOTO(  );
	void      FIN_REMITO(  );*/
/*------------------------------  NETCOM2  ------------------------------------*/
	/*int       CERRAR_CANAL_NETCOM2(  );
	int		 NETCOM2_FORMATO_MENSAJE_TCP(  );
	int		 SWITCH_NCR_CARGAR_RESPUESTAS(  );
	int		 SWITCH_NCR_TRANSFORMAR_RTA( int rta_switch );
	void		 SWITCH_NCR_AGREGAR_CAMPO_NUMERICO( char *buffer_out, int id_campo, int len, long dato );
	void		 SWITCH_NCR_ARMAR_PAQUETE_REQ( struct _transac2_sic *transac2, char *buffer_out, int *l_buffer_out );
	void		 NETCOM2_ARMAR_PAQUETE_REQ( struct _transac2_sic *transac2, char *buffer_out, int *l_buffer_out );
	int		 NETCOM2_PROCESAR_PAQUETE_RTA( char *buffer_in, struct _datos_transaccion *tran_temp,
		struct _transac2_sic *transac2 );
	void		 SWITCH_NCR_AGREGAR_CAMPO_CADENA( char *buffer_out, int id_campo, int len, char *dato );
	char *	 SWITCH_NCR_TIPO_PAQUETE( struct _transac2_sic *transac2 );
	int		 SWITCH_NCR_TIPO_PAQUETE_INT( struct _transac2_sic *transac2 );
	int		 SWITCH_NCR_PROCESAR_PAQUETE_RTA( char *buffer_in, struct _datos_transaccion *tran_temp,
		struct _transac2_sic *transac2 );
	long		 SWITCH_NCR_OBTENER_ENTERO( char *buffer_in, int offset, int len );
	void		 SWITCH_NCR_VERIFICAR_CAMPO( char *cadena, int l );
	int		 NETCOM2_ENVIAR_CONFIRMACION( struct _transac2_sic *transac2, struct _datos_transaccion *tran_temp );
	void		 NETCOM2_ARMAR_PAQUETE_CONFIRMACION( struct _transac2_sic *transac2, char *buffer_out,
		int *l_buffer_out, struct _datos_transaccion *tran_temp );
	int		 SWITCH_NCR_OBTENER_CAMPO( int orden, char separador, char *buffer_out, int l_buffer_out,
		char *buffer_in );
	void		 _GRABA_LOG_FALLA_INTEGRIDAD( char *men, struct _datos_transaccion *tran );
	int		 T_AGREGAR_TRANSACCION( struct _datos_transaccion *tran, long nro, int operacion );
   void      ENCRIPTA_UN_TRACK( int lo, unsigned fecha, char *track_normal, char *track_enciptado );
   void      DESENCRIPTA_UN_TRACK( int lo, unsigned fecha, char *track_enciptado,
      char *track_normal );
   void      CRIPTER_DE_UN_TRACK( int lo, char *charKey1, unsigned key2, char *charCipherText,
      char *charNewText );
   void      T_ENCRIPTAR_DATOS_TARJETA( void );
   int       NETCOM2_CARGAR_CLAVES( void );
   void      T_ENCRIPTAR_DATOS_TARJETA_RSA( void );
   int       TEF_HABILITADO( void );*/

/*------------------------------  ACTUAL  ------------------------------------*/
	/*void      ACTUALIZA_DEMORA(  );
	void      ACTUALIZA_ACUMULADORES_IVA( double signo );
	void      ACTUALIZA_ACUMULADORES_RAM( int h );
	int       ACTUALIZA_EVENTOS(  );
	void      GRABA_EVENTOS_PAGO( int offset_posicion, int devolucion_cobro );
	void      ACTUALIZA_MEMORIA(  );
	void      RESETEA_TOT_TICK(  );
	void      ACUMULA_FACTURAS(  );
	void      ACTUALIZA_ACU(  );
	void      _ACUMULA_DEPARTAMENTO( int depto, double importe, double cantidad );
	void      ACUMULA_MOZOS( int mozo );
	void      ACTUALIZA_IVAS_MULTIPLES(  );
	void      GRABA_EVENTOS_PAGO_ANULACION(  );*/

/*------------------------------  MEMORY  ------------------------------------*/
	/*void      COPY_STRUCT_TO_BUFFER( char *buffer, int origen );
	void      COPY_BUFFER_TO_STRUCT( char *buffer, int destino );
	void      _COPY_BUFFER_TO_STRUCT( char *buffer, int destino, int subindice );
	int       SIZEOF_ARRAY( int var );
	int       SIZEOF_STRUCT( int cod );
	int       _SIZEOF_STRUCT( int cod );
//   void      _( char *buffer, int origen, int subindice );
	void      CEREAR_STRUCT( int cod );
	void      CEREAR_ARRAY( int cod );
	void      COPY_STRUCT( int destino, int origen );

	void      _LOAD_MEMORY( int var, int subindice, char *valor );
	char      _GET_MEMORY_STR_CHAR( int var, int subindice, int elemento );
	void      _SET_MEMORY( int var, int subindice, char *valor );
	void      _CEREAR_MEMORY( int var, int subindice );
	char      GET_MEMORY_STR_CHAR( int var, int elemento );
	char     *GET_MEMORY_STR( int var );
	char     *_GET_MEMORY_STR( int var, int subindice );
	void      _COPY_MEMORY( int destino, int sub_des, int origen, int sub_ori );
	int       SIZEOF_VAR( int var );
	int       SIZEOF_ARRAY( int var );
	void      SET_MEMORY( int var, char *valor );
	void      CEREAR_MEMORY( int var );
	void      COPY_MEMORY( int destino, int origen );
	void      CEREAR_ARRAY( int cod );
	void      SET_MEMORY_STR_CHAR( int var, int elemento, char valor );
	void      _SET_MEMORY_STR_CHAR( int var, int subindice, int elemento, char valor );
#if defined(INVEL_W) || defined(INVEL_L)
	void      SET_MEMORY_CHAR( int var, int valor );
	void      _SET_MEMORY_CHAR( int var, int subindice, int valor );
#else
	void      SET_MEMORY_CHAR( int var, char valor );
	void      _SET_MEMORY_CHAR( int var, int subindice, char valor );
#endif
	void      SET_MEMORY_INT( int var, int valor );
	void      _SET_MEMORY_INT( int var, int subindice, int valor );
	void      SET_MEMORY_LONG( int var, long valor );
	void      _SET_MEMORY_LONG( int var, int subindice, long valor );
#if defined(INVEL_W) || defined(INVEL_L)
	void      SET_MEMORY_FLOAT( int var, double valor );
	void      _SET_MEMORY_FLOAT( int var, int subindice, double valor );
#else
	void      SET_MEMORY_FLOAT( int var, float valor );
	void      _SET_MEMORY_FLOAT( int var, int subindice, float valor );
#endif
	void      SET_MEMORY_DOUBLE( int var, double valor );
	void      _SET_MEMORY_DOUBLE( int var, int subindice, double valor );
	char      GET_MEMORY_CHAR( int var );
	char      _GET_MEMORY_CHAR( int var, int subindice );
	unsigned char GET_MEMORY_UCHAR( int var );
	unsigned char _GET_MEMORY_UCHAR( int var, int subindice );
	int       GET_MEMORY_INT( int var );
	int       _GET_MEMORY_INT( int var, int subindice );
	unsigned  GET_MEMORY_UNSIGNED( int var );
	unsigned  _GET_MEMORY_UNSIGNED( int var, int subindice );
	long      GET_MEMORY_LONG( int var );
	long      _GET_MEMORY_LONG( int var, int subindice );
	unsigned long GET_MEMORY_ULONG( int var );
	unsigned long _GET_MEMORY_ULONG( int var, int subindice );
	float     GET_MEMORY_FLOAT( int var );
	float     _GET_MEMORY_FLOAT( int var, int subindice );
	double    GET_MEMORY_DOUBLE( int var );
	double    _GET_MEMORY_DOUBLE( int var, int subindice );
	void      ADD_MEMORY_CHAR( int var, char valor );
	void      _ADD_MEMORY_CHAR( int var, int subindice, char valor );
	void      ADD_MEMORY_UCHAR( int var, unsigned char valor );
	void      _ADD_MEMORY_UCHAR( int var, int subindice, unsigned char valor );
	void      ADD_MEMORY_INT( int var, int valor );
	void      _ADD_MEMORY_INT( int var, int subindice, int valor );
	void      ADD_MEMORY_UINT( int var, unsigned int valor );
	void      _ADD_MEMORY_UINT( int var, int subindice, unsigned int valor );
	void      ADD_MEMORY_LONG( int var, long valor );
	void      _ADD_MEMORY_LONG( int var, int subindice, long valor );
	void      ADD_MEMORY_ULONG( int var, unsigned long valor );
	void      _ADD_MEMORY_ULONG( int var, int subindice, unsigned long valor );
	void      ADD_MEMORY_FLOAT( int var, float valor );
	void      _ADD_MEMORY_FLOAT( int var, int subindice, float valor );
	void      ADD_MEMORY_DOUBLE( int var, double valor );
	void      _ADD_MEMORY_DOUBLE( int var, int subindice, double valor );
	int       _APUNTAR_STRUCT( int cod );
	int       SIZEOF_STRUCT( int cod );
	int       _SIZEOF_STRUCT( int cod );
	void      ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int estructura );
	void      _ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int tipo );
	int       _APUNTAR_STRUCT_VERIFICANDO( int cod );
	void      ADD_BUFFER_TO_STRUCT( char *origen, int destino );
	void      _ADD_BUFFER_TO_STRUCT( char *origen, int var, int sub, int tipo );
	void		 _COPY_STRUCT_TO_BUFFER( char *buffer, int origen, int subindice );*/

/*------------------------------  EXT_TICK  ------------------------------------*/
	/*void      INCREMENTA_P_EXT_IMPUESTOS(  );
	void      ADD_EXT_IMPUESTOS( int sub1, int sub2, double importe );
	double    GET_EXT_IMPUESTOS( int sub1, int sub2 );
	void      GRABA_EVENTOS_EXT_IMPUESTOS(  );
	void      CARGA_TOTAL_IMPUESTOS( double *total, char cod_ext );
	void      DISCRIMINA_IMPUESTOS_EN_ARTICULO( double porc, char *procesar );
   void      _DISCRIMINA_IMPUESTOS_EN_ARTICULO( double porc, int h, int *incrementar, char *procesar );*/

/*------------------------------  INF00  ------------------------------------*/
  /* void      INFORME_00( int est, char cierre, int anterior );
	void      INFORME_00_FISCAL( double tot_ventas, double tot_retiros, double tot_cobros,
		long cant_tickets, double gran_total, double *dif, int ini_est );
	void      INFORME_00_CUERPO( char *nombre, double importe, int porcentaje,
		int cantidad );
	void      INFORME_00_PIE(  );
	void      INFORME_00_SEPARADOR(  );
	void      INF00_DISCRIMINAR_IVA( double tot_venta, int ini_est );
	void      INF00_ITEM( char *titulo, double tot, int cod_imp, int cod_can,
		int ini_est );
	void      _INF00_ITEM( char *titulo, double tot, double importe, int cod_can,
		int ini_est );
	int       __INF00_ITEM( char *titulo, double tot, double importe, int cantidad );
	void      IMPRIME_INFORMES_Y_EN_Z( int z );
	int       _IMPRIME_INFORMES_Y_EN_Z( int anterior );
	void      ACUMULAR_X(  );
	void      IMPRIME_DIA_OPERATIVO( int anterior );
	void      INFORME_00_CAJERO( int est );
   void      INFORME_00_CABECERA( char anterior, char cierre );
   void      EMITIR_INFORME_00( int anterior, int est, char cierre, int ini_est );
   void 		 IMPRIMIR_REPORTE_TRANSACCIONES(  );
*/
/*------------------------------  INF10  ------------------------------------*/
	/*void      INFORME_10( int est );
	void      EMITIR_INFORME_10( int anterior, int cod_imp, int cod_can );
	void      INFORME_10_CABECERA( char anterior );
	void      INFORME_10_CUERPO( char *nombre, double importe, int porcentaje,
		int cantidad );
	void      INFORME_10_PIE( double importe, int porcentaje, int cantidad, int anterior );*/

/*------------------------------  IMP_TICK  ------------------------------------*/
	/*void      LEER_DATOS_TICKET(  );
	void      IMPRIME_CABECERA_TICKET(  );
	void      _IMPRIME_CABECERA_TICKET( int lineas_corte, int lineas, int corte_activado );
	void      IMPRIME_TITULO_TICKET(  );
	void      IMPRIME_INTERESES( double importe, double porc );
	void      IMPRIME_SUBTOTAL( double importe, int incondicional );
	int       IMPRIME_TOTAL( double importe, int bultos, int modo );
	void      IMPRIME_IVA(  );
	void      IMPRIME_MEDIO_DE_PAGO( char modo, double importe );
	void      IMPRIME_VUELTO( double importe );
	void      _IMPRIME_VUELTO( char *nombre, double importe );
	void      IMPRIME_TICKET_ANULADO(  );
	void      IMPRIME_PIE(  );
	int       IMPRIME_BONOS(  );
	void      IMPRIME_NRO_TICKET(  );
	void      IMPRIME_REPROCESAMIENTO(  );
	void      IMPRIME_CABECERA_GENERAL( long nro_evento );
	int       DEBE_IMPRIMIR_ITEM(  );
	int       IMPRIMIR_TICKET_COMPLETO( int cabecera );
	void      IMPRIME_CAMBIO(  );
	void      IMPRIMIR_CAJERO_EN_SLIP( int ingreso );
	void      IMPRIME_CAJERO(  );
	int       IMPRIME_EMPLEADO(  );
	void      IMPRIME_TOTAL_TICKET_ENVIO_DOMICILIO(  );
	void      IMPRIMIR_COPIAS_TICKET_VOUCHERS(  );
	void      IMPRIME_FIN_TICKET( void );*/

/*------------------------------  INF  ------------------------------------*/
	int       MENU_INFORMES(  );
//	int       MENU_INFORMES_Z( int con_menu, int nro_pasada, char *k );
	int		  MENU_INFORMES_Z( int con_menu );
	void      IMPRIME_NRO_INFORME( char restar );
	void	  INFORME_DIFERENCIAS_SUBMEDIOS_CONTINUO( );
	void	  INFORME_DIFERENCIAS_SUBMEDIOS_RETAIL( char cierre );

/*------------------------------  FISCAL  ------------------------------------*/
	/*int       FISCAL_X(  );
	int       FISCAL_Z(  );
	int       FISCAL_B(  );
	void      FISCAL_V(  );
	void      FISCAL_Y(  );
	void      FISCAL_VERIFICA_ESTADO(  );
	int       FISCAL_T( double importe, int modo );
	void      FISCAL_FIN_INFORME(  );
	void      FISCAL_FIN_BOLETA(  );
	void      ENVIAR_COMANDO_FISCAL( unsigned char *datos );
	void      TOMAR_DATOS_TOTAL_FISCAL( int comando, char *respuesta );
	int       FISCAL_TOTAL_COMPROBANTE( int final );
	void      FISCAL_ABRIR_CAJON(  );
	int       FISCAL_CAJON_CERRADO(  );
	void      FISCAL_VERIFICACION(  );
	void      FISCAL_INICIALIZAR(  );
	int       PROCESAR_ESTADO_IMPRESORA( char *estado );
	int       PROCESAR_ESTADO_FISCAL( char *estado );
	void      ABRIR_DOCUMENTO_NO_FISCAL( char slip );
	int       IMPRIME_ITEM_FISCAL( char *desc, double importe, double cantidad );
	void      IMPRIME_PERCEPCION_FISCAL( char *desc, double importe );
	void      IMPRIME_MEDIO_DE_PAGO_FISCAL( char tipo, char *desc, double importe );
	void      CERRAR_DOCUMENTO_NO_FISCAL(  );
	int       ABRIR_DOCUMENTO_FISCAL(  );
	int       IMPRESORA_FISCAL_EN_LINEA( char *estado );
	int       CERRAR_DOCUMENTO_FISCAL(  );
	// int       HABILITAR_SLIP_FISCAL( int tipo_val, int mens, struct _valida *valida );
	int       ENVIAR_PERCEPCION_COMO_ARTICULO( char *nombre, double importe );
	void      TOMAR_DATOS_RESPUESTA_FISCAL( int comando, char *respuesta,
		char tipo_status );
	int       ERROR_IMPRESORA_FISCAL( char *estado );
	int       CHEQUEAR_CRC_FISCAL_EPSON( unsigned char *respuesta, int cantidad );
	int       ESPERAR_RESPUESTA_COMANDO_FISCAL_EPSON( char *respuesta, char *datos,
		int control_anterior );
	int       COMANDO_FISCAL_EPSON_A_CONTROLAR( char comando );
	int       RECIBIR_RESPUESTA_IMPRESORA_FISCAL( char *respuesta, int len,
		char delimitador, int extra );
	int       FISCAL_SUBTOTAL_COMPROBANTE( int imprimir );
	void      ANULA_TICKET_FISCAL( char *desc );*/

/*------------------------------  INF05  ------------------------------------*/
	/*void      INFORME_05( int est );
	void      EMITIR_INFORME_05( int ant, int cod_seg, int cod_vta );
	void      INFORME_05_CABECERA( char anterior );
	void      INFORME_05_CUERPO( int hora, int minutos, int segundos );
	void      INFORME_05_PIE(  );*/

/*------------------------------  INF15  ------------------------------------*/
	/*void      INFORME_15( int est );
	void      EMITIR_INFORME_15( int anterior, int cod_imp, int cod_can );
	void      INFORME_15_CABECERA( char anterior );
	void      INFORME_15_CUERPO( char *nombre, double importe, int porcentaje,
		int cantidad );
	void      INFORME_15_PIE( double importe, int porcentaje, int cantidad );
*/

/*------------------------------  INF_TAR  ------------------------------------*/
	//void      INFORME_TARJETAS( int est );
	/*void      EMITIR_INFORME_TARJETAS( int z, int anterior );
	void      INFORME_TARJETAS_CABECERA( int anterior );
	void      INFORME_TARJETAS_CUERPO( int h ,char *nombre, double importe, int cantidad );*/
   //SIN_LOTE_TARJETA
   //void      INFORME_TARJETAS_LOTE( int cod_marca );
	//void      INFORME_TARJETAS_PIE(  );


	// inf_com

	//INFORME_COMISION( int est );
	//INFORME_COMISION_TITULO( char cod_comision );
	//INFORME_COMISION_CABECERA();
	//INFORME_COMISION_CUERPO(char cod_comision, char cod_item, double importe, float porcentaje );
	//INFORME_COMISION_PIE(double importe, int porcentaje, int cantidad,int anterior);

	// ssaver

	/*void      INIT_SCREEN_SAVER(  );
	void      SET_TIME_SCREEN_SAVER(  );
	void      VERIFICAR_SCREEN_SAVER(  );
	void      SCREEN_SAVER(  );
	void      INIT_SCREEN_BMP( DAC * dac );
	void      FIN_SCREEN_BMP(  );
	void      EJECUTAR_SCREEN_SAVER( char *nom_arch, DAC * dac );
	int       MOSTRAR_BMP( char *nom_arch, DAC * dac );
	void      OCULTAR_BMP( DAC * dac );
	void      MOSTRAR_BMP_INICIAL( char *nombre );
	void      OCULTAR_BMP_INICIAL(  );
*/
	// ventas2

	/*void      ACTUALIZA_VENTAS2( int reproceso );
	int       ACTUALLIZA_EVENTO_ANTERIOR(  );
	int       EVENTOIZA_EVENTO_ANTERIOR( int cant );
	int       _ACTUA_A_PROCESAR(  );
	int       _ACTUALIZA_VENTAS2( long id_evento, int reprocesar );
	void      ABRIR_VTA_CLASIF(  );
	int       ACTUALIZA_VENTAS_POR_CLASIFICACION( long id_evento );
	int       _ACTUALIZA_VENTAS_POR_CLASIFICACION(  );
	int       ACUMULAR_VENTAS_POR_CLASIFICACION( float cantidad, double importe,
		long cod_clasificacion );


	void      CALCULA_FACTOR_DESPLAZAMIENTO( void );
	int       ACTUALIZA_VENTAS_POR_MEDIOS( long id_evento );
	int       _ACTUALIZA_VENTAS_POR_MEDIOS(  );
	int       ACUMULAR_VENTAS_POR_MEDIOS( float cantidad, double importe, char cod_medio,
		int cod_submedio );
	void      ACTUALIZA_VENTAS_XYZ( char modo );
	void      ACUMULAR_VENTAS_XYZ( char modo );
	void      ACTUALIZA_VENTAS2_ANTERIORES( int actualizar_si_o_si );
	int       ACTUALIZA_COBROS_TARJETAS( long id_evento );
	int       _ACTUALIZA_COBROS_TARJETAS(  );
	int       ACUMULAR_COBROS_TARJETAS( float cantidad, double importe, char cod_medio,
		int cod_submedio );
	int       _GRABA_TICKET_REMOTO(  );
	int       BORRAR_PAGO_ALL(  );
	int       _BORRAR_PAGO_ALL( long id_evento );
	int       _BORRAR_E_TICKET_ALL( long id_evento );
	int       BORRA_ALL(  );
	int       BORRA_TICKET_REMOTO(  );
	int       BUSCA_TICKET_ALL(  );
	int       CANCELAR_ACTUALIZACION( int actualizar_si_o_si, int cancelacion );
	int       ABRE_VENTAS2(  );
	int       CIERRA_VENTAS2(  );
	int       ACTUALIZA_STOCK_TRANSACCIONAL( float cant );
	int       ACTUALIZA_TICKET( long id_evento );
	int       EXISTE_TRANSACCION(  );
	int       VERIFICA_TRANSACCION(  );
	int       UPDATE_CTRLTRAN(  );
	int       INSERTA_CTRLTRAN(  );
	int       EVENTO_A_PROCESAR( struct _eventos *ticket );
*/
	// imp_piey
	//void      IMPRIME_PIE_Y(  );

	// imp_piez
	//void      IMPRIME_PIE_Z(  );


	// log

	/*void      GRABAR_LOG_SUPERVISOR( char *operacion, int supervisor );
	int GRABAR_LOG_SISTEMA( char *operacion ,int origen );
    void        GRABAR_LOG_SISTEMA_CONFIG( char *cadena, int origen );
	void      _LOG_EXISTE_ARCHIVO( char *nom_arch );
	void      GRABAR_LOG_MEMORIA(  );
	void      _GRABAR_LOG_MEMORIA(  );
	void      GRABAR_LOG_NRO_TICKET(  );
	int       GRABAR_LOG_BTRIEVE( char *cadena );
   int       GRABAR_LOG_BD( char *cadena, int error );
	void      VACIAR_LOG(  );
    void     glog( char *men );
    int      logPanelVentas( void );
    int      logPanelPagos( void );
    int      logPanelPosPago( void );
    int      putPurchase( int value, FILE *flujo );
*/
	// pagos

	//char     *PAGOS_OBTENER_NOMBRE( char medio, char submedio );
	//int		  PAGOS( int modo, int ( funcion_val ) ( double importe ), int dev_cobro );
	//int       PAGOS_PIDE_PAGO( int modo, int ( funcion_val ) ( double importe ), int dev_cobro );
	//int       PAGOS_INGRESA_PAGO( int nro, int modo, int dev_cobro );
	/*void      PAGOS_AGREGAR_PAGO( int pos, char modo, double importe, long autorizacion,
		unsigned cuenta, char tarjeta, long numero, char *numero_tar, unsigned banco,
		unsigned fecha_vto, unsigned cantidad_cupones, double valor_cupones, int c_postal_c,
		int c_hora_c, unsigned comprobante, char *cuit, char *telefono, char *cuenta_banco,
		int id_registro, char cod_medio_vuelto, char cod_submedio_vuelto, char tipo_vuelto,
      double importe_me, int cod_valida_cheque, int datos_adicionales, char *telefono_comprador,
	  int cod_restriccion_art );
	void      ACTUALIZA_REDONDEO_DE_VUELTO( int modo );
	void      PAGOS_ANULAR_PAGO(  );
	void      PAGOS_INTERESES(  );
	void      PAGOS_PEDIR_INTERESES(  );
	void      PAGOS_DISCRIMINA_IVA(  );
	void      PAGOS_MARCA_AUTORIZACION(  );
	double    PAGOS_PIDE_IMPORTE( char *titulo, int enteros, int decimales, int signo );
	void      PAGOS_MUESTRA_IMPORTE( double importe );
	long      PAGOS_PIDE_AUTORIZACION( char * );
	int       PAGOS_IMPRIME_PAGO( int modo );
	void      PAGOS_IMPRIME_PAGOS_ESPECIALES( int devolucion );
	int       PAGOS_GRABA_PAGO( int modo, int dev_cobro );
	double    CALCULA_VARIACION( double importe, int modo,int submedio );
	double    CALCULA_VARIACION_TARJETA( double importe, int modo );
	double    _VARIACION_TARJETA( double importe, float porc, double *ret_var,
		double *iva2, long art_variacion );
	void      PAGOS_AGREGAR_VARIACIONES(  );
	void      PAGOS_PREPARA_ITEM( int h, int anulacion );
	void      PAGOS_AGREGA_ITEM_INTERESES(  );
	void      PAGOS_ELIMINA_ITEM_INTERESES(  );
	void      _PAGOS_PREPARA_ITEM( double importe, double porc, int anulacion, int gravado,
		int origen, int tipo );
	int       VALIDAR_PAGO( int modo );
	int       MEDIO_DE_PAGO_ESPECIAL( int h );
	int       MEDIO_DE_PAGO_HABILITADO( int modo, int n, int dev_cobro );
	double    PAGOS_TOTAL_A_PAGAR(  );
//	double    PAGOS_TOTAL(  );
	int       VERIFICA_MARCA_EN_MEDIO( char cod_medio, char marca );
	int       DEBE_ABRIR_CAJON(  );
	void      PAGOS_MUESTRA_IMPORTE_TEMPORAL( double importe );
	void      PAGOS_ENVIAR_OFFLINE(  );
	void      PAGOS_VALIDA_CHEQUE_CLIENTE(  );
	void      PAGOS_VALIDA_200(  );
	void      LIMPIAR_VARIAS_TARJETAS(  );
	double    VARIACION_CUOTAS_MEDIO( int indice, double importe );
	int       BUSCA_CUO_MED( char medio, char submedio );
	int _elegir_cuotas_medio( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
	int _consulta_instituciones( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
   int       CANT_CUOTAS_MEDIO( int indice, unsigned* d_adicionales, int* cant_veces, int* nro_cuota );
   int       ELEGIR_CUOTAS_MEDIO( int medio, unsigned* d_adicionales, int* cant_veces, int* nro_cuota );
	int       ELEGIR_INSTITUCION(  );
	int       PROCESAR_DONACIONES(  );
	int       DONACION_INGRESA_IMPORTE( double imp_sugerido );
	int       GRABAR_MONEDERO_ELECTRONICO(  );
	void      ROLL_BACK_TARJETAS(  );
	int       VERIFICAR_IMPRESORA_FISCAL(  );
	void      PAGOS_MOSTRAR_INTERESES(  );
	int       BUSCA_MEDIO_EN_ARRAY( int k );
	int 		 PAGOS_IMPRIMIR_TICKET_COMPLETO( int modo );
   void      BORRAR_DATOS_ADICIONALES( int pos, int origen );
   int       ELEGIR_OPCION_GRILLA( int cant, char *titulo );
   int       MUTUAL_MONEDA_EXTRANJERA( int h );
*/
	// pan_pago

	/*void      PAGOS_CONFECCIONA_PANTALLA( int modo, int dev_cobro );
	void      MOSTRAR_INTERESES(  );
	void      PAGOS_MUESTRA_TOTAL( double resto );
	void      MOSTRAR_DISCRIMINACION_IVA(  );
	void      MOSTRAR_MARCA_AUTORIZACION(  );
	void      PAGOS_MOSTRAR_RESTO( double resto );
	void      MOSTRAR_VUELTO_EN_GRANDE( double resto );
	void      MOSTRAR_PAGO( int h );
	void      MOSTRAR_RETENCIONES(  );
	void      MOSTRAR_ENVIAR_OFFLINE(  );
	void      MOSTRAR_PEDIR_DIFERIMIENTO(  );
	double	  CALCULAR_VUELTO(char cod_mutual);
	double CALCULAR_RESTO(int moneda_extranjera , int cod_submedio, double resto);
	void	  MOSTRAR_IMPORTE_MUTUAL( double resto, int cod_submedio);*/

	// bancos
	/*int       ABRIR_CHEQUES_CLIENTES( void );
	int       PIDE_BANCO( char *cuenta_banco );
	int       PIDE_BANCO_CLIENTE( char *cuenta_banco );
	int       PIDE_NRO_BANCO(  );
	int       EXISTE_BANCO( int nro );
	int       EXISTE_BANCO__( int nro );
	int       PIDE_CUENTA_BANCO_CLIENTE( int banco, char *cuenta_banco );
	int       GRABA_CHEQUE_CLIENTE( int h, int pos, int borrar );
	int       EXISTEN_CHEQUES_CLIENTE(  );
	int       BORRA_CHEQUE_CLIENTE( int h );
	void      BUSCA_NOMBRE_BANCO( int codigo_banco );
	int       GRABAR_CHEQUES_EN_DBROUTER(  );
    void      BUSCAR_BANCO( int codigo_banco, char *aux );
    int       ELIMINA_CHEQUE( int cheque, int banco, char *cheque_str );
    int       CANCELA_CHEQUES( void );*/

/*-------------------------   PROMO     -------------------------*/
	/*void      ACUMULA_IVA( int h, struct _acu_iva *acu, double cantidad );
	void	  ACUMULA_IVA_ART( long codInt, char* codBar, struct _acu_iva *acu_gravado, struct _acu_iva *acu_excento, double monto );
	void      AGREGAR_PROMOCION_DESCUENTO( struct _acu_iva *acu, long cod_art,
		float porc_desc, char *procesar, int id_registro, double **base_descuento );
	int       CANTIDAD_ARTICULOS_EN_PROMOCION( int cod_promo,
		struct _pro_art *pro_art_sic );
	int       AGREGAR_ARTICULOS_DE_PROMOCION( int cod_promo, struct _art_ben *art_ben,
		struct _acu_art *acu_art, int p_art, float cant_min_canasta, int id_regitro,
		int limite_entrega, double **base_descuento );
	void      AGREGAR_DESCUENTO_ARTICULO( long cod_art, char* cod_bar, double precio, float cantidad,
		int id_registro );
	double    MODIFICAR_PUNTOS( double puntos, char modificar_puntos );
   int       PROMOCION_VIGENTE( int modo, struct _promo_sic *promo_sic,
	   struct _pro_med_sic *pro_med_sic, double *coef_medios, double *total_medios,
	   struct _pro_fid *pro_fid, struct _pro_plu *pro_plu,
	   struct _acu_art *acu_art, int p_art );
	void      ACUMULAR_BASE_DESCUENTO( double **base_descuento, struct _acu_art *acu_art,
		int p_art, double porc, int dpto, int pos, double cant );
	void      PROMOCION_DESCUENTO_PUNTOS( struct _acu_art *acu_art, int p_art,
		char *procesar, double **base_descuento );
	void      MOSTRAR_PROMOCION_CONSULTA( char *nombre_promo, char *promo, char *beneficio,
		char *inf_adicional, int *pos );
	void      MOSTRAR_ARTICULOS_AGREGADOS( char *nombre_promo, int cod_promo, char *promo,
		struct _art_ben *art_ben, int *pos );
	char     *OBTENER_STRING_TIPO_PROMO( int tipo_promo );
	char     *OBTENER_STRING_BENEFICIO_PROMO( int benef_promo );
	int       VERIFICA_PROMO_FID( int cod_promo, struct _pro_fid *pro_fid );
	void AGREGAR_ART_PROMO( long cod_art, float cant, double coef, int enganchar,
		double **base_descuento, struct _acu_art *acu_art,	int p_art, int h );
	void      CEREAR_VARIABLES_DE_PROMOCION(  );
	void      MARCAR_ARTICULOS_A_PROCESAR( char *procesar, int depto );
	void      RECALCULAR_PRECIO_PROMOCION( int h, double importe );
	int       CALCULAR_PROMOCIONES( int modo );
	int       ARTICULO_EN_PROMOCION( int cod_promo, struct _acu_art *art,
		int controlar_cant, short int *cantidad );
	void      REVERTIR_PROMOCIONES(  );
	void      MARCAR_ARTICULOS_A_PROCESAR_ART( char *procesar, long cod_art );
	void      MOSTRAR_BONOS( void );
	void      ACUMULAR_BONOS(  );
	int       AGREGAR_CUPONES_DE_PROMOCION( int formato, unsigned cantidad, char lugar_impresion,
		double valor, char *leyenda, struct _promo_sic *promo );
	void      IMPRIMIR_CUPONES_DE_PROMOCION( char lugar_impresion );
	int       CARGA_ACU_ART( struct _acu_art *acu_art, int l, int permite_exclusion_art,
		double *total_cobros, int agrupar_des, int agrupar_precio );
	void      REDISTRIBUIR_DESCUENTOS( double **base_descuento, struct _acu_art *acu_art,
		int p_art );
	void      BUSCAR_PUNTOS( int cod_promo, double *puntos, double *descuento );
	long      CLASE_OF( long cod );
	int       LINEA_OF( long cod );
	int       SUBLINEA_OF( long cod );
	void      MARCAR_ARTICULOS_A_PROCESAR( char *procesar, int depto );
	void      CARGAR_PROMO_MEMORIA(  );
	int       PROMO_EN_MEMORIA( int cod_promo, int busca_id );
	int       ARTICULO_INCLUIBLE_EN_PROMOCION( int h );
	int       ABRE_ARCHIVOS_PROMOCIONES( char *promo_sic, char *pro_art, char *pro_dep,
		char *pro_med, char *pro_clas, char *art_ben, char *pro_fid, char *pro_plu );
	int       DEPARTAMENTO_EN_PROMOCION( int cod_promo, int dep );
	int       CLASIFICACION_EN_PROMOCION( int cod_promo, struct _acu_art *art );
	int       ACEPTA_PROMOCION( struct _promo_sic *promo_sic,
		struct _pro_med_sic *pro_med_sic, struct _ind_pro_med_sic *ind_pro_med );
	int       MEDIO_EN_PROMOCION( int cod_promo, int h, struct _pro_med_sic *pro_med_sic );
	int       DEBE_PEDIR_SI_ACEPTA_PROMOCION( char cod_medio, char marca, char nro_orden );
	int       BUSCA_ACU_PROMO( struct _acu_art *acu_art, int h );
	void      LIBERAR_PROMO_MEMORIA(  );
   void      UBICAR_DESCUENTO( double *acumulado );
   void      ACUMULAR_TOTALES_IVA( struct _acu_iva *acu, struct _acu_iva *iva_art );
	double	 CALCULAR_VALOR( float cant, double importe,
		float porcentaje, double monto, int h, double *porc_base_dto,
		struct _promo_sic *promo );
	int		 CALCULAR_VIGENCIA_CUPON_PROMO( struct _promo_sic *promo );
	int 		 PROMOCION_PLU( struct _promo_sic *promo_sic, struct _pro_plu *pro_plu_sic,
		struct _acu_art *acu_art, int p_art );
	void	    MOSTRAR_CUPONES_DE_PROMOCION(  );
	void      RECALCULAR_BASE_DESCUENTO( double **base_descuento,  double porc );
   char      SUBMEDIO_EN_PROMOCION( int cod_medio, int pos_pago, char *cod_submedio );
   void      SALVADOR_RESETEAR_TICKET_CANT_DESC( void );*/
   //void      _AGREGAR_PROMOCION_DESCUENTO( struct _acu_iva *acu, long cod_art, char* cod_bar, float porc_desc,
   //   char *procesar, int id_registro, double **base_descuento, char origen );

/*-------------------------   GET_HORA   -------------------------*/
	//long      GET_HORA_DOS_BYTE(  );

/*---------------------  BANDA HORARIA   -------------------------*/
   /*int       VERIFICAR_BANDA_HORARIA (  );
   int       BANDA_VIGENTE( struct _bandas_ventas *bandas_sic );
   int       ABRE_ARCHIVOS_BANDAS( char *bandas_sic, char *band_art, char *band_clas );
   void      CIERRA_ARCHIVOS_BANDAS(  );
   int       ARTICULO_EN_BAND_ART( int cod_banda, struct _band_acu_art * art );
   int       BAND_ACU_ART( struct _band_acu_art *acu_art, int l );
   int       BANDAS_RECORRER_CLASIF( struct _clasif1 *clasif, struct _datos_clasif *datos );
   int       CLASIFICACION_EN_BANDA( int cod_banda, struct _band_acu_art *art );*/

	// recup

	/*void      CRR_INICIALIZAR_RAM_NOVOL(  );
	void      CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL(  );
	void      CRR_RECUPERA_POSICION_ARCHIVOS(  );
	int       INIT_VARIABLES_ESPEJADAS(  );
	void      GRABAR_TICKET( int h );
	void      RECUPERAR_TICKET(  );
	void      CEREAR_VARIABLES_RNV_TICKET(  );*/

/*---------------------  RECUPERO   -------------------------*/
	/*void	  GUARDAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes );
	void      ACTUALIZAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes,
		int indice );
	int       RECUPERAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes,
		int indice );
	void      BORRAR_ARCHIVO_REC( char *archivo );
	int       CANTIDAD_REGISTROS_REC( char *archivo, int bytes );
	int       CREAR_COPIA_ARCHIVO_REC( char *archivo, int forzar );
	void      BORRAR_COPIA_ARCHIVO_REC( char *archivo );
	int       RESTAURAR_COPIA_ARCHIVO_REC( char *archivo, int borrar_copia );
*/
	// valida


	/*void      VALIDAR_MEDIOS( int modo );
	void      VALIDAR_VUELTO( );
	void      VALIDAR_DEVOLUCIONES( double importe );
	void      VALIDAR_COBROS( int cod, struct _datos_cobro *datos_cobro, char *mensaje,
		int devolucion, int modo );
	void      IMPRIMIR_CHEQUE( double importe, int fecha, int cod_valida_cheque );
	void      IMPRIME_CUPON_PROMOCION( int cod_formato, double valor, int pos, int cantidad,
						int h, char *pto_str, int vigencia );
	void      VALIDAR_VOUCHER( int cod_formato );
	int       VALIDAR_CLIENTE_AFINIDAD(  );
	int       VALIDAR_LIMITE_DGI(  );
	void      VALIDAR( int cod, int tipo, int h, int ind_posicion, int modo );
	void      INIT_VALIDA(  );
	void      VALIDA_ejecutar_comando( int *ind_config, int h, int ind_posicion, int modo );
	void      VALIDA_importe_en_letras( int *ind_config );
	INT16     VALIDA_tomar_entero( int *ind_config );
	float     VALIDA_tomar_float( int *ind_config );
	void      VALIDA_SET_FILA( int _fila );
	void      VALIDA_CHEQUE_CLIENTE( double importe, double saldo );
	void      VALIDAR_DONACION(  );
	int       VALIDAR_FRANQUICIA(  );
	int       VALIDACION_POR_SLIP( int ind_config, int *mens );
	void      VALIDAR_UNIPAGO(  );
	void      VALIDAR_PAGOS( int cod, double importe, long cuenta, long comprobante,
		char *mensaje );
	void      REIMPRIMIR_VALIDACIONES_MEDIOS_DE_PAGO(  );*/

	//name_mon
	//void      NAME_MON( unsigned fecha, char *cadena );

	// nume

	//void      NUM_A_LETRAS( double importe, char *texto );

	// coop

	/*int       ELEGIR_CLIENTE_NUMERICO( int cheque, int mostrar );
	void      MOSTRAR_DATOS_CLIENTE( int y, int vacio );
	int       _consulta_cliente( int comando, char *cadena, int dato, struct _browse *b, double monto );
	int       ELEGIR_CLIENTE_ALFABETICO( int cheque );
    int       ELEGIR_CLIENTE_COD_BARRA(int cheque);
	int       _ELEGIR_CLIENTE(  );
	int       PEDIR_COD_FINANCIACION( float *recargo );
	void      ACTUALIZA_SOCIOS( double importe );
	void      COOP_FACTURA_SOCIOS(  );
	int       COOP_VERIFICA_CLIENTE_HABILITADO(  );
	int       PIDE_OC_GD(  );
    int       ELEGIR_CLIENTE_COD_BARRA(int cheque);
*/
	// fecha

	/*void      FECHA__( int fecha, int *dia, int *mes, int *anio );
	int       DOW( unsigned fecha );
	int       DIF_DIAS( unsigned fecha1, unsigned fecha2 );
	int       FECHA( char dia, char mes, unsigned anio );
	int       INC_DIA( int fecha, int dias );
	int       FECHA_VALIDA( int fecha );*/

	// htoc

	//void      HTOC( int hora, char *cadena );

   //asc

   /*int       GRABAR_ARCHIVO( int cod, int registro, char *buffer, int l_reg );
	int       ANULAR_REGISTRO( int cod, int registro, char *buffer, int l_reg,
		char *buf_cer, int l_buf_cer );
	void      GENERA_MUTUAL_ASC( int h );
	void      GENERA_TICKET_ASC( int h );
	void      GENERA_TOTALES_ASC(  );
	void      GENERA_DEPARTA_ASC(  );
	void      GENERA_CHEQUES_ASC( int h );
	void      GENERA_TARJETAS_ASC( int h );
	void      GENERA_COBROS_ASC( int id, struct _datos_cobro *datos_cobro );
	void      GENERA_PAGOS_ASC( int id, long comprobante, long cuenta, double importe,
		double gravado, double exento, double iva );
	void      ANULA_CHEQUES_ASC( int reg );
	void      ANULA_TARJETAS_ASC( int reg );*/
/*------------------------------   PAGO_CHE    ------------------------------------*/
	/*int       CHEQUE_EN_BOLETIN( char *cuenta, long nro_cheque );
	int       BUSCAR_CLIENTE_EN_BOLETIN( char *banco, char *cuenta, char *cuit,
		char *telefono, int *habilitado, long nro_cheque, int lector, char *serie,
		int plaza );
	int       PIDE_CUENTA_CHEQUE( char *cta );
	int       CHEQUEAR_DIGITO_VERIFICADOR_EN_RUT( char *cuit );
	long      PIDE_NRO_CHEQUE(  );
	unsigned  PIDE_FECHA_VTO(  );
	int       PIDE_COD_POST(  );
	int       PIDE_COD_HORA(  );
	int       PIDE_CUIT( char *cuit, int otro_cuit, char *titulo, unsigned pos );
	int       PIDE_TELEFONO( char *telefono, int otro_tel, char *titulo, unsigned pos );
	void      REPROCESAR_CHEQUES( char tipo_pago );
	void      IMPRIME_DATOS_CHEQUE( long nro, char *nom_banco, unsigned fecha,
		char *picture );
	int       PIDE_DATOS_CHEQUE( int al_dia, long *numero, int *banco, unsigned *fecha_vto,
		int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono,
		char *cuenta_banco, int *cod_valida_cheque, int *plaza, int pedir_fecha_vto );
	void      VERIFICAR_CHEQUE_EN_BOLETIN(  );
	int PIDE_PLAZA( int *numero );
	int       BUSCAR_DATOS_CHEQUE_EN_ARCHIVO_CLIENTES( char *banco, char *cuenta,
		int plaza, char *cuit, char *telefono );
	int       MOD11( int suma );
	int       PIDE_DATOS_COMPRADOR_EN_CHEQUE( char *cuit, char *telefono, int al_dia,
		unsigned *fecha_vto, int lector );
	int       PIDE_DATOS_CHEQUE_FALTANTES( int solicitar_supervisor, int al_dia,
		long *numero, char *banco, unsigned *fecha_vto, int *cod_post_cheq,
		int *cod_hora_acre, char *cuit, char *telefono, int pedir_fecha_vto );
	void      CEREAR_DATOS_CHEQUE( long *numero, char *banco, unsigned *fecha_vto,
		int *cod_post_cheq, int *cod_hora_acre, char *cuit, char *telefono );
	int		 VALIDAR_DIGITO_VERIFICADOR_EN_RUT( char *rut );
    int      PIDE_CADENA_NRO_CHEQUE( char *numero );*/

/*------------------------------   PAGO_EFE    ------------------------------------*/
	//void      REPROCESAR_EFECTIVO( char );
/*------------------------------   PAGO_MUT    ------------------------------------*/
	/*int       PIDE_MUTUAL( int *, long *, long *, char *, char *, char *, int * ,int , double, int );
	int       PIDE_CUOTAS_MUTUAL( int codigo );
	void      GO_CUO_MUT( int modo );
	void      REPROCESAR_MUTUALES( char );
	void      IMPRIME_DATOS_MUTUAL( char nro );
	int       INGRESAR_MUTUAL(  );
	long      PIDE_NRO_VALE_MUTUAL(  );
   void      NOMBRE_MUTUAL( char cod_mutual, char *cadena );*/
   //int       COD_VALIDA_MUTUAL( char marca );
//   char COTIZACION_MUTUAL( char cod_mutual, double *cotizacion, char *simbolo_me );
   int		 VALIDA_MUTUAL( char marca );
/*------------------------------    BOLETIN    ------------------------------------*/
   /*int BUSCAR_MEDIO_EN_BOLETIN( char tipo_boletin, int nro_archivo, char *nro_bono,
	   double *importe, int concatenar_importe, int verificar_importe, int local );

   int       PIDE_NRO_BONO( char *nro );
	int       EXISTE_BOLETIN_TCOMPRA_MUTUALES( int nro_archivo, char *nro_bono,
		double importe, int concatenar );
	int       ACTUALIZA_BOLETINES_TCOMPRA_MUTUALES( int *registros );
	void      CONTRUYE_MASCARA( char *nro_bono, double importe, int concatenar, char *dest,
		int size );
	int       OBTENER_BOLETIN_REMOTO( int nro_archivo, char *nro_bono, double importe,
		int concatenar );
	void 		 INICIALIZA_BOLETIN_TICKET(  );
*/
	//baucher

	/*int       IMPRIME_VOUCHER( int p, int para_comercio, long anulacion );
	void      IMPRIME_VOUCHER_CTACTE( int p, int para_comercio, long anulacion, int modo );
	void      OBTIENE_NOMBRE_CLIENTE( char *nombre );
	void      _VOUCHER_BLOQUE1( char *titulo, long anulacion );
	void      _VOUCHER_BLOQUE2(  );
   void       MINI_VOUCHER_CMR(  );
	int       IMPRIME_VOUCHER_IDEM_POSNET( int p, int para_comercio, long anulacion );*/
/*----------------------------------- IMP_COMP ----------------------------------*/
	/*void      IMPRIME_COMPROBANTE( char *titulo, double importe, int modo, int comp );
	void      IMPRIME_CABECERA_COMPROBANTE( char *titulo, int modo, int comp );
	void      IMPRIME_PIE_COMPROBANTE( int modo, double total );*/

	// pago_cta

	/*long      PIDE_CUENTA( char largo );
	int       PIDE_COMPROBANTE(  );
	void      REPROCESAR_CTA_CTE( char tipo_pago );
	void      IMPRIME_DATOS_CTA_CTE( long cta );
	void      IMPRIME_NOMBRE_CLIENTE( char *nom );*/

	// archivos

	/*int       INIT_ARCHIVOS(  );
	void      NOM_ARCHIVO( int cod, char *destino );
	int       COPY( int origen, int destino );
	int       APPEND( int origen, int destino );
	int       OPEN( int cod, int modo );
	int       OPEN_O_CREAT( int cod, int modo );
	int       CREAT( int cod, int modo );
	int       SETFTIME( int cod, unsigned fecha );
	int       ARCHIVO_LOCAL( int cod );
	int       COD_CARPETA( int cod );
	int       CARPETA_LOCAL( int cod );
	void      LOG_EXISTE_ARCHIVO( int cod );
	int       AREA_OF( int cod );
	int       SET_AREA( int cod, int area );
    void      NOM_CARPETA_ARCHIVO( int cod, char *destino );*/

	// cupones

	/*int       ABRIR_CUPONES(  );
	void      CERRAR_CUPONES( int handle );
	void      CREAR_CUPONES(  );
	void      COPIAR_CUPONES(  );
	void      GENERAR_CUPON( int p );
	int       COPIAR_CUPONES_ANTERIOR(  );
	int       CUPON_CALCULAR_STATUS(  );
	void      ANULAR_CUPON( int registro );*/

	// pago_tc
   /*int       PIDE_TICKET_COMPRA( int *id_registro, long *autorizacion, long *datos_ad,
		char *cod_medio_vuelto, char *cod_submedio_vuelto, char *tipo_vuelto,
		int *datos_adicionales, int cod_medio, int origen_ventas, int *cod_restriccion_art,
        int solicitar_submedio );
	int       PIDE_CANTIDAD_CUPONES(  );
	double    PIDE_VALOR_CUPONES(  );
	void      REPROCESAR_TICKETS_DE_COMPRA( char );
	void      IMPRIME_DATOS_TICKET_COMPRA( int nro );
   int       AGREGA_A_POSICION( char pos, char *codigo_barra, double importe );
	int       AGREGA_BIN_EV_T_COMPRA_A_BTRV( void );
	double    OBTENER_IMPORTE_T_COMPRA_DESDE_SCANNER( char *cadena_scanner,
		char cod_t_compra );
   void      NOMBRE_TICKET_COMPRA( int nro, char *cadena );
   int COD_VALIDA_T_COMPRA( char marca );
   int COD_VALIDA_MUTUAL( char marca );*/
   //int COD_VALIDA_VUELTO( char medio, char submedio );
   void COD_VUELTO_MEDIO( char medio, char submedio, int *vuelto_medio, int *vuelto_submedio );
   //int VALIDA_T_COMPRA( char marca );


	// on_line
	/*int       PEDIR_AUTORIZACION_ON_LINE( char origen );
	int       ON_LINE_CTA_CTE( int h, long *nro );
	int       ON_LINE_TARJETA( int h, long *nro, long anulacion );
	int       CONSULTA_ESTADO_TARJETA(  );
	int       _CONSULTA_ESTADO_TARJETA(  );
	int       _CONSULTA_ESTADO_TARJETA_COBRO(  );
	int       _CONSULTA_ESTADO_TARJETA_ADELANTO(  );
	int       ON_LINE_ADELANTO_TARJETA( double importe );
	int       _ON_LINE_MEDIO( int tipo_operacion_on, int tipo_operacion_off,
		double importe, int nodo );*/


	// ventas


	/*void      ACTUALIZA_VENTAS(  );
	void      _ACTUALIZA_VENTAS( int h );
	void      ACTUALIZA_STOCK( int h, double cant );
	int       ABRIR_VTA(  );
	void      CERRAR_VTA(  );
	void      COPIAR_VENTAS( int hacer_bak );
	void      COPIAR_VENTAS_A_VENTAS_Z(  );
	void      BACKUP_VENTAS(  );
	int       COPIAR_VENTAS_ANTERIOR(  );
	void      CEREAR_VENTAS(  );*/
	/*void      ACUMULAR_VENTAS( int area, struct _vta *vta, long cod_interno, double cantidad,
		double imp_sin_iva, double imp_int, double imp_iva1,
		double imp_iva2, double impues[4], double costo,
		double recargo_neto, double imp_exento );*/

	// envios

	/*int       AUTORIZACION_ON_LINE( int tipo_operacion, double importe, char *buf_rta,
		int l_buf_rta );
	int       ESPERAR_BANDERA( int bandera, int segundos );
	int       LEE_AUTOR_DAT(  );
	int       GRABA_AUTOR_DAT( int bandera );
	void      CIERRE_DE_LOTE(  );
	void      CREAR_OFF_LINE( int cerear );
*/
	// netcom

	/*int       ENVIAR_TRANSACCION( int nodo, int operacion, double importe,
		long comprobante, long *nro, long *autorizacion, long anulacion );
	void      MOSTRAR_IMPORTE_ACEPTADO(  );
	int       MOSTRAR_RESULTADO( char *mensaje, int autoriza );
	long      CALCULA_NRO_TRANSACCION(  );
	void      ENVIAR_TRANSACCIONES_PENDIENTES(  );
	int       ENVIAR_PAQUETE( int protocolo, int nodo );
	int       CERRAR_CANAL( int protocolo, int canal );
	int       ENVIAR_COMO_OFF_LINE(  );
	int       OPERACION_NO_REVERSABLE( int op );
	int       TIMEOUT_NODO( char nodo );
	int       AUTORIZAR_TRANSACCION( long *autorizacion, int ok );
	void      COMIENZO_NET_COM(  );
	int       ABRIR_TRANSAC(  );
	int       REVERSAR_TRANSACCION( int nodo, long nro );*/

	// dec_mes

	//int       DECREMENTAR_MES( unsigned fecha );

	// t_file

	/*int       _ENVIAR_PAQUETE_T_FILE( char *paquete, int l_paquete, char *rta, int l_rta,
		int time_out_disponible );
	int       ESPERAR_BANDERA_T_FILE( int bandera, int segundos, int bandera2 );
	int       LEE_BANDERA_T_FILE(  );
	int       GRABA_BANDERA_T_FILE( int bandera );*/



   	// pagxcaja
	/*void      PAGOS_POR_CAJA(  );
	void      PANTALLA_PAGOS_POR_CAJA(  );
	int       MOSTRAR_SUBNIVEL(  );
	int       PIDE_PAGOS_POR_CAJA(  );
	int       ELEGIR_SUBNIVEL(  );
	int       PEDIR_DATOS_PAGO(  );
	int       CREAR_PAGOS_ASC(  );
	int       COPIAR_PAGOS_ASC(  );
	void      TICKET_PAGOS_POR_CAJA(  );
	void      GRABAR_EVENTO_PAGO(  );
	int       ADELANTO_CMR(  );
	int       PEDIR_DATOS_ADELANTO_CMR( int _cta, int _importe, int _cuotas,
    	int _cod_vigencia );
	void      CONSULTA_CUENTA_ON_LINE_ADELANTO( char *cta );
	int       VALIDAR_CUENTA_ADELANTO( int no_usado, char *cta );
	int       TICKET_ADELANTO_CMR( int _cta_cmr, int _cuotas, int _cod_vigencia );
	int       GRABAR_EVENTO_ADELANTO( int _cta_cmr, int _importe, int _cuotas,
		int _cod_vigencia );
   int _consulta_pagos_caja( int comando, char *cadena, int dato, struct _browse *b, double monto );
   int _consulta_pagos_caja2( int comando, char *cadena, int dato, struct _browse *b, double monto );
   int ELEGIR_PAGOS_CAJA_BROWSE(  );*/

   // pagos especiales

 	/*void  PAGOS_ESPECIALES(  );
	void  PANTALLA_PAGOS_ESPECIALES(  );
   int   ELEGIR_PAGOS_ESP_BROWSE(  );
   int   _consulta_pagos_esp( int comando, char *cadena, int dato, struct _browse *b, double monto );
   int   _consulta_pagos_esp2( int comando, char *cadena, int dato, struct _browse *b, double monto );
   int   MOSTRAR_SUBNIVEL_ESP(  );
	int   PIDE_PAGOS_ESPECIALES(  );
	int   ELEGIR_SUBNIVEL_ESP(  );
   int   CONFIRMAR_DATOS_PAGO_ESP( long codnivel1, long codigo, long cod_cliente );
   void  TICKET_PAGOS_ESPECIALES(  );
   void  GRABAR_EVENTO_PAGO_ESP(  );*/


   // netcom2

	/*int       ENVIAR_TRANSACCION_AL_AUT_ON( int operacion, double importe,
		struct _rta_terminal *rta_terminal, int nodo );
	int       T_ENVIAR_TRANSACCION( int nodo, int operacion, double importe );
	int       T_ENVIAR( int nodo, int operacion );
	int       CARGA_TRANSACCION( long nro_transaccion );
	void      MARCAR_ENVIADA_SI_HUBO_ERROR( long nro_tran, int operacion );
	void      T_REVERSAR_TRANSACCION_SI_ES_NECESARIO(  );
	int       T__REVERSAR_TRANSACCION( long nro );
	int       T_DUPLICAR_TRANSACCION( long nro );
	int       T_ABRIR_TRANSAC2(  );
	long      T_CALCULA_NRO_TRANSACCION(  );
	void      T_CIERRE( int tipo_de_cierre );
	void      T_BUSCAR_PRIMERA_TRANSACCION_A_ENVIAR( int nodo );
	int       ABRIR_CANAL_NETCOM2( int protocolo, int reintentos );
	void      T_CORREGIR_ENVIO_FALLIDO(  );
	void      T_COMIENZO_NETCOM2(  );
	void      LIBERA_MEMORIA_NETCOM2(  );
	void      CARGA_TABLA_OPERACIONES(  );
	void      CARGA_TABLA( int operacion, int tipo_de_mensaje,
		long codigo_de_procesamiento, int reversable, char prioridad, char un_solo_intento,
		char espera_rta );
	void      BUSCA_MENSAJE_EN_TABLA( int operacion, int *tipo_de_mensaje,
		long *codigo_de_procesamiento, int tipo_cuenta );
	int       BACKUP_TRANSAC2(  );
	int       CHEQUEA_INTEGRIDAD(  );
	void      GRABA_LOG_FALLA_INTEGRIDAD(  );
	void      COPIAR_RTA(  );
	void      ABRIR_CANAL_TARJETAS(  );
	void 		 NETCOM2_FORMATEAR_AUTORIZACION( char *cod_autorizacion, int modo );*/
	// p_clave

	//int       PEDIR_CLAVE( int x, int y );

	// llave

	//int       ESPERAR_LLAVE( int modo );

	// afinidad

	/*void      PEDIR_CLIENTE_AFINIDAD(  );
	void      TOMAR_CLIENTE_AFINIDAD(  );
	void      LEER_PUNTOS(  );
	int       OBTENER_FECHA_PUNTOS( char *buffer );
	int       SELECCIONAR_AFINIDAD_CHIP(  );
	int       AFINIDAD_DESCUENTO_PUNTOS( double puntos, double descuento );
	void      TOMAR_CLIENTE_AFINIDAD_NORTE(  );
	int       OBTENER_DATOS_CLIENTE_AFINIDAD( int documento );
	int       VERIFICAR_CLIENTE_HABILITADO( char adicional );*/

	// monedero

	//TOMAR_MONEDERO_ELECTRONICO();
	//CONSULTAR_TARJETA_MONEDERO();
	//LEER_DATOS_FIJOS_MONEDERO(struct _datos_fijos *datos_fijos, struct _datos_variables *datos_variables);
	//double BALANCE_TARJETA_MONEDERO();
	//LEER_PUNTOS_MONEDERO(struct _status *status_icc);
	//GRABAR_PUNTOS_EN_MONEDERO(struct _actualizacion_puntos *act_puntos);
	//GRABAR_MONEDERO(double importe);

	// val2

	/*int       VAL_LEER_BANDA_TRACK1( char *buffer );
	int       VAL_LEER_BANDA_TRACK2( char *buffer );
	int       VAL_STATUS(  );
	int       VAL_RESET(  );
	int       VAL_ANULA_APAGADO(  );
	int       VAL_PERMITE_APAGADO(  );
	int       VAL_ESCRIBIR_TARJETA( unsigned char dir, unsigned char cant,
		unsigned char *buffer, unsigned char *_rta1, unsigned char *_rta2 );
	int       VAL_BORRAR_TARJETA( unsigned char dir, unsigned char cant,
		unsigned char *_rta1, unsigned char *_rta2 );
	int       VAL_LEER_TARJETA( unsigned char dir, unsigned char cant,
		unsigned char *buffer, unsigned char *_rta1, unsigned char *_rta2 );
	int       VAL_COD_SECRETO_TARJETA( unsigned char *clave, unsigned char *_rta1,
		unsigned char *_rta2 );
	int       VAL_COD_BORRADO1_TARJETA( unsigned char *clave, unsigned char *_rta1,
		unsigned char *_rta2 );
	int       VAL_COD_BORRADO2_TARJETA( unsigned char *clave, unsigned char *_rta1,
		unsigned char *_rta2 );
	int       VAL_VERSION( char *buffer );
	int       VAL_SELECT_CARD( char tipo );
	int       VAL_POWER_ON(  );
	int       VAL_POWER_OFF(  );
	int       VAL_SOLENOIDE(  );
	int       VAL_ENCRIPTADO( char *buf1, char *buf2 );
	int       VAL_ABRIR_CAJON(  );
	int       VAL_PAYFLEX_SELECT( unsigned dir, int tipo );
	int       VAL_PAYFLEX_READ( char registro, int bytes, char *buffer );
	int       VAL_PAYFLEX_WRITE( char registro, char *buffer, int bytes );
	int       VAL_PAYFLEX_CLAVE( unsigned clave );
	int       VAL_INICIALIZA_LECTOR(  );
	int       VAL_MPCOS_READ( char registro, int bytes, char *buffer );*/

	// val1

	/*void      VAL_SET_PUERTO( int puerto );
	int       VAL_INICIALIZAR_PUERTO(  );
	void      VAL_DESINSTALAR_PUERTO(  );
	int       VAL_ENVIAR_COMANDO( unsigned char *buf_datos, unsigned char long_datos,
		unsigned char *buffer_rta );
	int       VAL_RECIBIR_RESPUESTA( unsigned char *rta );
	int       __RECIBIR_RESPUESTA( unsigned char *rta, unsigned char cant );
	void      VAL_ENVIAR_DATOS( unsigned char *datos, unsigned char cant );
	int       VAL_GET_ESPERA_TICKS(  );
	void      VAL_SET_ESPERA_TICKS( int ticks );*/

	// pausa

	//void      PAUSA( int ticks );


	// carga

	/*void      CARGAR_CONFIG(  );
	int       CARGAR_MEDIOS_DE_PAGO(  );
	void      SN_TO_BIN( char *buffer, int cantidad );
	int       CARGAR_IMPUESTOS(  );
	int       CARGAR_IVA(  );
	int       OBTENER_COD_IVA_INICIAL(  );
	int       CARGAR_DPTOS(  );
   int       CARGAR_TIPO_ENVASES( );
   void      AGRUPAR_ENVASES(  );
   int       CARGAR_TIPOS_DE_ENVASES( );
   int       CARGAR_COBROS_ESPECIALES( void );*/

	// script
	//int       PROCESAR_SCRIPT( int inicial );

	//cr_disc2

    //Movido a CR_DISC2.h

	//export

	/*int       EXPORTAR_ARCHIVO(  );
	int       EXPORTAR_EVENTOS(  );
	int       EXPORTA_ARCHIVO_DE_LA_SEMANA( unsigned fecha, int origen, int destino );
	int       BACKUP_ARCHIVO_DE_LA_SEMANA( unsigned fecha, int origen, int destino );
	int       LEE_FECHA( int cod_arch );
    void      CEREAR_ARCHIVO_BINARIO( int cod_archivo );*/

	// import

	//int       IMPORTAR_ARCHIVO(  );

	// config



	// t_espera

	/*int       TICKET_EN_ESPERA( void );
	int       CONTROLAR_TICKET_EN_ESPERA(  );
	//void      TRAER_TICKET_EN_ESPERA(  );
	static int GRABA_TICKET_EN_ESPERA( char nro_ticket );
	static int RECUPERA_TICKET_EN_ESPERA( char nro_ticket );
	int       GRABAR_STRUCT( int handle, int est );
	int       _GRABAR_STRUCT( int handle, int est, int subindice );
	int       LEER_STRUCT( int handle, int est );
	int       _LEER_STRUCT( int handle, int est, int subindice );
	void      IMPRIME_MENSAJE_TICKET_EN_ESPERA( void );
	void      IMPRIME_MENSAJE_TRAE_TICKET_EN_ESPERA( void );
	int       AUTORIZA_TICKET_EN_ESPERA(  );*/
	void      MOSTRAR_T_ESPERA_POR_PANTALLA(  );
	void	  BORRAR_MENSAJE(void);
	/*int       BUSCAR_TICKET_LIBRE( void );*/

	//monedero
	//void MOSTRAR_MONEDERO_POR_PANTALLA();

	// mensaje

	/*void      MENSAJE_SIN_SONIDO( char *cadena );
	void      MENSAJE( char *cadena );
	void      MENSAJE_CON_PAUSA( char *cadena, int pausa );
	void      MENSAJE_SIN_PAUSA( char *cadena );
	void      MENSAJE_CON_TECLA( char *cadena, int tecla );
	void      MENSAJE_TECLA( char *cadena );
    int       MENSAJE_CON_TECLA_S_N( char *cadena, int tecla_si, int tecla_no );*/

	// anula_t

	//void      TECLA_ANULA_TICKET( int incondicional );
	//void      ANULA_TICKET(  );

	// invent

	/*int       PEDIR_TIPO_INVENTARIO( int permite_salir, int pedir_supervisor );
	void      FIN_INVENTARIO(  );
	void      IMPRIME_PIE_INVENTARIO(  );*/

	//modo_eje

	/*void      TESTEAR_MODO_EJECUCION(  );
	int       TESTEAR_CONTINUA_MODO_ENTRENAMIENTO(  );
	void      SETEAR_MODO_EJECUCION( int modo );
	void      MOSTRAR_MODO_ENTRENAMIENTO_EN_PANTALLA( int cajero_operando );*/

	// pinpad1

	/*void      SET_PUERTO( int puerto );
	int       INICIALIZAR_PUERTO(  );
	int       ENVIAR_COMANDO( unsigned char *comando, unsigned char *buf_datos,
		unsigned char *error1, unsigned char *error2, unsigned char *error3 );
	void      RECIBIR_RESPUESTA( unsigned char *rta, unsigned char cant );
	void      ENVIAR_DATOS( unsigned char *datos, unsigned char cant );
	int       EJECUTAR_COMANDO( unsigned char app, unsigned char com, unsigned char ref1,
		unsigned char ref2, unsigned char len );
	int       GET_RESPUESTA( unsigned char nro );
	void      PINPAD1_DELAY( int tiempo );*/

	// delay

	/*int       INIT_TIMER(  );
	unsigned long TIEMPO_PASADO_DESDE( unsigned long *desde );
	int       YA_PASARON_SEGUNDOS( unsigned long *desde, int segundos );
	void      DELAY( int milisegundos );*/

	// status

	//int       STATUS(  );

/*-----------------------------  VENDEDOR  ----------------------------------*/
	/*void      PEDIR_VENDEDOR(  );
	void      PEDIR_MOZO(  );
	void      PEDIR_VENDEDOR_SI_ES_NECESARIO(  );*/

/*-----------------------------  APAGADO  ----------------------------------*/
	/*void      SET_APAGADO( int estado );
	int       GET_APAGADO(  );*/

/*-----------------------------  DEVOLUCION  ----------------------------------*/
	/*int       TIPOS_DEVOLUCIONES(  );
	int       DIFERENCIA_PRECIO(  );
   int       PEDIR_DATOS_DIFERENCIA_PRECIO( int cambio );
	void      PANTALLA_DIFERENCIA_PRECIO( void );
	int       COMPROMISO_200(  );
	int       PRECIO_MAS_BAJO(  );
	int       __COMPROMISO_PRECIO_MAS_BAJO(  );
	void      PANTALLA_COMPROMISO_PRECIO_MAS_BAJO( void );*/

/*-----------------------------  DEVOL  ----------------------------------*/
	//void      CR1_INGRESA_DEVOLUCIONES(  );

/*-----------------------------  RAM  ----------------------------------*/
	/*void      SET_RAM( int estado );
	int       VALIDAR_RAM(  );
	int       VERIFICAR_E_NRO(  );
	int       VERIFICAR_E_ACU( int est );
	int       _VAL_IMPORTE( int cod, int h );
	int       _VAL_CANTIDAD( int cod, int h );
	int       _VAL_IMPORTE_Y_CANTIDAD( int cod_imp, int cod_can, int h );
	int       __VAL_IMPORTE( int cod, int h, double desde, double hasta );*/

/*-----------------------------  CAMBIO  ----------------------------------*/
	/*void      TECLA_CAMBIO(  );
	int       PEDIR_BOLETA_ANTERIOR(  );
	int       TIPOS_CAMBIO(  );
	int       IMPORTAR_ITEMS(  );
	int       EXISTE_ARTICULO_TICKET_ORIGINAL(  );
	int INGRESAR_CAJA_Y_SECUENCIA( int *caja, long *secuencia, int *fecha );*/
	int ASIGNAR_PRECIO_TICKET_ORIGINAL( struct _ev_cont ctrl_nc );
	/*double	 CANTIDAD_TOTAL_ITEM(  );*/

/*---------------------------   REDONDEO    --------------------------------*/
	/*double    REDONDEAR( double importe, int tipo, double minima_denominacion );
	int       PRECIO_SEGUN_CANTIDAD(  );
	void      REDONDEAR_PRECIO(  );*/

/*------------------------------ C_TICKET -----------------------------------*/
	/*void      CONSULTA_TICKET(  );
	void      RECUPERA_VENTANA_TICKET(  );*/

/*------------------------------  F_FIJO  -----------------------------------*/
	/*void      CR1_FONDO_FIJO(  );
	int       CR1_FONDO_FIJO_MULT_MEDIOS(  );
	void      FONDO_FIJO_MULTIPLE_CONFECCIONA_PANTALLA(  );
	void      FONDO_FIJO_AGREGAR_MEDIO( int pos, char modo, double importe,
		unsigned cuenta, int id_registro, double importe_me, int banco, long nro_cheque, long autorizacion );
	void      FONDO_FIJO_INGRESA_MEDIO( int nro );
	int       FONDO_FIJO_PIDE_MEDIO(  );
	int       MOSTRAR_FONDO_FIJO( int pos );*/

/*------------------------------ ADELANTOS -----------------------------------*/
	//void      CR1_ADELANTOS(  );

/*------------------------------- RETIROS ------------------------------------*/
	/*void      CR4_RETIROS(  );
	void      CR4_CONFECCIONA_PANTALLA(  );
	void      CR4_PIDE_RETIRO(  );
	void      CR4_INGRESA_RETIRO( int nro, int posicion );
	void      CR4_AGREGAR_RETIRO(  );
	void      CR4_ACUMULAR_RETIRO_EN_X( int modo, double importe, int anulacion,
		double importe_me, int motivo );
	void      CR4_ACTIVA_ANULACION(  );
	double    CR4_PIDE_IMPORTE(  );
	void      CR4_IMPRIME_RETIRO(  );
	double    CR4_BUSCA_COMPROBANTE( int modo, long id, int todos, int *id_registro,
		char *sub_medio, short *banco, short tipo_busqueda, double importe, char *id_cadena,
        short anulacion, short marca_usado );
	void      GRABA_EVENTO_RETIRO( int modo );
	void      _GRABA_EVENTO_RETIRO( int medio, double importe, int id_registro,
		char cod_sub_medio, double importe_me, int motivo_cheque, int posicion );
	void      CR4_SET_ACU_RETIRADO( int medio, double importe );
	int       RETIRO_REMOTO( char *param );
	void      CR4_RESET_ACU_RETIRADO(  );
	int       OBTIENE_CANT_SUB_RETIRO( );
	void      CR4_MOSTRAR_RETIRO(  );
	long      CR4_PIDE_NRO_CHEQUE(  );
	long      CR4_PIDE_NRO_CUPON(  );
	long      CR4_PIDE_CUENTA(  );
	double    CR4_BUSCA_CHEQUE(  );
	double    CR4_BUSCA_TARJETA(  );
	double    CR4_BUSCA_CUENTA(  );
	int		  _elegir_tarjeta( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
	int       PIDE_TARJETA( int *id_registro, int solicitar_submedio );
	void      GRABA_EVENTO_CABECERA_RETIRO(  );
    int       CONTROL_IMPORTES_ESP( int indice, int actualiza, long *codnivel1, long *codigo, int nro, double *importe, long nro_cheque, int banco, int motivo, short anulacion, char *nro_cheque_str );
    int       CONTROL_CONSISTENCA_RNV_REC();
    void      ACUMULA_RETIRADO( int medio, int submedio, double importe );
    double    OBTIENE_RETIRADO( int medio, int submedio );
	int       CARGAR_MEDIOS_RETIRO( int medio, int submedio, double importe, double importe_me, int expandir );
	int       POSICIONAR_EN_REGISTRO_RETIRO( int medio, int submedio );
	double	  ELEGIR_DENOMINACION( int grilla, int cant, char *simbolo);
	double    MOSTRAR_DENOMINACIONES( int cod_medio, int cod_submedio);
	int		  PIDE_CANTIDAD_DENOMINACION(int k);*/

/*------------------------------- STAND_BY --------------------------------------*/
	/*void      STAND_BY( int modo, int modo_operacion );
	void      GRABAR_EVENTO_INICIO_STAND_BY( void );
	void      GRABAR_EVENTO_FIN_STAND_BY( void );
*/
/*--------------------------------  CAJON  --------------------------------------*/
	/*int       ABRIR_CAJON(  );
	void      CERRAR_CAJON(  );
	int       CAJON_CERRADO(  );
	int       INIT_CAJON(  );*/

/*--------------------------------  COBROS  ------------------------------------*/

/********Constantes tipo Cobro************
#define	COBRO_NORMAL	1
#define	COBRO_GARANTIA	2
#define	COBRO_PAGATODO	3
*****************************************/

	/*void      GRABAR_EVENTO_COBROS_VARIOS( int codigo, int id, long comprobante,
		long cuenta, double importe, char *cta_cmr );
	void      _GRABAR_EVENTO_COBROS_TOTAL( int cantidad, double importe, int *_importe,
		int *_comprobante, int *_cuenta, int *_codigo );
	int       PIDE_COBRO( struct _datos_cobro *datos_cobro,	int pos, int devolucion );
	int       PEDIR_DATOS_COBRO( struct _datos_cobro *datos_cobro, int pos );
	int       PEDIR_CTA_CMR( char *cta, void f_online( char *cta ), int f_valid( int cod,
			char *cta ) );
	int       MENU_SELECCIONA_COBRO( int *cobro, double *importe );
	void      UNIPAGO_MENSAJE_TICKET(  );
	long      PEDIR_ENTERO_COD_BARRA( char x, char y, char largo, long _default,
		char **cod_barra, int busqueda );
	int       ANALIZAR_CODIGO_BARRA_PARA_SERVICIOS_ESP(int codCobro, char *scanner_cod_barra,
                                         struct _DatosCodBarra *datos);
	int       GRABAR_COD_BARRA_EN_BINARIO( char *cod_barra, int posicion );
	char     *LEER_COD_BARRA_EN_BINARIO( int posicion );
	void      INICIALIZAR_BINARIO_COD_BARRA( void );
	void      PANTALLA_COBROS(  );
	void      GRABAR_EVENTO_COBRO( struct _datos_cobro *datos_cobro, char *cta_cmr,
		int anulacion );
	void      TICKET_COBRANZA( char *concepto, struct _datos_cobro *datos_cobro,
		char *cta_cmr, int anulacion );
	void      PANTALLA_COBROS_VARIOS( int i, double importe );
	void      TICKET_COBRANZA_VARIOS( char *concepto, long comprobante, long cuenta,
		double importe, char *cta_cmr, char cabecera );
	void      COBROS( int devolucion );
	int       ELEGIR_COBRO(  );
	int       ELEGIR_COBRO_BROWSE(  );
	int       CREAR_COBROS_ASC(  );
	int       COPIAR_COBROS_ASC(  );
	int       VALIDAR_COMPROBANTE( int _comprobante );
	int       VALIDAR_CUENTA( char *_cuenta, char *cta_cmr );
	int       COBRO_CMR(  );
	int       PEDIR_DATOS_COBRO_CMR( int _cta, int _importe );
	void      CONSULTA_CUENTA_ON_LINE( char *cta );
	void      CONSULTA_CUENTA_ON_LINE_COBROS( char *cta );
	void      REVALIDAR_COBROS(  );
	void      COBROS_VARIOS(  );
	int       ELEGIR_REPACTACION(  );
	void      REVALIDAR_COBROS_VARIOS(  );
	void      ANULACION_COBROS(  );
	int       ANULACION_COBROS_CONFIRMACION( struct _datos_cobro *datos_cobro );
	void      GO_COBRO( int modo );
	int       ANALIZAR_CODIGO_BARRA_COBROS( char *scanner_cod_barra,
		struct _datos_cobro *datos_cobro, char *mascara );
	void     MOSTRAR_DATOS_CODIGO_BARRA( struct _datos_cobro *datos_cobro );
	int      EVALUAR_MASCARA_COD_BARRA( int cod_cobro, char *scanner_cod_barra,
		struct _datos_cobro *datos_cobro );
	int       COBROS_TARJETA_PERMITIDA( INT16 cod_cobro, INT16 cod_tarjeta );
	int       COBROS_F_ONLINE( double importe );
	void	  PANTALLA_COBROS( void );
	int       POSICIONAR_COBRO_SELECCIONADO( void );*/
	/*int       ENVIAR_IMPORTE_MONEDERO( double importe, char archivo );
	int       COBROS_GARANTIA( char devolucion );
	double    CALCULAR_MONTO_GARANTIA( double precio, INT16 tipo );
	int       PEDIR_DATOS_COBRO_GARANTIA( struct _datos_cobro *datos_cobro );
	void      GRABAR_EVENTO_COBRO_GARANTIA( struct _datos_cobro *datos_cobro, int anulacion );
	double    EVALUAR_COBROS_MONEDA_EXTRANJERA( double importe );
	int		ASIGNA_CODIGO_BARRA( char *cod_barra );
	int		BUSCA_CODIGO_BARRA( char *cod_barra );
	int MEDIOS_HABILITADOS_COBROS( int n, int dev_cobro, struct    _datos_cobro *datosCobros );
	int CONTAR_CUOTAS(int marca, unsigned short cod_medio );
	void REIMPRIMIR_TICKET_LOCAL();
	void CEREAR_TEMPORALES( void );
    void CEREAR_TABLA_LIN_MENS_TMP( void );
	int VALIDAR_COBROS_VENTA( void );
	void GRABAR_MEDIOS_COBRO( int cod_cobro , double importe );
	void GENERAR_REQ_MEDIOS( void );
	int VALIDACIONES_COBROS_VENTA( void );
	int HAY_MARGINALES_ONLINE( void );
	int	TRAER_COBROS_VENTA_ESPERA( long caja_z,long id_evento, int devolucion );
	int COBROS_VENTA_EN_TICKET( int *cobro_on_tick, int *unico );
    int COBROS_VENTA( int codigo, int *cobro_online, int *unico );
	int AGREGAR_ARTICULO_COBRO_VENTA( char *cod_barra, long cod_especial, double importe, INT16 cod_cobro );
	int CANCELAR_COBRO_VENTA( void *struct_datos, int tipo_cobro );
	int TIPO_COBRO_VENTA( INT16 cod_cobro );*/
/*-------------------------------- PENDIENT --------------------------------------*/
	/*void      PEDIR_VENTA_PENDIENTE(  );
	void      TRAER_VENTA_PENDIENTE( long nro_venta );
	void      AGREGA_ARTICULO_AL_TICKET( long codigo, double precio_unit, double cantidad,
		int anulacion );*/

/*-------------------------------- EXTERNOS --------------------------------------*/
	//void      EJECUTAR_EXTERNOS(  );

/*-------------------------------- T_REPROC --------------------------------------*/
	/*void      TECLA_REPROCESAR( void );
	void      VALIDAR_COBROS_POR_ARTICULO(  );
	int       POSICIONA_EN_SUPER( long nro_z, long id_evento );
	int       POSICIONA_EN_DPAGO( long nro_z, long id_evento );
   int       POSICIONA_EN_E_DESPRO( long nro_z, long id_evento );
	int		 POSICIONA_EN_FACTURA( long nro_z, long id_evento );
    int POSICIONA_EN_EV_B_PROMO( long nro_z, long id_evento );*/

/*--------------------------------  T_PAGOS  --------------------------------------*/
	/*int       BORRA_EVENTO( long nro_z, long id_evento );
	int       CERRAR_ARCHIVOS_EVENTOS_HISTORICOS(  );
	int       ABRIR_ARCHIVOS_EVENTOS_HISTORICOS(  );
	int       TECLA_PAGO(  );
	int       VERIFICA_ANULACIONES_AUTORIZADAS(  );
	void      VERIFICA_EL_TOTAL(  );
	int       AUTORIZAR_PRECIOS(  );
	int       VERIFICA_SUPERA_LIMITE_AL_FINAL(  );
	void      OBTIENE_PUNTEROS_A_EVENTOS( struct _eventos **ticket,
		struct _ev_cont **e_ticket, struct _ev_medios **pago,
		struct _event_cod_barra **cod_barra, struct _event_cierre **cierre,
		struct _ev_dmedios **dpago, struct _ev_super **super, long nro_z,
        struct _ev_despro **e_despro, struct _ev_factu **factura,
        struct _ev_bpromo **e_b_promo);
	void      PROCESA_DESCUENTOS_ESPECIALES(  );
	void      ACTUALIZA_PUNTOS_CLIENTES(  );
	int       BUSCAR_ARTICULO_DESCUENTO( long id_evento );
	void		 REVERSAR_PAGO_BOLETA_PLATA_EN_DEVOLUCIONES( );
	int		 CONSOLIDAR_E_TICKET( long id_eventos, int area,
		struct _ev_cont *ev, struct _art *art, int *p_art );
	void 		 INSERTAR_REVERSA_BOLETIN( struct _ev_dmedios dpago, double importe );
   void      REVERTIR_ARTICULO_DESCUENTO( void );
   void      CALCULA_AHORRO_OBTENIDO( void);
	int       REVERTIR_DIFERENCIA_PRECIO( long caja_z, long id_evento, int posicion );
   int       AGRUPAR_ARTICULOS_TRANSACCION_ACTIVA( void );
*/
/*--------------------------------  TOMRA  --------------------------------------*/
	/*int       ES_CODIGO_TOMRA( char *cadena );
	long      TOMAR_CODIGO_TOMRA( char *cadena );
	void      IMPRIMIR_CUPONES_TOMRA(  );
   int       BUSCAR_VOUCHER_TOMRA( long cod_tomra, int en_pago );
   int       VOUCHER_REPETIDO( long cod_tomra );
   void      VACIAR_TABLA_AUXILIAR(  );
   void      BUSCAR_ENVASES_FACTURADOS(  );
   int       RECORRER_ENVASES( int devolver );
   void      DEVOLVER_ENVASE( long codigo, int cantidad );
   int       MARCAR_TABLA_ENVASES(  );
   void      REVERTIR_ENVASES(  );
   int       VALIDA_ENVASES_ENTREGADOS(  );
   double    SALDO_ENVASES(  );
   int       CARGAR_ENVASES_POR_TIPO( char *cadena );
   int       CARGAR_VOUCHER_ENVASE(  );*/

/*--------------------------------  NOVIOS  --------------------------------------*/
	//void      ENVIAR_TRANSACCIONES_NOVIOS(  );

/*-------------------------------- TRANSFER --------------------------------------*/
	/*long      LEE_BANDERA_TRANSFER(  );
	int       GRABA_BANDERA_TRANSFER( long bandera );
	int       _GRABA_BANDERA_TRANSFER( long bandera );*/

/*--------------------------------  ROLLO   --------------------------------------*/
	//void      PEDIR_NRO_ROLLO_INTERNO(  );

/*--------------------------------  FACREM  --------------------------------------*/
	extern int   VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo );
	int       PEDIR_FECHAS_LIMITE(  );
	void      GO_REMITOS( int modo, int inicio );
	int       REMITO_EN_FILTRO(  );
	void      DESMARCAR_REMITOS(  );
	void      ACTUALIZA_ACU_DEPTOS(  );
	void      PASA_TOTALES( struct _tot tot );

/*--------------------------------    CUS   --------------------------------------*/
	/*void      TRAE_VENTA_DEL_CUS(  );
	int       _elegir_tmp_tran( int comando, char *cadena, int dato, struct _browse *b );
	int       SELECCIONAR_VENTAS_DEL_CUS(  );
	void      GO_TMP_TRAN( int modo );
	void      CUS_GRABA_PAGO(  );*/

/*--------------------------------   FUNC   --------------------------------------*/
	//int       SELECCIONA_FUNCION( int lugar );

/*--------------------------------  DRIVER  --------------------------------------*/
	/*void      DRIVER( int funcion, int subfuncion, int bx, void *valor, int longitud );
	void      INIT_DRIVERS_PRIMARIOS(  );
	void      INIT_DRIVERS_SECUNDARIOS(  );*/

/*--------------------------------  POWER   --------------------------------------*/
	/*int       INIT_POWER_DOWN(  );
	int       POWER_STATUS(  );
	int       POWER_DOWN(  );
	void      VERIFICA_POWER_DOWN(  );
	void      END_POWER_DOWN(  );*/

/*--------------------------------    DD    --------------------------------------*/
	/*long      DD_LEER_COD_ESPECIAL( unsigned short var );
	long      GET_LONGITUD_RNV( void );
	void      DD_MEMORY( int clase, int comando, long offset, char *valor,
		unsigned largo );
	int       DD_ABRIR_ARCHIVOS( void );
	void      DD_INIT(  );
	void      DD_INS( int var, int clase, long offset, unsigned largo, int elementos,
		char tipo_variable );
	void      DD_EXEC( int comando, int var, int subindice, char *valor );
	void      _DD_EXEC( int comando, int clase, long offset, char *valor, unsigned largo );
	int       DD_ULTIMO_HANDLE(  );
	int       DD_MAX_HANDLE(  );
	long      DD_OFFSET( int var );
	void      DD_CLOSE(  );
	int       DD_LARGO( int var );
	char      DD_CLASE( int var );
	int       DD_ELEMENTOS( int var );
	int       DD_TIPO( int var );
	int       EST_COD( int var );
	int       EST_VAR( int var );
	char     *DD_PTR( int var );
	void      ABORT( char *t );
*/
/*------------------------------   STACK  -----------------------------------*/
   //void      CLOSE_RECUPERO( int ptr, int handle );
   //void      OPEN_RECUPERO( int *ptr, int *handle );
   //void      STACK_INIT(  );
	/*int       STACK_INS( int bytes );
	void      STACK_DEL( int bytes );
	int       STACK_GET_POINTER(  );
	void      STACK_SET_POINTER( int puntero );
	int       STACK_GET_HANDLE(  );
	void      STACK_SET_HANDLE( int handle );
	void      STACK_CLEAR( int handle_desde );
	void      OPEN_RECUPERO( int *ptr, int *handle );
	void      CLOSE_RECUPERO( int ptr, int handle );
	void      STACK_INIT_DATOS(  );
	void      CEREO_STACK(  );
*/
/*------------------------------  UTI_COMP  ------------------------------------*/
	/*int       CARGAR_CONF_FACTURA(  );
	int       CARGAR_CONF_REMITO(  );
	long      _tomar_long( char **cadena );
	int       _tomar_fecha( char **cadena );
	float     _tomar_float( char **cadena );
	int       _tomar_entero( char **cadena );
	int       CARGAR_CONFIG_TKT( char *nom_arch, unsigned char, struct _ConfTkt *config[], int largo );*/
/*   int       CARGAR_CONF( char *nom_arch, unsigned char *config[], int largo );*/
/*	void      _agregar_parametros_funcion( int funcion, int *ind_config, char *cadena,
		unsigned char *CONFIG_COMP );*/
	/*void      TRANSFORMAR_CADENA_HEXA( char *cadena, int largo );
	void      _tomar_cadena( char **cadena );
	int       CARGAR_CONF_TKT(  );
	void		 LIBERA_UTI_COMP(  );
	void		 _tomar_cadena_dos( char **cadena, char **cobtenida );*/

/*------------------------------  G_CIERRE  ------------------------------------*/
	//int       GRABAR_CIERRE(  );

/*------------------------------  BALANZA   ------------------------------------*/
	/*float     LEER_BALANZA(  );
	int       INIT_BALANZA(  );
	float     LEER_BALANZA_KRETZ(  );
	int       BALANZA_obtener_peso( char *buffer );
	int       _obtener_byte( unsigned char *byte );
	void      _INIT_BALANZA(  );
	int       INIT_DRIVER_BALANZA(  );
	float     LEER_BALANZA_TOLEDO(  );
	float     _LEER_BALANZA_TOLEDO(  );
*/
/*------------------------------  PRESENT   ------------------------------------*/
	//void      CARTEL( char *version, char *branch );
    //void      ACTUAL_FECHA_PRESS();

/*------------------------------  GRAF   ------------------------------------*/
	/*void      SET_MODO_GRAFICO( int modo );
	void      SET_DAC( int desde, int cant, DAC * buffer );
	int       SHOW_BMP( char *archivo, DAC * dac );
	int       FADE( DAC * dac );
	void      CLS_GRAFICO(  );*/

/*------------------------------ IMP_DEV ------------------------------------*/
	//void      TICKET_DEVOLUCIONES( double importe );

/*------------------------------   PLU   ------------------------------------*/
	//long      BUSCAR_PLU( int nro );

/*------------------------------   DTOA   -----------------------------------*/
	/*void      DTOA( unsigned fecha, char *cadena, int anio_completo );
	long      DTOL( unsigned fecha );
	int 		 DLTOD( long fecha );*/

/*------------------------------ IMPORT_T -----------------------------------*/
	/*int       BUSCA_TICK_ALL( int fecha, long nro_ticket );
	int       CARGA_ITEM( int h, double *base_dto );
	int       PIDE_DATOS_IMPORTAR_T( int remoto, char *titulo );
	int       IMPORTAR_TICKET( int remoto, int ev_requerido, int reimpresion, char *titulo,
		int pedir_supervisor, char origen );
	void      AGREGAR_ITEMS_AL_TICKET( int remoto );
	void      PANTALLA_IMPORTAR_T( char *titulo );
	void      CERRAR_EVENTOS_TEMPORALES(  );
	void      REIMPRIMIR_TICKET_REMOTO(  );
	void	  IMPORTAR_DESCUENTOS( int remoto, long id, struct _ev_despro *ev_despro );
    int       CONTAR_TICKET_ESPERA(int ev_requerido, int caja, long secuencia);
	void      ASIGNAR_PRECIO_PADRON( );
	int       LOCAL_OBTENER_TABLA( int nombre_tabla_local, long caja, long secuencia, long evento,
			  int ev_requerido, int origen, long nro_ticket, int fecha );
	int       VERIFICAR_TICKET_ESPERA_PROCESADO( long caja_z, long id_evento );*/
	int       CONTAR_TICK_ESP_SIN_PROCESAR( long secuencia, int estado );
	/*int       VERIFICAR_ESTADO_TICK_ESP( long nro_caja, long nro_secuencia, int estado );
	void      AGRUPAR_ART_IMPORT( int permite_exclusion_art, int agrupar_des,
		        int agrupar_precio );
    void      GUARDAR_INFORMACION_IMPORTACION( int tipo_dev );
    int       CARGAR_INFORMACION_IMPORTACION( long id_evento, long caja_z,
                                    struct _comprob_importados *pComp_imp );*/

/*------------------------------ FTOASP --------------------------------------*/
	//void      FTOA_SIN_PUNTO( double numero, char *cadena, int enteros, int decimales,
	//	int signo, int comas );

/*------------------------------ CorbaPant -----------------------------------*/
	void      MOSTRAR_CADENA_( int x, int y, char *s );
	void      MOSTRAR_INPUT_CADENA( char *s, int posicion );
    void      OBTENER_INPUT_CADENA( char *s );
    void      AGREGAR_INPUT_CADENA( char *s, int pos );
    void      MOSTRAR_MENSAJE( char *s );
	void      BORRAR_MENSAJE_( int pCore );
	//bam
//	void      CR1_MOSTRAR_ITEM_( char *s1, char *s2, char *s3, char *s4, char *s5,
//		char *s6 );
   void      CR1_MOSTRAR_ITEM_( char *signo,char *cantidad,char *cinterno,char *cbarra,char *nombre,char *precio);
	void      ExecuteAction( int action );
	void      ExecuteSAction( int action, char *s );
   //void      ExecuteSSAction( int action, int type, char *s, char *s1 );
	void      ExecuteSSAction( int action, int type, char *s, char *s1,char *mascara, char *s2 );
   void      ExecuteSSSAction(int Action, char *s, char *s1, char *s2);
   void      ExecuteSSSSSAction(int Action, int type, char *s, char *s1, char *s2,  char *s3, char *s4 );
	void      agregarPago( int ad,char *s0, char *s1, char *s2, char *s3 );
   void      SetConfig(int var,int valor, char *strValor);
   void      SLEEP(long milSeg);
   void      BEEP(int freq, int duracion);
   void      GRABAR_LOG(char *msg, char *archivo_, int origen, int nivel);
   int       GetConfig(int var);
   void      agregarDeuda(char *s,char *s1,char *s2,char *s3,char *s4, char *s5);
   void      agregarCheque(char *s,char *s1,char *s2,char *s3,char *s4);
   void      mostrarCliCobros(char *Nombre,char *Tipo,char *Domicilio);
   void      agregarCobroEsp(char *numeroComp, char *letra, char *tipoMov, char *reglaAntigua, char *fechaVenc, char *importeMin, char *importe, char *moneda, char *fechaEmision, char *fechaHasta, char *periodoFact);
   int       setData( int action, char *buffer, int len );
   int       getData( int action, char *buffer, int len );

	void      INIT_PEDIR_( int x, int y, int largo );
	void      CLOSE_PEDIR_(  );
    void    MOSTRAR_MENSAJE_POP_UP( char *pTitulo, char **pMsg, int pCantLinea );
    int     TOMAR_FTP_SEGUNDO_PLANO(char *pOrigen,char *pDestino, int pModo, int pIdScript,int pIdNroSec);
    int     TOMANDO_FTP();
	int WIN_ACTIVA_ONTOP(void);
	int PANT_VENTAS_ACTIVA( void );
	int PANT_COBROS_ESP_ACTIVA( void );
	int IsStandbyActivo();
	int GetNumerRowsCash();
	int IsCashActivo();
	void setHabPing(int hab);

/*------------------------  CONSULTA PROMOCIONES   ----------------------------*/
   void     agregarPromo(char *nombre,char *descripcion );
   void		clearPromos( void );
   char * GetAppExePath();

/*------------------------------  UTI_VAL   -----------------------------------*/
   //void      LIBERA_UTI_VAL(  );

/*------------------------------   NETCOM   -----------------------------------*/
	//void      FIN_NET_COM(  );

/*------------------------------   FLUSH   -----------------------------------*/
	//int       FLUSH(  );

/*------------------------------  VENTAS2  -----------------------------------*/
	//int       TIPO_EVENTO_A_PROCESAR( char tipo_evento );

/*------------------------------  M_ENTERO  -----------------------------------*/
	/*void      SET_ENTERO( char *picture, long entero, char *cadena );
	void 		 FORMAT_ENTERO( char *picture, long entero, char *cadena );*/

/*------------------------------ CorbaFPrin -----------------------------------*/
	int       InitPrinterFiscal(  );
	void      FiscalReportZ(  );
	void      FiscalBeginNonFiscal(  );
	void      FiscalReportX(  );
	void      FiscalEndFiscalReceipt(  );
	void      FiscalSetTrailerLine( int line, char *text, int doubleWidth );
	void      FiscalPrintRecVoid( char *desc );
	void      FiscalPrintRecItemEx( char *des, double price, double quantity,
		double vatinfo, double unitPrice, char *unitName, double impInt );
	void      FiscalPrintRecTotal( double total, double payment, char *desc );
	void      FiscalBeginRemoval( int timeOut );
	void      FiscalEndRemoval(  );
	void      FiscalBeginInsertion( int timeOut );
	void      FiscalEndInsertion(  );
	void      FiscalEndNonFiscal(  );
	void      FiscalBeginFiscalReceipt(  );
	void      FiscalBeginFiscalReceiptDocument(  );
	void      FiscalEndFiscalReceiptDocument(  );
	int       FiscalGetPrinterState(  );
	void      FiscalGetData( int id, int *p, char *s );
	void      FiscalPrintNormal( int station, char *data );
	void      FiscalPrintFiscalText( char *data );
	void      FiscalPrintRecSubTotal( double total );
	void      FiscalPrintRecVoidItemEx( char *description, double _amount,
		double _quantity, int _adjustmentType, double _adjustment, double _vatInfo,
		double _ImpInt );
	int       InitScanner(  );
	int       EnableScanner( int deviceEnabled );
	void      BOOT(  );
	int       GET_FECHA_BACKUP(  );
	int       TEST_RED(  );

/*----------------------------- Modo_eje -----------------------------------*/
   /*int GRABAR_RNV_EN_ARCHIVO( char *filename );
   int SET_ENTRENAMIENTO_OFF(  );
   int SET_ENTRENAMIENTO_ON(  );*/
/*----------------------------- CorbaDisplay -----------------------------------*/
	int       DISPLAY_LINEA_( char *buffer, int linea );
	int       BORRAR_DISPLAY_(  );

/*---------------------------- CorbaCashDrawer ---------------------------------*/
	int       ABRIR_CAJON_(  );
	int       CorbaInitCajon(  );
	int       CAJON_CERRADO_(  );

/*---------------------------- CorbaBrowse ---------------------------------*/
	int       CorbaInitBrowse(  );

/*---------------------------- CorbaMenuGen ---------------------------------*/
	int       CorbaInitMenuGen(  );
	//int		 Menu_tickets_en_espera( char *titulo, struct _menu *items, int nitem );
   int       Menu( char* titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum );
	void      setFieldValuepCharMenu( int index, int field, char *value );

void		 Obt_NRO_CAJA( );
int CORRECCION_REDONDEO_SISTEMA( double *importes );
void ROUND2( double* numero, int enteros, int decimales );
/*---------------------------- NC ---------------------------------*/
   /*void      PANTALLA_IMPORTAR_NC(  );
   int       PIDE_DATOS_IMPORTAR_NC(  );
   int       PRECIO_TICKET_ORIGINAL(  );
   int       TIPOS_NC(  );
   int       REFERENCIA_NC(  );
	int		 PIDE_TIPO_COMPROBANTE_EN_NC( );
	int 		 PEDIR_PRECIO_NOTA_CR_LIBRE(  );
	int 		 VERIFICA_NC_AUTORIZADAS(  );*/

/*---------------------------- CALCULAR ---------------------------------*/
//double       RESOLVER( char *formula, struct _valores_calculo *valores_calculo );
//double       _RESOLVER( char *formula, struct _valores_calculo *valores_calculo, char *cadena );

/*----------------------------- TOUCH SCREEN ----------------------------*/

    //int     INICIO_TOUCH( void );
//    void    CAMBIO_PANTALLA_TOUCH( int valor );
//    void    CAMBIO_PANTALLA_TOUCH_ANTERIOR( void );
    //int     GET_TOUCH( void );


/*---------------------------- CONTROLA ---------------------------------*/

/*---------------------------- COBROS_ESP.C ---------------------------------*/
/*int       COBROS_ESP( void );
int       TECLAS_COBROS_ESPECIALES( int k, int *rta, int seguir, int *asignar );
int       PROCESAR_TECLAS_COBROS_ESPECIALES( int k, struct DatosCobro *datosCobro );
void      MOSTRAR_DATO_COBRO( int dato, short tkt_columna );
int       VALOR_OBLIGATORIO( void );*/

#define _Des_Prepaga {"00 SMG","01 Salud","02 Optar","03 Nubial","05 Qualitas","06 Medicien","07 Docthos"}
#define _Des_TipoCli {"00 Individual","01 Empresa","02 Obra Social"}
#define _Cod_Prepaga {0,1,2,3,5,6,7}
#define _Cod_TipoCli {0,1,2}

#define TMOV_FACTURA               1
#define TMOV_NOTA_CREDITO          2
#define TMOV_NOTA_DEBITO           3
#define TMOV_CUPON_RECIBO          4
#define TMOV_CARGO_DEBITO          5
#define TMOV_CARGO_CREDITO         6
#define TMOV_RESUMEN_CUENTA        7
#define TMOV_CARGO_DEB_CHEQUE_RECH 8

#define LOG_PANTALLAS       1
#define LOG_PAGOS           2
#define LOG_VENTAS          3
#define LOG_COBROS          4
#define LOG_MOTOR_PROMO     5
#define LOG_FONDO_FIJO      6
#define LOG_RETIROS         7
#define LOG_CIERRE          8
#define LOG_JAVA_SERVER     9
#define LOG_VARIOS          10
#define LOG_CAMBIO_MEDIO    11
#define LOG_ODBC			12
#define LOG_CON_SERVIDOR	13
#define LOG_COMUNICACIONES	14
#define LOG_ERRORES			15
#define LOG_DEBUG			16
#define LOG_ROBOT			17
#define LOG_SUPERVISOR		18
#define LOG_ARCHIVOS		19
#define LOG_MEMORIA			20
#define LOG_BTRIEVE			21
#define LOG_BASE_DATO		22
#define LOG_COMPROBANTES	23
#define LOG_INCONDICIONAL   24  //si lo queremos loguear si o si

#define _IMPORTAR_REIMPRESION    2
#define _IMPORTAR_REVERSO        3

/*---------------------------- SOCIOS.C ---------------------------------*/
/*long      PEDIR_CLIENTE( short int cod_cobro, char tipo_cli );
int       EXISTE_SOCIO( int tipo_busqueda, long cliente, short int cod_un_negocio, short int cod_cobro, char tipo_cli, char *cuit );
//int       ELEGIR_SOCIO_ALFABETICO( void );
//int       ELEGIR_SOCIO_CODSOCIO( void );
int       ELEGIR_SOCIO( int ( *f_usuario ) (int ,char *,int ,struct _browse *, double), int alfabetico );
int       _consulta_socio( int comando, char *cadena, int dato, struct _browse *b, double monto );
int       _consulta_socio2( int comando, char *cadena, int dato, struct _browse *b, double monto );
int       _consulta_socio3( int comando, char *cadena, int dato, struct _browse *b, double monto );
int       _consulta_socio4( int comando, char *cadena, int dato, struct _browse *b, double monto );
int       CLIENTE_AUTORIZADO( long nroCli, short int cod_cobro, short int tipo_cli  );
long      PEDIR_CLIENTE_ALFABETICO( void );
long      PEDIR_CLIENTE_CODSOCIO( void );
long      PEDIR_CLIENTE_ ( int busqueda );
int       ACTUALIZA_NOVEDADES_SOCIOS( void );*/

/*-------------------------- MED_REIN.C ------------------------------*/
/*void      MEDIO_REINT( void );
void      ACTUALIZA_ACUMULADORES_CARGA_RAPIDA_CHEQUES( double total, int h, int signo );
void      RECUPERO_DE_CHEQUES_CARGADOS( );
int       ACTUALIZA_MREINTEG( long codnivel1, long codigo, short nro );
int       SUCURSAL_DE_CAJA( void );
void      MREINTEG_EN_CIERRE( void );
long      OBTENER_CODIGO_REINTEG( long codnivel1, long codigo, short nro );*/

void	  COPIAR_RNV_A_MEMORIA_PARA_DEBUG( void );

#define   _CHQ_EN_CAJA  -1 //en la caja y no enviado al dbrouter
#define   _CHQ_ENVIADO   0 //en la caja y enviado al dbrouter
//cuando el chq se entrega para pago de reintegro, se borra el registro de la mreinteg.btr
//cuando el chq se retira por rechazo, se borra el registro de la mreinteg.btr

//int COBROS_PAGATODO( char modo, char devolucion );

#ifdef __cplusplus
}
#endif
#endif


//extern struct sub_medios sub_medios;
//int MOSTRAR_SUBMEDIO( int medio, double submedio );
//int GUARDAR_POSICION_SUBMEDIO(int medio, double submedio, double importe, double importe_dec );
//int IMPRIMIR_TKT_INFORME_SUPERVISOR( );
//void  IMPRIMIR_TKT_CABECERA_SUP(  );
//void  IMPRIMIR_TKT_CABECERA_DIF_SUB();

int OBTENER_TABLA_SERVIDOR( int pTabla, int pTipo, char *nombreTablaServ, char *condicionSelect, int vaciar, char *nombreTablaStruct );

void RECORRER_SUBMEDIOS( int tabla, char clave, char orden );
void IMPRIMIR_INFORME_MEDIOS( );
double OBTENER_MONTO_REC_SUBMEDIO( char medio, int submedio );
int OBTENER_TIPO_MEDIO( int nro );
char * GetAppExePath();
#ifdef COMPILAR_LOGS_VIA_TCP
   extern char IP_DEMO_LOGS[16];
   extern int PUERTO_IP_DEMO_LOGS;
   extern int CANAL_ABIERTO_DEMO_LOGS;
   extern int CAMBIAR_PUERTO;
   extern int HABILITA_LOGS_TCP;
#endif
#pragma pack (pop)

