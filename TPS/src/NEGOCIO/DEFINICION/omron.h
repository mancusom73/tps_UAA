#ifndef OMRON_H
#define OMRON_H
void ALOJA_ESPACIO_OMRON();
void OMRON_ABRIR_CAJON();
void OMRON_BORRAR_DISPLAY();
void OMRON_BORRAR_DISPLAY_LINEA( int linea );
int OMRON_CAJON_CERRADO();
void OMRON_DISPLAY( char *buffer );
void OMRON_DISPLAY_LINEA( char *buffer, int linea );
int OMRON_GETKEY();
int OMRON_INIT();
void OMRON_INIT_DISPLAY();
int OMRON_IS_PRINTER();
int OMRON_KBHIT();
int OMRON_PAPER_OUT();
void OMRON_PRINT( char *buffer );
int OMRON_TARJETA_MAGNETICA( unsigned char *buffer );
int _OMRON( char tipo, char *estructura, int longitud );
#endif
