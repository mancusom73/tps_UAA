#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <adelanto.h> 
#include <cajon.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <imp_adel.h> 
#include <log.h> 
#include <print.h> 
#include <stack.h> 
#include <tkt.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
void CR1_ADELANTOS()
/*****************************************************************************/
{
    int _ptr,_handle,_estado;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    /*------------------------------ Pide importe ----------------------------*/
    SET_MEMORY_DOUBLE( __ram_importe,
                       PEDIR_FLOAT_EN_RECUADRO_CC( ST( S__ADELANTOS_ ), SI, ENTEROS, DECIMALES, SI,
                                                   SI, SI, SI, SI, RAM_COD_FUNCION ) );
    if( RAM_IMPORTE > 0.0 ) {
        if( IMPRIME_ADELANTOS == 3 ) {
            PRN_HABILITA_SLIP( 1, NO );
        }
        GRABAR_LOG_SUPERVISOR( "Adelantos", USUARIO_CLAVE_ANTERIOR() ,LOG_VENTAS,4);
        _SET_ESTADO( 1 );
    }
    /*------------------------- actualiza acumuladores -----------------------*/
    if( _ESTADO == 1 ) {
        ADD_MEMORY_DOUBLE( __x_adelantos_importe, RAM_IMPORTE );
        ADD_MEMORY_INT( __x_adelantos_cantidad, 1 );
        ADD_MEMORY_DOUBLE( __x_tot_dinero, -( RAM_IMPORTE ) );
        _SET_ESTADO( 2 );
    }
    /*--------------------------- Emite un comprobante -----------------------*/
    if( _ESTADO == 2 ) {
        TICKET_ADELANTOS( RAM_IMPORTE );
        if( IMPRIME_ADELANTOS == 3 ) {
            PRN_INHABILITA_SLIP();
        }
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        _SET_ESTADO( 3 );
    }
    /*---------------- Cierra el caj¢n y recupera la pantalla ----------------*/
    CERRAR_CAJON();
    /*-------------------- Termina recupero de memoria -----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}

