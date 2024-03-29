#pragma pack (push, 1)
/******************************************************************************/
/* Sistema : TPV 1000 - II                                                    */
/* M�dulo  : Estructuras de datos y definiciones generales                    */
/* Version de Partida: TPVDAT_RE_10_07_2008_0263                              */
/* TPVDAT_RE_27_10_2014_0271												  */
/******************************* VERSION MYSQL ********************************/
/******************************************************************************/

#ifndef __TPV_DAT_H__
#define __TPV_DAT_H__

#include <time.h>
//#include <basetsd.h>
#pragma pack (1)
/** define la cantidad de tarjetas IMPORTANTE: no debe ser menor que 'A_RET_CANT_SUB' */
#define CANT_TARJETAS          50
#define CANT_DEPTOS            100
#define CANT_DEPTOS_RNV        100
#define CANT_MOZOS             10
#define CANT_PROM_CUPON        5
#define L_STACK_RECUP          500
#define CANTIDAD_HANDLES_RNV   500
#define L_REG_VARIOS           3000
#define _LARGO_EXT_IMPUESTOS   5
#define CANT_GRUPOS            5
#define CANT_DATOS_MEDIOS      15
#define INT16 short int
#define UINT16 unsigned short
#define MAX_CUERPOS 10

/*********Enteros super chicos**********/
typedef char BINARY;
typedef unsigned char _BINARY;

struct _tpvconfig
{
    BINARY status;
	INT16 sucursal;
    float porc_iva1;
    float limite;
    char momento_recargo;     /* 0 final, 1 renglon, 2 sin recargo */
    double limite2;
    float porc_iva2;
    INT16 primer_supervisor;
    UINT16 dia_operativo;
    UINT16 proximo_dia_operativo;
    long cod_recargo_intereses;
    char calendario;
    INT16 val_devoluciones;
    float tope_interes;
    long cod_dev_envase;
    INT16 val_dev_envase;
    INT16 val_afinidad;
    long cod_descuento_importe2;
    long cod_descuento_importe3;
    float tope_anulaciones_sin_supervisor;
    char reserva1;
    char nom_comercio[26];
	//Se comenta porque no se usa y trae problemas con el mapeo
	//debido que la tabla de la base del servidor no lo tiene
	//char viejo_cuit[14];
    BINARY id_suc;
    double precio_a_autorizar;
    long cod_descuento_importe1;
    long cod_descuento_porc;
    float tope_descuento_porc;
    long cod_desc_porc_fijo[5];
    float descuento_porc_fijo[5];
    float limite_adelantos;
    float tope_porc_cajero;
    long codigo_descuento_cajero;
    char pais;
    char grupo_dto[5];
    char sup_descuento;
    INT16 offset_tarjetas; //hasta aca es igual al de libertad
    float     limite_ticket_factura;
  	char		 cod_recargo_intereses_alfa[21];
  	char		 cod_desc_porc_fijo_alfa[5][21];
  	char		 cod_desc_importe_alfa[3][21];
  	char		 cod_desc_porc_alfa[21];
    char      clave_extendida_cajero;
    char      clave_extendida_supervisor;
    float     monto_max_dto[5];
    char      cod_dto_cli2[21];
    double    monto_min_dgi;
    char      tipo_calculo_dgr;
    float     tasa1_dgr;
    float     tasa2_dgr;
    double    monto_min_dgr;
    char		 cod_desc_marketec[21];
    char      cod_desc_canje_premios[21];
    float     porc_percep_tacis;
    //   char      cuit[20];
    INT16       municipio_percep; //se unifica de libertad
    double    monto_min_212;
    INT16     tipo_control_dias_percepcion;
    INT16     dias_devolucion_percep;
    char      mensaje_percepcion[19];
    char redondeo;//aca es lo mysql
    char redondeo_pesables;
    float piso_identificacion_clientes;
    float minima_denominacion_donacion;
    unsigned char mask_descuentos_empleados;
    char mascara_legajo[16];
    INT16 val_afinidad_adicional;
    long cod_descuento_cliente;
    char cuit[21];
    double tope_descuento_imp;
    float tope_descuento_ult_item_porc;
    double tope_descuento_utl_item_imp;//Notar que dice UTL y no ULT
    char mascara_scanner_cli[14];
	INT16     long_pref_barra;
    float  importe_minimo;
    char abrir_cajon_ultimo_camb; //abrir el cajon al ultimo en un cabio de valores
	double monto_minimo_retiro;
	double monto_maximo_retiro;
    float   limite_peso;
    INT16     cliente_busqueda;
	BINARY fin;
};


struct _medios
{
	 BINARY status;					//0
	 BINARY nro;					//1
	 char nombre[9];				//2
	 BINARY vuelto;					//11 // 0->no, 1->efectivo, 2->propio
	 char autorizacion;				//12
	 char mutual;					//13
	 char tarjeta;					//14
	 char ticket_compra;			//15
	 char efectivo;					//16
	 char cta_cte;					//17
	 char cheque;					//18
	 char hab_cobro;				//19
	 float variacion;				//20
	 long art_variacion;			//24
	 INT16 cod_valida;				//28
	 BINARY tipo_variacion;			//30
	 float porc_vuelto;				//31
	 float porc_del_pago;			//35
	 char credito;					//39
	 char al_dia;					//40
	 float minima_denominacion;     //41
	 char hab_fac_rem;				//45
	 char hab_nc;					//46
	 BINARY tipo;					//47
	 char abrir_cajon;				//48
	 double cotizacion;				//49
	 char cheque_3ro;				//57
	 char tarj_chip;				//58
	 char hab_modo_restringido;     //59
	 BINARY id_registro;			//60
	 char permite_retiros;			//61
	 char recargo_opcional;			//62
	 float vuelto_maximo;			//63
	 float maximo_en_caja;			//67
	 float minimo_en_caja;			//71
	 char moneda_extranjera;		//75
	 char simbolo_moneda[4];		//76
	 BINARY datos_adicionales;      //80 // 0 no, 1 A nivel de medio, 2 a nivel de submedio
	 INT16 cod_solicitud;			//81
	 char hab_ventas;				//83
	 char discrimina_iva;			//84
	 float pago_imp_maximo;			//85
	 float pago_imp_minimo;			//89
	 INT16 cod_restriccion_art;     //93
	 INT16 submedio_aut;			//95
	 char pide_supervisor;			//97
	 INT16 cod_sol_denom;			//98
	 float porc_min_pago;			//100
	 char permite_fondo_fijo;		//104
	 float maximo_bloqueo;			//105
	 char  art_variacion_alfa[21];	//109
	 char  hab_cobro_dev;			//130
	 double monto_maximo_devolucion;//131
	 INT16    modo_arqueo;			//139 //Ret y Arq Detallado (0) - Ret y Arq Acumulado (1) - Ret Det y Arqueo Acum (2)
	 double monto_minimo_retiro;
	 double monto_maximo_retiro;
	 double limite_dev_sup;
	 double limite_dev_ger;
	 char tarjeta_procesada;
	 BINARY fin;
};

struct _eventos
{
	 BINARY status;					//0
	 long id_evento;				//1
	 long caja_z;					//5
	 long nro_ticket;				//9 /* nro de retiro(7) - Y (9) - Z(10) */
	 UINT16 fecha;					//13
	 UINT16 hora;					//15
	 UINT16 cajero;					//17
	 UINT16 bultos;					//19  /* o codigo o tipo de inventario */
	 double importe_sin_iva;		//21
	 double imp_int;				//29
	 double costo;					//37
	 double recargo_tarjeta;		//45
	 double exento;					//53
	 double recargo;				//61
	 double iva1;					//69
	 long nro_cliente;				//77
	 double iva2;					//81
	 UINT16 suc_comprobante;		//89
	 long nro_comprobante;			//91
	 BINARY tipo_evento;			//95
	 double tot_variacion;			//96
	 double ila;					//104
	 double iha;					//112
	 INT16 sucursal;				//120
	 long cliente_afinidad;			//122
	 INT16 nro_vendedor;			//126
	 long nro_rollo;				//128
	 UINT16 hora_inicio;			// 132
	 long id_all;					// 134
	 INT16 cod_financiacion;	// 138
	 long nro_evento;				//    140
	 long nro_boleta_anterior;  //   144
	 BINARY cambio;				 //   148
	 double retencion_dgr;		 //   149
	 double percepcion_dgi;		 //   157
	 INT16 nro_sucursal_anterior; // 165
	 INT16 nro_caja_anterior;		//  167
	 INT16 nro_caja_dgi;			//  169
	 float puntos;					//  171
	 long nro_legajo_cajero;	//	  175
	 INT16 nro_dia_operativo;	//	  179
	 INT16 caja;					//  181
	 long nro_z;					//  183
	 char bien_de_uso;			//	  187
	 INT16 fecha_anterior;		//	  188
	 BINARY tipo_comprobante_anterior; // 190
	 double percep_212_iva1;	//	 191
	 double percep_212_iva2;	//	 199
	 BINARY reproceso;			//	 207
	 double diferencia_redondeo;// 208
	 BINARY importado;			//	 216
	 char str_cliente_afinidad[17]; // 217
	 char legajo_empleado[16];		//	 234
	 long nro_cotizacion;			//	 250
	 char envio_domicilio;			//  254
	 BINARY status_remote;			//		 255
	 double puntos_usados;			//		 256
	 float monto_donacion;			//		 264
	 INT16 motivo;							// 268
	 long nro_vendedorl;					// 270
	 BINARY cant_promo_afinidad[5]; //	 274
	 double alicuota;                //	 279
	 long id_evento_comprob_ant;     //  287
	 long nro_evento_anterior;       // 291
	 double monto_impuesto[4];       // 295
	 char nro_vendedors[17];		   // 327
	 float monto_donacion_ventas;	   // 344
	 INT16 cod_cobro;					   // 348
	 char nri[15];							//  350
	 char cuit[21];					  //	 365
	 char dosificacion_orden[13];	  //	 386
	 INT16 tmpo_reg_articulo;		  	//	 399
	 INT16 tmpo_pago;						//	 401
	 char serial_impresora[21];		//	 403
	 char serial_impresora_ant[21];  //	 424
	 long cajaz_importado;				// 445
	 long id_evento_importado;       //449
	 BINARY tipo_evento_importado;   //453
	 INT16 cod_institucion;          //454
	 BINARY tipo_evento_orig;        //456
	 UINT16 tmpo_no_venta;           //457
	 UINT16 tmpo_ocio;               //459
	 //UINT16 cod_lista;// puesto por compatibilidad con la version multiplataforma  descomentando se puede usar e
	 double nro_comprobante_new;
	 BINARY fin;                     //461
	/*NOTA: cuando se modifica la tabla ticket.btr, tambien hay que modificar la tabla
			ev_tmp.btr y en inStore tienen que modificar la vista (V_Ticket_Espera)*/
	 //char reserva[77];      // 550
};//cuidado cuando se agregue un campo por el cambio de lugar del ila.....

