#include "PanelVentas.h"
#include "actions.h"
#include "typeacti.h"
#include "GuiProperties.h"
#include <cr.h>
#include "path.h"

static wxMutex s_mutexHoraVenta;
extern "C"{
#include <cmemory.h>
}
PanelVentas::PanelVentas( const wxString &title, const wxPoint &pos, const wxSize &size,
                          long style ) : CPanel( title, pos, size, style )
{
    label_mensaje = new CLabel();
    labelTicket = new CLabel();
	labelGridVentas = new CLabel();
    Detalle = new CLabel();
    Total = new CLabel();
    Bultos = new CLabel();
    IBultos = new CLabel();
    Descripcion = new CLabel();
    Cantidad = new CLabel();
    Subtotal = new CLabel();
    Multiplica = new CLabel();
    nCajero = new CLabel();
    nroCaja = new CLabel();
    modoEjecucion = new CLabel();
    TitHora = new CLabel();
    TitFecha = new CLabel();

    TitFechaOp = new CLabel();
    FechaLocal = new CLabel();

    TipoInventario = new CLabel();

    Fecha = new CLabel();
    screenXml = new CScreencfg( "ventas", "default", "default" );
	CantConsecutivos = new CLabel();
	CantIdenticos = new CLabel();
	DescContador = new CLabel();
}


void PanelVentas::createLabels()
{
	labelFlagMonedero.setParent( this );
	labelFlagCambio.setParent( this );
    labelFlagFac.setParent( this );
	labelFlagTf.setParent( this );
    labelFlagNc.setParent( this ); 
    labelFlagCr.setParent( this ); 
    labelFlagBarr.setParent( this ); 
    labelFlagTke.setParent( this ); 
    labelFlagClie.setParent( this ); 
    labelFlagTarj.setParent( this ); 
    labelFlagVend.setParent( this ); 
    labelFlagDesc.setParent( this ); 
    labelFlagAfinidad.setParent( this ); 
    labelFlagPeticion.setParent( this ); 
    labelFlagPresentacion.setParent( this ); 
    labelFlagMutual.setParent( this ); 
    labelFlagDevolucion.setParent( this ); 
    labelFlagCambio.setParent( this ); 
    labelFlagBienUso.setParent( this ); 
    labelFlagDev.setParent( this ); 
    labelFlagDevItem.setParent( this );
    labelFlagEntrenamiento.setParent( this ); 
	labelFlagModInventario.setParent( this );
    labelFlagTipoInventario.setParent( this );
    labelFlagDetList.setParent( this ); 
    labelFlagPerfil.setParent( this ); 
    labelFlagMozo.setParent( this ); 
    labelFlagCupo.setParent( this ); 
    TitBultos.setParent( this );    
    TitRenglones.setParent( this ); //mmancuso
    Modo.setParent( this ); 
    labelFlagErrorRed.setParent( this ); 
    labelArt.setParent( this );
	fotoCajero.setParent( this);
    leyendasubtotal.setParent( this ); 
    Detalle->setParent( this ); 
    Total->setParent( this ); 
    Bultos->setParent( this );
    IBultos->setParent( this );
    Descripcion->setParent( this ); 
    Cantidad->setParent( this ); 
    Subtotal->setParent( this ); 
    Multiplica->setParent( this ); 
    nCajero->setParent( this ); 
    nroCaja->setParent( this ); 
    modoEjecucion->setParent( this ); 
    label_mensaje->setParent( this ); 
    labelTicket->setParent( this );
	labelGridVentas->setParent( this );
    tEspera_remoto.setParent( this ); 
    TitHora->setParent( this );
    TitFecha->setParent( this );
    Fecha->setParent( this );
    TipoInventario->setParent( this );

    TitFechaOp->setParent( this );
    FechaLocal->setParent( this );

	CantConsecutivos->setParent( this );
	CantIdenticos->setParent( this );
	DescContador->setParent( this );
    labelFlagRetDgr.setParent( this );

	this->AddObject( &labelFlagFac );
	this->AddObject( &labelFlagTf );
    this->AddObject( &labelFlagNc ); 
    this->AddObject( &labelFlagCr ); 
	this->AddObject( &labelFlagMonedero );
    this->AddObject( &labelFlagBarr ); 
    this->AddObject( &labelFlagTke ); 
    this->AddObject( &labelFlagClie ); 
    this->AddObject( &labelFlagTarj ); 
    this->AddObject( &labelFlagVend );
    this->AddObject( &tEspera_remoto );

    this->AddObject( &labelFlagDesc ); 

    this->AddObject( &labelFlagDev ); 
    this->AddObject( &labelFlagDevItem ); 

    this->AddObject( &labelFlagAfinidad ); 
    this->AddObject( &labelFlagPeticion ); 
    this->AddObject( &labelFlagPresentacion ); 
    this->AddObject( &labelFlagMutual ); 
    this->AddObject( &labelFlagDevolucion ); 
    this->AddObject( &labelFlagCambio ); 
    this->AddObject( &labelFlagBienUso ); 

    this->AddObject( &labelFlagEntrenamiento ); 
	this->AddObject( &labelFlagModInventario ); 
    this->AddObject( &labelFlagTipoInventario );
    this->AddObject( &labelFlagDetList ); 
    this->AddObject( &labelFlagPerfil ); 
    this->AddObject( &labelFlagMozo ); 
    this->AddObject( &labelFlagCupo ); 
    this->AddObject( &TitBultos );    
    this->AddObject( &TitRenglones ); //mmancuso
    this->AddObject( &Modo ); 
    this->AddObject( &labelFlagErrorRed ); 
    this->AddObject( &labelArt );
	this->AddObject( &fotoCajero );
    this->AddObject( &leyendasubtotal ); 
    //this->AddObject( &labelFlagCambio );
    
    this->AddObject( Detalle ); 
    this->AddObject( Total ); 
    this->AddObject( Bultos );
    this->AddObject( IBultos );
    this->AddObject( Descripcion ); 
    this->AddObject( Cantidad ); 
    this->AddObject( Subtotal ); 
    this->AddObject( Multiplica ); 
    this->AddObject( nCajero ); 
    this->AddObject( nroCaja ); 
    this->AddObject( modoEjecucion ); 
    this->AddObject( label_mensaje ); 
    this->AddObject( labelTicket ); 
    this->AddObject( labelGridVentas );
    this->AddObject( TitHora );
    this->AddObject( TipoInventario );
    this->AddObject( TitFecha );
    this->AddObject( Fecha );

    this->AddObject( TitFechaOp );
    this->AddObject( FechaLocal );

	this->AddObject( CantConsecutivos );
	this->AddObject( CantIdenticos );
	this->AddObject( DescContador );
    this->AddObject( &labelFlagRetDgr );

    backgroundLocation = screenXml->getBackgroundXML();
    setNormalBackground();
    //SetBackGround( screenXml->getBackgroundXML().c_str() );

    createGridXML();
    createLabelXML();
    createInputXML();
    createIconXML();
    createFlagXML();
}

