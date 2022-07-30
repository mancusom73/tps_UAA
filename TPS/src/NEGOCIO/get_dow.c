#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <fecha.h> 
#include <get_dow.h> 
#include <get_fech.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
int GET_DIA_DE_LA_SEMANA();
/******************************************************************************/
int GET_DOW()
/******************************************************************************/
{
    return ( GET_DIA_DE_LA_SEMANA() );
}
/******************************************************************************/
int GET_DIA_DE_LA_SEMANA()
/******************************************************************************/
{
    struct tm *tblock;
    time_t timer;
    time( &timer );
    tblock = localtime( &timer );
    return ( tblock->tm_wday );
}
/******************************************************************************/
//DOW( unsigned fecha )
/******************************************************************************/
/*{
 * //unsigned hoy;
 * int rta;
 *
 * hoy = _GET_FECHA_DOS();
 * SET_FECHA_DOS( fecha );
 * rta = GET_DIA_DE_LA_SEMANA();
 * SET_FECHA_DOS( hoy );
 *
 *
 *
 *
 * return( rta );
 * }
 */
/******************************************************************************/
void CDOW( unsigned fecha, char *cadena )
/******************************************************************************/
{
    _itoa( DOW( fecha ), cadena, 10 );
}

