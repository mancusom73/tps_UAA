#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <cio.h> 
#include <cr.h> 
#include <import.h> 
#include <tkt.h> 
#include <trim.h> 
#include <aarch.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***********************************************************************/
int IMPORTAR_ARCHIVO()
/***********************************************************************/
{
    char destino[51],origen[51];
    int handle, rta = 0;
    /*---- Obtengo el nombre del archivo a copiar ----*/
    handle = OPEN( _EXPORT, O_RDONLY );
    if( handle > 0 ) {
        if( _READ( handle, origen, 31, __LINE__, __FILE__ ) == 0 ) {
            if( _READ( handle, destino, 31, __LINE__, __FILE__ ) == 0 ) {
                TRIM( origen );
                TRIM( destino );
                /*------------------------ realiza la copia -------------------------*/
                if( !COPIAR_ARCHIVO( origen, destino, __LINE__, __FILE__ ) ) {
                    rta = 1;
                }
            }
        }
        /*------------ cierra import -------------*/
        _CLOSE( handle, __LINE__, __FILE__ );
    }
    return ( rta );
}

