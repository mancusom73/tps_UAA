#pragma pack (push,8) 
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



#ifndef __DBTABLES_H
#define __DBTABLES_H
#pragma warning (disable:4103)              //aviso de pragma pack en tpv_dat.h
#include <tpv_dat.h>
#pragma pack(8)
#include <cDbTable.h>
#include <DbRouterTable.h>
#include <deftables.h>
#include <string>
#include <vector>

using namespace std;


#define DATATYPE_VARCHAR		1
#define DATATYPE_LONG			2
#define DATATYPE_INT			3
#define DATATYPE_DOUBLE			4
#define DATATYPE_FLOAT			5
#define DATATYPE_BINARY			6
#define DATATYPE_FECHA_INVEL	7
#define DATATYPE_FECHA			8



class DbTables
{
    private:
        //DbTable     *oTabla[CANT_TABLAS];      //Array de vectores de punteros a tablas (todas las tablas)
        //vector< DbTable >  oTabla[CANT_TABLAS];
        DbTable     *oTabla[CANT_TABLAS][CANT_TIPOS];
        DbTable          *oCobros;               //Tabla cobros
        DbTable          *oOperCli;              //Tabla clientes para cobros
        DbRouterTable    *oDbRouter;             //Tabla dbrouter, contine la definicion de todas las demas tablas
        wxString         *mStrError;              //String para guardar el error
        struct _dbRouter mDbRouuter;             //Estructura del DbRouter
        
    public:
        DbTables();
        ~DbTables();
        //int setCliCobro(wxDb *pREADONLY_DB,struct _cli_cobro *pCliCobro);
        int setDbRouter(wxDb *pREADONLY_DB);
        DbRouterTable *getDbRouter();
        int setTabla(wxDb *pREADONLY_DB, int pTabla, int pTipo, char *pStruct, long pSizeof, char *pNombre);
        DbTable *getTabla(int pTabla, int pTipo);
        int opened(int pTabla, int pTipo);
        void closeTabla(int pTabla, int pTipo);
        char *getError();
        static int getDataType(int pDataTypeDef, int *pDataType, int *pCType);
        static int getStrTipo(int pTipo, char *pStrTipo, int pStrTipoLength);
};


#endif
#pragma pack (pop) 
