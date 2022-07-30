#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <error.h> 
#include <log.h> 
#include <mensaje.h> 
#include <stringt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***************************************************************************/
void ERROR__( int nro, char *mensaje )
/***************************************************************************/
{
    char cadena[80],aux[10];
    switch( nro ) {
        case 20:
            MENSAJE( "BTRIEVE NO CARGADO" );
            break;
        default:
            /*-------- otros errores ---------*/
            strcpy( cadena, "Error BTRV " );
            _itoa( nro, aux, 10 );
            strcat( cadena, aux );
            if( mensaje ) {
                strcat( cadena, " " );
                strcat( cadena, mensaje );
            }
            // No se muestra error por file not open
            if( nro != 3 ) {
                MENSAJE( cadena );
            }
            cadena[50] = 0;
            GRABAR_LOG_BTRIEVE( cadena , LOG_ERRORES,2);
            break;
    }
}

