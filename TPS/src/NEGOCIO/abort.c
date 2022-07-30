#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <abort.h> 
#include <cr.h> 
#include <dd.h> 
#include <log.h> 
#include <mensaje.h> 
#include <_cr0.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Funcion para abortar el programa
 * ------------------------------------------------------------------------------
 */
/***************************************************************************/
void ABORT( char *mensaje )
/***************************************************************************/
{
    char cadena[100];

	memset(cadena,0,sizeof(cadena));
    //MENSAJE( mensaje );
#if defined(INVEL_W)
	ALERTAS_SIN_CARGAR_CONF(mensaje, "Debe Bajar la Aplicacion");
#endif
	MENSAJE_SIN_SONIDO( mensaje , NO );
	_snprintf( cadena, sizeof(cadena)-1, "ABORT->%s", mensaje );
    printf( "%s\n", cadena );
    GRABAR_LOG_SISTEMA( cadena ,LOG_INCONDICIONAL,1);
    CR0_EXIT( 2 );
}

