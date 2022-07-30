#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
/*****************************************************************************/
void INIT_DELAY()
/*****************************************************************************/
{
    #ifdef __TURBOC__
    int h,far *p;
    char *c;
    c = getenv( "DELAY" );
    if( c ) {
        p = MK_FP( FP_SEG( delay ), 0 );
        h = atoi( c );
        p[0] = h;
    }
    #endif
}

