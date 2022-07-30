#ifndef GRAF_H
#define GRAF_H
void SET_MODO_GRAFICO( int modo );
void SET_DAC( int desde, int cant, DAC *buffer );
int SHOW_BMP( char *archivo, DAC *dac );
int FADE( DAC *dac );
void CLS_GRAFICO();
#endif
