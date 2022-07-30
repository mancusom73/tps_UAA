#pragma pack (push,8) 
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


#ifndef __ODBC_H
#define __ODBC_H

#include <wx/db.h>
#include <wx/dbtable.h>



class ODBC
{
    private:
		wxDbConnectInf    *oDbConnectInf[2]; //Informacion de la conexion con ODBC
        wxDb              *oREADONLY_DB[2];  //Base de Datos
        wxString          mStrError[2];             //string para guardar el error

    public:
        ODBC(){};
        int     Conect(char *pDsn,char *pUser,char *pPassw,char *pdir, int pNroConnect);     //Conecta a un ODBC especifico
        void    Disconect(int pNroConnect);
		bool    IsConect(int pNroConnect);
        wxDb    *getDb(int pNroConnect);
        char    *getError(int pNroConnect);
};

#endif
#pragma pack (pop) 
