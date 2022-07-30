/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   DbTables
*   Descripcion :   Contenedor de tablas
*
*   Autor
*   -----
*   Forray Juan Pablo
********************************************************************************/


#include <dbtables.h>



DbTables::DbTables()
{
    int cnt = 0, i = 0;
    //while(cnt <= CANT_TABLAS)
	while(cnt < CANT_TABLAS)
    {
        for( i = 0; i < CANT_TIPOS; i++ ) {
            oTabla[cnt][i] = NULL;
        }
        cnt++;
    }
    mStrError = new wxString();
}


DbTables::~DbTables()
{
    delete mStrError;
}


int DbTables::setDbRouter(wxDb *pREADONLY_DB)
{
    oDbRouter = new DbRouterTable(pREADONLY_DB, "pos_dbmap", 9, wxT(""),!wxDB_QUERY_ONLY, "");
	memset( mDbRouuter.tableName, 0, sizeof(mDbRouuter.tableName) );
	memset( mDbRouuter.columName, 0, sizeof(mDbRouuter.columName) );
	memset( mDbRouuter.columNameSuc, 0, sizeof(mDbRouuter.columNameSuc) );
    if(!oDbRouter){
        mStrError->Printf("No se pudo crear wxDbTable object en la memoria (setDbRouter())");
        return 1;
    }

    oDbRouter->SetColDefs(0, "TABLENAME",     DB_DATA_TYPE_VARCHAR, mDbRouuter.tableName,     SQL_C_CHAR,     sizeof(mDbRouuter.tableName));
    oDbRouter->SetColDefs(1, "COLUMNNAME",    DB_DATA_TYPE_VARCHAR, mDbRouuter.columName,     SQL_C_CHAR,     sizeof(mDbRouuter.columName));
    oDbRouter->SetColDefs(2, "DATATYPE",      DB_DATA_TYPE_INTEGER, &mDbRouuter.dataType,     SQL_C_LONG,     sizeof(mDbRouuter.dataType));
    oDbRouter->SetColDefs(3, "COLUMNOFFSET",  DB_DATA_TYPE_INTEGER, &mDbRouuter.columnOffset, SQL_C_LONG,     sizeof(mDbRouuter.columnOffset));
    oDbRouter->SetColDefs(4, "DATASIZE",      DB_DATA_TYPE_INTEGER, &mDbRouuter.dataSize,     SQL_C_LONG,     sizeof(mDbRouuter.dataSize));
    oDbRouter->SetColDefs(5, "TABLENUMBER",   DB_DATA_TYPE_INTEGER, &mDbRouuter.tableNumber,  SQL_C_LONG,     sizeof(mDbRouuter.tableNumber));
    oDbRouter->SetColDefs(6, "PKEY",          DB_DATA_TYPE_INTEGER, &mDbRouuter.pkey,         SQL_C_LONG,     sizeof(mDbRouuter.pkey));
    oDbRouter->SetColDefs(7, "COLUMNNAMESUC", DB_DATA_TYPE_VARCHAR, &mDbRouuter.columNameSuc, SQL_C_CHAR,     sizeof(mDbRouuter.columNameSuc));
    oDbRouter->SetColDefs(8, "FIN",           DB_DATA_TYPE_INTEGER, &mDbRouuter.fin,          SQL_C_UTINYINT, sizeof(mDbRouuter.fin));

    oDbRouter->setPtrStruct((char*)&mDbRouuter, 111);

    if (!oDbRouter->Open()){
        wxDELETE(oDbRouter);
        mStrError->Printf("ODBC, No se pudo abrir la tabla dbrouter");
        return 1;
    }

    return 0;
}

DbRouterTable *DbTables::getDbRouter()
{
    return oDbRouter;
}

