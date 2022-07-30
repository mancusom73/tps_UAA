#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <round.h> 
#include <math.h>

void ROUND2( double* numero, int enteros, int decimales );
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
//double ROUND( double numero, int enteros, int decimales )
/*****************************************************************************/
/*{
   char     cadena[80], mascara[10];
    if( numero != 0 )
    {
        numero += ( numero > 0 ) ? pow( 10.0, -10.0 ) : -pow( 10.0, -10.0 );
    }
    sprintf( mascara, "%c%d.%dlf", '%',enteros + decimales, decimales );
    sprintf( cadena, mascara, numero );
    numero = atof( cadena );
    return ( numero );
}


/*****************************************************************************/
void ROUND2( double* numero, int enteros, int decimales )
/*****************************************************************************/
{
    char *c1,c2[300];
    int decimal, signo, p1 = 0;
//    double xx, x2;

    //  ecvt( numero, enteros + decimales, &decimal, &signo );
    //  c1 = ecvt( numero, decimal + decimales, &decimal, &signo );
    c1 = _fcvt( *numero, decimales, &decimal, &signo );


    if( signo ) {
        c2[p1++] = '-';
    }
    if( decimal < 0 ) {
        if( -decimal >= decimales ) {
            c2[0] = '0';
            c2[1] = 0;
        }
        else {
            c2[p1++] = '.';
            memset( &c2[p1], '0', abs( decimal ) );
            c2[p1 + abs( decimal )] = 0;
            strcat( c2, c1 );
        }
    }
    else {
        memcpy( &c2[p1], c1, decimal );
        if( p1 + decimal > 0 && c2[p1 + decimal - 1] == 0 ) {
            c2[p1 + decimal - 1] = '0';
        }
        c2[p1 + decimal] = 0;
        strcat( c2, "." );
        strcat( c2, c1 + decimal );
    }

  //  xx = atof( c2 );
//	xx = strtod( c2, NULL );
	*numero = strtod( c2, NULL );
    //return ( xx );
}


/*****************************************************************************/
double ROUND( double doValue, int enteros, int nPrecision )
/*****************************************************************************/
{
   static const double doBase = 10.0;
   double doComplete5, doComplete5i;
      doComplete5 = doValue * pow(doBase, (double) (nPrecision + 1));
      if(doValue < 0.0)
       doComplete5 -= 5.01;
   else
       doComplete5 += 5.01;
      doComplete5 /= doBase;
   modf(doComplete5, &doComplete5i);
      return doComplete5i / pow(doBase, (double) nPrecision);
}

