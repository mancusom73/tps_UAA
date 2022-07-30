#ifndef PINPAD_H
#define PINPAD_H
int PINPAD_AUTORIZAR_COMPRA( char celda_pinpad, char *nro_tarjeta, char tipo_cuenta, char *pin,
                             int l_pin, int confirmar_importe, double importe );
int PINPAD_ACTUALIZAR_WORKING_KEY( char celda_pinpad, char *working_key );
void PINPAD_CARTEL();
int PINPAD_PEDIR_WORKING_KEY();
int INIT_PINPAD();
void MOSTRAR_SALDO_EN_PINPAD( double importe );
int _WORKING_KEY( int ( *funcion )(), char celda_pinpad, char *working_key );
int PINPAD_LEER_WORKING_KEY( char celda_pinpad, char *working_key );
int PINPAD_MENSAJES_ROTATIVOS( int cant, char *mens0, char *mens1, char *mens2, char *mens3,
                               char *mens4, char *mens5, char *mens6 );
int PINPAD_MOSTRAR( char *cadena );
int PINPAD_PEDIR_PIN( char *nro_tarjeta, char *working_key, char *pin, int l_pin );
int PINPAD_PEDIR_TECLA( int timeout, char* tecla );
int PINPAD_SELECCIONAR_MASTER_KEY( int master_key );
int PINPAD_CANCEL_SESSION( );
int PINPAD_SET_TIMEOUT( int timeout );
char PINPAD_CONFIRMAR( char *mensaje, int espera );
#endif
