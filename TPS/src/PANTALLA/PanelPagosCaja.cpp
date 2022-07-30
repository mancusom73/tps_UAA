#include "PanelPagosCaja.h"
#include "actions.h"
#include "typeacti.h"
#include "GuiProperties.h"
#include "CPanel.h"
#include "AppMain.h"

PanelPagosCaja::PanelPagosCaja( const wxString &title, const wxPoint &pos, const wxSize &size,
                                long style ) : CPanelBrowse( title, pos, size )
{
    titComprob = new CLabel();
    titCuenta = new CLabel();
    titImporte = new CLabel();
    titGravado = new CLabel();
    titExento = new CLabel();
    titIva = new CLabel();
    titCantidad = new CLabel();
    titTotal = new CLabel();
    cant_pc = new CLabel();
    total_pc = new CLabel();
    label_mensaje = new CLabel();
    screenXml = new CScreencfg( "pagoscaja", "default", "default" );
    #ifdef INVEL_W
    input_browse_del_panel = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_browse_del_panel = new CInput( this, 700, 335, 270, 30, false );
    #endif
}


void PanelPagosCaja::createLabels()
{
    CPanelBrowse::createLabels();
    titComprob->setParent( this );
    titCuenta->setParent( this );
    titImporte->setParent( this );
    titGravado->setParent( this );
    titExento->setParent( this );
    titIva->setParent( this );
    titCantidad->setParent( this );
    titTotal->setParent( this );
    cant_pc->setParent( this );
    total_pc->setParent( this );
    label_mensaje->setParent( this );
    modo.setParent( this );

    this->AddObject( titComprob );
    this->AddObject( titCuenta );
    this->AddObject( titImporte );
    this->AddObject( titGravado );
    this->AddObject( titExento );
    this->AddObject( titIva );
    this->AddObject( titCantidad );
    this->AddObject( titTotal );
    this->AddObject( cant_pc );
    this->AddObject( total_pc );
    this->AddObject( label_mensaje );
    this->AddObject( &modo );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createInputXML();
    createLabelXML();
    createIconXML();
    createBrowseXML();
}

void PanelPagosCaja::createBrowseXML()
{
    #ifdef INVEL_W
    input_browse_del_panel->SetNotVisible();
    #endif

    screenXml->setBrowseXML( browse_del_panel, this, input_browse_del_panel, "browseCobro",
                             _CONSULTA_PAGOS_CAJA );
    input_focus = input_browse_del_panel;
}

void PanelPagosCaja::createInputXML()
{
    screenXml->setInputXML( input_comprob, this, "InputComprob" );   
    screenXml->setInputXML( input_importe, this, "InputImporte" );   
    screenXml->setInputXML( input_iva, this, "InputIva" );   
    screenXml->setInputXML( input_exento, this, "InputExento" );   
    screenXml->setInputXML( input_cuenta, this, "InputCuenta" );   
    screenXml->setInputXML( input_gravado, this, "InputGravado" );
}

void PanelPagosCaja::createIconXML()
{
    screenXml->setIconXML( &modo, "ModoOperacion" );
    string s = screenXml->getImage( "modopago" );
    modo.setBitmap( s.c_str() );
}

void PanelPagosCaja::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( titComprob, "TitComprob" );
    screenXml->setLabelXML( titCuenta, "TitCuenta" );
    screenXml->setLabelXML( titImporte, "TitImporte" );
    screenXml->setLabelXML( titCantidad, "TitCantidad" );
    screenXml->setLabelXML( titTotal, "TitTotal" );
    screenXml->setLabelXML( titExento, "TitExento" );
    screenXml->setLabelXML( titGravado, "TitGravado" );
    screenXml->setLabelXML( titIva, "TitIva" );
    screenXml->setLabelXML( cant_pc, "CantPc" );
    screenXml->setLabelXML( total_pc, "TotalPc" );
    screenXml->setLabelXML( labelbrowse, "TitBrowse" );
    labelbrowse->setText( "     codigo          descripcion" );
}

void PanelPagosCaja::init()
{
}


CInput * PanelPagosCaja::getFocus()
{
    return input_focus;
}

void PanelPagosCaja::setPosInputBox( int Action )
{
    switch( Action ) {
        case A_MUESTRA_DATO_COMPROB_PAGOS_CAJA:
            input_focus = input_comprob;
            break;
        case A_MUESTRA_DATO_CUENTA_PAGOS_CAJA:
            input_focus = input_cuenta;
            break;
        case A_MUESTRA_DATO_IMPORTE_PAGOS_CAJA:
            input_focus = input_importe;
            break;
        case A_MUESTRA_DATO_GRAVADO_PAGOS_CAJA:
            input_focus = input_gravado;
            break;
        case A_MUESTRA_DATO_EXENTO_PAGOS_CAJA:
            input_focus = input_exento;
            break;
        case A_MUESTRA_DATO_IVA_PAGOS_CAJA:
            input_focus = input_iva;
            break;
    }
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input_focus );
}


void PanelPagosCaja::setTitComprob( const string &s )
{
    MutexGuiEnter();
    titComprob->setText( s );
    titComprob->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setTitCuenta( const string &s )
{
    MutexGuiEnter();
    titCuenta->setText( s );
    titCuenta->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setTitImporte( const string &s )
{
    MutexGuiEnter();
    titImporte->setText( s );
    titImporte->Draw();
    MutexGuiLeave();
}

void PanelPagosCaja::setTitGravado( const string &s )
{
    MutexGuiEnter();
    titGravado->setText( s );
    titGravado->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setTitExento( const string &s )
{
    MutexGuiEnter();
    titExento->setText( s );
    titExento->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setTitIva( const string &s )
{
    MutexGuiEnter();
    titIva->setText( s );
    titIva->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setTitCantidad( const string &s )
{
    MutexGuiEnter();
    titCantidad->setText( s );
    titCantidad->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setTitTotal( const string &s )
{
    MutexGuiEnter();
    titTotal->setText( s );
    titTotal->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setTotal_pc( const string &s )
{
    MutexGuiEnter();
    total_pc->setText( s );
    total_pc->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::setCant_pc( const string &s )
{
    MutexGuiEnter();
    cant_pc->setText( s );
    cant_pc->Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::clearInputs()
{
    input_comprob->SetText( "" );
    input_cuenta->SetText( "" );
    input_importe->SetText( "" );
    input_gravado->SetText( "" );
    input_exento->SetText( "" );
    input_iva->SetText( "" );
    input_focus = input_browse_del_panel;
}


void PanelPagosCaja::setModo( const string &s )
{
    MutexGuiEnter();
    modo.setVisible( true );
    modo.Draw();
    MutexGuiLeave();
}


void PanelPagosCaja::ShowDiscrimina( bool show )
{
    if( show == true ) {
        input_gravado->SetVisible();
        input_exento->SetVisible();
        input_iva->SetVisible();
    }
    else {
        input_gravado->SetNotVisible();
        input_exento->SetNotVisible();
        input_iva->SetNotVisible();
    }
}


void PanelPagosCaja::setLabelMensaje( string text )
{
    label_mensaje->setText( "" );
    label_mensaje->Draw();
}
