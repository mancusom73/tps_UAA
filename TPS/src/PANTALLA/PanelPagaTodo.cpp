#include "PanelPagaTodo.h"
#include "AppMain.h"
#include "actions.h"


PanelPagaTodo::PanelPagaTodo( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
	ImporteLabel = new CLabel();
    screenXml = new CScreencfg( "PanelPagaTodo", "default", "default" );
}

void PanelPagaTodo::createLabels()
{
    ImporteLabel->setParent( this );

	this->AddObject( ImporteLabel );

	SetBackGround( screenXml->getBackgroundXML().c_str() );

	createInputXML();
	createLabelXML();
}

void PanelPagaTodo::createLabelXML()
{
    screenXml->setLabelXML( ImporteLabel, "TitImporte" );
}

void PanelPagaTodo::createInputXML()
{
	screenXml->setInputXML( CodBarraInput, this, "InputCodigo" );
    screenXml->setInputXML( ImporteInput, this, "InputImporte" );
}

void PanelPagaTodo::setImporteLabel( const string &s )
{
    MutexGuiEnter();
    ImporteLabel->setText( s );
    ImporteLabel->Draw();
    MutexGuiLeave();
}

void PanelPagaTodo::clearInputs()
{
    MutexGuiEnter();
    CodBarraInput->SetText( "" );
    ImporteInput->SetText( "" );
    MutexGuiLeave();
}

void PanelPagaTodo::setPosInputBox( int Action )
{
    switch( Action ) {
        case A_MUESTRA_DATO_CODIGO_BARRA_PT:
            input_focus = CodBarraInput;
            break;
        case A_MUESTRA_DATO_IMPORTE_PT:
            input_focus = ImporteInput;
            break;
    }
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input_focus );
}

CInput * PanelPagaTodo::getFocus()
{
    return input_focus;
}
