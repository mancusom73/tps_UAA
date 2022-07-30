#ifndef PAGO_MUT_H
#define PAGO_MUT_H
//int PIDE_MUTUAL( int *id_registro, long *autorizacion, long *datos_ad, char *cod_medio_vuelto,
//                 char *cod_submedio_vuelto, char *tipo_vuelto, int *datos_adicionales, int cod_medio  );
int PIDE_MUTUAL( int *id_registro, long *autorizacion, long *datos_ad, char *cod_medio_vuelto,
                 char *cod_submedio_vuelto, char *tipo_vuelto, int *datos_adicionales, 
				 int cod_medio, double importe, int origen_ventas, int *cod_restriccion_art,
                 int solicitar_submedio );
int PIDE_CUOTAS_MUTUAL( int codigo );
//void GO_CUO_MUT( int modo );
void REPROCESAR_MUTUALES( char );
void IMPRIME_DATOS_MUTUAL( char nro );
int INGRESAR_MUTUAL();
long PIDE_NRO_VALE_MUTUAL();
void NOMBRE_MUTUAL( char cod_mutual, char *cadena );
int COD_VALIDA_MUTUAL( char marca );
char COTIZACION_MUTUAL( char cod_mutual, double *cotizacion, char *simbolo_me );
int COD_VALIDA_VUELTO( char medio, char submedio );
int VALIDA_MUTUAL( char marca );
#endif
