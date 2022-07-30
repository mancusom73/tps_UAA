#pragma pack (push,8) 
#ifndef __PANEL_MENU_H
#define __PANEL_MENU_H

#define MAXMENU 10
#pragma warning (disable:4786)
#include <string>
#include "CPanel.h"
#include "CGrid.h"
#include "CInput.h"
#include "CLabel.h"
#include "CScreencfg.h"

class PanelMenu : public CPanel
{
  public:
    PanelMenu( const wxString &title, const wxPoint &pos, const wxSize &size );
    //PanelMenu();
    void createLabels();
    void init();
    int Menu( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum );
	int Menu_Present( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum );
    void create( void );
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };
    CInput *getFocus();
    void changeFocus( int type );
    CInput *input;
        
  private:    
    string items_font,items_color,items_align;
    string title_font,title_color,title_align;
    int width,height;
    char SelectedField;
    int focus;
};
#endif

#pragma pack (pop) 
