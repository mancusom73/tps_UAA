#include "PanelGarantia.h"
#include "AppMain.h"
#include "actions.h"


PanelGarantia::PanelGarantia( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
	ImporteLabel = new CLabel();
	FolioLabel = new CLabel();
	CodArticuloLabel = new CLabel();
    screenXml = new CScreencfg( "PanelGarantia", "default", "default" );
}

void PanelGarantia::createLabels()
{
    ImporteLabel->setParent( this );
	FolioLabel->setParent( this );
	CodArticuloLabel->setParent( this );

	this->AddObject( ImporteLabel );
	this->AddObject( FolioLabel );
	this->AddObject( CodArticuloLabel );

	SetBackGround( screenXml->getBackgroundXML().c_str() );

	createInputXML();
	createLabelXML();
}

void PanelGarantia::createLabelXML()
{
    screenXml->setLabelXML( ImporteLabel,     "TitImporte" );
	screenXml->setLabelXML( FolioLabel,       "TitComprob" );
	screenXml->setLabelXML( CodArticuloLabel, "TitCuenta" );
}

void PanelGarantia::createInputXML()
{	
	screenXml->setInputXML( FolioInput, this, "InputComprob" );
	screenXml->setInputXML( CodArticuloInput, this, "InputCuenta" );
    screenXml->setInputXML( ImporteInput, this, "InputImporte" );
}

void PanelGarantia::setImporteLabel( const string &s )
{
    MutexGuiEnter();
    ImporteLabel->setText( s );
    ImporteLabel->Draw();
    MutexGuiLeave();
}

void PanelGarantia::setFolioLabel( const string &s )
{
    MutexGuiEnter();
    FolioLabel->setText( s );
    FolioLabel->Draw();
    MutexGuiLeave();
}

void PanelGarantia::setCodArticuloLabel( const string &s )
{
    MutexGuiEnter();
    CodArticuloLabel->setText( s );
    CodArticuloLabel->Draw();
    MutexGuiLeave();
}

void PanelGarantia::clearInputs()
{
    MutexGuiEnter();
    FolioInput->SetText( "" );
    ImporteInput->SetText( "" );
	CodArticuloInput->SetText( "" );
    MutexGuiLeave();
}

void PanelGarantia::setPosInputBox( int Action )
{
    switch( Action ) {
        case A_MUESTRA_DATO_FOLIO_G:
            input_focus = FolioInput;
            break;
		case A_MUESTRA_COD_ARTICULO_G:
            input_focus = CodArticuloInput;
            break;
        case A_MUESTRA_DATO_IMPORTE_G:
            input_focus = ImporteInput;
            break;
    }
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input_focus );
}

CInput * PanelGarantia::getFocus()
{
    return input_focus;
}
