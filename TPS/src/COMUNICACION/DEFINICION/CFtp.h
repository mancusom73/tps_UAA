/*******************************************************************************
*   Sistema: TPV 1000 - IV
*
*   Clases      :   CFtp
*   Descripcion :   Manejador de trasferencias por ftp
*
*   Autor
*   -----               
*   Forray Juan Pablo
********************************************************************************/

#pragma pack (push,8) 
#ifndef __CFTP_H
#define __CFTP_H

#include <wx/socket.h>
#include <wx/protocol/ftp.h>
#include <wx/thread.h>
#include <wx/splash.h>
#include <wx/gauge.h>



class CFtp : public wxThread
{
  public:
    CFtp(wxPoint pPosPaneles);
    void * Entry();
    void ObtenerFtp(wxString pUser,wxString pPassword,wxString pOrigen,wxString pDestino, int pModo);
    wxString StrError;
    void setIpOrNameServer(wxString pIpOrNameServer){
        mIpOrNameServer = pIpOrNameServer;
    }
    unsigned long getIdComandoScript( );
    int getError( );
    int getFinalizado( );
    void setIdComandoScript( unsigned long pIdComandoScript );
    const char *getStrError( );
    void Dummy( wxFocusEventFunction& event );
    wxSplashScreen *panel;
    int getReintento();
    void SetClearReintentos();
	unsigned long getIdNumeroSecuencia( );
	void setgetIdNumeroSecuencia( unsigned long pIdNumeroSecuencia );

  private:
    wxString mUser;
    wxString mPassword;
    wxString mOrigen;
    wxString mDestino;
    wxString mIpOrNameServer;
    int mModo;                //ASCCI, BINARIO
    int mReintento;           //Contador de reintentos
    wxClientDC *dc;
    wxBitmap bitmap;
    wxFont font;
    wxGauge *m_gauge;
    wxFTP ftp;
    bool mConectar;
    unsigned long mIdComandoScript;       //indica si ya esta descargando un archivo guardando el id del comando
    int mFinalizado;            //indica si ya se finalizo la transferencia
    int mError;                 //indica si hubo error en la transferencia
	unsigned long mpIdNumeroSecuencia;//indica El nro de archivo cuando se piden varios en un mismo script

    void MostrarDetalle(int pMaxValue);
    int Conect();
    void Desconect();
    void IncrementarGauge();
    void Fin();


};

#endif

#pragma pack (pop) 