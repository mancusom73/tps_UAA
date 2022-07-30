#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_GRAN_ENTERO( int x, int y, char *picture, long entero )
/******************************************************************************/
{
    _GRANDE = 1;
    MOSTRAR_ENTERO( x, y, picture, entero );
    _GRANDE = 0;
}

