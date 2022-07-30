#include "PanelImportacion.h"
#include "AppMain.h"
#include "actions.h"

PanelImportacion::PanelImportacion( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
    ImportacionLabel = new CLabel();
    TitPanel = new CLabel();
    TitGrid = new CLabel();
    screenXml = new CScreencfg( "Importacion", "default", "default" );
	/*habilita = new wxGridCellAttr( wxColour(255,255,255), wxColour(0,0,255),
                       wxFont( 1, wxNORMAL, wxNORMAL, wxNORMAL ), 1, 1 );
    deshabilita = new wxGridCellAttr( wxColour(0,0,0), wxColour(255,255,255),
                        wxFont( 1, wxNORMAL, wxNORMAL, wxNORMAL ), 1, 1 );*/
    #ifdef INVEL_W 
    input_grid = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_grid = new CInput( this, 700, 335, 270, 30, false );
    #endif
}

void PanelImportacion::createLabels()
{
    ImportacionLabel->setParent( this );
    TitPanel->setParent( this );
    TitGrid->setParent( this );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();

    #ifdef INVEL_W
    input_grid->SetNotVisible();
    #endif
}

void PanelImportacion::Init()
{
    SetLabelVisibles( true );
}

void PanelImportacion::createLabelXML()
{
    screenXml->setLabelXML( ImportacionLabel, "ImportacionLabel" );
    this->AddObject( ImportacionLabel );
    screenXml->setLabelXML( TitPanel, "panelImportacionTitle" );
    this->AddObject( TitPanel );
    screenXml->setLabelXML( TitGrid, "ImportacionTitleGrid" );
    this->AddObject( TitGrid );
}

CInput * PanelImportacion::getFocus()
{
    return input_grid;
}


void PanelImportacion::createGridXML()
{
    screenXml->setGridXML( Importacion, this, "GridImportacion" );
	//habilita->SetFont(Importacion->GetCellFont(0, 0));
	//deshabilita->SetFont(Importacion->GetCellFont(0, 0));
}


void PanelImportacion::SetLabelVisibles( bool visible )
{
    //MutexGuiEnter();
    ImportacionLabel->setVisible( visible );
    TitPanel->setVisible( visible );
    TitGrid->setVisible( visible );
    //MutexGuiLeave();
}


void PanelImportacion::AddImportacion( const string &s, const string &s1, const string &s2, const string &s3 )
{
    MutexGuiEnter();
    vector<string> row;
    row.push_back( s );
    row.push_back( s1 );
    row.push_back( s2 );
	row.push_back( s3 );
    Importacion->AddRow( row );
    MutexGuiLeave();
}


void PanelImportacion::ScrollImportacion( const string &lines )
{
    static int curr_line = 0;
    int             line = atoi( lines.c_str() );
    
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif

    if( line == -99 ) {                                    //deshabilita
        Importacion->Refresh();
        Importacion->SetGridCursor( curr_line, 1 );
    } else if( line == 0 ) {                               //habilita
        Importacion->Refresh();
        curr_line = 0;
        Importacion->SetGridCursor( curr_line, 0 );
        Importacion->SelectRow( curr_line );
    } else if( line > 0 && line != 100 ) {                 //baja line lineas
        for( int l = 0;l < line;l++ ) {
            if( curr_line< Importacion->GetRows() - 1 ) {
                curr_line++;
                Importacion->MoveCursorDown( false );
            }
		}
        Importacion->SelectRow( curr_line );
    } else if( line < 0 && line != -100) {                 //sube line lineas
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                Importacion->MoveCursorUp( false );
            }
        }
        Importacion->SelectRow( curr_line );
    } else if( line == 100 ){
		//Seleccionamos por celdas 
		Importacion->SetCellBackgroundColour(wxColour(255,164,164),curr_line,0);
		Importacion->SetCellBackgroundColour(wxColour(255,164,164),curr_line,1);
		Importacion->SetCellBackgroundColour(wxColour(255,164,164),curr_line,2);
		Importacion->SetCellBackgroundColour(wxColour(255,164,164),curr_line,3);
    } else if( line == - 100 ){   
		Importacion->SetCellBackgroundColour(wxColour(255,255,255),curr_line,0);
		Importacion->SetCellBackgroundColour(wxColour(255,255,255),curr_line,1);
		Importacion->SetCellBackgroundColour(wxColour(255,255,255),curr_line,2);
		Importacion->SetCellBackgroundColour(wxColour(255,255,255),curr_line,3);
    }
    wxYield();
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelImportacion::ClearGrid()
{
    int rows;
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    if( ( rows = Importacion->GetRows() ) > 0 ) {
        Importacion->DeleteRows( 0, rows, TRUE );
    }
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}