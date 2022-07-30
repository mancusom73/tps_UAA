#include "PanelDenominaciones.h"
#include "AppMain.h"
#include "actions.h"


PanelDenominaciones::PanelDenominaciones( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
    TitPanel = new CLabel();
    TitGrid = new CLabel();
	SubTotal = new CLabel();
	SubTotalTxt = new CLabel();
	CantidadTxt = new CLabel();
    screenXml = new CScreencfg( "denominaciones", "default", "default" );
	//cantidad = new CInput( this, 80, 335, 120, 30, false );
}

void PanelDenominaciones::mostrarInput(const wxString &s, const string &caretPosition)
{
	int  pos = atoi( caretPosition.c_str() );
	MutexGuiEnter();
	cantidad->SetText(s, pos);
	MutexGuiLeave();
}

void PanelDenominaciones::createLabels()
{

    TitPanel->setParent( this );
    TitGrid->setParent( this );
	SubTotal->setParent( this );
	SubTotalTxt->setParent( this );
	CantidadTxt->setParent( this );

    SetBackGround( screenXml->getBackgroundXML().c_str() );
	
	createLabelXML();
    createGridXML();
    createInputXML();
}

void PanelDenominaciones::createInputXML()
{
	screenXml->setInputXML( cantidad, this, "InputCantidad" );
}


void PanelDenominaciones::clearInputs()
{
    cantidad->SetText( "" );
}

void PanelDenominaciones::Init()
{
    SetLabelVisibles( true );
}

void PanelDenominaciones::createLabelXML()
{
    screenXml->setLabelXML( TitPanel, "panelDenominacionesTitle" );
    this->AddObject( TitPanel );
    screenXml->setLabelXML( TitGrid, "DenominacionesTitleGrid" );
    this->AddObject( TitGrid );
	screenXml->setLabelXML( SubTotal, "SubTotalDen" );
    this->AddObject( SubTotal );
	screenXml->setLabelXML( SubTotalTxt, "SubTotalTxt" );
    this->AddObject( SubTotalTxt );
	screenXml->setLabelXML( CantidadTxt, "CantidadTxt" );
    this->AddObject( CantidadTxt );
}



CInput * PanelDenominaciones::getFocus()
{
	cantidad->setColour("0000");
	return cantidad;
}


void PanelDenominaciones::createGridXML()
{
    screenXml->setGridXML( Denominaciones, this, "GridDenominaciones" );
}




void PanelDenominaciones::SetLabelVisibles( bool visible )
{
    TitPanel->setVisible( visible );
    TitGrid->setVisible( visible );
	SubTotal->setVisible( visible );
	SubTotalTxt->setVisible( visible );
	CantidadTxt->setVisible( visible );
}


void PanelDenominaciones::AddDenominaciones( const string &codigo, const string &descripcion )
{
    vector<string> row;
	row.push_back( codigo );
    row.push_back( descripcion );
    Denominaciones->AddRow( row );
}

void PanelDenominaciones::setCellText(const string &row, const string &col, const wxString &text)
{
	int fila = atoi( row.c_str());
	int columna = atoi ( col.c_str());
	MutexGuiEnter();
	Denominaciones->SetCellValue(fila, columna, text);
	MutexGuiLeave();
}

void PanelDenominaciones::getCellText(const string &row, const string &col, char *str)
{
	int fila = atoi( row.c_str());
	int columna = atoi ( col.c_str());
	wxString value;

	MutexGuiEnter();
	value = Denominaciones->GetCellValue(fila, columna);
	sprintf( str, "%s", value.c_str() );
	MutexGuiLeave();
	
}

void PanelDenominaciones::ScrollDenominaciones( const string &lines )
{
    static int curr_line = 0;
    int line = atoi( lines.c_str() );

    
    if( line == -99 )                                  //deshabilita
    {
		MutexGuiEnter();
        Denominaciones->Refresh();
        Denominaciones->SetGridCursor( curr_line, 1 );
		MutexGuiLeave();

    }
    else if( line == 0 )                                 //habilita
    {
        MutexGuiEnter();
		Denominaciones->Refresh();
        curr_line = 0;
        Denominaciones->SetGridCursor( curr_line, 0 );
        Denominaciones->SelectRow( curr_line );
		MutexGuiLeave();
    }
    else if( line > 0 )                                    //baja line lineas
    {
        MutexGuiEnter();
		for( int l = 0;l < line;l++ ) {
            if( curr_line< Denominaciones->GetRows() - 1 ) {
                curr_line++;
                Denominaciones->MoveCursorDown( false );
            }
        }

        Denominaciones->SelectRow( curr_line );
		MutexGuiLeave();
    }
    else if( line < 0 )                                    //sube line lineas
    {
        MutexGuiEnter();
		for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                Denominaciones->MoveCursorUp( false );
            }
        }
        Denominaciones->SelectRow( curr_line );
		MutexGuiLeave();
    }
    
}
void PanelDenominaciones::setSubTotal( const string &s )
{
    MutexGuiEnter();
    SubTotal->setText( s );
    SubTotal->Draw();
    MutexGuiLeave();
}
void PanelDenominaciones::ClearGrid()
{
    int rows;
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    if( ( rows = Denominaciones->GetRows() ) > 0 ) {
        Denominaciones->DeleteRows( 0, rows, TRUE );
    }
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}

int PanelDenominaciones::getRowCursor()
{
	return Denominaciones->GetCursorRow();

}
