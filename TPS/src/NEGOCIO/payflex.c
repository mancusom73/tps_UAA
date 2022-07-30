#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <val2.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if !defined(INVEL_W) && !defined(INVEL_L)
/*****************************************************************************/
PAYFLEX_LEER_ARCHIVO( unsigned dir, int long_reg, char *buffer, int bytes )
/*****************************************************************************/
{
    int ok = 1, i, resto, _bytes;
    if( !VAL_SELECT_CARD( _PAYFLEX ) && VAL_PAYFLEX_SELECT( dir ) ) {
        resto = bytes;
        for( i = 0;resto > 0 && ok;i++ ) {
            _bytes = ( resto > long_reg ) ? long_reg : resto;
            if( VAL_PAYFLEX_READ( i + 1, _bytes, &buffer[i * long_reg] ) ) {
                ok = 0;
            }
            resto -= _bytes;
        }
    }
    else {
        ok = 0;
    }
    return ( ok );
}
/*****************************************************************************/
PAYFLEX_GRABAR_ARCHIVO( unsigned dir, int long_reg, char *buffer, int bytes )
/*****************************************************************************/
{
    int ok = 1, i, resto, _bytes;
    if( !VAL_SELECT_CARD( _PAYFLEX ) && VAL_PAYFLEX_SELECT( dir ) ) {
        resto = bytes;
        for( i = 0;resto > 0 && ok;i++ ) {
            _bytes = ( resto > long_reg ) ? long_reg : resto;
            if( VAL_PAYFLEX_WRITE( i + 1, _bytes, &buffer[i * long_reg] ) ) {
                ok = 0;
            }
            resto -= _bytes;
        }
    }
    else {
        ok = 0;
    }
    return ( ok );
}
#endif

