#include "PanelCobro.h"
#include "actions.h"
#include "typeacti.h"
#include "GuiProperties.h"
#include "CPanel.h"
#include "AppMain.h"

PanelCobro::PanelCobro( const wxString &title, const wxPoint &pos, const wxSize &size, long style ) 
: CPanelBrowse( title, pos, size )
{
    titCodBarra = new CLabel();
    titComprob = new CLabel();
    titCuenta = new CLabel();
    titImporte = new CLabel();
	titFolio = new CLabel();
	titCodArticulo = new CLabel();
    titCantidad = new CLabel();
    titTotal = new CLabel();
    cant_pc = new CLabel();
	imp_me = new CLabel();
    total_pc = new CLabel();
    label_mensaje = new CLabel();
    screenXml = new CScreencfg( "cobros", "default", "default" );
    #ifdef INVEL_W
    input_browse_del_panel = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_browse_del_panel = new CInput( this, 700, 335, 270, 30, false );
    #endif
}

void PanelCobro::createLabels()
{
    CPanelBrowse::createLabels();
    titCodBarra->setParent( this );
    titComprob->setParent( this );
    titCuenta->setParent( this );
    titImporte->setParent( this );
	titFolio->setParent( this );
	titCodArticulo->setParent( this );
    titCantidad->setParent( this );
    titTotal->setParent( this );
    cant_pc->setParent( this );
	imp_me->setParent( this );
    total_pc->setParent( this );
    label_mensaje->setParent( this );
	////IconCobroGif.setParent( this );
	IconDevolucionCobro.setParent( this );
    IconCobroInputCodBarra.setParent( this );
    IconCobroInputComprob.setParent( this );
    IconCobroInputCuenta.setParent( this );
    IconCobroInputImporte.setParent( this );

    this->AddObject( titCodBarra );
    this->AddObject( titComprob );
    this->AddObject( titCuenta );
    this->AddObject( titImporte );
	this->AddObject( titFolio );
	this->AddObject( titCodArticulo );
    this->AddObject( titCantidad );
    this->AddObject( titTotal );
    this->AddObject( cant_pc );
	this->AddObject( imp_me );
    this->AddObject( total_pc );
    this->AddObject( label_mensaje );
	////this->AddObject( &IconCobroGif);
	this->AddObject( &IconDevolucionCobro );
    this->AddObject( &IconCobroInputCodBarra );
    this->AddObject( &IconCobroInputComprob );
    this->AddObject( &IconCobroInputCuenta );
    this->AddObject( &IconCobroInputImporte );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createInputXML();
    createIconXML();
    createLabelXML();
    createBrowseXML( "browseCobro" );
	createFlagXML();
}

CInput * PanelCobro::getFocus()
{
    return input_focus;
}

void PanelCobro::createBrowseXML( string name )
{
    #ifdef INVEL_W
    input_browse_del_panel->SetNotVisible();
    #endif

    screenXml->setBrowseXML( browse_del_panel, this, input_browse_del_panel, "browseCobro",
                             _CONSULTA_COBRO );
    input_focus = input_browse_del_panel;
}

void PanelCobro::createInputXML()
{
	screenXml->setInputXML( input_Folio, this, "InputComprob" );
	screenXml->setInputXML( input_CodArticulo, this, "InputCuenta" );
    screenXml->setInputXML( input_CodBarra, this, "InputCodigo" );
    screenXml->setInputXML( input_comprob1, this, "InputComprob" );
    screenXml->setInputXML( input_cuenta, this, "InputCuenta" );
    screenXml->setInputXML( input_importe, this, "InputImporte" );
    input_Folio->SetNotVisible();
    input_CodArticulo->SetNotVisible();
    input_CodBarra->SetNotVisible();
    input_comprob1->SetNotVisible();
    input_cuenta->SetNotVisible();
    input_importe->SetNotVisible();
}

void PanelCobro::createIconXML()
{
	////screenXml->setIconXML( &IconCobroGif, "CIconCobro");
	////IconCobroGif.setVisible( false );
	////IconCobroGif.Draw();
	screenXml->setIconXML( &IconDevolucionCobro, "IconDevCobro");
	IconDevolucionCobro.setVisible( false );
	IconDevolucionCobro.Draw();

    screenXml->setIconXML( &IconCobroInputCodBarra, "IconCobInputCodBarra");
	IconCobroInputCodBarra.setVisible( false );
	IconCobroInputCodBarra.Draw();
    
    screenXml->setIconXML( &IconCobroInputComprob, "IconCobInputComprob");
	IconCobroInputComprob.setVisible( false );
	IconCobroInputComprob.Draw();

    screenXml->setIconXML( &IconCobroInputCuenta, "IconCobInputCuenta");
	IconCobroInputCuenta.setVisible( false );
	IconCobroInputCuenta.Draw();

    screenXml->setIconXML( &IconCobroInputImporte, "IconCobInputImporte");
	IconCobroInputImporte.setVisible( false );
	IconCobroInputImporte.Draw();
    //   setIconXML(&modo, "ModoOperacion");
}

