/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/* ----------------------- versionado TPV IV ------------------------------------
 * TPV <xx> - <xxx>.<xxx>.<xxx> - <xxx> - R<xxx>
 * TPV <producto> - <macroversion>.<version>.<subversion> - <build> - R<release>
 *
 * producto:      Por ahora y por mucho tiempo sera el producto IV.
 *                Este valor cambia solo con autorizacion de la Gerencia.
 *
 * macro version: Este valor se incrementa con aparicion de nuevos modulos o nuevos
 *                fuentes.
 *                Ademas, se incrementa con agregados de nuevas funcionalidades,
 *                lo cual generaran un cambio de la ERS de producto. Por ej.: nueva
 *                tecla.
 *                Este valor vuelve a cero con un incremento del "release".
 *
 * version:       Este valor se incrementa con cambios de DICCIONARIOS LOCALES (dd*),
 *                cambios de tablas lo q puede incluir cambios en la base de datos.
 *                Este valor vuelve a cero con un incremento del "release".
 *
 * sub version:   Este valor se incrementa con cambios de funcionalidades existentes,
 *                mejoras q no generen nuevos fuentes ni cambios
 *                en el comportamiento de funcionalidades existentes.
 *                Este valor vuelve a cero con un incremento del "release".
 *
 * build:         Este valor incrementa con cada compilacion del producto, ya sea por
 *                correccion de errores o por algun otro motivo no contemplado en los
 *                anteriores.
 *                Este valor vuelve a cero con un incremento del "release".
 *
 * release:       Este valor se incrementa al momento de entregarla al cliente final.
 *
 *"TPV %2s - %03i.%03i.%03i - %03i - R%03i"
 *
 * branch:        Este valor solo se incrementa en caso de estar en un branch
 *                Este valor vuelve a cero con un merge al tronco
 *                macroversion.version.sub_version - build
 * ------------------------------------------------------------------------------
 */

//#include "cr.h"

#pragma warning (disable:4786)
#pragma pack(8)
#include "CFtp.h"
#include "MainThread.h"
#include "AppMain.h"
#include "PanelVentas.h"

#include "CIMain.h"
#include "MsgInMain.h"
#include "CDataTime.h"

#include "PanelPresentacion.h"
#include "PanelInicio.h"
#include "IniParser.h"
#include "CConfig.h"
#include "MsgEmergentes.h"
#include "MsgPantalla.h"
#include "VideoPlayer.h"


//extern struct _config_tps config_tps ;
#ifdef COMPILAR_UML
#define EVT_PROTO_DISPATCH_COMMAND(id, fn) \
    DECLARE_EVENT_TABLE_ENTRY( \
    AppMain::wxPROTO_DISPATCH_EVENT, id, -1, \
    (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction)&fn, \
    (wxObject *) NULL \
    ),

BEGIN_EVENT_TABLE(AppMain, wxApp)
EVT_PROTO_DISPATCH_COMMAND(-1, AppMain::OnDispatchEvent)
EVT_CHAR(AppMain::OnKey)
EVT_MOUSE_EVENTS(AppMain::Dummy)
EVT_COMMAND_SCROLL(-1, AppMain::Dummy1)
//EVT_LEFT_DOWN(AppMain::Dummy)
//EVT_LEFT_UP(AppMain::Dummy)
//EVT_MIDDLE_DOWN(AppMain::Dummy)
//EVT_MIDDLE_UP(AppMain::Dummy)
//EVT_RIGHT_DOWN(AppMain::Dummy)
//EVT_RIGHT_UP(AppMain::Dummy)
//EVT_MOTION(AppMain::Dummy)
//EVT_ENTER_WINDOW(AppMain::Dummy)
//EVT_LEAVE_WINDOW(AppMain::Dummy)
//EVT_LEFT_DCLICK(AppMain::Dummy)
//EVT_MIDDLE_DCLICK(AppMain::Dummy)
//EVT_RIGHT_DCLICK(AppMain::Dummy)
//EVT_SET_FOCUS(AppMain::Dummy)
//EVT_KILL_FOCUS(AppMain::Dummy)
//EVT_CHILD_FOCUS(AppMain::Dummy)
EVT_MOUSEWHEEL(AppMain::Dummy)

END_EVENT_TABLE()

IMPLEMENT_APP(AppMain)
IMPLEMENT_CLASS(AppMain,wxApp)
#endif

CDesktop *properties;
CConfig *Config;
//CConfigTPS *configtps;


