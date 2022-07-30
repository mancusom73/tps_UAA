#ifndef COOP_H
#define COOP_H
int ELEGIR_CLIENTE_NUMERICO( int cheque, int mostrar );
void MOSTRAR_DATOS_CLIENTE( int y, int vacio );
int _consulta_cliente( int comando, char *cadena, int dato, struct _browse *b, double monto );
int ELEGIR_CLIENTE_ALFABETICO( int cheque, int validar_cliente );
int _ELEGIR_CLIENTE();
int PEDIR_COD_FINANCIACION( float *recargo );
void ACTUALIZA_SOCIOS( double importe );
void COOP_FACTURA_SOCIOS();
int COOP_VERIFICA_CLIENTE_HABILITADO();
int PIDE_OC_GD();
int ELEGIR_CLIENTE_COD_BARRA( int cheque );
#endif
