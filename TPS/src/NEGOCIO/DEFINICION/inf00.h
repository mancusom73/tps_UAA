#ifndef INF00_H
#define INF00_H
void IMPRIMIR_Z_FISCAL();
void INFORME_00( int est, char cierre, int anterior );
void INFORME_00_FISCAL( double tot_ventas, double tot_retiros, double tot_cobros, long cant_tickets,
                        double gran_total, double *dif, int ini_est );
void INFORME_00_CUERPO( char *nombre, double importe, int porcentaje, int cantidad );
void INFORME_00_PIE();
void INFORME_00_SEPARADOR();
void INF00_DISCRIMINAR_IVA( double tot_venta, int ini_est );
void INF00_ITEM( char *titulo, double tot, int cod_imp, int cod_can, int ini_est );
void _INF00_ITEM( char *titulo, double tot, double importe, int cod_can, int ini_est );
int __INF00_ITEM( char *titulo, double tot, double importe, int cantidad );
void IMPRIME_INFORMES_Y_EN_Z( int z );
int _IMPRIME_INFORMES_Y_EN_Z( int anterior );
void ACUMULAR_X();
void IMPRIME_DIA_OPERATIVO( int anterior );
void INFORME_00_CAJERO( int est );
void INFORME_00_CABECERA( char anterior, char cierre );
void EMITIR_INFORME_00( int anterior, int est, char cierre, int ini_est );
void IMPRIMIR_REPORTE_TRANSACCIONES();
#endif