void PanelCobro::createLabelXML()
{
	screenXml->setLabelXML( titFolio, "TitComprob" );
    screenXml->setLabelXML( titCodArticulo, "TitCuenta" );
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( titCodBarra, "TitCodBarra" );
    screenXml->setLabelXML( titComprob, "TitComprob" );
    screenXml->setLabelXML( titCuenta, "TitCuenta" );
    screenXml->setLabelXML( titImporte, "TitImporte" );
    screenXml->setLabelXML( titCantidad, "TitCantidad" );
    screenXml->setLabelXML( titTotal, "TitTotal" );
    screenXml->setLabelXML( cant_pc, "CantPc" );
	screenXml->setLabelXML( imp_me, "ImpMe" );
    screenXml->setLabelXML( total_pc, "TotalPc" );
    screenXml->setLabelXML( labelbrowse, "TitBrowse" );
    labelbrowse->setText( "codigo          descripcion" );
}


void PanelCobro::createFlagXML()
{
}



void PanelCobro::init()
{
}


void PanelCobro::setPosInputBox( int Action )
{
    switch( Action ) {
        case A_MUESTRA_DATO_CODIGO_BARRA:
            input_focus = input_CodBarra;
            break;
        case A_MUESTRA_DATO_COMPROB_COBRO:
            input_focus = input_comprob1;
            break;
        case A_MUESTRA_DATO_CUENTA_COBRO:
            input_focus = input_cuenta;
            break;
        case A_MUESTRA_DATO_IMPORTE_COBRO:
            input_focus = input_importe;
            break;
        case A_MUESTRA_DATO_FOLIO_G:
            input_focus = input_Folio;
            break;
		case A_MUESTRA_COD_ARTICULO_G:
            input_focus = input_CodArticulo;
            break;
    }
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input_focus );
}

void PanelCobro::setTitCodBarra( const string &s )
{
    MutexGuiEnter();
    titCodBarra->setText( s );
    titCodBarra->Draw();
    if( s != " " ) {
        input_CodBarra->SetVisible();
    } else {
        input_CodBarra->SetNotVisible();
    }

    MutexGuiLeave();
}

void PanelCobro::setTitComprob( const string &s )
{
    MutexGuiEnter();
    titComprob->setText( s );
    titComprob->Draw();
    if( s != " " ) {
        input_comprob1->SetVisible();
    } else {
        input_comprob1->SetNotVisible();
    }
    MutexGuiLeave();
}


void PanelCobro::setTitCuenta( const string &s )
{
    MutexGuiEnter();
    titCuenta->setText( s );
    titCuenta->Draw();
    if( s != " " ) {
        input_cuenta->SetVisible();
    } else {
        input_cuenta->SetNotVisible();
    }
    MutexGuiLeave();
}


void PanelCobro::setTitImporte( const string &s )
{
    MutexGuiEnter();
    titImporte->setText( s );
    titImporte->Draw();
    if( s != " " ) {
        input_importe->SetVisible();
    } else {
        input_importe->SetNotVisible();
    }

    MutexGuiLeave();
}


void PanelCobro::setTitFolio( const string &s )
{
    MutexGuiEnter();
    titFolio->setText( s );
    titFolio->Draw();
    if( s != " " ) {
        input_Folio->SetVisible();
    } else {
        input_Folio->SetNotVisible();
    }
    MutexGuiLeave();
}


void PanelCobro::setTitCodArticulo( const string &s )
{
    MutexGuiEnter();
    titCodArticulo->setText( s );
    titCodArticulo->Draw();
    if( s != " " ) {
        input_CodArticulo->SetVisible();
    } else {
        input_CodArticulo->SetNotVisible();
    }
    MutexGuiLeave();
}


void PanelCobro::setTitCantidad( const string &s )
{
    MutexGuiEnter();
    titCantidad->setText( s );
    titCantidad->Draw();
    MutexGuiLeave();
}


