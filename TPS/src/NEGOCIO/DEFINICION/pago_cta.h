#ifndef PAGO_CTA_H
#define PAGO_CTA_H
long PIDE_CUENTA( char largo );
int PIDE_COMPROBANTE();
void REPROCESAR_CTA_CTE( char tipo_pago );
void IMPRIME_DATOS_CTA_CTE( long cta );
void IMPRIME_NOMBRE_CLIENTE( char *nom );
#endif
