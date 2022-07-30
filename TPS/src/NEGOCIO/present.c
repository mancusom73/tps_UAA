#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <present.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
void Presentacion( char *version, char *fecha, char *branch );
#endif
/*****************************************************************************/
void CARTEL( char *version, char *branch )
/*****************************************************************************/
{
	Presentacion( version, " ", branch );
	MUESTRA_FECHA_SISTEMA();
	if( NRO_DIA_OPERATIVO )
		MUESTRA_DIA_OPERATIV0();
	ExecuteSAction( A_MUESTRA_TIT_USUARIO_PRES, ST( S_USUARIO ) );
    ExecuteSAction( A_MUESTRA_TIT_CLAVE_PRES, ST( S_CLAVE_ ) );
}
void ACTUAL_FECHA_PRESS()
{
    char cadena[10];

	memset( cadena, 0, sizeof( cadena ) );
    DTOC( GET_FECHA_BACKUP(), cadena );
    ExecuteSAction( A_MUESTRA_FECHA_PRES, cadena );
}

void MUESTRA_DIA_OPERATIV0()
{
    char cadena[10];

	memset( cadena, 0, sizeof( cadena ) );
    DTOC( NRO_DIA_OPERATIVO, cadena );
    ExecuteSAction( A_MUESTRA_FECHA_OP_VENTA, cadena );
}

void MUESTRA_FECHA_SISTEMA()
{
    char cadena[10];

	memset( cadena, 0, sizeof( cadena ) );
    DTOC( GET_FECHA_DOS(), cadena );
    ExecuteSAction( A_MUESTRA_FECHA_SISTEMA, cadena );
}
