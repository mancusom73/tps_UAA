#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <getch.h> 
#include <imp_tick.h> 
#include <modo_eje.h> 
#include <print.h> 
#include <tkt.h> 
#include <trans.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***************************************************************************/
int PEDIR_TRANSFERENCIA()
/***************************************************************************/
{
    int rta = 0;
    if( RAM_MODO_EJECUCION == OPERACION ) {
        if( SOLICITAR_CLAVES( 30, 20, NO, SI, PEDIR_SUPERVISOR_TRANSFERENCIAS,
                              BUSCA_MENSAJE_STRING( S_MODO_TRANSFERENCIAS ),
                              ST( S_AUTORIZ__TRANSFERENCIAS ), RAM_COD_FUNCION, SI ) ) {
            if( PEDIR_DATOS_TRANSFERENCIA() ) {
                SETEAR_MODO_EJECUCION( TRANSFERENCIAS );
            }
        }
    }
    else if( RAM_MODO_EJECUCION == TRANSFERENCIAS && !RAM_RENGLONES ) {
        SETEAR_MODO_EJECUCION( OPERACION );
    }
    MUESTRA_BULTOS();
    MUESTRA_SUBTOTAL(NO);
    return ( rta );
}
/***************************************************************************/
int PEDIR_DATOS_TRANSFERENCIA()
/***************************************************************************/
{
    int rta = 0;
    SET_MEMORY_INT( __ram_sucursal_anterior,
                    ( int )PEDIR_ENTERO_EN_RECUADRO( ST( S_NRO_SUCURSAL ), SI, 3 ) );
    if( LASTKEY() != 27 ) {
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
void FIN_TRANSFERENCIA( double importe, int modo )
/***************************************************************************/
{
    if( IMPRIMIR_TICKET_AL_FINAL ) {
        IMPRIMIR_TICKET_COMPLETO( SI );
    }
    if( TKT_CONFIGURABLE ) {
		IMPRIME_TKT_IMPORTE_TOTAL( 1 );
    }
	IMPRIME_TOTAL( importe, RAM_BULTOS, modo );
    PRN_FIN_TICKET();
    /*-------------------------- Graba eventos -----------------------------*/
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = 49;
    event_ticket.nro_sucursal_anterior = RAM_SUCURSAL_ANTERIOR;
    GRABAR_EVENT_TICKET( NO );
    ACTUALIZA_ID_EVENTO_NEW();
    /*--------------------------- Otros seteos -----------------------------*/
    //SETEAR_MODO_EJECUCION(OPERACION);
    FIN_DE_TICKET( NO );
    PRN_SELECT( RECEIPT );
}

