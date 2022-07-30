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
int GP_LOCK_FILE( char nro_archivo, char b_lec, char b_esc, char b_mod )
/*****************************************************************************/
{
    int st;
    BUFFER[0] = 0;
    if( b_lec ) {
        BUFFER[0] |= 0x80;
    }
    if( b_esc ) {
        BUFFER[0] |= 0x08;
    }
    if( b_mod ) {
        BUFFER[0] |= 0x01;
    }
    st = EJECUTAR_COMANDO( 0xf1, 0xd8, 0, nro_archivo, 1 );
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

