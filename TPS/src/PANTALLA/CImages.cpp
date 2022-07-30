#include "CScreencfg.h"

CImages::CImages( DOMNodeList *list1 )
{
    images = list1;
}

CSkin * CImages::GetSkinByName( string name )
{
    CSkin *skin;

    for( int i = 0;i< (int)images->getLength();i++ ) {
        DOMNode *subnodes = images->item( i );

        string s = toString( ( XMLCh * )subnodes->getNodeName() );
        DOMNamedNodeMap *attr = subnodes->getAttributes();
        XMLCh *name1 = ( XMLCh * )( attr->item( 0 ) )->getNodeValue();

        if( !toString( name1 ).compare( name ) ) {
            skin = new CSkin( subnodes->getChildNodes() );
            return skin;
        }
    }
    return NULL;
}


CSkin * CImages::GetSkinAt( int index )
{
    CSkin *skin;

    DOMNode *subnodes = images->item( index );

    skin = new CSkin( subnodes->getChildNodes() );
    return skin;
}
