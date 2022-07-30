#pragma pack (push,8) 
#ifndef __APPMAIN_H
#define __APPMAIN_H

#pragma warning (disable:4786)
#include "CScreencfg.h"

#include "CObject.h"
#include "CPanel.h"
#include "CInput.h"
#include "CIMain.h"
#include "config.h"


class AppMain : public wxApp
{
  public:
    AppMain();
    virtual ~AppMain();
    virtual bool OnInit();
    virtual int OnExit();
    void OnDispatchEvent( wxCommandEvent &event );
    bool ShowPanel( CPanel *panel, CInput *input );
    bool HidePanel( CPanel *panel );
    bool ChangeFocus( CInput *input );
    void OnKey( wxKeyEvent &event );
    void Dummy( wxMouseEvent &event );
    void Dummy1( wxScrollEvent &event );
    static const wxEventType wxPROTO_DISPATCH_EVENT;
    bool Launch();
	void liberar();
    #ifdef COMPILAR_UML
    DECLARE_CLASS( AppMain )
    DECLARE_EVENT_TABLE()
    #endif
};

DECLARE_APP( AppMain );
#endif
#pragma pack (pop) 
