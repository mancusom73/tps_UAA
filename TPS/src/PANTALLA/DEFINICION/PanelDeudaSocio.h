#pragma pack (push,8) 
#ifndef __PANEL_DEUDA_SOCIO_H
#define __PANEL_DEUDA_SOCIO_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CScreencfg.h"


class PanelDeudaSocio : public CPanel
{
  public:
    PanelDeudaSocio( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

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
    void AddDeuda( const string &fecha, const string &tipoMov, const string &nroComp,
                   const string &cuotas, const string &importe, const string &impMin );
    void ScrollDeuda( const string &lines );
    void ClearGrid();

  private:
    CGrid *Deuda;

    CLabel *TitPanel;
    CLabel *SocioLabel;
    CLabel *TitGrid;

    CInput *input_grid_deuda;

    void SetLabelVisibles( bool visible );
};
#endif

#pragma pack (pop) 
