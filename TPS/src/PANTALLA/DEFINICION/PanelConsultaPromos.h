#pragma pack (push,8) 
#ifndef __PANEL_CONSULTAPROMOS_H
#define __PANEL_CONSULTAPROMOS_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h" 


class PanelConsultaPromos : public CPanel
{
  public:
    PanelConsultaPromos( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 
	CInput *InputCodBarra;

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
    void AddPromo( const string &nombre, const string &descripcion );
    void ScrollConsultaPromos( const string &lines );
    void ClearGrid();

    void clearInputs();
	void setPosInputBox( int Action );

  private:
    CGrid *ConsultaPromos;

    CLabel *TitPanel;
    CLabel *ConsultaPromosLabel;
    CLabel *TitGrid;
	CInput *input_grid;
	CInput *input_focus;

    void SetLabelVisibles( bool visible );
};
#endif

#pragma pack (pop) 
