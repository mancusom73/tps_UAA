#ifndef PAGOS_ESP_H
#define PAGOS_ESP_H
void PAGOS_ESPECIALES();
void PANTALLA_PAGOS_ESPECIALES();
int ELEGIR_PAGOS_ESP_BROWSE();
//int _consulta_pagos_esp( int comando, char *cadena, int dato, struct _browse *b, double monto );
int _consulta_pagos_esp( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
//int _consulta_pagos_esp2( int comando, char *cadena, int dato, struct _browse *b, double monto );
int _consulta_pagos_esp2( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int MOSTRAR_SUBNIVEL_ESP();
int PIDE_PAGOS_ESPECIALES();
int ELEGIR_SUBNIVEL_ESP();
int CONFIRMAR_DATOS_PAGO_ESP( long codnivel1, long codigo, long cod_cliente );
void TICKET_PAGOS_ESPECIALES();
void GRABAR_EVENTO_PAGO_ESP();
#endif
