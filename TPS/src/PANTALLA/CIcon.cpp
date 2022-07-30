#include "CIcon.h"

CIcon::CIcon()
{
    useText = true;
    SetRefresh();
}

CIcon::CIcon( CPanel *parent )
{
    icon.setParent( parent );
    text.setParent( parent );
    useText = true;
    SetRefresh();
}

CIcon::CIcon( CPanel *parent, int x, int y )
{
    icon.setParent( parent );
    text.setParent( parent );
    useText = true;
    SetRefresh();
}

CIcon::~CIcon()
{
}

void CIcon::Draw( wxDC *dc )
{
    icon.Draw( dc );
    if( ( curText != "" ) && useText ) {
        text.Draw( dc );
    }
}

void CIcon::Draw()
{
    icon.Draw();
    if( ( curText != "" ) && useText ) {
        text.Draw();
    }
}

void CIcon::setParent( CPanel *_parent )
{
    icon.setParent( _parent );
    text.setParent( _parent );
}

void CIcon::setText( const string &text1 )
{
    curText = text1;
    text.setText( text1 );
}

void CIcon::setBitmap( const wxString &filename, bool usedefault )
{
    icon.setBitmap( filename, usedefault );
}

void CIcon::setNotVisible()
{
    icon.setNotVisible();
    if( ( curText != "" ) && useText ) {
        text.setNotVisible();
    }
}

void CIcon::setVisible( bool visible )
{
    icon.setVisible( visible );
    if( ( curText != "" ) && useText ) {
        text.setVisible( visible );
    }
}

bool CIcon::isVisible()
{
    return ( icon.isVisible() );
}

void CIcon::setIconAtribute( int x, int y, const string &filename )
{
    icon.setPosition( x, y );
    icon.setBitmap( filename.c_str() );
    icon.setIcono();
    posx = x;
    posy = y;
}

void CIcon::setIconAtribute( int x, int y, const string &align, const string &filename )
{
    icon.setPosition( x, y );
    icon.setAlign( align );
    icon.setBitmap( filename.c_str() );
    icon.setIcono();
    posx = x;
    posy = y;
}

void CIcon::setTextAtribute( int x, int y, const string &font, const string &color,
                             const string &align )
{
    text.setAtribute( x, y, font, color, align );
}

void CIcon::setUseText( bool use )
{
    useText = use;
}

void CIcon::setIconPosition( int x, int y )
{
    icon.setPosition( x, y );
    posx = x;
    posy = y;
}

int CIcon::getXPosition( void )
{
    return posx;
}

int CIcon::getYPosition( void )
{
    return posy;
}
