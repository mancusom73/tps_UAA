#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <clave.h> 
#include <cr.h> 
#include <cr_disco.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <log.h> 
#include <mstring.h> 
#include <power.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
//static long ticks_power = 0, ticks_ant_power = 0;
/*****************************************************************************/
int INIT_POWER_DOWN()
/*****************************************************************************/
{
    int error = 0;
    if( TICKS_POWER_DOWN ) {
        DRIVER( _DRV_power, _PWR_inicializacion, 0, &error, sizeof( error ) );
    }
    return ( !error );
}
/*****************************************************************************/
int POWER_STATUS()
/*****************************************************************************/
{
    int error = 0;
    if( TICKS_POWER_DOWN ) {
        DRIVER( _DRV_power, _PWR_status, 0, &error, sizeof( error ) );
    }
    return ( !error );
}
/*****************************************************************************/
int POWER_DOWN()
/*****************************************************************************/
{
    int error = 0;
    if( TICKS_POWER_DOWN ) {
        DRIVER( _DRV_power, _PWR_down, 0, &error, sizeof( error ) );
    }
    return ( !error );
}
/*****************************************************************************/
void VERIFICA_POWER_DOWN()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    unsigned long *t;
    if( TICKS_POWER_DOWN ) {
        t = ( void* )MK_FP_BIOS_PROTEGIDO( 0x6c );
        // cambio de dia
        if( ticks_ant_power > *t ) {
            ticks_power = 0;
        }
        if( *t > ticks_power + TICKS_POWER_DOWN ) {
            ticks_power = *t;
            ticks_ant_power = ticks_power;
            if( !POWER_STATUS() ) {
                CIERRA_TODOS_LOS_ARCHIVOS();
                MENSAJE_STRING( S_POWER_DOWN );
                while( 1 ) {
                    GRABAR_LOG_SISTEMA( "*******************" );
                    GRABAR_LOG_SISTEMA_STRING( S_POWER_DOWN );
                    GRABAR_LOG_SISTEMA( "*******************" );
                    POWER_DOWN();
                }
            }
        }
    }
    return;
    #endif
}
/*****************************************************************************/
void END_POWER_DOWN()
/*****************************************************************************/
{
    int error;
    if( TICKS_POWER_DOWN ) {
        DRIVER( _DRV_power, _PWR_fin, 0, &error, sizeof( error ) );
    }
}

