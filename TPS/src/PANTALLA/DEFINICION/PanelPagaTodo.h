#pragma pack (push,8) 
#ifndef __PANEL_PAGATODO_H
#define __PANEL_PAGATODO_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h"


class PanelPagaTodo : public CPanel
{
  public:
    PanelPagaTodo( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 

    void createLabels();
    void Init();
	void setImporteLabel( const string &s );
	void clearInputs();
	void setPosInputBox( int Action );
	CInput *getFocus();

    void createXML();
    void createInputXML();
    void createLabelXML();

  private:
    CLabel *ImporteLabel;
    CInput *CodBarraInput;
	CInput *ImporteInput;
	CInput *input_focus;
};
#endif
#pragma pack (pop) 
