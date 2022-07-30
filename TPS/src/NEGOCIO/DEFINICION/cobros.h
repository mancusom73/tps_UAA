#ifndef COBROS_H
#define COBROS_H

/********Constantes tipo Cobro************/
#define	COBRO_NORMAL	1
#define	COBRO_GARANTIA	2
#define	COBRO_PAGATODO	3
/*****************************************/

/******** Datos Solicitados en Cobros ************/
#define _COBRO_DATOS_CODIGO_BARRA         1
#define _COBRO_DATOS_NRO_COMPROBANTE      2
#define _COBRO_DATOS_NRO_CUENTA           4
#define _COBRO_DATOS_IMPORTE              8
/*****************************************/

void GRABAR_EVENTO_COBROS_VARIOS( int codigo, int id, long comprobante, long cuenta, double importe,
                                  char *cta_cmr );
void _GRABAR_EVENTO_COBROS_TOTAL( int cantidad, double importe, int *_importe, int *_comprobante,
                                  int *_cuenta, int *_codigo );
int PIDE_COBRO( struct _datos_cobro *datos_cobro, int pos, int devolucion, int modo_usuario );
int PEDIR_DATOS_COBRO( struct _datos_cobro *datos_cobro, int pos );
int PEDIR_CTA_CMR( char *cta, void f_online( char *cta ), int f_valid( int cod, char *cta ) );
int MENU_SELECCIONA_COBRO( int *cobro, double *importe );
void UNIPAGO_MENSAJE_TICKET();
int ANALIZAR_CODIGO_BARRA_PARA_SERVICIOS_ESP( int codCobro, char *scanner_cod_barra,
                                              struct _DatosCodBarra *datos );
int GRABAR_COD_BARRA_EN_BINARIO( char *cod_barra, int posicion );
char *LEER_COD_BARRA_EN_BINARIO( int posicion );
void INICIALIZAR_BINARIO_COD_BARRA( void );
void PANTALLA_COBROS();
void GRABAR_EVENTO_COBRO( struct _datos_cobro *datos_cobro, char *cta_cmr, int anulacion );
void TICKET_COBRANZA( char *concepto, struct _datos_cobro *datos_cobro, char *cta_cmr,
                      int anulacion );
void PANTALLA_COBROS_VARIOS( int i, double importe );
void TICKET_COBRANZA_VARIOS( char *concepto, long comprobante, long cuenta, double importe,
                             char *cta_cmr, char cabecera );
void COBROS( int devolucion, int modo_usuario );
//int ELEGIR_COBRO();
int ELEGIR_COBRO_BROWSE( int modo_usuario, int solo_contar );
int CREAR_COBROS_ASC();
int COPIAR_COBROS_ASC();
int VALIDAR_NRO_COMPROBANTE( double _comprobante );
int VALIDAR_CUENTA( char *_cuenta, char *cta_cmr );
int COBRO_CMR();
int PEDIR_DATOS_COBRO_CMR( int _cta, int _importe );
void CONSULTA_CUENTA_ON_LINE( char *cta );
void CONSULTA_CUENTA_ON_LINE_COBROS( char *cta );
void REVALIDAR_COBROS();
void COBROS_VARIOS();
int ELEGIR_REPACTACION();
void REVALIDAR_COBROS_VARIOS();
void ANULACION_COBROS();
int ANULACION_COBROS_CONFIRMACION( struct _datos_cobro *datos_cobro );
int ANALIZAR_CODIGO_BARRA_COBROS( char *scanner_cod_barra, struct _datos_cobro *datos_cobro,
                                  char *mascara );
void MOSTRAR_DATOS_CODIGO_BARRA( struct _datos_cobro *datos_cobro );
int EVALUAR_MASCARA_COD_BARRA( int cod_cobro, char *scanner_cod_barra,
                               struct _datos_cobro *datos_cobro );
int COBROS_TARJETA_PERMITIDA( INT16 cod_cobro, INT16 cod_tarjeta );
int COBROS_F_ONLINE( double importe );
void GO_COBRO( int modo );
int VALIDAR_MONEDERO_ELECTRONICO( double importe );
int       ENVIAR_IMPORTE_MONEDERO( double importe, char archivo );
int COBROS_GARANTIA( char devolucion );
double    CALCULAR_MONTO_GARANTIA( double precio, INT16 tipo );
int       PEDIR_DATOS_COBRO_GARANTIA( struct _datos_cobro *datos_cobro );
void      GRABAR_EVENTO_COBRO_GARANTIA( struct _datos_cobro *datos_cobro, int anulacion );
void EVALUAR_COBROS_MONEDA_EXTRANJERA( double importe, struct _datos_cobro *datos_cobro );
long PEDIR_ENTERO_COD_BARRA( char x, char y, char largo, long _default, 
	char **cod_barra, int busqueda, int identificacion_barra, char *titulo );
int		ASIGNA_CODIGO_BARRA( char *cod_barra );
int		BUSCA_CODIGO_BARRA( char *cod_barra );
int MEDIOS_HABILITADOS_COBROS( int n, int dev_cobro, struct    _datos_cobro *datosCobros );
//int CONTAR_CUOTAS(int marca, unsigned short cod_medio );
int REIMPRIMIR_TICKET_LOCAL(char *archivo);
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
int TIPO_COBRO_VENTA( INT16 cod_cobro );
int PEDIR_DATOS_COBROS_KINACU( struct _datos_cobro *datos_cobro, int activar, int origen );
void MOSTRAR_DATOS_SOLICITADOS_EN_COBROS( int pedir_datos, int tipo_cobro );
void LIMPIAR_PANEL_COBROS( void );
void BORRAR_ARCHIVOS_COBROS_VENTA( int verificar );

#endif
