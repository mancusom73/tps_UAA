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
#ifndef INVEL_L
#endif
/******************************************************************************/
int PEDIR_GRAN_CADENA_NUMERICA( int x, int y, int largo, char *cadena )
/******************************************************************************/
{
    int rta;
    _GRANDE = 1;
    rta = PEDIR_CADENA_NUMERICA( ( char )x, ( char )y, ( char )largo, cadena, NULL, 0, SI, NO, NULL );
    _GRANDE = 0;
    return ( rta );
}

