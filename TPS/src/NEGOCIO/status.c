#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <pinpad1.h> 
#include <status.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
int STATUS()
/*****************************************************************************/
{
    int st = 0;
    if( HARD_INVEL ) {
        st = EJECUTAR_COMANDO( 0xf0, 0, 0, 0, 4 );
        if( st ) {
            st = 0;
        }
        else {
            st = BUFFER[0];
        }
    }
    return ( st );
}

