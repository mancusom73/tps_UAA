#ifndef PANT_H
#define PANT_H
void SET_ESTADO_CURSOR( int estado );
void DIBUJA_CURSOR( char estado );
void SET_SCREEN( int tipo_pantalla );
void CAJA( int x1, int y1, int x2, int y2 );
void CURSOR_ON();
void CURSOR_OFF();
void PUT_GRAN_CHAR( int x, int y, unsigned char caracter );
void SCROLL( int x1, int y1, int x2, int y2, int dir );
void LINEA_HORIZONTAL( int x1, int y1, int x2 );
void ACTUALIZA_CURSOR();
void SET_POSICION_CURSOR( int x, int y );
void GET_POSICION_CURSOR( int *x, int *y );
void INIT_FONTS();
void CLRSCR( int color );
void SALVA_CURSOR();
void RESTAURA_CURSOR();
#endif
