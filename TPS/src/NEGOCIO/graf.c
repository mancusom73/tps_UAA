#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <graf.h> 
#include <modo_pro.h> 
#include <math.h>
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
#ifdef INVEL_L
#else
#endif
//static unsigned char *_MEM_VGA;
unsigned char *_MEM_VGA;
/*****************************************************************************/
void SET_MODO_GRAFICO( int modo )
/*****************************************************************************/
{
}
/*****************************************************************************/
void SET_DAC( int desde, int cant, DAC *buffer )
/*****************************************************************************/
{
}
/*****************************************************************************/
int SHOW_BMP( char *archivo, DAC *dac )
/*****************************************************************************/
{
    unsigned h,colores;
    int y, handle, ok = 0;
    struct
    {
        char sign[2];
        unsigned size;
        char res2[14];
        unsigned ancho;
        char res3[2];
        unsigned alto;
        char res4[4];
        char bits;
        char res5[25];
    } cabecera;
    unsigned char buffer[1024];
    /*------------------ Abre el archivo ----------------------*/
    #ifdef INVEL_L
    handle = open( archivo, O_RDONLY );
    #else
    handle = _open( archivo, O_RDONLY | O_BINARY );
    #endif
    if( handle ) {
        /*--------------------------- Lee la cabecera --------------------------*/
        #ifdef INVEL_L
        read( handle, &cabecera, 54 );
        #else
        _read( handle, &cabecera, 54 );
        #endif
        colores = ( unsigned )pow( 2, cabecera.bits );
        if( memcmp( cabecera.sign, "BM", 2 ) == 0 && colores == 256 ) {
            /*----------------------- Lee la tabla de colores ----------------------*/
            #ifdef  INVEL_L
            read( handle, buffer, colores * 4 );
            #else
            _read( handle, buffer, colores * 4 );
            #endif
            for( h = 0;h < colores;h++ ) {
                dac[h].azul = buffer[h * 4] >> 2;
                dac[h].verde = buffer[h * 4 + 1] >> 2;
                dac[h].rojo = buffer[h * 4 + 2] >> 2;
            }
            /*--------------------------- Lee la imgen ----------------------------*/
            for( y = cabecera.alto;y > 0;y-- ) {
                #ifdef INVEL_L
                read( handle, &_MEM_VGA[320 * y], cabecera.ancho );
                #else
                _read( handle, &_MEM_VGA[320 * y], cabecera.ancho );
                #endif
            }
            SET_DAC( 0, colores, dac );
            ok = 1;
            /*------------------------- Cierra el archivo --------------------------*/
        }
        #ifdef INVEL_L
        close( handle );
        #else
        _close( handle );
        #endif
    }
    return ( ok );
}
/*****************************************************************************/
int FADE( DAC *dac )
/*****************************************************************************/
{
    int h, ok = 0;
    for( h = 0;h < 256;h++ ) {
        if( dac[h].rojo ) {
            dac[h].rojo--;
            ok = 1;
        }
        if( dac[h].verde ) {
            dac[h].verde--;
            ok = 1;
        }
        if( dac[h].azul ) {
            dac[h].azul--;
            ok = 1;
        }
    }
    SET_DAC( 0, 256, dac );
    return ( ok );
}
/*****************************************************************************/
void CLS_GRAFICO()
/*****************************************************************************/
{
    memset( _MEM_VGA, 0, 320 * 200 );
}

