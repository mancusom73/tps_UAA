#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <getch.h> 
#include <pant.h> 
#include <pedir.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
static int espera;
/******************************************************************************/
void INIT_PEDIR( int x, int y, int largo )
/******************************************************************************/
{
    espera = GET_ESPERA_GETCH();
    CURSOR_ON();
    if( _PERMITIR_SCANNER || _PERMITIR_BANDA ) {
        SET_ESPERA_GETCH( 2 );
    }
}
/******************************************************************************/
void CLOSE_PEDIR()
/******************************************************************************/
{
    SET_ESPERA_GETCH( espera );
    CURSOR_OFF();
    #if defined(INVEL_W) || defined(INVEL_L)
    CLOSE_PEDIR_();
    #endif
}
/*****************************************************************************/
long BOX_PEDIR_ENTERO( int x, int y, char *titulo, int digitos, int salvar_pantalla, int c_tit )
/*****************************************************************************/
{
    int x2,l;
    long numero;
    l = digitos * 3 + 2;
    if( !x ) {
        x = 40 - ( l + 1 ) / 2;
    }
    x2 = x + l;
    numero = PEDIR_GRAN_ENTERO( x + 2, y + 1, digitos );
    return ( numero );
}
/*****************************************************************************/
void SET_TECLA_EXCEPCION( int *teclas, void ( *funcion )() )
/*****************************************************************************/
{
    _TECLAS_EXCEPCION = teclas;
    _FUNCION_EXCEPCION = funcion;
}
/*****************************************************************************/
void TESTEAR_EXCEPCION( int *k )
/*****************************************************************************/
{
    int h, ok = 0;
    static char ejecutando;
    if( !ejecutando && _TECLAS_EXCEPCION && _FUNCION_EXCEPCION ) {
        ejecutando = 1;
        for( h = 0;_TECLAS_EXCEPCION[h] && !ok;h++ ) {
            if( *k == _TECLAS_EXCEPCION[h] ) {
                ok = 1;
                _FUNCION_EXCEPCION( k );
            }
        }
        ejecutando = 0;
    }
}

