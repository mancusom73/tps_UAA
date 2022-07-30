#include "PanelFondoFijo.h"
#include "GuiProperties.h"
#include "typeacti.h"
#include "CFontMapper.h"

extern CDesktop *properties;

PanelFondoFijo::PanelFondoFijo( const wxString &title, const wxPoint &pos, const wxSize &size,
                                long style ) : CPanel( title, pos, size, style )
{
    label_mensaje = new CLabel();
    total = new CLabel();
	labelGridPagos = new CLabel();
    descMedios = new CLabel();
    TitotalPpagos = new CLabel();
    //labelpago = new CLabel();
    label_medios = new CLabel();
    titulo_box = new CLabel();
    label_titulo = new CLabel();
    screenXml = new CScreencfg( "fondofijo", "default", "default" );
}


void PanelFondoFijo::init( const string &titulo )
{
    label_titulo->setText( titulo );
    total->setVisible( false );
    TitotalPpagos->setVisible( false );
    titulo_box->setVisible( false );
    desseleccionaMedio();
    borrarMedios();
    borrarPagos();
}


void PanelFondoFijo::setTituloFfijo( const string &titulo )
{
    label_titulo->setText( titulo );
}


void PanelFondoFijo::createLabels()
{
    labelFlagmedio1.setParent( this );
    labelFlagmedio2.setParent( this );
    labelFlagmedio3.setParent( this );
    labelFlagmedio4.setParent( this );
    labelFlagmedio5.setParent( this );
    labelFlagmedio6.setParent( this );
    labelFlagmedio7.setParent( this );
    labelFlagmedio8.setParent( this );
    labelFlagmedio9.setParent( this );
    labelFlagmedio10.setParent( this );
    //labelpago->setParent(this);
    total->setParent( this );
    descMedios->setParent( this );
    TitotalPpagos->setParent( this );
    label_mensaje->setParent( this );
    label_medios->setParent( this );
    label_titulo->setParent( this );
    titulo_box->setParent( this );
	labelGridPagos->setParent( this );

    this->AddObject( &labelFlagmedio1 );
    this->AddObject( &labelFlagmedio2 );
    this->AddObject( &labelFlagmedio3 );
    this->AddObject( &labelFlagmedio4 );
    this->AddObject( &labelFlagmedio5 );
    this->AddObject( &labelFlagmedio6 );
    this->AddObject( &labelFlagmedio7 );
    this->AddObject( &labelFlagmedio8 );
    this->AddObject( &labelFlagmedio9 );
    this->AddObject( &labelFlagmedio10 );
    this->AddObject( total );
    this->AddObject( descMedios );
    this->AddObject( TitotalPpagos );
    this->AddObject( label_mensaje );
    this->AddObject( label_medios );
    this->AddObject( label_titulo );
    this->AddObject( titulo_box );
	this->AddObject( labelGridPagos );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();
    createInputXML();
    createFlagXML();
}


void PanelFondoFijo::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( total, "TotalPagos" );
    screenXml->setLabelXML( descMedios, "DescMedios" );
    screenXml->setLabelXML( TitotalPpagos, "TitTotalPagos" );
    screenXml->setLabelXML( label_medios, "LabelMedios" );
    label_medios->setVisible( true );
    screenXml->setLabelXML( titulo_box, "TituloBox" );
    screenXml->setLabelXML( label_titulo, "ModoOperacionTexto" );
    label_titulo->setVisible( true );
	screenXml->setLabelXML( labelGridPagos, "TitGridPago" );
	labelGridPagos->setVisible( true );
}

void PanelFondoFijo::createInputXML()
{
    screenXml->setInputXML( input, this, "Importe" );
}

void PanelFondoFijo::createFlagXML()
{
    screenXml->setFlagXML( &labelFlagmedio1, "Pagos1" );
    screenXml->setFlagXML( &labelFlagmedio2, "Pagos2" );
    screenXml->setFlagXML( &labelFlagmedio3, "Pagos3" );
    screenXml->setFlagXML( &labelFlagmedio4, "Pagos4" );
    screenXml->setFlagXML( &labelFlagmedio5, "Pagos5" );
    screenXml->setFlagXML( &labelFlagmedio6, "Pagos6" );
    screenXml->setFlagXML( &labelFlagmedio7, "Pagos7" );
    screenXml->setFlagXML( &labelFlagmedio8, "Pagos8" );
    screenXml->setFlagXML( &labelFlagmedio9, "Pagos9" );
    screenXml->setFlagXML( &labelFlagmedio10, "Pagos10" );
}

void PanelFondoFijo::createGridXML()
{
    screenXml->setGridXML( datos, this, "datos" );
    screenXml->setGridXML( pagos, this, "pagos" );
}


void PanelFondoFijo::setTitotalFfijo( const string &s )
{
    MutexGuiEnter();
    TitotalPpagos->setText( s );
    TitotalPpagos->Draw();
    MutexGuiLeave();
}


