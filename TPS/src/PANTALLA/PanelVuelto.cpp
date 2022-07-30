#include "PanelVuelto.h"
#include "AppMain.h"
#include "actions.h"


PanelVuelto::PanelVuelto( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
//    SocioLabel = new CLabel();
    TitPanel = new CLabel();
    TitGrid = new CLabel();
    screenXml = new CScreencfg( "vueltos", "default", "default" );
    #ifdef INVEL_W /*INPUT OCULTO DE LA GRILLA DEUDA */
    input_grid_vuelto = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_grid_vuelto = new CInput( this, 700, 335, 270, 30, false );
    #endif
}

void PanelVuelto::createLabels()
{
//    SocioLabel->setParent( this );
    TitPanel->setParent( this );
    TitGrid->setParent( this );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();

    #ifdef INVEL_W /*INPUT OCULTO DE LA GRILLA DEUDA */
    input_grid_vuelto->SetNotVisible();
    #endif
}

void PanelVuelto::Init()
{
    SetLabelVisibles( true );
}

void PanelVuelto::createLabelXML()
{
    /*screenXml->setLabelXML( SocioLabel, "SocioLabel" );
    this->AddObject( SocioLabel );*/
    screenXml->setLabelXML( TitPanel, "panelVueltoTitle" );
    this->AddObject( TitPanel );
    screenXml->setLabelXML( TitGrid, "VueltoTitleGrid" );
    this->AddObject( TitGrid );
}



CInput * PanelVuelto::getFocus()
{
    return input_grid_vuelto;
}


void PanelVuelto::createGridXML()
{
    screenXml->setGridXML( Vuelto, this, "GridVuelto" );
}




void PanelVuelto::SetLabelVisibles( bool visible )
{
    TitPanel->setVisible( visible );
    TitGrid->setVisible( visible );
}


void PanelVuelto::AddVuelto( const string &codigo, const string &descripcion, const string &vuelto )
{
    vector<string> row;
	row.push_back( codigo );
    row.push_back( descripcion );
	row.push_back( vuelto );
    Vuelto->AddRow( row );
}


void PanelVuelto::setSocioLabel( const string &s )
{
    MutexGuiEnter();
//    SocioLabel->setText( s );
//    SocioLabel->Draw();
    MutexGuiLeave();
}


void PanelVuelto::ScrollVuelto( const string &lines )
{
    static int curr_line = 0;
    int             line = atoi( lines.c_str() );

    MutexGuiEnter();
    if( line == -99 )                                  //deshabilita
    {
        Vuelto->Refresh();
        Vuelto->SetGridCursor( curr_line, 1 );
    }
    else if( line == 0 )                                 //habilita
    {
        Vuelto->Refresh();
        curr_line = 0;
        Vuelto->SetGridCursor( curr_line, 0 );
        Vuelto->SelectRow( curr_line );
    }
    else if( line > 0 )                                    //baja line lineas
    {
        for( int l = 0;l < line;l++ ) {
            if( curr_line< Vuelto->GetRows() - 1 ) {
                curr_line++;
                Vuelto->MoveCursorDown( false );
            }
        }

        Vuelto->SelectRow( curr_line );
    }
    else if( line < 0 )                                    //sube line lineas
    {
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                Vuelto->MoveCursorUp( false );
            }
        }
        Vuelto->SelectRow( curr_line );
    }
    MutexGuiLeave();
}

void PanelVuelto::ClearGrid()
{
    int rows;
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    if( ( rows = Vuelto->GetRows() ) > 0 ) {
        Vuelto->DeleteRows( 0, rows, TRUE );
    }
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}
