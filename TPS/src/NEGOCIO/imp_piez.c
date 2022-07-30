#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h> 
#include <fiscal.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <imp_piez.h> 
#include <inf00.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr0.h> 
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
void IMPRIME_PIE_Z()
/**************************************************************************/
{
    char cadena[30];

	if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
        PRN_SELECT( RECEIPT );
        FISCAL_Z();
    }
    else {
        PRN_INHABILITA_AVANCE_RECEIPT();
    }
    MOSTRAR_CADENA( 1, PRN, ST( S_FECHA_____________ ) );
    MOSTRAR_FECHA( 20, PRN, GET_FECHA_DOS() );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_HORA______________ ) );
    MOSTRAR_HORA( 20, PRN, GET_HORA_DOS() );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_SUCURSAL__________ ) );
    MOSTRAR_ENTERO( 20, PRN, "000", config.sucursal );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CAJA______________ ) );
    MOSTRAR_ENTERO( 20, PRN, "0000", NUMERO_CAJA() );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_CAJERO____________ ) );
	memset(cadena, 0, sizeof(cadena));
    if( IMPRIMIR_LEGAJO_CAJERO ) {		
		_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
		MOSTRAR_ENTERO( 20, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
    }
    else {
        _snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
		MOSTRAR_ENTERO( 20, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
    }
    LF( 1 );
    if( IMPRESORA_FISCAL ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_ULTIMA_BOLETA_____ ) );
    }
    else {
        MOSTRAR_CADENA( 1, PRN, ST( S_ULTIMO_TICKET_____ ) );
    }
    MOSTRAR_ENTERO( 20, PRN, "0000", NRO_TICKET );
    LF( 1 );
    if( UTILIZAR_DIA_OPERATIVO ) {
        MOSTRAR_CADENA( 1, PRN, ST( S_D_A_OPERATIVO_____ ) );
        MOSTRAR_FECHA( 20, PRN, NRO_DIA_OPERATIVO );
        LF( 1 );
    }
    LF( 3 );
    PRN_HABILITA_AVANCES();
    /*if( IMPRESORA_FISCAL == FISCAL ) {
        FISCAL_Z();
    }*/
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_PRINT, NULL, 0 );
    #endif
}

