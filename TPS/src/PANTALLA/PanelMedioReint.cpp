#include "PanelMedioReint.h"
#include "GuiProperties.h"
#include "typeacti.h"
#include "CFontMapper.h"

extern CDesktop *properties;

PanelMedioReint::PanelMedioReint( const wxString &title, const wxPoint &pos, const wxSize &size,
                                  long style ) : CPanel( title, pos, size, style )
{
    label_mensaje = new CLabel();
    label_titulo = new CLabel();
    titDispGrid = new CLabel();
    titIngGrid = new CLabel();
    screenXml = new CScreencfg( "mediosreintegros", "default", "default" );
    #ifdef INVEL_W
    input_de_grilla = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_de_grilla = new CInput( this, 700, 335, 270, 30, false );
    #endif
}

void PanelMedioReint::createLabels()
{
    label_mensaje->setParent( this );
    label_titulo->setParent( this );
    titDispGrid->setParent( this );
    titIngGrid->setParent( this );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();

    #ifdef INVEL_W
    input_de_grilla->SetNotVisible();
    #endif
}

void PanelMedioReint::init()
{
    label_mensaje->setVisible( true );
    label_titulo->setVisible( true );
    titDispGrid->setVisible( true );
    titIngGrid->setVisible( true );
}

void PanelMedioReint::finish()
{
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    if( gridDisp->GetRows() > 0 ) {
        gridDisp->DeleteRows( 0, gridDisp->GetRows(), TRUE );
    }
    if( gridIng->GetRows() > 0 ) {
        gridIng->DeleteRows( 0, gridIng->GetRows(), TRUE );
    }
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}

void PanelMedioReint::setTitulo( const string &titulo )
{
    label_titulo->setText( titulo );
}

CInput * PanelMedioReint::getFocus()
{
    return input_de_grilla;
}

void PanelMedioReint::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    this->AddObject( label_mensaje );
    screenXml->setLabelXML( label_titulo, "medioreinttitulo" );
    this->AddObject( label_titulo );
    screenXml->setLabelXML( titDispGrid, "medioreintdisp" );
    this->AddObject( titDispGrid );
    screenXml->setLabelXML( titIngGrid, "medioreinting" );
    this->AddObject( titIngGrid );
}

void PanelMedioReint::createGridXML()
{
    screenXml->setGridXML( gridDisp, this, "grillaReintDisp" );
    screenXml->setGridXML( gridIng, this, "grillaReintIng" );
}

void PanelMedioReint::addGridDisp( const string &nom_banco, const string &cod_banco,
                                   const string &num_cheq, const string &importe,
                                   const string &codigo_rein )
{
    vector<string> row;
    row.push_back( nom_banco );
    row.push_back( cod_banco );
    row.push_back( num_cheq );
    row.push_back( importe );
    row.push_back( codigo_rein );
    gridDisp->AddRow( row );
}

void PanelMedioReint::addGridIng( const string &nom_banco, const string &cod_banco,
                                  const string &num_cheq, const string &importe,
                                  const string &codigo_rein )
{
    gridIng->SelectRow( gridIng->GetGridCursorRow() );
    vector<string> row;
    row.push_back( nom_banco );
    row.push_back( cod_banco );
    row.push_back( num_cheq );
    row.push_back( importe );
    row.push_back( codigo_rein );
    gridIng->AddRow( row );
}

void PanelMedioReint::remGridIng( const string &pos )
{
    MutexGuiEnter();
    //gridIng->RemoveRow( atoi( pos.c_str() ) );
    gridIng->RemoveRow( gridIng->GetRows() - 1 );
    MutexGuiLeave();
}

/* @lines (in) las lineas que hay que desplazar
 * @cantStat (out) cantidad de lineas en la grilla
 * @lineaStat (out) linea seleccionada actualmente
 */
void PanelMedioReint::scrollGridDisp( const string &lines, char *cantStat, char *lineaStat )
{
    static int curr_line = 0;
    int line = atoi( lines.c_str() );

    #ifdef INVEL_L
    MutexGuiEnter();
    #endif

    //gridDisp->SetGridCursor(curr_line,0);
    gridDisp->ClearSelection();

    if( line == -99 )                                  //refresca seleccion
    {
        //gridDisp->SetGridCursor(curr_line,1);
        //gridDisp->ScrollLines(gridDisp->GetRows());
        gridDisp->SelectRow( curr_line );
        //gridDisp->Refresh();
    }
    else if( line == 0 )                                 //habilita
    {
        gridDisp->Refresh();
        curr_line = 0;
        //gridDisp->SetGridCursor(curr_line,0);
        //gridDisp->ScrollLines(gridDisp->GetRows());
        gridDisp->SelectRow( curr_line );
    }
    else if( line > 0 )                                    //baja line lineas
    {
        for( int l = 0;l < line;l++ ) {
            if( curr_line< gridDisp->GetRows() - 1 ) {
                curr_line++;
                gridDisp->MoveCursorDown( false );
            }
        }

        gridDisp->SelectRow( curr_line );
    }
    else if( line < 0 )                                    //sube line lineas
    {
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                gridDisp->MoveCursorUp( false );
            }
        }
        gridDisp->SelectRow( curr_line );
    }

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif

    if( cantStat ) {
        sprintf( cantStat, "%d", gridDisp->GetRows() );
    }
    if( lineaStat ) {
        sprintf( lineaStat, "%d", curr_line );
    }
}

void PanelMedioReint::scrollGridIng( const string &lines )
{
    int line = atoi( lines.c_str() );
    if( line < 0 ) {
        gridIng->MoveCursorUp( false );
        gridIng->SelectRow( gridIng->GetGridCursorRow() );
    }
    else {
        gridIng->MoveCursorDown( false );
        gridIng->SelectRow( gridIng->GetGridCursorRow() );
    }
}

void PanelMedioReint::pedirGridIng( char *s, char *s1 )
{
    wxString str;
    int i = gridIng->GetRows();

    if( i > 0 ) {
        /*banco*/
        str = gridIng->GetCellValue( i - 1, 1 );
        sprintf( s, "%s", str.c_str() );
        /*nro_cheque*/
        str = gridIng->GetCellValue( i - 1, 2 );
        sprintf( s1, "%s", str.c_str() );
    }
}

//Entrada: linea seleccionada, Salida: valor de la columna 4
void PanelMedioReint::pedirGridDisp( char *s )
{
    wxString str;
    int selected = atoi( s );
    int i = gridDisp->GetRows();

    if( ( i > 0 ) && ( selected < i ) ) {
        str = gridDisp->GetCellValue( selected, 4 );
        sprintf( s, "%s", str.c_str() );
    }
}

void PanelMedioReint::removeRowGridDisp( const string &s )
{
    int row,i;
    row = atoi( s.c_str() );
    i = gridDisp->GetRows();

    if( ( i > 0 ) && ( row < i ) ) {
        gridDisp->RemoveRow( row );
    }
}

void PanelMedioReint::removeAllRowsGrid()
// elimina todas las filas de las dos grillas
{
    MutexGuiEnter();
    if( gridDisp->GetRows() > 0 ) {
        gridDisp->DeleteRows( 0, gridDisp->GetRows() );
    }
    if( gridIng->GetRows() > 0 ) {
        gridIng->DeleteRows( 0, gridIng->GetRows() );
    }
    MutexGuiLeave();
}


void PanelMedioReint::setMensaje( const string &s )
{
    MutexGuiEnter();
    label_mensaje->setText( s );
    label_mensaje->Draw();
    Refresh();
    MutexGuiLeave();
}

