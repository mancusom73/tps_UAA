#ifndef VENTAS_H
#define VENTAS_H
void ACTUALIZA_VENTAS();
void _ACTUALIZA_VENTAS( int h );
void ACTUALIZA_STOCK( int h, double cant );
int ABRIR_VTA();
void CERRAR_VTA();
void COPIAR_VENTAS( int hacer_bak );
void COPIAR_VENTAS_A_VENTAS_Z();
void BACKUP_VENTAS();
int COPIAR_VENTAS_ANTERIOR();
void CEREAR_VENTAS();
void ACUMULAR_VENTAS( int area, struct _vta *vta, long cod_interno, double cantidad,
                      double imp_sin_iva, double imp_int, double imp_iva1, double imp_iva2,
                      double impues[4], double costo, double recargo_neto, double imp_exento );
void ACUMULAR_VENTAS_SQL( int tabla, int tipo, struct _vta *vta, long cod_interno, double cantidad,
                      double imp_sin_iva, double imp_int, double imp_iva1, double imp_iva2,
                      double impues[4], double costo, double recargo_neto, double imp_exento );
#endif
