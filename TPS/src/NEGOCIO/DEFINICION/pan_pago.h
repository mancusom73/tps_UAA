#ifndef PAN_PAGO_H
#define PAN_PAGO_H
void PAGOS_CONFECCIONA_PANTALLA( int modo, int dev_cobro );
void MOSTRAR_INTERESES();
void PAGOS_MUESTRA_TOTAL( double resto );
void MOSTRAR_DISCRIMINACION_IVA();
void MOSTRAR_MARCA_AUTORIZACION();
void PAGOS_MOSTRAR_RESTO( double resto );
void MOSTRAR_VUELTO_EN_GRANDE( double resto );
void MOSTRAR_PAGO( int h );
void MOSTRAR_RETENCIONES();
void MOSTRAR_ENVIAR_OFFLINE();
void MOSTRAR_PEDIR_DIFERIMIENTO();
double CALCULAR_VUELTO(char cod_mutual);
double CALCULAR_RESTO(int moneda_extranjera , int cod_submedio, double resto);
void MOSTRAR_IMPORTE_MUTUAL( double resto, int cod_submedio);
int MOSTRAR_IMPORTE_PROMOCIONADO( );
double IMPORTE_MEDIO_PROMOCIONADO( int medio, int submedio, double *importe_ingresado );
int PROCESAR_PROMOCION( int medio, int submedio );
#endif
