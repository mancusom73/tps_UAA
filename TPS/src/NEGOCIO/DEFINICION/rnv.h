#ifndef RNV_H
#define RNV_H
void DRIVER_MEMORY( int comando, long offset, char *valor, int largo );
void DRIVER_MEMORY_VIRTUAL( int comando, long offset, char *valor, int largo );
int SET_RNV( int estado );
void OPEN_RNV_VIRTUAL();
void OPEN_ARCHIVO_RNV_VIRTUAL( char *nom_arch_rnv );
void CLOSE_RNV_VIRTUAL();
#endif
