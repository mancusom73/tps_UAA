#ifndef PAGO_TC_H
#define PAGO_TC_H
int PIDE_TICKET_COMPRA( int *id_registro, long *autorizacion, long *datos_ad,
                        char *cod_medio_vuelto, char *cod_submedio_vuelto, char *tipo_vuelto,
                        int *datos_adicionales, int cod_medio, int origen_ventas, int *cod_restriccion_art,
                        int solicitar_submedio );
int PIDE_CANTIDAD_CUPONES();
double PIDE_VALOR_CUPONES();
void REPROCESAR_TICKETS_DE_COMPRA( char );
void IMPRIME_DATOS_TICKET_COMPRA( int nro );
int AGREGA_A_POSICION( char pos, char *codigo_barra, double importe );
int AGREGA_BIN_EV_T_COMPRA_A_BTRV( void );
double OBTENER_IMPORTE_T_COMPRA_DESDE_SCANNER( char *cadena_scanner, char cod_t_compra );
void NOMBRE_TICKET_COMPRA( int nro, char *cadena );
int COD_VALIDA_T_COMPRA( char marca );
//int COD_VALIDA_MUTUAL( char marca );
int VALIDA_T_COMPRA( char marca );
#endif
