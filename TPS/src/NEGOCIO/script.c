#pragma pack(1)
#include <tpv_dat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <archivos.h>
#include <boletin.h>
#include <btrv.h>
#include <b_mens.h>
#include <carga.h>
#include <clientes.h>
#include <cmemory.h>
#include <cr.h>
#include <cr_disco.h>
#include <dbrouter.h>
#include <get_fech.h>
#include <get_hora.h>
#include <lista.h>
#include <log.h>
#include <mensaje.h>
#include <mstring.h>
#include <pant.h>
#include <print.h>
#include <promo.h>
#include <retiros.h>
#include <scanner.h>
#include <script.h>
#include <socios.h>
#include <tkt.h>
#include <t_pago.h>
#include <uti-comp.h>
#include <zip.h>
#include <_cr0.h>
#include <_cr1.h>
#include <dd.h>
#include <import_t.h>
#include <deftables.h>
#include <db.h>
#include <cio.h>
#include <tcp_ip.h>
#include <cmemory.h>
#include <cadafech.h>
#include <ini.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <string.h>
#include <aarch.h>
#undef MK_FP
#ifdef INVEL_L
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/time.h>
#else
#include <dos.h>
#include <sys/stat.h>
#endif
#include <actions.h>
#include <zip.h>
#include <path.h>
#include <myodbc.h>
#include <typeacti.h>

#define TAMANO_BUFFER_RTA 3072

struct _script
{
	BINARY status;			//0
	INT16 nro_caja;			//1
	unsigned long id;		//3
	INT16 cod_comando;      //7
	INT16 cod_estado;		//9
	char param[56];			//11
	UINT16 fecha_comando;   //67
	UINT16 hora_comando;    //69
	UINT16 fecha_estado;    //71
	UINT16 hora_estado;     //73
    BINARY cod_funcion;     //75
	BINARY reintentos;      //76
	BINARY fin;		//77
};
#define CANT_COL_SCRIPT     12

struct _respuesta_script {
 //El campo STATUS se usa para marcar que la respuesta fue enviada por el Jsincro.
 //Son los mismos codigos de estado que en eventos (0 pendiente, 1 enviado, 2 en uso). 
 BINARY status;			//0 
 INT16 nro_caja;		//1
 unsigned long id;      //3
 INT16 cod_comando;     //7
 char param[56];		//9
 INT16 cod_rta;			//65
 char reenviado;		//67
 INT16 fecha;			//68
 INT16 hora;			//70
 //char reserva[25];
 //char buffer[TAMANO_BUFFER_RTA];
 char buffer[3072];      //72
 BINARY fin;
};

/* COMANDOS DEL SCRIPT
 * ---------------------------------------------------------------------
 * case  1: *--- Copiar Archivos
 * case  2: *--- Agregar Altas
 * case  3: *--- Agregar Modificaciones
 * case  4: *--- Rebootear PC
 * case  5: *--- Borrar Memoria
 * case  6: *--- Copiar Listas
 * case  7: *--- Ejecutar programa
 * case  8: *--- Padron completo de Articulos
 * case  9: *--- Habilitar PC
 * case 10: *--- Retiros Remotos
 * case 11: *--- Emitir z_remoto
 * case 12: *--- Pide estructura de la rnv
 * case 20: *--- Insertar Tabla en DBRouter
 * case 21: *--- Obtener Archivo cabecera y pie de ticket desde DBR
 * case 22: *--- Obtener Archivo de configuracion config.dat desde dbr
 * case 23: *--- De Testeo - Devuelve siempre Error
 * case 24: *--- Obtener Tabla de DBRouter  TABLA ARCHIVO WHERE
 * case 25: *--- PUT archivo FTP al Server
 * case 26: *--- GET archivo FTP a la caja
 * case 27: *--- Novedades de Clientes
 * case 28: *--- Padron completo de Clientes
 * case 29: *--- PUT archivo FTP al Server ASCII
 * case 30: *--- GET archivo FTP a la caja ASCII
 * case 31: *--- Padron completo de Articulos por FTP.
 * case 32: *--- Novedades de Boletines t_compra y mutuales
 * case 33: *--- Padron completo de Boletines t_compra y mutuales
 * case 34: *--- Estado de articulos y modificaciones
 * case 35: *--- Envio de eventos off-line
 * case 36: *--- Padron completo de Clientes por FTP.
 * case 40: *--- Completo de Deudas por FTP
 * case 41: *--- Padron completo de Socios comprimido por FTP.
 * case 42: *--- Completo de Deudas comprimidas por FTP.
 * case 43: *--- Novedades de Deudas.
 */
//extern int       ACTUALIZA_CLIENTES( void );
int procesando_script = 0;
int _CANCELA_MODIFICACIONES = NO;
extern char _ALTAS_INICIALES_CANCELADA;
extern char _MODIFICACIONES_INICALES_CANCELADA;
extern int ACTUALIZA_BOLETINES_TCOMPRA_MUTUALES( int *registros );
int EJECUTA_COMANDO_SCRIPT( struct _script *script, int inicial,
                            struct _respuesta_script *script_rta );
int PROCESAR_VARIABLES_DD( char *buffer, int cambiar_memoria );
int BORRAMEM( char *param );

/***********************************************************************/
int PROCESAR_SCRIPT( int inicial )
/***********************************************************************/
{
    int seguir = 1, error = 0;
    struct _script script;
    struct _respuesta_script script_rta;
//    char codicion_select[64];
    if( !procesando_script ) {
      	int tabla_ant=0, tipo_ant=0;

      	tabla_ant = SELECTED2();
	    tipo_ant = SELECTED_TIPO();
		memset( &script, 0, sizeof( struct _script ) );
        procesando_script = 1;
        if( SCRIPT_RESPUESTA ) {
			error = OPEN_TABLE( T_R_SCRIPT, TT_ORIG, ( char* )&script_rta, sizeof( struct _respuesta_script ) );
            if( error ) {
                GRABAR_LOG_SISTEMA( BUSCA_MENSAJE_STRING( S_ERROR_ABRIR_SCRIPT_RTA ),LOG_VENTAS,2 );
            }
        }
		//para evitar cambio de script, lo primero que la caja hace es marcar el primer script como en uso por el pos
		DIRECT_DB_QUERY("UPDATE script SET CODESTADO=-2 where CodCaja = %i and codestado in(0,-2) limit 1",(short)NRO_CAJA);

        if( !error && OPEN_TABLE(T_SCRIPT, TT_ORIG,( char* )&script, sizeof( script )) == 0 ) {
            int reg = 0;
			SELECT_TABLE( T_SCRIPT, TT_ORIG );
			//busca el script que este marcado como en uso por el pos
			SET_WHERE("CodCaja = '%i' and codestado = -2",(short)NRO_CAJA);
			RUN_QUERY(NO);
            reg = RECCOUNT2(NULL,NULL);
            while( reg && script.nro_caja == NRO_CAJA && !dbEOF() && seguir ) {
                GRABAR_LOG_DBROUTER( "Ejecutar script" ,LOG_DEBUG,1);
                if( script.cod_estado == -2 ) {
                    seguir = EJECUTA_COMANDO_SCRIPT( &script, inicial, &script_rta );
                }
                else {
                    SKIP2( 1 );
                }
            }
			CLOSE_TABLE( T_SCRIPT, TT_ORIG );
        }
		CLOSE_TABLE( T_R_SCRIPT, TT_ORIG );
        procesando_script = 0;

        /*if( !inicial && !TOMANDO_FTP() ) {
			if( _ALTAS_INICIALES_CANCELADA ) {
                if( !( AGREGAR_ALTAS() == 1 ) ) {
                    _ALTAS_INICIALES_CANCELADA = 0;
                }
            }
            if( !_ALTAS_INICIALES_CANCELADA && _MODIFICACIONES_INICALES_CANCELADA ) {
                if( !( AGREGAR_MODIFICACIONES() == 1 ) ) {
                    _MODIFICACIONES_INICALES_CANCELADA = 0;
                }
            }
        }*/
    	SELECT_TABLE( tabla_ant, tipo_ant );
    }
    return 1;
}
/***********************************************************************/
int EJECUTA_COMANDO_SCRIPT( struct _script *script, int inicial,
                            struct _respuesta_script *script_rta )
