#pragma pack (push,8) 
#ifndef __PANEL_CHEQUES_H
#define __PANEL_CHEQUES_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h"


class PanelCheques : public CPanel
{
  public:
    PanelCheques( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

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
    void AddCheque( const string &s, const string &s1, const string &s2,
                   const string &s3, const string &s4 );
    void ScrollCheques( const string &lines );
    void ClearGrid();

  private:
    CGrid *Cheques;

    CLabel *TitPanel;
    CLabel *ChequesLabel;
    CLabel *TitGrid;

    CInput *input_grid;

    void SetLabelVisibles( bool visible );
};
#endif

#pragma pack (pop) 
