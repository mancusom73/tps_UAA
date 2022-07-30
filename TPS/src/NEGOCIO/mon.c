#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <mon.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
int MON( unsigned fecha )
/******************************************************************************/
{
    unsigned mes;
    mes = ( fecha & 0x01E0 ) >> 5;
    return ( mes );
}

