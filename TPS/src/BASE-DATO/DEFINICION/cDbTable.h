#pragma pack (push,8) 
/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   DbTable
*   Descripcion :   Deriva de wxDbTable, manejador de tablas.
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/


#ifndef __DBTABLE_H
#define __DBTABLE_H
#include <wx/db.h>
#include <wx/dbtable.h>

class DbTable : public wxDbTable
{
  private:
    wxString    mStrError;                  //String para guardar el error
    wxString    mStrFrom;
    wxString    mStrWhere;
    wxString    mStrOrder;
    int         mLimite;                    //Contine el limite de registros
    int         mPagina;                    //Contine la pagina actual en caso de paginado
    bool        mPaginado;                  //(true/false) Paginado o no
    bool        mVarEOF;                    //Utimo registro
    bool        mVarBOF;                    //Primer registro
    bool        mVarEOFPag;                 //Utimo registro de la pagina
    bool        mVarBOFPag;                 //Primer registro de la pagina
    void        setLimite();
    bool        execSql();
    int         mNroTable;                  //guarda el numero de tabla
    int         mNroTipo;                   //tipo de la tabla
    long        mNroRow;
    bool        mDistinct;

  protected:
    char        *mPtrStruct;                //puntero a la estructura mapeada
    int         mPtrLength;


  public:
    DbTable(wxDb *pPwxDb, const wxString &pTblName, const UWORD pNumColumns,
            const wxString &pQryTblName = "", bool pQryOnly = !wxDB_QUERY_ONLY,
            const wxString &pTblPath = "") : wxDbTable(pPwxDb, pTblName, pNumColumns,
            pQryTblName, pQryOnly, pTblPath){mLimite = -1; mPagina = 1; mPaginado = false; mVarEOF = false;
            mVarBOF = false;};

    void    setLimite(int pLimite);         //Setea un limite de registros en la consuta
    void    setPaginado(bool pPaginado);    //Setea si permite paginado o no
    int     moveReg(long pCant);            //Mueve pCant registros
    int     movePage( long pCant );
    bool    getEof();                       //Indica fin de archivo
    bool    getBof();                       //Indica principio de archivo
    int     runQuery(int pDistinct);        //Ejecuta la consulta
    int     setPagina(int pPagina);
    int     getLimite();
    int     getPagina();
	bool	getPaginado();
    void    setFrom(char *pFrom);
    void    setWhere(char *pWhere);
    void    setOrder(char *pOrder);
    long    getCount(char *pDistinct);
    char    *getError();                    //Retorna la descripcion del error
    int     getNroTable();
    int     getNroTipo();
    void    setNroTable(int pTable);
    void    setNroTipo(int pTipo);
    char    *getPtrStruct();
    char    *getWhere();
    void    setPtrStruct(char *pPtrStruct, int pPtrLength);

    //override
    bool    GetFirst();
    bool    GetLast();
    bool    GetNext();

};


#endif
#pragma pack (pop) 
