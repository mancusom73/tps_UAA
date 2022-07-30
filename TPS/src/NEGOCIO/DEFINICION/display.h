#ifndef DISPLAY_H
#define DISPLAY_H
void DISPLAY_STRING( int id );
void DISPLAY( char *buffer );
void DISPLAY_LINEA( char *buffer, int linea );
int INIT_DISPLAY();
void BORRAR_DISPLAY();
void BORRAR_DISPLAY_LINEA( int linea );
int DISPLAY_SET_XY( int x, int y );
#endif
