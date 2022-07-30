#include "PanelStandby.h"
#include "AppMain.h"

extern CDesktop *properties;
static wxMutex s_mutexHoraStand;
PanelStandby::PanelStandby( const wxString &title, const wxPoint &pos, const wxSize &size,
                            long style ) : CPanel( title, pos, size, style )
{
    titulo = new CLabel();
    titulo1 = new CLabel();
    TitHora = new CLabel();
    TitFecha = new CLabel();
    Fecha = new CLabel();
    label_mensaje = new CLabel();
    screenXml = new CScreencfg( "standby", "default", "default" );
}


void PanelStandby::createLabels()
{
    titulo->setParent( this );
    titulo1->setParent( this );
    TitHora->setParent( this );
    TitFecha->setParent( this );
    Fecha->setParent( this );
    label_mensaje->setParent( this );

    this->AddObject( titulo );
    this->AddObject( titulo1 );
    this->AddObject( label_mensaje );
    this->AddObject( TitHora );
    this->AddObject( TitFecha );
    this->AddObject( Fecha );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createLabelXML();

    create();
}


void PanelStandby::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( titulo, "Nombre" );
    screenXml->setLabelXML( titulo1, "Caja" );
    screenXml->setLabelXML( TitFecha, "TitFecha" );
    screenXml->setLabelXML( TitHora, "Hora" );
    screenXml->setLabelXML( Fecha, "FechaOperacion" );
}



void PanelStandby::create()
{
    int input_size = 17;
    int offset = 3;
    int start_offset = 35;
    //int x, y;

    input = new CInput( this, 300, 200, 90, 17, false );
    input->SetNotVisible();
}



void PanelStandby::setTitulos( const string &s, const string &s1 )
{
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    titulo->setText( s );
    titulo1->setText( s1 );
    titulo->setVisible( true );
    titulo1->setVisible( true );
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}

void PanelStandby::ShowHora(const wxString &SHora)
{
    //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
    wxMutexLocker lock(s_mutexHoraStand);
    TitHora->setText( SHora.c_str() );
    TitHora->setVisible( true );
}

void PanelStandby::ShowFecha(const wxString &SFecha)
{
    //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
    wxMutexLocker lock(s_mutexHoraStand);
    TitFecha->setText( "Fecha: " );
    Fecha->setText( SFecha.c_str() );
    TitFecha->setVisible( true );
    Fecha->setVisible( true );
}

