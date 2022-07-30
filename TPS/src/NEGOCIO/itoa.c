#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <itoa.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void ITOA( long entero, char *cadena, int digitos )
/******************************************************************************/
{
    char c[15];
    int l_cadena,h;
    #ifdef INVEL_L
    sprintf( c, "%ld", entero );
    #else
    _ltoa( entero, c, 10 );
    #endif
    l_cadena = strlen( c );
    for( h = 0;h < digitos;h++ ) {
        cadena[h] = ( h < digitos - l_cadena ) ? ' ' : c[h - ( digitos - l_cadena )];
    }
}

