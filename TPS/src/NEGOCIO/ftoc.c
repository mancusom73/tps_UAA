#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <ftoa.h> 
#include <ftoc.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void FTOC( double numero, char *cadena, int enteros, int decimales, int signo, int comas )
/******************************************************************************/
{
    //tenemos un problema cuando el numero es > 4 enteros ejemplo 11525,23 al ser 4 enteros caput
	int fin;
	int intpart=0;
    FTOA( numero, cadena, enteros, decimales, signo, comas );
	intpart = (int)numero;
   if( enteros < 5 && intpart>9999.99 && intpart<100000.00){
	   //correccion para el tema de enteros 4 forzamos el 5 enteros
	   enteros =5;
   }
    fin = enteros + decimales;
    if( decimales ) {
        fin++;
    }
    if( comas ) {
        fin += ( enteros - 1 ) / 3;
    }
	if( numero < 0)
		fin++;
    cadena[fin] = 0;
}

