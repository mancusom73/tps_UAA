#pragma pack (push,8) 
#ifndef __PANEL_PAGOS_H
#define __PANEL_PAGOS_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CIcon.h"
#include "CFlag.h"
#include "CScreencfg.h"

class PanelPagos : public CPanel
{
  public:
    PanelPagos( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 
    CLabel *label_mensaje;
    string textomodo[10];

    CInput * getFocus()
    {
        return input;
    };
    void init( const string &titulo );
    void createLabels();
    void setTitotalPpagos( const string &s );
    void addFlag( const string &s, const string &s1, int type );

    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

	bool isCashActivo( );
    void borrarMedios();
    void muestraTotal( const string &s );
    void setImporteTemporal( const string &s );
    void setResto( const string &tit, const string &imp, const string &imp1 );
    void borrarPago();
    void borrarPagos();
    void muestraTitulo( int type, const string &s, const string &s1 );
    void resaltarMedio( int type, const string &s, const string &s1 );
    void desseleccionaMedio();
    void agregarPago( bool aditivo, const string &s1, const string &s2, const string &s3,
                      const string &s4 );
    void addDatosRow( const string &s1 );
    void delDatosRow();
	int  GetNumberRows();
    void createIconXML();
    void createInputXML();
    void createLabelXML();
    void createFlagXML();
    void createGridXML();
    void refreshGrids();
    void setNormalBackground();
    void setErrorBackground();
	void PanelPagos::SetFocus( );

  private:

    vector<int> datosIndex;

    CLabel *total;
    CLabel *descResto;
	CLabel *descResto2;
    CLabel *titResto;
    CLabel *descMedios;
    CLabel *TitotalPpagos;
    CLabel *label_medios;
    CLabel *titulo_box;
    CLabel *ImporteTemporal;

    CIcon label_titulo;
    CIcon LogoInvel;
	CLabel *labelGridPagos;

    CLabel *labelpago;

    CInput *input;

    CGrid *pagos;
    CGrid *datos;

    int modeDatosTotAnt;                      //luc

    CFlag labelFlagClie;
    CFlag labelFlagVend;
    CFlag labelFlagAfinidad;
    CFlag labelFlagMutual;
    CFlag labelFlagDetList;
    CFlag labelFlagPerfil;
    CFlag labelFlagInt;
    CFlag labelFlagDisciva;
    CFlag labelFlagAutoriza;
    CFlag labelFlagBono;
    CFlag labelFlagNroFac;
    CFlag labelFlagEnvOffline;
    CFlag labelFlagRetDgr;
    CFlag labelFlagTotAnt;
    CFlag labelFlagPerDgi;
    CFlag labelFlagPer212;

    CFlag labelFlagDiferimiento;

    CFlag labelFlagCupo;
    CFlag labelFlagTarj;
    CFlag labelFlagMonExtr;

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
    string backgroundLocation;
};

#endif

#pragma pack (pop) 
