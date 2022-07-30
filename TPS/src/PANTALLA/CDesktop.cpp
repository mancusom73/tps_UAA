#include "CScreencfg.h"

DOMNodeList * CDesktop::GetNodeByTagName( string name )
{
    return document->getElementsByTagName( ( const XMLCh * )toXMLCh( name ) );
}

DOMNodeList * CDesktop::GetChildsByTagName( string tagname, string name )
{
    DOMNodeList *nodes = GetNodeByTagName( tagname );

    for( int i = 0;i< (int)nodes->getLength();i++ ) {
        DOMNode *subnodes = nodes->item( i );

        DOMNamedNodeMap *attr = subnodes->getAttributes();
        XMLCh *name1 = ( XMLCh * )( attr->item( 0 ) )->getNodeValue();

        if( !toString( name1 ).compare( name ) ) {
            return subnodes->getChildNodes();
        }
    }
    return NULL;
}

CDesktop::~CDesktop()
{
    delete windows;
    delete text;
    delete repository;
    delete forms;
    delete images;
    delete parser;

    XMLPlatformUtils::Terminate();
}

CDesktop::CDesktop( string xmlname )
{
    try {
        XMLPlatformUtils::Initialize();
    }
    catch( const XMLException &toCatch ) {
        char *message = XMLString::transcode( toCatch.getMessage() );
        cout << "Error during initialization! :\n" << message << "\n";
        XMLString::release( &message );
        error = INITIALIZE_ERR;
        return;
    }

    parser = new XercesDOMParser();
    parser->setValidationScheme( XercesDOMParser::Val_Always );    // optional.
    parser->setDoNamespaces( true );    // optional

    errHandler = ( ErrorHandler * ) new HandlerBase();
    parser->setErrorHandler( errHandler );

    try {
        parser->parse( xmlname.c_str() );
    }
    catch( const XMLException &toCatch ) {
        char *message = XMLString::transcode( toCatch.getMessage() );
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release( &message );
        error = PARSE_ERR;
        return;
    }
    catch( const DOMException &toCatch ) {
        char *message = XMLString::transcode( toCatch.msg );
        cout << "Exception message is: \n" << message << "\n";
        XMLString::release( &message );
        error = DOM_ERR;
        return;
    }
    catch( ... ) {
        cout << "Unexpected Exception \n" ;
        error = UNKNOWN_ERR;
        return;
    }
    document = parser->getDocument();

    windows = new CWindows( GetNodeByTagName( "window" ) );
    text = new CText( GetNodeByTagName( "lang" ) );
    repository = new CRepository( GetNodeByTagName( "component" ) );
    forms = new CForms( GetNodeByTagName( "form" ) );
    images = new CImages( GetNodeByTagName( "skin" ) );
}

CWindows * CDesktop::GetWindows()
{
    return windows;
}

CText * CDesktop::GetText()
{
    return text;
}

CForms * CDesktop::GetForms()
{
    return forms;
}

CRepository * CDesktop::GetRepository()
{
    return repository;
}

CImages * CDesktop::GetImages()
{
    return images;
}
