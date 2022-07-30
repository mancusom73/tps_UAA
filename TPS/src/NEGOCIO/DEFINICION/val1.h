#ifndef VAL1_H
#define VAL1_H
void VAL_SET_PUERTO( int puerto );
int VAL_INICIALIZAR_PUERTO();
void VAL_DESINSTALAR_PUERTO();
int VAL_ENVIAR_COMANDO( unsigned char *buf_datos, unsigned char long_datos,
                        unsigned char *buffer_rta );
int VAL_RECIBIR_RESPUESTA( unsigned char *rta );
int __RECIBIR_RESPUESTA( unsigned char *rta, unsigned char cant );
void VAL_ENVIAR_DATOS( unsigned char *datos, unsigned char cant );
int VAL_GET_ESPERA_TICKS();
void VAL_SET_ESPERA_TICKS( int ticks );
#endif
