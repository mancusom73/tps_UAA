#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <archivos.h> 
#include <cio.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <e_mens.h> 
#include <getch.h> 
#include <mensaje.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <tkt.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
#else
#endif
struct _mensajes
{   
	BINARY status;
    char cod_mensaje; 
    char nombre[11];
    char mensaje[61];
	BINARY fin;
};
/******************************************************************************/
void EMITIR_MENSAJES()
/******************************************************************************/
{
}
/***********************************************************************/
int GRABAR_SEND( char nro )
/***********************************************************************/
{
    int rta = 0;
    return ( !rta );
}

