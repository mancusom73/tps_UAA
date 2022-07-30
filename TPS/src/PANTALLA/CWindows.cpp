#include "CScreencfg.h"


CWindows::CWindows( DOMNodeList *list1 )
{
    window1 = list1;
}

CWindow * CWindows::GetWindowByName( string name )
{
    CWindow *window;

    for( int i = 0;i< (int)window1->getLength();i++ ) {
        DOMNode *subnodes = window1->item( i );

        DOMNamedNodeMap *attr = subnodes->getAttributes();
        XMLCh *name1 = ( XMLCh * )( attr->item( 0 ) )->getNodeValue();


        if( !toString( name1 ).compare( name ) ) {
            window = new CWindow( subnodes->getChildNodes() );
            return window;
        }
    }
    return NULL;
}


CWindow * CWindows::GetWindowAt( int index )
{
    CWindow *window;

    DOMNode *subnodes = window1->item( index );

    window = new CWindow( subnodes->getChildNodes() );
    return window;
}

