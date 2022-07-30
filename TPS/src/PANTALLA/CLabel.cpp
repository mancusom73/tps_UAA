#include "CLabel.h"
#include "GuiProperties.h"
#include "typeacti.h"
#include "CFontMapper.h"

static wxMutex s_mutexLabel;

void CLabel::setParent( CPanel *_parent )
{
    parent = _parent;
    is_texto = true;
    texto = " ";
    texto_old = " ";
    visible = false;
    align_h = ALIGN_NONE;
    align_v = ALIGN_NONE;
    font = wxFont( 12, wxROMAN, wxNORMAL, wxNORMAL );
    color = *wxBLACK;
}

CLabel::CLabel()
{
    color_str = "";
    texto = "";
    texto_old = "";
    defaultimage = "";

    x = 0;
    y = 0;
    w = 0;
    h = 0;
	x_ant = 0;
	y_ant = 0;
}

CLabel::CLabel( CPanel *_parent )
{
    setParent( _parent );
}


CLabel::CLabel( CPanel *_parent, int _x, int _y )
{
    parent = _parent;
    is_texto = true;
    texto = "";
    texto_old = "";
    visible = false;
    x = _x;
    y = _y;
    align_h = ALIGN_NONE;
    align_v = ALIGN_NONE;
    font = wxFont( 12, wxROMAN, wxNORMAL, wxNORMAL );
}


CLabel::~CLabel()
{
}


void CLabel::Draw()
{
    wxClientDC dc( parent );
    Draw( &dc );
}


