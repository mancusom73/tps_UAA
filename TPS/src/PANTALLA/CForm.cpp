#include "CScreencfg.h"


CForm::CForm( DOMNodeList *list2 )
{
    string s;

    for( int i = 0;i< (int)list2->getLength();i++ ) {
        DOMNode *subnodes = list2->item( i );
        s = toString( ( XMLCh * )subnodes->getNodeName() );
        if( s == "size" ) {
            size = new CSize( subnodes );
        }
        if( s == "imageRef" ) {
            img = new CImageRef( subnodes );
        }
        if( s == "labelRef" ) {
            comp = new CLabelRef( subnodes );
        }
        if( s == "inputdesc" ) {
            input = new CInputDesc( subnodes->getChildNodes() );
        }
    }
}


CSize * CForm::GetSize()
{
    return size;
}

CImageRef * CForm::GetImageRef()
{
    return img;
}

CLabelRef * CForm::GetLabelRef()
{
    return comp;
}

CInputDesc * CForm::GetInputDesc()
{
    return input;
}
