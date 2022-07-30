#ifndef GETCH_H
#define GETCH_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

void SET_LASTKEY( int tecla );
int LASTKEY();
void SET_ESPERA_GETCH( int ticks );
int GET_ESPERA_GETCH();
int GETCH();
void UNGETCH( int caracter );
void DEMORA_SET();
int DEMORA( unsigned long _clock );
int INICIALIZAR_TECLADO();
void SET_NUM_LOCK( int prender );
void SET_CAPS_LOCK( int prender );
void SET_LOCK( unsigned char mascara, int prender );
int KBHIT();
int _GETCH();
int FAST_GETCH( );
void SET_MOSTRAR_HORA_DISPLAY( int status );
int GET_MOSTRAR_HORA_DISPLAY();
void ENABLE_VERIFICAR_CAMBIO_ARHIVOS();
void DISABLE_VERIFICAR_CAMBIO_ARHIVOS();
void ENABLE_PAD_NUMERICO();
void DISABLE_PAD_NUMERICO();
void VERIFICA_CAMBIOS();
void ENABLE_STANDBY_AUTOMATICO();
void DISABLE_STANDBY_AUTOMATICO();
void INICIALIZAR_DEMORA_SET( int valor );
void VACIAR_KEY();

int INICIO_TOUCH( void );
//    void    CAMBIO_PANTALLA_TOUCH( int valor );
//    void    CAMBIO_PANTALLA_TOUCH_ANTERIOR( void );
int GET_TOUCH( void );
#endif
