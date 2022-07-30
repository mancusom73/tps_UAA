#ifndef IMP_RET_H
#define IMP_RET_H
void IMPRIME_CABECERA_RETIROS( unsigned tipo, int retiro );
void IMPRIME_CUERPO_RETIROS( int anulacion, char *nombre, double importe, long numero, int cantidad,
                             double importe_me, int medio );
void IMPRIME_PIE_RETIROS( double *, double, int, double *, int );
#endif
