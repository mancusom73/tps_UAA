#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <alarma.h> 
#include <cr.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void ALARMA( int tipo )
/******************************************************************************/
{
    //int h;
    switch( tipo ) {
        case 1:
            /*----------- Sonido 1 ------------*/
            /*
             * sound(2000); delay(50); nosound(); delay(50);
             * sound(2000); delay(20); nosound(); delay(50);
             * sound(2000); delay(20); nosound();
             */
            BEEP(1450,70);
            break;
        case 2:
            /*----------- Sonido 2 ------------*/
            //   for( h=500; h<2000; h+=20 ) {
            // sound(h);
            //delay(5);
            //   }
            //nosound();
            break;
        case 3:
            /*----------- Sonido 3 ------------*/
            // sound(1500); delay(20); nosound(); delay(70);
            // sound(1500); delay(20); nosound(); delay(70);
            // sound(1500); delay(20); nosound();
            break;
        case 4:
            /*----------- Sonido 4 ------------*/
            /*
             * sound(1400);
             * delay(400);
             * nosound();
             * delay(150);
             * sound(1300);sleep
             * delay(400);
             * nosound();
             */
            break;
    }
}

