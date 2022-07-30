#pragma pack (push,8) 
/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   DbScript
*   Descripcion :   Manejador de script de modificacion de base de datos
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/



#ifndef __DBSCRIPT_H
#define __DBSCRIPT_H

#include <path.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <odbc.h>


class DbScript
{
    private:
        
        //int RunScripts(ODBC *poODBCConect, wxString pPathScript);
        wxString mStrError; //guarda los mensaje de error
		bool EjecutarPorTipo(wxString pScript, wxString pNomTabla, ODBC *poODBCConect);

    public:
		int RunScripts(ODBC *poODBCConect, wxString pPathScript,wxString pPathLog);
		int RunScriptsSP(ODBC *poODBCConect, wxString pPathScript,wxString pPathLog);
        DbScript(){};
        int VerifScripts(ODBC *poODBCConect);   //verifica si hay script para ejecutar
        char *getError();

};


#endif
#pragma pack (pop) 
