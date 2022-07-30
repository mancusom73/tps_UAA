#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
//#include <io.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <driver.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <htoa.h> 
#include <malloc.h> 
#include <mensaje.h> 
#include <pausa.h> 
#include <pos_reg.h> 
#include <tkt.h> 
#include <_cr1.h> 
#include <cmemory.h>
#include <db.h>
#include <deftables.h>
#include <path.h>
#include <cio.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
#undef EOF
#include <memory.h>
#include <btrv.h>
#include <stdlib.h>
#include "path.h"
#include <myodbc.h>
#include <sys/stat.h>
#pragma pack(1)
#define DEBUG_DBROUTER( X ) GRABAR_LOG_DBROUTER( X )
/* Operacion de Testeo DBRouter ****************************************** */
int TEST_DBROUTER_ON_LINE( void )
/**************************************************************************/
{
    return _ACTUALIZAR_TOTAL_X( 1 );
}

/* Obtain en DBRouter *************************************************** */
/*int DBROUTER_OBTENER_TABLA_SQL( int pTabla, int pTipo, char *nombre_tabla, char *codicion_select,
                                int vaciar )*/
/*
* pTabla           = nro Tabla original
* pTipo            = tipo de tabla: tmp, bak, his etc..
* nombre_tabla     = Significado nuevo: nombre de la tabla en la base de datos remota (i.e. la tabla solicitada)
                   = Significado viejo: nombre de la tabla en las Bases de Datos (ej bancos). (en ambas bases, debe llamarse igual)
* condicion_select = codicion para la restriccion de registros.
* ************************************************************************** */
/*{
    char *buffer_para_recibir, *btrieve_buffer;
    //Nombre de la tabla en la base local
    char nombre_tabla_orig[35];
    //Nombre de la tabla base
    char nombre_tabla_base[35];
    //Nombre de la tabla de trabajo para la obtencion;
    char nombre_tabla_work[35];
    int registros_obtenidos = 0;
    int secuencia_transaccion = -1;
    int resp_tmp, bytes = 0;              // bytes leidos desde TCP-IP.
    char texto[250];
	int tabla_ant=0, tipo_ant=0;
    #ifdef INVEL_L
    char index_vacio[32],index_bak[32],index_original[32];
    char nombre_bak[32],nombre_vacio[32],*p;
    #else
    #endif

    struct _Peticion peticion;
    int rta = 1;
    struct
    {
        int rta;
        char *buffer;
        int l_buffer;
    } rta_drv;
    //long pos;
    if( ( buffer_para_recibir =(char *) MALLOC( MAXIMO_BUFFER_TRANSACCION_RECIBIR + 1 ) ) == NULL ) {
        // Error de Memoria.
        return 0;
    }
    if( ( btrieve_buffer = (char *)MALLOC( 1024 ) ) == NULL ) {
        // Error de Memoria.
        free( buffer_para_recibir );
        return 0;
    }
    
    //Nombres en la base local
    rta = GET_TABLE_NAME( pTabla, TT_ORIG, nombre_tabla_base, sizeof( nombre_tabla_base ) );
    rta = GET_TABLE_NAME( pTabla, TT_WORK_DBROUT, nombre_tabla_work, sizeof( nombre_tabla_work ) );
    rta = GET_TABLE_NAME( pTabla, pTipo, nombre_tabla_orig, sizeof( nombre_tabla_orig ) );
    
    // LOG
    if( ( strcmp( "script", nombre_tabla ) != 0 && strcmp( "SCRIPT", nombre_tabla ) != 0 ) && rta ) {
        sprintf( btrieve_buffer, "Obteniendo Tabla %s -> %s", nombre_tabla, nombre_tabla_orig );
        GRABAR_LOG_DBROUTER( btrieve_buffer );
    }
    //-------------------------------------------------
    // ARMAR-ENVIAR LA PETICION DE OBTENER TABLA
    //-------------------------------------------------

	tabla_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    if( rta ) {

        memset( btrieve_buffer, 0, 1024 );
        //  //Elimina la tabla temporal por si hubo un corte de luz 
        //  DIRECT_DB_QUERY( "drop table %s_work_dbrout", nombre_tabla_orig );
        //  //Crea la tabla temporal en base a la tabla original sin datos. Por eso WHERE 1=0
        //  DIRECT_DB_QUERY( "create table %s_work_dbrout  select * from %s where 1=0", nombre_tabla_orig,
        //                 nombre_tabla_base );

        DROP_TABLE( pTabla, TT_WORK_DBROUT );
        resp_tmp = !OPEN_TABLE( pTabla, TT_WORK_DBROUT, ( char* )btrieve_buffer, 0 );

        // Armar los contenidos del Header de la peticion.
        peticion.Header.wCodPet = OBTENER_REGISTROS;
        peticion.Header.wBufferLength = 0;
        strncpy( peticion.Header.szTabla, nombre_tabla, 29 );
        peticion.Header.szTabla[29] = 0;
        peticion.Buffer.sData = 0;
        peticion.Header.bNull = 0;
        if( codicion_select != NULL ) {
            peticion.Header.wSelectLength = strlen( codicion_select ) + 1;
            peticion.Buffer.szSelectCondition = codicion_select;
        }
        else {
            peticion.Header.wSelectLength = 0;
            peticion.Buffer.szSelectCondition = 0;
        }
        // Insertar la peticion.
		EMPEZAR_VTRANSACCION();
        if( INCLUIR_PETICION( &peticion ) ) {
            // Envia las transacciones pendientes.
            if( ENVIAR_VTRANSACCION( 1, 0 ) ) {
                //-------------------------------------------------
                // ESPERAR LA RESPUESTA DESDE EL DBROUTER - HOST
                //-------------------------------------------------
                //resp_tmp = !OPEN_TABLE_TMP( T_TEMP, nombre_tabla_orig, btrieve_buffer );
                //SELECT_TABLE( T_TEMP, 0 );
                SELECT_TABLE( pTabla, TT_WORK_DBROUT );
                if( resp_tmp ) {
                    do {
                        int h,i;
                        bytes = 0;
                        // Escuchar TCP-IP hasta recibir algo o salir por
                        // TIME-OUT
                        //for( h = 0; h < 100 * TIME_OUT_SERVER_NO_RESPONDIO; h++ ) {
                        for( h = 0;h < 15 * TIME_OUT_SERVER_NO_RESPONDIO;h++ ) {
                            DRIVER( _DRV_tcp, _TCP_status, 0, &bytes, sizeof( bytes ) );
                            if( bytes != 0 ) {
                                break;
                            }
                        }
                        if( bytes ) {
                            #if defined(INVEL_W) || defined(INVEL_L)
                            rta_drv.buffer = buffer_para_recibir;
                            rta_drv.l_buffer = MAXIMO_BUFFER_TRANSACCION_RECIBIR;
                            DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
                            rta_drv.rta = 0;
                            #else
                            rta_drv.buffer = puntero_memoria_real_driver_invel;
                            rta_drv.l_buffer = MAXIMO_BUFFER_TRANSACCION_RECIBIR;
                            DRIVER( _DRV_tcp, _TCP_recibir, 0, &rta_drv, sizeof( rta_drv ) );
                            memcpy( buffer_para_recibir, puntero_memoria_driver_invel,
                                    MAXIMO_BUFFER_TRANSACCION );
                            #endif
                            if( !rta_drv.rta ) {
                                //-------------------------------------------------
                                // RECIBE LA RESPUESTA DESDE EL DBROUTER -
                                // HOST
                                //-------------------------------------------------
                                unsigned short nro_caja;
                                long nro_transaccion;
                                int ultima_secuencia = -1;
                                char *stmp = buffer_para_recibir;
                                nro_caja = *( unsigned short* )stmp;
                                stmp += sizeof( unsigned short );
                                nro_transaccion = *( long* )stmp;
                                stmp += sizeof( long );
                                if( nro_caja == NRO_CAJA && nro_transaccion == ultima_transaccion ) {
                                    unsigned short secuencia;
                                    unsigned short cant_item = 0;
                                    char error_flag;
                                    secuencia_transaccion++;
                                    error_flag = *( char* )stmp;
                                    stmp += sizeof( char );
                                    secuencia = *( unsigned short* )stmp;
                                    stmp += sizeof( unsigned short );
                                    cant_item = *( unsigned short* )stmp;
                                    stmp += sizeof( unsigned short );
                                    // sprintf(texto, "cant items: %d",
                                    // cant_item);
                                    // DEBUG_DBROUTER(texto);
                                    if( secuencia_transaccion != secuencia ) {
                                        sprintf( texto, "TRANSACCION FUERA SECUENCIA: %d %d",
                                                 secuencia_transaccion, secuencia );
                                        GRABAR_LOG_DBROUTER( texto );
                                        rta = 0;
                                    }
                                    //
                                    // RECORRE LAS PETICIONES DEN DE LA
                                    // TRANSACCION.
                                    //
                                    if( error_flag ) {
                                        GRABAR_LOG_ID_DBROUTER( SDBR_RETORNO_ERROR_FLAG_ON );
                                        rta = 0;
                                    }
                                    else {
                                        for( i = 0;rta && i < cant_item;i++ ) {
                                            //int length = 0;
                                            unsigned short item_secuencia;
                                            short estado;
                                            item_secuencia = *( unsigned short* )stmp;
                                            // check item_secuencia
                                            stmp += sizeof( unsigned short );
                                            estado = *( short* )stmp;
                                            stmp += sizeof( short );
                                            ultima_secuencia++;
                                            if( ultima_secuencia != item_secuencia ) {
                                                sprintf( texto,
                                                         "ITEM TRANSACCION FUERA SECUENCIA  %d %d",
                                                         ultima_secuencia, item_secuencia );
                                                GRABAR_LOG_DBROUTER( texto );
                                                rta = 0;
                                                break;
                                            }
                                            // Chequeo de Estado
                                            if( estado < 0 ) {
                                                // Registro con Error;
                                                GRABAR_LOG_ID_DBROUTER(
                                                SDBR_RETORNO_ERROR_FLAG_ON );
                                                rta = 0;
                                            }
                                            else if( estado == 0 ) {
                                                // Ultimo Registro Obtenido
                                                // OK;
                                                registros_obtenidos = 1;
                                                if( cant_item != i + 1 ) {
                                                    sprintf( texto,
                                                             "RECIBIDA EOF FLAG INVALIDA: %d %d",
                                                             cant_item, i + 1 );
                                                    GRABAR_LOG_DBROUTER( texto );
                                                    rta = 0;
                                                }
                                            }
                                            else {
                                                // (estado > 0) Registro
                                                // Obtenido.
                                                if( ( stmp + estado )
                                                  > ( MAXIMO_BUFFER_TRANSACCION
                                                    + buffer_para_recibir ) ) {
                                                    sprintf(
                                                    texto,
                                                    "PTRO FUERA DE RANGO buffer: %p bytes: %d. Ptro: %p Estado: %d",
                                                    buffer_para_recibir,
                                                    MAXIMO_BUFFER_TRANSACCION,
                                                    stmp, estado );
                                                    GRABAR_LOG_DBROUTER( texto );
                                                    rta = 0;
                                                }
                                                else {
                                                    memcpy( btrieve_buffer, stmp, estado );
                                                    //pos = GET_POSITION();
                                                    #ifdef INVEL_L
                                                    //BEGIN_TRANSACTION();
                                                    #endif
                                                    if( INSERT2( NO ) != 0 ) {
                                                        sprintf(
                                                        texto,
                                                        "Error. Insertando en SQL Tabla %s ",
                                                        nombre_tabla_work );
                                                        GRABAR_LOG_DBROUTER( texto );
                                                        rta = 0;
                                                    } 
                                                    #ifdef INVEL_L
                                                    //END_TRANSACTION();
                                                    #endif
                                                    //GET_DIRECT(pos);
                                                    stmp += estado;
                                                }
                                            }
                                        }
                                    }
                                }
                                else {
                                    sprintf( texto, "ERROR DE RUTEO: (%d,%li) ESPERADA (%d,%li)",
                                             nro_caja, nro_transaccion, NRO_CAJA,
                                             ultima_transaccion );
                                    GRABAR_LOG_DBROUTER( texto );
                                    rta = 0;
                                }
                            }
                            else {
                                rta = 0;
                                // GRABAR_LOG_ID_DBROUTER(
                                // SDBR_RETORNO_ERROR_TCP_IP );
                                sprintf( texto, "SDBR_RETORNO_ERROR_TCP_IP %d", rta_drv.rta );
                                GRABAR_LOG_DBROUTER( texto );
                            }
                        }
                        else {
                            GRABAR_LOG_ID_DBROUTER( SDBR_TIME_OUT_RETORNO_TCP_IP );
                            rta = 0;
                        }
                    }
                    while( rta && !registros_obtenidos );
                    #ifdef INVEL_W
                    //borra los datos la original segun tipo
                    //DIRECT_DB_QUERY( "truncate table %s", nombre_tabla_orig );
                    CLOSE_TABLE( pTabla, TT_WORK_DBROUT );
                    if( rta ) {
                        OPEN_TABLE( pTabla, pTipo, ( char* )btrieve_buffer, 0 );
                        ZAP2();
                        //Copia todos los datos de la temporal a la original tipo
                        //DIRECT_DB_QUERY( "insert into %s select * from %s_temp", nombre_tabla_orig, nombre_tabla_orig );
                        DIRECT_DB_QUERY( "insert into %s select * from %s", nombre_tabla_orig, nombre_tabla_work );
                        //Elimina la tabla temporal
                        //DIRECT_DB_QUERY( "drop table %s_temp", nombre_tabla_orig );
                    }
                    DROP_TABLE( pTabla, TT_WORK_DBROUT );
                    CLOSE_TABLE( pTabla, pTipo );
                    #endif
                }
                else {
                    rta = 0;
                }
            }
            else {
                rta = 0;
            }
        }
        else {
            rta = 0;
        }
    }
    free( buffer_para_recibir );
    free( btrieve_buffer );
    //libera la tabla temporal
    CLOSE_TABLE( T_TEMP, 0 );
    ELIMINAR_VTRANSACCION();
    if( !rta ) {
        if( strcmp( "script", nombre_tabla ) == 0 ) {
            GRABAR_LOG_DBROUTER( "Obteniendo Tabla Script. Error" );
        }
        else {
            GRABAR_LOG_ID_DBROUTER( SDBR_TABLA_OBTENIDA_ERROR );
        }
        CERRAR_CANAL_DBR();
    }
	SELECT_TABLE( tabla_ant, tipo_ant );
    #ifdef INVEL_L
    if( !rta ) {
        //rta &= !COPIAR_ARCHIVO( nombre_bak, nombre_original, __LINE__, __FILE__ );
        //rta &= !COPIAR_ARCHIVO( index_bak, index_original, __LINE__, __FILE__ );
    }
    #endif
    return rta;
}*/
/***********************************************************************/
int DBROUTER_REQ_CON_NULL( int pet_code, int cant_col, char *patron_nulls, char *nombre_tabla,
                           char *buffer, int tamano_buffer, char *where, int espera_respuesta )
