#ifndef M_FECHA_H
#define M_FECHA_H
void MOSTRAR_FECHA_2000( char x, char y, unsigned fecha );
void DTOC4( unsigned fecha, char *cadena );
void DTOC_2000( unsigned fecha, char *cadena );
void FORMAT_FECHA( char *picture, unsigned fecha, char *cadena );
void FECHA_DD_MMM_AAAA( unsigned fecha, unsigned char result[200] );
#endif
