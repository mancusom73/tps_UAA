#include "CScreencfg.h"

CMessage::CMessage( DOMNodeList *subnodes1 )
{
    message = "";
    for( int i = 0;i< (int)subnodes1->getLength();i++ ) {
        DOMNode *subnodes = subnodes1->item( i );
        message += toString( ( XMLCh * )subnodes->getNodeValue() );
    }
}

string CMessage::GetMessage()
{
    return message;
}

