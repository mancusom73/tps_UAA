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


#pragma pack (push,8) 
#ifndef __MSG_PANTALLA_H
#define __MSG_PANTALLA_H

#include <wx/string.h>
#include "path.h"
#include <wx/thread.h>

#define CantMaxMsgPant 8

class CMensajeP
{
    public:
        CMensajeP(){
            mMostrado = true;
        };
        void SetMsg(wxString pSMsg);
        wxString GetMsg();
        bool GetMostrado();
        void SetMostrado(bool pMostrado);
        void SetPausa(unsigned long pPausa);
        unsigned long GetPausa();
    private:
        wxString mSMsg;
        bool mMostrado;
        unsigned long mPausa;

};

class MsgPantalla : public wxThread
{
  public:
    MsgPantalla() : wxThread( wxTHREAD_DETACHED )
    {
        mEsperar = false;
        cntAdd = 0;
    };
    void AddMsg(const wxString &pMsg,unsigned long pPausa, int pCore);
    void * Entry();

  private:
    CMensajeP mListMsg[CantMaxMsgPant];
    wxString mUltimoSMsg;
    CMensajeP mMsgNoCore;
    int cntAdd;
    bool mEsperar;
    int ExisteEnCola(const wxString &pMsg, int pos);

};

#endif

#pragma pack (pop) 
