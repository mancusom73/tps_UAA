#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <archivos.h> 
#include <btrv.h> 
#include <b_mens.h> 
#include <carga.h>
#include <cerear.h> 
#include <cio.h> 
#include <cmemory.h> 
#include <comprob.h> 
#include <cr.h>  
#include <cr_disco.h> 
#include <dbmem.h> 
#include <dbrouter.h> 
#include <eventos.h> 
#include <export.h> 
#include <getch.h> 
#include <log.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <netcom.h> 
#include <netcom2.h> 
#include <print.h> 
#include <rnv.h> 
#include <round.h> 
#include <tcp_ip.h> 
#include <tkt.h> 
#include <trim.h> 
#include <ventas.h> 
#include <ventas2.h> 
#include <_cr1.h> 
#include <db.h>
#include <deftables.h>
#include <math.h>
#include "ini.h"
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#define _CR_DISCO_C
#undef _CR_DISCO_C
#undef MK_FP
#include <memory.h>
#include <string.h>
#include <aarch.h>
#include <actions.h>
#include <myodbc.h>
#ifdef INVEL_W
#include <io.h>
#include <dos.h>
#else
#include <sys/io.h>
#endif
/*--------------------------- Otras variables --------------------------*/
static char verificar_inhabilitado = 1;
static int area_articulo;
struct _indice_cod_int_cod_barra
{
    long cod_interno;
    char cod_barra[17];
};
int archivo_cliente_abierto = 0;
int archivo_boletin_abierto = 0;
int archivo_clicobro_abierto = 0;
double precio_item;
char _ALTAS_INICIALES_CANCELADA = 1;//0;
char _MODIFICACIONES_INICALES_CANCELADA = 1;//0;
extern int GET_FUNCION_TEC( int k);
extern char imprimiendo_factura_journal;
extern int DEBE_PEDIR_PRECIO_DEVOL_Y_NC( int operacion );
int Modificar_Alicuota = 0;
int ABRIR_ARCHIVO_PRESENTACIONES(  );
/***********************************************************************/
int INICIALIZA_ARCHIVOS( int inicial )
/***********************************************************************/
{
    int error = 0, rta = -1;
    /*---------- Verifica copia de seguridad de articulos -------*/
    if( access( "ARTICULO.BAK", 0 ) == 0 ) {
        _unlink( "ARTICULO.SIC" );
        rename( "ARTICULO.BAK", "ARTICULO.SIC" );
    }
    /*----------- Abre los archivos -------------*/
    error = ABRE_ARTICULOS();
    error += ABRIR_VTA();
	//Se comenta provisoriamente hasta que quede claro para que se usa
    //error += ABRIR_TRANSAC();
    if( inicial && !CARGAR_IMPUESTOS() ) {
        error = 1;
    }
    if( inicial && !error && !CARGAR_IVA() ) {
        error = 1;
    }

	/*if( MODO_NEGOCIO != SERVICIOS && !error ) {
        //if( !error ) {
        if( AGREGAR_ALTAS() != 1 ) {
            _ALTAS_INICIALES_CANCELADA = 0;
            if( AGREGAR_MODIFICACIONES() == 1 ) {
                // Modificaciones Inicial Cancelada
                _MODIFICACIONES_INICALES_CANCELADA = 1;
            }
            else {
                _MODIFICACIONES_INICALES_CANCELADA = 0;
            }
        }
        else {
            // Altas Inicial Cancelada
            _ALTAS_INICIALES_CANCELADA = 1;
        }
        rta = 0;
    }*/
    if( ( FACTURACION_HABILITADA || PEDIR_CLIENTE_EN_CHEQUES ) && MODO_NEGOCIO != SERVICIOS ) {
        ABRIR_ARCHIVO_CLIENTE();
    }
    ABRIR_ARCHIVOS_EVENTOS();
    if( MODO_NEGOCIO == SERVICIOS ) {
        ABRIR_ARCHIVO_SOCIO();
    }
    if( MODO_NEGOCIO != SERVICIOS ) {
        if( inicial ) {
			error += DBMEM_LOAD_SQL( _CLASIF1_SIC, T_CLASIF1,  TT_ORIG, NULL);
            CALCULA_FACTOR_DESPLAZAMIENTO();
        }
    }
    if( PROTOCOLO_AUTORIZACION_TARJETA == _TCP_IP ) {
        T_ABRIR_TRANSAC2();
		if(config_tps.NapseModalidad ==1)
			T_ABRIR_NAPSE();
    }
    if( _HUBO_CLOSE_ALL ) {
        _HUBO_CLOSE_ALL = NO;
        ABRIR_STRING_TABLE();
    }
    return ( rta );
}
/***********************************************************************/
int ABRIR_ARCHIVO_CLIENTE()
/***********************************************************************/
{
        if( !OPENED( T_CLIENTES, TT_ORIG )) {
            if( OPEN_TABLE(T_CLIENTES, TT_ORIG ,(char*)&clientes,sizeof( struct _clientes ))== 0 ) {
                archivo_cliente_abierto = 1;
            }
            else {
                archivo_cliente_abierto = 0;
            }
        }
        else {
            archivo_cliente_abierto = 1;
        }
    return archivo_cliente_abierto;
}
/***********************************************************************/
void INICIALIZA_ARCHIVOS_RECONECCION()
/***********************************************************************/
{
    archivo_cliente_abierto = 0;
    archivo_clicobro_abierto = 0;
    CLOSE_ALL();
    BTRIEVE_RESET();
    INICIALIZA_ARCHIVOS( NO );
}
/****************************************************************************/
int ABRE_ARTICULOS()
/****************************************************************************/
{
    int error = 0;
    area_articulo = AREA_ARTICULOS;
    /*if( !USED( AREA_ARTICULOS ) ) {*/
        error = /*USE_DB( AREA_ARTICULOS, _ARTICULO_SIC, ( char* )&articulo, sizeof( articulo ), NULL,
                        0, 0 );*/
						OPEN_TABLE(T_ARTICULO, TT_ORIG ,( char* )&articulo,sizeof( articulo ));
    //}
    /*if( !error && UTILIZAR_ARTICULOS_REMOTO && !USED( AREA_ARTICULOS_REMOTO ) ) {
        if( USE_DB( AREA_ARTICULOS_REMOTO, _ARTICULO_SIC_REMOTO, ( char* )&articulo,
                    sizeof( articulo ), NULL, 0, 0 ) == 0 ) {
            area_articulo = AREA_ARTICULOS_REMOTO;
        }
    }*/
	error = ABRIR_ARCHIVO_PRESENTACIONES(  );

    return ( error );
}
/****************************************************************************/
void CIERRA_ARTICULOS()
/****************************************************************************/
{
    //CLOSE_DATABASE( AREA_ARTICULOS );
	CLOSE_TABLE( T_ARTICULO, TT_ORIG );
    /*if( UTILIZAR_ARTICULOS_REMOTO /*&& USED( AREA_ARTICULOS_REMOTO ) ) {
        CLOSE_DATABASE( AREA_ARTICULOS_REMOTO );
    }*/
    if( PRESENTACIONES_HABILITADAS && OPENED( T_PRESENT, TT_ORIG ) ) {
        //CLOSE_DATABASE( AREA_PRESENT );
		CLOSE_TABLE( T_PRESENT, TT_ORIG );
    }
}
/****************************************************************************/
int GET_AREA_ARTICULOS()
/****************************************************************************/
{
    return ( area_articulo );
}
/***********************************************************************/
void CIERRA_ARCHIVOS()
/***********************************************************************/
{
    if( ( FACTURACION_HABILITADA || PEDIR_CLIENTE_EN_CHEQUES ) && MODO_NEGOCIO != SERVICIOS ) {
        //CLOSE_DATABASE( AREA_CLIENTES );
		CLOSE_TABLE( T_CLIENTES, TT_ORIG );
    }
    if( MODO_NEGOCIO == SERVICIOS ) {
        //CLOSE_DATABASE( AREA_SOCIOS );
        CLOSE_TABLE( T_CLI_COBRO, TT_ORIG );
    }
    CERRAR_VTA();
    CLOSE_ALL();
    ABRIR_STRING_TABLE();
}
/***********************************************************************/
int AGREGAR_ALTAS()
/***********************************************************************/
{
    int ok = 0, _c1 = 0, _c2 = 0, evento = 0, c;
    char aux[70];
    unsigned long indice;
    struct _articulo altas;
    struct _present altas_present;
    int procesada = 0, max_procesada = 1, ok_dbr = 0, continua = 0;
    //#ifdef INVEL_L
    //#define ALTAS "altas"
    //#define ALTAS_PR "altas_pr.ctr"
    //#else
    //#define ALTAS "altas.sic"
    //#define ALTAS_PR "altas_pr.sic"
    //#endif
    memset( &altas, 0, sizeof( altas ) );
    memset( &altas_present, 0, sizeof( altas_present ) );
	if( !GET_ERROR_EN_RED() ) {
		if( MODO_NEGOCIO != SERVICIOS ) {
			MENSAJE_STRING_SIN_PAUSA( S_TOMANDO_ALTAS );
		}
    }
	do {
        if( CANTIDAD_MINIMA_ALTAS > 0 ) {
            max_procesada = CANTIDAD_MINIMA_ALTAS;
        }
		{
            #define ALTAS_POR_VEZ 75
            char sql_where[100];
            ok_dbr = 0;
            //sprintf( sql_where, " WHERE autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
			sprintf( sql_where, " autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
                     NRO_ULTIMA_ALTA_PROCESADA, NRO_ULTIMA_ALTA_PROCESADA + ALTAS_POR_VEZ );
            GRABAR_LOG_SISTEMA( sql_where ,LOG_DEBUG,3 );
            //borrar la siguiente linea (p pruebas de import)
            //OBTENER_TABLA_SERVIDOR( T_RANGOS, TT_TEMP, "rangos", "", SI, NULL );
            if( !OBTENER_TABLA_SERVIDOR( T_ARTICULO, TT_ALTA, "alta_art", sql_where, SI, NULL ) ) {
                if( PRESENTACIONES_HABILITADAS ) {
                    //sprintf( sql_where, " WHERE autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
					sprintf( sql_where, " autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
                             NRO_ULTIMA_ALTA_PROCESADA,
                             NRO_ULTIMA_ALTA_PROCESADA + ALTAS_POR_VEZ );
                    if( !OBTENER_TABLA_SERVIDOR( T_PRESENT, TT_ALTA, "alta_pr", sql_where, SI, NULL ) ) {
                        ok_dbr = 1;
                    }
                }
                else {
                    ok_dbr = 1;
                }
            }
            
            if( !ok_dbr ) {
                GRABAR_LOG_ID_DBROUTER( SDBR_ERROR_ALTAS , LOG_ERRORES,2);
                BORRAR_MENSAJE();
                return ( 0 );
            }
            /*------------------------------- Articulos ------------------------------*/
            if( /*USE_DB( AREA_AUX, _ALTAS_SIC, ( char* )&altas, sizeof( altas ), NULL, 0, 0 )*/
				OPEN_TABLE(T_ARTICULO, TT_ALTA ,( char* )&altas, sizeof( altas ))== 0 ) {
                SELECT_TABLE( T_ARTICULO, TT_ALTA );
                SET_WHERE( "" );
                SET_ORDER2( "AUTOINC" );
                RUN_QUERY( NO );
                GO2( BOTTOM );
                //SET_ORDER( 4 );
                //GO( BOTTOM );
                if( altas.autoinc
                 >= ( unsigned long ) ( NRO_ULTIMA_ALTA_PROCESADA + ALTAS_POR_VEZ - 5 ) ) {
                    continua = 1;
                }
                else {
                    continua = 0;
                }
                if( PRESENTACIONES_HABILITADAS ) {
                    if( /*USE_DB( AREA_AUX2, _ALTAS_PR_SIC, ( char* )&altas_present,
                                sizeof( altas_present ), NULL, 0, 0 )*/
						OPEN_TABLE( T_PRESENT, TT_ALTA , ( char* )&altas_present,
										sizeof( altas_present ))== 0 ) {
                        ok = 1;
                    }
                }
                else {
                    ok = 1;
                }
                if( ok_dbr && ok ) {
                    //SELECT( AREA_AUX );
					SELECT_TABLE( T_ARTICULO, TT_ALTA );
                    indice = NRO_ULTIMA_ALTA_PROCESADA;
                    //GET_GREATER( ( char* )&indice );
					SET_WHERE("AUTOINC > '%ld'",indice);
					SET_ORDER2("AUTOINC");
					RUN_QUERY(NO);
                    sprintf( aux, "Inicia ALTAS (%li) (%li)", NRO_ULTIMA_ALTA_PROCESADA,
                             altas.autoinc );
                    GRABAR_LOG_SISTEMA( aux ,LOG_DEBUG,3 );
                    while( !dbEOF() && altas.autoinc > ( unsigned )NRO_ULTIMA_ALTA_PROCESADA
                        && !evento ) {
                        // Modificacion de altas...
                        ALTA_ARTICULO( &altas, &altas_present, &_c1, &_c2 );
                        /*
                         * SELECT(AREA_ARTICULOS);
                         * memcpy(&articulo, &altas, sizeof(altas));
                         * articulo.autoinc = 0;
                         * INSERT();
                         * _c1++;
                         * //------------------ Procesa las presentaciones -----------------
                         * if (PRESENTACIONES_HABILITADAS) {
                         * SELECT(AREA_AUX2);
                         * ind_present.autoinc = altas.autoinc;
                         * ind_present.cod_present = 0;
                         * GET_GREATER_E((char *) &ind_present);
                         * while (!BtrvEOF() && altas_present.autoinc == altas.autoinc) {
                         * SELECT(AREA_PRESENT);
                         * memcpy(&present, &altas_present, sizeof(altas_present));
                         * present.autoinc = articulo.autoinc;
                         * _c2++;
                         * INSERT();
                         * SELECT(AREA_AUX2);
                         * SKIP(1);
                         * }
                         * } */
                        /*--------------------- Actualiza punteros ----------------------*/
                        SET_MEMORY( __nro_ultima_alta_procesada, ( char* )&( altas.autoinc ) );
						{char a[80];
						sprintf(a,"1: alta-auto_inc: %d", altas.autoinc);
						GRABAR_LOG_SISTEMA(a,LOG_DEBUG,3 );
						}
                        procesada++;
                        if( procesada > max_procesada ) {
                            procesada = 0;
                            if( KBHIT() ) {
                                c = _GETCH();
                                if( c != -999 && GET_FUNCION_TEC(c) == 27 ) {
                                    evento = 1;
                                }
                            }
                        }
                        //SELECT( AREA_AUX );
						SELECT_TABLE( T_ARTICULO, TT_ALTA );
                        SKIP2( 1 );
                    }
                    CLOSE_TABLE( T_PRESENT, TT_ALTA );
                    sprintf( aux, "Termina ALTAS (%li) (%i) (%i)", NRO_ULTIMA_ALTA_PROCESADA, _c1,
                             _c2 );
                    GRABAR_LOG_SISTEMA( aux ,LOG_DEBUG,3 );
                }                    // IF OK
                CLOSE_TABLE( T_ARTICULO, TT_ALTA );
            }                       // IF USE
            //BORRAR_MENSAJE();
			}
    }
    while( continua && !evento );
	BORRAR_MENSAJE();

    return evento;
}
/***********************************************************************/
int AGREGAR_MODIFICACIONES( void )
/***********************************************************************/
{
    char cod_barra[17],aux[80];
    int ok = 0, salir = 0, _c1 = 0, _c2 = 0, evento = 0, c, ok_dbr = 1, rta_btr;
    long autoinc, indice = 0;
    struct _articulo modif;
    struct _present modif_present;
    struct _indice_cod_int_cod_barra indice_cod_int_cod_barra;
    int procesada = 0, max_procesada = 1, continua = 0, seguir;

	/*Se pone a prueba, aumentar la cant de modif por vez, de 75 a 200 para minimizar 
	el tiempo de demora que emplea cuando va ha obtener la tabla modif_art de la base en la 
	finalización de	cada bloque*/
    #define MODIFICACIONES_POR_VEZ 200

    /*#ifdef INVEL_L
    #define MODIF "modificaciones"
    #define MODIF_PR "modif_pr.ctr"
    #else
    #define MODIF "modif.sic"
    #define MODIF_PR "modif_pr.sic"
    #endif*/
    memset( &modif, 0, sizeof( modif ) );
    memset( &modif_present, 0, sizeof( modif_present ) );
	if( !GET_ERROR_EN_RED() ) {
		if( MODO_NEGOCIO != SERVICIOS ) {
			MENSAJE_STRING_SIN_PAUSA( S_TOMANDO_MODIFICACIONES );
		}
    }
	do {
        if( CANTIDAD_MINIMA_MODIFICACIONES > 0 ) {
            max_procesada = CANTIDAD_MINIMA_MODIFICACIONES;
        }
		{
                char where_sql[100];
                ok_dbr = 0;
                //sprintf( where_sql, "WHERE autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
				sprintf( where_sql, " autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
                         NRO_ULTIMA_MODIF_PROCESADA,
                         NRO_ULTIMA_MODIF_PROCESADA + MODIFICACIONES_POR_VEZ );
                if( !OBTENER_TABLA_SERVIDOR( T_ARTICULO, TT_MODIF, "modi_art", where_sql, SI, NULL ) ) {
                    if( PRESENTACIONES_HABILITADAS ) {
                        //sprintf( where_sql, "WHERE autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
						sprintf( where_sql, " autoinc > %li AND autoinc <= %li ORDER BY autoinc ",
                                 NRO_ULTIMA_MODIF_PROCESADA,
                                 NRO_ULTIMA_MODIF_PROCESADA + MODIFICACIONES_POR_VEZ );
                        if( !OBTENER_TABLA_SERVIDOR( T_MODIF_PR, TT_ORIG, "modi_pr", where_sql, SI, NULL ) ) {
                            ok_dbr = 1;
                        }
                    }
                    else {
                        ok_dbr = 1;
                    }
				}
		}
            if( !ok_dbr ) {
                GRABAR_LOG_ID_DBROUTER( SDBR_ERROR_MODIF, LOG_ERRORES,2 );
                BORRAR_MENSAJE();
                return ( 0 );
            }
            if( /*USE_DB( AREA_MODIF_ART, _MODIF_SIC, ( char* )&modif, sizeof( modif ), NULL, 0, 0 )*/
				OPEN_TABLE( T_ARTICULO, TT_MODIF ,( char* )&modif, sizeof( modif ))== 0 ) {
                SELECT_TABLE( T_ARTICULO, TT_MODIF );
                SET_WHERE( "" );
                SET_ORDER2( "AUTOINC" );
                RUN_QUERY( NO );
                GO2( BOTTOM );
                //SET_ORDER( 4 );
                //GO( BOTTOM );
                //if( RECCOUNT( ) == MODIFICACIONES_POR_VEZ ) {
                if( modif.autoinc
                 >= ( unsigned long ) ( NRO_ULTIMA_MODIF_PROCESADA + MODIFICACIONES_POR_VEZ - 5 ) ) {
                    continua = 1;
                }
                else {
                    continua = 0;
                }                
                if( PRESENTACIONES_HABILITADAS ) {
                    if( /*USE_DB( AREA_AUX2, _MODIF_PR_SIC, ( char* )&modif_present,
                                sizeof( modif_present ), NULL, 0, 0 )*/
						OPEN_TABLE( T_MODIF_PR, TT_ORIG ,( char* )&modif_present,
                                sizeof( modif_present ))== 0 ) {
                        ok = 1;
                    }
                }
                else {
                    ok = 1;
                }
                if( ok ) {
                    //SELECT( AREA_MODIF_ART );
                    //SET_ORDER( 4 );
					SELECT_TABLE( T_ARTICULO, TT_MODIF );
                    indice = NRO_ULTIMA_MODIF_PROCESADA;
					SET_WHERE("AUTOINC > '%ld'",indice);
					SET_ORDER2("AUTOINC");
                    //GET_GREATER( ( char* )&indice );
					RUN_QUERY(NO);
                    sprintf( aux, "Inicia MODIFICACIONES (%li) (%li)", NRO_ULTIMA_MODIF_PROCESADA,
                             modif.autoinc );
                    GRABAR_LOG_SISTEMA( aux ,LOG_DEBUG,3 );
                    while( !dbEOF() && modif.autoinc > ( unsigned )NRO_ULTIMA_MODIF_PROCESADA
                        && !evento ) {
                        _c1++;
                        //SELECT( AREA_ARTICULOS );
						SELECT_TABLE( T_ARTICULO, TT_ORIG );
                        if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                            //SET_ORDER( INDICE_COD_INT_COD_BARRA );
                            indice_cod_int_cod_barra.cod_interno = modif.cod_interno;
                            memcpy( indice_cod_int_cod_barra.cod_barra, modif.cod_barra, 17 );
							SET_WHERE("COD_INTERNO = '%ld' and CODIGO_BARRA = '%s'",
										indice_cod_int_cod_barra.cod_interno,
										indice_cod_int_cod_barra.cod_barra);
                            //GET_EQUAL( ( char* )&indice_cod_int_cod_barra );
							RUN_QUERY(NO);
                        }
                        else {
                            //SET_ORDER( 3 );
							SET_WHERE("COD_INTERNO = '%ld'",modif.cod_interno);
                            //GET_EQUAL( ( char* )&modif.cod_interno );
							RUN_QUERY(NO);
                        }
                        if( FOUND2() ) {
                            //while( modif.cod_interno == articulo.cod_interno && !BtrvEOF(  ) ) {
                            seguir = 1;
                            while( seguir && !dbEOF() ) {
                                // Reinicio la variable salir, para cargar la presentacion siguiente.
                                salir = 0;
                                if( BUSQUEDA_DE_ARTICULOS == BUSQUEDA_COD_INT_Y_COD_BARRA ) {
                                    if( !( modif.cod_interno == articulo.cod_interno
                                        && ( strcmp( modif.cod_barra, articulo.cod_barra ) == 0 )
                                        && !dbEOF() ) ) {
                                        seguir = 0;
                                    }
                                }
                                else {
                                    if( !( modif.cod_interno == articulo.cod_interno
                                        && !dbEOF() ) ) {
                                        seguir = 0;
                                    }
                                }
                                if( seguir ) {
                                    _c2++;
                                    strcpy( cod_barra, articulo.cod_barra );
                                    autoinc = articulo.autoinc;
                                    memcpy( &articulo, &modif, sizeof( modif ) );
                                    articulo.autoinc = autoinc;
                                    //SELECT( AREA_MODIF_ART );
									SELECT_TABLE( T_ARTICULO, TT_MODIF );
                                    //####COMO TRADUCIR TESTEAR!!!!!!!?????####
                                    if( !TESTEAR( 2 ) ) {
                                        strcpy( articulo.cod_barra, cod_barra );
                                    }
                                    //if( TESTEAR( 3 ) ) {
                                    if( modif.status == _baja_ ) {
                                        if( strcmp( modif.cod_barra, cod_barra ) == 0 ) {
                                            //SELECT( AREA_ARTICULOS );
											SELECT_TABLE( T_ARTICULO, TT_ORIG );
                                            #ifdef INVEL_L
                                            //BEGIN_TRANSACTION();
                                            #endif
                                            //_DELETE();
											DELETE2();
                                            #ifdef INVEL_L
                                            //END_TRANSACTION();
                                            #endif
                                            //SET_ORDER( 3 );
                                            //GET_EQUAL( ( char* )&modif.cod_interno );
											SET_WHERE("COD_INTERNO = '%ld'",modif.cod_interno);
											RUN_QUERY(NO);
                                            if( FOUND2() ) {
                                                continue;
                                            }
                                            else {
                                                break;
                                            }
                                        }
                                    }
                                    else {
                                        //SELECT( AREA_ARTICULOS );
										SELECT_TABLE( T_ARTICULO, TT_ORIG );
                                        #ifdef INVEL_L
                                        //BEGIN_TRANSACTION();
                                        #endif
                                        rta_btr = UPDATE2();
                                        #ifdef INVEL_L
                                        //END_TRANSACTION();
                                        #endif
                                    }
                                    //---------------- Procesa las presentaciones ----------------*/
                                    if( PRESENTACIONES_HABILITADAS ) {
                                        //------ localiza la primera en presentaciones -----*/
                                        //SELECT( AREA_PRESENT );
										SELECT_TABLE( T_PRESENT, TT_ORIG );
                                        ind_present.autoinc = articulo.autoinc;
                                        ind_present.cod_present = 0;
										/*SET_WHERE("CONCAT(AUTOINC,COD_PRESENT) >= CONCAT(%ld,%i)",
													ind_present.autoinc,
													ind_present.cod_present);
										SET_ORDER2("AUTOINC,COD_PRESENT");*/
                                        SET_WHERE(" AUTOINC >= '%ld' ", ind_present.autoinc );
                                        SET_ORDER2("AUTOINC");
                                        //GET_GREATER_E( ( char* )&ind_present );
										RUN_QUERY(NO);
                                        //---borra todas las presentaciones para ese articulo----*/
                                        while( !dbEOF() && present.autoinc == articulo.autoinc ) {
                                            #ifdef INVEL_L
                                            //BEGIN_TRANSACTION();
                                            #endif
                                            //_DELETE();
											DELETE2();
                                            #ifdef INVEL_L
                                            //END_TRANSACTION();
                                            #endif
                                            //SKIP( 1 );
                                        }
                                        //------ localiza la primera en modificaciones -----*/
                                        //SELECT( AREA_AUX2 );
										SELECT_TABLE( T_MODIF_PR, TT_ORIG );
                                        ind_present.autoinc = modif.autoinc;
                                        ind_present.cod_present = 0;
										/*SET_WHERE("CONCAT(AUTOINC,COD_PRESENT) >= CONCAT(%ld,%i)",
														ind_present.autoinc,
														ind_present.cod_present);
										SET_ORDER2("AUTOINC,COD_PRESENT");*/
                                        SET_WHERE(" AUTOINC >= '%ld' ", ind_present.autoinc );
                                        SET_ORDER2("AUTOINC");
                                        //GET_GREATER_E( ( char* )&ind_present );
										RUN_QUERY(NO);
                                        while( !dbEOF()
                                            && modif_present.autoinc == modif.autoinc ) {
                                            //SELECT( AREA_PRESENT );
											SELECT_TABLE( T_PRESENT, TT_ORIG );
                                            memcpy( &present, &modif_present,
                                                    sizeof( modif_present ) );
                                            present.autoinc = articulo.autoinc;
                                            #ifdef INVEL_L
                                            //BEGIN_TRANSACTION();
                                            #endif
                                            INSERT2(SI);
                                            #ifdef INVEL_L
                                            //END_TRANSACTION();
                                            #endif
                                            //selecciona las modificaciones y pasa al siguiente
                                            //SELECT( AREA_AUX2 );
											SELECT_TABLE(T_MODIF_PR, TT_ORIG);
                                            SKIP2( 1 );
                                        }
                                      
                                    }// Fin Presentaciones
                                }
                                //----------------- Pasa al proximo articulo ---------------*/
                                //SELECT( AREA_ARTICULOS );
								SELECT_TABLE( T_ARTICULO, TT_ORIG );
                                SKIP2( 1 );
                            }
                        }
                        else {
                            //SELECT( AREA_MODIF_ART );
							SELECT_TABLE( T_ARTICULO, TT_MODIF );
                            //if( !TESTEAR( 3 ) ) {
                            if( modif.status == _alta_ ) {
                                ALTA_ARTICULO( &modif, &modif_present, &_c1, &_c2 );
                            }
                        }
                        //--------------------- Actualiza punteros ----------------------*/
                        //SELECT( AREA_MODIF_ART );
						SELECT_TABLE( T_ARTICULO, TT_MODIF );
						SET_MEMORY( __nro_ultima_modif_procesada, ( char* )&( modif.autoinc ) );
						{char a[80];
						sprintf(a,"1: modif-auto_inc: %d", modif.autoinc);
						GRABAR_LOG_SISTEMA(a,LOG_DEBUG,3 );
						}
                        SKIP2( 1 );
                        procesada++;
                        if( procesada > max_procesada ) {
                            procesada = 0;
                            if( KBHIT() ) {
                                c = _GETCH();
								{char prueba[20];sprintf(prueba,"TECLA %i",c);glog(prueba,LOG_DEBUG,4 );}
                                if( c != -999 && GET_FUNCION_TEC(c) == 27 ) {
                                    glog("PRESIONA ESCAPE",LOG_VENTAS,2);
                                    evento = 1;
                                }
                            }
                        }
                    }
                    if( PRESENTACIONES_HABILITADAS ) {
                        CLOSE_TABLE( T_MODIF_PR, TT_ORIG );
                    }
                    sprintf( aux, "Termina MODIFICACIONES (%li) (%i) (%i)",
                             NRO_ULTIMA_MODIF_PROCESADA, _c1, _c2 );
                    GRABAR_LOG_SISTEMA( aux , LOG_VENTAS,2);
                }
                CLOSE_TABLE( T_ARTICULO, TT_MODIF );
            }
            //BORRAR_MENSAJE();
        else {
            GRABAR_LOG_SISTEMA( "SIN RED. NO SE TOMARON MODIFICACIONES DE ARTICULOS", LOG_ERRORES,2 );
        }
		}
    while( continua && !evento );
    BORRAR_MENSAJE();

    return evento;
}
/***********************************************************************/
int GUARDAR_MODIFICACIONES_DEUDAS( int max_procesada ) 
/***********************************************************************/
{
		//Eliminar (Servicios)
    return 0;
}