void PanelCobro::setTitTotal( const string &s )
{
    MutexGuiEnter();
    titTotal->setText( s );
    titTotal->Draw();
    MutexGuiLeave();
}


void PanelCobro::setTotal_pc( const string &s )
{
    MutexGuiEnter();
    total_pc->setText( s );
    total_pc->Draw();
    MutexGuiLeave();
    input_focus = input_browse_del_panel;
}


void PanelCobro::setCant_pc( const string &s )
{
    MutexGuiEnter();
    cant_pc->setText( s );
    cant_pc->Draw();
    MutexGuiLeave();
}

void PanelCobro::setImp_me( const string &s )
{
    MutexGuiEnter();
    imp_me->setText( s );
    imp_me->Draw();
    MutexGuiLeave();
}

void PanelCobro::clearInputs()
{
    //MutexGuiEnter();
    input_CodBarra->SetText( "" );
	input_comprob1->SetText( "" );
    input_cuenta->SetText( "" );
    input_importe->SetText( "" );
    input_Folio->SetText( "" );
	input_CodArticulo->SetText( "" );
    //MutexGuiLeave();
    input_focus = input_browse_del_panel;
}


void PanelCobro::setModo( const string &s )
{
    //MutexGuiEnter();
    /*    modo.setText(s);
        modo.setVisible(true);
        modo.Draw();
        if( textomodo[0] != "" )
        {
            modo.setVisible(false);
            modo.setBitmap(wxString(textomodo[0].c_str()),true);
            modo.setVisible(true);
        }*/
    //MutexGuiLeave();
}


void PanelCobro::getLabelPanelAttribute( CTLabel *label, string &text, string &font, string &color,
                                         string &align )
{
    font = label->GetLabelFont()->ToString(); 
    align = label->GetLabelAlign(); 
    color = label->GetLabelColor();
    text = label->GetLabelText();
}

CComponent * PanelCobro::getComponentAndPos( string name, int &x, int &y )
{
    CComponentRef *compRef;
    CComponent *comp;

    compRef = cobros->GetComponents()->GetComponentRefByName( name );
    x = compRef->GetPosx();
    y = compRef->GetPosy();
    comp = repository->GetComponentByName( name );
    return comp;
}

CComponent * PanelCobro::getComponentAndPos( string name, string type, int &x, int &y )
{
    CComponentRef *compRef;
    CComponent *comp;
    MutexGuiEnter();
    compRef = cobros->GetComponents()->GetComponentRefByName( name );
    x = compRef->GetPosx();
    y = compRef->GetPosy();
    comp = repository->GetComponentByName( name, type );
    MutexGuiLeave();
    return comp;
}


void PanelCobro::setLabelMensaje( string text )
{
    //MutexGuiEnter();
    label_mensaje->setText( text );
    label_mensaje->Draw();
    //MutexGuiLeave();
}


void PanelCobro::addIcon( const string &opcion, int type )
{

    MutexGuiEnter();
    switch( type ) {
        case T_ICON_DEV_COBRO_AGREGAR :
            IconDevolucionCobro.setVisible( true );
            break;
        case T_ICON_DEV_COBRO_BORRAR :
            IconDevolucionCobro.setVisible( false );
            break;
        case T_ICON_COBRO_AGREGAR :
            ////IconCobroGif.setVisible( true );
            break;
        case T_ICON_COBRO_BORRAR :
            ////IconCobroGif.setVisible( false );
            break;
        case T_ICON_COBRO_COD_BARRA_AGREGAR :
                IconCobroInputCodBarra.setVisible( true );
            break;
        case T_ICON_COBRO_COD_BARRA_BORRAR :
                IconCobroInputCodBarra.setVisible( false );
            break;
        case T_ICON_COBRO_INPUT_COMPROB_AGREGAR :
                IconCobroInputComprob.setVisible( true );
            break;
        case T_ICON_COBRO_INPUT_COMPROB_BORRAR :
                IconCobroInputComprob.setVisible( false );
            break;
        case T_ICON_COBRO_INPUT_CUENTA_AGREGAR :
                IconCobroInputCuenta.setVisible( true );
            break;
        case T_ICON_COBRO_INPUT_CUENTA_BORRAR :
                IconCobroInputCuenta.setVisible( false );
            break;
        case T_ICON_COBRO_INPUT_IMPORTE_AGREGAR :
                IconCobroInputImporte.setVisible( true );
            break;
        case T_ICON_COBRO_INPUT_IMPORTE_BORRAR :
                IconCobroInputImporte.setVisible( false );
            break;
    }
    MutexGuiLeave();
}