struct tick_tmp
{
	BINARY status;
    long id_evento;
    long caja_z;
    UINT16 fecha;
    char tipo_evento;
    long nro_evento;
    BINARY fin;
};

/*
 * tipo_evento :
 * -----------
 * 0->ticket
 * 1->cobro
 * 2->pago
 * 3->factura A
 * 4->factura B
 * 5->nota de credito A
 * 6->nota de credito B
 * 7->retiro
 * 8->importes informados al cierre
 * 9->Y
 * 10->Z
 * 11->remito
 * 12->anulacin de comprobante
 * 13->inventario
 * 14->facturas A de remitos
 * 15->facturas B de remitos
 * 16->factura remota A
 * 17->factura remota B
 * 18->remito remoto
 * 19->NC remota A
 * 20->NC remota B
 * 21->Modo devoluciones
 * 25->fondo fijo
 * 26->Entrada Cajero
 * 27->Salida Cajero
 * 28->Entrada Stand By
 * 29->Salida Stand By
 * 30->Ticket en espera
 * 99->anulado
 */

struct _ind2_eventos_all
{
    char tipo_evento;
    long nro_cliente;
    UINT16 fecha;
};

struct _ev_cont
{
	BINARY status;					//0
	long id_evento;					//1
	long cod_articulo;				//5
	float cantidad;					//9
	UINT16 cant_presentacion;		//13
	double importe_sin_iva;			//15
	double imp_int;					//23
	double costo;					//31
	double recargo_tarjeta;			//39
	double exento;					//47
	double iva1;					//55
	double iva2;					//63
	double ila;						//71
	double iha;						//79
	char cod_barra[17];				//87
	BINARY cod_presentacion;		//104
	BINARY leido_por_scanner;		//105
	char tipo_precio;				//106
	BINARY cod_ext;					//107
	INT16 nro_vendedor;				//108
	long cod_clasificacion_old;		//110
	double precio_promocion;		//114
	char stockeable;				//122
	BINARY cod_depto_old;			//123 //reutilizado para el desarrollo de iva 0 para consumidores finales
	BINARY origen;					//124
	INT16 tipo;						//125
	double total;					//127
	BINARY habilitado_cuotas;			//135
	BINARY cod_iva;					//136
	BINARY cod_movimiento;			//137
	double precio_unitario;			//138
	long caja_z;					//146
	INT16 posicion;					//150
	INT16 caja;						//152
	long nro_z;						//154
	double descuento;				//158
	INT16 tipo2;					//166
	INT16 cod_depto;				//168
	long comprobante;				//170
	INT16 motivo;					//174
	INT16 cod_sucursal;				//176
	INT16 fecha_ticket;				//178
	char hab_desc_cliente;	    	//180
	float alicuota;					//181
	BINARY cod_impuesto[4];			//185
	double monto_impuesto[4];		//189
	char nro_vendedors[17];			//221
	BINARY tipo3;					//238
	char participa_donacion;		//239
	double precio_original;			//240
    char cod_articulo_alfa[21];
	char cod_clasificacion[36];
  	BINARY excluir_promocion; //0 o 1
	INT16 cod_cobro;
	INT16 motivodev;
	BINARY fin;
};//cuidado cuando se agregue un campo por el cambio de lugar del ila.....

struct _ev_medios {
	BINARY status;					//0
	long id_evento;					//1
	UINT16 modo_pago;				//5
	double importe;					//7
	long numero_id;					//15
	BINARY retirado;				//19
	BINARY reproceso;				//20
	BINARY tarjeta;					//21
	long buffer1_long;				//22
	INT16 banco;					//26
	UINT16 fecha_vto;				//28
	INT16 cantidad_cupones;			//30
	double valor_cupones;			//32
	INT16 cod_post_cheq;			//40
	INT16 cod_hora_acre;			//42
	char nro_tarjeta[21];			//44
	char auto_tarjeta[11];			//65
	long nro_cupon1;				//76
	BINARY cuotas;					//80
	char nro_comercio[16];			//81
	UINT16 nro_boletin;				//97
	char tabla[6];					//99
	BINARY status_tarjeta;			//105
	UINT16 nro_retiro;				//106
	double vuelto_efectivo;			//108
	double variacion;				//116
	long nro_cupon_int1;			//124
	//Se comenta porque no se usa y trae problemas con el mapeo
	//debido que la tabla de la base del servidor no lo tiene
	//char viejo_cuit[12];			//128
	char telefono[12];				//140
	double vuelto_propio;			//152
	BINARY moneda;					//160
	BINARY cod_vigencia;			//161
	UINT16 primer_vto;				//162
	double importe_entrega;			//164
	BINARY posicion;				//172
	BINARY id_registro;				//173
	long caja_z;					//174
	INT16 caja;						//178
	long nro_z;						//180
	BINARY cod_sub_medio;			//184
	double vuelto_medio;			//185
	BINARY cod_medio_vuelto;		//193
	BINARY cod_submedio_vuelto;     //194
	double importe_me;				//195
	double vuelto_medio_me;			//203
	INT16 cod_sucursal;				//211
	INT16 fecha_ticket;				//213
	double iva_proporcional;		//215
	double imp_pago;				//223
	char nro_cta_cheque[16];		//231
	long datafono;					//247
	char cuit[21];					//251
	//Se comenta porque ocaciona problemas con el mapeo
	//debido que la tabla de la base del servidor no tiene
	//este campo. Adem�s se utiliza en una funcion que no es
	//llamada en ninguna parte del codigo
	//BINARY tipo_cuenta_tarj;		//272
	double recargo_tarjeta;			//273
    INT16 expandir;					//281
    BINARY fin;						//283
};

struct _cab_varios
{
    BINARY status;
    long id_evento;
    char reserva[10];
};

struct _event_varios
{
    struct _cab_varios cabecera;  //0
    char buffer[L_REG_VARIOS];    //15
	BINARY fin;
};

struct _cajeros {
	BINARY status;				//0
	UINT16 cod_cajero;			//1
	char nombre[26];			//3
	char clave[4];				//29
	char documento[15];		//33
	BINARY tipo;					//48    /* 1-cajero 2-supervisor */
	long nro_legajo;			//49
	char puede_retirar;		//53
	BINARY cod_ff;
	char clave_extendida[9];
	BINARY fin;
};

struct _tarjetas_fisc {
	int p_pago;
	char marca_tarjeta;
	char numero_tarjeta[21];
	float saldo_cliente;
	double importe;
	char track1[80];
	char track2[80];
	long nro_cupon;
	long nro_cupon_int;
	char cod_autorizacion[10];
	char cuotas;
	char moneda;
	UINT16 nro_lote;
	long nro_terminal;
	char nombre[11];
	UINT16 fecha_vto;
};//seba

struct _tarjetas {
	 BINARY status;						//0
	 UINT16 cod_tarjeta;				//1
	 char nombre[11];					//3
	 float limite;						//14
	 UINT16 nro_archivo;				//18
	 BINARY digitos_1_bloque;			//20
	 BINARY nro_funcion_valid;			//21
	 BINARY offset_1_bloque;			//22
	 UINT16 fecha_vto_boletin;			//23
	 char nro_comercio_pesos[21];		//25
	 BINARY pide_numero;				//46
	 UINT16 nro_boletin;				//47
	 BINARY on_line;					//49
	 float limite2;						//50
	 char manual_especial;				//54
	 char nro_comercio_dol[21];			//55
	 BINARY cod_numeracion;				//76
	 BINARY nodo;						//77
	 BINARY captura_on_line;			//78
	 BINARY largo1;						//79
	 BINARY largo2;						//80
	 BINARY monedas;					//81   // 0-> $/uSs  1-> $/Cecor
	 BINARY celda_pinpad;				//82
	 BINARY largo_pin;					//83
	 char imprime_mensaje;				//84
	 char pide_cod_vigencia;			//85
	 BINARY val_voucher_pesos;			//86
	 BINARY val_voucher_dolares;		//87
	 char boletin_on_line;				//88
	 BINARY id_registro;				//89
	 char confirmar_numero_tarjeta;     //90
	 char nro_comercio_pesos_cuotas[21];    //91
	 char nro_comercio_dol_cuotas[21];		//112
	 UINT16 cambia_marca;					//133
	 BINARY diferimiento;					//135
	 UINT16 diferimiento_maxima_cant_dias;  //136
	 float recargo_diferimiento;			//138
	 float maximo_vuelto;					//142
	 char pedir_fecha_vto;					//146
	 INT16 cod_solicitud;					//147
	 float pago_imp_maximo;					//149
	 float pago_imp_minimo;					//153
	 char tarjetas_esp_track2;				//157
	 char permite_ingreso_manual;			//158
	 char invertir_tracks;					//159
	 float porc_vuelto;						//160
	 char enmasc_nro_tarjeta;				//164
	 char permite_consulta_saldo;			//165
	 BINARY cod_medio_vuelto;				//166
	 BINARY cod_submedio_vuelto;			//167
	 char vuelto;							//168
	 long prefijo_bin;						//169
	 long prefijo_cod_barra;				//173
	 INT16 permite_id_barra;				//177
	 INT16 permite_id_banda ;				//179
	 char hab_nc;							//181
	 INT16 tipo_tarjeta;					//182
	 char hab_promo;						//184
	 char hab_voucher_dev;					//185
	 char pide_supervisor;					//186
	 char      usar_fecha_venc_defecto; //(S-N) //187
	 INT16     fecha_vencimiento;         //189  
     char    exclusivo_online;            //190
	 char enmasc_fecha;				//191
	 UINT16 enmascara_desde;		
	 UINT16 enmascara_hasta;		
	 BINARY fin;
};

struct _tar_esp
{
    BINARY status;
    _BINARY cod_tesp;
    char desc_tesp[21];
    char cod_tarjeta;
    char prefijo_tesp[11];
    short int offset;
    short int digitos;
	char autor_devol_offline;
    BINARY fin;
};

/*
 * Pide numero: 0 -> pide cupon de 1 pago
 * 1 -> pide cupon de varios pagos
 * 2 -> pide cupon cuando financia la tarjeta
 * 7 -> financiacion tarjeta habilitada
 */
struct _bancos
{
    BINARY status; //0
    INT16 nro_banco;//1
    char nom_banco[21];//3
    INT16 cod_valida_cheque;//24
    BINARY fin;
};

struct _cta_banco
{
    INT16 cod_banco;
    INT16 nro_suc_banco;
    char cod_cuenta[2][10];
};


