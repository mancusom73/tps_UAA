#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <imp_comp.h> 
#include <imp_dev.h> 
#include <_cr1.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/**************************************************************************/
void TICKET_DEVOLUCIONES( double importe )
/**************************************************************************/
{
    if( IMPRESORA_FISCAL ) {
        IMPRIME_COMPROBANTE( ST( S_NOTA_DE_CREDITO ), importe, IMPRIME_DEVOLUCIONES, _NOTA_CRED, NO );
    }
    else {
        IMPRIME_COMPROBANTE( ST( S_DEVOLUCIONES ), importe, IMPRIME_DEVOLUCIONES, _DEVOL, NO );
    }
}

