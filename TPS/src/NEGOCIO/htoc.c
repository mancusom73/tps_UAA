#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <htoc.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Transforma una hora a cadena con formato HHMM
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void HTOC( int hora, char *cadena )
/******************************************************************************/
{
    unsigned h,m;
    unsigned char _h[3],_m[3];
    h = hora / 100;
    m = hora - h * 100;
    _itoa( h, _h, 10 );
    _itoa( m, _m, 10 );
    cadena[0] = ( _h[1] == 0 ) ? '0' : _h[0];
    cadena[1] = ( _h[1] == 0 ) ? _h[0] : _h[1];
    cadena[2] = ( _m[1] == 0 ) ? '0' : _m[0];
    cadena[3] = ( _m[1] == 0 ) ? _m[0] : _m[1];
    if( hora == -1 ) {
        memset( cadena, ' ', 4 );
    }
}

