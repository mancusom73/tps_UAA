#pragma pack (push,8) 
#ifndef __CFLAG_H
#define __CFLAG_H

#include "CIcon.h"
#include "CGrid.h"

typedef struct
{
    int x,y;
    int textx,texty;
    string align,color,font;
    string filename;
} state;

class CFlag : public CObject
{
  private:
    CIcon icon;
    CGrid *grid;
    string show,align,currText,currIconText;
    vector<state> states;
    int currState,x,y;


  public:

    CFlag();
    ~CFlag();

    void setIconAtribute( int x, int y, const string &align, const string &filename );
    void setIconAtribute( int x, int y, const string &filename );
    void setIconUseText( bool use );

    void setTextAtribute( int x, int y, const string &text1, const string &font,
                          const string &color, const string &align );

    virtual void Draw( wxDC *dc );
    virtual void Draw();

    void setParent( CPanel *_parent );   

    void CFlag::addState( int x, int y, int textx, int texty, string align, string color,
                          string font, string filename );

    void setVisible( bool visible );

    void setShowType( string s );

    bool isVisible();

    void changeCurrentState( int curr );

    void setText( const string &text1 );
    void setIconText( const string &text1 );

    void setPosition( int x1, int y1 );

    void setGrid( CGrid *grid1 );

    void setAlign( string align );
};

#endif


#pragma pack (pop) 
