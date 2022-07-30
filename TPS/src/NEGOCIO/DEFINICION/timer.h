#ifndef TIMER_H
#define TIMER_H
unsigned long TIEMPO_PASADO_DESDE( unsigned long *desde );
int YA_PASARON_SEGUNDOS( unsigned long *desde, int segundos );
int INIT_TIMER();
void DELAY( int milisegundos );

#endif
