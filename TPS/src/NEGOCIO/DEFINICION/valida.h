#ifndef VALIDA_H
#define VALIDA_H
#include <pagatodo.h>
void MOSTRAR_VARIABLE_RNV( int cod_variable, int subindice, int columna, char *picture );
int VALIDA_COD_COMANDO();
int MENU_SELECCIONA_MEDIO_A_REIMPRIMIR();
void VALIDA_ENVASES_TOMRA();
void VALIDAR_COBROS_POR_ARTICULO();
int OBTENER_VALOR_RNV( int cod_variable, int subindice );
void CALCULAR_CODIGO_VOUCHER_BOLETA_PLATA();
void VALIDA_tomar_cadena_compuesta( int *ind_config, char *val_cad, int h );
double OBTENER_FLOTANTE_DE_CADENA( char *buffer );
void CARGAR_VARIABLE_RNV( int cod_variable, char *buffer, int l_buffer, int indice, char *picture );
void VALIDA_SET_SALIDA_CUPON( int destino );
int VALIDA_GET_SALIDA_CUPON();
void VALIDAR_T_ESPERA( int cod );
int VALIDA_OBTENER_GRUPO_CUOTA( char *mensaje );
long CALCULA_CODIGO_VERIFICACION( int caja, int fecha, long ticket, int digito, int sucursal );
void CARGAR_DATOS_D_IMPRESION( int posic, int cod_sol, int nro_orden,
								int modalidad, int borra_arch_impresion, int reimpresion );
void VALIDAR_MEDIOS(int modo);
void VALIDAR_DEVOLUCIONES( double importe );
void VALIDAR_COBROS( int cod, struct _datos_cobro *datos_cobro, char *mensaje,
	int devolucion, int modo );
void IMPRIMIR_CHEQUE( double importe, int fecha, int cod_valida_cheque );
void IMPRIME_CUPON_PROMOCION( int cod_formato, double valor, int pos, int cantidad, int h, char *pto_str, int vigencia );
void VALIDAR_VOUCHER( int cod_formato );
int VALIDAR_CLIENTE_AFINIDAD();
int VALIDAR_LIMITE_DGI();
void VALIDAR( int cod, int tipo, int h, int ind_posicion, int modo );
void INIT_VALIDA();
void VALIDA_ejecutar_comando( int *ind_config, int h, int ind_posicion, int modo, int reimpresion );
void VALIDA_importe_en_letras( int *ind_config );
INT16 VALIDA_tomar_entero( int *ind_config, int *indice );
float VALIDA_tomar_float( int *ind_config );
void VALIDA_SET_FILA( int _fila );
void VALIDA_CHEQUE_CLIENTE( double importe, double saldo );
void VALIDAR_DONACION();
int VALIDAR_FRANQUICIA();
int VALIDACION_POR_SLIP( int ind_config, int *mens );
void VALIDAR_UNIPAGO();
void VALIDAR_PAGOS( int cod, double importe, long cuenta, long comprobante, char *mensaje );
void REIMPRIMIR_VALIDACIONES_MEDIOS_DE_PAGO();
void VALIDAR_REIMPRESION_FACTURA( double comprobante, long cuenta );
void VALIDAR_UN_MEDIO( int h, int cod, int modo );
void VALIDAR_VUELTO( );
void IMPRIMIR_LINEAS_MENSAJE( long id_transaccion, char nro_cuerpo );
void VALIDAR_COBROS_PAGATODO( struct DatosCobroPagaTodo *datos_cobros, int modo );
void VALIDAR_COBROS_GARANTIA( struct _datos_cobro *datos_cobros, int modo );
void VALIDAR_VOUCHER_MUTUAL( int h, int cod_formato, double importe );
void VALIDAR_VOUCHER_TICKET_COMPRA( int h, int cod_formato, double importe );
void VALIDAR_COBROS_KINACU( struct _datos_cobro_kinacu *datos_cobro_kinacu, int modo, int posicion );
int IMPRIMIR_COMPROBANTE_VAL( int tipo, int reimpresion );
void VALIDAR_COMPROBANTE();
void VALIDA_CIERRE_CAJERO( int cod_formato );
void VALIDAR_CAMBIO_MEDIO( int cod_formato );
#endif
