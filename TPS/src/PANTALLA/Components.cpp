#pragma warning (disable:4243)
#include "CScreencfg.h"


CComponents::CComponents( DOMNodeList *components )
{
    components1 = components;
}

CComponentRef * CComponents::GetComponentRefByName( string name )
{
    CComponentRef *ref;

    for( int i = 0;i< (int)components1->getLength();i++ ) {
        DOMNode *subnodes = components1->item( i );

        string s = toString( ( XMLCh * )subnodes->getNodeName() );

        if( ( s == "componentRef" ) || ( s == "labelRef" ) ) {
            DOMNamedNodeMap *attr = subnodes->getAttributes();
            XMLCh *name1 = ( XMLCh * )( attr->item( 0 ) )->getNodeValue();
            XMLCh *type = ( XMLCh * )( attr->item( 3 ) )->getNodeValue();

            if( toString( name1 ) == name ) {
                if( ( toString( type ) == "TLabelRef" ) ) {
                    ref = ( CComponentRef * )( new CLabelRef( subnodes ) );
                }
                else if( ( toString( type ) == "TInputRef" ) ) {
                    ref = ( CComponentRef * )( new CInputRef( subnodes ) );
                }
                else if( ( toString( type ) == "TIconRef" ) ) {
                    ref = ( CComponentRef * )( new CInputRef( subnodes ) );
                }
                else if( ( toString( type ) == "TFlagRef" ) ) {
                    ref = ( CComponentRef * )( new CInputRef( subnodes ) );
                }
                else if( ( toString( type ) == "TGridRef" ) ) {
                    ref = ( CComponentRef * )( new CGridRef( subnodes ) );
                }
                return ref;
            }
        }
    }
    return NULL;
}

CComponentRef * CComponents::GetComponentAt( int index )
{
    return NULL;
}