/**
/* Verifica existe una deuda en la base de datos.
/* @param id_deuda identificador unico de deuda
/* @return 1 si la deuda existe, 0 en caso contrario
/***********************************************************************/
int EXISTE_DEUDA( long id_deuda )
/***********************************************************************/
{
		//Eliminar (Servicios)
    return 0;
}
/** 
/* Se toman datos de novedades de deudas (operaciones cli) y se actualiza el padron
/* de deudas principal con estos datos.
/* Las novedades incluyen: ALTAS, BAJAS Y MODIFICACIONES
/* @return codigo de error
/***********************************************************************/
int ACTUALIZA_DEUDAS( void )
/***********************************************************************/
{
		//Eliminar (Servicios)
    return 0;
}
//**********************************************************************/
int BUSCA_CODIGO_BARRA( char *cod_barra )
//**********************************************************************/
{
    char buffer[17];
	int longitud1=0,longitud2=0;
    
	memset( buffer, 0, sizeof(buffer) );
	longitud1=sizeof(buffer);
	longitud2=strlen(cod_barra);

	SELECT_TABLE( T_ARTICULO, TT_ORIG );
    
    
	if( longitud1 > longitud2 )
		strncpy( buffer, cod_barra,longitud2 );
	else
		strncpy( buffer, cod_barra,longitud1 );


    CEREAR( buffer, L_CODIGO );
    //GET_EQUAL( buffer );
	SET_WHERE("CODIGO_BARRA = '%s'",buffer);
	RUN_QUERY(NO);
    if( FOUND2() ) {
        while( !strcmp( cod_barra, articulo.cod_barra ) && articulo.habilitado == 'N'
            && !dbEOF() && !RAM_FLAG_COBRO_VENTA  && !articulo.cod_cobro ) {
            SKIP2( 1 );
        }
        if( !strcmp( cod_barra, articulo.cod_barra ) && ( articulo.habilitado != 'N' || RAM_FLAG_COBRO_VENTA || articulo.cod_cobro ) ) {
            SET_STATUS( S_FOUND, 1 );
            PREPARA_REGISTRO_ARTICULOS( SI);
            return 1;
        }
    }
    return 0;
}
//**********************************************************************/
int BUSCA_DESCRIPCION( char *cadena )
//**********************************************************************/
{
    //SELECT( GET_AREA_ARTICULOS() );
    //SET_ORDER( 2 );
    //GET_EQUAL( cadena );
	SELECT_TABLE( T_ARTICULO, TT_ORIG );
	SET_WHERE("NOMBRE = '%s'",cadena);
	RUN_QUERY(NO);
    PREPARA_REGISTRO_ARTICULOS( NO );
    return ( FOUND2() );
}
//**********************************************************************/
int BUSCA_CODIGO_ESPECIAL( long cod )
//**********************************************************************/
{
    //int area_ant = 0;
	int tabla_ant, tipo_ant;
    int rta = 0;
    //area_ant = SELECTED();
	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( GET_AREA_ARTICULOS() );
    //SET_ORDER( 3 );
    //GET_EQUAL( ( char* )&cod );
	SELECT_TABLE( T_ARTICULO, TT_ORIG );
	SET_WHERE("COD_INTERNO = '%ld'",cod);
		RUN_QUERY(NO);
    PREPARA_REGISTRO_ARTICULOS( SI );
    rta = FOUND2();
    //SELECT(area_ant);
	SELECT_TABLE( tabla_ant, tipo_ant );
    return ( rta );
}
//**********************************************************************/
int ASIGNA_CODIGO_BARRA( char *cod_barra )
//**********************************************************************/
{
    int rta = 0;
    long cod;
    if( BUSCA_CODIGO_BARRA( cod_barra ) ) {
        rta = ASIGNA_ITEM( NO );
    }
    if( !rta && BUSQUEDA_COD_INTERNO_ALTERNATIVA ) {
        if( _leyo_scanner && DESCARTAR_VERIFICADOR_BUSQ_ALTER ) {
            cod_barra[L_CODIGO - 1] = 0;
        }
        else {
            cod_barra[L_CODIGO] = 0;
        }
        cod = atol( cod_barra );
        rta = ASIGNA_CODIGO_ESPECIAL( cod );
    }
    return ( rta );
}
/***********************************************************************/
int ASIGNA_CODIGO_ESPECIAL( long cod )
/***********************************************************************/
{
    int rta = 0;
    if( BUSCA_CODIGO_ESPECIAL( cod ) ) {
        rta = ASIGNA_ITEM( NO );
    }
    return ( rta );
}
/***********************************************************************/
void CALCULA_ARTICULO_EXT()
/***********************************************************************/
{
    //double    precio = 0.0;
    double tasa_iva1 = 0.0;
    double tasa_iva2 = 0.0;
    double tasa[4], tasa_tot = 0.0;
    INT16 suma[4];
    int i, j = 0, x = 0;
    double monto_tasa = 0.0;
   
	PREPARA_REGISTRO_ARTICULOS( NO );
    articulo_ext.iva1 = 0.0;
    articulo_ext.iva2 = 0.0;
    articulo_ext.precio = articulo.precio_sin_iva - articulo.imp_int;
    for( i = 0;i < 4;i++ ) {
        tasa[i] = 0.0;
        suma[i] = 0;
    }
    for( i = 0;i < 4;i++ ) {
        j = 0;             
        while( j < 40 ) {
            if( articulo.cod_impuesto[i] > 0
             && ( impuestos[j].cod_impuesto == articulo.cod_impuesto[i] ) ) {
                if( config.pais == CHILE ) {
                    tasa[x] = ( RAM_FACTURA ) ? ( impuestos[j].tasa / 100.0 ) : 0;            
                    if( RAM_NOTA_CR && RAM_TIPO_EVENTO_COMP_ORIGINAL == 0 && i != 0 ) {
                        tasa[x] = 0.0;
                    }
                } else {
                    tasa[x] = impuestos[j].tasa / 100.0;
                    suma[x] = !impuestos[j].accion; //0:SUMA; 1:NO SUMA
                }
                x++;
            }
            j++;
        }
    }
    for( i = 0;i < 4;i++ ) {
        articulo_ext.porc_imp[i] = ( float )tasa[i] * 100;
        articulo_ext.sumar_al_tot[i] = suma[i];
    }   
    for( i = 0;i < 4;i++ ) {
        articulo_ext.monto_imp[i] = articulo_ext.precio * tasa[i];
    }

    if( articulo.gravado == 'S' /*&& !EXENTO_PARA_DIPLOMATICOS()*/ ) {
        articulo_ext.iva1 = articulo_ext.precio * iva[INDICE_IVA_ARTICULO()].porc_iva1 / 100.0;
        tasa_iva1 = iva[INDICE_IVA_ARTICULO()].porc_iva1 / 100.0;
        if( EMITIR_FACTURA()
         && ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva == RESP_NO_INSC ) ) {
            articulo_ext.iva2 = articulo_ext.precio * iva[INDICE_IVA_ARTICULO()].porc_iva2 / 100.0;
            tasa_iva2 = iva[INDICE_IVA_ARTICULO()].porc_iva2 / 100.0;
        }
		articulo_ext.precio =
                articulo_ext.precio + articulo.imp_int + articulo_ext.iva1 + articulo_ext.iva2;
    }
    else {
        articulo_ext.precio = articulo.precio_sin_iva + articulo.imp_int;
    }
    for( i = 0;i < 4;i++ ) {
        monto_tasa += ( suma[i] ? articulo_ext.monto_imp[i] : 0 );
    }
    //fabiana 15-02-05 ** nuevo manejo de redondeos.
    //"precio_sin_redondeo", es el precio recalculado para que dï¿½exacto el precio final. Pero
    //se necesita conserva el iva, ï¿½te se usarï¿½en el cï¿½culo del iva      
    articulo_ext.precio = articulo_ext.precio + monto_tasa;
    //articulo_ext.precio = ROUND( articulo_ext.precio, ENTEROS, DECIMALES ); 
	//Se comenta por diferencias al grabar en eventos
    /*for( i = 0;i < 4;i++ ) {
        tasa_tot += ( suma[i] ? tasa[x] : 0 );
    }
    precio = articulo_ext.precio - articulo.imp_int;
    articulo.precio_sin_iva = precio / ( 1 + tasa_iva1 + tasa_iva2 + tasa_tot ); */ 

    articulo.precio_sin_iva = articulo_ext.precio - articulo_ext.iva1 - articulo_ext.iva2
                            - /*tasa_tot*/monto_tasa;
    for( i = 0;i < 4;i++ ) {
        articulo_ext.monto_imp[i] = articulo.precio_sin_iva * tasa[i];
    }
    /*articulo_ext.iva1 = articulo.precio_sin_iva * tasa_iva1;
    articulo_ext.iva2 = articulo.precio_sin_iva * tasa_iva2;*/
    
    if( articulo.gravado == 'S' && EXENTO_PARA_DIPLOMATICOS(  ) ) {
        //articulo_ext.precio_sin_redondeo = articulo.precio_sin_iva;
        articulo.precio_sin_iva = ROUND( articulo.precio_sin_iva , ENTEROS, DECIMALES );
        articulo_ext.precio = ROUND( articulo.precio_sin_iva , ENTEROS, DECIMALES );
        articulo_ext.iva1 = 0.0;
	    articulo_ext.iva2 = 0.0;
    }

	// Recalculo importes para el caso de impuestos precalculados
	if( PRECALCULAR_IMPUESTOS ) {
		double tot = 0.0;
		for( i = 0; i < 4; i++ ) {
			tot += articulo_ext.monto_imp[i];
		}
		articulo_ext.precio = articulo.precio_sin_iva - articulo.imp_int + tot;
		if( articulo.gravado == 'S' ){
			articulo_ext.iva1 = articulo_ext.precio * ( iva[INDICE_IVA_ARTICULO()].porc_iva1 / 100.0 );
			articulo_ext.iva2 = articulo_ext.precio * ( iva[INDICE_IVA_ARTICULO()].porc_iva2 / 100.0 );
		}
		if( !( articulo.gravado == 'S' && EXENTO_PARA_DIPLOMATICOS(  ) ) ) {
		articulo_ext.precio =
	        articulo_ext.precio + articulo.imp_int + articulo_ext.iva1 + articulo_ext.iva2;
		}
	}
	
}
/***********************************************************************/
int ASIGNA_ITEM( int segunda_pasada )
/***********************************************************************/
{
    int rta,i, t;
    double recargo = 0;
    //fabiana 15-02-05 ** nuevo manejo de redondeos
    //   if( MODO_FACTURACION_ESPECIAL(  ) ) {
    //      articulo.precio_sin_iva = ROUND( articulo.precio_sin_iva, 10, DECIMALES );
    //   }
	SET_MEMORY_DOUBLE( __item_precio_original, 0.0);
    if( !EXENTO_PARA_DIPLOMATICOS() && RAM_FACTURA && RAM_NRO_CLIENTE
     && ( ES_FACTURA_A( clientes.cond_iva )
       || ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva == RESP_NO_IDEN )
       || ( ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && PERMITIR_USO_DE_MONOTRIBUTO
           && clientes.cond_iva == MONOTRIBUTO ) ) ) ) {
        if( articulo.cod_impuesto[2] != 0 ) {
            int cod_iva_temp = articulo.cod_iva;
            articulo.cod_iva = articulo.cod_impuesto[2];
            articulo.cod_impuesto[2] = cod_iva_temp + 1;
        }
    }
    else {
        articulo.cod_impuesto[2] = 0;
    }
    //CALCULA_IMPUESTOS(  );
    if( ( RAM_NOTA_CR || MODO_DEVOLUCION ) && !segunda_pasada ) {
        articulo.precio_sin_iva *= -1;
        articulo.imp_int *= -1;
        articulo.costo *= -1;
		for( i = 0;i < 4;i++ ) {
			articulo.porc_impuesto[i] *= -1;
		}
    }
    if( strlen( articulo.nombre ) < sizeof( articulo.nombre ) - 1 ) {
        for( i = strlen( articulo.nombre );
            i < sizeof( articulo.nombre ) - 1;i++ ) {
            articulo.nombre[i] = ' ';
        }
    }
    articulo.nombre[sizeof( articulo.nombre ) - 1] = 0;

    if( DEBE_PEDIR_PRECIO_DEVOL_Y_NC( 0 )){
    /*if( MODO_DEVOLUCION && PEDIR_PRECIO_EN_NC ) {*/
       SET_MEMORY_CHAR( __ram_pedir_precio, 1 );
    }
    SET_MEMORY( __item_cod_barra, articulo.cod_barra );
    TRIM( articulo.nombre );
    SET_MEMORY( __item_nombre_largo, articulo.nombre );
    SET_MEMORY( __item_marca, articulo.marca );
    SET_MEMORY( __item_contenido, articulo.contenido );
    SET_MEMORY( __item_unidad, articulo.unidad );
	for( t = 0; t < 4; t++ ){
	    _SET_MEMORY_CHAR( __item_cod_impuesto, t, articulo.cod_impuesto[t] );
	}
    SET_MEMORY_INT( __item_depto, articulo.cod_dpto );
    SET_MEMORY_INT( __item_tipo_envase, articulo.tipo_envase );
    SET_MEMORY_CHAR( __item_envase, ( articulo.envase == 'S' ) ? 1 : 0 );
    SET_MEMORY_CHAR( __item_dev_envase, ( articulo.dev_envase == 'S' ) ? 1 : 0 );
    if( !RAM_USA_TEMPLATE ) {
        SET_MEMORY_CHAR( __item_pesable, ( articulo.pesable == 'S' ) ? 1 : 0 );
        SET_MEMORY_CHAR( __item_pedir_precio,( articulo.precio == 'S'
            || RAM_PEDIR_PRECIO ) ? 1 : 0 );
        //fabiana 15-02-05 ** nuevo manejo de redondeos--esta linea esta de mas
        //SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva );
    } else if( RAM_USA_TEMPLATE == 1 ) {        //por precio
        //por precio
        SET_MEMORY_CHAR( __item_pesable, 0 );
        SET_MEMORY_CHAR( __item_pedir_precio, 1 );
    } else if( RAM_USA_TEMPLATE == 2 ) {        //por peso
        //por peso
        SET_MEMORY_CHAR( __item_pesable, 1 );
        SET_MEMORY_CHAR( __item_pedir_precio, 0 );
        //fabiana 15-02-05 ** nuevo manejo de redondeos--esta linea esta de mas
        //SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva );
    } else if( RAM_USA_TEMPLATE == 3 ) {
        //por unidades
        SET_MEMORY_CHAR( __item_pesable, 0 );
        SET_MEMORY_CHAR( __item_pedir_precio, 0 );
        SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva );
    }
    SET_MEMORY_CHAR( __item_gravado, OBTENER_COD_GRAVADO() );
    SET_MEMORY_CHAR( __item_inhabilitado, ( articulo.habilitado == 'N' ) ? 1 : 0 );
    SET_MEMORY_CHAR( __item_credito,
                     ( ( articulo.cr_habilitado == 'S'
                      && ( RAM_USAR_PRECIO_CR || USAR_PRECIO_CREDITO ) )
                      || articulo.habilitado_oferta == 'S' ) ? 1 : 0 );
    SET_MEMORY_CHAR( __item_scanner, _leyo_scanner );
    //fabiana 15-02-05 ** nuevo manejo de redondeos--primera asignacion sin redondeo
    SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva );
    SET_MEMORY_DOUBLE( __item_imp_int, articulo.imp_int );
    SET_MEMORY_DOUBLE( __item_costo, articulo.costo );
    SET_MEMORY_CHAR( __item_cant_present, 0 );
	SET_MEMORY_CHAR( __item_cod_presentacion, 0 );
    SET_MEMORY_LONG( __item_cod_especial, articulo.cod_interno );
    SET_MEMORY_LONG( __item_cod_enganche, articulo.cod_enganche );
    SET_MEMORY_LONG( __item_autoinc, articulo.autoinc );
    SET_MEMORY_CHAR( __item_imprimir_barra, articulo.imp_cod_barra );
    //SET_MEMORY_FLOAT( __item_porc_ila, articulo.porc_ila );
    //SET_MEMORY_FLOAT( __item_porc_iha, articulo.porc_iha );
    for( i = 0;i < 4;i++ ) {
        _SET_MEMORY_FLOAT( __item_porc_impuesto, i, articulo.porc_impuesto[i] );
    }
    SET_MEMORY_CHAR( __item_cupon_dev_env, 0 );
    SET_MEMORY_LONG( __item_cod_clasificacion, articulo.cod_clasificacion_old );
	SET_MEMORY( __item_cod_clasificacion_str, articulo.cod_clasificacion );
 //   SET_MEMORY_CHAR( __item_cod_iva, INDICE_IVA_ARTICULO() );
    SET_MEMORY_CHAR( __item_atrib_descuentos, articulo.atrib_descuentos );
    SET_MEMORY_CHAR( __item_habilitado_en_promocion,
                     ( articulo.incluible_promocion != 'N' ) ? 1 : 0 );
    SET_MEMORY_DOUBLE( __item_descuento_cantidad, 0 );
    SET_MEMORY_CHAR( __item_hab_desc_cliente, articulo.hab_desc_cliente );
    SET_MEMORY_INT( __item_cod_solicitud, articulo.cod_solicitud );
    //SET_MEMORY_INT( __item_cod_banda, articulo.cod_banda );
    SET_MEMORY_INT( __item_cod_banda, 0 );
    SET_MEMORY_CHAR( __item_pedir_supervisor, ( articulo.pedir_supervisor == 'S' ) ? 1 : 0 );
    /*if( MODO_DEVOLUCION && RAM_PEDIR_PRECIO ) {
     *   SET_MEMORY_CHAR( __ram_pedir_precio, 0 );
    }*/
    for( i = 0;i < 4;i++ ) {
        _SET_MEMORY_INT( __item_cod_promocion, i, articulo.cod_promocion[i] );
    }
    SET_MEMORY_CHAR( __item_habilitado_en_cuotas, ( articulo.habilitado_cuotas == 'N' ) ? 0 : 1 );
    SET_MEMORY_INT( __item_cod_cobro, articulo.cod_cobro );
    /*--------------------- calcula variacion de precio ----------------------*/
    if( NRO_VARIACION_PRECIO ) {
        if( RAM_VARIAR_PRECIO || USAR_VARIACION_PRECIO_SIEMPRE ) {
            recargo = ITEM_PRECIO_SIN_IVA * NRO_VARIACION_PRECIO / 100.0;
            //fabiana 15-02-05 ** nuevo manejo de redondeos-- esta lï¿½ea estï¿½de mas   
            //ADD_MEMORY_DOUBLE( __item_precio_sin_iva, recargo );
            articulo.precio_sin_iva += recargo;
        }
    }
    /*----------------------- Calcula otros campos ---------------------------*/
    CALCULAR_DIFERENCIA_AFINIDAD();
    //fabiana 15-02-05 ** nuevo manejo de redondeos--acï¿½se recalcula el precio sin iva
    CALCULA_ARTICULO_EXT();

    SET_MEMORY_DOUBLE( __item_precio_sin_iva, articulo.precio_sin_iva );
	SET_MEMORY_CHAR( __item_cod_iva, INDICE_IVA_ARTICULO() );
	if( config_tps.ModificarAlicuotaDeArticuloConsFinal == 1 && Modificar_Alicuota == 1 ) {
		SET_MEMORY_INT( __item_cod_bajo, articulo.cod_exento );
		if( RAM_EN_IMPORTACION_DE_TICKET == 1 /*Cargando items de importacion TE*/
			|| ( RAM_MODO_DEVOLUCION 
				&& RAM_MODO_DEVOLUCION != _NC_LIBRE 
				&& RAM_MODO_DEVOLUCION != _DEVOL_LIBRE ) ) {
			if( event_e_ticket.cod_depto_old && event_e_ticket.tipo2 & _PEDIR_PRECIO )
				SET_MEMORY_DOUBLE( __item_precio_sin_iva, event_e_ticket.importe_sin_iva );
		}
	} else {
		SET_MEMORY_INT( __item_cod_bajo, 0 );
	}	
	
	//hmmancuso  para cantidades grandes puede fallar recalculo el precio sin iva exacto

   if( RAM_CANTIDAD > 15 && articulo.gravado =='S'){ //
     double precioprinter=0.00, preciocaja =0.00;
     double preciofinal =0.00, diff22=0.00;
     preciocaja = (( articulo.precio_sin_iva - articulo.imp_int ) *(1 + iva[articulo.cod_iva].porc_iva1 / 100)+ articulo.imp_int )  ; // 4.454 * 1.21 * 400 = 2155,736
     preciocaja = preciocaja * RAM_CANTIDAD;
     precioprinter = ROUND(((articulo.precio_sin_iva  - articulo.imp_int ) * (1 + iva[articulo.cod_iva].porc_iva1 / 100)) + articulo.imp_int,ENTEROS,2); //5.39
     precioprinter = precioprinter * RAM_CANTIDAD; //2156
     diff22= FABS( precioprinter - preciocaja);
     
	 if( diff22 > 0.01 && iva[articulo.cod_iva].porc_iva1> 0.00 ){ //zonamos hay que ajustar llevar de 4.454 a por lo menos 4.45454  5 decimales
   			double precio_ajustado =0.00;
   			precio_ajustado = (precioprinter / RAM_CANTIDAD); //12.15
				precio_ajustado = (( precio_ajustado -  articulo.imp_int) / ((1 + iva[articulo.cod_iva].porc_iva1 / 100))) +  articulo.imp_int ;
        			SET_MEMORY_DOUBLE( __item_precio_sin_iva, precio_ajustado );
        		}
    }
    //SET_MEMORY_FLOAT( __item_porc_ila, articulo_ext.porc_ila );
    //SET_MEMORY_FLOAT( __item_porc_iha, articulo_ext.porc_iha );
    //SET_MEMORY_FLOAT( __item_porc_ica, articulo_ext.porc_ica );
    for( i = 0;i < 4;i++ ) {
        _SET_MEMORY_FLOAT( __item_porc_impuesto, i, articulo_ext.porc_imp[i] );
        _SET_MEMORY_CHAR( __item_suma_impuesto_al_total, i, articulo_ext.sumar_al_tot[i] );
    }
    /*----------- verifica si esta habilitado y tiene presentaciones ---------*/
    if( ITEM_INHABILITADO && verificar_inhabilitado && !RAM_FLAG_COBRO_VENTA && !ITEM_COD_COBRO ) {
        rta = 0;
    }
    else {
        SET_MEMORY_INT( __item_presentacion, 1 );
        SET_MEMORY_CHAR( __item_cod_presentacion, 1 );
        rta = 1;
    }
    CR1_ADOPTA_PRESENTACION( 1, SI );
    if( !ITEM_INHABILITADO && RAM_CLIENTE_AFINIDAD && UTILIZAR_PRECIO_AFINIDAD
     && !RAM_CANT_PROMOCIONES && !ITEM_PEDIR_PRECIO ) {
        ADD_MEMORY_DOUBLE( __ram_diferencia_afinidad, ( precio_item - ITEM_PRECIO ) * RAM_CANTIDAD );
        if( precio_item != ITEM_PRECIO ) {
            if( ITEM_PESABLE ) {
                ADD_MEMORY_INT( __ram_cant_articulos_afinidad, RAM_ANULACION ? -1 : 1 );
            }
            else {
                ADD_MEMORY_INT( __ram_cant_articulos_afinidad,
                                ( int )( RAM_ANULACION ? ( -1 )*RAM_CANTIDAD : RAM_CANTIDAD ) );
            }
        }
    }
    return ( rta );
}
/***********************************************************************/
int OBTENER_COD_GRAVADO( void )
/***********************************************************************/
{
    int rta = 0;

    if( EXENTO_PARA_DIPLOMATICOS() ) {
        rta = 0;
    }
    else if( articulo.gravado == 'S' ) {
        rta = 1;
    }
    else if( articulo.gravado == 'E' ) {
        rta = 2;
    }
    else {
        rta = 0;
    }
    return( rta );
}
/***********************************************************************
void CALCULA_IMPUESTOS(  )
***********************************************************************
{
   articulo.porc_ila = impuestos[articulo.cod_impuesto[0]];
   articulo.porc_iha = impuestos[articulo.cod_impuesto[1]];
}*/

