/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   -
*   Descripcion :   Funciones utilizadas como interface entre C y C++
*                   Para el manejo de base de datos con el servidor
*
********************************************************************************/
#include <DbTables.h>
#include <DBServer.h>
#include <DbFileUtils.h>
#include <Odbc.h>
#include <str.h>
#include <path.h>
#include <wx/filefn.h>
#include <cio.h>
#include "CObject.h"//sacar
#include <cr.h>

extern ODBC *oODBCConect;
extern DbTables *oTablas;

/**
 * Abre y devuelve una estructura de tabla.
 * @param pTabla numero identificador de la tabla en la tabla pos_dbmap
 * @param pStruct estructura sobre la que se mapean los registros (notar que es una referencia)
 * @param pSizeof tamaño del buffer de la estructura 
 * @param pNombreTablaServidor nombre de la tabla en el servidor
 * @param pNombreTablaStruct nombre de la tabla de la que se toma la estructura. Si
 *                           es null, se toma pNombreTablaServidor.
 */
DbTable *DBServer::openTable( int pTabla, struct _tBuffer& pStruct, char *pNombreTablaServ, char *pNombreTablaStruct )
{
    wxString    mWhere;
    wxString    mQuery;
    wxString    mTableName;
	wxString    str;
    wxArrayString dateFields;
    int cnt = 0, pos;
    int mDataType, mCType;
    long cant = 0, specialColumns = 0, mSizeof = 0;
    DbTable *workTable = NULL;
    char columnNameSucTmp[31];
    bool ignore = false;

    //Se averigua si la tabla tiene los campos especiales "STATUS" y "FIN"
    mWhere.Printf("TABLENUMBER = %i AND ( COLUMNNAME = \"STATUS\" OR COLUMNNAME = \"FIN\" )", pTabla);
    dbRTable->setWhere( (char *) mWhere.c_str() );
    specialColumns = dbRTable->Count();

    //Se obtienen los nombres de los campos con tipo DATE
    dateFields = getDateFields( (pNombreTablaStruct)?wxT(pNombreTablaStruct):wxT(pNombreTablaServ) );
    
    //Se obtiene la cantidad de campos de la tabla
    mWhere.Printf("TABLENUMBER = %i", pTabla);
    dbRTable->setWhere( (char *) mWhere.c_str() );
    str = "COLUMNOFFSET";
	dbRTable->setOrder( (char *) str.c_str() );
    dbRTable->runQuery(0);
    cant = dbRTable->Count(); //obtiene la cantidad de registros = cant de campos

    if( cant < 1 ){
        dbError->Printf("No hay campos definidos en el pos_dbmap para la tabla: %i",pTabla);
        return NULL;
    }

    //Se abre la tabla en el servidor
    workTable = new DbTable(serverDb, pNombreTablaServ, cant - specialColumns, wxT(""),!wxDB_QUERY_ONLY, "" );
    if( !workTable ){
        dbError->Printf("No se pudo crear wxDbTable object en la memoria (setTabla(tabla: %i))",pTabla);
        return NULL;
    }
    workTable->setNroTable( pTabla ); //guarda el numero de tabla
    workTable->setNroTipo( 0 );       //no tiene sentido para las tablas del servidor
    workTable->setPtrStruct( pStruct.buffer, pStruct.size ); //guarda el puntero a la estructura que va a mapear

    //Se mapean los campos
    dbRTable->GetFirst();
    while(!dbRTable->getEof() && pStruct.buffer ){
        dbRTable->getColumnNameSuc( columnNameSucTmp, sizeof( columnNameSucTmp ) );
        if( strcmp( columnNameSucTmp, "" ) == 0 ) {
            dbRTable->getColumnName( columnNameSucTmp, sizeof( columnNameSucTmp ) );
        }
        //Los campos STATUS y FIN no se mapean ya que no se encuentran en las 
        //tablas del servidor.
        if( strcmp( columnNameSucTmp, "STATUS" ) == 0 || strcmp( columnNameSucTmp, "FIN" ) == 0 ) {
            dbRTable->moveReg(1);
            continue;
        }
        //Si la columna es de tipo DATE en vez de mapear a la posicion del struct
        //(ya que no entra en 2 bytes) se mapean dentro de variables donde si 
        //entra el tipo DATE (DATE_STRUCT)
        if( ( pos = dateFields.Index( columnNameSucTmp, 0 ) ) != wxNOT_FOUND ) {
            DbTables::getDataType( DATATYPE_FECHA, &mDataType, &mCType );
            workTable->SetColDefs(cnt, columnNameSucTmp, mDataType, &(pStruct.dates[pos]), mCType, sizeof( DATE_STRUCT ), (dbRTable->getPKey() == 1)?true:false);
            pStruct.positions[pos] = dbRTable->getColumnOffset();
    	    dbRTable->moveReg(1);
            cnt++;
        } else {
            if( DbTables::getDataType( dbRTable->getDataType(), &mDataType, &mCType ) == 0 ) {
				//Se comenta el +1 porque la funcion OBTENER_TABLA_SERVIDOR al traer la tabla 
				//solicitada toma el tamaño de cada columna del definido en datasize de la tabla pos_dbmap
				//y los campo varchar de la tabla fisica tienen definida uno menos.
				workTable->SetColDefs(cnt, columnNameSucTmp, mDataType, &(pStruct.buffer[dbRTable->getColumnOffset()]), mCType, dbRTable->getDataSize() /*+ 1*/, (dbRTable->getPKey() == 1)?true:false);
			    dbRTable->moveReg(1); 
                cnt++;
            } else {
                wxDELETE( workTable );
                return NULL;
            }
        }
    }

    if( !workTable->Open() ){
        wxDELETE( workTable );
        dbError->Printf("ODBC, No se pudo abrir la tabla del servidor: %s", pNombreTablaServ );
        return NULL;
    }
    return workTable;
    
}

