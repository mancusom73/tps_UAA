#pragma pack (push,8) 
#ifndef __CGRID_H
#define __CGRID_H

#include "CPanel.h"
#include "CObject.h"
#include "config.h"

#define _NORMAL_NORMAL 0
#define _NORMAL_BOLD   1
#define _ITALIC_NORMAL 2
#define _ITALIC_BOLD   3
#define _NORMAL_LIGHT  4
#define _ITALIC_LIGHT  5
#define _SLANT_NORMAL  6
#define _SLANT_LIGHT   7
#define _SLANT_BOLD    8

struct browse_field
{
    string Name;
    int Alignment;
    int Width;
    int Type;
};

class CGrid : public CObject, public wxGrid
{
  public:
    wxFont font;
    wxColour backgroundColour;
    wxColour fontColour;
    wxColour selBackgroundColour;
    void setParent( CPanel *_parent, wxPoint p, wxSize s );
    void setParent( CPanel *_parent, wxPoint p, wxSize s, long style );
    CGrid()
    {
    };
    CGrid( CPanel *_parent, wxPoint p, wxSize s );
    CGrid( CPanel *_parent, wxPoint p, wxSize s, long style );
    ~CGrid();
    virtual void Draw( wxDC *dc );
    virtual void Draw();
    void Init( const vector<browse_field> &fieldsDefinition, int selected_field, int width );
    void AddRow( const vector<string> &row );
    void InsertRow( int linea, const vector<string> &row );
	void UpdateRow( int linea, const vector<string> &row );
    void RemoveRow( int linea );
    void SetRowsSize( int size );
    int getRowSize( void );
    int getHeight( void );
    int getNumFilas( void );
    void _SetRowSize( const string &f );
    void iniFont( const string &f );
    void iniColour( const string &f, int type );
    int getWidthColGrid( int ind1, int ind2 );
    void setWidthColGrid( int ind1, int ind2, int w );
    void setWidthColBrowse( const string &f, int ind1 );
  private:
    vector<int> col_alignment;
    CPanel *parent;
    int row_size;
    int height;
    int width;
    void OnKey( wxKeyEvent &event );
    void Dummy( wxGridEvent &event );
    void Dummy1( wxGridSizeEvent &event );
    void Dummy2( wxMouseEvent &event );
    void Dummy3( wxFocusEvent &event );
    int vectWidth[CANTIDAD_PANELES_BROWSE][30];

    #ifdef COMPILAR_UML
    DECLARE_CLASS( CGrid )
    DECLARE_EVENT_TABLE()
        #endif
};
#endif

#pragma pack (pop) 