/***********************************************************************/
void MODIFICAR_ALICUOTA_DE_ARTICULO( )
/***********************************************************************/
{
   	if( config_tps.ModificarAlicuotaDeArticuloConsFinal == 1 ) {
		int subtipo = 0, tipo_comprobante = 0;
		if( !Modificar_Alicuota ) {
			tipo_comprobante = TIPO_COMPROBANTE( &subtipo );
			if( RAM_NRO_CLIENTE > 0 && clientes.cod_cliente == RAM_NRO_CLIENTE  ) {
				//RAM_COND_IVA_CLIENTE;
				if( ES_CONSUMIDOR_FINAL( clientes.cond_iva ) ){
					Modificar_Alicuota = 1;
				}			
			} else {
				if( tipo_comprobante == 5 ) {
					Modificar_Alicuota = 1;
				}
			}
		}
		if( Modificar_Alicuota == 1 ) {
			if( articulo.cod_exento > 0 ) {
				char aux[150];
				//Se modifica el alicuota al articulo en analisis 
				memset( aux, 0, sizeof( aux ) );

				if( articulo.cod_exento <= MAX_COD_IVA
						&& DIRECT_DB_QUERY("select * from articulo_iva where cod_iva=%i", articulo.cod_exento)  > 0 ) { //= 0->No existe registro, = 1->Existe registro
					articulo.cod_iva = articulo.cod_exento;
				} else {
					_snprintf(aux, sizeof(aux) - 1, "cod_exento %i inexistente en tabla articulo_iva (Art. COD_ART %ld COD_BARRA %s)",
							articulo.cod_exento, articulo.cod_interno, articulo.cod_barra );
					GRABAR_LOG_SISTEMA(aux, LOG_ERRORES, 1);
				}
			}
		}

	}
}
/***********************************************************************/
int ALICUOTA_MODIFICADO( int cod_exento, char *cod_barra, long cod_interno )
/***********************************************************************/
{
	int _cod_iva=-999, iva = -1;
	
	if( config_tps.ModificarAlicuotaDeArticuloConsFinal == 1 ) {
		if( cod_exento > 0 ) {
			DIRECT_DB_QUERY("select cod_iva from articulo where cod_interno=%ld and codigo_barra='%s'",
				cod_interno, cod_barra );
			GET_DATO(1, C_INT, (char *) &_cod_iva, 4);
			if( _cod_iva!=-999 && _cod_iva <= MAX_COD_IVA) {
				iva = _cod_iva;//Cod iva del padron
			}
		} 
	}

	return iva;
}


