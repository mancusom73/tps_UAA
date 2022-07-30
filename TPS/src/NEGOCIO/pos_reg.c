#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cio.h> 
#include <cr.h> 
#include <pos_reg.h> 
#include <tkt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***********************************************************************/
void POSICIONAR_REGISTRO( int handle, int registro, int l_registro )
/***********************************************************************/
{
    long pos;
    if( registro > 0 ) {
        pos = ( registro - 1 ) * l_registro;
        LSEEK( handle, pos, SEEK_SET, __LINE__, __FILE__ );
    }
    else {
        pos = registro * l_registro;
        LSEEK( handle, pos, SEEK_END, __LINE__, __FILE__ );
    }
}

