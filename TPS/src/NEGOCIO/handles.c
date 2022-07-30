#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <handles.h> 
#include <tkt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Ampliacion de handles a la declarada en el config.sys
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
//static char tabla_handles[100];
extern unsigned _psp;
/***************************************************************************/
int AJUSTA_CANTIDAD_HANDLES( int cant_pedida )
/***************************************************************************/
{
    int rta = 0;
    return ( rta );
}

