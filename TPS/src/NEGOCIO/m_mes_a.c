#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <m_cadena.h> 
#include <stringt.h>
#include <str.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_MES_ANIO( int x, int y, unsigned fecha )
/******************************************************************************/
{
    unsigned char cadena[9],mes,mm[3],aa[5];
    unsigned anio;
    anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    mes = ( ( fecha & 0x01E0 ) >> 5 );
    _itoa( mes, mm, 10 );
    _itoa( anio, aa, 10 );
    cadena[0] = ( mm[1] == 0 ) ? '0' : mm[0];
    cadena[1] = ( mm[1] == 0 ) ? mm[0] : mm[1];
    cadena[2] = '/';
    cadena[3] = ( aa[1] == 0 ) ? '0' : aa[2];
    cadena[4] = ( aa[1] == 0 ) ? aa[0] : aa[3];
    cadena[5] = 0;
    if( fecha == 0 ) {
        strcpy( (char *)cadena, "  /  " );
    }
    MOSTRAR_CADENA( x, y, (char *)cadena );
}

