#ifndef COMPROB_H
#define COMPROB_H
int ABRIR_ARCHIVOS_FACTURAS( int clase, char *buffer );
int AGRUPAR_ARTICULOS( int* cantidad_de_items, struct _art_fact **art_fact2, struct _art_fact_iva **art_fact_iva2 , struct _art_fact_iva ** art_fact_iva_backup2,struct _art_fact **art_desc2);

void APUNTA_CFG( int tipo );
void CALCULAR_RETENCIONES();
double PERCEPCION_TACIS( double importe_neto, double iva );
int REALIZAR_PERCEPCION_TACIS( void );
int CANTIDAD_FORMULARIOS( int renglones );
void COMP_SET_FILA( int _fila );
void COMP_ajustar_largo( char *cadena );
void COMP_ejecutar_comando( int *ind_config );
int COMP_evaluar_condiciones( int codigo );
INT16  COMP_tomar_entero( int *ind_config, int *indice );
void COMP_total_en_letras( int *ind_config );
int DATOS_COMPROBANTE( int tipo );
void EJECT();
void EJECT_FISCAL( int habilita );
int EMITIR_FACTURA();
int EMITIR_FACTURA_REMOTA();
char *FORMATEA_NRI( char *nro_nri );    
int ES_COMPROBANTE( int tipo );
int ES_FACTURA_A( int cond_cli );
int ES_FACTURA_B();
int ES_CONSUMIDOR_FINAL( int cond_cli );
void GRABA_EVENTO_ANULACION( double nro, unsigned evento, double nuevo_nro, long act_ram_id_evento );
void IMPRIME_RETIRAR_FACTURA( double total );
void IMPRIMIR_CADENA( int x, char *cadena );
int IMPRIMIR_COMPROBANTE( int tipo, int reimpresion, int actualizo_ev );
int _IMPRIMIR_COMPROBANTE( int tipo, int reimpresion, int actualizo_ev );
void IMPRIMIR_DETALLE_FACTURA_EN_JOURNAL();
void IMPRIMIR_TICKET_FACTURA( int cant );
void IMPRIMIR_TICKET_NC();
void INIT_COMPROBANTE( int tipo );
int MODO_FACTURACION_ESPECIAL();
void MOSTRAR_NRO_FACTURA();
int OBTENER_DATOS_COMPROBANTE( int clase, int tipo, int cant, short int *nro_suc, long *nro_comp,
                               short int *copias, int actualizar_nro, int pedir_nro );
int OBTENER_DATOS_RETENCIONES();
double PERCEPCION( double importe_neto, char verificar );
double PERCEPCION_212_IVA_1();
double PERCEPCION_212_IVA_2();
float OBTENER_ALICUOTAS( int municipio, int cod_iva, double importe ,unsigned  fecha_fin_a,
       unsigned fechaini, unsigned fechafin );
float OBTENER_ALICUOTAS_DGR( int tasa_iibb, int cod_iva, double * minimo_retenc_dgr );
void POSICIONA_EN_COMPROBANTE( void );
void REIMPRIMIR_COMPROBANTE( int tipo );
double RETENCION( double importe_neto, char verificar );
void VALIDAR_REIMPRESION_FACTURA( double comprobante, long cuenta );
double VARIACION_PERCEPCION_212( double importe_neto );
int VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo );
int _VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo, int *cantidad_pagos , int *cant_items);
char *FORMATEA_CUIT( char *nro_cuit );
char *OBTENER_TIPO_IVA( int cond_iva );
void CALCULAR_RETENCIONES_TOTALES();
int OBTENER_FLAG_COMPROBANTE( int cond_cli, int comprob );
void VALIDAR_FACTURA_REMOTA( double total, char *nombre_cliente, int nro_cliente );
void CALCULAR_VALOR_CADA_ARTICULO_RESTANDO_DESCUENTOS( int renglon );
void GRABA_EVENTO_COMPROBANTE_ANULADO( double nro, long id_evento );
int IMPRIMIR_COMPROBANTE_JOURNAL( int tipo, int reimpresion );
void IMPRIMIR_CABECERA_FACTURA_JOURNAL( int bandera );
void IMPRIMIR_PIE_FACTURA_JOURNAL( void );
int BUSCAR_DATO_ADICIONAL_MEDIO_PAGO( int posicion, char *cadena );
void CALCULAR_MAXIMO_DATOS_ADICIONALES_MEDIO( int cod_sol, int nro_orden, long id_evento, int bin,
                                              int origen, int cant_max, int posicion );
int ES_CREDITO_FISCAL( int ev );

double SUMAR_TOTAL_FAC_CAJA( int recalcular );
int TIPO_COMPROBANTE( int *subtipo );
void  AGREGA_BIN_A_EXTENDIDO( int h, int origen, char anulado );
int  INSERT_A_BTRV_EXT( struct _datos_en_bin *datos_en_bin, char anulado, int posicion );
int OBTENER_DIGITOS( char *valor, int tipo_dato );
int _CANTIDAD_FORMULARIOS( int renglones , int _tipo_loc);
void AGREGAR_EXT_BTRV( int actualizo_ev );
int CANTIDAD_DIGITOS( double cantidad, int enteros );
double FABS( double x );
void ESPERAR_IMPRESION_SLIP_DRIVER( char tipo );
//int AUTORIZAR_RETENCION_NC( void );
int AUTORIZAR_RETENCION_NC(double retencion_calculada );
//int AUTORIZAR_PERCEPCION_NC( void );
int AUTORIZAR_PERCEPCION_NC( double percepcion_calculada);
int REALIZAR_RETENCION_DGR( void );
int REALIZAR_PERCEPCION_DGI( void );
int CONTROL_DIAS_EN_DEVOLUCION(int tipo,int dias_devolucion, int *calcularNC );
#endif
