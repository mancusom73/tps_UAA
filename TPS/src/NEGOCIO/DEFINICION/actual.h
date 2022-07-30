#ifndef ACTUAL_H
#define ACTUAL_H
void ACTUALIZA_ACUMULADORES_IVA( double signo );
void ACTUALIZA_ACUMULADORES_RAM( int h );
void ACTUALIZA_BASE_DTO_EVENTO();
void ACUMULAR_BASE_DTO_EVENTO( double *base_total );
void ACTUALIZA_DESCUENTOS_BOLETIN_ARTICULOS();
void PRORRATEA_DESCUENTO_BOLETIN_ARTICULO( int pos, struct _art *art, int cant );
void ACUMULA_CONSUM_FINAL_FACT( void );
void ACTUALIZA_CONSUM_FINAL_TKT( void );
void ACUMULA_DESCUENTOS( void );
void ACTUALIZA_DEMORA();
int ACTUALIZA_EVENTOS();
void ACTUALIZA_MEMORIA();
void GRABA_EVENTOS_PAGO( int offset_posicion, int devolucion_cobro );
void RESETEA_TOT_TICK();
void ACUMULA_FACTURAS();
void ACTUALIZA_ACU();
void _ACUMULA_DEPARTAMENTO( int depto, double importe, double cantidad );
void ACUMULA_MOZOS( int mozo );
void ACTUALIZA_IVAS_MULTIPLES();
void GRABA_EVENTOS_PAGO_ANULACION();
char CALCULAR_TIPO_EVENTO( void );
void GRABAR_B_PROMO( struct _ev_bpromo promocionados );
#endif
