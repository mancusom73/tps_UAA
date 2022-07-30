#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <flush.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
/*****************************************************************************/
int FLUSH()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    union REGS reg;
    reg.x.ax = 0x0d00;
    intdos( &reg, &reg );
    #endif
    return 0;
}

