/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <stdlib.h>
#include <bridge.h>
#include "path.h"
#include "motor.h"
#include <cmemory.h>
#include <stack.h>
#include <clave.h>
#include <_cr1.h>
#include <imp_tick.h>
#include <anula_t.h>
#include <valida.h>
#include <cr_displ.h>
#include <descuent.h>
#include <recupero.h>
#include <_cr0.h>
extern void IMPRIME_MENSAJE_TICKET_EN_ESPERA( void );
/*****************************************************************************/
void TECLA_ANULA_TICKET( int incondicional )
/*****************************************************************************/
{
    int _ptr,_handle,_estado, inconsistencia = 0;

    if( PERMITE_ANULAR_TICKET || incondicional ) {
        /*--------------------- Prepara recupero de memoria ----------------------*/
        OPEN_RECUPERO( &_ptr, &_handle );
        _estado = STACK_INS( sizeof( short ) );
		
		if( _ESTADO == 0 ) {
			if( incondicional || SOLICITAR_CLAVES( 30, 20, NO, SI,
				SUPERVISOR_PARA_ANULAR_TICKET, ST( S_ANULACI_N_DEL_TICKET ),
					ST( S__ANULACION_DEL_TICKET_ ), RAM_COD_FUNCION, SI ) ) {
				_SET_ESTADO( 1 );
			} else
				_SET_ESTADO( 99 );
		} //else
			 //_SET_ESTADO( 99 );
		  
		if( _ESTADO == 1 ) {
			int salir = 0;
			//Falta variable en del conftpv
			/*while( ( incondicional && PEDIR_SUPERVISOR_EN_INCONSISTENC ) && !salir  ){
				salir =  SOLICITAR_CLAVES( 30, 20, NO, SI, SUPERVISOR_PARA_ANULAR_TICKET,
					ST( S_CTRL_ANULACION_POR_INC ), ST( S_CTRL_ANULACION_POR_INC ), RAM_COD_FUNCION, SI );
				BORRAR_MENSAJE( );
			 }*/
			 _SET_ESTADO( 2 );
		}
        /*--------------------------- Graba el evento ---------------------------*/
        if( _ESTADO == 2 ) {
            SETEAR_TIEMPO( _ENTRADA_OCIO );
            GRABA_EVENTO_ANULADO();
            _SET_ESTADO( 3 );
        }
        /*--------------------------- Anula el ticket ---------------------------*/
        if( _ESTADO == 3 ) {
            ADD_MEMORY_DOUBLE( __x_imp_cancelados, RAM_TOTAL );
            ADD_MEMORY_INT( __x_cant_cancelados, 1 );
            _SET_ESTADO( 4 );
        }
        /*------------------------ Anula art. reservados ------------------------*/
		if( _ESTADO == 4 ) {
			ELIMINAR_ARTICULOS_RESERVADOS( );
			_SET_ESTADO( 5 );
		}		
        /*--------------------------- Anula el ticket ---------------------------*/
        if( _ESTADO == 5 ) {
            BORRAR_ARCHIVO_REC(ARCH_REC_IMPORT);
            BORRAR_ARCHIVO_REC(ARCH_REC_ASIG_BENEFICIO);
            BORRAR_ARCHIVO_REC(ARCH_REC_ACT_MEDIO_MOTOR);
            ANULA_TICKET( NO );
            _SET_ESTADO( 6 );
        }
        /*--------------------------- Cerear Variables  ---------------------------*/
        if( _ESTADO == 99 ) {
            SET_MEMORY_INT( __ram_motivo_devolucion, NO );
			SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_CANCELADA); //se presionó la tecla de anulacion pero luego ESC en supervisor
            _SET_ESTADO( 100 );
        }
        /*-------------------- Termina recupero de memoria -----------------------*/
        CLOSE_RECUPERO( _ptr, _handle );
    }
}
/*****************************************************************************/
void ANULA_TICKET( int suspendido )
/*****************************************************************************/
{
	int ok= 0, cabecera = 1;

	if( !( RAM_ANULAR_TICKET == _ANUL_INCONSISTENCIA_DOC_CERRADO ) ) {
		//Variable que se utiliza tambien para Imprimir la cadena "TICKET ANULADO" 
		if( !suspendido && !RAM_ANULAR_TICKET ) {
			SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_TICKET_NORMAL );
		}
		if( IMPRIMIR_TICKET_AL_FINAL && MODO_NEGOCIO != SERVICIOS ) {
			if( IMPRIMIR_CABECERA_PRINCIPIO ) {
				cabecera = 0;
			}
			if( !( RAM_FACTURA && !IMPRIMIR_TICKET_SI_ES_REMOTA ) ) {
				ok = IMPRIMIR_TICKET_COMPLETO( cabecera );
				LF( 2 );
			}
		}
		if( !suspendido ) {
			//No se puede condicionar una impresora porque la configuración es IMPRESORA_FISCAL == FISCAL
			//Se tiene que poner RAM_NOTA_CR para TM220 para que imprima el ticket anulado
			if( ( RAM_NOTA_CR || !( RAM_FACTURA && !IMPRIMIR_TICKET_SI_ES_REMOTA ) ) && 
					config.pais != CHILE && config.pais != BRASIL ) {
				IMPRIME_TICKET_ANULADO( suspendido );
			}
			if( VARIOS_COPIA_TICKET_PRN ) {
				if( SINCRONIZAR_PRINTER() ) {
					IMPRIME_COPIA_TICKET( SI );
				}
			}
			if( RAM_ANULAR_TICKET < 100 )
				SET_MEMORY_CHAR( __ram_anular_ticket, 0 );
		} else {
			IMPRIME_TICKET_ANULADO( suspendido );
			if( VALIDACION_TICKET_EN_ESPERA ) {
				VALIDAR_T_ESPERA( VALIDACION_TICKET_EN_ESPERA );
			} else {
				IMPRIME_MENSAJE_TICKET_EN_ESPERA();
			}
		}
	}
	if( RAM_ANULAR_TICKET < 100 )
		FIN_DE_TICKET( NO );
    SET_MEMORY_CHAR( __ram_no_reprocesar, SI );
    #ifdef COMPILAR_DISPLAY
    CR1_DISPLAY_CAJA_ABIERTA();
    #endif
    ELIMINA_BASE_DTO( 5 );
    ELIMINA_BASE_DTO( 6 );
}

