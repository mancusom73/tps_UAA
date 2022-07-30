#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <mstring.h> 
#include <name_mon.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void NAME_MON( unsigned fecha, char *cadena, int minusculas )
/******************************************************************************/
{
    char *meses[] = {"ENERO","FEBRERO","MARZO","ABRIL","MAYO","JUNIO","JULIO","AGOSTO","SEPTIEMBRE",
                     "OCTUBRE","NOVIEMBRE","DICIEMBRE"};
	char *meses_min[] = {"enero","febreor","marzo","abril","mayo","junio","julio","agosto","septiembre",
                     "octubre","noviembre","diciembre"};

    unsigned char mes;

    mes = ( ( fecha & 0x01E0 ) >> 5 );
    if( mes ) {
		if( minusculas ) {
			strcpy( cadena, meses_min[mes - 1] );
		} else {
			strcpy( cadena, meses[mes - 1] );
		}
    }
    else {
        MENSAJE_STRING( S_FECHA_INVALIDA );
    }
}

