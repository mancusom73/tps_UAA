#pragma pack (push,1) 
#ifndef CR_VAR_H
#define CR_VAR_H

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Estructuras de datos y definiciones generales
 * ------------------------------------------------------------------------------
 */

#include <tpv_dat.h>   

struct _medios *medios = NULL;
struct _eventos event_ticket;
struct _ev_cont event_e_ticket;
struct _eventos event_ticket_all;
struct _ev_cont event_e_ticket_all;
struct _ev_bpromo event_promo;

struct _ev_cod_barra event_cod_barra;
struct _ev_cierre *event_cierre = NULL;
struct _ev_medios event_pago,event_pago_all;
struct _ev_dmedios *event_dpago = NULL;
struct _ev_super *event_super = NULL;
struct _cierre_tarjetas *event_cie_lote = NULL;
struct _ind2_eventos_all ind2_eventos_all;
struct _event_varios *event_varios = NULL, *event_varios_all = NULL;
struct _ev_despro *event_despro = NULL;
struct _ev_despro *event_despro_hist = NULL;
struct _ev_factu *event_factura;
struct _ev_bpromo event_b_promo;
struct _ev_despro *event_despro;
//struct _ind_e_des_pro_btr *indice = NULL;
struct _cajeros datos_cajero,datos_cajero_actual;
struct _detalle_fondofijo detalle_fondofijo;
struct _rend_caj rend_caj;
struct _tarjetas *TARJ = NULL;
struct _medios_tar tar_med;
struct _tar_esp tar_esp;
struct _version VERSION;
struct _bancos bancos;
struct _clientes clientes;
struct _articulo articulo;
struct _articulo_ext articulo_ext;
struct _present present;
struct _ind_present ind_present;
struct _comprobante comp;
struct _articulo_iva iva[10];
struct _boletin_protectivo boletin_prot;
struct _cobros cobros;
struct _tipo_envase tenvase[10];
struct _clave_tef *clave_tef = NULL;
struct _pagos2 pagos2;
struct _vueltos_x_medio vueltos_sic;
struct _tarjetas_fisc tarjetas_fisc, tarjetas_fisc_rec[10]; //Seba
/*SWISS MEDICAL*/
struct _operaciones_cli oper_cli;
struct _detalle_op_cli det_op_cli;
struct _cli_cobros cli_cobro;
struct _ind5_cli_cobro ind5_cli_cobro;
struct _ind6_cli_cobro ind6_cli_cobro;
struct _sucursales sucursales;
struct _detalle_descrip d_descrip;
struct _tpvconfig config;
struct _funcion_sic _funcion;
struct _alicuota alicuota;
struct _porcperc porcperc;
struct _plu plu;


char *_dCompania[] = {"Swiss Medical"," ","Docthos"," "," "};
char *_dPrepaga[] = {"00 SMG","01 Salud","02 Optar","03 Nubial","   ","05 Qualitas","06 Medicien",
                     "07 Docthos"};
//char *_dUnidadNegocio[] = {" "," "," "," ","Prepaga"};
char *_dTipoSocio[] = {"Individual","Empresa","Obra Social",""};

// Estructuras para archivos Historicos
struct _eventos *event_ticket_hist = NULL;
struct _ev_cont *event_e_ticket_hist = NULL;
struct _ev_medios *event_pago_hist = NULL;
struct _event_varios *event_varios_hist = NULL;
struct _ev_cod_barra *event_cod_barra_hist = NULL;
struct _ev_cierre *event_cierre_hist = NULL;
struct _ev_dmedios *event_dpago_hist = NULL;
struct _ev_super *event_super_hist = NULL;
struct _ev_despro *event_despro_hist;
struct _ev_factu *event_factura_hist;
struct _ev_bpromo *event_b_promo_hist = NULL;


