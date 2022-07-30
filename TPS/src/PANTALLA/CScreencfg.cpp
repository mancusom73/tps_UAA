#pragma warning (disable:4243)
#include <iostream> 
#include <fstream> 
#include "CScreencfg.h"
#include "cr.h"

extern CDesktop *properties;

using namespace std;

/******************* Auxiliar Functions *************************/

XMLCh * toXMLCh( string s )
{
    int i, len = s.length();

    XMLCh *aux = ( XMLCh * )calloc( len + 1, sizeof( XMLCh ) );

    for( i = 0;i < len;i++ ) {
        aux[i] = ( XMLCh )s[i];
    }
    aux[i] = 0;
    return aux;
}

string toString( XMLCh *s )
{
    string aux;
    char c;

    for( int i = 0;s[i] != 0;i++ ) {
        c = ( char )s[i];
        aux += c;
    }
    return aux;
}

/******************* ScreenCfg Functions *************************/

CScreencfg::CScreencfg( string winname, string skin, string lang )
{
    CImages *images;
    //CSkin* skin1;
    string id;

    repository = properties->GetRepository();

    window = properties->GetWindows()->GetWindowByName( winname );
    id = window->GetImageRef()->GetID();

    images = properties->GetImages();

    def = images->GetSkinByName( skin );
    background = def->GetImageByName( id )->GetFilename();

    texts = properties->GetText()->GetLangByName( lang );
}

void CScreencfg::getLabelPanelAttribute( CTLabel *label, string &text1, string &font, string &color,
                                         string &align )
{
    CText *text;
    CLang *lang;
    string id;

    font = label->GetLabelFont()->ToString(); 
    align = label->GetLabelAlign(); 
    color = label->GetLabelColor();
    id = label->GetLabelText();

    text = properties->GetText();
    lang = text->GetLangByName( "default" );
    text1 = lang->GetMessageByName( id )->GetMessage();
    delete lang;
}

CComponent * CScreencfg::getComponentAndPos( string name, string type, int &x, int &y )
{
    CComponentRef *compRef;
    CComponent *comp;

    compRef = window->GetComponents()->GetComponentRefByName( name );
    x = compRef->GetPosx();
    y = compRef->GetPosy();
    comp = repository->GetComponentByName( name, type );
    delete compRef;
    return comp;
}

CComponent * CScreencfg::getComponentAndPos( string name, int &x, int &y )
{
    CComponentRef *compRef;
    CComponent *comp;

    compRef = window->GetComponents()->GetComponentRefByName( name );
    x = compRef->GetPosx();
    y = compRef->GetPosy();
    comp = repository->GetComponentByName( name );
    delete compRef;
    return comp;
}

void CScreencfg::setFlagXML( CFlag *flag1, string name, CGrid *datos )
{
    CTFlag *flag;
    CTIcon *icon;
    CStates *states;
    CComponentRef *compRef;

    string ref;
    string align,font,color,text,s,id,background;
    string aux;
    int x,y,textX,textY,iconY,iconX;

    flag = ( CTFlag * )getComponentAndPos( name, "TFlag", x, y );
    states = flag->GetFlagStates();
    for( int i = 1;i<= states->GetCount();i++ ) {
        compRef = ( CComponentRef * )states->GetIconRefAt( i );
        iconX = compRef->GetPosx();
        iconY = compRef->GetPosy();
        ref = compRef->GetID();
        icon = ( CTIcon * )repository->GetComponentByName( ref, "TIcon" );
        id = icon->GetIconImageRef()->GetID();
        aux = def->GetImageByName( id )->GetFilename();
        textX = icon->GetIconPosx();
        textY = icon->GetIconPosy();
        align = icon->GetIconAlign();
        color = icon->GetIconColor();
        font = icon->GetIconFont()->ToString();
        flag1->addState( x + iconX, y + iconY, x + textX, y + textY, align, color, font, aux );
        delete compRef;
    }
    flag1->setShowType( flag->GetFlagShow() );
    flag1->setGrid( datos );
    flag1->changeCurrentState( 0 );
    flag1->setVisible( false );
    delete icon;
    delete flag;
}

void CScreencfg::setFlagXML( CFlag *flag1, string name )
{
    setFlagXML( flag1, name, NULL );
}

void CScreencfg::setIconXML( CIcon *icon1, string name )
{
    CTIcon *icon;
    CComponentRef *iconRef;
    ifstream inFile;
    wxString wxstr = "";

    string align,font,color,text,s,id,background;
    string aux;
    int x,y,iconX,iconY,textX,textY;


    icon = ( CTIcon * )getComponentAndPos( name, "TIcon", x, y );
    iconRef = ( CComponentRef * )icon->GetIconImageRef(); 
    id = iconRef->GetID();
    aux = def->GetImageByName( id )->GetFilename();

    // controlo que el archivo exista, 
    // en caso contrario muestro un mesaje.
    // esto debe ser reemplazado por una exception
    //inFile.open(aux.c_str());
    //wxstr.Printf("Archivo inexistente: %s", aux.c_str());
    //if(!inFile.is_open())
    //throw(E_FileNotFound);
    //wxMessageBox(wxstr, "Error", wxOK|wxICON_ERROR, NULL);
    //else
    //inFile.close();

    align = icon->GetIconAlign();
    iconX = iconRef->GetPosx();
    iconY = iconRef->GetPosy();
    icon1->setIconAtribute( x + iconX, y + iconY, "left-top", aux );
    textX = icon->GetIconPosx();
    textY = icon->GetIconPosy();
    align = icon->GetIconAlign();
    color = icon->GetIconColor();
    font = icon->GetIconFont()->ToString();

    icon1->setUseText( false );
    icon1->setTextAtribute( x + textX, y + textY, font, color, align );
    icon1->setText( "" );
    icon1->setVisible( false );
    delete icon;
}

