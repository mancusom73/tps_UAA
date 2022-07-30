#include "CScreencfg.h"

CImage::CImage( DOMNode *node )
{
    DOMNamedNodeMap *attr = node->getAttributes();
    filename = toString( ( XMLCh * )( attr->item( 0 ) )->getNodeValue() );
}

string CImage::GetFilename()
{
    return filename;
}