/*-------------------------- Variables Globales -----------------------------*/
char cadena_scanner[14],usar_cadena_scanner,_leyo_scanner;
time_t hora_inicio_ticket,hora_ingreso_cajero;
//float     impuestos[21];
//Se crea verificar_tablas para definir zona donde en una sincronizacion da errores 
//porque las tablas aun no estan creadas (Tarea 4980).
extern int verificar_tablas = NO;
extern int DICCIO_CARGADO;
extern int db_status_ant = 0;
extern int db_nativeError_ant = 0;
extern int db_msj_MySQL = 0;
extern int nro_tabla_actual_ant = 0;
char      bandera_online = 1;
int NO_INC_NRO = 0;
char      cadena_debug[80];
/*char      cadena_prn[100];
int       _tipo_letra_prn = 0;
int       _tipo_letra_prn_ant = 0;
int		  new_line = 0;*/
char      forzar_online  = 0;
char  bandera_offline = 0;
int mostrar_cobro = 1;
int _CAJON_POR_IMPRESORA_ABIERTO = -1;
int SI_FRANQUE0_AL_FINAL = 0;
int COD_VALIDACION_AL_FINAL = 0;
struct _impuestos *impuestos; 

char _PERMITIR_SCANNER = 0;
char _PERMITIR_BANDA = 0;
int *_TECLAS_EXCEPCION = NULL;
void      ( *_FUNCION_EXCEPCION ) () = NULL;
int TECLADO_ALFABETICO = 0;
char _LLAVE = 'O';
int _medio_efectivo = -1,
    _medio_cheque = -1,
    _medio_tarjeta = -1, _medio_ctacte = -1, _medio_moneda_extranjera = -1;
int _medio_ticket_compra = -1, _medio_mutual = -1, _medio_tarjeta2 = -1;

int _LARGO_TICKET = 400;
int _LARGO_TRANSFERENCIA = 400;
int INT_DRIVER = 0x80;
char _IMPRIMIENDO_Z_ANT = 0;
int error_matematico = 0;
struct _ConfTkt *CONFIG_FACTURA;
struct _ConfTkt *CONFIG_REMITO;
struct _ConfTkt *CONFIG_VALIDA;
struct _ConfTkt *CONFIG_TKT1; 
struct _ConfTkt *CONFIG_CLIENTES; 

//struct _ConfTkt *CONFIG_COMPROBANTE1;
unsigned *TABLA_VALIDA;
unsigned _FECHA_ARCHIVO;
char _UTILIZAR_FECHA_OPERACION = 0;
int NRO_ARCHIVO;
int _LETRA_GRANDE_PEGADA = 0;
char _ATRIBUTO = 7, _CURSOR;
int _GRANDE = 0;
int TIPO_DE_LETRA = LETRA_NORMAL;
int ESTACION_SELECCIONADA = RECEIPT;
int COLUMNA = 0;
char BUFFER_LINEA_PRN[200];
char _MODO_GRAFICO = 0;
char _TIPO_PANTALLA = 0;
char _TOUCH_SCREEN = 0;
char *_PANTALLA = NULL;
unsigned char ATRIB_DESC_DPTO[CANT_DEPTOS],BUFFER[256];
unsigned char ATRIB_DISCR_DPTO[CANT_DEPTOS];
unsigned char res1,res2;
struct _stringt *string_table;
int _HUBO_CLOSE_ALL = 0;
int buscar_articulo_en_mostrar_item = 0;
struct _ind_e_des_pro_btr *indice;
//struct _cuerpo CUERPOS MAX_CUERPOS;
struct _cuerpo *CUERPOS;
char linea_descripcion[4][100];
int DATOS_RNV_DIRECTO_MEMORIA = 0;
int BLOQUEAR_DATOS = 0;// PARA QUIE NO MODIFICQUE TABLS POR SCRIP CUANDO SE ESTA OPERANDO.. CAIDA DE BROWSERS Y DEMAS

char *rta_fiscal;
int TKT_VERSION_SERVICIOS;
int TKT_VERSION_RETAIL;
struct _medios_para_reintegros medios_para_reintegros;
#ifdef COMPILAR_LOGS_VIA_TCP
   char IP_DEMO_LOGS[16];
   int PUERTO_IP_DEMO_LOGS = 0;
   int CANAL_ABIERTO_DEMO_LOGS =0;
   int CAMBIAR_PUERTO = 0;
   int HABILITA_LOGS_TCP= 0;
#endif

#endif
#pragma pack (pop) 
