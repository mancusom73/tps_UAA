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


#include <cDbTable.h>


int DbTable::runQuery(int pDistinct)
{
    mVarEOF = false;
    mVarBOF = false;
    mDistinct = (pDistinct) ? true : false;
    if(mPaginado){
        mPagina = 1;            //primer pagina
    }
    mVarEOF = !execSql();

    return mVarEOF;
}


bool DbTable::execSql()
{
    //this->SetFromClause("");
    setLimite();
    /*if( mPtrStruct ) {
        memset( mPtrStruct, '\0', mPtrLength );
    }*/
    this->Query(false,mDistinct);
    mNroRow = 1;
    return this->GetNext();
}

bool DbTable::GetNext() 
{
    if( mPtrStruct ) {
        memset( mPtrStruct, '\0', mPtrLength );
    }   
    return wxDbTable::GetNext();
}

bool DbTable::GetFirst() {

    if( mPtrStruct ) {
        memset( mPtrStruct, '\0', mPtrLength );
    }   
    return wxDbTable::GetFirst();
}

bool DbTable::GetLast() {

    if( mPtrStruct ) {
        memset( mPtrStruct, '\0', mPtrLength );
    }   
    return wxDbTable::GetLast();
}

int DbTable::moveReg(long pCant)
{
    mNroRow = this->GetRowNum() /*- ((mPagina - 1) * mLimite)*/;
    if( pCant > 0 ) {
        while( pCant > 0 ) {
            mNroRow = this->GetRowNum() /*- ((mPagina - 1) * mLimite)*/;
            mVarEOFPag = !this->GetNext();      //va al siguiente registro
            if(mVarEOFPag){                     //si es el ultimo de la pagina
                if(mPaginado && mNroRow == mLimite){
                    mPagina++;                  //incrementa la pagina
                    mVarEOF = !execSql();       //busca la siguiente pagina
                    if(mVarEOF){                //si no hay mas paginas
                        mPagina--;              //regresa a la pagina anterior
                        execSql();
                        this->GetLast();        //se va al ultimo
                    }
                }else{                          //si no admite paginado es el fin de archivo
                    this->GetLast();        //se va al ultimo
                    mVarEOF = true;
                }
            }else{
                mVarEOF = false;
                mVarBOF = false;
            }
            pCant--;
        }
    }
    else {
        while( pCant < 0 ) {
            if(1 == this->GetRowNum() || 0 == this->GetRowNum()){ 
                mVarEOFPag = true;
            }else{
                mVarEOFPag = false;
            }
            this->GetPrev();                    //va al registro anterior
            mVarEOF = false;
            if(mVarEOFPag){                     //si es el primero de la pagina
                if(mPagina != 1){               //si no es la primer pagina    
                    mPagina--;                  //decrementa la pagina
                    mVarBOF = !execSql();       //busca la pagina anterior
                    this->GetLast();            //se va al ultimo
                }else{
                    mVarBOF = true;
                    this->GetNext();            //primer registro
                }
            }
            pCant++;
        }
    }
    return 0;
}

/**
/* Mueve el registro actual al primer elemento de la siguiente pagina
/* o no hace nada si no existe otra pagina
 */
int DbTable::movePage( long pCant )
{
    long avance;
    enum { anterior, siguiente } direccion;
    int ok;
    
    if( !mPaginado && mLimite < 1 ) {
        return 0;
    }
    
    direccion = ( pCant > 0 )? siguiente : anterior;

    do {
        if( direccion == siguiente ) {
            avance = mLimite - GetRowNum() + 1;
        }
        else {
            avance = -GetRowNum();
        }
        ok = moveReg( avance ); //siempre devuelve 0;
        pCant --;
    } while ( pCant > 0 );
    
    return ok;
    
}


void DbTable::setLimite(int pLimite)
{
    mLimite = pLimite;   
}



void DbTable::setPaginado(bool pPaginado)
{
    mPaginado = pPaginado;   
}



int DbTable::setPagina(int pPagina)
{
    int rta = 0;

	mPagina = pPagina;
    rta = !execSql();

    return rta;
}



void DbTable::setLimite()
{
    wxString strTmp;
    wxString strTmp2;
    if(mLimite > 0){
        strTmp.Printf("LIMIT %i,%i",((mLimite * mPagina) - mLimite),(mLimite /* * mPagina*/));
    }else{
        strTmp.Printf(" ");
    }

    if(this->GetOrderByClause().Len() > 1 ){
        strTmp2.Printf("%s %s",mStrOrder.c_str(),strTmp.c_str());
        this->SetOrderByClause(strTmp2);
    }else if(this->GetWhereClause().Len() > 1 ){
        strTmp2.Printf("%s %s",mStrWhere.c_str(),strTmp.c_str());
        this->SetWhereClause(strTmp2);
    }else{
        strTmp2.Printf("%s %s",mStrFrom.c_str(),strTmp.c_str());
        this->SetFromClause(strTmp2);
    }
}



void DbTable::setFrom(char *pFrom)
{
    if(strlen(pFrom) > 0){
        mStrFrom.Printf(" ,%s",pFrom);
        this->SetFromClause(mStrFrom);
    }else{
        this->SetFromClause("");
    }
}


void DbTable::setWhere(char *pWhere)
{
    mStrWhere.Printf("%s",pWhere);
    this->SetWhereClause(mStrWhere);
}

char *DbTable::getWhere()
{
    return ((char *) mStrWhere.c_str());
}

void DbTable::setOrder(char *pOrder)
{
    mStrOrder.Printf("%s",pOrder);
    this->SetOrderByClause(mStrOrder);
}



int DbTable::getLimite()
{
    return mLimite;   
}


int DbTable::getPagina()
{
    return mPagina;   
}

bool DbTable::getPaginado()
{
	return mPaginado;
}

bool DbTable::getEof()
{
    return mVarEOF;   
}

bool DbTable::getBof()
{
    return mVarBOF;   
}

long DbTable::getCount(char *pDistinct)
{
    return (pDistinct) ? this->Count(pDistinct) : this->Count();   
}

int DbTable::getNroTable()
{
    return mNroTable;   
}

int DbTable::getNroTipo()
{
    return mNroTipo;   
}

void DbTable::setNroTable(int pTable)
{
    mNroTable = pTable;   
}

void DbTable::setNroTipo(int pTipo)
{
    mNroTipo = pTipo;
}

char *DbTable::getPtrStruct()
{
    return mPtrStruct;   
}

void DbTable::setPtrStruct(char *pPtrStruct, int pPtrLength)
{
    mPtrStruct = pPtrStruct;
    mPtrLength = pPtrLength;
}
