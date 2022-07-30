#include "CScreencfg.h"

CColumn::CColumn( DOMNode *column )
{
    DOMNodeList *childs;
    string name;

    DOMNamedNodeMap *attr = column->getAttributes();

    width = atoi(
    toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "width" ) ) )->getNodeValue() ).c_str() );
    align = toString(
    ( XMLCh * )( attr->getNamedItem( toXMLCh( "align" ) ) )->getNodeValue() ).c_str();

    childs = column->getChildNodes();
    for( int i = 0;i< (int)childs->getLength();i++ ) {
        name = toString( ( XMLCh * )( childs->item( i ) )->getNodeName() );
        if( name == "font" ) {
            font = new CFont( childs->item( i ) );
        }
        if( name == "colors" ) {
            color = new CColor( childs->item( i ) );
        }
        if( name == "selected" ) {
            selected = new CColor( childs->item( i ) );
        }
    }
}

CFont * CColumn::GetFont()
{
    return font;
}

CColor * CColumn::GetColor()
{
    return color;
}

CColor * CColumn::GetSelectedColor()
{
    return selected;
}

string CColumn::GetAlign()
{
    return align;
}

int CColumn::GetWidth()
{
    return width;
}

