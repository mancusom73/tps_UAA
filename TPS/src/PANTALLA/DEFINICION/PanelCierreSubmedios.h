#pragma pack (push,8) 
#ifndef __PanelCierreSubmedios_H
#define __PanelCierreSubmedios__H

#include "CPanel.h"
#include "CLabel.h"
#include "CInput.h"
#include "CGrid.h"
#include "CIcon.h"
#include "CFlag.h"
#include "GuiProperties.h"
#include "CScreencfg.h"

class PanelCierreSubmedios : public CPanel
{
  public:

	CScreencfg *screenXml;

    PanelCierreSubmedios( const wxString &title, const wxPoint &pos, const wxSize &size,
                          long style );

    CLabel *label_mensaje;
    string textomodo[10];

    void createLabels();
    void create();
    void init( const string &s );
    void cerearSubmedios();
    void cerearMedios();
    void mostrarImporteMedioInf( const string &s, const string &s1, int type );
    void mostrarImporteMedioDif( const string &s, const string &s1, int type );
    void agregarMedioCierreSub( const string &s );
    void mostrarItemRendicion( const string &s, const string &s1, int type );
    void resaltarMedioElegido( const string &s, int type );
    void resaltarSubmedioElegido( const string &s, int type );
    void mostrarSumaCierre( const string &s, const string &s1, int type, int x );
	void mostrarSumaParcialMedio( const string &s, const string &s1, int type, int x );
	void mostrarSumaParcialSubmedio( const string &s, const string &s1, int type, int x );
    void mostrarImporteSubmedio( const string &s, const string &s1, int type );
	void mostrarImporteSubmedioDif( const string &s, const string &s1, int type );  
    void mostrarMsgPaginaCierre( int type );
	void borrarImporteSubmedioDif();
	void ocultarImporteSubmedioDif( int type );
    CInput *getFocus();
    void Draw()
    {
    };
    void Draw( wxDC *dc )
    {
    };

    void createIconXML();
    void createInputXML();
    void createLabelXML();


  private: 

    CInput *InputMedios[10];
    CInput *InputSubmedios[10];

    CLabel *LabelSubmediosImporte[10];
    CLabel *LabelSubmedios[10];
    CLabel *LabelMediosImporteInf[10];
    CLabel *LabelMediosImporteDif[10];
	CLabel *LabelSubmediosImporteDif[10];/**/

    int medio_selected;
    int submedio_selected;


    CIcon medios1;
    CIcon medios2;
    CIcon medios3;
    CIcon medios4;
    CIcon medios5;
    CIcon medios6;
    CIcon medios7;
    CIcon medios8;
    CIcon medios9;
    CIcon medios10;
    CIcon title;


    CLabel *medios_title,*sub_medios_title;
    CInput *dummy;

    int med_index,sub_med_index;
};

#endif


#pragma pack (pop) 
