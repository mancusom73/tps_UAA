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
int GP_UPDATE_SECRET_CODE_EXT( char nro, char cl_actual[8], char cl_nueva[8] )
/*****************************************************************************/
{
    int st;
    memcpy( BUFFER, cl_actual, 8 );
    memcpy( &BUFFER[8], cl_nueva, 8 );
    st = EJECUTAR_COMANDO( 0xf1, 0x24, 0, nro, 16 );
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

