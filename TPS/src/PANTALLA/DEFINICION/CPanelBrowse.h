#pragma pack (push,8) 
#ifndef __PANEL_P_BROWSE_H
#define __PANEL_P_BROWSE_H
#pragma warning (disable:4786)
#include <string>
#include "CBrowse.h"
#include "CPanel.h"
#include "CInput.h"
#include "CLabel.h"

class CPanelBrowse : public CPanel
{
  public:
    CInput *input_browse_del_panel;
    CBrowse *browse_del_panel;
    CLabel *labelbrowse;
    CLabel *tituloPanelBrowse;
    int vectCoord[CANTIDAD_PANELES_BROWSE][30];
    string detailTit[CANTIDAD_PANELES_BROWSE][CANTIDAD_CARACTERISTICAS];
    string detailPanelBrowse[CANTIDAD_PANELES_BROWSE][CANTIDAD_CARACTERISTICAS];

    CPanelBrowse( const wxString &title, const wxPoint &pos, const wxSize &size );

    void createLabels();
    void create();
    void SetDefaultBrowseBrowse( CBrowse *b )
    {
        browse_del_panel = b;
    };
    void SetDefaultInputBrowse( CInput *i )
    {
        input_browse_del_panel = i;
    };
    CInput * getFocus()
    {
        return input_browse_del_panel;
    };
    void setCharTit( const string &f, int type, int ind1 );
    string getCharTit( int ind1, int ind2 );
    void setSizePanelBrowse( const string &f, int ind1 );
    int getSizePanelBrowse( int ind1, int ind2 );
    void setCharPanelBrowse( const string &f, int type, int ind1 );
    string getCharPanelBrowse( int ind1, int ind2 );
};
#endif

#pragma pack (pop) 
