#pragma pack (push,8) 
#ifndef __CBROWSE_H
#define __CBROWSE_H

#include "CGrid.h"
#include "CInput.h"
#include "config.h"

class CBrowse : public CGrid
{
  public:
    CPanel *_panel_browse;
    CInput *_input_browse;
    CBrowse( CPanel *_parent, wxPoint p, wxSize s );
    ~CBrowse();
    // Copia de codigo de java
    void init( int id, const vector<browse_field> &fieldsDefinition, int selected_field,
               int width, int pAlto );
    //void setTituloBrowse(int id,const wxString &titulo);
    void setEnable( int id, bool state );
    void selRow( int id, int linea );
    void scroll( int id, int lineas );
    void close( int id );
    void setRow( int id, const vector<string> &row );
	void setRow( int id, const vector<string> &row, int agrego );
    void insertRow( int id, int linea, const vector<string> &row );
    void setPanelBrowseActivo( char *p )
    {
        _panel_browse = ( CPanel * )p;
    };
    void setInputBrowseActivo( CInput *i )
    {
        _input_browse = i;
    };
    void deleteRows( int rows );
    void deleteInput();
    void setTargetWindow( wxWindow *target );
	int alto;
};
#endif

#pragma pack (pop) 