void PanelVentas::init()
{
    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
	labelArt.setVisible( false );
    if( RAM_TIPO_INVENTARIO == 0 ) {
      TitRenglones.setVisible( false ); //mmancuso
      TitRenglones.setText( " " );
    } else {
       TitBultos.setVisible( false ); //mmancuso
       TitBultos.setText( " " );
     }

    if( ticket->GetRows() > 0 ) {
        ticket->DeleteRows( 0, ticket->GetRows(), TRUE );
    }
    Total->setVisible( false );
    Total->setText( "" );
    Total->setVisible( true );
    //#ifdef INVEL_L
    MutexGuiLeave();
    //#endif
}

void PanelVentas::createInputXML()
{
    screenXml->setInputXML( input_barra, this, "CodBarra" );
}

void PanelVentas::createIconXML()
{
    string aux;
	
	screenXml->setIconXML( &labelFlagCr, "FlagCr" );
	screenXml->setIconXML( &labelFlagBarr, "CodBarr" );
    screenXml->setIconXML( &tEspera_remoto, "tEspera_remoto" );
    screenXml->setIconXML( &TitBultos, "TitBultos" );
    screenXml->setIconXML( &TitRenglones, "TitRenglones" );
    if( RAM_TIPO_INVENTARIO == 0 ) {
      TitRenglones.setVisible( false ); //mmancuso
      TitBultos.setVisible( true ); //mmancuso
      TitBultos.setText( "" );
    } else {
       TitBultos.setVisible( false ); //mmancuso
       TitRenglones.setVisible( true ); //mmancuso
       TitBultos.setText( " " );
     }
    screenXml->setIconXML( &leyendasubtotal, "LeyendaSubtotal" );
    mostrar_flag_articulo = 1;
    screenXml->setIconXML( &labelArt, "Articulo" );
    screenXml->setIconXML( &fotoCajero, "FotoCaj" );

    screenXml->setIconXML(&Modo,"ModoOperacion");  
    aux = screenXml->getImage( "modoventa" );
    textomodo[OPERACION] = aux;
    aux = screenXml->getImage( "modoentrenamiento" );
    textomodo[ENTRENAMIENTO] = aux;
    aux = screenXml->getImage( "flaginventario" );
    textomodo[INVENTARIO] = aux;
    aux = screenXml->getImage( "modoetiquetas" );
    textomodo[ETIQUETAS] = aux;
    aux = screenXml->getImage( "modotransfere" );
    textomodo[TRANSFERENCIAS] = aux;

    
}

void PanelVentas::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( nCajero, "NCajero" );
    screenXml->setLabelXML( nroCaja, "NroCaja" );
    screenXml->setLabelXML( Detalle, "Detalle" );
    screenXml->setLabelXML( Bultos, "Bultos" );
    screenXml->setLabelXML( IBultos, "IBultos" );
    screenXml->setLabelXML( Total, "Total" );
    screenXml->setLabelXML( Descripcion, "Descripcion" );
    screenXml->setLabelXML( Cantidad, "Cantidad" );
    screenXml->setLabelXML( Subtotal, "Subtotal" );
    screenXml->setLabelXML( Multiplica, "Multiplica" );
    screenXml->setLabelXML( labelTicket, "TitTicket" );
	screenXml->setLabelXML( labelGridVentas, "TitGridVenta" );
    screenXml->setLabelXML( TitFecha, "TitFecha" );
	screenXml->setLabelXML( FechaLocal, "FechaLocal" );
	screenXml->setLabelXML( TitFechaOp, "TitFechaOperativa" );
    screenXml->setLabelXML( Fecha, "FechaOperacion" );
	screenXml->setLabelXML( TitHora, "TitHora" );
	screenXml->setLabelXML( TipoInventario, "TipoInventario" );
	screenXml->setLabelXML( CantConsecutivos, "CantConsecutivos" );
	screenXml->setLabelXML( CantIdenticos, "CantIdenticos" );
	screenXml->setLabelXML( DescContador, "DescContador" );
}

