#include "PanelPresentacion.h"
#include "GuiProperties.h"
#include <path.h>

extern CDesktop *properties;
static wxMutex s_mutexHoraPres;

PanelPresentacion::PanelPresentacion( const wxString &title, const wxPoint &pos, const wxSize &size,
                                      long style ) : CPanel( title, pos, size, style )
{
    NomVersion = new CLabel();
    NomBranch = new CLabel();
    FecOper = new CLabel();
    Fecha = new CLabel();
    Usuario = new CLabel();
    Password = new CLabel();
    label_mensaje = new CLabel();
    TitHora = new CLabel();
    CajeroActual = new CLabel();
	TitFechaOp = new CLabel();
	FechaSis = new CLabel();
    screenXml = new CScreencfg( "presentacion", "default", "default" );
}

void PanelPresentacion::init()
{
}

void PanelPresentacion::createLabels()
{
    NomVersion->setParent( this );
    NomBranch->setParent( this );
    FecOper->setParent( this );
    Fecha->setParent( this );
    Usuario->setParent( this );
    Password->setParent( this );
    label_mensaje->setParent( this );
    IconPres.setParent( this ); 
    TitHora->setParent( this );
    CajeroActual->setParent( this );
	TitFechaOp->setParent( this );
	FechaSis->setParent( this );

    this->AddObject( NomVersion );
    this->AddObject( NomBranch );
    this->AddObject( FecOper );
    this->AddObject( Fecha );
    this->AddObject( Usuario );
    this->AddObject( Password );
    this->AddObject( label_mensaje );
    this->AddObject( &IconPres );
    this->AddObject( TitHora );
    this->AddObject( CajeroActual );
	this->AddObject( TitFechaOp );
	this->AddObject( FechaSis );

    createXML();
}

void PanelPresentacion::createXML()
{
    string background;

    background = screenXml->getBackgroundXML();
    SetBackGround( background.c_str() );

    screenXml->setLabelXML( NomVersion, "Version" );
    screenXml->setLabelXML( NomBranch, "Branch" );
    screenXml->setLabelXML( Fecha, "TitFecha" );
    screenXml->setLabelXML( FecOper, "FechaOperacion" );
    screenXml->setLabelXML( Usuario, "TitUsuario" );
    screenXml->setLabelXML( Password, "TitPassword" );
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( TitHora, "Hora" );
    screenXml->setLabelXML( CajeroActual, "CajeroActual" );
    screenXml->setInputXML( input_password, this, "Password" );
    screenXml->setInputXML( input_usuario, this, "Usuario" );
	screenXml->setLabelXML( TitFechaOp, "TitFechaOperativa" );
	screenXml->setLabelXML( FechaSis, "FechaSistema" );

    screenXml->setIconXML( &IconPres, "FotoPres" );

    input_focus = input_usuario;
}

void PanelPresentacion::addFlag( const string &s, const string &s1, int type )
{
}


void PanelPresentacion::addData( const string &s, const string &s1, int type )
{
}


int PanelPresentacion::ifFlag( int type )
{
    return 0;
}


void PanelPresentacion::setFecha( const string &s )
{
    MutexGuiEnter();
    Fecha->setText( s );
    Fecha->Draw();
    MutexGuiLeave();
}


void PanelPresentacion::setUsuario( const string &s )
{
    MutexGuiEnter();
    Usuario->setText( s );
    Usuario->Draw();
    MutexGuiLeave();
}


void PanelPresentacion::setPassword( const string &s )
{
    MutexGuiEnter();
    Password->setText( s );
    Password->Draw();
    MutexGuiLeave();
}

void PanelPresentacion::setFechaOp( const string &s )
{
    MutexGuiEnter();
    FecOper->setText( s );
    FecOper->Draw();
    MutexGuiLeave();
}

void PanelPresentacion::setPosInputBox( bool clave )
{
    if( !clave ) {
        input_focus = input_usuario;
        FotoDefault();
    }
    else {
        input_focus = input_password;
    }
}

void PanelPresentacion::MostrarFoto( const string &path )
{
    string aux;
    aux = PATH_FOTO + path;
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    IconPres.setText( "" );
    if( IconPres.isVisible() ) {
        IconPres.setVisible( false );
    }

    if( !wxFileExists( aux.c_str() ) ) {
        //aux = PATH_FOTO+"fotodefault.gif";
        aux = FOTODEFECTO;
    }

    IconPres.setBitmap( wxString( aux.c_str() ), true );
    IconPres.setVisible( true );
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}

void PanelPresentacion::FotoDefault()
{
    MostrarFoto( "fotodefault.gif" );
}

void PanelPresentacion::ShowHora(const wxString &SHora)
{
    //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
    wxMutexLocker lock(s_mutexHoraPres);
    TitHora->setText( SHora.c_str() );
    TitHora->setVisible( true );
}

void PanelPresentacion::setCajeroActual(const wxString &Scajero)
{
    //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
    wxMutexLocker lock(s_mutexHoraPres);
    CajeroActual->setText( Scajero.c_str() );
	CajeroActual->setVisible( true );
}

bool PanelPresentacion::isShowCajeroActual( )
{
    if( !CajeroActual->isVisible())
		return false;
	
	return true;

}

void PanelPresentacion::ShowFechaOp(const wxString &SFecha)
{
    MutexGuiEnter();
    FecOper->setText( SFecha.c_str() );
    TitFechaOp->setVisible( true );
    FecOper->setVisible( true );
    MutexGuiLeave();

}

void PanelPresentacion::ShowFechaSis(const wxString &SFecha)
{
    MutexGuiEnter();
    FechaSis->setText( SFecha.c_str() );
    Fecha->setVisible( true );
    FechaSis->setVisible( true );
    MutexGuiLeave();
}
