#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <actual.h> 
#include <afinidad.h> 
#include <anula_t.h> 
#include <btrv.h> 
#include <cio.h> 
#include <ini.h> 
#include <clave.h> 
#include <clientes.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <cr_pant.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <get_hora.h> 
#include <htoa.h> 
#include <import_t.h> 
#include <imp_tick.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <menu.h> 
#include <mstring.h> 
#include <m_cadena.h> 
#include <print.h> 
#include <recup.h> 
#include <rnv.h> 
#include <tkt.h> 
#include <t_espera.h> 
#include <_cr1.h> 
#include <bridge.h>
#include <actions.h>
#include <typeacti.h>
#include <db.h>
#include <deftables.h>
#include <path.h>
#include <_cr0.h>
#include <log.h>
#include <t_pago.h>
#include <round.h>
#include <cobros.h>
#include <recupero.h>
#include <tarjetas.h>
#include <fiscal.h>
#include <b_mens.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifdef INVEL_L
#else
#endif
#if defined(INVEL_W) || defined(INVEL_L)
#endif
void IMPRIME_MENSAJE_TRAE_TICKET_EN_ESPERA( void );
int BUSCAR_TICKET_LIBRE( void );
int OBTENER_COD_ESPECIAL( void );
char ticket_espera_pedir_cod_esp[10] = {0,0,0,0,0,0,0,0,0,0};
struct _cab_standby
{
    int nro_vendedor;
    int nro_cod_cuota;
    float nro_recargo_financiacion;
    int nro_mozo;
    int ram_p_ticket;
    double ram_total;
    int ram_item_supervisado;
    int ram_bultos;
    long ram_hora_inicio_ticket;
    char ram_factura;
    long ram_nro_cliente;
    char ram_no_reprocesar;
    long ram_id_evento;
    char ram_variar_precio;
    char ram_track1[80];
    char ram_track2[50];
    long ram_cliente_afinidad;
    char ram_nota_cr;
    double ram_cupon_envases;
    char ram_usar_precio_cr;
    double ram_total_vta_contado;
    unsigned ram_hora_inicio_evento;
    long ram_id_evento_all;
    int ram_p_ext_impuestos;
    double ram_tarj_importe;
    char ram_tarj_cuotas;
    char ram_tarj_numero[21];
    char modo_devolucion;
    double ram_puntos_usados;
    long nro_vendedorl;
    long ram_numero_ticket_tomra[5];
    int ram_cant_envases[5];
    char      ram_ticket_factura;
};
int GRABAR_STRUCT( int handle, int est );
int _GRABAR_STRUCT( int handle, int est, int subindice );
int _LEER_STRUCT( int handle, int est, int subindice );
int LEER_STRUCT( int handle, int est );
static int GRABA_TICKET_EN_ESPERA( char nro_ticket );
int AUTORIZA_TICKET_EN_ESPERA( char *titulo );
static int RECUPERA_TICKET_EN_ESPERA( char nro_ticket );
static void _TRAER_TICKET_EN_ESPERA( char nro_ticket );
int _CONTROLAR_TICKET_EN_ESPERA( char nro_ticket );
static int TRAER_TICKET_EN_ESPERA( void );
static int t_espera_on = 0;
extern int tipo_t_ev_cont;

