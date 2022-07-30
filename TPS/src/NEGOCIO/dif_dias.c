#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <get_fech.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Calcula la diferencia en días entre dos fechas
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
//DIF_DIAS( unsigned fecha1, unsigned fecha2 )
/******************************************************************************/
/*{
 * #ifdef INVEL_W
 * #else
 * unsigned  fecha, dias;
 * time_t    time1, time2;
 *
 * fecha = _GET_FECHA_DOS(  );
 * SET_FECHA_DOS( fecha1 );
 * time( &time1 );
 * SET_FECHA_DOS( fecha2 );
 * time( &time2 );
 * SET_FECHA_DOS( fecha );
 * dias = difftime( time1, time2 ) / ( ( long ) 24 * 60 * 60 );
 *
 * return ( dias );
 * #endif
 * }
 */

