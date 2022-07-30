#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <rollo.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
void PEDIR_NRO_ROLLO_INTERNO()
/*****************************************************************************/
{
    long nro;
    nro = PEDIR_ENTERO_EN_RECUADRO( ST( S__NRO_ROLLO_ ), SI, 6 );
    if( nro > 0 ) {
        if( SOLICITAR_CLAVES( 30, 21, NO, SI, SI, " NRO ROLLO ", " NRO ROLLO ",
                              _F_SUP_ROLLO_INTERNO, SI ) ) {
            SET_MEMORY_LONG( __nro_rollo, nro );
        }
    }
}

