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
#ifndef INVEL_L
#endif
/******************************************************************************/
long PEDIR_GRAN_ENTERO( int x, int y, int largo )
/******************************************************************************/
{
    long entero;
    _GRANDE = 1;
    entero = PEDIR_ENTERO( x, y, largo, NO );
    _GRANDE = 0;
    return ( entero );
}