/****************************************************************************/
int TICKET_EN_ESPERA( int modo )
/*****************************************************************************/
{
    char str_cliente_afinidad[20];
    long ram_cliente_afinidad;
	long nro_cliente;
    int cnt = 0, rta = 0,temp_nro_cod_cuota=0;
    UINT16 ram_tmpo_reg_articulo, ram_tmpo_pago;
    char msg[100];
	char tipo_evento_orig;
	char titulo[100];
//	double tot, subto;
	char ram_ticket_factura_anterior = RAM_TICKET_FACTURA;

    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        MENSAJE_STRING( S_FUNCION_NO_HABILITADA_EN_MODO_ENTRENAMIENTO );
        return( 0 );
    }
    #endif
    #if defined(INVEL_W) || defined(INVEL_L)
    if( MODALIDAD_TICKET_EN_ESPERA == 0 ) {
        //Local
        //hay un bug en esta modalidad. Se debera corregir en un proximo release
        MENSAJE( "TICKET EN ESPERA=NORMAL, NO HABILITADA EN VERSION VISUAL" );
        return( 0 );
    }
    #endif
	if( RAM_P_TICKET ) {
		if( MODALIDAD_TICKET_EN_ESPERA == 0 ) {
            //Local
            char nro_ticket = BUSCAR_TICKET_LIBRE();
            if( nro_ticket != ( char ) - 1 ) {
                if( AUTORIZA_TICKET_EN_ESPERA( ST( S_TICKET_ESPERA ) ) ) {
                    SETEAR_TIEMPO( _ENTRADA_OCIO ); 
                    ram_tmpo_pago = RAM_TMPO_PAGO;
                    ram_tmpo_reg_articulo = RAM_TMPO_REG_ARTICULO;
                    ACTUALIZA_BASE_DTO_EVENTO();
                    SETEAR_TIEMPO( _INDEFINIDO );
                    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
                    GRABA_TICKET_EN_ESPERA( nro_ticket );
                    ACUMULAR_TIEMPOS( );
                    CEREAR_TIEMPOS( );
                    SETEAR_TIEMPO( _ENTRADA_OCIO );
                    tipo_evento_orig = CALCULAR_TIPO_EVENTO( );
					ANULA_TICKET( SI );
                    memset( &event_ticket, 0, sizeof( event_ticket ) );
                    event_ticket.tipo_evento_orig = tipo_evento_orig;
                    event_ticket.tipo_evento = _EVENT_TICKET_ESPERA;
					event_ticket.bultos = RAM_BULTOS;
                    event_ticket.tmpo_pago = ram_tmpo_pago;
                    event_ticket.tmpo_reg_articulo = ram_tmpo_reg_articulo;
                    GRABAR_EVENT_TICKET( DEBE_GRABAR_ALL() );
                    ACTUALIZA_ID_EVENTO_NEW();
                    _SET_MEMORY_CHAR( __ram_mult_ticket_en_espera, nro_ticket, 1 );
                    _SET_MEMORY_LONG( __ram_hora_mult_ticket_en_espera, nro_ticket,
                                      GET_HORA_COMPLETA_DOS() );
                    ADD_MEMORY_CHAR( __ram_ticket_en_espera, 1 );
					//SET_MEMORY_DOUBLE( __ram_subtotal,  );
					rta = 1;
						if( ram_ticket_factura_anterior ) {
					   	char ram_tick_fac = RAM_TICKET_FACTURA;
					   	SET_MEMORY_CHAR( __ram_ticket_factura, ram_ticket_factura_anterior );
					   	GRABA_EVENTO_ANULADO( );
					   	SET_MEMORY_CHAR( __ram_ticket_factura, ram_tick_fac );
						}
                }
            }
            else {
                MENSAJE_STRING( S_MAXIMO_DE_TICKET_EN_ESPERA );
            }
        }
        else {
            //Remota
			SET_MEMORY_DOUBLE( __ram_subtotal,  RAM_TOTAL);
			//Contamos ticket en espera no enviados a la base y ticket en espera 
			//enviados a la base para saber la cantidad real de ticket en espera que hay
			//cnt = CONTAR_TICKET_ESPERA( 30, NRO_CAJA, NRO_Z, -1 );
			cnt = CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF );
            
			if( cnt < MAX_CANTIDAD_TICKET_EN_ESPERA ) {
					strcpy( titulo, "TICKET EN ESPERA REMOTA" );
				if( AUTORIZA_TICKET_EN_ESPERA( titulo ) ) {
						MENSAJE( "ENVIANDO TICKET EN ESPERA REMOTO..." );
                    ACTUALIZA_BASE_DTO_EVENTO();
                    //GRABA_TICKET_EN_ESPERA( 0 );
                    ram_cliente_afinidad = RAM_CLIENTE_AFINIDAD;
                    strncpy( str_cliente_afinidad, RAM_STRING_CLIENTE_AFINIDAD,
                             sizeof( str_cliente_afinidad ) );
					if( RAM_NRO_CLIENTE > 0 ) {
						nro_cliente = RAM_NRO_CLIENTE;
					} else {
						nro_cliente = 0;
					}
					temp_nro_cod_cuota=NRO_COD_CUOTA;
                    ram_tmpo_pago = RAM_TMPO_PAGO;
                    ram_tmpo_reg_articulo = RAM_TMPO_REG_ARTICULO;
					tipo_evento_orig = CALCULAR_TIPO_EVENTO( );
                    //ANULA_TICKET( SI );
                    SET_MEMORY_LONG( __ram_cliente_afinidad, ram_cliente_afinidad );
                    SET_MEMORY( __ram_string_cliente_afinidad, str_cliente_afinidad );
                    memset( &event_ticket, 0, sizeof( event_ticket ) );
					event_ticket.tipo_evento_orig = tipo_evento_orig;
                    event_ticket.tipo_evento = _EVENT_TICKET_ESPERA;
					SET_MEMORY_INT( __nro_cod_cuota, temp_nro_cod_cuota);
                    if( IMPRESORA_FISCAL == FISCAL ) {
                        event_ticket.nro_ticket = RAM_NRO_COMP + 1;
                    }else{
                        event_ticket.nro_ticket = NRO_EVENTO + 1;
                    }
                    event_ticket.cliente_afinidad = RAM_CLIENTE_AFINIDAD;
					event_ticket.nro_cliente = nro_cliente;
                    event_ticket.tmpo_pago = ram_tmpo_pago;
                    event_ticket.tmpo_reg_articulo = ram_tmpo_reg_articulo;
                    SETEAR_TIEMPO( _INDEFINIDO );
                    SET_MEMORY_INT( __ram_tmpo_ocio_aux, RAM_TMPO_OCIO );  
                    GRABAR_EVENT_TICKET( DEBE_GRABAR_ALL() );
                    ACUMULAR_TIEMPOS( );
                    CEREAR_TIEMPOS( );
                    ACTUALIZA_ID_EVENTO_NEW();
                    SETEAR_TIEMPO( _ENTRADA_OCIO );
                    SET_MEMORY_LONG( __ram_cliente_afinidad, 0 );
                    CEREAR_MEMORY( __ram_string_cliente_afinidad );
					//SLEEP(5000); //5 segundos de demora para darle tiempo al jsincro
					//cnt = CONTAR_TICKET_ESPERA( 30, NRO_CAJA, NRO_Z, -1 );
					//Se pone funcion ANULA_TICKET( SI ) despues de la grabacion
					//del evento para que ceree la variable __ram_instancia_evento al final
					ANULA_TICKET( SI );
					cnt = CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF );
                    _snprintf(msg, sizeof(msg)-1,"HAY %i TICKET EN ESPERA",cnt );
                    MENSAJE( msg );
                    ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_REM_VTAS_AGREGAR, " ", " ", NULL, " " );
					rta = 1;
            }
            }else{
                _snprintf(msg, sizeof(msg)-1, "HAY %i TICKET EN ESPERA", cnt );
                MENSAJE( msg );
                MENSAJE_STRING( S_MAXIMO_DE_TICKET_EN_ESPERA );
            }
        }
	}
    else if( !MODO_DEVOLUCION ) {
        if( MODALIDAD_TICKET_EN_ESPERA == 0 ) {//LOCAL
			if( TRAER_TICKET_EN_ESPERA( ) ) {
				rta = 1;
			}
		} else {//REMOTA
			/* Primero envio los offline por si quedo algun tespera procesado localmente */
			{
				int tabla_ant = SELECTED2();
				int tipo_ant = SELECTED_TIPO();
				SELECT_TABLE( tabla_ant, tipo_ant );
			}
            //Contar los tickets nos permite recuperar una supuesta desconexion
            //que en realiad fue un error logico en el servidor.
            //CONTAR_TICKET_ESPERA( 30, NRO_CAJA, NRO_Z, -1);
			strcpy( titulo, "INICIA TICKET EN ESPERA REMOTA" );
			if( IMPORTAR_TICKET( SI , 30, NO, titulo, PEDIR_SUPERVISOR_TRAE_TICKET_ESP,
				_ORIGEN_TICKET_EN_ESPERA, modo ,SI ,NO) ) {
				rta = 1;
			}
        }
		if( rta ) {
			MUESTRA_SUBTOTAL(NO);
			if( RAM_CLIENTE_AFINIDAD ) {
				if( CONSULTA_AFINIDAD_ON_LINE ) {
					if( config.pais == COLOMBIA
					 || ( ( config.pais == CHILE || config.pais == BRASIL )
					   && USAR_CLI_AFI_PARA_CLIENTES_AFINI ) ) {
						if( RAM_PEDIR_CLIENTE_AFINIDAD == 1 ) {
							OBTENER_DATOS_CLIENTE_AFINIDAD( NO );
						}
						else if( RAM_PEDIR_CLIENTE_AFINIDAD == 2 ) {
							OBTENER_DATOS_CLIENTE_AFINIDAD( SI );
						}
					}
					else {
						LEER_PUNTOS();
					}
				}
			}
			if( RAM_NRO_CLIENTE > 0 ) {
				//int area = SELECTED();
				int tabla_ant = SELECTED2();
				int	tipo_ant = SELECTED_TIPO();
				//MUESTRA_COMPROBANTE( );//1ro muestra cliente para ticket factura
				//MUESTRA_COMPROBANTE( );//2do muestra saldo 
				rta = EXISTE_CLIENTE( RAM_NRO_CLIENTE );
				CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
				//SELECT( area );
				SELECT_TABLE( tabla_ant, tipo_ant );
			}
			//MOSTRAR_CLIENTE_AFINIDAD();
		}
	} else {
        MENSAJE("SALGA DEL MODO DEVOLUCION PARA TRAER UN TICKET EN ESPERA...");
    }

	MUESTRA_PERFIL(  );
	MUESTRA_COMPROBANTE( );
	MOSTRAR_CLIENTE_AFINIDAD();
    MOSTRAR_T_ESPERA_POR_PANTALLA();
    CLOSE_RNV_VIRTUAL();
    OPEN_RNV_VIRTUAL();

    return( rta );
}
/***************************************************************************/
int REVERTIR_DIFERENCIA_PRECIO( long caja_z, long id_evento, int posicion )
/***************************************************************************/
{
    int /*area_ant,*/ tabla_ant, tipo_ant, ok = 0, signo = 0, i = 0;

	struct
	{
		long nro_caja_z;
		long nro_evento;
		int nro_posicion;
	} indice3;
	

    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
	
    //SELECT( AREA_E_TICKET );
	SELECT_TABLE( T_EV_CONT, tipo_t_ev_cont );

	indice3.nro_caja_z = caja_z;
	indice3.nro_evento = id_evento;
	indice3.nro_posicion = posicion;

    GO2( TOP );
	//SET_ORDER( 3 );
    //GET_EQUAL( ( char* )&indice3 );
	SET_WHERE("CAJA_Z='%ld' AND ID_EVENTO='%ld' AND POSICION='%d'", caja_z, id_evento, posicion);
	RUN_QUERY(NO);
	if( FOUND2( ) ) {
		if( RAM_AGREGA_ITEM_TICK_IMPORT == 2 && event_e_ticket.origen == _ORIGEN_DIF_PRECIO ) {
			CARGA_ITEM( RAM_P_TICKET, NULL );
			SET_MEMORY_DOUBLE( __ram_cantidad, event_e_ticket.cantidad );
			SET_MEMORY_DOUBLE( __item_imp_int, ITEM_IMP_INT );
			SET_MEMORY_DOUBLE( __item_costo, ITEM_COSTO );
			SET_MEMORY_DOUBLE( __item_precio_sin_iva, ITEM_PRECIO_SIN_IVA );
			if( event_e_ticket.cod_movimiento == _DIFERENCIA_PRECIO ) {
			   SET_MEMORY_CHAR( __item_cod_movimiento, _DIFERENCIA_PRECIO_ANULADA );
			   SET_MEMORY_INT( __ram_anulacion, 1 );
			} else {
				SET_MEMORY_CHAR( __item_cod_movimiento, _DIFERENCIA_PRECIO );
				SET_MEMORY_INT( __ram_anulacion, 0 );
			}
			if( event_e_ticket.importe_sin_iva ) {
				SET_MEMORY_CHAR( __item_gravado, 1 );
				SET_MEMORY_DOUBLE( __item_iva1_precio, ITEM_IVA1_PRECIO );
				SET_MEMORY_DOUBLE( __item_iva2_precio, ITEM_IVA2_PRECIO );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 0, _ITEM_MONTO_IMPUESTO( 0 ) );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 1, _ITEM_MONTO_IMPUESTO( 1 ) );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 2, _ITEM_MONTO_IMPUESTO( 2 ) );
				_SET_MEMORY_FLOAT( __item_monto_impuesto, 3, _ITEM_MONTO_IMPUESTO( 3 ) );
			} else {
				SET_MEMORY_CHAR( __item_gravado, 0 );
				SET_MEMORY_DOUBLE( __item_iva1_precio, 0 );
				SET_MEMORY_DOUBLE( __item_iva2_precio, 0 );
				for( i = 0;i < 4;i++ ) {
					_SET_MEMORY_FLOAT( __item_monto_impuesto, i, 0 );
				}
			}
			SET_MEMORY_DOUBLE( __item_precio , ITEM_PRECIO );
			SET_MEMORY_DOUBLE( __ram_importe,
				ROUND( RAM_CANTIDAD
                * ( ITEM_PRECIO + ITEM_RECARGO ), ENTEROS,
                DECIMALES ) );
			//SET_MEMORY_DOUBLE( __item_recargo, 0 );
			//SET_MEMORY_DOUBLE( __item_iva1_rec, 0); 
			//SET_MEMORY_DOUBLE( __item_iva2_rec, 0 ); 
			//_SET_MEMORY_FLOAT( __ticket_ila_precio, h, ITEM_ILA_PRECIO );
			//_SET_MEMORY_FLOAT( __ticket_ila_rec, h, ITEM_ILA_REC );
			//_SET_MEMORY_FLOAT( __ticket_iha_precio, h, ITEM_IHA_PRECIO );
			//_SET_MEMORY_FLOAT( __ticket_alicuota_precio, h, ITEM_ALICUOTA_PRECIO );
			//_SET_MEMORY_FLOAT( __ticket_iha_rec, h, ITEM_IHA_REC );
			CR1_AGREGA_ITEM( NO, _ORIGEN_DIF_PRECIO, 0, 0 );
			SET_MEMORY_INT( __ram_agrega_item_tick_import, 2 );
		}
	}
    //SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
    return ( ok );
}

