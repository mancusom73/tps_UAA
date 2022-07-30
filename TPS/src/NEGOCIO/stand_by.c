#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <alarma.h> 
#include <clave.h> 
#include <consulta.h> 
#include <cr.h> 
#include <ini.h> 
#include <cr_disc2.h> 
#include <cr_displ.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <getch.h> 
#include <log.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <panel.h> 
#include <pant.h> 
#include <scanner.h> 
#include <stand_by.h> 
#include <_cr0.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <actions.h>
#include <cajon.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
extern int disable_standby_automatico;
/*****************************************************************************/
void STAND_BY( int modo, int modo_operacion )
/*****************************************************************************/
{
    int espera = 1, k, grabo_log = 0, attr, continua = 1, espera_getch;
	//char perm_scanner_ant = VARIOS_SCANNER_HABILITADO;

    espera_getch = GET_ESPERA_GETCH();
    DISABLE_STANDBY_AUTOMATICO();
    Orden = NUMERO_PANEL_CAJERO;
    PreparePanel( &pPanel, Orden );
    ShowPanel( pPanel );
    CURSOR_OFF();
    if( !modo ) {
        continua = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, SI, NO, SI, ST( S__STAND_BY_ ), NULL,
                                     RAM_COD_FUNCION, SI );
    }
    if( continua ) {
        char cad[30];
        char aux[15];
		memset(cad,0, sizeof(cad));
		memset(aux,0, sizeof(aux));
        if( USAR_LEGAJO == RUT ) {
            strcpy( aux, RAM_DOC_CAJERO );
            _snprintf(cad, sizeof(cad)-1, "CAJA: %04d - CAJERO: %s", NUMERO_CAJA(), aux );
        }
        else if( IMPRIMIR_LEGAJO_CAJERO ) {
            _snprintf(cad, sizeof(cad)-1, "CAJA: %04d - CAJERO: %08ld", NUMERO_CAJA(), NRO_LEGAJO_CAJERO );
        }
        else {
            _snprintf(cad, sizeof(cad)-1, "CAJA: %04d - CAJERO: %04d", NUMERO_CAJA(), NRO_CAJERO );
        }
        ExecuteAction( ( modo_operacion == _MODO_VENTA ? A_DES_VENTAS_ACT_STANDBY
                       : A_DES_COBROSESP_ACT_STANDBY ) );
        ExecuteSSAction( A_SET_TIT_STANDBY, 1, ST( S__CAJA_EN_ESPERA_ ), cad, NULL, " " );
        #ifdef COMPILAR_DISPLAY
        CR1_DISPLAY_CAJA_ESPERA();
        #endif
        GRABAR_LOG_SISTEMA( "Inicio STAND BY" ,LOG_VENTAS,1);
        GRABAR_EVENTO_INICIO_STAND_BY();
        ENABLE_VERIFICAR_CAMBIO_ARHIVOS();
        DEMORA_SET();
        INICIALIZAR_DEMORA_SET( NO );
        SET_ESPERA_GETCH( 1 );
		/*if( VARIOS_SCANNER_HABILITADO ) { //Deshabilito el scanner
			CR1_HABILITA_SCANNER( NO );
		}*/
		//Desabilita ventanas on top
		ExecuteAction( A_DES_WIN_ON_TOP );
        while( espera ) {
            k = GETCH();
            if( GetConfig( CONF_TOUCH_SCREEN ) ) {
                if( k != 37 ) {
                    k = 38;
                }
            }
            switch( k ) {
                case -999:
                    /*-------- testea status -------*/
                    if( CONTROLAR_CAJON ) {
                        if( !CAJON_CERRADO() ) {
                            ALARMA( 2 );
                            if( !grabo_log ) {
                                GRABAR_LOG_SISTEMA( "Apertura de caj¢n durante STAND BY",LOG_VENTAS,3 );
                                grabo_log = 1;
                            }
                            SLEEP( 200 );
                        }
                        else {
                            grabo_log = 0;
                        }
                    }
                    break;
                case 38:
                    /*-------- stand by -----------*/
                    _ATRIBUTO = 52;
                    if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 19, SI, NO, SI,
                                          ST( S__HABILITA_CAJA_ ), NULL, RAM_COD_FUNCION, SI ) ) {
                        espera = 0;
						disable_standby_automatico = SI;
                        MENSAJE_SIN_SONIDO_STRING( S_PROCESANDO_SCRIPT );
						VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS(NO);
						BORRAR_MENSAJE();
                    }
                    break;
                case '%':
                    /*---------- Consulta ------------*/
                    attr = _ATRIBUTO;
	    			/*if( !VARIOS_SCANNER_HABILITADO ) { //Habilito el scanner
						CR1_HABILITA_SCANNER( NO );
					}*/
                    CR1_CONSULTA( 1 );
					/*if( VARIOS_SCANNER_HABILITADO ) { //Deshabilito el scanner
						CR1_HABILITA_SCANNER( NO );
					}*/
                    /*envio un cambio de pantalla a touch para q se activen las teclas correctas*/
                    ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "08" );//08 es el panel de Stand By
                    _ATRIBUTO = attr;
                    CURSOR_OFF();
                    break;
                default:
                    break;
            }
        }
        DISABLE_VERIFICAR_CAMBIO_ARHIVOS();
		GRABAR_LOG_SISTEMA( "Fin STAND BY" ,LOG_VENTAS,3);
		GRABAR_EVENTO_FIN_STAND_BY();
    }
    ClearPanel( pPanel );
    SET_ESPERA_GETCH( espera_getch );
    ExecuteAction( ( modo_operacion == _MODO_VENTA ? A_DES_STANDBY_ACT_VENTAS
                   : A_DES_STANDBY_ACT_COBROSESP ) );
    #ifdef COMPILAR_DISPLAY
    CR1_DISPLAY_CAJA_ABIERTA();
    #endif
    CURSOR_ON();
    INICIALIZAR_DEMORA_SET( SI );
	/*if( perm_scanner_ant != VARIOS_SCANNER_HABILITADO ) { //Vuelvo scanner a estado original
		CR1_HABILITA_SCANNER( NO );
	}*/
}
/*****************************************************************************/
void GRABAR_EVENTO_INICIO_STAND_BY( void )
/*****************************************************************************/
{
    
	memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = 28;  //Ingreso StandBy
	event_ticket.nro_comprobante_new = 0;
    event_ticket.nro_ticket = 0;
    SETEAR_TIEMPO( _INDEFINIDO );
    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
    GRABAR_EVENT_TICKET( NO );
    ACUMULAR_TIEMPOS( );
    CEREAR_TIEMPOS( );
    SETEAR_TIEMPO( _ENTRADA_SBY );
    ACTUALIZA_ID_EVENTO_NEW();
}
/*****************************************************************************/
void GRABAR_EVENTO_FIN_STAND_BY( void )
/*****************************************************************************/
{
    
	memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.tipo_evento = 29;  //Salida StandBy
	event_ticket.nro_comprobante_new = 0;
    event_ticket.nro_ticket = 0;
    SETEAR_TIEMPO( _INDEFINIDO );
    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
    GRABAR_EVENT_TICKET( NO );
    ACUMULAR_TIEMPOS( );
    CEREAR_TIEMPOS( );
    SETEAR_TIEMPO( _ENTRADA_REG );
    ACTUALIZA_ID_EVENTO_NEW();
}

