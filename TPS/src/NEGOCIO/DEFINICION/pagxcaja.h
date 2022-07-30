#ifndef PAGXCAJA_H
#define PAGXCAJA_H
void PAGOS_POR_CAJA();
void PANTALLA_PAGOS_POR_CAJA();
int MOSTRAR_SUBNIVEL();
int PIDE_PAGOS_POR_CAJA();
int ELEGIR_SUBNIVEL();
int PEDIR_DATOS_PAGO();
int CREAR_PAGOS_ASC();
int COPIAR_PAGOS_ASC();
void TICKET_PAGOS_POR_CAJA();
void GRABAR_EVENTO_PAGO();
int ADELANTO_CMR();
int PEDIR_DATOS_ADELANTO_CMR( int _cta, int _importe, int _cuotas, int _cod_vigencia );
void CONSULTA_CUENTA_ON_LINE_ADELANTO( char *cta );
int VALIDAR_CUENTA_ADELANTO( int no_usado, char *cta );
int TICKET_ADELANTO_CMR( int _cta_cmr, int _cuotas, int _cod_vigencia );
int GRABAR_EVENTO_ADELANTO( int _cta_cmr, int _importe, int _cuotas, int _cod_vigencia );
int _consulta_pagos_caja( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int _consulta_pagos_caja2( int comando, char *cadena, int dato, struct _browse *b, double monto, unsigned short cod_medio );
int ELEGIR_PAGOS_CAJA_BROWSE();
#endif