MainThread *main_thread;
extern void CREATE_PANTALLA();
extern char *CREATE_BROWSE_PANEL(AppMain *_app);
extern bool CREATE_PANTALLA_INICIO(char *path);
extern PanelPresentacion *panPres;
extern PanelInicio *panInicio;
CIMain *com_thread;
MsgInMain *com2_thread;
CDataTime *DataTime_thread;
MsgEmergentes *MsgEmerg_thread;
MsgPantalla *MsgPantalla_thread;
CFtp *Ftp_thread;
VideoPlayer *video_thread;
extern char var[30];
const wxEventType AppMain::wxPROTO_DISPATCH_EVENT = wxNewEventType();

char ver_posvisual[40];
//char ver_branch_posvisual[60];
char ver_branch_posvisual[100];

extern "C"
{
	//#include "ini.h"
    extern int core_iniciado;
	extern int init_core;
	extern int init_archivos;
//	extern void SET_VALORES_CONFIG_TPS(struct _config_tps auxi);
}

AppMain::AppMain()
{
    #ifdef INVEL_W
	ShowCursor(false);
    #endif
    ver_branch_posvisual[0] = 0;
    _snprintf( VERSION.producto, sizeof(VERSION.producto)-1, "%s","V");

	/*Viejo versionado*/
    /*VERSION.macroversion = 8;
    VERSION.version      = 17;
    VERSION.sub_version  = 27;
    VERSION.build        = 192;
    VERSION.release      = 4;*/

    /*Versionado del branch "macroversion.version.sub_version - build"
      Ejemplo: sprintf(VERSION.branch, "%s","001.007.004 - 007");*/

    /*Nuevo versionado*/
	/*Revisar el uso interno y eliminar macroversion, sub_version y release*/
	VERSION.macroversion = 0;
	VERSION.release      = 0;
	/*----------------*/
	VERSION.version      = 2;
    VERSION.build        = 195;

	//esta linea es para releases --> 	_snprintf(VERSION.branch, sizeof(VERSION.branch)-1,"TPSV_%02i_%03i_R00n_mm",VERSION.version, VERSION.build);  
	//esta linea es para proyecto de branch --> _snprintf(VERSION.branch, sizeof(VERSION.branch)-1, "%s","Sprint_<Nro_Sprint>_<Año>_<Proyecto_00_00>");
    _snprintf(VERSION.branch, sizeof(VERSION.branch)-1,"TPSV_02_195_R006_103_Feature_2022_NAPSE_07",VERSION.version, VERSION.build);  
	_snprintf(ver_posvisual, sizeof(ver_posvisual)-1, "TPS%2s - %02i.%03i", VERSION.producto,
             VERSION.version, VERSION.build );
    if(strlen(VERSION.branch) > 0){
        _snprintf( ver_branch_posvisual, sizeof(ver_branch_posvisual)-1, "%s", VERSION.branch );
    }
    TKT_VERSION_RETAIL = 11;
    TKT_VERSION_SERVICIOS = 0;
}

AppMain::~AppMain()
{
}

