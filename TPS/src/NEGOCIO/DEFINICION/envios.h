#ifndef ENVIOS_H
#define ENVIOS_H
void PREPARAR_DATOS_TERMINAL( char tipo_operacion, double importe,
                              struct _datos_terminal *datos_terminal );
int AUTORIZACION_ON_LINE( int tipo_operacion, double importe, char *buf_rta, int l_buf_rta );
int GRABAR_OFF_LINE( int tipo_operacion, double importe );
//int AUTORIZACION_ON_LINE( int tipo_operacion, double importe, char *buf_rta, int l_buf_rta );
int ESPERAR_BANDERA( int bandera, int segundos );
int LEE_AUTOR_DAT();
int GRABA_AUTOR_DAT( int bandera );
void CIERRE_DE_LOTE( int tipo_de_cierre );
void CREAR_OFF_LINE( int cerear );
#endif
