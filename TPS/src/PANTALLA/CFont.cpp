#include "CScreencfg.h"

CFont::CFont( DOMNode *node )
{
    char aux[20];
    int size1;

    DOMNamedNodeMap *attr1 = node->getAttributes();

    name = toString( ( XMLCh * )( attr1->getNamedItem( toXMLCh( "type" ) ) )->getNodeValue() );
    style = toString( ( XMLCh * )( attr1->getNamedItem( toXMLCh( "style" ) ) )->getNodeValue() );
    size = toString(
    ( XMLCh * )( attr1->getNamedItem( toXMLCh( "size" ) ) )->getNodeValue() ).c_str();
    sscanf( size.c_str(), "%d", &size1 );

    #ifdef INVEL_L
    sprintf( aux, "%d", size1 );
    #else
    _snprintf(aux, sizeof(aux)-1, "%d", ( int )( ( float )size1 / 1.33 ) );
    #endif

    size = string( aux );
}

string CFont::GetFont()
{
    return name;
}

string CFont::GetStyle()
{
    return style;
}

int CFont::GetSize()
{
    return atoi( size.c_str() );
}

string CFont::ToString()
{
    string s;

    if( style == "B" ) {
        s = "1";
    }
    else {
        s = "0";
    }

    return name + "-" + s + "-" + size;
}