/*    pet_code         = Codigo de Peticion (INSERT, DELETE o UPDATE).
* cant_col         = Cantidad de Columnas que tiene la tabla.
* patron_nulls     = Buffer que contiene los 0 y 1s para columnas nulas.
* nombre_tabla     = Buffer que contiene el nombre de la tablas (ej. eventos)
* buffer           = Apunta al data buffer a insertar (ej. &sript).
* tamano_buffer    = Tamano del buffer.
* where            = Agrega condicion de restriccion a la tabla.
* espera_respuesta = si espera confirmacion ACK desde DBR.
* ************************************************************************** */
{
    struct _Peticion peticion;
    char *buffer_con_nulls = 0;
    int rta = 0;
    if( !EMPEZAR_VTRANSACCION() ) {
        ELIMINAR_VTRANSACCION();
        if( !EMPEZAR_VTRANSACCION() ) {
            return 0;
        }
    }
    if( pet_code != ACTUALIZAR_REGISTRO && pet_code != BORRAR_REGISTRO
     && pet_code != INSERTAR_REGISTRO ) {
        return 0;
    }
    buffer_con_nulls = malloc( tamano_buffer + cant_col );
    if( !buffer_con_nulls ) {
        return 0;
    }
    memset( buffer_con_nulls, 0, tamano_buffer + cant_col );
    // Crear el buffer - NULL VECTOR - RAW
    memcpy( buffer_con_nulls, patron_nulls, cant_col );
    memcpy( buffer_con_nulls + cant_col, buffer, tamano_buffer );
    peticion.Header.wCodPet = pet_code;
    strncpy( peticion.Header.szTabla, nombre_tabla, 29 );
    peticion.Header.szTabla[29] = 0;
    peticion.Buffer.sData = buffer_con_nulls;
    peticion.Header.wBufferLength = tamano_buffer + cant_col;
    peticion.Header.bNull = 1;
    if( where ) {
        peticion.Header.wSelectLength = strlen( where ) + 1;
        peticion.Buffer.szSelectCondition = where;
    }
    else {
        peticion.Header.wSelectLength = 0;
        peticion.Buffer.szSelectCondition = 0;
    }
    // Insertar la peticion.
    if( INCLUIR_PETICION( &peticion ) )
                    // Envia las transacciones pendientes.
    {
        if( ENVIAR_VTRANSACCION( 0, espera_respuesta ) ) {
            rta = 1;
        }
    }
    free( buffer_con_nulls );
    ELIMINAR_VTRANSACCION();
    return rta;
}
/**************************************************************************/
int DBROUTER_INSERT( char *nombre_tabla, char *buffer, int tamano_buffer, int espera_respuesta )
/*    nombre_tabla     = Buffer que contiene el nombre de la tablas (ej. eventos)
* buffer           = Apunta al data buffer a insertar (ej. &sript).
* tamano_buffer    = Tamano del buffer.
* espera_respuesta = si espera confirmacion ACK desde DBR.
* ************************************************************************** */
{
    return DBROUTER_REQ( INSERTAR_REGISTRO, nombre_tabla, buffer, tamano_buffer, 0,
                         espera_respuesta );
}
/**************************************************************************/
int DBROUTER_DELETE( char *nombre_tabla, char *where, int espera_respuesta )
/*    nombre_tabla     = Buffer que contiene el nombre de la tablas (ej. eventos)
* where            = Agrega condicion de restriccion a la tabla.
* espera_respuesta = si espera confirmacion ACK desde DBR.
* ************************************************************************** */
{
    return DBROUTER_REQ( BORRAR_REGISTRO, nombre_tabla, 0, 0, where, espera_respuesta );
}
/**************************************************************************/
int DBROUTER_REQ( int pet_code, char *nombre_tabla, char *buffer, int tamano_buffer, char *where,
                  int espera_respuesta )