/**
 * Devuelve una lista con los nombres de los campos de tipo DATE de la tabla solicitada.
 * @param tablename el nombre de la tabla
 */
wxArrayString DBServer::getDateFields( wxString tablename ) {

    wxString query;
    wxArrayString  result;
    char buffer[50];
    long cantBytes;
	//Clientes Servidores definidos en ini.h
	int BTRIEVE = 1, MYSQL = 2, SQLSERVER = 3, ORACLE = 4;
	int Cliente_DBServer = get_Cliente_DBServer( );


	if( Cliente_DBServer == ORACLE ) {
		query.Printf("select c.column_name from user_tables u join user_tab_columns c on (u.table_name = c.table_name) where c.data_type = upper('date') and u.table_name = upper('%s') ", tablename );
	}
	if( Cliente_DBServer == SQLSERVER ) {
		//Valido para SQLSERVER unicamente
		query.Printf("select sc.name from sysobjects so, syscolumns sc, systypes st where so.id = sc.id and sc.xtype = st.xtype and so.xtype = 'U' and st.name = 'datetime' and so.name = '%s'", tablename );
	}
    serverDb->ExecSql( query );
    while ( serverDb->GetNext() ) {
        serverDb->GetData(1, SQL_C_CHAR, &buffer, sizeof(buffer), &cantBytes);
        result.Add( wxT( buffer ) );
    }
    return result;
}


/**
 * Devuelve el error actual
 */
    wxString query;
wxString* DBServer::getError()
{
    return dbError;
}