/****************************************************************************/
static int TRAER_TICKET_EN_ESPERA( void )
/****************************************************************************/
{
    char i,ticket;
    int rta, ok = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    struct _cabmenu *cab;
    int columnas;
    #endif
    if( !RAM_P_TICKET ) {
        struct _menu items[5];
        char nombres[5][30];
        int cant_pend = 0;
        char hora[20];
        for( i = 0;i < MAX_CANTIDAD_TICKET_EN_ESPERA;i++ ) {
            if( _GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, i ) ) {
                cant_pend++;
            }
        }
        // Hay Ticket Pendientes
        if( cant_pend ) {
            for( i = -1, ticket = 0;ticket < MAX_CANTIDAD_TICKET_EN_ESPERA;ticket++ ) {
                if( !_GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, ticket ) ) {
                    continue;
                }
                i++;
                // Armar string hora
                HTOAL( _GET_MEMORY_LONG( __ram_hora_mult_ticket_en_espera, ticket ), hora );
                // Armar string menu
                sprintf( ( char* )&nombres[i][0], " %d) %s ", ticket, hora );
                items[i].nom = ( char* )&nombres[i][0];
                items[i].tecla = ticket + 48;   // para obtener el ascii
                // '1',...'n'
                items[i].rta = ticket;
            }
            #if defined(INVEL_W) || defined(INVEL_L)
            columnas = 2;
            cab = ( struct _cabmenu * )calloc( columnas, sizeof( struct _cabmenu ) );
            if( cab != NULL ) {
                cab[0].nombre = "TICKET";
                cab[0].align = ALIGN_CENTER;
                cab[0].width = 8;
                cab[1].nombre = "HORA";
                cab[1].align = ALIGN_LEFT;
                cab[1].width = 20;
                rta = MENU( ST( S_TICKETS_EN_ESPERA ), items, i + 1, cab, columnas );
                //             rta = Menu( ST( S_TICKETS_EN_ESPERA ), items, i + 1, cab, columnas );
                free( cab );
            }
            #else
            rta = MENU( 0, 17, ST( S_TICKETS_EN_ESPERA ), items, i + 1, 120, 52 );
            #endif
            if( rta != -1 ) {
                if( AUTORIZA_TICKET_EN_ESPERA( ST( S_TICKET_ESPERA ) ) ) {
                    _TRAER_TICKET_EN_ESPERA( ( char )rta );
                    CR1_INICIALIZAR_VAR_TICKET( NO );
                    PETICION_DE_CODIGO( ticket_espera_pedir_cod_esp[rta] );
					ok = 1;
                }
            }
        }
        else {
            MENSAJE_STRING( S_No_hay_ticket_en_espera_para_recuperar );
        }
    }

	return( ok );
}
/*****************************************************************************/
int BUSCAR_TICKET_LIBRE( void )
/*****************************************************************************/
{
    char i;

	
	if( CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF ) < MAX_CANTIDAD_TICKET_EN_ESPERA ) {
		for( i = 0;i < MAX_CANTIDAD_TICKET_EN_ESPERA;i++ ) {
			if( !_GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, i ) ) {
				return i;
			}
		}
	}
    return -1;
}
/*****************************************************************************/
int CONTROLAR_TICKET_EN_ESPERA()
/*****************************************************************************/
{
    int i;
    #ifdef COMPILAR_MODO_ENTRENAMIENTO
    if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
        return 1;
    }
    #endif
    if(MODALIDAD_TICKET_EN_ESPERA == 0){ //si es local
        for( i = 0;i < MAX_CANTIDAD_TICKET_EN_ESPERA && _CONTROLAR_TICKET_EN_ESPERA( ( char )i );i++ ) {
            if( _GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, i ) ) {
                _ADD_MEMORY_CHAR( __ram_mult_ticket_en_espera, i, 1 );
            }
        }
        // Incrementa a los pendientes
        for( ;i < MAX_CANTIDAD_TICKET_EN_ESPERA;i++ ) {
            if( _GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, i ) ) {
                _ADD_MEMORY_CHAR( __ram_mult_ticket_en_espera, i, 1 );
            }
        }
    }
    return 1;
}
/*****************************************************************************/
int _CONTROLAR_TICKET_EN_ESPERA( char nro_ticket )
/*****************************************************************************/
{
    if( _GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, nro_ticket ) && PERMITIDOS_DURANTE_ESPERA ) {
        if( _GET_MEMORY_CHAR( __ram_mult_ticket_en_espera, nro_ticket )
         >= PERMITIDOS_DURANTE_ESPERA ) {
            _TRAER_TICKET_EN_ESPERA( nro_ticket );
            return 0;
        }
    }
    return 1;
}
/*****************************************************************************/
static void _TRAER_TICKET_EN_ESPERA( char nro_ticket )
/*****************************************************************************/
{
    RECUPERA_TICKET_EN_ESPERA( nro_ticket );
    IMPRIME_MENSAJE_TRAE_TICKET_EN_ESPERA();
    BORRAR_EVENT_TICKET( DEBE_GRABAR_ALL() );
    SET_MEMORY_CHAR( __ram_empieza_ticket, 0 );
    // Todavia no borro
    CR1_CONFECCIONA_PANTALLA();
    REGENERAR_PANTALLA_TICKET();
    if( !( RAM_FACTURA && !IMPRIMIR_TICKET_SI_ES_REMOTA ) && DEBE_IMPRIMIR_ITEM() ) {
        IMPRIMIR_TICKET_COMPLETO( SI );
    }
}
/*****************************************************************************/
static int GRABA_TICKET_EN_ESPERA( char nro_ticket )
/*****************************************************************************/
{
    int handle, rta = 0, h, i = 0;
    struct _cab_standby cab;
    char file_name[24];
    ticket_espera_pedir_cod_esp[nro_ticket] = OBTENER_COD_ESPECIAL();
    _snprintf(file_name, sizeof(file_name)-1, "standby.%d", nro_ticket );
    handle = _CREAT( file_name, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
    if( handle > 0 ) {
        /*---------------------- Carga la cabecera ---------------------------*/
        cab.nro_cod_cuota = NRO_COD_CUOTA;
        cab.nro_recargo_financiacion = NRO_RECARGO_FINANCIACION;
        cab.nro_mozo = NRO_MOZO;
        cab.ram_p_ticket = RAM_P_TICKET;
        cab.ram_total = RAM_TOTAL;
        cab.ram_item_supervisado = RAM_ITEM_SUPERVISADO;
        cab.ram_bultos = RAM_BULTOS;
        cab.ram_hora_inicio_ticket = RAM_HORA_INICIO_TICKET;
        cab.ram_factura = RAM_FACTURA;
        cab.ram_nro_cliente = RAM_NRO_CLIENTE;
        cab.ram_no_reprocesar = RAM_NO_REPROCESAR;
        cab.ram_id_evento = RAM_ID_EVENTO;
        cab.ram_variar_precio = RAM_VARIAR_PRECIO;
        cab.ram_cliente_afinidad = RAM_CLIENTE_AFINIDAD;
        cab.ram_nota_cr = RAM_NOTA_CR;
        cab.ram_cupon_envases = RAM_CUPON_ENVASES;
        cab.ram_usar_precio_cr = RAM_USAR_PRECIO_CR;
        cab.ram_total_vta_contado = RAM_TOTAL_VTA_CONTADO;
        cab.ram_hora_inicio_evento = RAM_HORA_INICIO_EVENTO;
        cab.ram_id_evento_all = RAM_ID_EVENTO_ALL;
        cab.ram_p_ext_impuestos = RAM_P_EXT_IMPUESTOS;
        if( MODALIDAD_TOMRA ) {
            for( i = 0;i <= 4;i++ ) {
                if( _RAM_NUMERO_TICKET_TOMRA( i ) ) {
                    cab.ram_numero_ticket_tomra[i] = _RAM_NUMERO_TICKET_TOMRA( i );
                }
                else {
                    cab.ram_numero_ticket_tomra[i] = 0;
                }
                _SET_MEMORY_LONG( __ram_numero_ticket_tomra, i, 0 );
                cab.ram_cant_envases[i] = _RAM_CANT_ENVASES( i );
            }
        }
        cab.ram_tarj_importe = RAM_TARJ_IMPORTE;
        cab.ram_tarj_cuotas = RAM_TARJ_CUOTAS;
        cab.modo_devolucion = RAM_MODO_DEVOLUCION;
        cab.ram_puntos_usados = RAM_PUNTOS_USADOS;
        memcpy( cab.ram_tarj_numero, RAM_TARJ_NUMERO, sizeof( cab.ram_tarj_numero ) );
        memcpy( cab.ram_track1, GET_TRACK_1_PRIVADO( ), sizeof( cab.ram_track1 ) );
        memcpy( cab.ram_track2, GET_TRACK_2_PRIVADO( ), sizeof( cab.ram_track2 ) );
        cab.ram_ticket_factura = RAM_TICKET_FACTURA;
        cab.nro_vendedorl = NRO_VENDEDOR_LONG;
        /*---------------------- Graba los datos -----------------------------*/
        _WRITE( handle, ( char* )&cab, sizeof( cab ), __LINE__, __FILE__ );
        GRABAR_STRUCT( handle, _var_tarjetas );
        GRABAR_STRUCT( handle, _mutual );
        GRABAR_STRUCT( handle, _tot_tick );
        GRABAR_STRUCT( handle, _item );
        GRABAR_STRUCT( handle, _ext_impuestos );
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            _GRABAR_STRUCT( handle, _ticket, h );
        }
        _CLOSE( handle, __LINE__, __FILE__ );
        rta = 1;
        _SET_MEMORY_CHAR( __ram_mult_ticket_en_espera, nro_ticket, 0 );
    }
    return ( rta );
}
/*****************************************************************************/
static int RECUPERA_TICKET_EN_ESPERA( char nro_ticket )
/*****************************************************************************/
{
    int handle, rta = 0, h, i = 0;
    struct _cab_standby cab;
    char file_name[32];
    _snprintf(file_name, sizeof(file_name)-1, "standby.%d", nro_ticket );
    handle = _OPEN( file_name, O_RDONLY, __LINE__, __FILE__ );
    if( handle > 0 ) {
        /*---------------------- Carga la cabecera ---------------------------*/
        _READ( handle, ( char* )&cab, sizeof( cab ), __LINE__, __FILE__ );
        
        if( RAM_TICKET_FACTURA != cab.ram_ticket_factura ) {
               MENSAJE_CON_TECLA( "NO SE PUEDE REALIZAR EL CAMBIO DE CLIENTE Presione Enter ",
   					13 );
   				rta = 0;
   	   }else{
        SET_MEMORY_LONG( __nro_vendedor_long, cab.nro_vendedorl );
        SET_MEMORY_INT( __nro_cod_cuota, cab.nro_cod_cuota );
        SET_MEMORY_FLOAT( __nro_recargo_financiacion, cab.nro_recargo_financiacion );
        SET_MEMORY_INT( __nro_mozo, cab.nro_mozo );
        SET_MEMORY_INT( __ram_p_ticket, cab.ram_p_ticket );
        SET_MEMORY_DOUBLE( __ram_total, cab.ram_total );
        SET_MEMORY_INT( __ram_item_supervisado, cab.ram_item_supervisado );
        SET_MEMORY_INT( __ram_bultos, cab.ram_bultos );
        SET_MEMORY_LONG( __ram_hora_inicio_ticket, cab.ram_hora_inicio_ticket );
        SET_MEMORY_CHAR( __ram_factura, cab.ram_factura );
        SET_MEMORY_LONG( __ram_nro_cliente, cab.ram_nro_cliente );
        if( RAM_NRO_CLIENTE > 0 ) {
            //int area = SELECTED();
			int tabla_ant = SELECTED2();
			int	tipo_ant = SELECTED_TIPO();
            rta = EXISTE_CLIENTE( RAM_NRO_CLIENTE );
            CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
            //SELECT( area );
			SELECT_TABLE( tabla_ant, tipo_ant );
        }
        SET_MEMORY_CHAR( __ram_no_reprocesar, cab.ram_no_reprocesar );
        SET_MEMORY_LONG( __ram_id_evento, cab.ram_id_evento );
        SET_MEMORY_CHAR( __ram_variar_precio, cab.ram_variar_precio );
        SET_MEMORY_LONG( __ram_cliente_afinidad, cab.ram_cliente_afinidad );
        SET_MEMORY_CHAR( __ram_nota_cr, cab.ram_nota_cr );
        SET_MEMORY_DOUBLE( __ram_cupon_envases, cab.ram_cupon_envases );
        SET_MEMORY_CHAR( __ram_usar_precio_cr, cab.ram_usar_precio_cr );
        SET_MEMORY_DOUBLE( __ram_total_vta_contado, cab.ram_total_vta_contado );
        SET_MEMORY_INT( __ram_hora_inicio_evento, cab.ram_hora_inicio_evento );
        SET_MEMORY_LONG( __ram_id_evento_all, cab.ram_id_evento_all );
        SET_MEMORY_INT( __ram_p_ext_impuestos, cab.ram_p_ext_impuestos );
        if( MODALIDAD_TOMRA ) {
            i = 0;
            while( cab.ram_numero_ticket_tomra[i] != 0 && ( i <= 4 ) ) {
                _SET_MEMORY_LONG( __ram_numero_ticket_tomra, i, cab.ram_numero_ticket_tomra[i] );
                i++;
            }
            for( i = 0 ;i < 4 ;i++ ) {
                _SET_MEMORY_INT( __ram_cant_envases, i, cab.ram_cant_envases[i] );
            }
        }
        SET_MEMORY_CHAR( __ram_tarj_cuotas, cab.ram_tarj_cuotas );
        SET_MEMORY_DOUBLE( __ram_tarj_importe, cab.ram_tarj_importe );
        SET_MEMORY( __ram_tarj_numero, cab.ram_tarj_numero );
        SET_MEMORY_INT( __ram_modo_devolucion, cab.modo_devolucion );
        SET_MEMORY_DOUBLE( __ram_puntos_usados, cab.ram_puntos_usados );
	    SET_MEMORY_CHAR( __ram_ticket_factura, cab.ram_ticket_factura );
        /*
		estos son remplazados por  los  SET_TRACK_1_PRIVADO y SET_TRACK_2_PRIVADO, los que administrar el tema de privacidad..
		SET_MEMORY( __var_tarjetas_track1, cab.ram_track1 );
		SET_MEMORY( __var_tarjetas_track2, cab.ram_track2 );
		*/
		SET_TRACK_1_PRIVADO( cab.ram_track1 );
        
		SET_TRACK_2_PRIVADO( cab.ram_track2 );

        /*------------------------- Lee el resto -----------------------------*/
        LEER_STRUCT( handle, _var_tarjetas );
        LEER_STRUCT( handle, _mutual );
        LEER_STRUCT( handle, _tot_tick );
        LEER_STRUCT( handle, _item );
        LEER_STRUCT( handle, _ext_impuestos );
        for( h = 0;h < RAM_P_TICKET;h++ ) {
            _LEER_STRUCT( handle, _ticket, h );
            GRABAR_TICKET( h );
        }
        _CLOSE( handle, __LINE__, __FILE__ );
        rta = 1;
        if( MODO_NEGOCIO != SERVICIOS ) {
            if( RAM_NRO_CLIENTE ) {
                //SELECT( AREA_CLIENTES );
                //GET_EQUAL( ( char* )&cab.ram_nro_cliente );
				SELECT_TABLE( T_CLIENTES, TT_ORIG );
				SET_WHERE("COD_CLIENTE = %ld",cab.ram_nro_cliente);
				RUN_QUERY(SI);
            }
        }
        _SET_MEMORY_CHAR( __ram_mult_ticket_en_espera, nro_ticket, 0 );
        ADD_MEMORY_CHAR( __ram_ticket_en_espera, -1 );  // Vuelve
    }
    }
    return ( rta );
}
/*****************************************************************************/
int GRABAR_STRUCT( int handle, int est )
/*****************************************************************************/
{
    return ( _GRABAR_STRUCT( handle, est, -1 ) );
}
/*****************************************************************************/
int _GRABAR_STRUCT( int handle, int est, int subindice )
/*****************************************************************************/
{
    int l_buf;
    char *buffer;
    l_buf = ( subindice < 0 ) ? SIZEOF_STRUCT( est ) : _SIZEOF_STRUCT( est );
    buffer = (char *)MALLOC( l_buf );
    if( buffer ) {
        if( subindice < 0 ) {
            COPY_STRUCT_TO_BUFFER( buffer, est );
        }
        else {
            _COPY_STRUCT_TO_BUFFER( buffer, est, subindice );
        }
        _WRITE( handle, buffer, l_buf, __LINE__, __FILE__ );
        free( buffer );
    }
    return 0;
}
/*****************************************************************************/
int LEER_STRUCT( int handle, int est )
/*****************************************************************************/
{
    return ( _LEER_STRUCT( handle, est, -1 ) );
}
/*****************************************************************************/
int _LEER_STRUCT( int handle, int est, int subindice )
/*****************************************************************************/
{
    int l_buf;
    char *buffer;
    l_buf = ( subindice < 0 ) ? SIZEOF_STRUCT( est ) : _SIZEOF_STRUCT( est );
    buffer = MALLOC( l_buf );
    if( buffer ) {
        _READ( handle, buffer, l_buf, __LINE__, __FILE__ );
        if( subindice < 0 ) {
            COPY_BUFFER_TO_STRUCT( buffer, est );
        }
        else {
            _COPY_BUFFER_TO_STRUCT( buffer, est, subindice );
        }
        free( buffer );
    }
    return 0;
}
/*****************************************************************************/
void IMPRIME_MENSAJE_TICKET_EN_ESPERA( void )
/*****************************************************************************/
{
    /*if( config.pais != COLOMBIA ) {
        if( IMPRESORA_FISCAL == FISCAL_IBM_4610 ) {
            PRN_INHABILITA_AVANCE_JOURNAL();
        }
        else {
            PRN_INHABILITA_AVANCE_RECEIPT();
        }
    }*/
	char cadena[30];

	if( !TKT_CONFIGURABLE ) {
		MOSTRAR_CADENA( 1, PRN, "LE ATENDIO: " );
		MOSTRAR_CADENA( 1, PRN, datos_cajero_actual.nombre );
		LF( 1 );
		MOSTRAR_CADENA( 1, PRN, "CAJERO: " );
		memset(cadena, 0, sizeof(cadena));
		if( IMPRIMIR_LEGAJO_CAJERO ) {
			_snprintf( cadena, sizeof(cadena)-1,"%ld", NRO_LEGAJO_CAJERO );
			MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 8, cadena ), NRO_LEGAJO_CAJERO );
		}
		else {
			_snprintf( cadena, sizeof(cadena)-1,"%d", NRO_CAJERO );
			MOSTRAR_ENTERO( 1, PRN, MASCARA( '0', 4, cadena ), NRO_CAJERO );
		}
		MOSTRAR_CADENA( 1, PRN, "  " );
		MOSTRAR_CADENA( 1, PRN, "CAJA: " );
		//MOSTRAR_ENTERO( 1, PRN, "0000", NRO_CAJA );
		MOSTRAR_ENTERO( 1, PRN, ( USAR_NRO_CAJA_DGI )?
				"0000":"000", NUMERO_CAJA( ) );
		PRN_SELECT( JOURNAL_RECEIPT );
		LF( 1 );
		MOSTRAR_CADENA( 13, PRN, ST( S_TICKET_ESPERA ) );
		LF( 2 );
		MOSTRAR_CADENA( 1, PRN, "NRO. SECUENCIA: " );
		MOSTRAR_ENTERO( 1, PRN, "000000000",NRO_EVENTO);
		LF( 10 );
		if( IMPRESORA_FISCAL ) {
			PRN_SELECT( RECEIPT );
		}
		else {
			PRN_HABILITA_AVANCES();
		}
		PRN_CORTE_TOTAL();
	} else {
		IMPRIMIR_TKT_TICKET_ESPERA( );
	}
}
/*****************************************************************************/
void IMPRIME_MENSAJE_TRAE_TICKET_EN_ESPERA( void )
/*****************************************************************************/
{
    PRN_INHABILITA_AVANCE_RECEIPT();
    LF( 1 );
    LF( 1 );
    MOSTRAR_CADENA( 1, PRN, ST( S_RECUPERA_TICKET_EN_ESPERA ) );
    LF( 1 );
    LF( 1 );
    if( IMPRESORA_FISCAL ) {
        PRN_SELECT( RECEIPT );
    }
    else {
        PRN_HABILITA_AVANCES();
    }
}
/*****************************************************************************/
int AUTORIZA_TICKET_EN_ESPERA( char *titulo  )
/*****************************************************************************/
{
    int rta = 1;
    rta = SOLICITAR_CLAVES( 30, 20, NO, SI, PEDIR_SUPERVISOR_TICKET_EN_ESPER,
                            titulo/*ST( S_AUTORIZACION_DE_TICKET_EN_ESPERA )*/, ST( S_TICKET_ESPERA ),
                            RAM_COD_FUNCION, SI );
    return ( rta );
}
/*****************************************************************************/
void MOSTRAR_MONEDERO_POR_PANTALLA()
/*****************************************************************************/
{
    if( strlen( RAM_CUENTA_CLIENTE ) ) {
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_MONEDERO_AGREGAR, " ", " ", NULL, " " );
    }
    else{
        ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_MONEDERO_BORRAR, " ", " ", NULL, " " );
    }
}
/*****************************************************************************/
void MOSTRAR_T_ESPERA_POR_PANTALLA()
/*****************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
	int cant = 0;
	GRABAR_LOG_SISTEMA("##MOSTRAR_T_ESPERA_POR_PANTALLA",LOG_DEBUG,4);
	if( MODALIDAD_TICKET_EN_ESPERA == 0 ) {/*LOCAL*/
		if( RAM_TICKET_EN_ESPERA >= 1 ) {
			ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_VTAS_AGREGAR, " ", " ", NULL, " " );
		}
		else if( RAM_TICKET_EN_ESPERA == 0 ) {
			ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_VTAS_BORRAR, " ", " ", NULL, " " );
			SET_MEMORY_INT( __ram_ticket_en_espera, 0 );
		}
	} else {/*REMOTO*/
		/*if( CONTAR_TICKET_ESPERA( 30, NRO_CAJA, NRO_Z, -1 ) >= 1 ) {
			ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_REM_VTAS_AGREGAR, " ", " ", NULL, " " );
			} else*/ {
			/*El TE puede estar off y no tener un evento asociado*/
			/*Se considera solo los TE OFF porque si hay un TE ON, no se sabe si fue procesado
			  o no por otra caja cuando esta la caja esta OFFLINE*/
			//if( !GET_DBR_ON_LINE( ) ) { //sin dbrouter esto no tiene sentido
 			cant = CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF );
			SET_MEMORY_INT( __ram_ticket_en_espera, cant ); //agregado para modo remoto
			//}
			if( cant ) {
				ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_REM_VTAS_AGREGAR, " ", " ", NULL, " " );
			} else {
				/*El TE puede que ya se ha levantado desde la misma caja o desde otra*/
				ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_REM_VTAS_BORRAR, " ", " ", NULL, " " );
			}
		}
	}
    #else
    if( RAM_TICKET_EN_ESPERA == 0 ) {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 70, 23, "         " );
    }
    else if( RAM_TICKET_EN_ESPERA >= 1 ) {
        _ATRIBUTO = 15;
        MOSTRAR_CADENA( 70, 23, "T. ESPERA" );
    } 
    else {
        _ATRIBUTO = 7;
        MOSTRAR_CADENA( 70, 23, "         " );
        SET_MEMORY_INT( __ram_ticket_en_espera, 0 );
    }
    #endif
}

