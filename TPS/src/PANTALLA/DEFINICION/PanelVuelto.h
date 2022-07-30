#pragma pack (push,8) 
#ifndef __PANEL_VUELTO_H
#define __PANEL_VUELTO_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h"


class PanelVuelto : public CPanel
{
  public:
    PanelVuelto( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 

    void createLabels();
    void Init();

    void createXML();
    void createInputXML();
    void createLabelXML();
    void createGridXML();

    void setSocioLabel( const string &s );

    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

    void scroll( const string &lines );

    CInput *getFocus();
    wxWindow *targetWin;
    void PanelVuelto::AddVuelto( const string &codigo, const string &descripcion, const string &vuelto);
    void ScrollVuelto( const string &lines );
    void ClearGrid();

  private:
    CGrid *Vuelto;
    CLabel *TitPanel;
    //CLabel *SocioLabel;
    CLabel *TitGrid;
    CInput *input_grid_vuelto;

    void SetLabelVisibles( bool visible );
};
#endif

#pragma pack (pop) 
