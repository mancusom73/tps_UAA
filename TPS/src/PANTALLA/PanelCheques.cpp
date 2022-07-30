#include "PanelCheques.h"
#include "AppMain.h"
#include "actions.h"

PanelCheques::PanelCheques( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
    ChequesLabel = new CLabel();
    TitPanel = new CLabel();
    TitGrid = new CLabel();
    screenXml = new CScreencfg( "cheques", "default", "default" );
    #ifdef INVEL_W /*INPUT OCULTO DE LA GRILLA DEUDA */
    input_grid = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_grid = new CInput( this, 700, 335, 270, 30, false );
    #endif
}

void PanelCheques::createLabels()
{
    ChequesLabel->setParent( this );
    TitPanel->setParent( this );
    TitGrid->setParent( this );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();

    #ifdef INVEL_W
    input_grid->SetNotVisible();
    #endif
}

void PanelCheques::Init()
{
    SetLabelVisibles( true );
}

void PanelCheques::createLabelXML()
{
    screenXml->setLabelXML( ChequesLabel, "ChequesLabel" );
    this->AddObject( ChequesLabel );
    screenXml->setLabelXML( TitPanel, "panelChequesTitle" );
    this->AddObject( TitPanel );
    screenXml->setLabelXML( TitGrid, "chequesTitleGrid" );
    this->AddObject( TitGrid );
}



CInput * PanelCheques::getFocus()
{
    return input_grid;
}


void PanelCheques::createGridXML()
{
    screenXml->setGridXML( Cheques, this, "GridCheques" );
}




void PanelCheques::SetLabelVisibles( bool visible )
{
    MutexGuiEnter();
    ChequesLabel->setVisible( visible );
    TitPanel->setVisible( visible );
    TitGrid->setVisible( visible );
    MutexGuiLeave();
}


void PanelCheques::AddCheque( const string &s, const string &s1, const string &s2,
                                const string &s3, const string &s4 )
{
    MutexGuiEnter();
    vector<string> row;
    row.push_back( s );
    row.push_back( s1 );
    row.push_back( s2 );
    row.push_back( s3 );
    row.push_back( s4 );
    Cheques->AddRow( row );
    MutexGuiLeave();
}


void PanelCheques::setSocioLabel( const string &s )
{
    MutexGuiEnter();
    ChequesLabel->setText( s );
    ChequesLabel->Draw();
    MutexGuiLeave();
}


void PanelCheques::ScrollCheques( const string &lines )
{
    static int curr_line = 0;
    int             line = atoi( lines.c_str() );

    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif


    if( line == -99 )                                  //deshabilita
    {
        Cheques->Refresh();
        Cheques->SetGridCursor( curr_line, 1 );
        //Deuda->ScrollLines(Deuda->GetRows());
    }
    else if( line == 0 )                                 //habilita
    {
        Cheques->Refresh();
        curr_line = 0;
        Cheques->SetGridCursor( curr_line, 0 );
        //Deuda->ScrollLines(Deuda->GetRows());
        Cheques->SelectRow( curr_line );
    }
    else if( line > 0 )                                    //baja line lineas
    {
        for( int l = 0;l < line;l++ ) {
            if( curr_line< Cheques->GetRows() - 1 ) {
                curr_line++;
                Cheques->MoveCursorDown( false );
            }
        }

        Cheques->SelectRow( curr_line );
    }
    else if( line < 0 )                                    //sube line lineas
    {
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                Cheques->MoveCursorUp( false );
            }
        }
        Cheques->SelectRow( curr_line );
    }

    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelCheques::ClearGrid()
{
    int rows;
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    if( ( rows = Cheques->GetRows() ) > 0 ) {
        Cheques->DeleteRows( 0, rows, TRUE );
    }
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}