/***********************************************************************/
void PREPARA_REGISTRO_ARTICULOS( int precio_original )
/***********************************************************************/
{
    int signo = 1/*, area_ant*/;
	double tot = 0.0;
	int tabla_ant = SELECTED2();
	int tipo_ant = SELECTED_TIPO();

    SELECT_TABLE( T_ARTICULO, TT_ORIG );
	MODIFICAR_ALICUOTA_DE_ARTICULO( );	
	 if( FOUND2()  && ( articulo.cr_habilitado == 'S' && ( RAM_USAR_PRECIO_CR || USAR_PRECIO_CREDITO ) )
	 || articulo.habilitado_oferta == 'S' ) {
        if( precio_original ) {
            struct _articulo_ext articulo_aux;
            articulo_aux.iva1 = 0.0;
            articulo_aux.iva2 = 0.0;
	
			if( PRECALCULAR_IMPUESTOS ) {
				double tasa[4];
				int j = 0, x = 0, i = 0;
				
				memset( tasa, 0, sizeof( double )* 4 );

				while( j < 40 ) {
					if( articulo.cod_impuesto[i] > 0
					&& ( impuestos[j].cod_impuesto == articulo.cod_impuesto[i] ) ) {
						tasa[x] = impuestos[j].tasa / 100.0;
						x++;
					}
					j++;
				}

				for( i = 0; i < 4; i++ ) {
					if( tasa[i] != 0.00 )
						tot += articulo.precio_sin_iva * tasa[i];
				}
			}

		    articulo_aux.precio = articulo.precio_sin_iva - articulo.imp_int + tot;
            articulo_aux.ila = articulo_aux.precio * articulo.porc_ila / 100.0;
            articulo_aux.iha = ( RAM_FACTURA ) ? articulo_aux.precio * articulo.porc_iha / 100.0
                             : 0;
            if( articulo.gravado == 'S' && !EXENTO_PARA_DIPLOMATICOS() ) {
                // articulo_ext.iva1 = articulo_ext.precio * config.porc_iva1 /
                // 100.0;
                articulo_aux.iva1 = articulo_aux.precio * iva[INDICE_IVA_ARTICULO()].porc_iva1
                                  / 100.0;
                if( EMITIR_FACTURA()
                 && ( !FACT_INDEP_DEL_TIPO_DE_CLIENTE && clientes.cond_iva == RESP_NO_INSC ) ) {
                    // articulo_ext.iva2 = articulo_ext.precio * config.porc_iva2 /
                    // 100.0;
                    articulo_aux.iva2 = articulo_aux.precio * iva[INDICE_IVA_ARTICULO()].porc_iva2
                                      / 100.0;
                }
                articulo_aux.precio = articulo_aux.precio + articulo.imp_int + articulo_aux.iva1
                                    + articulo_aux.iva2;
			}
            else {
                articulo_aux.precio = articulo.precio_sin_iva + articulo.imp_int;
            }
            articulo_aux.precio = ROUND( articulo_aux.precio + articulo_aux.ila + articulo_aux.iha,
                                         ENTEROS, DECIMALES );
            articulo.precio_sin_iva = articulo_aux.precio - articulo_aux.iva1 - articulo_aux.iva2
                                    - articulo_aux.ila - articulo_aux.iha;
            SET_MEMORY_DOUBLE( __item_precio_original, articulo_aux.precio );
		}
        if( articulo.precio_sin_iva < 0 ) {
            signo = -1;
        }
        articulo.precio_sin_iva = articulo.precio_sin_iva_cr * signo;
		SET_MEMORY_DOUBLE( __item_precio_original, ITEM_PRECIO_ORIGINAL * signo );
    } 
	else {
	     SET_MEMORY_DOUBLE( __item_precio_original, 0.00 );
    }
	//SELECT( area_ant );
	SELECT_TABLE( tabla_ant, tipo_ant );
}
/***********************************************************************/
void CREAR_ARCHIVOS_SPAR()
/***********************************************************************/
{
    #ifdef COMPILAR_ASC
    CEREAR_ARCHIVO_BINARIO( _TARJETAS_ASC );
    CEREAR_ARCHIVO_BINARIO( _CHEQUES_ASC );
    CEREAR_ARCHIVO_BINARIO( _TICKET_ASC );
    CEREAR_ARCHIVO_BINARIO( _MUTUAL_ASC );
    #endif
}

