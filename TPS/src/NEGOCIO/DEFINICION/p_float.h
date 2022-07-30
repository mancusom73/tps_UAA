#ifndef P_FLOAT_H
#define P_FLOAT_H
double PEDIR_FLOTANTE( int x, int y, int enteros, int decimales );
double PEDIR_FLOTANTE_COD_BARRA( char x, char y, char enteros, char decimales, double _default,
                                 char **cod_barra );
double PIDE_IMPORTE_T_COMPRA( char x, char y, char enteros, char decimales, char cod_t_compra,
                              char pos, char **output_cadena_scanner );
double PEDIR_FLOTANTE_CON_VALOR( int x, int y, int enteros, int decimales, double defecto );
#endif
