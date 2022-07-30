#pragma pack (push,1) 
#ifndef _CR_VAR_H
#define _CR_VAR_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#pragma pack (1)

#include <tpv_dat.h>

//extern struct _medios medios[9];
extern struct _medios *medios;
extern struct _vueltos_x_medio vueltos_sic;
extern struct _eventos event_ticket,event_ticket_all;
extern struct _ev_cont event_e_ticket,event_e_ticket_all;
extern struct _ev_medios event_pago,event_pago_all;

extern struct _ev_cod_barra event_cod_barra;
extern struct _ev_cierre *event_cierre;

extern struct _ev_dmedios *event_dpago;
extern struct _ev_super *event_super;
extern struct _cierre_tarjetas *event_cie_lote;
//extern struct _ev_bpromo event_promo;
extern struct _detalle_descrip d_descrip;
extern struct _tpvconfig config;
extern struct _funcion_sic _funcion;

// Estructuras para archivos Historicos
extern struct _eventos *event_ticket_hist;
extern struct _ev_cont *event_e_ticket_hist;
extern struct _ev_medios *event_pago_hist;
extern struct _event_varios *event_varios_hist;
extern struct _ev_cod_barra *event_cod_barra_hist;
extern struct _ev_cierre *event_cierre_hist;
extern struct _ev_dmedios *event_dpago_hist;
extern struct _ev_super *event_super_hist;
extern struct _ev_despro *event_despro_hist;
extern struct _ev_factu *event_factura_hist;
extern struct _ev_bpromo *event_b_promo_hist;

extern struct _ind2_eventos_all ind2_eventos_all;
extern struct _event_varios *event_varios,*event_varios_all;
extern struct _cajeros datos_cajero,datos_cajero_actual;
extern struct _detalle_fondofijo detalle_fondofijo;
extern struct _rend_caj rend_caj;
extern struct ImpresionDatosAdicionales ImpDatosAdi;
extern struct _datos_cobro_kinacu datos_cobro_kinacu;

//extern struct _tarjetas TARJ[CANT_TARJETAS];
extern struct _tarjetas *TARJ;
extern struct _medios_tar tar_med;
extern struct _tar_esp tar_esp;
extern struct _version VERSION;
extern struct _bancos bancos;
extern struct _clientes clientes;
extern struct _articulo articulo;
extern struct _articulo_ext articulo_ext;
extern struct _present present;
extern struct _ind_present ind_present;
extern struct _comprobante comp;
extern struct _articulo_iva iva[10];
extern struct _boletin_protectivo boletin_prot;
extern struct _cobros cobros;
extern struct _tipo_envase tenvase[10];
extern struct _alicuota alicuota;
extern struct _porcperc porcperc;
extern struct _plu plu;

//SWISS MEDICAL
extern struct _operaciones_cli oper_cli;
extern struct _detalle_op_cli det_op_cli;
extern struct _cli_cobros cli_cobro;
extern struct _ind5_cli_cobro ind5_cli_cobro;
extern struct _ind6_cli_cobro ind6_cli_cobro;
extern int ACELERACION_PANTALLAS; //para pc lentas y preferentemente  micro de 1 core ej celeron

extern char *_dCompania[];
//extern char *_dUnidadNegocio[];
extern char *_dTipoSocio[];
extern char *_dPrepaga[];


/*-------------------------- Variables Globales -----------------------------*/
extern char cadena_scanner[14],usar_cadena_scanner,_leyo_scanner;
extern time_t hora_inicio_ticket,hora_ingreso_cajero;
extern int NO_INC_NRO;
extern char cadena_debug[80];
/*extern char  cadena_prn[100];
extern int  _tipo_letra_prn;
extern int  _tipo_letra_prn_ant;
extern int  new_line;*/
extern int verificar_tablas;
extern int DICCIO_CARGADO;
extern int db_status_ant;
extern int db_nativeError_ant;
extern int nro_tabla_actual_ant;
extern int db_msj_MySQL;
extern char  bandera_offline;
extern char  bandera_online;
extern char  forzar_online;
extern int mostrar_cobro;
extern int _CAJON_POR_IMPRESORA_ABIERTO;
extern int SI_FRANQUE0_AL_FINAL;
extern int COD_VALIDACION_AL_FINAL;
//extern float impuestos[21];
extern struct _impuestos *impuestos;
extern char ancho_panel;
extern char ancho_pantalla;
extern char numero_serie[];
extern char ver[];
extern char ver_posvisual[40];
//extern char ver_branch_posvisual[60];
extern char ver_branch_posvisual[100];
extern char con_request;
extern struct _cliecheq *cheques_clientes;
extern int archivo_cliente_abierto;
extern int archivo_clicobro_abierto;
extern char dato__cajero[80];
extern int pos_x_cajero;
extern char cajero_operando;
extern char ssaver_activo;
extern char corte_habilitado;
extern int _CANCELA_MODIFICACIONES;
extern struct _stringt *string_table;
extern int _HUBO_CLOSE_ALL;
extern unsigned char ATRIB_DISCR_DPTO[CANT_DEPTOS];
extern unsigned char ATRIB_DESC_DPTO[CANT_DEPTOS];
extern int archivo_boletin_abierto;
extern int buscar_articulo_en_mostrar_item;
extern struct _ev_despro *event_despro;
extern struct _ind_e_des_pro_btr *indice;
//extern struct _cuerpo CUERPOS MAX_CUERPOS;
extern struct _cuerpo *CUERPOS;
extern struct _ev_factu *event_factura;
extern struct _ev_bpromo event_b_promo;
extern char *rta_fiscal;
extern struct _clave_tef *clave_tef;
extern int TKT_VERSION_SERVICIOS;
extern int TKT_VERSION_RETAIL;
extern char linea_descripcion[4][100];
extern struct _medios_para_reintegros medios_para_reintegros;
#endif
#pragma pack (pop) 