void PanelFondoFijo::borrarMedios()
{
    addFlag( " ", " ", T_DATOS_MEDIO_0_BORRAR ); 
    addFlag( " ", " ", T_DATOS_MEDIO_1_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_2_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_3_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_4_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_5_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_6_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_7_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_8_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_9_BORRAR );
    addFlag( " ", " ", T_DATOS_MEDIO_10_BORRAR );
}



void PanelFondoFijo::addFlag( const string &opcion, const string &textIcono, int type )
{
    int a = 0;
    MutexGuiEnter();
    switch( type ) {
        case T_DATOS_MEDIO_1_AGREGAR:
            labelFlagmedio1.setIconText( textIcono );
            labelFlagmedio1.setVisible( true );
            break;
        case T_DATOS_MEDIO_1_BORRAR:
            labelFlagmedio1.setVisible( false );
            break;
        case T_DATOS_MEDIO_2_AGREGAR:
            labelFlagmedio2.setIconText( textIcono );
            labelFlagmedio2.setVisible( true );
            break;
        case T_DATOS_MEDIO_2_BORRAR:
            labelFlagmedio2.setVisible( false );
            break;
        case T_DATOS_MEDIO_3_AGREGAR:
            labelFlagmedio3.setIconText( textIcono );
            labelFlagmedio3.setVisible( true );
            break;
        case T_DATOS_MEDIO_3_BORRAR:
            labelFlagmedio3.setVisible( false );
            break;
        case T_DATOS_MEDIO_4_AGREGAR:
            labelFlagmedio4.setIconText( textIcono );
            labelFlagmedio4.setVisible( true );
            break;
        case T_DATOS_MEDIO_4_BORRAR:
            labelFlagmedio4.setVisible( false );
            break;
        case T_DATOS_MEDIO_5_AGREGAR:
            labelFlagmedio5.setIconText( textIcono );
            labelFlagmedio5.setVisible( true );
            break;
        case T_DATOS_MEDIO_5_BORRAR:
            labelFlagmedio5.setVisible( false );
            break;
        case T_DATOS_MEDIO_6_AGREGAR:
            labelFlagmedio6.setIconText( textIcono );
            labelFlagmedio6.setVisible( true );
            break;
        case T_DATOS_MEDIO_6_BORRAR:
            labelFlagmedio6.setVisible( false );
            break;
        case T_DATOS_MEDIO_7_AGREGAR:
            labelFlagmedio7.setIconText( textIcono );
            labelFlagmedio7.setVisible( true );
            break;
        case T_DATOS_MEDIO_7_BORRAR:
            labelFlagmedio7.setVisible( false );
            break;
        case T_DATOS_MEDIO_8_AGREGAR:
            labelFlagmedio8.setIconText( textIcono );
            labelFlagmedio8.setVisible( true );
            break;
        case T_DATOS_MEDIO_8_BORRAR:
            labelFlagmedio8.setVisible( false );
            break;
        case T_DATOS_MEDIO_9_AGREGAR:
            labelFlagmedio9.setIconText( textIcono );
            labelFlagmedio9.setVisible( true );
            break;
        case T_DATOS_MEDIO_9_BORRAR:
            labelFlagmedio9.setVisible( false );
            break;
        case T_DATOS_MEDIO_10_AGREGAR:
            labelFlagmedio10.setIconText( textIcono );
            labelFlagmedio10.setVisible( true );
            break;
        case T_DATOS_MEDIO_10_BORRAR:
            labelFlagmedio10.setVisible( false );
            break;
    }
    #ifdef INVEL_L
    Refresh( false );
    #endif
    MutexGuiLeave();
}


void PanelFondoFijo::muestraTotal( const string &s )
{
    MutexGuiEnter();
    total->setText( s );
    total->Draw();
    MutexGuiLeave();
}


void PanelFondoFijo::borrarPago()
{
    MutexGuiEnter();
    if( pagos->GetRows() > 0 ) {
        pagos->DeleteRows( pagos->GetRows() - 1, 1 );
    }
    MutexGuiLeave();
}


void PanelFondoFijo::borrarPagos()
{
    MutexGuiEnter();
    if( pagos->GetRows() > 0 ) {
        pagos->DeleteRows( 0, pagos->GetRows() );
    }
    MutexGuiLeave();
}


void PanelFondoFijo::agregarPago( bool aditivo, const string &s1, const string &s2,
                                  const string &s3, const string &s4 )
{
    int i;
    vector<string> row;
    wxString s = s1.c_str();

    MutexGuiEnter();
    row.push_back( s1 );
    row.push_back( s2 );
    row.push_back( s3 );
    row.push_back( s4 );

    if( aditivo ) {
        for( i = 0;i< pagos->GetNumberRows();i++ ) {
            if( pagos->GetCellValue( i, 0 ) == s ) {
                break;
            }
        }
        if( i == pagos->GetNumberRows() ) {
            pagos->AddRow( row );
        }
        else {
            for( int j = 0;j < (int)row.size() && j < (int)pagos->GetNumberCols();j++ ) {
                pagos->SetCellValue( i, j, row[j].c_str() );
            }
        }
    }
    else {
        pagos->AddRow( row );
    }
    // pagos->SetGridCursor(pagos->GetNumberRows(),0);
    MutexGuiLeave();
}


