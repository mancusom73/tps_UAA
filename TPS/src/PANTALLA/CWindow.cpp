#include "CScreencfg.h"

CWindow::CWindow( DOMNodeList *list1 )
{
    window1 = list1;
}


CSize * CWindow::GetSize()
{
    CSize *size;
    for( int i = 0;i< (int)window1->getLength();i++ ) {
        DOMNode *subnodes = window1->item( i );

        if( toString( ( XMLCh * )subnodes->getNodeName() ) == "size" ) {
            size = new CSize( subnodes );   
            return size;
        }
    }
    return NULL;
}

CImageRef * CWindow::GetImageRef()
{
    CImageRef *imageref;

    for( int i = 0;i< (int)window1->getLength();i++ ) {
        DOMNode *subnodes = window1->item( i );

        if( toString( ( XMLCh * )subnodes->getNodeName() ) == "imageRef" ) {
            imageref = new CImageRef( subnodes );   
            return imageref;
        }
    }
    return NULL;
}

CComponents * CWindow::GetComponents()
{
    CComponents *components;
    for( int i = 0;i< (int)window1->getLength();i++ ) {
        DOMNode *subnodes = window1->item( i );
        if( toString( ( XMLCh * )subnodes->getNodeName() ) == "components" ) {
            components = new CComponents( subnodes->getChildNodes() );
            return components;
        }
    }
    return NULL;
}
