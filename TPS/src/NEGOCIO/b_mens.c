#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
//#include <tpv_dat.h>
#include <b_mens.h> 
#include <cr.h>  
#include <pant.h> 
#include <tkt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Borrar la linea de mensajes
 * ------------------------------------------------------------------------------
 */
extern int _GRANDE;
extern int TIPO_DE_LETRA;
extern int ESTACION_SELECCIONADA;
extern char BUFFER_LINEA_PRN[200];
extern int COLUMNA ;
extern char _ATRIBUTO;
/******************************************************************************/
void BORRAR_MENSAJE(void)
/******************************************************************************/
{
    BORRAR_MENSAJE_(SI);
}