// 'Main program' equivalent: the program execution "starts" here
bool AppMain::OnInit()
{
	wxString pathXml, pathIni;
	wxSetWorkingDirectory(wxString(argv[1]).AfterFirst('='));
    setlocale(LC_NUMERIC,"C"); //PARA LOS INSERTS DE WXODBC
	Config = new CConfig();
    CPanel::SetLauncher( this );
    wxImage::AddHandler( new wxGIFHandler );

    main_thread = new MainThread();
    main_thread->Create( this );
    main_thread->activate_from = 1;
    main_thread->Run();
    while( !init_core ) {
		#ifdef INVEL_W
			Sleep(10);
		#else
			usleep(10000);
		#endif
    }
	
	MsgPantalla_thread = new MsgPantalla( );
    MsgPantalla_thread->Create();
    MsgPantalla_thread->SetPriority( 50 );
    MsgPantalla_thread->Run();

	pathXml.sprintf("%sproperties.xml", Config->getDirSkin());
	pathIni.sprintf("%sproperties.ini", Config->getDirSkin());
	if( CREATE_PANTALLA_INICIO(Config->getDirSkin()) ) {
		try {
			//configtps = new CConfigTPS("conftps.xml");
			//configtps->GetConfig_tps(config_tps);
		}
		catch( int e ) {
			glog("ERROR ARCHIVO conftps.xml",LOG_ERRORES,1);
		}
		catch(char* c) {
			char mensaerror[100];
			memset(mensaerror,0,100);
			_snprintf(mensaerror,100,"ERROR conftps.xml %50s",c);
			glog(mensaerror,LOG_ERRORES,1);

		}
		panInicio->setProceso( "Cargando properties.xml..." );
		properties = new CDesktop( pathXml.c_str());
		panInicio->setProceso( "Cargando properties.ini..." );
	}
	else {
		return false;
	}
	if( !CREATE_PROPERTIES((char *)pathIni.c_str()) ) {
        sglog("\nNO SE PUDO ABRIR properties.ini\n");
        return false;
    }

	panInicio->setProceso( "Iniciando core..." );
    //#ifdef INVEL_W
	init_core = 2;
    while( !core_iniciado ) {
		#ifdef INVEL_W
			Sleep(1);
		#else
			usleep(10000);
		#endif
    }
    //#endif
    CREATE_PANTALLA();
    panInicio->setProceso( "Creando browser..." );
    CREATE_BROWSE_PANEL( this );

    SetTopWindow( panPres );

    /*#ifdef INVEL_L
    com_thread = new CIMain();
    #else*/
    com_thread = new CIMain( Config->get_IpJavaServer() );
    com2_thread = new MsgInMain( Config->get_IpJavaServer(), 2749);
    /*#endif*/

    core_iniciado = 2;
    com_thread->Create();
    com_thread->SetPriority( 40 );
    com_thread->Run();
    com2_thread->Create();
    com2_thread->SetPriority( 70 );
    com2_thread->Run();
    panInicio->setProceso( "Finalizando Carga..." );

    DataTime_thread = new CDataTime();
    DataTime_thread->Create();
    DataTime_thread->SetPriority( 50 );
    DataTime_thread->Run();
	
	video_thread = new VideoPlayer();
	if( video_thread->HayVideos( ) == true ) {
		video_thread->Create();
		video_thread->SetPriority( 50 );
		video_thread->Run();
	} else {
		delete video_thread;
	}

	while( !panPres->isShowing( ) ) {
		#ifdef INVEL_W
			wxYield();
		#else
			usleep(10000); 
		#endif 
	}

    MsgEmerg_thread = new MsgEmergentes(panPres->GetPosition() + wxPoint(panPres->GetSize().GetWidth(),panPres->GetSize().GetHeight()) );
    MsgEmerg_thread->Create();
    MsgEmerg_thread->SetPriority( 50 );
    MsgEmerg_thread->Run();
	
	init_archivos = 0;
    Ftp_thread = new CFtp(panPres->GetPosition());
    Ftp_thread->Create();
    Ftp_thread->SetPriority( 50 );
    Ftp_thread->Run();
	init_archivos = 1;
    /*Sleep(1000);
    Ftp_thread->setIpOrNameServer("ftp.invel.com.ar");
    Ftp_thread->setIdComandoScript(1);
    Ftp_thread->ObtenerFtp("ftp_posvisual","omgt315425","/parche/val-caja.cfg","../val-cajaftp.cfg",1);
    Ftp_thread->Resume();*/
    //delete properties;

    return true;
}



int AppMain::OnExit()
{
    return 1;
}

#define EVTD_SHOW 1
#define EVTD_FOCUS 2
#define EVTD_HIDE 3
//#define EVTD_PAUSE 3
//#define EVTD_START 4

void AppMain::OnDispatchEvent( wxCommandEvent& event )
{
    CPanel *panel;
    CInput *input;
    pair<char *,char *> *p;

    switch( event.GetInt() ) {
        case EVTD_SHOW:
            p = (pair<char *,char *> *) event.GetClientData();
            panel = (CPanel *) p->first;
            input = (CInput *) p->second;
            if( panel ) {
                panel->Show( true );
            }
            if( input ) {
                input->SetFocus();
            }
            delete p;
            break;
        case EVTD_FOCUS:
            input = (CInput *) event.GetClientData();
            if( input ) {
                input->SetFocus();
            }
            break;
        case EVTD_HIDE:
            panel = (CPanel *) event.GetClientData();
            if(panel) {
                panel->Hide();
            }
        default:
            break;
    }
}


bool AppMain::ShowPanel(CPanel *panel,CInput *input)
{
    pair<char *,char *> *p = new pair<char *,char *>;
    wxCommandEvent event( wxPROTO_DISPATCH_EVENT );
    event.SetInt( EVTD_SHOW );
    p->first = (char *) panel;
    p->second = (char *) input;
    event.SetClientData( (void *) p );
    wxPostEvent( this, event );
    return true;
}

