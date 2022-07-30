#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <dtoai.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Transforma una fecha a cadena en formato AAMMDD
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void DTOA_INVERSA( unsigned fecha, char *cadena )
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
    cadena[4] = ( dd[1] == 0 ) ? '0' : dd[0];
    cadena[5] = ( dd[1] == 0 ) ? dd[0] : dd[1];
    cadena[2] = ( mm[1] == 0 ) ? '0' : mm[0];
    cadena[3] = ( mm[1] == 0 ) ? mm[0] : mm[1];
    cadena[0] = ( aa[1] == 0 ) ? '0' : aa[2];
    cadena[1] = ( aa[1] == 0 ) ? aa[0] : aa[3];
}

/******************************************************************************/
void DTOA_INVERSA2( unsigned fecha, char *cadena )
/* pasa de fecha invel a fecha cadena formato DDMMAA
*/
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
    cadena[2] =  '/' ;

	cadena[3] = ( mm[1] == 0 ) ? '0' : mm[0];
    cadena[4] = ( mm[1] == 0 ) ? mm[0] : mm[1];
    
	cadena[5] =  '/' ;
	cadena[6] = ( aa[1] == 0 ) ? '0' : aa[2];
    cadena[7] = ( aa[1] == 0 ) ? aa[0] : aa[3];
}