void CScreencfg::setLabelXML( CLabel *label1, string name )
{
    CTLabel *label;

    string align,font,color,text,s,id,background;
    string aux;
    int x,y;

    label = ( CTLabel * )getComponentAndPos( name, "TLabel", x, y );
    getLabelPanelAttribute( label, text, font, color, align );
    label1->setAtribute( x, y, font, color, align );
    label1->setText( text );
    label1->setVisible( false );
    delete label;
}

void CScreencfg::setGridXML( CGrid *&grid, CPanel *panel, string name )
{
    int x,y,w,h;
    int rowSize;
    string fontStr,align;
    string fontColor,backColor,selColor;

    browse_field field;
    vector<browse_field> fields;


    CTGrid *grid1;
    CFont *font;
    CColumn *column;
    CColor *col;
    CGrid *grid_local;

    grid1 = ( CTGrid * )getComponentAndPos( name, "TGrid", x, y );
    w = grid1->GetGridSize()->GetWidth();
    h = grid1->GetGridSize()->GetHeight();

    for( int i = 1;i<= grid1->GetGridColumnCount();i++ ) {
        column = grid1->GetGridColumnAt( i );
        align = column->GetAlign();
        field.Name = "";
        if( align == "left" ) {
            field.Alignment = ALIGN_LEFT;
        }
        else if( align == "right" ) {
            field.Alignment = ALIGN_RIGHT;
        }
        else if( align == "center" ) {
            field.Alignment = ALIGN_CENTER;
        }
        field.Width = column->GetWidth();
        fields.push_back( field );
    }

    column = grid1->GetGridColumnAt( 1 );

    rowSize = grid1->GetGridRowSize();
    font = column->GetFont();
    fontStr = font->ToString();

    col = column->GetColor();
    fontColor = col->GetText();
    backColor = col->GetBackground();
    delete col;

    col = column->GetSelectedColor();
    selColor = col->GetBackground();
    delete col;
    delete font;

    grid_local = new CGrid( panel, wxPoint( x, y ), wxSize( w, h ) );
    grid_local->Init( fields, 0, 0 );
    grid_local->SetColLabelSize( 0 );
    grid_local->SetRowLabelSize( 0 );
    grid_local->EnableEditing( false );
    grid_local->EnableGridLines( false );
    grid_local->SetRowsSize( rowSize );
    grid_local->iniFont( fontStr );
    grid_local->iniColour( fontColor, 0 );
    grid_local->iniColour( backColor, 1 );
    grid_local->iniColour( selColor, 2 );

    grid = grid_local;
}

void CScreencfg::setInputXML( CInput *&input1, CPanel *panel, string name )
{
    CTInput *input;
    string align,font,color,colorBack,text,s,id,background;
    string aux;
    int x,y,w,h;

    input = ( CTInput * )getComponentAndPos( name, "TInput", x, y );
    w = input->GetInputSize()->GetWidth();
    h = input->GetInputSize()->GetHeight();
    color = input->GetInputColor()->GetText();
    colorBack = input->GetInputColor()->GetBackground();
    font = input->GetInputFont()->ToString();
    input1 = new CInput( panel, x, y, w, h, true );
    input1->setFont( font );
    input1->setColour( color );
    input1->setBackgroundColour( colorBack );
    delete input;
}


void CScreencfg::setBrowseXML( CBrowse *&browse, CPanelBrowse *panel, CInput *input, string name,
                               int scr )
{
    int x,y,w,h;
    int rowSize;
    string fontStr,columnSize;
    string fontColor,backColor,selColor;
    CTGrid *grid1;
    CFont *font;
    CColumn *column;
    CColor *col;
    //   CGrid* grid_local;
    char aux[10];
    int i;

    grid1 = ( CTGrid * )getComponentAndPos( name, "TGrid", x, y );
    w = grid1->GetGridSize()->GetWidth();
    h = grid1->GetGridSize()->GetHeight();


    columnSize = "(";

    for( i = 1;i<= grid1->GetGridColumnCount();i++ ) {
        column = grid1->GetGridColumnAt( i );
        _snprintf(aux, sizeof(aux)-1, "%d", column->GetWidth() );
        columnSize += string( aux ) + "-";
    }
    columnSize[columnSize.length() - 1] = ')';

    column = grid1->GetGridColumnAt( 1 );
    rowSize = grid1->GetGridRowSize();
    font = column->GetFont();
    fontStr = font->ToString();
    col = column->GetColor();
    fontColor = col->GetText();
    backColor = col->GetBackground();
    col = column->GetSelectedColor();
    selColor = col->GetBackground();
    delete col;
    delete font;

    browse = new CBrowse( panel, wxPoint( x, y ), wxSize( w, h ) );
    browse->setPanelBrowseActivo( ( char* )panel );
    browse->iniFont( fontStr );
    browse->iniColour( fontColor, 0 );
    browse->iniColour( backColor, 1 );
    browse->iniColour( selColor, 2 );
    browse->SetRowsSize( rowSize );
    browse->setWidthColBrowse( columnSize, scr );

    panel->SetDefaultBrowseBrowse( browse );
    panel->SetDefaultInputBrowse( input );
}

string CScreencfg::getBackgroundXML()
{
    return background;
}

string CScreencfg::getImage( string name )
{
    CImage *aux;
    string str;
    aux = def->GetImageByName( name );
    str = aux->GetFilename();
    delete aux;
    return str;
}

string CScreencfg::getMessage( string name )
{
    CMessage *msg;
    string aux;
    msg = texts->GetMessageByName( name );
    aux = msg->GetMessage();
    delete msg;
    return aux;
}

void CScreencfg::setBackground(string filename)
{
	background = filename;
}
