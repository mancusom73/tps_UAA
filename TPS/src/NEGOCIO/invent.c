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
#include <fiscal.h> 
#include <invent.h> 
#include <modo_eje.h> 
#include <m_cadena.h> 
#include <pant.h> 
#include <print.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <actions.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if defined(INVEL_W) || defined(INVEL_L)
#endif
void IMPRIME_PIE_INVENTARIO();
/***************************************************************************/
int PEDIR_TIPO_INVENTARIO( int permite_salir, int pedir_supervisor )
/***************************************************************************/
{
    int rta = 0, seguir = 1;
    if( PERMITE_MODO_INVENTARIO ) {
        if( !RAM_TIPO_INVENTARIO ) {
			while( seguir ) {
                SET_MEMORY_INT( __ram_tipo_inventario,
                                ( int )PEDIR_ENTERO_EN_RECUADRO( ST( S__TIPO_INVENTARIO_ ), SI,
                                                                 3 ) );
                if( RAM_TIPO_INVENTARIO > 0 ) {
                    if( pedir_supervisor ) {
                        if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__MODO_INVENTARIO_ ),
                                              ST( S_AUTORIZO_EL_MODO_INVENTARIO ),
                                              RAM_COD_FUNCION, SI ) ) {
                            seguir = 0;
                        }
                    }
                    else {
                        seguir = 0;
                    }
                }
                else if( permite_salir ) {
                    SET_MEMORY_INT( __ram_tipo_inventario, -1 );
                    seguir = 0;
				}else{
					/* forma de salir si presionan con 0 o vacio */
					if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__SALIR_INVENTARIO_ ),
						ST( S_SALIR_MODO_INVENTARIO ), _F_SUP_SALIR_INVENTARIO, SI ) ) {
							SETEAR_MODO_EJECUCION( OPERACION );
							SET_MEMORY_INT( __ram_tipo_inventario, 0 );
							CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL();
							seguir = 0;
#ifdef INVEL_W
							ExecuteAction( A_DESACT_MODO_INVENTARIO );
							ExecuteSAction( A_DESACT_MODO_INVENTARIO, "");
#endif
							MUESTRA_BULTOS();
							MUESTRA_SUBTOTAL(NO);
					}
				}
            }
            if( RAM_TIPO_INVENTARIO > 0 ) {
                SETEAR_MODO_EJECUCION( INVENTARIO );
            }
            else {
                SETEAR_MODO_EJECUCION( OPERACION );
                SET_MEMORY_INT( __ram_tipo_inventario, 0 );
            }
            //esto estaba comentado
            #ifdef INVEL_W
                        if( RAM_TIPO_INVENTARIO > 0 ) {
                           char mensa[5];
						   memset( mensa, 0 , sizeof(mensa));
                           _snprintf(mensa, sizeof(mensa)-1,"%3i",RAM_TIPO_INVENTARIO);
                            SETEAR_MODO_EJECUCION( INVENTARIO );
                           ExecuteAction( A_ACTIVA_MODO_INVENTARIO );                //esta linea estaba recomentada
						   ExecuteSAction( A_ACTIVA_MODO_INVENTARIO, mensa);
						}
            #endif
			if(config_tps.activarTFpermanente == 1)  //sacamos lo de ticket factura porque sino no abria DOC no fiscal
				SET_MEMORY_CHAR( __ram_ticket_factura, 0 );	
        }
		else {
            if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__SALIR_INVENTARIO_ ),
                                  ST( S_SALIR_MODO_INVENTARIO ), _F_SUP_SALIR_INVENTARIO, SI ) ) {
                SETEAR_MODO_EJECUCION( OPERACION );
                SET_MEMORY_INT( __ram_tipo_inventario, 0 );
				CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL();

                #ifdef INVEL_W
                  ExecuteAction( A_DESACT_MODO_INVENTARIO );              
                  ExecuteSAction( A_DESACT_MODO_INVENTARIO, "");
                #endif

            }
        }
        MUESTRA_BULTOS();
        MUESTRA_SUBTOTAL(NO);
    }
	return ( rta );
}
/***************************************************************************/
void FIN_INVENTARIO()
/***************************************************************************/
{
    /*-------------------------- Graba eventos -----------------------------*/
    memset( &event_ticket, 0, sizeof( event_ticket ) );
    event_ticket.bultos = RAM_TIPO_INVENTARIO;
    event_ticket.tipo_evento = 13;
	event_ticket.nro_comprobante_new = 0;
    event_ticket.nro_ticket = 0;
    GRABAR_EVENT_TICKET( NO );
    /*--------------------------- Otros seteos -----------------------------*/
    IMPRIME_PIE_INVENTARIO();
    ACTUALIZA_ID_EVENTO_NEW();
    SETEAR_MODO_EJECUCION( OPERACION );
	/*----------------------------------------------------------------------*/
    //salvador: Cuando se totaliza en Pant. Venta estando en Modo Inventario
	//se requiere que solicite nuevamente el Tipo de Inventario. Creo que por esa razon
	//se seteo la variable ram_tipo_inventario = 0 a esta altura del codigo a pesar de que 
	//la funcion PEDIR_TIPO_INVENTARIO tambien lo hace desactivando el modo y seteando el
	//tamaño original del total (el tamaño del total se modifica en modo inventario, 
	//se divide por 1.4). El problema es cuando ingresa a la funcion PEDIR_TIPO_INVENTARIO 
	//como ram_tipo_inventario = 0 no setea los valores originales en el tamaño de del 
	//total que visualiza en pantalla porque no se desactivaba el inventario sino que 
	//vuelve a activarse teniendo con el ultimo tamaño del total seteado. 
	//Se agrega la desactivacion del modo para tomar los tamaños originales en el total
    SET_MEMORY_INT( __ram_tipo_inventario, 0 );
	ExecuteAction( A_DESACT_MODO_INVENTARIO );              
    ExecuteSAction( A_DESACT_MODO_INVENTARIO, "");
    FIN_DE_TICKET( NO );
    PEDIR_TIPO_INVENTARIO( NO, NO );
}
/*****************************************************************************/
void IMPRIME_PIE_INVENTARIO()
/*****************************************************************************/
{
char aux[50];
memset(aux, 0, sizeof(aux));
if(IMPRESORA_FISCAL ==FISCAL)
{
	IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, " ", SI ); //LF
	_snprintf(aux, sizeof(aux)-1, "%s %04li",ST( S_TIPO_INVENTARIO__ ),RAM_TIPO_INVENTARIO);
	IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, aux, SI );
	IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, " ", SI ); //LF
	memset(aux, 0, sizeof(aux));
	_snprintf(aux, sizeof(aux)-1, "%s %04li",ST( S_TOTAL_RENGLONES__ ),RAM_RENGLONES);
	IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, aux, SI );
	IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, " ", SI ); //LF
	memset(aux, 0, sizeof(aux));
	_snprintf(aux, sizeof(aux)-1, "%s %.0f",ST( S_TOTAL_ARTICULOS__ ),RAM_CANTIDAD_TOTAL);
	IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, aux, SI );
	IMPRIMIR_TEXTO_NO_FISCAL(1, LETRA_NORMAL, " ", SI ); //LF
}
else{
	LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_TIPO_INVENTARIO__ ) );
    MOSTRAR_ENTERO( 19, PRN, "000", RAM_TIPO_INVENTARIO );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_TOTAL_RENGLONES__ ) );
    MOSTRAR_ENTERO( 19, PRN, "9999", RAM_RENGLONES );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_TOTAL_ARTICULOS__ ) );
    MOSTRAR_FLOTANTE( 19, PRN, 6, 2, NO, NO, RAM_CANTIDAD_TOTAL );
    LF( 1 );
    if( IMPRESORA_FISCAL == FISCAL_IBM_4610 && _NRO_COMANDO_INVENTARIO != -1 ) {
        CERRAR_DOCUMENTO( 0, NO, 0 );
    }
}
PRN_FIN_TICKET();

}
