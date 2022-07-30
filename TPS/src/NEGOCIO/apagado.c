#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <apagado.h> 
#include <cr.h> 
#include <ram.h> 
#include <val2.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
static int estado_power;
/******************************************************************************/
void SET_APAGADO( int estado )
/******************************************************************************/
{
    estado_power = estado;
    switch( CONTROL_DE_APAGADO ) {
        case APAGADO_POR_FLASH:
            SET_RAM( ON );
            break;
        case APAGADO_POR_TORRETA:
            if( HARD_INVEL == VERSION_2 ) {
                if( estado == ON ) {
                    VAL_PERMITE_APAGADO();
                }
                else {
                    VAL_ANULA_APAGADO();
                }
            }
            break;
    }
}
/******************************************************************************/
int GET_APAGADO()
/******************************************************************************/
{
    return ( estado_power );
}