/***********************************************************************/
void CEREAR_ARCHIVO_BINARIO( int cod_archivo )
/***********************************************************************/
{
    int handle;
    handle = CREAT_( cod_archivo, S_IFREG );
    _CLOSE( handle, __LINE__, __FILE__ );
}
/***********************************************************************/
int INDICE_IVA_ARTICULO()
/***********************************************************************/
{
    int indice = 0;
    indice = ( ( articulo.cod_iva >= MAX_COD_IVA ) ? 0 : articulo.cod_iva );
    if( iva[indice].porc_iva1 < .009 ) {
		//Los articulo exentos estan cargados con porc_iva1 = 0
		if( !DIRECT_DB_QUERY("select * from articulo_iva where cod_iva=%i", indice)  > 0 ) { //= 0->No existe registro, = 1->Existe registro
			 /* no solo asigna sobre los items tickeados Si no tambien sobre los
			 * consultados. Por lo tanto si no se tickean van a salir en el
			 * informe00 con importe = 0 */
			if( !( config_tps.ModificarAlicuotaDeArticuloConsFinal == 1 
					&& articulo.cod_exento == indice ) ){ 
				iva[indice].porc_iva1 = iva[0].porc_iva1;
				iva[indice].porc_iva2 = iva[0].porc_iva2;
				strcpy( iva[indice].nombre_iva, iva[0].nombre_iva );
			}
		}
    }
    return indice;
}
/***********************************************************************/
void CIERRA_TODOS_LOS_ARCHIVOS()
/***********************************************************************/
{
    CLOSE_ALL();
    //  CLOSE_RNV_VIRTUAL();
    PING_CLOSE_SOCKET();
    //CLOSE_COPIA_VOUCHER_PRN();
    //CLOSE_COPIA_TICKET_PRN();
	/*for( h = 4;h < 100;h++ ) {
        #ifdef INVEL_L
        close( h );
        #else
		//Este _close produce error produciendo la caida de la caja
		//_VALIDATE_CLEAR_OSSERR_RETURN((_osfile(fh) & FOPEN), EBADF, -1);
		//de la funcion _close

        //_close( h );
        #endif
    }*/
}
/***********************************************************************/
void ALTA_ARTICULO( struct _articulo *altas, struct _present *altas_present, int *_c1, int *_c2 )
/***********************************************************************/
{
    //SELECT( AREA_ARTICULOS );
	SELECT_TABLE( T_ARTICULO, TT_ORIG );
    memcpy( &articulo, altas, sizeof( struct _articulo ) );
    articulo.autoinc = 0;
    #ifdef INVEL_L
    //BEGIN_TRANSACTION();
    #endif
    INSERT2(SI);
    #ifdef INVEL_L
    //END_TRANSACTION();
    #endif
    ( *_c1 )++;
    /*------------------ Procesa las presentaciones -----------------*/
    if( PRESENTACIONES_HABILITADAS ) {
        //SELECT( AREA_AUX2 );
		SELECT_TABLE( T_PRESENT, TT_ALTA );
        ind_present.autoinc = altas->autoinc;
        ind_present.cod_present = 0;
		/*SET_WHERE("CONCAT(AUTOINC,COD_PRESENT) >= CONCAT(%ld,%i)",
						ind_present.autoinc,
						ind_present.cod_present);
		SET_ORDER2("AUTOINC,COD_PRESENT");*/
        SET_WHERE(" AUTOINC >= '%ld' ", ind_present.autoinc );
        SET_ORDER2("AUTOINC");
		RUN_QUERY(NO);
        //GET_GREATER_E( ( char* )&ind_present );
        while( !dbEOF() && altas_present->autoinc == altas->autoinc ) {
            //SELECT( AREA_PRESENT );
			SELECT_TABLE( T_PRESENT, TT_ORIG );
            memcpy( &present, altas_present, sizeof( struct _present ) );
            present.autoinc = articulo.autoinc;
            ( *_c2 )++;
            #ifdef INVEL_L
            //BEGIN_TRANSACTION();
            #endif
            INSERT2(SI);
            #ifdef INVEL_L
            //END_TRANSACTION();
            #endif
            //SELECT( AREA_AUX2 );
			SELECT_TABLE( T_PRESENT, TT_ALTA );
            SKIP2( 1 );
        }
    }
}
/***********************************************************************/
int ABRIR_STRING_TABLE()
/***********************************************************************/
{
    int error;
    //struct _stringt STRTABLE;
    //error = USE_DATABASE( AREA_STRING_TABLE, "STRINGT.SIC", ( char* )string_table,
    //                      sizeof( struct _stringt ), NULL, 0, 0 );
	memset( string_table, 0, sizeof(struct _stringt) );
    error = OPEN_TABLE( T_STRINGT, TT_ORIG, ( char* )string_table, sizeof( struct _stringt ) );

    //SELECT( AREA_STRING_TABLE );

    /*while(!BtrvEOF()){
    memcpy((void *)&STRTABLE, (void *)string_table,(int)sizeof( struct _stringt ));
    INSERT2(NO);
    SKIP(1);
    }*/



    return error;
}
/***********************************************************************/
int ABRIR_ARCHIVO_BOLETIN_TCOMPRA_MUTUALES()
/***********************************************************************/
{
		if( !OPENED( T_BOLETIN, TT_ORIG ) ) {
            if( OPEN_TABLE( T_BOLETIN, TT_ORIG, ( char* )&boletin_prot, sizeof( boletin_prot ) ) == 0 ) {
                archivo_boletin_abierto = 1;
            }
            else {
                archivo_boletin_abierto = 0;
            }
        }
        else {
            archivo_boletin_abierto = 1;
        }
    return archivo_boletin_abierto;
}
/***********************************************************************/
void CALCULAR_DIFERENCIA_AFINIDAD( void )
{
    /***********************************************************************/
    precio_item = 0;
    if( !ITEM_INHABILITADO && RAM_CLIENTE_AFINIDAD && !RAM_STRING_CLIENTE_AFINIDAD && UTILIZAR_PRECIO_AFINIDAD ) {
        SET_MEMORY_INT( __item_presentacion, 1 );
        SET_MEMORY_CHAR( __item_cod_presentacion, 1 );
        CR1_ADOPTA_PRESENTACION( 1, SI );
        precio_item = ITEM_PRECIO;
        articulo.precio_sin_iva = articulo.precio_sin_iva_cr;
        SET_MEMORY_CHAR( __ram_utiliza_precio_afinidad, 1 );
    }
}
/***********************************************************************/
int CLIENTE_GUBERNAMENTAL( void )
/***********************************************************************/
{
	int	 ok = 0;

    //Diplomáticos Y Exportación exentos para El Salvador, cuando les hacen fact B
    if( ( RAM_NRO_CLIENTE > 0 ) 
        && ( FACT_INDEP_DEL_TIPO_DE_CLIENTE )
        && (  clientes.cond_iva == INST_GUBERNAMENTAL  )  
        && ( clientes.exento_ing_brutos == 'N'
            || clientes.exento_ing_brutos == 'n' )
        && (  RAM_FACTURA  ) ) {
		ok = 1;
	}

  	return ok;
}

