#ifndef T_FILE_H
#define T_FILE_H
int _ENVIAR_PAQUETE_T_FILE( char *paquete, int l_paquete, char *rta, int l_rta,
                            int time_out_disponible );
int ESPERAR_BANDERA_T_FILE( int bandera, int segundos, int bandera2 );
int LEE_BANDERA_T_FILE();
int GRABA_BANDERA_T_FILE( int bandera );
#endif
