#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <fecha.h> 
#include <inc_mes.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
int INCREMENTAR_MES( unsigned fecha, unsigned meses )
/******************************************************************************/
{
    unsigned char mes,dia;
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 80;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    mes += meses;
    for( ;mes > 12;anio++, mes -= 12 ) {
        ;
    }
    fecha = FECHA( dia, mes, anio );
    if( !fecha && dia >= 28 ) {
        if( mes == 2 ) {
            dia = 28;
        }
        else {
            dia--;
        }
        fecha = FECHA( dia, mes, anio );
    }
    return ( fecha );
}

