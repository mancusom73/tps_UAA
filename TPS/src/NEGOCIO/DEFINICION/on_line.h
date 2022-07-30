#ifndef ON_LINE_H
#define ON_LINE_H
int ON_LINE_TARJETA( int h, long *nro, long anulacion );
int ON_LINE_PAGO_TARJETA( double importe, int anulacion, int tipo_cobro_tarjeta );
int PEDIR_AUTORIZACION_ON_LINE(char origen, int dev_cobro);
int ON_LINE_CTA_CTE( int h, long *nro );
int CONSULTA_ESTADO_TARJETA();
int _CONSULTA_ESTADO_TARJETA();
int _CONSULTA_ESTADO_TARJETA_COBRO();
int _CONSULTA_ESTADO_TARJETA_ADELANTO();
int ON_LINE_ADELANTO_TARJETA( double importe );
int _ON_LINE_MEDIO( int tipo_operacion_on, int tipo_operacion_off, double importe, int nodo );
#endif
