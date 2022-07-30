#pragma pack (push,8) 
#ifndef __PANEL_IMPORTACION_H
#define __PANEL_IMPORTACION_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h"


class PanelImportacion : public CPanel
{
  public:
    PanelImportacion( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 

    void createLabels();
    void Init();
    void createXML();
    void createInputXML();
    void createLabelXML();
    void createGridXML();

    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

    void scroll( const string &lines );

    CInput *getFocus();
    wxWindow *targetWin;
    void AddImportacion( const string &s, const string &s1, const string &s2, const string &s3 );
    void ScrollImportacion( const string &lines );
    void ClearGrid();

  private:
    CGrid *Importacion;

    CLabel *TitPanel;
    CLabel *ImportacionLabel;
    CLabel *TitGrid;

    CInput *input_grid;

    void SetLabelVisibles( bool visible );
	//wxGridCellAttr *habilita;
	//wxGridCellAttr *deshabilita;
};
#endif

#pragma pack (pop) 
