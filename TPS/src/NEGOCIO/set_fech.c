#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <ctime.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
#if !defined(INVEL_W) && !defined(INVEL_L)
/******************************************************************************/
SET_FECHA_DOS( unsigned fecha )
/******************************************************************************/
{
    struct date f;
    unsigned long t;
    t = GET_TIME();
    f.da_year = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    f.da_mon = ( ( fecha & 0x01E0 ) >> 5 );
    f.da_day = ( fecha & 0x001F );
    setdate( &f );
    SET_TIME( t );
}
#endif

