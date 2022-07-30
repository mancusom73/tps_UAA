#ifndef EXT_TICK_H
#define EXT_TICK_H
double GET_EXT_IMPUESTOS( int sub1, int sub2 );
void _DISCRIMINA_IMPUESTOS_EN_ARTICULO( double porc, int h, int *incrementar, char *procesar );
void INCREMENTA_P_EXT_IMPUESTOS();
void ADD_EXT_IMPUESTOS( int sub1, int sub2, double importe );
void GRABA_EVENTOS_EXT_IMPUESTOS();
void CARGA_TOTAL_IMPUESTOS( double *total, char cod_ext );
void DISCRIMINA_IMPUESTOS_EN_ARTICULO( double porc, char *procesar );
#endif
