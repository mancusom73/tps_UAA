#ifndef FECHA_H
#define FECHA_H
void FECHA__( int fecha, int *dia, int *mes, int *anio );
int DOW( unsigned fecha );
int DIF_DIAS( unsigned fecha1, unsigned fecha2 );
int FECHA( char dia, char mes, unsigned anio );
int INC_DIA( int fecha, int dias );
int FECHA_VALIDA( int fecha );
#endif
