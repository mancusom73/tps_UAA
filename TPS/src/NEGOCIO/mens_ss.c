#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <b_mens.h> 
#include <cr.h> 
#include <mensaje.h> 
#include <m_cadena.h> 
#include <tkt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
extern int _GRANDE;
extern int TIPO_DE_LETRA;
extern int ESTACION_SELECCIONADA;
extern char _ATRIBUTO;
char cadena_mensaje[200];
extern void MOSTRAR_MENSAJE2( char *s, unsigned long pPausa, int pCore );
#include <assert.h>
/******************************************************************************/
void MENSAJE_SIN_SONIDO( char *cadena, unsigned long pPausa )
/******************************************************************************/
{

	assert( strlen(cadena) < 200 );
    _snprintf( cadena_mensaje, 199, "%s", cadena );
	cadena_mensaje[199] = '\0';
    MOSTRAR_MENSAJE2( cadena_mensaje,pPausa,SI);
}
