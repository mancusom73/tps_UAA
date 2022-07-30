#pragma pack (push,1) 
#ifndef _TIME_
#define _TIME_
#include "etpv.h"

#pragma pack (1)

/* Anio desde el que se empiezan a contar los min transcurridos. */
#define Y0 1900

typedef long idate;  /* Cantidad de dias transcurridos desde 2000-01-01 */
typedef long itime;  /* Cantidad de minutos transcurridos desde 00:00 hs */

int mk_date( idate *da, char *d );

int strdate( char *buf, int max, idate d, int format );

idate getdate_( void );

int mk_time( itime *tm, char *t );

int strtime( char *buf, int max, itime t );

itime gettime_( void );

idate mk_date_from_invel( int fecha );

itime mk_time_from_invel( unsigned hora );

#endif

#pragma pack (pop) 
