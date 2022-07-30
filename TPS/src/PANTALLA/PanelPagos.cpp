#include "PanelPagos.h"
#include "GuiProperties.h"
#include "typeacti.h"
#include "CFontMapper.h"
#include "FileVerif.h"
#include "bridge.h"
#include <cr.h>
// 27/06/07 FR pragma Notar que ahora queda el pack en 8 a diferencia del 1 como era antes del mecanismo de pila en cr.h
extern "C"{
#include <cmemory.h>
}
extern CDesktop *properties;

PanelPagos::PanelPagos( const wxString &title, const wxPoint &pos, const wxSize &size, long style ) 
: CPanel( title, pos, size, style )
{
    label_mensaje = new CLabel();
    total = new CLabel();
	labelGridPagos = new CLabel();
    descResto = new CLabel();
	descResto2 = new CLabel();
    titResto = new CLabel();
    descMedios = new CLabel();
    TitotalPpagos = new CLabel();
    label_medios = new CLabel();
    titulo_box = new CLabel();
    labelpago = new CLabel();
    ImporteTemporal = new CLabel();
    //labelFlagTotAnt = new CLabel();
    screenXml = new CScreencfg( "pagos", "default", "default" );
}

void PanelPagos::init( const string &titulo )
{
    int cant = 0;

    //#ifdef INVEL_L
	if( ACELERACION_PANTALLAS == NO)
		MutexGuiEnter();
   // #endif
    //label_titulo.setText(titulo);
    label_titulo.setText( "" );
    titulo_box->setVisible( false );
    cant = datos->GetNumberRows();
    
    
    if( cant > 0 ) {
        if( labelFlagRetDgr.isVisible() ) {
            labelFlagRetDgr.setVisible( false );
        }
        if( labelFlagPerDgi.isVisible() ) {
            labelFlagPerDgi.setVisible( false );
        }
        if( labelFlagPer212.isVisible() ) {
            labelFlagPer212.setVisible( false );
        }
        datosIndex.clear();
		if( ACELERACION_PANTALLAS == SI) {
			mYield();
			SLEEP(10); //esperemos que alcance este sleep
		} 
        datos->DeleteRows( 0, cant );
    }

    if( textomodo[0] != "" ) {
        label_titulo.setVisible( false );
        label_titulo.setBitmap( wxString( textomodo[0].c_str() ), true );
        label_titulo.setVisible( true );
    }

    input->SetText( " ", 0 );
    if( ACELERACION_PANTALLAS == NO)
		MutexGuiLeave();
    muestraTitulo( 0, " ", " " );
    desseleccionaMedio();
    borrarPagos();
}

