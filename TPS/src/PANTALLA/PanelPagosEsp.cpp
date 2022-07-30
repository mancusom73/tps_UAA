#include "PanelPagosEsp.h"
#include "actions.h"
#include "typeacti.h"
#include "GuiProperties.h"
#include "CPanel.h"
#include "AppMain.h"

PanelPagosEsp::PanelPagosEsp( const wxString &title, const wxPoint &pos, const wxSize &size,
                              long style ) : CPanelBrowse( title, pos, size )
{
    TitNroSoc = new CLabel();
    TitNroReint = new CLabel();
    TitImport = new CLabel();
    TitFormaPago = new CLabel();
    TitCheque = new CLabel();
    NroSoc = new CLabel();
    NroReint = new CLabel();
    NImport = new CLabel();
    NomMedio = new CLabel();
    NomSubmedio = new CLabel();
    NroCheq = new CLabel();
    NomBanco = new CLabel();

    titCantidad = new CLabel();
    titTotal = new CLabel();
    cant_pc = new CLabel();
    total_pc = new CLabel();
    label_mensaje = new CLabel();
    screenXml = new CScreencfg( "pagosespeciales", "default", "default" );

    #ifdef INVEL_W
    input_browse_del_panel = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_browse_del_panel = new CInput( this, 700, 335, 270, 30, false );
    #endif
}

void PanelPagosEsp::createLabels()
{
    CPanelBrowse::createLabels();
    TitNroSoc->setParent( this );
    TitNroReint->setParent( this );
    TitImport->setParent( this );
    TitFormaPago->setParent( this );
    TitCheque->setParent( this );
    NroSoc->setParent( this );
    NroReint->setParent( this );
    NImport->setParent( this );
    NomMedio->setParent( this );
    NomSubmedio->setParent( this );
    NroCheq->setParent( this );
    NomBanco->setParent( this );

    titCantidad->setParent( this );
    titTotal->setParent( this );
    cant_pc->setParent( this );
    total_pc->setParent( this );
    label_mensaje->setParent( this );
    modo.setParent( this );

    this->AddObject( TitNroSoc );
    this->AddObject( TitNroReint );
    this->AddObject( TitImport );
    this->AddObject( TitFormaPago );
    this->AddObject( TitCheque );
    this->AddObject( NroSoc );
    this->AddObject( NroReint );
    this->AddObject( NImport );
    this->AddObject( NomMedio );
    this->AddObject( NomSubmedio );
    this->AddObject( NroCheq );
    this->AddObject( NomBanco );

    this->AddObject( titCantidad );
    this->AddObject( titTotal );
    this->AddObject( cant_pc );
    this->AddObject( total_pc );
    this->AddObject( label_mensaje );
    this->AddObject( &modo );

    SetBackGround( screenXml->getBackgroundXML().c_str() );


    //createInputXML();
    createLabelXML();
    createIconXML();
    createBrowseXML();
}

void PanelPagosEsp::createBrowseXML()
{
    #ifdef INVEL_W
    input_browse_del_panel->SetNotVisible();
    #endif

    screenXml->setBrowseXML( browse_del_panel, this, input_browse_del_panel, "browseCobro",
                             _CONSULTA_PAGOS_CAJA );
    input_focus = input_browse_del_panel;
}

/*void PanelPagosEsp::createInputXML()
{
   screenXml->setInputXML(input_comprob, this, "InputComprob");   
   screenXml->setInputXML(input_importe, this, "InputImporte");   
   screenXml->setInputXML(input_iva, this, "InputIva");   
   screenXml->setInputXML(input_exento, this, "InputExento");   
   screenXml->setInputXML(input_cuenta, this, "InputCuenta");   
   screenXml->setInputXML(input_gravado, this, "InputGravado");   
}*/

void PanelPagosEsp::createIconXML()
{
    screenXml->setIconXML( &modo, "ModoOperacion" );
    string s = screenXml->getImage( "modopago" );
    modo.setBitmap( s.c_str() );
}

