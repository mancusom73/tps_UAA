#pragma pack (push,8) 
#ifndef __PANEL_FONDO_FIJO_H
#define __PANEL_FONDO_FIJO_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CIcon.h"
#include "CFlag.h"
#include "CScreencfg.h"

class PanelFondoFijo : public CPanel
{
  public:
    PanelFondoFijo( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CLabel *label_mensaje;

    CScreencfg *screenXml;

    CInput *getFocus();
    void init( const string &titulo );
    void createLabels();
    void setTituloFfijo( const string &titulo );
    void setTitotalFfijo( const string &s );
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

  private:

    CLabel *total;
    CLabel *descMedios;
    CLabel *TitotalPpagos;
	CLabel *labelGridPagos;

    //CLabel *labelpago;

    CLabel *label_medios;
    CLabel *titulo_box;

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
};

#endif

#pragma pack (pop) 
