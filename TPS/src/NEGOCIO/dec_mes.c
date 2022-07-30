#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <dec_mes.h> 
#include <fecha.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Decrementa uno a mas meses a partir de una fecha
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
int DECREMENTAR_MES( unsigned fecha )
/******************************************************************************/
{
    unsigned char mes,dia;
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 80;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    mes--;
    if( mes == 0 ) {
        mes = 12;
        anio--;
    }
    fecha = FECHA( dia, mes, anio );
    return ( fecha );
}