void PanelPagos::createLabels()
{
    labelFlagClie.setParent( this );
    labelFlagVend.setParent( this );
    labelFlagAfinidad.setParent( this );
    labelFlagMutual.setParent( this );
    labelFlagDetList.setParent( this );
    labelFlagPerfil.setParent( this );
    labelFlagInt.setParent( this );
    labelFlagDisciva.setParent( this );
    labelFlagAutoriza.setParent( this );
    labelFlagBono.setParent( this );
    labelFlagNroFac.setParent( this );
    labelFlagEnvOffline.setParent( this );
    labelFlagRetDgr.setParent( this );
    labelFlagTotAnt.setParent( this );
    labelFlagPerDgi.setParent( this );
    labelFlagPer212.setParent( this );
    labelFlagDiferimiento.setParent( this );
    labelFlagCupo.setParent( this );
    labelFlagTarj.setParent( this );
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
    descResto->setParent( this );
	descResto2->setParent( this );
    titResto->setParent( this );
    descMedios->setParent( this );
    TitotalPpagos->setParent( this );
    label_mensaje->setParent( this );
    label_medios->setParent( this );
    label_titulo.setParent( this );
    titulo_box->setParent( this );
    labelFlagMonExtr.setParent( this );
    LogoInvel.setParent( this );
	labelGridPagos->setParent( this );
    ImporteTemporal->setParent( this );

    this->AddObject( &labelFlagClie );
    this->AddObject( &labelFlagVend );
    this->AddObject( &labelFlagAfinidad );
    this->AddObject( &labelFlagMutual );
    this->AddObject( &labelFlagDetList );
    this->AddObject( &labelFlagPerfil );
    this->AddObject( &labelFlagInt );
    this->AddObject( &labelFlagDisciva );
    this->AddObject( &labelFlagAutoriza );
    this->AddObject( &labelFlagBono );
    this->AddObject( &labelFlagNroFac );
    this->AddObject( &labelFlagEnvOffline );
    this->AddObject( &labelFlagRetDgr );
    this->AddObject( &labelFlagTotAnt );
    this->AddObject( &labelFlagPerDgi );
    this->AddObject( &labelFlagPer212 );
    this->AddObject( &labelFlagDiferimiento );
    this->AddObject( &labelFlagCupo );
    this->AddObject( &labelFlagTarj );
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
    this->AddObject( descResto );
	this->AddObject( descResto2 );
    this->AddObject( titResto );
    this->AddObject( descMedios );
    this->AddObject( TitotalPpagos );
    this->AddObject( label_mensaje );
    this->AddObject( label_medios );
    this->AddObject( &label_titulo );
    this->AddObject( titulo_box );
    this->AddObject( &labelFlagMonExtr );
    this->AddObject( &LogoInvel );
	this->AddObject( labelGridPagos );
    this->AddObject( ImporteTemporal );

    backgroundLocation = screenXml->getBackgroundXML();
    setNormalBackground();
    //SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();
    createInputXML();
    createIconXML();
    createFlagXML();
}

void PanelPagos::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( total, "TotalPagos" );
    screenXml->setLabelXML( descResto, "DescResto" );   
	screenXml->setLabelXML( descResto2, "DescResto2" );
    screenXml->setLabelXML( titResto, "TitResto" );
    //screenXml->setLabelXML(total, "TotalPagos");
    screenXml->setLabelXML( descMedios, "DescMedios" );
    screenXml->setLabelXML( TitotalPpagos, "TitTotalPagos" );
    screenXml->setLabelXML( label_medios, "LabelMedios" );
    label_medios->setVisible( true );
    screenXml->setLabelXML( titulo_box, "TituloBox" );
    screenXml->setLabelXML( ImporteTemporal, "ImporteTemporal" );
	screenXml->setLabelXML( labelGridPagos, "TitGridPago" );
	labelGridPagos->setVisible( true );
}

void PanelPagos::createInputXML()
{
    screenXml->setInputXML( input, this, "Importe" );
}

void PanelPagos::createIconXML()
{
    FileVerif verif;
    screenXml->setIconXML( &label_titulo, "ModoPago" );
    label_titulo.setVisible( true );
    screenXml->setIconXML( &LogoInvel, "LogoInvel" );
//    verif.VerifLogo( true, true, "default", "pagos" );
    LogoInvel.setVisible( false );
}

