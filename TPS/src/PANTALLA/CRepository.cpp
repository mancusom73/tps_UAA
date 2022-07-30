#pragma warning (disable:4243)
#include "CScreencfg.h"


CRepository::CRepository( DOMNodeList *list2 )
{
    list1 = list2;
    DOMNodeIndex aux;
    string s;

    for( int i = 0;i< (int)list1->getLength();i++ ) {
        DOMNode *subnodes = list1->item( i );

        s = toString( ( XMLCh * )subnodes->getNodeName() );

        if( ( s == "component" ) ) {
            DOMNamedNodeMap *attr = subnodes->getAttributes();
            string type = toString(
                 ( XMLCh * )(
                          attr->getNamedItem( toXMLCh( "xsi:type" ) ) )->getNodeValue() );
            string name1 = toString(
                ( XMLCh * )( attr->getNamedItem( toXMLCh( "name" ) ) )->getNodeValue() );
            if( type == "TLabel" ) {
                aux.name = name1;
                aux.type = type;
                aux.pos = i;
                labelIndex.push_back( aux );
            }
            else if( type == "TInput" ) {
                aux.name = name1;
                aux.type = type;
                aux.pos = i;
                inputIndex.push_back( aux );
            }
            else if( type == "TIcon" ) {
                aux.name = name1;
                aux.type = type;
                aux.pos = i;
                iconIndex.push_back( aux );
            }
            else if( type == "TFlag" ) {
                aux.name = name1;
                aux.type = type;
                aux.pos = i;
                flagIndex.push_back( aux );
            }
            else if( type == "TGrid" ) {
                aux.name = name1;
                aux.type = type;
                aux.pos = i;
                gridIndex.push_back( aux );
            }
        }
    }
}

DOMNodeIndex CRepository::GetGridIndexElement( string name )
{
    DOMNodeIndex aux;
    for( int i = 0;i < (int)gridIndex.size();i++ ) {
        if( gridIndex[i].name == name ) {
            return gridIndex[i];
        }
    }
    return aux;
}

DOMNodeIndex CRepository::GetInputIndexElement( string name )
{
    DOMNodeIndex aux;
    for( int i = 0;i < (int)inputIndex.size();i++ ) {
        if( inputIndex[i].name == name ) {
            return inputIndex[i];
        }
    }
    return aux;
}

DOMNodeIndex CRepository::GetLabelIndexElement( string name )
{
    DOMNodeIndex aux;
    for( int i = 0;i < (int)labelIndex.size();i++ ) {
        if( labelIndex[i].name == name ) {
            return labelIndex[i];
        }
    }
    return aux;
}

DOMNodeIndex CRepository::GetFlagIndexElement( string name )
{
    DOMNodeIndex aux;
    for( int i = 0;i < (int)flagIndex.size();i++ ) {
        if( flagIndex[i].name == name ) {
            return flagIndex[i];
        }
    }
    return aux;
}

DOMNodeIndex CRepository::GetIconIndexElement( string name )
{
    DOMNodeIndex aux;
    for( int i = 0;i < (int)iconIndex.size();i++ ) {
        if( iconIndex[i].name == name ) {
            return iconIndex[i];
        }
    }
    return aux;
}

DOMNodeIndex CRepository::GetIndexElement( string name, string type )
{
    if( type == "TGrid" ) {
        return GetGridIndexElement( name );
    }
    else if( type == "TLabel" ) {
        return GetLabelIndexElement( name );
    }
    else if( type == "TIcon" ) {
        return GetIconIndexElement( name );
    }
    else if( type == "TInput" ) {
        return GetInputIndexElement( name );
    }
    else if( type == "TFlag" ) {
        return GetFlagIndexElement( name );
    }
    else {
        DOMNodeIndex err;
        return err;
    }
}

