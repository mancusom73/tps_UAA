#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <ftoc.h> 
#include <m_cadena.h> 
#include <trim.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_FLOTANTE( int x, int y, int enteros, int decimales, int signo, int comas,
                       double importe )
/******************************************************************************/
{
    char cadena[20];
    FTOC( importe, cadena, enteros, decimales, signo, comas );
    MOSTRAR_CADENA( x, y, cadena );
}

/******************************************************************************/
void MOSTRAR_FLOTANTE_ALINEADO( int x, int y, int enteros, int decimales, int signo, int comas,
                       double importe )
/******************************************************************************/
{
    char cadena[20];
    FTOC( importe, cadena, enteros, decimales, signo, comas );
	LTRIM( cadena );
    MOSTRAR_CADENA( x, y, cadena );
}

/******************************************************************************/
void FORMAT_FLOTANTE( char *picture, double importe, char *cadena )
/******************************************************************************/
{
    int i, enteros = 0, decimales = 0, signo = 0, comas = 0, etapa = 0,
        ceros = 0;
    for( i = 0 ;picture[i] ;i++ ) {
        switch( picture[i] ) {
            case '-':
                enteros++;
                signo = 1;
                break;
            case '.':
                comas = 1;
                break;
            case ',':
                etapa = 1;
                break;
            case '0':
                ceros = 1;
            case '9':
                if( etapa == 0 ) {
                    enteros++;
                }
                else {
                    decimales++;
                }
                break;
        }
    }
    FTOC( importe, cadena, enteros, decimales, signo, comas );
    if( ceros ) {
        for( i = 0 ;cadena[i] ;i++ ) {
            if( cadena[i] == ' ' ) {
                cadena[i] = '0';
            }
        }
    }
}

