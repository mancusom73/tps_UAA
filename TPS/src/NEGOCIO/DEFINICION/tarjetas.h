#ifndef TARJETAS_H
#define TARJETAS_H
long _CALCULAR_NRO_CUPON( char marca );
void CADENA_NRO_TARJETA( char *buffer, char *nro, char marca );
int OBTENER_COD_SERVICIO( char *buffer );
void NRO_TARJETA( char *buffer, double *n1, double *n2, char marca_especial );
void FECHA_VTO_TARJETA( char *buffer, unsigned * fecha );
int PEDIR_DATOS_TARJETA( char *cadena, char marca_especial );
int PEDIR_CUOTAS( int marca, int financia_tarjeta, unsigned short cod_medio );
int PEDIR_FINANCIA_TARJETA();
int PEDIR_MONEDA( int marca, int financia_tarjeta, int consulta_saldo );
int PEDIR_TIPO_CUENTA( int marca );
int PEDIR_CVV();
int _elegir_tar_esp( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int PIDE_MARCA_ESPECIAL();
long PEDIR_NRO_CUPON( int marca, int cuotas, int financia_tarjeta );
long CALCULAR_NRO_CUPON( char _marca );
void DECREMENTAR_NRO_CUPON( char marca );
int VALIDA_TARJETA( int consulta_saldo, int consulta_boletin, int operacion, int hab_promo, 
	char *mensaje, int origen_ventas, int medio );
int PIDE_DATOS_TARJETA( int consulta_saldo, unsigned short cod_medio );
void NUMERO_DE_COMERCIO_TARJETA();
int AUTORIZA_TARJETA_EN_BOLETIN();
int VALIDA_TARJETA_NUEVA();
int VALIDA_FECHA_VTO( unsigned fecha_vto );
int MONEDA_EXTENDIDA();
int TARJETA_CON_CECOR( int marca );
int _elegir_cuotas( int comando, char *cadena, int dato, struct _browse *b, double monto2, unsigned short cod_medio );
int _elegir_cuotas_grupo( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _elegir_grupo( int comando, char *cadena, int dato, struct _browse *b );
int ELEGIR_CUOTAS( int marca, unsigned short cod_medio );
int _ELEGIR_CUOTAS( int marca, int grupo, unsigned short cod_medio );
int ELEGIR_GRUPO_CUOTAS( int marca, unsigned short cod_medio );
void GO_CUO_TAR( int modo, unsigned short cod_medio );
void GO_CUO_TAR_GRU( int modo );
void GO_CUO_GRU( int modo );
int VALIDA_LARGO_NUMERO( int marca, int largo );
long NRO_TERMINAL( int marca );
int LEE_CUO_TAR( int marca, int cuotas );
int CAJA_HABILITADA_PARA_CUOTAS();
int _elegir_vigencia( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int ELEGIR_VIGENCIA( int marca );
void GO_VIGENCIA( int modo );
int BUSCAR_VIGENCIA( int cod_tarjeta, int cod_vigencia, char *nom );
double PIDE_ENTREGA( double importe );
double _PIDE_ENTREGA( char *titulo, int enteros, int decimales, int signo );
int CONFIRMAR_NUMERO_TARJETA( int marca );
int PEDIR_CUPON_ORIGINAL();
int PEDIR_FECHA_ORIGINAL();
int BUSCAR_EN_BOLETIN( int marca );
void GRABA_MODIFICACION_TARJETAS( int posicion, int id );
int LEE_TARJETAS( int posicion, int id );
int PEDIR_HORA_ORIGINAL();
int PEDIR_AUT_ORIGINAL();
int CANTIDAD_TARJETAS_DEBITOS_INGRESADAS();
int PEDIR_TASA_APLICADA();
int PEDIR_IMPORTE_CUOTA();
void ENMASCARA_NUMERO_TARJETA( int marca, char *nro_cadena, char *temp );
void PEDIR_MARCAS_TARJETA( int medio );
int VALIDAR_MODO_INGRESO_TARJETA( int marca, int operacion );
int CONTAR_CUOTAS(int marca, unsigned short cod_medio );
char      HABILITAR_VOUCHER_MEDIO( int submedio );
int CARGAR_CONFIGURACION_TAR_MED( int medio );
void	  SET_NUMERO_TARJETA_PRIVADO(char* valor);
char	  *GET_NUMERO_TARJETA_PRIVADO( void );
void	  SET_FECHA_VTO_PRIVADA( int valor);
int		  GET_FECHA_VTO_PRIVADA( void );
void	  SET_COD_SERVICIO_PRIVADO(char* valor);
char	  *GET_COD_SERVICIO_PRIVADO( void );
void      SET_PIN_PRIVADO(char* valor);
char      * GET_PIN_PRIVADO( void );
void      SET_TRACK_1_PRIVADO(char* valor);
char      * GET_TRACK_1_PRIVADO( void );
void      SET_TRACK_2_PRIVADO(char* valor);
char      * GET_TRACK_2_PRIVADO( void );
int       VALIDAR_NUMERO_TARJETA( char codigo_cobro );
int      CUOTA_VIGENTE( );

#ifdef COMPILA_DATOS_TARJETAS_PRIVADOS
	void datos_privados_tarjeta_limpiar ( void );
#endif

#endif
