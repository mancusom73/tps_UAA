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
int GP_STATUS( char tipo, char *buffer )
/*****************************************************************************/
{
    int st,cant;
    cant = ( tipo == 0 ) ? 14 : 32;
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    st = EJECUTAR_COMANDO( 0xf2, 0xf2, 0, tipo, cant );
    #endif
    memcpy( buffer, BUFFER, cant );
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

