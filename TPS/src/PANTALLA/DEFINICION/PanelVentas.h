#pragma pack (push,8) 
#ifndef __PANEL_VENTAS_H
#define __PANEL_VENTAS_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CIcon.h"
#include "CFlag.h"
#include "GuiProperties.h"
#include "CScreencfg.h"


class PanelVentas : public CPanel
{
  public:
    PanelVentas( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CScreencfg *screenXml; 

    CLabel *label_mensaje;
    CIcon leyendasubtotal;

    string textomodo[10];



    void createLabels();
    void init();

    void createIconXML();
    void createInputXML();
    void createLabelXML();
    void createFlagXML();
    void createGridXML();
    void setNormalBackground();
    void setErrorBackground();

    void addArticle( const string &signo, const string &cantidad, const string &cinterno,
                     const string &cbarra, const string &nombre, const string &precio );
	void updArticle( int yRow, const string &signo, const string &cantidad, const string &cinterno,
                     const string &cbarra, const string &nombre, const string &precio );
    void delArticle( int row );
    void setTotal( const string &s );
    void setDetalle( const string &s );
    void setBultos( const string &s );
    void setTitBultos( const string &s );
    void setModo( const string &s );
    void setDescripcion( const string &s );
    void setCantidad( const string &s );
    void setSubtotal( const string &s );
    void setMultiplica( const string &s );
    void setFlagVisible( int type );
    void setPropertiesLabelsInit();
    void setFlagComprobNotVisible();
    void setFlagCambioNotVisible();
    void setFlagTipoDevNotVisible();
    void setFlagModoNotVisible();
	void ShowImgArt(const string &cod_barra);
    void addFlag( const string &s, const string &s1, int type );
	void setFlagMonederoNotVisible();
    void showCashier( int type, const string &s, const string &s1 );
	void setCantConsecutivos( const string &s );
	void setCantIdenticos( const string &s );
	void setDescContador( const string &s );
	void setTamanioTotal( );
	int  getTamanioTotal( );
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

    void scroll( const string &lines );
    void batch( const string &status );

    CInput * getFocus()
    {
        return input_barra;
    };
    wxWindow *targetWin;
    void ShowHora(const wxString &SHora);
    void ShowFecha(const wxString &SFecha);
    void ShowFechaOp(const wxString &SFecha);
    void ShowRenglones( bool type );
    void PanelVentas::ShowTipoInventario( const wxString &SInvetario);
	void PanelVentas::SetFocus( );
  private:
    CGrid *datos;

    CFlag labelFlagClie;
    CFlag labelFlagTke;
    CFlag labelFlagTarj;
    CFlag labelFlagVend;

    CFlag labelFlagDesc;
    CFlag labelFlagCambioMas;
    CFlag labelFlagCambioMenos;
    CFlag labelFlagFac; 
    CFlag labelFlagNc;
	
    CFlag labelFlagDev;
    CFlag labelFlagDevItem;

    CFlag labelFlagAfinidad;
    CFlag labelFlagPeticion;
    CFlag labelFlagPresentacion;
    CFlag labelFlagMutual;
    CFlag labelFlagDevolucion;
    CFlag labelFlagCambio;
    CFlag labelFlagBienUso;

    CFlag labelFlagEntrenamiento;
	CFlag labelFlagModInventario;
	CFlag labelFlagTipoInventario;
    CFlag labelFlagDetList;
    CFlag labelFlagPerfil;
    CFlag labelFlagMozo;
    CFlag labelFlagCupo;
    CFlag labelFlagErrorRed;
    CFlag labelFlagRetDgr;

    CFlag labelFlagTf;
	CFlag labelFlagMonedero;
    CIcon labelFlagBarr;
    CIcon labelArt;
	CIcon fotoCajero;
    CIcon TitBultos;
    CIcon TitRenglones; //mmancuso
    CIcon Modo;

    CIcon labelFlagCr;
    CIcon tEspera_remoto;

    CLabel *labelTicket;
    CLabel *labelGridVentas;

    // --------------- //
    CLabel *Detalle;
    CLabel *Total;
    CLabel *Bultos;
    CLabel *IBultos;

    CLabel *Descripcion;
    CLabel *Cantidad;
    CLabel *Subtotal;
    CLabel *Multiplica;

    CLabel *nCajero;
    CLabel *nroCaja;
    CLabel *modoEjecucion;

    CLabel *TitHora;
    CLabel *TitFecha;
    CLabel *Fecha;

    CLabel *TitFechaOp;
    CLabel *FechaLocal;
    CInput *input_barra;
    CGrid *ticket;
	
	CLabel *CantConsecutivos;
	CLabel *CantIdenticos;
	CLabel *DescContador;
	CLabel *TipoInventario;

    string backgroundLocation;

    int x_total,y_total;
    int x_modo,y_modo;
    bool mostrar_flag_articulo;
};
#endif

#pragma pack (pop) 
