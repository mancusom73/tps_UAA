#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <pinpad1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_GEMPLUS
/*****************************************************************************/
int GP_WRITE_MEMORY( unsigned dir, char cant, char *buffer )
/*****************************************************************************/
{
    int st,dir_hi,dir_lo;
    dir_hi = dir / 256;
    dir_lo = dir - ( dir_hi * 256 );
    memcpy( BUFFER, buffer, cant );
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    st = EJECUTAR_COMANDO( 0xf1, 0xd0, dir_hi, dir_lo, cant );
    #endif
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