#ifdef __cplusplus
extern "C"
{
#endif
#include <ini.h>
#include <mensaje.h>
#include <log.h>
#include <fecha.h>
	int get_Cliente_DBServer( )
	{
		return( ConfIni.clienteDbServer );
	}
    /* **************************************************************************/
    int OBTENER_TABLA_SERVIDOR( int pTabla, int pTipo, char *nombreTablaServ, char *condicionSelect, int vaciar, char *nombreTablaStruct )
	/*
	* pTabla            = nro Tabla original
	* pTipo             = tipo de tabla: tmp, bak, his etc..
	* nombreTablaServ   = Significado nuevo: nombre de la tabla en la base de datos remota (i.e. la tabla solicitada)
	* condicionSelect   = codicion para la restriccion de registros.
    * vaciar            = no usado
    * nombreTablaStruct = es la tabla remota de la cual se tomara la estructura. Util en casos
    *                     en los que se quiera obtener una vista, ya que en la base no existe como una tabla.
	* ***************************************************************************/
	{
        /** Para renombrar archivos de la base */
        DbFileUtils *dbFile = NULL;
        /** El administrador de tablas */
        DBServer *serverTables = NULL;
        /** La tabla del servidor a copiar */
        DbTable  *workTable = NULL;
        wxString  tmpQuery;
        long      cantBytes = 0, tamBuffer = 0;
        struct _tBuffer tBuffer = {NULL,0,NULL,0}; 
        char      msg[180] = {""};
        int       toTransfer = 0, transferred = 0;
        int       abort = 0, cant_reg = 0;

        //Verificamos si la caja se encuentra offline
		if( bandera_offline ) {
			DIRECT_DB_QUERY("select count(*) from SCRIPT_POS where CODCOMANDO = '%ld'", SPOS_OFFLINE ); 
			GET_DATO( 1,C_INT,(char *)&cant_reg,4);
			if( cant_reg ) {
				abort = 1;
			} else {
				bandera_offline = 0;
			}
		} 
		//Conexion odbc con el servidor
		if( ConfIni.odbcServer[0] == 0 )
			return 0;
        if( !abort && initOdbc( ConfIni.odbcServer, ConfIni.userServer, ConfIni.passwordServer, ODBC_SERVIDOR ) == 1){
            //return 1; //error
            abort = 1;
			INSERT_SCRIPT_POS( SPOS_OFFLINE, "" );
			bandera_offline = 1;
        }
        if( !abort ) {
            serverTables = new DBServer( oODBCConect->getDb(ODBC_SERVIDOR), oTablas->getDbRouter() );
            
            //Se obtiene el tamaño necesario para el intercambio de los datos
            tmpQuery.Printf("select sum(datasize) from pos_dbmap where tablenumber = %i",pTabla);
            oODBCConect->getDb(ODBC_LOCAL)->ExecSql( tmpQuery );
            oODBCConect->getDb(ODBC_LOCAL)->GetNext();
            oODBCConect->getDb(ODBC_LOCAL)->GetData(1, SQL_C_LONG, &tamBuffer, sizeof(tamBuffer), &cantBytes);
            if( cantBytes > 0 && tamBuffer > 0 ) {
                tBuffer.buffer = (char*) malloc( tamBuffer );
                memset( tBuffer.buffer, 0, tamBuffer );
                tBuffer.size = tamBuffer;
            } else {
                _snprintf( msg, sizeof( msg), "Error: No hay campos definidos para la tabla %d", pTabla); 
                abort = 1;
            }
        }

        if( !abort ) {
            //Se abre la tabla temporal local
            if( OPEN_TABLE( pTabla, TT_IMPORTACION, (char*)(tBuffer.buffer), tamBuffer ) == 1 ){
                abort = 1;
            }
        }

        if( !abort ) {
            SELECT_TABLE( pTabla, TT_IMPORTACION );
            SET_EMPTY_TABLE( pTabla, TT_IMPORTACION );

            //Se abre la tabla en el servidor (sobre el mismo buffer)
            workTable = serverTables->openTable( pTabla, tBuffer, nombreTablaServ, nombreTablaStruct );
            if( workTable == NULL ) {
                _snprintf( msg, sizeof( msg), serverTables->getError()->c_str() ); 
                abort = 1;
            }
        }

        if( !abort ) {
            workTable->setWhere( condicionSelect );
            workTable->runQuery( 0 );
            toTransfer = workTable->Count();
            workTable->GetFirst();

            //Transferencia
            while( !workTable->getEof() ) {
                COMBINAR_FECHA( &tBuffer );
                INSERT2(0);
                workTable->moveReg(1);
                transferred++;
            }
            
            //Control
            if( toTransfer != transferred ) {
                _snprintf( msg, sizeof( msg), "Error: Import tabla %d. Se esperaban %d registros, se transfirieron %d", pTabla, toTransfer, transferred ); 
                abort = 1;
            }
        }

        if( !abort ) {
            //Se pisa la tabla solicitada con la importada
            /*dbFile = new DbFileUtils( pTabla, TT_IMPORTACION, ConfIni.pathDatabase );
            if( dbFile->Replace( pTabla, pTipo ) != 0 ) {
                _snprintf( msg, sizeof( msg ), dbFile->getError() );
                abort = 1;
            }*/
            if( REPLACE( pTabla, TT_IMPORTACION, pTipo ) != 0 ) {
                _snprintf( msg, sizeof( msg ), "Error: Reemplazando la tabla %d con los datos importados. ¿Existe la tabla de destino?", pTabla );
                abort = 1;
            }
        }

        if( abort ) {
            GRABAR_LOG_SISTEMA( "ERROR EN IMPORTACION. REVISAR LOG DE BASE",LOG_CON_SERVIDOR,2 );
        }
        //Si el error es guardado en esta funcion, avisamos.
        if( strlen( msg ) > 0 ) {
            MENSAJE( "ERROR EN IMPORTACION. LEER LOG PARA MAS DETALLES" );
            GRABAR_LOG_BD( msg, 1,LOG_CON_SERVIDOR,2);
        }

        if( tBuffer.buffer )
            free( tBuffer.buffer );
        wxDELETE( workTable );
        wxDELETE( serverTables );
        wxDELETE( dbFile );
        endOdbc( ODBC_SERVIDOR );
        
        return abort; //0 = ok, 1 = error
    }

    /* ************************************************************************** */
    /* Combina los datos de fechas en el array "dates" dentro del buffer luego de
     * convertirlas al formato de fecha invel de 2 bytes. */
    void COMBINAR_FECHA( struct _tBuffer *tBuffer )
    /* tBuffer = la estructura con los registros de una tabla
    /* ************************************************************************** */
    {
        INT16 fechaInvel;
        int i = 0;

        while( tBuffer->positions[i] != 0 && i < 8 ) {
            fechaInvel = FECHA( tBuffer->dates[i].day, tBuffer->dates[i].month, tBuffer->dates[i].year );
            memcpy( &tBuffer->buffer[ tBuffer->positions[i] ], &fechaInvel, sizeof( INT16 ) );
            i++;
        }
    }
    
    /* ************************************************************************** */
    /* Copia los contenidos de una tabla en otra. Eliminando la primera y
     * realizando un backup de la segunda.*/
    /* ************************************************************************** */
    int REPLACE( int pTabla, int pTipoOrigen, int pTipoDestino ) {
        
        int res = -1;
        char mTableName[40];
        char mTableNameDest[40];
		char mensaje[128]; 
		
		memset( mensaje, 0, sizeof( mensaje ) );
        //Por si la tabla destino no existe
//        GET_TABLE_NAME(pTabla, TT_ORIG, mTableName, sizeof( mTableName ) );
		GET_TABLE_NAME(pTabla, pTipoOrigen, mTableName, sizeof( mTableName ) );
		
        GET_TABLE_NAME(pTabla, pTipoDestino, mTableNameDest, sizeof( mTableNameDest ) );
        DIRECT_DB_QUERY( "create table if not exists %s like %s", mTableNameDest, mTableName );
        //Se reemplazan
		if( BACKUP_TABLE_TYPE( pTabla, pTipoDestino ) == 0 ) 
		{
            SET_EMPTY_TABLE( pTabla, pTipoDestino );
            if( ( res = TRANSFER_TABLE( pTabla, pTipoOrigen, pTipoDestino ) ) == 0 ) 
			{
                DROP_TABLE( pTabla, pTipoOrigen );
				_snprintf(mensaje, sizeof(mensaje)-1,"Borrar tabla `%s_bak`", mTableNameDest );
				GRABAR_LOG_BD(mensaje,SI ,LOG_CON_SERVIDOR,5);
				DIRECT_DB_QUERY("drop table if exists %s_bak", mTableNameDest );
            }
            else 
			{
				_snprintf(mensaje, sizeof(mensaje)-1,"Restaurar tabla `%s`", mTableNameDest );
				GRABAR_LOG_BD(mensaje,SI ,LOG_CON_SERVIDOR,5);
                RESTORE_TABLE_TYPE( pTabla, pTipoDestino );
				memset( mensaje, 0, sizeof( mensaje ) );
				_snprintf(mensaje, sizeof(mensaje)-1,"Borrar tabla `%s`", mTableName );
				GRABAR_LOG_BD(mensaje,SI ,LOG_CON_SERVIDOR,5);
				DROP_TABLE( pTabla, pTipoOrigen );
            }
		} 
		else 
		{
			_snprintf(mensaje, sizeof(mensaje)-1,"Borrar tabla `%s`", mTableName );
			GRABAR_LOG_BD(mensaje,SI ,LOG_CON_SERVIDOR,5);
			DROP_TABLE( pTabla, pTipoOrigen );
		}
        return res;
    }

    int INSERT_EN_SERVER( char *nombre_tabla, char *buffer, int tamano_buffer, int espera_respuesta )
	/*    nombre_tabla     = Buffer que contiene el nombre de la tablas (ej. eventos)
	* buffer           = Apunta al data buffer a insertar (ej. &sript).
	* tamano_buffer    = Tamano del buffer.
	* espera_respuesta = si espera confirmacion ACK desde DBR.
	* ************************************************************************** */
	{
		return 1;
	}
#ifdef __cplusplus
}
#endif

