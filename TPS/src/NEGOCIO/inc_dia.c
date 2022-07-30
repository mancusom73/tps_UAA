#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <dbrouter.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
#if !defined(WIN32) && !defined(INVEL_L)
/******************************************************************************/
INCREMENTAR_DIA_DOS()
/******************************************************************************/
{
    struct date d,d2;
    getdate( &d );
    d.da_day++;
    setdate( &d );
    getdate( &d2 );
    if( memcmp( &d, &d2, sizeof( struct date ) ) != 0 ) {
        d.da_day = 1;
        d.da_mon++;
        setdate( &d );
        getdate( &d2 );
        if( memcmp( &d, &d2, sizeof( struct date ) ) != 0 ) {
            d.da_day = 1;
            d.da_mon = 1;
            d.da_year++;
            setdate( &d );
        }
    }
}
#endif

