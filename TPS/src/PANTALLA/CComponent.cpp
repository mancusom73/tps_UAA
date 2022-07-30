#include "CScreencfg.h"

int hexCharToInt( char c )
{
    if( ( c >= '0' ) && ( c <= '9' ) ) {
        return ( c - '0' );
    }
    else if( ( c >= 'A' ) && ( c <= 'F' ) ) {
        return c - 'A' + 10;
    }
    return 0;
}


//CTLabel

CTLabel::~CTLabel()
{
    delete font;
    delete size;
}

CTLabel::CTLabel( DOMNode *font1, DOMNode *size1, DOMNode *node )
{
    font = new CFont( font1 );
    size = new CSize( size1 );

    DOMNamedNodeMap *attr = node->getAttributes();

    string halign,valign;
    string posx,posy;
    string color1;
    char r[5],b[5],g[5];
    int blue,red,green;


    halign = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "align" ) ) )->getNodeValue() );
    valign = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "valign" ) ) )->getNodeValue() );
    if( valign == "middle" ) {
        valign == "center";
    }
    color1 = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "color" ) ) )->getNodeValue() );
    text = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "msgid" ) ) )->getNodeValue() );

    align = halign + "-" + valign;

    red = hexCharToInt( color1[0] ) * 16 + hexCharToInt( color1[1] );
    green = hexCharToInt( color1[2] ) * 16 + hexCharToInt( color1[3] );
    blue = hexCharToInt( color1[4] ) * 16 + hexCharToInt( color1[5] );

    #ifdef INVEL_L
    sprintf( r, "%d", red );
    sprintf( b, "%d", blue );
    sprintf( g, "%d", green );
    #else
    _itoa( red, r, 10 );
    _itoa( green, g, 10 );
    _itoa( blue, b, 10 );
    #endif

    color = string( r ) + "-" + string( g ) + "-" + string( b );
}

CFont * CTLabel::GetLabelFont()
{
    return font;
}

CSize * CTLabel::GetLabelSize()
{
    return size;
}

string CTLabel::GetLabelColor()
{
    return color;
}

string CTLabel::GetLabelAlign()
{
    return align;
}

string CTLabel::GetLabelText()
{
    return text;
}



//CTInput
CTInput::~CTInput()
{
    delete size;
    delete color;
    delete font;
}

CTInput::CTInput( DOMNode *font1, DOMNode *size1, DOMNode *color1, DOMNode *node )
{
    string s = toString( ( XMLCh * )node->getNodeName() );

    size = new CSize( size1 );
    color = new CColor( color1 );
    font = new CFont( font1 );

    string halign,valign;
    string fontname,fontstyle,fontsize;

    DOMNamedNodeMap *attr = node->getAttributes();
    DOMNamedNodeMap *attr1 = font1->getAttributes();

    halign = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "align" ) ) )->getNodeValue() );
    valign = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "valign" ) ) )->getNodeValue() );
    if( valign == "middle" ) {
        valign = "center";
    }

    align = halign + "-" + valign;
}

CFont * CTInput::GetInputFont()
{
    return font;
}

CSize * CTInput::GetInputSize()
{
    return size;
}

CColor * CTInput::GetInputColor()
{
    return color;
}

string CTInput::GetInputAlign()
{
    return align;
}

//CTIcon
CTIcon::~CTIcon()
{
    delete font;
    delete size;
    delete imageRef;
}

CTIcon::CTIcon( DOMNode *font1, DOMNode *size1, DOMNode *imageRef1, DOMNode *node )
{
    font = new CFont( font1 );
    size = new CSize( size1 );
    imageRef = new CImageRef( imageRef1 );

    DOMNamedNodeMap *attr = node->getAttributes();

    string halign,valign;
    string color1,posx,posy;
    char r[5],b[5],g[5];
    int blue,red,green;

    halign = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "align" ) ) )->getNodeValue() );
    valign = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "valign" ) ) )->getNodeValue() );
    if( valign == "middle" ) {
        valign == "center";
    }
    color1 = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "color" ) ) )->getNodeValue() );
    text = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "msgid" ) ) )->getNodeValue() );

    align = halign + "-" + valign;

    red = hexCharToInt( color1[0] ) * 16 + hexCharToInt( color1[1] );
    green = hexCharToInt( color1[2] ) * 16 + hexCharToInt( color1[3] );
    blue = hexCharToInt( color1[4] ) * 16 + hexCharToInt( color1[5] );

    #ifdef INVEL_L
    sprintf( r, "%d", red );
    sprintf( b, "%d", blue );
    sprintf( g, "%d", green );
    #else
    _itoa( red, r, 10 );
    _itoa( green, g, 10 );
    _itoa( blue, b, 10 );
    #endif

    color = string( r ) + "-" + string( g ) + "-" + string( b );

    posx = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "posx" ) ) )->getNodeValue() );
    posy = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "posy" ) ) )->getNodeValue() );

    x = atoi( posx.c_str() );
    y = atoi( posy.c_str() );
}

CFont * CTIcon::GetIconFont()
{
    return font;
}

CSize * CTIcon::GetIconSize()
{
    return size;
}

CImageRef * CTIcon::GetIconImageRef()
{
    return imageRef;
}

string CTIcon::GetIconText()
{
    return text;
}

string CTIcon::GetIconColor()
{
    return color;
}

string CTIcon::GetIconAlign()
{
    return align;
}  


int CTIcon::GetIconPosx()
{
    return x;
}

int CTIcon::GetIconPosy()
{
    return y;
}

//CTFlag
CTFlag::~CTFlag()
{
    delete size;
    delete states;
}

CTFlag::CTFlag( DOMNode *size1, DOMNodeList *states1, DOMNode *node )
{
    size = new CSize( size1 );
    states = new CStates( states1 );

    DOMNamedNodeMap *attr = node->getAttributes();

    show = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "show" ) ) )->getNodeValue() );

    string s = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "state" ) ) )->getNodeValue() );

    state = atoi( s.c_str() );
}

CSize * CTFlag::GetFlagSize()
{
    return size;
}

CStates * CTFlag::GetFlagStates()
{
    return states;
}

int CTFlag::GetFlagCurrentState()
{
    return state;
}

string CTFlag::GetFlagShow()
{
    return show;
}

//Grid Component
CTGrid::CTGrid( DOMNode *size1, DOMNodeList *list1, DOMNode *node )
{
    string row,col,name;
    int i;

    size = new CSize( size1 );

    DOMNamedNodeMap *attr = node->getAttributes();
    row = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "rowsize" ) ) )->getNodeValue() );

    rowSize = atoi( row.c_str() );

    for( i = 0;i< (int)list1->getLength();i++ ) {
        name = toString( ( XMLCh * )( list1->item( i ) )->getNodeName() );
        if( name == "column" ) {
            columns.push_back( new CColumn( list1->item( i ) ) );
        }
    }
}

CSize * CTGrid::GetGridSize()
{
    return size;
}

int CTGrid::GetGridRowSize()
{
    return rowSize;
}

CColumn * CTGrid::GetGridColumnAt( int i )
{
    return columns[i - 1];
}

int CTGrid::GetGridColumnCount()
{
    return columns.size();
}