CInput * PanelFondoFijo::getFocus()
{
    return input;
}


void PanelFondoFijo::muestraTitulo( int type, const string &s, const string &s1 )
{
    MutexGuiEnter();
    if( type ) {
        titulo_box->setText( s );
		titulo_box->Draw();
        //titulo_box->setVisible( true );
    }
    else {
        titulo_box->setText( " " );
		titulo_box->Draw();
        //titulo_box->setVisible( false );
    }
    MutexGuiLeave();
}


void PanelFondoFijo::resaltarMedio( int type, const string &s, const string &s1 )
{
    desseleccionaMedio();

    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    switch( type ) {
        case 1:
            labelFlagmedio1.setVisible( false );
            labelFlagmedio1.changeCurrentState( 1 );
            labelFlagmedio1.setVisible( true );
            break;
        case 2:
            labelFlagmedio2.setVisible( false );
            labelFlagmedio2.changeCurrentState( 1 );
            labelFlagmedio2.setVisible( true );
            break;
        case 3:
            labelFlagmedio3.setVisible( false );
            labelFlagmedio3.changeCurrentState( 1 );
            labelFlagmedio3.setVisible( true );
            break;
        case 4:
            labelFlagmedio4.setVisible( false );
            labelFlagmedio4.changeCurrentState( 1 );
            labelFlagmedio4.setVisible( true );
            break;
        case 5:
            labelFlagmedio5.setVisible( false );
            labelFlagmedio5.changeCurrentState( 1 );
            labelFlagmedio5.setVisible( true );
            break;
        case 6:
            labelFlagmedio6.setVisible( false );
            labelFlagmedio6.changeCurrentState( 1 );
            labelFlagmedio6.setVisible( true );
            break;
        case 7:
            labelFlagmedio7.setVisible( false );
            labelFlagmedio7.changeCurrentState( 1 );
            labelFlagmedio7.setVisible( true );
            break;
        case 8:
            labelFlagmedio8.setVisible( false );
            labelFlagmedio8.changeCurrentState( 1 );
            labelFlagmedio8.setVisible( true );
            break;
        case 9:
            labelFlagmedio9.setVisible( false );
            labelFlagmedio9.changeCurrentState( 1 );
            labelFlagmedio9.setVisible( true );
            break;
        case 10:
            labelFlagmedio10.setVisible( false );
            labelFlagmedio10.changeCurrentState( 1 );
            labelFlagmedio10.setVisible( true );
            break;
    }
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}


void PanelFondoFijo::desseleccionaMedio()
{
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    if( labelFlagmedio1.isVisible() ) {
        labelFlagmedio1.setVisible( false );
        labelFlagmedio1.changeCurrentState( 0 );
        labelFlagmedio1.setVisible( true );
    }
    if( labelFlagmedio2.isVisible() ) {
        labelFlagmedio2.setVisible( false );
        labelFlagmedio2.changeCurrentState( 0 );
        labelFlagmedio2.setVisible( true );
    }

    if( labelFlagmedio3.isVisible() ) {
        labelFlagmedio3.setVisible( false );
        labelFlagmedio3.changeCurrentState( 0 );
        labelFlagmedio3.setVisible( true );
    }

    if( labelFlagmedio4.isVisible() ) {
        labelFlagmedio4.setVisible( false );
        labelFlagmedio4.changeCurrentState( 0 );
        labelFlagmedio4.setVisible( true );
    }

    if( labelFlagmedio5.isVisible() ) {
        labelFlagmedio5.setVisible( false );
        labelFlagmedio5.changeCurrentState( 0 );
        labelFlagmedio5.setVisible( true );
    }

    if( labelFlagmedio6.isVisible() ) {
        labelFlagmedio6.setVisible( false );
        labelFlagmedio6.changeCurrentState( 0 );
        labelFlagmedio6.setVisible( true );
    }

    if( labelFlagmedio7.isVisible() ) {
        labelFlagmedio7.setVisible( false );
        labelFlagmedio7.changeCurrentState( 0 );
        labelFlagmedio7.setVisible( true );
    }

    if( labelFlagmedio8.isVisible() ) {
        labelFlagmedio8.setVisible( false );
        labelFlagmedio8.changeCurrentState( 0 );
        labelFlagmedio8.setVisible( true );
    }

    if( labelFlagmedio9.isVisible() ) {
        labelFlagmedio9.setVisible( false );
        labelFlagmedio9.changeCurrentState( 0 );
        labelFlagmedio9.setVisible( true );
    }
    if( labelFlagmedio10.isVisible() ) {
        labelFlagmedio10.setVisible( false );
        labelFlagmedio10.changeCurrentState( 0 );
        labelFlagmedio10.setVisible( true );
    }

    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