/*Si pNombre es igual a !NULL entonces setea la temporal*/
int DbTables::setTabla(wxDb *pREADONLY_DB, int pTabla, int pTipo, char *pStruct, long pSizeof, char *pNombre)
{
    wxString    mWhere;
    wxString    mQuery;
    wxString    mTableName;
	wxString    str;
    char        mStrTipo[15];
    SDWORD      cb;
    int cnt = 0;
    int mDataType, mCType;
    long cant = 0, mSizeof = 0;

    if(pTabla > CANT_TABLAS){ //pTabla Nunca puede ser > a CANT_TABLAS
        mStrError->Printf("La tabla actual (%i) supera CANT_TABLAS", pTabla);
        return 1;
    }
    if( pTipo > CANT_TIPOS ) {
        mStrError->Printf("El tipo actual (%i) supera CANT_TIPOS", pTipo);
        return 1;
    }

    if(!oTabla[pTabla][pTipo]){                             //si no esta seteada
        if(!pNombre){
            mWhere.Printf("TABLENUMBER = %i", pTabla);
        }else{
            mWhere.Printf("TABLENAME = '%s'", pNombre);
        }
        oDbRouter->setWhere( (char *) mWhere.c_str() );
		str.Printf("COLUMNOFFSET");
		oDbRouter->setOrder( (char *) str.c_str() );
        oDbRouter->runQuery(0);
        cant = oDbRouter->Count();                  //obtiene la cantidad de registros = cant de campos
        if(cant < 1){
            if(!pNombre){
                mStrError->Printf("No hay campos definidos en el pos_dbmap para la tabla: %i",pTabla);
            }else{
                mStrError->Printf("No hay campos definidos en el pos_dbmap para la tabla: %s",pNombre);
            }
            return 1;
        }

        /* suma datasize que tiene que ser igual al sizeof de la estructura */
        if(!pNombre){
            mQuery.Printf("select sum(datasize) from pos_dbmap where tablenumber = %i",pTabla);
        }else{
            mQuery.Printf("select sum(datasize) from pos_dbmap where tablename = '%s'",pNombre);
        }
        pREADONLY_DB->ExecSql(mQuery);
        pREADONLY_DB->GetNext();
        /* Obtiene el dato */
        pREADONLY_DB->GetData(1, SQL_C_LONG, &mSizeof, sizeof(mSizeof), &cb);
        if(pSizeof > 0){
            if(mSizeof != (long)pSizeof){
//                mStrError->Printf("La suma (sizeof) de los campos de la tabla: %i (%s) no es igual al sizeof de la estructura",pTabla, mDbRouuter.tableName);
                mStrError->Printf("La suma (sizeof) de los campos de la tabla: %i (%s) no es igual al sizeof de la estructura %li vs %li",pTabla, mDbRouuter.tableName,mSizeof , (long)pSizeof );
                return 1;
            }
        }
        if(pNombre){
            mTableName.Printf("%s_temp",pNombre);
        }else{
            getStrTipo( pTipo, mStrTipo, sizeof(mStrTipo) );
            mTableName.Printf("%s%s",mDbRouuter.tableName, mStrTipo );
        }
        mTableName.MakeLower(); //convierte el nombre a minusculas
        oTabla[pTabla][pTipo] = new DbTable(pREADONLY_DB, (char *)mTableName.c_str(), cant, wxT(""),!wxDB_QUERY_ONLY, "" );
        if(!oTabla[pTabla][pTipo]){
            mStrError->Printf("No se pudo crear wxDbTable object en la memoria (setTabla(tabla: %i))",pTabla);
            return 1;
        }
        oTabla[pTabla][pTipo]->setNroTable(pTabla);    //guarda el numero de tabla
        oTabla[pTabla][pTipo]->setNroTipo(pTipo);
        oTabla[pTabla][pTipo]->setPtrStruct(pStruct,pSizeof);  //guarda el puntero a la estructura que va a mapear
        /*Mapea todos los campos*/
        while(!oDbRouter->getEof() && pStruct){
            if(getDataType(mDbRouuter.dataType, &mDataType, &mCType) == 0){
				oTabla[pTabla][pTipo]->SetColDefs(cnt, mDbRouuter.columName, mDataType, &pStruct[mDbRouuter.columnOffset], mCType, mDbRouuter.dataSize + 1, (mDbRouuter.pkey == 1)?true:false);
                oDbRouter->moveReg(1);                  //avanza un registro
                cnt++;
            }else{
                wxDELETE(oTabla[pTabla][pTipo]);
                return 1;
            }
        }

        if (!oTabla[pTabla][pTipo]->Open()){
            wxDELETE(oTabla[pTabla][pTipo]);
            mStrError->Printf("ODBC, No se pudo abrir la tabla: %i, tipo: %d, (%s)",pTabla,pTipo,(char *)mTableName.c_str());
            return 1;
        }
    }

    return 0;
}




