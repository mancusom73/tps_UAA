#pragma pack (push,8) 
/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   -
*   Descripcion :   Definicion de las funciones que van a ser utilizadas en 'C'
*                   para la conexion con el servidor
*
********************************************************************************/


#ifndef __DBSERVER_H
#define __DBSERVER_H

#include <DefTables.h>
#include <cDbTable.h>
#include <Db.h>

//Estructura para mapear temporalmente una tabla
struct _tBuffer {
    /** donde van los datos de la tabla excepto los tipo fecha */
    char *buffer;
    /** el tamano del buffer */
    int  size;
    /** donde mapeados los campos tipo fecha */
    DATE_STRUCT dates[8];
    /** a que offset de buffer pertenecen los campos mapeados arriba */
    int positions[8];
};

class DBServer {
private:
    wxString *dbError;
    wxDb *serverDb;
    DbRouterTable *dbRTable ;

    wxArrayString getDateFields( wxString tablename );
    long getField( wxString field, wxString tablename, wxString whereS );

public:
    DBServer( wxDb *db, DbRouterTable *dbRT ) {
        dbError = new wxString();
        serverDb = db;
        dbRTable = dbRT;
    };
    virtual ~DBServer() {
        delete dbError;
    };
    DbTable *openTable( int pTabla, struct _tBuffer& pStruct, char *pNombreTablaServ, char *pNombreTablaStruct );
    wxString *getError();
    DbTable* workTable;
};


#ifdef __cplusplus
extern    "C" {
#endif
//codigo
    int OBTENER_TABLA_SERVIDOR( int pTabla, int pTipo, char *nombreTablaServ, char *codicionSelect, int vaciar, char *nombreTablaStruct );
	int REPLACE_TABLE( int tableDest, int typeDest, int tableSource, int typeSource );
    void COMBINAR_FECHA( struct _tBuffer *tBuffer );
    int REPLACE( int pTabla, int pTipoOrigen, int pTipoDestino );
	int get_Cliente_DBServer( );
#ifdef __cplusplus
}
#endif

#endif
#pragma pack (pop) 
