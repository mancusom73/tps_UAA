#pragma warning (disable:4786)
#include "PanelCobrosEsp.h"
#include "AppMain.h"
#include "GuiProperties.h"
#include "actions.h"
#include "FileVerif.h"
#include <cr.h>


PanelCobrosEsp::PanelCobrosEsp( const wxString &title, const wxPoint &pos, const wxSize &size,
                                long style ) : CPanel( title, pos, size, style )
{
    label_mensaje = new CLabel();
    TitCodigo = new CLabel();
    TitUniNegocioInput = new CLabel();
    TitUniNegocioGrid = new CLabel();
    TitSocioInput = new CLabel();
    SocioLabel = new CLabel();
    TitDetalleGrid = new CLabel();
    TitSubtotal = new CLabel();
    SubtotalLabel = new CLabel();
    TipoCliLabel = new CLabel();
    Domicilio = new CLabel();
    #ifdef INVEL_W
    input_grid_uni_neg = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_grid_uni_neg = new CInput( this, 700, 335, 270, 30, false );
    #endif
    screenXml = new CScreencfg( "cobrosespeciales", "default", "default" );
}

void PanelCobrosEsp::createLabels()
{
    FileVerif verif;

    label_mensaje->setParent( this );
    TitCodigo->setParent( this );
    TitUniNegocioInput->setParent( this );
    TitUniNegocioGrid->setParent( this );
    TitSocioInput->setParent( this );
    SocioLabel->setParent( this );
    TitDetalleGrid->setParent( this );
    TitSubtotal->setParent( this );
    SubtotalLabel->setParent( this );
    TipoCliLabel->setParent( this );
    Domicilio->setParent( this );
    LogoInvel.setParent( this );
    FlagOpMult.setParent( this );
    this->AddObject( &FlagOpMult );
    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();
    createInputXML();
    createIconXML();

    screenXml->setIconXML( &LogoInvel, "LogoInvel" );
    //verif.VerifLogo( true, true, "default", "cobrosespeciales" );

    this->AddObject( &LogoInvel );

    #ifdef INVEL_W /*INPUT OCULTO DE LA GRILLA UNIDAD DE NEGOCIO */
    input_grid_uni_neg->SetNotVisible();
    #endif
    input_focus = input_grid_uni_neg;
}


void PanelCobrosEsp::createIconXML()
{
    screenXml->setIconXML( &FlagOpMult, "FlagOpMult" );
    FlagOpMult.setVisible( false );
}


void PanelCobrosEsp::init( const string &titulo )
{
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    if( DetalleCobros->GetRows() > 0 ) {
        DetalleCobros->DeleteRows( 0, DetalleCobros->GetRows(), TRUE );
    }
    if( UniNegocio->GetRows() > 0 ) {
        UniNegocio->DeleteRows( 0, UniNegocio->GetRows(), TRUE );
    }
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    setMensaje( " " );
    setSocio( " ", " ", " " );
    setSubTotal( " " );
    SetLabelVisibles( true );
    LogoInvel.setVisible( true );
}

void PanelCobrosEsp::createInputXML()
{
    screenXml->setInputXML( InputCodBarra, this, "InputCodigo" );
    screenXml->setInputXML( InputUniNegocio, this, "InputUniNegocio" );
    screenXml->setInputXML( InputSocio, this, "InputSocio" );
}


void PanelCobrosEsp::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    this->AddObject( label_mensaje );
    screenXml->setLabelXML( TitCodigo, "codigoTitle" );
    this->AddObject( TitCodigo );
    screenXml->setLabelXML( TitUniNegocioInput, "UniNegTitle" );
    this->AddObject( TitUniNegocioInput );
    screenXml->setLabelXML( TitUniNegocioGrid, "UniNegTitle2" );
    this->AddObject( TitUniNegocioGrid );
    screenXml->setLabelXML( TitSocioInput, "socioTitle" );
    this->AddObject( TitSocioInput );
    screenXml->setLabelXML( SocioLabel, "SocioLabel" );
    this->AddObject( SocioLabel );
    screenXml->setLabelXML( TitDetalleGrid, "GridCobroTitle" );
    this->AddObject( TitDetalleGrid );
    screenXml->setLabelXML( SubtotalLabel, "SubTotalLabel" );
    this->AddObject( SubtotalLabel );
    screenXml->setLabelXML( TitSubtotal, "SubTotalTitle" );
    this->AddObject( TitSubtotal );
    screenXml->setLabelXML( Domicilio, "Domicilio" );
    this->AddObject( Domicilio );
    screenXml->setLabelXML( TipoCliLabel, "TipoCliLabel" );
    this->AddObject( TipoCliLabel );
}



CInput * PanelCobrosEsp::getFocus()
{
    return input_focus;
}


void PanelCobrosEsp::createGridXML()
{
    screenXml->setGridXML( DetalleCobros, this, "GridDetalleCobrosEsp" );
    screenXml->setGridXML( UniNegocio, this, "GridUniNegCobroEsp" );
    //DetalleCobros->EnableGridLines(true);
}


void PanelCobrosEsp::setMensaje( const string &s )
{
    //MutexGuiEnter();
    label_mensaje->setText( s );
    label_mensaje->Draw();
    //MutexGuiLeave();
}


