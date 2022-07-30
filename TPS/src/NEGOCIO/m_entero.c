#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <m_cadena.h> 
#include <m_entero.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_ENTERO( int x, int y, char *picture, long entero )
/******************************************************************************/
{
    char cadena[15];
    int l_cadena,l_picture,h;
    _ltoa( entero, cadena, 10 );
    l_cadena = strlen( cadena );
    l_picture = strlen( picture );
    for( h = l_picture - 1;h >= 0;h-- ) {
        if( picture[h] == '9' ) {
            if( l_cadena > 0 ) {
                cadena[h] = cadena[--l_cadena];
            }
            else {
                cadena[h] = ' ';
            }
        }
        else if( picture[h] == '0' ) {
            if( l_cadena > 0 ) {
                cadena[h] = cadena[--l_cadena];
            }
            else {
                cadena[h] = '0';
            }
        }
        else if( picture[h] == '.' ) {
            if( config.pais != CHILE && config.pais != BRASIL ) {
                if( l_cadena > 0 ) {
                    cadena[h] = cadena[--l_cadena];
                }
                else {
                    cadena[h] = '.';
                }
            }
            else {
                cadena[h] = '.';
            }
        }
        else if( l_cadena > 0 ) {
            cadena[h] = picture[h];
        }
        else {
            cadena[h] = ' ';
        }
    }
    cadena[l_picture] = 0;
    MOSTRAR_CADENA( x, y, cadena );
}
/******************************************************************************/
void SET_ENTERO( char *picture, long entero, char *cadena )
/******************************************************************************/
{
    int l_cadena,l_picture,h;
    _ltoa( entero, cadena, 10 );
    l_cadena = strlen( cadena );
    l_picture = strlen( picture );
    for( h = l_picture - 1;h >= 0;h-- ) {
        if( picture[h] == '9' ) {
            if( l_cadena > 0 ) {
                cadena[h] = cadena[--l_cadena];
            }
            else {
                cadena[h] = ' ';
            }
        }
        else if( picture[h] == '0' ) {
            if( l_cadena > 0 ) {
                cadena[h] = cadena[--l_cadena];
            }
            else {
                cadena[h] = '0';
            }
        }
        else if( l_cadena > 0 ) {
            cadena[h] = picture[h];
        }
        else {
            cadena[h] = ' ';
        }
    }
    cadena[l_picture] = 0;
    //MOSTRAR_CADENA(x,y,cadena);
}
/******************************************************************************/
void FORMAT_ENTERO( char *picture, long entero, char *cadena )
/******************************************************************************/
{
    int l_cadena,l_picture,h;
    _ltoa( entero, cadena, 10 );
    l_cadena = strlen( cadena );
    l_picture = strlen( picture );
    for( h = l_picture - 1;h >= 0;h-- ) {
        if( picture[h] == '9' ) {
            if( l_cadena > 0 ) {
                cadena[h] = cadena[--l_cadena];
            }
            else {
                cadena[h] = ' ';
            }
        }
        else if( picture[h] == '0' ) {
            if( l_cadena > 0 ) {
                cadena[h] = cadena[--l_cadena];
            }
            else {
                cadena[h] = '0';
            }
        }
        else if( l_cadena > 0 ) {
            cadena[h] = picture[h];
        }
        else {
            cadena[h] = ' ';
        }
    }
    cadena[l_picture] = 0;
}

