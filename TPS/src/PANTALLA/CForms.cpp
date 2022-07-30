#include "CScreencfg.h"

CForms::CForms( DOMNodeList *list2 )
{
    list1 = list2;
}

CForm * CForms::GetFormByName( string name )
{
    string s;

    for( int i = 0;i< (int)list1->getLength();i++ ) {
        DOMNode *subnodes = list1->item( i );
        s = toString( ( XMLCh * )subnodes->getNodeName() );
        if( ( s == "form" ) ) {
            DOMNamedNodeMap *attr = subnodes->getAttributes();
            string name1 = toString(
                ( XMLCh * )( attr->getNamedItem( toXMLCh( "name" ) ) )->getNodeValue() );
            if( name1 == name ) {
                return new CForm( subnodes->getChildNodes() );
            }
        }
    }
    return NULL;
}  
