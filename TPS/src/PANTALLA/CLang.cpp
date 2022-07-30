#include "CScreencfg.h"

CLang::CLang( DOMNodeList *list1 )
{
    lang = list1;
}


CMessage * CLang::GetMessageByName( string name )
{
    CMessage *mgs;

    int j = lang->getLength();
    for( int i = 0;i< (int)lang->getLength();i++ ) {
        DOMNode *subnodes = lang->item( i );

        string s = toString( ( XMLCh * )subnodes->getNodeName() );

        if( toString( ( XMLCh * )subnodes->getNodeName() ) == "message" ) {
            DOMNamedNodeMap *attr = subnodes->getAttributes();
            XMLCh *name1 = ( XMLCh * )( attr->getNamedItem( toXMLCh( "name" ) ) )->getNodeValue();
            if( toString( ( XMLCh * )name1 ) == name ) {
                mgs = new CMessage( subnodes->getChildNodes() );   
                return mgs;
            }
        }
    }
    return NULL;
}

CMessage * CLang::GetMessageAt( int index )
{
    return NULL;
}
