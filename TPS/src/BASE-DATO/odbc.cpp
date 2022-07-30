/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   ODBC
*   Descripcion :   Manejador de conexion a ODBC
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/


#include <odbc.h>
/*Conexiones*/
#define ODBC_LOCAL    0
#define ODBC_SERVIDOR 1


int ODBC::Conect(char *pDsn,char *pUser,char *pPassw,char *pdir, int pNroConnect)
{
    oDbConnectInf[pNroConnect] = new wxDbConnectInf(NULL, pDsn, "", "", "");   
    if (!oDbConnectInf[pNroConnect] || !oDbConnectInf[pNroConnect]->GetHenv())
    {
        mStrError[pNroConnect].Printf("NO se encuentra el DSN (%s)",pDsn);
        wxDELETE(oDbConnectInf[pNroConnect]);
        return 1;
    }
    oDbConnectInf[pNroConnect]->SetDsn(pDsn);
    oDbConnectInf[pNroConnect]->SetUserID(pUser);
    oDbConnectInf[pNroConnect]->SetPassword(pPassw);
    oDbConnectInf[pNroConnect]->SetDefaultDir(pdir);
	oREADONLY_DB[pNroConnect] = new wxDb(oDbConnectInf[pNroConnect]->GetHenv());
	oREADONLY_DB[pNroConnect] = wxDbGetConnection(oDbConnectInf[pNroConnect],!wxODBC_FWD_ONLY_CURSORS);
    if (oREADONLY_DB[pNroConnect] == 0)
    {
        mStrError[pNroConnect].Printf("No se puede conectar a %s, Verifique el ODBC, el Usuario y Password", pDsn);
        wxDELETE(oDbConnectInf[pNroConnect]);
        return 1;
    }
    return 0;
}

void ODBC::Disconect(int pNroConnect) {
    
    if( oREADONLY_DB[pNroConnect] != NULL ) {
		if ( pNroConnect == ODBC_SERVIDOR)
			oREADONLY_DB[pNroConnect]->CommitTrans();
		oREADONLY_DB[pNroConnect]->Close();
    }
}

bool ODBC::IsConect(int pNroConnect) 
{
	/*
	SQL State = 08S01
	Native Error Code = 2006
	Error Message = [MySQL][ODBC 5.1 Driver][mysqld-5.0.89-community-nt-log]
	MySQL server has gone away
	*/
	//Valores tomados de las librerias.
	int DB_ERR_COMM_LINK_FAILURE = 19; // --> SQLState = 08S01
	int CR_SERVER_GONE_ERROR = 2006; //el servidor MySQL no está disponible. 

	if( oREADONLY_DB[pNroConnect] != NULL 
		&&( ( oREADONLY_DB[pNroConnect]->DB_STATUS == DB_ERR_COMM_LINK_FAILURE 
				&& oREADONLY_DB[pNroConnect]->nativeError == CR_SERVER_GONE_ERROR  )
			 || oREADONLY_DB[pNroConnect]->DB_STATUS == DB_ERR_COMM_LINK_FAILURE )) {
		return( TRUE );
	}

	return( FALSE );

}

wxDb *ODBC::getDb(int pNroConnect)
{
    return oREADONLY_DB[pNroConnect];
}



char *ODBC::getError(int pNroConnect)
{
    return (char *) mStrError[pNroConnect].c_str();
}


