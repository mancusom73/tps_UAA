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
int GP_LOAD_SECRET_CODE_EXT( char nro, char clave[8], unsigned valor_aut )
/*****************************************************************************/
{
    int st;
    memcpy( BUFFER, clave, 8 );
    BUFFER[8] = valor_aut / 256;
    BUFFER[9] = valor_aut - ( BUFFER[8] * 256 );
    st = EJECUTAR_COMANDO( 0xf1, 0x22, 0, nro, 10 );
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

