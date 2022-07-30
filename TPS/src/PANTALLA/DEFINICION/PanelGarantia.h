#pragma pack (push,8) 
#ifndef __PANEL_GARANTIA_H
#define __PANEL_GARANTIA_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h"


class PanelGarantia : public CPanel
{
  public:
    PanelGarantia( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 

    void createLabels();
    void Init();
	void setImporteLabel( const string &s );
	void setFolioLabel( const string &s );
	void setCodArticuloLabel( const string &s );
	void clearInputs();
	void setPosInputBox( int Action );
	CInput *getFocus();

    void createXML();
    void createInputXML();
    void createLabelXML();

  private:
    CLabel *ImporteLabel;
	CLabel *CodArticuloLabel;
	CLabel *FolioLabel;
    CInput *FolioInput;
	CInput *CodArticuloInput;
	CInput *ImporteInput;
	CInput *input_focus;
};
#endif
#pragma pack (pop) 
