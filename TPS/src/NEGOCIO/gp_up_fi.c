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
int GP_UPDATE_FILE( char nro_archivo, char offset, char cant, char *buffer )
/*****************************************************************************/
{
    int st;
    memcpy( BUFFER, buffer, cant );
    st = EJECUTAR_COMANDO( 0xf1, 0xd6, offset, nro_archivo, cant );
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

