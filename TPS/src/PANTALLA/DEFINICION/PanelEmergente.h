#pragma pack (push,8) 
#ifndef __PANEL_EMERGENTE_H
#define __PANEL_EMERGENTE_H
#include <wx/timer.h>
#include <wx/splash.h>
#include <wx/app.h>
#include <wx/dbtable.h>

//#include "path.h"
// 27/06/07 FR pragma Notar que ahora queda el pack en 8 a diferencia del 1 como era antes del mecanismo de pila en cr.h
#define cWidth  303
#define cHeight 140



class CMensaje
{
    public:
        CMensaje(){
            mCantLineas = 0;
        };
        void SetCantLineas(int pCantLineas);
        int GetCantLineas(void);
        void SetTitulo(wxString pSTitulo);
        wxString GetTitulo(void);
        void SetMsg(wxString pSMsg,int pPos);
        wxString GetMsg(int pPos);
        void SetClear();
    private:
        wxString mSTitulo;
        wxString mSMsg[50]; //wxString mSMsg[20];
        int mCantLineas;

};



class PanelEmergente : public wxThread
{
  public:

    PanelEmergente( const wxBitmap &bitmap,int x, int y,
        const CMensaje pMsg, wxMutex *Mutex) : wxThread( wxTHREAD_DETACHED )
    {
        mbitmap = bitmap;
        mPosicion.x = x;
        mPosicion.y = y;
        mSMsg = pMsg;
        mutexProtecting = Mutex;
        createLabels();
        mRun = true;
    };
    ~PanelEmergente();
    void init();
    void fin();
    void createLabels();
    void create();
    wxSize getSize();
    void * Entry();
    bool getStatusRun();
    wxSplashScreen *panel;
  private:
    bool mRun; //indica cuando esta en ejecucion
    wxFont mTitulo;
    wxFont mMsg;
    wxSize mSize;
    wxBitmap mbitmap;
    wxClientDC *dc;
    wxPoint mPosicion;
    CMensaje mSMsg;
    wxMutex *mutexProtecting;
    void MostrarTexto();
    int cnt;
    int cnt2;
};

#endif

#pragma pack (pop) 