struct _clientes_nov {
	BINARY status;
	unsigned long novedad;
	char novedadregistro;
	long cod_cliente;
	char nombre[31];
	char domicilio[41];
	char localidad[21];
	char provincia[16];
	char telefono[21];
	BINARY cond_iva;
	char viejo_cuit[12];
	char ing_brutos[11];
	char ctacte;
	char cheques;
	char banco[16];
	char nro_cuenta[11];
	double cupo;
	double saldo;
	char giro[31];
	char exento_ing_brutos;
	char realizar_percepcion_dgi;
	char habilita_cheque_3ro;
	INT16 plazo_financiacion;
	struct _cta_banco cuentas[2];
	double limite_cheques;
	double cupo_usado_cheques;
	BINARY tasa_ingr_bruto;
	char exento_ing_cap;
	INT16 perfil_impositivo;
	char hab_envio_domic;
	char cod_documento;
	long nro_documento;
	float descuento_especial;
	long cod_cliente_titular;
	char hab_cliente_frecuente;
	UINT16 fecha_nac;
	float porc_desc;
	BINARY cod_mutual;
	UINT16 fec_entrega_ptos_cumple;
	float puntos;
	char replicar;
	long cod_perfil;
	char cuit[21];
	char nri[15];
	char cod_grupo_cliente;
	char valor_grupo_cliente[21];
	char empleado;
	double cupo_sobre_giro;
	BINARY empresa;
	char habilitado;
    char retencion_descuento;
	float     porc_exento_ib;
    UINT16    fec_ini_exento_ib;
    UINT16    fec_fin_exento_ib;
	UINT16    fec_fin_acredita; //percepcion tacis municipal( fecha cobrar aun sin considerar el minimo
    UINT16    fec_fin_acred_dgr; //retencion dgr o Percepcion IIBB( Instore) sin considerar el minimo
    UINT16    fec_ini_tacis;    //tacis, fecha entre los que se cobrara la percercepcion municipal.
    UINT16    fec_fin_tacis;  //
	BINARY fin;
};

struct _clientes {
	BINARY status;
	long cod_cliente;
	char nombre[31];
	char domicilio[41];
	char localidad[21];
	char provincia[16];
	char telefono[21];
	BINARY cond_iva;
	char viejo_cuit[12];
	char ing_brutos[11];
	char ctacte;
	char cheques;
	char banco[16];
	char nro_cuenta[11];
	double cupo;
	double saldo;
	char giro[31];
	char exento_ing_brutos;
	char realizar_percepcion_dgi;
	char habilita_cheque_3ro;
	INT16 plazo_financiacion;
	struct _cta_banco cuentas[2];
	double limite_cheques;
	double cupo_usado_cheques;
	BINARY tasa_ingr_bruto;
	char exento_ing_cap;
	INT16 perfil_impositivo;
	char hab_envio_domic;
	char cod_documento;
	long nro_documento;
	float descuento_especial;
	long cod_cliente_titular;
	char hab_cliente_frecuente;
	UINT16 fecha_nac;
	float porc_desc;
	BINARY cod_mutual;
	UINT16 fec_entrega_ptos_cumple;
	float puntos;
	char replicar;
	long cod_perfil;
	char cuit[21];
	char nri[15];
	BINARY cod_grupo_cliente;
	char valor_grupo_cliente[21];
	char empleado;
	double cupo_sobre_giro;
	BINARY empresa;
	char habilitado;
    char retencion_descuento;
	float porc_exento_ib;
    INT16 fec_ini_exento_ib;
    INT16 fec_fin_exento_ib;
	INT16 fec_fin_acredita; //percepcion tacis municipal( fecha cobrar aun sin considerar el minimo
    INT16 fec_fin_acred_dgr; //retencion dgr o Percepcion IIBB( Instore) sin considerar el minimo
    INT16 fec_ini_tacis;    //tacis, fecha entre los que se cobrara la percercepcion municipal.
    INT16 fec_fin_tacis;  //
	float porc_exceptuar_percep;
	INT16 fecha_vto_excepcion;
	INT16 fecha_ini_excepcion;
	INT16 fec_ini_ins_municipal;
	INT16 fec_fin_ins_municipal;
	char  realizar_percepcion_tacis;
//	char  mayorista_hab; lo puse para saber que existe este campo en la version multiplataforma
//	BINARY alcanzado_iva;
	BINARY fin;
};

struct _alicuota {
   BINARY status;
   INT16  municipio;  // 1- Tucuman (TACIS)   2- Posadas .. y otros a medida que se agreguen
   BINARY  cod_iva;    // condicion de iva a la cual se aplica.
   double  alicuota;
   double  minimo;     // m�nimo importe a partir del cual se aplica la alicuota
   INT16  tipo_control_dias_percepcion;
   INT16  dias_devolucion_percep;
   BINARY fin;
}; 

struct _porcperc {
   BINARY status;
   BINARY  tasa_iibb;    // ej 0-local  1-convenio  -2- Nuevocaso
   BINARY  cod_iva;    // condicion de iva a la cual se aplica.
   double  alicuota; //porcentaje a cobrarle
   double  minimo;     // m�nimo importe a partir del cual se aplica la alicuota, 0 no se tiene en cuenta el 		      minimo
   INT16  tipo_control_dias_percep; // Para las NC
   INT16  dias_devolucion_percep; //para el case 1 del campo anterio r cantidad de dias validas para devolver la percepcion. NC
   BINARY fin;
}; 

struct _articulo
{
	BINARY status;
	char cod_barra[17];
	char nombre[31];
	unsigned long cod_interno;
	double precio_sin_iva;
	double imp_int;
	double costo;
	unsigned long cod_enganche;
	UINT16 nro_registro;
	char envase;
	char dev_envase;
	char pesable;
	char gravado;
	char precio;
	char habilitado;
	unsigned long autoinc;
	char marca[16];
	//BINARY cod_dpto_old;
	char unidad[3];
	char contenido[5];
	float porc_ila;
	float porc_iha;
	BINARY cod_impuesto[4];
	double precio_sin_iva_cr;
	char cr_habilitado;
	char tipo_precio;
	long cod_clasificacion_old;
	BINARY novedad_registro;			// 0 - Actualizado  1 - Alta  2 - Modificado
	UINT16 financiacion_fecha;
	UINT16 financiacion_dias;
	char incluible_promocion;
	INT16 tipo_envase;
	char habilitado_cuotas;
	BINARY cod_iva;
	char incluible_promo_cred;
	char habilita_descuento_especial;
	BINARY atrib_descuentos;			// Bit 0 a 2 - Habilitado descuentos 1,2,3
	INT16 cod_promocion[4];
	long cod_articulo_promocion;
	char imprimir;
	char bien_de_uso;
	INT16 cod_dpto;
	INT16 cod_cobro;
	INT16 cod_solicitud;
	char pedir_supervisor;
	BINARY hab_desc_cliente;
	char multiplicable;
	BINARY cod_exento;
	float porc_impuesto[4];
	char aceptar_precio_cero;
	char vta_asistida;
	char participa_donacion;
	INT16 cod_banda;
	char habilitado_oferta;
	char cod_interno_alfa[21];
	char cod_enganche_alfa[21];
	char imp_cod_barra;
	UINT16 cod_prov;
	char cod_clasificacion[36];
	INT16 tipo;
	BINARY fin;
};

struct _articulo_ext
{
    double iva1;
    double iva2;
    double precio;
    double ila;
    double iha;
    double ica;
    float porc_ila;
    float porc_iha;
    float porc_ica;
    double monto_imp[4];
    float porc_imp[4];
    INT16 sumar_al_tot[4];
};

struct _dev_envases {
	BINARY status;			//0
	long cod_transaccion;   //1
	long cod_interno;		//5
	char cantidad;			//9
	UINT16 fecha;			//10
	UINT16 hora;			//12
	UINT16 caja;			//14
	UINT16 cajero;			//16
	long nro_ticket;		//18
	UINT16 fecha_proc;      //22
	UINT16 hora_proc;		//24
	char estado;			//26
    // 1- pendiente
	// 2- cerrado
    BINARY fin;
};

struct _aux_envases {
	 BINARY status;				//0
	 long cod_interno;			//1
	 char cantidad_voucher;     //5
	 char cantidad_facturados;  //6
	 char tipo;					//7
     BINARY fin;
};


struct _present
{
	BINARY status;				//0
	unsigned long autoinc;		//1
	BINARY cod_present;			//5
	float cantidad;				//6
	float porcentaje;			//10
	char habilitada;			//14
	BINARY dig1_dun14;          //15
	BINARY fin;  			//17
};

struct _ind_present
{
    unsigned long autoinc;
    BINARY cod_present;
};

struct _comprobante
{
    INT16 tipo_comp;
    INT16 subtipo;
    INT16 fecha;
    INT16 hora;
    INT16 nro_suc;
    //long nro_comp;
	double nro_comp;
    char cond_cli;
    INT16 copias;
    INT16 cant_formularios;
    char sucursal[71];
    char emision[16];
};


struct _acu_art
{
    long cod;
    float cant;
    INT16 pos;
    BINARY status;
    char cod_barra[17];
    INT16 cod_promocion[5];
    double importe;
    double descuento_cantidad;
    float cant_excluir;
};
struct _articulo_dif
{
	INT16 pos;
	long cod_articulo;
	char cod_barra[17];
	float cantidad;
};
struct _cont_art
{
	long cod;
	char cod_barra[17];
	int cant_consecutivos;
	int cant;
	INT16 pos;
};
struct _acu_iva
{
    double importe_sin_iva;
    double exento;
    double iva1;
    double iva2;
    //double    ila;
    //double    iha;
    //double    alicuota;
    double monto_impuesto[4];
	double imp_int;
};

struct _version
{
    char producto[3];
    char macroversion;
    char version;
    char sub_version;
    int build;
    char release;
    char branch[75];
};

struct _menu
{
    char *nom;
    INT16 tecla;
    INT16 rta;
    INT16 index;
};

struct _clasif1

{
	BINARY status;				//0
	BINARY cod_clasif1;			//1
	char descripcion[15];		//2
	BINARY digito_ini;			//17
	BINARY digito_fin;			//18
	BINARY fin;			//19
};

struct _articulo_iva
{
    BINARY status;
    char cod_iva;
    float porc_iva1;
    float porc_iva2;
    char nombre_iva[10];
    float tasa_dgi;
    float porc_212;
    BINARY fin;
};

struct _cliecheq {
	BINARY status;      //0
	long nro_cliente;      //1
	UINT16 cod_banco;      //5
	UINT16 suc_banco;      //7
	char nro_cuenta[10];      //9
	long nro_cheque;      //19
	double importe;      //23
	UINT16 fecha_vencimiento;      //31
	UINT16 dia_operativo;      //33
	UINT16 hora;      //35
	UINT16 caja;      //37
	UINT16 cajero;      //39
	char estado;      //41
	// 0- pendiente de acreditacion
	// 1- acreditado
	// 2 - rechazado
	//char reserva[50];
	char cod_banco_str[10];      //42
	char nro_cheque_str[15];      //52
	BINARY fin;     //67
};

struct _stringt
{
    BINARY status;
    INT16 id;
    char mensaje[100];
    char alias[50];
	UINT16 pausa;
    BINARY fin;
};

//struct _depto_sic {
struct _departamentos {
	BINARY status;				//0
	INT16 cod_depto;			//1
	char nom_depto[12];		//3
	char atrib_descuentos;     //15
	INT16 cod_rubro;			//16
	char reportar;				//18
	BINARY fin;
};


