#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <pausa.h> 
#include <pinpad1.h> 
#include <val2.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_PINPAD
/*****************************************************************************/
void RESET_PINPAD()
/*****************************************************************************/
{
    switch( HARD_INVEL ) {
        case VERSION_1:
            EJECUTAR_COMANDO( 0xF7, 1, 0, 0, 0 );
            PAUSA( 36 );
            break;
        case VERSION_2:
            VAL_RESET();
            break;
    }
}
#endif

