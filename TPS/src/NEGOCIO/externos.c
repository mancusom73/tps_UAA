#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <eventos.h> 
#include <externos.h> 
#include <log.h> 
#include <pant.h> 
#include <scanner.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***************************************************************************/
void EJECUTAR_EXTERNOS()
/***************************************************************************/
{
    CERRAR_ARCHIVOS_EVENTOS();
    GRABAR_LOG_SISTEMA( "Salida a TPV_MENU" ,LOG_VENTAS,2);
    GRABAR_LOG_MEMORIA();
    #ifdef BLINKER
    //  SWPRUNCMD("tpv_menu.bat", 0, ".", "");
    #endif
    GRABAR_LOG_SISTEMA( "Regreso de TPV_MENU" ,LOG_VENTAS,2);
    GRABAR_LOG_MEMORIA();
    ABRIR_ARCHIVOS_EVENTOS();
    INIT_FONTS();
    INIT_SCANNER();
}