void CLabel::setAlign( const string &f )
{
    char f_h[100];
    char f_v[100];
    int l_h = 0;
    int l_v = 0;

    int j = 0;

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' ) {
            f_h[l_h++] = f[i];
        }
        if( j == 1 && f[i] != '-' ) {
            f_v[l_v++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_h[l_h] = 0x00;
    f_v[l_v] = 0x00;

    align_h = str2align( f_h );
    align_v = str2align( f_v );
}


int CLabel::str2align( const string &align )
{
    if( align == "left" ) {
        return ALIGN_LEFT;
    }
    if( align == "center" ) {
        return ALIGN_CENTER;
    }
    if( align == "right" ) {
        return ALIGN_RIGHT;
    }
    if( align == "top" ) {
        return ALIGN_LEFT;
    }
    if( align == "bottom" ) {
        return ALIGN_RIGHT;
    }

    return ALIGN_NONE;
}


int CLabel::alignment( int align, int pos, int width )
{
    switch( align ) {
        case ALIGN_NONE:
        case ALIGN_LEFT:
            return pos;
        case ALIGN_RIGHT:
            return pos - width;
        case ALIGN_CENTER:
            return pos - width / 2;
    }
    return pos;
}


void CLabel::Draw( wxDC *dc )
{
    int s_w;
    int s_h;
    int w1,w2,h1,h2;
	wxMutexLocker lock(s_mutexLabel);
	int len1 = strlen( texto_old );

    dc->SetTextForeground( color );
    if( visible ) {
        if( is_texto ) {
            dc->SetFont( font );
            dc->GetTextExtent( texto, &w1, &h1, NULL, NULL, &font );
            dc->GetTextExtent( texto_old, &w2, &h2, NULL, NULL, &font );
            if( w1 > w2 ) {
                w = w1;
            }
            else {
                w = w2;
            }
            if( h1 > h2 ) {
                h = h1;
            }
            else {
                h = h2;
            }
            s_h = h;
            s_w = w;
            memDC.SelectObject( *parent->GetBackGround() );
            dc->Blit( alignment( align_h, x, w ), alignment( align_v, y, h ), w, h, &memDC,
                      alignment( align_h, x, w ), alignment( align_v, y, h ), wxCOPY );
            dc->SetTextForeground( color );
            memDC.SelectObject( wxNullBitmap );
            w = w1;
            h = h1;
            s_h = h;
            s_w = w;
            dc->DrawText( texto, alignment( align_h, x, w ), alignment( align_v, y, h ) );
        }
        else {
            memDC.SelectObject( bmp );
            w = bmp.GetWidth();
            h = bmp.GetHeight();
            dc->Blit( alignment( align_h, x, w ), alignment( align_v, y, h ), w, h, &memDC, 0, 0,
                      wxCOPY, true );
            memDC.SelectObject( wxNullBitmap );
        }
    }
    else {
        if( is_texto ) {
            dc->SetFont( font );
            dc->GetTextExtent( texto, &w1, &h, NULL, NULL, &font );
            dc->GetTextExtent( texto_old, &w2, &h, NULL, NULL, &font );
            if( w1 > w2 ) {
                w = w1;
            }
            else {
                w = w2;
            }
			memDC.SelectObject( *parent->GetBackGround() );
            dc->Blit( alignment( align_h, x, w ), alignment( align_v, y, h ), w, h, &memDC,
                      alignment( align_h, x, w ), alignment( align_v, y, h ), wxCOPY, true );
            memDC.SelectObject( wxNullBitmap );
        }
        else {
            memDC.SelectObject( *parent->GetBackGround() );
            dc->Blit( alignment( align_h, x, w ), alignment( align_v, y, h ), w, h, &memDC,
                      alignment( align_h, x, w ), alignment( align_v, y, h ), wxCOPY, true );
            memDC.SelectObject( wxNullBitmap );
        }
    }
}


void CLabel::setText( const string &text )
{
    if( is_texto ) {
        texto_old = texto.c_str();
        texto = text.c_str();
        is_texto = true;
        //      SetRefresh();
        w = 0;
        h = 0;
        if( text != " " && text != "" ) {
            visible = true;
            SetRefresh();
        }
        else {
            visible = false;
            SetRefresh( false );
        }
    }
    else {
        if( text != " " && text != "" ) {
            visible = true;
            SetRefresh();
        }
        else {
            visible = false;
            SetRefresh( false );
        }
    }
}


void CLabel::setBitmap( const wxString &filename, bool usedefault )
{
    /*#ifndef INVEL_L
        wxImage image;
        if(usedefault)
        {
            if(wxFileExists(filename))
                image.LoadFile( filename, wxBITMAP_TYPE_GIF );
            else
                image.LoadFile( ini_def.get("CLABEL","DEFAULT_IMAGE_FILE_NOT_FOUND").value.String, wxBITMAP_TYPE_GIF );            
        } else {
          image.LoadFile( filename, wxBITMAP_TYPE_GIF );
        }
        
        bmp = image.ConvertToBitmap();
        w = image.GetWidth();
        h = image.GetHeight();
        is_texto = false;
        visible = true;
        SetRefresh();
    #else*/
    if( usedefault ) {
        if( wxFileExists( filename ) ) {
            bmp = wxBitmap( filename, wxBITMAP_TYPE_GIF );
        }else {
			char  mensaje[100];
			memset(mensaje,0, sizeof(mensaje));
            bmp = wxBitmap( ini_def.get( "CLABEL", "DEFAULT_IMAGE_FILE_NOT_FOUND" ).value.String,
                           wxBITMAP_TYPE_GIF );
			_snprintf(mensaje,sizeof(mensaje),"No se encuentra IMAGEN; %s",filename);
			/* sino encuentra una imagen dejo indicado en el sistema log..ya que se debe solucionar si o si */
			sglog(mensaje);
        }
    }
    else {
        bmp = wxBitmap( filename, wxBITMAP_TYPE_GIF );
    }
    w = bmp.GetWidth();
    h = bmp.GetHeight();
    is_texto = false;
    visible = true;
    SetRefresh();
    //#endif
}

void CLabel::setVisible( bool v )
{
    visible = v;
    SetRefresh( v );
    Draw();
}


void CLabel::setNotVisible()
{
    SetRefresh( false );
    visible = false;
    Draw();
}


void CLabel::setPosition( int _x, int _y )
{
    x = _x;
    y = _y;
}


void CLabel::setFontSize( int size )
{
    font.SetPointSize( size );
}

void CLabel::setTamanioTotal( )
{
   tamanioTotal = getFontSize();
 }

int CLabel::getTamanioTotal( )
{
   return tamanioTotal;
 }


void CLabel::setStyle( int style )
{
    font.SetStyle( style );
}


void CLabel::setWeight( int weight )
{
    font.SetWeight( weight );
}


void CLabel::setType( int _type )
{
    type = _type;
}


void CLabel::setColour( const unsigned char red, const unsigned char green,
                        const unsigned char blue )
{
    wxColour c( red, green, blue );
    color = c;
}


int CLabel::getFontSize()
{
    return font.GetPointSize();
}


int CLabel::getTextWidth()
{
    int w,h;
    memDC.GetTextExtent( texto, &w, &h, NULL, NULL, &font );
    return w;
}


void CLabel::__setPosition( const string &f )
{
    char f_x[10],f_y[10],f_w[10],f_h[10];
    int l_x = 0, l_y = 0, l_w = 0, l_h = 0;
    int j = 0;
    int pos_x = 0, pos_y = 0, pos_w = 0, pos_h = 0;

    memset( f_x, 0, sizeof( f_x ) );
    memset( f_y, 0, sizeof( f_y ) );
    memset( f_w, 0, sizeof( f_w ) );
    memset( f_h, 0, sizeof( f_h ) );

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

    setPosition( pos_x, pos_y );
}


void CLabel::setIcono()
{
    is_texto = false;
    //visible = true;
}


void CLabel::setFont( const string &f )
{
    char f_name[100];
    char f_size[100];
    char f_style[100];
    int l_name = 0;
    int l_size = 0;
    int l_style = 0;
    int j = 0;

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
}


void CLabel::setColour( const string &f )
{
    char f_red[100];
    char f_blue[100];
    char f_green[100];
    int l_red = 0;
    int l_blue = 0;
    int l_green = 0;
    int j = 0;

    color_str = f;

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
    setColour( atoi( f_red ), atoi( f_blue ), atoi( f_green ) );
}

void CLabel::setAtribute( int x, int y, string font, string color, string align )
{
    setText( "" );
    setPosition( x, y );
    setFont( font );
    setColour( color );
    setAlign( align );
}
