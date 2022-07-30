#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <pant.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
void MOSTRAR_GRAN_CADENA( int x, int y, char *cadena )
/*****************************************************************************/
{
    int l_cadena,h;
    l_cadena = strlen( cadena );
    for( h = 0;h < l_cadena;h++ ) {
        PUT_GRAN_CHAR( x, y, cadena[h] );
        x += ( _LETRA_GRANDE_PEGADA ) ? 2 : 3;
    }
}

