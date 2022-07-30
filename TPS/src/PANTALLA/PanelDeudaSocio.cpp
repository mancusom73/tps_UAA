#include "PanelDeudaSocio.h"
#include "AppMain.h"
#include "actions.h"


PanelDeudaSocio::PanelDeudaSocio( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
    SocioLabel = new CLabel();
    TitPanel = new CLabel();
    TitGrid = new CLabel();
    screenXml = new CScreencfg( "deudasocio", "default", "default" );
    #ifdef INVEL_W /*INPUT OCULTO DE LA GRILLA DEUDA */
    input_grid_deuda = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_grid_deuda = new CInput( this, 700, 335, 270, 30, false );
    #endif
}

void PanelDeudaSocio::createLabels()
{
    SocioLabel->setParent( this );
    TitPanel->setParent( this );
    TitGrid->setParent( this );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();

    #ifdef INVEL_W /*INPUT OCULTO DE LA GRILLA DEUDA */
    input_grid_deuda->SetNotVisible();
    #endif
}

void PanelDeudaSocio::Init()
{
    SetLabelVisibles( true );
}

void PanelDeudaSocio::createLabelXML()
{
    screenXml->setLabelXML( SocioLabel, "SocioLabel" );
    this->AddObject( SocioLabel );
    screenXml->setLabelXML( TitPanel, "panelDeudaTitle" );
    this->AddObject( TitPanel );
    screenXml->setLabelXML( TitGrid, "deudaTitleGrid" );
    this->AddObject( TitGrid );
}



CInput * PanelDeudaSocio::getFocus()
{
    return input_grid_deuda;
}


void PanelDeudaSocio::createGridXML()
{
    screenXml->setGridXML( Deuda, this, "GridDeudaSocio" );
}




void PanelDeudaSocio::SetLabelVisibles( bool visible )
{
    SocioLabel->setVisible( visible );
    TitPanel->setVisible( visible );
    TitGrid->setVisible( visible );
}


void PanelDeudaSocio::AddDeuda( const string &fecha, const string &tipoMov, const string &nroComp,
                                const string &cuotas, const string &importe, const string &impMin )
{
    vector<string> row;
    row.push_back( fecha );
    row.push_back( tipoMov );
    row.push_back( nroComp );
    row.push_back( cuotas );
    row.push_back( importe );
    row.push_back( impMin );
    Deuda->AddRow( row );
}


void PanelDeudaSocio::setSocioLabel( const string &s )
{
    MutexGuiEnter();
    SocioLabel->setText( s );
    SocioLabel->Draw();
    MutexGuiLeave();
}


void PanelDeudaSocio::ScrollDeuda( const string &lines )
{
    static int curr_line = 0;
    int             line = atoi( lines.c_str() );

    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    //Deuda->SetGridCursor(curr_line,0);
    //Deuda->ClearSelection();
    if( line == -99 )                                  //deshabilita
    {
        Deuda->Refresh();
        Deuda->SetGridCursor( curr_line, 1 );
        //Deuda->ScrollLines(Deuda->GetRows());
    }
    else if( line == 0 )                                 //habilita
    {
        Deuda->Refresh();
        curr_line = 0;
        Deuda->SetGridCursor( curr_line, 0 );
        //Deuda->ScrollLines(Deuda->GetRows());
        Deuda->SelectRow( curr_line );
    }
    else if( line > 0 )                                    //baja line lineas
    {
        for( int l = 0;l < line;l++ ) {
            if( curr_line< Deuda->GetRows() - 1 ) {
                curr_line++;
                Deuda->MoveCursorDown( false );
            }
        }

        Deuda->SelectRow( curr_line );
    }
    else if( line < 0 )                                    //sube line lineas
    {
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                Deuda->MoveCursorUp( false );
            }
        }
        Deuda->SelectRow( curr_line );
    }

    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelDeudaSocio::ClearGrid()
{
    int rows;
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    if( ( rows = Deuda->GetRows() ) > 0 ) {
        Deuda->DeleteRows( 0, rows, TRUE );
    }
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}