void PanelVentas::createFlagXML()
{

	screenXml->setFlagXML( &labelFlagClie, "FlagCliente", datos );
    screenXml->setFlagXML( &labelFlagTke, "FlagTke", datos );
	screenXml->setFlagXML( &labelFlagFac, "FlagFac", datos );
    screenXml->setFlagXML( &labelFlagNc, "FlagNc", datos );
    screenXml->setFlagXML( &labelFlagTf, "FlagTf" , datos);
    screenXml->setFlagXML( &labelFlagTarj, "FlagTarj", datos );
    screenXml->setFlagXML( &labelFlagVend, "FlagVend", datos );
    screenXml->setFlagXML( &labelFlagDesc, "FlagDesc", datos );
    screenXml->setFlagXML( &labelFlagAfinidad, "FlagAfinidad", datos );
    screenXml->setFlagXML( &labelFlagPeticion, "FlagPeticion", datos );
    screenXml->setFlagXML( &labelFlagPresentacion, "FlagPresentacion", datos );
    screenXml->setFlagXML( &labelFlagMutual, "FlagMutual", datos );
    screenXml->setFlagXML( &labelFlagDevolucion, "FlagDevolucion", datos );
    screenXml->setFlagXML( &labelFlagCambio, "FlagCambio", datos );
    screenXml->setFlagXML( &labelFlagBienUso, "FlagBienUso", datos );
    screenXml->setFlagXML( &labelFlagDev, "FlagDev", datos );
    screenXml->setFlagXML( &labelFlagDevItem, "FlagDevItem", datos );
    screenXml->setFlagXML( &labelFlagEntrenamiento, "FlagEntrenamiento", datos );
	screenXml->setFlagXML( &labelFlagModInventario, "FlagModInventario", datos );
	 screenXml->setFlagXML( &labelFlagTipoInventario, "FlagTipoInventario", datos );
    screenXml->setFlagXML( &labelFlagDetList, "FlagDetList", datos );
    screenXml->setFlagXML( &labelFlagPerfil, "FlagPerfil", datos );
    screenXml->setFlagXML( &labelFlagMozo, "FlagMozo", datos );
    screenXml->setFlagXML( &labelFlagCupo, "FlagCupo", datos );
    screenXml->setFlagXML( &labelFlagErrorRed, "FlagErrorRed", datos );
    screenXml->setFlagXML( &labelFlagCambio, "FlagCambio", datos );
   	
	screenXml->setFlagXML( &labelFlagMonedero, "FlagMonedero" );
    screenXml->setFlagXML( &labelFlagRetDgr, "FlagRetDgr", datos );
}

void PanelVentas::createGridXML()
{
    screenXml->setGridXML( datos, this, "datos" );
    screenXml->setGridXML( ticket, this, "ticket" );
}

void PanelVentas::setNormalBackground() 
{
    SetBackGround( backgroundLocation.c_str() );
    Refresh();
}

void PanelVentas::setErrorBackground() 
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

void PanelVentas::addArticle( const string &signo, const string &cantidad, const string &cinterno,
                              const string &cbarra, const string &nombre, const string &precio )
{

    string aux;
	char referenciada = ( RAM_MODO_DEVOLUCION == _NC_TOTAL || RAM_MODO_DEVOLUCION == _NC_RF_INTERME 
			|| RAM_MODO_DEVOLUCION == _DEVOL_TOTAL || RAM_MODO_DEVOLUCION == _DEVOL_INTERME  );

	vector<string> row;
    row.push_back( signo );
    row.push_back( cantidad );
    row.push_back( cinterno );
    row.push_back( cbarra );
	//hmm ver abajo tambien por demora en mostrar el item en la grilla
	// Dnc & Mcuso "Comentar para ganar velocidad en la impresion.no deberia producri caida en los browsers" 
	MutexGuiEnter();
    if( mostrar_flag_articulo ) {
        labelArt.setText( "" );
        if( labelArt.isVisible() ) {
            labelArt.setVisible( false );
        }
        //aux = "images/art/"+cbarra+".gif";
        aux = PATH_ART + cbarra + ".gif";
        labelArt.setBitmap( wxString( aux.c_str() ), true );
        labelArt.setVisible( true );
    }
    row.push_back( nombre );
    row.push_back( precio );
    ticket->AddRow( row );
    ticket->SetGridCursor( ticket->GetNumberRows() - 1, 0 );

    #ifdef INVEL_L
    ticket->Scroll( 0, ticket->GetNumberRows() );
    #else
    ticket->ScrollLines( ticket->GetRows() );
    #endif
    // Dnc & Mcuso "Comentar para ganar velocidad en la impresion.no deberia producri caida en los browsers" 
	MutexGuiLeave();
}

void PanelVentas::delArticle( int row )
{
    MutexGuiEnter();

    labelArt.setText( "" );
    if( labelArt.isVisible() ) {
        labelArt.setVisible( false );
    }    

    ticket->RemoveRow( row );
	if( ticket->GetNumberRows() > 0 ) {
	    ticket->SetGridCursor( ticket->GetNumberRows() - 1 , 1 );

	} else {
	    ticket->SetGridCursor( 0, 1 );

	}
    MutexGuiLeave();
}
void PanelVentas::updArticle( int yRow, const string &signo, const string &cantidad, const string &cinterno,
                              const string &cbarra, const string &nombre, const string &precio )
{

    string aux;
    vector<string> row;
    row.push_back( signo );
    row.push_back( cantidad );
    row.push_back( cinterno );
    row.push_back( cbarra );
    MutexGuiEnter();
    if( mostrar_flag_articulo ) {
        labelArt.setText( "" );
        if( labelArt.isVisible() ) {
            labelArt.setVisible( false );
        }
        //aux = "images/art/"+cbarra+".gif";
        aux = PATH_ART + cbarra + ".gif";
        labelArt.setBitmap( wxString( aux.c_str() ), true );
        labelArt.setVisible( true );
    }
    row.push_back( nombre );
    row.push_back( precio );
    ticket->UpdateRow( yRow,row );

	if( ticket->GetNumberRows() > 0 ) {
	    ticket->SetGridCursor( ticket->GetNumberRows() - 1, 1 );
	} else {
	    ticket->SetGridCursor( 0, 1 );
	}

    #ifdef INVEL_L
    ticket->Scroll( 0, ticket->GetNumberRows() );
    #else
    ticket->ScrollLines( ticket->GetRows() );
    #endif

    MutexGuiLeave();
}