// nota: char      atrib_descuentos;
// Bit 0 a 2 - Habilitado descuentos 1,2,3
// Bit 7 en 1 tomar atributos del dpto,
// si no todos estan habilitados

struct _rta_terminal
{
    char cod_rta;
    char autorizacion[13];
    char cupon[13];
    char adherente[9];
    UINT16 lote;
    float anticipo;
    float saldo;
    float cuota;
    char cuotas;
    UINT16 primer_vencimiento;
    char moneda;                  /* 0-Pesos , 1-Dolar  2-Cecor */
    char nro_referencia[12];
    char mensaje[37];
    char working_key[8];
    INT16 fecha_contable;
    INT16 fecha_host;
    long hora_host;
    char nro_cuenta[20];
    float tasa_aplicada;
	double monto_compra;
    double    importe;           //importe cobrado por la tarjeta de cr�dito.
	INT16 nro_ticket;			

};

struct _t_mutuales
{
	BINARY status;					//0
	BINARY cod_mutual;				//1
	char des_mutual[12];			//2
	float variacion;				//14
	unsigned long cod_articulo;    //18
	INT16 id_registro;				//22
	char codigo_barra[17];			//24
	BINARY tipo_boletin;			//41
	INT16 nro_archivo;				//42
	char autorizacion;				//44
	char datos_adicionales;		//45
	float vuelto_maximo;			//46
	BINARY cod_medio_vuelto;		//50
	BINARY cod_submedio_vuelto;    //51
	BINARY vuelto;					//52
	INT16 cod_solicitud;			//53
	float pago_imp_maximo;			//55
	float pago_imp_minimo;			//59
	INT16 cod_restriccion_art;     //63
	INT16 cod_valida;				//65
	float porc_vuelto;				//67
	INT16 cod_medio_padre;			//71
	BINARY cheque;					//73
	double cotizacion;				//74
	char moneda_extranjera;		//82
	char simbolo_moneda[4];		//83
	char pide_supervisor;			//87
	char		   exclusivo_promocion;	// 'S' o 'N'
	char      hab_ventas;
	//Se agrega campo fin para darle el corte en mysql sino pisa memoria para cuando la tabla
	//termina en char
	BINARY fin;
};

struct _cuo_mut
{
	BINARY status;				//0
	BINARY cod_mutual;			//1
	BINARY nro_orden;			//2
	char descripcion[21];      //3
	BINARY nro_cuota;			//24
	float rec_cuota;			//25
	BINARY fin;			//29
};

struct _ind_cuo_mut
{
    char cod_mutual;
    char nro_orden;
};

//struct _event_cod_barra
struct _ev_cod_barra

{
    BINARY status;				//0
    INT16 caja;					//1
    long nro_z;					//3
    long id_evento;				//7
    BINARY posicion_medio;		//11
    BINARY posicion_cod_barra;	//12
    INT16 cuenta;				//13
    char codigo_barra[80];		//15
    double importe;				//95
    long nro_comprobante;		//103
    long nro_cuenta_comprob;	//107
    INT16 cod_sucursal;			//111
    INT16 fecha_ticket;			//113
    long caja_z;				//115
    INT16 motivo;				//119
    INT16 origen;				//121
    char texto;					//123
    INT16 puntos;				//124
    double importe_original;	//132
    char nro_cuenta_ext[41];	//175
	double nro_comprobante_new;	//132
    BINARY fin;			//200
};

struct _t_compra {
	BINARY status;						//0
	BINARY cod_t_compra;				//1
	char des_t_compra[12];				//2
	float variacion;					//14
	unsigned long cod_articulo;		//18
	INT16 id_registro;					//22
	char habilitado_scanner;			//24
	BINARY mascara_scanner[32];		//25
	char codigo_barra[17];				//57
	char tipo_boletin;					//74
	INT16 nro_archivo;					//75
	char autorizacion;					//77
	char datos_adicionales;			//78
	float vuelto_maximo;				//79
	BINARY cod_medio_vuelto;			//83
	BINARY cod_submedio_vuelto;		//84
	char vuelto;						//85
	INT16 cod_solicitud;				//86
	float pago_imp_maximo;				//88
	float pago_imp_minimo;				//92
	float porc_vuelto;					//96
	INT16 cod_medio_padre;				//100
	INT16 cod_validacion;				//102
	char pide_supervisor;				//104
	INT16 cod_restriccion_art;			//105
	char exclusivo_promocion;	// 'S' o 'N'//108
	//Se agrega campo fin para darle el corte en mysql sino pisa memoria para cuando la tabla
	//termina en char
	BINARY fin;
};

//extern struct rendicion_en_memoria *mem_tarjetas;
//extern struct rendicion_en_memoria *mem_mutuales;
//extern struct rendicion_en_memoria *mem_t_compra;
//extern int cant_mem_tarjetas;
//extern int cant_mem_mutuales;
//extern int cant_mem_t_compra;


/*struct templates {
    BINARY      status;
    INT16     cod_template;
    char      mask_template[16];
    INT16     cant_prefijo;
    double    monto_max;
   char     reserva[11];
};*/

struct templates
{
    BINARY status;
    INT16 cod_template;
    char mask_template[16];
    BINARY evaluar_tipo_art;     //precio/peso
    INT16 cod_tipo1;
    char mask_template1[16];
    INT16 cod_tipo2;
    char mask_template2[16];
    INT16 cod_tipo3;
    char mask_template3[16];
    BINARY fin;
};


struct _boletin_protectivo
{
	BINARY status;			//0
	INT16 nro_archivo;      //1
	char nro_bono[20];      //3
	float importe;			//23
	char utilizado;			//27
	INT16     fecha_vigencia;//28
    BINARY fin;
    /*
    02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
    UINT16   fecha_vigencia;
    char      reserva[13];
    */
};

struct _estado_cajeros
{
    BINARY status;
    INT16 nro_cajero;
    INT16 nro_caja;
	INT16 fecha;
    BINARY estado_cajero;
	BINARY fin;
};

struct _cobros {
 BINARY status;      //0
 INT16 codigo;      //1
 char nombre[21];      //3
 INT16 id;      //24
 INT16 cod_valida;      //26
 BINARY nro_funcion_valid;      //28
 BINARY valid_comprobante;      //29
 BINARY ctacte;      //30
 BINARY tarjeta;      //31
 char permite_pago_venci;      //32
 char mascara_scanner[80];      //33
 char validacion_pie;      //113
 char autor_offline;      //114
 char autor_anulacion_offline;      //115
 BINARY cod_especial;      //116 // 0 = cobro normal, 1 = cobro de pines
 UINT16 cod_tarjeta;      //117
 INT16 cod_solicitud;      //119
 INT16 grupo_cobro;      //121
 BINARY cobro_a_cuenta;      //123
 INT16 cod_funcion;      //124
 INT16 codigo_cobro_padre;      //126
 char cod_item_compania[13];      //128
 double importe;      //141
 char devolucion;      //149
 double costo_servicio;      //150
 double iva_costo_servicio;      //158
 BINARY datos_especiales;      //166
 BINARY  hab_todos_medios;
 //INT16 impresiones_esp;      //167
 BINARY hab_todos_medios_dev;      //169
 BINARY cod_medio_me;      //170
 BINARY cod_submedio_me;      //171
 char barra_obligatoria;      //172 //S/N
 INT16 venta;      //173
 INT16 cobro_unico;      //175
 BINARY online;      //177
 char cod_barra[17];      //178
 unsigned long cod_interno;      //195
 char cod_interno_alfa[21];      //199
 INT16 datos_pedir;      //220
 BINARY fin;
};

struct _motivos {
	BINARY status;					//0
	INT16 cod_motivo;				//1
	char des_motivo[50];			//3
	INT16 cod_solicitud;			//53
	INT16 mostrar;					//55
	INT16 supervisor;				//57
	INT16 concepto;					//59
	BINARY fin;
};

struct _ev_dmedios {
    BINARY status;                //0
    long id_evento;             //1
    long caja_z;                //5
    INT16 caja;                 //9
    long nro_z;                 //11
    BINARY pos_pago;              //15
    BINARY indice_posicion;       //16
    INT16 nro_orden;            //17
    INT16 id_dato;              //19
    char valor_viejo[31];       //21
    BINARY boletin_consultado;    //52
    INT16 cod_supervisor;       //53
    INT16 cod_sucursal;         //55
    INT16 fecha_ticket;         //57
    char valor_en_boletin[20];  //59
    INT16 cod_solicitud;        //79
    INT16 posicion;             //81
    double importe;             //83
    INT16 accion;               //91
    INT16 nro_acumulador;       //93
    UINT16 fecha_vigencia;      //95
    INT16 tipo_boletin;         //97
    INT16 nro_boletin;          //99
    BINARY anulado;             //101  // 0 - Normal, 1 - Anulado, 2 - Retirado
    char valor[121];            //102
    INT16 origen;               //223
    INT16 tipo_entero;        //225
    double tipo_double;       //227
    BINARY fin;        //235
};

struct _ind_evdpago_3
{
    long id_evento;
    INT16 nro_orden;
};

struct _ind_evdpago_5
{
    long id_evento;
    INT16 cod_solicitud;
    INT16 nro_orden;
    char indice_posicion;
};

struct _ind_evdpago_6
{
    long id_evento;
    INT16 cod_solicitud;
    INT16 nro_orden;
    char pos_pago;
};

struct _dmedios {
 BINARY status;      //0
 INT16 cod_medio;      //1
 INT16 cod_submedio;      //3
 INT16 nro_orden;      //5
 INT16 id_dato;      //7
 char descripcion[21];      //9
 INT16 tipo_dato;      //30
 INT16 digitos;      //32
 INT16 digitos_minimos;      //34
 INT16 tipo_boletin;      //36
 INT16 nro_boletin;      //38
 char verificar_importe;      //40
 char boletin_en_servidor;      //41
 char permite_consulta_local;      //42
 char mascara_boletin[41];      //43
 char permite_boletin_sup;      //84
 char habilitado;      //85
 char mascara_scanner_viejo[41];      //86
 INT16 cod_solicitud;      //127
 char lector_cheque;      //129
 char pos_pedido;      //130
 char operacion;      //131
 // 0 = los datos adicionales se piden siempre
 // 1 = los datos adicionales solo se piden en ventas
 // 2 = los datos adicionales solo en devoluciones.
 char nro_funcion_valid;      //132
 char mascara_scanner[121];      //133
 INT16 cod_valida;      //254
 INT16 cod_referencia;      //256 //indica si es importe, Nro comprobante, etc.
 char imprimible;      //258 // valores numericos 1 o 0, indica si el dato adicional es imprimible en comprobantes.
 INT16 cod_descriptor;      //259
 char modo_descriptor;      //261
 INT16 tipo_mensaje;      //262
 // 0 = no se muestra mensaje
 // 1 = se muestra antes de pedir los datos adicionales
 // 2 = se muestra despues de pedir los datos adicionales
 char mensaje[50];      //264
 char reingresar;      //314
 BINARY fin;		   //315
};

