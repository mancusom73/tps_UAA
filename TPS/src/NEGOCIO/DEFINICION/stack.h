#ifndef STACK_H
#define STACK_H
void CLOSE_RECUPERO( int ptr, int handle );
void OPEN_RECUPERO( int *ptr, int *handle );
int STACK_INS( int bytes );
void STACK_INIT();
void STACK_DEL( int bytes );
int STACK_GET_POINTER();
void STACK_SET_POINTER( int puntero );
int STACK_GET_HANDLE();
void STACK_SET_HANDLE( int handle );
void STACK_CLEAR( int handle_desde );
void STACK_INIT_DATOS();
void CEREO_STACK();
#endif
