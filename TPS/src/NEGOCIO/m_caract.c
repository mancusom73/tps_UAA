#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <m_caract.h> 
#include <pant.h> 
#include <print.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_CARACTER( int x, int y, char caracter )
/******************************************************************************/
{
    char cadena[2] ;
	memset(cadena,0,sizeof(cadena));
    if( y < 26 ) {
        //textattr(_ATRIBUTO);
        //PUT_CHAR( x - 1, y - 1, caracter );
    }
    else {
        //&cadena[0] = caracter;
		
		_snprintf(cadena,sizeof(cadena)-1,"%c",caracter);
        PRINT( ( char )x, ( char )y, cadena );
    }
}

