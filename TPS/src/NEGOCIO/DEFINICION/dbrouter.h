#pragma pack (push,1) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#ifndef _DBROUTER_H_
#define _DBROUTER_H_

#define CANT_COL_X_DBROUTER               253   // =(**)

// Tamano Maximo Buffer TCP-IP
#define MAXIMO_BUFFER_TRANSACCION         4096
#define MAXIMO_BUFFER_TRANSACCION_RECIBIR 5120
// Cantidad maxima de peticiones por transaccion
#define BYTES_AGREGADOS_POR_DRIVER        15

//#define MAXIMO_PETICIONES_VTRANSACCION  200
#define MAXIMO_PETICIONES_VTRANSACCION    2000

// bEsperaRespuesta flags
#define ITEM_TRANSACCION                  0
#define FIN_TRANSACCION_SIN_RESPUESTA     1
#define FIN_TRANSACCION_CON_RESPUESTA     3
#define COMIENZO_TRANSACCION              4

#define INSERTAR_REGISTRO           0x01
#define BORRAR_REGISTRO             0x02
#define ACTUALIZAR_REGISTRO         0x03
#define OBTENER_REGISTROS           0x04


struct imp_can
{
    double imp;
    INT16 can;
};

struct _RNV_numero
{
    INT16 codcaja;
    unsigned long id;
    INT16 nro_caja;
    INT16 nro_dia_operativo;
    INT16 nro_cajero;
    INT16 nro_hubo_operacion;
};

struct script_rta_en_dbrouter
{
    INT16 nro_caja;
    unsigned long id;
    INT16 cod_comando;
    char param[56];
    INT16 cod_rta;
    char reenviado;
};

struct _ev_cierre 
{
    BINARY status;                        //0
    INT16 caja;                         //1
    long nro_z;                         //3
    long caja_z;                        //7
    long id_evento;                     //11
    double gran_total;                  //15
    long nro;                           //23
    double imp_ventas[10];              //27
    INT16 can_ventas[10];               //107
    struct imp_can adelantos;           //127
    struct imp_can intereses;           //137
    struct imp_can fondo_fijo;          //147
    struct imp_can anulaciones;         //157
    struct imp_can dev_envases;         //167
    double ventas_gravadas;             //177
    double ventas_exentas;              //185
    double imp_int;                     //193
    double tot_dinero;                  //201
    double iva1;                        //209
    double iva2;                        //217
    struct imp_can retiro_fondo;        //225
    double imp_cobros[10];              //235
    INT16 can_cobros[10];               //315
    struct imp_can pagos_por_caja;      //335
    struct imp_can facturas_a;          //345
    struct imp_can facturas_b;          //355
    struct imp_can vueltos;             //365
    UINT16 bonos_entregados;            //375
    double diferencia;                  //377
    struct imp_can nc_a;                //385
    struct imp_can nc_b;                //395
    double imp_fondo_fijo_multiple[10]; //405
    INT16 can_fondo_fijo_multiple[10];  //485
    double retencion_dgr;               //505
    double percepdion_dgi;              //513
    double importes_iva[10];            //521
    double sobretasas_iva[10];          //601
    double imp_correccion;              //681
    INT16 can_correcion;                //689
    long primer_ticket;                 //691
    long cantidad_tickets;              //695
    double imp_retiros[10];             //699
    INT16 can_retiros[10];              //779
    INT16 cobros_cantidad_servicios;    //799
    double ret_rg_212_1;                //801
    double ret_rg_212_2;                //809
    double redondeo;                    //817
    struct imp_can anulados;            //825
    INT16 cant_articulos;               //835
    INT16 cant_art_escaneados;          //837
    UINT16 tmpo_reg_articulo;
    UINT16 tmpo_pago;
    UINT16 tmpo_standby;
    UINT16 tmpo_ocio;
    struct imp_can devoluciones;        //847
    double imp_ventas_me[10];           //857
    double imp_fondo_fijo_multiple_me[10];  //937
    double imp_retiros_me[10];          //1017
    INT16 cod_sucursal;                 //1097
    INT16 fecha_ticket;                 //1099
    struct imp_can donaciones;          //1101
    double imp_cambios[10];             //1111
    INT16 can_cambios[10];              //1191
    INT16 cant_ticket_dev_envase;       //1211
    double boletas_derecho_a_nc;        //1213
    INT16 cant_bol_derecho_nc;          //1221
    double devoluciones_medio[11];      //1223
    double factura_a_importe_exento;    //1311
    double factura_a_importe_gravado;   //1319
    double nc_a_importe_exento;         //1327
    double nc_a_importe_gravado;        //1335
    double cons_final_importe_exent_fact;   //1343
    double cons_final_importe_grav_fact;    //1351
    INT16 cons_final_cantidad_fact;         //1359
    double dev_cons_f_importe_exent_fact;   //1361
    double dev_cons_f_importe_grav_fact;    //1369
    INT16 dev_cons_f_cantidad_fact;         //1377
    double ventas_diplom_importe;           //1379
    INT16 ventas_diplom_cantidad;           //1387
    double devoluciones_diplom_importe;     //1389
    INT16 devoluciones_diplom_cantidad;     //1397
    long nro_transaccion_inicial;           //1399
    long nro_transaccion_final;             //1403
    double devoluciones_export_importe;     //1407
    INT16 devoluciones_export_cantidad;     //1415
    double cons_final_importe_exent_tkt;    //1417
    double cons_final_importe_grav_tkt;     //1425
    INT16 cons_final_cantidad_tkt;          //1433
    double dev_cons_f_importe_exent_tkt;    //1435
    double dev_cons_f_importe_grav_tkt;     //1443
    INT16 dev_cons_f_cantidad_tkt;          //1451
    double iva_factura_a;                   //1453
    double iva_cons_final;                  //1461
    double ventas_export_importe;           //1469
    INT16 ventas_export_cantidad;           //1477
    double ofertas_importe;                 //1479
    double iva_nc_a;                        //1487
    double iva_devol_consumidor_final;      //1495
    double imp_pagosesp[10];                //1503
    INT16 can_pagosesp[10];                 //1583
    UINT16 tmpo_no_venta;
    double impuestos[4]; 
	double descuentos_medio[11]; 
	double ventas_importe_sin_redondeo[10]; 
	double cobros_importe_me[10]; 
	double retiro_fondo_importe_pago[10]; 
	long cant_cierres_y; //1969
	double cobros_importe_servicios;
	BINARY fin; // 2200 bytes
    //WARNING ! !  cuando se vuelva a poner reserva descomentar lo correspondiente
    //peticion.Header.wBufferLength = sizeof(struct _event_cierre) - sizeof(cierre->reserva);
};

