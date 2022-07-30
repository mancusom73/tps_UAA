#include "PanelRetiros.h"
#include "GuiProperties.h"
#include "typeacti.h"
#include "FileVerif.h"
#include "CFontMapper.h"
extern "C"
{
	#include <_cr_var.h>

}
extern CDesktop *properties;
#define CANT_MEDIOS_RETIRO 11

PanelRetiros::PanelRetiros( const wxString &title, const wxPoint &pos, const wxSize &size,
                            long style ) : CPanel( title, pos, size, style )
{
    labelpago = new CLabel();
    total = new CLabel();
	labelGridPagos = new CLabel();
	TitotalPpagos = new CLabel();
    descMedios = new CLabel();
    label_mensaje = new CLabel();
    label_medios = new CLabel();
//    label_titulo = new CLabel();
    titulo_box = new CLabel();
    screenXml = new CScreencfg( "retiros", "default", "default" );
}

void PanelRetiros::init( const string &titulo )
{
	if(ACELERACION_PANTALLAS == 0)	
    	MutexGuiEnter();
   // label_titulo->setText( titulo );
	label_titulo.setText( "" );
    total->setText( "" );
	if(ACELERACION_PANTALLAS == 0)
    	MutexGuiLeave();
    desseleccionaMedio();
    borrarPagos();
}


void PanelRetiros::setTituloRetiros( const string &titulo )
{
    //label_titulo->setText( titulo );
	label_titulo.setText( "" );

}

void PanelRetiros::createLabels()
{
    labelFlagmedio0.setParent( this );
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
    labelpago->setParent( this );
    total->setParent( this );
	TitotalPpagos->setParent( this);
    descMedios->setParent( this );

    label_mensaje->setParent( this );
    label_medios->setParent( this );
    //label_titulo->setParent( this );

	label_titulo.setParent( this );
    titulo_box->setParent( this );
	labelGridPagos->setParent( this );

    this->AddObject( &labelFlagmedio0 );
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
    this->AddObject( labelpago );
    this->AddObject( total );
	this->AddObject( TitotalPpagos );
    this->AddObject( descMedios );
    this->AddObject( label_mensaje );
    this->AddObject( label_medios );
    //this->AddObject( label_titulo );
	this->AddObject( &label_titulo );
    this->AddObject( titulo_box );
	this->AddObject( labelGridPagos );
	

    SetBackGround( screenXml->getBackgroundXML().c_str() );
    createIconXML();
    createGridXML();
    createLabelXML();
    createInputXML();
    createFlagXML();
}

void PanelRetiros::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( total, "TotalPagos" );
    screenXml->setLabelXML( descMedios, "DescMedios" );
    screenXml->setLabelXML( TitotalPpagos, "TitTotalPagos");
    screenXml->setLabelXML( label_medios, "LabelMedios" );
    label_medios->setVisible( true );
    screenXml->setLabelXML( titulo_box, "TituloBox" );
    //screenXml->setLabelXML( label_titulo, "ModoOperacionTexto" );
	//    label_titulo->setVisible( true );
	screenXml->setLabelXML( labelGridPagos, "TitGridPago" );
	labelGridPagos->setVisible( true );
}
void PanelRetiros::createIconXML()
{
    FileVerif verif;
    screenXml->setIconXML( &label_titulo, "ModoRetiro" );
    label_titulo.setVisible( true );
    
}

void PanelRetiros::createInputXML()
{
    screenXml->setInputXML( input, this, "Importe" );
}

void PanelRetiros::createFlagXML()
{
    screenXml->setFlagXML( &labelFlagmedio0, "Pagos0" );
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

void PanelRetiros::createGridXML()
{
    screenXml->setGridXML( datos, this, "datos" );
    screenXml->setGridXML( pagos, this, "pagos" );
}

void PanelRetiros::setTitotalRetiros( const string &s )
{
    MutexGuiEnter();
    TitotalPpagos->setText(s);
    TitotalPpagos->Draw();
    MutexGuiLeave();
}


void PanelRetiros::borrarMedios()
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



void PanelRetiros::addFlag( const string &opcion, const string &textIcono, int type )
{
    int a = 0;
    MutexGuiEnter();
    switch( type ) {
        case T_DATOS_MEDIO_0_AGREGAR:
            labelFlagmedio0.setIconText( textIcono );
            labelFlagmedio0.setVisible( true );
            break;
        case T_DATOS_MEDIO_0_BORRAR:
            labelFlagmedio0.setVisible( false );
            break;
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
    //Refresh();
    MutexGuiLeave();
}

void PanelRetiros::muestraTotal( const string &s )
{
    MutexGuiEnter();
    total->setText( s );
    total->Draw();
    total->setVisible( true );
    MutexGuiLeave();
}


void PanelRetiros::borrarPago()
{
    MutexGuiEnter();
    if( pagos->GetRows() > 0 ) {
        pagos->DeleteRows( pagos->GetRows() - 1, 1 );
    }
    MutexGuiLeave();
}


void PanelRetiros::borrarPagos()
{
    MutexGuiEnter();
    if( pagos->GetRows() > 0 ) {
        pagos->DeleteRows( 0, pagos->GetRows() );
    }
    MutexGuiLeave();
}


void PanelRetiros::agregarPago( bool aditivo, const string &s1, const string &s2, const string &s3,
                                const string &s4 )
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
    //    pagos->SetGridCursor(pagos->GetNumberRows(),0);
    MutexGuiLeave();
}


CInput * PanelRetiros::getFocus()
{
    return input;
}


void PanelRetiros::muestraTitulo( int type, const string &s, const string &s1 )
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


void PanelRetiros::resaltarMedio( int type, const string &s, const string &s1 )
{
    desseleccionaMedio();
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    switch( type ) {
        case 11:
            labelFlagmedio0.setVisible( false );
            labelFlagmedio0.changeCurrentState( 1 );
            labelFlagmedio0.setVisible( true );
            break;
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
    Refresh();
    MutexGuiLeave();
    //#endif
}


void PanelRetiros::desseleccionaMedio()
{
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    if( labelFlagmedio0.isVisible() ) {
        labelFlagmedio0.setVisible( false );
        labelFlagmedio0.changeCurrentState( 0 );
        labelFlagmedio0.setVisible( true );
    }
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
    //Refresh();
    MutexGuiLeave();
    //#endif
}

