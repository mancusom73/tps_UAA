#pragma pack (1) 

//#ifdef MODULO_PROMOCIONES
#include "time2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <get_fech.h>
#include <get_hora.h>
#include <fecha.h>
#include <htoa.h>

#ifdef __MSDOS__
#define TIME_OFFSET 10800
//#define TIME_OFFSET2 3600
#define TIME_OFFSET2 7200
#else
#define TIME_OFFSET 10800
#define TIME_OFFSET2 0
#endif

#ifdef INVEL_W
#include "cr.h"
#endif //INVEL_W

#pragma pack (1)

/* 10957 = cant de dias desde 1970-01-01 hasta 2000-01-01 */
/* CANTDIASOFFSET = cant de dias desde 1970-01-01 hasta Y0 */
#define CANTDIASOFFSET -25567

/* 86400 = cant de seg por dia*/
#define CANTSEGDIA 86400


/* 946695600 = Cant de seg desde 1970-01-01 hasta 2000-01-01 */
#define CANTSEGOFFSET 946695600
/* CANTSEGOFFSET = cant de seg desde 1970-01-01 hasta Y0 */
//#define CANTSEGOFFSET (CANTDIASOFFSET*CANTSEGDIA) 


/*
 * 1 si year es bisiesto, 0 c.c.
 */
int bisiesto( int year )
{
    return ( ( ( year % 4 ) == 0 ) && ( ( year % 100 != 0 ) || ( year % 400 ) == 0 ) ? 1 : 0 );
}

/*
 * Cantidad de dias que tiene el anio year
 */
int days_y( int year )
{
    return 365 + bisiesto( year );
}

/*
 * Cantidad de dias que tiene el mes m del anio year
 * CUIDADO: si el mes no es un mes correcto, devuelve 0.
 */
int days_m( int m, int year )
{
    switch( m ) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 2:
            return 28 + bisiesto( year );
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        default:
            return 0;
    }
}

/*
 * Construir un date a partir de un string
 */
int mk_date( idate *da, char *str )
{
    int y,m,d;

    sscanf( str, "%d-%d-%d", &y, &m, &d );
    if( y < Y0 ) {
        return -1;
    }
    if( ( m < 1 ) || ( m > 12 ) ) {
        return -1;
    }
    if( d < 1 ) {
        return -1;
    }
    switch( m ) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if( d > 31 ) {
                return -1;
            }
            break;
        case 2:
            if( ( d > 29 ) || ( !bisiesto( y ) && ( d > 28 ) ) ) {
                return -1;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if( d > 30 ) {
                return -1;
            }
            break;
        default:
            return -1;
            //break;
    }
    *da = 0;
    {
        int i;
        for( i = Y0;i < y;i++ ) {
            *da += 365 + bisiesto( i );
        }
    }
    {
        int i = bisiesto( y );
        switch( m ) {
            case 1:
                break;
            case 2:
                *da += 31;
                break;
            case 3:
                *da += 59 + i;
                break;
            case 4:
                *da += 90 + i;
                break;
            case 5:
                *da += 120 + i;
                break;
            case 6:
                *da += 151 + i;
                break;
            case 7:
                *da += 181 + i;
                break;
            case 8:
                *da += 212 + i;
                break;
            case 9:
                *da += 243 + i;
                break;
            case 10:
                *da += 273 + i;
                break;
            case 11:
                *da += 304 + i;
                break;
            case 12:
                *da += 334 + i;
                break;
        }
    }
    *da += d;
    return 0;
}

/*
 * Construir un string a partir de un date.
 * format: 0 -> aaaa-mm-dd / 1 -> dd/mm/aaaa
 */
