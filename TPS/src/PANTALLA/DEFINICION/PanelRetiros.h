#pragma pack (push,8) 
#ifndef __PANEL_RETIROS_H
#define __PANEL_RETIROS_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CIcon.h"
#include "CFlag.h"
#include "CScreencfg.h"


class PanelRetiros : public CPanel
{
  public:
    PanelRetiros( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CLabel *label_mensaje;

    CScreencfg *screenXml;

    CInput *getFocus();
    void init( const string &titulo );
    void createLabels();

    void setTituloRetiros( const string &titulo );
    void setTitotalRetiros( const string &s );
    void addFlag( const string &s, const string &s1, int type );

    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

    void borrarMedios();
    void muestraTotal( const string &s );
    void borrarPago();
    void borrarPagos();
    void agregarPago( bool aditivo, const string &s1, const string &s2, const string &s3,
                      const string &s4 );
    void muestraTitulo( int type, const string &s, const string &s1 );
    void desseleccionaMedio();
    void resaltarMedio( int type, const string &s, const string &s1 );

    void createInputXML();
    void createLabelXML();
    void createFlagXML();
    void createGridXML();
	void createIconXML();

  private:
    vector<CLabel*> label_list_medios;
    vector<string> str_list_color;
    list<int> index_data;

    CWindow *retiros;
    CRepository *repository;
    CSkin *def;


    CLabel *total;
    CLabel *descMedios;

    CLabel *TitotalPpagos;
	CLabel *labelGridPagos;

    CLabel *labelpago;
    CLabel *label_medios;
    
	CIcon label_titulo;
    
    CLabel *titulo_box;

    CInput *input;
    CGrid *pagos;
    CGrid *datos;
    string cur_str_color;

    CFlag labelFlagmedio0;
    CFlag labelFlagmedio1;
    CFlag labelFlagmedio2;
    CFlag labelFlagmedio3;
    CFlag labelFlagmedio4;
    CFlag labelFlagmedio5;
    CFlag labelFlagmedio6;
    CFlag labelFlagmedio7;
    CFlag labelFlagmedio8;
    CFlag labelFlagmedio9;
    CFlag labelFlagmedio10;

    void getLabelPanelAttribute( CTLabel *label, string &text, string &font, string &color,
                                 string &align );
    CComponent *getComponentAndPos( string name, int &x, int &y );
    CComponent *getComponentAndPos( string name, string type, int &x, int &y );
};

#endif

#pragma pack (pop) 
