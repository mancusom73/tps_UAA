#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <trim.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***********************************************************************/
void TRIM( char *cadena )
/***********************************************************************/
{
    int h;
    for( h = strlen( cadena ) - 1;h >= 0 && cadena[h] == ' ';h-- ) {
        ;
    }
	if(h > 0 && strlen( cadena ) - 1 > h )
		cadena[h + 1] = 0;

}
/***********************************************************************/
void LTRIM( char *cadena )
/***********************************************************************/
{
    unsigned int h;
    int cnt = 0;
    for( h = 0;h < strlen( cadena ) && cadena[h] == ' ';h++ ) {
        cnt++;
    }
    if( cnt ) {
        for( h = 0;h < ( strlen( cadena ) - cnt );h++ ) {
            cadena[h] = cadena[cnt + h];
        }
		if(h>0)
        cadena[h] = 0;
    }
}