/*    pet_code         = Codigo de Peticion (INSERT, DELETE o UPDATE).
* nombre_tabla     = Buffer que contiene el nombre de la tablas (ej. eventos)
* buffer           = Apunta al data buffer a insertar (ej. &sript).
* tamano_buffer    = Tamano del buffer.
* where            = Agrega condicion de restriccion a la tabla.
* espera_respuesta = si espera confirmacion ACK desde DBR.
* ************************************************************************** */
{
    struct _Peticion peticion;
    int rta = 0;
    if( !EMPEZAR_VTRANSACCION() ) {
        ELIMINAR_VTRANSACCION();
        if( !EMPEZAR_VTRANSACCION() ) {
            return 0;
        }
    }
    if( pet_code != ACTUALIZAR_REGISTRO && pet_code != BORRAR_REGISTRO
     && pet_code != INSERTAR_REGISTRO ) {
        return 0;
    }
    if( !buffer ) {
        tamano_buffer = 0;
    }
    peticion.Header.wCodPet = pet_code;
    peticion.Header.wBufferLength = tamano_buffer;
    strncpy( peticion.Header.szTabla, nombre_tabla, 29 );
    peticion.Header.szTabla[29] = 0;
    peticion.Buffer.sData = buffer;
    peticion.Header.bNull = 0;
    if( where ) {
        peticion.Header.wSelectLength = strlen( where ) + 1;
        peticion.Buffer.szSelectCondition = where;
    }
    else {
        peticion.Header.wSelectLength = 0;
        peticion.Buffer.szSelectCondition = 0;
    }
    // Insertar la peticion.
    if( INCLUIR_PETICION( &peticion ) )
                    // Envia las transacciones pendientes.
    {
        if( ENVIAR_VTRANSACCION( 0, espera_respuesta ) ) {
            rta = 1;
        }
    }
    ELIMINAR_VTRANSACCION();
    return rta;
}
/******************************************************************************/
void GRABAR_LOG_ID_DBROUTER( int id, int origen, int nivel){
/******************************************************************************/
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		_GRABAR_LOG_ID_DBROUTER( id);
}


