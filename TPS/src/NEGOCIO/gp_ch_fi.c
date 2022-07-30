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
int GP_CHECKSUM_FILE( char nro_archivo, char *buffer )
/*****************************************************************************/
{
    int st;
    st = EJECUTAR_COMANDO( 0xf2, 0xf0, 0, nro_archivo, 2 );
    memcpy( buffer, BUFFER, 2 );
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

