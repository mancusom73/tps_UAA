#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <ctime.h> 
#include <pausa.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void PAUSA( int ticks )
/******************************************************************************/
{
    long desde,t;
    int salir = 0;
    desde = GET_TIME();
    while( !salir ) {
        t = GET_TIME();
        if( ( t - desde ) >= ticks || t < desde ) {
            salir = 1;
        }
    }
}

