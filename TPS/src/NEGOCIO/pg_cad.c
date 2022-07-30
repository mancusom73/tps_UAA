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
#else
#endif
#pragma warning (disable:4103)  //aviso de pragma pack en cr.h
/******************************************************************************/
int PEDIR_GRAN_CADENA( char x, char y, char largo, char *cadena, char *mascara_scanner )
/******************************************************************************/
{
    int rta;
    _GRANDE = 1;
    rta = PEDIR_CADENA( x, y, largo, cadena, mascara_scanner, SI, NO );
    _GRANDE = 0;
    return ( rta );
}