/***********************************************************************/
{
    char s1[80],s2[80],fecha1[20],fecha2[20],*p,where[90];
    int error = 0, ejecutar_boot = 0, n, rta=0, continua = 0, registros;
    int grabar_rta = 0, final = 0, rta_z, cant, recargar = 0;
    long l_variable = 0;
    unsigned dia_operativo_servidor,proximo_dia_operativo_servidor;
    int procesar = SI, cod_script;
    char tabla[50],tablaTmp[50],tablaTmpGz[50];
    char tabla2[50],tablaTmp2[50];
    #ifdef INVEL_L
    #define BOLETIN "boletin.ctr"
    #define BOL_NOV "bol_nov.ctr"
    #define SOCIOS "socios.ctr"
    #define CLIENTES "clientes.ctr"
    #define CLI_NOV "cli_nov.ctr"
    #define ARTICULO "articulo.ctr"
    #define PRESENT "present.ctr"
    #define OP_CLI_NOV "opcl_nov.ctr"
    #else
    #define BOLETIN "boletin.sic"
    #define BOL_NOV "bol_nov.sic"
    #define SOCIOS "socios.sic"
    #define CLIENTES "clientes.sic"
    #define CLI_NOV "cli_nov.sic"
    #define ARTICULO "articulo.sic"
    #define PRESENT "present.sic"
    #define OP_CLI_NOV "opcl_nov.sic"
    #endif
    // log
    sprintf( where, "Ejecuta Comando Script %d (Params %s)", script->cod_comando, script->param );
    GRABAR_LOG_SISTEMA( where ,LOG_VENTAS,1);
    memset( script_rta, 0, sizeof( struct _respuesta_script ) );
    cod_script = script->cod_comando;
    switch( script->cod_comando ) {
        case 1:
            /*--- Copiar Archivos ---*/
            if( sscanf( script->param, "%s %s", s1, s2 ) == 2 ) {
                error = COPIAR_ARCHIVO( s1, s2, __LINE__, __FILE__ );
            }
            else {
                error = _SCRIPT_ERROR_EN_PARAMETROS;
            }
            break;
		//Codigo Obsoleto porque ya no usamos dbrouter
        /*case 2:
            //--- Agregar Altas ---
            if( MODO_NEGOCIO != SERVICIOS ) {
                if( !inicial ) {
                    _ALTAS_INICIALES_CANCELADA = 0;
                    if( AGREGAR_ALTAS() == 1 ) {
                        procesar = NO;
                        _CANCELA_MODIFICACIONES = SI;
                    }
                    else {
                        _CANCELA_MODIFICACIONES = NO;
                    }
                }
            }
            break;*/
		//Codigo Obsoleto porque ya no usamos dbrouter
        /*case 3:
            //--- Agregar Modificaciones ---
			if( MODO_NEGOCIO != SERVICIOS ) {
                if( !inicial && !_CANCELA_MODIFICACIONES ) {
                    int rta_mod;

					_MODIFICACIONES_INICALES_CANCELADA = 0;
                    rta_mod = AGREGAR_MODIFICACIONES(  );
					if( rta_mod == 1 ) {
                        procesar = NO;
					} else if( rta_mod == -1 ) {
						error = 1; //marco como error porque al obtener la tabla modi_art fallo algo.
						//si no se setea esto se borra el script y parece que todo resulto OK.
                    }
                }
                else {
                    procesar = NO;            // Solucion
                }
            }
            break;*/
        case 4:
            /*--- Rebootear PC ---*/
            ejecutar_boot = 1;
            break;
        case 5:
            /*--- Borrar Memoria ---*/
            error = BORRAMEM( script->param );
            break;
        case 6:
            /*--- Copiar Listas ---*/
            if( sscanf( script->param, "%i", &n ) == 1 && n >= 1 && n <= 30 ) {
                MENSAJE_SIN_SONIDO_STRING( S_CARGANDO_BOLETINES_NUEVOS_ESPERE_UN_MOMENTO );
                FIN_BUSCA_TARJETA();
                sprintf( s1, "M:LISTA1.%03i", n );
                sprintf( s2, "C:LISTA1.%03i", n );
                error = COPIAR_ARCHIVO( s1, s2, __LINE__, __FILE__ );
                sprintf( s1, "M:LISTA2.%03i", n );
                sprintf( s2, "C:LISTA2.%03i", n );
                error += 100 * COPIAR_ARCHIVO( s1, s2, __LINE__, __FILE__ );
                INICIALIZAR_BUSCA_TARJETA();
            }
            else {
                error = _SCRIPT_ERROR_EN_PARAMETROS;
            }
            break;
		case 2:
            //*--- Agregar Altas ---*/
		case 3:
			//*--- Agregar Modificaciones
        case 8:
            //*--- Padron completo de Articulos

			if( MODO_NEGOCIO != SERVICIOS ) {
				if( script->cod_comando == 2 ) {
					MENSAJE_STRING_SIN_PAUSA( S_TOMANDO_ALTAS );
				} else {
					if( script->cod_comando == 3 ) 
					{
						MENSAJE_STRING_SIN_PAUSA( S_TOMANDO_MODIFICACIONES );
					}
					else 
					{
						MENSAJE_SIN_SONIDO_STRING( S_TOMANDO_PADRON_DE_ARTICULOS );
					}	
				}
				CIERRA_ARTICULOS();
				//{
					//char trace[128];
					//clock_t start,end;
					//start = clock();
				error = TOMAR_TABLA_SVR("articulo");
				if( !error ) {
					//SET_MEMORY_LONG( __nro_ultima_alta_procesada, 0 );
					//SET_MEMORY_LONG( __nro_ultima_modif_procesada, 0 );
					if( PRESENTACIONES_HABILITADAS ) {
						if( !TOMAR_TABLA_SVR("articulo_pr") ) {
							GRABAR_LOG_ID_DBROUTER( SDBR_TABLA_OBTENIDA_ERROR , LOG_COMUNICACIONES,3);
						}
					}
				} //else {
					//error = 1;
				//}
					//end = clock();
					//sprintf( trace, "PADRON: %.2f", ( end - start ) / CLK_TCK );
					//GRABAR_LOG_DBROUTER( trace );
				//}
				BORRAR_MENSAJE();
				ABRE_ARTICULOS();
			}
            break;
        case 9:
            /*----- Habilitar PC -----*/
            SET_MEMORY_CHAR( __nro_modo, 0 );
            break;
        case 10:
            /*---- Retiros Remotos -----*/
            // TOLEDO (OK): Realizar el retiro Remoto si esta habilitado
            if( RETIRO_REMOTO_HABILITADO && !inicial ) {
                error = RETIRO_REMOTO( script->param );
            }
            break;
        case 11:
            /*---- emitir z_remoto -------*/
            if( !inicial ) {
                if( SCRIPT_RESPUESTA && PERMITE_Z_REMOTA ) {
                    grabar_rta = 1;
                    cant = sscanf( script->param, "%d %d", &dia_operativo_servidor,
                                   &proximo_dia_operativo_servidor );
                    if( cant > 0 ) {
                        if( dia_operativo_servidor < NRO_DIA_OPERATIVO ) {
                            script_rta->cod_rta = Z_DUPLICADA;
                            grabar_rta = 0;
                            memset( fecha1, 0, sizeof( fecha1 ) );
                            memset( fecha2, 0, sizeof( fecha2 ) );
                            DTOC( NRO_DIA_OPERATIVO, fecha1 );
                            DTOC( dia_operativo_servidor, fecha2 );
                            GRABAR_LOG_SISTEMA( "Z duplicada Remota" ,LOG_VENTAS,2);
                            sprintf( s1, "Dia operativo servidor: %s   (%u)", fecha2,
                                     dia_operativo_servidor );
                            GRABAR_LOG_SISTEMA( s1 ,LOG_VENTAS,2);
                            sprintf( s1, "Dia operativo POS     : %s   (%u)", fecha1,
                                     NRO_DIA_OPERATIVO );
                            GRABAR_LOG_SISTEMA( s1 ,LOG_VENTAS,2);
                        }
                        else {
                            script_rta->cod_rta = _SCRIPT_RTA_PROCESANDO_Z;
                        }
                    }
                    else {
                        script_rta->cod_rta = _SCRIPT_RTA_PROCESANDO_Z;
                    }
                    if( cant > 1 ) {
                        SET_MEMORY_INT( __nro_proximo_dia_operativo_servidor,
                                        proximo_dia_operativo_servidor );
                    }
                    memcpy( script_rta->param, script->param, 56 );
                    script_rta->id = script->id;
                    script_rta->cod_comando = script->cod_comando;
                    script_rta->nro_caja = NRO_CAJA;
                    script_rta->fecha = _GET_FECHA_DOS();
                    script_rta->hora = GET_HORA_DOS();
                    {
                        int rta = 0;
                        struct script_rta_en_dbrouter rta_dbr;
                        rta_dbr.nro_caja = script_rta->nro_caja;
                        rta_dbr.id = script_rta->id;
                        rta_dbr.cod_comando = script_rta->cod_comando;
                        memcpy( rta_dbr.param, script_rta->param, 56 );
                        rta_dbr.cod_rta = script_rta->cod_rta;
                        rta_dbr.reenviado = script_rta->reenviado;
                        rta = INSERT_EN_SERVER( "respuesta_script", ( char* )&rta_dbr,
                                               sizeof( struct script_rta_en_dbrouter ), 1 );
                        {
                            char debug[100];
                            sprintf( debug, "respuesta %d comando %d resp %d", rta,
                                     rta_dbr.cod_comando, rta_dbr.cod_rta );
                            GRABAR_LOG_DBROUTER( debug ,LOG_DEBUG,1);
                        }
                    }
                    if( grabar_rta ) {
                        rta_z = CR0_MODO_Z( 0 );
                        {
                            char debug[100];
                            sprintf( debug, "Modo Z: ret %d", rta_z );
                            GRABAR_LOG_DBROUTER( debug ,LOG_DEBUG,1);
                        }
                        switch( rta_z ) {
                            case 1:
                                script_rta->cod_rta = _SCRIPT_RTA_OK;
                                final = 1;
                                break;
                            case -1:
                                script_rta->cod_rta = _SCRIPT_RTA_Z_DUPLICADA;
                                break;
                            case -2:
                                script_rta->cod_rta = _SCRIPT_RTA_IMPRESORA_OFF_LINE;
                                break;
                            default:
                                script_rta->cod_rta = _SCRIPT_RTA_CAJA_OPERANDO;
                        }
                    }
                    //cerrar caja
				}
                else {
                    script_rta->cod_rta = _SCRIPT_RTA_MAL_CONFIGURADO;
                }
            }
            else {
                procesar = NO;
            }
            break;
		case 12: /*---- pide estructura de la rnv -------*/
			    //ahora solo sirve para simular un ping a la caja en el cierre remoto
			    error = 0;
				procesar = SI;
			break;
        case 15:
            /* Cambiar valores del diccionario */
            error = PROCESAR_VARIABLES_DD( script->param, NO );
            break;
        case 17:
            /* Cambiar valores del diccionario y en memoria */
            error = PROCESAR_VARIABLES_DD( script->param, SI );
            break;
        case 20:
            /*---- Insertar Tabla en DBRouter -------*/
            break;
        case 22:
            //*--- Obtener Archivo de configuracion config.dat desde dbr
			error = TOMAR_TABLA_SVR("TPVCONFIG");
			/*switch ( script->cod_funcion ) {
				case 5: //codigo de funcion = 5 es para insertar en mysql
					if(!OBTENER_TABLA_SERVIDOR(T_TPVCONFIG, TT_ORIG, "TPVCONFIG", NULL, SI, NULL)){
						error = 1;
						MENSAJE_STRING( S_ERROR_TABLA_NO_OBTENIDA );
					}
					break;
			}*/
			if(!error) {
				int pant_activa = 0;
				CARGAR_CONFIG( );
				//esto lo agrego para que actualice cuando llega un script 
				ExecuteSAction( A_GET_PANEL_ACTIVO, ( char* )&pant_activa );
				if( NRO_HUBO_OPERACIONES == 0 && pant_activa == PANTALLA_PRESENTACION  && config.dia_operativo > NRO_DIA_OPERATIVO){
					CR0_TOMAR_DIA_OPERATIVO(SI);
					MUESTRA_DIA_OPERATIV0();
				}
			}
            break;
        case 23:
            //*--- De Testeo - Devuelve siempre Error
            error = _SCRIPT_ERROR_INTERNO;
            break;
		case 24:
            /*---- Obtener Tabla de DBRouter  TABLA ARCHIVO WHERE ----*/
			{
				
				int reabrir = 0;
				if( ( p = strchr( script->param, ' ' ) ) != NULL ) {
					*p = 0;
					strcpy( s1, script->param );    // TABLA = S1
					*p = ' ';
					reabrir = 1;
					if( strstr( ( char* )_strupr( s1 ), "RANGOS" ) ) {
						CLOSE_TABLE( T_RANGOS, TT_ORIG );
					}
					error = TOMAR_TABLA_SVR(s1);

					if( !error&& strstr( ( char* )_strupr( s1 ), "MEDIOS_TAR" ) ) {
						recargar |= _RECARGAR_MEDIOS_TAR;
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "TARJETAS" ) ) {
						recargar |= _RECARGAR_TARJETAS;
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "RANGOS" ) ) {
						if( !DBMEM_LOAD_SQL( _RANGOS_SIC ,T_RANGOS, TT_ORIG,"COD_DESDE" ) ) {
							ABORT( ST( S_ERROR_AL_CARGAR_RANGOS ) );
						}
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "COBROS" )
						&& MODO_NEGOCIO == SERVICIOS ) {
						recargar |= _RECARGAR_COBROS;
						if( reabrir  ) {
							OPEN_TABLE(T_COBROS, TT_ORIG,( char* )&cobros, sizeof( cobros ));
						}
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "MEDIOS" ) ) {
						recargar |= _RECARGAR_MEDIOS;
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "DEPARTAMENTOS" ) ) {
						recargar |= _RECARGAR_DEPARTAMENTOS;
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "ARTICULO_IVA" ) ) {
						recargar |= _RECARGAR_ARTICULO_IVA;
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "IMPUESTOS" ) ) {
						recargar |= _RECARGAR_IMPUESTOS;
					}
					if( !error && strstr( ( char* )_strupr( s1 ), "TENVASES" ) ) {
						recargar |= _RECARGAR_TENVASES;
					}
					//Las promociones la maneja el Motor de promociones
					/*if( !error && strstr( ( char* )_strupr( s1 ), "PROMO" ) ) {
						recargar |= _RECARGAR_PROMO;
					}*/
					BORRAR_MENSAJE();
				}else {
					error = _SCRIPT_ERROR_EN_PARAMETROS; 
				}
			}

            break;
        case 26:
            if( ( p = strchr( script->param, ' ' ) ) != NULL ) {
                int rta2 = 0;
                *p = '\0';
                // Origen
                strcpy( s1, script->param );
                // Destino
                strcpy( s2, p + 1 );
                //Se vuelve a poner el espacio para no alterar la tabla Scrip
                *p = ' ';
                rta2 = TOMAR_FTP_SEGUNDO_PLANO( s1, s2,BINARIO,script->id, 1 );
                if( rta2 == 1 ) {
                    error = 0;
                }
                else if(rta2 == -1){
                    sprintf( s1, "Error FTP: %i", 1 );
                    GRABAR_LOG_SISTEMA( s1 ,LOG_COMUNICACIONES,3);
                    error = 1;
                }else{
                    procesar = NO;
                }
            }
            else {
                error = 1;
            }
            break;
		//Codigo Obsoleto porque ya no usamos dbrouter
        /*case 27:
            //--- Novedades de clientes
            if( MODO_NEGOCIO != SERVICIOS ) {
                    char where[100];

                    MENSAJE_SIN_SONIDO_STRING( S_ACTUALIZANDO_CLIENTES );
                    sprintf( where, " nov_id > %d ORDER BY nov_id", NRO_ULTIMA_NOVEDAD_CLIENTE );
					error = ( !OBTENER_TABLA_SERVIDOR( T_CLIENTES_NOV, TT_ORIG, "clientes_nov", where, SI, NULL ) ) ? 0 : 1;
                    if( !error ) {
                        error = ACTUALIZA_CLIENTES();
                    }
                    BORRAR_MENSAJE();
                    SELECT_TABLE( T_SCRIPT, TT_ORIG );
                    GRABAR_LOG_DBROUTER( "Fin novedades" );
            }
            break;*/
		case 27:
			//*--- Novedades de clientes
		case 28:
            //*--- Padron completo de Clientes

			if( MODO_NEGOCIO != SERVICIOS ) {
				if( script->cod_comando == 27 ) {
					MENSAJE_SIN_SONIDO_STRING( S_ACTUALIZANDO_CLIENTES );
				} else {
					MENSAJE_SIN_SONIDO_STRING( S_OBTENIENDO_COMPLETO_DE_CLIENTES );
				}
                CLOSE_TABLE( T_CLIENTES, TT_ORIG );
				error = TOMAR_TABLA_SVR("clientes");
				/*if( !error ) {
                    SET_MEMORY_LONG( __nro_ultima_novedad_cliente, 0 );
                }*/
                ABRIR_ARCHIVO_CLIENTE();
				BORRAR_MENSAJE();
            }
            break;
        case 30:
            if( ( p = strchr( script->param, ' ' ) ) != NULL ) {
                int rta2 = 0;
                *p = 0;
                // Origen
                strcpy( s1, script->param );
                // Destino
                strcpy( s2, p + 1 );
                rta2 = TOMAR_FTP_SEGUNDO_PLANO( s1, s2,ASCII,script->id, 1);
                if( rta2 == 1 ) {
                    error = 0;
                }
                else if(rta2 == -1){
                    sprintf( s1, "Error FTP: %i", 1 );
                    GRABAR_LOG_SISTEMA( s1 , LOG_COMUNICACIONES,3 );
                    error = 1;
                }else{
                    procesar = NO;
                }
            }
            else {
                error = 1;
            }
            break;
		case 31://padron completo de articulos por FTP en segundo plano, con cod_funcion == 7 indica que esta comprimido
        {
			int rta2 = 0;
			char nombre_tabla_base[35];
			char path_mysql[50], path_mysql_base[50];
			char pedido_base[40];

			memset(path_mysql,0, sizeof(path_mysql));
			memset(path_mysql_base,0, sizeof(path_mysql));

			strncpy(path_mysql,PATH_MYSQL,sizeof(PATH_MYSQL)); 
			if(config_tps.PadronesPorFTP == 1)
				strncpy(path_mysql_base,config_tps.RutaFtp,sizeof(config_tps.RutaFtp)); 
			else
				strncpy(path_mysql_base,PATH_MYSQL,sizeof(PATH_MYSQL)); 

			
			
		

			path_mysql[49]=0;
			path_mysql_base[49]=0;
			
			if( inicial ) 
			{
         		procesar = NO;
				break;
			}
			memset(nombre_tabla_base,0,sizeof(nombre_tabla_base));
			memset(pedido_base,0,sizeof(pedido_base));
			memset(tablaTmpGz,0,sizeof(tablaTmpGz));
			//busco nombre de la tabla
			GET_TABLE_NAME( T_ARTICULO, TT_ORIG, nombre_tabla_base, sizeof( nombre_tabla_base ) );
			//si es compactado no hace falta el temporal. aunque se podria usar el xxx_tmp.myftp
			if( script->cod_funcion == 7 )
				//articulo.MYD - Convierto en minuscula el nombre_tabla_base
				sprintf( tablaTmp, "%s%s.MYD", PATH_MYSQL,_strlwr(nombre_tabla_base) );
			else {
				//articulo_tmp.MYD
				sprintf( tablaTmp, "%s%s_tmp.MYD", PATH_MYSQL,_strlwr(nombre_tabla_base) );
			}
			//ARTICULO.MYD - tabla se pide con mayusculas. nombre en el ftp
						
			sprintf(pedido_base,"%s%s.MYD", path_mysql_base,_strupr(nombre_tabla_base));

			if( script->cod_funcion == 7 )
			{
         		//agregamos extension si es cod funcion 7 = comprimido
				strcat(tablaTmp,".gz");
         		strcat(pedido_base,".gz");
			}
			//pedido de la tabla de datos
			//"ARTICULO.MYD","articulo_tmp.MYD"
			MENSAJE_SIN_SONIDO_STRING( S_TOMANDO_PADRON_DE_ARTICULOS );
			rta2 = TOMAR_FTP_SEGUNDO_PLANO( pedido_base, tablaTmp,BINARIO,script->id , 1 );
			if( rta2 == 1 )
			{
				rta2 = 2;// por defecto debe esperar... asi vuelve a entrar
				memset( tablaTmp2,0,sizeof( tablaTmp2 ));
				memset(pedido_base,0,sizeof(pedido_base));
				//si es compactado no hace falta el temporal. aunque se podria usar el xxx_tmp.myftp
				if( script->cod_funcion == 7 )
					//tablas locales en minusculas
					sprintf( tablaTmp2, "%s%s.MYI", PATH_MYSQL,_strlwr(nombre_tabla_base) );
				else {
					//tablas locales en minusculas
					sprintf( tablaTmp2, "%s%s_tmp.MYI", PATH_MYSQL,_strlwr(nombre_tabla_base) );
				}

				//tablas servidor en MAYUSCULAS
			
				sprintf(pedido_base,"%s%s.MYI", path_mysql_base, _strupr(nombre_tabla_base));

				//pedido del indice de la tabla de datos
				if( script->cod_funcion == 7 )
				{
					strcat(tablaTmp2,".gz");
	     			strcat(pedido_base,".gz");
		 		}
				MENSAJE_SIN_SONIDO_STRING( S_TOMANDO_PADRON_DE_ARTICULOS );
				rta2 = TOMAR_FTP_SEGUNDO_PLANO( pedido_base, tablaTmp2,BINARIO,script->id , 2 );
			}

			if( rta2 == 1 ) 
			{//si ya tenemos todas las tablas....
				char copia_tabla_bak[40];
				int tiene =0;

				GRABAR_LOG_DBROUTER( "Borrando y renombrado tablas" ,LOG_DEBUG,2);
				CIERRA_ARTICULOS();
				memset( pedido_base, 0, sizeof( pedido_base ));
				memset( copia_tabla_bak, 0, sizeof( copia_tabla_bak ));

				//"articulo.MYD"
				sprintf(pedido_base,"%s%s.MYD", PATH_MYSQL,_strlwr(nombre_tabla_base));

				//RESGUARDO			"articulo.DBAK"
				sprintf(copia_tabla_bak,"%s%s.DBAK", PATH_MYSQL,_strlwr(nombre_tabla_base));

				//BORRO EL ANTERIOR SI EXISTE
				_unlink( copia_tabla_bak);

				//RENOMBRO "articulo.MYD, "A articulo.DBAK
				rename( pedido_base,copia_tabla_bak );

				//DESCOMPRIMO O
				if( script->cod_funcion == 7 )
				{
					//sprintf(tablaTmpGz,"%s%s.MYD.gz", PATH_MYSQL,_strlwr(nombre_tabla_base));
					error = !DESCOMPRIMIR( tablaTmp );
				}
				else
					//RENOMBRO AL ORIGINAL
					rename( tablaTmp, pedido_base );


				memset(pedido_base,0,sizeof(pedido_base));

				//tablas locales en minusculas
				sprintf(pedido_base,"%s%s.MYI", PATH_MYSQL,_strlwr(nombre_tabla_base));

				//RESGUARDO
				memset(copia_tabla_bak,0,sizeof(copia_tabla_bak));
				sprintf(copia_tabla_bak,"%s%s.IBAK", PATH_MYSQL,_strlwr(nombre_tabla_base));

				_unlink( copia_tabla_bak);

				rename( pedido_base ,copia_tabla_bak );

				//BORRO ORIGINAL
				//_unlink( pedido_base);

				//RENOMBRO
				if( script->cod_funcion == 7 )
				{
					//sprintf(tablaTmpGz,"%s%s.MYI.gz", PATH_MYSQL,_strlwr(nombre_tabla_base));
					//descomprimo"ARTICULO.MYI.GZ"
					error = !DESCOMPRIMIR( tablaTmp2 );
					//QUEDO EN "articulo.MYI"
				}
				else
					rename( tablaTmp2, pedido_base );

				//prueba de apertura y contenido de registros
				ABRE_ARTICULOS();
				tiene = RECCOUNT2( NULL, NULL );
				if( error || ABRE_ARTICULOS() > 0 || RECCOUNT2( NULL, NULL ) == 0 ) 
				{
					error = 1;

					CIERRA_ARTICULOS();

					memset(pedido_base,0,sizeof(pedido_base));
					memset(copia_tabla_bak,0,sizeof(copia_tabla_bak));

					sprintf(pedido_base,"%s%s.MYD", PATH_MYSQL,_strlwr(nombre_tabla_base));
					sprintf(copia_tabla_bak,"%s%s.DBAK", PATH_MYSQL,_strlwr(nombre_tabla_base));

					_unlink( pedido_base);
					rename( copia_tabla_bak, pedido_base  );

					memset(pedido_base,0,sizeof(pedido_base));
					memset(copia_tabla_bak,0,sizeof(copia_tabla_bak));
					sprintf(pedido_base,"%s%s.MYI", PATH_MYSQL,_strlwr(nombre_tabla_base));

					sprintf(copia_tabla_bak,"%s%s.IBAK", PATH_MYSQL,_strlwr(nombre_tabla_base));

					_unlink( pedido_base);
					rename( copia_tabla_bak, pedido_base  );


					ABRE_ARTICULOS();
				} 
				else 
				{
					error = 0;
					if( PRESENTACIONES_HABILITADAS ) 
					{
						MENSAJE_SIN_SONIDO( "Obteniendo Completo de Presentaciones", NO );
						CLOSE_TABLE( T_PRESENT, TT_ORIG );
      					if( !TOMAR_TABLA_SVR("articulo_pr") ) 
						{
							GRABAR_LOG_ID_DBROUTER( SDBR_TABLA_OBTENIDA_ERROR , LOG_COMUNICACIONES,3);
						}
						error = ABRIR_ARCHIVO_PRESENTACIONES(  );
						if( error )
						{
							GRABAR_LOG_DBROUTER( "Error al tomar abrir PRESENTACIONES" ,LOG_ERRORES,2);
						}
					}
				}
			} else
				if( rta2 == -1 ) 
				{
					memset(pedido_base,0,sizeof(pedido_base));
					sprintf(pedido_base,"Error FTP de %s: %i",nombre_tabla_base, error);
					GRABAR_LOG_SISTEMA( pedido_base , LOG_COMUNICACIONES,3 );
					GRABAR_LOG_DBROUTER( pedido_base , LOG_COMUNICACIONES,4 );
					GRABAR_LOG_DBROUTER( "..recuerde que es case sensitive..chequee los nombres" , LOG_COMUNICACIONES,3 );
					error = 1;
				}
				else
				{
					procesar = NO;
				}
				BORRAR_MENSAJE();
				break;
		}
		case 32:
            //*--- Novedades de Boletines t_compra y mutuales
            {
                char where[100];
                MENSAJE_SIN_SONIDO_STRING( S_ACTUALIZANDO_BOLETINES_PRES_ESC_PARA_ANULAR );
                do {
                    //sprintf( where, " WHERE id > %li AND id <= %li ORDER BY id",
					sprintf( where, " id > %li AND id <= %li ORDER BY id",
                             NRO_ULTIMA_NOVEDAD_BOLETIN,
                             NRO_ULTIMA_NOVEDAD_BOLETIN + NOVEDADES_POR_VEZ );
                    error = ( !OBTENER_TABLA_SERVIDOR( T_BOLETIN_NOV, TT_ORIG, "boletin_novedades", where, SI, NULL ) ) ? 0 : 1;
                    if( !error ) {
                        error = ACTUALIZA_BOLETINES_TCOMPRA_MUTUALES( &registros );
                        continua = ( !error && ( registros == NOVEDADES_POR_VEZ ) ) ? SI : NO;
                        if( error == 99 ) {
                            error = 0;
                            procesar = NO;
                            continua = NO;
                        }
                    }
                }
                while( continua && !error );
                BORRAR_MENSAJE();
                SELECT_TABLE( T_SCRIPT, TT_ORIG );
                GRABAR_LOG_DBROUTER( "Fin novedades",LOG_VENTAS,2 );
            }
            break;
        case 33:
            //*--- Padron completo de Boletines t_compra y mutuales
            MENSAJE_SIN_SONIDO_STRING( S_OBTENIENDO_COMPLETO_DE_BOLETINES );
            //CLOSE_DATABASE( AREA_BOLETIN );
			CLOSE_TABLE( T_BOLETIN, TT_ORIG );
            error = ( !OBTENER_TABLA_SERVIDOR( T_BOLETIN, TT_ORIG, "boletin_protectivo", NULL, SI, NULL ) ) ? 0 : 1;
            if( !error ) {
                SET_MEMORY_LONG( __nro_ultima_novedad_boletin, 0 );
            }
            ABRIR_ARCHIVO_BOLETIN_TCOMPRA_MUTUALES();
            BORRAR_MENSAJE();
            break;
		case 36:{
			int rta2 = 0;
			char path_mysql[50];
			if(config_tps.PadronesPorFTP == 1)
				strncpy(path_mysql,config_tps.RutaFtp,sizeof(config_tps.RutaFtp)); 
			else
				strncpy(path_mysql,PATH_MYSQL,sizeof(PATH_MYSQL)); 

            /*--- Padron completo de Clientes por FTP */
            if( MODO_NEGOCIO != SERVICIOS ) {
                CLOSE_TABLE( T_CLIENTES, TT_ORIG );
	            /*Archivos de datos*/
	            sprintf( tablaTmp, "%scli_tmp.MYD", PATH_MYSQL );
	            sprintf( tabla, "%sclientes.MYD", PATH_MYSQL );
	            /*Archivos de indices*/
	            sprintf( tablaTmp2, "%scli_tmp.MYI", PATH_MYSQL );
	            sprintf( tabla2, "%sclientes.MYI", PATH_MYSQL );
	            rta2 = TOMAR_FTP_SEGUNDO_PLANO( tabla, tablaTmp, BINARIO, script->id, 1);
	            if( rta2 == 1 ) {
	                rta2 = TOMAR_FTP_SEGUNDO_PLANO( tabla2, tablaTmp2, BINARIO, script->id, 2 );
	            }
                if( rta2 == 1) {
					int rta =0 ;
					 CLOSE_TABLE( T_CLIENTES, TT_ORIG );
                    rta =_unlink( tabla );
					if( rta != 0) {
						sprintf( s1, "Error unlink CLIENTES %i %i", rta, errno );
						GRABAR_LOG_SISTEMA( s1 , LOG_ERRORES,3 );
					} 
					rta =0;
                    rename( tablaTmp, tabla );
					if( rta != 0) {
						sprintf( s1, "Error renameCLIENTES %i %i", rta, errno );
						GRABAR_LOG_SISTEMA( s1 , LOG_ERRORES,3 );
					} 
                   rta= _unlink( tabla2 );
					if( rta != 0) {
						sprintf( s1, "Error unlink2 CLIENTES %i %i", rta, errno );
						GRABAR_LOG_SISTEMA( s1 , LOG_ERRORES,3 );
					} 
                    rta=rename( tablaTmp2, tabla2 );
					if( rta != 0) {
						sprintf( s1, "Error rename2CLIENTES %i %i", rta, errno );
						GRABAR_LOG_SISTEMA( s1 , LOG_ERRORES,3 );
					} 
					
					
					ABRIR_ARCHIVO_CLIENTE();
                }
                else if( rta2 == -1  ) {
                    sprintf( s1, "Error FTP de clientes: %i", error );
                    GRABAR_LOG_SISTEMA( s1 , LOG_COMUNICACIONES,3 );
                    GRABAR_LOG_DBROUTER( s1 , LOG_COMUNICACIONES,4 );
	                GRABAR_LOG_DBROUTER( "..recuerde que es case sensitive..chequee los nombres" , LOG_COMUNICACIONES,4 );
				}else{
					procesar = NO;
				}
                BORRAR_MENSAJE();
                ABRIR_ARCHIVO_CLIENTE();
            }
            break;
			}
        case 43:
            /*CONTROLA SI TIENE TICKET EN ESPERA*/
            //if( CONTAR_TICKET_ESPERA(30,NRO_CAJA,NRO_Z,-1) >= 1 ) {
			if( CONTAR_TICK_ESP_SIN_PROCESAR( 0, ON_OFF ) >= 1 ) {
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_REM_VTAS_AGREGAR, " ", " ", NULL, " " );
            }
            else{
                ExecuteSSAction( A_MUESTRA_FLAGS, T_FLAGS_TKE_REM_VTAS_BORRAR, " ", " ", NULL, " " );
            }
            break;

		default:
			error = _SCRIPT_ERROR_SCRIPT_NO_RECONOCIDO;
    }
    /*---------------- Elimina registro o graba error -------------------*/
        if( procesar ) {
            rta = 1;
            SELECT_TABLE( T_SCRIPT, TT_ORIG );
            if( error ) {
                script->cod_estado = error;
                script->fecha_estado = GET_FECHA_DOS();
                script->hora_estado = GET_HORA_DOS();
                SELECT_TABLE( T_SCRIPT, TT_ORIG );
                UPDATE2();
                SKIP2( 1 );
            }else { //si no hay error, elimina el script
                SELECT_TABLE( T_SCRIPT, TT_ORIG );
                DELETE2();
                if( grabar_rta ) {
                    {
                        struct script_rta_en_dbrouter rta_dbr;
                        int rta = 0;
                        rta_dbr.nro_caja = script_rta->nro_caja;
                        rta_dbr.id = script_rta->id;
                        rta_dbr.cod_comando = script_rta->cod_comando;
                        memcpy( rta_dbr.param, script_rta->param, 56 );
                        rta_dbr.cod_rta = script_rta->cod_rta;
                        rta_dbr.reenviado = script_rta->reenviado;
                        rta = INSERT_EN_SERVER( "respuesta_script", ( char* )&rta_dbr,
                                               sizeof( struct script_rta_en_dbrouter ), 1 );
                        {
                            char debug[100];
                            sprintf( debug, "resp %d com %d resp %d", rta, rta_dbr.cod_comando,
                                     rta_dbr.cod_rta );
                            GRABAR_LOG_DBROUTER( debug ,LOG_DEBUG,4);
                        }
                    }
                }
            }
        }
        else {
            SELECT_TABLE( T_SCRIPT, TT_ORIG );
            SKIP2( 1 );
        }
    if( final ) {
        CR0_FINAL( ver_posvisual );
        exit(0);
    }
    /*-------------------- Si hay que bootear la pc ---------------------*/
    if( ejecutar_boot ) {
		CLOSE_TABLE( T_SCRIPT, TT_ORIG );
		CLOSE_TABLE( T_R_SCRIPT, TT_ORIG );
        BOOT();
    } 
	if( recargar & _RECARGAR_MEDIOS_TAR ) {
		CARGAR_CONFIGURACION_TAR_MED( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
	if( recargar & _RECARGAR_TARJETAS ) {
		CARGAR_TARJETAS( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
    if( recargar & _RECARGAR_MEDIOS ) {
        CARGAR_MEDIOS_DE_PAGO( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
    if( recargar & _RECARGAR_COBROS ) {
        CARGAR_COBROS_ESPECIALES( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
	if( recargar & _RECARGAR_ARTICULO_IVA ) {
		CARGAR_IVA( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
	if( recargar & _RECARGAR_DEPARTAMENTOS ) {
		CARGAR_DPTOS( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
	if( recargar & _RECARGAR_IMPUESTOS ) {
		CARGAR_IMPUESTOS( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
	if( recargar & _RECARGAR_TENVASES ) {
		CARGAR_TIPOS_DE_ENVASES( );
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }
	//Las promociones la maneja el Motor de promociones
	/*if( recargar & _RECARGAR_PROMO ) {
        SELECT_TABLE( T_SCRIPT, TT_ORIG );
    }*/

    return ( rta );
}
/***************************************************************************/
int BORRAMEM( char *param )
/***************************************************************************/
{
    char s[5][20];
    int c, h, i, no_procesados = 0;
    c = sscanf( param, "%s %s %s %s %s", s[0], s[1], s[2], s[3], s[4] );
    for( h = 0;h < c;h++ ) {
        if( strcmp( ( char* )_strupr( s[h] ), "/GT" ) == 0 ) {
            SET_MEMORY_DOUBLE( __nro_gran_total, 0.0 );
        }
        else if( strcmp( ( char* )_strupr( s[h] ), "/DO" ) == 0 ) {
            SET_MEMORY_CHAR( __nro_z_emitida, 1 );
            SET_MEMORY_INT( __nro_dia_operativo, 0 );
        }
        else if( strcmp( ( char* )_strupr( s[h] ), "/RAM" ) == 0 ) {
            // CEREAR_STRUCT( _ram );
        }
        else if( strcmp( ( char* )_strupr( s[h] ), "/CUP" ) == 0 ) {
            for( i = 0;i < 20;i++ ) {
                // _SET_MEMORY_LONG( __nro_cupones, i, 0 );
            }
        }
        else {
            no_procesados++;
        }
    }
    return ( no_procesados );
}
/***************************************************************************/
int PROCESAR_VARIABLES_DD( char *buffer, int cambiar_memoria )
/***************************************************************************/
{
    char *pos, *pos2 = NULL, *valor, nombre[36];
    int error = 0, i;
    int val_entero;
    long val_long;
    float val_float;
    int permite_cambio_rnv = 0;
    char men[200];
    if( /*USE_DATABASE( AREA_AUX, "dd_var.sic", ( char* )&dd_var, sizeof( dd_var ), NULL, 0, 0 ) */
		OPEN_TABLE(DD_VAR, TT_ORIG ,(char*)&dd_var, sizeof(struct _dd_var ))== 0 ) {
        //SET_ORDER( 4 );
		SET_ORDER2("NOM_ESTRUCTURA");
        memset( men, 0, sizeof( men ) );
        strcpy( men, buffer );
        _strupr( men );
        pos = strchr( men, '[' );
        if( memcmp( men, "RNV", 3 ) == 0 ) {
            permite_cambio_rnv = 1;
        }
        if( pos ) {
            pos2 = strchr( pos, ']' );
        }
        if( pos && pos2 ) {
            *pos2 = 0;
            strncpy( nombre, ( char* )&pos[1], sizeof( nombre ) );
            if( strlen( nombre ) < 35 ) {
                for( i = strlen( nombre );i < 35;i++ ) {
                    nombre[i] = ' ';
                }
                nombre[35] = 0;
            }
            //GET_EQUAL( ( char* )&nombre );
			SET_WHERE("NOM_ESTRUCTURA = '%s'", nombre);
			RUN_QUERY(NO);
            if( FOUND2() ) {
                if( dd_var.clase != 1 || permite_cambio_rnv ) {
                    valor = &pos2[1];
                    switch( dd_var.tipo_variable ) {
                        case _SHORT_INT:
                            val_entero = _tomar_entero( &valor );
                            *( int* )dd_var.valor_inicial = val_entero;
							UPDATE2();
                            if( cambiar_memoria ) {
                                SET_MEMORY_CHAR( dd_var.cod_variable, val_entero );
                            }
                            break;
                        case _INT:
                        case _UNSIGNED:
                        case _HORA:
                            val_entero = _tomar_entero( &valor );
                            *( int* )dd_var.valor_inicial = val_entero;
							UPDATE2();
                            if( cambiar_memoria ) {
                                SET_MEMORY_INT( dd_var.cod_variable, val_entero );
                            }
                            break;
                        case _LONG:
                            val_long = _tomar_long( &valor );
                            *( long* )dd_var.valor_inicial = val_long;
                            if( cambiar_memoria ) {
                                SET_MEMORY_LONG( dd_var.cod_variable, val_long );
                            }
							UPDATE2();
                            break;
                        case _FLOAT:
                            val_float = _tomar_float( &valor );
                            *( float* )dd_var.valor_inicial = val_float;
							UPDATE2();
                            if( cambiar_memoria ) {
                                SET_MEMORY_FLOAT( dd_var.cod_variable, val_float );
                            }
                            break;
                        case _DOUBLE:
                            val_float = _tomar_float( &valor );
                            *( double* )dd_var.valor_inicial = val_float;
							UPDATE2();
                            if( cambiar_memoria ) {
                                SET_MEMORY_DOUBLE( dd_var.cod_variable, val_float );
                            }
                            break;
                        case _STRING:
                            _tomar_cadena( &valor );
                            strncpy( ( char* )&dd_var.valor_inicial, valor,
                                     sizeof( dd_var.valor_inicial ) );
							UPDATE2();
                            if( cambiar_memoria ) {
                                SET_MEMORY( dd_var.cod_variable, valor );
                            }
                            break;
                        case _DATE:
                            val_entero = _tomar_fecha( &valor );
                            *( int* )dd_var.valor_inicial = val_entero;
							UPDATE2();
                            if( cambiar_memoria ) {
                                SET_MEMORY_INT( dd_var.cod_variable, val_entero );
                            }
                            break;
                        default:
                            {
                                char auxi[100];

                                sprintf( auxi, "tipo %d", dd_var.tipo_variable );
                                MENSAJE( auxi );
                            }
                            error = 1;
                            break;
                    }
                }
                else {
                    error = 2;
                }
            }
            else {
                error = 3;
            }
        }
        else {
            error = 4;
        }
		CLOSE_TABLE(DD_VAR, TT_ORIG);
    }
    else {
        error = 5;
    }
    SELECT_TABLE( T_SCRIPT, TT_ORIG );
    return ( error );
}


int TOMAR_TABLA_SVR(char *pTableName){
	char mensaje[128];  // debug
	int error = 0;

	_snprintf(mensaje, sizeof(mensaje)-1,ST( S_OBTENIENDO_TABLA__S ), pTableName );
	MENSAJE_SIN_SONIDO( mensaje,NO );
	GRABAR_LOG_BD(mensaje,NO , LOG_COMUNICACIONES,3 );
	memset( mensaje, 0, sizeof( mensaje ) );

	//si existe la tabla svr (tabla grabada por el servidor)
	if( DIRECT_DB_QUERY("SHOW FIELDS FROM %s_svr",pTableName ) > 0 ) {
		// Elimina la old en caso de que ya exista
		DIRECT_DB_QUERY( "DROP TABLE IF EXISTS `%s_old`", pTableName );
		// renombra la tabla original a old por si hay un corte de energia
		if( !DIRECT_DB_QUERY( "ALTER TABLE `%s` RENAME `%s_old`", pTableName, pTableName ) ) {
			// renombra la tabla grabada por el servidor (tabla_svr) como original
			if( !DIRECT_DB_QUERY( "ALTER TABLE `%s_svr` RENAME `%s`", pTableName, pTableName ) ) {
				// elimina la old
				DIRECT_DB_QUERY( "DROP TABLE IF EXISTS `%s_old`", pTableName );
			} else {
				_snprintf(mensaje, sizeof(mensaje)-1,"RENAME table %s_old` a `%s`", pTableName, pTableName );
				GRABAR_LOG_BD(mensaje,NO , LOG_COMUNICACIONES,4 );
				// renombra la tabla old a original 
				DIRECT_DB_QUERY( "ALTER TABLE `%s_old` RENAME `%s`", pTableName, pTableName );
				memset( mensaje, 0, sizeof( mensaje ) );
				_snprintf(mensaje, sizeof(mensaje)-1,"DROP si existe table %s_svr", pTableName );
				GRABAR_LOG_BD(mensaje,NO , LOG_COMUNICACIONES,4 );
				DIRECT_DB_QUERY( "DROP TABLE IF EXISTS `%s_svr`", pTableName );
				error = _SCRIPT_ERROR_OBTENER_TABLA;
			}
		} else {
			memset( mensaje, 0, sizeof( mensaje ) );
			_snprintf(mensaje, sizeof(mensaje)-1,"DROP si existe table %s_svr", pTableName );
			GRABAR_LOG_BD(mensaje,NO , LOG_COMUNICACIONES,4 );
			DIRECT_DB_QUERY( "DROP TABLE IF EXISTS `%s_svr`", pTableName );
			error = _SCRIPT_ERROR_OBTENER_TABLA;
		}
	}else{
		error = _SCRIPT_ERROR_NO_EXISTE_TABLA_SVR;
		_snprintf(mensaje, sizeof(mensaje)-1,"Error NO Existe tabla %s_svr", pTableName );
		GRABAR_LOG_BD(mensaje,NO , LOG_COMUNICACIONES,4 );
	}
	return error;
}
