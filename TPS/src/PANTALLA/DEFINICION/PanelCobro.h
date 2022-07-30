#pragma pack (push,8) 
#ifndef __PANEL_COBRO_H
#define __PANEL_COBRO_H

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



class PanelCobro : public CPanelBrowse
{
  public:
    CScreencfg *screenXml; 

    CLabel *titCodArticulo;
	CLabel *titFolio;
    CLabel *titCodBarra;
    CLabel *titComprob;
    CLabel *titCuenta;
    CLabel *titImporte;
    CLabel *titCantidad;
    CLabel *titTotal;
    CLabel *cant_pc;
	CLabel *imp_me;
    CLabel *total_pc;
    CLabel *label_mensaje;

    //CIcon modo;
    CInput *input_Folio;
	CInput *input_CodArticulo;
	CInput *input_CodBarra;
    CInput *input_comprob1;
    CInput *input_cuenta;
    CInput *input_importe;
    CInput *input_focus;

	CIcon IconDevolucionCobro;
    CIcon IconCobroInputCodBarra;
    CIcon IconCobroInputComprob;
    CIcon IconCobroInputCuenta;
    CIcon IconCobroInputImporte;
	////CIcon IconCobroGif;

    string textomodo[10];

    PanelCobro( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    void init();
    void createLabels();
    void setPosInputBox( int Action );
    void setTitCodBarra( const string &s );
    void setTitComprob( const string &s );
    void setTitCuenta( const string &s );
    void setTitImporte( const string &s );
   	void setTitFolio( const string &s );
	void setTitCodArticulo( const string &s );
    void setTitCantidad( const string &s );
    void setTitTotal( const string &s );
    void setTotal_pc( const string &s );
    void setCant_pc( const string &s );
	void setImp_me( const string &s );
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };
    CInput *getFocus();
    void clearInputs();
    void setModo( const string &s );
    void setLabelMensaje( string text );

    void createIconXML();
    void createInputXML();
    void createLabelXML();
    void createBrowseXML( string name );
	void createFlagXML();

	void addIcon( const string &opcion, int type );


  private:
    CWindow *cobros;
    CRepository *repository;
    CSkin *def;

    int SelectedField;
    int id;
    int width;
    int numberOfFields;
	//CIcon IconDevolucionCobro;
	//CIcon IconCobro;

    void getLabelPanelAttribute( CTLabel *label, string &text, string &font, string &color,
                                 string &align );
    CComponent *getComponentAndPos( string name, int &x, int &y );
    CComponent *getComponentAndPos( string name, string type, int &x, int &y );
};

#endif


#pragma pack (pop) 