struct _total_x
{
	long caja_z;          
    UINT16 cod_caja;           
    UINT16 dia_operativo;      
    UINT16 fecha_ticket;       
    long nro_ticket;      
    double imp_ventas[10];  
    INT16 can_ventas[10];     
    struct imp_can intereses; 
    struct imp_can adelantos;  
    struct imp_can fondo_fijo;
    struct imp_can anulaciones;
    struct imp_can dev_envases;
    double ventas_gravadas;
    double ventas_exentas;  
    double imp_int;        
    double tot_dinero;      
    double iva1;           
    double iva2;           
    struct imp_can retiro_fondo;
    double imp_cobros[10]; 
    INT16 can_cobros[10];     
    struct imp_can pagos_por_caja;
    struct imp_can facturas_a;
    struct imp_can facturas_b; 
    struct imp_can vueltos;    
    UINT16 bonos_entregados;   
    double diferencia;     
    struct imp_can nc_a;     
    struct imp_can nc_b;      
    double imp_fondo_fijo_multiple[10]; 
    INT16 can_fondo_fijo_multiple[10];
    double retencion_dgr;  
    double percepdion_dgi;  
    double importes_iva[10];
    double sobretasas_iva[10];
    struct imp_can correccion; 
    long primer_ticket;  
    long cantidad_tickets;
    double imp_retiros[10]; 
    INT16 can_retiros[10];     
    double cobros_importe_servicios;
    INT16 cobros_cantidad_servicios;
    double ret_rg_212_1;
    double ret_rg_212_2;
    double redondeo;
    struct imp_can cancelados;
    INT16 cant_articulos;     
    INT16 cant_art_escaneados; 
    INT16 tmpo_reg_articulo; 
    INT16 tmpo_pago;        
    INT16 tmpo_standby;       
    INT16 tmpo_ocio;         
    UINT16 tmpo_no_venta;
    struct imp_can devoluciones; 
    double imp_ventas_me[10]; 
    double imp_fondo_fijo_multiple_me[10];
    double imp_retiros_me[10];
    UINT16 cajero;         
    struct imp_can donaciones; 
    double imp_cambios[10];
    INT16 can_cambios[10]; 
    INT16 cant_ticket_dev_envase;
    double boletas_derecho_a_nc;
    INT16 cant_bol_derecho_nc;
    double devoluciones_medio[11];
    double factura_a_importe_exento;
    double factura_a_importe_gravado;
    double nc_a_importe_exento;  
    double nc_a_importe_gravado;  
    double cons_final_importe_exent_fact;
    double cons_final_importe_grav_fact;
    INT16 cons_final_cantidad_fact;
    double dev_cons_f_importe_exent_fact;
    double dev_cons_f_importe_grav_fact;
    INT16 dev_cons_f_cantidad_fact;
    double ventas_diplom_importe; 
    INT16 ventas_diplom_cantidad; 
    double devoluciones_diplom_importe;
    INT16 devoluciones_diplom_cantidad;
    long nro_transaccion_inicial; 
    long nro_transaccion_final;   
    double devoluciones_export_importe;
    INT16 devoluciones_export_cantidad;
    double cons_final_importe_exent_tkt;
    double cons_final_importe_grav_tkt;
    INT16 cons_final_cantidad_tkt;
    double dev_cons_f_importe_exent_tkt;
    double dev_cons_f_importe_grav_tkt;
    INT16 dev_cons_f_cantidad_tkt;
    double iva_factura_a;      
    double iva_cons_final;     
    double ventas_export_importe;
    INT16 ventas_export_cantidad;
    double ofertas_importe;    
    double iva_nc_a;
    double iva_devol_consumidor_final;
	BINARY fin;
};

