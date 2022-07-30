#include "CFlag.h"


CFlag::CFlag()
{
    SetRefresh();
}

CFlag::~CFlag()
{
}

void CFlag::Draw( wxDC *dc )
{
	if(icon.isVisible())
		icon.Draw( dc );
}

void CFlag::Draw()
{
	if(icon.isVisible())
		icon.Draw();
}

void CFlag::setParent( CPanel *_parent )
{
    icon.setParent( _parent );
}

void CFlag::addState( int x, int y, int textx, int texty, string align, string color, string font,
                      string filename )
{
    state s;
    s.x = x;
    s.y = y;
    s.textx = textx;
    s.texty = texty;
    s.align = align;
    s.color = color;
    s.font = font;
    s.filename = filename;
    states.push_back( s );
}


void CFlag::setVisible( bool visible )
{
    vector<string> aux;
    aux.push_back( currText );

    if( visible ) {
        if( ( ( show == "WD" ) || ( show == "D" ) ) && grid != NULL ) {
            for( int i = 0;i< grid->GetNumberRows();i++ ) {
                if( ( grid->GetCellValue( i, 0 ) ).c_str() == currText ) {
                    grid->DeleteRows( i, 1 );
                }
            }
            if( currText.length() > 0 ) {
                grid->AddRow( aux );
            }
        }
        if( ( show == "WD" ) || ( show == "W" ) ) {
            icon.setVisible( true );
        }
    }
    else {
        if( grid != NULL ) {
            for( int i = 0;i< grid->GetNumberRows();i++ ) {
                if( ( grid->GetCellValue( i, 0 ) ).c_str() == currText ) {
                    grid->DeleteRows( i, 1 );
                }
            }
        }
        icon.setVisible( false );
    }
}


bool CFlag::isVisible()
{
    return icon.isVisible();
}

void CFlag::setIconAtribute( int x, int y, const string &align, const string &filename )
{
    icon.setIconAtribute( x, y, align, filename );
}

void CFlag::setIconAtribute( int x1, int y1, const string &filename )
{
    icon.setIconAtribute( x1, y1, filename );
}

void CFlag::setTextAtribute( int x1, int y1, const string &text1, const string &font,
                             const string &color, const string &align )
{
    icon.setTextAtribute( x1, y1, font, color, align );
}

void CFlag::setIconUseText( bool use )
{
    icon.setUseText( use );
}

void CFlag::setText( const string &text1 )
{
    currText = text1;
}

void CFlag::setIconText( const string &text1 )
{
    currIconText = text1;
    icon.setText( text1 );
}
void CFlag::setShowType( string s )
{
    show = s;
}

void CFlag::changeCurrentState( int curr )
{
    currState = curr;
    state s = states[currState];
    icon.setIconAtribute( s.x, s.y, s.align, s.filename );
    icon.setTextAtribute( s.textx, s.texty, s.font, s.color, s.align ); 
    icon.setText( currIconText );
}

void CFlag::setPosition( int x1, int y1 )
{
    x = x1;
    y = y1;
}


void CFlag::setGrid( CGrid *grid1 )
{
    grid = grid1;
}

void CFlag::setAlign( string align1 )
{
    align = align1;
}
