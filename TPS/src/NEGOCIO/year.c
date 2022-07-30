#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <year.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
int YEAR( unsigned fecha )
/******************************************************************************/
{
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    return ( anio );
}