/***********************************************************************/
int BUSCA_COD_INTERNO_COD_BARRA( long cod, char *cod_barra )
/***********************************************************************/
{

	SELECT_TABLE( T_ARTICULO, TT_ORIG );
	SET_WHERE("COD_INTERNO = %ld and CODIGO_BARRA = '%s'", cod, cod_barra);
			RUN_QUERY(NO);
    PREPARA_REGISTRO_ARTICULOS( SI );
    return ( FOUND2() );
}
/***********************************************************************/
int ASIGNA_COD_ESPECIAL_COD_BARRA( long cod, char *cod_barra )
/***********************************************************************/
{
    int rta = 0;
    if( BUSCA_COD_INTERNO_COD_BARRA( cod, cod_barra ) ) {
        rta = ASIGNA_ITEM( NO );
    }
    return ( rta );
}
/***********************************************************************/
int EXENTO_PARA_DIPLOMATICOS( void )
/***********************************************************************/
{
    int ok = 0;
    //Diplomï¿½icos Y Exportacin exentos para el salvador, cuando se les hace factura B
    //Se comenta "imprimiendo_factura_journal" porque cuando se imprime el Journal no esta calculando 
	//correctamente los importes para cliente DIPLOMATICO_EXPORTACION ya que no valida la condicion
	//por ende se esta imprimiendo incorrectamente los importes.
	//if(!imprimiendo_factura_journal)
	//{
    if( ( RAM_NRO_CLIENTE > 0 )
            && ( FACT_INDEP_DEL_TIPO_DE_CLIENTE )
            && ( clientes.cond_iva == DIPLOMATICO_EXPORTACION )
     && ( ( RAM_FACTURA == FACTURA_TIPO_B ) ) ) {
        ok = 1;
    }
    //}

    return ok;
}
/****************************************************************************/
int ABRE_DEUDAS()
/****************************************************************************/
{
		//Eliminar (Servicios)
    return 0;
}
/****************************************************************************/
int CIERRA_DEUDAS()
/****************************************************************************/
{
		//Eliminar (Servicios)
    return 0;
}
/***********************************************************************/
int ABRIR_ARCHIVO_SOCIO( void )
/***********************************************************************/
{
        if( !OPENED( T_CLI_COBRO, TT_ORIG ) ) {
            if( OPEN_TABLE( T_CLI_COBRO, TT_ORIG , ( char* )&cli_cobro, sizeof( cli_cobro ) ) == 0 ) {
                archivo_clicobro_abierto = 1;
            }
            else {
                archivo_clicobro_abierto = 0;
            }
        }
        else {
            archivo_clicobro_abierto = 1;
        }
    return archivo_clicobro_abierto;
}

/***********************************************************************/
int ABRIR_ARCHIVO_PRESENTACIONES(  )
/***********************************************************************/
{
	int	error = 0;

    if( PRESENTACIONES_HABILITADAS && !/*USED( AREA_PRESENT )*/ OPENED( T_PRESENT, TT_ORIG ) ) {
        error += //USE_DB( AREA_PRESENT, _PRESENT_SIC, ( char* )&present, sizeof( present ), NULL, 0,
                        // 0 );
				 OPEN_TABLE(T_PRESENT, TT_ORIG ,( char* )&present, sizeof( present ));
    }

	return error;
}