void PanelVentas::setTotal( const string &s )
{
    MutexGuiEnter();
    Total->setText( s );
    Total->Draw();
    leyendasubtotal.setVisible( ( s == " " ) ? false : true );
    leyendasubtotal.Draw();
    MutexGuiLeave();
}


void PanelVentas::setDetalle( const string &s )
{
    MutexGuiEnter();
    Detalle->setText( s );
    Detalle->Draw();
    MutexGuiLeave();
}


void PanelVentas::setBultos( const string &s )
{
    MutexGuiEnter();
    if( RAM_MODO_EJECUCION == INVENTARIO ) {
		if( Bultos->isVisible())
			Bultos->setVisible( false);
		//IBultos->setText( "0" );
		IBultos->setText( s );
		IBultos->Draw();
	} else {
		if( IBultos->isVisible())
			IBultos->setVisible( false);
		//Bultos->setText( "0" );
		Bultos->setText( s );
		Bultos->Draw();
    }

    MutexGuiLeave();
}


void PanelVentas::setTitBultos( const string &s )
{
    MutexGuiEnter();
    //TitBultos.setText(s);
    TitBultos.setVisible( true );
    TitBultos.Draw();
    MutexGuiLeave();
}


void PanelVentas::setModo( const string &s )
{
    MutexGuiEnter();
    //Modo.setText(s);

    if( textomodo[RAM_MODO_EJECUCION] != "" ) {
        Modo.setVisible( false );
        Modo.setBitmap( wxString( textomodo[RAM_MODO_EJECUCION].c_str() ), true );
        Modo.setVisible( true );
    }
    Modo.setVisible( true );
    Modo.Draw();
    MutexGuiLeave();
}


void PanelVentas::setDescripcion( const string &s )
{
    MutexGuiEnter();
    Descripcion->setText( s );
    Descripcion->Draw();
    MutexGuiLeave();
}

void PanelVentas::setCantConsecutivos( const string &s )
{
	MutexGuiEnter();
    CantConsecutivos->setText( s );
    CantConsecutivos->Draw();
    MutexGuiLeave();
}

void PanelVentas::setCantIdenticos( const string &s )
{
	MutexGuiEnter();
    CantIdenticos->setText( s );
    CantIdenticos->Draw();
    MutexGuiLeave();
}

void PanelVentas::setDescContador( const string &s )
{
    MutexGuiEnter();
    DescContador->setText( s );
    DescContador->Draw();
    MutexGuiLeave();
}

void PanelVentas::setCantidad( const string &s )
{
    MutexGuiEnter();
    Cantidad->setText( s );
    Cantidad->Draw();
    MutexGuiLeave();
}


void PanelVentas::setSubtotal( const string &s )
{
    MutexGuiEnter();
    Subtotal->setText( s );
    Subtotal->Draw();
    MutexGuiLeave();
}


void PanelVentas::setMultiplica( const string &s )
{
    MutexGuiEnter();
    Multiplica->setText( s );
    Multiplica->Draw();
    MutexGuiLeave();
}


