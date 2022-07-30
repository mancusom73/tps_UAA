#pragma pack (push,8) 
#ifndef __CINPUT_H
#define __CINPUT_H

#include "CPanel.h"
#include "CObject.h"

#include <list>
using namespace std;

#define _NORMAL_NORMAL 0
#define _NORMAL_BOLD   1
#define _ITALIC_NORMAL 2
#define _ITALIC_BOLD   3
#define _NORMAL_LIGHT  4
#define _ITALIC_LIGHT  5
#define _SLANT_NORMAL  6
#define _SLANT_LIGHT   7
#define _SLANT_BOLD    8

class CInput : public CObject, public wxTextCtrl
{
  public:   
    CInput( CPanel *parent );
    CInput( CPanel *parent, int x, int y, int w, int h, bool is_password );
    CInput( CPanel *parent, int w, int h, long style );
    CInput( CPanel *parent, int x, int y, int w, int h, bool is_password, long style );
    ~CInput();
    virtual void Draw( wxDC *dc );
    virtual void Draw();
    void SetNotVisible();
    void SetVisible();
    void SetPositionSizeDefault( void );    
    void SetFontSize( int size );
    void SetText( const wxString &str );
    void SetText( const wxString &str, long pos );
    void AddText( const wxString &str, long pos );
    int Getch();
    char *GetValue();
    int id;
    void Clear();
    void setFont( const string &f );
    void setColour( const string &f, int color = 1 );
    void setBackgroundColour( const string &f );
    static void setKey( int key );
    static void removeKey();
    static int sizeKey();
    int GetTextSize();
    void __setPosition( const string &f );
    void __setPosition( int pos_x, int pos_y, int pos_w, int pos_h );
    void __setYPosition( int new_y );
  private:
    int x;
    int y;
    int w;
    int h;
    static list<int> buffer;
    char buffer_str[1024];
    wxFont font;   
    CPanel *parent;
    #ifdef COMPILAR_UML
    DECLARE_CLASS( CInput )
    DECLARE_EVENT_TABLE()
#endif
};
#endif

#pragma pack (pop) 
