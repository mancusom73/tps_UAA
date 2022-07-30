//#include "cr.h"
#pragma pack(8)

#include "CInput.h"
#include "MainThread.h"
#include "CFontMapper.h"
#include <stdlib.h>
#include <stdio.h>
#include "config.h"

#define ID_CINPUT 10000

#ifdef COMPILAR_UML
IMPLEMENT_CLASS( CInput, wxTextCtrl )
BEGIN_EVENT_TABLE( CInput, wxTextCtrl )
//EVT_KEY_DOWN(CInput::OnKey)
//EVT_CHAR(CInput::OnKey)
//EVT_TEXT_ENTER(ID_CINPUT,CInput::OnText)
END_EVENT_TABLE()
#endif

list<int> CInput::buffer;

CInput::CInput( CPanel *_parent ) : wxTextCtrl( _parent, ID_CINPUT, wxString( "Prueba" ),
                                                wxPoint( 100, 100 ), wxSize( 100, 50 ),
                                                wxNO_BORDER )
{
    parent = _parent;
    wxFont f = wxFont( 12, wxMODERN, wxNORMAL, wxBOLD );
    SetFont( f );
}


CInput::CInput( CPanel *_parent, int _x, int _y, int _w, int _h, bool is_password ) 
: wxTextCtrl( _parent, ID_CINPUT, wxString( "" ), wxPoint( _x, _y ), wxSize( _w, _h ),
              ( is_password ) ? /*wxTE_PASSWORD |*/ wxNO_BORDER : wxNO_BORDER )
{
    parent = _parent;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    wxFont f = wxFont( 12, wxMODERN, wxNORMAL, wxBOLD );
    SetFont( f );
}


CInput::CInput( CPanel *_parent, int _w, int _h, long style ) : wxTextCtrl( _parent, ID_CINPUT,
                                                                            wxString( "" ),
                                                                            wxPoint( 0, 0 ),
                                                                            wxSize( _w, _h ),
                                                                            style )
{
    parent = _parent;
    x = 0;
    y = 0;
    w = _w;
    h = _h;
    wxFont f = wxFont( 12, wxMODERN, wxNORMAL, wxBOLD );
    SetFont( f );
}


CInput::CInput( CPanel *_parent, int _x, int _y, int _w, int _h, bool is_password, long style ) 
: wxTextCtrl( _parent, ID_CINPUT, wxString( "" ), wxPoint( _x, _y ), wxSize( _w, _h ), style )
{
    parent = _parent;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    wxFont f = wxFont( 12, wxMODERN, wxNORMAL, wxBOLD );
    SetFont( f );
}


CInput::~CInput()
{
}


void CInput::Draw()
{
}


void CInput::Draw( wxDC *dc )
{
}


void CInput::SetFontSize( int size )
{
    wxFont f = GetFont();
    f.SetPointSize( size );
    SetFont( f );
}


void CInput::SetVisible()
{
    Show();
}


void CInput::SetNotVisible()
{
    Show( FALSE );
}


char * CInput::GetValue()
{
    wxString str = wxTextCtrl::GetValue();
    strcpy( buffer_str, str );
    return buffer_str;
}


int CInput::Getch()
{
    int rta = -999;

    if( CInput::buffer.size() > 0 ) {
        rta = CInput::buffer.front();
        CInput::buffer.pop_front();
    }
    return rta;
}


void CInput::SetText( const wxString &str )
{
    SetValue( str );
}


void CInput::SetText( const wxString &str, long pos )
{
    SetValue( str );
    SetInsertionPoint( pos );
}


void CInput::AddText( const wxString &str, long pos )
{
    AppendText( str );
    SetInsertionPoint( pos );
}

void CInput::Clear()
{
    SetValue( "" );
    CInput::buffer.clear();
}


