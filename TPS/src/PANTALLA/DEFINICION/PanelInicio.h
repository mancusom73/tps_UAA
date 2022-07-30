#pragma pack (push,8) 
#ifndef __PANEL_INICIO_H
#define __PANEL_INICIO_H

#include <wx/splash.h>
#include <wx/gauge.h>
#include <wx/dbtable.h>
#include "CLabel.h"
#include "cr.h"
// 27/06/07 FR pragma Notar que ahora queda el pack en 8 a diferencia del 1 como era antes del mecanismo de pila en cr.h

class PanelInicio : public wxSplashScreen
{
  public:

    PanelInicio( const wxBitmap &bitmap, wxString pathImg ) : wxSplashScreen( bitmap,
                                                            wxSPLASH_CENTRE_ON_SCREEN
                                                          | wxSPLASH_NO_TIMEOUT, 6000, NULL, -1,
                                                            wxDefaultPosition, wxDefaultSize,
                                                            wxSIMPLE_BORDER )
    {
		image = pathImg;
        createLabels();
    };
    wxGauge *m_gauge;
    void init();
    void createLabels();
    void create();
    void setProceso( const string &s );

  private:
    wxFont Procefont;
	wxString image;
    int Value;
};

#endif

#pragma pack (pop) 
