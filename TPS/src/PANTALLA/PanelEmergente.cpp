#pragma warning (disable:4786)
#include "PanelEmergente.h"
#include "CConfig.h"
#include <wx/event.h>
extern CConfig *Config;

void CMensaje::SetCantLineas(int pCantLineas)
{
    mCantLineas = pCantLineas;
}

int CMensaje::GetCantLineas(void)
{
    return mCantLineas;
}

void CMensaje::SetTitulo(wxString pSTitulo)
{
    mSTitulo = pSTitulo;
}

wxString CMensaje::GetTitulo(void)
{
    return mSTitulo;
}

void CMensaje::SetMsg(wxString pSMsg,int pPos)
{
    mSMsg[pPos] = pSMsg;
}

wxString CMensaje::GetMsg(int pPos)
{
    return mSMsg[pPos];
}

void CMensaje::SetClear()
{
    mCantLineas = 0;
    mSTitulo.Clear();
}




void * PanelEmergente::Entry()
{
    cnt = 0;
    cnt2 = 0;
    unsigned long TiempoMils = 0;
    //mutexProtecting->Lock();
    panel = new wxSplashScreen( mbitmap,wxSPLASH_NO_TIMEOUT, 6000, NULL, -1,
                                                            wxPoint(mPosicion.x,mPosicion.y), wxSize(0,0),
                                                            wxSIMPLE_BORDER|wxSTAY_ON_TOP  );
    panel->Enable(false);
    panel->SetSize(cWidth,0);
    dc = new wxClientDC(panel);
    dc->DrawBitmap( mbitmap, 0, 0 );
    MostrarTexto();
    init();
    Sleep(TiempoMils = (Config->getTiempoPopUp() * 1000));
    //mutexProtecting->Lock();
    fin();
    //mutexProtecting->Unlock();
    while(1){
        this->Pause();
    }
}
PanelEmergente::~PanelEmergente()
{
    
}

void PanelEmergente::createLabels()
{
    mTitulo.SetPointSize( 12 );
    mTitulo.SetStyle( wxNORMAL );
    mTitulo.SetWeight( wxBOLD );
    mMsg.SetPointSize( 11 );
    mMsg.SetStyle( wxNORMAL );
    mMsg.SetWeight( wxBOLD );
    this->create();
}

void PanelEmergente::create()
{
}


void PanelEmergente::init()
{
    int Salir = 0;
    while(Salir == 0){
        
        if(cnt >= cHeight){
            Salir = 1;
            cnt = cHeight;
        }
        //dc->BeginDrawing();
        panel->Move(mPosicion.x,mPosicion.y - cnt);
        panel->SetSize(cWidth,cnt);
        dc->DrawBitmap( mbitmap, 0, 0 );
        MostrarTexto();
        //dc->EndDrawing();
        cnt+=40;
        this->Yield();
        this->Sleep(30);
    }
    MostrarTexto();

}
void PanelEmergente::fin()
{
    int Salir = 0;
    while(Salir == 0){
        if(cnt <= 0)
            Salir = 1;
        dc->BeginDrawing();
        panel->SetSize(cWidth,cnt);
        panel->Move(mPosicion.x,mPosicion.y - cnt);
        dc->DrawBitmap( mbitmap, 0, 0 );
        MostrarTexto();
        dc->EndDrawing();
        cnt-=15;
        //wxSafeYield(NULL, true);
        this->Sleep(15);
    }
    wxDELETE(dc);
    wxDELETE(panel);
    mRun = false; //indica que finalizó la ejecucion
}

wxSize PanelEmergente::getSize()
{
    return mSize;
}

bool PanelEmergente::getStatusRun()
{
    return mRun;
}

void PanelEmergente::MostrarTexto()
{
    int cntLineas = 1;
    dc->SetFont( mTitulo );
    dc->DrawText( mSMsg.GetTitulo(), 40, 3 );
    dc->SetFont( mMsg );
    while(cntLineas <= mSMsg.GetCantLineas()){
        dc->DrawText(mSMsg.GetMsg(cntLineas - 1), 10, (20 + (cntLineas * 15)) );
        cntLineas++;
    }
    
}