struct _ev_super {
    BINARY status;            //0
    long id_evento;         //1
    long caja_z;            //5
    INT16 caja;             //9
    long nro_z;             //11
    long hora;              //15
    INT16 pos_contenido;    //19
    INT16 pos_pago;         //21
    INT16 cod_funcion;      //23
    INT16 cod_supervisor;   //25
    long legajo_supervisor; //27
    BINARY clave_ok;        //31
    INT16 cod_sucursal;     //32
    INT16 fecha_ticket;     //34
    long posicion;          //36 //40 //este campo no se envia al dbrouter
    BINARY fin;
};

struct _olmedios {
 BINARY status;				//0
 INT16 cod_medio;			//1
 INT16 cod_submedio;		//3
 char on_line_habilitado;   //5
 char admite_offline;       //6
 double limite_offline;     //7
 INT16 cod_marca;			//15
 INT16 nodo;				//17
 INT16 numerador_cupon;     //19
 char mascara_track2[41];   //21
 char mascara_comercio[21]; //62
 char mascara_fecha_vto[21];//83
 char mascara_terminal[21]; //104
 char mascara_cuota[21];    //125
 BINARY fin;			//146
    /*
    02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
    char      mask_operaciones_habilitadas[8];
    INT16     id_dato_autorizacion;
    char      reserva[14];        // 170 Bytes
    */
};

struct _tipo_eventos
{
    BINARY status;
    char cod_evento;
    char inc_nro_evento;
    BINARY fin;         // 20 Bytes
};

struct tab_det
{
    BINARY status;
    long ident;
    char posic;
    long art;
    float cant;
    float prec;
    char cod[17];
    BINARY fin;
};

struct tab_trans
{
    BINARY status;
    long ident;
    long op;
    long serie;
    INT16 fec;
    INT16 hora;
    INT16 nro_caja;
    INT16 nro_cajero;
    BINARY fin;
};

struct _promo_sic
{
    BINARY status;
    INT16 cod_promo;
    char tipo_promo;
    char benef_promo;
    char tipo_vigencia;
    UINT16 fecha_desde;
    UINT16 fecha_hasta;
    UINT16 hora_desde;
    UINT16 hora_hasta;
    float porc_desc;
    unsigned long cod_art;
    UINT16 formato_cupon;
    UINT16 no_usado;              // cant_grupo;
    float monto;
    char credito;             // 'S' o 'N'
    char limita_medios;   // 'S' o 'N'
    char exclusivo_afinidad;    // 'S' o 'N'
    float monto_max;
    unsigned char cant_grupo[CANT_GRUPOS];
    char excluir_articulos; // 'S' o 'N'
    char modo_evaluacion;     // 0 Pre-pago  1 Pos-pago
    float puntos;
    UINT16 limite_entrega;
    float puntos_minimo;
    char redondear_importe; // 'S' o 'N'
    char modificar_puntos;  /* 0 - No   1 Redondear  2 Truncar */
    INT16 id_registro;
    char nombre_promo[21];
    INT16 cantidad1;              // Agregado Promo.

    char lugar_impresion;
    char nro_acumulador;
    char grabar_en_eventos;
    char tipo_fecha_vigencia;//1:fecha invel hasta, 2:cant dias, 3:cant meses.
    UINT16 fecha_vigencia;      // Fecha o dias o meses.
    double limite_monto_inf;
    double limite_monto_sup;
    char acumular_nxm;           // 'S' , 'N'
    char tipo_cupon;            // Para beneficion cupon
    INT16 formato_cupon_pant;
    INT16 grupo_cupon;
    double monto_min;
    char cada_apartirde;     // Integer de 1 byte
    char con_osinrepeticion; // Integer de 1 byte
    char mejoritem;          // Integer de 1 byte
    char tipo_prorrateo;     // Integer de 1 byte
    char consulta_remota_puntos; // 'S' o 'N'
    INT16 caja_desde;
    INT16 caja_hasta;
    char evaluar_mayor_beneficio;      // 'S' , 'N'
    long cod_premio;
    INT16 cant_pro_plu;
    //char reserva[17];
};

/*
 * Tipo_prom
 * 1-articulo
 * 2-departamento
 * 3-canasta
 * 4-grupo
 * 5-incondicional -> con cupon
 * 6-monto         -> con cupon
 * 7-artculos diferentes
 * 8-artculos exceptuados
 * 9-clasificacion
 *
 * Benef_promo
 * 1-descuento
 * 2-cupones
 * 3-producto
 * 4-mostrar
 * 5-acumula cupones
 * 6-cupon
 * 7-mostrar un cupon
 * 8-agregar articulos
 *
 * Tipo_vigencia
 * 1-diario
 * 2-domingo
 * 3-lunes
 * ...
 * 8-sabado
 */
struct _pro_art
{
    BINARY status;
    INT16 cod_promo;
    unsigned long cod_art;
    INT16 cantidad;
    char cupones;
    float puntos;
    char cod_barra[17];
    //char reserva[19];
    /*
    02/12/2004-- fabiana -- se comenta para poder igualar la base de datos
    float     porcentaje;
    double    monto;
    char      reserva[7];
    */
};

struct _pro_dep_sic
{
    BINARY status;
    INT16 cod_promo;
    char cod_depto;
    //char reserva[6];
};

struct _pro_med_sic
{
    BINARY status;
    INT16 cod_promo;
    char cod_medio;
    char cod_submedio;
    char nro_orden;
    char reserva[4];
};

struct _ind_pro_med_sic
{
    INT16 cod_promo;
    char cod_medio;
};

struct _pro_clas_sic
{
    BINARY status;
    INT16 cod_promo;
    char cod_clasif1;
    long valor;
    INT16 cantidad;
    char cupones;
    float porcentaje;
    double monto;
    //char      reserva[17];
    char beneficiario;
    //char reserva[16];
};

struct ind_pro_clas_sic
{
    INT16 cod_promo;
    char cod_clasif1;
    long valor;
};

struct _art_ben
{
    BINARY status;
    INT16 cod_promo;
    unsigned long cod_art;
    double precio;
    unsigned long cod_art_agr;
    float cantidad;
    float descuento;
    char cod_barra[17];
    //char reserva[26];
};

struct _pro_cli
{
	BINARY status;//0
    INT16 cod_promo;//1
	long cod_cliente;//3
	char reserva[13];//7
};

struct _pro_fid
{
    BINARY status;
    INT16 cod_promo;
    char mes_cumpleanos;
    char tipo_tarjeta;        // 0=ninguna; n=cod_tarjeta_afinidad (de tar_afin.sic)
    char actividad_cliente;
    char limite_diario;
    char dia_cumple;
    char cond_tit;
    char pedir_confirmacion;    // 'S' o 'N
    char tiene_limite_promo;
    char limite_promo;
    char leyenda[21];
    float puntos_exigidos;
    INT16 formato_cupon1;
    INT16 formato_cupon2;
    //char reserva[63];
};

struct _pro_plu {
    BINARY status;
    INT16 cod_promo;
    char sucursal;
    char tipo_promo;
    unsigned long cod_art;
    char cod_barra[17];
    INT16 cantidad;
    char cod_art_alfa[21];
    //char reserva[11];
};

struct _cant_tickets
{
    BINARY status;
    INT16 tipo_evento;
    INT16 cod_medio;
    INT16 cod_submedio;
    INT16 cant_tickets;
    INT16 cant_vouchers;
	BINARY fin;
};
struct _tot
{
    double importe_sin_iva;
    double imp_int;
    double costo;
    double recargo_tarjeta;
    double exento;
    double iva1;
    double iva2;
    //double    ila;
    //double    iha;
    INT16 bultos;
    //double    alicuota;
    double monto_impuesto[4];
};
struct _datos_terminal
{
    char tipo_de_operacion;
    char lectura_banda;   /* 0 - No, 1 - Si */
    char datos_tarjeta[40];
    char vto_tarjeta[4];      /* AAMM */
    long cupon;
    long cupon_original;
    char moneda;                  /* 0 - Pesos , 1 - Dolar */
    char cuotas;
    double importe_total;
    double monto_entrega;
    char plan_de_pagos[6];
    UINT16 primer_vencimiento;
    char vigencia;
    char marca_tarjeta;
    INT16 boletin;
    INT16 cajero;
    long hora;
    INT16 fecha;
    INT16 fecha_original;
    char numero_comercio[21];
    char autorizacion[13];
    char nro_referencia[12];
    INT16 cvv;
    long terminal;
    char tipo_cuenta;         // 0-No, 1-Caja Ahorro, 2-CtaCte
    char pin[8];
};

struct _bandas_ventas {
	BINARY status;      //0
	INT16 cod_banda;      //1
	char nombre[30];      //3
	UINT16 fecha_desde;      //33
	UINT16 fecha_hasta;      //35
	UINT16 hora_desde;      //37
	UINT16 hora_hasta;      //39
	BINARY fin;      //41
};

struct _band_art
{
    BINARY status;
    INT16 cod_banda;
    unsigned long cod_art;
    char cod_barra[17];
	BINARY fin;
};

struct _banda_clasificacion {
	BINARY status;      //0
	INT16 cod_banda;      //1
	char cod_clasif;      //3
	long valor;      //4
	BINARY fin;      //8
};


struct ind1_band_art_sic
{
    INT16 cod_banda;
    unsigned long cod_art;
};

struct ind2_band_art_sic
{
    INT16 cod_banda;
    unsigned long cod_art;
    char cod_barra[17];
};

struct ind_band_clas_sic
{
    INT16 cod_banda;
    char cod_clasificacion;
    long valor;
};

struct _band_acu_art
{
    long cod;
    float cant;
    INT16 pos;
    char cod_barra[17];
    INT16 cod_banda;
    double importe;
};

struct _datos_clasif
{
    INT16 cod_banda;
    long cod_clas_old;
    char encontro;
    INT16 cantidad;
	char cod_clas[36];
};
//struct _cuo_med
struct _cuotas_medio
{
    BINARY status;
    UINT16 cod_medio;
    UINT16 cod_submedio;
    UINT16 nro_cuota;
    UINT16 cantidad;
    char descripcion[21];
    float recargo;
    float porc_entrega;
    INT16 datos_adicionales;
    INT16 nro_datos_adicionales;
    INT16 id_dato_fecha;
    INT16 offset_dias_primera_cuota;
    INT16 offset_meses_primera_cuota;
    BINARY fin;
};

struct _ev_despro
{
    BINARY status;            //0
    long id_evento;         //1
    INT16 posicion;         //5
    BINARY tipo_motivo;     //7
    INT16 id_motivo;        //8
    double importe;         //10
    char codigo_barra[17];  //18
    long cod_articulo;      //35
    INT16 cod_sucursal;     //39
    INT16 fecha_ticket;     //41
    long caja_z;            //43
    INT16 caja;             //47
    long nro_z;             //49
    char no_es_dto;         //53
    BINARY fin;        //54
};