void PanelVentas::addFlag( const string &opcion, const string &textIcono, int type )
{
    int a;
    MutexGuiEnter();
    switch( type ) {
        case T_FLAGS_COMP_VTAS_AGREGAR:
            a = atoi( opcion.c_str() );
            setFlagVisible( a );
            break;
        case T_FLAGS_CRED_VTAS_AGREGAR:
            if(labelFlagCr.isVisible() == false)
				labelFlagCr.setVisible( true );
            break;
        case T_FLAGS_CODE_VTAS_AGREGAR:
/**/glog("Ejecutando T_FLAGS_CODE_VTAS_AGREGAR[3] ...",LOG_PANTALLAS,5 );
			if(labelFlagBarr.isVisible() == false) //forray controlar bug
				labelFlagBarr.setVisible(true);
/**/glog("... T_FLAGS_CODE_VTAS_AGREGAR[3] Listo",LOG_PANTALLAS,5 );
            break;
        case T_FLAGS_TKE_REM_VTAS_AGREGAR:
			if(tEspera_remoto.isVisible() == false)
				tEspera_remoto.setVisible(true);
            break;
        case T_FLAGS_TKE_VTAS_AGREGAR:
            if(labelFlagTke.isVisible() == false)
				labelFlagTke.setVisible( true );
            break;
        case T_DATOS_CLIENTE_AGREGAR:
            labelFlagClie.setText( opcion );
			if(labelFlagClie.isVisible() == false)
				labelFlagClie.setVisible( true );
            break;
        case T_DATOS_TARJETA_AGREGAR:
            labelFlagTarj.setText( opcion );
			if(labelFlagTarj.isVisible() == false)
				labelFlagTarj.setVisible( true );
            break;
        case T_FLAGS_COMP_VTAS_BORRAR:
            setFlagComprobNotVisible();
            break;
		case T_FLAGS_MONEDERO_AGREGAR:
			if(labelFlagMonedero.isVisible() == false)
				labelFlagMonedero.setVisible( true );
            break;
		case T_FLAGS_MONEDERO_BORRAR:
            setFlagMonederoNotVisible();
            break;
        case T_FLAGS_CRED_VTAS_BORRAR:
			if(labelFlagCr.isVisible() == true)
				labelFlagCr.setVisible( false );
            break;
        case T_FLAGS_CODE_VTAS_BORRAR:
///**/glog("Ejecutando T_FLAGS_CODE_VTAS_BORRAR[3] ...");
			if(labelFlagBarr.isVisible() == true) //forray controlar bug
				labelFlagBarr.setVisible(false);
///**/glog("... T_FLAGS_CODE_VTAS_BORRAR[3] LISTO");
            break;
        case T_FLAGS_TKE_REM_VTAS_BORRAR:
			if(tEspera_remoto.isVisible() == true)
				tEspera_remoto.setVisible( false);
            break;
        case T_FLAGS_TKE_VTAS_BORRAR:
///**/glog("Ejecutando T_FLAGS_TKE_VTAS_BORRAR[3] ...");
			if(labelFlagTke.isVisible() == true)
				labelFlagTke.setVisible( false );
///**/glog("... T_FLAGS_TKE_VTAS_BORRAR[3] LISTO");
            break;
        case T_DATOS_CLIENTE_BORRAR:
			if(labelFlagClie.isVisible() == true)
				labelFlagClie.setVisible( false );
            break;
        case T_DATOS_TARJETA_BORRAR:
			if(labelFlagTarj.isVisible() == true)
				labelFlagTarj.setVisible( false );
            break;
        case T_DATOS_VENDEDOR_AGREGAR:
            labelFlagVend.setText( opcion );
			if(labelFlagVend.isVisible() == false)
				labelFlagVend.setVisible( true );
            break;
        case T_DATOS_VENDEDOR_BORRAR:
			if(labelFlagVend.isVisible() == true)
				labelFlagVend.setVisible( false );
            break;
        case T_DATOS_SALDO_AGREGAR:
        case T_DATOS_CUPO_CHEQUE_AGREGAR:
            labelFlagCupo.setText( opcion );
			if(labelFlagCupo.isVisible() == false)
				labelFlagCupo.setVisible( true );
            break;
        case T_DATOS_SALDO_BORRAR:
        case T_DATOS_CUPO_CHEQUE_BORRAR:
			if(labelFlagCupo.isVisible() == true)
				labelFlagCupo.setVisible( false );
            break;
        case T_FLAGS_DESCUENTO_1_AGREGAR:
			if( labelFlagDesc.isVisible() == true )
				labelFlagDesc.setVisible( false );
            labelFlagDesc.changeCurrentState( 0 );
            labelFlagDesc.setText( opcion );
            labelFlagDesc.setVisible( true );
            break;
        case T_FLAGS_DESCUENTO_2_AGREGAR:
			if( labelFlagDesc.isVisible() == true )
				labelFlagDesc.setVisible( false );
            labelFlagDesc.changeCurrentState( 1 );
            labelFlagDesc.setText( opcion );
            labelFlagDesc.setVisible( true );
            break;
        case T_FLAGS_DESCUENTO_3_AGREGAR:
			if( labelFlagDesc.isVisible() == true )
				labelFlagDesc.setVisible( false );
            labelFlagDesc.changeCurrentState( 2 );
            labelFlagDesc.setText( opcion );
            labelFlagDesc.setVisible( true );
            break;
        case T_FLAGS_DESCUENTO_4_AGREGAR:
			if( labelFlagDesc.isVisible() == true )
				labelFlagDesc.setVisible( false );
            labelFlagDesc.changeCurrentState( 3 );
            labelFlagDesc.setText( opcion );
            labelFlagDesc.setVisible( true );
            break;
        case T_FLAGS_DESCUENTO_5_AGREGAR:
			if( labelFlagDesc.isVisible() == true )
				labelFlagDesc.setVisible( false );
            labelFlagDesc.changeCurrentState( 4 );
            labelFlagDesc.setText( opcion );
            labelFlagDesc.setVisible( true );
            break;
        case T_FLAGS_DESCUENTO_1_BORRAR:
        case T_FLAGS_DESCUENTO_2_BORRAR:
        case T_FLAGS_DESCUENTO_3_BORRAR:
        case T_FLAGS_DESCUENTO_4_BORRAR:
        case T_FLAGS_DESCUENTO_5_BORRAR:
			if(labelFlagDesc.isVisible() == true)
				labelFlagDesc.setVisible( false );
            break;
        case T_DATOS_AFINIDAD_AGREGAR:
            labelFlagAfinidad.setText( opcion );
			if(labelFlagAfinidad.isVisible() == false)
				labelFlagAfinidad.setVisible( true );
            break;
        case T_DATOS_AFINIDAD_BORRAR:
			if(labelFlagAfinidad.isVisible() == true)
				labelFlagAfinidad.setVisible( false );
            break;
        case T_DESACTIVA_CUADRO_PETICION:
			if(labelFlagPeticion.isVisible() == true)
				labelFlagPeticion.setVisible( false );
            break;
        case T_NOVIOS:
        case T_CLIENTE_AFINIDAD:
        case T_NRO_DOCUMENTO_CLIENTE:
        case T_NRO_COMPROBANTE:
        case T_FECHA_INICIAL:
        case T_FECHA_FINAL:
            labelFlagPeticion.setText( opcion );
			if(labelFlagPeticion.isVisible() == false)
				labelFlagPeticion.setVisible( true );
            break;
        case T_FLAGS_PRESENT_AGREGAR:
            labelFlagPresentacion.setText( opcion );
			if(labelFlagPresentacion.isVisible() == false)
				labelFlagPresentacion.setVisible( true );
            break;
        case T_FLAGS_PRESENT_BORRAR:
			if(labelFlagPresentacion.isVisible() == true)
				labelFlagPresentacion.setVisible( false );
            break;
        case T_DATOS_PERFIL_AGREGAR:
            labelFlagPerfil.setText( opcion );
			if(labelFlagPerfil.isVisible() == false)
				labelFlagPerfil.setVisible( true );
            break;
        case T_DATOS_PERFIL_BORRAR:
			if(labelFlagPerfil.isVisible() == true)
				labelFlagPerfil.setVisible( false );
            break;
        case T_DATOS_MUTUAL_AGREGAR:
			if(labelFlagMutual.isVisible() == true)
				labelFlagMutual.setVisible( false );
            labelFlagMutual.setText( opcion );
            labelFlagMutual.setVisible( true );
            break;
        case T_DATOS_MUTUAL_BORRAR:
			if(labelFlagMutual.isVisible() == true)
				labelFlagMutual.setVisible( false );
            break;
        case T_FLAGS_DEVOLUCION_AGREGAR :
            labelFlagDevolucion.setText( opcion );
			if(labelFlagDevolucion.isVisible() == false)
				labelFlagDevolucion.setVisible( true );
            break;
        case T_FLAGS_DEVOLUCION_BORRAR :
			if(labelFlagDevolucion.isVisible() == true)
				labelFlagDevolucion.setVisible( false );
            setFlagTipoDevNotVisible();
            break;
        case T_FLAGS_CAMBIO_AGREGAR :
            /*labelFlagCambio.setText( opcion );
			if(labelFlagCambio.isVisible() == false )
				labelFlagCambio.setVisible( true );
            break;*/
            a = atoi( opcion.c_str() );
            setFlagVisible( a );
             break;
        case T_FLAGS_CAMBIO_BORRAR :
			/*if(labelFlagCambio.isVisible() == true )
				labelFlagCambio.setVisible( false );*/
            setFlagCambioNotVisible();
            break;
        case T_FLAGS_BIEN_USO_AGREGAR :
            labelFlagBienUso.setText( opcion );
			if(labelFlagBienUso.isVisible() == false)
				labelFlagBienUso.setVisible( true );
            break;
        case T_FLAGS_BIEN_USO_BORRAR :
			if(labelFlagBienUso.isVisible() == true)
				labelFlagBienUso.setVisible( false );
            break;
        case T_FLAGS_TIPO_DEV_AGREGAR:
            a = atoi( opcion.c_str() );
            switch( a ) {
                case FLAG_DIF:
					if(labelFlagDev.isVisible() == true)
						labelFlagDev.setVisible( false );
                    labelFlagDev.changeCurrentState( 0 );
                    labelFlagDev.setText( opcion );
                    labelFlagDev.setVisible( true );
                    break;
                case FLAG_COMPROMISO:
					if(labelFlagDev.isVisible() == true)
						labelFlagDev.setVisible( false );
                    labelFlagDev.changeCurrentState( 1 );
                    labelFlagDev.setText( opcion );
                    labelFlagDev.setVisible( true );
                    break;
                case FLAG_BAJO:
					if(labelFlagDev.isVisible() == true)
						labelFlagDev.setVisible( false );
                    labelFlagDev.changeCurrentState( 2 );
                    labelFlagDev.setText( opcion );
                    labelFlagDev.setVisible( true );
            }
            break;
        case T_FLAGS_TIPO_DEV_BORRAR:
            setFlagTipoDevNotVisible();
            break;
        case T_FLAGS_DEV_ITEM_AGREGAR:
			if(labelFlagDevItem.isVisible() == false)
				labelFlagDevItem.setVisible( true );
            break;
        case T_FLAGS_DEV_ITEM_BORRAR:
			if(labelFlagDevItem.isVisible() == true)
				labelFlagDevItem.setVisible( false );
            break;
        case T_FLAGS_MODO_AGREGAR:
            a = atoi( opcion.c_str() );
            setFlagVisible( a );
            break;
        case T_FLAGS_MODO_BORRAR:
            setFlagModoNotVisible();
            break;
        case T_DATOS_MOZO_AGREGAR:
            labelFlagMozo.setText( opcion );
			if(labelFlagMozo.isVisible() == false)
				labelFlagMozo.setVisible( true );
            break;
        case T_DATOS_MOZO_BORRAR:
			if(labelFlagMozo.isVisible() == true)
				labelFlagMozo.setVisible( false );
            break;
        case T_FLAGS_ERROR_RED_AGREGAR:
            if(labelFlagErrorRed.isVisible() == false) {
				setErrorBackground();
                labelFlagErrorRed.setVisible( true );
            }
            break;
        case T_FLAGS_ERROR_RED_BORRAR:
            if(labelFlagErrorRed.isVisible() == true) {
			    setNormalBackground();
                labelFlagErrorRed.setVisible( false );
            }
            break;
        case T_DATOS_RETENCION_DGR_AGREGAR:
            labelFlagRetDgr.setText( opcion );
            labelFlagRetDgr.setVisible( true );
            break;
        case T_DATOS_RETENCION_DGR_BORRAR:
            labelFlagRetDgr.setVisible( false );
            break;

    }
    MutexGuiLeave();
}

