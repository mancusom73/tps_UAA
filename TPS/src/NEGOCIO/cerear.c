#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cerear.h> 
#include <cr.h>  
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Cambia los blancos por ceros
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
void CEREAR( char *puntero, int cant )
/*****************************************************************************/
{
    int h;
    for( h = 0;h < cant;h++ ) {
        if( puntero[h] == ' ' ) {
            puntero[h] = '0';
        }
    }
}

