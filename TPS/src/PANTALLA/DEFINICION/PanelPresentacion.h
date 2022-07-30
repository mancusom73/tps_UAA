#pragma pack (push,8) 

#ifndef __PANEL_PRESENTACION_H
#define __PANEL_PRESENTACION_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CScreencfg.h"


class PanelPresentacion : public CPanel
{
  public:
    CLabel *NomVersion;
	CLabel *TitFechaOp;
    CLabel *FecOper;
    CLabel *Fecha;
	CLabel *FechaSis;
    CLabel *Usuario;
    CLabel *Password;
    CLabel *label_mensaje;
    CIcon IconPres;
    CInput *input_usuario;
    CInput *input_password;

    CScreencfg *screenXml;

    PanelPresentacion( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    void init();
    void createLabels();

    void createXML();

    void addFlag( const string &s, const string &s1, int type );
    void addData( const string &s, const string &s1, int type );
    int ifFlag( int type );
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };
    void setFecha( const string &s );
    void setUsuario( const string &s );
    void setPassword( const string &s );
    void setFechaOp( const string &s );
    void setPosInputBox( bool usuario );
	void ShowRenglones( bool type);
	void ShowFechaOp(const wxString &SFecha);
	void ShowFechaSis(const wxString &SFecha);
    CInput * getFocus()
    {
        return input_focus;
    };
    CLabel *NomBranch;
    void MostrarFoto( const string &path );
    void FotoDefault();
    void ShowHora(const wxString &SHora);
    void ShowFecha(const wxString &SFecha);
    void setCajeroActual(const wxString &Scajero);
	bool PanelPresentacion::isShowCajeroActual( );
  private:
    CLabel *TitHora;
    CLabel *CajeroActual;
    CInput *input_focus;
};
#endif

#pragma pack (pop) 