void PanelVentas::setFlagComprobNotVisible()
{
	if(labelFlagFac.isVisible() == true)
		labelFlagFac.setVisible( false );
	if(labelFlagTf.isVisible() == true)
		labelFlagTf.setVisible( false );
	if(labelFlagNc.isVisible() == true)
		labelFlagNc.setVisible( false );
}

void PanelVentas::setFlagCambioNotVisible()
{
	if(labelFlagCambio.isVisible() == true)
		labelFlagCambio.setVisible( false );
}

void PanelVentas::setFlagMonederoNotVisible()
{
	if(labelFlagMonedero.isVisible() == true)
		labelFlagMonedero.setVisible( false );
}

void PanelVentas::setFlagTipoDevNotVisible()
{
	if(labelFlagDev.isVisible() == true)
		labelFlagDev.setVisible( false );
}


void PanelVentas::setFlagModoNotVisible()
{
	if(labelFlagEntrenamiento.isVisible() == true)
		labelFlagEntrenamiento.setVisible(false);
        //labelFlagModOperacion.setVisible(false);
        //labelFlagModEntrenamiento.setVisible(false);
        labelFlagModInventario.setVisible(false);
        labelFlagTipoInventario.setVisible(false);
        //labelFlagModEtiquetas.setVisible(false);
        //labelFlagModTransferencia.setVisible(false);
}

