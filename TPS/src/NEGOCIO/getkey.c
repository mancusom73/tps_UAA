#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <getch.h> 
#include <omron.h> 
#include <pausa.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
//static int ctrl;
#if !defined(WIN32) && !defined(INVEL_L)
/******************************************************************************/
GETKEY()
/******************************************************************************/
{
    int tecla = 230, funcion = 0, k = 0;
    if( HARD_OMRON ) {
        #ifdef COMPILAR_OMRON
        tecla = OMRON_GETKEY();
        #endif
    }
    else {
        if( ctrl ) {
            tecla = 260;
            ctrl = 0;
        }
        else {
            while( !funcion ) {
                if( _GETKEY( 0x12 ) ) {
                    funcion = 2;
                }
                if( KBHIT() ) {
                    funcion = 1;
                }
                if( k > tecla ) {
                    funcion = 3;
                }
                k++;
            }
            if( funcion == 1 ) {
                tecla = _GETKEY( 0x10 );
                if( ( tecla & 0xFF ) == 0xE0 ) {
                    tecla = tecla / 256;
                }
                else if( ( tecla & 0xFF ) == 0x00 ) {
                    tecla = tecla / 256 + 256;
                }
                else {
                    tecla = tecla & 0xFF;
                }
            }
            if( funcion == 2 ) {
                if( ( tecla = _GETKEY( 0x12 ) ) == 260 ) {
                    PAUSA( 3 );
                    ctrl = 1;
                }
                if( tecla == 1 ) {
                    PAUSA( 3 );
                }
            }
            if( funcion == 3 ) {
                tecla = -999;
            }
        }
    }
    return ( tecla );
}
/******************************************************************************/
_GETKEY( char ah )
/******************************************************************************/
{
    union REGS regs;
    regs.h.ah = ah;
    int86( 0x16, &regs, &regs );
    return ( regs.x.ax );
}
#endif