struct _ind_e_des_pro_btr
{
    long id_evento;
    char tipo_motivo;
    INT16 id_motivo;
    INT16 posicion;
};

struct _art
{
    long cod_articulo;
    float cantidad;
    double precio;
    char origen;
    INT16 posicion;
    INT16 tipo;
    double total;
};

struct _cuerpo
{
    long nro_evento;
    long nro_sucursal;
    //long nro_comprobante;
	double nro_comprobante;
    double exento;
    double imp_int;
    double iva1;
    double iva2;
    double ila;
    double iha;
    double retencion_dgr;
    double percepcion_dgi;
    double neto;
    double subtotal;
    double total;
    double total_descuento;
    double bultos;
    double costo;
    INT16 tipo;
    double alicuota;
    double total_impuestos[40];
    INT16 nro_orden;
    double monto_impuesto[4];
};

struct _ev_factu {
    BINARY status;                //0
    long id_evento;             //1
    long caja_z;                //5
    INT16 sucursal;             //9
    UINT16 fecha_ticket;        //11
    UINT16 bultos;              //13  /* Cantidad de Items */
    double importe_sin_iva;     //15
    double exento;              //23
    double importe_total;       //31
    double costo;               //39
    long cod_cliente;           //47
    char tipo_factura;          //51
    long nro_comprobante;       //52
    INT16 nro_dia_operativo;    //56
    INT16 caja;                 //58
    long nro_z;                 //60
    double imp_int;             //64
    double iva1;                //72
    double iva2;                //80
    double impuestos[40];
    double importe_descuento;   //408
    INT16 nro_orden;            //416
	double nro_comprobante_new;       //52
    BINARY fin;           //418
};



struct _id_terminal
{
    char tipo;
    char version;
    char sub_version;
    char revision;
    long numero;
};                                        // 8 bytes
//char      tipo;   :1 TPV, 2 TPV-TEL, 3 TPV-ROUTER, 4 CAT
struct _bloque_encriptado
{
	INT16 id_clave;			//6//0
	INT16 version_bloque;   //8//2
	INT16 len;				//10//4
	char buffer[512];		//12//6

};

struct _bloque_encriptado_2 //esto surge de duplicar el tama�o por hacerlo en hexa. Mysql cortaba las cadenas..
{
	INT16 id_clave;			//6//0
	INT16 version_bloque;   //8//2
	INT16 len;				//10//4
	char buffer[1024];		//

};

struct _datos_transaccion {
	char      nom_caja[4];
    struct _id_terminal id_terminal;  //8 bytes
    char      nodo;
    INT16     tipo_de_mensaje;
    long      codigo_de_procesamiento;
    char      tipo_de_tarjeta;   // 0-credito 1-debito
    char      tipo_de_cuenta;
    // 0 N/A
    // 1 Caja de ahorro $
    // 2 cuenta corriente $
    // 8 caja de ahorro u$s
    // 9 cuenta corrient u$s
    INT16     codigo_marca;
    char      modo_de_ingreso;
    // 0 manual
    // 1 banda magnetica
    char      numero_cuenta_tarjeta[21];
    char      fecha_expiracion[4];   // AAMM
    char      track_2[38];
    INT16     cvv;
    INT16     ticket;
    char      moneda;            // 0-pesos, 1-dolar, 2-cecor
    char      plan;
    char      cuotas;
    double     importe;
    //float     entrega;
    float     importe_cuota;
    float     saldo;
    char      vigencia;
    long      hora;              //HHMMSS
    long      fecha;             //AAAAMMDD
    long      autorizacion;      //N(6)
    INT16     codigo_de_respuesta;
    long      numero_de_trace;
    long      numero_de_terminal; // N(8)
    char      numero_de_comercio[15];   // N(15)
    char      mensaje[41];
    char      retrieval_reference_number[12];
    char      pin_working_key[8];
    INT16     id_internacional_de_red;
    char      reserva[9];
    UINT16    ticket_original;
    long      fecha_original;    //DDMMAA
    long      fecha_de_captura;  //MMDD
    INT16     tipo_de_mensaje_original;
    long      numero_de_trace_original;
    INT16     lote;              // N(4)
    INT16     compras;
    INT16     devoluciones;
    INT16     anulaciones;
    double    monto_compras;
    double    monto_devoluciones;
    double    monto_anulaciones;
    char      track_1[80];       //348
    char      autorizacion_alfa[11];
    INT16     fecha_contable;    //DDMMAA
    INT16     fecha_host;
    long      hora_host;
    char      nro_cuenta[20];
    long      hora_original;
    INT16     nro_boletin;
    char      autorizacion_original[11];
    float     tasa;
    double    vuelto;
    float     tasa_aplicada;
    long      nro_comp_transaccion;
    BINARY    l_pin;
    long      nro_comp_transaccion_original;
    INT16     caja_original;
    INT16     suc_original;
    char      financia_tarjeta;
    INT16     fecha_primer_vto;
    char      intervalo_cuotas;
    char      mes_cerrado;
    char      garantia;
    char      primera_cuota_vista;//1/0 Este campo lo voy a utilizar para indicar que fue procesado la transaccion CAMBIO MEDIO AUTOMATICO
    char      plan_sitef;
    char      pin_working_ascii[21]; //hasta aqui lee el jauton //462
    char datos_tarjetas_encriptados; //463
    struct _bloque_encriptado bloque_encriptado; //518 bytes //464....468 y  469...980
    INT16 cod_sucursal; //981-982 -> idsucursal) 983..985
    long caja_z; //983....986  
    long id_evento; //987..990
    double interes; ////991..998 //este campo se usara para enviar el retiro de plata en cashplus
    char   tabla[6];//999..1004
    char   nro_cuenta2[30]; //1035 agregado para multiprocolo version nueva
    char reserva1[33]; // 1068*/
};

struct _transac2
{
    BINARY status;                  //1 2
    long id_transaccion;      //autoincremental;  //2 5
    UINT16 fecha_generacion;  //6 7
    UINT16 hora_generacion;   //8 9
    UINT16 ultima_fecha_transmision;    //10 11
    long ultima_hora_transmision;   //12 15
    char prioridad;           //16 16
    INT16 operacion;              //17 18
    char enviado;             //0 no enviado 1 enviado    //19 19
    char error;               //20 20
    char nodo;                //21 21
    char enviando;            //22 22
    //fabiana
    //char      reserva[40];
    //old struct _datos_transaccion dt; // 530
    INT16 cod_sucursal;      //24
    long caja_z;            //28
    long id_evento;         //32
    UINT16 fecha_ticket;      //34
    char reserva[28];       //62
    struct _datos_transaccion dt; // 1068
	BINARY fin;
    //1130
};


struct _ind2_transac2
{
    char nodo;                // 19 1
    char enviado;             // 17 1
    char enviando;            // 20 1
    char prioridad;           // 14 1
    long id_transaccion;      //2 4
};

struct _codigos_operacion
{
    INT16 reversable;
    INT16 tipo_de_mensaje;
    long codigo_de_procesamiento;
    char prioridad;
    char un_solo_intento;
    char espera_rta;
};

struct _rta_switch_ncr
{
    BINARY status;
    INT16 rta_switch;
    INT16 rta_pos;
    BINARY fin;
};

struct _transaccion
{
    char clase;               // 0-Normal, 1-Reverso
    char operacion;           // 0-Compra, 1-Anulacin, 2-Devolucin, 3-Pago, 4-Anulacin de Pago, 5-Consulta de Saldo
    char modo;                // 0-OnLine, 1-OffLine
    long nro_transaccion;
    UINT16 fecha_operacion;
    INT16 hora_operacion;
    INT16 caja;
    INT16 cajero;
    char numero_comercio[16];
    long terminal;
    UINT16 lote;
    char nodo;
    double importe_total;
    double importe_entrega;
    char moneda;                  // 0-Pesos, 1-Dlares, 2-Cecor
    char cuotas;
    char plan[6];
    UINT16 primer_vto;
    long comprobante;
    char cuenta[40];
    char modo_ingreso;        // 0-Manual, 1-Banda
    UINT16 fecha_expiracion;
    char marca;
    INT16 boletin;
    INT16 cvv;
    char pin[8];
    char tipo_cuenta;         // 0-CtaCte, 1-Caja de Ahorro
    double intereses;
    UINT16 fecha_original;
    long comprobante_original;
    long cod_autorizacion;
    char nro_referencia[12];
    char cod_vigencia;
    char reserva[44];
};                                        // 200 bytes

struct _cam_val_hab {
	BINARY status;      //0
	char cod_medioing;      //1
	INT16 cod_submedioing;      //2
	char cod_medioret;      //4
	INT16 cod_submedioret;      //5
	INT16 cod_val_entrante;      //7
	INT16 cod_val_saliente;      //9
	char pide_supervisor;      //11
	BINARY fin;      //12
};

struct _tar_adicional {
	BINARY status;				//0
	UINT16 cod_tarjeta;		//1
	char pto_inicio;			//3
	char nro_especial[21];     //4
	UINT16 cod_tarj_especial;  //25
	BINARY fin;		//27
};

struct _cuo_gru
{
    BINARY status;
    UINT16 cod_tarjeta;
    INT16 cod_grupo;
    char descripcion[21];
    BINARY fin;
};

struct _tipo_envase {
	BINARY status;      //0
	long cod_envase;      //1
	unsigned long cod_interno;      //5
	INT16 cod_grupo;      //9
	BINARY fin;      //11
};

struct _grupo_envase
{
    BINARY status;
    INT16 cod_grupo;
    INT16 cod_tipos_combinables[10];
    char reserva[7];
};

struct _datos
{
    char valor[31];
    char boletin_consultado;    // 0 Ninguno, 1 Local, 2 Remoto
    INT16 cod_supervisor;
    char valor_en_boletin[20];
};

struct _impuestos
{
	BINARY status;				//0
	BINARY cod_impuesto;		//1
	char nom_impuesto[21];		//2
	float tasa;					//23
	INT16 cod_categoria;		//27
	INT16 accion;				//29
	BINARY fin;
};


struct _categoria_impuesto_sic
{
    BINARY status;
    INT16 cod_categoria;
    INT16 cod_formula;
    char nom_cat[21];
    char reserva[44];
};

struct _motivo_exento_sic
{
    BINARY status;
    char cod_exento;
    char descrip[21];
    char reserva[47];
};

struct _vta {
	BINARY status;					//0
	long cod_especial;
	float cantidad;				//5
	double importe_sin_iva;		//9
	double imp_int;				//17
	double importe_iva1;			//25
	double costo;					//33
	double recargo_tarjeta;		//41
	double exento;					//49
	double importe_iva2;			//57
	double importe_impuestos[4];	//65
    //  double    importe_iha;
    BINARY fin;
};

struct _art_dat
{
    BINARY status;
    unsigned long cod_interno;
    float stock;
    UINT16 fecha_ult_compra;
    float cant_ult_compra;
    BINARY fin;
};