int DbTables::getDataType(int pDataTypeDef, int *pDataType, int *pCType)
{
    int rta = 1;
    switch( pDataTypeDef ) {        //tipo de dato definido en la tabla invelDbrouter
        case DATATYPE_VARCHAR:
            *pDataType = DB_DATA_TYPE_VARCHAR;
            *pCType    = SQL_C_CHAR;
            rta = 0;
            break;
        case DATATYPE_LONG:
            *pDataType = DB_DATA_TYPE_INTEGER;
            *pCType    = SQL_C_SLONG;
            rta = 0;
            break;
		case DATATYPE_FECHA_INVEL:
        case DATATYPE_INT:
            *pDataType = DB_DATA_TYPE_INTEGER;
            *pCType    = SQL_C_SSHORT;
            rta = 0;
            break;
        case DATATYPE_DOUBLE:
            *pDataType = DB_DATA_TYPE_FLOAT;
            *pCType    = SQL_C_DOUBLE;
            rta = 0;
            break;
        case DATATYPE_FLOAT:
            *pDataType = DB_DATA_TYPE_FLOAT;
            *pCType    = SQL_C_FLOAT;
            rta = 0;
            break;
        case DATATYPE_BINARY:
            *pDataType = DB_DATA_TYPE_INTEGER;
            *pCType    = SQL_C_UTINYINT;
            rta = 0;
            break;
        case DATATYPE_FECHA:
            *pDataType = DB_DATA_TYPE_DATE;
            *pCType    = SQL_C_DATE;
            rta = 0;
            break;
        default:
            //mStrError->Printf("ERROR: No existe el tipo de dato: %i (invelDbrouter)",pDataTypeDef);
            rta = 1;
            break;
    }
    return rta;
}

int DbTables::getStrTipo(int pTipo, char *pStrTipo, int pStrTipoLength )
{
    int mRta = 1;

    switch( pTipo ) {
        case TT_ORIG:
            strncpy( pStrTipo, "", pStrTipoLength );
            break;
        case TT_BAK:
            strncpy( pStrTipo, "_bak", pStrTipoLength );
            break;
		case TT_BAK_ULT:
            strncpy( pStrTipo, "_bak_ult", pStrTipoLength );
            break;
        case TT_TEMP:
            strncpy( pStrTipo, "_tmp", pStrTipoLength );
            break;
        case TT_ENTR:
            strncpy( pStrTipo, "_entr", pStrTipoLength );
            break;
        case TT_ALTA:
            strncpy( pStrTipo, "_alta", pStrTipoLength );
            break;
        case TT_MODIF:
            strncpy( pStrTipo, "_modif", pStrTipoLength );
            break;
		case TT_HIST:
            strncpy( pStrTipo, "_his", pStrTipoLength );
            break;
		case TT_IMPORTACION:
            strncpy( pStrTipo, "_import", pStrTipoLength );
            break;
		case TT_OLD:
            strncpy( pStrTipo, "_old", pStrTipoLength );
            break;
		case TT_TEMP1:
            strncpy( pStrTipo, "_tmp1", pStrTipoLength );
            break;
        default :
            mRta = 0;
            strncpy( pStrTipo, "", pStrTipoLength );
    }
    pStrTipo[pStrTipoLength - 1] = NULL;
    return ( mRta );
}


DbTable *DbTables::getTabla(int pTabla, int pTipo)
{
    return oTabla[pTabla][pTipo];
}


void DbTables::closeTabla(int pTabla, int pTipo)
{
    wxDELETE(oTabla[pTabla][pTipo]);
}


int DbTables::opened(int pTabla, int pTipo)
{
    return (!oTabla[pTabla][pTipo]) ? 0 : 1;
}


char *DbTables::getError()
{
    return (char *) mStrError->c_str();
}
