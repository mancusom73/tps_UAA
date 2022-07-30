// Modulo de Emulación de Funcionalidades del TPV
// Se utiliza por el interprete de promociones genérico.

#ifdef EMULAR_FUNCIONALIDADES_TPV

#include "etpv.h"
#include <stdlib.h>
#include <stdio.h>

void glog( char* str , int origen ) {
	
	fprintf(stderr,"%s\n",str);

}

void MENSAJE( char* str ) {
	
	fprintf(stderr,"MENSAJE: %s\n",str);

}


void BORRAR_ARCHIVO_REC( char *archivo ) {

}



int DD_TIPO( int var ) {
	return _INT;
}









unsigned GET_MEMORY_UNSIGNED( int var ) {
	return 0;
}

long GET_MEMORY_LONG( int var ) {
	return 0;
}

double GET_MEMORY_DOUBLE( int var ) {
	return 0;
}

float GET_MEMORY_FLOAT( int var ) {
	return 0;
}

int GET_MEMORY_INT( int var ) {
	return 0;
}

char GET_MEMORY_CHAR( int var ) {
	return 0;
}

int DD_LARGO( int var ) {
	return 0;
}

int DD_ELEMENTOS( int var ) {
	return 0;
}



#endif //EMULAR_FUNCIONALIDADES_TPV
