#pragma pack (push,1) 
/*****************************************************************************/
/* DEFINICIONES JGATEWAY                                                     */
/*****************************************************************************/
#pragma pack(1)

#define REINTENTOS_JGATEWAY 1
#define INT16 short int

//------ Productos
#define DISCOPLUS			  1
#define MARKETEC			  2
#define DISCOPAGO			  3
#define ORDENES_LIBERTAD      4
#define PROMOCIONES           5
#define MNC                   6
#define RAPIPAGO              7
#define CONTROL_VERSIONES     9
#define KINACU                10
#define HIPERCARD             12
#define PAGATODO              14

//------ Servicios por Producto
#define KINACU_RESERVA           1
#define KINACU_CONFIRMACION      2
#define KINACU_ANULACION         3
//PagaTodo
#define PAGATODO_PAGO                     1
#define PAGATODO_REVERSA                  2
#define PAGATODO_DEVOLUCION               3


//-------------------------- ESTRUCTURAS -----------------------------
//Cabecera Invel
struct _cab_req {
	INT16		producto;
	INT16		servicio;
	INT16		version;
	char    	reserva[4];
};

// Cabecera Kinacu

struct _cab_gral_datos {
  INT16  fecha;  //Fecha Invel
  INT16  sucursal;
  long id_evento;
  long caja_z;
};
//-------------------- Estructuras de PAGATODO ------------------------
#define V_PAGATODO_PAGO_REQ     		1	// Version 1
struct _cons_pagatodo_pago_req {
	struct 	 _cab_req cab;
	INT16    sucursal;	          //Numero de tienda
	INT16    caja;                //Numero de terminal
	int      id_evento;
	int      caja_z;
	INT16    fecha;
	int      hora;
	char     codigo_producto[13];
	char     cuenta_tel[41];      //Cuenta o telefono
	char     llavero[14];         //Cliente PagaTodo
	double   importe;
	double   importe_costo_servicio;
	double   importe_IVA_costo_servicio;
};

struct _cons_pagatodo_pago_rta {
	struct 	 _cab_req cab;
    INT16    codigo_error;              //Codigo de respuesta
    char     mensaje_error[73];         //Mensaje de error para mostrar por pantalla
    char     cuenta_tel[41];                //Cuenta o telefono
    char     nip[17];                   //PIN
    char     folio[17];
    char     confirmacion[11];          //Numero de confirmacion
    INT16    numero_transaccion;        //Numero de transaccion usado en la peticion
    char     transaccion_pagatodo[11];	//Numero de transaccion PagaTodo
    double   importe_costo_servicio;
    double   importe_IVA_costo_servicio;
    double   saldo;	
    char     cant_1;                    //Cantidad de lineas del mensaje 1
    char     cant_2;                  	//Cantidad de lineas del mensaje 2
    //char     texto_1[41]              //Lineas del mensaje 1
    //char     texto_2[41]              //Lineas del mensaje 2
};

#define V_PAGATODO_REVERSA_REQ		    1 	// Version 1
struct _cons_pagatodo_reversa_req {
	struct 	 _cab_req cab;
	INT16    sucursal;                  //Numero de tienda
    INT16    caja;                      //Numero de terminal
    int      id_evento;
	int      caja_z;
	INT16    fecha;
	int      hora;
	char     nip[17];
	char     confirmacion[11];          //Numero de confirmacion original
	INT16    sucursal_original;
	INT16    caja_original;
	INT16    fecha_original;
	int      hora_original;
	INT16    numero_transaccion;        //Valor indicado en la respuesta, si se tiene
	double   importe;
	double   importe_costo_servicio;
	double   importe_IVA_costo_servicio;
};

struct _cons_pagatodo_reversa_rta {
	struct 	 _cab_req cab;
    INT16    codigo_error;              //Codigo de respuesta
    char     mensaje_error[73];         //Mensaje de error para mostrar por pantalla
    char     confirmacion[11];          //Numero de confirmacion
    char     transaccion_pagatodo[11];	//Numero de transaccion PagaTodo
    char     cuenta_tel[41];            //Cuenta o telefono
    char     folio[17];
	char     cant_1;                    //Cantidad de lineas del mensaje 1
    char     cant_2;                  	//Cantidad de lineas del mensaje 2
    //char     texto_1[41]              //Lineas del mensaje 1
    //char     texto_2[41]              //Lineas del mensaje 2
};

//Reversa y devolucion comparten la misma estructura. Solo cambia el servicio.
#define V_PAGATODO_DEVOLUCION_REQ	    1 	// Version 1

//-------------------------------PROTOTIPOS--------------------------------
int   ABRIR_CANAL_JGATEWAY( void );
void  CERRAR_CANAL_JGATEWAY( void );
int   ENVIAR_COMANDO_JGATEWAY( char *buffer, int l_buffer, char reintentos, char servicio );
int  ENVIAR_TRANSACCION_KINACU( int modo, long numero, char *nro_linea );
int  PREPARAR_ENVIO_KINACU( int comando, char *buffer, int *l_buffer,char *buffer_kinacu/*struct _kinacu_reserva_req *kinacu_reserva*/, long valor, char *nro_linea );
int  PROCESAR_RESPUESTA_KINACU( int comando, char *buffer , char *buffer_kinacu/*struct _kinacu_reserva_req *kinacu_reserva*/, int valor );
#pragma pack (pop) 
