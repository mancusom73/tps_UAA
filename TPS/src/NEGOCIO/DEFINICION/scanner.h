#ifndef SCANNER_H
#define SCANNER_H
int INIT_SCANNER();
int LEER_SCANNER( unsigned char *buffer, int l_buffer );
int CRC_SCANNER( char *buffer, int largo, int empieza_impar );
int IS_SCANNER();
void LIMPIAR_SCANNER();
void SET_RTS_SCANNER( int estado );
int CALCULA_CRC_EAN( char *buffer, int largo, int empieza_impar );
void CALCULA_CRC_EAN13( char *buffer );
int LEER_CADENA_SCANNER( char *cadena, int largo );
int SCANNER_LIBRE();
int SCANNER_POR_TECLADO( int tecla );
#endif
