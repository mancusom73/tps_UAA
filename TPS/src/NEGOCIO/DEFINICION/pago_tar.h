#ifndef PAGO_TAR_H
#define PAGO_TAR_H
int PIDE_AUTORIZACION_TARJETA( double importe );
void CONSULTA_SALDO_TARJETA_CREDITO();
void VALIDACION_TELEFONICA( double importe );
int EXIGE_AUTORIZACION( double importe );
int PIDE_AUTORIZACION_SI_NECESITA( int h, int pedir_aut );
int EXIGE_AUTORIZACION_ON_LINE();
//int ON_LINE( int h );
int ON_LINE( int h, int dev_cobro, int posipago );
//int COMPRA_OFF_LINE( int h, int pedir_aut );
int COMPRA_OFF_LINE( int h, int pedir_aut, int dev_cobro );
int MOSTRAR_RESULTADO_ON_LINE( int autoriza, char *mensaje );
void CAMBIA_TARJETA_CREDITO();
void GRABAR_RESULTADO_ON_LINE( int ok );
void REPROCESAR_TARJETAS( char tipo_pago, int actualizar );
long PIDE_NRO_CUPON( int digitos );
void IMPRIME_DATOS_TARJETA( long cupon );
int EN_CUOTAS();
int SUPERA_LIMITE_LIBERTAD( int h );
int PIDE_CLAVE_EN_PINPAD( int cartel, int confirmar_importe, double importe );
void CONSULTA_SALDO_TARJETA();
void PEDIR_SALDO_ON_LINE();
//void _ANULAR_TARJETAS( int cuantas );
int _ANULAR_TARJETAS( int cuantas, int AnulacionIndividual, int posiciontarjeta );
double ENVIAR_PAQUETE_AFINIDAD( char tipo_operacion, long cliente, double puntos );
int ON_LINE___TARJETA( char tipo_operacion, struct _rta_terminal *rta_terminal, double importe );
void ENCOLAR_VOUCHER_TARJETA( void );
void VALIDAR_VOUCHER_ENCOLADOS( void );

#endif