CComponent * CRepository::GetComponentByName( string name, string type )
{
    CComponent *comp;
    string s,s1;
    int i,font_index,size_index,color_index;
    int imageref_index,states_index,columns_index;
    DOMNodeList *list2;
    //   DOMNode* font, *size;
    DOMNodeIndex aux;


    aux = GetIndexElement( name, type );

    DOMNode *subnodes = list1->item( aux.pos );

    s = toString( ( XMLCh * )subnodes->getNodeName() );

    DOMNamedNodeMap *attr = subnodes->getAttributes();
    XMLCh *name1 = ( XMLCh * )( attr->getNamedItem( toXMLCh( "name" ) ) )->getNodeValue();

    list2 = subnodes->getChildNodes();

    if( type == "TLabel" ) {
        for( i = 0;i< (int)list2->getLength();i++ ) {
            s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
            if( s == "font" ) {
                font_index = i;
            }
            if( s == "size" ) {
                size_index = i;
            }
        }
        comp = ( CComponent * )( new CTLabel( list2->item( font_index ), list2->item( size_index ),
                                              subnodes ) );
    }
    else if( type == "TInput" ) {
        for( i = 0;i< (int)list2->getLength();i++ ) {
            s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
            if( s == "font" ) {
                font_index = i;
            }
            if( s == "size" ) {
                size_index = i;
            }
            if( s == "colors" ) {
                color_index = i;
            }
        }
        comp = ( CComponent * )( new CTInput( list2->item( font_index ), list2->item( size_index ),
                                              list2->item( color_index ), subnodes ) );
    }
    else if( type == "TIcon" ) {
        for( i = 0;i< (int)list2->getLength();i++ ) {
            s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
            if( s == "font" ) {
                font_index = i;
            }
            if( s == "size" ) {
                size_index = i;
            }
            if( s == "imageRef" ) {
                imageref_index = i;
            }
        }
        comp = ( CComponent * )( new CTIcon( list2->item( font_index ), list2->item( size_index ),
                                             list2->item( imageref_index ), subnodes ) );
    }
    else if( type == "TFlag" ) {
        for( i = 0;i< (int)list2->getLength();i++ ) {
            s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
            if( s == "states" ) {
                states_index = i;
            }
            if( s == "size" ) {
                size_index = i;
            }
        }
        comp = ( CComponent * )( new CTFlag( list2->item( size_index ),
                                             ( list2->item( states_index ) )->getChildNodes(),
                                             subnodes ) );
    }
    else if( type == "TGrid" ) {
        for( i = 0;i< (int)list2->getLength();i++ ) {
            s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
            if( s == "columns" ) {
                columns_index = i;
            }
            if( s == "size" ) {
                size_index = i;
            }
        }
        comp = ( CComponent * )( new CTGrid( list2->item( size_index ),
                                             ( list2->item( columns_index ) )->getChildNodes(),
                                             subnodes ) );
    }
    return comp;
}

CComponent * CRepository::GetComponentByName( string name )
{
    CComponent *comp;
    string s,s1;
    int i,font_index,size_index,color_index;
    int imageref_index,states_index,columns_index;
    //   DOMNodeList* list2;
    //   DOMNode* font, *size;


    for( i = 0;i< (int)list1->getLength();i++ ) {
        DOMNode *subnodes = list1->item( i );

        s = toString( ( XMLCh * )subnodes->getNodeName() );

        if( ( s == "component" ) ) {
            DOMNamedNodeMap *attr = subnodes->getAttributes();
            XMLCh *name1 = ( XMLCh * )( attr->getNamedItem( toXMLCh( "name" ) ) )->getNodeValue();
            XMLCh *type = ( XMLCh * )( attr->getNamedItem( toXMLCh( "xsi:type" ) ) )->getNodeValue();
            if( toString( name1 ) == name ) {
                DOMNodeList *list2 = subnodes->getChildNodes();
                if( ( toString( type ) == "TLabel" ) ) {
                    for( i = 0;i< (int)list2->getLength();i++ ) {
                        s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
                        if( s == "font" ) {
                            font_index = i;
                        }
                        if( s == "size" ) {
                            size_index = i;
                        }
                    }
                    comp = ( CComponent * )( new CTLabel( list2->item( font_index ),
                                                          list2->item( size_index ), subnodes ) );
                }
                else if( ( toString( type ) == "TInput" ) ) {
                    i = list2->getLength();
                    s1 = toString( name1 );
                    for( i = 0;i< (int)list2->getLength();i++ ) {
                        s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
                        if( s == "font" ) {
                            font_index = i;
                        }
                        if( s == "size" ) {
                            size_index = i;
                        }
                        if( s == "colors" ) {
                            color_index = i;
                        }
                    }
                    comp = ( CComponent * )( new CTInput( list2->item( font_index ),
                                                          list2->item( size_index ),
                                                          list2->item( color_index ), subnodes ) );
                }
                else if( ( toString( type ) == "TIcon" ) ) {
                    for( i = 0;i< (int)list2->getLength();i++ ) {
                        s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
                        if( s == "font" ) {
                            font_index = i;
                        }
                        if( s == "size" ) {
                            size_index = i;
                        }
                        if( s == "imageRef" ) {
                            imageref_index = i;
                        }
                    }
                    comp = ( CComponent * )( new CTIcon( list2->item( font_index ),
                                                         list2->item( size_index ),
                                                         list2->item( imageref_index ),
                                                         subnodes ) );
                }
                else if( ( toString( type ) == "TFlag" ) ) {
                    for( i = 0;i< (int)list2->getLength();i++ ) {
                        s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
                        if( s == "states" ) {
                            states_index = i;
                        }
                        if( s == "size" ) {
                            size_index = i;
                        }
                    }
                    comp = ( CComponent * )(
                    new CTFlag( list2->item( size_index ),
                                ( list2->item( states_index ) )->getChildNodes(), subnodes ) );
                }
                else if( ( toString( type ) == "TGrid" ) ) {
                    for( i = 0;i< (int)list2->getLength();i++ ) {
                        s = toString( ( XMLCh * )( list2->item( i ) )->getNodeName() );
                        if( s == "columns" ) {
                            columns_index = i;
                        }
                        if( s == "size" ) {
                            size_index = i;
                        }
                    }
                    comp = ( CComponent * )(
                    new CTGrid( list2->item( size_index ),
                                ( list2->item( columns_index ) )->getChildNodes(), subnodes ) );
                }
                return comp;
            }
        }
    }   
    return NULL;
}

CComponent * CRepository::GetComponentAt( int name )
{
    return NULL;
}