void PanelPagos::createFlagXML()
{
    screenXml->setFlagXML( &labelFlagClie, "FlagCliente", datos );
    screenXml->setFlagXML( &labelFlagVend, "FlagVend", datos );
    screenXml->setFlagXML( &labelFlagAfinidad, "FlagAfinidad", datos );
    screenXml->setFlagXML( &labelFlagMutual, "FlagMutual", datos );
    screenXml->setFlagXML( &labelFlagDetList, "FlagDetList", datos );
    screenXml->setFlagXML( &labelFlagPerfil, "FlagPerfil", datos );
    screenXml->setFlagXML( &labelFlagInt, "FlagIntereses", datos );
    screenXml->setFlagXML( &labelFlagDisciva, "FlagDiscIva", datos );
    screenXml->setFlagXML( &labelFlagAutoriza, "FlagAutoriza", datos );
    screenXml->setFlagXML( &labelFlagBono, "FlagBono", datos );
    screenXml->setFlagXML( &labelFlagNroFac, "FlagNroFac", datos );
    screenXml->setFlagXML( &labelFlagEnvOffline, "FlagEnvOffline", datos );
    screenXml->setFlagXML( &labelFlagRetDgr, "FlagRetDgr", datos );
    screenXml->setFlagXML( &labelFlagTotAnt, "FlagTotAnt", datos );
    screenXml->setFlagXML( &labelFlagPerDgi, "FlagPerDgi", datos );
    screenXml->setFlagXML( &labelFlagPer212, "FlagPer212", datos );
    screenXml->setFlagXML( &labelFlagDiferimiento, "FlagDev", datos );
    screenXml->setFlagXML( &labelFlagCupo, "FlagCupo", datos );
    screenXml->setFlagXML( &labelFlagTarj, "FlagTarj", datos );
    screenXml->setFlagXML( &labelFlagMonExtr, "FlagMonExtr", datos );

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

void PanelPagos::createGridXML()
{
    screenXml->setGridXML( datos, this, "datos" );
    screenXml->setGridXML( pagos, this, "pagos" );
}

void PanelPagos::setNormalBackground() 
{
    SetBackGround( backgroundLocation.c_str() );
    Refresh();
}

void PanelPagos::setErrorBackground() 
{
    int pos;
    string backErr( backgroundLocation ); //copiamos
    string extra = "_off.";
    
    pos = backErr.find_last_of('.');
    backErr.replace(pos, 1, extra);

    if( wxFileExists( backErr.c_str() ) ) {
        SetBackGround( backErr.c_str() );
        Refresh();
    }
}
 
void PanelPagos::setTitotalPpagos( const string &s )
{
    MutexGuiEnter();
    TitotalPpagos->setText( s );
    TitotalPpagos->Draw();
    //Refresh();
    MutexGuiLeave();
}


void PanelPagos::borrarMedios()
{
    addFlag( "		  ", " ", T_DATOS_MEDIO_1_BORRAR );
    addFlag( "		  ", " ", T_DATOS_MEDIO_2_BORRAR );
    addFlag( "		  ", " ", T_DATOS_MEDIO_3_BORRAR );
    addFlag( "		  ", " ", T_DATOS_MEDIO_4_BORRAR );
    addFlag( "		  ", " ", T_DATOS_MEDIO_5_BORRAR );
    addFlag( "        ", " ", T_DATOS_MEDIO_6_BORRAR );
    addFlag( "        ", " ", T_DATOS_MEDIO_7_BORRAR );
    addFlag( "        ", " ", T_DATOS_MEDIO_8_BORRAR );
    addFlag( "        ", " ", T_DATOS_MEDIO_9_BORRAR );
}


void PanelPagos::addFlag( const string &opcion, const string &textIcono, int type )
{
    int a = 0;
    MutexGuiEnter();
    switch( type ) {
        case T_DATOS_CLIENTE_AGREGAR:
            labelFlagClie.setText( opcion );
            labelFlagClie.setVisible( true );
            break;
        case T_DATOS_CLIENTE_BORRAR:
            labelFlagClie.setVisible( false );
            break;
        case T_DATOS_VENDEDOR_AGREGAR:
            labelFlagVend.setText( opcion );
            labelFlagVend.setVisible( true );
            break;
        case T_DATOS_VENDEDOR_BORRAR:
            labelFlagVend.setVisible( false );
            break;
        case T_DATOS_AFINIDAD_AGREGAR:
            labelFlagAfinidad.setText( opcion );
            labelFlagAfinidad.setVisible( true );
            break;
        case T_DATOS_AFINIDAD_BORRAR:
            labelFlagAfinidad.setVisible( false );
            break;
        case T_DATOS_MUTUAL_AGREGAR:
            labelFlagMutual.setText( opcion );
            labelFlagMutual.setVisible( true );
            break;
        case T_DATOS_MUTUAL_BORRAR:
            labelFlagMutual.setVisible( false );
            break;
        case T_DATOS_DET_LIST_AGREGAR:
            labelFlagDetList.setText( opcion );
            labelFlagDetList.setVisible( true );
            break;
        case T_DATOS_DET_LIST_BORRAR:
            labelFlagDetList.setText( opcion );
            break;
        case T_DATOS_PERFIL_AGREGAR:
            labelFlagPerfil.setText( opcion );
            labelFlagPerfil.setVisible( true );
            break;
        case T_DATOS_PERFIL_BORRAR:
            labelFlagPerfil.setVisible( false );
            break;
        case T_DATOS_INTERES_AGREGAR:
            labelFlagInt.setText( opcion );
            labelFlagInt.setVisible( true );
            break;
        case T_DATOS_INTERES_BORRAR:
            labelFlagInt.setVisible( false );
            break;
        case T_DATOS_DISC_IVA_AGREGAR:
            labelFlagDisciva.setText( opcion );
            labelFlagDisciva.setVisible( true );
            break;
        case T_DATOS_DISC_IVA_BORRAR:
            labelFlagDisciva.setVisible( false );
            break;
        case T_DATOS_AUTORIZACION_AGREGAR:
            labelFlagAutoriza.setText( opcion );
            labelFlagAutoriza.setVisible( true );
            break;
        case T_DATOS_AUTORIZACION_BORRAR:
            labelFlagAutoriza.setVisible( false );
            break;
        case T_DATOS_BONOS_AGREGAR:
            labelFlagBono.setText( opcion );
            labelFlagBono.setVisible( true );
            break;
        case T_DATOS_BONOS_BORRAR:
            labelFlagBono.setVisible( false );
            break;
        case T_DATOS_NRO_FACTURA_AGREGAR:
            labelFlagNroFac.setText( opcion );
            //Temporalmente no se muestra el icono
            //labelFlagNroFac.setVisible( true );
            break;
        case T_DATOS_NRO_FACTURA_BORRAR:
            labelFlagNroFac.setVisible( false );
            break;
        case T_DATOS_ENV_OFFLINE_AGREGAR:
            labelFlagEnvOffline.setText( opcion );
            labelFlagEnvOffline.setVisible( true );
            break;
        case T_DATOS_ENV_OFFLINE_BORRAR:
            labelFlagEnvOffline.setVisible( false );
            break;
        case T_DATOS_RETENCION_DGR_AGREGAR_PAGO:
            labelFlagRetDgr.setText( opcion );
			labelFlagRetDgr.setVisible( true );
            break;
        case T_DATOS_RETENCION_DGR_BORRAR_PAGO:
            labelFlagRetDgr.setVisible( false );
            break;
        case T_DATOS_TOTAL_ANT_AGREGAR:
            labelFlagTotAnt.setText( opcion );
            labelFlagTotAnt.setVisible( true );
            break;
        case T_DATOS_TOTAL_ANT_BORRAR:
            labelFlagTotAnt.setVisible( false );
            break;
        case T_DATOS_PERCEPCION_DGI_AGREGAR:
            labelFlagPerDgi.setText( opcion );
            labelFlagPerDgi.setVisible( true );
            break;
        case T_DATOS_PERCEPCION_DGI_BORRAR:
            labelFlagPerDgi.setVisible( false );
            break;
        case T_DATOS_PERCEPCION_212_AGREGAR:
            labelFlagPer212.setText( opcion );
            labelFlagPer212.setVisible( true );
            break;
        case T_DATOS_PERCEPCION_212_BORRAR:
            labelFlagPer212.setVisible( false );
            break;
        case T_DATOS_DIFERIMIENTO_AGREGAR:
            labelFlagDiferimiento.setText( opcion );
            labelFlagDiferimiento.setVisible( true );
            break;
        case T_DATOS_DIFERIMIENTO_BORRAR:
            labelFlagDiferimiento.setVisible( false );
            break;
        case T_DATOS_SALDO_AGREGAR:
        case T_DATOS_CUPO_CHEQUE_AGREGAR:
            labelFlagCupo.setText( opcion );
            labelFlagCupo.setVisible( true );
            break;
        case T_DATOS_SALDO_BORRAR:
        case T_DATOS_CUPO_CHEQUE_BORRAR:
            labelFlagCupo.setVisible( false );
            break;
        case T_DATOS_TARJETA_AGREGAR:
            labelFlagTarj.setText( opcion );
            labelFlagTarj.setVisible( true );
            break;
        case T_DATOS_TARJETA_BORRAR:
            labelFlagTarj.setVisible( false );
            break;
        case T_DATOS_MEDIO_1_AGREGAR:
            labelFlagmedio1.setIconText( textIcono );
            labelFlagmedio1.setVisible( true );
            break;
        case T_DATOS_MEDIO_1_BORRAR:
			labelFlagmedio1.setIconText( textIcono );
            labelFlagmedio1.setVisible( false );
            break;
        case T_DATOS_MEDIO_2_AGREGAR:
            labelFlagmedio2.setIconText( textIcono );
            labelFlagmedio2.setVisible( true );
            break;
        case T_DATOS_MEDIO_2_BORRAR:
			labelFlagmedio2.setIconText( textIcono );
            labelFlagmedio2.setVisible( false );
            break;
        case T_DATOS_MEDIO_3_AGREGAR:
            labelFlagmedio3.setIconText( textIcono );
            labelFlagmedio3.setVisible( true );
            break;
        case T_DATOS_MEDIO_3_BORRAR:
			labelFlagmedio3.setIconText( textIcono );
            labelFlagmedio3.setVisible( false );
            break;
        case T_DATOS_MEDIO_4_AGREGAR:
            labelFlagmedio4.setIconText( textIcono );
            labelFlagmedio4.setVisible( true );
            break;
        case T_DATOS_MEDIO_4_BORRAR:
			labelFlagmedio4.setIconText( textIcono );
            labelFlagmedio4.setVisible( false );
            break;
        case T_DATOS_MEDIO_5_AGREGAR:
            labelFlagmedio5.setIconText( textIcono );
            labelFlagmedio5.setVisible( true );
            break;
        case T_DATOS_MEDIO_5_BORRAR:
			labelFlagmedio5.setIconText( textIcono );
            labelFlagmedio5.setVisible( false );
            break;
        case T_DATOS_MEDIO_6_AGREGAR:
            labelFlagmedio6.setIconText( textIcono );
            labelFlagmedio6.setVisible( true );
            break;
        case T_DATOS_MEDIO_6_BORRAR:
			labelFlagmedio6.setIconText( textIcono );
            labelFlagmedio6.setVisible( false );
            break;
        case T_DATOS_MEDIO_7_AGREGAR:
            labelFlagmedio7.setIconText( textIcono );
            labelFlagmedio7.setVisible( true );
            break;
        case T_DATOS_MEDIO_7_BORRAR:
			labelFlagmedio7.setIconText( textIcono );
            labelFlagmedio7.setVisible( false );
            break;
        case T_DATOS_MEDIO_8_AGREGAR:
            labelFlagmedio8.setIconText( textIcono );
            labelFlagmedio8.setVisible( true );
            break;
        case T_DATOS_MEDIO_8_BORRAR:
			labelFlagmedio8.setIconText( textIcono );
            labelFlagmedio8.setVisible( false );
            break;
        case T_DATOS_MEDIO_9_AGREGAR:
            labelFlagmedio9.setIconText( textIcono );
            labelFlagmedio9.setVisible( true );
            break;
        case T_DATOS_MEDIO_9_BORRAR:
			labelFlagmedio9.setIconText( textIcono );
            labelFlagmedio9.setVisible( false );
            break;
        case T_DATOS_MEDIO_10_AGREGAR:
            labelFlagmedio10.setIconText( textIcono );
            labelFlagmedio10.setVisible( true );
            break;
        case T_DATOS_MEDIO_10_BORRAR:
			labelFlagmedio10.setIconText( textIcono );
            labelFlagmedio10.setVisible( false );
            break;
        case T_FLAGS_MEDIO_EXTR_AGREGAR:
            if( MODO_NEGOCIO == RETAIL ) {
                labelFlagMonExtr.setText( opcion );
                labelFlagMonExtr.setVisible( true );
            }
            else {
                addDatosRow( opcion );
            }
            break;
        case T_FLAGS_MEDIO_EXTR_BORRAR:
            if( MODO_NEGOCIO == RETAIL ) {
                labelFlagMonExtr.setVisible( false );
            }
            else {
                delDatosRow();
            }   
            break;
    }
    if( MODO_NEGOCIO == RETAIL ) {
        //Refresh();
    }
    MutexGuiLeave();
}


void PanelPagos::muestraTotal( const string &s )
{
    MutexGuiEnter();
    total->setText( s );
    total->Draw();
    //Refresh();
    MutexGuiLeave();
}


void PanelPagos::setResto( const string &tit, const string &imp, const string &imp1)
{
    MutexGuiEnter();
    //titResto
    titResto->setText( tit );
    titResto->Draw();
    descResto->setText( imp );
    descResto->Draw();
	descResto2->setText( imp1 );
    descResto2->Draw();
    //Refresh();
    MutexGuiLeave();
}


void PanelPagos::borrarPago()
{
    int c;
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    c = pagos->GetNumberRows();
    if( c > 0 ) {
		pagos->Refresh();
        pagos->DeleteRows( c - 1 );
    }
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}


void PanelPagos::borrarPagos()
{
    //#ifdef INVEL_L
	if( ACELERACION_PANTALLAS == NO)//truchada
    MutexGuiEnter();
    //#endif
    if( pagos->GetNumberRows() > 0 ) {
        pagos->DeleteRows( 0, pagos->GetNumberRows() );
        //Refresh();
    }
    //#ifdef INVEL_L
	if( ACELERACION_PANTALLAS == NO)//truchada
    MutexGuiLeave();
    //#endif
}


void PanelPagos::agregarPago( bool aditivo, const string &s1, const string &s2, const string &s3,
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
    pagos->SetGridCursor( pagos->GetNumberRows() - 1, 0 );
    #ifdef INVEL_L
    pagos->Scroll( 0, pagos->GetNumberRows() );
    #else
    pagos->ScrollLines( pagos->GetRows() );
    #endif
    MutexGuiLeave();
    muestraTitulo( 0, " ", " " );
    desseleccionaMedio();
}


void PanelPagos::muestraTitulo( int type, const string &s, const string &s1 )
{
    if( ACELERACION_PANTALLAS == NO)//truchada
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
    if( ACELERACION_PANTALLAS == NO)//truchada
    MutexGuiLeave();
}


void PanelPagos::resaltarMedio( int type, const string &s, const string &s1 )
{
    //desseleccionaMedio();
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


void PanelPagos::desseleccionaMedio()
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
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelPagos::addDatosRow( const string &s1 )
{
    vector<string> row;

    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif

    row.push_back( s1 );
    datos->AddRow( row );
    datosIndex.push_back( datos->GetNumberRows() - 1 );

    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

int PanelPagos::GetNumberRows()
{
	return datos->GetNumberRows();
}

void PanelPagos::delDatosRow()
{
    int i;
    //#ifdef INVEL_L   
    MutexGuiEnter();
    //#endif
    if( datosIndex.size() > 0 ) {
        i = datosIndex[datosIndex.size() - 1];
        datosIndex.pop_back();
        datos->DeleteRows( i );
        //datos->Refresh();
    }
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif INVEL_L
}

void PanelPagos::refreshGrids()
{
    #ifdef INVEL_L   
    MutexGuiEnter();
    #endif
    //pagos->Refresh();
    //datos->Refresh();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif INVEL_L
}
void PanelPagos::setImporteTemporal( const string &s )
{
    MutexGuiEnter();
    ImporteTemporal->setText( s );
    ImporteTemporal->Draw();
    MutexGuiLeave();
}

void PanelPagos::SetFocus( )
/*Esta funcion pone al tope de la lista de ventanas a la ventana .
Por eso la llamo setfocus. Hay que tener cuidado que no haya otra ventana arriba porque le cambiara el foco
*/
{ 
	this->Raise();
}

bool PanelPagos::isCashActivo( )
{
	if(labelFlagMonExtr.isVisible()){
		return TRUE;
	}
	return FALSE;
}

 
    
