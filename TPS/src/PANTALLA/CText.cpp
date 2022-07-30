#include "CScreencfg.h"

CText::CText( DOMNodeList *list2 )
{
    list1 = list2;
}

CLang * CText::GetLangByName( string name )
{
    CLang *lang;

    for( int i = 0;i< (int)list1->getLength();i++ ) {
        DOMNode *subnodes = list1->item( i );

        string s = toString( ( XMLCh * )subnodes->getNodeName() );
        DOMNamedNodeMap *attr = subnodes->getAttributes();
        XMLCh *name1 = ( XMLCh * )( attr->item( 0 ) )->getNodeValue();

        if( !toString( name1 ).compare( name ) ) {
            lang = new CLang( subnodes->getChildNodes() );
            return lang;
        }
    }  
    return NULL;
}