void PanelVentas::showCashier( int type, const string &s, const string &s1 )
{   MutexGuiEnter();
	string aux,aux2;
    //labelTicket->Draw();
    labelTicket->setVisible( true );
    labelGridVentas->setVisible( true );
    nroCaja->setText( "CAJERO: " + s );
    nCajero->setText( "CAJA: " + s1 );
    fotoCajero.setText( "" );
    if( fotoCajero.isVisible() ) {
       fotoCajero.setVisible( false );
    }
	aux2=s.substr(0,1);
	aux = PATH_FOTO + aux2 + ".gif";
	if( !wxFileExists( aux.c_str() ) ) {
		aux = FOTODEFECTO;
	}
	fotoCajero.setBitmap( wxString( aux.c_str() ), true );
    fotoCajero.setVisible( true );
    nroCaja->Draw();
    nCajero->Draw();
    MutexGuiLeave();
}


void PanelVentas::setFlagVisible( int type )
{
    //MutexGuiEnter(); 
    switch( type ) {
        case FLAG_CAMBIO_MAS:
			if(labelFlagCambio.isVisible() == true)
				labelFlagCambio.setVisible( false );
            labelFlagCambio.changeCurrentState( 0 );
            labelFlagCambio.setVisible( true );
            break;
        case FLAG_CAMBIO_MENOS:
			if(labelFlagCambio.isVisible() == true)
				labelFlagCambio.setVisible( false );
            labelFlagCambio.changeCurrentState( 1 );
            labelFlagCambio.setVisible( true );
            break;
        case FLAG_FACA:
			if(labelFlagFac.isVisible() == true)
				labelFlagFac.setVisible( false );
            labelFlagFac.changeCurrentState( 0 );
            labelFlagFac.setVisible( true );
            break;
        case FLAG_FACB:
			if(labelFlagFac.isVisible() == true)
				labelFlagFac.setVisible( false );
            labelFlagFac.changeCurrentState( 1 );
            labelFlagFac.setVisible( true );
            break;
        case FLAG_FACC:
			if(labelFlagFac.isVisible() == true)
				labelFlagFac.setVisible( false );
            labelFlagFac.changeCurrentState( 2 );
            labelFlagFac.setVisible( true );
            break;
        case FLAG_FCF:
			if(labelFlagFac.isVisible() == true)
				labelFlagFac.setVisible( false );
            labelFlagFac.changeCurrentState( 3 );
            labelFlagFac.setVisible( true );
            break;
        case FLAG_CF:
			if(labelFlagFac.isVisible() == true)
				labelFlagFac.setVisible( false );
            labelFlagFac.changeCurrentState( 4 );
            labelFlagFac.setVisible( true );
            break;
        case FLAG_TFA:
			if(labelFlagTf.isVisible() == true)
				labelFlagTf.setVisible( false );
            labelFlagTf.changeCurrentState( 0 );
			labelFlagTf.setVisible( true );
            break;
        case FLAG_TFB:
			if(labelFlagTf.isVisible() == true)
				labelFlagTf.setVisible( false );
            labelFlagTf.changeCurrentState( 1 );
            labelFlagTf.setVisible( true );
            break;
        case FLAG_TFC:
			if(labelFlagTf.isVisible() == true)
				labelFlagTf.setVisible( false );
            labelFlagTf.changeCurrentState( 2 );
            labelFlagTf.setVisible( true );
            break;
		case FLAG_NC:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 0 );
            labelFlagNc.setVisible( true );
            break;
        case FLAG_GUIA:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 1 );
            labelFlagNc.setVisible( true );
            break;
        case FLAG_REM:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 2 );
            labelFlagNc.setVisible( true );
            break;
        case FLAG_NC_REF:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 3 );
            labelFlagNc.setVisible( true );
            break;
		case FLAG_NC_A:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 4 );
            labelFlagNc.setVisible( true );
         break;
		case FLAG_NC_B:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 5 );
            labelFlagNc.setVisible( true );
         break;
		case FLAG_NC_A_REF:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 6 );
            labelFlagNc.setVisible( true );
         break;
		case FLAG_NC_B_REF:
			if(labelFlagNc.isVisible() == true)
				labelFlagNc.setVisible( false );
            labelFlagNc.changeCurrentState( 7 );
            labelFlagNc.setVisible( true );
        
         break;
        case FLAG_CR:
			if(labelFlagCr.isVisible() == false)
				labelFlagCr.setVisible( true );
            break;
        case FLAG_COD:
            //labelFlagBarr.setVisible( false ); 
            //labelFlagCod.setVisible(true);
            break;
        case FLAG_TKE:
			if(labelFlagTke.isVisible() == false)
				labelFlagTke.setVisible( true );
            break;
        case FLAG_DIF:
			if(labelFlagDev.isVisible() == true)
				labelFlagDev.setVisible( false );
            labelFlagDev.changeCurrentState( 0 );
            labelFlagDev.setVisible( true );
            break;
        case FLAG_COMPROMISO:
			if(labelFlagDev.isVisible() == true)
				labelFlagDev.setVisible( false );
            labelFlagDev.changeCurrentState( 1 );
            labelFlagDev.setVisible( true );
            break;
		case FLAG_MONEDERO:
			if(labelFlagMonedero.isVisible() == true)
				labelFlagMonedero.setVisible( false );
            labelFlagMonedero.changeCurrentState( 0 );
            labelFlagMonedero.setVisible( true );
            break;
        case FLAG_BAJO:
			if(labelFlagDev.isVisible() == true)
				labelFlagDev.setVisible( false );
            labelFlagDev.changeCurrentState( 2 );
            labelFlagDev.setVisible( true );
            break;
        case FLAG_MODO_OPERACION:
            //labelFlagModOperacion.setVisible(true);
            break;
        case FLAG_MODO_ENTRENAMIENTO:
			if(labelFlagEntrenamiento.isVisible() == false)
                 labelFlagEntrenamiento.setVisible(true);
            break;
        case FLAG_MODO_INVENTARIO:
            if(labelFlagModInventario.isVisible() == false) {
				labelFlagModInventario.setVisible(true);
				labelFlagTipoInventario.setVisible(true);
			}
            break;
        case FLAG_MODO_ETIQUETAS:
            //    labelFlagModEtiquetas.setVisible(true);
            break;
        case FLAG_MODO_TRANSFERENCIA:
            //     labelFlagModTransferencia.setVisible(true);
            break;
    }
    //MutexGuiLeave();
}

