#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <crypt.h> 
#include <get_fech.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
/******************************************************************************/
int GET_FECHA_DOS()
/******************************************************************************/
{
    unsigned fecha;
    if( _UTILIZAR_FECHA_OPERACION ) {
		if(!NRO_FECHA_OPERACION)
			SET_MEMORY_INT( __nro_fecha_operacion, _GET_FECHA_DOS() );
        fecha = NRO_FECHA_OPERACION;
    }
    else {
        fecha = _GET_FECHA_DOS();
    }
    return ( fecha );
}
/******************************************************************************/
unsigned _GET_FECHA_DOS()
/******************************************************************************/
{
    unsigned fecha, conversor1 = 17, conversor2 = 2;
    struct tm f;
    time_t now;
    CRYPT( __CONVERSOR_FECHA1, ( char* )&conversor1 );
    CRYPT( __CONVERSOR_FECHA2, ( char* )&conversor2 );
    time( &now );
    f = *localtime( &now );
    fecha = ( f.tm_year + 1900 - conversor1 ) << 9;
    fecha += ( ( f.tm_mon + 1 ) << conversor2 );
    fecha += f.tm_mday;
    return ( fecha );
}
/******************************************************************************/
int GET_FECHA_BACKUP()
/******************************************************************************/
{
    unsigned fecha;
    fecha = ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS();
	return ( fecha );
}

