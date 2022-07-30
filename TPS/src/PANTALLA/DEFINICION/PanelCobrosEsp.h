#pragma pack (push,8) 
#ifndef __PANEL_COBROS_ESP_H
#define __PANEL_COBROS_ESP_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CIcon.h"
#include "CFlag.h"
#include "GuiProperties.h"
#include "CScreencfg.h"


class PanelCobrosEsp : public CPanel
{
  public:
    PanelCobrosEsp( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 

    CLabel *label_mensaje;
    CInput *input_focus;

    void createLabels();
    void init( const string &titulo );

    void createXML();
    void createInputXML();
    void createLabelXML();
    void createGridXML();
    void createIconXML();

    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };
    void setMensaje( const string &s );
    void setSocio( const string &s, const string &TipoCli, const string &DomicilioText );
    void setSubTotal( const string &s );
    void scroll( const string &lines );
    CInput *getFocus();
    wxWindow *targetWin;
    void AddUniNegocio( const string &cod, const string &descripcion );
    //void AddCobroGrid(const string &uniNeg,const string &detalle,const string &importe);
    void AddCobroGrid( const string &numeroComp, const string &letra, const string &tipoMov,
                       const string &reglaAntigua, const string &fechaVenc,
                       const string &importeMin, const string &importe, const string &moneda,
                       const string &fechaEmision, const string &fechaHasta,
                       const string &periodoFact );
    void setPosInputBox( int Action );
    void ScrollUniNeg( const string &lines );
    void clear();
    void setOpMult( bool visible );

  private:
    void ScrollGrid( const string &lines, CGrid *Grid );
    CGrid *DetalleCobros;
    CGrid *UniNegocio;

    CLabel *TitCodigo;
    CLabel *TitUniNegocioInput;
    CLabel *TitUniNegocioGrid;
    CLabel *TitSocioInput;
    CLabel *SocioLabel;
    CLabel *TitDetalleGrid;
    CLabel *TitSubtotal;
    CLabel *SubtotalLabel;
    CLabel *TipoCliLabel;
    CLabel *Domicilio;

    CInput *InputCodBarra;        
    CInput *InputUniNegocio;
    CInput *InputSocio;
    CInput *input_grid_uni_neg;

    CIcon LogoInvel;
    CIcon FlagOpMult;

    void SetLabelVisibles( bool visible );
};
#endif

#pragma pack (pop) 
