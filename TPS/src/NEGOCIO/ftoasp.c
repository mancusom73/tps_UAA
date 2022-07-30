#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <ftoa.h> 
#include <ftoasp.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void FTOA_SIN_PUNTO( double numero, char *cadena, int enteros, int decimales, int signo, int comas )
/******************************************************************************/
{
    char aux[30];
    FTOA( numero, aux, enteros, decimales, signo, comas );
    memcpy( cadena, aux, enteros );
    memcpy( &cadena[enteros], &aux[enteros + 1], decimales );
}

