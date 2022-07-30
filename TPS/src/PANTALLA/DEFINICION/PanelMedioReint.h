#pragma pack (push,8) 
#ifndef __PANEL_MEDIO_REINT_H
#define __PANEL_MEDIO_REINT_H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
//#include "CIcon.h"
//#include "CFlag.h"
#include "CScreencfg.h"

class PanelMedioReint : public CPanel
{
  public:
    CInput *input_de_grilla; 
    CLabel *label_titulo;

    PanelMedioReint( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    CLabel *label_mensaje;

    CScreencfg *screenXml;

    CInput *getFocus();
    void createLabels();
    void init();
    void finish();
    void setTitulo( const string &titulo );
    void addGridDisp( const string &nom_banco, const string &cod_banco, const string &num_cheq,
                      const string &importe, const string &codigo_rein );
    void remGridIng( const string &pos );
    void pedirGridIng( char *s, char *s1 );
    void pedirGridDisp( char *s );
    void addGridIng( const string &nom_banco, const string &cod_banco, const string &num_cheq,
                     const string &importe, const string &codigo_rein );
    void scrollGridDisp( const string &lines, char *cantStat, char *lineaStat );
    void scrollGridIng( const string &lines );
    void removeRowGridDisp( const string &s );
    void removeAllRowsGrid();

    void setMensaje( const string &s );
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

    void muestraTitulo( int type, const string &s, const string &s1 );

    void createLabelXML();
    void createGridXML();

  private:

    CLabel *titDispGrid;
    CLabel *titIngGrid;

    CGrid *gridDisp;
    CGrid *gridIng;
};

#endif
#pragma pack (pop) 
