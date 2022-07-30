#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <ftoa.h> 
#include <round.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <bridge.h>
#include <cmemory.h>
/******************************************************************************/
void FTOA( double importe, char *cadena, int enteros, int decimales, int signo, int comas )
/******************************************************************************/
{
    char *cadena2;
    int digitos, signo2, l_cadena, l_cadena2, h, punto = 0;
	int param=0;

   param	= (int)importe;
   if( enteros < 5 && param>9999.99 && param <100000.00){
	   //correccion para el tema de enteros 4
	   enteros =5;
   }
  

    /*--------------------- convierte a cadena --------------------*/
    importe = ROUND( importe, enteros, decimales );
    cadena2 = _fcvt( importe, decimales, &digitos, &signo2 );
    l_cadena2 = strlen( cadena2 );
    /*------------- pasa a cadena final y rellena con ceros -----------------*/
    if( decimales ) {
        punto = 1;
    }
    l_cadena = enteros + decimales + punto;
    if( comas ) {
        l_cadena += (( enteros - 1 ) / 3 ) + signo2; 
    }
    if( l_cadena2 <= enteros + decimales ) {
        memcpy( &cadena[l_cadena - l_cadena2], cadena2, l_cadena2 );
        cadena[l_cadena] = 0;
        memset( cadena, '0', l_cadena - l_cadena2 );
        /*------------------------ coloca punto decimal -------------------------*/
		
		if( decimales ) {
			memmove( cadena, &cadena[1], l_cadena - decimales );
			if( MODO_NEGOCIO != RETAIL ) {
				cadena[l_cadena - decimales - 1] = ',';
			} else {
				cadena[l_cadena - decimales - 1] = '.';
			}
		}
        /*-------------------------- rellena con blancos ------------------------*/
        if( digitos < 1 ) {
            digitos = 1;
        }
        memset( cadena, ' ', l_cadena - decimales - punto - digitos );
        /*--------------------------- coloca comas ------------------------------*/
        if( comas ) {
            for( h = 1;h < 4;h++ ) {
                if( digitos > h * 3 ) {
                    //movmem( &cadena[1], cadena,
                    // l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) ) );
                    //memmove( &cadena[1], cadena,
                    //  l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) ) );
                    memmove( cadena, &cadena[1],
                             l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) ) );
					if( MODO_NEGOCIO != RETAIL ) {
						cadena[l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) )] = '.';
					} else {
                        cadena[l_cadena - decimales - 1 - punto - ( h * 3 + ( h - 1 ) )] = ',';
					}
                }
            }
        }
        /*------------------------- coloca signo --------------------------------*/
        if( signo && signo2 ) {
            for( h = 0;cadena[h] == ' '; ) {
                h++;
            }
            if( h == 0 ) {
                memset( cadena, '-', l_cadena );
            }
            else {
                cadena[h - 1] = '-';
            }
        }
    }
    else {
        memset( cadena, '-', l_cadena );
    }
}