struct _browse
{
    int alto;
    int seguir;
    int mostrar;
    int linea;
    int pos;
    int posicionar;
    int tecla;
    int cola_proceso[25];
    int cant_cola;
    int index;
    int modo;
    int registros;
};

struct _FieldDescription
{
    char colName[20];
    int width;
    int alignment;
    int type;
};
struct _dat
{
    int nFields;
    int selectedField;
    struct    _FieldDescription FieldDescription[10];
};

struct _formula_dgr {
	BINARY status;      //0
	BINARY tipo_calculo;      //1
	BINARY cod_iva_cliente;      //2
	char formula_base_imp[301];
	BINARY fin;
};

struct _valores_calculo
{
    double importe_neto;
    double iva;
    double exento;
    INT16 tkt_ini_est;
};

struct _tkt_tmp
{
    BINARY status;
    INT16 cod_medio;
    INT16 cod_submedio;
    char descripcion[21];
    double importe;
    INT16 cantidad;
    BINARY fin;
};

struct _datos_en_bin
{
    INT16 indice_posicion;
    INT16 nro_orden;
    INT16 id_dato;
    char valor[30];
    char boletin_consultado;    // 0 Ninguno, 1 Local, 2 Remoto
    INT16 cod_supervisor;
    char valor_en_boletin[20];
    INT16 cod_solicitud;
    double importe;
};

struct _data_entry {
	BINARY status;      //0
	BINARY codigo;      //1
	char descripcion[31];      //2
	INT16 cod_solicitud;      //33
	INT16 cod_validacion;      //35
	INT16 cod_funcion_especial;      //37
	BINARY fin;      //39
};



struct _valida
{
    INT16/*unsigned*/ fecha;
    INT16/*unsigned*/ fecha1;
    INT16/*unsigned*/ hora;
    double importe;
    long nro_cliente;
    INT16/*int*/ cod_medio;
    char cuenta[21];
    double comprobante;
    char mensaje[20];
    char cuit[12];
    char telefono[16];
    double saldo;
    long numero;
    long autorizacion;
    char telefono_comprador[12];
    INT16/*int*/ orden_medio;
	char cod_barra[81];
	char dato_adicional[31];
	char mensaje_largo[80];
    char *puntero_msg;
    double costo_servicio;
    char cuenta_ext[41];
    char confirmacion[11];
    long hora_larga;
    char folio[17];
    char NIP[17];
	char codigo_llavero[14];
    int vigencia;
	char mensaje2[41];
	char cond_cli;
    INT16 copias;
    INT16 cant_formularios;
    char sucursal[71];
    char emision[16];
	INT16 tipo_comp;
	INT16 nro_suc;
	INT16 subtipo;
	//long nro_comp;
	double nro_comp;
};

struct _emision_informes
{
	INT16 prod_cod_seg;
	INT16 prod_cod_vta;
};

struct _nro_cli_tmp
{
    BINARY status;
    long cod_cliente;
    BINARY fin;
};

struct _grupos_clientes {
	BINARY status;				//0
	BINARY cod_grupo_cliente;    //1
	char descripcion[31];      //2
	BINARY fin;			//33
};


//struct _cond_iva {
struct _condiciones_iva {
	BINARY status;				//0
	char cod_cond_iva;			//1
	char desc_cond_iva[31];    //2
	char cond_iva_aux;			//33
	char habilitado;			//34
	BINARY fin;			//35
};


struct _clave_tef
{
    //int id_clave;
	INT16 id_clave;
    //int id_algoritmo;
	INT16 id_algoritmo;
    //int len_bloque;
	INT16 len_bloque;
    char bloque[4096];
};

struct _perf_des
{
	BINARY status;			//0
	INT16 cod_desc;			//1
	INT16 cod_perfil;		//3
	float descuento;		//5
	char descripcion[12];   //9
	INT16 tipo;				//21
	BINARY fin;		//23
};

struct _art_perf
{
	BINARY status;			//0
	long cod_interno;		//1
	INT16 cod_perfil;		//5
	INT16 cod_desc;			//7
	char cod_barra[17];
	char cod_interno_alfa[21];
	BINARY fin;		//9
};

struct _perfiles
{
	BINARY status;			//0
	INT16 cod_perfil;		//1
	char nombre[12];		//3
	long cod_interno;		//15
	char excluir_articulos; //19
#ifdef INVEL_L
	char reserva[10];		//20
#else
	BINARY fin;		//30
#endif
};

//char      excluir_articulos; //valor por defecto = 1
//S->excluir aquellos articulos que no est� habilitados para promoci�
//N->incluir todos los art�ulos del ticket sin excepci� (habilitados o no para promos)
//D->incluir todos los art�ulos del ticket excepto los que tuvieron descuento por credito y
//   fueron incluidos en promociones

struct _clas_per
{
	BINARY status;			//0
	BINARY cod_clasif1;     //1
	long   valor;			//2
	INT16 cod_perfil;		//3
	INT16 cod_desc;			//5
	BINARY fin;		//7
};


struct _cuo_cta
{
	BINARY status;			//0
	INT16 codigo;			//1
	INT16 cuotas;			//3
	float recargo;			//5
	char habilitada;		//9
	BINARY fin;      //10
};

struct _mov_cta
{
	BINARY status;				//0
	INT16 fecha;				//1
	long nro_socio;				//3
	INT16 nro_cajero;			//7
	long nro_comprobante;		//9
	double importe;				//13
	INT16 cod_financiacion;		//21
	BINARY fin;  			//23
};

struct _operaciones_cli
{
    BINARY status;
    long nro_comprobante;
    long nro_socio;
    long id_deuda;
    INT16 fecha_comp;
    double neto_grabado;
    double iva;
    double impuestos;
    char anulado;
    char pagado;
    char cod_moneda;
    INT16 vencimiento1;
    double importe_venc1;
    double importe_min1;
    INT16 vencimiento2;
    double importe_venc2;
    double importe_min2;
    INT16 vencimiento3;
    double importe_venc3;
    double importe_min3;
    INT16 fecha_limite_pago;
    INT16 cod_cobro;
    char letra_comp;
    INT16 grupo_cobro;
    INT16 tipo_mov;
    INT16 punto_venta;
    INT16 tipo_cli;
    INT16 periodo_fac;
    INT16 cobro_antig;
    unsigned long autoinc;
    BINARY fin;
};

struct _cli_cobros
{
    BINARY status;              //1
    long id_cli_cobro;        //5
    INT16 unidad_negocio;      //7
    INT16 cod_cobro;           //9
    long nro_socio;           //13
    char empresa;             //14
    INT16 integrante;          //16
    INT16 codigo;              //18
    char apellynom[61];       //79
    char domicilio[41];       //120
    char localidad[21];       //141
    char provincia[16];       //157
    char telefono[21];        //178
    char cod_documento;       //179
    long nro_documento;       //183
    char cuit[21];            //204
    char plan[21];            //225
    char categoria[11];       //236
    char modo_emision;        //237
    char habilitado;          //238
    INT16 fecha_baja;          //240
    BINARY fin;
};

struct _ind1_cli_cobro
{
    long id_cli_cobro;
};

struct _ind2_cli_cobro
{
    char apellynom[61];
};

struct _ind3_cli_cobro
{
    char cuit[21];
};

struct _ind4_cli_cobro
{
    char cod_documento;
    long nro_documento;
};

struct _ind5_cli_cobro
{
    INT16 unidad_negocio;
    INT16 cod_cobro;
    long nro_socio;
    char empresa;
    INT16 integrante;
};

struct _ind6_cli_cobro
{
    INT16 unidad_negocio;
    INT16 cod_cobro;
    char cuit[21];
    char empresa;
};

struct _mascara_scanner
{
    BINARY status;
    INT16 id_mascara;
    INT16 cod_cobro;
    char mascara[80];
    BINARY fin;  //150
};

//SIN_LOTE_TARJETA
/*struct _lote_tar{
   long id_evento;
   long lote;
   int cod_tar;
   double importe;
   char ingresado;
};*/

struct _detalle_op_cli
{
    BINARY status;
    long id_detalle;
    INT16 fecha;
    long referencia;
    char detalle_cargo[50];
    double importe_cargo;
    double neto;
    double iva;
    double impuestos;
    long nro_comprobante;
    long nro_socio;
    char reserva[49];  //150
};


struct _tipo_movimiento
{
    INT16 grupo_cobro;
    INT16 cod_cobro;
    INT16 codigo_interno;
    char letra_comp;
    INT16 tipo_mov;
    BINARY fin; //50
};



struct _DatosCodBarra
{
    int cod_cobro;
    long punto_venta;
    INT16 letra_comp;
    double importe;
    long nro_cuenta;
    long nro_comprobante;
    int moneda;
    int anio_emision;
};

struct _pagos1
{
    BINARY status;
    long codigo;
    char nombre[21];
    INT16 id;
    char subnivel;
    char discrimina;
    INT16 cod_valida;
    BINARY fin;
};

struct _pagos2
{
    BINARY status;         //0
    long cod_nivel1;     //1
    long codigo;         //5
    char nombre[21];     //9
    INT16 id;             //30
    char discrimina;     //32
    INT16 cod_valida;     //33
    INT16 cod_solicitud;  //35
    char cod_medio;      //37
    char cod_sub_medio;  //38
    double importe;        //39
    long nro_cheque;     //47
    INT16 banco;          //51
    long cod_cliente;    //53
    char clave_id[21];   //57
    INT16 suc_pago;       //78
    long idlong;         //80
    char nro_chq_str[21];//84
    INT16 suc_origen;    //105
    long id_cli_cobro;   //107
	BINARY fin;
};

struct _ind1_pagos2
{
    long cod_nivel1;
    long codigo;
};

struct _ind_pagos2
{
    long cod_nivel1;
    long codigo;
};

struct _ind2_pagos2
{
    long cod_nivel1;
    long cod_cliente;
    long codigo;
};

struct _ind3_pagos2
{
    long cod_nivel1;
    char clave_id[21];
};

struct _ind_pagos2_4
{
    long cod_nivel1;
    INT16 banco;
};


struct _medios_para_reintegros
{
    BINARY status;         //
    long cod_nivel1;     //
    long codigo;         //
    char cod_medio;      //
    char cod_sub_medio;  //
    double importe;        //
    long nro_cheque;     //
    INT16 banco;          //
    INT16 estado;         //-1:en la caja y no enviado al dbrouter,
    //0:en caja y enviado al dbrouter
    //1:entregado para reintegro y enviado al dbrouter,
    //2:rechazado y en caja, y enviado al dbrouter;
    INT16 suc_pago;
    long idlong;
    char nro_chq_str[21];
    long id_cli_cobro;
    BINARY fin;
};

struct _ind_mreintegro_1
{
    long cod_nivel1;
    long codigo;
};

struct _cajas_suc
{
    INT16 num_caja;
    INT16 sucursal;
	BINARY fin;
};

