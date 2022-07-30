#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <pant.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_GRAN_CARACTER( int x, int y, char caracter )
/******************************************************************************/
{
    PUT_GRAN_CHAR( x, y, caracter );
}