bool AppMain::HidePanel(CPanel *panel)
{
    wxCommandEvent event( wxPROTO_DISPATCH_EVENT );
    event.SetInt( EVTD_HIDE );
    event.SetClientData( (void *) panel );
    wxPostEvent( this, event );
    return true;
}

bool AppMain::ChangeFocus(CInput *input)
{
    wxCommandEvent event( wxPROTO_DISPATCH_EVENT );
    event.SetInt( EVTD_FOCUS );
    event.SetClientData( (void *) input );
    wxPostEvent( this, event );
    //    Sleep(1000);
    return true;
}

bool AppMain::Launch()
{
    return true;
}

void AppMain::OnKey(wxKeyEvent& event)
{
    MainThread::m_mutex->Lock();

    /*int alt_chrs[]={-30,-48,-46, 114, 120, 113, 112, 111, 0,
    110, 109, 108, 0, 0, 115, -23, 122, 117, -31, 118, 116,
    0, 0, 121, 0, 0, 0};
    */
                //ALT+                                                             M    N
    int alt_chrs[]= { -30, -48, -46, -32, -18, -33, -34, -35, -23, -36, -37, -38, -39, -40, -24, -25,
                      -16, -19, -31, -20, -22, 0, -17, 0, -21, 0 };
    int key;

    key = event.GetKeyCode();
    MainThread::activate_from=1;
    if( event.AltDown() ) {
        if( event.GetKeyCode() >= 97 ) {
            CInput::setKey( alt_chrs[event.GetKeyCode() - 97] );
            MainThread::m_mutex->Unlock();
            //nico_signal MainThread::m_condition->Signal();
            return;
        }
        else if( event.GetKeyCode() == 43 ) {
            CInput::setKey( 126 );
            MainThread::m_mutex->Unlock();
            //nico_signal MainThread::m_condition->Signal();
            return;
        }
    }

    /*----------------------------------------------------*/

    if( event.GetKeyCode() == 124 ) {
        CInput::setKey( 132 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    /*----------------------------------------------------*/

    if( event.GetKeyCode() >= WXK_F1 && event.GetKeyCode() <= WXK_F10 ) {
        CInput::setKey( - event.GetKeyCode() + WXK_F1 - 59);
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if(event.GetKeyCode() >= WXK_F11 &&  event.GetKeyCode() <= WXK_F12) {
        CInput::setKey( - event.GetKeyCode() + WXK_F11 - 133 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }

    if( event.GetKeyCode() == WXK_BACK ) {
        if( CInput::sizeKey() > 0 ) {
            CInput::removeKey();
        }
        CInput::setKey( WXK_BACK );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_UP ) {
        CInput::setKey( -72 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_DOWN ) {
        CInput::setKey( -80 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_LEFT ) {
        CInput::setKey( -74 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_RIGHT ) {
        CInput::setKey( -75 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_HOME ) {
        CInput::setKey( -76 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_END ) {
        CInput::setKey( -77 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == 313 ) {
        CInput::setKey( -81 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == 312 ) {
        CInput::setKey( -73 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
	if( event.GetKeyCode() == 324 ) {// doble cero 00, verificar para teclados distintos del kb78
        CInput::setKey( -82 );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() - 55 == WXK_MULTIPLY ) {
        CInput::setKey( '*' );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() - 55 == WXK_ADD ) {
        CInput::setKey( '+' );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_SEPARATOR ) {
        CInput::setKey( '|' );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_SUBTRACT ) {
        CInput::setKey( '-' );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }
    if( event.GetKeyCode() == WXK_DIVIDE ) {
        CInput::setKey( '/' );
        MainThread::m_mutex->Unlock();
        //nico_signal MainThread::m_condition->Signal();
        return;
    }

    // tecla por defecto
    if( /*(event.GetKeyCode() != WXK_TAB) &&*/ (event.GetKeyCode() < 300) ) {
        CInput::setKey( event.GetKeyCode() );
    }

    MainThread::m_mutex->Unlock();
    //nico_signal MainThread::m_condition->Signal();
}

void AppMain::Dummy( wxMouseEvent &event )
{
}

void AppMain::Dummy1( wxScrollEvent &event )
{
}

void AppMain::liberar( )
{
	this->Yield(true);
}


