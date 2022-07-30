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
void MOSTRAR_GRAN_FLOTANTE( int x, int y, int enteros, int decimales, int signo, int comas,
                            double importe )
/******************************************************************************/
{
    _GRANDE = 1;
    MOSTRAR_FLOTANTE( x, y, enteros, decimales, signo, comas, importe );
    _GRANDE = 0;
}

