#include "PanelConsultaPromos.h"
#include "AppMain.h"
#include "actions.h"

PanelConsultaPromos::PanelConsultaPromos( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
    ConsultaPromosLabel = new CLabel();
    TitPanel = new CLabel();
    TitGrid = new CLabel();
    screenXml = new CScreencfg( "ConsultaPromos", "default", "default" );
    #ifdef INVEL_W /*INPUT OCULTO DE LA GRILLA DEUDA */
    input_grid = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_grid = new CInput( this, 700, 335, 270, 30, false );
    #endif
	input_focus = input_grid;
}

void PanelConsultaPromos::createLabels()
{
    ConsultaPromosLabel->setParent( this );
    TitPanel->setParent( this );
    TitGrid->setParent( this );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();
	createInputXML();

    #ifdef INVEL_W
    input_grid->SetNotVisible();
    #endif
}

void PanelConsultaPromos::Init()
{
    SetLabelVisibles( true );
}

void PanelConsultaPromos::createLabelXML()
{
    screenXml->setLabelXML( ConsultaPromosLabel, "ConsultaPromosLabel" );
    this->AddObject( ConsultaPromosLabel );
    screenXml->setLabelXML( TitPanel, "panelConsultaPromosTitle" );
    this->AddObject( TitPanel );
    screenXml->setLabelXML( TitGrid, "ConsultaPromosTitleGrid" );
    this->AddObject( TitGrid );
}



CInput * PanelConsultaPromos::getFocus()
{
    return input_focus;
}


void PanelConsultaPromos::createGridXML()
{
    screenXml->setGridXML( ConsultaPromos, this, "GridConsultaPromos" );
}

void PanelConsultaPromos::createInputXML()
{
	screenXml->setInputXML( InputCodBarra, this, "InputCodigoConsultaPromos" );
    //screenXml->setInputXML( input_comprob1, this, "InputComprob" );
    //screenXml->setInputXML( input_cuenta, this, "InputCuenta" );
    //screenXml->setInputXML( input_importe, this, "InputImporte" );
}


void PanelConsultaPromos::clearInputs()
{
    InputCodBarra->SetText( "" );
    //input_focus = input_browse_del_panel;
}


void PanelConsultaPromos::SetLabelVisibles( bool visible )
{
    MutexGuiEnter();
    ConsultaPromosLabel->setVisible( visible );
    TitPanel->setVisible( visible );
    TitGrid->setVisible( visible );
    MutexGuiLeave();
}


//void PanelConsultaPromos::AddPromo( char *nombre, char *descripcion )
void PanelConsultaPromos::AddPromo( const string &nombre, const string &descripcion )
{
    MutexGuiEnter();
    vector<string> row;
    row.push_back( nombre );
    row.push_back( " " );
    row.push_back( descripcion );
    //row.push_back( s2 );
    //row.push_back( s3 );
    //row.push_back( s4 );
    ConsultaPromos->AddRow( row );
    MutexGuiLeave();
}


void PanelConsultaPromos::ScrollConsultaPromos( const string &lines )
{
    static int curr_line = 0;
    int             line = atoi( lines.c_str() );

    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif


    if( line == -99 )                                  //deshabilita
    {
        ConsultaPromos->Refresh();
        ConsultaPromos->SetGridCursor( curr_line, 1 );
        //Deuda->ScrollLines(Deuda->GetRows());
    }
    else if( line == 0 )                                 //habilita
    {
        ConsultaPromos->Refresh();
        curr_line = 0;
        ConsultaPromos->SetGridCursor( curr_line, 0 );
        //Deuda->ScrollLines(Deuda->GetRows());
        ConsultaPromos->SelectRow( curr_line );
    }
    else if( line > 0 )                                    //baja line lineas
    {
        for( int l = 0;l < line;l++ ) {
            if( curr_line< ConsultaPromos->GetRows() - 1 ) {
                curr_line++;
                ConsultaPromos->MoveCursorDown( false );
            }
        }

        ConsultaPromos->SelectRow( curr_line );
    }
    else if( line < 0 )                                    //sube line lineas
    {
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                ConsultaPromos->MoveCursorUp( false );
            }
        }
        ConsultaPromos->SelectRow( curr_line );
    }

    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelConsultaPromos::ClearGrid()
{
    int rows;
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    if( ( rows = ConsultaPromos->GetRows() ) > 0 ) {
        ConsultaPromos->DeleteRows( 0, rows, TRUE );
    }
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelConsultaPromos::setPosInputBox( int Action )
{
	input_focus = InputCodBarra;
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input_focus );
}