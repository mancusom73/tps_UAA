/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   CMensajeP, MsgPantalla
*   Descripcion :   Manejador de mensajes por pantalla
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/

#pragma warning (disable:4786)
#include "MsgPantalla.h"
#include "MainPanel.h"
#define Uses_wxThread
extern MainPanel frame;
static wxMutex s_mutexMsgP;
void CMensajeP::SetMsg(wxString pSMsg)
{
    mSMsg = pSMsg;
}

wxString CMensajeP::GetMsg()
{
    return mSMsg;
}

bool CMensajeP::GetMostrado()
{
    return mMostrado;
}

void CMensajeP::SetMostrado(bool pMostrado)
{
    mMostrado = pMostrado;
}

void CMensajeP::SetPausa(unsigned long pPausa)
{
    mPausa = pPausa;
}

unsigned long CMensajeP::GetPausa()
{
    return mPausa;
}


void MsgPantalla::AddMsg(const wxString &pMsg, unsigned long pPausa, int pCore)
{
    wxMutexLocker lock(s_mutexMsgP);
    if(pCore == 0){ // cuando el msg no es del hilo principal
        mMsgNoCore.SetMsg(pMsg);
        mMsgNoCore.SetPausa(pPausa);
        mMsgNoCore.SetMostrado(false);
    }else{
        if(!ExisteEnCola(pMsg,cntAdd)){
            while(!mListMsg[cntAdd].GetMsg().IsEmpty() && !mListMsg[cntAdd].GetMostrado()){
                this->Sleep(10);
            }
            mListMsg[cntAdd].SetMsg(pMsg);
            mListMsg[cntAdd].SetPausa(pPausa);
            mListMsg[cntAdd].SetMostrado(false);
            cntAdd++;
            if(cntAdd == (CantMaxMsgPant)){
                cntAdd = 0;
            }
        }
    }
}

void * MsgPantalla::Entry()
{
	/*Tener en cuenta que si alguien manda un mensaje de otro hilo que no es el principal, 
	  este tambien tiene que mandar un borrado " " ya que se mostrara hasta que alguien lo borre...*/
    int cnt = 0;
    while(1){
        if(!mListMsg[cnt].GetMostrado()){
            frame.MostrarMensaje( mListMsg[cnt].GetMsg().c_str() );
            mListMsg[cnt].SetMostrado(true);
            mUltimoSMsg = mListMsg[cnt].GetMsg();
            this->Sleep(mListMsg[cnt].GetPausa());
            cnt++;
        }
        if(!mMsgNoCore.GetMostrado()){
            frame.MostrarMensaje( mMsgNoCore.GetMsg().c_str() );
            if(mMsgNoCore.GetMsg() == " "){ //si es un borrado
                mMsgNoCore.SetMostrado(true);
            }
            this->Sleep(mMsgNoCore.GetPausa());
            frame.MostrarMensaje( mUltimoSMsg.c_str() ); // muestra el ultimo principal del la lista
        }
        if(cnt == CantMaxMsgPant){
            cnt = 0;
        }
        Sleep(10);
    }
    
}

int MsgPantalla::ExisteEnCola(const wxString &pMsg,int pos){
    int cont = 0;
    int encolado = 0;
    int posAnt = 0;
    if(pMsg != " "){
        while(cont < CantMaxMsgPant){
            if(mListMsg[cont].GetMsg() == pMsg && !mListMsg[cont].GetMostrado()){
                encolado = 1;
                cont = CantMaxMsgPant;
            }else{
                cont++;
            }
        }
    }else{
        //calcula la posicion anterior
        posAnt = (pos == 0) ? (CantMaxMsgPant - 1) : (pos - 1);
        if(mListMsg[posAnt].GetMsg() == " "){
            encolado = 1;   //si el msg anterior es un borrado, retorna como que ya esta
        }else{
            encolado = 0;   //si el msg anterior no es un borrado, retorna que no esta en la cola
        }
    }
    return encolado;
}
