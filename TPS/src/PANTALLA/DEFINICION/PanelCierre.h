#pragma pack (push,8) 
#ifndef __PANEL_CIERRE_H
#define __PANEL_CIERRE_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "GuiProperties.h"
#include "CScreencfg.h"

class PanelCierre : public CPanel
{
  public:
    PanelCierre( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CLabel *label_mensaje;
    CScreencfg *screenXml;

    void createLabels();
    void create();

    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };
    CInput *getFocus();

    void createXML();
    void createLabelXML();


  private:


    CInput *dummy;
};

#endif


#pragma pack (pop) 
