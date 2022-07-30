
#pragma once
#include <tpv_dat.h>
#pragma pack(8)
#include <cDbTable.h>

class DbRouterTable :
    public DbTable
{
public:
    DbRouterTable(wxDb *pPwxDb, const wxString &pTblName, const UWORD pNumColumns,
            const wxString &pQryTblName, bool pQryOnly, const wxString &pTblPath ) : DbTable(pPwxDb, 
            pTblName, pNumColumns, pQryTblName, pQryOnly, pTblPath){};
    virtual ~DbRouterTable(void);
    
    void getTableName( char* name, int size );
    void getColumnName( char* name, int size );
    char getDataType();
    int getColumnOffset();
    int getDataSize();
    int getTableNumber();
    bool getPKey();
    void getColumnNameSuc( char* name, int size );
};
