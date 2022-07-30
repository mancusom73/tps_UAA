#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <fecha.h> 
#include <inc_year.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
int INC_YEAR( unsigned fecha, unsigned anios )
/******************************************************************************/
{
    unsigned char mes,dia;
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 80;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    anio += anios;
    fecha = FECHA( dia, mes, anio );
    return ( fecha );
}