void PanelPagosEsp::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( TitNroSoc, "TitNroSoc" );
    screenXml->setLabelXML( TitNroReint, "TitNroReint" );
    screenXml->setLabelXML( TitImport, "TitImport" );
    screenXml->setLabelXML( titCantidad, "TitCantidad" );
    screenXml->setLabelXML( titTotal, "TitTotal" );
    screenXml->setLabelXML( TitCheque, "TitCheque" );
    screenXml->setLabelXML( TitFormaPago, "TitFormaPago" );
    screenXml->setLabelXML( NroSoc, "NroSoc" );
    screenXml->setLabelXML( NroReint, "NroReint" );
    screenXml->setLabelXML( NImport, "NImport" );
    screenXml->setLabelXML( NomMedio, "NomMedio" );
    screenXml->setLabelXML( NomSubmedio, "NomSubmedio" );
    screenXml->setLabelXML( NroCheq, "NroCheq" );
    screenXml->setLabelXML( NomBanco, "NomBanco" );

    screenXml->setLabelXML( cant_pc, "CantPc" );
    screenXml->setLabelXML( total_pc, "TotalPc" );
    screenXml->setLabelXML( labelbrowse, "TitBrowse" );
    labelbrowse->setText( "     codigo          descripcion" );
}

void PanelPagosEsp::init()
{
}


CInput * PanelPagosEsp::getFocus()
{
    return input_focus;
}

/*
void PanelPagosEsp::setPosInputBox(int Action)
{
    switch(Action)
    {
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
    ((AppMain *)CPanel::GetLauncher())->ChangeFocus(input_focus);
}
*/

void PanelPagosEsp::createTitles()
{
    MutexGuiEnter();
    TitNroSoc->setVisible( true );
    TitNroSoc->Draw();
    TitNroReint->setVisible( true );
    TitNroReint->Draw();
    TitImport->setVisible( true );
    TitImport->Draw();
    TitFormaPago->setVisible( true );
    TitFormaPago->Draw();
    TitCheque->setVisible( true );
    TitCheque->Draw();
    MutexGuiLeave();
} 


void PanelPagosEsp::setNroSoc( const string &s )
{
    MutexGuiEnter();
    NroSoc->setText( s );
    NroSoc->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setNroReint( const string &s )
{
    MutexGuiEnter();
    NroReint->setText( s );
    NroReint->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setNImport( const string &s )
{
    MutexGuiEnter();
    NImport->setText( s );
    NImport->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setNomMedio( const string &s )
{
    MutexGuiEnter();
    NomMedio->setText( s );
    NomMedio->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setNomSubmedio( const string &s )
{
    MutexGuiEnter();
    NomSubmedio->setText( s );
    NomSubmedio->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setNroCheq( const string &s )
{
    MutexGuiEnter();
    NroCheq->setText( s );
    NroCheq->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setNomBanco( const string &s )
{
    MutexGuiEnter();
    NomBanco->setText( s );
    NomBanco->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setTitCantidad( const string &s )
{
    MutexGuiEnter();
    titCantidad->setText( s );
    titCantidad->Draw();
    MutexGuiLeave();
}

void PanelPagosEsp::setTitTotal( const string &s )
{
    MutexGuiEnter();
    titTotal->setText( s );
    titTotal->Draw();
    MutexGuiLeave();
}


void PanelPagosEsp::setTotal_pc( const string &s )
{
    MutexGuiEnter();
    total_pc->setText( s );
    total_pc->Draw();
    MutexGuiLeave();
}


void PanelPagosEsp::setCant_pc( const string &s )
{
    MutexGuiEnter();
    cant_pc->setText( s );
    cant_pc->Draw();
    MutexGuiLeave();
}


void PanelPagosEsp::clearInputs()
{
    /*input_comprob->SetText("");
    input_cuenta->SetText("");
    input_importe->SetText("");
    input_gravado->SetText("");
    input_exento->SetText("");
    input_iva->SetText("");*/
    input_focus = input_browse_del_panel;
}


void PanelPagosEsp::setModo()
{
    MutexGuiEnter();
    modo.setVisible( true );
    modo.Draw();
    MutexGuiLeave();
}

/*
void PanelPagosEsp::ShowDiscrimina(bool show)
{

   if ( show == true )
   {
    input_gravado->SetVisible();
    input_exento->SetVisible();
    input_iva->SetVisible();
   }
   else{
    input_gravado->SetNotVisible();
    input_exento->SetNotVisible();
    input_iva->SetNotVisible();
   }

}
*/

void PanelPagosEsp::setLabelMensaje( string text )
{
    label_mensaje->setText( text );
    label_mensaje->Draw();
}
