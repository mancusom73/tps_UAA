#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <imp_piey.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <actions.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
/**************************************************************************/
void IMPRIME_PIE_Y()
/**************************************************************************/
{
    char cadena[30];

	memset(cadena, 0, sizeof(cadena));
	MOSTRAR_CADENA( 1, PRN, ST( S_CAJERO_N___ ) );
    if( IMPRIMIR_LEGAJO_CAJERO ) {
		_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
		MOSTRAR_ENTERO( 13, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
    }
    else {
        _snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
		MOSTRAR_ENTERO( 13, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
    }
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_NOMBRE_____ ) );
    MOSTRAR_CADENA( 13, PRN, datos_cajero_actual.nombre );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_DOCUMENTO__ ) );
    MOSTRAR_CADENA( 13, PRN, datos_cajero_actual.documento );
    LF( 12 );
    MOSTRAR_CADENA( 1, PRN, ST( S_FIRMA_CAJERO_________FIRMA_SUPERVISOR ) );
    LF( 1 );
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}

