#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <stringt.h> 
#include <string.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void ITOAH( long entero, char *cadena, int digitos )
/******************************************************************************/
{
    char c[15];
    int l_cadena,h;
    _ltoa( entero, c, 16 );
    l_cadena = strlen( c );
    for( h = 0;h < digitos;h++ ) {
        cadena[h] = ( h < digitos - l_cadena ) ? ' ' : c[h - ( digitos - l_cadena )];
    }
}

