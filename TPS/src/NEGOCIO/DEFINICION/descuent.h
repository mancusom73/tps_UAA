#ifndef DESCUENT_H
#define DESCUENT_H
int AGREGAR_DESCUENTO_IMPORTE( int id );
void AGREGAR_DESCUENTO_PORCENTUAL();
void TECLA_DESCUENTO_PORCENTUAL( unsigned desc );
void ACUMULA_IMPUESTOS_ITEM( int h, struct _acu_iva *acu, double cantidad, int flags_impuestos,
                             int cod_iva );
void MUESTRA_DESCUENTO_PORCENTUAL_FIJO();
void IMPRIMIR_BASE_DESCUENTO( unsigned desc );
double CALCULAR_BASE_DESCUENTOS( unsigned desc, double porc, int incondicional, int posicion,
                                 int acumulativo );
int _CALCULAR_BASE_DESCUENTOS( int desc, int h, int incondicional, double porc, double *total,
                               double *tot_aux, double *bases_descuento, int acumulativo );
void APLICAR_DESCUENTO( char desc );
void APLICAR_DESCUENTOS_PENDIENTES( void );
void REVERTIR_DESCUENTOS_FINAL_TICKET( void );
struct _acu_iva *CALCULAR_IMPORTE_IMPUESTOS( struct _acu_iva *acum, double porc );
struct _acu_iva *ACUMULAR_IMPUESTOS_TICKET( int disc_departamentos, unsigned desc,
                                            int flags_impuestos, int cod_iva,
                                            int disc_articulo_habilitado );
void AGREGAR_DESCUENTO_ULTIMO_ARTICULO( int descuento_importe );
int ELEGIR_MOTIVO( int *cod_solicitud, int concepto );
void MOSTRAR_DESCUENTOS_EN_PANTALLA( void );
void ELIMINA_BASE_DTO( int desc );
void SET_FILTRO_ORIGEN_ARTICULO( int origen );
void AGREGAR_DESCUENTO_PORC_CLIENTE( float porc_desc );
int CARGA_BASE_DESCUENTO( int desc, double *base_aux );
int ELEGIR_MOTIVO_RETIRO( int *cod_solicitud );
int VERIFICAR_MOTIVO( int tipo_verificacion, int *cod_solicitud, short *supervisor,
                      int cod_motivo );
#endif
