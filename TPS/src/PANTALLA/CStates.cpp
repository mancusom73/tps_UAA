#include "CScreencfg.h"

CStates::CStates( DOMNodeList *list1 )
{
    int k;
    string s;
    DOMNode *state;

    for( k = 0;k< (int)list1->getLength();k++ ) {
        state = list1->item( k );
        s = toString( ( XMLCh * )state->getNodeName() );
        if( s == "iconRef" ) {
            states.push_back( state );
        }
    }
}

CIconRef * CStates::GetIconRefAt( int i )
{
    return ( new CIconRef( states[i - 1] ) );
}

int CStates::GetCount()
{
    return states.size();
}
