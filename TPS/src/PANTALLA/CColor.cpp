#include "CScreencfg.h"


CColor::CColor( DOMNode *node )
{
    DOMNamedNodeMap *attr = node->getAttributes();

    background = toString(
    ( XMLCh * )( attr->getNamedItem( toXMLCh( "background" ) ) )->getNodeValue() ).c_str();
    border = "000000";//toString((XMLCh*)(attr->getNamedItem(toXMLCh("border")))->getNodeValue()).c_str();
    text = toString(
    ( XMLCh * )( attr->getNamedItem( toXMLCh( "text" ) ) )->getNodeValue() ).c_str();
}

string CColor::GetBackground()
{
    return hexColorToDecColor( background );
}

string CColor::GetBorder()
{
    return hexColorToDecColor( border );
}

string CColor::GetText()
{
    return hexColorToDecColor( text );
}

int CColor::hexCharToInt( char c )
{
    if( ( c >= '0' ) && ( c <= '9' ) ) {
        return ( c - '0' );
    }
    else if( ( c >= 'A' ) && ( c <= 'F' ) ) {
        return c - 'A' + 10;
    }
    return 0;
}

string CColor::hexColorToDecColor( string color )
{
    int red,green,blue;
    char r[5],g[5],b[5];
    string color1;

    red = hexCharToInt( color[0] ) * 16 + hexCharToInt( color[1] );
    green = hexCharToInt( color[2] ) * 16 + hexCharToInt( color[3] );
    blue = hexCharToInt( color[4] ) * 16 + hexCharToInt( color[5] );

    #ifdef INVEL_L
    sprintf( r, "%d", red );
    sprintf( g, "%d", green );
    sprintf( b, "%d", blue );
    #else
    _itoa( red, r, 10 );
    _itoa( green, g, 10 );
    _itoa( blue, b, 10 );
    #endif

    color1 = string( r ) + "-" + string( g ) + "-" + string( b );


    return color1;
}