int strdate( char *buf, int max, idate da, int format )
{
    int y = 0;
    int m = 0;
    int d = 0;
    while( days_y( y + Y0 ) < da ) {
        da -= days_y( y + Y0 );
        y++;
    }
    for( m = 1;( ( m < 13 ) && ( days_m( m, y + Y0 ) < da ) );m++ ) {
        da -= days_m( m, y + Y0 );
    }
    d = da;
    if( max < 11 ) {
        return -1;
    }
	switch (format) {
		case 0:
    if( sprintf( buf, "%4d-%2d-%2d", y + Y0, m, d ) != -1 ) {
        if( m < 10 ) {
            buf[5] = '0';
        }
        if( d < 10 ) {
            buf[8] = '0';
        }
        return 0;
    }
    else {
        return -1;
    }
			break;
		case 1:
			if( sprintf( buf, "%2d/%2d/%2d", d, m, ( y + Y0 ) % 100 ) != -1 ) {
				if( ( y + Y0 ) % 100  < 10 ) {
					buf[6] = '0';
				}
				if( m < 10 ) {
					buf[3] = '0';
				}
				if( d < 10 ) {
					buf[0] = '0';
				}
				return 0;
			}
			else {
				return -1;
			}
			break;
		default:
			return -1;
			break;
	} 
}

idate getdate_()
{
    #ifdef INVEL_W
    //return mk_date_from_invel( ( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_FECHA_DOS() );
    return mk_date_from_invel( _GET_FECHA_DOS() );	// 26-03-07
    #else //INVEL_W
    time_t t;
    ldiv_t d;
    if( time( &t ) == -1 ) {
        return -1;
    }
    d = ldiv( (long)t - TIME_OFFSET, CANTSEGDIA ); // 86400 = cant de seg por dia//
    return ( idate )d.quot - CANTDIASOFFSET + ( d.rem > 0 ? 1 : 0 );
    // CANTDIASOFFSET = cant de dias desde 1970-01-01 hasta Y0 //
    #endif //INVEL_W
}

int mk_time( itime *tm, char *t )
{
    int h,m;
    sscanf( t, "%d:%d", &h, &m );
    if( ( h < 0 ) || ( h > 23 ) ) {
        return -1;
    }
    if( ( m < 0 ) || ( m > 59 ) ) {
        return -2;
    }
    *tm = h * 60;
    *tm += m;
    return 0;
}

int strtime( char *buf, int max, itime t )
{
    div_t h;
    h = div( t, 60 );
    if( max < 6 ) {
        return -1;
    }
    if( sprintf( buf, "%2d:%2d", h.quot, h.rem ) == -1 ) {
        return -1;
    }
    if( h.quot < 10 ) {
        buf[0] = '0';
    }
    if( h.rem < 10 ) {
        buf[3] = '0';
    }
    return 0;
}

itime gettime_()
{
    #ifdef INVEL_W
    return mk_time_from_invel( GET_HORA_DOS() ) ;
    #else //INVEL_W
    time_t t;
    ldiv_t d;
    if( time( &t ) == -1 ) {
        return -1;
    }
    t -= CANTSEGOFFSET; // Cant de seg desde 1970-01-01 hasta Y0 //
    d = ldiv( (long)t - TIME_OFFSET2, CANTSEGDIA ); // 86400 = cant de seg por dia//
    d = ldiv( d.rem, 60 );
    return ( itime )d.quot;
    #endif //INVEL_W
}

#if defined(INVEL_W) || defined(INVEL_L)
idate mk_date_from_invel( int fecha )
{
    int dia = 0, mes = 0, anio = 0;
    char buff[11];
    idate rta;
    memset( buff, 0, 11 );
    FECHA__( fecha, &dia, &mes, &anio );
    //_snprintf(buff,11,"%4i-%2i-%2i",anio,mes,dia);
    sprintf( buff/*,11*/, "%4i-%2i-%2i", anio, mes, dia );
    mk_date( &rta, buff );
    return rta;
}


itime mk_time_from_invel( unsigned hora )
{
    char buff[7];
    itime tm;

    memset( buff, 0, 7 );
    HTOA( hora, buff );
    mk_time( &tm, buff );
    return tm;
}
#endif //INVEL_W , INVEL_L

//#endif // MODULO_PROMOCIONES


