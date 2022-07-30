#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <pg_fecha.h> 
#include <p_fecha.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
/******************************************************************************/
int PEDIR_GRAN_FECHA( int x, int y, char *fecha_cad )
/******************************************************************************/
{
    unsigned fecha;
    _GRANDE = 1;
    fecha = PEDIR_FECHA( x, y, fecha_cad );
    _GRANDE = 0;
    return ( fecha );
}
/******************************************************************************/
int PEDIR_GRAN_FECHA_MMAA( int x, int y )
/******************************************************************************/
{
    unsigned fecha;
    _GRANDE = 1;
    fecha = PEDIR_FECHA_MMAA( x, y );
    _GRANDE = 0;
    return ( fecha );
}