struct _Header
{
    UINT16 wSelectLength;
    UINT16 wBufferLength;
    UINT16 wNroCaja;
    unsigned long lNroTransaccion;
    UINT16 wFecha;
    UINT16 wHora;
    UINT16 wCodPet;
    char bEsperaRespuesta;
    char bNull;
    char szTabla[30];
};

struct _Buffer
{
    char *szSelectCondition;
    char *sData;
};

struct _Peticion
{
    struct _Header Header;
    struct _Buffer Buffer;
};

struct _control_eventos
{
    BINARY status;
    long id_evento;
    long caja_z;
    UINT16 fecha;
    INT16 sucursal;
	INT16 cant_registros_e_ticket;
	INT16 cant_registros_pagos;
    char reserva[33];// 50
};

/* ***********************
 * BUFFERTR
 * * ********************** */
int INSERTAR_PETICION_TCPIP( struct _Peticion *peticion );
int MANDAR_BUFFER_TCPIP( void );
int ENTRA_EN_BUFFER_TCPIP( struct _Peticion *peticion );
int INIT_BUFFER_TRANSACCION( void );
int GET_DBR_ON_LINE( void );
void CERRAR_CANAL_DBR( void );

/* ***********************
 * VTRANSAC
 * * ********************** */
extern long ultima_transaccion;
int EMPEZAR_VTRANSACCION( void );
int ELIMINAR_VTRANSACCION( void );
int ENVIAR_VTRANSACCION( int espera_respuesta_tcp, int espera_por_ack );
int INCLUIR_PETICION( struct _Peticion *peticion );

/* ***********************
 * DBROUTER
 * * ********************** */
int DBROUTER_REQ_CON_NULL( int pet_code, int cant_col, char *patron_nulls, char *nombre_tabla,
                           char *buffer, int tamano_buffer, char *where, int espera_respuesta );
int DBROUTER_REQ( int pet_code, char *nombre_tabla, char *buffer, int tamano_buffer, char *where,
                  int espera_respuesta );
int DBROUTER_INSERT( char *nombre_tabla, char *buffer, int tamano_buffer, int espera_respuesta );
int DBROUTER_DELETE( char *nombre_tabla, char *where, int espera_respuesta );

int TEST_DBROUTER_ON_LINE( void );

/* ***********************
 * LOGS
 * * ********************** */
void GRABAR_LOG_ID_DBROUTER( int id, int origen, int nivel);
void _GRABAR_LOG_ID_DBROUTER( int id );
int GRABAR_LOG_DBROUTER( char *operacion, int origen, int nivel);

/* ***********************
 * BINARIOS
 * * ********************** */
int DBROUTER_TICKET_EN_BIN( void );
int DBROUTER_OBTENER_TABLA_BIN( char *nombre_archivo, char *nombre_tabla, char *codicion_select );
/* ***********************
 * FTP
 * * ********************** */
#define BINARIO  1
#define ASCII    0
#define GET      1
#define PUT      2

int FTP_ARCHIVO_ASCII( char *origen, char *destino, int get_put );
int FTP_ARCHIVO_BIN( char *origen, char *destino, int get_put );

/* ***********************
 * CIERRE
 * * *********************** */
int GRABAR_CIERRE_CAJERO( void );

int POSICIONA_EN_TICKET( long nro_z, long id_evento );
int POSICIONA_EN_PAGO( long nro_z, long id_evento );
int POSICIONA_EN_COD_BARRA( long nro_z, long id_evento );
int POSICIONA_EN_E_TICKET( long nro_z, long id_evento );
int POSICIONA_EN_CIERRE( long nro_z, long id_evento );


#define NO_EVENTO_REPROCESAR          0
#define EVENTO_REPROCESAR_NO_ENVIADO  1
#define EVENTO_REPROCESAR_ENVIADO     2
#define EVENTO_REPROCESADO            3

#define CANTIDAD_COLUMNAS_EVENTOS     60
#define CANTIDAD_COLUMNAS_EVENT_PAGO  45
#define TAM_EVENTOS_CON_NULL          (60 + sizeof(event_ticket) /*- sizeof(event_ticket.reserva)*/)
#define TAM_EVENT_PAGO_CON_NULL       (45 + sizeof(event_pago) - sizeof(event_pago.reserva))


#endif

#pragma pack (pop) 
