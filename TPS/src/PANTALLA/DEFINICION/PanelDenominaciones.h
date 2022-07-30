#pragma pack (push,8) 
#ifndef __PANEL_DENOMINACIONES_H
#define __PANEL_DENOMINACIONES_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h"


class PanelDenominaciones : public CPanel
{
  public:
    PanelDenominaciones( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 

    void createLabels();
    void Init();

    void createXML();
    void createInputXML();
    void createLabelXML();
    void createGridXML();

    void scroll( const string &lines );

    CInput *getFocus();
    wxWindow *targetWin;
    void PanelDenominaciones::AddDenominaciones( const string &codigo, const string &descripcion );
    void ScrollDenominaciones( const string &lines );
    void ClearGrid();
    void clearInputs();
	int getRowCursor();
	void setCellText(const string &row, const string &col, const wxString &text);
	void getCellText(const string &row, const string &col, char *str);
	void setSubTotal( const string &s );
	void mostrarInput(const wxString &s, const string &caretPosition);

  private:
    CGrid *Denominaciones;
    CLabel *TitPanel;
    CLabel *TitGrid;
	CLabel *SubTotal;
    CInput *cantidad;
	CLabel *SubTotalTxt;
	CLabel *CantidadTxt;

    void SetLabelVisibles( bool visible );
};
#endif

#pragma pack (pop) 