/*****************************************************************************/
void PROCESAR_TICKET_EN_ESPERA( int modo )
/*****************************************************************************/
{ 
	int rta = -1;
	int online = 0, unico = 0, enviar = 1, i = 0;
	struct _datos_cobro_kinacu dck;
	int posicionTransaccionKinacu = 0;

	if( RAM_P_TICKET ){
		if( RAM_HAY_COBRO_VENTA ){
			COBROS_VENTA_EN_TICKET( &online, &unico );
			if( online >= 0 ){
				if( MENSAJE_CON_TECLA_S_N( 
					ST( S_COBROS_ONLINE_TICKET_DESEA_CANCELARLOS_ENTER_ESC ),
						13, 27 ) == 13 ){
					/*---------- Cancelo el item ---------*/
					while( RECUPERAR_ARCHIVO_REC( ARCH_REC_COBROS_KINACU,(char *)&dck, 
							sizeof( struct _datos_cobro_kinacu ), posicionTransaccionKinacu ) == 0 ) {
						if( dck.estado != 1 ) {//estado = 1 -> cancelar
							SET_MEMORY_INT( __ram_anulacion, 1 );
							SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );
							AGREGAR_ARTICULO_COBRO_VENTA( _TICKET_COD_BARRA( dck.pos_ticket ), _TICKET_COD_ESPECIAL( dck.pos_ticket ), 
							_TICKET_PRECIO( dck.pos_ticket ), _TICKET_COD_COBRO( dck.pos_ticket ) );
						}
							posicionTransaccionKinacu ++;
					}
					if(IMPRESORA_FISCAL == FISCAL) {
						IMPRIME_ITEM_FISCAL( ITEM_NOMBRE_LARGO, (ITEM_PRECIO + ITEM_IMP_INT), ITEM_PRECIO,((int) -RAM_CANTIDAD ) );//Seba
					}
					SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
					SET_MEMORY_INT( __ram_anulacion, 0 );
				}else{
					enviar = 0;
				}
			} 
			//Verificamos que haya por lo menos un art. distintos de cobro para enviar a espera
			if( enviar ) {
				enviar = 0;
				for( i = 0; i < RAM_P_TICKET && !enviar; i++ ) {
					if( !( _TICKET_COD_COBRO( i ) ) ) { 
						enviar = 1 ;
					}
				}
				if( !enviar ){
					MENSAJE("NO HAY ART. PARA ENVIAR TICKET EN ESPERA");
					BORRAR_MENSAJE();
				}
			}
		}
		if( enviar ) {
			if( !VERIFICA_ANULACIONES_AUTORIZADAS() ) {
				MENSAJE_CON_PAUSA( BUSCA_MENSAJE_STRING( S_DEBE_AUTORIZAR_LAS_ANULACIONES ), 18 );
			} else {
				rta = TICKET_EN_ESPERA( modo );
				if( rta ){
					SET_MEMORY_CHAR( __ram_hay_cobro_venta, 0 );
					BORRAR_ARCHIVOS_COBROS_VENTA( NO);
				}else{
					MENSAJE_STRING( S_ERROR_AL_ENVIAR_TICKET_EN_ESPERA );
				}
			}
		}
	} else {
		struct _eventos evento;
		//DATOS PREVIOS DE TICKET.. POR SI APRIETAN ESC...
		int t_aux_nota_cr         = RAM_NOTA_CR        ;
		int t_aux_referencia_nc   = RAM_REFERENCIA_NC  ;
		int t_aux_factura         = RAM_FACTURA        ;
		int t_aux_ticket_factura  = RAM_TICKET_FACTURA ;
		int t_aux_remito          = NRO_REMITO         ;
		int t_aux_nro_cliente     = RAM_NRO_CLIENTE    ;

		SET_MEMORY_INT( __ram_flag_cobro_venta, 1 );

		
		rta = TICKET_EN_ESPERA( modo );
		
		SET_MEMORY_INT( __ram_flag_cobro_venta, 0 );
		if( rta && COBROS_VENTA_EN_TICKET( &online, &unico ) ){
			RECUPERAR_ARCHIVO_REC(ARCH_REC_IMPORT,(char *)&evento,sizeof( struct _eventos),0);
			if( !TRAER_COBROS_VENTA_ESPERA( evento.caja_z,evento.id_evento, ( evento.tipo_evento_orig == 21 ) ? 1 : 0 ) ){
				MENSAJE_STRING( S_ERROR_AL_TRAER_COBROS_VENTA_EN_ESPERA );
				rta = -1;
			} else {
				SET_MEMORY_CHAR( __ram_hay_cobro_venta, 1 );
			}
		} else
			if( !rta ) {
			//MENSAJE( "ERROR AL IMPORTAR TICKET EN ESPERA" );
            GRABAR_LOG_SISTEMA("PROCESAR_TICKET_EN_ESPERA: SE TERMINA LA OPERACION (ESC o ERROR)",LOG_VENTAS,3);
			//desactivo..por el tema de refresco de pantalla..deberia quedar todo igual pero por esc no retoma bien los datos
			SET_MEMORY_CHAR(__ram_nota_cr        ,0        );  
			SET_MEMORY_CHAR(__ram_referencia_nc  ,0);  
			SET_MEMORY_CHAR(__ram_factura        ,0);  
			SET_MEMORY_CHAR(__ram_ticket_factura ,0);  
			SET_MEMORY_LONG(__nro_remito         ,0);  
			MUESTRA_COMPROBANTE( );
			MOSTRAR_CLIENTE_AFINIDAD();
			//recargo los datos previos...
			SET_MEMORY_CHAR(__ram_nota_cr        ,t_aux_nota_cr         );  
			SET_MEMORY_CHAR(__ram_referencia_nc  ,t_aux_referencia_nc   );  
			SET_MEMORY_CHAR(__ram_factura        ,t_aux_factura         );  
			SET_MEMORY_CHAR(__ram_ticket_factura ,t_aux_ticket_factura  );  
			SET_MEMORY_LONG(__nro_remito         ,t_aux_remito          );  
			if( t_aux_nro_cliente > 0 && EXISTE_CLIENTE( t_aux_nro_cliente )) {
				SET_MEMORY_LONG(__ram_nro_cliente    ,t_aux_nro_cliente     );  
				CARGAR_PERFIL_CLIENTE( ( int )clientes.cod_perfil );
			}
			MUESTRA_COMPROBANTE( );
			MOSTRAR_CLIENTE_AFINIDAD();
		}
	}

	switch ( rta ) {
	case 0:
		// Ticket NO enviado NI recibido de espera.
		break;
	case 1:
		// Ticket enviado o recibido de espera CORRECTAMENTE.			
		PROCESAR_PROMOCIONES_EN_TICKET_EN_ESPERA( );
		break;
	default:
		break;
	} //fin del switch
}

