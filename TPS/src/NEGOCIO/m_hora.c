#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <htoa.h> 
#include <m_cadena.h> 
#include <m_hora.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/******************************************************************************/
void MOSTRAR_HORA( int x, int y, int hora )
/******************************************************************************/
{
    unsigned char cadena[6];
    HTOA( hora, cadena );
    MOSTRAR_CADENA( x, y, cadena );
}
/******************************************************************************/
void MOSTRAR_HORA_COMPLETA( int x, int y, long hora )
/******************************************************************************/
{
    unsigned char cadena[9];
    HTOAL( hora, cadena );
    MOSTRAR_CADENA( x, y, cadena );
}

