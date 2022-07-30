#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <dtoa.h> 
#include <fecha.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void DTOA( unsigned fecha, char *cadena, int anio_completo )
/******************************************************************************/
{
    unsigned char mes,dia,dd[3],mm[3],aa[5];
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    _itoa( dia, dd, 10 );
    _itoa( mes, mm, 10 );
    _itoa( anio, aa, 10 );
    cadena[0] = ( dd[1] == 0 ) ? '0' : dd[0];
    cadena[1] = ( dd[1] == 0 ) ? dd[0] : dd[1];
    cadena[2] = ( mm[1] == 0 ) ? '0' : mm[0];
    cadena[3] = ( mm[1] == 0 ) ? mm[0] : mm[1];
    if( anio_completo ) {
        cadena[4] = ( aa[1] == 0 ) ? '0' : aa[0];
        cadena[5] = ( aa[1] == 0 ) ? aa[0] : aa[1];
        cadena[6] = ( aa[1] == 0 ) ? '0' : aa[2];
        cadena[7] = ( aa[1] == 0 ) ? aa[0] : aa[3];
    }
    else {
        cadena[4] = ( aa[1] == 0 ) ? '0' : aa[2];
        cadena[5] = ( aa[1] == 0 ) ? aa[0] : aa[3];
    }
}
/******************************************************************************/
long DTOL( unsigned fecha )
/******************************************************************************/
{
    long fec = 0;
    unsigned mes,dia;
    long anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    dia = ( fecha & 0x001F );
    fec = ( ( anio ) << 16 ) + ( mes << 8 ) + dia;
    return fec;
}
/******************************************************************************/
int DLTOD( long fecha )
/******************************************************************************/
{
    int fec = 0;
    unsigned mes,dia;
    long anio;
    anio = fecha >> 16;
    mes = ( fecha & 0xff00 ) >> 8;
    dia = ( fecha & 0x00ff );
    fec = FECHA( ( char )dia, ( char )mes, anio );
    return fec;
}

