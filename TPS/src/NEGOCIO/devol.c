#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cajon.h> 
#include <clave.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <devol.h> 
#include <imp_dev.h> 
#include <log.h> 
#include <print.h> 
#include <actions.h>
#include <stack.h> 
#include <tkt.h> 
#include <valida.h> 
#include <_cr1.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
void CR1_INGRESA_DEVOLUCIONES()
/*****************************************************************************/
{
    int _ptr,_handle,_estado;
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //  _estado = STACK_INS( sizeof( int ) );
    _estado = STACK_INS( sizeof( short ) );
    /*------------------------------ Pide importe ----------------------------*/
    SET_MEMORY_DOUBLE( __ram_importe,
                       PEDIR_FLOAT_EN_RECUADRO_CC( ST( S__DEVOLUCIONES_ ), SI, ENTEROS, DECIMALES,
                                                   SI, SI, SI, SI, SI, RAM_COD_FUNCION ) );
    if( RAM_IMPORTE > 0.0 ) {
        if( IMPRIME_DEVOLUCIONES == 3 ) {
            PRN_HABILITA_SLIP( 1, NO );
        }
        GRABAR_LOG_SUPERVISOR( "Devoluciones", USUARIO_CLAVE_ANTERIOR() , LOG_VENTAS,4 );
        _SET_ESTADO( 1 );
    }
    /*------------------------- actualiza acumuladores -----------------------*/
    if( _ESTADO == 1 ) {
        ADD_MEMORY_DOUBLE( __x_devoluciones_importe, RAM_IMPORTE );
        ADD_MEMORY_INT( __x_devoluciones_cantidad, 1 );
        ADD_MEMORY_DOUBLE( __x_tot_dinero, -( RAM_IMPORTE ) );
        _SET_ESTADO( 2 );
    }
    /*------------------------- Emite un comprobante -------------------------*/
    if( _ESTADO == 2 ) {
        if( IMPRIME_DEVOLUCIONES ) {
            TICKET_DEVOLUCIONES( RAM_IMPORTE );
        }
        if( IMPRIME_DEVOLUCIONES == 3 ) {
            PRN_INHABILITA_SLIP();
        }
        SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
        _SET_ESTADO( 3 );
    }
    /*------------------------- Valida un comprobante ------------------------*/
    if( _ESTADO == 3 ) {
        VALIDAR_DEVOLUCIONES( RAM_IMPORTE );
        _SET_ESTADO( 4 );
    }
    /*---------------- Cierra el caj¢n y recupera la pantalla ----------------*/
    CERRAR_CAJON();
    /*-------------------- Termina recupero de memoria -----------------------*/
    CLOSE_RECUPERO( _ptr, _handle );
}

/*****************************************************************************/
void PEDIR_MOTIVO_OPERACION_NEGATIVA( int *cod_solicitud, int motivo )
/*****************************************************************************/
{
    int rta = 0;
 
    switch( motivo ) {
        case _MOTIVO_ANUL_ITEM:
            rta = PEDIR_MOTIVO_EN_ANULACIONES;
            break;
        case _MOTIVO_ANUL_TICKET:
            rta = PEDIR_MOTIVO_ANULACION_TICKET;
            break;
        case _MOTIVO_DESCUENTO:
            rta = PEDIR_MOTIVO_EN_DESCUENTOS;
            break;
        case _MOTIVO_DEVOL_TOTAL:
        case _MOTIVO_DEVOL_RF:	    
        case _MOTIVO_DEVOL_COBRO:
            if( RAM_NOTA_CR ){
                rta = PEDIR_MOTIVO_EN_NOTA_DE_CREDITO;
            } else {
                rta = PEDIR_MOTIVO_EN_DEVOLUCIONES;
            }
            break;
        default:
            rta = 0;
            break;
    }

    while( rta ) {
        //En caso de la anulacion de un cobro marginal se desactiva pantalla cobros antes 
        //de la pedir los motivos
        if( RAM_ANULACION && ITEM_COD_COBRO ) {
            ExecuteAction( A_DES_COBROS_ACT_VENTAS );
        }
        SET_MEMORY_INT( __ram_motivo_devolucion, ELEGIR_MOTIVO( cod_solicitud, motivo ) );
        //RAM_MOTIVO_DEVOLUCION > 0  ---> Motivo Anulacion
        //RAM_MOTIVO_DEVOLUCION = -1 ---> No hay motivo configurado, valor por defecto
        if( RAM_MOTIVO_DEVOLUCION > 0 || RAM_MOTIVO_DEVOLUCION == -1 ) {
            if( RAM_MOTIVO_DEVOLUCION == -1 ) {
                SET_MEMORY_INT( __ram_motivo_devolucion, 0 );
            }
            SET_MEMORY_INT( __item_motivo_devolucion, RAM_MOTIVO_DEVOLUCION );
            rta = 0;
        } 
    }
}
