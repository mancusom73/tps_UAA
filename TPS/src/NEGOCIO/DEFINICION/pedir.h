#ifndef PEDIR_H
#define PEDIR_H
void INIT_PEDIR( int x, int y, int largo );
void CLOSE_PEDIR();
long BOX_PEDIR_ENTERO( int x, int y, char *titulo, int digitos, int salvar_pantalla, int c_tit );
void SET_TECLA_EXCEPCION( int *teclas, void ( *funcion )() );
void TESTEAR_EXCEPCION( int *k );
#endif
