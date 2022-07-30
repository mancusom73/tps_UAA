#ifndef TOMRA_H
#define TOMRA_H
int ES_CODIGO_TOMRA( char *cadena );
long TOMAR_CODIGO_TOMRA( char *cadena );
void IMPRIMIR_CUPONES_TOMRA();
int BUSCAR_VOUCHER_TOMRA( long cod_tomra, int en_pago );
int VOUCHER_REPETIDO( long cod_tomra );
void VACIAR_TABLA_AUXILIAR();
void BUSCAR_ENVASES_FACTURADOS();
int RECORRER_ENVASES( int devolver );
void DEVOLVER_ENVASE( long codigo, int cantidad );
int MARCAR_TABLA_ENVASES();
void REVERTIR_ENVASES();
int VALIDA_ENVASES_ENTREGADOS();
double SALDO_ENVASES();
int CARGAR_ENVASES_POR_TIPO( char *cadena );
int CARGAR_VOUCHER_ENVASE();
#endif