/******************************************************************************/
void _GRABAR_LOG_ID_DBROUTER( int id)
/******************************************************************************/
{
    int area_ant, tipo_ant;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_STRING_TABLE );
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    //GET_EQUAL( ( char* )&id );
    if( FOUND2() ) {
        _GRABAR_LOG_DBROUTER( string_table->mensaje );
    }
    else {
        char mensaje[80];
        sprintf( mensaje, "ID MENSAJE DESCONOCIDO %d", id );
        _GRABAR_LOG_DBROUTER( mensaje );
    }
    //SELECT( area );
    SELECT_TABLE(area_ant, tipo_ant);
}

/******************************************************************************/
int GRABAR_LOG_DBROUTER( char *operacion ,int origen, int nivel ){
/******************************************************************************/
	int rta=0;
	if( DEBE_GRABAR_LOG_SISTEMA1( origen, nivel ))
		rta = _GRABAR_LOG_DBROUTER( operacion );
	return rta;
}
/******************************************************************************/
int _GRABAR_LOG_DBROUTER( char *operacion )
/******************************************************************************/
{
    int rta,handle,nro_arch;
#define STR_LOG_BUFFER_LEN 84
    struct _log_sistema
    {
        char fecha[8];
        char b1;
        char hora[8];
        char b2;
        char operacion[STR_LOG_BUFFER_LEN];
    } log_sistema;
    char strArch[20];
    /*------------------------- arma registro -------------------------*/
    DTOC( GET_FECHA_DOS(), log_sistema.fecha );
    log_sistema.b1 = ' ';
    HTOAL( GET_HORA_COMPLETA_DOS(), log_sistema.hora );
    log_sistema.b2 = ' ';

    memset( log_sistema.operacion, '\0', sizeof( log_sistema.operacion ) );
	_snprintf(log_sistema.operacion, STR_LOG_BUFFER_LEN, "%s\r\n", operacion);
    if( strlen( operacion ) >= STR_LOG_BUFFER_LEN )
        log_sistema.operacion[STR_LOG_BUFFER_LEN - 1] = 0;

/*
    memset( log_sistema.operacion, ' ', sizeof( log_sistema.operacion ) );
    if( strlen( operacion ) > 80 ) {
        operacion[80] = 0;
    }
    strcpy( log_sistema.operacion, operacion );
    strcat( log_sistema.operacion, "\r\n" );
*/
    nro_arch = NRO_ARCHIVO;
    NRO_ARCHIVO = 0;
    sprintf( strArch, "%sdbrouter.log", PATH_LOGS );
    handle = _OPEN( strArch, O_RDWR, __LINE__, __FILE__ );
    if( handle < 0 ) {
		#ifdef INVEL_L
    	handle = _CREAT( strArch, S_IFREG | S_IWRITE | S_IREAD, __LINE__, __FILE__ );
		#else
    	handle = _CREAT( strArch, /*S_IFREG |*/ S_IWRITE | S_IREAD /*| O_BINARY*/, __LINE__, __FILE__ );
		#endif
        if( handle > 0 ) {
            _CLOSE( handle, __LINE__, __FILE__ );
			#ifdef INVEL_L
            handle = _OPEN( strArch, O_WRONLY, __LINE__, __FILE__ );
			#else
			handle = _OPEN( strArch, O_WRONLY | O_BINARY, __LINE__, __FILE__ );
			#endif
        }
    }
    POSICIONAR_REGISTRO( handle, 0, 0 );
    rta = _WRITE( handle, ( char* )( &log_sistema ), strlen( ( char* )( &log_sistema ) ), __LINE__,
                  __FILE__ );
    _CLOSE( handle, __LINE__, __FILE__ );
    NRO_ARCHIVO = nro_arch;
    return ( !rta );
}

