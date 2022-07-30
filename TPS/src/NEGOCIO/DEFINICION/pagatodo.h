#pragma pack (push,1) 
#ifndef PAGATODO_H
#define PAGATODO_H
#pragma pack(1)

#define REINTENTOS_PAGATODO               0
#define REINTENTOS_PAGATODO_REVERSA       3

/*#define PAGATODO_PAGO                     1
#define PAGATODO_REVERSA                  2
#define PAGATODO_DEVOLUCION               3*/

#define PAGATODO_RECARGA                  1
#define PAGATODO_VENTA_PIN                2
#define PAGATODO_SERVICIOS                3

#define OPERACION_OK                      0
#define OPERACION_NO_NECESITA_REVERSO    -1
#define OPERACION_NECESITA_REVERSO       -2
#define OPERACION_FINALIZADA            -99

#define TRANSACCION_SIN_ENVIAR            0
#define TRANSACCION_INICIAL_ENVIANDO      1
#define TRANSACCION_INICIAL_ENVIADA       2
#define TRANSACCION_REVERSO_ENVIANDO      3
#define TRANSACCION_REVERSO_ENVIADA       4

/************************************************************************************/

#define V_KINACU_RESERVA_PRODUCTO_REQ  1  // Version 1

// Nota: customer y monto vienen sin informacion relevante si se trata
//    de compra de pines
struct _kinacu_reserva_req {
  struct _cab_req cab;
  struct _cab_gral_datos gral;
  char   cod_barra[17];
  char   customer[16]; // linea de celular, max. 15 chars
  double monto;
  char   commit_sale; // 0 = reserva, 1 = venta inmediata
};//64 byte

struct _kinacu_reserva_rta {
  struct _cab_req cab;
  INT16    rta;           // 0 = OK
                          // 1 = error reportado por Kinacu
                          // 2 = error reportado por JGateway
  char   mensaje[73];     // explicacion en caso de error
                          // imprimir en pantalla siempre que no sea reserva
  long   id_transaccion;  // se reserva para confirmar o cancelar y
                          // debe imprimirse en el voucher
  char   lineas_voucher_encabezado;  // cantidad de lineas para imprimir en el voucher
                          // (puede valer 0)
  char   lineas_voucher_cuerpo;
  char   lineas_voucher_pie;
};//92 bytes

// Esta estrucutura se repite tantas veces como indique el campo
// lineas_voucher de _kinacu_reserva_rta
struct _kinacu_linea_voucher {
  char  linea[41];       // hasta 40 caracteres por linea
};

#define V_KINACU_CONFIRMA_PRODUCTO_REQ  1  // Version 1

struct _kinacu_confirma_req {
  struct _cab_req cab;
  struct _cab_gral_datos gral;
  long   id_transaccion;
};//26 bytes

struct _kinacu_confirma_rta {
  struct _cab_req cab;
  INT16    rta;             // 0 = OK
  char   mensaje[73];     // explicacion en caso de error
                          // imprimir en pantalla siempre (puede estar vacio)
  char   lineas_voucher_encabezado;  // cantidad de lineas para imprimir en el voucher
                          // (puede valer 0)
  char   lineas_voucher_cuerpo;
  char   lineas_voucher_pie;
};//88 bytes

// Siguen tantas _kinacu_linea_voucher como indique el campo
// lineas_voucher de _kinacu_confirma_rta


#define V_KINACU_ANULA_PRODUCTO_REQ  1  // Version 1

// el pedido de anulacion es igual al _kinacu_confirma_req pero
// en struct _cab_req el campo servicio vale KINACU_ANULACION

struct _kinacu_anula_rta {
  struct _cab_req cab;
  INT16    rta;          // 0 = OK
  char   mensaje[73];  // explicacion en caso de error, o vacio en otro
                       // caso. Puede mostrarse siempre como en
                       // los casos anteriores para mantener esa politica
};
struct DatosCobroPagaTodo
{
	char     _codigoBarra[81];
	char     _codClientePagatodo[14];
	char     _cuenta[41];
	double   _importe;
	char     __devolucion;
	char     _anulacion;
	char     _modo;
	char     _confirmacion_original[11];
	INT16    _sucursal_original;
	INT16    _caja_original;
	INT16    _fecha_original;
	int      _hora_original;
	INT16    _numero_transaccion;
	char     _transaccion_pagatodo[11];
	char     _cod_producto[13];
	double   _costo_servicio;
	double   _iva_costo_servicio;
	char     _nip[17];
	char     _folio[17];
    INT16    _codigo;
	char     _estado_envio;   //0 sin enviar / 1 enviando / 2 enviado.
	char     _rta_envio;	  //0 ok / -1 Error no necesita reverso / -2 Error necesita reverso
	INT16    _fecha;
	int      _hora;
	int      _cod_valida;
	char     _nombre_cobro[21];
    INT16    _datos_pedir;
};

int COBROS_PAGATODO( char modo, char devolucion );
int  F_PAGATODO_COBRO( double importe );
int  F_PAGATODO_COBRO_VENTA( double importe );
int  ENVIAR_TRANSACCION_PAGATODO( int modo, struct DatosCobroPagaTodo *datosCobros );
int  PREPARAR_ENVIO_PAGATODO( int comando, struct DatosCobroPagaTodo *datosCobros, 
		char *buffer, int *l_buffer );
int  PROCESAR_RESPUESTA_PAGATODO( int comando, char *buffer, 
	struct DatosCobroPagaTodo *datosCobros );
//extern void VALIDAR_COBROS_PAGATODO( struct DatosCobroPagaTodo *datos_cobros, int modo );
void GRABAR_EVENTO_PAGATODO( struct DatosCobroPagaTodo *datos_cobros );
int  PEDIR_DATOS_PAGATODO( struct DatosCobroPagaTodo *datosCobros );
void MOSTRAR_DATOS_CODIGO_BARRA_PAGATODO( struct _datos_cobro *datos_cobro );
int POSICIONAR_COBRO_SELECCIONADO( void );
#endif
#pragma pack (pop) 
