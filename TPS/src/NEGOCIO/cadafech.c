#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cadafech.h> 
#include <cr.h>  
#include <fecha.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
int CADENA_A_FECHA( char *cadena )
/******************************************************************************/
{
    char cadena2[20],dia,mes;
    unsigned fecha,anio;
    strcpy( cadena2, cadena );
    if( strlen( cadena ) < 8 ) {
        fecha = 0;
    }
    else {
        cadena2[2] = 0;
        cadena2[5] = 0;
        dia = atoi( cadena2 );
        mes = atoi( cadena2 + 3 );
        anio = atoi( cadena2 + 6 );
        fecha = FECHA( dia, mes, anio );
    }
    return ( fecha );
}