void PanelVentas::batch( const string &status )
{
    if( status[0] == 'b' ) {
        if( ticket->GetBatchCount() == 0 ) {
            ticket->BeginBatch();
        }
        return;
    }
    if( status[0] == 'e' ) { 
        if( ticket->GetBatchCount() > 0) {
            ticket->EndBatch();    
        }
        return;
    }
}

void PanelVentas::scroll( const string &lines )
{
    static int curr_line = 0;
    int filas = 0;
    int line = (lines.length() > 0) ? atoi( lines.c_str() ) : 0;
    
    curr_line = (curr_line > (ticket->GetRows() - 1) ? ticket->GetRows() - 1 : curr_line);

    //#ifdef INVEL_L
    MutexGuiEnter();
    //#endif
    //ticket->BeginBatch();

    //ticket->SetGridCursor( curr_line, 0 );
    //ticket->ClearSelection();
    if( line == -99 ) {
        //deshabilita
        ticket->Refresh();
        ticket->SetGridCursor( ticket->GetRows()-1, 0 );
		ticket->ClearSelection();
    }
    else if( line == 0 ) {
        //habilita
        ticket->Refresh();
        curr_line = ticket->GetRows() - 1;
        ticket->SetGridCursor( curr_line, 0 );
        ticket->ScrollLines( ticket->GetRows() );
        ticket->SelectRow( curr_line );
    }
    else if( line > 0 ) {
        //baja line lineas
        for( int l = 0;l < line;l++ ) {
            if( curr_line< ticket->GetRows() - 1 ) {
                curr_line++;
                ticket->MoveCursorDown( false );
            }
        }
        ticket->SelectRow( curr_line );
    }
    else if( line < 0 ) {
        //sube line lineas
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                ticket->MoveCursorUp( false );
            }
        }
        ticket->SelectRow( curr_line );
    }
    //ticket->ScrollVisible = true;
    //#ifdef INVEL_L
    //MutexGuiEnter();
    //ticket->EndBatch();
    MutexGuiLeave();
    //#endif
}
void PanelVentas::ShowImgArt(const string &cod_barra)
{
	string aux;
    MutexGuiEnter();
	if( mostrar_flag_articulo ) {
        labelArt.setText( "" );
        if( labelArt.isVisible() ) {
            labelArt.setVisible( false );
        }
        aux = PATH_ART + cod_barra + ".gif";
        labelArt.setBitmap( wxString( aux.c_str() ), true );
        labelArt.setVisible( true );
    }
    MutexGuiLeave();
}

void PanelVentas::ShowHora(const wxString &SHora)
{
    //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
    wxMutexLocker lock(s_mutexHoraVenta);
    TitHora->setText( SHora.c_str() );
    TitHora->setVisible( true );
}

void PanelVentas::ShowFecha(const wxString &SFecha)
{
    //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
    wxMutexLocker lock(s_mutexHoraVenta);
    //TitFecha->setText( "Fecha: " );
    FechaLocal->setText( SFecha.c_str() );
    TitFecha->setVisible( true );
    FechaLocal->setVisible( true );
}

void PanelVentas::ShowFechaOp(const wxString &SFecha)
{
    MutexGuiEnter();
    Fecha->setText( SFecha.c_str() );
	//FecOper->setText( SFecha.c_str() );
    TitFechaOp->setVisible( true );
    Fecha->setVisible( true );
	//FecOper->setVisible( true );
    MutexGuiLeave();
}

void PanelVentas::ShowRenglones( bool type)
{
    MutexGuiEnter();
    //TitRenglones.setVisible( type );
	if( type == true ) {
			int  tamanio;
			Total->setTamanioTotal( );
			tamanio = (int) Total->getTamanioTotal() / 1.4;
			Total->setFontSize(tamanio);
	    if( Bultos->isVisible()) {
          Bultos->setVisible( false);
        }
        if( TitBultos.isVisible())
           TitBultos.setVisible( false );
	} else {
         TipoInventario->setText(" ");
         TipoInventario->setVisible( type );
         Total->setFontSize(Total->getTamanioTotal());
      if( IBultos->isVisible())
          IBultos->setVisible( false);
      TitBultos.setVisible( true );
    }
	TitRenglones.setVisible( type );

    MutexGuiLeave();

}

void PanelVentas::ShowTipoInventario( const wxString &SInvetario)
{
    MutexGuiEnter();
    TipoInventario->setText(" ");
    TipoInventario->setText( SInvetario.c_str() );
    TipoInventario->setVisible( true );
    MutexGuiLeave();
}

void PanelVentas::SetFocus( )
/*Esta funcion pone al tope de la lista de ventanas a la ventana .
Por eso la llamo setfocus. Hay que tener cuidado que no haya otra ventana arriba porque le cambiara el foco
*/
{
	this->Raise();
}

