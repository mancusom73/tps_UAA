#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <day.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Devuelve el dia del mes
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
int DAY( unsigned fecha )
/******************************************************************************/
{
    unsigned dia;
    dia = ( fecha & 0x001F );
    return ( dia );
}
/******************************************************************************/
void CDAY( unsigned fecha, char *cadena )
/******************************************************************************/
{
    int dia;
    dia = DAY( fecha );
    sprintf( cadena, "%02i", dia );
}

