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
int GP_MAKE_FILE_EXT( char tipo, char palabras, char a_lec, char a_esc, char a_mod )
/*****************************************************************************/
{
    int st;
    BUFFER[0] = tipo;
    BUFFER[1] = a_lec;
    BUFFER[2] = a_esc;
    BUFFER[3] = palabras;
    BUFFER[4] = a_mod;
    st = EJECUTAR_COMANDO( 0xf1, 0xd4, 0, 0, 5 );
    if( res1 != 0x90 || res2 != 0x00 ) {
        st = 1;
    }
    return ( st );
}
#endif

