#pragma pack (push,8) 
#ifndef __CICON_H
#define __CICON_H

#include "CLabel.h"


class CIcon : public CObject
{
  public:
    CIcon();
    CIcon( CPanel *parent );
    CIcon( CPanel *parent, int x, int y );
    ~CIcon();
    virtual void Draw( wxDC *dc );
    virtual void Draw();
    void setParent( CPanel *_parent );
    void setText( const string &text1 );

    void setBitmap( const wxString &filename, bool usedefault = true );
    void setNotVisible();
    void setVisible( bool visible );

    bool isVisible();

    void setIconAtribute( int x, int y, const string &align, const string &filename );
    void setIconAtribute( int x, int y, const string &filename );
    void setTextAtribute( int x, int y, const string &font, const string &color,
                          const string &align );
    void setUseText( bool use );
    void setIconPosition( int x, int y );
    int getXPosition( void );
    int getYPosition( void );

    CLabel icon,text;
    string curText;

  private:
    int posx,posy;
    bool useText;
};

#endif


#pragma pack (pop) 
