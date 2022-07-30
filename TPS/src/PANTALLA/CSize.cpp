#include "CScreencfg.h"


CSize::CSize( DOMNode *node )
{
    DOMNamedNodeMap *attr = node->getAttributes();

    width = atoi(
    toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "width" ) ) )->getNodeValue() ).c_str() );
    height = atoi(
    toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "height" ) ) )->getNodeValue() ).c_str() );
}

int CSize::GetWidth()
{
    return width;
}

int CSize::GetHeight()
{
    return height;
}