void PanelCobrosEsp::setSocio( const string &s, const string &TipoCli, const string &DomicilioText )
{
    MutexGuiEnter();
    SocioLabel->setText( s );
    SocioLabel->Draw();
    TipoCliLabel->setText( TipoCli );
    TipoCliLabel->Draw();
    Domicilio->setText( DomicilioText );
    Domicilio->Draw();
    MutexGuiLeave();
}


void PanelCobrosEsp::setSubTotal( const string &s )
{
    MutexGuiEnter();
    SubtotalLabel->setText( s );
    SubtotalLabel->Draw();
    MutexGuiLeave();
}




void PanelCobrosEsp::SetLabelVisibles( bool visible )
{
    label_mensaje->setVisible( visible );
    TitCodigo->setVisible( visible );
    TitUniNegocioInput->setVisible( visible );
    TitUniNegocioGrid->setVisible( visible );
    TitSocioInput->setVisible( visible );
    SocioLabel->setVisible( visible );
    TitDetalleGrid->setVisible( visible );
    TitSubtotal->setVisible( visible );
    SubtotalLabel->setVisible( visible );
}


void PanelCobrosEsp::AddUniNegocio( const string &cod, const string &descripcion )
{
    vector<string> row;
    row.push_back( cod );
    row.push_back( descripcion );
    UniNegocio->AddRow( row );
}


/*void PanelCobrosEsp::AddCobroGrid(const string &uniNeg,const string &detalle,const string &importe)
{
   vector<string> row;
   row.push_back(uniNeg);
   row.push_back(detalle);
   row.push_back(importe);
   DetalleCobros->AddRow(row);

}*/

void PanelCobrosEsp::AddCobroGrid( const string &numeroComp, const string &letra,
                                   const string &tipoMov, const string &reglaAntigua,
                                   const string &fechaVenc, const string &importeMin,
                                   const string &importe, const string &moneda,
                                   const string &fechaEmision, const string &fechaHasta,
                                   const string &periodoFact )
{
    MutexGuiEnter();

    vector<string> row;
    row.push_back( numeroComp );
    row.push_back( letra );
    row.push_back( tipoMov );
    row.push_back( reglaAntigua );
    row.push_back( fechaVenc );
    row.push_back( importeMin );
    row.push_back( importe );
    row.push_back( moneda );
    row.push_back( fechaEmision );
    row.push_back( fechaHasta );
    row.push_back( periodoFact );
    DetalleCobros->AddRow( row );
    DetalleCobros->SetGridCursor( DetalleCobros->GetNumberRows() - 1, 0 );

    #ifdef INVEL_L
    DetalleCobros->Scroll( 0, DetalleCobros->GetNumberRows() );
    #else
    DetalleCobros->ScrollLines( DetalleCobros->GetRows() );
    #endif

    MutexGuiLeave();
}


void PanelCobrosEsp::setPosInputBox( int Action )
{
    switch( Action ) {
        case A_MUESTRA_DATO_CODIGO_BARRA:
            input_focus = InputCodBarra;
            break;
        case A_MUESTRA_DATO_UNIDAD_NEGOCIO:
            input_focus = InputUniNegocio;
            break;
        case A_MUESTRA_DATO_CLIENTE:
            input_focus = InputSocio;
            break;
    }
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input_focus );
}



void PanelCobrosEsp::ScrollUniNeg( const string &lines )
{
    input_focus = input_grid_uni_neg;
    ScrollGrid( lines, UniNegocio );
}


void PanelCobrosEsp::ScrollGrid( const string &lines, CGrid *Grid )
{
    static int curr_line = 0;
    int line = atoi( lines.c_str() );

    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif

    Grid->SetGridCursor( curr_line, 0 );
    Grid->ClearSelection();

    if( line == -99 )                                  //deshabilita
    {
        Grid->Refresh();
        Grid->SetGridCursor( curr_line, 1 );
        Grid->ScrollLines( Grid->GetRows() );
    }
    else if( line == 0 )                                 //habilita
    {
        Grid->Refresh();
        curr_line = 0;
        Grid->SetGridCursor( curr_line, 0 );
        Grid->ScrollLines( Grid->GetRows() );
        Grid->SelectRow( curr_line );
        setPosInputBox( -1 );
    }
    else if( line > 0 )                                    //baja line lineas
    {
        for( int l = 0;l < line;l++ ) {
            if( curr_line< Grid->GetRows() - 1 ) {
                curr_line++;
                Grid->MoveCursorDown( false );
            }
        }

        Grid->SelectRow( curr_line );
    }
    else if( line < 0 )                                    //sube line lineas
    {
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                Grid->MoveCursorUp( false );
            }
        }
        Grid->SelectRow( curr_line );
    }

    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelCobrosEsp::clear()
{
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    if( DetalleCobros->GetRows() > 0 ) {
        DetalleCobros->DeleteRows( 0, DetalleCobros->GetRows(), TRUE );
    }
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    setMensaje( " " );
    setSocio( " ", " ", " " );
    setSubTotal( " " );
    SetLabelVisibles( true );
    LogoInvel.setVisible( true );
}



void PanelCobrosEsp::setOpMult( bool visible )
{
    FlagOpMult.setVisible( visible );
}
