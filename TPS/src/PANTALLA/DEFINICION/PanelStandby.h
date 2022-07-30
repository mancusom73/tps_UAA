#pragma pack (push,8) 
#ifndef __PANEL_STANDBY_H
#define __PANEL_STANDBY_H
#pragma warning (disable:4786)
#include <string>
#include "CPanel.h"
#include "CGrid.h"
#include "CInput.h"
#include "CLabel.h"
#include "CScreencfg.h"

class PanelStandby : public CPanel
{
  public:
    PanelStandby( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    void createLabels();
    void create();

    void setTitulos( const string &s, const string &s1 );
    CInput * getFocus()
    {
        return input;
    };
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

    void createLabelXML();

    void setLabelXML( CLabel *label, string name );

    CLabel *label_mensaje;
    void ShowHora(const wxString &SHora);
    void ShowFecha(const wxString &SFecha);
    CScreencfg *screenXml;
  private:
    CLabel *TitHora;
    CLabel *TitFecha;
    CLabel *Fecha;
    CInput *input;
    CLabel *titulo,*titulo1;
};

#endif

#pragma pack (pop) 
