#pragma pack (push,8) 
#ifndef __CLABEL_H
#define __CLABEL_H

#include "CPanel.h"
#include "CObject.h"
#include "GuiProperties.h"

#define _NORMAL_NORMAL 0
#define _NORMAL_BOLD   1
#define _ITALIC_NORMAL 2
#define _ITALIC_BOLD   3
#define _NORMAL_LIGHT  4
#define _ITALIC_LIGHT  5
#define _SLANT_NORMAL  6
#define _SLANT_LIGHT   7
#define _SLANT_BOLD    8

#define  ALIGN_NONE 0
#define  ALIGN_LEFT 1
#define  ALIGN_CENTER 2
#define  ALIGN_RIGHT 3

class CLabel : public CObject
{
  public:
    CLabel();
    CLabel( CPanel *parent );
    CLabel( CPanel *parent, int x, int y );
    ~CLabel();
    virtual void Draw( wxDC *dc );
    virtual void Draw();
    void setParent( CPanel *_parent );
    void setText( const string &text );
    void setTextEnLista( const string &text, int type )
    {
        setText( text );
    };
    void setBitmap( const wxString &filename, bool usedefault = true );
    void setNotVisible();
    void setVisible( bool visible );
    void setPosition( int x, int y );
    void __setPosition( const string &f );
    void setFontSize( int size );
    void setStyle( int style );
    void setWeight( int weight );
    //void setPropFlagComprob(int type);
    void setType( int type );
    void setColour( const unsigned char red, const unsigned char green, const unsigned char blue );
    int getFontSize();
    int getTextWidth();
    void setIcono();
    void setFont( const string &f );
    void setColour( const string &f );
    void setTamanioTotal( );
    int  getTamanioTotal( );


    bool isVisible()
    {
        return visible;
    };
    const wxFont & getFont()
    {
        return font;
    };
    void setAlign( const string &align );

    void setAtribute( int x, int y, string font, string color, string align );

    string getColor()
    {
        return color_str;
    }

    int x;
    int y;
    int w;
    int h;
	int x_ant;
	int y_ant;

  private:
    int type;
    int band;
    bool is_texto;
    bool visible;
    string color_str;
    wxString texto;
    wxString texto_old,defaultimage;
    wxBitmap bmp;
    wxMemoryDC memDC;
    wxColour color;
    int align_h;
    int align_v;
    wxFont font;
    CPanel *parent;
    int alignment( int align, int pos, int width );
    int str2align( const string &align );
    int    tamanioTotal;
};
#endif

#pragma pack (pop) 
