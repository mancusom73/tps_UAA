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
#include <get_fech.h> 
#include <math.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
int tabla_fecha[] = {31,28,31,30,31,30,31,31,30,31,30,31};
/******************************************************************************/
int FECHA( char dia, char mes, unsigned anio )
/******************************************************************************/
{
    unsigned fecha2 = 0x1f80;
    char error = 0;
	char cyear[5], cyear_aux[5];
	int year = 0;
	
	memset( cyear, '0', sizeof(cyear) );
    memset( cyear_aux, '0', sizeof(cyear_aux) );

    if( anio < 1900 ) {
        if( anio > 79 ) {
            anio += 1900;
        }
        else {
            //anio += 2000;
			_snprintf( cyear_aux, sizeof( cyear_aux ),"%d", YEAR( _GET_FECHA_DOS( ) ) );
			cyear_aux[sizeof( cyear_aux )-1] = 0;
        	strncpy( cyear, &cyear_aux[0], ( anio <= 9 )? 3 : 2 );
        	cyear[sizeof( cyear )-1] = 0;
			year = atoi( cyear );
			anio += year;
        }
    }
    if( dia > 31 || dia <= 0 || mes > 12 || mes <= 0 || anio > 2099 ) {
        fecha2 = 0;
    }
    else {
        fecha2 = 0;
        if( mes == 2 && anio % 4 == 0 ) {
            if( dia > 29 ) {
                error = 1;
            }
        }
        else if( dia > tabla_fecha[mes - 1] ) {
            error = 1;
        }
        if( !error ) {
            fecha2 = ( anio - 1980 ) << 9;
            fecha2 += ( mes << 5 );
            fecha2 += dia;
        }
    }
    return ( fecha2 );
}
/******************************************************************************/
void FECHA__( int fecha, int *dia, int *mes, int *anio )
/******************************************************************************/
{
    *anio = ( ( fecha & 0xFE00 ) >> 9 ) + 1980;
    *mes = ( ( fecha & 0x01E0 ) >> 5 );
    *dia = ( fecha & 0x001F );
}
/******************************************************************************/
int DOW( unsigned fecha )
/******************************************************************************/
{
    int rta;
    time_t time1;
    struct tm time_check;
    int year,month,day;
    FECHA__( fecha, &day, &month, &year );
    time_check.tm_year = year - 1900;
    time_check.tm_mon = month - 1;
    time_check.tm_mday = day;
    time_check.tm_hour = 0;
    time_check.tm_min = 0;
    time_check.tm_sec = 1;
    time_check.tm_isdst = -1;
    time1 = mktime( &time_check );
    if( time1 == -1 ) {
        time_check.tm_wday = 7;
    }
    rta = time_check.tm_wday;
    return ( rta );
}
/******************************************************************************/
int DIF_DIAS( unsigned fecha1, unsigned fecha2 )
/******************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    unsigned dias;
    time_t time1,time2;
    struct tm time_check;
    int year,month,day;
    FECHA__( fecha1, &day, &month, &year );
    time_check.tm_year = year - 1900;
    time_check.tm_mon = month - 1;
    time_check.tm_mday = day;
    time_check.tm_hour = 0;
    time_check.tm_min = 0;
    time_check.tm_sec = 1;
    time_check.tm_isdst = -1;
    time1 = mktime( &time_check );
    if( time1 == -1 ) {
        time_check.tm_wday = 7;
    }
    FECHA__( fecha2, &day, &month, &year );
    time_check.tm_year = year - 1900;
    time_check.tm_mon = month - 1;
    time_check.tm_mday = day;
    time_check.tm_hour = 0;
    time_check.tm_min = 0;
    time_check.tm_sec = 1;
    time_check.tm_isdst = -1;
    time2 = mktime( &time_check );
    if( time1 == -1 ) {
        time_check.tm_wday = 7;
    }
    dias = ( int )floor( ( difftime( time1, time2 ) / ( ( long )24 * 60 * 60 ) ) + .5 );
    #else
    unsigned fecha,dias;
    time_t time1,time2;
    fecha = _GET_FECHA_DOS();
    SET_FECHA_DOS( fecha1 );
    time( &time1 );
    SET_FECHA_DOS( fecha2 );
    time( &time2 );
    SET_FECHA_DOS( fecha );
    dias = difftime( time1, time2 ) / ( ( long )24 * 60 * 60 );
    #endif
    return ( dias );
}
/******************************************************************************/
int INC_DIA( int fecha, int dias )
/******************************************************************************/
{
    int fec;
    struct tm time_check;
    int year,month,day;
    FECHA__( fecha, &day, &month, &year );
    time_check.tm_year = year - 1900;
    time_check.tm_mon = month - 1;
    time_check.tm_mday = day + dias;
    time_check.tm_hour = 0;
    time_check.tm_min = 0;
    time_check.tm_sec = 1;
    time_check.tm_isdst = -1;
    mktime( &time_check );
    fec = FECHA( ( char )time_check.tm_mday, ( char )( time_check.tm_mon + 1 ),
                 ( unsigned int ) time_check.tm_year );
    return fec;
}
/******************************************************************************/
int FECHA_VALIDA( int fecha )
/******************************************************************************/
{
    int valida = 1;
    int year,month,day;
    FECHA__( fecha, &day, &month, &year );
    if( day < 1 ) {
        valida = 0;
    }
    if( month < 1 || month > 12 ) {
        valida = 0;
    }
    if( month == 2 && year % 4 == 0 ) {
        if( day > 29 ) {
            valida = 0;
        }
    }
    else {
        if( day > tabla_fecha[month - 1] ) {
            valida = 0;
        }
    }
    return valida;
}

