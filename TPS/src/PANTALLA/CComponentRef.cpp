#include "CScreencfg.h"

//Component Ref
CComponentRef::CComponentRef( DOMNode *node )
{
    string s = toString( ( XMLCh * )node->getNodeName() );
    DOMNamedNodeMap *attr = node->getAttributes();
    id = toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "id" ) ) )->getNodeValue() );
    posx = atoi(
    toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "posx" ) ) )->getNodeValue() ).c_str() );
    posy = atoi(
    toString( ( XMLCh * )( attr->getNamedItem( toXMLCh( "posy" ) ) )->getNodeValue() ).c_str() );
}

//Label Ref
string CLabelRef::GetID()
{
    return id;
}

int CLabelRef::GetPosx()
{
    return posx;
}

int CLabelRef::GetPosy()
{
    return posy;
}

//Input Ref

string CInputRef::GetID()
{
    return id;
}

int CInputRef::GetPosx()
{
    return posx;
}

int CInputRef::GetPosy()
{
    return posy;
}


//Image Ref

string CImageRef::GetID()
{
    return id;
}

int CImageRef::GetPosx()
{
    return posx;
}

int CImageRef::GetPosy()
{
    return posy;
}


//Flag Ref

string CFlagRef::GetID()
{
    return id;
}

int CFlagRef::GetPosx()
{
    return posx;
}

int CFlagRef::GetPosy()
{
    return posy;
}


//Icon Ref
string CIconRef::GetID()
{
    return id;
}

int CIconRef::GetPosx()
{
    return posx;
}

int CIconRef::GetPosy()
{
    return posy;
}


//Grid Ref
string CGridRef::GetID()
{
    return id;
}

int CGridRef::GetPosx()
{
    return posx;
}

int CGridRef::GetPosy()
{
    return posy;
}