void CInput::setFont( const string &f )
{
    char f_name[100];
    char f_size[100];
    char f_style[100];
    int l_name = 0;
    int l_size = 0;
    int l_style = 0;
    int j = 0;

    memset( f_name, 0, sizeof( f_name ) );
    memset( f_size, 0, sizeof( f_size ) );
    memset( f_style, 0, sizeof( f_style ) );

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' ) {
            f_name[l_name++] = f[i];
        }
        if( j == 1 && f[i] != '-' ) {
            f_style[l_style++] = f[i];
        }
        if( j == 2 && f[i] != '-' ) {
            f_size[l_size++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_name[l_name] = 0x00;
    f_size[l_size] = 0x00;
    f_style[l_style] = 0x00;

    #ifndef INVEL_L
    font.SetFaceName( fontEnumerator.GetFacename( f_name ) );
    #endif
    font.SetPointSize( atoi( f_size ) );

    //;# Fuente para el texto. name,style,size.
    //;# The style can be
    //;# NORMAL+NORMAL=0,NORMAL+BOLD=1,ITALIC+NORMAL=2,ITALIC+BOLD=3,NORMAL+LIGHT=4,ITALIC+LIGHT=5,
    //;# SLANT+NORMAL=6,SLANT+LIGHT=7,SLANT+BOLD=8
    //;# (The style  can be wxNORMAL=90, wxSLANT=94 and wxITALIC=93)
    //;# (The weight can be wxNORMAL=90, wxLIGHT=91 and wxBOLD=92.)
    switch( atoi( f_style ) ) {
        case _NORMAL_NORMAL:
            font.SetStyle( wxNORMAL );
            font.SetWeight( wxNORMAL );
            break;
        case _NORMAL_BOLD:
            font.SetStyle( wxNORMAL );
            font.SetWeight( wxBOLD );
            break;
        case _ITALIC_NORMAL:
            font.SetStyle( wxITALIC );
            font.SetWeight( wxNORMAL );
            break;
        case _ITALIC_BOLD:
            font.SetStyle( wxITALIC );
            font.SetWeight( wxBOLD );
            break;
        case _NORMAL_LIGHT:
            font.SetStyle( wxNORMAL );
            font.SetWeight( wxLIGHT );
            break;
        case _ITALIC_LIGHT:
            font.SetStyle( wxITALIC );
            font.SetWeight( wxLIGHT );
            break;
        case _SLANT_NORMAL:
            font.SetStyle( wxSLANT );
            font.SetWeight( wxNORMAL );
            break;
        case _SLANT_LIGHT:
            font.SetStyle( wxSLANT );
            font.SetWeight( wxLIGHT );
            break;
        case _SLANT_BOLD:
            font.SetStyle( wxSLANT );
            font.SetWeight( wxBOLD );
            break;
    }

    SetFont( font );
}

void CInput::setBackgroundColour( const string &f )
{
    setColour( f, 0 );
}

void CInput::setColour( const string &f, int color )
{
    char f_red[100];
    char f_blue[100];
    char f_green[100];
    int l_red = 0;
    int l_blue = 0;
    int l_green = 0;
    int j = 0;

    memset( f_red, 0, sizeof( f_red ) );
    memset( f_blue, 0, sizeof( f_blue ) );
    memset( f_green, 0, sizeof( f_green ) );

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' ) {
            f_red[l_red++] = f[i];
        }
        if( j == 1 && f[i] != '-' ) {
            f_blue[l_blue++] = f[i];
        }
        if( j == 2 && f[i] != '-' ) {
            f_green[l_green++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_red[l_red] = 0x00;
    f_blue[l_blue] = 0x00;
    f_green[l_green] = 0x00;
    wxColour c( atoi( f_red ), atoi( f_blue ), atoi( f_green ) );
    if( color ) {
        SetForegroundColour( c );
    }
    else {
        SetBackgroundColour( c );
    }
    Refresh( true );
}


void CInput::setKey( int key )
{
    CInput::buffer.push_back( key );
}


void CInput::removeKey()
{
    CInput::buffer.pop_front();
}


int CInput::sizeKey()
{
    return CInput::buffer.size();
}


int CInput::GetTextSize()
{
    wxString str = GetLineText( 0 );
    int len = 0;
    for( unsigned int i = 0;i < str.length();i++ ) {
        if( ( str[i] >= ( ( int )'0' ) ) && ( str[i] <= ( ( int )'9' ) ) ) {
            len++;
        }
    }
    return len;
}

void CInput::__setYPosition( int new_y )
{
    Move( x, new_y );
}

void CInput::__setPosition( const string &f )
{
    char f_x[10],f_y[10],f_w[10],f_h[10];
    int l_x = 0, l_y = 0, l_w = 0, l_h = 0;
    int j = 0;
    int pos_x = 0, pos_y = 0, pos_w = 0, pos_h = 0;

    memset( f_x, 0, 10 );
    memset( f_y, 0, 10 );
    memset( f_w, 0, 10 );
    memset( f_h, 0, 10 );

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_x[l_x++] = f[i];
        }
        if( j == 1 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_y[l_y++] = f[i];
        }
        if( j == 2 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_w[l_w++] = f[i];
        }
        if( j == 3 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_h[l_h++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_x[l_x] = 0;
    f_y[l_y] = 0;
    f_w[l_w] = 0;
    f_h[l_h] = 0;

    pos_x = atoi( f_x );
    pos_y = atoi( f_y );
    pos_w = atoi( f_w );
    pos_h = atoi( f_h );
    SetSize( pos_x, pos_y, pos_w, pos_h );
}


void CInput::__setPosition( int pos_x, int pos_y, int pos_w, int pos_h )
{
    SetSize( pos_x, pos_y, pos_w, pos_h );
}

void CInput::SetPositionSizeDefault( void )
{
    SetSize( x, y, w, h );
}
