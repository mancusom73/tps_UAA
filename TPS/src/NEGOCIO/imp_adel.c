#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <imp_adel.h> 
#include <imp_comp.h> 
#include <_cr1.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/**************************************************************************/
void TICKET_ADELANTOS( double importe )
/**************************************************************************/
{
    //ImpFiscal-verificar
    IMPRIME_COMPROBANTE( ST( S_ADELANTOS ), importe, /*IMPRIME_ADELANTOS*/3, _ADELANTOS, NO );
}

