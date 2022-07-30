/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#pragma pack(1)
#include <cr.h>
#include <cierre.h>
#include <cr_disco.h>
#include <cr_disc2.h>
#include <config.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <valida.h>
#include <pagos.h>
#undef MK_FP
#include <math.h>
#include <memory.h>
#include <string.h>
#include <btrv.h>
#include <panel.h>
#include <dbrouter.h>
#include <aarch.h>
#include <fcntl.h>
#include <db.h>
#include <deftables.h>
#include <cmemory.h>
#include <fiscal.h>
#include <mensaje.h>
#include <modo_eje.h>
#include <recup.h>
#include <imp_tick.h>
#include <cr_pant.h>
#include <scanner.h>
#include <t_espera.h>
#include <_cr1.h>
#include <rnv.h>
#include <vendedor.h>
#include <stack.h>
#include <cobros_esp.h>
#include <afinidad.h>
#include <apagado.h>
#include <nc.h>
#include <cambio.h>
#include <devoluci.h>
#include <clientes.h>
#include <_cr0.h>
#include <anula_t.h>
#include <b_mens.h>
#include <round.h>
#include <actual.h>
#include <promo.h>
#include <ram.h>
#include <clave.h>
#include <boletin.h>
#include <tarjetas.h>
#include <pant.h>
#include <m_cadena.h>
#include <llave.h>
#include <tar_mag.h>
#include <balanza.h>
#include <tomra.h>
#include <menu.h>
#include <cerear.h>
#include <redondeo.h>
#include <log.h>
#include <c_ticket.h>
#include <f_fijo.h>
#include <adelanto.h>
#include <retiros.h>
#include <devol.h>
#include <stand_by.h>
#include <cobros.h>
#include <pagxcaja.h>
#include <coop.h>
#include <comprob.h>
#include <remito.h>
#include <externos.h>
#include <invent.h>
#include <t_reproc.h>
#include <trans.h>
#include <t_pago.h>
#include <rollo.h>
#include <facrem.h>
#include <pago_tar.h>
#include <pago_mut.h>
#include <perfiles.h>
#include <descuent.h>
#include <on_line.h>
#include <import_t.h>
#include <cambio_m.h>
#include <pago_che.h>
#include <d_entry.h>
#include <recupero.h>
#include <alarma.h>
#include <pausa.h>
#include <get_hora.h>
#include <cajon.h>
#include <consulta.h>
#include <func.h>
#include <envios.h>
#include <plu.h>
#include <trim.h>
#include <display.h>
#include <itoa.h>
#include <l_valid.h>
#include <malloc.h>
#include <xyz.h>
#include <get_fech.h>
#include <tkt.h>
#include <stack.h>
#include <fecha.h>
#include <t_espera.h>
#include <ftoc.h>
#include <lista.h>
#include <pago_tc.h>
#include <motor.h>
#include <pan_pago.h>
#ifdef INVEL_W
#include <sys\types.h>
#include <io.h>
#include <dos.h>
#endif
#ifdef INVEL_L
#include <sys/types.h>
#include <sys/io.h>
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#include <actions.h>
#include <typeacti.h>
#endif
#include "ini.h"
#include "driver.h"
/*----------------- motor ----------------------------*/
#pragma pack (push, 1)
//#ifdef MODULO_PROMOCIONES
//#define PROMO_TYPECHECK
//#define PROMO_FAULT_TOLERANCE
#include <interpr.h>
#include "init_pr.h"
#include <process.h> //Prueba:Compilador

//#define PROMO_TRADUCIR
//#define PROMO_COMPILAR
#ifdef PROMO_FAULT_TOLERANCE
#include "fault.h"
#endif //PROMO_FAULT_TOLERANCE
#include "path.h"
#include "pagatodo.h"
#include "lib_req.h"
#include "lib_dreq.h"
#include "libcache.h"
//#endif //MODULO_PROMOCIONES
#pragma pack (pop)
/*----------------- motor ----------------------------*/
#define NC_ORIGEN_TICKET 1
#define NC_ORIGEN_FACT_A 2
#define NC_ORIGEN_FACT_B 3
#define NC_ORIGEN_TF            4
int l_cod_busqueda2 =4;
double difer=0.0, total_acumulado=0.0; 
static char mantener_cod_barra,salida_cierre,articulo_pesable;
static int pedir_cod_especial,presentaciones,menos,aut_exceder;
static long nro_ticket_tomra;
long cod_especial;
long item_cod_enganche = 0;
int Response;
int RestaurarPrimerPanel = 0;
extern int BLOQUEAR_DATOS;
long cod_clasificacion_compromiso_200 = 98000000;
char importando_ticket_devolucion = 0;
//int usar_tres_decimales= NO;
//extern int  muestra_cliente = 0;
//extern long nro_cliente_aux = 0L;
static int bien_uso_on = 0;
//Contadores de Articulos
extern long codigo_consecutivo;
extern char barrar_consecutivo[17];
extern double cantidad_consecutivo;
struct _cont_art cont_art;
int cod_interno_cod_barra = 0;
extern int en_importacion_de_ticket;
extern int grabar_copia_ticket;
int posicionTransaccionKinacu = 0;
struct _datos_cobro_kinacu datos_cobro_kinacu;
//extern int area_articulo;
extern int pant_activa_cliente;
extern void LOAD_PANTALLA_ENTR(int b);
extern long _secuencia_ant;
extern char imprimiendo_factura_journal;
extern void REDONDEAR_PRECIO2(void);
extern void ACTUALIZAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes, int indice );
extern int GUARDAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes );
extern void BORRAR_ARCHIVO_REC( char *archivo );
extern int CANTIDAD_REGISTROS_REC( char *archivo, int bytes );
extern int DEBE_PEDIR_PRECIO_DEVOL_Y_NC( int operacion );
int tipo_devolucion;
double cant_kinacu;
extern int cobro_on_tick;
extern int cobro_unico;
extern int recalcular_precio_neto_grupo;
void simular_promociones ();
int consulta_x_no_template =0;
extern void RECALCULAR_PRECIO_NETO_GRUPO( void );
extern int RECUPERAR_ARCHIVO_REC( char *archivo, char *estructura, int bytes, int indice );
extern int EVALUAR_MASK(int l_cod, int *l_id, int *l_cod_busqueda, int *barra, int *l_cod_pp, int *l_unidades, char *prefijo, char *mask_template, struct tipos_template *precio, struct tipos_template *peso);
extern double  RECUPERA_IMPORTE_PROMOCION( unsigned long cod_articulo, double cant_articulo,char *cod_barra, double precio_articulo );
extern int DEBE_PEDIR_PRECIO_DEVOL_Y_NC( int operacion );
extern int tipo_t_ev_cont;
extern void ACTUALIZA_ACUMULADORES_RAM( int h );
extern void BORRAR_EVENT_SUPER( long id_evento );
extern int PEDIR_DATOS_COBROS_KINACU( struct _datos_cobro *datos_cobro, int activar, int origen );
extern int ACUMULA_RTAS( int rta,int *acumula );
int BUSCA_ULTIMO_ITEM_SIN_ENGANCHE(long* cod_esp,int* presentacion,char* barra);
extern void ROUND2( double* numero, int enteros, int decimales );
extern double RETORNAR_TOTAL_PRINTER( double variacion );
extern int RETORNAR_TOTAL_PRINTER_POR_IVA( double variacion );
extern int PIDE_SUBTOTAL( double *importe );
int CR1_VERIFICAR_LIMITES_FACTURA( double total, int modo );
int CONDICIONES_PARA_ESPERA(void);
extern double RETORNAR_PRECIO_IMPRESORA( double cantidad, double precio_siva );
extern void SETEA_FOCUS_TPS( );
int ANULACION_TARJETA_TECLA(int cashback);
int CantCambioMediosAProcesar = 0;
void MOSTRAR_MENSAJE_UBUNTU( int icono, char *pMsg, int segTimeout );
void MENSAJE_ID_CON_PAUSA_CONF( int id );
void MOSTRAR_SALDO_EFECTIVO();
void ENVIAR_OPERACION_OFFLINE(  );
int DESENCRIPTAR_BLOQUE();
void ECHO_TEST( );
/*----------------- motor ----------------------------*/
#pragma pack (push, 1)
//#ifdef MODULO_PROMOCIONES

pc_version promo_ver = {1,12,21,24};

/*
 * Variables Globales
 */
promo pr = NULL, gpr = NULL;
states ss = NULL, ss_bak = NULL;
recover_info state_venta = NULL, state_pago = NULL;
#ifdef PROMO_FAULT_TOLERANCE
//typedef enum {normalState, recoveredState, recoveringState} rState;
rState recState = normalState;
cashdesk_state globalCashdeskState = NULL;
#endif //PROMO_FAULT_TOLERANCE
//#endif //MODULO_PROMOCIONES
#pragma pack (pop)

#include <assert.h>

struct tipos_template
{
    int enteros;
    int decimales;
    double monto;
    int pos;
};

extern int Modificar_Alicuota;
/************************************************************/
double PRECIO_UNITARIO_PARCIAL( int h, double* cant );
/************************************************************/

/*----------------- motor ----------------------------*/
void CR1_INCOSISTENCIA_FISCAL( int sumar, int origen, int t_espera, int cerear );
void CR1_TICKET_FACTURA( void );
int CARGAR_MONEDERO_ELECTRONICO( int origen_promo, char *mensaje );
/*****************************************************************************/
int CR1()
/*****************************************************************************/
{
    //char strArch[20];+

    if( !RAM_MODO_EJECUCION || RAM_MODO_EJECUCION == CAMBIO_MEDIO ) {
        SETEAR_MODO_EJECUCION( OPERACION );
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
        ExecuteAction( A_ACTIVAR_PANEL_COBROS_ESPECIALES );
    }
    else {
        ExecuteAction( A_ACTIVAR_PANEL_VENTAS );
    }
    /*if( MOSTRAR_HORA_PANTALLA ) {
        ExecuteAction( A_MOSTRAR_HORA );
    }*/

    GRABAR_LOG_SISTEMA( "CRR_INICIALIZAR_RAM_NOVOL()" ,LOG_VENTAS,1); //Log Borrar
    CRR_INICIALIZAR_RAM_NOVOL();
    /*-------- Recupera la variable de modo de operacin ------------*/
    if( RAM_TIPO_INVENTARIO ) {
        SETEAR_MODO_EJECUCION( INVENTARIO );
        if( RAM_TIPO_INVENTARIO > 0 ) {
			char mensa[5];
			memset( mensa, 0 , sizeof(mensa));
            _snprintf(mensa, sizeof(mensa)-1,"%3i",RAM_TIPO_INVENTARIO);
			ExecuteSAction( A_ACTIVA_MODO_INVENTARIO, mensa);
			ExecuteAction( A_ACTIVA_MODO_INVENTARIO ); //agregado
		}
    }
    if( DOCUMENTO_ABIERTO( ) && RAM_P_TICKET ) {//Documentos Fiscales
        if( IMPRESORA_FISCAL == FISCAL && !RAM_EMPIEZA_TICKET ) {
            //ADD_MEMORY_LONG( __nro_ticket, -1L );
            COMANDO_LEER_CONTADORES_PRINTER_FISCAL( 1, SI, NULL );
            if( !IMPRIMIR_TICKET_AL_FINAL && !RAM_IMPRESION_COMPROBANTE ) {
                FISCAL_B();
            }
        }
    }
    if( IMPRESORA_FISCAL == FISCAL &&  (!RAM_P_TICKET ) )
		COMANDO_LEER_CONTADORES_PRINTER_FISCAL( 1, SI, NULL );

    // VERIFICA_EL_TOTAL();
    GRABAR_LOG_SISTEMA( "Inicializa promociones",LOG_VENTAS,1 ); //Log Borrar
    /*----------------- motor ----------------------------*/
	if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ) {
		load_promos( );
    }
    /*----------------- motor ----------------------------*/
    GRABAR_LOG_SISTEMA( "Fin init promos" ,LOG_VENTAS,2); //Log Borrar
    /*------------------------ Setea variables -----------------------*/
    if( CABECERA_INGRESO_EGRESO_CAJERO && !RAM_P_TICKET ) {
        IMPRIMIR_CAJERO_EN_SLIP( 1 );
    }
    aut_exceder = 0;
    salida_cierre = 0;
    articulo_pesable = 0;
    SET_MEMORY_DOUBLE( __ram_intereses, 0 );
    time( &hora_ingreso_cajero );
    /**/glog ("******************INICIO DEL TICKET, ESPERANDO FUNCION****************************",LOG_DEBUG,1);
	if( RAM_P_TICKET == 0) {
		if(config_tps.activarTFpermanente == 1) {
			long cli =  atol( config_tps.DniCliente );
			if( !RAM_NRO_CLIENTE ){
				SET_MEMORY_LONG( __ram_nro_cliente, cli ); 
				SET_MEMORY_CHAR( __ram_ticket_factura, 0 );
				CR1_TICKET_FACTURA( );
			}
		}
	}
    CR1_CONFECCIONA_PANTALLA();
    //GRABAR_LOG_SISTEMA( "LIMPIAR_SCANNER()" ,LOG_VENTAS,2); //Log Borrar
    LIMPIAR_SCANNER();
    GRABAR_LOG_SISTEMA( "MUESTRA_CARTEL_INICIAL" ,LOG_VENTAS,4); //Log Borrar
    MUESTRA_CARTEL_INICIAL();
	
//    GRABAR_LOG_SISTEMA( "REGENERAR_PANTALLA_TICKET()" ,LOG_VENTAS,2); //Log Borrar
    REGENERAR_PANTALLA_TICKET();
    GRABAR_LOG_SISTEMA( "MOSTRAR_T_ESPERA_POR_PANTALLA()" ,LOG_VENTAS,4); //Log Borrar
    MOSTRAR_T_ESPERA_POR_PANTALLA();
    {
        char *msg[2];

        struct _cajeros datos_cajero;
        msg[0] = (char*)malloc(255);
        msg[1] = (char*)malloc(255);

        memset( &datos_cajero,  0, sizeof( struct _cajeros ) );
		LEE_CAJERO( NRO_CAJERO, &datos_cajero );
		_snprintf(msg[0], 254, "Cajero: %s",datos_cajero.nombre);
        //sprintf(msg[1],"%s","  INGRESO AL SISTEMA");
        //MOSTRAR_MENSAJE_POP_UP("INGRESO AL SISTEMA",(char **)msg,1/*2*/);
        if( msg[0])
        free(msg[0]);
        if( msg[0])
        free(msg[1]);
    }
    if( !VARIOS_SCANNER_HABILITADO ) {
        PETICION_DE_CODIGO( 1 );
    }
    Orden = NUMERO_PANEL_PRINCIPAL;
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
    GRABAR_LOG_SISTEMA( "CLOSE_RNV_VIRTUAL();" ,LOG_VENTAS,4); //Log Borrar
    CLOSE_RNV_VIRTUAL();
    OPEN_RNV_VIRTUAL();
    if( VARIOS_COPIA_TICKET_PRN ) {
        //OPEN_COPIA_TICKET_PRN();
    }   // COPIA_TICKET_PRm
    while( !RAM_FIN_TICKET ) {
        /*----------------- motor ----------------------------*/
      if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && ( !RAM_NOTA_CR && !MODO_DEVOLUCION ) ) {
            #ifdef PROMO_FAULT_TOLERANCE
          GRABAR_LOG_SISTEMA( "MOtor guardando" ,LOG_VENTAS,4); //Log Borrar
            if( recState == recoveredState ) {
                GRABAR_LOG_SISTEMA( "MOtor guardando recoveredState" ,LOG_VENTAS,4); //Log Borrar
                recState = normalState;
            }
            else {
                #endif //PROMO_FAULT_TOLERANCE
                if( state_venta != NULL ) {
                    // en este caso ya se evaluaron las promos prepago,
                    // en cuyo caso hay que revertir el estado para poder
                    // volver a ventas con el estado en el que habia quedado.
                    GRABAR_LOG_SISTEMA( "MOtor ss = recover( state_venta, ss );" ,LOG_VENTAS,4); //Log Borrar
                    ss = recover( state_venta, ss );
                    GRABAR_LOG_SISTEMA( "MOtor state_venta = free_recover_info( state_venta );" ,LOG_VENTAS,4); //Log Borrar
                    state_venta = free_recover_info( state_venta );
                    GRABAR_LOG_SISTEMA( "MOtor state_pago = free_recover_info( state_pago );" ,LOG_VENTAS,4); //Log Borrar
                    state_pago = free_recover_info( state_pago );

                    #ifdef PROMO_FAULT_TOLERANCE
                    GRABAR_LOG_SISTEMA( "MOtor logPanelVentas();" ,LOG_VENTAS,4); //Log Borrar
                    logPanelVentas();
					_SET_MEMORY_CHAR( __ram_promociones_afinidad_usadas,0,0  );//esta varible la use para contar los descuentos/reversiones de las postpago
					if( IMPRESORA_FISCAL == FISCAL) {
						double toti_fiscal, diff;
						int rta_f = 0;
						rta_f = PIDE_SUBTOTAL( &toti_fiscal );
						//toti_fiscal = 116.98;
						SET_MEMORY_DOUBLE( __ram_puntos_esp_3, toti_fiscal);
						diff = toti_fiscal - fabs(RAM_TOTAL);
						if( fabs(diff) > 0.0045 && fabs(diff) < 0.02 )
							ADD_MEMORY_DOUBLE( __ram_total,	diff );
						
						MUESTRA_SUBTOTAL(SI);
						SET_MEMORY_DOUBLE( __ram_puntos_esp_3, 0.00);
					}
                    #endif //PROMO_FAULT_TOLERANCE
                }
                #ifdef PROMO_FAULT_TOLERANCE
            }
            #endif //PROMO_FAULT_TOLERANCE
        }
        /*----------------- motor ----------------------------*/
        GRABAR_LOG_SISTEMA( "MOtor fin guardando",LOG_VENTAS,4 ); //Log Borrar
        if( !RAM_P_TICKET ) {
            PEDIR_VENDEDOR_SI_ES_NECESARIO();
        }
		
		CR1_LAZO_PRINCIPAL();
    }
    /*----------------- motor ----------------------------*/
   if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ) {
        // Esto debe ejecutarse solo al finalizar el programa
        // >>> este es el lugar correcto para eso ???
        /**/glog( "Liberando promociones...",LOG_VENTAS,4 );
        unref_pr( pr );
        /**/glog( "promos liberadas." ,LOG_VENTAS,4);
        #ifdef PROMO_FAULT_TOLERANCE
        stopFaultToleranceEngine();
        #endif //PROMO_FAULT_TOLERANCE
    }
    /*----------------- motor ----------------------------*/
    if( CABECERA_INGRESO_EGRESO_CAJERO ) {
        IMPRIMIR_CAJERO_EN_SLIP( 0 );
    }
    ClearPanel( pPanel );
    return ( salida_cierre );
}
/*****************************************************************************/
void CR1_LAZO_PRINCIPAL()
/*****************************************************************************/
{
    int _ptr,_handle,_estado,rta;
	char grabarlog[80];
    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );

	if( RAM_ANULAR_TICKET )
      _SET_ESTADO( 500 );

    if( MODO_NEGOCIO == SERVICIOS ) {
        if( !COBROS_ESP() ) {
            //cierre
            salida_cierre = 1;
            SET_MEMORY_INT( __ram_fin_ticket, 1 );
            _SET_ESTADO( 8 );
        }
    }

	if( _ESTADO == 0 ) {
        MOSTRAR_PETICION_DE_CODIGO( pedir_cod_especial );
        if( CR1_PIDE_CODIGO() == '*' ) {
            _SET_ESTADO( 1 );
        }
        else {
            _SET_ESTADO( 2 );
        }
        if( RAM_FIN_TICKET ) {
            _SET_ESTADO( 99 );
        }
    }
    /*-------------------------- Ingresa cantidad ---------------------------*/
    if( _ESTADO == 1 ) {
        if( RAM_CANTIDAD > 9999.999 ) {
            MENSAJE_STRING( S_CANTIDAD_GRANDE );
            SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
        }
        CR1_MUESTRA_MULTIPLICACION();
        SET_MEMORY_INT( __ram_cant_bultos, ( int )( RAM_CANTIDAD ) );
        _SET_ESTADO( 0 );
    }
    /*------------------------- Ingresa un cdigo ----------------------------*/
	if( _ESTADO == 2 ) {
        // Chequea Impresora
        if( VERIFICAR_PAPER_OUT && PAPER_OUT() ) {
            _SET_ESTADO( 0 );
        }
		if( ERROR_IMPRESORA( ) ) {
            _SET_ESTADO( 0 );
			SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_ERROR_IMPRESORA );
        }
        else if( RAM_COD_FUNCION ) {
            _SET_ESTADO( 3 );
        }
        else if( RAM_PEDIR_CLIENTE_AFINIDAD ) {
            TOMAR_CLIENTE_AFINIDAD( NO);
            CEREAR_MEMORY( __ram_cod_barra );
            _SET_ESTADO( 0 );
        }
        else if( CR1_BUSCA_CODIGO(NO) && RAM_CANTIDAD != 0 ) {
			/*Cuando se levanta un ticket en espera, los articulos del ticket se agrupan
            de manera q si se tickea un art y luego se anul y se manda a espera, cuando se
            levanta esto se agrupa y qda en 0 art para este caso es un nuevo ticket*/
			if( RAM_AGREGA_ITEM_TICK_IMPORT == 1 && RAM_P_TICKET ) {
				MENSAJE_CON_PAUSA( "NO SE PUEDE AGREGAR ARTICULOS, TIENE QUE TERMINAR EL TICKET", 10 );
				_SET_ESTADO( 0 );
			} else {
                /*Se modifica el valor de la variab ram para que permita agregar art en un
                ticket sin articulos*/
                if( RAM_AGREGA_ITEM_TICK_IMPORT == 1 && !RAM_P_TICKET ) {
                    SET_MEMORY_INT( __ram_agrega_item_tick_import, 2 );
                }
				_SET_ESTADO( 4 );
				//Cobros disparados por articulos
				if( ITEM_COD_COBRO ){
					COBROS( MODO_DEVOLUCION, _MODO_AUTOMATICO );
					SET_MEMORY_INT( __item_cod_cobro, 0 );
					_SET_ESTADO( 0 );
				}
			}
        }
        else {
			_SET_ESTADO( 9 );
            if( RAM_PEDIR_NOVIOS ) {
                SET_MEMORY_CHAR( __ram_novios, 0 );
            }
        }
    }
    /*------------------------ Ejecuta una funcin ---------------------------*/
    if( _ESTADO == 3 ) {
        SET_APAGADO( OFF );
        /**/GRABAR_LOG_SISTEMA( "Entra a CR1_EJECUTA_FUNCION ..." ,LOG_VENTAS,4); //Log Borrar
        rta = CR1_EJECUTA_FUNCION( RAM_COD_FUNCION );
        /**/GRABAR_LOG_SISTEMA( "Sale de CR1_EJECUTA_FUNCION" ,LOG_VENTAS,4); //Log Borrar
        if( rta && RAM_COD_FUNCION == 65 ) {
            OPEN_RECUPERO( &_ptr, &_handle );
            //_estado = STACK_INS( sizeof( int ) );
            _estado = STACK_INS( sizeof( short ) );
        }
        SET_APAGADO( ON );
        _SET_ESTADO( 500 );
        SET_MEMORY_INT( __ram_cod_funcion, 0 );
        if( RAM_P_TICKET ) {
            SETEAR_TIEMPO( _ENTRADA_REG );
        } else {
            SETEAR_TIEMPO( _ENTRADA_OCIO );
        }
        //CLOSE_RNV_VIRTUAL(  );
        //OPEN_RNV_VIRTUAL(  );
    }
    /*---------------------------- Agrega item -------------------------------*/
    if( _ESTADO == 4 ) {
        if( TIPOS_NC() ) {
            _SET_ESTADO( 43 );
        }
    }
    if( _ESTADO == 43 ) {
        if( TIPOS_DEVOLUCIONES() ) {
            _SET_ESTADO( 44 );
        }
    }
    if( _ESTADO == 44 ) {
        if( TIPOS_CAMBIO() ) {
            _SET_ESTADO( 45 );
        } else {
            _SET_ESTADO( 9 );
        }
    }
    if( _ESTADO == 45 ) {
        if( RAM_FACTURA && !RAM_NRO_CLIENTE ) {
            if( PEDIR_NRO_CLIENTE( SI, NO ) ) {
                _SET_ESTADO( 47 );
            }
            else {
                _SET_ESTADO( 0 );
            }
        }
        else {
            _SET_ESTADO( 47 );
        }
    }
    /*
    if( _ESTADO == 46 ) {
        if( config.pais == CHILE && NC_PERMANENTE
                && RAM_NOTA_CR && RAM_TIPO_EVENTO_COMP_ORIGINAL == -1 ) {
            if( REFERENCIA_NC(  ) == 0 )
                _SET_ESTADO( 47 );
            else {
                MENSAJE_STRING( S_DEBE_IDENTIFICAR_COMPROBANTE_ORIGINAL );
                _SET_ESTADO( 0 );
            }
        } else
            _SET_ESTADO( 47 );
    }
    */
    if( _ESTADO == 47 ) {
        SETEAR_TIEMPO( _ENTRADA_REG );
   		/**/glog ("Entra _ESTADO == 47",LOG_DEBUG,4);
        if( CR1_AGREGA_ITEM( NO, _ORIGEN_TICKET, 0, 0 ) ) {
            _SET_ESTADO( 5 );
        } else {
            _SET_ESTADO( 9 );
        }
    }

    /*----------------- Motor Promociones ----------------------------*/
	if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES && ( !RAM_NOTA_CR && !MODO_DEVOLUCION ) ) {
		/**/glog( "INICIO PROMOCIONES EN CR1" ,LOG_VENTAS,2);
		if( _ESTADO == 4 || _ESTADO == 5 ) {
            if( !RAM_ANULACION ) {
                /**/char buf[256];
				double diff = 0.00;
				if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
					if ( ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ) {
						// Se realiza un Cambio de Precio

						/**/_snprintf( buf, sizeof(buf)-1, "PROMO: ---> Precio de Articulo MODIFICADO: i%li - cantidad: %lf [Promoc:%i] ...", ITEM_COD_ESPECIAL, RAM_CANTIDAD, ITEM_HABILITADO_EN_PROMOCION );
						update_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, 1, PRECIO_UNITARIO_PARCIAL( RAM_P_TICKET - 1, NULL ), ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
					} else if ( ITEM_COD_MOVIMIENTO == _CAMBIO_DE_MERCADERIA_MENOS ) {
						// Se agrega un descuento externo
                        add_external_discount( ITEM_PRECIO, pr, &ss );
					} else {
						// Se agrega un artiuclo.
						//add_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
						add_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
					}
					
				} else {

					if ( ITEM_COD_MOVIMIENTO == _DIFERENCIA_PRECIO ) {
						// Se realiza un Cambio de Precio
						update_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, 1, PRECIO_UNITARIO_PARCIAL( RAM_P_TICKET - 1, NULL ), ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
					} else if ( ITEM_COD_MOVIMIENTO == _CAMBIO_DE_MERCADERIA_MENOS ) {
						// Se agrega un descuento externo
						add_external_discount( ITEM_PRECIO, pr, &ss );
					} else if ( ( ITEM_ENVASE || ITEM_DEV_ENVASE ) && RAM_CANTIDAD < 0 ) {
						double precUnit, cant;
						// DEVOLUCION DE ENVASE
						precUnit = PRECIO_UNITARIO_PARCIAL( RAM_P_TICKET - 1, &cant );
						remove_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, -RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
						if ( cant == 0 ) {
							update_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, -RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
						}
					} else {
						// Se agrega un artiuclo.
						if( RAM_TICKET_FACTURA == 1 && IMPRESORA_FISCAL ==FISCAL && ITEM_PEDIR_PRECIO == 1){
							double dd= 0.00, total = 0.00; //veamos que tome el correcto ojo con la reversion, los negativos y envases Iinternos
							double precio, cantidad;
							precio = ITEM_PRECIO;
							cantidad = RAM_CANTIDAD;
							total = precio * cantidad ;
							dd= RETORNAR_PRECIO_IMPRESORA( cantidad, ITEM_PRECIO_SIN_IVA );
							diff = fabs( dd - fabs(total) );
							if( ITEM_PRECIO == 1.00) {
								if( fabs(total) < dd)
									diff = -diff;
							} else
								diff = 0.00;

							add_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD - diff, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
						} else
 						add_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
					}
				}
			/**/_snprintf( buf, sizeof(buf)-1, "PROMO: ---> Agregar Articulo: i%li - cantidad: %lf [Promoc:%i] ...", ITEM_COD_ESPECIAL, RAM_CANTIDAD - diff, ITEM_HABILITADO_EN_PROMOCION );
			/**/glog( buf ,LOG_MOTOR_PROMO,4);
            }
            else {
				double precUnit, cant;
                /**/char buf[256];
				/**/_snprintf( buf, sizeof(buf)-1, "PROMO: ---> Anular Articulo: i%li - cantidad: %lf [Promoc:%i] ...", ITEM_COD_ESPECIAL, RAM_CANTIDAD, ITEM_HABILITADO_EN_PROMOCION );
                /**/glog( buf,LOG_MOTOR_PROMO,4 );

				precUnit = PRECIO_UNITARIO_PARCIAL( RAM_P_TICKET - 1, &cant );

				if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
					if ( ITEM_COD_MOVIMIENTO == _CAMBIO_DE_MERCADERIA_MENOS ) {
						// Se agrega un descuento externo
						add_external_discount( precUnit, pr, &ss );
					} else {
						remove_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
						if (  cant == 0 ) {
							update_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
						}
					}
				} else {
					if ( ITEM_COD_MOVIMIENTO == _CAMBIO_DE_MERCADERIA_MENOS ) {
						// Se agrega un descuento externo
                        add_external_discount( -precUnit, pr, &ss );
					} else {
						remove_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
						if ( cant == 0 ) {
							update_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
						}
					}
				}
            }
			/**/glog( "FIN PROMOCIONES EN CR1" ,LOG_VENTAS,2);
            //MUESTRA_BULTOS(  );
            //este si recalcular
			MUESTRA_SUBTOTAL(SI);
			//RECALCULAR_PRECIO_NETO_GRUPO3(); //hmm por si necesitamos tener el mismo importe sin iva en los mismos articulos
            //MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
			/**/glog( "PROMO: LISTO." ,LOG_VENTAS,4);
		}
	} else {
		if( ( _ESTADO == 4 || _ESTADO == 5 ) &&  RAM_P_TICKET && 
			 ( RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL ) &&
			 _TICKET_ORIGEN( RAM_P_TICKET - 1) == _ORIGEN_TICKET  ){
				double importe=0.0;
				char cod_barra_prov[17];
				
				strcpy( cod_barra_prov, ITEM_COD_BARRA );
				if( ITEM_COD_ESPECIAL == 0 && BUSCA_CODIGO_BARRA( cod_barra_prov )){
					SET_MEMORY_LONG( __item_cod_especial, articulo.cod_interno );
				}
				if( ITEM_COD_ESPECIAL > 0 )
					importe = RECUPERA_IMPORTE_PROMOCION( ITEM_COD_ESPECIAL,((RAM_CANTIDAD==0.00)?1:RAM_CANTIDAD),cod_barra_prov,ITEM_PRECIO ) * -1 ;
		}
		if( RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL )
			MUESTRA_SUBTOTAL(SI);	
		else
			MUESTRA_SUBTOTAL(NO);	
	}

	/*--------------------------- Busca enganche -----------------------------*/
    if( _ESTADO == 5 ) {
        if( ITEM_COD_ENGANCHE && !( ITEM_ENVASE ) && !RAM_DIFERENCIA_PRECIO
         && ( ITEM_COD_MOVIMIENTO != _COMPROMISO_200 )
		 && ( ITEM_COD_MOVIMIENTO != _COMPROMISO_PRECIO_MAS_BAJO ) ) {

			 /*  en NCr Libre  el enganche debe ingresar el monto total del importe a devolver y calcula la cantidad igual que el item vendido con enganche*/
			if( DEBE_PEDIR_PRECIO_DEVOL_Y_NC( RAM_MODO_DEVOLUCION ) )
				SET_MEMORY_DOUBLE( __ram_cantidad, 1);
			else
				SET_MEMORY_DOUBLE( __ram_cantidad, RAM_CANTIDAD * ITEM_PRESENTACION );

            cod_interno_cod_barra = 0;
            CEREAR_MEMORY( __ram_cod_barra );
            cod_especial = ITEM_COD_ENGANCHE;
			if( CR1_BUSCA_CODIGO(SI) ) {
				//Se realiza este control para casos donde 1ro se ingresa el articulo enganche B 
				//y luego se ingresa el articulo A con enganche trayendo su enganche B otra vez.
				item_cod_enganche = cod_especial;
				//Se usa para identificar que se esta analizando un codigo enganche 
				if( TIPOS_CAMBIO() ) {
					SET_MEMORY( __ram_cod_barra, ITEM_COD_BARRA );
					SET_MEMORY( __ram_cod_barra_original, ITEM_COD_BARRA );
					_SET_ESTADO( 6 );
				} else {
					_SET_ESTADO( 7 );
				}
				item_cod_enganche = 0;
			} else {
				_SET_ESTADO( 7 );
			}
		} else {
			_SET_ESTADO( 7 );
        }
    }

    /*------------------------ Agrega item enganche --------------------------*/
    if( _ESTADO == 6 ) {
		if( TIPOS_NC()) {
			CR1_AGREGA_ITEM( SI, _ORIGEN_TICKET, 0, 0 );
			if( !RAM_DIFERENCIA_PRECIO ) {
				if( _leyo_scanner ) {
					ADD_MEMORY_INT( __x_cant_art_escaneados, 1 );
				}
				ADD_MEMORY_INT( __x_cant_articulos, 1 );
			}
			if( ITEM_ENVASE && !( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES) )
				MUESTRA_SUBTOTAL(NO);
			else
				MUESTRA_SUBTOTAL( SI);
			_SET_ESTADO( 46 );
		}
    }

    /*----------------- Motor Promociones ----------------------------*/
	if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ){
		if( _ESTADO == 46 ) {
			if(  !RAM_NOTA_CR && !MODO_DEVOLUCION) {
				if( !RAM_ANULACION ) {
				/**/char buf[256];
				/**/_snprintf( buf, sizeof(buf)-1, "PROMO: ---> Agregar Articulo: i%li - cantidad: %lf [Promoc:%i] ...", ITEM_COD_ESPECIAL, RAM_CANTIDAD, ITEM_HABILITADO_EN_PROMOCION );
				/**/glog( buf,LOG_MOTOR_PROMO,4 );
					if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
						add_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
					} else {
						add_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
					}
				}
				else {
					double precUnit, cant;
					/**/char buf[256];
					/**/_snprintf( buf, sizeof(buf)-1, "PROMO: ---> Anular Articulo: i%li - cantidad: %lf [Promoc:%i] ...", ITEM_COD_ESPECIAL, RAM_CANTIDAD, ITEM_HABILITADO_EN_PROMOCION );
					/**/glog( buf ,LOG_MOTOR_PROMO,4);
					precUnit = PRECIO_UNITARIO_PARCIAL( RAM_P_TICKET - 1, &cant );

						if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
							remove_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
							if (  cant == 0 ) {
								update_articles( ITEM_COD_ESPECIAL, NULL, ITEM_COD_PRESENTACION, RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
							}
						} else {
							remove_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, !FUNCION_PIDE_PRECIO && ITEM_PEDIR_PRECIO, pr, &ss );
							if ( cant == 0 ) {
								update_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, ITEM_COD_PRESENTACION, RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
							}
						}
				}
				//MUESTRA_BULTOS(  );
				MUESTRA_SUBTOTAL(SI);
				//MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
				/**/glog( "PROMO: LISTO",LOG_VENTAS,4);
			}else {
				if( RAM_P_TICKET && 
					( RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL || RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL ) &&
					_TICKET_ORIGEN( RAM_P_TICKET - 1) == _ORIGEN_TICKET  ){
						double importe=0.0;
						char cod_barra_prov[17];
						strcpy( cod_barra_prov, ITEM_COD_BARRA );
						if( ITEM_COD_ESPECIAL == 0 && BUSCA_CODIGO_BARRA( cod_barra_prov )){
							SET_MEMORY_LONG( __item_cod_especial, articulo.cod_interno );
						}
						if( ITEM_COD_ESPECIAL > 0 )
							importe = RECUPERA_IMPORTE_PROMOCION( ITEM_COD_ESPECIAL,((RAM_CANTIDAD==0.00)?1:RAM_CANTIDAD),cod_barra_prov,ITEM_PRECIO ) * -1 ;
				}
				MUESTRA_SUBTOTAL(NO);
			}
			_SET_ESTADO( 7 );
		}
	
	}

	/*---------------- Diferencia de Precio Automática ----------------*/
	if( _ESTADO == 7 ) {
        SETEAR_TIEMPO( _ENTRADA_REG );
		if( !MODO_DEVOLUCION && !RAM_DIFERENCIA_PRECIO && !RAM_CAMBIO ) {
			long cod_esp= 0;
			int presentacion=0;
			char barra[17];
			_snprintf( barra, sizeof(barra)-1, "%s", ITEM_COD_BARRA );
			//que pasa si tenia enganchado?? se lo intenta aplicar al enganchado
			if( BUSCA_ULTIMO_ITEM_SIN_ENGANCHE(&cod_esp,&presentacion,barra) &&
				CR1_EXISTE_ITEM_DIF_PRECIO( cod_esp, presentacion, NO,
					barra ) == -1 ) {
				 _snprintf( grabarlog, sizeof(grabarlog)-1, "DIFERENCIA DE PRECIO AUTOMATICA SOBRE COD_ART %ld  COD_BARRA %s", ITEM_COD_ESPECIAL, ITEM_COD_BARRA );
				GRABAR_LOG_SISTEMA( grabarlog,LOG_MOTOR_PROMO,4);
				DIFERENCIA_PRECIO_AUTOMATICA( ITEM_COD_ESPECIAL, ITEM_PRESENTACION, NO,
						ITEM_COD_BARRA );
                _SET_ESTADO( 8 );
			} else {
				_SET_ESTADO( 9 );
			}
		} else {
			_SET_ESTADO( 9 );
		}
    }

    /*------------------------ Agrega item Diferencia de Precio --------------------------*/
	if( _ESTADO == 8 ) {
		/*Seteo buscar_articulo_en_mostrar_item en 1 p/q' en la busqueda se posicione correctamente
		en el articulo de descarga que quiero mostrar en pantalla*/
		buscar_articulo_en_mostrar_item = 1;
		CR1_AGREGA_ITEM( NO, _ORIGEN_TICKET, 0, 0 );
		buscar_articulo_en_mostrar_item = 0;
		MUESTRA_SUBTOTAL(NO);	
        _SET_ESTADO( 9 );
    }

    /*------------------- Inicializa variables y pantalla --------------------*/
    /*Inicialización de Variables + Inicializacion de Pantalla*/
	if( _ESTADO == 4 || _ESTADO == 5 || _ESTADO == 43 || _ESTADO == 46  || _ESTADO == 9 ) {
        CR1_INICIALIZAR_VAR_TICKET( SI );
        MOSTRAR_PETICION_DE_CODIGO( pedir_cod_especial );
        LIMPIAR_RECUADRO_PETICION( 0 );
        CR1_MUESTRA_MULTIPLICACION();
        if( !VARIOS_SCANNER_HABILITADO ) {
            PETICION_DE_CODIGO( 1 );
        }
		_SET_ESTADO( 500 );
	}
	/*Inicialización de Pantalla*/
	if( _ESTADO == 500 ) {//Estado de actualizacion de pantalla
		//aqui va lo de ticket factura permanete
		if( RAM_P_TICKET == 0 && !RAM_TIPO_INVENTARIO ) {
			if(config_tps.activarTFpermanente == 1) {
				long cli =  atol( config_tps.DniCliente );
				if( !RAM_NRO_CLIENTE ){
					SET_MEMORY_LONG( __ram_nro_cliente, cli ); //aca reemplazar por el real del xml
					CR1_TICKET_FACTURA( );
				}
			}
		}

        MUESTRA_COMPROBANTE();
        MOSTRAR_CLIENTE_AFINIDAD();
        MUESTRA_ET_TOMRA();
        #ifdef COMPILAR_TECLA_MODO_CREDITO
        MUESTRA_TIPO_PRECIO();
        #endif
        MUESTRA_TIPO_DEVOLUCION();
        /**/GRABAR_LOG_SISTEMA( "MOSTRAR_MODO_CAMBIO();" ,LOG_VENTAS,5); //Log Borrar
        MOSTRAR_MODO_CAMBIO();
		MOSTRAR_ANULACION();
		MUESTRA_ERROR_EN_RED();
		//En los recuperos de las funciones tenia problemas... si salto la caja esto no se cerea..sino se cerea
		if( !RAM_P_TICKET && RAM_IMPRESION_COMPROBANTE != 0 ) {
			SET_MEMORY_INT( __ram_impresion_comprobante, 0 );
		}
    }
    SETEAR_FUNCION( 0, SI );
    if( RAM_ANULAR_TICKET ) {
        TECLA_ANULA_TICKET( SI );
		BORRAR_ARCHIVOS_COBROS_VENTA( NO);
	}
	BORRAR_MENSAJE();
    /*-------------------- Termina recupero de memoria -----------------------*/
    BORRAR_MENSAJE();
	/**/GRABAR_LOG_SISTEMA( "CLOSE_RECUPERO()" ,LOG_VENTAS,5); //Log Borrar
	CLOSE_RECUPERO( _ptr, _handle );

}

/*****************************************************************************/
void CR1_ACTIVA_ANULACION()
/*****************************************************************************/
{
    if( ANULACIONES_PERMITIDAS ) {
        _ATRIBUTO = 120;
        SET_MEMORY_INT( __ram_anulacion, 1 );
    }
}

/*****************************************************************************/
int CR1_EXISTE_ITEM( long cod_especial, int presentacion, char *cod_barra )
/*****************************************************************************/
{
    int h, existe, ok, sub = 0, por_cod_int_cod_barra = 0, cond = 0, art_desc = 0;
    float c = 0, signo;
    char cod_barra_tmp[17];
	
    memcpy( cod_barra_tmp, cod_barra, 17 );
    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
        por_cod_int_cod_barra = 1;
    }
    for( h = 0;h < RAM_P_TICKET;h++ ) {
		if( _TICKET_ORIGEN( h ) != _ORIGEN_PROMOCION 
				|| ( RAM_MODO_DEVOLUCION == _NC_TOTAL 
					||  RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL  
					||  RAM_MODO_DEVOLUCION == _NC_RF_INTERME )
				&& _TICKET_ORIGEN( h ) == _ORIGEN_PROMOCION ) {
			cond = 0;
			if( por_cod_int_cod_barra ) {
				if( _TICKET_COD_ESPECIAL( h ) == cod_especial
				&& _TICKET_PRESENTACION( h ) == presentacion
				&& ( strcmp( _TICKET_COD_BARRA( h ), cod_barra_tmp ) == 0 ) ) {
					cond = 1;
				}
			}
			else {
				if( _TICKET_COD_ESPECIAL( h ) == cod_especial
				&& _TICKET_PRESENTACION( h ) == presentacion ) {
					cond = 1;
				}
			}
   			
			if( cond ) {
				ok = 1;
			
				if( ITEM_PEDIR_PRECIO 
						&& ( 
							!FUNCION_PIDE_PRECIO && fabs( _TICKET_PRECIO( h ) - ITEM_PRECIO ) > 0.0001 
							|| ( FUNCION_PIDE_PRECIO &&  fabs( _TICKET_CANTIDAD( h ) - RAM_CANTIDAD ) > 0.0001 )
							) 
					) {
					ok = 0;
				}
				if( ( _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO 
						|| _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO_ANULADA ) ) {
				ok = 0;
				}
				if( ok ) {
					sub = h + 1;
					signo = ( float )( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 );
					c += _TICKET_CANTIDAD( h ) * signo;
				}
			}
		}
    }
	if( !art_desc ) {
		existe = ( c + 0.001 >= RAM_CANTIDAD ) ? sub : 0;
	} else {
	   existe = -1;
	}
    return ( existe );
}
/*****************************************************************************/
void FIN_DE_TICKET( int script )
/*****************************************************************************/
{
    double tot_caja = 0;
    int h, i = 0;

   	if( RAM_FACTURA && RAM_NOTA_CR )
	{
		ADD_MEMORY_DOUBLE( __x_dif_redondeo_nc, RAM_DIFERENCIA_REDONDEO_GRAV);
		ADD_MEMORY_DOUBLE( __x_dif_redondeo_iva_nc, RAM_DIFERENCIA_REDONDEO_IVA );
	}
	else if(RAM_FACTURA && !RAM_NOTA_CR && FACT_INDEP_DEL_TIPO_DE_CLIENTE)
	{
		ADD_MEMORY_DOUBLE( __x_dif_redondeo_vtas_contrib, RAM_DIFERENCIA_REDONDEO_GRAV);
		ADD_MEMORY_DOUBLE( __x_dif_redondeo_iva_contrib, RAM_DIFERENCIA_REDONDEO_IVA );
	}


    if( RAM_MODO_EJECUCION == TRANSFERENCIAS ) {
        SETEAR_MODO_EJECUCION( OPERACION );
    }

	/* verifica tickets en espera para actualizar la bandera porque pueden haber sido procesados en otra caja */
	MOSTRAR_T_ESPERA_POR_PANTALLA( );
    /*------------------------- Inicializa variables ------------------------*/
    //CLOSE_COPIA_TICKET_PRN();
	BORRAR_CONTADOR_ARTICULO( );
    //Esta funcion produce error cuando hacemos un cobro y antes de finalizarlo le damos escape.
	//quiere cerrar el voucher.prn cuando ya se encuentra cerrado.
	//Porque se comento _COPIA_VOUCHER_PRN ?
	//CLOSE_COPIA_VOUCHER_PRN();

    IMPRIMIR_COPIAS_TICKET_VOUCHERS();

    SET_MEMORY_INT( __ram_bultos_ant, RAM_BULTOS );

    SET_MEMORY_CHAR( __ram_no_reprocesar, 0 );

    //--- Si la compra no tuvo tarjeta de credito se impide reimprimir voucher
    /*if( !VAR_TARJETAS_NRO_CUPON && !VAR_TARJETAS_NRO_CUPON_INT ) {
        unlink( "voucher.prn" );
    }*/

    COPY_STRUCT( _var_tarjetas_ant, _var_tarjetas );
    COPY_STRUCT( _tot_tick_ant, _tot_tick );
	CEREAR_STRUCT( _var_tarjetas );
	VALIDAR_VOUCHER_ENCOLADOS(  );

	if(RAM_P_PAGO) {
		int h=0, haytarjeta = 0;
		for( h = 0; h < RAM_P_PAGO; h++ ) {
			if( medios[_PAGO_MODO( h )].tarjeta ) {
				haytarjeta = 1;
			}
		}
		if(haytarjeta ==1){
			glog("ENVIO OPERACIONES PENDIENTES TARJ",1,1);
			ENVIAR_OPERACION_OFFLINE(  );//funcion para que en ventas se pueda enviar una operacion offline pendiente
		}
	}

    CRR_INICIALIZAR_VAR_TICKET_RAM_NOVOL();
    BORRAR_ARCHIVOS_COBROS_VENTA( NO );
    aut_exceder = 0;
    articulo_pesable = 0;
	recalcular_precio_neto_grupo = SI;
    memset( &comp, 0, sizeof( comp ) );
    RESETEA_TOT_TICK();
    //CEREAR_VARIABLES_DE_PROMOCION();
    //falta la funcion de libertad que setea estas variables RAM_CALCULAR_RETENC_NC.
    //SET_MEMORY_INT( __ram_calcular_retenc_nc , 0);

    /*--------------------------- Prepara pantalla --------------------------*/
    GRABAR_LOG_SISTEMA( "FIN_DE_TICKET",LOG_VENTAS, 1); //Log Borrar
    CR1_CONFECCIONA_PANTALLA();
    MUESTRA_CARTEL_INICIAL();
    MUESTRA_SUBTOTAL(NO);
    MUESTRA_COMPROBANTE();
    MOSTRAR_RETENCIONES();
	presentaciones = 0;
	MOSTRAR_PETICION_DE_PRESENTACIONES( presentaciones );
    /*------------- Cerea Bandera de Cliente seleccionado en Pago -------------*/
    pant_activa_cliente = 0;
    #ifdef COMPILAR_MOZO
    MOSTRAR_MOZO();
    #endif
    if( !VARIOS_SCANNER_HABILITADO ) {
        PETICION_DE_CODIGO( 1 );
    }
	Modificar_Alicuota = 0;
    GRABAR_LOG_SISTEMA( "Confeccion de pantalla terminada." ,LOG_VENTAS,4); //Log Borrar

    /*----------------------------- Varios ----------------------------------*/
    LIMPIA_BUFFER_TARJETA( NO );

    if( script ) {
        MENSAJE_STRING_SIN_PAUSA( S_PROCESANDO_SCRIPT );
        //debido a la demora del jsincro debemos darle tiempo que actualice la vista en el servidor para los tickets

        if( event_ticket.tipo_evento_importado == 30 && event_ticket.tipo_evento == 0 )
            SLEEP(8000); //8 segundos aprox.

        VERIFICA_CAMBIO_ARCHIVOS_ACTIVOS(NO);
    }

    VALIDAR_RAM();

    /*-------------------- Controla el total de la caja ---------------------*/
    if( CONTROLAR_LIMITE_VENTA ) {
        if( VERIFICAR_MINIMO_Y_MAXIMO_EN_CAJ ) {
            char mensaje[100];
            int ver_mensaje = 0;
			for( h = 0;h < 10;h++ ) {
                /* Se modifica para que no controle tarjeta, ya que no tiene sentido */
                if( medios[h].tarjeta ) {
                    continue;
                }
                if( medios[h].moneda_extranjera == SI ) {
                    tot_caja = _X_VENTAS_IMPORTE_ME( h ) + _X_COBROS_IMPORTE_ME( h )
                             - _X_RETIROS_IMPORTE_ME( h ) + _X_FONDO_FIJO_MULTIPLE_IMP_ME( h );
							 //+ _X_CAMBIOS_IMPORTE_ME( h );
                }
                else {
                    tot_caja = _X_VENTAS_IMPORTE( h ) + _X_COBROS_IMPORTE( h )
                             - _X_RETIROS_IMPORTE( h ) + _X_FONDO_FIJO_MULTIPLE_IMPORTE( h )
                             + _X_CAMBIOS_IMPORTE( h );
                }
                if( medios[h].efectivo ) {
                    tot_caja = X_TOT_DINERO;
                }
                if( tot_caja > medios[h].maximo_en_caja && medios[h].maximo_en_caja > 0.0 ) {
					_snprintf( mensaje, sizeof(mensaje)-1, /*sizeof(mensaje)-1,*/ "EL MEDIO DE PAGO %s ESTA LLEGANDO AL LIMITE", medios[h].nombre );
                    ver_mensaje = 1;
                }
				if( tot_caja > medios[h].maximo_bloqueo && medios[h].maximo_bloqueo > 0.0 ) {
                    _snprintf(mensaje, sizeof(mensaje)-1, ST( S_EL_MEDIO_DE_PAGO__S_ESTA_EXCEDIDO ), medios[h].nombre );
                    MENSAJE( mensaje );
					ver_mensaje = 0;
					while( !SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI,
						"MEDIO DE PAGO EXCEDIDO", NULL, _F_SUP_CODIGO_0, SI ) ){
					     MENSAJE( mensaje );
					}

                }
                if( tot_caja < medios[h].minimo_en_caja ) {
                    _snprintf( mensaje, sizeof(mensaje)-1, ST( S_NO_TIENE_SUFICIENTE__S___ ), medios[h].nombre );
                    ver_mensaje = 1;
                }
                if( ver_mensaje ) {
                    ver_mensaje = 0;
                    MENSAJE( mensaje );
                }
            }
        }
        else {
            for( h = 0;h < 10;h++ ) {
                tot_caja += ( medios[h].efectivo ) ? X_TOT_DINERO
                          : _X_VENTAS_IMPORTE( h ) - _X_RETIROS_IMPORTE( h )
                          + _X_COBROS_IMPORTE( h );
            }
            if( tot_caja > config.limite2 ) {
                MENSAJE_STRING( S_TOTAL_CAJA_EXCEDIDO );
            }
        }
    }
    VERIFICAR_IMPRESORA();    ///--------------------
    CONTROLAR_TICKET_EN_ESPERA();
    INICIALIZA_BOLETIN_TICKET();
    CLOSE_RNV_VIRTUAL();
    OPEN_RNV_VIRTUAL();
	SET_MEMORY_DOUBLE( __ram_diferencia_redondeo_grav, 0 );
	SET_MEMORY_DOUBLE( __ram_diferencia_redondeo_exent, 0 );
	SET_MEMORY_DOUBLE( __ram_diferencia_redondeo_iva, 0 );

    //   PEDIR_VENDEDOR_SI_ES_NECESARIO();
}
/*****************************************************************************/
void BORRAR_CONTADOR_ARTICULO( )
/*****************************************************************************/
{
	MUESTRA_DATOS_CONTADOR_PANTALLA( NO, NO );
	BORRAR_ARCHIVO_REC( ARCH_REC_CONTADOR_ARTICULO );

}
/*****************************************************************************/
int CR1_AUTORIZA_ANULACIONES()
/*****************************************************************************/
{
    int rta = 0;
    SET_MEMORY_LONG( __ram_codigo_supervisor, 1 );
	if( !en_importacion_de_ticket ) {
        rta = SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_DE_ANULACIONES_ ),
                                ST( S_AUTORIZO_ANULACIONES ), RAM_COD_FUNCION, SI );
    }
    else {
        rta = 0;
    }
    if( rta ) {
        SET_MEMORY_INT( __ram_item_supervisado, RAM_P_TICKET - 1 );
    }
    else {
        SET_MEMORY_LONG( __ram_codigo_supervisor, 0 );
    }
    return ( rta );
}
/*****************************************************************************/
void CR1_INFORMES()
/*****************************************************************************/
{
    if( SOLICITAR_CLAVES( 29 - ancho_panel / 2, 19, NO, SI, SI, ST( S__EMISION_DE_INFORMES_ ),

                          ST( S_INFORMES ), RAM_COD_FUNCION, SI ) ) {
		if( MENU_INFORMES() ){
			GRABAR_CABECERA_EVENTO_INFORMES( NO, SI );//EL EVENTO TIPO 8 ESTA MARCADO COMO INFORMES
	}else{
			GRABA_EVENTO_SALIDA_CAJERO(  );
		}
    }
}
/*****************************************************************************/
void CR1_VALIDA_TARJETA( int mensaje, int medio_tarjeta )
/*****************************************************************************/
{
    /*
    #ifdef INVEL_W
        if( VAR_TARJETAS_STATUS )
            GUI_MUESTRA_TARJETA( T_DATOS_TARJETA_BORRAR );
    #endif
    */
	int medio= -1;

	if( medio_tarjeta > 0)
		medio = medio_tarjeta;


    SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
    if( VALIDA_TARJETA( NO, SI, VENTAS, NO, NULL, NO, medio/*NO*/ ) ) {
        if( mensaje ) {
            MENSAJE_CON_PAUSA( ST( S_TARJETA_OK ), 9 );
        }
        /*
        #ifdef INVEL_W
                GUI_MUESTRA_TARJETA(T_DATOS_TARJETA_AGREGAR);
        #endif
        */
    }
    else {
        CEREAR_STRUCT( _var_tarjetas );
    }
    CR1_MUESTRA_TARJETA();
}
/*****************************************************************************/
int CR1_CIERRE()
/*****************************************************************************/
{
    int rta = 0, continua = 1, tit = 0, existen_t_espera = 0;
	long id = 0L;
    tit = 1;
	CantCambioMediosAProcesar = 0;
    MOSTRAR_CADENA( 30 - ancho_panel / 2, 19, ST( S_CIERRE_DE_LA_CAJA_ ) );
    if( PIDE_SUPERVISOR_PARA_CIERRE ) {
        continua = ESPERAR_LLAVE( 'X' );
    }
    if( continua ) {
		
		if( config_tps.CambioMedioAutomatico && RAM_MODO_EJECUCION != INVENTARIO ) {
			id = PROXIMO_TRANSACCION_CAMBIO_MEDIO_A_PROCESAR( 0, 0, &CantCambioMediosAProcesar );
		}
		if( PEDIR_CAJERO_PARA_CIERRE ) {
            if( INGRESAR_CLAVE( 30 - ancho_panel / 2, 21, ST( S_CAJERO___ ), 1, 0,
                                ( tit ? ST( S_CIERRE_DE_CAJA ): NULL ), 0, NO, SI ) ) {
                continua = 1;
            }
            else {
                continua = 0;
            }
        }
        else {
            continua = 1;
        }
        if( continua ) {
            if( PIDE_SUPERVISOR_PARA_CIERRE ) {
                if( INGRESAR_CLAVE( 30 - ancho_panel / 2, 21, ST( S_SUPERVISOR___ ), 2, 0,
                                    ( tit ? ST( S_CIERRE_DE_CAJA ): NULL ), _F_SUP_CIERRE, NO, SI ) ) {
                    if( ESPERAR_LLAVE( 'S' ) ) {
                        if( USUARIO_PUEDE_RETIRAR( USUARIO_CLAVE_ANTERIOR() ) ) {
                            rta = 1;
                        }
                        else {
                            MENSAJE_STRING( S_CIERRE_NO_AUTORIZADO );
                        }
                    }
                }
            }
            else {
                rta = 1;
            }
        }
    }
	if( rta == 1 ) {
        if ( PEDIR_SUP_CIERRE_CON_TICKET_ESPE ){
            if ( MODALIDAD_TICKET_EN_ESPERA == 0 ) { 
                if( RAM_TICKET_EN_ESPERA ) {
                    existen_t_espera = 1;
                }
            }
            else {
				if( CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF ) >= 1 ) {
                
                    existen_t_espera = 1;
                }
            }
            if( existen_t_espera == 1 ) {
                if(!INGRESAR_CLAVE( 30 - ancho_panel / 2, 21, ST( S_SUPERVISOR___ ), 2, 0,
                    "CIERRE CON TICKETS EN ESPERA", _F_SUP_CIERRE_CON_TICKET_ESPERA, NO, SI ) ) {            
                    rta = 0;
                }
            }
        }
    }
	if( rta == 1 ) {
		if( CantCambioMediosAProcesar ) {
			if( config_tps.PedirAutorizacionEnCierreConCambioMedio ) {
				MENSAJE( "DEBE CONFIRMAR EL CAMBIO DE MEDIO PARA CONTINUAR CON EL CIERRE DE CAJERO");
				MOSTRAR_MENSAJE_UBUNTU( 0,"CONFIRMAR PARA CONTINUAR", 2 );
			}
			if( !( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI,
			config_tps.PedirAutorizacionEnCierreConCambioMedio, 
			"      CIERRE CON CAMBIO DE MEDIO",
               "AUTORIZACION CIERRE CON CAMBIO DE MEDIO", 
			_F_SUP_CIERRE_CON_CAMBIO_MEDIOS,
			SI ) ) ) {        
                   rta = 0;
				MENSAJE( "Cambio de Medio y Cierre de Cajero cancelado...");
				MOSTRAR_MENSAJE_UBUNTU( 0,"OPERACION CANCELADA", 2 );
			} else {
				int funcion = RAM_COD_FUNCION;
				SET_MEMORY_INT( __ram_cod_funcion, 127 );					
				CAMBIO_MEDIOS(SI);
				SET_MEMORY_INT( __ram_cod_funcion, funcion );
				CantCambioMediosAProcesar = 0;					
			}
			BORRAR_MENSAJE();
       }
	}

    if( rta && !GRABAR_SALIDA_CAJERO() ) {
        rta = 0;
    }
    return ( rta );
}
/*****************************************************************************/
void PETICION_DE_CODIGO( int status )
/*****************************************************************************/
{
    pedir_cod_especial = status;
    MOSTRAR_PETICION_DE_CODIGO( pedir_cod_especial );
}
/*****************************************************************************/
void SWITCH_PETICION_DE_CODIGO()
/*****************************************************************************/
{
    int val = !pedir_cod_especial;
    char aux[10];
    if( val ) {
        _itoa( FLAG_COD, aux, 10 );
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CODE_VTAS_BORRAR, aux, " ", NULL, " " );
    }
    else {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CODE_VTAS_AGREGAR, " ", " ", NULL, " " );
    }
    PETICION_DE_CODIGO( !pedir_cod_especial );
}
/*****************************************************************************/
void CR1_PETICION_DE_PRESENTACIONES()
/*****************************************************************************/
{
    if( PRESENTACIONES_HABILITADAS ) {
        if( !presentaciones ) {
            presentaciones = 1;
        }
        else {
            presentaciones = 0;
        }
    }
    MOSTRAR_PETICION_DE_PRESENTACIONES( presentaciones );
}
/*****************************************************************************/
void CR1_FACTURA( int nc, int tipo  )
/*****************************************************************************/
{
    int ok = 1;
    #ifdef COMPILAR_COMPROBANTES
    #if defined(INVEL_W) || defined(INVEL_L)
    int hay_cli_cargado = 0;
    char stringData[80];
    memset( stringData, 0, 80 );
    hay_cli_cargado = RAM_NRO_CLIENTE;
    #endif
    if( !RAM_FACTURA ) {
		int clientess = 0;
        if( nc ) {
            ok = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_NOTA_DE_CREDITO_ ),
                                   ST( S_AUTORIZO_NC ), RAM_COD_FUNCION, SI );
            if( ok && !PEDIR_CAJA_Y_SECUENCIA ) {
                ok = PEDIR_BOLETA_ANTERIOR( NO );
            }
        }
        SET_MEMORY_CHAR( __ram_cond_iva_cliente, 0 );
  	    clientess = PEDIR_NRO_CLIENTE( 1, NO );
		if ( LASTKEY()== 27)
		   clientess = 0;
       if( ok && clientess > 0 ) {
            //Seteo la condicion de iva directamente desde la tabla de clientes.
            SET_MEMORY_CHAR( __ram_cond_iva_cliente, clientes.cond_iva );
            if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
                 //tipo = FACTURA_TIPO_A = 1 ---> Credito Fiscal
                 //tipo = FACTURA_TIPO_B = 2 ---> Fact Cons Final
                 SET_MEMORY_CHAR( __ram_factura, tipo );
            } else {
				SET_MEMORY_CHAR( __ram_factura,
                    ( ES_FACTURA_A( RAM_COND_IVA_CLIENTE ) )? FACTURA_TIPO_A: FACTURA_TIPO_B );
            } 
            SET_MEMORY_CHAR( __ram_nota_cr, ( nc ) ? 1 : 0 );
			if( nc && DEBE_PEDIR_PRECIO_DEVOL_Y_NC( _NC_LIBRE )){// funcion 27 Nota da credito libre, en caso de referenciar se puede cambiar
            /*if( nc && PEDIR_PRECIO_EN_NC ) {*/
				SET_MEMORY_INT( __ram_modo_devolucion, _NC_LIBRE );
			    SET_MEMORY_CHAR( __ram_pedir_precio, 1 );
                SET_MEMORY_CHAR( __ram_negativo, 1 );
            }
        } else {
            SET_MEMORY_CHAR( __ram_bien_de_uso, 0 );
            SET_MEMORY_LONG( __ram_boleta_anterior, 0 );
            SET_MEMORY_INT( __ram_sucursal_anterior, 0 );
            SET_MEMORY_INT( __ram_caja_anterior, 0 );
            SET_MEMORY_INT( __ram_fecha_anterior, 0 );
            SET_MEMORY_CHAR( __ram_tipo_comp_anterior, 0 );
        }
    }
    else {
        SET_MEMORY_CHAR( __ram_factura, 0 );
        SET_MEMORY_CHAR( __ram_nota_cr, 0 );
        SET_MEMORY_CHAR( __ram_bien_de_uso, 0 );
		SET_MEMORY_CHAR( __ram_tipo_comp_anterior, 0 );
        if( PEDIR_PRECIO_EN_NC ) {
            SET_MEMORY_CHAR( __ram_pedir_precio, 0 );
        }
    }
    MUESTRA_COMPROBANTE();
    //MUESTRA_REFERENCIA_NC(  );
    MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
    #endif
}
/*****************************************************************************/
void CARGAR_RAM_ANTERIOR()
/*****************************************************************************/
{
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

    if( ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago, &ev_despro, &ev_dpago,
                                      &ev_cod_barra, &ev_b_promo ) ) {
		SELECT_TABLE( T_EV_TMP, TT_ORIG );
		SET_WHERE("");
		RUN_QUERY(NO);

        SET_MEMORY_INT( __ram_caja_anterior, eventos.caja );
        SET_MEMORY_INT( __ram_sucursal_anterior, eventos.sucursal );

        //SET_MEMORY_CHAR( __ram_tipo_comp_anterior, eventos. );
        SET_MEMORY_LONG( __ram_boleta_anterior, eventos.nro_comprobante_new );

        SET_MEMORY_INT( __ram_fecha_anterior, eventos.fecha );

        CERRAR_EVENTOS_TEMPORALES();
    }
}

/*****************************************************************************/
void CEREAR_VARIABLES_NC_DEVOLUCION()
/*****************************************************************************/
{
    SET_MEMORY_CHAR( __ram_factura, 0 );
    SET_MEMORY_CHAR( __ram_nota_cr, 0 );
    SET_MEMORY_CHAR( __ram_bien_de_uso, 0 );
    if( PEDIR_PRECIO_EN_NC ) {
        SET_MEMORY_CHAR( __ram_pedir_precio, 0 );
    }
    SET_MEMORY_INT( __ram_referencia_nc, 0 );
    SET_MEMORY_INT( __ram_modo_devolucion, 0 );
    SET_MEMORY_INT( __modo_devolucion, 0 );
	SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, ( char ) - 1 );
}

/*****************************************************************************/
int VERIFICA_EVENTO_NC_DEV( char tipoEvento )
/*****************************************************************************/
{
    int habilitado = 0;
    char continuar = 1, eventoTmp;
    struct _eventos eventos;
    struct _ev_cont ev_cont;
    struct _ev_medios ev_pago;
    struct _ev_despro ev_despro;
    struct _ev_dmedios ev_dpago;
    struct _ev_cod_barra ev_cod_barra;
    struct _ev_bpromo ev_b_promo;

	memset(&ev_cont, 0, sizeof( struct _ev_cont));
    if( tipoEvento <= 0 ) {
        continuar = ABRIR_EVENTOS_TEMPORALES( &eventos, &ev_cont, &ev_pago,
                                              &ev_despro, &ev_dpago, &ev_cod_barra, &ev_b_promo );
		SELECT_TABLE( T_EV_TMP, TT_ORIG );
		SET_WHERE("");
		RUN_QUERY(NO);
		eventoTmp = eventos.tipo_evento;
    }
    else {
        eventoTmp = tipoEvento;
    }

    if( continuar ) {
        switch( eventoTmp ) {
			//comprobantes habilitados para ser devueltos
            case _EVENT_TICKET :
            case _EVENT_PAGO :
            case _EVENT_FAC_A :
            case _EVENT_FAC_B :
            case _EVENT_FACTURA_A_REMITO :
            case _EVENT_FACTURA_B_REMITO :
            case _EVENT_FAC_A_REMOTA :
            case _EVENT_FAC_B_REMOTA :
            case _EVENT_REMITO_REMOTO :
            case _EVENT_TICKET_FACTURA_A :
            case _EVENT_TICKET_FACTURA_B :
                habilitado = SI;
                break;
            default :
				//comprobantes no habilitados para ser devueltos
				//case _EVENT_NC_A :
				//case _EVENT_NC_B :
				//case _EVENT_NC_A_REMOTA :
				//case _EVENT_NC_B_REMOTA :
            
            
				MENSAJE_CON_PAUSA( ST( S_TIPO_COMPROB_INVALIDO_PARA_OPERACION ), 18 );
				habilitado = NO;
				
        }
    }
    if( tipoEvento <= 0 ) {
        CERRAR_EVENTOS_TEMPORALES( );
    }
	if( habilitado == NO )
		BORRAR_MENSAJE( );
    return habilitado;
}

/*****************************************************************************/
int VERIFICA_MEDIOS_HABILITADOS_NC_DEV( )
/*****************************************************************************/
{
    int rta = 1;
    struct _ev_medios ev_pago;

    /*if( USE_DATABASE( AREA_BOTON, EVM_TMP, ( char* )&ev_pago, sizeof( struct _ev_medios ),
                        NULL, 0, 0 ) == 0 ) {*/
	if( OPEN_TABLE( T_EVM_TMP, TT_ORIG, ( char* )&ev_pago, sizeof( struct _ev_medios ) ) == 0 ) {
        SET_ORDER2( "ID_EVENTO" );
        SET_WHERE("");
        RUN_QUERY(NO);
        //SELECT( AREA_BOTON );
        while( /*!BtrvEOF()*/ !dbEOF() ) {
            if( medios[ev_pago.modo_pago - 1].hab_nc != 1 ) {
                rta = 0;
                break;
            } else if( medios[ev_pago.modo_pago - 1].tarjeta ) {
                /* para el caso de tarjetas, la habilitacion para NC, es por submedio */
                if( TARJ[ev_pago.cod_sub_medio - 1].hab_nc != 'S' ) {
                    rta = 0;
                    break;
                }
            }
            //SKIP( 1 );
            SKIP2( 1 );
        }
        //CLOSE_DATABASE( AREA_BOTON );
    }

    return rta;
}

/**
/* Inicio de todas las Notas de Credito y Devoluciones.
/* Nota de credito Libre
/* Notas de credito Referenciadas: Parcial, Intermedia y Total
/* Devolucion Libre
/* Devoluciones Referenciadas: Parcial, Intermedia y Total
/* @param tipo representa el tipo de NC o devolucion
/*****************************************************************************/
void CR1_FACTURACION_CREDITO( int operacion )
/*****************************************************************************/
{
    /* ok: Indica si se continua con la operacion */
    int ok = 0, caja_ant = 0, fecha_ant = 0, tipo_comp_ant = 0;
	int tipo_nc = 0, grabosupervisor = 0;
    long secuencia_ant = 0, nro_ticket_ant = 0;
    /* referenciada: indica si la operacion es libre o referenciada */
    char referenciada = ( operacion == _DEVOL_TOTAL || operacion == _DEVOL_PARCIAL ||
        operacion == _DEVOL_INTERME || operacion == _NC_TOTAL ||
        operacion == _NC_RF_PARCIAL || operacion == _NC_RF_INTERME );
    /* tipo: indica si la operacion es devolucion o nota de credito*/
    enum tipo {devolucion, notaCredito} tipo = ( operacion == _DEVOL_TOTAL
        || operacion == _DEVOL_PARCIAL ||  operacion == _DEVOL_LIBRE
        || operacion == _DEVOL_INTERME ) ? devolucion : notaCredito;

    /*(!RAM_P_TICKET): es verdadero por la precondicion.*/
    //para la impresora TM200 no se permite devolucion porque es un ticket con valor negativo

	if( IMPRESORA_FISCAL == FISCAL && 
		 ( operacion == _DEVOL_TOTAL || operacion == _DEVOL_PARCIAL ||
		 operacion == _DEVOL_INTERME  || operacion ==_DEVOL_LIBRE )){
			 ok = 0;
			 MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_TIPO_COMPROB_INVALIDO_PARA_OPERACION ), 18 );
			 
	} else {

		if( !RAM_FACTURA && !RAM_MODO_DEVOLUCION && !RAM_REFERENCIA_NC && !MODO_DEVOLUCION ) {
		   ok = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ( tipo == devolucion ? ST( S_DEVOLUCIONES ) : ST( S_NOTA_DE_CREDITO_ ) ),
								  ST( S_AUTORIZO_NC ), RAM_COD_FUNCION, SI );
		   if( ok == 1)
			   grabosupervisor = 1;
		}
	}
    /* Se inicializan las variables */
    CEREAR_VARIABLES_NC_DEVOLUCION();

    MUESTRA_COMPROBANTE();
    MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
    MOSTRAR_MODO_DEVOLUCION();

    /* Para todas las NC y Devoluciones Referenciadas
     * se solicitan datos para la importacion */
    if( referenciada ) {
        if( ok && PEDIR_CAJA_Y_SECUENCIA ) {
            ok = INGRESAR_CAJA_Y_SECUENCIA( &caja_ant, &secuencia_ant, &fecha_ant );
			_secuencia_ant = secuencia_ant;
			SET_MEMORY_INT( __ram_caja_anterior, caja_ant );
        }
        else if( ok ) { // Notar que vale !PEDIR_CAJA_Y_SECUENCIA
            ok = PEDIR_BOLETA_ANTERIOR( operacion );
            fecha_ant = RAM_FECHA_ANTERIOR;
            caja_ant = RAM_CAJA_ANTERIOR;
            //nro_ticket_ant = RAM_BOLETA_ANTERIOR;
            secuencia_ant = RAM_BOLETA_ANTERIOR;
            tipo_comp_ant = TIPO_EVENTO( RAM_TIPO_COMP_ANTERIOR, NO );
        }
    }

    /* Solo para las operaciones Libres de NC y Devolucion
     * se solicitan datos para tenerlos disponibles, ya que no hay que importar */
    if ( !referenciada && ok && PEDIR_DATOS_EN_DEVOLUCIONES ) {
		if( DEBE_PEDIR_BOLETA_ANTERIOR ) {
			ok = PEDIR_BOLETA_ANTERIOR( operacion );
			SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, 
					TIPO_EVENTO( RAM_TIPO_COMP_ANTERIOR, NO ) );
		}
		//Las Operaciones Libres de NC Y Devolucion no importan el evento de manera que 
		//no hace falta pedir los Datos de la Boleta anterior.
		//Se utiliza la seleccion del Tipo de Comprobante como seleccion del Comprobante que
		//voy a utilizar solo para Devoluciones Libres.
        //Solo nos interesa la fecha por ahora.
		if( ok && PEDIR_CAJA_Y_SECUENCIA ) {
			SET_MEMORY_INT( __ram_sucursal_anterior,
						    ( int )PEDIR_ENTERO_EN_RECUADRO( ST( S_SUCURSAL_ANTERIOR ), SI, 3 ) );
			ok = INGRESAR_CAJA_Y_SECUENCIA( &caja_ant, &secuencia_ant, &fecha_ant );
			_secuencia_ant = secuencia_ant;
			SET_MEMORY_INT( __ram_caja_anterior, caja_ant );
			SET_MEMORY_INT( __ram_fecha_anterior, fecha_ant );
        }
        //fecha_ant = RAM_FECHA_ANTERIOR;
    }

    /* Todas las NC y Devoluciones
     * Se verifica que la factura importada no sea demasiado vieja */
    if( ok && ( DIAS_EN_IMPORTACION_EN_DEVOLUCIO > 0 ) && fecha_ant
        && ( INC_DIA( fecha_ant, DIAS_EN_IMPORTACION_EN_DEVOLUCIO ) < GET_FECHA_BACKUP( ) ) ) {
        ok = 0; //Se aborta la NC o Devolucion
        MENSAJE("LIMITE DE FECHA EXCEDIDA");
    }

    /* Para todas las NC y Devoluciones Referenciadas
     * se efectua la importacion */
    if( ok && referenciada ) {
        /* res_online: indica si los eventos importados se obtuvieron de
         * tablas locales o remotas (online) */
        int error = 0, res_online = 1;
        /* Es necesario establecer la variable ahora */
        SET_MEMORY_INT( __ram_referencia_nc, 1 );
        if( !( error = OBTENER_EVENTOS_TEMPORALES( tipo_comp_ant /*ev_requerido*/,
                                                   caja_ant, nro_ticket_ant,
                                                   secuencia_ant, fecha_ant, 0,
                                                   _ORIGEN_DEVOLUCION, SI,
												   &res_online /*online*/, NO ) ) ) {

            ok = VERIFICA_DATOS_IMPORTACION( res_online /*remoto*/,
                                             tipo_comp_ant /*ev_requerido*/, fecha_ant,
                                             nro_ticket_ant, caja_ant,
                                             secuencia_ant );
			if( ok ) {
				if( ( operacion == _DEVOL_PARCIAL || operacion == _NC_RF_PARCIAL ) 
						&& VERIFICAR_EVENTO_EN_PROMOCION( ) ) {
				    MENSAJE_TECLA( ST( S_EVENTO_CON_PROMOCION_REALIZAR_NC_LIBRE_TOTAL_Pres_Tecla ) ); 
					ok = 0;
				}
			}
			if( ok ) {
                /* Verificaciones finales de los eventos importados */
                ok = VERIFICA_EVENTO_NC_DEV( 0 ) ; 
            }
            if ( ok && !PERMITE_MEDIO_DESHAB_EN_DEV_TOT &&
               ( operacion == _NC_TOTAL || operacion == _DEVOL_TOTAL ) ) {
                if ( !( ok = VERIFICA_MEDIOS_HABILITADOS_NC_DEV( ) ) ){
                    MENSAJE_STRING( S_PAGADO_CON_MEDIOS_NO_HAB_PARA_LA_OPERACION );
                    BORRAR_MENSAJE( );
                }
            }
            if ( ok ) {
                /* se almacenan los datos para poder volver a importar el evento si es necesario */
                GUARDAR_INFORMACION_IMPORTACION( operacion );
				/*if( !( ( operacion == _DEVOL_PARCIAL || operacion == _NC_RF_PARCIAL )
					&& IMPORTACION_CON_SELECCION_ART ) ) {
                    AGRUPAR_ART_IMPORT( NO, NO, NO, operacion );
				}*/
                IMPORTAR_ITEMS( );
                CARGAR_RAM_ANTERIOR( );
            }
        } else {
            ok = 0; //Se aborta la NC o Devolucion
        }
    }

    /* Se setean variables segun el operacion */
    if( ok ) {
        if( tipo == notaCredito ) {
            SET_MEMORY_CHAR( __ram_factura, 1 );
			SET_MEMORY_CHAR( __ram_ticket_factura, 0 );
			SET_MEMORY_CHAR( __ram_nota_cr, 3 );// solo para que muestre el flag de nota de credito al no tener cargado cliente, no se si es A o B
        } else if( tipo == devolucion ) {
            SET_MEMORY_INT( __modo_devolucion, 1 );
			if( operacion == _DEVOL_LIBRE ) {
				//Las Operaciones Libres de NC Y Devolucion no importan el evento de manera que 
				//no hace falta pedir los Datos de la Boleta anterior.
				//Se utiliza la seleccion del Tipo de Comprobante como seleccion del Comprobante que
				//voy a utilizar solo para Devoluciones Libres.
				SET_MEMORY_CHAR( __ram_tipo_comp_anterior, TIPO_EVENTO( RAM_TIPO_COMP_ANTERIOR, NO ) ); 
				SETEAR_COMPROBANTE( ( int )RAM_TIPO_COMP_ANTERIOR );
			}
        }
    }

    /* Para todas las NC
     * Se obliga a cargar cliente si no esta ya cargado alguno */
    if( ok 
			&& ( tipo == notaCredito 
			|| ( operacion == _DEVOL_LIBRE  && RAM_FACTURA ) ) ) {
		int clientess = 0, nro_cliente_ant = 0;
		nro_cliente_ant = RAM_NRO_CLIENTE_ANT;

//		clientess = VALIDAR_CAMBIO_DE_CLIENTE( nro_cliente_ant, SI, _ORIGEN_DEVOLUCION, operacion );

		clientess = VALIDAR_CAMBIO_DE_CLIENTE( nro_cliente_ant,
					(operacion == _NC_RF_INTERME||operacion == _NC_TOTAL )? NO : SI ,//LAS NOTAS DE CREDITO CON IMPORTACION YA TRAEN VALIDADO EL CLIENTE ya trae el cliente porque pedirlo de nuevo???
					_ORIGEN_DEVOLUCION,
					operacion );

		
		if( RAM_NRO_CLIENTE && clientess != 2 && !NRO_COD_CUOTA ) {
			_ELEGIR_CLIENTE();
		}
		if ( LASTKEY()== 27 )
			clientess = 0;

        if( RAM_NRO_CLIENTE || ( clientess!= -1 && clientess !=0 ) ) {
			if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
				tipo_nc = RESP_INSC;
			} else {
				tipo_nc = ( clientes.cond_iva < 3 || clientes.cond_iva == 8 )? 1 : 2;
			}
			// si es un comprobante tipo 0 o B que no detalla el iva. no podria hacerse una NC A(por el cliente). origina error
			if( tipo == notaCredito && tipo_nc == 1/*A*/ 
				&& !FACT_INDEP_DEL_TIPO_DE_CLIENTE 
				&& ( RAM_TIPO_EVENTO_COMP_ORIGINAL == _EVENT_TICKET ||
					 RAM_TIPO_EVENTO_COMP_ORIGINAL  == _EVENT_TICKET_FACTURA_B ||
					 RAM_TIPO_EVENTO_COMP_ORIGINAL  == _EVENT_FAC_B)
				){
			
					MENSAJE_ID_CON_PAUSA_CONF( S_ERROR_CLIENTE_FISCAL );
					ok = 0;
					BORRAR_MENSAJE();
			}

			SET_MEMORY_CHAR( __ram_cond_iva_cliente, clientes.cond_iva );
            MUESTRA_COMPROBANTE();
            MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
			//Verificar cliente habilitado para factura??
			if( tipo == notaCredito ) {
				SET_MEMORY_CHAR( __ram_nota_cr, tipo_nc );
				//Se pone dentro del if porque sino se desactiva el flag devolucion
				MOSTRAR_MODO_DEVOLUCION();
			}
        } else {
            ok = 0; //Se aborta la NC
			SET_MEMORY_CHAR( __ram_nota_cr, 0 );
        }
    }

    /* Para las NC y Devolucioes referenciadas Totales
     * se indica que se deben cargar los medios de pago automaticamente */
    if( ok && ( operacion == _NC_TOTAL || operacion == _DEVOL_TOTAL ) ) {
        importando_ticket_devolucion = 1;
    }

	if( ok && RAM_IMPORTACION_PEDIDOS && ( operacion == _NC_TOTAL || operacion == _NC_RF_INTERME ) ) //para la NC automatica en la anulacion por tarjeta
		importando_ticket_devolucion = 2;

    /* Para las NC y Devoluciones Libre y Referenciada Parcial */
	if( ok && DEBE_PEDIR_PRECIO_DEVOL_Y_NC( operacion )){
    /*if( ok && PEDIR_PRECIO_EN_NC && ( operacion == _NC_LIBRE || 
                                      operacion == _NC_RF_PARCIAL ||
                                      operacion == _DEVOL_LIBRE 
                                      ||operacion == _DEVOL_PARCIAL ) ) {*/
        SET_MEMORY_CHAR( __ram_pedir_precio, 1 );
        //SET_MEMORY_CHAR( __ram_negativo, 1 );
    }

    /* Para la NC y Devoluciones Referenciadas */
    if( ok && referenciada ) {
        SET_MEMORY_INT( __ram_referencia_nc, 1 );
		if( RAM_NRO_CLIENTE ){
			SET_MEMORY_CHAR( __ram_cond_iva_cliente, clientes.cond_iva );
		}
    }
    
    if( ok ) {
        SET_MEMORY_INT( __ram_modo_devolucion, operacion );
    }

    if( ok
			&& ( operacion == _DEVOL_PARCIAL || operacion == _NC_RF_PARCIAL )
			&& IMPORTACION_CON_SELECCION_ART ) {
        ok = CARGAR_ARTICULOS_IMPORT_EN_PANEL( );
        if( ok ){
            AGREGAR_ITEMS_IMPORT_EN_PANEL_AL_TICKET(  );
        }
        BORRAR_ARCHIVO_REC( ARCH_REC_IMPORT_EN_PANEL );
    }

    /* NC y Devolucion Referenciada Total e Intermedia
     * Se cargan los items del comprobante anterior automaticamente */
    if( ok && ( operacion == _NC_TOTAL || operacion == _NC_RF_INTERME
                || operacion == _DEVOL_TOTAL || operacion == _DEVOL_INTERME ) ) {
        /* Se utilizará el mismo motivo porque son operaciones similares */
        PEDIR_MOTIVO_OPERACION_NEGATIVA( NULL, _MOTIVO_DEVOL_TOTAL );
        if( AGREGAR_ITEMS_AL_TICKET( SI ) == 1 ){
			UNGETCH( 32 ); 
		}else{
			ok = 0 ;
		}
    }
																		/*-14/07/09 se agrega la _NC_RF_PARCIAL*/
    if( ok && ( operacion == _NC_LIBRE || operacion == _DEVOL_LIBRE || operacion == _DEVOL_PARCIAL 
		|| operacion == _NC_RF_PARCIAL )){
		PEDIR_MOTIVO_OPERACION_NEGATIVA( NULL, _MOTIVO_DEVOL_RF );
	}
    if( !ok ) {
		if( grabosupervisor == 1 ) {//aca debemos borrar la ev_super si no se hizo el evento
           BORRAR_EVENT_SUPER( event_super->id_evento );
     }
        /* Se borran en el caso que se este abortando */
        CEREAR_VARIABLES_NC_DEVOLUCION();
		CEREAR_PERFIL_CLIENTE( );
		SET_MEMORY_LONG( __ram_nro_cliente, 0 );
		SET_MEMORY_LONG( __ram_nro_cliente_ant, 0 );
		SET_MEMORY_INT( __nro_cod_cuota, 0 );
    }
    MUESTRA_SUBTOTAL(NO);
	MUESTRA_PERFIL();
    MUESTRA_COMPROBANTE();
    MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
    MOSTRAR_MODO_DEVOLUCION();
}

/*****************************************************************************/
/*float*/ double CR1_PEDIR_PESO( void )
/*****************************************************************************/
{
    /*float*/ double peso = 0.00;
    if( !RAM_USA_TEMPLATE ) {
		double peso1 = /* ( float )*/atof( &cadena_scanner[2] ); //controlamos que no tome 0sss al sacar de usar_cadena_scanner
        if( peso1 > 0 && usar_cadena_scanner ) {
            usar_cadena_scanner = 0;
            if( config.pais == ELSALVADOR ) {
                peso = /*( float )*/atof( &cadena_scanner[3] );
                peso = /*( float )*/ROUND( peso, 10, 3 );
                peso = /*( float )*/ROUND( peso / 100.0, 10, 3 );
                cadena_scanner[3] = 0;
                peso = /*( float )*/ROUND( peso + atof( cadena_scanner ), 10, 3 );
            }
            else {
                peso = /* ( float )*/atof( &cadena_scanner[2] );
                peso = /*( float )*/ROUND( peso, 10, 3 );
                peso = /*( float )*/ROUND( peso / 1000.0, 10, 3 );
                cadena_scanner[2] = 0;
                peso = /*( float )*/ROUND( peso + atof( cadena_scanner ), 10, 3 );
            }
        }
        else {
            #ifdef COMPILAR_BALANZA
            peso = LEER_BALANZA();
            #else
            peso = 0;
            #endif
            if( !peso ) {
                if( HABILITAR_CODIGOS_PREEMPAQUE ) {
					while(1){
						peso = /*( float )*/PEDIR_FLOAT_EN_RECUADRO( " PESO ", SI, 3, 3 );
						peso = /*( float )*/ROUND( peso, 3, 3 );
						if( peso < 0.0 ) {
							peso = 0.0;
						}
						if( peso > config.limite_peso ){
							MENSAJE("LIMITE DE PESO EXCEDIDO!");
						}else{
							break;
						}
					}
                }
            }
        }
    }
    else {
        if( RAM_USA_TEMPLATE == 2 ) {
            peso = /*( float )*/RAM_TOMADO_POR_TEMPLATE;
        }
    }
    return ( peso );
}
/*****************************************************************************/
float CR1_PEDIR_PRECIO()
/*****************************************************************************/
{
    int h, pedir = 1, pedir1 = 1, precio_manual = 0;
    double importe = 0, saldo, importe_marcado = 0;
    long nrosucursal,nroticket;
    char cod[] = {"00000000000000"};
    if( !RAM_USA_TEMPLATE ) {
        if( usar_cadena_scanner ) {
            usar_cadena_scanner = 0;
            if( ITEM_DEV_ENVASE ) {
                SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
                /*----------- Cuando el articulo viene de TOMRA -----------*/
                importe = atof( cadena_scanner ) / pow( 10, DECIMALES );
                saldo = SALDO_ENVASES();
                if( RAM_ANULACION ) {
                    /*----- Busca el articulo marcado ----*/
                    for( h = 0;h < RAM_P_TICKET;h++ ) {
                        if( _TICKET_TIPO( h ) & _CUPON_DEV_ENV ) {
                            importe_marcado = _TICKET_PRECIO( h );
                        }
                        if( _TICKET_TIPO( h ) & _DEV_ENVASE ) {
                            importe_marcado = _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h );
                        }
                    }
                    /*---- Determina si la anulacin corresponde ---*/
                    if( fabs( importe - ( RAM_CUPON_ENVASES + ( -importe_marcado ) ) ) <= .001 ) {
                        importe -= RAM_CUPON_ENVASES;
                        SET_MEMORY_DOUBLE( __ram_cupon_envases, 0 );
                    }
                    else {
                        importe = 0;
                        MENSAJE_STRING( S_ARTICULO_NO_TICKEADO );
                        SET_MEMORY_INT( __ram_anulacion, 0 );
                        pedir = 0;
                    }
                }
                else if( VERIFICAR_IMPORTE_ENVASES ) {
                    if( saldo <= 0.01 ) {
                        importe = 0;
                        MENSAJE_STRING( S_NO_QUEDAN_ENVASES_DEVOLVER );
                        pedir1 = 0;
                    }
                    else if( importe > saldo + 0.01 ) {
                        ADD_MEMORY_DOUBLE( __ram_cupon_envases, importe - saldo );
                        importe = saldo;
                        SET_MEMORY_CHAR( __item_cupon_dev_env, 1 );
                    }
                }
            }
            else {
                /*------------------ Tratamiento normal -------------------*/
                //if( !IMPRESORA_FISCAL ) {
                    importe = ROUND( atof( &cadena_scanner[3] ), 10, 2 );
                    importe = ROUND( importe / 100.0, 10, 2 );
                    cadena_scanner[3] = 0;
                //}
                importe = ROUND( importe + atof( cadena_scanner ), 10, 2 );
                if( MODO_DEVOLUCION || RAM_NOTA_CR || RAM_NEGATIVO ) {
                    importe = -importe;
                }
            }
            if( importe ) {
                pedir = 0;
            }
            else {
                if( HABILITAR_CODIGOS_PREEMPAQUE && pedir1 ) {
                    pedir = 1;
                }
                else {
                    pedir = 0;
                }
            }
        }
        else {
            if( !HABILITAR_CODIGOS_PREEMPAQUE && !RAM_PEDIR_PRECIO ) {
                if( !RAM_ANULACION ) {
                    MENSAJE_STRING( S_ITEM_PRECIO_PREFIJO_DESCONOCIDO );
                }
                pedir = 0;
            }
        }
        if( RAM_CORRECCION && !importe ) {
            SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
            importe = _TICKET_PRECIO( RAM_P_TICKET - 1 ) * _TICKET_CANTIDAD( RAM_P_TICKET - 1 );
            pedir = 0;
        }
        if( pedir ) {
            precio_manual = SI;
            importe = PEDIR_FLOAT_EN_RECUADRO( ST( S__PRECIO_ ), SI, ENTEROS, DECIMALES );
            importe = ROUND( importe, ENTEROS + 1, DECIMALES );
            if( importe < 0.0 ) {
                importe = 0.0;
            }
            if( MODO_DEVOLUCION || RAM_NOTA_CR /*|| RAM_NEGATIVO*/ ) {
                importe = -importe;
            }
        }
        /*------- Si es devolucion de envase le cambia el signo ------*/
        if( ( ( ITEM_DEV_ENVASE && !RAM_CORRECCION ) || RAM_NEGATIVO ) && importe ) {
            importe = -importe;
        }
        if( ITEM_DEV_ENVASE && importe && SOLICITAR_NROSUCURSAL_Y_TICKET ) {
            nrosucursal = PEDIR_ENTERO_EN_RECUADRO( ST( S_NRO_SUCURSAL ), NO, 3 );
            nroticket = PEDIR_ENTERO_EN_RECUADRO( ST( S_NRO_TICKET ), NO, 5 );
            if( nrosucursal > 0 && nroticket > 0 ) {
                _snprintf( cod, sizeof(cod)-1, "%s%03li00%05liV", PREFIJO_COD_DEV_ENVASES_EVENTO, nrosucursal,
                         nroticket );
                CALCULA_CRC_EAN13( cod );
                SET_MEMORY( __item_cod_barra, cod );
            }
            else {
                importe = 0;
            }
        }
	}
	else {
        if( RAM_USA_TEMPLATE == 1 ) {
            importe = RAM_TOMADO_POR_TEMPLATE;
			if( MODO_DEVOLUCION || RAM_NOTA_CR  ) {
                importe = -importe;
            }
        }
    }

    if( RAM_CAMBIO == 1 && PEDIR_IMPORTE_SIN_IVA_EN_CAMBIO
     && ( CR1_ITEM_GRAVADO() || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) ) {
        importe *= 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0;
    }
    else {
        if( precio_manual && PEDIR_PRECIO_SIN_IVA
         && ( CR1_ITEM_GRAVADO() || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) ) {
            importe *= 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0;
        }
    }
    return ( ( float )importe );
}
/*****************************************************************************/
void CR1_HABILITA_SCANNER(int clave)
/*****************************************************************************/
{
    int t = 0;
    char scanner, *titulo[] = {" HABILITAR SCANNER "," INHABILITAR SCANNER "};
    if( VARIOS_SCANNER_HABILITADO ) {
        t = 1;
    }
    if( !clave || SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, SI, titulo[t],
                          ST( S_HABILITACION_DEL_SCANNER ), RAM_COD_FUNCION, SI ) ) {
        if( VARIOS_SCANNER_HABILITADO ) {
            setData( ACTION_HABILITA_SCANNER, "false", 5 );
            scanner = 0;
        }
        else {
            setData( ACTION_HABILITA_SCANNER, "true", 4 );
            scanner = 1;
        }
        //scanner = ( VARIOS_SCANNER_HABILITADO ) ? 0 : 1;
        SET_MEMORY_CHAR( __varios_scanner_habilitado, scanner );
        SWITCH_PETICION_DE_CODIGO();
		LIMPIAR_SCANNER(  );
    }
}
/*****************************************************************************/
void CR1_INICIALIZAR_VAR_TICKET( int inicializar_tarjetas )
/*****************************************************************************/
{
    int i;
    SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
    SET_MEMORY_INT( __ram_cant_bultos, 1 );
    SET_MEMORY_INT( __ram_anulacion, 0 );
    SET_MEMORY_INT( __ram_correccion, 0 );
    SET_MEMORY_INT( __ram_present_ant, -1 );
    SET_MEMORY_INT( __ram_present_ant_bak, -1 );
    SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );
    SET_MEMORY_CHAR( __ram_pedir_novios, 0 );
    CEREAR_MEMORY( __ram_cod_barra );
    cod_especial = 0;
    pedir_cod_especial = 0;
    presentaciones = 0;
    if( MODO_DEVOLUCION || NC_PERMANENTE ) {
        SET_MEMORY_CHAR( __item_cod_movimiento, _DEVOLUCION );
    }
    else {
        SET_MEMORY_CHAR( __item_cod_movimiento, _VENTA_NORMAL );
    }
    if( inicializar_tarjetas ) {
        SET_MEMORY( __ram_tarj_numero, "" );
        SET_MEMORY_DOUBLE( __ram_tarj_importe, 0.00f );
        SET_MEMORY_DOUBLE( __ram_tarj_cuotas, 0 );
		//CLOSE_COPIA_VOUCHER_PRN(); esta causaba problemas?
    }
    SET_MEMORY_LONG( __item_comprobante, 0L );
	SET_MEMORY_INT( __item_cod_bajo, 0 );//Agrego porque no cerea la variable en la funcion CEREAR_STRUCT( _item );
    SET_MEMORY_INT( __ram_motivo_descuento, 0 );
    if( RAM_MODO_DEVOLUCION != _DEVOL_TOTAL
        && RAM_MODO_DEVOLUCION != _DEVOL_INTERME
        && RAM_MODO_DEVOLUCION != _NC_RF_INTERME
        && RAM_MODO_DEVOLUCION != _NC_TOTAL) {
        SET_MEMORY_INT( __ram_motivo_devolucion, 0 );
    }
	SET_MEMORY_CHAR( __ram_diferencia_precio, 0 );

    for( i = 0;i < 5;i++ ) {
        _SET_MEMORY_DOUBLE( __ram_acumulador, i, 0.0 );
    }
    CEREAR_MEMORY( __ram_cod_boleta_plata );
    CEREAR_MEMORY( __ram_cod_barra_original );
}
/*****************************************************************************/
int CR1_AUTORIZA_PASAR_LIMITE()
/*****************************************************************************/
{
    int rta = -1, k;
    MENSAJE_STRING( S_LIMITE_VENTA_EXCEDIDO );
    TECLADO_ALFABETICO = 1;
    while( rta == -1 ) {
        k = GETCH();
        if( k == 'S' ) {
            rta = 1;
        }
        if( k == 27 || k == 'N' ) {
            rta = 0;
        }
    }
    TECLADO_ALFABETICO = 0;
    return ( rta );
}
/*****************************************************************************/
int CR1_ELEGIR_PRESENTACION()
/*****************************************************************************/
{
    int rta = 0, iter = 0, elementos = 0, menu_ok = 1;
    #ifdef COMPILAR_PRESENTACIONES
    int k = 0;
    int columnas = 2;
    struct _menu *items;
    char nombres[9][60], tmp[7];
	struct _cabmenu *cab;

    if( PRESENTACIONES_HABILITADAS ) {
        /*----------------- Busca la primera presentacion ---------------------*/
		SELECT_TABLE( T_PRESENT, TT_ORIG );
        ind_present.autoinc = ITEM_AUTOINC;
        ind_present.cod_present = 0;
		SET_WHERE("AUTOINC = %ld and COD_PRESENT >= %i and HABILITADA = 'S'",
				ind_present.autoinc,
				ind_present.cod_present);
		SET_ORDER2("AUTOINC,COD_PRESENT");
		RUN_QUERY(NO);
		elementos = RECCOUNT2( NULL, NULL);
		items = calloc( sizeof( struct _menu ), elementos );

        /*------------------- Muestra las presentaciones ----------------------*/
		while( !dbEOF() && iter < elementos ) {
                sprintf( ( char* )&nombres[iter][0], 
						"           %d             -           %d", present.cod_present,
                         ( int )present.cantidad );
				items[iter].nom = ( char* )&nombres[iter][0];
				_snprintf( tmp, sizeof( tmp ), "%d", present.cod_present );
				tmp[sizeof( tmp ) - 1] = '\0';
				items[iter].tecla = ( char ) tmp[0];
				items[iter].rta = present.cod_present;
				iter++;
			SKIP2( 1 );
		}
		if( elementos <= 0 ) {
			menu_ok = 0;
		}
		cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
		if( menu_ok && cab != NULL ) {
				cab[0].nombre = "   Cod. Pres.   ";
				cab[0].align = ALIGN_LEFT;
				cab[0].width = 8;
				cab[1].nombre = "   Cant. Art.";
				cab[1].align = ALIGN_LEFT;
				cab[1].width = 20;
				k = MENU( ITEM_NOMBRE_LARGO, items, elementos,  cab, columnas );
		}
		if( cab != NULL )
			free( cab );
		
		if( items != NULL ) {
			free( items );
		}

		if( k >= 0 && k <= 9 ) {
			ind_present.cod_present = k;
			SET_WHERE("AUTOINC = '%ld' and COD_PRESENT = '%i' and HABILITADA = 'S'",
							ind_present.autoinc,
							ind_present.cod_present);
			RUN_QUERY(NO);
            if( FOUND2() ) {
                rta = ( int )present.cantidad;
				CR1_ADOPTA_PRESENTACION( k , NO );
            }
        }
	}
    else {
        rta = 1;
    }
    SET_MEMORY_INT( __item_presentacion, rta );
    #endif

    return ( rta );
}
/*****************************************************************************/
void CR1_ADOPTA_PRESENTACION( int nro, int inicial )
/*****************************************************************************/
{
    double Tot = 0.0, porcentaje = 0.0, precio_sin_iva = 0.0;
    int i;
    /*----------------------- Busca la presentacion --------------------------*/
    if( PRESENTACIONES_HABILITADAS ) {
		SELECT_TABLE( T_PRESENT, TT_ORIG );
        ind_present.autoinc = ITEM_AUTOINC;
        ind_present.cod_present = (char)nro;
		SET_WHERE("AUTOINC = '%ld' and COD_PRESENT = '%i'",
							ind_present.autoinc,
							ind_present.cod_present);
		RUN_QUERY(NO);
		//TIPO_PRESENTACIONES = 1 = RANGOS X CANTIDAD
        if( !FOUND2() || ( inicial && TIPO_PRESENTACIONES == 1 ) ) {
            present.cod_present = 0;
            present.cantidad = 1;
            present.porcentaje = 0.0;
        }
    }
    else {
        present.cod_present = 1;
        present.cantidad = 1;
        present.porcentaje = 0.0;
    }
    if( TIPO_PRESENTACIONES == 1 ) {
        present.cantidad = 1;
    }
    /*-------------------------- calcula recargo -----------------------------*/
    //fabiana 15-02-05 ** nuevo manejo de redondeos--modo facturacin especial quedarï¿½eliminado
    //if( MODO_FACTURACION_ESPECIAL(  ) ) {
    //  SET_MEMORY_DOUBLE( __item_precio_sin_iva, ROUND( ITEM_PRECIO_SIN_IVA, 10,
    //          DECIMALES ) );
    //}
	
	//Se hace esto porque da el error -1.#IND000000000000
	porcentaje = (double)present.porcentaje;
	precio_sin_iva = ITEM_PRECIO_SIN_IVA;
    SET_MEMORY_DOUBLE( __item_precio,
                       precio_sin_iva * ( 1 + ( porcentaje / 100.0 ) ) );

    //SET_MEMORY_DOUBLE( __item_precio,
    //  ( ITEM_PRECIO_SIN_IVA * ( 1 + ( present.porcentaje / 100.0 ) ) ) );
    //CR1_CALCULA_RECARGO(  );
    CR1_CALCULA_IVA();
	//if( PRESENTACIONES_HABILITADAS && present.cod_present != 0 && TIPO_PRESENTACIONES != 1 ) {
    if( TIPO_PRESENTACIONES != 1 ) {
        SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO * present.cantidad );
        SET_MEMORY_DOUBLE( __item_recargo, ITEM_RECARGO * present.cantidad );
    }
    SET_MEMORY_DOUBLE( __item_iva1_precio, ITEM_IVA1_PRECIO * present.cantidad );
    SET_MEMORY_DOUBLE( __item_iva2_precio, ITEM_IVA2_PRECIO * present.cantidad );
    //SET_MEMORY_FLOAT( __item_ila_precio, ITEM_ILA_PRECIO * present.cantidad );
    //SET_MEMORY_FLOAT( __item_iha_precio, ITEM_IHA_PRECIO * present.cantidad );
    //SET_MEMORY_FLOAT( __item_alicuota_precio, ITEM_ALICUOTA_PRECIO * present.cantidad );
    SET_MEMORY_DOUBLE( __item_iva1_rec, ITEM_IVA1_REC * present.cantidad );
    SET_MEMORY_DOUBLE( __item_iva2_rec, ITEM_IVA2_REC * present.cantidad );
    //SET_MEMORY_FLOAT( __item_ila_rec, ITEM_ILA_REC * present.cantidad );
    //SET_MEMORY_FLOAT( __item_iha_rec, ITEM_IHA_REC * present.cantidad );
    SET_MEMORY_DOUBLE( __item_costo, ITEM_COSTO * present.cantidad );
    SET_MEMORY_INT( __item_presentacion, ( int )present.cantidad );
    SET_MEMORY_CHAR( __item_cod_presentacion, present.cod_present );
    for( i = 0;i < 4;i++ ) {
		_SET_MEMORY_FLOAT( __item_monto_impuesto, i, _ITEM_MONTO_IMPUESTO( i ) * present.cantidad );
		if( PRECALCULAR_IMPUESTOS ) {
			Tot += _ITEM_MONTO_IMPUESTO( i );
		} else {
			Tot += ( _ITEM_SUMA_IMPUESTO_AL_TOTAL( i ) ? _ITEM_MONTO_IMPUESTO( i ) : 0 );
		}
    }
    //if( !MODO_FACTURACION_ESPECIAL(  ) ) {
	if( present.cantidad > 1 && IMPRESORA_FISCAL == FISCAL && TIPO_PRESENTACIONES != 1 ) {//
		double item_precio = ITEM_PRECIO;
		ROUND2(&item_precio, ENTEROS, 2);
		SET_MEMORY_DOUBLE( __item_precio, item_precio );
	}


    
    SET_MEMORY_DOUBLE( __item_precio_sin_iva,
                       ITEM_PRECIO - ITEM_IVA1_PRECIO - ITEM_IVA2_PRECIO - Tot );
    //  }
}
/*****************************************************************************/
void CR1_CALCULA_RECARGO( void )
/*****************************************************************************/
{
    SET_MEMORY_DOUBLE( __item_recargo, 0.0 );
    SET_MEMORY_DOUBLE( __item_imp_int_rec, 0.0 );
    if( config.momento_recargo == AL_RENGLON && VAR_TARJETAS_STATUS
     && fabs( VAR_TARJETAS_RECARGO + VAR_TARJETAS_RECARGO_DIFERIM ) > 0.01 ) {
        ADD_MEMORY_DOUBLE( __item_recargo,
                           ITEM_PRECIO * ( VAR_TARJETAS_RECARGO + VAR_TARJETAS_RECARGO_DIFERIM )
                         / 100.0 );
        ADD_MEMORY_DOUBLE( __item_imp_int_rec,
                           ITEM_IMP_INT * ( VAR_TARJETAS_RECARGO + VAR_TARJETAS_RECARGO_DIFERIM )
                         / 100.0 );
    }
    if( RAM_NRO_CLIENTE && fabs( NRO_RECARGO_FINANCIACION ) > 0.01 ) {
        ADD_MEMORY_DOUBLE( __item_recargo, ITEM_PRECIO * NRO_RECARGO_FINANCIACION / 100.0 );
        ADD_MEMORY_DOUBLE( __item_imp_int_rec, ITEM_IMP_INT * NRO_RECARGO_FINANCIACION / 100.0 );
    }
    #ifdef COMPILAR_MUTUALES
    if( MUTUAL_CODIGO && fabs( MUTUAL_RECARGO ) > 0.01 && TRATAMIENTO_MUTUALES == AL_RENGLON ) {
        ADD_MEMORY_DOUBLE( __item_recargo, ITEM_PRECIO * MUTUAL_RECARGO / 100.0 );
        ADD_MEMORY_DOUBLE( __item_imp_int_rec, ITEM_IMP_INT * MUTUAL_RECARGO / 100.0 );
    }
    #endif
}
/*****************************************************************************/
void CR1_CALCULA_IVA( void )
/*****************************************************************************/
{
    int i = 0;
    double tot = 0;
	ADD_MEMORY_DOUBLE( __item_precio, -( ITEM_IMP_INT ) );
    ADD_MEMORY_DOUBLE( __item_recargo, -( ITEM_IMP_INT_REC ) );
	if( CR1_ITEM_GRAVADO() || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) {
		if( PRECALCULAR_IMPUESTOS ) {
			for( i = 0;i < 4;i++ ) {
				_SET_MEMORY_FLOAT( __item_monto_impuesto, i,
								ITEM_PRECIO * _ITEM_PORC_IMPUESTO( i ) / 100.0 );
				tot += _ITEM_MONTO_IMPUESTO( i );
			}
			SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO + tot );
		}
        //aca hay que tener cuidado con el redondeo psiva = 0.8264 iva = 0.173544 = 0.999944 y no es 1
		if( ITEM_PEDIR_PRECIO == 1) {
			double corregir, redondeado ;
			SET_MEMORY_DOUBLE( __item_iva1_precio, ITEM_PRECIO * iva[ITEM_COD_IVA].porc_iva1 / 100.0 ); //0.173544
			corregir = ITEM_IVA1_PRECIO;  //0.173544
			corregir = ITEM_PRECIO + corregir; //0.999944
			redondeado = corregir;
			ROUND2(&redondeado,ENTEROS, 3); //1.0000
			if( (redondeado - corregir ) <0.00006 )
				ADD_MEMORY_DOUBLE( __item_iva1_precio,(redondeado - corregir ));
			
				//SET_MEMORY_DOUBLE( __item_iva1_precio, ITEM_PRECIO * iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
		} else
        SET_MEMORY_DOUBLE( __item_iva1_precio, ITEM_PRECIO * iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
        SET_MEMORY_DOUBLE( __item_iva1_rec, ITEM_RECARGO * iva[ITEM_COD_IVA].porc_iva1 / 100.0 );
		if( RAM_FACTURA
         && ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva == RESP_NO_INSC ) ) {
            SET_MEMORY_DOUBLE( __item_iva2_precio,
                               ITEM_PRECIO * iva[ITEM_COD_IVA].porc_iva2 / 100.0 );
            SET_MEMORY_DOUBLE( __item_iva2_rec,
                               ITEM_RECARGO * iva[ITEM_COD_IVA].porc_iva2 / 100.0 );
		} else {
			SET_MEMORY_DOUBLE( __item_iva2_precio, 0.00 );
			SET_MEMORY_DOUBLE( __item_iva2_rec, 0.00 );
        }
    } else {
		SET_MEMORY_DOUBLE( __item_iva1_precio, 0.00 );
		SET_MEMORY_DOUBLE( __item_iva1_rec, 0.00 );
		SET_MEMORY_DOUBLE( __item_iva2_precio, 0.00 );
		SET_MEMORY_DOUBLE( __item_iva2_rec, 0.00 );
    }
    /*SET_MEMORY_FLOAT( __item_ila_precio, ITEM_PRECIO * ITEM_PORC_ILA / 100.0 );
    SET_MEMORY_FLOAT( __item_ila_rec, ITEM_RECARGO * ITEM_PORC_ILA / 100.0 );
    SET_MEMORY_FLOAT( __item_iha_precio, ITEM_PRECIO * ITEM_PORC_IHA / 100.0 );
    SET_MEMORY_FLOAT( __item_ila_rec, ITEM_RECARGO * ITEM_PORC_IHA / 100.0 );
    SET_MEMORY_FLOAT( __item_alicuota_precio, ITEM_PRECIO * ITEM_PORC_ICA / 100.0 );
     * /
    SET_MEMORY_FLOAT( __item_iha_precio,
        ( RAM_FACTURA ) ? ITEM_PRECIO * ITEM_PORC_IHA / 100.0 : 0 );
    SET_MEMORY_FLOAT( __item_alicuota_precio, ( ( RAM_FACTURA
                && articulo.cod_impuesto[3] ) ) ? ITEM_PRECIO *
        impuestos[articulo.cod_impuesto[3]] / 100.0 : 0 );
    if( config.pais == CHILE && RAM_NOTA_CR && RAM_TIPO_EVENTO_COMP_ORIGINAL == 0 ) {
        SET_MEMORY_FLOAT( __item_alicuota_precio, 0.0 );
    }
    SET_MEMORY_FLOAT( __item_iha_rec,
        ( RAM_FACTURA ) ? ITEM_RECARGO * ITEM_PORC_IHA / 100.0 : 0 );
    */
	if( !PRECALCULAR_IMPUESTOS || ( PRECALCULAR_IMPUESTOS &&
		!( CR1_ITEM_GRAVADO() || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) ) ) {
		for( i = 0;i < 4;i++ ) {
			_SET_MEMORY_FLOAT( __item_monto_impuesto, i, ITEM_PRECIO *
				_ITEM_PORC_IMPUESTO( i ) / 100.0 );
			tot += ( _ITEM_SUMA_IMPUESTO_AL_TOTAL( i ) ? _ITEM_MONTO_IMPUESTO( i ) : 0 );
		}
	}
    //SET_MEMORY_DOUBLE( __item_precio,
    //  ITEM_PRECIO + ITEM_IMP_INT + ITEM_IVA1_PRECIO + ITEM_IVA2_PRECIO + ITEM_ILA_PRECIO +
    //  ITEM_IHA_PRECIO + ITEM_ALICUOTA_PRECIO );
    //SET_MEMORY_DOUBLE( __item_recargo,
    //  ITEM_RECARGO + ITEM_IMP_INT_REC + ITEM_IVA1_REC + ITEM_IVA2_REC + ITEM_ILA_REC +
    //  ITEM_IHA_REC );
	if( PRECALCULAR_IMPUESTOS ) {
		SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO + ITEM_IMP_INT + ITEM_IVA1_PRECIO +
			ITEM_IVA2_PRECIO );
	} else {
		SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO + ITEM_IMP_INT + ITEM_IVA1_PRECIO +
			ITEM_IVA2_PRECIO + tot );
	}
    //fabiana 15-02-05 ** nuevo manejo de redondeos--Primer Calculo de IVA: ITEM_PRECIO sin redondeos
    #ifdef DEBUG_R
    if( MODO_DEBUG == _DEBUG_REDONDEO ) {
        memset( cadena_debug, 0, strlen( cadena_debug ) );
        sprintf( cadena_debug, "4.Calc iva, %li, IPSI=%lf, IP=%lf, I1=%lf, I2=%lf",
                 articulo.cod_interno, ITEM_PRECIO_SIN_IVA, ITEM_PRECIO, ITEM_IVA1_PRECIO,
                 ITEM_IVA2_PRECIO );
        GRABAR_LOG_SISTEMA( cadena_debug );
    }
    #endif
}

/***************************************************************************************/
void CR1_CONTADOR_INIT( )
/***************************************************************************************/
{
	int cnt = 0, cant = 0, signo, encontro = 0;
    int item;//float cantidad;
	int contar, consecutivo = 0;
	char desc[31], cant2[15], cant1[15];

	memset( desc, 0, 31 );
	memset( cant2, 0, 15 );
	memset( cant1, 0, 15 );

	if( RAM_P_TICKET ){
		item = RAM_P_TICKET - 1;
		signo = ( _TICKET_TIPO( item ) & _RESTA ) ? -1 : 1;
		contar = 1;
		if( ( ( _TICKET_ORIGEN( item ) == _ORIGEN_DESC_PESOS
				|| _TICKET_ORIGEN( item ) == _ORIGEN_DESC_PORC
				|| _TICKET_ORIGEN( item ) == _ORIGEN_DESC_PORC_FIJO
				|| _TICKET_ORIGEN( item ) == _ORIGEN_PROMOCION
				|| _TICKET_COD_MOVIMIENTO( item ) == _DIFERENCIA_PRECIO )
				&& signo > 0 )
				|| ( _TICKET_COD_MOVIMIENTO( item ) == _DIFERENCIA_PRECIO_ANULADA
				|| _TICKET_ORIGEN( item ) == _ORIGEN_DIF_PRECIO ) ) {
			contar = 0;
		}
		cant = CANTIDAD_REGISTROS_REC( ARCH_REC_CONTADOR_ARTICULO,
			sizeof( struct _cont_art ) );
		while( ( cnt < cant ) && !encontro && contar ) {
			RECUPERAR_ARCHIVO_REC( ARCH_REC_CONTADOR_ARTICULO, ( char* )&cont_art,
					sizeof( struct _cont_art ), cnt );
			if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
				if( _TICKET_COD_ESPECIAL( item ) == cont_art.cod
						&& ( strcmp( _TICKET_COD_BARRA( item ), cont_art.cod_barra ) == 0 ) ) {
					encontro = 1;
					if( encontro
							&& ( ( _TICKET_TIPO( item ) & _DEV_ENVASE )
							&& !( _TICKET_TIPO( cont_art.pos ) & _DEV_ENVASE ) ) ) {
						encontro = 0;
					}
				}
			}
			else {
				if( _TICKET_COD_ESPECIAL( item ) == cont_art.cod ) {
					encontro = 1;
				}
			}
				cnt++;
		}
		FTOC( cont_art.cant_consecutivos, cant2, 4, 0, 0, 0 );
		FTOC( cont_art.cant, cant1, 4, 0, 0, 0 );
	}
	
	ExecuteSAction( A_MUESTRA_DESCRIPCION_CONTADOR, desc );
	ExecuteSAction( A_MUESTRA_CONTADOR_DIFERENCIAL, cant2 );
	ExecuteSAction( A_MUESTRA_CONTADOR_TOTAL_IDEN, cant1 );
}

/***************************************************************************************/
int CR1_CONTADOR_ARTICULO( long cod_especial, char *cod_barra, int item )
/***************************************************************************************/
{
    int cnt = 0, cant = 0, signo, encontro = 0;
    int cantidad;//float cantidad;
	int contar = 0, consecutivo = 0;

    signo = ( _TICKET_TIPO( item ) & _RESTA ) ? -1 : 1;

	if(!ITEM_PESABLE && !ITEM_PEDIR_PRECIO ){
		cantidad = (int)_TICKET_CANTIDAD( item ) * signo;
	}else{
		cantidad = signo;
	}

	encontro = 0;
	contar = 1;
    if( ( ( _TICKET_ORIGEN( item ) == _ORIGEN_DESC_PESOS
			|| _TICKET_ORIGEN( item ) == _ORIGEN_DESC_PORC
			|| _TICKET_ORIGEN( item ) == _ORIGEN_DESC_PORC_FIJO
            || _TICKET_ORIGEN( item ) == _ORIGEN_PROMOCION
			|| _TICKET_COD_MOVIMIENTO( item ) == _DIFERENCIA_PRECIO )
			&& signo > 0 )
			|| ( _TICKET_COD_MOVIMIENTO( item ) == _DIFERENCIA_PRECIO_ANULADA
			|| _TICKET_ORIGEN( item ) == _ORIGEN_DIF_PRECIO ) ) {
		contar = 0;
    }
    cant = CANTIDAD_REGISTROS_REC( ARCH_REC_CONTADOR_ARTICULO,
			sizeof( struct _cont_art ) );
	while( ( cnt < cant ) && !encontro && contar ) {
		RECUPERAR_ARCHIVO_REC( ARCH_REC_CONTADOR_ARTICULO, ( char* )&cont_art,
				sizeof( struct _cont_art ), cnt );
		if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
			if( _TICKET_COD_ESPECIAL( item ) == cont_art.cod
					&& ( strcmp( _TICKET_COD_BARRA( item ), cont_art.cod_barra ) == 0 ) ) {
				encontro = 1;
				if( encontro
						&& ( ( _TICKET_TIPO( item ) & _DEV_ENVASE )
						&& !( _TICKET_TIPO( cont_art.pos ) & _DEV_ENVASE ) ) ) {
					encontro = 0;
				}
			}
		}
		else {
			if( _TICKET_COD_ESPECIAL( item ) == cont_art.cod ) {
				encontro = 1;
			}
		}
		//Contador Total
		if( encontro && contar ) {
			cont_art.cant += cantidad;
			/*ACTUALIZAR_ARCHIVO_REC( ARCH_REC_CONTADOR_ARTICULO, (char*)&cont_art,
				sizeof( struct _cont_art ), cnt );*/
		}
			cnt++;
	}
	// Contador Consecutivo de Articulo
	if( encontro && contar ) {
		if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
			if( _TICKET_COD_ESPECIAL( item ) == codigo_consecutivo
					&& ( strcmp( _TICKET_COD_BARRA( item ), barrar_consecutivo ) == 0 ) ) {
				consecutivo = 1;
			}
		}
		else {
			if( _TICKET_COD_ESPECIAL( item ) == codigo_consecutivo ) {
				consecutivo = 1;
			}
		}
		if( consecutivo && cantidad > 0 ) {
			cont_art.cant_consecutivos += cantidad;
		} else if( !consecutivo && cantidad > 0 ){
			codigo_consecutivo = _TICKET_COD_ESPECIAL( item );
			memcpy( barrar_consecutivo, _TICKET_COD_BARRA( item ), 17 );
			cont_art.cant_consecutivos = cantidad;
		}else{
			if( consecutivo ){
				cont_art.cant_consecutivos += ( cont_art.cant_consecutivos > 0 ) ? cantidad : 0;
			}else{
				cont_art.cant_consecutivos = 0;
			}
		}
		cantidad_consecutivo = cont_art.cant_consecutivos;
		ACTUALIZAR_ARCHIVO_REC( ARCH_REC_CONTADOR_ARTICULO, (char*)&cont_art,
				sizeof( struct _cont_art ), cnt - 1);
	}
	if( !encontro && contar ) {
		if( cantidad ) {
			// Contador total de Articulo
			cont_art.cod = _TICKET_COD_ESPECIAL( item );
            memcpy( cont_art.cod_barra, _TICKET_COD_BARRA( item ), 17 );
			cont_art.cant = cantidad;
			cont_art.cant_consecutivos = cantidad;
			cont_art.pos = item;
			GUARDAR_ARCHIVO_REC( ARCH_REC_CONTADOR_ARTICULO, ( char* )&cont_art,
									sizeof( struct _cont_art ) );
			// Contador Consecutivo de Articulo
			codigo_consecutivo = _TICKET_COD_ESPECIAL( item );
            memcpy( barrar_consecutivo, _TICKET_COD_BARRA( item ), 17 );
			cantidad_consecutivo = cantidad;
           }
	}
	return( 1 );
}

/*****************************************************************************/
int CR1_BUSCA_CODIGO(int enganche)
/*****************************************************************************/
{
	int encontro, rta = 1, cant = 0, nro = 0, recalcular,
        articulo_x_importe = 1, tit = 0, ingresar = 0, cod_dun14 = 0,
		tecla_escape = NO;
    char cod_barra_ant[17];
    double pesable_redondeado,p_unitario = 0.0;
	
	tit = 1;
    if( RAM_CORRECCION ) {
        return ( 1 );
    }
	if( PRESENTACIONES_EN_DUN14 ) {
		cod_dun14 = _RAM_COD_BARRA( 0 );
		if( L_CODIGO != 14 ) {
			MENSAJE_STRING( S_ATENCION_EN_LONGITUD_CODIGO_SCANNER );
		}
		if( cod_dun14 == ' ' || cod_especial ) {
			cod_dun14 = -1;
		} else {
			cod_dun14 -= 48;
		}
		if( cod_dun14 != -1 ) {
			strcpy( cod_barra_ant, &RAM_COD_BARRA[1] );
		} else {
			strcpy( cod_barra_ant, RAM_COD_BARRA );
			//quitamos el espacio por ser L_CODIGO = 14 
			LTRIM( cod_barra_ant );
		}
		if( cod_dun14 != -1 && atof( RAM_COD_BARRA ) > 0 ) {//si esta vacio pisa fuera de la cadena _atold
   			CALCULA_CRC_EAN13( cod_barra_ant );
   		}
		SET_MEMORY( __ram_cod_barra, cod_barra_ant );
		CEREAR( cod_barra_ant, L_CODIGO - 1 );
		EVALUAR_CODIGOS_ESPECIALES( L_CODIGO - 1 );
	} else {
		strcpy( cod_barra_ant, RAM_COD_BARRA );
		CEREAR( cod_barra_ant, L_CODIGO );
		EVALUAR_CODIGOS_ESPECIALES( L_CODIGO );
	}
    if( CLIENTE_OBLIGATORIO && !RAM_NRO_CLIENTE ) {
        MENSAJE_STRING( S_FAVOR_SELECCIONE_CLIENTE );
        return ( 0 );
    }
    if( VENDEDOR_OBLIGATORIO && !NRO_VENDEDOR_LONG ) {
        MENSAJE_STRING( S_FAVOR_INGRESE_VENDEDOR );
        return ( 0 );
    }
    if( cod_especial == 0 && _RAM_COD_BARRA( 0 ) == 0 ) {
        return ( 0 );
    }
    //HMM cuando en el padron los articulos balanza estan completos ej 2501212000003 cod_inte 1212 cuando seria 000000001212X el codigo de barra
	if( cod_especial == 0 && cod_interno_cod_barra && BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
		cod_interno_cod_barra = 0;
		usar_cadena_scanner = 0;
	}

    if( cod_interno_cod_barra && BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
        strcpy( cod_barra_ant, RAM_COD_BARRA );
        CEREAR( cod_barra_ant, L_CODIGO );
        if( BUSCA_COD_INTERNO_COD_BARRA( cod_especial, cod_barra_ant ) ) {
            if( RAM_BIEN_DE_USO && articulo.bien_de_uso == 'N' ) {
                MENSAJE_STRING( S_Articulo_no_Hablilidato_para_Venta_de_Bienes_de_Uso );
                return ( 0 );
            }
            if( RAM_CANTIDAD >= 2 && articulo.multiplicable == 'N' && !enganche ) {
                MENSAJE_STRING( S_ESTE_ARTICULO_NO_ADMITE_SER_MULTIPLICADO );
                return ( 0 );
            }
            encontro = ASIGNA_COD_ESPECIAL_COD_BARRA( cod_especial, cod_barra_ant );
        }
        else {
            encontro = 0;
        }
        cod_interno_cod_barra = 0;
    }
    else if( cod_especial ) {
        if( BUSCA_CODIGO_ESPECIAL( cod_especial ) ) {
            if( RAM_BIEN_DE_USO && articulo.bien_de_uso == 'N' ) {
                MENSAJE_STRING( S_Articulo_no_Hablilidato_para_Venta_de_Bienes_de_Uso );
                return ( 0 );
            }
            if( RAM_CANTIDAD >= 2 && articulo.multiplicable == 'N' && !enganche ) {
                MENSAJE_STRING( S_ESTE_ARTICULO_NO_ADMITE_SER_MULTIPLICADO );
                return ( 0 );
            }
            if( RAM_USA_TEMPLATE == 3 ) {
                SET_MEMORY_DOUBLE( __ram_cantidad, RAM_TOMADO_POR_TEMPLATE );
            }
			//encontro = ASIGNA_CODIGO_ESPECIAL( cod_especial );
            encontro = ASIGNA_ITEM( NO );
        }
        else {
            encontro = 0;
        }
    }
    else {
        strcpy( cod_barra_ant, RAM_COD_BARRA );
        CEREAR( cod_barra_ant, L_CODIGO );
        if( BUSCA_CODIGO_BARRA( cod_barra_ant ) ) {
            if( RAM_BIEN_DE_USO && articulo.bien_de_uso == 'N' ) {
                MENSAJE_STRING( S_Articulo_no_Hablilidato_para_Venta_de_Bienes_de_Uso );
                return ( 0 );
            }
            if( RAM_CANTIDAD >= 2 && articulo.multiplicable == 'N' && !enganche ) {
                MENSAJE_STRING( S_ESTE_ARTICULO_NO_ADMITE_SER_MULTIPLICADO );
                return ( 0 );
            }
            encontro = ASIGNA_CODIGO_BARRA( ( char* )&cod_barra_ant );
        }
        else {
            encontro = 0;
        }
    }
    if( encontro == 1 || encontro == -1 ) {
		//A los Art. por importe no se le permite cambiar el precio cuando FUNCION_PIDE_PRECIO = NORMAL;
		//Esta restriccion es porque el motor de promociones necesita tener el mismo precio_unitario 
		//(total/cantidad) dentro del mismo ticket para articulos iguales para aplicar las promociones
        if(  ITEM_PEDIR_PRECIO && RAM_DIFERENCIA_PRECIO && !FUNCION_PIDE_PRECIO ) {
			 MENSAJE( ST( S_NO_SE_PERMITE_CAMBIO_PRECIO_EN_ART_POR_IMPORTE ) );
			 BORRAR_MENSAJE( );
			 return ( 0 );
		}
        /*---------------- Recupera el codigo de barras leido ---------------*/
        if( mantener_cod_barra ) {
            SET_MEMORY( __item_cod_barra, cod_barra_ant );
        }
        /*---------- Tratamiento especial de codigo de barras en 0 ----------*/
        if( TRATAMIENTO_CODIGO_0 && !memcmp( cod_barra_ant, "0000000000000000", L_CODIGO ) ) {
            if( !INGRESA_CODIGO_BARRAS() ) {
                return ( 0 );
            }
        }
        /*------------ Especial para cuando no hay scanner ---------------*/
		//if( !VARIOS_SCANNER_HABILITADO && RAM_PRESENT_ANT < 0 && TIPO_PRESENTACIONES == 0 ) {
		if( RAM_PRESENT_ANT < 0 && TIPO_PRESENTACIONES == 0 ) {
            /*-------- Cuantas presentaciones activas hay ? --------*/
        #ifdef COMPILAR_PRESENTACIONES
			if( PRESENTACIONES_HABILITADAS ) {
				int elementos = 0;

				SELECT_TABLE( T_PRESENT, TT_ORIG );
                ind_present.autoinc = ITEM_AUTOINC;
                ind_present.cod_present = 0;
				SET_WHERE("AUTOINC = %ld and COD_PRESENT >= %i and HABILITADA = 'S'",
						ind_present.autoinc, ind_present.cod_present);
				SET_ORDER2("AUTOINC,COD_PRESENT");
				RUN_QUERY(NO);
				elementos = RECCOUNT2( NULL, NULL);
				SET_MEMORY_CHAR( __item_cant_present, elementos );
				//Por ahora las devoluciones de un art. con presentacion se realizara 
				//por Notas de Credito libre debido al prorrateo del descuentos.
				if( elementos && ( RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL 
							|| RAM_MODO_DEVOLUCION == _DEVOL_PARCIAL ) ) {
					MENSAJE_TECLA("Para Art. con Presentaciones relizar una Nota de Credito Libre o Total, PRES. UNA TECLA"); 
					encontro = 0;
					presentaciones = 0;
				}
				if( encontro == 1 ) {
					if( PRESENTACIONES_EN_DUN14 && cod_dun14 != -1 ) {
						presentaciones = 0;
						nro = -1;
						while( !dbEOF() && ( nro == -1 ) ) {
							if( present.dig1_dun14 == cod_dun14 ) {
								nro = present.cod_present;
							}
							SKIP2( 1 );
						}
						if( nro == -1 ) {
						  encontro = 0;
						  MENSAJE_STRING( S_NO_PRESENTACIONES_HABILITADAS );
						} else {
							  encontro = 1;
							  CR1_ADOPTA_PRESENTACION( nro, NO );
						}
					} else {
						/*-------- Si hay una la toma si no pide presentaciones ---------*/
						while( !dbEOF() ) {
							cant++;
							nro = present.cod_present;
							SKIP2( 1 );
						}
						/*-------- Si hay una la toma si no pide presentaciones ---------*/
						if( cant == 1 || cant == 0 ) {
							encontro = 1;
							CR1_ADOPTA_PRESENTACION( cant ? nro : 1, NO );
							presentaciones = 0;
						} 
						else if( !presentaciones && !( RAM_MODO_DEVOLUCION == _NC_TOTAL ) ) {
						   presentaciones = MENU_PRESENTACIONES_INCONDICIONA;
						}
					}
				} else {
					presentaciones = 0; 
				}
			} 
			#endif
		}
        /*------------------- pide presentaciones -------------------*/
        if( presentaciones ) {
            rta = CR1_ELEGIR_PRESENTACION();
            encontro = ( rta == 0 ) ? 0 : 1;
            presentaciones = 0;
        }
        /*-------------------- toma presentacion anterior -----------------*/
        else if( RAM_PRESENT_ANT >= 0 ) {
            //fabiana 15-02-05 ** nuevo manejo de redondeos--no se tratarï¿½ presentaciones,
            //pero hay q tener en cuenta que en estas asingaciones no hay redondeos extras
            SET_MEMORY_INT( __item_presentacion, RAM_PRESENT_ANT );
            SET_MEMORY_CHAR( __item_cod_presentacion, _TICKET_COD_PRESENT( RAM_P_TICKET - menos ) );
			if( !( IMPRESORA_FISCAL == FISCAL && RAM_TICKET_FACTURA == 1 ) 
					|| RAM_PRESENT_ANT > 1 ) {
				SET_MEMORY_DOUBLE( __item_precio, _TICKET_PRECIO( RAM_P_TICKET - menos ) );
				SET_MEMORY_DOUBLE( __item_recargo, _TICKET_RECARGO( RAM_P_TICKET - menos ) );
				SET_MEMORY_DOUBLE( __item_iva1_precio, _TICKET_IVA1_PRECIO( RAM_P_TICKET - menos ) );
				SET_MEMORY_DOUBLE( __item_iva2_precio, _TICKET_IVA2_PRECIO( RAM_P_TICKET - menos ) );
				SET_MEMORY_DOUBLE( __item_iva1_rec, _TICKET_IVA1_REC( RAM_P_TICKET - menos ) );
				SET_MEMORY_DOUBLE( __item_iva2_rec, _TICKET_IVA2_REC( RAM_P_TICKET - menos ) );
				//SET_MEMORY_FLOAT( __item_ila_precio,
				//  _TICKET_ILA_PRECIO( RAM_P_TICKET - menos ) );
				//SET_MEMORY_FLOAT( __item_iha_precio,
				//  _TICKET_IHA_PRECIO( RAM_P_TICKET - menos ) );
				//SET_MEMORY_FLOAT( __item_alicuota_precio,
				//  _TICKET_ALICUOTA_PRECIO( RAM_P_TICKET - menos ) );
				//SET_MEMORY_FLOAT( __item_ila_rec, _TICKET_ILA_REC( RAM_P_TICKET - menos ) );
				//SET_MEMORY_FLOAT( __item_iha_rec, _TICKET_IHA_REC( RAM_P_TICKET - menos ) );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 0,
								   _TICKET_IMPUESTO1_PRECIO( RAM_P_TICKET - menos ) );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 1,
								   _TICKET_IMPUESTO2_PRECIO( RAM_P_TICKET - menos ) );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 2,
								   _TICKET_IMPUESTO3_PRECIO( RAM_P_TICKET - menos ) );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 3,
								   _TICKET_IMPUESTO4_PRECIO( RAM_P_TICKET - menos ) );
				SET_MEMORY_DOUBLE( __item_imp_int, _TICKET_IMP_INT( RAM_P_TICKET - menos ) );
			}
			SET_MEMORY_CHAR( __item_cod_movimiento,
							 _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - menos ) );
            if( _TICKET_EXENTO( RAM_P_TICKET - menos ) ) {
                SET_MEMORY_DOUBLE( __item_precio_sin_iva, _TICKET_EXENTO( RAM_P_TICKET - menos ) );
            }
            else {
				if( !( IMPRESORA_FISCAL == FISCAL && RAM_TICKET_FACTURA == 1 ) 
					|| RAM_PRESENT_ANT > 1 ) {
					SET_MEMORY_DOUBLE( __item_precio_sin_iva,
						               _TICKET_IMPORTE_SIN_IVA( RAM_P_TICKET - menos ) );
				}
            }
            encontro = 1;
            #ifdef COMPILAR_COMPROMISO_200
            if( RAM_COMPROMISO_200 ) {
                SET_MEMORY_LONG( __item_cod_clasificacion, cod_clasificacion_compromiso_200 );
            }
            /*--- Si registramos un compromiso 200% y el articulo anterior es diferencia
                         de precio, debemos sumarle el precio anterior ---*/
            if( RAM_COMPROMISO_200
				&& _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - menos ) == _DIFERENCIA_PRECIO ) {
                ADD_MEMORY_DOUBLE( __item_precio, _TICKET_PRECIO( RAM_P_TICKET - menos - 1 ) );
                ADD_MEMORY_DOUBLE( __item_recargo, _TICKET_RECARGO( RAM_P_TICKET - menos - 1 ) );
                ADD_MEMORY_DOUBLE( __item_iva1_precio,
                                   _TICKET_IVA1_PRECIO( RAM_P_TICKET - menos - 1 ) );
                ADD_MEMORY_DOUBLE( __item_iva2_precio,
                                   _TICKET_IVA2_PRECIO( RAM_P_TICKET - menos - 1 ) );
                ADD_MEMORY_DOUBLE( __item_iva1_rec, _TICKET_IVA1_REC( RAM_P_TICKET - menos - 1 ) );
                ADD_MEMORY_DOUBLE( __item_iva2_rec, _TICKET_IVA2_REC( RAM_P_TICKET - menos - 1 ) );
                /*
                        ADD_MEMORY_FLOAT( __item_ila_precio,
                            _TICKET_ILA_PRECIO( RAM_P_TICKET - menos - 1 ) );
                        ADD_MEMORY_FLOAT( __item_iha_precio,
                            _TICKET_IHA_PRECIO( RAM_P_TICKET - menos - 1 ) );
                        ADD_MEMORY_FLOAT( __item_alicuota_precio,
                            _TICKET_ALICUOTA_PRECIO( RAM_P_TICKET - menos - 1 ) );
                        ADD_MEMORY_FLOAT( __item_ila_rec,
                            _TICKET_ILA_REC( RAM_P_TICKET - menos - 1 ) );
                        ADD_MEMORY_FLOAT( __item_iha_rec,
                            _TICKET_IHA_REC( RAM_P_TICKET - menos - 1 ) );
                        */
                _ADD_MEMORY_FLOAT( __item_monto_impuesto, 0,
                                   _TICKET_IMPUESTO1_PRECIO( RAM_P_TICKET - menos ) );
                _ADD_MEMORY_FLOAT( __item_monto_impuesto, 1,
                                   _TICKET_IMPUESTO2_PRECIO( RAM_P_TICKET - menos ) );
                _ADD_MEMORY_FLOAT( __item_monto_impuesto, 2,
                                   _TICKET_IMPUESTO3_PRECIO( RAM_P_TICKET - menos ) );
                _ADD_MEMORY_FLOAT( __item_monto_impuesto, 3,
                                   _TICKET_IMPUESTO4_PRECIO( RAM_P_TICKET - menos ) );
                ADD_MEMORY_DOUBLE( __item_imp_int, _TICKET_IMP_INT( RAM_P_TICKET - menos - 1 ) );
                if( _TICKET_EXENTO( RAM_P_TICKET - menos ) ) {
                    ADD_MEMORY_DOUBLE( __item_precio_sin_iva,
                                       _TICKET_EXENTO( RAM_P_TICKET - menos - 1 ) );
                }
                else {
                    ADD_MEMORY_DOUBLE( __item_precio_sin_iva,
                                       _TICKET_IMPORTE_SIN_IVA( RAM_P_TICKET - menos - 1 ) );
                }
            }
            #endif
        }
        SET_MEMORY_INT( __ram_present_ant_bak, RAM_PRESENT_ANT );
        SET_MEMORY_INT( __ram_present_ant, -1 );
	}
    else {
        CODIGO_INEXISTENTE();
    }
    if( encontro == 1 && rta != 0 ) {
        /*-------------------- Controla cantidades decimales ------------------*/
        if( RAM_CORRECCION && ITEM_PEDIR_PRECIO ) {
            SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
        }
		{char cad[100];_snprintf(cad, sizeof(cad)-1,"VER Cant: %lf FloorCant: %lf cod: %s ", RAM_CANTIDAD, floor( RAM_CANTIDAD ),ITEM_COD_BARRA );glog(cad,LOG_DEBUG,4);} 
        if( RAM_CANTIDAD != floor( RAM_CANTIDAD )
				&& ( !CANTIDADES_CON_DECIMALES || ( CANTIDADES_CON_DECIMALES == 2 && !ITEM_PESABLE ) ) ) {
            MENSAJE_STRING( S_NO_PUEDO_UTILIZAR_CANTIDAD_TIPO_PRODUCTO );
            rta = 0;
            articulo_x_importe = 0;
        }
        /*------------------- Procesa pesables ------------------*/
		//Si estoy aplicando Diferencia de Precio en un articulo pesable, no solicitar PESO.
		if( ITEM_PESABLE /*&& !ITEM_PEDIR_PRECIO*/ && !RAM_DIFERENCIA_PRECIO ) {
			int art_con_dif = 0;
			//Verificar que al articulo pesable que se esta por anular 
			//no se le haya aplicado la Diferencia de Precio, de ser asi
			//no solicitar que ingrese el peso.
			if( RAM_ANULACION ) {
				art_con_dif = CR1_EXISTE_ITEM_DIF_PRECIO( ITEM_COD_ESPECIAL, ITEM_PRESENTACION, SI,
													 ITEM_COD_BARRA );
			}
			if( !( art_con_dif == -1 ) ) {
				if( RAM_CANTIDAD != 1.0 && !PERMITE_MULTIPLICAR_ITEM_PESABLE ) {
					MENSAJE_STRING( S_NO_MULTIPLICAR_PESABLES );
					encontro = 0;
				} else {
					SET_MEMORY_DOUBLE( __ram_cantidad, RAM_CANTIDAD * CR1_PEDIR_PESO() );
					if( RAM_CANTIDAD ) {
						SET_MEMORY_DOUBLE( __ram_cantidad_original, RAM_CANTIDAD );
						if( config.redondeo_pesables ) {
							//SET_MEMORY_DOUBLE( __ram_cantidad_original, RAM_CANTIDAD );
							if( ITEM_PRECIO ) {
								//pesable_redondeado = REDONDEAR( RAM_CANTIDAD * ITEM_PRECIO, 3, 1 );
								pesable_redondeado = ROUND( RAM_CANTIDAD * ITEM_PRECIO, ENTEROS,
															DECIMALES );
								p_unitario = pesable_redondeado / ITEM_PRECIO;
								SET_MEMORY_DOUBLE( __ram_cantidad, p_unitario );
							}
							else {
								MENSAJE_STRING( S_ARTICULO_SIN_PRECIO );
								rta = 0;
							}
						}
					} else {
						rta = 0;
					}
				}
			}
		}
        #ifdef COMPILAR_PRESENTACIONES
        //---------------- Presentaciones especiales -----------------//
        if( rta && TIPO_PRESENTACIONES == 1 && PRESENTACIONES_HABILITADAS ) {
            int cod = PRECIO_SEGUN_CANTIDAD();
            CR1_ADOPTA_PRESENTACION( cod, NO );
        }
        #endif
		if( rta )
			REDONDEAR_PRECIO2( );
        //REDONDEAR_PRECIO();
        //si es factura recalcula el importe sin iva y el iva en base al total redondeado
        //Forray: verificar tipos de facturas que discriminen iva ya que RECALCULAR_PRECIO_NETO es para las que discriminan iva

		//Cuando la impresora es fiscal no puedo recalcular el precio en grupo ya que 
		//anulo algun articulo del grupo el importe del item de anulacion no coincide con 
		//ninguno del grupo generando una inconsistencia fiscal para las impresoras fiscales
		if( IMPRESORA_FISCAL != FISCAL 
			&& rta 
			&& ( RAM_FACTURA || RAM_TICKET_FACTURA == 1 ) 
			&& !ITEM_PEDIR_PRECIO ) {//REFS 899 no recalcule cuando todavia no lo ingreso
			if( ES_FACTURA_A( clientes.cond_iva ) ) {
				int art_con_dif = 0;

				if( !recalcular_precio_neto_grupo ) {
					//Identifico si se aplico una diferencia de precio al articulo actual
					//sin importar si se anulo o no el articulo. Esto es para evitar el recalculo 
					//de los importes para el articulo en el comprobante.
					art_con_dif = CR1_ITEM_DIF_PRECIO( ITEM_COD_ESPECIAL, ITEM_PRESENTACION, 
								ITEM_COD_BARRA );
				}

				if( recalcular_precio_neto_grupo || !art_con_dif ) {
					//calculo del grupo
					if( RAM_TICKET_FACTURA != 1 && ( RAM_MODO_DEVOLUCION !=_NC_RF_PARCIAL &&
						  RAM_MODO_DEVOLUCION != _NC_RF_INTERME	))
						RECALCULAR_PRECIO_NETO_GRUPO();
				}
            }
        }
        /*------------------- Procesa por importe ------------------*/
		if( rta && ITEM_PEDIR_PRECIO ) {
			if( !RAM_NOTA_CR && RAM_MODO_DEVOLUCION && config.pais == MEXICO ) {
			    double importe = 0.0;
				if( ITEM_PESABLE ) {
					SET_MEMORY_DOUBLE( __ram_cantidad, RAM_CANTIDAD * CR1_PEDIR_PESO(  ) );
				}
				if( fabs( RAM_CANTIDAD ) > 0.0005 ) {
					RECALCULA_ITEM_POR_PRECIO( CR1_PEDIR_PRECIO(  ) );
					if( !ITEM_PRECIO && !ACEPTA_PRECIO_CERO ) {
						encontro = 0;
					}
				} else {
					encontro = 0;
				}
			} else {
				double importe = 0.0;
				if( articulo_pesable ) {
					recalcular = RECALCULAR_PESO_EN_PESABLES;
				} else {
					//Articulos con presentacion no se tiene que recalcular su cantidad.
					ITEM_CANT_PRESENT;
					recalcular = ( !( PRESENTACIONES_HABILITADAS 
										&& RAM_MODO_DEVOLUCION == _NC_TOTAL
										&& ITEM_CANT_PRESENT )? 
										FUNCION_PIDE_PRECIO : 0 );
					//Los Cobros Marginales Kinacu por saldo, son configurado con 
					//articulos pide precio . A estos articulos no se le tiene que 
					//recalcular la cantidad, solo debe tomar el precio que se ingresa con
					//cantidad 1.
					if( ITEM_COD_COBRO ) {
						recalcular = 0;
					}
				}
				if( config_tps.ModificarAlicuotaDeArticuloConsFinal == 1 
					&& Modificar_Alicuota == 1 
					&& ITEM_COD_BAJO ) {
					int _iva=0;
					//Los art. con modificacion en el iva segun condicion fiscal del cliente (consumidor final)
					//No modificaran su cantidad
					recalcular = 0;
					_iva = ALICUOTA_MODIFICADO(ITEM_COD_BAJO,ITEM_COD_BARRA, ITEM_COD_ESPECIAL );
					if( _iva != -1 ) {
						//Recupero el iva original de padron
						SET_MEMORY_CHAR( __item_cod_iva, _iva );
					}
				}
				if( recalcular ) {
					if( ITEM_PRECIO == 0 ) {
						MENSAJE_STRING( S_ARTICULO_SIN_PRECIO );
						encontro = 0;
					}
					else {
						if( RAM_CANTIDAD != 1.0 && !PERMITE_MULTIPLICAR_ITEM_PRECIO ) {
							MENSAJE_STRING( S_NO_MULTIPLICAR_CON_PRECIO );
							encontro = 0;
						}
						else {
							int recal_cant = 1, continuar = 0;
							if( articulo_x_importe ) {
								if( PEDIR_SUPERVISOR_ARTICULO_IMPORT ) {
									if( INGRESAR_CLAVE( 30 - ancho_panel / 2, 21, ST( S_SUPERVISOR___ ),
										2, 0, ( tit ? "Supervisor" : NULL ),
										_F_SUP_AUT_PRECIO, NO, SI ) )
									continuar = 1;
								} else {
									continuar = 1;
								}
								if( continuar ) {
									int recal_item = 0;
									importe = CR1_PEDIR_PRECIO();
									
									if( !( fabs( importe ) > 0.00001 ) ) {
									    SET_MEMORY_DOUBLE( __item_precio, 0.0 );
										if( LASTKEY( )== 27 ) {// no entro precio y salio por esc..
											tecla_escape = SI;
											rta = 0;
										}
									}
									if( !tecla_escape ) { 
										if( config.pais != ELSALVADOR ) {
											double dd = 0, psiva = 0, diff = 0, dcantidad = 0;
											double coef= 0.00;
								
											dd = ITEM_PRECIO;
											if (!( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1 ) )
												ROUND2(&dd,ENTEROS,3); //cuando el importe es tipo 0.999944 si la cantidad es grande
												//produce mucho error ej si importe es 900 da = 1 *( 900 / 0.999944 )
												//vemoas si la correccion modifica mucho el precio sin iva
												SET_MEMORY_DOUBLE( __item_precio, dd ); //almacen x 900.00
												dcantidad = RAM_CANTIDAD;

												if( ITEM_GRAVADO ) {
													if( RAM_NOTA_CR == 2 ) {
														SET_MEMORY_DOUBLE( __ram_cantidad,
														RAM_CANTIDAD * ( importe
															/ ( dd )) );
													} else {
														double importe_provi = 0.00;
														double cantidad_provi =0.00;
														double total_provi =0.00;
														double auxiliar=0.0;
														double cant_orig=0.0;
														
														auxiliar=ITEM_PRECIO_SIN_IVA ;

														coef = ( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT ) * (1+  iva[ITEM_COD_IVA].porc_iva1 /100 );
														if( RAM_TICKET_FACTURA != 1 && RAM_NOTA_CR != 1 )
														ROUND2(&coef, ENTEROS, 3);
														
														//aca debemos ver si el importe ingresado va a dar con loq ue voy a calcular, con diferencia de > 0.01
														//TENGO QUE OBTENER 41.35 

														auxiliar=RAM_CANTIDAD ;
														cant_orig=RAM_CANTIDAD ;
														auxiliar=importe ;
														auxiliar=RAM_CANTIDAD * ( importe
															/ ((coef) + ITEM_IMP_INT ));
														SET_MEMORY_DOUBLE( __ram_cantidad,
															RAM_CANTIDAD * ( importe
															/ ((coef) + ITEM_IMP_INT ))) ;
														//vemos si podemos tener igual el importe
														//todo este lio es para acomoda el importe del ticket al ingresado por codigo de barra del articulo por importe
														//ej 2081863064901 importe es 64.90
														cantidad_provi =RAM_CANTIDAD;
														ROUND2(&cantidad_provi, ENTEROS, 3);//calculemos como lo haria la printer. Trabajemos con 3 decimales la cantidad 1.338
														importe_provi= (coef * cantidad_provi )+ ITEM_IMP_INT; 
														
														// este es el original pero  trae errores al multiplicar.. 
														//importe_provi = importe / cantidad_provi;

														// este corrige el error pero da redonde en cantidad 1
														//importe_provi = importe_provi / cantidad_provi;
														
														//se verifica una opcion de al calculo correcto unitario multiplicarlo
														importe_provi = (importe * cant_orig)/ cantidad_provi;

														ROUND2(&importe_provi, ENTEROS,3);//saco cuanto deberia ser el precio que se debe mandar a la printer 48.505 para que de exacto
																		
														SET_MEMORY_DOUBLE( __item_precio, importe_provi /*importe*/);
														SET_MEMORY_DOUBLE( __item_precio_sin_iva, importe_provi /( 1+(iva[ITEM_COD_IVA].porc_iva1 / 100.0 )));
														total_provi = importe_provi;
														importe_provi = importe_provi -ITEM_PRECIO_SIN_IVA; //saco el iva
														SET_MEMORY_DOUBLE( __item_iva1_precio, importe_provi ); //
														//todo esto que es lo que sigue es por si necesito usar mas de 2 decimales en el precio con la impresora. Lo dejo por las dudas
														/*ROUND2(&total_provi, ENTEROS,2);
														total_provi =  total_provi * cantidad_provi;
														ROUND2(&total_provi, ENTEROS,2);
														importe_provi = importe;
														ROUND2(&importe_provi, ENTEROS,2);
														cantidad_provi = fabs(importe_provi-total_provi); //comparo el total ingresado con el que dara la printer vs caja 64.91 vs 64.90
														if( cantidad_provi >0.009 && cantidad_provi < 0.04 )
															usar_tres_decimales = SI;
														else
															usar_tres_decimales = NO;
															glog("se paso", LOG_VARIOS,1);

//														}*/

										
													}
												} else {
													SET_MEMORY_DOUBLE( __ram_cantidad,
														RAM_CANTIDAD * ( importe
														/ ITEM_PRECIO ) );												
												}
												{    char mensa[45];
													  sprintf(mensa,"Ingr imp %lf Cantidad %lf", importe, RAM_CANTIDAD);
													  glog(mensa, LOG_VENTAS,4);
												}
												if( ITEM_GRAVADO 
														&& ( RAM_NOTA_CR == 2 || RAM_TICKET_FACTURA == 2 )
														&& IMPRESORA_FISCAL == FISCAL ) {
													double import, iva;
													if( RAM_NOTA_CR == 2) {
														dd= RAM_CANTIDAD;
														ROUND2(&dd,ENTEROS,3);
														SET_MEMORY_DOUBLE( __ram_cantidad, dd);
														psiva = ITEM_PRECIO * RAM_CANTIDAD;
													} else { 
														import = ROUND( RAM_CANTIDAD * ITEM_PRECIO_SIN_IVA, ENTEROS , 3);
														iva = ROUND( RAM_CANTIDAD * ITEM_IVA1_PRECIO, ENTEROS , 8);
														psiva = import + iva;
													}
													
												
												} else
													psiva = RAM_CANTIDAD * ( ITEM_PRECIO_SIN_IVA + ITEM_IVA1_PRECIO ); //899.9496

												diff= fabs( ( importe * dcantidad ) - psiva);
												
												if( diff > 0.009 
														&& ( RAM_NOTA_CR == 2 /*|| RAM_TICKET_FACTURA == 2*/ ) ) { //recalculemos cantidad
   												   //ajustamos LA CANTIDAD
												   //dcantidad = importe / ( ITEM_PRECIO_SIN_IVA *( 1+  iva[ITEM_COD_IVA].porc_iva1 /100 ));
												   dcantidad = importe / 
														( (( ITEM_PRECIO_SIN_IVA - ITEM_IMP_INT ) 
															* ( 1+  iva[ITEM_COD_IVA].porc_iva1 /100 )) 
																+ ITEM_IMP_INT)  ;
												
												   ROUND2(&dcantidad,ENTEROS,4); 
												} else {
													double auxiliar = 0.00;
													dcantidad = RAM_CANTIDAD;
													ROUND2(&dcantidad,ENTEROS,3); 
												}
												SET_MEMORY_DOUBLE( __ram_cantidad, dcantidad );
												{    char mensa[45];
													  sprintf(mensa,"Cantidad final %lf", RAM_CANTIDAD);
													  glog(mensa, LOG_VENTAS,4);
												}
										} else {
											/*if( ( config.pais == ELSALVADOR ) || (( RAM_MODO_DEVOLUCION || RAM_NOTA_CR )
												&& PRECIO_UNITARIO_EN_DEVOLUCIONES && !RAM_DEVOLUCION_PARCIAL_REF)) {*/
											if( ( RAM_MODO_DEVOLUCION || RAM_NOTA_CR ) && ( config.pais == ELSALVADOR )
													&& PRECIO_UNITARIO_EN_DEVOLUCIONES && !RAM_DEVOLUCION_PARCIAL_REF ){
												importe *= RAM_CANTIDAD;
												recal_cant = 0;
											}
											if( recal_cant ) {
												float porc_iva;
												double temp_new_cant = 0.0;
												int signo = 1;

												if( articulo.gravado == 'S'
														&& EXENTO_PARA_DIPLOMATICOS()
														&& config.pais == ELSALVADOR && !RAM_CAMBIO ) {
													//if( articulo.cod_iva ) { //se comenta porque no estï¿½ usando actualmente
													porc_iva = iva[articulo.cod_iva].porc_iva1 / 100;
													//} else {
													//  porc_iva = config.porc_iva1 / 100;  //configurar config.porc_iva1
													//}
												} else {
													porc_iva = 0.0;
												}
													
												if( porc_iva ) {
													temp_new_cant = fabs( RAM_CANTIDAD * ( importe / (ITEM_PRECIO_SIN_IVA + ITEM_PRECIO_SIN_IVA * porc_iva ) ) );
												} else {
													if( articulo.gravado == 'S' && iva[articulo.cod_iva].porc_iva1 > 0.01  ){
														temp_new_cant = fabs(RAM_CANTIDAD *( ( importe /( 100 + iva[articulo.cod_iva].porc_iva1 )* 100 ) / ITEM_PRECIO_SIN_IVA ));
													} else {
														temp_new_cant = fabs( RAM_CANTIDAD * ( importe / ITEM_PRECIO ) );
													}
												}
												signo = ( RAM_CANTIDAD < 0.0 ) ? -1 : 1;
												SET_MEMORY_DOUBLE( __ram_cantidad,
														signo * temp_new_cant );
											}
											if( ( CR1_ITEM_GRAVADO() && _ITEM_COD_IMPUESTO( 2 ) != 0 ) 
													|| RAM_CAMBIO 
													|| ( RAM_MODO_DEVOLUCION 
														&& !( articulo.gravado == 'S' && EXENTO_PARA_DIPLOMATICOS() /*&& config.pais == ELSALVADOR*/ ) ) 
													|| ( RAM_MODO_DEVOLUCION && !FACT_INDEP_DEL_TIPO_DE_CLIENTE/*config.pais != ELSALVADOR*/ ) 
													|| !recal_cant ) {
												recal_item = 1;
											}
										}
										if( ( CR1_ITEM_GRAVADO() && _ITEM_COD_IMPUESTO( 2 ) != 0 ) || recal_item ) {
											RECALCULA_ITEM_POR_PRECIO( importe / RAM_CANTIDAD );
										}
									}
									/*}
									else {
										RECALCULA_ITEM_POR_PRECIO( CR1_PEDIR_PRECIO() );
										if( !ITEM_PRECIO && !ACEPTA_PRECIO_CERO ) {
											encontro = 0;
										}
									}*/
								} else {
									SET_MEMORY_DOUBLE( __ram_cantidad,
                                                   RAM_CANTIDAD * ( importe / ITEM_PRECIO ) );
								}
							}
						}
					}
				}
				else {
					#ifdef INVEL_L
					RECALCULA_ITEM_POR_PRECIO( CR1_PEDIR_PRECIO() );
					#else
					RECALCULA_ITEM_POR_PRECIO( CR1_PEDIR_PRECIO() );
					#endif
					if( !ITEM_PRECIO && !ACEPTA_PRECIO_CERO ) {
						encontro = 0;
					}
				}
			}
		}
        /*---------------------- Controla que tenga precio --------------------*/
		if( rta && encontro ) {
            if( !ITEM_PRECIO && !ACEPTA_PRECIO_CERO ) {
                MENSAJE_STRING( S_ARTICULO_SIN_PRECIO );
                encontro = 0;
            }
            if( fabs( ITEM_PRECIO ) > 0.004 && RAM_PEDIR_NOVIOS ) {
                MENSAJE_STRING( S_DEBE_SER_ARTICULO_SIN_PRECIO );
                encontro = 0;
            }
        }
    }
	MOSTRAR_PETICION_DE_PRESENTACIONES( presentaciones );
    if( encontro == -1 ) {
        MENSAJE_STRING( S_NO_DISPONIBLE_PRESENTACION1 );
        encontro = 0;
    }
    if( rta == 0 ) {
        encontro = 0;
    }
    usar_cadena_scanner = 0;
    mantener_cod_barra = 0;

    return ( encontro );
}

/*****************************************************************************/
int CR1_EJECUTA_FUNCION( int funcion )
/*****************************************************************************/
{
    int _ptr, _handle, _fun, rta = 1, ok = 0;
//	int autorizo_consulta;
    char aux[95];
    char *nfun[] = {"","Consulta ticket","Autoriza anulaciones","Activa anulacin","Anula ticket",
                    "Fondo fijo","Informes","Adelantos","Retiros","Valida tarjeta","Devoluciones",
                    "Pedir cdigo","Pedir presentaciones","Hab/des scanner","Stand By","Factura",
                    "Cobros","Pagos x caja","Elegir cliente alfabtico","Elegir cliente numrico",
                    "Factura de cooperativa","Reimprimir factura","Cliente afinidad",
                    "Trae venta pendiente","Pide remito","Ejecuta programa externo","Pide vendedor",
                    "Nota de crdito","Inventario","Reprocesar pago","Tecla total","Pide nro rollo",
                    "Habilita precio crdito","Facturacin de remitos","Reimpresion de remitos",
                    "Ingresa mozo","Consulta saldo tarjeta","Ingresa mutual","Ticket en espera",
                    "Cambio","Imprime subtotal","Ingresa perfil","Descuento %","Descuento $",
                    "Consulta estado tarjeta","Cobro CMR","Tecla novios","Trae venta del CUS",
                    "Abrir cajn","Descuento $ 2","Descuento $ 3","Mostrar cajones habilitados",
                    "Pide datos tarjeta","Adelanto CMR","Revalidacin Cobros","Cliente en Cheques",
                    "Reimprimir ticket","Reimprimir voucher","Fondo Fijo Multiple",
                    "Descuento fijo % 1","Descuento fijo % 2","Descuento fijo % 3",
                    "Descuento fijo % 4","Descuento fijo % 5","Arqueo Cajero","Modo Entrenamiento",
                    "Cobros Varios","Consulta Promociones","Reimpresion Fac-NC",
                    "Modalidad Bien de Uso","Reimprimir Val Medios Pago","Sub-Total descuento 1",
                    "Sub-Total descuento 2","Sub-Total descuento 3","Sub-Total descuento 4",
                    "Sub-Total descuento 5","Importacion Ticket","Desc1","Desc2",
                    "Cliente Afinidad Documento","Reimpresion de Ticket Remoto",
                    "Descuento $ ultimo articulo","Cambio de Medios","Devolucion e Importacion",
                    "Descuento % ultimo articulo","Consulta tarjeta","Verif. Cliente",
                    "Trasferencias","Legajo empleado","Consulta Habilitacion para Cheque",
                    "De cheques en boletin","Anulacion Cobro","Ident. Cliente","Referencia NC",
                    "Reimprimir NC o Factura","Factura A","Factura B","Alta de Clientes",
                    "Data Entry","TF","","","","","","","Devolucion Cobros","",
                    "Elegir Cliente Cod. Barra","PagaTodo Recarga","PagaTodo Venta PIN",
                    "Pagatodo Servicios","Monedero Electronico","Cobro Garantia","Fraqueo",
					"Devolucion Parcial Referenciada","Nota de credito Libre",
                    "Nota de credito Referenciada Parcial","Nota de credito Referenciada Intermedia",
					"Nota de credito Total",
                    "Devolucion Libre", "Devolucion Referenciada Parcial", "Devolucion Referenciada Intermedia",
                    "Devolucion Total", "Ticket en espera con Menu","Consulta Prom. Activas","Importacion En TF o Fact", 
					"Cambios de Medios Automatico", NULL};

    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    //_fun = STACK_INS( sizeof( int ) );
    _fun = STACK_INS( sizeof( short ) );
    if( GET_MEMORY_INT( _fun ) ) {
        funcion = GET_MEMORY_INT( _fun );
    }
    else {
        SET_MEMORY_INT( _fun, funcion );
    }

    _snprintf( aux, sizeof(aux)-1, "Funcin %i (%s)", funcion, nfun[funcion] );
    //  sprintf(aux, "Funcin %i (%s)", funcion, nfun[0]);
	#ifdef COMPILAR_LOGS_VIA_TCP
   LOGS_VIA_TCP(aux );
   #endif
    GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,1);
    GRABAR_LOG_MEMORIA();
    ClearPanel( pPanel );
    DISABLE_STANDBY_AUTOMATICO();
	if( funcion != -999 )
		BLOQUEAR_DATOS = SI;
    switch( funcion ) {
        case 1:
            /*  Consulta ticket  */
            CONSULTA_TICKET();
            break;
        case 2:
            /*  Autoriza anulaciones  */
            CR1_AUTORIZA_ANULACIONES();
            break;
        case 3:
            /*  Activa anulacin  */
            CR1_ACTIVA_ANULACION();
            break;
        case 4:
            /*  Anula ticket  */
            if( SCANNER_LIBRE() ) {

                PEDIR_MOTIVO_OPERACION_NEGATIVA( NULL, _MOTIVO_ANUL_TICKET );
				TECLA_ANULA_TICKET( NO );
                /*----------------- motor ----------------------------*/
				if( RAM_ANULAR_TICKET != _ANUL_CANCELADA && UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ) {
					reinit_promos();
				}
				SET_MEMORY_CHAR( __ram_anular_ticket, 0 );
                /*----------------- motor ----------------------------*/
            } else {
                MENSAJE_STRING( S_QUEDAN_ARTICULOS_EN_SCANNER );
            }
			/***********************Borra Marginales************************/
			//BORRAR_ARCHIVOS_COBROS_VENTA( NO);
			break;
        case 5:
            /*  Fondo fijo  */
			grabar_copia_ticket = 0;
            CR1_FONDO_FIJO();
            break;
        case 6:
            /*  Informes  */
            CR1_INFORMES();
            break;
        case 7:
            /*  Adelantos  */
            CR1_ADELANTOS();
            break;
        case 8:
            /*  Retiros  */
			grabar_copia_ticket = 0;
            CR4_RETIROS();
            break;
        case 9:
            /*  Valida tarjeta  */
            CR1_VALIDA_TARJETA( SI, 0 );
            if( CEREAR_TARJETA_DESPUES_DE_VALIDA ) {
                CEREAR_STRUCT( _var_tarjetas );
                CR1_MUESTRA_TARJETA();
                if( VAR_TARJETAS_STATUS ) {
                    GUI_MUESTRA_TARJETA( T_DATOS_TARJETA_BORRAR );
                }
            }
            break;
        case 10:
            /*  Devoluciones  */
            CR1_INGRESA_DEVOLUCIONES();
            break;
        case 11:
            /*  Pedir cdigo  */
            SWITCH_PETICION_DE_CODIGO();
            break;
        case 12:
            /*  Pedir presentaciones  */
            CR1_PETICION_DE_PRESENTACIONES();
            break;
        case 13:
            /*  Hab/des scanner  */
            CR1_HABILITA_SCANNER(SI);
            break;
        case 14:
            /*  Stand By  */
            STAND_BY( 0, _MODO_VENTA );
            break;
        case 15:
            /*  Factura  */
            if( RAM_MODO_EJECUCION != INVENTARIO )
				CR1_FACTURA( 0, RAM_TIPO_COMP_ANTERIOR );
            break;
        case 16:
            /*  Cobros  */
            COBROS( RAM_MODO_DEVOLUCION, _MODO_INTERACTIVO );
            break;
        case 17:
            /*  Pagos x caja  */
            if( MODO_NEGOCIO != SERVICIOS ) {
				grabar_copia_ticket = 0;
                PAGOS_POR_CAJA();
            }
            break;
        case 18:
            /*  Elegir cliente alfabtico  */
            ELEGIR_CLIENTE_ALFABETICO( 0, SI );
            break;
        case 19:
            /*  Elegir cliente numerico  */
			CEREAR_PERFIL_CLIENTE();
			if( GetConfig( CONF_TOUCH_SCREEN ) ){
				/* Informo al touch screen un cambio de pantalla */
				ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "20" );
			}
            ELEGIR_CLIENTE_NUMERICO( 0, 1 );
			if( GetConfig( CONF_TOUCH_SCREEN ) ){
				/* Informo al touch screen un cambio de pantalla */
				ExecuteSAction( A_CAMBIO_PANTALLA_TOUCH, "01" );
			}

            break;
        case 20:
            /*  Factura de cooperativa  */
            COOP_FACTURA_SOCIOS();
            break;
        case 21:

            break;
        case 22:
            /*  Cliente afinidad  */
            PEDIR_CLIENTE_AFINIDAD( NO );
            break;
        case 23:
            /*  Trae venta pendiente  */
            #ifdef COMPILAR_PENDIENTES
            PEDIR_VENTA_PENDIENTE();
            #endif
            break;
        case 24:
            /*  Pide remito  */
            #ifdef COMPILAR_REMITOS
            PEDIR_REMITO();
            #endif
            break;
        case 25:
            /*  Ejecuta programa externo  */
            EJECUTAR_EXTERNOS();
            break;
        case 26:
            /*  Pide vendedor  */
            PEDIR_VENDEDOR();
            break;
        case 27:
            /*  Nota de crdito  */
            CR1_FACTURA( 1, RAM_TIPO_COMP_ANTERIOR );
            break;
        case 28:
            /*  Inventario  */
            PEDIR_TIPO_INVENTARIO( SI, SI );
            break;
        case 29:
            /*  Reprocesar pago  */
            #ifdef COMPILAR_TECLA_REPROCESAR
            if( !IMPRESORA_FISCAL ) {
                TECLA_REPROCESAR();
            }
            #endif
            break;
        case 30:
            GRABAR_LOG_SISTEMA( "Tecla total" ,LOG_VENTAS,1); //Log Borrar
            /*  Tecla total  */
            if( SCANNER_LIBRE(  ) )
            {
                if( RAM_MODO_EJECUCION == INVENTARIO ) {
                    FIN_INVENTARIO();
                }
                else if( RAM_MODO_EJECUCION == TRANSFERENCIAS ) {
                    FIN_TRANSFERENCIA( RAM_TOTAL, TRANSFERENCIAS );
                    #ifdef COMPILAR_REMITOS
                }
                else if( EMITIR_REMITO() ) {
                    FIN_REMITO();
                    #endif
                }
                else {
					if( IMPRIME_SUBTOTAL_AL_PASAR_A_TOTA ) {
                        IMPRIME_SUBTOTAL( RAM_TOTAL, NO );
                    }
                    TECLA_PAGO();
                    /**/glog ("********************** FIN DEL TICKET ********************************************" ,LOG_VENTAS,2);
					SET_MEMORY_CHAR( __ram_importacion_pedidos, 0 );
					ExecuteAction( A_VENTAS_FOCUS );					
					#ifdef INVEL_W
						//SETEA_FOCUS_TPS( );
					#endif INVEL_W
                }
            } else
                    MENSAJE_STRING( S_QUEDAN_ARTICULOS_EN_SCANNER );
            break;
        case 31:
            /*  Pide nro rollo  */
            PEDIR_NRO_ROLLO_INTERNO();
            break;
        case 32:
            /*  Habilita precio crdito  */
            #ifdef COMPILAR_TECLA_MODO_CREDITO
            HABILITAR_PRECIO_CR();
            #endif
            break;
        case 33:

            break;
        case 34:

            break;
        case 35:
            /*  Ingresa mozo  */
            #ifdef COMPILAR_MOZO
            PEDIR_MOZO();
            #endif
            break;
        case 36:
            /*   Consulta saldo tarjeta  */
            CONSULTA_SALDO_TARJETA();
            break;
        case 37:
            /*  Ingresa mutual  */
            #ifdef COMPILAR_MUTUALES
            INGRESAR_MUTUAL();
            #endif
            break;
        case 38:
            /*  Ticket en espera  */
			PROCESAR_TICKET_EN_ESPERA( _TE_TRADICIONAL );
            break;
        case 39:
            /*  Cambio Mercaderia  */
            #ifdef COMPILAR_TECLA_CAMBIO
            TECLA_CAMBIO();
            #endif
            break;
        case 40:
            /*  Imprime subtotal  */
            IMPRIME_SUBTOTAL( RAM_TOTAL, NO );
            break;
        case 41:
            /*  Ingresa perfil   */
            #ifdef COMPILAR_PERFILES
            INGRESA_PERFIL();
            #endif
            break;
        case 42:
            /*  Descuento %  */
            AGREGAR_DESCUENTO_PORCENTUAL();
            break;
        case 43:
            /*  Descuento $  */
            AGREGAR_DESCUENTO_IMPORTE( 1 );
            break;
        case 44:
            /*  Consulta estado tarjeta  */
            CONSULTA_ESTADO_TARJETA();
            break;
        case 45:
            /*  Cobro CMR  */
            #ifdef COMPILAR_CMR
            COBRO_CMR();
            #else
            MENSAJE_SIN_CMR();
            #endif
            break;
        case 46:
            /*  Tecla novios  */
            #ifdef COMPILAR_NOVIOS
            TECLA_NOVIOS();
            #endif
            break;
        case 47:
            /*  Trae venta del CUS  */
            #ifdef COMPILAR_CUS
            TRAE_VENTA_DEL_CUS();
            #endif
            break;
        case 48:
            /*  Abrir cajn  */
            CR1_ABRIR_CAJON();
            break;
        case 49:
            /*  Descuento $ 2  */
            AGREGAR_DESCUENTO_IMPORTE( 2 );
            break;
        case 50:
            /*  Descuento $ 3  */
            AGREGAR_DESCUENTO_IMPORTE( 3 );
            break;
        case 51:
            /*  Mostrar cajones habilitados  */
            CR0_MOSTRAR_CAJONES_HABILITADOS( NO );
            break;
        case 52:
            /*  Pide datos tarjeta  */
            if( VAR_TARJETAS_STATUS > 0 ) {
                PIDE_DATOS_TARJETA( NO, medios[_medio_tarjeta].nro );
                CR1_MUESTRA_TARJETA();
            }
            break;
        case 53:
            /*  Adelanto CMR  */
            #ifdef COMPILAR_CMR
            ADELANTO_CMR();
            #else
            MENSAJE_SIN_CMR();
            #endif
            break;
        case 54:
            /*  Revalidacin Cobros  */
            #ifdef COMPILAR_REVALIDAR_COBROS
            REVALIDAR_COBROS_VARIOS();
            #endif
            break;
        case 55:
            /*  Cliente en Cheques  */
            ELEGIR_CLIENTE_NUMERICO( 1, 1 );
            break;
        case 56:
            /*  Reimprimir ticket  */
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_COPIA_TICKET ),
                                  ST( S_COPIA_TICKET ), RAM_COD_FUNCION, SI ) ) {
                IMPRIME_COPIA_TICKET( SI );
            }
            break;
        case 57:
            /*  Reimprimir voucher  */
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_COPIA_VOUCHER ),
                                  ST( S_COPIA_VOUCHER ), RAM_COD_FUNCION, SI ) ) {
					if( IMPRIME_COPIA_VOUCHER( SI ) == NO ){
						GRABA_EVENTO_SALIDA_CAJERO(  );
					}
            }
            break;
        case 58:
            /*   Fondo Fijo Multiple  */
			grabar_copia_ticket = 0;
            CR1_FONDO_FIJO_MULT_MEDIOS( NO );
            break;
        case 59:
            /*  Descuento fijo % 1  */
            TECLA_DESCUENTO_PORCENTUAL( 1 );
            break;
        case 60:
            /*   Descuento fijo % 2  */
            TECLA_DESCUENTO_PORCENTUAL( 2 );
            break;
        case 61:
            /*  Descuento fijo % 3  */
            TECLA_DESCUENTO_PORCENTUAL( 3 );
            break;
        case 62:
            /*  Descuento fijo % 4  */
            TECLA_DESCUENTO_PORCENTUAL( 4 );
            break;
        case 63:
            /*  Descuento fijo % 5  */
            TECLA_DESCUENTO_PORCENTUAL( 5 );
            break;
        case 64:
            /*  Arqueo Cajero  */
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_ARQUEO ),
                                  ST( S_ARQUEO ), RAM_COD_FUNCION, SI ) ) {
				ABRIR_CAJON();
                ARQUEO();
            }
            break;
        case 65:
            /*  Modo Entrenamiento  */
            #ifdef COMPILAR_MODO_ENTRENAMIENTO

			if(  RAM_MODO_EJECUCION != ENTRENAMIENTO 
					&& IMPRESORA_FISCAL == FISCAL ) {
				MENSAJE_TECLA( ST( S_ATENCION_CONFIGURADA_IMPRESORA_FISCAL_Pres_Tecla ) );
				if( MENSAJE_CON_TECLA_S_N
					( ST( S_DESEA_CONTINUAR_MODALIDAD_ENTRENAMIENTO_ENTER_ESC ),
					13,27 ) == 27 ) {
					rta = 0;         
					break;			
				}
			}

            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_ENTRENAMIENTO ),
						ST( S_ENTRENAMIENTO ), RAM_COD_FUNCION, SI ) ) {
                CLOSE_RECUPERO( _ptr, _handle );
                if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
                    SET_ENTRENAMIENTO_OFF();
					LOAD_PANTALLA_ENTR(0);
                    OPEN_RECUPERO( &_ptr, &_handle );
                    //              _fun = STACK_INS( sizeof( int ) );
                    _fun = STACK_INS( sizeof( short ) );
                }
                else {
                    SET_ENTRENAMIENTO_ON();
					LOAD_PANTALLA_ENTR(1);
                    OPEN_RECUPERO( &_ptr, &_handle );
                    //_fun = STACK_INS( sizeof( int ) );
                    _fun = STACK_INS( sizeof( short ) );
                }
			}
            else {
                rta = 0;
            }
            #endif
            //Se setea area_articulo porque pierde su valor al entrar a MODO ENTRENAMIENTO
            //area_articulo = AREA_ARTICULOS;
            /*if( UTILIZAR_ARTICULOS_REMOTO && !USED( AREA_ARTICULOS_REMOTO ) ) {
                area_articulo = AREA_ARTICULOS_REMOTO;
            }*/
            break;
        case 66:
            /*  Cobros Varios  */
            COBROS_VARIOS();
            break;
        case 67:
            /*  Consulta Promociones  */
            #ifdef COMPILAR_CONSULTA_ARTICULO
			if( SUPERVISOR_EN_CONSULTA_ARTICULOS ) {
			/*-------------- Verifica si pedir supervisor ----------------*/
				if( !( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_ ), "Autoriz¢ ",
						RAM_COD_FUNCION ) ) ) {
					autorizo_consulta = 0;
					MENSAJE_STRING( S_NO_SE_AUTORIZA_CONSULTA );
					PAUSA( 9 );
					BORRAR_MENSAJE(  );
				}
			} else
				autorizo_consulta = 1;
				if( autorizo_consulta )
					CONSULTA_DE_ARTICULO( SI );
            break;
			#endif
        case 68:
            /*  Reimpresion Fac-NC  */
            break;
        case 69:
            /*  Modalidad Bien de Uso  */
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI,
                                  PEDIR_SUPERVISOR_BIEN_DE_USO && !RAM_BIEN_DE_USO,
                                  ST( S_BIEN_DE_USO ), ST( S_BIEN_DE_USO ), RAM_COD_FUNCION, SI ) ) {
                MODALIDAD_BIEN_DE_USO();
            }
            break;
        case 70:
            /*  Reimprimir Val Medios Pago  */
            REIMPRIMIR_VALIDACIONES_MEDIOS_DE_PAGO();
            break;
        case 71:
            /*  Sub-Total descuento 1  */
            IMPRIMIR_BASE_DESCUENTO( 1 );
            break;
        case 72:
            /*  Sub-Total descuento 2  */
            IMPRIMIR_BASE_DESCUENTO( 2 );
            break;
        case 73:
            /*  Sub-Total descuento 3  */
            IMPRIMIR_BASE_DESCUENTO( 3 );
            break;
        case 74:
            /*  Sub-Total descuento 4  */
            IMPRIMIR_BASE_DESCUENTO( 4 );
            break;
        case 75:
            /*  Sub-Total descuento 5  */
            IMPRIMIR_BASE_DESCUENTO( 5 );
            break;
        case 76:
            /*  Importacion Total de Ticket  */
            PEDIR_MOTIVO_OPERACION_NEGATIVA( NULL, _MOTIVO_DEVOL_TOTAL );
            SET_MEMORY_INT( __ram_modo_devolucion, _DEVOL_TOTAL );
         	SET_MEMORY_CHAR( __ram_devolucion_parcial_ref, NO );
            if( IMPORTAR_TICKET( SI, 0, NO, ST( S_IMPORTAR_TICKET ), NO, 0, _NO_APLICA , SI, NO ) ) {
                if( config.pais != ECUADOR && config.pais != CHILE && config.pais != BRASIL ) {
                    SET_MEMORY_CHAR( __varios_restringir_volver_de_pagos, SI );
                    importando_ticket_devolucion = 1;
                    UNGETCH( 32 );
                }
            } else {
                SET_MEMORY_INT( __ram_modo_devolucion, _DEVOL_LIBRE );
                SET_MEMORY_CHAR( __ram_devolucion_parcial_ref, NO );
                SET_MEMORY_CHAR( __ram_motivo_devolucion, NO );
            }
            break;
        case 77:
            /*  Desc1  */
            break;
        case 78:
            /*  Desc2  */
            break;
        case 79:
            /*  Cliente Afinidad Documento  */
            PEDIR_CLIENTE_AFINIDAD( SI );
            break;
		case 80:{
			int respuesta=NO;
            /*  Reimpresion de Ticket Local  */
            //REIMPRIMIR_TICKET_REMOTO();
				if( INGRESAR_CLAVE( 30 - ancho_panel / 2, 21, ST( S_SUPERVISOR___ ), 2, 0,
										"REIMPRIMIR TICKET", _F_SUP_CIERRE, NO, SI ) ) {
					if( ESPERAR_LLAVE( 'S' ) ) {
						grabar_copia_ticket = 0;
						respuesta = REIMPRIMIR_TICKET_LOCAL("ticket.prn");
					}
					if( respuesta == NO ){
						GRABA_EVENTO_SALIDA_CAJERO(  );
					}
				}
			}
			break;
        case 81:
            /*  Descuento $ ultimo articulo*/
            AGREGAR_DESCUENTO_ULTIMO_ARTICULO( SI );
            break;
        case 82:
            /*  Cambio de Medios  */
			grabar_copia_ticket = 0;
            CAMBIO_MEDIOS( NO );
            break;
        /* Codigo obsoleto
        case 83:
            /*  Devolucion e Importacion
            ok = 0;
            if( !NC_PERMANENTE || ( NC_PERMANENTE && RAM_NRO_CLIENTE && RAM_NOTA_CR ) ) {
                ok = 1;
            }
            else {
                if( NC_PERMANENTE ) {
                    MENSAJE( ST( S_DEBE_INGRESAR_CLIENTE ) );
                }
            }
            if( ok && SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_DEVOLUCION_ ),
                    ST( S_AUTORIZO_DEVOLUCIONES ), RAM_COD_FUNCION, SI ) ) {
                ok = 1;

                if( PEDIR_MOTIVO_EN_DEVOLUCIONES ) {
                    SET_MEMORY_CHAR( __modo_devolucion, SI );
                    SET_MEMORY_INT( __ram_motivo_devolucion, ELEGIR_MOTIVO( NULL ) );
                    SET_MEMORY_CHAR( __modo_devolucion, NO );
                    if( RAM_MOTIVO_DEVOLUCION <= 0 ) {
                        ok = 0;
                    }
                }
				if( ok && !RAM_MODO_DEVOLUCION && PEDIR_DATOS_EN_DEVOLUCIONES
						&& ( !PEDIR_CAJA_Y_SECUENCIA || config.pais == MEXICO ) ) {
					ok = PEDIR_BOLETA_ANTERIOR( );
					if( ok && RAM_BOLETA_ANTERIOR <= 0 && RAM_CAJA_ANTERIOR <= 0
							&& RAM_FECHA_ANTERIOR <= 0 ) {
						ok = 0;
					}
                }
				if( ok && !RAM_MODO_DEVOLUCION ) {
					int tipo_evento_req = 0;

					if( !FUNCION_PIDE_PRECIO ) {
         				SET_MEMORY_INT( __ram_negativo, SI );
         			}
					SET_MEMORY_INT( __ram_modo_devolucion, _NC_TOTAL );
         			SET_MEMORY_CHAR( __modo_devolucion, SI );
         			SET_MEMORY_CHAR( __ram_devolucion_parcial_ref, NO );
					if( RAM_TIPO_COMP_ANTERIOR == NC_ORIGEN_TICKET ) {
         		   		tipo_evento_req = 0;
         		   	} else if( RAM_TIPO_COMP_ANTERIOR == NC_ORIGEN_TF ) {
         		   		tipo_evento_req = 35;
         		   	}
					MOSTRAR_MODO_DEVOLUCION( );
					if( IMPORTAR_TICKET( SI, tipo_evento_req, NO, ST( S_IMPORTAR_TICKET ), NO, _ORIGEN_DEVOLUCION, _NO_APLICA ) ) {
                        if( config.pais != ECUADOR && config.pais != CHILE
                         && config.pais != BRASIL ) {
                            SET_MEMORY_CHAR( __varios_restringir_volver_de_pagos, SI );
                            importando_ticket_devolucion = 1;
                            UNGETCH( 32 );
                        }
                        if( ( config.pais == CHILE || config.pais == BRASIL ) && NC_PERMANENTE
                         && CONTROL_NOTA_DE_CREDITO ) {
                            if( IMPORTAR_ITEMS() == 0 ) {
                                SET_MEMORY_CHAR( __ram_referencia_nc, 1 );
                            }
                        }
                    }
                    else {
                        SET_MEMORY_INT( __ram_modo_devolucion, NO );

                        SET_MEMORY_CHAR( __modo_devolucion, NO );
                        MOSTRAR_MODO_DEVOLUCION();
                    }
                }
            }
            break;
            */
        case 84:
            /*  Descuento % ultimo articulo  */
            AGREGAR_DESCUENTO_ULTIMO_ARTICULO( NO );
            break;
        case 85:
            //while(1)
            /*  Consulta tarjeta  */
            CONSULTA_SALDO_TARJETA_CREDITO();
            break;
        case 86:
            /*  Verif. Cliente  */
            VERIFICA_EMPADRONAMIENTO_CLIENTES( SI );
            break;
        case 87:
            /*  Trasferencias  */
            PEDIR_TRANSFERENCIA();
            break;
        case 88:
            /*  Ingresa Legajo Empleado  */
            #ifdef COMPILAR_PERFILES
            if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, PEDIR_CLAVE_EN_LEGAJO_EMPLEADO,
                                  ST( S_CLAVE ), NULL, RAM_COD_FUNCION, SI ) ) {
                if( !INGRESA_NRO_LEGAJO_EMPLEADO() ) {
                    if( USAR_LEGAJO == RUT ) {
                        MENSAJE_STRING( S_NUMERO_DE_RUT_ERRONEO );
                    }
                    else {
                        MENSAJE_STRING( S_ERROR_EN_NRO_LEGAJO );
                    }
                }
            }
            #endif
            break;
        case 89:
            /*  Consulta Habilitacion para Cheque  */
            if( !RAM_P_TICKET ) {
                VERIFICAR_CLIENTE_HABILITADO_PARA_CHEQUES();
            }
            break;
        case 90:
            /*  De cheques en boletin  */
            if( !RAM_P_TICKET ) {
                VERIFICAR_CHEQUE_EN_BOLETIN();
            }
            break;
        case 91:
            /*  Anulacion Cobro  */
            ANULACION_COBROS();
            break;
        case 92:
            /*  Ident. Cliente  */
            IDENTIFICAR_CLIENTE_POR_CONSULTA();
            break;
        /* Codigo obsoleto
        case 93:
            /*  Referencia NC  
            REFERENCIA_NC();
            break;
            */
			#ifdef COMPILAR_LOGS_VIA_TCP
			/* funcion que habilita el logueo via TCPIP , fue un pedido puntual para pruebas de otra empresa
			de control del cajero via remota, no se levanto caso o incidente , pedido por J.C.MURGUI*/

				HABILITA_LOGS_TCP=!HABILITA_LOGS_TCP;
				if( HABILITA_LOGS_TCP ){
					glog("HABILITA_LOGS_TCP");
					mmmm("HABILITA_LOGS_TCP");
					LOGS_VIA_TCP("Inicia jouRnal");
				} else
					glog("DEShabilita_LOGS_TCP");
		#endif
        case 94:

            break;
        case 95:
            /*Factura A para El Salvador*/
            //CR1_FACTURA_TIPO( NO, FACTURA_TIPO_A );
            break;
        case 96:
            /*Factura B para El Salvador*/
            //CR1_FACTURA_TIPO( NO, FACTURA_TIPO_B );
            break;
        case 97:
            /*Alta de Clientes */
            ALTA_CLIENTE();
            break;
        case 98:
            DATA_ENTRY();
            break;
		case 101:
			{
			VALIDAR(VALIDACION_A_IMPRIMIR_TECLA,0,0,0,0);
			GRABAR_LOG_SISTEMA("Se imprime Cabecera en Rollo de Auditor",LOG_VENTAS,4);
			}
			break;
	    case 107:/*Ticket Factura*/
      		CR1_TICKET_FACTURA( );
   			break;
        case 106:
            /* Devolucion Cobros*/
			COBROS( SI, _MODO_INTERACTIVO );
            break;
        case 108:
            /* Cliente por codigo de barra*/
			   ELEGIR_CLIENTE_COD_BARRA(0);
            break;
		case 109: /*PagaTodo Recarga*/
			{
				struct	  _datos_cobro datos_cobro;

				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )&datos_cobro,
					sizeof( struct _datos_cobro ), 0 );
				rta = COBROS_PAGATODO( PAGATODO_RECARGA, ( char ) datos_cobro.devolucion );
			}
   			break;
        case 110: /*PagaTodo Venta PIN*/
			{
				struct	  _datos_cobro datos_cobro;

				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )&datos_cobro,
					sizeof( struct _datos_cobro ), 0 );
				rta = COBROS_PAGATODO( PAGATODO_VENTA_PIN, ( char ) datos_cobro.devolucion );
			}
   			break;
        case 111: /*PagaTodo Servicios*/
   			{
				struct	  _datos_cobro datos_cobro;

				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )&datos_cobro,
					sizeof( struct _datos_cobro ), 0 );
				rta = COBROS_PAGATODO( PAGATODO_SERVICIOS, ( char ) datos_cobro.devolucion );
			}
			break;
		case 112:
            /* Carga Monedero Electronico*/
			CARGAR_MONEDERO_ELECTRONICO( NO, NULL );
            break;

		case 113:
            /* Cobro Garantia*/
			{
				struct	  _datos_cobro datos_cobro;

				RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS, ( char* )&datos_cobro,
					sizeof( struct _datos_cobro ), 0 );

				rta = COBROS_GARANTIA( ( char ) datos_cobro.devolucion );
			}
            break;
		case 114:
            /* Franqueo */
			CR1_FRANQUEO( RAM_TOTAL, _MODO_VENTA );
            break;
		/* Codigo Obsoleto
        case 115:
            /* Devolucion Parcial Referenciada
			if( RAM_MODO_DEVOLUCION ? 1 :
					SOLICITAR_CLAVES( 30, 20, NO, SI, SI, ST( S__AUTORIZACION_DEVOLUCION_ ),
						ST( S_AUTORIZO_DEVOLUCIONES ), RAM_COD_FUNCION, SI ) ) {
				int remoto = 1, error = 1, tipo_evento_req = 0, ok = 1;

				if( !RAM_MODO_DEVOLUCION && PEDIR_MOTIVO_EN_DEVOLUCIONES ) {
					SET_MEMORY_CHAR( __modo_devolucion, SI );
                    SET_MEMORY_INT( __ram_motivo_devolucion, ELEGIR_MOTIVO( NULL ) );
                    SET_MEMORY_CHAR( __modo_devolucion, NO );
                    if( RAM_MOTIVO_DEVOLUCION <= 0 ) {
						ok = 0;
                    }
                }
				if( ok && !RAM_MODO_DEVOLUCION && PEDIR_DATOS_EN_DEVOLUCIONES
						&& ( !PEDIR_CAJA_Y_SECUENCIA || config.pais == MEXICO ) ) {
					ok = PEDIR_BOLETA_ANTERIOR( );
					if( ok && RAM_BOLETA_ANTERIOR <= 0 && RAM_CAJA_ANTERIOR <= 0
							&& RAM_FECHA_ANTERIOR <= 0 ) {
						ok = 0;
					}
                }
				if( ok && !RAM_MODO_DEVOLUCION ) {
         			if( !FUNCION_PIDE_PRECIO ) {
         					SET_MEMORY_INT( __ram_negativo, SI );
         			}
         			SET_MEMORY_INT( __ram_modo_devolucion, _DEVOL_RF );
         			SET_MEMORY_CHAR( __modo_devolucion, SI );
         			SET_MEMORY_CHAR( __ram_devolucion_parcial_ref, SI );
         		   	MENSAJE_SIN_SONIDO( ST( S_OBTIENE_EVENTOS_TEMPORALES ) , NO );

					if( RAM_TIPO_COMP_ANTERIOR == NC_ORIGEN_TICKET ) {
         		   		tipo_evento_req = 0;
         		   	} else if( RAM_TIPO_COMP_ANTERIOR == NC_ORIGEN_TF ) {
         		   		tipo_evento_req = 35;
         		   	}
					error = OBTENER_EVENTOS_TEMPORALES( tipo_evento_req, RAM_CAJA_ANTERIOR, 0,
						RAM_BOLETA_ANTERIOR, RAM_FECHA_ANTERIOR, _ORIGEN_DEVOLUCION, SI );
       				if( !error ) {
						if( VERIFICA_DATOS_IMPORTACION( remoto, tipo_evento_req, RAM_FECHA_ANTERIOR,
								0, RAM_CAJA_ANTERIOR, RAM_BOLETA_ANTERIOR ) ) {
							AGRUPAR_ART_IMPORT( NO, NO, NO );
							IMPORTAR_ITEMS(  );
       					} else {
       						error = 1;
       					}
					}
       				BORRAR_MENSAJE(  );
      			} else {
      				error = 1;
      			}
      			if( error ) {
      				CEREAR_DATOS_COMPROB_ANTERIOR(  );
      			}
   			} else {
				CEREAR_DATOS_COMPROB_ANTERIOR(  );
			}
   			MOSTRAR_MODO_DEVOLUCION(  );
			MUESTRA_COMPROBANTE( );//1ro para cliente para ticket factura
			MUESTRA_COMPROBANTE( );//2do para saldo
      		break;
            */
        case 116:
            CR1_FACTURACION_CREDITO( _NC_LIBRE );
            break;
        case 117:
            CR1_FACTURACION_CREDITO( _NC_RF_PARCIAL );
            break;
        case 118:
            CR1_FACTURACION_CREDITO( _NC_RF_INTERME );
            break;
        case 119:
            CR1_FACTURACION_CREDITO( _NC_TOTAL );
            break;
        case 120:
            CR1_FACTURACION_CREDITO( _DEVOL_LIBRE );
			tipo_devolucion = _DEVOL_LIBRE;
            break;
        case 121:
            CR1_FACTURACION_CREDITO( _DEVOL_PARCIAL );
			tipo_devolucion = _DEVOL_PARCIAL;
            break;
        case 122:
            CR1_FACTURACION_CREDITO( _DEVOL_INTERME );
            break;
        case 123:
            CR1_FACTURACION_CREDITO( _DEVOL_TOTAL );
            break;
		case 124:
            /*---- Ticket en espera con Menu de seleccion para los tespera locales ---*/
			PROCESAR_TICKET_EN_ESPERA( _TE_CON_MENU );
            break;
		case 125:
            /*---- Consulta de promociones activas ---*/
			consultaPromociones( );
            break;
		case 126:
			{int tf =0;
			//Importacion En TF o Fact limitado a TF o FActura.
	/*		tf =RAM_TICKET_FACTURA;
			tf =RAM_FACTURA;*/
            IMPORTAR_TICKET( SI, 0, NO, ST( S_IMPORTAR_TICKET ), NO, NO, _DATOS_CON_MENU ,NO ,DIAS_EN_IMPORTACION_A_TF_O_FACTU) ;
			MUESTRA_SUBTOTAL(SI);
/*			tf =RAM_TICKET_FACTURA;
			tf =RAM_FACTURA;*/
			}
			break;
		case 127:
			/*---- Cambios de Medios Automatico ---*/
			grabar_copia_ticket = 0;
			CAMBIO_MEDIOS( SI );
			break;
	}
	BLOQUEAR_DATOS = NO;
    Orden = 1;
    /**/glog( "Preparando PANEL ..." ,LOG_VENTAS,4); //Log Borrar
    Response = PreparePanel( &pPanel, Orden );
    if( Response ) {
        ShowPanel( pPanel );
    }
    /**/glog( "... PANEL Mostrado." ,LOG_VENTAS,5); //Log Borrar
    /*--------------------- Fin de recupero de memoria ----------------------*/
	LIMPIAR_SCANNER();
    CLOSE_RECUPERO( _ptr, _handle );
    /**/glog( "... CLOSE_RECUPERO( ... ) Finalizado" ,LOG_VENTAS,4); //Log Borrar
    GRABAR_LOG_MEMORIA();
//    /**/glog( "... GRABAR_LOG_MEMORIA() Finalizado",LOG_VENTAS,2 ); //Log Borrar
    ENABLE_STANDBY_AUTOMATICO();
  //  /**/glog( "... ENABLE_STANDBY_AUTOMATICO() Finalizado" ,LOG_VENTAS,2); //Log Borrar
    ExecuteAction( A_DES_TODOS_ACT_VENTAS );
    /**/glog( "... DESACTIVAR TODOS MENOS VENTAS Finalizado",LOG_VENTAS,4 ); //Log Borrar

    return ( rta );
}
/*****************************************************************************/
int CR1_AGREGA_ITEM( int enganche, char origen, int tipo, char tipo_evento_orig )
/*****************************************************************************/
{
	int ok = 1, rta = 0, id_transaccion = 0, status = 0;               // rta=0;
    char resta = 0;
    int _ptr = 0, _handle = 0, _estado = 0, h = 0, excedido = 0, cod_promo = 0, datos_adicionales = 0,
        pos = 0;
    char cadena[25];
    double importe_auxiliar = 0.0;

    /*--------------------- Prepara recupero de memoria ----------------------*/
    OPEN_RECUPERO( &_ptr, &_handle );
    _estado = STACK_INS( sizeof( short ) );

	//Cuando hay corte de luz la funcion REGENERAR_PANTALLA_TICKET, visualiza
	//los items cargados en la estructura TICKET de manera que no tendria que 
	//volver a mostrarlos. 
	if( _ESTADO == 2 || _ESTADO == 3 || _ESTADO == 4 || _ESTADO == 5 || _ESTADO == 6 
			|| _ESTADO == 17 || _ESTADO == 42 || _ESTADO == 51
			|| _ESTADO == 411 || _ESTADO == 412 || _ESTADO == 413 ) { 
 		status = 1;
	}
	/*--------------- Verifica si existe el articulo cobro --------------*/
	if( RAM_ANULACION && ITEM_COD_COBRO ) {
		int i;
		for( i = 0; i < RAM_P_TICKET; i++ ){
			double precio = 0.0;
			precio = ITEM_PRECIO * RAM_CANTIDAD;
			if( ITEM_COD_COBRO == _TICKET_COD_COBRO(i) && !strcmp( _TICKET_COD_BARRA(i), ITEM_COD_BARRA ) && 
				_TICKET_COD_ESPECIAL(i) == ITEM_COD_ESPECIAL && fabs( _TICKET_PRECIO(i) - precio ) < 0.0005 ) {
				ok = 1;
				break;
			} else {
                if( RAM_NRO_PEDIDO_PIN ) {//??? 
                    int posicionTransaccionKinacu = 0;
                    char barra1[17], barra2[17], barra3[17];

                    while( !id_transaccion && RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&datos_cobro_kinacu,
                            sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {

                         strncpy( barra1, _TICKET_COD_BARRA( i ), sizeof( barra1 ) );
                         strncpy( barra2, ITEM_COD_BARRA, sizeof( barra2 ) );
                         strncpy( barra3, datos_cobro_kinacu.cod_barra, sizeof( barra3 ) );
                         if( !strcmp( barra1, barra2 ) ) {
                            if( !strcmp( barra2, barra3 ) ) {
                                id_transaccion = datos_cobro_kinacu.id_transaccion;
                                if( id_transaccion ) {
                                    /*------------------------- Deshabilita Panel PAGATODO -------------------------*/
                                    PEDIR_DATOS_COBROS_KINACU( NULL, NO, 0 );
                                }
                            }
                        }
                        posicionTransaccionKinacu++;
                    }
                }
				ok = 0;
			}
		}
		if( !ok ){
			MENSAJE( "NO HAY ARTICULO ASOCIADO AL COBRO CON ESE IMPORTE" );
            if( RAM_NRO_PEDIDO_PIN && !id_transaccion ) {
                PEDIR_DATOS_COBROS_KINACU( NULL, NO, 0 );
            }
			return ok;
		}
	}
	/*------------------------------------------------------------------------*/
    /*-------------------- En caso de anulacion activa -----------------------*/
	if( RAM_ANULACION ) {
		if( RAM_MODO_EJECUCION != INVENTARIO 
				&& ( !RAM_EN_IMPORTACION_DE_TICKET 
					|| RAM_EN_IMPORTACION_DE_TICKET == 3 
					|| RAM_AGREGA_ITEM_TICK_IMPORT == 2 ) ) {
            //rta = OBTENER_COD_ESPECIAL_COBRO( ITEM_COD_COBRO ); 
            if( !ITEM_COD_COBRO/*!rta*/ ) {
				/*if( ITEM_COD_COBRO ) {//esto es para el caso que el importe este configurado en cobros
					cant_cobro = RAM_CANTIDAD;//Para cobro kinacu aun no viene seteado cantidad en 1
					SET_MEMORY_DOUBLE( __ram_cantidad, 1);
				}*/
		        ok = CR1_EXISTE_ITEM( ITEM_COD_ESPECIAL, ITEM_PRESENTACION, ITEM_COD_BARRA );
				//aca controlar que no genere desborde de totales en NC al intentar anular un item que prorrateo el descuento
				if( RAM_NOTA_CR > 0 && RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL){//aca controlo el desborde de totals
					int supera = NO;
					if (CR1_ITEM_GRAVADO()) {
						supera = RETORNAR_TOTAL_PRINTER_POR_IVA(ITEM_PRECIO);
						if( supera == SI)
							ok = -2;
						//dd = RETORNAR_TOTAL_PRINTER( ITEM_PRECIO );
					}else {	
						double  dd;
						dd = fabs(ITEM_PRECIO) - fabs(TOT_TICK_EXENTO);
						if( dd > 0.00)
							ok = -2;

					}
				}
		    } else {
                ok = CR1_EXISTE_ITEM_ESTRICTO( ITEM_COD_ESPECIAL, ITEM_PRESENTACION,
                ITEM_COD_BARRA, SI );
            }
           
			if( ok > 0 && origen != _ORIGEN_PROMOCION && origen != _ORIGEN_DIF_PRECIO ) {
			   ok = CR1_EXISTE_ITEM_DIF_PRECIO( ITEM_COD_ESPECIAL, ITEM_PRESENTACION, SI,
			      ITEM_COD_BARRA );
			   if( ok == -1 ) {
			      RECALCULA_ITEM_POR_PRECIO( ITEM_PRECIO );
			      MENSAJE( "SE ANULARAN TODOS LOS ARTICULOS CON EL CODIGO ESPECIFICADO" );
			      ok = 1;
			   }
			}
            h = ok;
        }
        resta = 1;
		if( ok == 0 ) {
            MENSAJE_STRING( S_ARTICULO_NO_TICKEADO );
			CR1_MUESTRA_MULTIPLICACION();
			ok = 0;
        } else if( ok == -1 ) {
	      MENSAJE( "NO SE PUEDE ANULAR ART. EN DEVOLUCIONES CON DESCUENTOS" );
		} else if( ok == -2 ) {
		  MENSAJE_STRING(S_NO_SE_PUEDE_REALIZAR_LA_ANULACION); //ponerle delay
		  ok = 0;
		} else if( AUTORIZAR_ANULACIONES == 2 && !RAM_CORRECCION 
				//No solicitar autorizacion en articulos anulados de importaciones totales
				&& !( RAM_MODO_DEVOLUCION == _DEVOL_TOTAL || RAM_MODO_DEVOLUCION == _NC_TOTAL ) ) {
            if( !RAM_EN_IMPORTACION_DE_TICKET && !CR1_AUTORIZA_ANULACIONES() ) {
                ok = 0;
            }
        }
		if( ok <= 0 ) {
            SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
            SET_MEMORY_INT( __ram_anulacion, 0 );
            cod_especial = 0;
            memset( cadena, '0', L_CODIGO );
            cadena[L_CODIGO] = 0;
            SET_MEMORY( __ram_cod_barra, cadena );
            if( RAM_NRO_PEDIDO_PIN && !id_transaccion ) {
                PEDIR_DATOS_COBROS_KINACU( NULL, NO, 0 );
            }
        }
        else {
            if( (RAM_MODO_DEVOLUCION != _DEVOL_TOTAL) && !RAM_NOTA_CR ) {
                PEDIR_MOTIVO_OPERACION_NEGATIVA( NULL, _MOTIVO_ANUL_ITEM );
            }
        }
        LIMPIAR_RECUADRO_PETICION( 0 );
	} else {
		/*if(  RAM_REFERENCIA_NC ) {
			ok = EXISTE_ARTICULO_TICKET_ORIGINAL(  );
	   }*/
	}

    /*------------------- Controla la cantidad de items ---------------------*/
	if( ok && _ESTADO == 0 ) {
        if( RAM_MODO_EJECUCION == INVENTARIO ) {
            excedido = ( RAM_RENGLONES >= _LARGO_TICKET - 10 ) ? SI : NO;
        }
        else {
            if( RAM_MODO_EJECUCION == TRANSFERENCIAS ) {
                excedido = ( RAM_P_TICKET >= _LARGO_TRANSFERENCIA ) ? SI : NO;
            }
            else {
                excedido = ( RAM_P_TICKET >= _LARGO_TICKET - 10 ) ? SI : NO;
            }
        }
        if( excedido ) {
            MENSAJE_STRING( S_LONGITUD_TICKET_EXCEDIDA );
            MENSAJE_STRING( S_DEBE_FINALIZAR_EL_TICKET );
        }
        else if( EMITIR_FACTURA() 
				&& LIMITE_LINEAS_FACTURA 
				&& ( ( RAM_FACTURA != 0 &&  !RAM_NOTA_CR )//PARA FACTURA 
					|| ( RAM_NOTA_CR != 0 && RAM_REFERENCIA_NC == 0 ) )//PARA NOTA DE CREDITO QUE NO SEA REFERENCIADA, EN LA REFERENCIA YA DEBE ESTAR LIMITADA.....
				&& ( unsigned )RAM_P_TICKET >= LIMITE_LINEAS_FACTURA ) {
            MENSAJE_STRING( S_LONGITUD_FACTURA_EXCEDIDA );
        }
		else if( LIMITE_DE_LINEAS_DE_TICKETS 
				&& RAM_P_TICKET >= LIMITE_DE_LINEAS_DE_TICKETS ) {
            MENSAJE_STRING( S_LONGITUD_TICKET_EXCEDIDA );
        }
        else {
            _SET_ESTADO( 1 );
        }
    }
    /*--------------------- copia datos en el ticket ------------------------*/
	if( _ESTADO == 1 ) {
        //if( RAM_MODO_EJECUCION == INVENTARIO )  se comenta porque no permite la recuperacion del ticket en modo inventario
        //SET_MEMORY_INT( __ram_p_ticket, 0 ); // esto es para que en la estructura ticket solo quede el ultimo item tickeado solamente.
        if( RAM_ANULACION && h && _TICKET_ORIGEN( h - 1 ) == _ORIGEN_DESC_PORC ) {
            SET_MEMORY_CHAR( __ram_descuento_porcentual, 0 );
        }
        if( RAM_ANULACION && h && _TICKET_ORIGEN( h - 1 ) == _ORIGEN_DESC_PESOS ) {
            SET_MEMORY_CHAR( __ram_descuento_importe, 0 );
        }
        SET_MEMORY_DOUBLE( __item_descuento_cantidad, 0 );
/* -- FR 03-07-07 Se comenta para eliminar todo el código correspondiente a las promos 'tradicionales' --
		if( RAM_CLIENTE_AFINIDAD ) {
            cod_promo = BUSCA_ITEM_PROMO();
            if( cod_promo ) {
                BUSCA_ID_PROMO( cod_promo );
                BUSCAR_PUNTOS( cod_promo, &puntos, &descuento );
                puntos *= RAM_CANTIDAD;

                if( RAM_ANULACION ) {
                    if( CR1_EXISTE_DESCUENTO_CANTIDAD( ITEM_COD_ESPECIAL, ITEM_PRESENTACION ) ) {
                        ADD_MEMORY_DOUBLE( __ram_puntos_usados, -puntos );
                        //GRABAR_EVENTO_PROMO_ARTICULO( id_promo );
                    }
                }
                else {
                    if( AFINIDAD_DESCUENTO_PUNTOS( puntos, descuento ) ) {
                        ADD_MEMORY_DOUBLE( __ram_puntos_usados, puntos );
                        SET_MEMORY_DOUBLE( __item_descuento_cantidad, RAM_CANTIDAD );
                        //GRABAR_EVENTO_PROMO_ARTICULO( id_promo );
                    }
                }
            }
        }
   ------------------------------------------------------------------------------------------------------ */
        _SET_ESTADO( 11 );
    }
	if( _ESTADO == 11 ) {
		if( RAM_EN_IMPORTACION_DE_TICKET == 2 ) {
			if( PERMITE_MODIFICAR_TICKET_IMPORTA == NO ) {
				ok = NO;
				MENSAJE_STRING( S_NO_SE_PERMITE_MODIFICAR_TICKET );
				BORRAR_MENSAJE();
			} else {
				ok = SI;
			}
		} else {
			ok = SI;
		}
        if(	ok && (!ITEM_PEDIR_SUPERVISOR
				|| SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, ST( S_AUTORIZACION_ARTICULO ),
					ST( S_AUTORIZACION_ARTICULO ), _F_SUP_AUT_ARTICULO, SI ) ) ) {
            _SET_ESTADO( 12 );
        }
        else {
            ok = NO;
        }
    }
    if( _ESTADO == 12 ) {
        if( RAM_MODO_EJECUCION != INVENTARIO ) {
        if( !RAM_ANULACION ) {
            if( RESERVAR_PRODUCTO_JGATEWAY( origen ) > 0 ) {
                _SET_ESTADO( 13 );
            } else {
   			    ok = NO;
				SET_MEMORY_INT( __item_cod_cobro, 0 );
                _SET_ESTADO( 99 );
   		    }
        } else {
		    CANCELAR_PRODUCTO_JGATEWAY( ITEM_COD_BARRA, ITEM_PRECIO * RAM_CANTIDAD );
            _SET_ESTADO( 13 );
        }
       } else
         _SET_ESTADO( 13 );
    }
    if( _ESTADO == 13 ) {
        double imp_aux = ITEM_PRECIO;
        datos_adicionales = ITEM_COD_SOLICITUD;
        if( datos_adicionales ) {
            if( PAGOS_PEDIR_DATOS_ADICIONALES( RAM_P_TICKET, 0, datos_adicionales, &imp_aux,
                                               D_ITEMS, NULL, 1, NULL, NULL , '\0', '\0') ) {
                if( RAM_ANULACION && fabs( ITEM_PRECIO - imp_aux ) > 0.01 ) {
                    pos = CR1_BUSCAR_ITEM_ORIGINAL_POR_DATO_ADICIONAL( datos_adicionales );
                    if( pos >= 0 ) {
                        imp_aux = _TICKET_PRECIO( pos );
                    }
                    else {
                        ok = 0;
                        MENSAJE_STRING( S_ARTICULO_NO_TICKEADO );
                    }
                }
                if( ok ) {
                    if( fabs( ITEM_PRECIO - imp_aux ) > 0.01 && imp_aux != 0.00 ) {
                        //if( RAM_ANULACION ) imp_aux *= -1;
                        if( imp_aux < 0 && ( RAM_TOTAL + imp_aux ) <= 1 && !RAM_ANULACION ) {
                            imp_aux = -( RAM_TOTAL - 1.00 );
                        }
                        if( imp_aux > 0 || RAM_TOTAL + imp_aux > 0.00 || RAM_ANULACION ) {
                            RECALCULA_ITEM_POR_PRECIO( imp_aux );
                            origen = _ORIGEN_BOLETIN_D_ADIC;
                        }
                    }
                    _SET_ESTADO( 14 );
                }
            }
        }
        else {
            _SET_ESTADO( 14 );
        }
    }
	if( _ESTADO == 14 ) {
        #ifdef COMPILAR_TIPOS_DEVOLUCIONES
		switch( RAM_CAMBIO ) {
            case 1:/* CAMBIO - */ //Ver con Motor de Promociones
				//Se restringe el cambio de precio para articulos por importe ya que el motor
				//de promociones no puede identificar un mismo articulo en un tiquet con 
				//precios unitarios diferentes y esto proboca errores al aplicarse una promoción
                if( /*!ITEM_PEDIR_PRECIO &&*/ !PEDIR_DATOS_DIFERENCIA_PRECIO( SI ) ) {
                    _SET_ESTADO( 99 );
                    ok = 0;
                }
                break;
            case 2:/* CAMBIO + */
                SET_MEMORY_CHAR( __item_cod_movimiento, _CAMBIO_DE_MERCADERIA_MAS );
                break;
            //default:
                //break;
        }
        #endif
		if( _ESTADO == 14 ) {
			_SET_ESTADO( 15 );
		}
     }
	if( _ESTADO == 15 ) {
		glog ("Entrando _ESTADO 15",LOG_VENTAS,4);
		AGREGA_ITEM_AL_TICKET( RAM_P_TICKET, resta, enganche, SI, origen );
		_SET_ESTADO( 16 );
	}
	if( _ESTADO == 16 ) {
		glog ("Entrando _ESTADO 16",LOG_VENTAS,4);
		ADD_MEMORY_INT( __ram_p_ticket, 1 );
		_SET_ESTADO( 17 );
	}
	if( _ESTADO == 17 ) {
		glog ("Entrando _ESTADO 17",LOG_VENTAS,4);
		ADD_MEMORY_INT( __ram_renglones, 1 );
		_SET_ESTADO( 2 );
	}

    /*------------------ calcula el subtotal y los bultos -------------------*/
	if( _ESTADO == 2 ) {
		glog ("Entrando _ESTADO 2",LOG_VENTAS,4);
        SET_MEMORY_DOUBLE( __ram_total_ant, RAM_TOTAL );
        SET_MEMORY_INT( __ram_bultos_ant, RAM_BULTOS );
        //fabiana 15-02-05 ** nuevo manejo de redondeos--se elimina lo de modo facturacion especial
        //if( MODO_FACTURACION_ESPECIAL(  ) ) {
        //  SET_MEMORY_DOUBLE( __ram_importe,
        //      _TICKET_CANTIDAD( RAM_P_TICKET - 1 ) * ( ITEM_PRECIO +
        //          _TICKET_RECARGO( RAM_P_TICKET - 1 ) ) );
        //} else {
        //fabiana 15-02-05 ** nuevo manejo de redondeos--se asigna por primera vez el ram_IMPORTE y con redondeo!!!
        importe_auxiliar = _TICKET_CANTIDAD( RAM_P_TICKET - 1 )
                         * ( ITEM_PRECIO + _TICKET_RECARGO( RAM_P_TICKET - 1 ) );
        SET_MEMORY_DOUBLE( __ram_importe, ROUND( importe_auxiliar, ENTEROS, DECIMALES ) );
        //}
        #ifdef DEBUG_R
        if( MODO_DEBUG == _DEBUG_REDONDEO ) {
            memset( cadena_debug, 0, strlen( cadena_debug ) );
            sprintf( cadena_debug, "9.cr1 Agreg Item, RIMPORTE=%lf, RT=%lf, RIS/R=%lf", RAM_IMPORTE,
                     RAM_TOTAL, importe_auxiliar );
            GRABAR_LOG_SISTEMA( cadena_debug );
        }
        #endif
        if( ITEM_ENVASE || ITEM_DEV_ENVASE || origen == _ORIGEN_DESC_PESOS || origen == _ORIGEN_PROMOCION_DESC
				|| origen == _ORIGEN_DESC_PORC || RAM_DIFERENCIA_PRECIO || origen == _ORIGEN_VARIACION_MEDIO
				|| ( RAM_AGREGA_ITEM_TICK_IMPORT == 1 && origen == _ORIGEN_DIF_PRECIO ) ) {
            SET_MEMORY_INT( __ram_cant_bultos, 0 );
        }
        /* if(MODO_DEVOLUCION){ if( RAM_ANULACION ) { SET_MEMORY_DOUBLE(
             * __ram_total, ROUND( RAM_TOTAL - RAM_IMPORTE , 10, DECIMALES) );
             * ADD_MEMORY_INT(    __ram_bultos, (RAM_CANT_BULTOS) ); } else {
             * SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + RAM_IMPORTE ,
             * 10, DECIMALES) ); ADD_MEMORY_INT(    __ram_bultos,
             * -(RAM_CANT_BULTOS) ); } } else{   */
        if( RAM_ANULACION ) {
            SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL - RAM_IMPORTE, 10, DECIMALES ) );
            ADD_MEMORY_INT( __ram_bultos, -( RAM_CANT_BULTOS ) );
        }
        else {
		    SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + RAM_IMPORTE, 10, DECIMALES ) );
            ADD_MEMORY_INT( __ram_bultos, RAM_CANT_BULTOS );
        }
        // }
        if( RAM_MODO_EJECUCION == INVENTARIO ) {
            SET_MEMORY_DOUBLE( __ram_total, 0 );
        }
        _SET_ESTADO( 3 );
    }
    /*----------------------- Verificaciones varias -------------------------*/
	if( _ESTADO == 3 ) {
        ok = SI;
      /*------------- verifica que no se exceda el limite de venta ---------*/
      ok = CR1_VERIFICAR_LIMITES_FACTURA( RAM_TOTAL, VERIFICAR_LIMITE_AL_RENGLON );
        /*------------------ verifica que haya error matematico --------------*/
        if( error_matematico ) {
            ok = NO;
            MENSAJE_STRING( S_ERROR_MATEMATICO );
            ALARMA( 2 );
            ALARMA( 2 );
            ALARMA( 2 );
            error_matematico = 0;
        }
        /*-------- verifica que no se exceda el limite de la tarjeta ------*/
        if( RAM_TOTAL > VAR_TARJETAS_LIMITE && RAM_TOTAL_ANT < VAR_TARJETAS_LIMITE
         && VAR_TARJETAS_CUOTAS > 0 ) {
            if( !CR1_AUTORIZA_PASAR_LIMITE() ) {
                ok = NO;
            }
        }
        /*-------- verifica que no se exceda el saldo del cliente ------*/
        if( CONTROLAR_CUPO_EN_VENTAS ) {
            switch( VERIFICA_CUPO_CLIENTE( RAM_TOTAL, RAM_TOTAL_ANT, NO, _MODO_VENTA ) ) {
                case 0:
                    ok = NO;
                    break;
                case 1:
                    aut_exceder = 1;
                    break;
            }
        }
        /*------------------ Validamos los envases entregados ----------------*/
        if( VERIFICAR_ENVASES_POR_RENGLON && !VALIDA_ENVASES_ENTREGADOS() ) {
            ok = NO;
            MENSAJE_STRING( S_EXCESO_ENVASES_ENTREGADOS );
            PAUSA( 18 );
        }
        /*---------- Segn OK pasa al proximo estado o anula la entrada ------*/
        BORRAR_MENSAJE();
        if( ok ) {
            _SET_ESTADO( 4 );
        }
        else {
            ANULA_ENTRADA();
        }
    }
	if( _ESTADO == 4 ) {
        /*------------------ muestra datos en la pantalla --------------------*/
		if( RAM_P_TICKET ==1 && IMPRESORA_FISCAL == FISCAL && DEBE_IMPRIMIR_ITEM() ) { //para que no parezca tanto la demora
			MENSAJE_SIN_PAUSA( ST(S_IMPRIMIENDO));
		}
        //MUESTRA_BULTOS(); 
        //MUESTRA_SUBTOTAL();
        //por el tema de redondeo y el recalculo por grupo
        //com MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );
		//come MUESTRA_DATOS_CONTADOR_PANTALLA( RAM_P_TICKET - 1 , SI );
        /*----------- Operaciones especiales para empezar un ticket ----------*/
        if( RAM_EMPIEZA_TICKET ) {
			//hay casos donde  no se cereo porque cortaron la caja y no vaciaron rnv.. genera dif en pantalla y demas.
			if( TOT_TICK_IMPORTE_SIN_IVA != 0.0 ||
				TOT_TICK_IMP_INT != 0.0 ||
				TOT_TICK_EXENTO != 0.0 ){
					RESETEA_TOT_TICK();
					/**/glog ("Reseteo Totales por Difs.",LOG_ERRORES,4);
			}
			//Si el 1er art. es un Cobro Marginal, no se habra los arch. PRN 
			//porque ya se abrieron en la funcion Cobros
			if( !ITEM_COD_COBRO ) {
				CLOSE_COPIA_TICKET_PRN(  );
				OPEN_COPIA_TICKET_PRN(  );
			}
            if( DEBE_IMPRIMIR_ITEM() ) {
                _SET_ESTADO( 41 );
            }
            else {
				if( IMPRIMIR_CABECERA_PRINCIPIO  ){
					_SET_ESTADO( 41 );
				} else {
					_SET_ESTADO( 42 );
				}
            }
        }
        else {
            _SET_ESTADO( 5 );
        }
    }
    /*------------------ Sub-estados al iniciar un ticket -------------------*/
	if( _ESTADO == 41 ) {
        if( RAM_MODO_EJECUCION != INVENTARIO ) {
            _SET_ESTADO( 411 );
        }
        else {
            _SET_ESTADO( 413 );
        }
    }
	if( _ESTADO == 411 ) {
        //OPEN_COPIA_TICKET_PRN();
        //      if( IMPRESORA_FISCAL && IMPRESORA_FISCAL != FISCAL_IBM_4694 ) {
        //         FISCAL_B(  );
        //      }
        _SET_ESTADO( 412 );
    }
	if( _ESTADO == 412 ) {
		/**/glog ("GRABAR_LOG_NRO_TICKET() [2] ...",LOG_VENTAS,4);
        GRABAR_LOG_NRO_TICKET();
		/**/glog ("... GRABAR_LOG_NRO_TICKET() [2] LISTO",LOG_VENTAS,5);
        _SET_ESTADO( 413 );
    }
	if( _ESTADO == 413 ) {
		int		ram_modo_dev_ant = 0;
		char    modo_dev_ant = 0;

		if( tipo_evento_orig == 21 ) {
			ram_modo_dev_ant = RAM_MODO_DEVOLUCION;
			modo_dev_ant = MODO_DEVOLUCION;
			SET_MEMORY_INT( __ram_modo_devolucion, 1 );
            SET_MEMORY_CHAR( __modo_devolucion, 1 );
		}
		/**/glog ("Entrando IMPRIME_CABECERA_TICKET( )...",LOG_VENTAS,4);
		rta = IMPRIME_CABECERA_TICKET( NO ); //controlar que haya abierto el comprobante
		/**/glog ("... IMPRIME_CABECERA_TICKET( ) OK",LOG_VENTAS,4);
		if( tipo_evento_orig == 21 ) {
			SET_MEMORY_INT( __ram_modo_devolucion, ram_modo_dev_ant );
            SET_MEMORY_CHAR( __modo_devolucion, modo_dev_ant );
		}
		if(rta == 1 && IMPRIMIR_TICKET_AL_FINAL ) {
		/**/glog ("Entrando a setData(ACTION_FORCE_PRINT,NULL,00) ....",LOG_VENTAS,4);
            setData(ACTION_FORCE_PRINT,NULL,00);
		/**/glog ("... setData(ACTION_FORCE_PRINT,NULL,00) Terminado OK",LOG_VENTAS,4);
		}
        //#endif
        if( IMPRESORA_FISCAL != FISCAL ) {
            IMPRIME_TITULO_TICKET();
            #ifdef COMPILAR_TECLA_CAMBIO
            IMPRIME_CAMBIO();
        }
        #endif
		if( rta == 3) { //anular ticket
			ok = 0;
			_SET_ESTADO( 51 );
		} else
        _SET_ESTADO( 42 );
    }

	if( _ESTADO == 42 ) {
		/**/glog ("Entrando _ESTADO 42",LOG_VENTAS,4);
        SET_MEMORY_CHAR( __ram_empieza_ticket, 0 );
        time( &hora_inicio_ticket );
        SET_MEMORY_INT( __ram_hora_inicio_evento, GET_HORA_DOS() );
        
        _SET_ESTADO( 5 );
    }
    /*------------------------ Imprime el articulo --------------------------*/
	if( _ESTADO == 5 ) {
		if( DEBE_IMPRIMIR_ITEM() ) {
            if( IMPRIME_ITEM( RAM_P_TICKET - 1 ) == DEBE_ANULAR_TICKET ) {
                _SET_ESTADO( 51 );
            }else {
                _SET_ESTADO( 6 );
            }
        }/*las condiciones deben estar dentro de DEBE_IMPRIMIR_ITEM!!!!
		 else {
			_SET_ESTADO( 6 );
            if(IMPRESORA_FISCAL == FISCAL ) {
           	    if( IMPRIME_ITEM( RAM_P_TICKET - 1 ) == DEBE_ANULAR_TICKET ) {
                    _SET_ESTADO( 51 );
                }
            }
        }*/
		else {
            _SET_ESTADO( 6 );
        }
    }

	if( _ESTADO == 51 ) {
		/**/glog ("Entrando _ESTADO 51",LOG_VENTAS,4);
        CR1_INCOSISTENCIA_FISCAL( SI, origen, SI, SI );
		ok = NO;
    }

    /*------------------ Actualiza acumuladores en RNV ----------------------*/
    if( _ESTADO == 6 ) {
		/**/glog ("Entrando _ESTADO 6",LOG_VENTAS,4);
		  //por la demora de impresion de items en tm220
		MUESTRA_BULTOS();
        // no deberia carcular el total
		MUESTRA_SUBTOTAL( NO);
		_SET_ESTADO( 7 );
	}    
	if( _ESTADO == 7 ) {
		//lo subi para poder tener el ticket_precio actualizado y coincida con el recalculo
		/**/glog ("Entrando _ESTADO 7",LOG_VENTAS,4);
		ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET - 1 );
		_SET_ESTADO( 8 );
	}
	if( _ESTADO == 8 ) {
        //por el tema de redondeo y el recalculo por grupo
		if( !status ) {
			MUESTRA_ITEM_EN_PANTALLA( RAM_P_TICKET - 1 );//esta funcion es la demora
		}
		MUESTRA_DATOS_CONTADOR_PANTALLA( RAM_P_TICKET - 1 , SI );
        SET_MEMORY_CHAR( __ram_prohibir_mas, 0 );
		_SET_ESTADO( 9 );
	}
	if( _ESTADO == 9 ) {
		/**/glog ("Entrando _ESTADO 9",LOG_VENTAS,4);
        ADD_MEMORY_DOUBLE( __ram_cantidad_total, RAM_CANTIDAD );
        _SET_ESTADO( 10 );
    }
	/*-----------------------------------------------------------------------*/

    /*-------------------------- graba evento -------------------------------*/
    if( _ESTADO == 10 ) {
        if( !tipo ) {
            tipo = _TICKET_TIPO( RAM_P_TICKET - 1 );
        }
        GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, NO, origen, tipo, NO );
		//Mientras este en CAMBIO - (RAM_CAMBIO = 1) los importes deben ser negativos
		if( RAM_CAMBIO != 1 ) {
			SET_MEMORY_CHAR( __ram_negativo, 0 );//libertad
		}
        _SET_ESTADO( 99 );
#ifdef COMPILAR_LOGS_VIA_TCP
	   {
	   char item_aux[150];
   	memset(item_aux,0,150);
   	sprintf(item_aux,"[%13s]   %7.2lf  %30s $%7.2lf", event_e_ticket.cod_barra,event_e_ticket.cantidad,articulo.nombre,event_e_ticket.total);
	    LOGS_VIA_TCP(item_aux);
	   }
#endif
    }

    /*------------------------- Control del cajon ---------------------------*/
    if( CONTROLAR_CAJON ) {
		///**/glog ("Entrando CONTROLAR CAJON",LOG_VENTAS);
        CERRAR_CAJON();
    }
    /*------------------- Termina recupero de memoria -----------------------*/
	///**/glog ("Entrando CLOSE_RECUPERO",LOG_VENTAS,2);
	
    CLOSE_RECUPERO( _ptr, _handle );

	/**/glog ("CR1_AGREGA_ITEM Termino OK",LOG_VENTAS,4);
    return ( ok );
}
/*****************************************************************************/
void ANULA_ENTRADA()
/*****************************************************************************/
{
    ADD_MEMORY_INT( __ram_p_ticket, -1 );
    SET_MEMORY_DOUBLE( __ram_total, RAM_TOTAL_ANT );
    SET_MEMORY_INT( __ram_bultos, RAM_BULTOS_ANT );
    // CLOSE_RNV_VIRTUAL();
    // OPEN_RNV_VIRTUAL();
}
/*****************************************************************************/
void AGREGA_ITEM_AL_TICKET( int h, int resta, int enganche, int grabar, char origen )
/*****************************************************************************/
{
    unsigned char tipo = 0, tipo2 = 0, tipo3 = 0;
    double imp;
    tipo |= ( ITEM_ENVASE ) ? _ENVASE : 0;
    tipo |= ( ITEM_DEV_ENVASE ) ? _DEV_ENVASE : 0;
    tipo |= ( resta ) ? _RESTA : 0;
    tipo |= ( CR1_ITEM_GRAVADO() || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) ? _GRAVADO : 0;
    tipo |= ( ITEM_CUPON_DEV_ENV ) ? _CUPON_DEV_ENV : 0;
    tipo |= ( ITEM_CREDITO ) ? _CREDITO : 0;
    tipo |= ( ITEM_SCANNER ) ? _SCANNER : 0;
    tipo |= ( enganche ) ? _ENGANCHADO : 0;
    tipo2 |= ( ITEM_HABILITADO_EN_CUOTAS ) ? _HABILITADO_CUOTAS : 0;
    tipo2 |= ( RAM_CORRECCION ) ? _CORRECCION : 0;
    tipo2 |= ( ITEM_PEDIR_PRECIO ) ? _PEDIR_PRECIO : 0;
    tipo2 |= ( ITEM_HABILITADO_EN_PROMOCION ) ? _HABILITADO_PROMO : 0;
	tipo3 |= ( ITEM_PESABLE ) ? _PESABLE : 0;

/*	if( origen != _ORIGEN_PROMOCION_DESC ) 
		DIFERENCIA_REDONDEO_CAJA_IMPRESORA( );*/
	_SET_MEMORY_CHAR( __ticket_cod_iva, h, ITEM_COD_IVA );
    _SET_MEMORY_INT( __ticket_depto, h, ITEM_DEPTO );
    _SET_MEMORY_DOUBLE( __ticket_precio, h, ITEM_PRECIO );
    _SET_MEMORY_FLOAT( __ticket_cantidad, h, RAM_CANTIDAD );
    //if( EXCLUIR_ART_AFECTADOS_POR_DESC ) {
    //     _SET_MEMORY_FLOAT( __ticket_cant_excl_desc, h,( ITEM_CREDITO )
    //        ? RAM_CANTIDAD : 0.00 );
    //   }
    _SET_MEMORY_CHAR( __ticket_tipo, h, tipo );
	_SET_MEMORY_CHAR( __ticket_tipo3, h, tipo3 );
    _SET_MEMORY_INT( __ticket_presentacion, h, ITEM_PRESENTACION );
    _SET_MEMORY_INT( __ticket_tipo_envase, h, ITEM_TIPO_ENVASE );
    _SET_MEMORY_CHAR( __ticket_cod_present, h, ITEM_COD_PRESENTACION );
    _SET_MEMORY_LONG( __ticket_cod_especial, h, ITEM_COD_ESPECIAL );
    _SET_MEMORY_DOUBLE( __ticket_imp_int, h, ITEM_IMP_INT );
    _SET_MEMORY_DOUBLE( __ticket_costo, h, ITEM_COSTO );
    _SET_MEMORY_DOUBLE( __ticket_recargo, h, ITEM_RECARGO );
    _SET_MEMORY_DOUBLE( __ticket_iva1_precio, h, ITEM_IVA1_PRECIO );
    _SET_MEMORY_DOUBLE( __ticket_iva1_rec, h, ITEM_IVA1_REC );
    _SET_MEMORY_DOUBLE( __ticket_iva2_precio, h, ITEM_IVA2_PRECIO );
    _SET_MEMORY_DOUBLE( __ticket_iva2_rec, h, ITEM_IVA2_REC );
    //  _SET_MEMORY_FLOAT( __ticket_ila_precio, h, ITEM_ILA_PRECIO );
    _SET_MEMORY_CHAR( __ticket_gravado, h, ITEM_GRAVADO );
    //  _SET_MEMORY_FLOAT( __ticket_ila_rec, h, ITEM_ILA_REC );
    //  _SET_MEMORY_FLOAT( __ticket_iha_precio, h, ITEM_IHA_PRECIO );
    //uso como variable temporal
	_SET_MEMORY_FLOAT( __ticket_alicuota_precio, h, 0 );
	_SET_MEMORY_DOUBLE( __ticket_descuento, h, 0.0 );
    //  _SET_MEMORY_FLOAT( __ticket_iha_rec, h, ITEM_IHA_REC );
    _SET_MEMORY_LONG( __ticket_cod_clasificacion, h, ITEM_COD_CLASIFICACION );
	_SET_MEMORY( __ticket_cod_clasificacion_str, h, ITEM_COD_CLASIFICACION_STR );
    _SET_MEMORY_CHAR( __ticket_tipo2, h, tipo2 );
    _SET_MEMORY_CHAR( __ticket_cod_movimiento, h, ITEM_COD_MOVIMIENTO );
    _SET_MEMORY_INT( __ticket_motivo_devolucion, h, ITEM_MOTIVO_DEVOLUCION );
    _SET_MEMORY_INT( __ticket_cod_promocion, h, _ITEM_COD_PROMOCION( 0 ) );
    _SET_MEMORY_INT( __ticket_cod_promocion1, h, _ITEM_COD_PROMOCION( 1 ) );
    _SET_MEMORY_INT( __ticket_cod_promocion2, h, _ITEM_COD_PROMOCION( 2 ) );
    _SET_MEMORY_INT( __ticket_cod_promocion3, h, _ITEM_COD_PROMOCION( 3 ) );

	_SET_MEMORY_INT( __ticket_plazo_financiacion, h, ITEM_ENG_COD_ALTO );//Variable reutilizada para ev_cont.precio_promocion = articulo.cod_articulo_promocion
	_SET_MEMORY_FLOAT( __ticket_cant_excl_desc, h, ITEM_COD_BAJO ); //Se reutiliza este campo para indicar el cod_iva de articulo.cod_exento (codigo iva aplicado al articulo)
	    _SET_MEMORY_INT( __ticket_cod_solicitud, h, ITEM_COD_SOLICITUD );
    _SET_MEMORY_INT( __ticket_cod_banda, h, ITEM_COD_BANDA );
    _SET_MEMORY_CHAR( __ticket_hab_desc_cliente, h, ITEM_HAB_DESC_CLIENTE );
    /*Se agrega porque asi lo maneja Salvador:
      evaluo la condicion del articulo porque a esta altura el item está
      harcodeado para que sea gravado si es DIPLOMATICO o EXPORTACION */
     //if( CR1_ITEM_GRAVADO(  ) || EXENTO_COMO_GRAVADO( ITEM_GRAVADO ) ) {
    if( CR1_ITEM_GRAVADO( )
            || EXENTO_COMO_GRAVADO( ITEM_GRAVADO )
            || ( config.pais == ELSALVADOR && articulo.gravado == 'S' ) ) {
        //_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, ROUND(ITEM_PRECIO_SIN_IVA,ENTEROS,2) );
		_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, ITEM_PRECIO_SIN_IVA );
        _SET_MEMORY_DOUBLE( __ticket_exento, h, 0 );
    }
    else {
        _SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, 0 );
        _SET_MEMORY_DOUBLE( __ticket_exento, h, ITEM_PRECIO_SIN_IVA );
    }   

   //EN LAS DEVOLUCIONES/NC TOTALES EL articulo de promo si habia items exentos y grabado se graba con iva, importe sin iva y exento.
   //se acumulaba todo en importe sin iva
   if( ( RAM_MODO_DEVOLUCION == 6 || RAM_MODO_DEVOLUCION ==2 )
       && fabs(event_e_ticket.importe_sin_iva)> 0.01 &&  fabs(event_e_ticket.exento) > 0.01 && event_e_ticket.origen == 4 ){
      _SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, event_e_ticket.importe_sin_iva );
      _SET_MEMORY_DOUBLE( __ticket_exento, h, event_e_ticket.exento );
      _SET_MEMORY_DOUBLE( __ticket_iva1_precio, h, event_e_ticket.iva1 );
    }

    _SET_MEMORY_CHAR( __ticket_cod_impuesto1, h, _ITEM_COD_IMPUESTO( 0 ) );
    _SET_MEMORY_CHAR( __ticket_cod_impuesto2, h, _ITEM_COD_IMPUESTO( 1 ) );
    _SET_MEMORY_CHAR( __ticket_cod_impuesto3, h, _ITEM_COD_IMPUESTO( 2 ) );
    _SET_MEMORY_CHAR( __ticket_cod_impuesto4, h, _ITEM_COD_IMPUESTO( 3 ) );
    _SET_MEMORY_CHAR( __ticket_cod_ext, h, 0 );
    _SET_MEMORY_DOUBLE( __ticket_descuento_cantidad, h, ITEM_DESCUENTO_CANTIDAD );
    imp = ROUND( RAM_CANTIDAD * ( ITEM_PRECIO + ITEM_RECARGO ), 10, DECIMALES );
    _SET_MEMORY_CHAR( __ticket_atrib_descuentos, h, ITEM_ATRIB_DESCUENTOS );
    _SET_MEMORY_CHAR( __ticket_origen, h, origen );
	SET_MEMORY_CHAR( __item_origen, origen ); //carquemos el origen del item
    _SET_MEMORY_INT( __ticket_cod_cobro, h, ITEM_COD_COBRO );
    _SET_MEMORY( __ticket_cod_barra, h, ITEM_COD_BARRA );
    _SET_MEMORY( __ticket_cod_barra_original, h, ITEM_COD_BARRA/*RAM_COD_BARRA_ORIGINAL*/ );
    _SET_MEMORY_FLOAT( __ticket_impuesto1_precio, h, _ITEM_MONTO_IMPUESTO( 0 ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto2_precio, h, _ITEM_MONTO_IMPUESTO( 1 ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto3_precio, h, _ITEM_MONTO_IMPUESTO( 2 ) );
    _SET_MEMORY_FLOAT( __ticket_impuesto4_precio, h, _ITEM_MONTO_IMPUESTO( 3 ) );
    _SET_MEMORY_DOUBLE( __ticket_precio_original, h,
                        ( ITEM_CREDITO ) ? ITEM_PRECIO_ORIGINAL : 0.00 );
    if( resta ) {
        imp = -imp;
    }
    if( !( ITEM_CREDITO ) ) {
        ADD_MEMORY_DOUBLE( __ram_total_vta_contado, imp );
    }
    //if( grabar )
    GRABAR_TICKET( h );
}
/*****************************************************************************/
int CR1_PIDE_CODIGO( void )
/*****************************************************************************/
{
    char cadena[17];
    char seguir = 1, asignar = 0, mostrar = 1, resetear_variables = 1;
    int posicion = 0, k, _k = 0, x, desplazamiento, rta = 13, c_cajon = 0, band, ok;
    int validar_cobro = 0;
    /*--------------------------- limpia buffer ------------------------------*/
    x = ( 80 - ancho_panel - L_CODIGO * 3 + 1 ) / 2;
    CURSOR_ON();
    _ATRIBUTO = 52;
    SET_ESPERA_GETCH( 2 );
    while( seguir ) {
        if( resetear_variables ) {
            _leyo_scanner = 0;
            memset( cadena, ' ', L_CODIGO );
            cadena[L_CODIGO] = 0;
            resetear_variables = 0;
            posicion = 0;
            mostrar = 1;
        }
        if( mostrar ) {
            MOSTRAR_INPUT_CADENA( cadena, posicion );
            mostrar = 0;
        }
        if( _k ) {
            k = _k;
            _k = 0;
        }
        else {
			k = GETCH();
        }
        if( !MODO_DEBUG ) {
            BORRAR_MENSAJE();
        }
        switch( k ) {
            case -83:
                /*-------- DEL ---------*/
                strset( cadena, ' ' );
                posicion = 0;
                break;
            case 13:
                /*-------- ENTER --------*/
                if( posicion > 0 ) {
                    asignar = 1;
                    validar_cobro = 1;
                    seguir = 0;
                    if( !pedir_cod_especial && ES_CODIGO_TOMRA( cadena )
                     && MODALIDAD_TOMRA == _NORMAL ) {
                        nro_ticket_tomra = TOMAR_CODIGO_TOMRA( cadena );
                        pedir_cod_especial = 1;
                        usar_cadena_scanner = 1;
                    }
                }
                break;
            case 10:
                /*-------- CTRL-ENTER --------*/
                if( posicion > 0 ) {
                    asignar = 1;
                    validar_cobro = 1;
                    seguir = 0;
                    pedir_cod_especial = 1;
                }
                break;
            case '+':
                /*-------- SUMA (repite articulo) --------*/
                if( posicion == 0 && RAM_P_TICKET > 0 && !ITEM_PESABLE && !ITEM_PEDIR_PRECIO
                 && !RAM_PROHIBIR_MAS && !RAM_DIFERENCIA_PRECIO ) {
                    /*----- Controla que no se remita un descuento porcentual ----*/
                    if( _TICKET_ORIGEN( RAM_P_TICKET - 1 ) != _ORIGEN_DESC_PORC
						&& _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 1 ) != _DIFERENCIA_PRECIO
						&& !( _TICKET_TIPO( RAM_P_TICKET - 1 ) & _RESTA ) ) {
                        asignar = 2;
                        validar_cobro = 1;
                        seguir = 0;
                    }
                }
                break;
            case '*':
                /*-------- MULTIPLICA --------*/
                if( posicion > 0 && RAM_CANTIDAD == 1 && !RAM_PRECIO_MAS_BAJO ) {
                    seguir = 0;
                    asignar = 3;
                    validar_cobro = 1;
                    rta = '*';
                }
                break;
            case 45:
                /*---- '-' ----*//*--- anulacion ---*/
                if( RAM_P_TICKET ) {
                    SET_MEMORY_INT( __ram_cod_funcion, 3 );
                }
                break;
            case '/':
                /*-------- ENGANCHE OPCIONAL DE ENVASES --------*/
				//Se analiza que el ultimo articulo tickeado sea envase y no sea diferencia de precio
			
				if( ( _TICKET_TIPO( RAM_P_TICKET - 1 ) & _ENVASE ) /*ITEM_ENVASE */) {                        
					if( !( _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 1 ) == _DIFERENCIA_PRECIO
							|| _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 1 ) == _DIFERENCIA_PRECIO_ANULADA )  ) {
						DestroyPanel( pPanel );
						if( HAY_COD_DEV_ENVASE ) {
							if( ITEM_COD_ENGANCHE ) {
								cod_especial = ITEM_COD_ENGANCHE;
								asignar = 0;
								seguir = 0;
							}
						}
						else {
							if( _TICKET_CANTIDAD( RAM_P_TICKET - 1 ) > 0 ) {
							   glog( "ENGANCHE OPCIONAL DE ENVASES" ,LOG_VENTAS,4);
								cod_especial = /*ITEM_COD_ESPECIAL*/_TICKET_COD_ESPECIAL( RAM_P_TICKET - 1 );
								
								/* no controlaba la cantidad de envases en la NC Libre. sino que los sumaba.. 
								//por esta condicion..*/
								if( !( RAM_NOTA_CR > 0 
										&& RAM_MODO_DEVOLUCION == _NC_LIBRE 
										&& DEBE_PEDIR_PRECIO_DEVOL_Y_NC( RAM_MODO_DEVOLUCION ))) {
									SET_MEMORY_DOUBLE( __ram_cantidad, - RAM_CANTIDAD );						
								}

								/*esto hace que se devuelva la cantidad exactamente igual a la vendida...
								pero se comenta para que intrduzcan n x '/'--- ej 3*'/' 2*'/'
								SET_MEMORY_DOUBLE( __ram_cantidad,
												   -( _TICKET_CANTIDAD( RAM_P_TICKET - 1 ) ) );*/
																	

								SET_MEMORY_CHAR( __ram_negativo, 1 ); //libertad
								asignar = 0;
								seguir = 0;
							}
						}
						Orden = NUMERO_PANEL_PRINCIPAL;
						PreparePanel( &pPanel, Orden );
						ShowPanel( pPanel );
					}
					else
					{
						//No se devuelve envase a un item con caracteristica envase y diferencia de precio
						if( (_TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 1 ) == _DIFERENCIA_PRECIO
								|| _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 1 ) == _DIFERENCIA_PRECIO_ANULADA ) ) {
							MENSAJE( ST( S_NO_SE_PUEDE_DEVOLVER_ENVASE_ULTIMO_ARTICULO ) );
							BORRAR_MENSAJE();
						}
					}
				}
                break;
            case 8:
                /*--------- BACKSPACE ---------*/
                posicion--;
                if( posicion < 0 ) {
                    posicion = 0;
                }
                cadena[posicion] = ' ';
                mostrar = 1;
                break;
            case 27:
                /*-------- ESC ---------*/
                if( !ancho_panel ) {
                    band = 1;
                }
                else {
                    if( Orden == NUMERO_PANEL_PRINCIPAL ) {
                        band = 1;
                    }
                    else {
                        band = 0;
                    }
                }
                if( band ) {
                    resetear_variables = 1;
                    if( RAM_CANTIDAD != 1 ) {
                        SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
                        SET_MEMORY_INT( __ram_cant_bultos, 1 );
                        CR1_MUESTRA_MULTIPLICACION();
                    }
                    else if( RAM_ANULACION ) {
                        SET_MEMORY_INT( __ram_anulacion, 0 );
						MOSTRAR_ANULACION(  );
                        LIMPIAR_RECUADRO_PETICION( 0 );
                    }
                    else if( RAM_PEDIR_CLIENTE_AFINIDAD ) {
                        SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 0 );
                        SET_MEMORY_LONG( __ram_cliente_afinidad, 0 );
                        MOSTRAR_CLIENTE_AFINIDAD();
                        LIMPIAR_RECUADRO_PETICION( 0 );
						if( RAM_PERFIL > 0){
							CEREAR_PERFIL_CLIENTE();
							MUESTRA_PERFIL();
						}
                    }
                    else if( RAM_PEDIR_NOVIOS ) {
                        SET_MEMORY_CHAR( __ram_pedir_novios, 0 );
                        SET_MEMORY_CHAR( __ram_novios, 0 );
                        LIMPIAR_RECUADRO_PETICION( 0 );
                    }
                    else if( USAR_ESCAPE_PARA_CIERRE ) {
						SET_MEMORY_INT( __ram_cierre_y, 1 );
                        if( CONDICIONES_PARA_CIERRE() ) {
                            seguir = 0;
                            asignar = 0;
                        }
						LIMPIAR_SCANNER(  );
                        mostrar = 1;
						SET_MEMORY_INT( __ram_cierre_y, 0 );
                    }
                    else if( !RAM_P_TICKET && SALIR_DESPUES_DE_CADA_TICKET ) {
                        // else if( !RAM_P_TICKET &&
                        // MULTIPLES_USUARIOS_PARA_VENTA ){
                        seguir = 0;
                        asignar = 0;
                        SET_MEMORY_INT( __ram_fin_ticket, 1 );
                    }
                }
                else {
                    DestroyPanel( pPanel );
                    Orden = NUMERO_PANEL_PRINCIPAL;
                    PreparePanel( &pPanel, Orden );
                    ShowPanel( pPanel );
                }
                break;
            case 119:
                /*---- ALT Y ----*//*--- Cierre ---*/
                SET_MEMORY_INT( __ram_cierre_y, 1 );
                if( CONDICIONES_PARA_CIERRE() ) {
                    seguir = 0;
                    asignar = 0;
                }
                mostrar = 1;
				SET_MEMORY_INT( __ram_cierre_y, 0 );
                break;
            case -23:
                /*---- Alt+I ----*//*--- INFORMES ---*/
                SETEAR_FUNCION( 6, !RAM_P_TICKET );
                break;
            case -59:
                /*---- F1 ----*//*--- Consulta ticket ---*/
                SETEAR_FUNCION( 1, RAM_P_TICKET );
                break;
            case -60:
                /*---- F2 ----*//*--- Retiros ---*/
                SETEAR_FUNCION( 8, !RAM_P_TICKET );
                break;
            case -61:
                /*---- F3 ----*//*--- autoriza anulaciones ---*/
                SETEAR_FUNCION( 2, RAM_P_TICKET && AUTORIZAR_ANULACIONES == 1 );
                break;
            case -62:
                /*---- F4 ----*//*--- anulacion completa ---*/
                SETEAR_FUNCION( 4, RAM_P_TICKET );
                break;
            case -63:
                /*---- F5 ----*//*--- fondo fijo ---*/
                if( FONDO_FIJO_MULTIPLES_MEDIOS ) {
                    SETEAR_FUNCION( 58, !RAM_P_TICKET );
                }
                else {
                    SETEAR_FUNCION( 5, !RAM_P_TICKET );
                }
                break;
            case -64:
                /*--- F6 ---*//*--- CONSULTA CODIGO ESPECIAL ---*/
                ClearPanel( pPanel );
                if( CR1_CONSULTA( 3 ) ) {
					consulta_x_no_template = 1;
                    cod_especial = articulo.cod_interno;
                    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                        SET_MEMORY( __ram_cod_barra, articulo.cod_barra );
                        cod_interno_cod_barra = 1;
                    }
                    seguir = 0;
                    asignar = 0;
                    validar_cobro = 1;
                }
                Orden = 1;
                Response = PreparePanel( &pPanel, Orden );
                if( Response ) {
                    ShowPanel( pPanel );
                }
                _ATRIBUTO = 52;
                break;
            case '$':
                /*--- CONSULTA MARCA ---*/
                ClearPanel( pPanel );
                if( CR1_CONSULTA( 5 ) ) {
					consulta_x_no_template = 1;
                    cod_especial = articulo.cod_interno;
                    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                        SET_MEMORY( __ram_cod_barra, articulo.cod_barra );
                        cod_interno_cod_barra = 1;
                    }
                    seguir = 0;
                    asignar = 0;
                    validar_cobro = 1;
                }
                Orden = 1;
                Response = PreparePanel( &pPanel, Orden );
                if( Response ) {
                    ShowPanel( pPanel );
                }
                _ATRIBUTO = 52;
                break;
            case -65:
                /*---- F7 ----*//*--- Validacion tarjeta ---*/
				if (!RAM_PEDIR_CLIENTE_AFINIDAD) {
                SETEAR_FUNCION( 9,
                                ( !RAM_P_TICKET || config.momento_recargo != AL_RENGLON )
                             && ( !RAM_P_TICKET || !PASAR_TARJETA_SOLO_AL_INICIO ) );
				} else {
					SET_MEMORY_CHAR( __ram_pedir_cliente_afinidad, 2 );
				}
                break;
            case -66:
                /*---- F8 ----*//*--- CONSULTA DESCRIPCION ---*/
                ClearPanel( pPanel );
                if( CR1_CONSULTA( 2 ) ) {
					consulta_x_no_template = 1;
                    cod_especial = articulo.cod_interno;
                    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                        SET_MEMORY( __ram_cod_barra, articulo.cod_barra );
                        cod_interno_cod_barra = 1;
                    }
                    seguir = 0;
                    asignar = 0;
                    validar_cobro = 1;
                }
                Orden = 1;
                Response = PreparePanel( &pPanel, Orden );
                if( Response ) {
                    ShowPanel( pPanel );
                }
                _ATRIBUTO = 52;
                break;
            case '%':
                /*---- % ----*//*--- CONSULTA CODIGO SCANNER ---*/
                ClearPanel( pPanel );
                if( CR1_CONSULTA( 1 ) ) {
					consulta_x_no_template = 1;
                    cod_especial = articulo.cod_interno;
                    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                        SET_MEMORY( __ram_cod_barra, articulo.cod_barra );
                        cod_interno_cod_barra = 1;
                    }
                    seguir = 0;
                    asignar = 0;
                    validar_cobro = 1;
                }
                Orden = 1;
                Response = PreparePanel( &pPanel, Orden );
                if( Response ) {
                    ShowPanel( pPanel );
                }
                _ATRIBUTO = 52;
				LIMPIAR_SCANNER(  ); 
                break;
            case -67:
                /*---- F9 ----*//*--- devoluciones ---*/
                SETEAR_FUNCION( 10, !RAM_P_TICKET && DEVOLUCION_HABILITADA );
                break;
            case -68:
                /*---- F10 ----*//*--- adelantos ---*/
                SETEAR_FUNCION( 7, !RAM_P_TICKET );
                break;
            case -133:
                /*---- F11 ----*//*---- Peticion de codigo especial ---*/
                SETEAR_FUNCION( 11, SI );
                break;
            case -134:
                /*---- F12 ----*//*--- Peticion de presentaciones ---*/
                SETEAR_FUNCION( 12, PRESENTACIONES_HABILITADAS );
                break;
            case -31:
                /*---- ALT S ----*//*--- Habilita scanner ---*/
                SETEAR_FUNCION( 13, SI );
                break;
            case 38:
                /*---- & (STAND BY)  ----*/
                SETEAR_FUNCION( 14, !RAM_P_TICKET );
                break;
            case 32:
                /*---- ESPACIO ----*//*---- TOTAL -----*/
                if( ( config.pais == CHILE || config.pais == BRASIL )
                 && MOSTRAR_MENSAJE_DE_ENVASES ) {
                    MENSAJE_TECLA_STRING( S_RECUERDE_SOLICITAR_DEVOLUCION_DE_ENVASES );
                }
                ok = RAM_P_TICKET
                     && ( ( RAM_TOTAL > 0.009 && !MODO_DEVOLUCION && !NC_PERMANENTE )
                     /*|| ( RAM_NOTA_CR && !NC_PERMANENTE )*/
                     || ( RAM_CAMBIO && RAM_TOTAL > -0.009 )
                     || RAM_MODO_EJECUCION == INVENTARIO
                     || ( ( MODO_DEVOLUCION || RAM_NOTA_CR || NC_PERMANENTE )
                          && ( RAM_TOTAL < -0.009 || ( config.pais == ECUADOR && RAM_TOTAL <= 0.0 ) ) ) );
				
				

                if( !ok ) {
                    MENSAJE_STRING( S_NO_PUEDE_FINALIZAR_LA_OPERACION );
                }
				VERIFICAR_EVENTOS( );
                SETEAR_FUNCION( 30, ok );

				if( RAM_MODO_EJECUCION != INVENTARIO ){
				/****** ***************************************/
				if( ok && !FISCAL_TOTAL_COMPROBANTE( SI ) ) {
						SETEAR_FUNCION( 0, SI );
					CR1_INCOSISTENCIA_FISCAL( NO ,0 , SI, SI );
					ok = NO;
				}
				}
				/****** *********************/

                break;
            case 9:
                /*---- TAB ----*//*---- REPROCESAR -----*/
                SETEAR_FUNCION( 29, !RAM_P_TICKET && !RAM_NO_REPROCESAR );
                break;
            case -30:
                /*---- ALT A ----*//*---- Factura ---*/
                if( UTILIZA_SOCIOS ) {
                    SETEAR_FUNCION( 20, RAM_NRO_CLIENTE && !RAM_P_TICKET );
                } else {
                    SETEAR_FUNCION( 15, FACTURACION_HABILITADA && !FACT_INDEP_DEL_TIPO_DE_CLIENTE && !RAM_P_TICKET && RAM_MODO_EJECUCION != INVENTARIO);
                }
                break;
            case -48:
                /*---- ALT B ----*//*--- Reimprimir Factura ---*/
                SETEAR_FUNCION( 21, !RAM_P_TICKET );
                break;
            case -46:
                /*---- ALT C ----*//*---- Cobranzas ---*/
                //if( !RAM_MODO_DEVOLUCION ) {
                    SETEAR_FUNCION( 16,!((RAM_P_TICKET&&IMPRESORA_FISCAL)|| RAM_FACTURA) );
                //}else{
                    //MENSAJE("SALGA DEL MODO DEVOLUCION PARA INGRESAR A COBROS!");
                //}
                break;
            case -25:
                /*---- ALT P ----*//*---- Pagos ---*/
                SETEAR_FUNCION( 17, !RAM_P_TICKET );
                break;
            case 108:
                /*---- ALT L ----*//*---- Cliente Alfabetico ---*/
                CEREAR_PERFIL_CLIENTE();
				SETEAR_FUNCION( 18,
                                ( !RAM_P_TICKET || config.pais == ELSALVADOR ) && !RAM_FACTURA );
				break;
            case 109:
                /*---- ALT K ----*//*---- Cliente Numerico ---*/
                SETEAR_FUNCION( 19,
                                ( !RAM_P_TICKET || config.pais == ELSALVADOR ) && !RAM_FACTURA );
                break;
            case 110:
                /*---- ALT J ----*//*---- Cliente Afinidad ---*/
                SETEAR_FUNCION( 22, ( !RAM_P_TICKET || config.pais == ELSALVADOR ) );
                break;
            case 111:
                /*---- ALT H ----*//*---- Ventas pendientes ---*/
                SETEAR_FUNCION( 23, SI );
                break;
            case 112:
                /*---- ALT G ----*//*---- Remito ---*/
                SETEAR_FUNCION( 24, !RAM_P_TICKET );
                break;
            case 113:
                /*---- ALT F ----*//*---- Men externo ---*/
                SETEAR_FUNCION( 25, !RAM_P_TICKET );
                break;
            case 114:
                /*---- ALT D ----*//*---- Pide vendedor ---*/
                SETEAR_FUNCION( 26, !RAM_P_TICKET || PERMITE_CAMBIAR_VENDEDOR );
                break;
            case 115:
                /*---- ALT O ----*//*---- Nota de Crdito ---*/
                SETEAR_FUNCION( 27, !RAM_P_TICKET );
                break;
            case 116:
                /*---- ALT U ----*//*---- Modo inventario ---*/
                SETEAR_FUNCION( 28,
                                !RAM_P_TICKET && ( RAM_MODO_EJECUCION != ENTRENAMIENTO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) );
                break;
            case 117:
                /*---- ALT R ----*//*---- Nro rollo interno ---*/
                SETEAR_FUNCION( 31, !RAM_P_TICKET );
                break;
            case 118:
                /*---- ALT T ----*//*---- Modo Credito ---*/
                SETEAR_FUNCION( 32, !RAM_P_TICKET || PERMITIR_TECLA_CREDITO );
                break;
            case 120:
                /*---- ALT E ----*//*---- Facturacion de remitos  ---*/
                SETEAR_FUNCION( 33, !RAM_P_TICKET );
                break;
            case 121:
                /*---- ALT W ----*//*----  Reimpresion de remitos ---*/
                SETEAR_FUNCION( 34, !RAM_P_TICKET );
                break;
            case 122:
                /*---- ALT Q ----*//*---- Pide mozo ---*/
                SETEAR_FUNCION( 35, !RAM_P_TICKET );
                break;
            case 123:
                /*---- Consulta saldo tarjeta ---*/
                SETEAR_FUNCION( 36, SI );
                break;
            case 124:
                /*---- Ingresa mutual ---*/
                #ifdef COMPILAR_MUTUALES
                SETEAR_FUNCION( 37, !RAM_P_TICKET && TRATAMIENTO_MUTUALES );
                #endif
                break;
            case 126:
                /*-------- (~) Intereses ------*/
                if( NRO_VARIACION_PRECIO ) {
                    CR1_INTERESES();
                }
                break;
            case 125:
                /*---- Ticket en espera ---*/
                SETEAR_FUNCION( 38,
                                ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS )
                             && !RAM_NRO_PEDIDO_PIN
                             && !RAM_NOTA_CR
                             && !MODO_DEVOLUCION 
							 && RAM_CAMBIO != 1);
				if( RAM_CAMBIO == 1 ) 
					MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_NO_SE_PUEDE_MANDAR_ESPERA ), 18 );
                break;
            case 128:
                /*---- Cambio Mercaderia  ---*/
                SETEAR_FUNCION( 39, ( !IMPRESORA_FISCAL ) );// NO ESTA HABILITADA LA FUNCIONALIDAD PARA IMPRESORAS FISCALES SI );
				//Se comenta mensaje a pedido de TST
				/*if( IMPRESORA_FISCAL  ) 
					MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_NO_SE_PUEDE_REALIZAR_CAMBIO ), 18 );*/
					
                break;
            case 127:
                /*---- Subtotal ---*/
                SETEAR_FUNCION( 40, RAM_P_TICKET );
                break;
            case 129:
                /*---- Perfil ---*/
                SETEAR_FUNCION( 41, SI );
                break;
            case 130:
                /*---- Descuento % ---*/
                SETEAR_FUNCION( 42,
					RAM_P_TICKET && ( RAM_MODO_EJECUCION != INVENTARIO )
                    && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) );
                break;
            case 131:
                /*---- Descuento $ 1 ---*/
				SETEAR_FUNCION( 43,
					RAM_P_TICKET && ( RAM_MODO_EJECUCION != INVENTARIO )
					&& ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) );
                break;
            case 132:
                /*---- Menu de funciones ---*/
                if( VARIOS_SCANNER_HABILITADO ) { //Deshabilito el scanner
					CR1_HABILITA_SCANNER( NO );
				}
				_k = SELECCIONA_FUNCION( 1 );
				if( !VARIOS_SCANNER_HABILITADO ) { //Deshabilito el scanner
					CR1_HABILITA_SCANNER( NO );
				}
                break;
            case 133:
                /*---- Consulta de estado de la tarjeta ---*/
                SETEAR_FUNCION( 44, ( !RAM_P_TICKET || !PASAR_TARJETA_SOLO_AL_INICIO ) );
                break;
            case 134:
                /*---- Cobro de la tarjeta CMR ---*/
                SETEAR_FUNCION( 45, !RAM_P_TICKET && !RAM_FACTURA );
                break;
            case 135:
                /*---- Tecla novios ---*/
                SETEAR_FUNCION( 46, SI );
                break;
            case 136:
                /*---- Tecla CUS ---*/
                SETEAR_FUNCION( 47, SI );
                break;
            case 137:
                /*--- Abrir cajn --*/
                SETEAR_FUNCION( 48, !RAM_P_TICKET );
                break;
            case 138:
                /*--- Descuento $ 2 --*/
				SETEAR_FUNCION( 49, SI );
                break;
            case 139:
                /*--- Descuento $ 3 --*/
                SETEAR_FUNCION( 50, SI );
                break;
            case 140:
                /*--- Ver cajones habilitados --*/
                SETEAR_FUNCION( 51, SI );
                break;
            case 141:
                /*--- Pedir datos de Tarjetas --*/
                SETEAR_FUNCION( 52, SI );
                break;
            case 142:
                /*--- Avance en Efectivo CMR --*/
                SETEAR_FUNCION( 53, !RAM_P_TICKET );
                break;
            case 143:
                /*-------- Correccion (repite articulo) --------*/
                if( posicion == 0 && RAM_P_TICKET > 0 && !RAM_PROHIBIR_MAS ) {
                    if( !( _TICKET_TIPO( RAM_P_TICKET - 1 ) & _RESTA ) ) {
                        SET_MEMORY_CHAR( __ram_correccion, 1 );
                        CR1_ACTIVA_ANULACION();
                        asignar = 2;
                        validar_cobro = 1;
                        seguir = 0;
                    }
                }
                break;
            case 144:
                /*-------- Diferencia de precio --------*/
                GRABAR_LOG_SISTEMA_STRING( S_FUNCION_144_DIFERENCIA_DE_PRECIO,LOG_VENTAS,4 );
                if( RAM_DIFERENCIA_PRECIO ) {
                    SET_MEMORY_CHAR( __ram_diferencia_precio, 0 );
                }
                else if( RAM_P_TICKET && RAM_BULTOS && !MODO_DEVOLUCION ) {
                    SET_MEMORY_CHAR( __ram_diferencia_precio, 1 );
                }
                SET_MEMORY_CHAR( __ram_compromiso_200, 0 );
                SET_MEMORY_CHAR( __ram_precio_mas_bajo, 0 );
                MUESTRA_TIPO_DEVOLUCION();
                break;
            case 145:
                /*-------- Compromiso 200% --------*/
                #ifdef COMPILAR_COMPROMISO_200
                if( ( MODO_DEVOLUCION || NC_PERMANENTE ) && RAM_P_TICKET ) {
                    GRABAR_LOG_SISTEMA_STRING( S_FUNCION_145_COMPROMISO_200 ,LOG_VENTAS,4);
                    ok = 0;
                    if( _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 1 ) == _DEVOLUCION ) {
                        ok = 1;
                    }
                    else if( _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 1 ) == _DIFERENCIA_PRECIO
                          && RAM_P_TICKET > 1
                          && _TICKET_COD_MOVIMIENTO( RAM_P_TICKET - 2 ) == _DEVOLUCION
                          && _TICKET_COD_ESPECIAL( RAM_P_TICKET - 1 )
                          == _TICKET_COD_ESPECIAL( RAM_P_TICKET - 2 ) ) {
                        ok = 1;
                    }
                    /*---- Si no es por precio se pide autorizacion ---*/
                    if( ok && !( _TICKET_TIPO2( RAM_P_TICKET - 1 ) & _PEDIR_PRECIO ) ) {
                        ok = SOLICITAR_CLAVES( 30 - ancho_panel, 21, NO, SI,
                                               PEDIR_SUPERVISOR_COMPROMISO_200,
                                               ST( S_COMPROMISO_200_ ), NULL, RAM_COD_FUNCION, SI );
                    }
                    if( ok ) {
                        SET_MEMORY_CHAR( __ram_compromiso_200, 1 );
                        asignar = 2;
                        validar_cobro = 1;
                        seguir = 0;
                        SET_MEMORY_CHAR( __ram_precio_mas_bajo, 0 );
                        SET_MEMORY_CHAR( __ram_diferencia_precio, 0 );
                        MUESTRA_TIPO_DEVOLUCION();
                    }
                    else {
                        char aux[50];
                        memset(aux,0,sizeof(aux));
                        _snprintf( aux, sizeof(aux)-1, ST( S_NO_ES_POSIBLE_REG_COMPR_200 ) );
                        MENSAJE_CON_PAUSA( aux, 50 );
                        BORRAR_MENSAJE();
                    }
                }
                #endif
                break;
            case 146:
                /*-------- Compromiso precio mas bajo --------*/
                if( MODO_DEVOLUCION || NC_PERMANENTE ) {
                    GRABAR_LOG_SISTEMA_STRING( S_FUNCION_146_COMPROMISO_PRECIO_MAS_BAJO ,LOG_VENTAS,4);
                    if( RAM_PRECIO_MAS_BAJO ) {
                        SET_MEMORY_CHAR( __ram_precio_mas_bajo, 0 );
                    }
                    else {
                        SET_MEMORY_CHAR( __ram_precio_mas_bajo, 1 );
                    }
                    SET_MEMORY_CHAR( __ram_compromiso_200, 0 );
                    SET_MEMORY_CHAR( __ram_diferencia_precio, 0 );
                    MUESTRA_TIPO_DEVOLUCION();
                }
                break;
            case 147:
                /*--- Reimpresion de la validacin de cobros ---*/
                SETEAR_FUNCION( 54, !RAM_P_TICKET );
                break;
            case 148:
                /*--- Seleccionar offline ( en pagos ) ---*/
                break;
            case 149:
                /*-------- saldo cheque --------*/
                if( PEDIR_CLIENTE_EN_CHEQUES ) {
                    SETEAR_FUNCION( 55, SI );
                }
                else {
                    MENSAJE_STRING( S_CLIENTE_EN_CHEQUES_NO_CONF );
                }
                break;
            case 150:
                /*--- Reimprimir ticket ---*/
                //SETEAR_FUNCION( 56, !RAM_P_TICKET );
				MOSTRAR_SALDO_EFECTIVO();
                break;
            case 151:
                /*--- Reimprimir voucher ---*/
                SETEAR_FUNCION( 57, !RAM_P_TICKET );
                break;
            case 152:
                /*--- Descuento fijo % 1 --*/
                SETEAR_FUNCION( 59,
                                ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) 
							 && !RAM_DESCUENTO_IMPORTE );
                break;
            case 153:
                /*--- Descuento fijo % 2 --*/
                SETEAR_FUNCION( 60,
                                ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) 
							 && !RAM_DESCUENTO_IMPORTE );
                break;
            case 154:
                /*--- Descuento fijo % 3 --*/
                SETEAR_FUNCION( 61,
                                ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) 
							 && !RAM_DESCUENTO_IMPORTE );
                break;
            case 155:
                /*--- Descuento fijo % 4 --*/
                SETEAR_FUNCION( 62,
                                ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) );
                break;
            case 156:
                /*--- Descuento fijo % 5 --*/
                SETEAR_FUNCION( 63,
                                ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) );
                break;
            case 157:
                /*--- Arqueo de Cajero ---*/
                SETEAR_FUNCION( 64, !RAM_P_TICKET );
                break;
            case 158:
                /*--- Salida del cajero ---*/
                if( RAM_TICKET_EN_ESPERA ) {
                    char mensaje[80];
                    memset(mensaje,0,sizeof(mensaje));
                    _snprintf( mensaje, sizeof(mensaje)-1, ST( S_TIENE__D_TICKETS_EN_ESPERA ), RAM_TICKET_EN_ESPERA );
                    MENSAJE( mensaje );
                }
                else if( !RAM_P_TICKET && MULTIPLES_USUARIOS_PARA_VENTA ) {
                    seguir = 0;
                    asignar = 0;
                    SET_MEMORY_INT( __ram_fin_ticket, 1 );
                }
                break;
            case 159:
                /*------ Pedir Diferimiento ----*/
                break;
            case 160:
                /*--- Modo Entrenamiento ---*/
                SETEAR_FUNCION( 65, !RAM_P_TICKET );
                break;
            case 161:
                /*--- Cobros Varios -------*/
                SETEAR_FUNCION( 66, !RAM_P_TICKET && !RAM_FACTURA );
                break;
            case 162:
                SETEAR_FUNCION( 67, SI );
                break;
            case 163:
                SETEAR_FUNCION( 68, SI );
                break;
            case 164:
                SETEAR_FUNCION( 69, !RAM_P_TICKET && RAM_FACTURA );
                break;
            case 165:
                SETEAR_FUNCION( 70, SI );
                break;
            case 166:
                SETEAR_FUNCION( 71, SI );
                break;
            case 167:
                SETEAR_FUNCION( 72, SI );
                break;
            case 168:
                SETEAR_FUNCION( 73, SI );
                break;
            case 169:
                SETEAR_FUNCION( 74, SI );
                break;
            case 170:
                SETEAR_FUNCION( 75, SI );
                break;
            case 171:
                SETEAR_FUNCION( 76, !RAM_P_TICKET && RAM_MODO_DEVOLUCION );
                break;
            /* Codigo obsoleto
            case 172:
                if( !RAM_P_TICKET
                 && ( RAM_MODO_DEVOLUCION ? 1
                    : SOLICITAR_CLAVES( 30, 20, NO, SI, NO, ST( S__AUTORIZACION_DEVOLUCION_ ),
                                        ST( S_AUTORIZO_DEVOLUCIONES ), _F_SUP_MODO_DEVOLUCION, SI ) ) ) {
                    ok = 1;
                    if( !RAM_MODO_DEVOLUCION && PEDIR_MOTIVO_EN_DEVOLUCIONES ) {
                        SET_MEMORY_CHAR( __modo_devolucion, SI );
                        SET_MEMORY_INT( __ram_motivo_devolucion, ELEGIR_MOTIVO( NULL ) );
                        SET_MEMORY_CHAR( __modo_devolucion, NO );
                        if( RAM_MOTIVO_DEVOLUCION <= 0 ) {
                            ok = 0;
                        }
                    }

                    if( ok && !RAM_MODO_DEVOLUCION && PEDIR_DATOS_EN_DEVOLUCIONES
                     && !PEDIR_CAJA_Y_SECUENCIA ) {
						 ok = PEDIR_BOLETA_ANTERIOR();
                    }
                    if( ok ) {
                        SET_MEMORY_INT( __ram_modo_devolucion, !RAM_MODO_DEVOLUCION );

                        SET_MEMORY_CHAR( __modo_devolucion, !MODO_DEVOLUCION );
						//SET_MEMORY_CHAR( __ram_devolucion_parcial_ref, !RAM_DEVOLUCION_PARCIAL_REF );
                        MOSTRAR_MODO_DEVOLUCION();
                        if( config.pais == ELSALVADOR
                                && RAM_TIPO_COMP_ANTERIOR == NC_ORIGEN_FACT_B ) {
                            //CR1_FACTURA_TIPO( NO, FACTURA_TIPO_B );
                        }
                    }
                    if( !RAM_MODO_DEVOLUCION && RAM_PRECIO_MAS_BAJO ) {
                        SET_MEMORY_CHAR( __ram_precio_mas_bajo, 0 );
                        MUESTRA_TIPO_DEVOLUCION();
                    }
                }
                BORRAR_MENSAJE();
                break;
                */
            case 174:
                /*  CLIENTE POR DOCUMENTO  */
                SETEAR_FUNCION( 79, SI );
                break;
            case 175:
                /*** Reimpresion de Ticket Local ***/
                SETEAR_FUNCION( 80, !RAM_P_TICKET && HABILITA_COPIA_TICKET_PRN);
                break;
            case 176:
                /*** Descuento por $ del ultimo articulo ***/
                SETEAR_FUNCION( 81,
                                RAM_P_TICKET && ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) );
                break;
            case 177:
                /*** Cambio de medios de pagos ***/
                SETEAR_FUNCION( 82, !RAM_P_TICKET );
                break;

            case 178:
                //  Consulta de promociones
                SETEAR_FUNCION( 125, SI);
                break;

            case 179: /*** Descuento por % del ultimo articulo ***/
                SETEAR_FUNCION( 84,
                                RAM_P_TICKET && ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS ) );
                break;

            case 180:
                // Utilizada en Pagos.
                break;
            case 181:
                SETEAR_FUNCION( 85, SI );
                break;
            case 182:
                SETEAR_FUNCION( 86, !RAM_NRO_CLIENTE );
                break;
            case 183:
                SETEAR_FUNCION( 87, !RAM_P_TICKET );
                break;
            case 184:
                SETEAR_FUNCION( 88, SI );
                break;
            case 185:
                /*---- Consulta Habilitacion para Cheque ---*/
                SETEAR_FUNCION( 89, SI );
                break;
            case 186:
                /*---- De cheques en boletin ---*/
                SETEAR_FUNCION( 90, SI );
                break;
            case 187:
                SETEAR_FUNCION( 91, !RAM_P_TICKET );
                break;
            case 188:
                SETEAR_FUNCION( 92, SI );
                break;
            /* Codigo obsoleto
            case 189:
                /*  Referencia NC
                SETEAR_FUNCION( 93, RAM_NOTA_CR && !RAM_P_TICKET );
                break;
                */
            case 190:
                /*---- Reimprimir NC o Factura ---*/
                SETEAR_FUNCION( 94, RAM_FACTURA && !RAM_P_TICKET );
                break;
            case 191:
                /*Factura A para El Salvador*/
				SET_MEMORY_CHAR( __ram_tipo_comp_anterior, FACTURA_TIPO_A );
                SETEAR_FUNCION( 15, FACTURACION_HABILITADA
                        && FACT_INDEP_DEL_TIPO_DE_CLIENTE && !RAM_P_TICKET );
                break;
            case 192:
                /*Factura B para El Salvador*/
				SET_MEMORY_CHAR( __ram_tipo_comp_anterior, FACTURA_TIPO_B );
                SETEAR_FUNCION( 15, FACTURACION_HABILITADA
                        && FACT_INDEP_DEL_TIPO_DE_CLIENTE && !RAM_P_TICKET );
                break;
            case 193:
                /*Alta de Clientes */
                SETEAR_FUNCION( 97, SI );
                break;
            case 194:
                /* Data Entry */
                SETEAR_FUNCION( 98, !RAM_P_TICKET );
                break;
            case 195:
                /* Mostrar Version*/

                VERSION_DIALOG();
                break;
			case 197:
				/* Imprime Validación */
				SETEAR_FUNCION( 101, IMPRIMIR_VALIDACION_1_TECLA &&  VALIDACION_A_IMPRIMIR_TECLA > 0);
				break;
			
            case 201:/* Consulta Codigo Especial */
            /*#ifdef COMPILAR_DYNAKEY
			            ClearPanel( pPanel );
            #endif*/
                if( CR1_CONSULTA( 13 ) ) {
				    cod_especial = articulo.cod_interno;
				    SET_MEMORY( __ram_cod_barra, articulo.cod_barra );
                    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                        cod_interno_cod_barra = 1;
                    }
				    seguir = 0;
				    asignar = 0;
					validar_cobro = 1;
			    }
			    Orden = 1;
                /*#ifdef COMPILAR_DYNAKEY
			    Response = PreparePanel( &pPanel, Orden );
			    if( Response ) {
				    ShowPanel( pPanel );
			    }
                #endif*/
			    _ATRIBUTO = 52;
		        break;
            case 203:
                /* Devolucion Cobros*/
                SETEAR_FUNCION( 106, !RAM_P_TICKET );
                break;
		    case 204:
                /* Ticket Factura */
                SETEAR_FUNCION( 107, !RAM_P_TICKET );
                break;
			case 205:
                /* Cliente por codigo de barra*/
                SETEAR_FUNCION( 108, SI );
                break;
			case 206:
                /* Monedero Electronico*/
                SETEAR_FUNCION( 112, SI );
                break;
			case 207:
                /* Franqueo */
                SETEAR_FUNCION( 114, RAM_P_TICKET && !MODO_DEVOLUCION );
                break;
			/* Codigo Obsoleto
            case 208:
                /* Devolucion Parcial Referenciada
                SETEAR_FUNCION( 115, !RAM_P_TICKET && ( RAM_MODO_EJECUCION != INVENTARIO ) );
                break;
                */
            case 209:
                /* Nota de Credito Libre */
                SETEAR_FUNCION( 116, ( !RAM_P_TICKET &&
                                       ( !( RAM_FACTURA && !RAM_NOTA_CR ) ) &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( FACTURACION_HABILITADA == SI ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
            case 210: /* Nota de Credito Referenciada Parcial */
                SETEAR_FUNCION( 117, ( !RAM_P_TICKET &&
                                       ( !( RAM_FACTURA && !RAM_NOTA_CR ) ) &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( FACTURACION_HABILITADA == SI ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
            case 211: /* Nota de Credito Referenciada Intermedia */
                SETEAR_FUNCION( 118, ( !RAM_P_TICKET &&
                                       ( !( RAM_FACTURA && !RAM_NOTA_CR ) ) &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( FACTURACION_HABILITADA == SI ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
            case 212: /*---- Nota de Credito Total ---*/
                SETEAR_FUNCION( 119, ( !RAM_P_TICKET &&
                                       ( !( RAM_FACTURA && !RAM_NOTA_CR ) ) &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( FACTURACION_HABILITADA == SI ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
            case 213: /*---- Devolucion Libre ---*/
                SETEAR_FUNCION( 120, ( !RAM_P_TICKET /*&& !RAM_FACTURA*/ &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
            case 214: /*---- Devolucion Referenciada Parcial ---*/
                SETEAR_FUNCION( 121, ( !RAM_P_TICKET && !RAM_FACTURA &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
            case 215: /*---- Devolucion Referenciada Intermedia ---*/
                SETEAR_FUNCION( 122, ( !RAM_P_TICKET && !RAM_FACTURA &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
            case 216: /*---- Devolucion Total ---*/
                SETEAR_FUNCION( 123, ( !RAM_P_TICKET && !RAM_FACTURA &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( DEVOLUCION_HABILITADA == SI ) ) );
                break;
			case 217:
                /*---- Ticket en espera con Menu de seleccion para los tespera locales ---*/
                SETEAR_FUNCION( 124,
                                ( RAM_MODO_EJECUCION != INVENTARIO )
                             && ( RAM_MODO_EJECUCION != TRANSFERENCIAS )
                             /*&& !RAM_NRO_PEDIDO_PIN*/
                             && !RAM_NOTA_CR
                             && !MODO_DEVOLUCION
							 && RAM_CAMBIO != 1);
				if( RAM_CAMBIO == 1 ) 
					MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_NO_SE_PUEDE_MANDAR_ESPERA ), 18 );
                break;

			case 218:                 /* Importar ticket en modo factura */
				SETEAR_FUNCION( 126, (/*( RAM_FACTURA || RAM_TICKET_FACTURA )&&*/ !RAM_P_TICKET && RAM_MODO_EJECUCION != INVENTARIO ));
			break;
			case 219:                 /* Cambio de Medio Automatico */
				SETEAR_FUNCION( 127, ( config_tps.CambioMedioAutomatico && !RAM_P_TICKET && RAM_MODO_EJECUCION != INVENTARIO ) );
			break;
            case 800:
                MENSAJE_STRING( S_CIERRE_DE_LOTE );
                #ifdef COMPILAR_ON_LINE
                CIERRE_DE_LOTE( _CIERRE_DE_LOTE_MANUAL );
                #endif
                break;
			case 801:
                #ifdef COMPILAR_ON_LINE
				if( !RAM_P_TICKET)  {
					MENSAJE("ENVIANDO OPERACION PENDIENTE");
					ENVIAR_OPERACION_OFFLINE(  );//funcion para que en ventas se pueda enviar una operacion offline pendiente
				}
                #endif
                break;
			case 802:
                #ifdef COMPILAR_ANULACION_TARJETA
				//ojo si quiero deshabilitar la tarjeta ya pasada tipo ESC!!
				if( !RAM_P_TICKET)  {
					int resultado;
					MENSAJE("INGRESE SUPERVISOR PARA ANULAR COMPRA TARJETA");
					if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, "ANULACION COMPRA TARJETA",
						"ANULACION COMPRA TARJ", _F_SUP_AUTORIZACION_COMPRA_TARJ, SI )){
							resultado = ANULACION_TARJETA_TECLA(NO);
							if(resultado == 1)
								//MENSAJE("ANULACION DE COMPRA CON TARJETA EXITOSA!");
								MENSAJE_CON_PAUSA_CAJA( "ANULACION DE COMPRA CON TARJETA EXITOSA!", 127 );
							else
								MENSAJE("FALLO ANULACION DE COMPRA CON TARJETA");
					}
				
					BORRAR_MENSAJE();
					if(resultado == 1)
						MENSAJE_TECLA("DEBE REALIZAR LA NC REFERENCIADA CORRESPON -PRES. TECLA" );
					SET_MEMORY_CHAR( __var_tarjetas_status, 0 );
				CR1_MUESTRA_TARJETA();
				}
                #endif
                break;

				case 803: //desencriptar bloque encritado
           //     #ifdef COMPILAR_ANULACION_TARJETA
				 DESENCRIPTAR_BLOQUE();

			//	#endif
                break;
				case 804: /* Nota de Credito Referenciada Intermedia PARA ANULACIONES DE TARJETA */
                if( !RAM_P_TICKET && ( !( RAM_FACTURA && !RAM_NOTA_CR ) ) &&
                                       ( RAM_MODO_EJECUCION != INVENTARIO ) &&
                                       ( FACTURACION_HABILITADA == SI ) &&
									   ( DEVOLUCION_HABILITADA == SI ) ) {
				    SET_MEMORY_CHAR( __ram_importacion_pedidos, 1 );
					CR1_FACTURACION_CREDITO(  _NC_RF_INTERME );
					//CR1_FACTURACION_CREDITO(  _NC_TOTAL );
					//SET_MEMORY_CHAR( __ram_importacion_pedidos, 0 );
				}
                break;
     #ifdef COMPILAR_CARGAUTOMATICAITEMS
			case 903://levantar item solo para desarrollo
				LEVANTAR_ITEMS_DE_ARCHIVO( );
			break;
     #endif
			case 805:
				/*--- echo test al jauton mensaje 800 --*//* z */
                ECHO_TEST( );
                
                break;
			case 806:
				/*--- prueba informe*/
                INFORME_TARJETAS( _z );
                
                break;
            case -157:
                /*--- inicializar fiscal --*//* z */
                if( !RAM_P_TICKET ) {
                    FISCAL_INICIALIZAR();
                }
                break;
            case -158:
                if( !RAM_P_TICKET ) {
                    FISCAL_VERIFICACION();
                }
                break;
            case CODIGO_BOTON_PANEL_CLIENTE:
                Orden = NUMERO_PANEL_CLIENTE;
                DestroyPanel( pPanel );
                PreparePanel( &pPanel, Orden );
                ShowPanel( pPanel );
                break;
            case CODIGO_BOTON_PANEL_SUPERVISOR:
                Orden = NUMERO_PANEL_SUPERVISOR;
                DestroyPanel( pPanel );
                PreparePanel( &pPanel, Orden );
                ShowPanel( pPanel );
                break;
            case CODIGO_BOTON_PANEL_CAJERO:
                Orden = NUMERO_PANEL_CAJERO;
                DestroyPanel( pPanel );
                PreparePanel( &pPanel, Orden );
                ShowPanel( pPanel );
                break;
            case CODIGO_BOTON_PANEL_ESPECIALES:
                Orden = NUMERO_PANEL_ESPECIALES;
                DestroyPanel( pPanel );
                PreparePanel( &pPanel, Orden );
                ShowPanel( pPanel );
                break;
            case FLECHA_ARRIBA:
                Orden = NUMERO_PANEL_PRINCIPAL;
                DestroyPanel( pPanel );
                PreparePanel( &pPanel, Orden );
                ShowPanel( pPanel );
                break;
            case -999:
                /*------------- Sale por espera -------------------*/
                if( VARIOS_SCANNER_HABILITADO ) {
                    if( IS_SCANNER() ) {
                        CURSOR_OFF();
                        if( LEER_SCANNER( cadena, sizeof( cadena ) ) ) {
                            _leyo_scanner = 1;
                            cadena[L_CODIGO] = 0;
                            posicion = L_CODIGO - 1;
                            UNGETCH( 13 );
                            mostrar = 1;
                        }
                        else {
                            MENSAJE_STRING( S_ERROR_LECTURA_SCANNER );
                            memset( cadena, ' ', L_CODIGO );
                            cadena[L_CODIGO] = 0;
                            mostrar = 1;
                            posicion = 0;
                        }
                        CURSOR_ON();
                    }
                }
                if( _PERMITIR_BANDA && RAM_PEDIR_CLIENTE_AFINIDAD && LECTOR_TARJETA_LEIDA() ) {
                    _PERMITIR_BANDA = 0;
                    seguir = 0;
                }
                break;
            default:
                /*---------- Nmeros ----------*/
                if( k >= '0' && k <= '9'
                 || ( k == '.' && RAM_CANTIDAD == 1 && CANTIDADES_CON_DECIMALES ) ) {
                    if( posicion < L_CODIGO ) {
                        cadena[posicion++] = k;
                        mostrar = 1;
                    }
                }
                /*-------- PLU ---------*/
                else if( k > 900 && k < 1000 ) {
                    if( !RAM_P_TICKET ) {
                        ADD_MEMORY_LONG( __nro_evento, -1L );
                        VALIDAR( k - 900, 0, 0, 0, 0 );
                        ADD_MEMORY_LONG( __nro_evento, 1L );
                    }
                }
                else if( k >= 1000 ) {
                    cod_especial =  BUSCAR_PLU( k - 1000, cadena );
                    seguir = 0;
                    asignar = 0;
                }
                break;
        }

        if( ancho_panel && RestaurarPrimerPanel && Orden != NUMERO_PANEL_PRINCIPAL ) {
            RestaurarPrimerPanel = 0;
            DestroyPanel( pPanel );
            Orden = NUMERO_PANEL_PRINCIPAL;
            PreparePanel( &pPanel, Orden );
            ShowPanel( pPanel );
        }
        if( RAM_COD_FUNCION || RAM_PEDIR_CLIENTE_AFINIDAD == 2 ) {
            seguir = 0;
            asignar = 0;
        }
        /*---------------- Controla que el cajon est cerrado ------------*/
        if( c_cajon > 10 ) {
            if( CONTROLAR_CAJON ) {
                CERRAR_CAJON();
            }
            c_cajon = 0;
        }
        else {
            c_cajon++;
        }
    }
	/*-------------Para Cobros Venta Unicos-----------------*/
	{
		int online = 0, unico = 0;
		COBROS_VENTA_EN_TICKET( &online, &unico );
		if( unico >= 0 && validar_cobro && !RAM_ANULACION ) {
			MENSAJE( "HAY COBRO UNICO EN TICKET, NO SE PUEDE AGREGAR ARTICULO" );
			CEREAR_MEMORY( __ram_cod_barra );
			cod_especial = 0;
			asignar = 0;
		}
	}
	/*-----------------------------------------------------*/
    switch( asignar ) {
        case 1:
            /*-------------------- Asigna lo ingresado --------------------*/
            /*--------- Arregla formato del numero --------*/
            cadena[L_CODIGO] = 0;
            TRIM( cadena );
            desplazamiento = L_CODIGO - strlen( cadena );
            // reescribimos por que el memmove copia desde el principio
            // y no desde el final,
            // entonces pisa el final
            //memcpy(&cad_tmp[desplazamiento],cadena,L_CODIGO - desplazamiento);
            //memset(cad_tmp, ' ', desplazamiento);
            //memcpy(cadena,cad_tmp,L_CODIGO);
            memmove( &cadena[desplazamiento], cadena, L_CODIGO - desplazamiento );
            memset( cadena, ' ', desplazamiento );
            /* for(h=L_CODIGO-1; h>0 && cadena[h]==' '; h--); desplazamiento =
                 * L_CODIGO - 1 - h; for(; h>=0; h--) cadena[h+desplazamiento] =
                 * cadena[h]; memset(cadena, ' ', desplazamiento); */
            SET_MEMORY( __ram_cod_barra, cadena );
            if( pedir_cod_especial ) {
                cod_especial = atol( cadena );
            }
            break;
        case 2:
            /*------------ Asigna el ultimo codigo ------------*/
            menos = ( _TICKET_TIPO( RAM_P_TICKET - 1 ) & _ENGANCHADO ) ? 2 : 1;
            cod_especial = _TICKET_COD_ESPECIAL( RAM_P_TICKET - menos );
            if( RAM_CANTIDAD == 1 ) {
                SET_MEMORY_DOUBLE( __ram_cantidad, _TICKET_CANTIDAD( RAM_P_TICKET - menos ) );
                SET_MEMORY_INT( __ram_cant_bultos, ( int )RAM_CANTIDAD );
            }
            if( RAM_CORRECCION && ITEM_PEDIR_PRECIO ) {
                SET_MEMORY_INT( __ram_cant_bultos, 1 );
                SET_MEMORY_DOUBLE( __ram_cantidad, _TICKET_CANTIDAD( RAM_P_TICKET - menos ) );
            }
            SET_MEMORY_INT( __ram_present_ant, _TICKET_PRESENTACION( RAM_P_TICKET - menos ) );
            if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                cod_interno_cod_barra = 1;
                SET_MEMORY( __ram_cod_barra, _TICKET_COD_BARRA( RAM_P_TICKET - menos ) );
            }
            break;
        case 3:
            /*------------- asigna cantidad -----------------*/
            SET_MEMORY_DOUBLE( __ram_cantidad, atof( cadena ) );
            if( !CANTIDADES_CON_DECIMALES && strchr( cadena, '.' ) ) {
                SET_MEMORY_DOUBLE( __ram_cantidad, 1 );
            }
            break;
    }
    CURSOR_OFF();
    SET_ESPERA_GETCH( 0 );
    return ( rta );
}
/*****************************************************************************/
void SETEAR_FUNCION( int funcion, int condicion )
/*****************************************************************************/
{
    if( condicion ) {
        SET_MEMORY_INT( __ram_cod_funcion, funcion );
    }
    else {
        SET_MEMORY_INT( __ram_cod_funcion, 0 );
    }
}
/*****************************************************************************/
int ES_ENVASE( int h )
/*****************************************************************************/
{
    int rta = 0;
    if( HAY_COD_DEV_ENVASE ) {
        rta = ( _TICKET_TIPO( h ) & _DEV_ENVASE );
    }
    else {
        if( ( _TICKET_TIPO( h ) & _ENVASE ) && _TICKET_CANTIDAD( h ) < 0 ) {
            rta = 1;
        }
        else if( _TICKET_TIPO( h ) & _DEV_ENVASE ) {
            rta = 1;
        }
    }
    return ( rta );
}
/*****************************************************************************/
void CR1_INTERESES()
/*****************************************************************************/
{
    int color;
    /*--------------------- Pide clave de supervisor -------------------------*/
    color = _ATRIBUTO;
    if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, PEDIR_CLAVE_EN_INTERESES, ST( S_CLAVE ),
                          ST( S_CLAVE ), _F_SUP_INTERESES, SI ) ) {
        if( RAM_VARIAR_PRECIO ) {
            SET_MEMORY_CHAR( __ram_variar_precio, 0 );
            _ATRIBUTO = 7;
            MOSTRAR_CADENA( 17 - ancho_panel / 2, 24, "   " );
        }
        else {
            /*----------------------- Agrega intereses ----------------------------*/
            GRABAR_LOG_SUPERVISOR( "Variacin de precio", USUARIO_CLAVE_ANTERIOR(),LOG_VENTAS,4 );
            SET_MEMORY_CHAR( __ram_variar_precio, 1 );
            _ATRIBUTO = 32;
            MOSTRAR_CADENA( 17 - ancho_panel / 2, 24, " % " );
        }
    }
    _ATRIBUTO = color;
}
/********************************************************************************/
void CODIGO_INEXISTENTE()
/********************************************************************************/
{
    int espera, l_id, tomra = 0, tecla, HabScanner=0;
    char buffer[20];
    if( MODALIDAD_TOMRA ) {
        l_id = strlen( PREFIJO_TOMRA );
        strcpy( buffer, RAM_COD_BARRA );
        if( l_id && !RAM_PEDIR_CLIENTE_AFINIDAD && memcmp( buffer, PREFIJO_TOMRA, l_id ) == 0 ) {
            tomra = 1;
        }
    }
    if( !tomra ) {
        #ifdef COMPILAR_DISPLAY
        DISPLAY_LINEA( ST( S_______CODIGO________ ), 1 );
        DISPLAY_LINEA( ST( S_____INEXISTENTE_____ ), 2 );
        #endif
        if( TRABAR_CODIGO_INEXISTENTE ) {

            if( VARIOS_SCANNER_HABILITADO ) {
                CR1_HABILITA_SCANNER( NO );
                HabScanner = SI;
            }
            MENSAJE_SIN_SONIDO_STRING( S_CODIGO_INEXISTENTE );
            espera = GET_ESPERA_GETCH();
            SET_ESPERA_GETCH( 2 );
            while( 1 ) {
                        if( (tecla = GETCH(  )) != 27 && tecla != -999 )
                            ALARMA( 1 );
                        else
                            if(tecla != -999)
                                break;
                        //PAUSA( 2 );
                        SLEEP(1);
                    };
            if(HabScanner){
                CR1_HABILITA_SCANNER( NO );
            }
            //SET_RTS_SCANNER( 1 );
            SET_ESPERA_GETCH( espera );
            #ifdef COMPILAR_DISPLAY
            DISPLAY_LINEA( "clearText", 1 );
            DISPLAY_LINEA( "clearText", 2 );
            #endif
        }
        else {
            MENSAJE_STRING( S_CODIGO_INEXISTENTE );
			DISPLAY_LINEA( "clearText", 1 );
            DISPLAY_LINEA( "clearText", 2 );
        }
    }
}
/********************************************************************************/
int INGRESA_CODIGO_BARRAS()
/********************************************************************************/
{
    int  rta = 0;
    long nro;
    char cod[] = {"0000000000000000"};
    switch( TRATAMIENTO_CODIGO_0 ) {
        case 1:
            /*------------ Codigo de 6 digitos modulo 10 -------------*/
            nro = PEDIR_ENTERO_EN_RECUADRO( ST( S__CODIGO_DE_BARRAS_ ), NO, 6 );
            if( nro ) {
                ITOA( nro, &cod[7], 6 );
                CEREAR( cod, L_CODIGO );
                cod[L_CODIGO] = 0;
                if( EJECUTAR_L_VALID( &cod[7], 1 ) ) {
                    SET_MEMORY( __item_cod_barra, cod );
                    rta = 1;
                }
                else {
                    MENSAJE_STRING( S_CODIGO_INCORRECTO );
                }
            }
            break;
        case 2:
            /*------------ Codigo de barras normal -------------*/
            _PERMITIR_SCANNER = SI;
            if( PEDIR_CADENA_EN_RECUADRO( ST( S_CODIGO ), NO, L_CODIGO, cod, NULL ) ) {
                SET_MEMORY( __item_cod_barra, cod );
                rta = 1;
            }
            _PERMITIR_SCANNER = NO;
            break;
    }
    /*-------------------- Pide clave de supervisor ------------------------*/
    if( rta ) {
        if( !SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, PEDIR_SUPERVISOR_PARA_CODIGO_0,
                               ST( S_CLAVE ), NULL, _F_SUP_CODIGO_0, SI ) ) {
            rta = 0;
        }
    }
    return ( rta );
}
#ifdef COMPILAR_TECLA_MODO_CREDITO
/*****************************************************************************/
void HABILITAR_PRECIO_CR()
/*****************************************************************************/
{
    SET_MEMORY_CHAR( __ram_usar_precio_cr, ( RAM_USAR_PRECIO_CR ) ? 0 : 1 );
    if( RAM_USAR_PRECIO_CR ) {
        char aux[10];
        _itoa( FLAG_CR, aux, 10 );
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CRED_VTAS_AGREGAR, aux, " ", NULL, " " );
    }
    else {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_CRED_VTAS_BORRAR, " ", " ", NULL, " " );
    }
    MUESTRA_TIPO_PRECIO();
}
#endif
/*****************************************************************************/
void EVALUAR_CODIGOS_ESPECIALES( int largo_codigo )
/*****************************************************************************/
{
    int l_cod, l_id, es_balanza = 0, i = 0;
    char cadena[25],cadena_aux[25], d_verificador,codigo_tomra[10];
    long cod_tomra;
    char bak[25],id[20];
    strcpy( cadena, RAM_COD_BARRA );
    strcpy( bak, RAM_COD_BARRA );
    articulo_pesable = 0;
    SET_MEMORY_CHAR( __ram_pos_mask_eliminar, -1 );
	EVALUAR_COD_TEMPLATE();
	consulta_x_no_template = 0;
	//Cuando el codigo verificador es invalido
	strcpy( cadena_aux, RAM_COD_BARRA );
	TRIM( cadena_aux );
	if( !strlen( cadena_aux ) ) {
		strcpy( cadena, RAM_COD_BARRA );
		strcpy( bak, RAM_COD_BARRA );
	}

    /*-------------------- Artculos de confeccin ----------------------*/
    strncpy( id, ID_CONFECCION_INICIAL, sizeof( id ) );
    TRIM( id );
    l_id = strlen( id );
    if( memcmp( cadena, ID_CONFECCION_INICIAL, l_id ) >= 0
		&& memcmp( cadena, ID_CONFECCION_FINAL, l_id ) <= 0 ) {
        l_cod = strlen( cadena );
        memset( &cadena[l_cod - LARGO_CODIGO_DE_CONFECCION - 1], '0', LARGO_CODIGO_DE_CONFECCION );
        CALCULA_CRC_EAN13( cadena );
        SET_MEMORY( __ram_cod_barra, cadena );
        mantener_cod_barra = 1;
    }
    es_balanza = EVALUAR_CODIGOS_BALANZA( ID_BALANZA, cadena, largo_codigo );
	l_cod_busqueda2 =0;
	if( !es_balanza ) {
        es_balanza = EVALUAR_CODIGOS_BALANZA( PREFIJO_2_DE_BALANZA, cadena, largo_codigo );
    }
    if( !es_balanza ) {
        if( EVALUAR_CODIGOS_BALANZA( PREFIJO_ARTICULOS_POR_UNIDAD, cadena, largo_codigo ) ) {
            //articulos por unidad
            TOMAR_CANTIDAD_SCANNER();
        }
    }
	SET_MEMORY_CHAR( __ram_pos_mask_eliminar, -1 );
    /*-------------------- Devolucion de envases ----------------------*/
    strncpy( id, ID_DEVOLUCION_ENVASES, sizeof( id ) );
    TRIM( id );
    l_id = strlen( id );
    if( l_id && !RAM_PEDIR_CLIENTE_AFINIDAD && memcmp( cadena, ID_DEVOLUCION_ENVASES, l_id ) == 0 ) {
        d_verificador = cadena[largo_codigo - 1];
        CALCULA_CRC_EAN13( cadena );
        if( d_verificador == cadena[largo_codigo - 1] ) {
            if( config.pais == ELSALVADOR ) {
                // parche para que reconozca sus codigos de balanza.
                char cadena_aux[14];
                memset( cadena_aux, 0, 14 );
                memset( cadena_scanner, 0, 14 );
                memcpy( cadena_scanner, &cadena[largo_codigo - 6], 5 );
                memset( cadena_aux, '0', 13 );
                memcpy( &cadena_aux[7], &cadena, 6 );
                usar_cadena_scanner = 1;
                strcpy( cadena, cadena_aux );
                SET_MEMORY( __ram_cod_barra, cadena );
                articulo_pesable = 1;
            }
            else {
                memcpy( cadena_scanner, &cadena[largo_codigo - 6], 5 );
                usar_cadena_scanner = 1;
                memset( &cadena[largo_codigo - 6], '0', 5 );
                CALCULA_CRC_EAN13( cadena );
                SET_MEMORY( __ram_cod_barra, cadena );
                // mantener_cod_barra = 1;
            }
		}
		else {
            MENSAJE_STRING( S_DIGITO_VERIFICADOR_INVALIDO );
            memset( cadena, 0, strlen( cadena ) );
        }
    }
    /*---------------- Vouchers TOMRA - Devolucion de envases ----------------*/
    if( MODALIDAD_TOMRA ) {
        strcpy( cadena, bak ); //si ID_DEVOLUCION_ENVASE == PREFIJO_TOMRA
        strncpy( id, PREFIJO_TOMRA, sizeof( id ) );
        TRIM( id );
        l_id = strlen( id );
        if( l_id && !RAM_PEDIR_CLIENTE_AFINIDAD && memcmp( cadena, PREFIJO_TOMRA, l_id ) == 0 ) {
            if( RAM_P_TICKET || MODALIDAD_TOMRA == _POR_TIPO ) {
                d_verificador = cadena[largo_codigo - 1];
                CALCULA_CRC_EAN13( cadena );
                if( d_verificador == cadena[largo_codigo - 1] ) {
                    memset( codigo_tomra, 0, sizeof( codigo_tomra ) );
                    memcpy( &codigo_tomra, &cadena[OFFSET_NUMERO_TICKET_TOMRA - 1],
                            L_TICKET_TOMRA );
                    cod_tomra = atol( codigo_tomra );
                    if( MODALIDAD_TOMRA == _POR_ARCHIVO ) {
                        if( !BUSCAR_VOUCHER_TOMRA( cod_tomra, 0 ) || VOUCHER_REPETIDO( cod_tomra ) ) {
                            MENSAJE_STRING( S_TICKET_DE_ENVASES_INVALIDO );
                        }
                        else {
                            while( _RAM_NUMERO_TICKET_TOMRA( i ) ) {
                                i++;
                            }
                            if( i <= 4 ) {
                                _SET_MEMORY_LONG( __ram_numero_ticket_tomra, i, cod_tomra );
                                MUESTRA_ET_TOMRA();
                            }
                            else {
                                MENSAJE_STRING( S_EL_LIMITE_DE_T_DE_ENVASES_POR_TICKET_ES_5 );
                            }
                        }
                    }
                    else if( MODALIDAD_TOMRA == _POR_TIPO ) {
                        //i = 0;
                        //while( _RAM_NUMERO_TICKET_TOMRA( i ) ) i++;
                        //if( i < 5 ) {
                        if( CARGAR_ENVASES_POR_TIPO( cadena ) ) {
                            //_SET_MEMORY_LONG( __ram_numero_ticket_tomra, i, cod_tomra );
                            _SET_MEMORY_LONG( __ram_numero_ticket_tomra, 0,
                                              _RAM_NUMERO_TICKET_TOMRA( 0 ) + 1 );
                            MUESTRA_ET_TOMRA();
                        }
                        //} else {
                        //  MENSAJE_STRING( S_EL_LIMITE_DE_T_DE_ENVASES_POR_TICKET_ES_5 );
                        //}
                    }
				}
                else {
                    MENSAJE_STRING( S_DIGITO_VERIFICADOR_INVALIDO );
                }
            }
            else {
                MENSAJE_STRING( S_NO_PUEDE_PASAR_T_ENVASE_SIN_TICKEAR_ITEM );
            }
        }
    }
}
/*****************************************************************************/
void CAMBIA_ESPACIO_POR_CERO( char *m )
/*****************************************************************************/
{
    int j;
    for( j = 0;m[j] == 32;j++ ) {
        m[j] = '0';
    }
}
/*****************************************************************************/
void ARMA_COD_BARRA( char *cod_barra, double precio )
/*****************************************************************************/
{
    int dec, sign, ndig = 2;
    char temp[20],*str;
    str = _fcvt( precio, ndig, &dec, &sign );
    if( strlen( str ) > 5 ) {
        memset( temp, '0', 5 );
    }
    else {
        _snprintf( temp, sizeof(temp)-1,"%5s", str );
    }
    CAMBIA_ESPACIO_POR_CERO( temp );
    memcpy( &cod_barra[L_CODIGO - 6], temp, 5 );
    CALCULA_CRC_EAN13( cod_barra );
}
/*****************************************************************************/
int CONDICIONES_PARA_CIERRE()
/*****************************************************************************/
{
    int rta = 0, x, y, att, continuar = 1, cant;
    char mensaje_te[80] = {'\0'};
    
    DISABLE_STANDBY_AUTOMATICO();
    if( !RAM_P_TICKET ) {
        if( !IS_PRINTER() ) {
            MENSAJE_STRING( S_IMPRESORA_FUERA_LINEA );
            continuar = 0;
        }
        else if( RAM_MODO_DEVOLUCION ) {
            MENSAJE_STRING( S_DEBE_SALIR_DE_MODO_DEVOLUCION );
            continuar = 0;
        }
        else if ( MODALIDAD_TICKET_EN_ESPERA == 0 ) { 
            if( RAM_TICKET_EN_ESPERA ) {
                _snprintf( mensaje_te, sizeof( mensaje_te ), ST( S_TIENE__D_TICKETS_EN_ESPERA ), RAM_TICKET_EN_ESPERA );
            }
        }
        else {
			cant = CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF );
            if( cant >= 1  ) {
                _snprintf( mensaje_te, sizeof( mensaje_te ), ST( S_TIENE__D_TICKETS_EN_ESPERA ), cant );
            }
        }
        if( strlen( mensaje_te ) > 0 ){
            strncat(mensaje_te, "  ([ENTER] CONTINUA, [ESC] CANCELA)", sizeof( mensaje_te ) - strlen( mensaje_te ) - 1 );  
            for(;;){
                MENSAJE_TECLA( mensaje_te );
                if( LASTKEY() == 27 ){
                    continuar = 0;
                    break;
                }
                else if( LASTKEY() == 13 ){
                    break;
                }
            }
        }
        if( continuar == 1 ){
            GET_POSICION_CURSOR( &x, &y );
            att = _ATRIBUTO;
            if( CR1_CIERRE() ) {
                if( ABRIR_CAJON() ) {
                    SET_MEMORY_CHAR( __ram_no_reprocesar, 1 );
                    SET_MEMORY_LONG( __ram_nro_cliente, 0 );
                    CEREAR_PERFIL_CLIENTE();
                    SET_MEMORY_INT( __ram_fin_ticket, 1 );
                    SET_MEMORY_INT( __ram_cod_funcion, 0 );
                    SET_MEMORY_CHAR( __ram_tipo_evento_comp_original, ( char ) - 1 );
                    rta = 1;
                    salida_cierre = 1;
                }
                rta = 1;
            }
            _ATRIBUTO = att;
            SET_POSICION_CURSOR( x, y );
        }
    }
    ENABLE_STANDBY_AUTOMATICO();
    return ( rta );
}
/***************************************************************************/
void RECALCULA_ITEM_POR_PRECIO( double importe )
/***************************************************************************/
{
    double tasas = 0.0;
    
	if(event_ticket.tipo_evento == 30 && IMPRESORA_FISCAL == FISCAL ) {
		ROUND2(&importe,ENTEROS, DECIMALES);
	}
	
    SET_MEMORY_DOUBLE( __item_precio, importe );
	//SET_MEMORY_DOUBLE( __item_imp_int, 0.00 );
	if( /*cerear_ii ||*/ ( !RAM_NOTA_CR && ITEM_PRECIO <= ITEM_IMP_INT ) ) {
		//SET_MEMORY_DOUBLE( __item_imp_int, 0.0 );
		if( CR1_ITEM_GRAVADO() ) {
			if( _ITEM_COD_IMPUESTO( 2 ) != 0 ) {
				SET_MEMORY_DOUBLE( __item_precio,
								   ITEM_PRECIO
								 / ( 1 + iva[_ITEM_COD_IMPUESTO( 2 ) - 1].porc_iva1 / 100.0 ) );
			} else {
				//tasas = iva[ITEM_COD_IVA].porc_iva1 + ITEM_PORC_ILA;
				if( PRECALCULAR_IMPUESTOS ) {
					 tasas = (( ( ITEM_PRECIO - ITEM_IMP_INT ) / ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) ) +	ITEM_IMP_INT );
					// tasas = ITEM_PRECIO / ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 ); //asi estaba antes de la tm220
					SET_MEMORY_DOUBLE( __item_precio, tasas / ( 1 + _ITEM_PORC_IMPUESTO( 0 ) / 100.0 ) );
				} else {
					tasas = iva[ITEM_COD_IVA].porc_iva1 + _ITEM_PORC_IMPUESTO( 0 );
					SET_MEMORY_DOUBLE( __item_precio, ITEM_PRECIO / ( 1 + tasas / 100.0 ) );
				}
				//SET_MEMORY_DOUBLE( __item_precio,
				//  ITEM_PRECIO / ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) );
			}
		}
	} else if( CR1_ITEM_GRAVADO(  ) ){ //solo para cambio de precio en NC
		double dif_final=0.00;
		double diff = fabs(( (articulo.precio_sin_iva - articulo.imp_int ) * ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 )) + articulo.imp_int)  - fabs(importe)  ;
		double valor_absoluto = fabs(diff);
		dif_final = fabs( importe - (event_e_ticket.importe_sin_iva + event_e_ticket.iva1 ));

		if(fabs(dif_final) > 0.0001 && ( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME )
				&&( valor_absoluto > 0.00002 && valor_absoluto < 0.03 && ( event_e_ticket.origen != _ORIGEN_PROMOCION &&event_e_ticket.origen != _ORIGEN_VARIACION_MEDIO ) )) {
					int signo = 1;
					if( diff > 0.00)
						signo = -1;
					 importe+= valor_absoluto * signo;
				SET_MEMORY_DOUBLE( __item_precio,importe);
			} 
			if( (event_e_ticket.origen == _ORIGEN_PROMOCION || event_e_ticket.origen == _ORIGEN_VARIACION_MEDIO)   && RAM_NOTA_CR ) {
				double descuentos = 0.00;
				if( fabs(event_e_ticket.alicuota) > 0.01 ) {
						//hubo correccion de importe sin iva
						if( event_e_ticket.origen == _ORIGEN_VARIACION_MEDIO ) {
							descuentos = event_e_ticket.importe_sin_iva - event_e_ticket.alicuota; 
							if( fabs( descuentos ) > 0.0001 ) {
								double itemprecio;
								itemprecio = ( event_e_ticket.alicuota * ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0)) + event_e_ticket.imp_int ;
								SET_MEMORY_DOUBLE( __item_precio,itemprecio);
							}
						}
						else {
						descuentos = event_e_ticket.alicuota *( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) + event_e_ticket.imp_int;
						descuentos = fabs(event_e_ticket.total) - fabs(descuentos);
						ADD_MEMORY_DOUBLE( __item_precio, -descuentos );
						/*descuentos = event_e_ticket.importe_sin_iva - fabs(event_e_ticket.alicuota); 
						ADD_MEMORY_DOUBLE( __item_precio, -descuentos ); //ojo aca no siempre se resta*/
						}
				} else { //aqui no hubo correccion del importe sin iva pero hay que ver si el campo total es igual al importe sin iva por el iva
					//esto es por el importe sin iva de la printer
					descuentos = event_e_ticket.importe_sin_iva *( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) + event_e_ticket.imp_int;
					descuentos = fabs(event_e_ticket.total) - fabs(descuentos);
					ADD_MEMORY_DOUBLE( __item_precio, -descuentos );
				}
		}
		SET_MEMORY_DOUBLE( __item_precio,
			( ( ITEM_PRECIO - ITEM_IMP_INT ) / ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) ) + 	ITEM_IMP_INT );
	} else {
		double diff = fabs( articulo.precio_sin_iva ) - fabs(importe);
		double valor_absoluto = fabs(diff);
			if(( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME )
				&&( valor_absoluto > 0.0004 && valor_absoluto < 0.03 && (event_e_ticket.origen != _ORIGEN_PROMOCION  && event_e_ticket.origen != _ORIGEN_VARIACION_MEDIO))){
					int signo = 1;
					if( diff > 0.00)
						signo = -1;
					 importe+= valor_absoluto * signo;
				SET_MEMORY_DOUBLE( __item_precio,importe);
	}
  }
    
    SET_MEMORY_DOUBLE( __item_precio_sin_iva, ITEM_PRECIO );

    if( RAM_CAMBIO == 1 ) {
        SET_MEMORY_DOUBLE( __item_costo, ITEM_COSTO * ( -1 ) );
    }
    //CR1_CALCULA_RECARGO(  );
	if( config_tps.ModificarAlicuotaDeArticuloConsFinal == 1 
			&& Modificar_Alicuota == 1 
			&& ITEM_COD_BAJO ) {
		//Recupero el iva modificado
		SET_MEMORY_CHAR( __item_cod_iva,  ITEM_COD_BAJO );
	}
    CR1_CALCULA_IVA();
}
/***************************************************************************/
int AGREGAR_ARTICULO( long cod, double importe, char origen, int tipo )
/***************************************************************************/
{
    int ok = 0;
    if( ASIGNA_CODIGO_ESPECIAL( cod ) ) {
        RECALCULA_ITEM_POR_PRECIO( importe );
        ok = CR1_AGREGA_ITEM( NO, origen, tipo, 0 );
    }
    return ( ok );
}

/***************************************************************************/
int AGREGAR_ARTICULO_GRAVADO( long cod, double importe, int tipo, char origen )
/***************************************************************************/
{
	int       ok = 0, h = 0;

   h = RAM_P_TICKET;
	if( ASIGNA_CODIGO_ESPECIAL( cod ) ) {
		SET_MEMORY_CHAR( __item_gravado, SI );//Fuerza el descarga a GRAVADO
		//RECALCULA_ITEM_POR_PRECIO( importe );
		articulo.gravado = 'S';//Forzamos que sea GRAVADO
		AGREGA_ITEM_AL_TICKET( h, 0, NO, NO, origen );
		_SET_MEMORY_LONG( __ticket_cod_especial, h, cod );

		/************ Recalcula Importes ***************************************/
		SET_MEMORY_DOUBLE( __item_precio_sin_iva, importe
		   / ( 1 + iva[ITEM_COD_IVA].porc_iva1 / 100.0 ) );
		SET_MEMORY_DOUBLE( __item_iva1_precio, importe - ITEM_PRECIO_SIN_IVA );
	   /***********************************************************************/
		SET_MEMORY_DOUBLE( __item_precio, importe );
		_SET_MEMORY_DOUBLE(__ticket_precio, h, importe );
		if( !EXENTO_PARA_DIPLOMATICOS( ) || origen == _ORIGEN_DESC_TARJETA ) {
			_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, ITEM_PRECIO_SIN_IVA );
			_SET_MEMORY_DOUBLE( __ticket_exento, h, 0.00 );
			_SET_MEMORY_DOUBLE( __ticket_iva1_precio, h, ITEM_IVA1_PRECIO );
			_SET_MEMORY_DOUBLE( __ticket_iva2_precio, h, ITEM_IVA2_PRECIO );
			_SET_MEMORY_FLOAT( __ticket_ila_precio, h, ITEM_ILA_PRECIO );
			_SET_MEMORY_FLOAT( __ticket_iha_precio, h, ITEM_IHA_PRECIO );
			_SET_MEMORY_DOUBLE( __ticket_recargo, h,  ITEM_RECARGO );
			_SET_MEMORY_DOUBLE( __ticket_imp_int, h, ITEM_IMP_INT);
			_SET_MEMORY_DOUBLE( __ticket_iva1_rec, h, ITEM_IVA1_REC);
			_SET_MEMORY_DOUBLE( __ticket_iva2_rec, h, ITEM_IVA2_REC );
			_SET_MEMORY_FLOAT( __ticket_ila_rec, h, ITEM_ILA_REC);
			_SET_MEMORY_FLOAT( __ticket_iha_rec, h, ITEM_IHA_REC );
		} else {
			_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h, importe );
			_SET_MEMORY_DOUBLE( __ticket_exento, h, 0.00 );
			_SET_MEMORY_DOUBLE( __ticket_iva1_precio, h, 0.00 );
			_SET_MEMORY_DOUBLE( __ticket_iva2_precio, h, 0.00 );
			_SET_MEMORY_FLOAT( __ticket_ila_precio, h, 0.00 );
			_SET_MEMORY_FLOAT( __ticket_iha_precio, h, 0.00 );
			_SET_MEMORY_DOUBLE( __ticket_recargo, h, ITEM_RECARGO );
			_SET_MEMORY_DOUBLE( __ticket_imp_int, h, 0.00 );
			_SET_MEMORY_DOUBLE( __ticket_iva1_rec, h, 0.00 );
			_SET_MEMORY_DOUBLE( __ticket_iva2_rec, h, 0.00 );
			_SET_MEMORY_FLOAT( __ticket_ila_rec, h, 0.00 );
			_SET_MEMORY_FLOAT( __ticket_iha_rec, h, 0.00 );
		}
		_SET_MEMORY_CHAR( __ticket_tipo, h, _GRAVADO );
		_SET_MEMORY_DOUBLE( __ticket_costo, h,  ITEM_COSTO );
		//DISCRIMINA_IMPUESTOS_EN_ARTICULO( importe, procesar );
		ADD_MEMORY_INT( __ram_p_ticket, 1 );
		h = RAM_P_TICKET - 1;
		SET_MEMORY_DOUBLE( __ram_total, ROUND( RAM_TOTAL + importe, ENTEROS, DECIMALES ) );
		if( DEBE_IMPRIMIR_ITEM(  ) ) {
			IMPRIME_ITEM( h );
		}
	   ACTUALIZA_ACUMULADORES_RAM( h );
      GRABAR_EVENT_E_TICKET( h, NO, SI, origen, tipo, NO );
		GRABAR_TICKET( h );
		ok = 1;
	}

	return ( ok );
}

/***************************************************************************/
int AGREGAR_ARTICULO_EXENTO( long cod, double importe, int tipo, char origen )
/***************************************************************************/
{
	int       ok = 0, h = 0;

	h = RAM_P_TICKET;
	if( ASIGNA_CODIGO_ESPECIAL( cod ) ) {
	   SET_MEMORY_CHAR( __item_gravado, NO );//Fuerza el descarga a EXENTO
		//RECALCULA_ITEM_POR_PRECIO( importe );
		articulo.gravado = 'N';//Forzamos que sea Exento
		AGREGA_ITEM_AL_TICKET( h, 0, NO, NO, origen );
		_SET_MEMORY_LONG( __ticket_cod_especial, h, cod );
		SET_MEMORY_DOUBLE( __item_precio, importe);
		_SET_MEMORY_DOUBLE( __ticket_precio, h, importe );
		_SET_MEMORY_DOUBLE( __ticket_importe_sin_iva, h,	0.00 );
		_SET_MEMORY_DOUBLE( __ticket_exento, h, importe );
		_SET_MEMORY_DOUBLE( __ticket_iva1_precio, h, 0.00 );
		_SET_MEMORY_DOUBLE( __ticket_iva2_precio, h, 0.00 );
		_SET_MEMORY_FLOAT( __ticket_ila_precio, h, 0.00 );
		_SET_MEMORY_FLOAT( __ticket_iha_precio, h, 0.00 );
		_SET_MEMORY_DOUBLE( __ticket_recargo, h, ITEM_RECARGO );
		_SET_MEMORY_DOUBLE( __ticket_imp_int, h, 0.00 );
		_SET_MEMORY_DOUBLE( __ticket_iva1_rec, h, 0.00 );
		_SET_MEMORY_DOUBLE( __ticket_iva2_rec, h, 0.00 );
		_SET_MEMORY_FLOAT( __ticket_ila_rec, h, 0.00 );
		_SET_MEMORY_FLOAT( __ticket_iha_rec, h, 0.00 );
		//_SET_MEMORY_CHAR( __ticket_tipo, h, 0 /*unsigned char ) tipo*/);
		_SET_MEMORY_DOUBLE( __ticket_costo, h, ITEM_COSTO );
		//DISCRIMINA_IMPUESTOS_EN_ARTICULO( importe, procesar );
		ADD_MEMORY_INT( __ram_p_ticket, 1 );
		h = RAM_P_TICKET - 1;
		SET_MEMORY_DOUBLE(__ram_total, ROUND( RAM_TOTAL + importe, ENTEROS, DECIMALES ) );
		_SET_MEMORY_CHAR( __ticket_tipo, RAM_P_TICKET, 0 );//dmp - es EXENTO
		if( DEBE_IMPRIMIR_ITEM(  ) ) {
			IMPRIME_ITEM( h );
		}
	   ACTUALIZA_ACUMULADORES_RAM( h );
      GRABAR_EVENT_E_TICKET( h, NO, SI, origen, tipo, NO );
		GRABAR_TICKET( h );
	   ok = 1;
	}

	return ( ok );
}

#ifdef COMPILAR_NOVIOS
/*****************************************************************************/
void TECLA_NOVIOS()
/*****************************************************************************/
{
    SET_MEMORY_CHAR( __ram_pedir_novios, 1 );
    SET_MEMORY_CHAR( __ram_novios, 1 );
    LIMPIAR_RECUADRO_PETICION( ST( S__NOVIOS_ ) );
}
#endif
/*****************************************************************************/
int EXENTO_COMO_GRAVADO( int tipo )
/*****************************************************************************/
{
    int rta = 0;

    // TOLEDO NEW: Agregar condicion de exento especial
	if(!imprimiendo_factura_journal)
	{
	//antes    rta = EXENTO_COMO_GRAVADO_EN_FAC_A && RAM_FACTURA && ES_FACTURA_A( clientes.cond_iva );

      rta = EXENTO_COMO_GRAVADO_EN_FAC_A && ( RAM_FACTURA || RAM_TICKET_FACTURA == 1 )
        && ES_FACTURA_A( clientes.cond_iva ) && tipo;
    if( !rta )
       rta = ( tipo == 2 && EXENTO_COMO_GRAVADO_EN_FAC_A ) &&
            ( RAM_FACTURA || RAM_TICKET_FACTURA == 1 ) &&
            ES_FACTURA_A( clientes.cond_iva );



    /*if( !rta ) {
        rta = tipo == 2 && RAM_FACTURA && ES_FACTURA_A( clientes.cond_iva );
    }*/
    }
    return ( rta );
}

/*****************************************************************************/
void CR1_ABRIR_CAJON()
/*****************************************************************************/
{
    char cadena[30];

	if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, SUPERVISOR_PARA_ABRIR_CAJON,
                          ST( S__ABRIR_CAJON_ ), ST( S_AUTORIZO_TECLA_ABRIR_CAJON ),
                          RAM_COD_FUNCION, SI ) ) {
        if( IMPRESORA_FISCAL != FISCAL_IBM_4610 ) {
            if( IMPRIMIR_EN_JOURNAL ) {
                PRN_INHABILITA_AVANCE_RECEIPT();
                LF( 1 );
                MOSTRAR_CADENA( 1, PRN, ST( S_FECHA ) );
                MOSTRAR_CADENA( 12, PRN, ST( S_HORA ) );
                MOSTRAR_CADENA( 20, PRN, ST( S_SUC_ ) );
                MOSTRAR_CADENA( 27, PRN, ST( S_CAJA ) );
                MOSTRAR_CADENA( 34, PRN, ST( S_CAJERO ) );
                LF( 1 );
                MOSTRAR_FECHA( 1, PRN, GET_FECHA_DOS() );
                MOSTRAR_HORA( 12, PRN, GET_HORA_DOS() );
                MOSTRAR_ENTERO( 20, PRN, "000", config.sucursal );
                MOSTRAR_ENTERO( 27, PRN, "000", NRO_CAJA );
				memset(cadena, 0, sizeof(cadena));
                if( USAR_LEGAJO == RUT ) {
                    MOSTRAR_CADENA( 32, PRN, RAM_DOC_CAJERO );
                }
                else if( IMPRIMIR_LEGAJO_CAJERO ) {
					_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
                    MOSTRAR_ENTERO( 34, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
                }
                else {                
					_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
					MOSTRAR_ENTERO( 35, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
                }
                LF( 1 );
                MOSTRAR_CADENA( 1, PRN, ST( S_______APERTURA_DEL_CAJON______ ) );
                LF( 1 );
            }
            PRN_HABILITA_AVANCES();
        }
        ABRIR_CAJON();
    }
}
/*****************************************************************************/
void REPINTAR_PANEL()
/*****************************************************************************/
{
    if( ancho_panel && PanelActivo ) {
        RefreshPanel( pPanel );
    }
}
/*****************************************************************************/
int CR1_ITEM_GRAVADO()
/*****************************************************************************/
{
    return ( ( ITEM_GRAVADO == 1 ) ? 1 : 0 );
}
/*****************************************************************************/
char * BUSCA_MENSAJE_STRING( int id )
/*****************************************************************************/
{
	//ESTA FUNCION HACE LO MISMO QUE EL ST(_____s__..)
    /*int area;
    area = SELECTED();
    SELECT( AREA_STRING_TABLE );
    GET_EQUAL( ( char* )&id );
    if( !FOUND() ) {
        sprintf( string_table->mensaje, "MENSAJE NO ENCONTRADO (%d)", id );
    }
    SELECT( area );*/

    int area_ant, tipo_ant;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_STRING_TABLE );
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    if( !FOUND2() ) {
        _snprintf( string_table->mensaje, sizeof(string_table->mensaje)-1, "MENSAJE NO ENCONTRADO (%d)", id );
    }
    SELECT_TABLE(area_ant, tipo_ant);
    return string_table->mensaje;
}
/*****************************************************************************/
void ACTUALIZAR_X( void )
/*****************************************************************************/
{
	_ACTUALIZAR_TOTAL_X( 0 );
}
/*****************************************************************************/
int OBTENER_COD_ESPECIAL( void )
/*****************************************************************************/
{
    return pedir_cod_especial;
}
/*****************************************************************************/
void MENSAJE_SIN_CMR()
/*****************************************************************************/
{
    MENSAJE( "MENSAJE SIN CMR" );
}
/*****************************************************************************/
void MODALIDAD_BIEN_DE_USO( void )
/*****************************************************************************/
{
    if( RAM_BIEN_DE_USO ) {
        SET_MEMORY_CHAR( __ram_bien_de_uso, 0 );
        MENSAJE_STRING( S_ANULA_MODALIDAD_BIEN_DE_USO );
    }
    else {
        SET_MEMORY_CHAR( __ram_bien_de_uso, 1 );
        MENSAJE_STRING( S_MODALIDAD_BIEN_DE_USO );
    }
    MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
}
/*****************************************************************************/
void MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA( void )
/*****************************************************************************/
{
    if( RAM_BIEN_DE_USO && !bien_uso_on ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_BIEN_USO_AGREGAR, "B", " ", NULL, " " );
        bien_uso_on = 1;
    }
    else if( !RAM_BIEN_DE_USO && bien_uso_on ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_BIEN_USO_BORRAR, " ", " ", NULL, " " );
        bien_uso_on = 0;
    }
}
/*****************************************************************************/
int _ACTUALIZAR_TOTAL_X( int espera_respuesta )
/*****************************************************************************/
{
    int error = 1, i = 0, cant = 0;
    long l_buffer;
    char *buffer;
	struct _total_x total_x;
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();
	//char patron_con_null[CANT_COL_X_DBROUTER];

    //tiempos de productividad
    SETEAR_TIEMPO( _ENTRADA_OCIO );
    ACUMULAR_TIEMPOS();

    l_buffer = SIZEOF_STRUCT( _x );
    buffer = MALLOC( l_buffer );
    if( buffer != NULL ) {
        COPY_STRUCT_TO_BUFFER( buffer, _x );
        SUMAR_X_USUARIOS( buffer );
        CEREAR_STRUCT( _x_cajero );
        COPY_BUFFER_TO_STRUCT( buffer, _x_cajero );
        FREEMEM( buffer );
    }
	
    // Setears Nulos
    //patron_con_null[0] = 1;
    //patron_con_null[1] = 0;
    //memset( patron_con_null + 2, 1, CANT_COL_X_DBROUTER - 2 );
    memset( &total_x, 0, sizeof( struct _total_x ) );

	if( OPEN_TABLE( T_TOTAL_X, TT_ORIG, ( char* )&total_x, sizeof( struct _total_x ) )== 0 ) {
		SET_WHERE("");
		RUN_QUERY(NO);
		cant = RECCOUNT2( NULL, NULL);
		//En caso de cambio de nro de caja
		if( cant && total_x.cod_caja != NRO_CAJA ) {
			cant = 0;
			DELETE2();
		}
		total_x.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
		total_x.cod_caja = NRO_CAJA;
		total_x.dia_operativo = NRO_DIA_OPERATIVO;
		total_x.fecha_ticket = GET_FECHA_DOS();
		total_x.nro_ticket = NRO_TICKET;
		total_x.adelantos.imp = XC_ADELANTOS_IMPORTE;
		total_x.adelantos.can = XC_ADELANTOS_CANTIDAD;
		total_x.intereses.imp = XC_INTERESES_IMPORTE;
		total_x.intereses.can = XC_INTERESES_CANTIDAD;
		total_x.fondo_fijo.imp = XC_FONDO_FIJO_IMPORTE;
		total_x.fondo_fijo.can = XC_FONDO_FIJO_CANTIDAD;
		total_x.anulaciones.imp = XC_ANULACIONES_IMPORTE;
		total_x.anulaciones.can = XC_ANULACIONES_CANTIDAD;
		total_x.dev_envases.imp = XC_DEV_ENVASES_IMPORTE;
		total_x.dev_envases.can = XC_DEV_ENVASES_CANTIDAD;
		total_x.ventas_gravadas = XC_VENTAS_GRAVADAS;
		total_x.ventas_exentas = XC_VENTAS_EXENTAS;
		total_x.imp_int = XC_IMP_INT;
		total_x.tot_dinero = XC_TOT_DINERO;
		total_x.iva1 = XC_IVA1;
		total_x.iva2 = XC_IVA2;
		total_x.retiro_fondo.imp = XC_RETIRO_FONDO_IMPORTE;
		total_x.retiro_fondo.can = XC_RETIRO_FONDO_CANTIDAD;
		total_x.pagos_por_caja.imp = XC_PAGOS_POR_CAJA_IMPORTE;
		total_x.pagos_por_caja.can = XC_PAGOS_POR_CAJA_CANTIDAD;
		total_x.facturas_a.imp = XC_FACTURAS_A_IMPORTE;
		total_x.facturas_a.can = XC_FACTURAS_A_CANTIDAD;
		total_x.facturas_b.imp = XC_FACTURAS_B_IMPORTE;
		total_x.facturas_b.can = XC_FACTURAS_B_CANTIDAD;
		total_x.vueltos.imp = XC_VUELTOS_IMPORTE;
		total_x.vueltos.can = XC_VUELTOS_CANTIDAD;
		total_x.bonos_entregados = XC_BONOS_ENTREGADOS;
		total_x.diferencia = XC_DIFERENCIA;
		total_x.nc_a.imp = XC_NC_A_IMPORTE;
		total_x.nc_a.can = XC_NC_A_CANTIDAD;
		total_x.nc_b.imp = XC_NC_B_IMPORTE;
		total_x.nc_b.can = XC_NC_B_CANTIDAD;
		total_x.retencion_dgr = XC_RETENCION_DGR;
		total_x.percepdion_dgi = XC_PERCEPCION_DGI;
		total_x.correccion.imp = XC_CORRECION_IMPORTE;
		total_x.correccion.can = XC_CORRECCION_CANTIDAD;
		total_x.primer_ticket = XC_NRO_PRIMER_TICKET;
		total_x.cantidad_tickets = XC_CANTIDAD_TICKETS;
		total_x.redondeo = XC_DIFERENCIA_REDONDEO;
		total_x.devoluciones.imp = XC_IMP_DEVOLUCIONES;
		total_x.devoluciones.can = XC_CANT_DEVOLUCIONES;
		total_x.cant_articulos = XC_CANT_ARTICULOS;
		total_x.cant_art_escaneados = XC_CANT_ART_ESCANEADOS;
		total_x.tmpo_reg_articulo = XC_TMPO_REG_ARTICULO;
		total_x.tmpo_pago = XC_TMPO_PAGO;
		total_x.tmpo_standby = XC_TMPO_STANDBY;
		total_x.tmpo_ocio = XC_TMPO_OCIO;
		total_x.tmpo_no_venta = XC_TMPO_NO_VENTA;
		total_x.cajero = ( NRO_CAJERO ) ? NRO_CAJERO : NRO_CAJERO_ANT;
		total_x.donaciones.imp = XC_DONACIONES_IMPORTE;
		total_x.donaciones.can = XC_DONACIONES_CANTIDAD;
		total_x.cancelados.imp = XC_IMP_CANCELADOS;
		total_x.cancelados.can = XC_CANT_CANCELADOS;
		for( i = 0;i < 10;i++ ) {
			total_x.imp_ventas[i] = _XC_VTAS_IMPORTE( i );
			total_x.can_ventas[i] = _XC_VTAS_CANTIDAD( i );
			total_x.imp_cobros[i] = _XC_COBROS_IMPORTE( i );
			total_x.can_cobros[i] = _XC_COBROS_CANTIDAD( i );
			total_x.imp_fondo_fijo_multiple[i] = _XC_FONDO_FIJO_MULTIPLE_IMPORTE( i );
			total_x.imp_fondo_fijo_multiple_me[i] = _XC_FONDO_FIJO_MULTIPLE_IMP_ME( i );
			total_x.can_fondo_fijo_multiple[i] = _XC_FONDO_FIJO_MULTIPLE_CANTIDAD( i );
			total_x.importes_iva[i] = _XC_IMPORTE_IVA( i );
			total_x.sobretasas_iva[i] = _XC_IMPORTE_SOBRETASA_IVA( i );
			total_x.imp_retiros[i] = _XC_RETIROS_IMPORTE( i );
			total_x.imp_retiros_me[i] = _XC_RETIROS_IMPORTE_ME( i );
			total_x.can_retiros[i] = _XC_RETIROS_CANTIDAD( i );
			total_x.imp_cambios[i] = _XC_CAMBIOS_IMPORTE( i );
			//total_x.imp_cambios_me[i] = _XC_CAMBIOS_IMPORTE_ME( i );
			total_x.can_cambios[i] = _XC_CAMBIOS_CANTIDAD( i );
			total_x.devoluciones_medio[i] = _XC_DEVOLUCIONES_MEDIO( i );
		}
		if( cant ) {
			error = UPDATE2();
		} else {
			error = INSERT2(NO);	
		}
		if( error ){
			char *mensaje = "Error Grabando Total X";
			GRABAR_LOG_DBROUTER( mensaje, LOG_ERRORES,2 );
		}
		CLOSE_TABLE( T_TOTAL_X, TT_ORIG );
	}
	SELECT_TABLE( tabla_ant, tipo_ant );

	return !error;

}
/****************************************************************************/
void EVALUAR_COD_TEMPLATE()
/****************************************************************************/
{
    struct templates mascara;
    struct tipos_template precio,peso;
    char cadena_aux[25],cadena_entrada[25],prefijo[15],d_verificador,cadena_final[25],
         cadena_precio[10],cadena_peso[10],cadena_unidades[6];
    int /*area_ant,*/ cnti, l_cod, l_id, l_cod_busqueda, l_cod_pp, barra = 0, bandera = 1;
    int tipo_ant, tabla_ant, l_unidades = 0, cantidad = 0;

    memset( &precio, 0, sizeof(struct tipos_template) );
    memset( &peso, 0, sizeof(struct tipos_template) );
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SET_MEMORY_CHAR( __ram_usa_template, 0 );
	memset( mascara.mask_template, 0, 16 );
	memset( &mascara, 0, sizeof( struct templates ) );
	if( OPEN_TABLE( T_TEMPLATES, TT_ORIG, ( char* )&mascara, sizeof( struct templates ) ) == 0 ) {
        SET_WHERE("");
		RUN_QUERY(NO);
		strncpy( cadena_entrada, RAM_COD_BARRA, 25 );
        strncpy( cadena_aux, RAM_COD_BARRA, 25 );
		GRABAR_LOG_SISTEMA( "EVALUAR COD TEMPLATE" ,LOG_VENTAS,2);
		GRABAR_LOG_SISTEMA( cadena_aux ,LOG_VENTAS,2);
        TRIM( mascara.mask_template );
		GRABAR_LOG_SISTEMA( mascara.mask_template,LOG_VENTAS,4 );
        l_cod = strlen( mascara.mask_template );
		while( !dbEOF() && bandera ) {
            l_id = 0;
            l_cod_busqueda = 0;
			l_cod_busqueda2 = 0;
            l_cod_pp = 0;
            precio.enteros = 0;
            precio.decimales = 0;
            peso.enteros = 0;
            peso.decimales = 0;
			l_unidades = 0;
            //interno = 0;
            barra = 0;
            EVALUAR_MASK(l_cod,&l_id,&l_cod_busqueda,&barra,&l_cod_pp,&l_unidades,prefijo,mascara.mask_template,&precio,&peso);
            l_cod = l_cod - l_id; //saca el prefijo del largo de la mascara
			l_cod_busqueda2 = l_cod_busqueda;
            /*for( i = 0;i < l_cod;i++ ) {
                switch( mascara.mask_template[i] ) {
                    case 'B':
                    case 'b':
                        barra = 1;
                        l_cod_busqueda++;
                        break;
                    case 'C':
                    case 'c':
                        //interno = 1;
                        l_cod_busqueda++;
                        break;
                    case 'P':
                    case 'p':
                        peso.enteros++;
                        l_cod_pp++;
                        break;
                    case 'Q':
                    case 'q':
                        peso.decimales++;
                        l_cod_pp++;
                        break;
                    case 'U':
                    case 'u':
                        l_unidades++;
                        break;
                    case '$':
                        precio.enteros++;
                        l_cod_pp++;
                        break;
                    case '#':
                        precio.decimales++;
                        l_cod_pp++;
                        break;
                    case 'X':
                    case 'x':
                        SET_MEMORY_CHAR( __ram_pos_mask_eliminar, i );
                        break;
                    default:
                        if( mascara.mask_template[i] >= '0' && mascara.mask_template[i] <= '9' ) {
                            prefijo[l_id] = mascara.mask_template[i];
                            l_id++;
                        }
                }
            }*/
            if( memcmp( prefijo, cadena_entrada, l_id ) == 0 && consulta_x_no_template == 0 ) {
                bandera = 0;
                //l_id = 0;
            }
            SELECT_TABLE( T_TEMPLATES, TT_ORIG );
            SKIP2( 1 );
        }
		if( !bandera ) {
            d_verificador = cadena_entrada[l_cod - 1];
            CALCULA_CRC_EAN13( cadena_entrada );
            if( d_verificador != cadena_entrada[l_cod - 1] ) {
				CEREAR_MEMORY( __ram_cod_barra );
                MENSAJE_STRING( S_DIGITO_VERIFICADOR_INVALIDO );
            }
			else {
				int  posx = 0;
				if( RAM_POS_MASK_ELIMINAR > 0 ) {
					posx = 1;
				}
                if( barra && !GENERAR_COD_BARRA_INVERTIDO ) {
                    //memcpy( cadena_final, &cadena_entrada[l_id], l_cod_busqueda);
                    memset( &cadena_aux, '0', l_cod );
                    memcpy( &cadena_aux, cadena_entrada, l_id + l_cod_busqueda );
                    //CALCULA_CRC_EAN13( cadena_aux );
                    SET_MEMORY( __ram_cod_barra, cadena_aux );
                    //            usar_cadena_scanner = 1;
                    mantener_cod_barra = 1;
                    cod_especial = 0;
                }
                else if(barra && GENERAR_COD_BARRA_INVERTIDO){
                    memset( &cadena_aux, 0, sizeof(cadena_aux) );
                    memset( &cadena_aux, '0', l_cod );
                    for(cnti=0;cnti<l_cod_busqueda;cnti++){
                        cadena_aux[l_cod - cnti - 1] = cadena_entrada[l_cod_busqueda - cnti - 1];
                    }
                    //CALCULA_CRC_EAN13( cadena_aux );
                    SET_MEMORY( __ram_cod_barra, cadena_aux );
                    mantener_cod_barra = 1;
                    cod_especial = 0;
                }else{
                    memset( cadena_final, 0, sizeof( cadena_final ) );
                    memcpy( cadena_final, &cadena_entrada[l_id], l_cod_busqueda );
                    cod_especial = atol( cadena_final );
                    usar_cadena_scanner = 0;
                }
                if(mascara.evaluar_tipo_art){
                    int encontro=0;
                    if(barra)
                        encontro = BUSCA_CODIGO_BARRA( RAM_COD_BARRA );
                    else
                        encontro = BUSCA_CODIGO_ESPECIAL(cod_especial);

                    if(encontro){
                        //segun el tipo (pesable, precio, etc..) de articulo analiza la mascara
                        if(mascara.cod_tipo1 && articulo.tipo == mascara.cod_tipo1)
                            EVALUAR_MASK(l_cod,&l_id,&l_cod_busqueda,&barra,&l_cod_pp,&l_unidades,prefijo,mascara.mask_template1,&precio,&peso);
                        else if (mascara.cod_tipo1 && articulo.tipo == mascara.cod_tipo2)
                            EVALUAR_MASK(l_cod,&l_id,&l_cod_busqueda,&barra,&l_cod_pp,&l_unidades,prefijo,mascara.mask_template2,&precio,&peso);
                        else if (mascara.cod_tipo1 && articulo.tipo == mascara.cod_tipo3)
                            EVALUAR_MASK(l_cod,&l_id,&l_cod_busqueda,&barra,&l_cod_pp,&l_unidades,prefijo,mascara.mask_template3,&precio,&peso);
                    }
                }
                memset( cadena_precio, 0, sizeof( cadena_precio ) );
                memset( cadena_peso, 0, sizeof( cadena_peso ) );
                memset( cadena_unidades, 0, sizeof( cadena_unidades ) );
                if( precio.enteros || precio.decimales ) {
                    //memcpy( cadena_precio, &cadena_entrada[l_id + l_cod_busqueda], l_cod_pp );
                    memcpy( cadena_precio, &cadena_entrada[precio.pos], l_cod_pp );
                    precio.monto = ( atof( cadena_precio ) / pow( 10, precio.decimales ) );
                    SET_MEMORY_DOUBLE( __ram_tomado_por_template, precio.monto );
                    SET_MEMORY_CHAR( __ram_usa_template, 1 );
                }
                if( peso.enteros || peso.decimales ) {
					//memcpy( cadena_peso, &cadena_entrada[l_id + l_cod_busqueda + posx], l_cod_pp );
                    memcpy( cadena_peso, &cadena_entrada[peso.pos], l_cod_pp );
                    peso.monto = ( atof( cadena_peso ) / pow( 10, peso.decimales ) );
                    SET_MEMORY_DOUBLE( __ram_tomado_por_template, peso.monto );
                    SET_MEMORY_CHAR( __ram_usa_template, 2 );
                }
                if( l_unidades ) {
                    memcpy( cadena_unidades, &cadena_entrada[l_id + l_cod_busqueda + posx], l_unidades );
                    cantidad = atoi( cadena_unidades );
                    //sprintf(aux,"cant %d ", cantidad);
                    //MENSAJE(aux);
                    //SET_MEMORY_DOUBLE( __ram_tomado_por_template, cantidad );
                    //SET_MEMORY_CHAR( __ram_usa_template, 3 );
                    SET_MEMORY_DOUBLE( __ram_cantidad, cantidad );
                }
                articulo_pesable = 0;
                mantener_cod_barra = 0;
            }
        }
    }
    CLOSE_TABLE( T_TEMPLATES, TT_ORIG );
    SELECT_TABLE( tabla_ant, tipo_ant );
}
/********************************************************************************/
int EVALUAR_MASK(int l_cod, int *l_id, int *l_cod_busqueda, int *barra, int *l_cod_pp, int *l_unidades, char *prefijo, char *mask_template, struct tipos_template *precio, struct tipos_template *peso)
/********************************************************************************/
{
    int i = 0;
    for( i = 0;i < l_cod;i++ ) {
        switch( mask_template[i] ) {
            case 'B':
            case 'b':
                *barra = 1;
                (*l_cod_busqueda)++;
                break;
            case 'C':
            case 'c':
                //interno = 1;
                (*l_cod_busqueda)++;
                break;
            case 'P':
            case 'p':
                if(peso->pos == 0)
                    peso->pos = i;
                peso->enteros++;
                (*l_cod_pp)++;
                break;
            case 'Q':
            case 'q':
                peso->decimales++;
                (*l_cod_pp)++;
                break;
            case 'U':
            case 'u':
                (*l_unidades)++;
                break;
            case '$':
                if(precio->pos == 0)
                    precio->pos = i;
                precio->enteros++;
                (*l_cod_pp)++;
                break;
            case '#':
                precio->decimales++;
                (*l_cod_pp)++;
                break;
            case 'X':
            case 'x':
                //SET_MEMORY_CHAR( __ram_pos_mask_eliminar, i );
                break;
            default:
                if( mask_template[i] >= '0' && mask_template[i] <= '9' ) {
                    prefijo[*l_id] = mask_template[i];
                    (*l_id)++;
                }
        }
    }
    return 0;
}

/********************************************************************************/
int INGRESA_CODIGO_COMPROBANTE()
/********************************************************************************/
{
    int rta = 0;
    long nro;
    nro = PEDIR_ENTERO_EN_RECUADRO( ST( S__COMPROBANTE_ ), NO, 9 );
    if( nro > 0 ) {
        SET_MEMORY_LONG( __item_comprobante, nro );
        rta = 1;
    }
    return ( rta );
}
/*****************************************************************************/
int CR1_EXISTE_DESCUENTO_CANTIDAD( long cod_especial, int presentacion )
/*****************************************************************************/
{
    int h,existe;
    float c = 0, signo;
    for( h = 0;h < RAM_P_TICKET;h++ ) {
        if( _TICKET_COD_ESPECIAL( h ) == cod_especial && _TICKET_PRESENTACION( h ) == presentacion ) {
            if( _TICKET_DESCUENTO_CANTIDAD( h ) ) {
                signo = ( float )( ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1 );
                c += ( float )( _TICKET_DESCUENTO_CANTIDAD( h ) * signo );
                if( RAM_CANTIDAD >= c ) {
                    _SET_MEMORY_DOUBLE( __ticket_descuento_cantidad, h, 0 );
                    existe = 1;
                }
            }
        }
    }
    return ( existe );
}

/*****************************************************************************
int BUSCA_ITEM_PROMO()
 *****************************************************************************
{
    int cod_promo = 0;
    int j;

    for( j = 0;j < 4;j++ ) {
        if( _ITEM_COD_PROMOCION( j ) ) {
            cod_promo = _ITEM_COD_PROMOCION( j );
            break;
        }
    }
    if( cod_promo ) {
        cod_promo = PROMO_EN_MEMORIA( cod_promo, NO );
    }
    if( cod_promo ) {
        //Verificamos que exista el codigo de descarga....
        if( !BUSCA_CODIGO_ESPECIAL( articulo.cod_articulo_promocion ) ) {
            MENSAJE_STRING( S_ARTICULO_DE_DESCARGA_INEXISTE );
            cod_promo = 0;
        }
        BUSCA_CODIGO_ESPECIAL( ITEM_COD_ESPECIAL );
    }

    return cod_promo;
}
 *****************************************************************************/

/*****************************************************************************
int BUSCA_ID_PROMO( int cod_promo )
 *****************************************************************************
{
    int id_promo = 0;

    if( cod_promo ) {
        id_promo = PROMO_EN_MEMORIA( cod_promo, SI );
    }

    return id_promo;
}
 *****************************************************************************/

/*****************************************************************************/
int EVALUAR_CODIGOS_BALANZA( char *id_balanza, char *buffer, int largo_codigo )
/*****************************************************************************/
{
    int l_id=0, h=0, migracion_spar = 0, rta = 0;
    char cadena[25],d_verificador,aux[60];
    int dato=0;
	long l_id_balanza=0;
	long sl=0;
	char *end;
    /*-------------------- Artculos de preempaque ----------------------*/
	TRIM( id_balanza );
	memset( aux, 0, sizeof( aux ) );
    memcpy( aux, id_balanza, strlen( id_balanza ) + 1 );
    l_id = strlen( id_balanza );
	memset( cadena, 0, sizeof( cadena ) );
	strncpy( cadena, buffer,( sizeof( cadena )-1 ) );  
    //strcpy( cadena, RAM_COD_BARRA );
	if(strlen( id_balanza) > 7){//todo esto es porque si la conversion atol supera el long da errno 34
		sl = strtol(id_balanza, &end, 10);
		 if (end == id_balanza) {
			l_id_balanza=0;
			errno=0;
		  } else if ('\0' != *end) {
			l_id_balanza=0;
			errno=0;
		  } else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno) {
			l_id_balanza=0;
			errno=0;
		  } else if (sl > INT_MAX) {
			l_id_balanza=0;
			errno=0;
		  } else if (sl < INT_MIN) {
			 l_id_balanza=0;
			 errno=0;
		  } else {
			l_id_balanza= sl;
			errno=0;
		  }
	}else 
		l_id_balanza = atol( id_balanza );
	
    if( l_id_balanza > 0  && l_id > 1 ) {
        if( MODALIDAD_PREEMPAQUE == PREEMPAQUE_CODIGO_INTERNO ) {
            /*---- Especial para migracin de SPAR ----*/
            #ifdef COMPILAR_SCANNER_SPAR
            if( TIPO_DE_SCANNER == SCANNER_SPAR && strcmp( aux, "0" ) == 0 ) {
                migracion_spar = 1;
                if( cadena[0] == '0' && cadena[1] >= '1' && cadena[1] <= '4' ) {
                    migracion_spar = 2;
                }
            }
            #endif
            /*-----------------------------------------*/
            if( ( memcmp( cadena, aux, l_id ) == 0 && !migracion_spar && !RAM_PEDIR_CLIENTE_AFINIDAD
               && l_id > 0 ) || migracion_spar == 2 ) {
                /*----------- codigos de balanza ----------*/
                // Codigo de barras de 13 o 14 digitos
                dato = largo_codigo - 6;
                memcpy( cadena_scanner, &cadena[dato], 5 );
                for( h = l_id;h < dato;h++ ) {
                    cadena[h - l_id] = cadena[h];
                }
                memset( &cadena[dato - l_id], ' ', 6 + l_id );
                cadena[largo_codigo] = 0;
                cadena_scanner[5] = 0;
                usar_cadena_scanner = 1;
                TRIM( cadena );
                strcat( cadena, SUFIJO_CODIGOS_DE_PREEMPAQUE );
                cod_especial = atol( cadena );
                rta = 1;
                articulo_pesable = 1;
            }
        }
        else {
            if( l_id && !RAM_PEDIR_CLIENTE_AFINIDAD && memcmp( cadena, aux, l_id ) == 0 ) {
                d_verificador = cadena[largo_codigo - 1];
                CALCULA_CRC_EAN13( cadena );
				if( d_verificador == cadena[largo_codigo - 1] ) {
					if( GENERAR_COD_BARRA_INVERTIDO ) {
						char cad_aux[14], cadena_aux[14];
						int  pos = 0, i=0, l_cod = 13, l_cod_busqueda = 4;
						memset( cad_aux, 0, sizeof( cad_aux ) );
						memset( cadena_aux, 0, sizeof( cadena_aux ) );
						for( i = 0; i < l_cod - 1; i++ ) {
							if( i != RAM_POS_MASK_ELIMINAR ) {
								cad_aux[pos] = cadena[i];
								pos++;
							}
						}
						memcpy( cadena_scanner, &cadena[largo_codigo - 6], 5 );
						usar_cadena_scanner = 1;
						memset( cadena_aux, '0', sizeof( cadena_aux )-1 );
						cadena_aux[l_cod] = 0;
						memcpy( &cadena_aux[l_cod - l_id - l_cod_busqueda - 0], cad_aux,
							l_cod - l_id - l_cod_busqueda - 1 );
						SET_MEMORY( __ram_cod_barra, cadena_aux );
						rta = 1;
						cod_especial = 0;
						articulo_pesable = 1;
					} else {
						
						int cant_copiado = largo_codigo - (l_id + l_cod_busqueda2);
							if( cant_copiado >4 && l_id >1 && l_cod_busqueda2>1) {
								memcpy( cadena_scanner, &cadena[largo_codigo - cant_copiado ], cant_copiado  );
								usar_cadena_scanner = 1;				
								memset( &cadena[largo_codigo - cant_copiado /*5*/], '0', /*5*/cant_copiado );
						
							} else {
								memcpy( cadena_scanner, &cadena[largo_codigo - 7], 7 );
								usar_cadena_scanner = 1;				
								memset( &cadena[largo_codigo - 7/*5*/], '0', /*5*/7 );
							}
						CALCULA_CRC_EAN13( cadena );
						SET_MEMORY( __ram_cod_barra, cadena );
						rta = 1;
						cod_especial = 0;
						articulo_pesable = 1;
					
					}
                }
            }
        }
    }
    return ( rta );
}
/*****************************************************************************/
void TOMAR_CANTIDAD_SCANNER()
/*****************************************************************************/
{
    double cantidad = 0.0;
    cantidad = atof( cadena_scanner );
    cantidad = ROUND( cantidad, 10, 0 );
    SET_MEMORY_DOUBLE( __ram_cantidad, cantidad );
}
/*****************************************************************************/
int CR1_BUSCAR_ITEM_ORIGINAL_POR_DATO_ADICIONAL( int datos_adicionales )
/*****************************************************************************/
{
    int pos = -1, i, ok;
    struct _datos datos1,datos2;
    ok = _PAGOS_OBTENER_DATOS( RAM_P_TICKET, 0, &datos1, D_ITEMS );
    for( i = RAM_P_TICKET - 1 ;i > 0 && ok && pos < 0 ;i-- ) {
        if( _TICKET_COD_SOLICITUD( i ) == datos_adicionales ) {
            ok = _PAGOS_OBTENER_DATOS( i, 0, &datos2, D_ITEMS );
            if( memcmp( datos1.valor, datos2.valor, sizeof( datos1.valor ) ) == 0 ) {
                if( _TICKET_TIPO( i ) & _RESTA ) {
                    i = 0;
                }
                else {
                    pos = i;
                }
            }
        }
    }
    return( pos );
}
//Notar que en la funcion siguiente hay riesgo de error en el casos como por ej:
//
//char *titulo;
//titulo = ST( S_TITULO );
//....4 usos mas de ST()....
//sprintf( buffer, ST( S_EL_TITULO__S ), titulo );
//
//Como ST() tiene un buffer ciclico, titulo se borra y se sobreescribe con el resultado de ST().
//Como resultado: titulo y el resultado de ST() tienen la misma dir.
//Luego el resultado de sprintf() no es el deseado
/*****************************************************************************/
char * ST( int id )
/*****************************************************************************/
{
    int area_ant, tipo_ant;
    static char *b_cadenas = NULL;
    static int p = 0, t_cadena = 0, t_buffer = 0;
    char *p_cadena = NULL;
    int cant_cadenas_buffer = 25;
    if( b_cadenas == NULL ) {
        t_cadena = sizeof( string_table->mensaje );
        t_buffer = t_cadena * cant_cadenas_buffer;
        b_cadenas = ( char* )MALLOC( t_buffer );
        memset( b_cadenas, 0x00, t_buffer );
        p = 0;
    }
    if( p >= t_buffer ) {
        //Si no hay espacio para alojarla en el buffer
        p = 0;                     //Comienza en la primera
    }
    p_cadena = &b_cadenas[p];
    memset( p_cadena, 0x00, t_cadena );


    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );

    if( !FOUND2() ) {
        _snprintf( string_table->mensaje, t_cadena - 1, "MENSAJE NO ENCONTRADO (%d)", id );
        GRABAR_LOG_SISTEMA( string_table->mensaje ,LOG_ERRORES,1);
    }
    else {
        strncpy( p_cadena, string_table->mensaje, t_cadena - 1 );
        //p_cadena[t_cadena - 1/*me parece que esto es el error*/] = 0x00;
    }
    p = p + t_cadena;
    SELECT_TABLE(area_ant, tipo_ant);
    return p_cadena;
}

/**
/* Guarda el mensaje identificado por 'stringtId' en el puntero 'var' (asignandole 
/* la memoria necesaria).
/* Se utiliza para evitar el uso constante del buffer ciclico de ST() en casos
/* muy repetitivos.
/* La idea del uso es pasarle un puntero a char nulo, el cual, al terminar
/* la funcion apunta al string requerido en una direccion de memoria normal.
/* Si se le pasa un puntero no nulo, devuelve ese mismo puntero sin hacer nada.
/* Entonces la variable se setea en el primer llamado y luego es reutilizada
/* durante toda la ejecucion.
/* Ej:
/* ....
/* static char* dir = NULL;
/* ST_MEM( S_DIRECCION_SERVIDOR, &dir );
/*
/* Advertencia: Utilizar solo con variables globales o static para que unicamente
/* se reserve memoria en el primer llamado (cuando la variable es null).
/* Es responsabilidad del llamador liberar la memoria en caso q sea necesario.
/**************************************************************************/
char *ST_MEM( int stringtId, char **var ) 
/**************************************************************************/
{
    char *tmp;
    int largo;

    if( *var ) {
        return *var;
    } else {
        tmp = ST ( stringtId );
        //tmp es un string correcto (terminado en null)
        largo = strlen( tmp ) + 1;
        *var = malloc( largo );
        strncpy( *var, tmp, largo );
        return *var;
    }
}

/*****************************************************************************/
int MOSTRAR_SIN_IVA_ELSALVADOR( void )
/*****************************************************************************/
{
    return ( !FACTURA_CON_PRECIO_FINAL && RAM_FACTURA == 1 && FACT_INDEP_DEL_TIPO_DE_CLIENTE );
}
/*****************************************************************************/
//void CR1_FACTURA_TIPO( int nc, int tipo )
/*****************************************************************************/
/*{
    int ok = 1;

    if( !RAM_FACTURA && FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
        if( nc ) {
            ok = SOLICITAR_CLAVES( 30 - ancho_panel / 2, 20, NO, SI, SI, " NOTA DE CREDITO ",
                                   "Autoriz NC", RAM_COD_FUNCION, SI );
            if( ok ) {
                ok = PEDIR_BOLETA_ANTERIOR( );
            }
        }

        SET_MEMORY_CHAR( __ram_cond_iva_cliente, 0 );

        if( ok && PEDIR_NRO_CLIENTE( 1 ) ) {
            //Se debe validar la condiciï¿½ fiscal o impositiva del Cliente seleccionado
            //y el comprobante a realizar
            if( tipo == FACTURA_TIPO_A ) {
                //El Salvador: Credito Fiscal
                if( clientes.cond_iva != RESP_INSC ) {
                    MENSAJE( "VERIFIQUE CONDICION FISCAL DEL CLIENTE" );
                    ok = 0;
                }
            }

            if( ok ) {
                //Seteo la condicion de iva directamente desde la tabla de clientes.
                SET_MEMORY_CHAR( __ram_cond_iva_cliente, clientes.cond_iva );
                //Seteo el ram_factura con el tipo de factura.
                SET_MEMORY_CHAR( __ram_factura, tipo );
                //Seteos para las notas de crï¿½ito.
                SET_MEMORY_CHAR( __ram_nota_cr, ( nc ) ? 1 : 0 );
                if( nc && PEDIR_PRECIO_EN_NC ) {
                    SET_MEMORY_CHAR( __ram_pedir_precio, 1 );
                    SET_MEMORY_CHAR( __ram_negativo, 1 );
                }
            }
        }
    }
    else {
        SET_MEMORY_CHAR( __ram_factura, 0 );
        SET_MEMORY_CHAR( __ram_nota_cr, 0 );
        SET_MEMORY_CHAR( __ram_bien_de_uso, 0 );
        if( PEDIR_PRECIO_EN_NC ) {
            SET_MEMORY_CHAR( __ram_pedir_precio, 0 );
        }
    }
    MUESTRA_COMPROBANTE();
    MOSTRAR_MODO_BIEN_DE_USO_EN_PANTALLA();
}
*/

/*****************************************************************************/
void CR1_INCOSISTENCIA_FISCAL( int sumar, int origen, int t_espera, int cerear )
/*****************************************************************************/
{
	int fin_ticket_no = 0;

	MENSAJE( "INCONSISTENCIA FISCAL" );
	GRABAR_LOG_SISTEMA( "INCONSISTENCIA FISCAL" ,LOG_ERRORES,1);
	PAUSA( 18 );
	if( !cerear )
		fin_ticket_no = 100;

	if( CONDICIONES_PARA_ESPERA() && BUSCAR_TICKET_LIBRE() != -1 && !RAM_NOTA_CR ){//se decidio que por  inconsistencias fiscales no se envien a espera y se anulen, por la recuperacion de las mismas de espera
		if( sumar ){
			ACTUALIZA_ACUMULADORES_RAM( RAM_P_TICKET-1 );
			ADD_MEMORY_DOUBLE( __ram_cantidad_total, RAM_CANTIDAD );
			ADD_MEMORY_INT( __ram_renglones, 1 );
			//tarea: 3574
			GRABAR_EVENT_E_TICKET( RAM_P_TICKET - 1, NO, NO, (char)origen, _TICKET_TIPO( RAM_P_TICKET - 1 ), NO);
		}
		SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_INCONSISTENCIA_FISCAL_SUSPENDIDO + fin_ticket_no );
		TICKET_EN_ESPERA( NO );
		MENSAJE_TECLA_STRING( S_TICKET_PASADO_A_ESPERA_presione_enter );
	} else { 
		SET_MEMORY_CHAR( __ram_anular_ticket, _ANUL_INCONSISTENCIA_FISCAL_ANULADO + fin_ticket_no );
		TECLA_ANULA_TICKET( SI );
		MENSAJE_TECLA_STRING( S_TICKET_ANULADO_presione_enter );
	}
	reinit_promos(); //reiniciemos las promos
	if(config_tps.activarTFpermanente == 1) {
		long cli =  atol( config_tps.DniCliente );
		SET_MEMORY_LONG( __ram_nro_cliente, cli ); //aca reemplazar por el real del xml
		CR1_TICKET_FACTURA( );
		MUESTRA_COMPROBANTE();
	}
}
/*******************************************************************************/
int CONDICIONES_PARA_ESPERA(void){
/*******************************************************************************/
int rta = NO;	
if( RAM_P_TICKET > 1 && event_ticket.tipo_evento != 30 ){// los de espera quedan en espera!!
		if( IMPRESORA_FISCAL == FISCAL ) {
			rta = ((RAM_NRO_COMP )?SI:NO);
		}else{
			rta = ((NRO_EVENTO)?SI:NO);
		}
	}
	return rta;
}
/*******************************************************************************/
int CARGAR_MONEDERO_ELECTRONICO( int origen_promo, char *mensaje )
/*******************************************************************************/
{
	char num_tarj[21];
	int rta = 0;
	CEREAR_STRUCT( _var_tarjetas );
	if( VAR_TARJETAS_STATUS == 0 ) {
		rta = VALIDA_TARJETA( SI, NO, VENTAS, origen_promo, mensaje, NO, -1/*NO*/ );
	}
	if( rta ) {
		strncpy( num_tarj, GET_NUMERO_TARJETA_PRIVADO()/*VAR_TARJETAS_NUMERO*/, 21 );
		num_tarj[20] = 0;
		SET_MEMORY( __ram_cuenta_cliente, num_tarj );
		//Se utiliza variable provisoria para imprimir en TKT
		SET_MEMORY_INT( __ram_ptr, strlen( num_tarj ));
		GRABA_MODIFICACION_TARJETAS( 0, TARJETA_MONEDERO );
		CEREAR_STRUCT( _var_tarjetas );
	} else {
		SET_MEMORY( __ram_cuenta_cliente, "" );
		SET_MEMORY_INT( __ram_ptr, 0 );
	}
	MOSTRAR_MONEDERO_POR_PANTALLA();
	return( rta );
}
/***************************************************************************** */
void CR1_TICKET_FACTURA( void )
/*******************************************************************************/
{
	int       ok = 1/*, area_ant*/;
	int 	 tabla_ant, tipo_ant, clientess = 0;

   if( !RAM_TICKET_FACTURA ) {
	   if(config_tps.activarTFpermanente == 1) {
		   clientess = 1;
	   } else
	  	clientess = PEDIR_NRO_CLIENTE( 1, NO );
      if( ok && clientess ) {
			//area_ant = SELECTED(  );
			tabla_ant = SELECTED2( );
			tipo_ant = SELECTED_TIPO( );
			if( EXISTE_CLIENTE( RAM_NRO_CLIENTE ) ) {
				//Se agrega condicion porque LIBERTAD utiliza TF A y TF B
				//SELECTO solo tiene TF A
				if( FACT_INDEP_DEL_TIPO_DE_CLIENTE ) {
					SET_MEMORY_CHAR( __ram_ticket_factura, 1 );
				} else {
					/*
					if( clientes.cond_iva > 2 )
						SET_MEMORY_CHAR( __ram_ticket_factura, 2 );
					else {
						if( strlen( clientes.cuit ) > 0 )
							//Se debe obtener la limitacion de los TF desde la impresora ,sino no se debe permitir cargar el cliente
							SET_MEMORY_CHAR( __ram_ticket_factura, 1 );
						else {
							MENSAJE( " <<< CLIENTE CON CUIT INVALIDO >>>" );
							SET_MEMORY_LONG( __ram_nro_cliente, 0 );
							SET_MEMORY_CHAR( __ram_ticket_factura, 0 );
						}
					} reemplazado por el switch()*/
					switch(clientes.cond_iva){
						case RESP_INSC:
						case RESP_NO_INSC:{
							if( strlen( clientes.cuit ) > 0 )//Se debe obtener la limitacion de los TF desde la impresora ,sino no se debe permitir cargar el cliente
								SET_MEMORY_CHAR( __ram_ticket_factura, 1 );
							else {
								MENSAJE( " <<< CLIENTE CON CUIT INVALIDO >>>" );
								SET_MEMORY_LONG( __ram_nro_cliente, 0 );
								SET_MEMORY_CHAR( __ram_ticket_factura, 0 );
							}
							   }break;
 						case RESP_NO_IDEN:{
							if( strlen( clientes.cuit ) > 0 && atof(clientes.cuit)>0.0 )
								//Se debe obtener la limitacion de los TF desde la impresora ,sino no se debe permitir cargar el cliente
								SET_MEMORY_CHAR( __ram_ticket_factura, 2 );
							else {
								MENSAJE( " <<< CLIENTE CON CUIL INVALIDO >>>" );
								SET_MEMORY_LONG( __ram_nro_cliente, 0 );
								SET_MEMORY_CHAR( __ram_ticket_factura, 0 );
							}
							   }break;
					default:{
							SET_MEMORY_CHAR( __ram_ticket_factura, 2 );
							};
					}
					
					if( RAM_NRO_CLIENTE && clientess != 2 && !NRO_COD_CUOTA ) {
						_ELEGIR_CLIENTE();
					}
				}
			} else {
				SET_MEMORY_LONG( __ram_nro_cliente, 0 );
			}
			//SELECT( area_ant );
			SELECT_TABLE( tabla_ant, tipo_ant );
		} else {
			SET_MEMORY_LONG( __ram_nro_cliente, 0 );
		}
	} else {
		CEREAR_PERFIL_CLIENTE( );
		SET_MEMORY_CHAR( __ram_ticket_factura, 0 );
		SET_MEMORY_LONG( __ram_nro_cliente, 0 );
		SET_MEMORY_INT( __nro_cod_cuota, 0 );
	}
	MUESTRA_PERFIL();
	MUESTRA_COMPROBANTE( );
}
/***************************************************************************** */
void CR1_FRANQUEO( double importe, int modo )
/*******************************************************************************/
{
    int ok = 0, salir = 0, k = 0;

    MENSAJE( "DESEA FRANQUEAR? <ENTER> PARA CONTINUAR - <ESC> PARA SALIR" );
	while( !ok ) {
		k = GETCH( );
		switch( k ) {
			case 13:
				ok = 1;
				salir = 1;
			break;
			case 27:
				ok = 1;
				salir = 0;
			break;
			}
	}
	BORRAR_MENSAJE(  );
	if ( salir ) {
		if( modo == _MODO_COBRANZA ){
		   IMPRIMIR_TKT_FRANQUEO( modo );
		} else if( modo == _MODO_VENTA ){
		   SET_MEMORY_DOUBLE( __ram_subtotal, importe );
           IMPRIMIR_TKT_FRANQUEO( modo );
		}
	}
}

/************************************************************************************/
int CR1_ITEM_DIF_PRECIO( long cod_especial, int presentacion, char *cod_barra )
/************************************************************************************/
{
   int       h, rta = 0;
   char      cod_barra_tmp[17];

   memcpy( cod_barra_tmp, cod_barra, 17 );

   for( h = 0; h < RAM_P_TICKET && !rta; h++ ) { //se recorre el ticket buscando art.c/dif.prec.
      if( _TICKET_COD_ESPECIAL( h ) == cod_especial
			 && _TICKET_PRESENTACION( h ) == presentacion
			 && ( strcmp( _TICKET_COD_BARRA( h ), cod_barra_tmp ) == 0 ) 
			 && ( _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO 
				|| _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO_ANULADA )
		) {
		rta = 1;
      }
   }

   return( rta );
}
/************************************************************************************/
int BUSCA_ULTIMO_ITEM_SIN_ENGANCHE(long* cod_esp,int* presentacion,char* barra){
/************************************************************************************/
	int h=0, ok = NO;
	char codbarra[17];
	for( h = RAM_P_TICKET - 1 ;h > 0 && ok == NO ;h-- ){
		if(!(_TICKET_TIPO( h ) & _ENGANCHADO )){//de ser necesario agregar otros tipo o combinaciones
			* cod_esp = _TICKET_COD_ESPECIAL( h );
			* presentacion=_TICKET_PRESENTACION(h);
			 _snprintf( codbarra, sizeof(codbarra)-1, "%s", _TICKET_COD_BARRA( h ) );
			  memcpy( barra,codbarra, 17 );
			ok = SI;
		}
	}
	return ok;
}

/************************************************************************************/
int CR1_EXISTE_ITEM_DIF_PRECIO( long cod_especial, int presentacion, int anulacion,
   char *cod_barra )
/************************************************************************************/
{
   int       h, dif = 0, _anul = 1, cant_dif_anterior = 0;
   int       dif_prec = 0, existe = 0, dif_precio_anulada = 0, ok = 0;
   float     cont = 0.0, cont_dif_anu = 0, cond = 0, cond_anterior = 0;
   double    art = 0.0;
   char      cod_barra_tmp[17];

   memcpy( cod_barra_tmp, cod_barra, 17 );

   total_acumulado = 0.0;
   difer = 0.0;
   for( h = 0; h < RAM_P_TICKET; h++ ) { //se recorre el ticket buscando art.c/dif.prec.



      if( _TICKET_COD_ESPECIAL( h ) == cod_especial
         && _TICKET_PRESENTACION( h ) == presentacion
		 && ( strcmp( _TICKET_COD_BARRA( h ), cod_barra_tmp ) == 0 ) ) {

		 if( !( _TICKET_TIPO( h ) & _RESTA )
				&& _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO ) { //origen dif.prec.
			cond += _TICKET_CANTIDAD( h );
			
         } else if( ( _TICKET_ORIGEN( h ) == _ORIGEN_DIF_PRECIO )
						&& ( _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO_ANULADA ) ) { //origen anulación
				cond -= _TICKET_CANTIDAD( h );
				
		 } else if( ( _TICKET_TIPO( h ) & _RESTA ) && ( _TICKET_ORIGEN( h ) == _ORIGEN_TICKET ) ) {
			    //cond_anterior = cond;
				cond = 0;
					
         }
      }
   }
   
   if( cond || !anulacion ) {
      for( h = 0; h < RAM_P_TICKET; h++ ) {
         if( _TICKET_COD_ESPECIAL( h ) == cod_especial
            && _TICKET_PRESENTACION( h ) == presentacion
            && strcmp( _TICKET_COD_BARRA( h ), cod_barra_tmp ) == 0 ) {
            /*--------------------- Origen Artículo -------------------*/
            
            if( _TICKET_ORIGEN( h ) == _ORIGEN_TICKET
					&& ( _TICKET_COD_MOVIMIENTO( h ) != _DIFERENCIA_PRECIO )
					&& !( _TICKET_TIPO( h ) & _RESTA ) ) {

			   ok = 1;
			   
			   
			   
			   if( ok ) {
					if( !dif_prec ) {
						dif_prec = 1; //activo dif.prec.
					}
                    art += _TICKET_CANTIDAD( h ); //contador para determinar cantidad a anular


					if( cont_dif_anu > 0 ) {
						cont += cont_dif_anu + _TICKET_CANTIDAD( h );
						cont_dif_anu = 0;
					} else {
						cont += _TICKET_CANTIDAD( h ); //contador para determinar si permite dif.prec.
					}


					total_acumulado += _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h );
					
					
					_anul = -1; //activo anulación con art.dif.prec.


					if( art ){
						ASIGNA_COD_ESPECIAL_COD_BARRA( _TICKET_COD_ESPECIAL( h ) , _TICKET_COD_BARRA( h ) );
						SET_MEMORY_DOUBLE( __item_precio, fabs( total_acumulado ) / art );
					}


					SET_MEMORY_DOUBLE( __ram_cantidad, art );
			  }
           /*--------------------- Origen Dif.Precio anulada -------------------*/
            } else
				if( _TICKET_ORIGEN( h ) == _ORIGEN_DIF_PRECIO
					&& ( _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO_ANULADA )
					&& ( _TICKET_TIPO( h ) & _RESTA ) ) {
				  ok = 1;
				  dif_precio_anulada --;
				  /*if( ITEM_PESABLE
						&& ( ROUND( (_TICKET_CANTIDAD( h ) ), 5, 3 ) != ROUND( RAM_CANTIDAD, 5, 3 ) ) ) {
					ok = 0;
					dif_precio_anulada ++;
				  }
				  if( ITEM_PEDIR_PRECIO && fabs( ( _TICKET_PRECIO( h ) * -1 ) - ITEM_PRECIO ) > 0.0001 ) {
						ok = 0;
						dif_precio_anulada ++;
				  }*/
				  if( ok ) {
					total_acumulado += _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h ) * -1;
					ASIGNA_COD_ESPECIAL_COD_BARRA( _TICKET_COD_ESPECIAL( h ) , _TICKET_COD_BARRA( h ) );
					if( art ) {
						SET_MEMORY_DOUBLE( __item_precio, fabs( total_acumulado ) / art );
						SET_MEMORY_DOUBLE( __ram_cantidad, art );
					} else {
						SET_MEMORY_DOUBLE( __item_precio, fabs( total_acumulado ) );
					}
					dif_prec = 1; //art.c/dif.prec.
					cont = cont_dif_anu;
					cont_dif_anu = 0;
					dif = 0; //bandera para anular dif.prec.

				  }

			  /*--------------------- Origen Dif.Precio -------------------*/
			} else
				if( !( _TICKET_TIPO( h ) & _RESTA )
			      && _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO ) {
				ok = 1;
		  
				if( ok ) {
					dif_precio_anulada ++;
					
					total_acumulado += _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h );
					
					difer = _TICKET_PRECIO( h );//Guardamos la diferencia para las impresoras fiscales

					ASIGNA_COD_ESPECIAL_COD_BARRA( _TICKET_COD_ESPECIAL( h ) , _TICKET_COD_BARRA( h ) );
					
					
					if( art ) {
						SET_MEMORY_DOUBLE( __item_precio, fabs( total_acumulado ) / art );
						SET_MEMORY_DOUBLE( __ram_cantidad, art );
					} else {
						SET_MEMORY_DOUBLE( __item_precio, fabs( total_acumulado ) );
					}


					if( dif_precio_anulada != 0 ) {
						dif_prec = -1; //art.c/dif.prec.
						cont_dif_anu += cont;
						cont = 0.0; //contador para determinar si permite dif.prec.
						dif = 1; //bandera para anular dif.prec.
					}
				}
            /*--------------------- Origen Anulación -------------------*/
			} else if(  _TICKET_TIPO( h ) & _RESTA
               && _TICKET_ORIGEN( h ) == _ORIGEN_TICKET ) { //origen anulación
                ok = 1;
			

				if( ok ) {
               if( cont != 0.0 && !dif ) {
					//cont --; //contador para determinar si permite dif.prec.
						cont -= _TICKET_CANTIDAD( h );
               } else {
                  cont = 0.0; //contador para determinar si permite dif.prec.
               }
               if( anulacion && cont == 0.0 && dif ) {
                  _anul = 0; //desactivo anulación con art.dif.prec.
               } else {
                  _anul = -1; //activo anulación con art.dif.prec.
               }
               if( cont == 0.0 ) { //contador para determinar si permite dif.prec.
                  dif_prec = 0; //desactivo dif.prec.
                  total_acumulado = 0.0;
				  //cont_dif_anu --;
				  cont_dif_anu -= _TICKET_CANTIDAD( h );
               } else {
                  dif_prec = 1; //activo dif.prec.
                  total_acumulado -= _TICKET_PRECIO( h ) * _TICKET_CANTIDAD( h );
               }
			   art -= _TICKET_CANTIDAD( h ); //contador para determinar bultos a anular
			   if( fabs( art ) > 0.005 )
               SET_MEMORY_DOUBLE( __item_precio, fabs( total_acumulado ) / art );
               SET_MEMORY_DOUBLE( __ram_cantidad, art );
               if( dif ) {
                  art = 0.0; //contador para determinar bultos a anular
               }
               dif = 0; //bandera para anular dif.prec.
            }
         }
         }

      }
   }
   // Controlo que sean solo cantidades enteras.
   if( RAM_ANULACION && ROUND( art, ENTEROS, 0 ) == art && art > 1 && dif ) { //calculo bultos a anular
       if(ITEM_PESABLE || ITEM_PEDIR_PRECIO){
            ADD_MEMORY_INT( __ram_cant_bultos, ( int )(fabs( total_acumulado ) / art) );
       }else{
            ADD_MEMORY_INT( __ram_cant_bultos, ( int )( art - 1 ) );
       }

   }
   if( anulacion ) { //origen anulación
      existe = _anul;
   } else { //origen dif.prec.
      existe = dif_prec;
   }

	return ( existe );
}
/*****************************************************************************/
void IMPRIMIR_COPIA_TICKET_FACTURA( )
/*****************************************************************************/
{
	int  i = 0 ;

	if( TKT_CONFIGURABLE && RAM_TICKET_FACTURA && IMPRIMIR_COPIA_TF
			&& RAM_COD_FUNCION == 30 ) {
		SET_MEMORY_INT( __ram_copia, 1 );
		IMPRIMIR_TICKET_COMPLETO( SI );
		if( TKT_CONFIGURABLE ) {
			IMPRIME_TKT_IMPORTE_TOTAL( 1 );
		}
		PAGOS_IMPRIME_PAGO( _MODO_VENTA );
		if( RAM_ANULAR_TICKET > 100 )
			FIN_DE_TICKET( NO );
		IMPRIME_PIE( );
	}
}

/*********************************************************************/
char* MASCARA( char digito, int cantidad, char *valor )
/*********************************************************************/
{
	static char *b_mask = NULL;
	static int p = 0;
	char *p_mask = NULL;
	int t_buffer = 200;

	int cantidad_digitos = 0;
	char _valor[25];

	memset(_valor, 0, sizeof(_valor));
	
	if( strcmp( valor, "" ) != 0 ) {
		_snprintf( _valor, sizeof(_valor)-1,"%s", valor );
		cantidad_digitos = strlen( _valor );
		if( cantidad_digitos > cantidad )
			cantidad = cantidad_digitos;
	}

   if( b_mask == NULL ) {
      b_mask = ( char* ) MALLOC( t_buffer );
      memset( b_mask, 0x00, t_buffer );
      p = 0;
   }
   if( ( p + cantidad + 1 ) > ( t_buffer - 1 ) ) { //Si no hay espacio para alojarla en el buffer
      p = 0;
   }
   p_mask = &b_mask[p];
   memset( p_mask, digito, cantidad );
   p_mask[cantidad] = 0x00;
   p = p + cantidad + 1;

   return p_mask;
}


/************************************************************/
double PRECIO_UNITARIO_PARCIAL( int h, double* cant )
/************************************************************/
/*
	Dada una posición 0 <= h < RAM_P_TICKET, devuelve el precio unitario del artículo
	en dicha posición, teniendo en cuenta todos los cambios de precios y sus posibles
	anulación en las posiciónes anteriores a h dentro del ticket.

	En cantidad devuelve la cantidad con que ha sido tickeado dicho articulo.
*/
{
	int j;
	char codBarra[256];
	long codEsp;
	int buscar_promocion_por_codigo_inte;
	double precio = 0, cantidad = 0;
	double precioPrimeraOcurrencia = 0;

	assert( 0 <= h );
	assert( h < RAM_P_TICKET );

	buscar_promocion_por_codigo_inte = (BUSCAR_PROMOCION_POR_CODIGO_INTE ? 1 : 0 );
	memset( codBarra, 0, sizeof( codBarra ) );
	if ( buscar_promocion_por_codigo_inte ) {
		codEsp = _TICKET_COD_ESPECIAL(h);
	} else {
		codEsp = _TICKET_COD_ESPECIAL(h);
		_snprintf( codBarra, sizeof(codBarra)-1, "%s", _TICKET_COD_BARRA( h ) );
		//strncpy( codBarra, _TICKET_COD_BARRA( h ), 256 );
	}

	for ( j = 0; j <= h; j++ ) {
		int encontrado = 0;
		if ( buscar_promocion_por_codigo_inte ) {
			if ( codEsp == _TICKET_COD_ESPECIAL( j ) ) {
				encontrado = 1;
			}
		} else {
			if ( ( codEsp == _TICKET_COD_ESPECIAL( j ) ) &&
				 ( strcmp( codBarra, _TICKET_COD_BARRA( j ) ) == 0 ) ){
				encontrado = 1;
			}
		}
		if ( ( encontrado ) && ( _TICKET_ORIGEN( j ) != _ORIGEN_PROMOCION_DESC ) ){
			if ( !precioPrimeraOcurrencia ) {
				// Si es la primera vez que aparece en el ticket, guardo su precio unitario
				// esto se utiliza para conocer el precio unitario, en caso que la cantidad
				// total en el ticket resulte 0.
				precioPrimeraOcurrencia = _TICKET_PRECIO( j );
			}
//			if( _TICKET_ORIGEN( j ) != _ORIGEN_DIF_PRECIO ) {
			if( ( _TICKET_COD_MOVIMIENTO( j ) != _DIFERENCIA_PRECIO ) &&
			    ( _TICKET_COD_MOVIMIENTO( j ) != _DIFERENCIA_PRECIO_ANULADA ) ) {
					cantidad += ( _TICKET_TIPO( j ) & _RESTA ? - _TICKET_CANTIDAD( j ) : _TICKET_CANTIDAD( j ) );
			}
			//precio += _TICKET_COSTO( j );//debug -> eliminar
				precio += ( _TICKET_TIPO( j ) & _RESTA ? - _TICKET_CANTIDAD( j ) * _TICKET_PRECIO( j ) : _TICKET_CANTIDAD( j ) * _TICKET_PRECIO( j ) );
		}
	}

	if ( cant ) {
		*cant = cantidad;
	}
	if ( cantidad != 0 ) {
		return precio / cantidad ;
	} else {
		return precioPrimeraOcurrencia;
	}
}
/*******************************************************************************/
void CEREAR_DATOS_COMPROB_ANTERIOR( void )
/*******************************************************************************/
{
 	SET_MEMORY_LONG( __ram_boleta_anterior, 0L );
  	SET_MEMORY_INT( __ram_sucursal_anterior, NO );
 	SET_MEMORY_INT( __ram_caja_anterior, NO );
 	SET_MEMORY_INT( __ram_fecha_anterior, NO );
   SET_MEMORY_CHAR( __ram_tipo_comp_anterior, NO );
   SET_MEMORY_INT( __ram_hora_anterior, NO );
   SET_MEMORY_INT( __ram_modo_devolucion, NO );
   SET_MEMORY_CHAR( __modo_devolucion, NO );
   SET_MEMORY_CHAR( __ram_devolucion_parcial_ref, NO );
   CEREAR_MEMORY( __ram_nro_serie_impresora_ant );
   SET_MEMORY_INT( __ram_negativo, NO );
   SET_MEMORY_LONG( __nro_evento_anterior, 0 );
   SET_MEMORY_LONG( __ram_nro_cliente, 0 );
   SET_MEMORY_LONG ( __ram_nro_cliente_ant, 0 );
   CEREAR_MEMORY( __cliente_afinidad_nombre );
   SET_MEMORY_LONG( __ram_cliente_afinidad, 0 );
}

/**************************************************************************************/
void GUARDAR_DATOS_PROD_RESERVADOS( long id_transaccion, char *nro_linea,
   double monto, long valor )
/**************************************************************************************/
{

    _snprintf( datos_cobro_kinacu.cod_barra, sizeof(datos_cobro_kinacu.cod_barra)-1, "%s", ITEM_COD_BARRA );
	datos_cobro_kinacu.cod_interno = ITEM_COD_ESPECIAL;
    datos_cobro_kinacu.id_transaccion = id_transaccion;
    strncpy( datos_cobro_kinacu.nro_linea, nro_linea, sizeof( datos_cobro_kinacu.nro_linea ) );
	datos_cobro_kinacu.estado = 0;
    datos_cobro_kinacu.monto = monto;
    datos_cobro_kinacu.cod_valida = ( int )valor;
	datos_cobro_kinacu.pos_ticket = RAM_P_TICKET;
    strncpy( datos_cobro_kinacu.mensaje1, ITEM_NOMBRE_LARGO, sizeof( datos_cobro_kinacu.mensaje1 ) );
	GUARDAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU, (char*)&datos_cobro_kinacu,
		sizeof( struct _datos_cobro_kinacu ) );
    ADD_MEMORY_LONG( __ram_nro_pedido_pin, 1 );
    posicionTransaccionKinacu++;
}

/**************************************************************************************/
int OBTENER_COD_ESPECIAL_COBRO( int cod_cobro )
/**************************************************************************************/
{
   int   rta = 0;
   int   area_ant, tipo_ant/*, area_aux*/;

   if( cod_cobro ) {
      area_ant = SELECTED2();
      tipo_ant = SELECTED_TIPO();
      //area_aux = AREA_DISPONIBLE(  );
      if(OPEN_TABLE(T_COBROS, TT_ORIG ,( char* )&cobros, sizeof( cobros )) == 0 ) {
         //SELECT_TABLE( T_COBROS, TT_ORIG );
         SET_WHERE("CODIGO = '%i'",cod_cobro);
         RUN_QUERY(NO);
         //GET_EQUAL( ( char * ) &cod_cobro );
   		if( FOUND2( ) ) {
            if( cobros.cod_especial == 2 ) { //si es recarga saldo
   		      rta = 1;
   		   }
         }
         CLOSE_TABLE( T_COBROS, TT_ORIG );
      }
      SELECT_TABLE( area_ant, tipo_ant );
   }

   return rta;
}

/*******************************************************************************/
int CR1_EXISTE_ITEM_ESTRICTO( long cod_esp, int presentacion, char *cod_barra,
   int comp_por_importe )
/*******************************************************************************/
//comp_por_importe = SI -> Se compara por importe
//comp_por_importe = NO -> Se compara por importe
{
    int       h, existe = 0, t;
    int       por_cod_int_cod_barra = 0, cond = 0;
    int       _presentacion;
    double    _cantidad, _precio;
    char      cod_barra_tmp[17], item_cod_barra[17];
    char      *anulados;
    long      cod_especial_aux;
    char cadena_debug[80];

    memcpy( item_cod_barra, cod_barra, 17 );

    anulados = ( char * ) MALLOC( _LARGO_TICKET );

    memset( anulados, 0x00, _LARGO_TICKET );

    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA && !cod_esp )
        por_cod_int_cod_barra = 1;

    t = RAM_P_TICKET - 1;
    while( t >= 0 ) {
        if( _TICKET_TIPO( t ) & _RESTA && _TICKET_ORIGEN( t ) == _ORIGEN_TICKET ) {

            _presentacion = _TICKET_PRESENTACION( t );
            _cantidad = _TICKET_CANTIDAD( t );
            _precio = _TICKET_PRECIO( t );
            cod_especial_aux = _TICKET_COD_ESPECIAL( t );
            strcpy( cod_barra_tmp, _TICKET_COD_BARRA( t ) );
            cond = 0;
            for( h = 0; h < RAM_P_TICKET && !cond; h++ ) {
                if( anulados[h] == 0 && !( _TICKET_TIPO( h ) & _RESTA )
      	        && _TICKET_ORIGEN( h ) == _ORIGEN_TICKET
      	        && ( !comp_por_importe )
      	        ? ROUND( _TICKET_CANTIDAD( h ), 5, 3 ) == ROUND( _cantidad, 5, 3 )
      	        : ROUND( _TICKET_PRECIO( h ), 5, 3 ) == ROUND( _precio, 5, 3 ) ) {
                if( por_cod_int_cod_barra ) {
                    if( _TICKET_COD_ESPECIAL( h ) == cod_especial_aux
                        && _TICKET_PRESENTACION( h ) == _presentacion
                        && ! strcmp( _TICKET_COD_BARRA( h ), cod_barra_tmp ) ) {
                        cond = 1;
                    }
                } else {
                    if(  _TICKET_COD_ESPECIAL( h ) == cod_especial_aux
                        && _TICKET_PRESENTACION( h ) == _presentacion ) {
                        cond = 1;
                    }
                }
                }
            }
            if( cond && h > 0 ) {
                anulados[h-1] = 1;
            }
        }
        t--;
    }
    if( MODO_DEBUG >= 50 || MODO_DEBUG == 44 ) {
        GRABAR_LOG_SISTEMA( "ANULADOS:",LOG_DEBUG,4 );
        for( h = 0; h < RAM_P_TICKET; h++ ) {
		    _snprintf( cadena_debug, 80, "%i %i", h, anulados[h] );
            cadena_debug[79] = '\0';

            GRABAR_LOG_SISTEMA(cadena_debug,LOG_DEBUG,4);
        }
    }
    for( h = 0; h < RAM_P_TICKET  && !existe; h++ ) {
        cond = 0;
        if( anulados[h] == 0 && !( _TICKET_TIPO( h ) & _RESTA )
            && _TICKET_ORIGEN( h ) == _ORIGEN_TICKET ) {
            if( por_cod_int_cod_barra ) {
                if( _TICKET_COD_ESPECIAL( h ) == cod_esp
                && _TICKET_PRESENTACION( h ) == presentacion
                && !strcmp( _TICKET_COD_BARRA( h ), item_cod_barra ) ) {
                cond = 1;
                }
            } else {
                if( _TICKET_COD_ESPECIAL( h ) == cod_esp
                && _TICKET_PRESENTACION( h ) == presentacion ) {
                cond = 1;
                }
            }
            if( cond ) {
                if( !comp_por_importe ) {
      			    if( ( ROUND( _TICKET_CANTIDAD( h ), 5, 3 ) == ROUND( RAM_CANTIDAD, 5, 3 ) ) ) {
      				    existe = 1;
      			    }
      	    } else {
      	        if( ( ROUND( _TICKET_PRECIO( h ), 5, 3 ) == ROUND( ITEM_PRECIO * RAM_CANTIDAD, 5, 3 ) ) ) {
      	            existe = 1;
      	        }
      	    }
                if( MODO_DEBUG >= 50 || MODO_DEBUG == 44 ) {
                char buff[100];

		        _snprintf( buff, 100, "ANULANDO: %i %li",h,_TICKET_COD_ESPECIAL( h ) );
                buff[99] = '\0';

                GRABAR_LOG_SISTEMA(buff,LOG_DEBUG,4);
                }
   		    }
		    }
	    }
        FREEMEM( anulados );

	    return ( existe );
}

/*************************************************************************/
int VALIDAR_ARTICULO_TICKET( int h ,int *acumulador_rtas)
/*************************************************************************/
{
	double signo;
    float tasas = 0,cant, cant_dif = 1;
    int  tabla_ant = 0, tipo_ant = 0;
    int i, rta = 1, _iva =0;
    char cod_barra[17];
	int leido_por_scanner = 0;

	
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

    if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
        memcpy( cod_barra, _TICKET_COD_BARRA( h ), 17 );
        BUSCA_COD_INTERNO_COD_BARRA( _TICKET_COD_ESPECIAL( h ), cod_barra );
    }
    else {
        BUSCA_CODIGO_ESPECIAL( _TICKET_COD_ESPECIAL( h ) );
    }

    signo = ( _TICKET_TIPO( h ) & _RESTA ) ? -1 : 1;

	if( ( _TICKET_TIPO( h ) & _ENVASE ) &&
			_TICKET_CANTIDAD( h ) < 0 &&
			( RAM_MODO_DEVOLUCION != _DEVOL_TOTAL
			&& RAM_MODO_DEVOLUCION != _NC_TOTAL ) ) {
		SET_MEMORY_CHAR( __ram_negativo, 1 );
	}

    if(( ( MODO_DEVOLUCION || RAM_NOTA_CR ) && ( _TICKET_TIPO( h ) & _DEV_ENVASE ) )
         || RAM_NEGATIVO) { 
        signo *= -1;
    }

    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.id_evento == RAM_ID_EVENTO;    
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.caja_z == ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_articulo == _TICKET_COD_ESPECIAL( h );
        rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.cantidad 
			- ( ( float )( _TICKET_CANTIDAD( h ) * ( RAM_NEGATIVO? 1 : signo ) ) ) ) < 0.1 ;

    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cant_presentacion == _TICKET_PRESENTACION( h );

	if( IMPRESORA_FISCAL == FISCAL && ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1) ){
		int rta2= rta;
		rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
			&& fabs( event_e_ticket.importe_sin_iva 
					- fabs((_TICKET_IMPORTE_SIN_IVA( h ) - _TICKET_IMP_INT( h ) )  * signo ) ) < 0.05 ;	
		if ( rta2 == 1 && rta == 0 && IMPRESORA_FISCAL == FISCAL && (RAM_MODO_DEVOLUCION == _NC_TOTAL ||( RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL  && event_e_ticket.origen == _ORIGEN_PROMOCION)) ) { //LA PARCIAL puede variar mucho este item
				rta = 1;
				rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
						&& fabs( event_e_ticket.importe_sin_iva 
							- ( (_TICKET_IMPORTE_SIN_IVA( h ) - _TICKET_IMP_INT( h ) )  * signo ) ) < 0.05 ;
		}
	} else
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.importe_sin_iva 
				- ( (_TICKET_IMPORTE_SIN_IVA( h ) - _TICKET_IMP_INT( h ) )  * signo ) ) < 0.5 ;
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.imp_int - ( _TICKET_IMP_INT( h ) * signo ) ) < 0.01 ;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.costo - ( _TICKET_COSTO( h ) * signo ) ) < 0.01 ;
    //  event_e_ticket.recargo_tarjeta =
    //      ( _TICKET_RECARGO( h ) - _TICKET_IVA1_REC( h ) - _TICKET_IVA2_REC( h ) -
    //      _TICKET_ILA_REC( h ) - _TICKET_IHA_REC( h ) ) * signo;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& event_e_ticket.recargo_tarjeta == ( _TICKET_RECARGO( h ) - _TICKET_IVA1_REC( h )
			- _TICKET_IVA2_REC( h ) ) * signo;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.exento - ( _TICKET_EXENTO( h ) * signo ) ) < 0.01 ;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& (fabs( event_e_ticket.iva1 )
			-  fabs( ( _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h ) )* signo )) < 0.05 ;
    
	
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.iva2 
			- ( ( _TICKET_IVA2_PRECIO( h ) + _TICKET_IVA2_REC( h ) )* signo ) ) < 0.01 ;
    //  event_e_ticket.ila = ( _TICKET_ILA_PRECIO( h ) + _TICKET_ILA_REC( h ) ) * signo;
    //  event_e_ticket.iha = ( _TICKET_IHA_PRECIO( h ) + _TICKET_IHA_REC( h ) ) * signo;
    //  event_e_ticket.alicuota = ( float ) ( _TICKET_ALICUOTA_PRECIO( h ) * signo );
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.monto_impuesto[0] 
			- ( ( float )( _TICKET_IMPUESTO1_PRECIO( h ) )* signo ) ) < 0.01 ;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.monto_impuesto[1] 
			- ( ( float )( _TICKET_IMPUESTO2_PRECIO( h ) )* signo ) ) < 0.01 ;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.monto_impuesto[2] 
			- ( ( float )( _TICKET_IMPUESTO3_PRECIO( h ) )* signo ) ) < 0.01 ;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
		&& fabs( event_e_ticket.monto_impuesto[3] 
			- ( ( float )( _TICKET_IMPUESTO4_PRECIO( h ) )* signo ) ) < 0.01 ;

    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.hab_desc_cliente == _TICKET_HAB_DESC_CLIENTE( h );
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_presentacion == _TICKET_COD_PRESENT( h );
	leido_por_scanner = ( _TICKET_TIPO( h ) & _SCANNER ) ? 1 : 0;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.leido_por_scanner == leido_por_scanner;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.tipo_precio == articulo.tipo_precio;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_ext == _TICKET_COD_EXT( h );
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.nro_vendedor == NRO_VENDEDOR;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_clasificacion_old == _TICKET_COD_CLASIFICACION( h );
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) && !strcmp( event_e_ticket.cod_clasificacion,_GET_MEMORY_STR( __ticket_cod_clasificacion_str, h ) );
    cant = _TICKET_CANTIDAD( h );
    cant = ( cant > 0 ) ? cant : -cant;

	if( IMPRESORA_FISCAL == FISCAL && ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1) ){
		int rta2= rta;
		rta = fabs( event_e_ticket.total) 
				- fabs(ROUND( ( _TICKET_PRECIO( h ) 
					+ _TICKET_RECARGO( h ) ) * cant * signo , ENTEROS, DECIMALES + 2 )) <= 0.05 ;
		if ( rta2 == 1 && rta == 0) { //verifiquemos si no se modifico a proposito en un tot+esc con promoç
			  rta = 1;
			   rta = fabs( event_e_ticket.total 
						- ROUND( ( _TICKET_ILA_REC(h) 
							+ _TICKET_RECARGO( h ) ) * cant * signo , ENTEROS, DECIMALES + 2) ) <= 0.1 ;
		}
		if ( rta2 == 1 && rta == 0 && IMPRESORA_FISCAL == FISCAL /*&& event_e_ticket.origen == _ORIGEN_PROMOCION */&& RAM_MODO_DEVOLUCION == _NC_RF_PARCIAL ) { //el origen promo puede tener mucha diferencia
			  rta = 1;
   			  rta = fabs( event_e_ticket.total 
						- ROUND( ( _TICKET_ILA_REC(h) 
							+ _TICKET_RECARGO( h ) ) * cant * signo , ENTEROS, DECIMALES + 2) ) <= 0.1 ;
		}
		rta = ACUMULA_RTAS( rta,acumulador_rtas ) ;
		/*	&& fabs( event_e_ticket.total 
				- ROUND( ( _TICKET_PRECIO( h ) 
					+ _TICKET_RECARGO( h ) ) * cant * signo , ENTEROS, DECIMALES + 2 ) ) < 0.00000001 ;*/

	}else
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
			&& fabs( event_e_ticket.total 
				- ROUND( ( _TICKET_PRECIO( h ) 
					+ _TICKET_RECARGO( h ) ) * cant * signo , ENTEROS, DECIMALES ) ) < 0.011 ;

	rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_depto == _TICKET_DEPTO( h );
	_iva = RECUPERAR_ALICUOTA_DE_ARTICULO( SI, event_e_ticket.cod_depto_old, event_e_ticket.cod_barra, event_e_ticket.cod_articulo );
	if( _iva != -1 ) {
		event_e_ticket.cod_iva = _iva;
	}
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_iva == _TICKET_COD_IVA( h );
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_movimiento == _TICKET_COD_MOVIMIENTO( h );
	if( RAM_DIFERENCIA_PRECIO && _TICKET_COD_MOVIMIENTO( h ) == _DIFERENCIA_PRECIO ) {
	   rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.origen == _ORIGEN_DIF_PRECIO;
	} else {
	   rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.origen == _TICKET_ORIGEN( h );
	}
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.tipo == (unsigned char)_TICKET_TIPO( h );
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.caja == NRO_CAJA;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.nro_z == NRO_Z;
    //event_e_ticket.posicion = h; //Ver esto de la version de Luis..
	if( RAM_POS_REV_PROMO ){
		rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.posicion == RAM_POS_REV_PROMO + h;
		//SET_MEMORY_INT( __ram_pos_rev_promo, RAM_POS_REV_PROMO + 1 );
	} else {
		rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.posicion == h + 1;
	}
  //  rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.descuento == 0.0;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.comprobante == ITEM_COMPROBANTE;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.motivo == RAM_MOTIVO_DESCUENTO;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && !strcmp( event_e_ticket.cod_barra, _TICKET_COD_BARRA( h ) );

	//ojo con estos campos en NC
	if( RAM_NOTA_CR == 0 ) {
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.habilitado_cuotas == ( ( _TICKET_TIPO2( h ) & _HABILITADO_CUOTAS ) ? 1 : 0 ) ;
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.tipo2 == (unsigned char)_TICKET_TIPO2( h );
	}
    if( articulo.gravado == 'S' ) {
        if( articulo.cod_iva ) {
            tasas = iva[( int )articulo.cod_iva].porc_iva1;
        }
        else {
            tasas = config.porc_iva1;
        }
    }
    else {
        tasas = 0;
    }
    for( i = 0;i < 4;i++ ) {
        if( articulo.cod_impuesto[i] ) {
            tasas += impuestos[( int )articulo.cod_impuesto[i] - 1].tasa;
        }
    }
    for( i = 0;i < 4;i++ ) {
        rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_impuesto[i] == articulo.cod_impuesto[i];
    }

    //event_e_ticket.precio_unitario =
    //  ROUND( ( ( event_e_ticket.importe_sin_iva - articulo.imp_int ) * ( tasas / 100 +
    //          1 ) ) + articulo.imp_int, 8, 2 );
    
	//R43
	if( IMPRESORA_FISCAL == FISCAL && ( RAM_TICKET_FACTURA == 1 || RAM_NOTA_CR == 1) ){
		int rta2= rta;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
				&& fabs( event_e_ticket.precio_unitario 
					- ( ROUND( fabs( _TICKET_IMPORTE_SIN_IVA( h )
                                                + _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h )
                                                + _TICKET_IMPUESTO1_PRECIO( h )
                                                + _TICKET_IMPUESTO2_PRECIO( h )
                                                + _TICKET_IMPUESTO3_PRECIO( h )
                                                + _TICKET_IMPUESTO4_PRECIO( h )
												+ _TICKET_EXENTO( h ) ), ENTEROS,
                                            DECIMALES ) ) ) <= 0.05 ;
		if ( rta2 == 1 && rta == 0) {
			rta = 1;
				rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
					   && fabs( event_e_ticket.precio_unitario
					    - ( ROUND( fabs( event_e_ticket.importe_sin_iva 
												+ event_e_ticket.iva1
												+ event_e_ticket.iva2
												+ event_e_ticket.monto_impuesto[0]
												+ event_e_ticket.monto_impuesto[1]
												+ event_e_ticket.monto_impuesto[2]
												+ event_e_ticket.monto_impuesto[3]
												+ event_e_ticket.exento), ENTEROS,
											 DECIMALES ) ) ) <= 0.011;

		}

	}else {
		rta = ACUMULA_RTAS( rta,acumulador_rtas ) 
				&& fabs( event_e_ticket.precio_unitario 
					- ( ROUND( fabs( _TICKET_IMPORTE_SIN_IVA( h )
                                                + _TICKET_IVA1_PRECIO( h ) + _TICKET_IVA1_REC( h )
                                                + _TICKET_IMPUESTO1_PRECIO( h )
                                                + _TICKET_IMPUESTO2_PRECIO( h )
                                                + _TICKET_IMPUESTO3_PRECIO( h )
                                                + _TICKET_IMPUESTO4_PRECIO( h )
												+ _TICKET_EXENTO( h ) ), ENTEROS,
                                            DECIMALES ) ) ) <= 0.011 ;
		}
    
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.cod_sucursal == config.sucursal;
    rta = ACUMULA_RTAS( rta,acumulador_rtas ) && ( event_e_ticket.fecha_ticket == NRO_FECHA_OPERACION/*( UTILIZAR_DIA_OPERATIVO ) ? NRO_DIA_OPERATIVO : GET_HORA_DOS()*/ );
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.tipo3 == _TICKET_TIPO3( h );
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.precio_original == 0.0;
	rta = ACUMULA_RTAS( rta,acumulador_rtas ) && event_e_ticket.excluir_promocion == 0;
	if( RAM_CAMBIO != 1 ) {
		SET_MEMORY_CHAR( __ram_negativo, 0 );
	}


	SELECT_TABLE( tabla_ant, tipo_ant );

	return rta;
}
/*************************************************************************/
int ACUMULA_RTAS( int rta,int *acumula ){
/*************************************************************************/
	if(rta == 1 ) 
		*acumula+=1;
	return rta;
}


/*************************************************************************/
void VERIFICAR_EVENTOS( void )
/*************************************************************************/
{
	int tabla_ant, tipo_ant;
	char log[100];
	int rtas_ok = 0,re_procesar = NO, cant_reg = 0;
	struct
	{
		long caja_z;
        long id_evento;
		INT16 posicion;
	} index;

	GRABAR_LOG_SISTEMA( "INICIANDO VERIFICACION DE E_TICKET" ,LOG_VENTAS,4);

	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();

	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	index.caja_z = ( long )( NRO_CAJA )*100000L + ( NRO_Z % 100000L );
	index.id_evento = RAM_ID_EVENTO;

	SET_WHERE( "CAJA_Z = %ld AND ID_EVENTO = %ld AND POSICION > 0", index.caja_z, index.id_evento );
	SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION");
	RUN_QUERY(NO);
	cant_reg = RECCOUNT2( NULL, NULL);
	//Uso RAM_P_TICKET para validar entre tabla ev_cont y estructura ticket
	for( index.posicion = 1; index.posicion <= RAM_P_TICKET && cant_reg; index.posicion++ )
	{
		rtas_ok = 0;
		re_procesar = NO;
		if( !VALIDAR_ARTICULO_TICKET( index.posicion - 1 ,&rtas_ok ) ) 
		{
			re_procesar = SI;
			_snprintf( log, sizeof( log ), "EVENTO E_TICKET NO ENCONTRADO caja z: %lu, id evento: %lu, posicion: %i (R:%i)\n",
					index.caja_z, index.id_evento, index.posicion, rtas_ok );
			log[sizeof( log ) - 1] = 0;
			GRABAR_LOG_SISTEMA( log ,LOG_VENTAS,2);
		}
		if( re_procesar == SI ) {
			//DIRECT_DB_QUERY("DELETE FROM EV_CONT WHERE CAJA_Z = '%ld' AND ID_EVENTO = '%ld' AND POSICION ='%i';",
					//index.caja_z, index.id_evento, index.posicion );
			GRABAR_EVENT_E_TICKET( index.posicion - 1, NO, SI,
				_TICKET_ORIGEN( index.posicion - 1 ),
				_TICKET_TIPO( index.posicion - 1 ) , SI );
			if( RAM_CAMBIO != 1 ) {
			   SET_MEMORY_CHAR( __ram_negativo, 0 );
			}
			/*Me posiciono nuevamente*/
			//Esto no hace falta porque comento la funcion MARCAR_ITEM_ORIGINAL_EN_E_TICKET( h ) que recorre 
			//toda la tabla hasta el fin.
			//SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );
	 		//SET_WHERE( "CAJA_Z = %ld AND ID_EVENTO = %ld AND POSICION >='%i'", index.caja_z, index.id_evento, index.posicion );
			//SET_ORDER2("CAJA_Z, ID_EVENTO, POSICION");
			//RUN_QUERY(NO);
		}
		SKIP2( 1 );
	}
	SELECT_TABLE( tabla_ant, tipo_ant );

	GRABAR_LOG_SISTEMA( "FINALIZANDO VERIFICACION DE E_TICKET" ,LOG_VENTAS,4);
}

/*******************************************************************************/
void NOMBRE_MEDIO_SUBMEDIO( int medio, int submedio, char *nombre, int largo )
/*******************************************************************************/
{
    char temp[80];
    int res;
    if( medios[medio].tarjeta && submedio > 0 ) {
        res = _snprintf( nombre, largo, NOMBRE_TARJETA( submedio - 1) );
    } else if( medios[medio].mutual ) {
        NOMBRE_MUTUAL( submedio, temp );
        res = _snprintf( nombre, largo, temp );
    } else if( medios[medio].ticket_compra ) {
        NOMBRE_TICKET_COMPRA( submedio, temp );
        res = _snprintf( nombre, largo, temp );
    } else {
        res = _snprintf( nombre, largo, medios[ medio ].nombre );
    }
    if( res == -1 && largo > 0 ) {
        nombre[largo - 1] = 0x00;//NULL
    }
}

/*******************************************************************************/
void PROCESAR_PROMOCIONES_EN_TICKET_EN_ESPERA( void )
/*******************************************************************************/
{
	/*----------------- motor ----------------------------*/
	if( UTILIZA_PROMOCIONES && UTILIZA_MODULO_PROMOCIONES ) {
		/*----------------- motor - reinicializar ------------*/
		reinit_promos( );
			/*----------------- motor - reinicializado ------------*/

		// Ahora se chequea si se trata de un recupero o un envio de ticket a espera
		if( RAM_P_TICKET ) {
			/* Se recuperó un ticket en espera.
			* Agregando a promociones los artículos recuperados.
			*/
			int h;

			for( h = 0;h < RAM_P_TICKET;h++ ) {

				if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
					BUSCA_CODIGO_ESPECIAL ( _TICKET_COD_ESPECIAL(h) );
				} else {
					BUSCA_COD_INTERNO_COD_BARRA( _TICKET_COD_ESPECIAL(h), _TICKET_COD_BARRA( h ) );
				}

				if ( _TICKET_ORIGEN( h ) != _ORIGEN_PROMOCION_DESC ) {
					if( !(_TICKET_TIPO( h ) & _RESTA) ) {
					//if( _TICKET_CANTIDAD( h ) > 0 ) {
						/**/char buf[256];
						/**/_snprintf( buf, sizeof(buf)-1, "PROMO: [IMPORTACION_TICKET] ---> Agregar Articulo: i%li - cantidad: %lf [Promoc:%i]", _TICKET_COD_ESPECIAL(h), _TICKET_CANTIDAD( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ) );
						/**/glog( buf ,LOG_DEBUG,4);

						if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
							if( _TICKET_ORIGEN( h ) == _ORIGEN_DIF_PRECIO ) {
								//update_articles( _TICKET_COD_ESPECIAL(h), NULL, _TICKET_CANTIDAD( h ), _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
					
								update_articles( _TICKET_COD_ESPECIAL(h), NULL, _TICKET_COD_PRESENT(h), 1, PRECIO_UNITARIO_PARCIAL( h, NULL ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
							} else if( _TICKET_COD_MOVIMIENTO( h ) == _CAMBIO_DE_MERCADERIA_MENOS ) {
								add_external_discount( PRECIO_UNITARIO_PARCIAL( h, NULL ), pr, &ss  );
							} else {
								// Se agrega un artiuclo.
								int por_importe = ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO ) ? 1 : 0;
								add_articles( _TICKET_COD_ESPECIAL(h), NULL, _TICKET_COD_PRESENT(h), _TICKET_CANTIDAD( h ), _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ), !FUNCION_PIDE_PRECIO && por_importe, pr, &ss );
							}
						} else {

							if( _TICKET_ORIGEN( h ) == _ORIGEN_DIF_PRECIO ) {
								//update_articles( _TICKET_COD_ESPECIAL(h), _TICKET_COD_BARRA( h ), _TICKET_CANTIDAD( h ), _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
								update_articles( _TICKET_COD_ESPECIAL(h), _TICKET_COD_BARRA( h ), _TICKET_COD_PRESENT(h), 1, PRECIO_UNITARIO_PARCIAL( h, NULL ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
							} else if( _TICKET_COD_MOVIMIENTO( h ) == _CAMBIO_DE_MERCADERIA_MENOS ) {
                                add_external_discount( PRECIO_UNITARIO_PARCIAL( h, NULL ), pr, &ss  );
							} else {
								// Se agrega un artiuclo.
								int por_importe = ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO ) ? 1 : 0;
								add_articles( _TICKET_COD_ESPECIAL(h), _TICKET_COD_BARRA( h ), _TICKET_COD_PRESENT(h), _TICKET_CANTIDAD( h ), _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ), !FUNCION_PIDE_PRECIO && por_importe, pr, &ss );
							}

						}
					}
					else {
						/**/char buf[256];
						/**/_snprintf( buf, sizeof(buf)-1, "PROMO: [IMPORTACION_TICKET] ---> Anular Articulo: i%li - cantidad: %lf [Promoc:%i]", _TICKET_COD_ESPECIAL(h), _TICKET_CANTIDAD( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ) );
						///**/sprintf( buf, "PROMO: [IMPORTACION_TICKET] ---> Anular Articulo: i%li - cantidad: %lf", ITEM_COD_ESPECIAL, RAM_CANTIDAD );
						/**/glog( buf ,LOG_DEBUG,4);

						if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
							if( _TICKET_ORIGEN( h ) == _ORIGEN_DIF_PRECIO ) {
								// Se anula un Cambio de Precio
								//update_articles( _TICKET_COD_ESPECIAL(h), NULL, _TICKET_CANTIDAD( h ), - _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
								update_articles( _TICKET_COD_ESPECIAL(h), NULL, _TICKET_COD_PRESENT(h), 1, PRECIO_UNITARIO_PARCIAL( h, NULL ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
							} else if( _TICKET_COD_MOVIMIENTO( h ) == _CAMBIO_DE_MERCADERIA_MENOS ) {
                                add_external_discount( -PRECIO_UNITARIO_PARCIAL( h, NULL ), pr, &ss  );
							} else {
								//remove_articles( ITEM_COD_ESPECIAL, NULL, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, pr, &ss );
								double precUnit, cant;
								int por_importe = ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO ) ? 1 : 0;
								precUnit = PRECIO_UNITARIO_PARCIAL( h, &cant );
								remove_articles( _TICKET_COD_ESPECIAL(h), NULL, _TICKET_COD_PRESENT(h), _TICKET_CANTIDAD( h ), _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ), !FUNCION_PIDE_PRECIO && por_importe, pr, &ss );
								if (  cant == 0 ) {
									update_articles( ITEM_COD_ESPECIAL, NULL, _TICKET_COD_PRESENT(h), RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
								}
							}
						} else {
							if( _TICKET_ORIGEN( h ) == _ORIGEN_DIF_PRECIO ) {
								// Se anula un Cambio de Precio
								//update_articles( _TICKET_COD_ESPECIAL(h), _TICKET_COD_BARRA( h ), _TICKET_CANTIDAD( h ), - _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
								update_articles( _TICKET_COD_ESPECIAL(h), _TICKET_COD_BARRA( h ), _TICKET_COD_PRESENT(h), 1, PRECIO_UNITARIO_PARCIAL( h, NULL ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ),pr, &ss );
							} else if( _TICKET_COD_MOVIMIENTO( h ) == _CAMBIO_DE_MERCADERIA_MENOS ) {
                                add_external_discount( -PRECIO_UNITARIO_PARCIAL( h, NULL ), pr, &ss  );
							} else {
								double precUnit, cant;
								int por_importe = ( _TICKET_TIPO2( h ) & _PEDIR_PRECIO ) ? 1 : 0;
								precUnit = PRECIO_UNITARIO_PARCIAL( h, &cant );
								//remove_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, pr, &ss );
								remove_articles( _TICKET_COD_ESPECIAL(h), _TICKET_COD_BARRA( h ), _TICKET_COD_PRESENT(h), _TICKET_CANTIDAD( h ), _TICKET_PRECIO( h ), _TICKET_DEPTO( h ), ARTICULO_INCLUIBLE_EN_PROMOCION( h ), !FUNCION_PIDE_PRECIO && por_importe, pr, &ss );
								if (  cant == 0 ) {
									update_articles( ITEM_COD_ESPECIAL, _TICKET_COD_BARRA( h ), _TICKET_COD_PRESENT(h), RAM_CANTIDAD, precUnit, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION,pr, &ss );
								}
							}

						}
					}
				}
				// ALGO COMO ESTO--------
			}


        } else {
			// ------------------- Se puso en espera el ticket actual
			/* Ya se anularon las promociones en curso.
			* Se inicializaron promociones para un nuevo ticket.
			*/

		}
    }
	/*----------------- motor ----------------------------*/

}

/*****************************************************************************************/
int DEBE_PEDIR_PRECIO_DEVOL_Y_NC( int operacion ){
/*****************************************************************************************/
/*esto se puede hacer configurable por tipo de comprobante  o tabla  para hacerlo de acuerdo al cliente*/
 int rta = NO;
	if( operacion == 0 ) 
		operacion = RAM_MODO_DEVOLUCION;
 
	if( operacion && PEDIR_PRECIO_EN_NC && ( operacion == _NC_LIBRE || 
                                      /*operacion == _NC_RF_PARCIAL ||*/
                                      operacion == _DEVOL_LIBRE 
                                      /*||operacion == _DEVOL_PARCIAL*/ ) ) {
       rta = SI;
    }
	return rta;
}

/*****************************************************************************/
int CR1_VERIFICAR_LIMITES_FACTURA( double total, int modo )
/*****************************************************************************/
{
	int       ok = SI;

	if( IMPRESORA_FISCAL && RAM_FACTURA && !( RAM_NOTA_CR /*&& IMPRESORA_FISCAL == FISCAL*/ ) ) {
		if( clientes.cod_cliente == NRO_CLIENTE_POR_DEFECTO && total > config.limite ) {
			MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
		}
	} else {
		if( ok && IMPRESORA_FISCAL && ( total > config.limite_ticket_factura ) ) {
			MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
			ok = NO;
		} else {
			if( total > config.limite ) {
				if( IMPRESORA_FISCAL  ) {
					if( ( !RAM_TICKET_FACTURA && ( !RAM_NOTA_CR &&
							IMPRESORA_FISCAL == FISCAL ) ) ) {
						if ( config.importe_minimo > total && config.pais == MEXICO && !RAM_FACTURA && !RAM_MODO_DEVOLUCION) {
							MENSAJE_STRING( S_IMPORTE_TICKET_EXCEDIDO );
						}
						if( PERMITE_SUPERAR_LIMITE_TICKET ) {
							if( !aut_exceder && ( ( VERIFICAR_LIMITE_DGI == modo ) || !modo ) ) {
								if( SOLICITAR_CLAVES( 30, 20, NO, SI, SI, "Autorizar limite ticket",
									"Autorizar limite ticket", RAM_COD_FUNCION, SI )  ) {
									aut_exceder = 1;
								} else {
								ok = NO;
								}
							}
						  } else {
							  MENSAJE( "DEBE CAMBIAR POR TICKET FACTURA" );
							  BORRAR_MENSAJE();
							 ok = NO;
						  }
					}
				} else {
					if( PERMITE_SUPERAR_LIMITE_TICKET || RAM_FACTURA ) {
						if( !aut_exceder && ( ( VERIFICAR_LIMITE_DGI == modo ) || !modo ) ) {
							MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
							if( SOLICITAR_CLAVES( 30 - ancho_panel / 2, 21, NO, SI, SI, "Autoriz¢ importe excedido",
								"Autoriz¢ importe excedido", _F_SUP_LIMITE_TICKET, SI ) ) {
								aut_exceder = 1;
							} else
								ok = NO;
						}
				   } else {
					  MENSAJE_STRING( S_IMPORTE_EXCEDIDO );
					  ok = NO;
				   }
				}
			}
		}
	}

	return ( ok );
}

/*****************************************************************************/
int VERIFICAR_TIPO_EVENTO_HABILITADO_IMPORTACION( int tipoEvento )
/*****************************************************************************/
{
	int habilitado = NO;
	char msj[100];

	memset( msj, 0 , sizeof(msj));
	switch( tipoEvento ) {
		//Tipo de eventos que pudo importar comprobante
        case _EVENT_TICKET :
        case _EVENT_PAGO :
        case _EVENT_FAC_A :
        case _EVENT_FAC_B :
        case _EVENT_FACTURA_A_REMITO :
        case _EVENT_FACTURA_B_REMITO :
        case _EVENT_FAC_A_REMOTA :
        case _EVENT_FAC_B_REMOTA :
        case _EVENT_REMITO_REMOTO :
        case _EVENT_TICKET_FACTURA_A :
        case _EVENT_TICKET_FACTURA_B :
		case _EVENT_ANULADO :
        case _EVENT_TICKET_ESPERA :
        	habilitado = SI;
            break;
        default :
        	//Tipo de eventos que no pude importar comprobante
			//case _EVENT_NC_A :
			//case _EVENT_NC_B :
			//case _EVENT_NC_A_REMOTA :
			//case _EVENT_NC_B_REMOTA :
        	_snprintf(msj, sizeof(msj)-1,"Tipo de Evento %i no habilitado para importacion",tipoEvento);
        	glog(msj, LOG_DEBUG, 4);
			habilitado = NO;
    }

    return habilitado;
}
