#pragma pack (push,8) 
#ifndef __PANEL_CAMBIO_MEDIO_H
#define __PANEL_CAMBIO_MEDIO_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CIcon.h"
#include "CFlag.h"
#include "CScreencfg.h"


class PanelCambioMedio : public CPanel
{
  public:
    PanelCambioMedio( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CLabel *label_mensaje;
    CScreencfg *screenXml;

    CInput *getFocus();
    void init( const string &titulo );
    void createLabels();
    void create();
    void setTituloCambioMedio( const string &titulo );
    void setTitotalCambioMedio( const string &s );
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
	void addDatosRow( const string &s1 );
    void delDatosRow();
	int GetNumberRows();

    void createIconXML();
    void createInputXML();
    void createLabelXML();
    void createFlagXML();
    void createGridXML();


  private:
	vector<int> datosIndex;

    CLabel *total;
    //CLabel *descMedios;
    CLabel *TitotalPpagos;
    //CLabel *labelpago;
    CLabel *label_medios;
    CLabel *label_titulo;
    CInput *input;

    CGrid *pagos;
    CGrid *datos;


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
    CFlag labelFlagDevItem;

    CLabel *titulo_box;
};

#endif

#pragma pack (pop) 
