#pragma pack (push,8) 
#ifndef __PANEL_PAGOS_ESP_H
#define __PANEL_PAGOS_ESP_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "GuiProperties.h"
#include "CBrowse.h"
#include "PanelBrowse.h"
#include "CPanelBrowse.h"
#include "cr.h"
// 27/06/07 FR pragma Notar que ahora queda el pack en 8 a diferencia del 1 como era antes del mecanismo de pila en cr.h
#include "CIcon.h"
#include "CScreencfg.h"


class PanelPagosEsp : public CPanelBrowse
{
  public:
    CScreencfg *screenXml;
    CLabel *TitNroSoc;
    CLabel *TitNroReint;
    CLabel *TitImport;
    CLabel *TitFormaPago;
    CLabel *TitCheque;
    CLabel *NroSoc;
    CLabel *NroReint;
    CLabel *NImport;
    CLabel *NomMedio;
    CLabel *NomSubmedio;
    CLabel *NroCheq;
    CLabel *NomBanco;

    CLabel *titCantidad;
    CLabel *titTotal;
    CLabel *cant_pc;
    CLabel *total_pc;
    CLabel *label_mensaje;

    /*CInput *input_comprob;
    CInput *input_cuenta;
    CInput *input_importe;
    CInput *input_gravado;
    CInput *input_exento;
    CInput *input_iva;*/

    PanelPagosEsp( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    void init();
    void createLabels();
    void createTitles();
    void setPosInputBox( int Action );
    void setNroSoc( const string &s );
    void setNroReint( const string &s );
    void setNImport( const string &s );
    void setNomMedio( const string &s );
    void setNomSubmedio( const string &s );
    void setNroCheq( const string &s );
    void setNomBanco( const string &s );

    void setTitCantidad( const string &s );
    void setTitTotal( const string &s );
    void setTotal_pc( const string &s );
    void setCant_pc( const string &s );
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };
    CInput *getFocus();
    void clearInputs();
    void setModo();
    //void ShowDiscrimina(bool show);

    void createIconXML();
    //void createInputXML();
    void createLabelXML();
    void createBrowseXML();

    void setLabelMensaje( string text );

  private:

    CIcon modo;

    CInput *input_focus;



    int SelectedField;
    int id;
    int width;
    int numberOfFields;
};

#endif


#pragma pack (pop) 
