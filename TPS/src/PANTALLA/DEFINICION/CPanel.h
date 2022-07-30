#pragma pack (push,8) 
#ifndef __CPANEL_H
#define __CPANEL_H

#include "CObject.h"
#include "config.h"


#define ID_CPANEL 10001

class CPanel : public wxFrame
{
  public:
    wxPoint pos_click;
    CPanel( CPanel *parent, const wxString &title, const wxPoint &pos, const wxSize &size,
            long style = wxDEFAULT_FRAME_STYLE );
    CPanel( const wxString &title, const wxPoint &pos, const wxSize &size,
            long style = wxDEFAULT_FRAME_STYLE );
    virtual ~CPanel();
    void SetBackGround( const wxString &filename );
    wxBitmap * GetBackGround()
    {
        return &bmp;
    };
    void OnPaint( wxPaintEvent &event );
    void OnPaint(  );
    void AddObject( CObject *o );  
    void DelAllObject( int nitems );
    void ClearObjects();
    bool isVisible();
    void setVisible( bool v );
    void Activate();
	void mYield();
    void DesActivate();
    bool isShowing();
    static void SetLauncher( wxApp *app )
    {
        main_application = app;
    };
    static wxApp * GetLauncher()
    {
        return main_application;
    };
    void PosClick(wxMouseEvent& event);

    vector<CObject*> objects;

  private:
    bool background;
    bool input;
    int i_x;
    int i_y;
    int i_w;
    int i_h;
    wxBitmap bmp;
    wxPanel *panel;
    wxTextCtrl *text;
    static wxApp *main_application;
    #ifdef COMPILAR_UML
    DECLARE_CLASS( CPanel )
    DECLARE_EVENT_TABLE()
        #endif
};
#endif

#pragma pack (pop) 