struct _cierre_tarjetas
{
    BINARY status;      //0
    long id_evento;   //1
    long caja_z;      //5
    INT16 sucursal;   //9
    UINT16 fecha;     //11
    INT16 medio;      //13
    INT16 sub_medio;  //15
    double imp;       //17
    INT16 terminal;   //25
    INT16 lote;       //27
	BINARY fin;
};

struct _tipo_movimiento_caja
{
    INT16 tipo_mov;        //0
    char descripcion[31]; //2
    char abrev[6];        //33
    BINARY fin;     //39 //50
};

struct _ConfTkt
{
    UINT16 codigo;
    char *parametros;
    UINT16 linea;         //linea correspondiente en el tkt
};

struct _informe_sup_sic {
	INT16 codigo;				//0
	char descripcion[60];		//2
	char reserva[38];			//62
};



struct _dbRouter
{
    char tableName[30];
    char columName[30];
    int dataType;
    int columnOffset;
    int dataSize;
    int tableNumber;
    int pkey;
    char columNameSuc[30];
    BINARY fin;

};

struct _sucursales {
	BINARY status;
	BINARY cod_suc;
	char desc_suc[31];
	char path_suc[21];
	char iniciales[2];
    BINARY fin;
};


struct _datos_cobro
{
    INT16 codigo;
    //long comprobante;
	double comprobante;
    char cod_barra[81];
    char cuenta[41];
    double importe;
    char dato_adicional[31];
    char tarjeta;
	char devolucion;
	char cod_barra_articulo[21];
	INT16  anios_garantia;
	BINARY hab_todos_medios;
	BINARY hab_todos_medios_dev;
	char mensaje[41];
	INT16 datos_pedir;
	INT16 tipo_cobro;
	char cod_barra_art_venta[17];
	unsigned long cod_interno_art_venta;
	char estado;
};

struct _cob_tar
{
    BINARY status;
    INT16 cod_cobro;
    INT16 cod_tarjeta;
    BINARY fin;
};

struct _cob_med {
   BINARY status;
   BINARY  cod_medio;
   INT16 cod_submedio;
   INT16 cod_cobro;
   char      habilitado; // valores 0 o 1
   char      hab_devo; // valores 0 o 1
   BINARY fin; //20
};

struct _lin_mens {
	BINARY status;			//0
	long id_transaccion;   //1
	BINARY nro_linea;		//5
	char descripcion[41];  //6
	char nro_cuerpo;       //47  //0-encabezado 1-cuerpo 2-pie
	BINARY fin;
};

struct _datos_cli
{
    int prefijo;
    int sucursal;
    long cuenta;
};

struct _vueltos_x_medio {
	BINARY status;				//0
	char cod_medio;			//1
	char cod_submedio;			//2
	char cod_medio_vuelto;     //3
	char cod_submedio_vuelto;  //4
	INT16 cod_val_vuelto;      //5
	float max_vuelto;			//7
	float porc_vuelto;			//11
	BINARY fin;			//15
};


struct _ev_bpromo {
    BINARY status;                //0   //BYTE
    long id_evento;             //1
    INT16 id_promo;             //5
    INT16 cod_promo;            //7
    float porc_desc;            //9
    unsigned long cod_art_agr;  //13
    char cod_barra_agr[17];     //17  //STRING
    float cantidad;             //34
    double imp_sin_iva;         //38
    double exento;              //46
    double iva1;                //54
    double iva2;                //62
    double iha;                 //70
    double ila;                 //78
    double imp_int;             //86
    unsigned long cod_arti;     //94
    char cod_barra[17];         //98  //STRING
    INT16 cupones;              //115
    INT16 bonos;                //117
    INT16 tipo;                  //120
    float tot_puntos;           //120
    char cod_art_agr_ext[21];   //124 //Z_STRING
    char cod_art_ext[21];       //145 //Z_STRING
    INT16 t_promo;              //166
    INT16 b_promo;              //168
    INT16 id_grupo;             //170
    INT16 sucursal;             //172
    long caja_z;                //174
    INT16 fecha_ticket;         //178
    long nro_z;                 //180
    double precio_fijo;         //184
    INT16 posicion;             //192
    INT16 cantidad_minima;      //194
    INT16 nro_caja_dgi;         //196
    char puede_revertir;        //198 // Entero de 1 byte
    long trace;                 //199
    double monto_impuesto1;     //203
    double monto_impuesto2;     //211
    double monto_impuesto3;     //219
    double monto_impuesto4;     //227
    BINARY cod_medio;             //235
    BINARY cod_submedio;          //236
    INT16 modo_plan_pago;       //237
    char nro_tarjeta_string[21];//239
    char aplicado;              //260
	char cod_ext[25];
    BINARY fin;// 300 bytes
};


struct _garantias_porc {
	BINARY status;					 //0
	char tipo;					     //1
	char meses;						 //2
	double limite_inferior;		 	 //3
	double limite_superior;		     //11
	float porcentaje;				 //19
	BINARY fin;				 //23
};


struct _cuo_tar
{
	BINARY status;							//0
	UINT16 cod_tarjeta;			//1
	char nro_cuota;						//3
	char hab_cuota_pesos;					//4
	float rec_cuota_pesos;					//5
	char hab_cuota_fin_tar_pesos;			//9
	float rec_cuota_fin_tar_pesos;			//10
	char nro_orden;						//14
	char descripcion[21];					//15
	float entrega;							//36
	char hab_cuota_dol;					//40
	float rec_cuota_dol;					//41
	char hab_cuota_fin_tar_dol;			//45
	float rec_cuota_fin_tar_dol;			//46
	char tabla[6];							//50
	float coeficiente;						//56
	char hab_afinidad;						//60
	char pide_entrega;						//61
	char vigencia;							//62
	char controlar_total_cuotas;			//63
	char credito;							//64
	char hab_diferimiento;					//65
	float tasa;							//66
	short cod_grupo;						//70
	char unicamente_online;				//72
	char exclusivo_promocion;				//73
	INT16 dias_semana;
	BINARY fin;
};

struct _grupo_art {
	BINARY status;				//0
	INT16 cod_grupo;			//1
	long cod_interno;			//3
	char cod_barra[17];		//7
	char cod_interno2[21];		//24
	BINARY fin;			//45
};

struct _chd_tck_comparativo {
	BINARY  status;
	char cod_barra[17];
	UINT16 fec_cm;
	char nom_competencia[25];
	double precio;
	unsigned long cod_articulo;
	unsigned long id_registro;
};

struct busqueda {
   int codigo;
   char *mensaje;

};

struct _datos_cobro_kinacu {
	char cod_barra[17];
	long id_transaccion;
	char nro_linea[16];
	char estado;
	double monto;
	char mensaje1[41];
	char mensaje2[41];
	char mensaje3[41];
	char mensaje4[41];
	INT16 cod_valida;
    unsigned long cod_interno;
	INT16 pos_ticket;
};
struct _detalle_descrip {
    BINARY      status;
    INT16     cod_descriptor;
    char      valor[31];
    char      descripcion[31];
    BINARY fin;
};

struct _rangos
{
    BINARY status;          	   //1
    double cod_desde;    	   //8
    double cod_hasta;    	   //8
    INT16 cod_tarjeta; 	//2
    float coef;               //4
    BINARY fin;          //30
};

struct _medios_tar {
 BINARY status;      //0
 char cod_medio;      //1
 char cod_tarjeta;      //2
 char pedir_fecha_vto;      //3
 char manual_especial;      //4
 char permite_ingreso_manual;      //5
 char captura_on_line;      //6
 char on_line;      //7
 char pide_numero;      //8
 char pide_supervisor;      //9
 char permite_consulta_saldo;      //10
 char cuotas_online;      //11
 //Se agrega campo fin para darle el corte en mysql sino pisa memoria para cuando la tabla
 //termina en char
 BINARY fin;
};

struct d_f_fijo_sic {
	BINARY status;           //0
	char cod_ff;			//1
	char cod_medio;		//2
	INT16 cod_submedio;    //3
	double monto;			//5
	char reserva[17];      //13
};

struct _factu_mem {
	BINARY status;			//1
	UINT16 codigo;			//3
	char descripcion[31];	//34
	INT16 tipo_dato;		//36
	char valor[81];		//117
	BINARY fin;
};

struct _detalle_fondofijo {
	BINARY   status;
    INT16   cod_ff;
    BINARY   cod_medio;
    INT16    cod_submedio;
    double monto;
	BINARY fin;
};

struct _rend_caj
{
    /*** Indice: 1 --> cod_cajero + caja + nro_y ***/
	BINARY status;					//0
	unsigned short cod_cajero;      //1
	unsigned short caja;			//3
	unsigned long nro_y;			//5
	unsigned short fecha_ing;		//9
	unsigned short hora_ing;		//11
	unsigned short fecha_cie;		//13
	unsigned short hora_cie;		//15
	BINARY estado;					//17
	BINARY fin;				//18
};

struct _funcion_sic
{
	BINARY status;				//0
	char nom_funcion[41];		//1
	short tecla_devuelta;		//42
	short lugar_uso;			//44
	short orden;				//46
	char habilitada;			//48
	BINARY fin;			//49 //necesario porque el mapeo de tipos falla y se guarda el fin de cadena
};

struct _fecha_controlador {
   int da_year;
   int da_mon;
   int da_day;
};

struct _hora_controlador {
   int ti_hour;
   int ti_min;
   int ti_sec;
};

struct _moneda {
	BINARY status;
	BINARY cod_moneda;
	char descripcion[15];
	char simbolo[5];
	BINARY cod_sucursal;
	BINARY fin;		
};

struct _plu
{
	BINARY status;			
	BINARY plu_tecla;			
	long cod_especial;      
	char cod_barra[17];
	char cod_especial_alfa[21];
	INT16 cant_dig_ingresar;
	BINARY cod_sucursal; 
	BINARY fin;		
};
struct _respuesta_napse {   
    BINARY status;          //0      
	long id_evento;         //1
	long caja_z;            //5
	long id_transaccion;
	char autorizacion[50];       // 9 
	long     ticket;        //13
	char     autorizacionTelefono[10]; //17
	long      numero_de_comercio; //27
	long      numero_de_terminal;//31
	long    lote; //35
	char	  tarjeta[20]; //39
	char      tipo_lectura[11]; //59
	long      tipo_de_tarjeta;//70
	char      comercio_tarjeta[20];//74
	long 	cpte_original; //94
	long 	nro_transaccion; //98
	char	nombre_titular[40];//102
	char	identifica_tarjeta[20]; //142
	char 	nro_cuenta[20]; //161
	char	modo_autorizacion[10]; //182
	char	apl[20]; //192
	char	apn[20]; //212
	char	env_data[20]; //232
	long  	pin_verified; //252
	long    tipo_cta; //256
	char 	fhcpte_original[10]; //260
	long   estado; //270
	BINARY  fin;	//274
};

#include "dbrouter.h"
#include <jgateway.h>

#endif

#pragma pack (pop)


