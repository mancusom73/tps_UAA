#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <htoa.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Transforma una hora a cadena con formato HH:MM
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void HTOA( unsigned hora, char *cadena )
/******************************************************************************/
{
    unsigned char h,m,_h[3],_m[3];
    h = hora / 100;
    m = hora - h * 100;
    _itoa( h, _h, 10 );
    _itoa( m, _m, 10 );
    cadena[0] = ( _h[1] == 0 ) ? '0' : _h[0];
    cadena[1] = ( _h[1] == 0 ) ? _h[0] : _h[1];
    cadena[2] = ':';
    cadena[3] = ( _m[1] == 0 ) ? '0' : _m[0];
    cadena[4] = ( _m[1] == 0 ) ? _m[0] : _m[1];
    cadena[5] = 0;
    if( hora == ( unsigned ) - 1 ) {
        strcpy( cadena, "  :  " );
    }
}
/******************************************************************************/
void HTOAL( long hora, char *cadena )
/******************************************************************************/
{
    int h,m,s;
    h = hora / 10000L;
    m = ( hora / 100L ) - h * 100;
    s = hora % 100L;
    sprintf( cadena, "%02i:%02i:%02i", h, m, s );
    if( hora == -1 ) {
        strcpy( cadena, "  :  :  " );
    }
}

