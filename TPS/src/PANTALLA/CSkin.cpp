#include "CScreencfg.h"

CSkin::CSkin( DOMNodeList *list1 )
{
    skin = list1;
}


CImage * CSkin::GetImageByName( string name )
{
    CImage *image;

    int j = skin->getLength();
    for( int i = 0;i< (int)skin->getLength();i++ ) {
        DOMNode *subnodes = skin->item( i );

        string s = toString( ( XMLCh * )subnodes->getNodeName() );

        if( toString( ( XMLCh * )subnodes->getNodeName() ) == "image" ) {
            DOMNamedNodeMap *attr = subnodes->getAttributes();
            XMLCh *name1 = ( XMLCh * )( attr->item( 1 ) )->getNodeValue();
            if( toString( ( XMLCh * )name1 ) == name ) {
                image = new CImage( subnodes );   
                return image;
            }
        }
    }
    return NULL;
}

CImage * CSkin::GetImageAt( int index )
{
    CImage *image;

    DOMNode *subnodes = skin->item( index );

    image = new CImage( subnodes );   
    return image;
}

