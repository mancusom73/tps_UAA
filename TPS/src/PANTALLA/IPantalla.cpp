/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <fstream> 
#ifdef INVEL_L
#include <unistd.h>
#endif
extern "C"
{
	#include <cmemory.h>
	#include <pant.h>
	#include <log.h>
	#include <_cr0.h>
    #include <cio.h>
	#include <get_fech.h>
	#include <day.h>
	extern int GET_POSICION_PANTALLAS_X(void);
	extern int GET_POSICION_PANTALLAS_Y(void);
	extern int SET_POSICION_PANTALLAS_X(int x );
	extern int SET_POSICION_PANTALLAS_Y(int y);
	extern int DICCIO_CARGADO;
	extern int win_act_ontop;

}
//#include <LOG.h>
#include <_cr0.h>
//#include <PANT.h>
#define Uses_wxThread

#include <config.h>
#include <CFtp.h>
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#pragma pack(1)
#include <cr.h>
#pragma pack(8)
#include "MainThread.h"
#include "CGrid.h"
#include "CLabel.h"
#include "CInput.h"
#include "AppMain.h"
#include "actions.h"
#include "typeacti.h"
#include <sys/stat.h>
#include "CIMain.h"
#include "MsgInMain.h"
//#include "ini.h"

#include "MainPanel.h"

#include "PanelInicio.h"
#include "PanelVentas.h"
#include "PanelPagos.h"
#include "PanelCierre.h"
#include "PanelCobro.h"
#include "PanelPagosCaja.h"
#include "PanelFondoFijo.h"
#include "PanelRetiros.h"
#include "PanelStandby.h"
#include "PanelCierreSubmedios.h"
#include "PanelPresentacion.h"
#include "PanelInput.h"
#include "PanelMenu.h"
#include "CBrowse.h"
#include "PanelCambioMedio.h"
#include "PanelCobrosEsp.h"
#include "PanelDeudaSocio.h"
#include "PanelVuelto.h"
#include "PanelDenominaciones.h"
#include "PanelCheques.h"
#include "PanelImportacion.h"
#include "PanelGrill.h"
#include "PanelPagosEsp.h"
#include "PanelMedioReint.h"
#include "CConfig.h"
#include "path.h"
#include "PanelPagaTodo.h"
#include "PanelGarantia.h"
#include "PanelConsultaPromos.h"
#include "PanelScreenSaver.h"
#include <wx/utils.h>
#include "MsgEmergentes.h"
#include "MsgPantalla.h"
#include <io.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include "VideoPlayer.h"
extern CIMain *com_thread;
extern MsgInMain *com2_thread;
//#define CORRIMIENTO_PANTALLA
extern CConfig *Config;
extern CFtp *Ftp_thread;
extern VideoPlayer *video_thread;
CWindow *browseWin;
PanelInicio *panInicio;

PanelVentas *panVentas;
PanelPagos *panPagos;
PanelCierre *panCierre;
PanelCobro *panCobro;
PanelPagosCaja *panPagosCaja;
PanelFondoFijo *panFondoFijo;
PanelRetiros *panRetiros;
PanelStandby *panStandby;
PanelCierreSubmedios *panCierreSub;
PanelPresentacion *panPres;
PanelInput *panInput;
PanelMenu *panMenu;
CPanelBrowse *panBrowse;
PanelCambioMedio *panCambioMedio;
PanelCobrosEsp *panCobrosEsp;
PanelDeudaSocio *panDeudaSocio;
PanelVuelto *panVuelto;
PanelCheques *panCheques;
PanelImportacion *panImportacion;
PanelGrill *panGrill;
PanelPagosEsp *panPagosEsp;
PanelMedioReint *panMedioReint;
PanelPagaTodo *panPagaTodo;
PanelGarantia *panGarantia;
PanelDenominaciones *panDenominaciones;
PanelConsultaPromos *panConsultaPromos;
PanelScreenSaver *panScreenSaver;

int WIN_ACTIVA_ONTOP(void);
const int pantalla_borde = 0;
#define STRLEN_LOG     1024
extern CDesktop *properties;
extern MsgEmergentes *MsgEmerg_thread;
extern MsgPantalla *MsgPantalla_thread;
static wxMutex s_mutexLog;
int mReintento_aux = 0;
wxSize *TamanioVentana;
void MostrarMemoria( );
CConfigTPS *configtps;
char * GetAppExePath();

void CREATE_PANTALLA()
{
    CWindow *window;
	wxPoint PosicionVentasXY;
	int x=GET_POSICION_PANTALLAS_X();
	int y=GET_POSICION_PANTALLAS_Y();
	PosicionVentasXY = wxPoint(x,y) ;/*wxDefaultPosition*/
	

    panInicio->setProceso( "Creando panel ventas..." );
	
    window = properties->GetWindows()->GetWindowByName( "ventas" );
	/**********declaracion de una variable con los datos de loc. de Pant.Ventas ***/
		TamanioVentana= new wxSize(0,0);
		TamanioVentana->Set(GET_POSICION_PANTALLAS_X(),GET_POSICION_PANTALLAS_Y());
		TamanioVentana->SetHeight(window->GetSize()->GetHeight() + pantalla_borde);
		TamanioVentana->SetWidth(window->GetSize()->GetWidth());
	/*******************************************************************************/
    panVentas = new PanelVentas( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                 wxSize( window->GetSize()->GetWidth(),
                                         window->GetSize()->GetHeight() + pantalla_borde ),
                                 wxMINIMIZE_BOX );
	if( GET_POSICION_PANTALLAS_X() + GET_POSICION_PANTALLAS_Y() == 0){
		panVentas->Center();
		PosicionVentasXY = wxPoint(panVentas->GetPosition().x,panVentas->GetPosition().y) ;
		SET_POSICION_PANTALLAS_X(PosicionVentasXY.x );
		SET_POSICION_PANTALLAS_Y(PosicionVentasXY.y);
	}
	
	panVentas->createLabels();
	
	
	panInicio->setProceso( "Creando panel cobros especiales..." );
	
    window = properties->GetWindows()->GetWindowByName( "cobrosespeciales" );
	
	
    panCobrosEsp = new PanelCobrosEsp( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                       wxSize( window->GetSize()->GetWidth(),
                                               window->GetSize()->GetHeight() + pantalla_borde ),
                                       wxMINIMIZE_BOX );
    panCobrosEsp->createLabels();

    panInicio->setProceso( "Creando panel pagos..." );
    window = properties->GetWindows()->GetWindowByName( "pagos" );
	panPagos = new PanelPagos( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                               wxSize( window->GetSize()->GetWidth(),
                                       window->GetSize()->GetHeight() + pantalla_borde ),
                               wxMINIMIZE_BOX );
	//| wxSTAY_ON_TOP  para tener la ventana ONTOP pero los browsers quedan atras ojo

	panPagos->createLabels();

    panInicio->setProceso( "Creando panel cierre..." );
    window = properties->GetWindows()->GetWindowByName( "cierre_z" );
    panCierre = new PanelCierre( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                 wxSize( window->GetSize()->GetWidth(),
                                         window->GetSize()->GetHeight() + pantalla_borde ),
                                 wxMINIMIZE_BOX );
    panCierre->createLabels();

    panInicio->setProceso( "Creando panel cobro..." );
    window = properties->GetWindows()->GetWindowByName( "cobros" );
    panCobro = new PanelCobro( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                               wxSize( window->GetSize()->GetWidth(),
                                       window->GetSize()->GetHeight() + pantalla_borde ),
                               wxMINIMIZE_BOX );
    panCobro->createLabels();

    panInicio->setProceso( "Creando panel pagos por caja..." );
    window = properties->GetWindows()->GetWindowByName( "pagoscaja" );
    panPagosCaja = new PanelPagosCaja( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                       wxSize( window->GetSize()->GetWidth(),
                                               window->GetSize()->GetHeight() + pantalla_borde ),
                                       wxMINIMIZE_BOX );
    panPagosCaja->createLabels();

    panInicio->setProceso( "Creando panel fondo fijo..." );
    window = properties->GetWindows()->GetWindowByName( "fondofijo" );
    panFondoFijo = new PanelFondoFijo( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                       wxSize( window->GetSize()->GetWidth(),
                                               window->GetSize()->GetHeight() + pantalla_borde ),
                                       wxMINIMIZE_BOX );
    panFondoFijo->createLabels();

    panInicio->setProceso( "Creando panel retiros..." );
    window = properties->GetWindows()->GetWindowByName( "retiros" );
    panRetiros = new PanelRetiros( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                   wxSize( window->GetSize()->GetWidth(),
                                           window->GetSize()->GetHeight() + pantalla_borde ),
                                   wxMINIMIZE_BOX );
    panRetiros->createLabels();

    panInicio->setProceso( "Creando panel browse..." );
    panBrowse = new CPanelBrowse( "Consulta", PosicionVentasXY/*wxPoint( 0, 0 )*/, wxSize( 15 + 30 * 2, 350 ));
	/*********es distinto el size que Pant.Ventas por lo cual se debe centrar resp. de ella ***/
		panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(panBrowse->GetSize().GetWidth())/2), 
		GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(panBrowse->GetSize().GetHeight()/2)),0);
	/*******************************************************************************************/
	panBrowse->createLabels();

    panInicio->setProceso( "Creando panel standby..." );
    window = properties->GetWindows()->GetWindowByName( "cierre_z" );
    panStandby = new PanelStandby( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                   wxSize( window->GetSize()->GetWidth(),
                                           window->GetSize()->GetHeight() + pantalla_borde ),
                                   wxMINIMIZE_BOX );
    panStandby->createLabels();

    panInicio->setProceso( "Creando panel cierre sub..." );
    window = properties->GetWindows()->GetWindowByName( "cierresubmedio" );
    panCierreSub = new PanelCierreSubmedios( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                             wxSize( window->GetSize()->GetWidth(),
                                                     window->GetSize()->GetHeight()
                                                   + pantalla_borde ), wxMINIMIZE_BOX );
    panCierreSub->createLabels();

    panInicio->setProceso( "Creando panel presentacion..." );
    window = properties->GetWindows()->GetWindowByName( "presentacion" );
    panPres = new PanelPresentacion( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                     wxSize( window->GetSize()->GetWidth(),
                                             window->GetSize()->GetHeight() + pantalla_borde ),
                                     wxMINIMIZE_BOX );    
    panPres->createLabels();
	
	panInicio->setProceso( "Creando panel screenSaver..." );
	window = properties->GetWindows()->GetWindowByName( "screensaver" );
    panScreenSaver = new PanelScreenSaver( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                   wxSize( window->GetSize()->GetWidth(),
                                           window->GetSize()->GetHeight() + pantalla_borde ),
                                   wxMINIMIZE_BOX  );
    panScreenSaver->createLabels();


	//Se descomenta wxSTAY_ON_TOP porque el input para NRO. LINEA solicitado en Cobros Kinacu
	//estaba superpuesta (no se veia) por la pantalla de cobros.
    panInicio->setProceso( "Creando panel input..." );

	
    panInput = new PanelInput( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/, wxSize( 200, 200 ),
                               wxSIMPLE_BORDER | wxSTAY_ON_TOP );
	
	panInput->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(panInput->getWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(panInput->getHeight()/2)),0);

	panInput->createLabels();

    panInicio->setProceso( "Creando panel cambio de medio..." );
    window = properties->GetWindows()->GetWindowByName( "cambiomedio" );
    panCambioMedio = new PanelCambioMedio( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                           wxSize( window->GetSize()->GetWidth(),
                                                   window->GetSize()->GetHeight()
                                                 + pantalla_borde ), wxMINIMIZE_BOX );
    panCambioMedio->createLabels();

    panInicio->setProceso( "Creando panel menu..." );
    window = properties->GetWindows()->GetWindowByName( "menu" );
    panMenu = new PanelMenu( "Invel Input", PosicionVentasXY/*wxDefaultPosition*/,
                             wxSize( window->GetSize()->GetWidth(),
                                     window->GetSize()->GetHeight() ) );
	
	panMenu->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(panMenu->GetSize().GetWidth())/2), 
		GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(panMenu->GetSize().GetHeight()/2)),0);
    panMenu->createLabels();

    panInicio->setProceso( "Creando panel pagos especiales..." );
    window = properties->GetWindows()->GetWindowByName( "pagosespeciales" );
    panPagosEsp = new PanelPagosEsp( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                     wxSize( window->GetSize()->GetWidth(),
                                             window->GetSize()->GetHeight() + pantalla_borde ),
                                     wxMINIMIZE_BOX );
    panPagosEsp->createLabels();

    panInicio->setProceso( "Creando panel medios reintegro..." );
    window = properties->GetWindows()->GetWindowByName( "mediosreintegros" );
    panMedioReint = new PanelMedioReint( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                         wxSize( window->GetSize()->GetWidth(),
                                                 window->GetSize()->GetHeight() + pantalla_borde ),
                                         wxMINIMIZE_BOX );
    panMedioReint->createLabels();

    panInicio->setProceso( "Creando panel deuda socio..." );
    window = properties->GetWindows()->GetWindowByName( "deudasocio" );
//    panDeudaSocio = new PanelDeudaSocio( "TPV 1000 - IV", wxPoint( 430, 90 ),
//                                         wxSize( window->GetSize()->GetWidth(),
//                                                 window->GetSize()->GetHeight() + pantalla_borde ),
//                                         wxSTAY_ON_TOP );
    panDeudaSocio = new PanelDeudaSocio( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                         wxSize( window->GetSize()->GetWidth(),
                                                 window->GetSize()->GetHeight() + pantalla_borde ),
                                         wxMINIMIZE_BOX );
	panDeudaSocio->createLabels();

    panInicio->setProceso( "Creando panel vueltos..." );
	window = properties->GetWindows()->GetWindowByName( "vueltos" );
	panVuelto = new PanelVuelto( "TPV 1000 - IV", PosicionVentasXY/*wxDefaultPosition*/,
                                         wxSize( 330, 345 ),
                                         wxMINIMIZE_BOX );
	panVuelto->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(panVuelto->GetSize().GetWidth())/2), 
		GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(panVuelto->GetSize().GetHeight()/2)),0);
	
	panVuelto->createLabels();
	
	panInicio->setProceso( "Creando panel denominaciones..." );
    window = properties->GetWindows()->GetWindowByName( "denominaciones" );
	panDenominaciones = new PanelDenominaciones( "DENOMINACIONES", PosicionVentasXY/*wxDefaultPosition*/,
                                         wxSize( 330, 410 ),
                                         wxMINIMIZE_BOX );
	panDenominaciones->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(panDenominaciones->GetSize().GetWidth()/2)), 
		GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(panDenominaciones->GetSize().GetHeight()/2)),0);


	
	panDenominaciones->createLabels();
#ifdef CORRIMIENTO_PANTALLA
    panPagos->Center();
#endif

    panInicio->setProceso( "Creando panel cheques..." );
    window = properties->GetWindows()->GetWindowByName( "cheques" );
    panCheques = new PanelCheques( "TPV 1000 - IV", panPagos->GetPosition(),
                                         wxSize( window->GetSize()->GetWidth(),
                                                 window->GetSize()->GetHeight() + pantalla_borde ),
                                         wxMINIMIZE_BOX );
    panCheques->createLabels();

    window = properties->GetWindows()->GetWindowByName( "Importacion" );
    panImportacion = new PanelImportacion( "TPV 1000 - IV", panVentas->GetPosition(),
                                         wxSize( window->GetSize()->GetWidth(),
                                                 window->GetSize()->GetHeight() + pantalla_borde ),
                                         wxMINIMIZE_BOX );
    panImportacion->createLabels();
    
	panGrill = new PanelGrill( "TPV 1000 - IV", PosicionVentasXY/*wxPoint( 700, 300 )*/, wxSize( 170, 200 ),
                               wxSIMPLE_BORDER | wxSTAY_ON_TOP );

	
	panGrill->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(panGrill->getWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(panGrill->getHeight()/2)),0);


    panGrill->createTitle();

	panInicio->setProceso( "Creando sub paneles cobros..." );
	window = properties->GetWindows()->GetWindowByName( "PanelPagaTodo" );
    panPagaTodo = new PanelPagaTodo( "TPV 1000 - IV", panPagos->GetPosition() + wxPoint( 340, 113 ),
                                         wxSize( window->GetSize()->GetWidth(),
                                                 window->GetSize()->GetHeight() + pantalla_borde ),
                                         wxNO_BORDER | wxSTAY_ON_TOP );

	panPagaTodo->createLabels();

	window = properties->GetWindows()->GetWindowByName( "PanelGarantia" );
    panGarantia = new PanelGarantia( "TPV 1000 - IV", panPagos->GetPosition() + wxPoint( 340, 113 ),
                                         wxSize( window->GetSize()->GetWidth(),
                                                 window->GetSize()->GetHeight() + pantalla_borde ),
                                         wxNO_BORDER | wxSTAY_ON_TOP );

	panGarantia->createLabels();

    panInicio->setProceso( "Creando panel consulta de promociones..." );
	window = properties->GetWindows()->GetWindowByName( "ConsultaPromos" );
	panConsultaPromos = new PanelConsultaPromos( "TPV 1000 - IV", panPagos->GetPosition(),
                                         wxSize( window->GetSize()->GetWidth(),
                                                 window->GetSize()->GetHeight() + pantalla_borde ),
                                         wxMINIMIZE_BOX | wxSTAY_ON_TOP );
	//panConsultaPromos->Centre( wxBOTH );
    panConsultaPromos->createLabels();


#ifdef CORRIMIENTO_PANTALLA	    
    panVentas->Center();
    panCierre->Center();
    panCobro->Center();
    panPagosCaja->Center();
    panFondoFijo->Center();
    panRetiros->Center();
    panStandby->Center();
    panCierreSub->Center();
    panPres->Center();
	panVuelto->Center();
	panDenominaciones->Center();
    if(!Config->get_touch_screen()){
		panBrowse->Center();
	}
    panImportacion->Center();
    panInput->Center();
    panMenu->Center();
    panCambioMedio->Center();
    panCobrosEsp->Center();
    panDeudaSocio->Center();
    panPagosEsp->Center();
    panMedioReint->Center();
    //panGrill->Center();
	panConsultaPromos->Center();
	panScreenSaver->Center();
#endif    
    
}

bool CREATE_PANTALLA_INICIO(char *path)
{
    wxBitmap bitmap;
	wxString image;
	bool exito;
	image.sprintf("%sinicio.gif",path);
    if( exito = bitmap.LoadFile( image/*IMAGE_INICIO*/, wxBITMAP_TYPE_GIF ) ) {
        panInicio = new PanelInicio( bitmap, image );
    }
    wxYield();
	return exito;
}


MainPanel frame;

extern "C"
{
#ifdef _DEBUG
    /** Herramienta para determinar que thread esta ejecutando un segmento
      * de codigo C determinado.
      */
    int currThread() {
        if( wxThread::This()->IsMain() ) {
            return 0;
        }else{        
   	        return wxThread::This()->GetId();
        }
        return 0;
    }
#endif
    void WAIT_INPUT()
    {
    }

    int getData( int action, char *buffer, int len )
    {
        int rta = 0;
        int key = -999;
        string aux;

        MainThread::m_mutex->Lock();
		if( MODO_NEGOCIO != SERVICIOS ) {
            switch( action ) {
				case ACTION_DETECT_ERROR_PRINTER:
					rta = com_thread->getErrorPrinter( );
					break;
				case ACTION_IS_SCANNER:
					rta = com_thread->isScanner( NULL );
					break;
				case ACTION_READ_DATA_SCANNER:
					rta = com_thread->getScanner( buffer );
                    //rta = 1;
					break;
				case ACTION_CLEAN_BUFFER_SCANNER:
					/**/glog("com_thread->cleanBufferScanner() ...",LOG_JAVA_SERVER,5);
					rta = com_thread->cleanBufferScanner();
					/**/glog("... com_thread->cleanBufferScanner()  OK",LOG_JAVA_SERVER,5);
                    //rta = 1;
					break;
				case ACTION_KEYLOCK_POSITION_COUNT:
					rta = com_thread->getPositionCountKeyLock( *buffer );
					key = 1;
                    break;
				case ACTION_KEYLOCK_GET_POSITION:
					rta = com_thread->getPositionKeyLock( *buffer );
					key = 1;
                    break;
				case ACTION_KEYLOCK_WAIT_POSITION:
                    com_thread->WaitForPositionKeyLock( *buffer );
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_READ:
                    if( MainThread::activate_from == 1 ) {
                        key = frame.getch();
                        *( int* )buffer = key;
                        rta = 1;
                    }
                    else if( MainThread::activate_from == 2 ) {
                        *( int* )buffer = -999;
                        rta = 1;
					} else {
						rta = 1;
					}
                    break;

                case ACTION_IS_MSR:
                    //rta = ( com_thread->IsEmpty( FROM_MSR_TRACK1 ) ) ? 0 : 1;
                    rta = !com2_thread->GetMSREmpty();
                    key = 1;
                    MainThread::m_mutex->Unlock();
                    return rta;
                    break;
                case ACTION_READ_DATA_MSR_TRACK1:
                    aux = com2_thread->GetMSRTrack1();
                    /**/glog("CEREANDO TRACK1",LOG_JAVA_SERVER,4);
                    memset( buffer, 0, len );
                    //buffer[12] = 0;
                    /**/glog("COPIANDO BUFFER EN TRACK1",LOG_JAVA_SERVER,4);
                    strncpy(buffer,aux.c_str(),(len - 1));
                    //memcpy( buffer, aux.c_str(), len ); 
                    //buffer[len - 1] = '\0';
                    /**/glog("FIN COPIANDO BUFFER EN TRACK1",LOG_JAVA_SERVER,4);
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_READ_DATA_MSR_TRACK2:
                    aux = com2_thread->GetMSRTrack2();
                    /**/glog("CEREANDO TRACK2",LOG_JAVA_SERVER,4);
                    memset( buffer, 0, len );
                    /**/glog("COPIANDO BUFFER EN TRACK2",LOG_JAVA_SERVER,4);
                    //buffer[12] = 0;
                    //memcpy( buffer, aux.c_str(), len );
                    strncpy(buffer,aux.c_str(),(len - 1));
                    //buffer[len - 1] = '\0';
                    /**/glog("FIN COPIANDO BUFFER EN TRACK2",LOG_JAVA_SERVER,4);
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_DRAWER_OPENED:
                    bool re;
                    re = com_thread->GetDrawerOpened( "cajon" );
                    if( re ) {
                        *( int* )buffer = 1;
                    }
                    else {
                        *( int* )buffer = 0;
                    }
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_WAIT_FOR_DRAWER_CLOSE:
                    com_thread->WaitForDrawerClose( "cajon" );
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_READ_PRINTER_STATUS:
                    *( int* )buffer = com_thread->GetPrinterStatus();
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_SLP_EMPTY:
                    *( int* )buffer = com_thread->GetSlpEmpty();
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_PRINT_FINISH:
                    *( int* )buffer = com_thread->GetPrinterFinish();
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_READ_WEIGHT:
                    int peso;
                    peso = com_thread->ReadWeight();
                    *( int* )buffer = peso;
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_GET_FISCAL_STATE:
                    rta = com_thread->GetFiscalData( 0, buffer );
                    //rta = 1;
                    key = 1;
                    break;
				case ACTION_GET_FISCAL_ACUM:
					rta =com_thread->GetFiscalData( 4, buffer );
                    //rta = 1;
                    key = 1;
					break;
                case ACTION_GET_TERMINAL_NUM:
                    com_thread->GetFiscalData( 1, buffer );
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_GET_Z_NUM:
                    com_thread->GetFiscalData( 2, buffer );
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_GETCH_KEY:
                    //key = com_thread->GetchKey();
                    key = com2_thread->getKey();
                    *( int* )buffer = key;
                    rta = 1;
                    break;
                case ACTION_GET_CHARSET:
                    *( int* )buffer = com_thread->getCharSet();
                    break;
                case ACTION_GET_PRINT_STATION:
                     int station;
                     station = com_thread->getPrinterStation( );
                     *( int* )buffer = station;
                    break;
                case ACTION_GET_PRINT_LETTER:
                     int letra;
                     letra = com_thread->getPrinterLetter( );
                     *( int* )buffer = letra;
                    break;
				case ACTION_FISCAL_OBTENER_FECHA_HORA:
					com_thread->getDateFechaHora( buffer );
					break;
				case  ACTION_PINPAD_VERSION:
					rta = com_thread->pinpad_version( buffer );
					break;
				case  ACTION_PINPAD_PEDIR_PIN:
					rta = com_thread->pinpad_pedir_pin( buffer );
					break;
				case  ACTION_PINPAD_GET_KEY:
					rta = com_thread->pinpad_get_key( buffer );
					break;
				case  ACTION_PINPAD_GET_STRING:
					break;
				case  ACTION_PINPAD_ONLINE:
					rta = com_thread->pinpad_online(buffer);
					break;
                default:
                    rta = 1;
                    key = 1;
                    break;
            }
        }
        else {
            //MODO_NEGOCIO != SERVICIOS
            switch( action ) {
                case ACTION_READ:
                    if( MainThread::activate_from == 1 ) {
                        key = frame.getch();
                        *( int* )buffer = (key < 1000 && key > -1000) ? key : 0;
                        rta = 1;
                    }
                    else if( MainThread::activate_from == 2 ) {
                        *( int* )buffer = -999;
                        rta = 1;
                    }
                    break;
                case ACTION_READ_PRINTER_STATUS:
                    *( int* )buffer = com_thread->GetPrinterStatus();
                    rta = 1;
                    key = 1;
                    break;
                case ACTION_GETCH_KEY:
                    //key = com_thread->GetchKey();
                    key = com2_thread->getKey();
                    *( int* )buffer = key;
                    rta = 1;
                    break;
                default:
                    rta = 1;
                    key = 1;
                    break;
            }
        }
        if( MainThread::activate_from == 3 ) {
            MainThread::m_mutex->Unlock();
            MainThread::activate_from = 0;
            //break;
        }
        //nico_signal MainThread::m_condition->Wait(1);
        MainThread::Sleep( 1 );
        /*if( !rta || key == -999 ) {
            MainThread::activate_from = 0;
            MainThread::m_condition->Wait();
        }*/
        MainThread::m_mutex->Unlock();
        return rta;
    }
    int setData( int action, char *buffer, int len )
    {
        int rta = 0;
        if( MODO_NEGOCIO != SERVICIOS ) {
			switch( action ) {
				case ACTION_PRINT_EXISTE_POS_PRINTER:
					com_thread->setExistePosPrinter( buffer );
                    break;
				case ACTION_PRINT_BAR_CODE:
					com_thread->PrintBarCode(  );
                    break;
				case ACTION_SET_PRINT_BAR_CODE:
					com_thread->SetPrintBarCode( buffer );
                    break;
				case ACTION_PRINT_BITMAP:
					com_thread->PrintBitmap( buffer );
                    break;
				case ACTION_SET_PRINT_BITMAP:
					rta = com_thread->SetPrintBitmap( buffer );
                    break;
				case ACTION_HABILITA_SCANNER:
                    com_thread->scanOut.PutData( buffer );
                    break;
                case ACTION_SET_DATA_PRINTER:
                    com_thread->SetDataToPrint( TO_POS_PRINTER, ( string )buffer );
                    break;
                case ACTION_SET_HEADER:
                    com_thread->SetDataToPrint( TO_POS_PRINTER, ( string )buffer );
                    break;
                case ACTION_OPEN_DRAWER:
                    com_thread->OpenDrawer( "cajon" );
                    break;
                case ACTION_PRINT:
                    com_thread->Print();
                    break;
                case ACTION_SET_PRINT_POS:
                    com_thread->Print( atoi( buffer ) );
                    break;
                case ACTION_PRINT_LF:
                    com_thread->PrintLF( buffer, len );
                    break;
                case ACTION_PRINT_CUT_PAPER:
                    com_thread->PrintCutPaper(IMPRIMIR_TICKET_AL_FINAL);
                    break;
                case ACTION_PRINT_MODE:
                    com_thread->PrintMode( atoi( buffer ) );
                    break;
                case ACTION_SET_PRINT_STATION:
                    com_thread->PrintSelect( atoi( buffer ) );
                    break;
                case ACTION_PRINT_CENTER:
                    com_thread->PrintAlign( 100 );
                    break;
                case ACTION_PRINT_NORMAL:
                    com_thread->PrintAlign( 101 );
                    break;
                case ACTION_SET_DISPLAY_TEXT_LINE_1:
                    com_thread->DisplayText( buffer, 12 );
                    break;
                case ACTION_SET_DISPLAY_TEXT_LINE_2:
                    com_thread->DisplayText( buffer, 13 );
                    break;
                case ACTION_SET_DISPLAY_CLEAR_TEXT:
                    com_thread->ClearDisplayText();
                    break;
                case ACTION_HABILITA_MSR:
                    com_thread->HabilitarMsr( buffer );
                    break;
                case ACTION_HABILITA_SCALE:
                    com_thread->HabilitarScale( buffer );
                    break;
                case ACTION_BEGIN_FISCAL_REC:
                    com_thread->BeginFiscalReceipt();
                    break;
                case ACTION_END_FISCAL_REC:
                    rta = com_thread->EndFiscalReceipt(buffer);
                    break;
                case ACTION_BEGIN_NON_FISCAL:
                    com_thread->BeginNonFiscal();
                    break;
				case ACTION_UPDATE_ESTADO_DOC_NO_FISCAL:
                    com_thread->UpdateNonFiscal(buffer);
                    break;
				case ACTION_FISCAL_OPEN_CASHDRAWER:
                    rta = com_thread->FiscalOpenCashDrawer();
                    break;
                case ACTION_END_NON_FISCAL:
                    com_thread->EndNonFiscal();
                    break;
                case ACTION_PRINT_REC_ITEM:
                    rta = com_thread->PrintRecItem( buffer );
                    break;
				/*case ACTION_PRINT_REC_ITEM: dnc proximo comando
                    rta = com_thread->PrintRecItem( buffer );setPrinterPref(boolean setSlipDefaultStation, boolean usarDefaultStationParaNoFiscal) 
                    break;									setPaperPref(int cantColumnas, int cantFilas)
															setComprPref(String leyendaPref, boolean activar)*/
				case ACTION_SET_PRINTER_PREF	:
					rta = com_thread->setPrinterPref( buffer );
				break;
				case ACTION_SET_PAPER_PREF		:
					rta = com_thread->setPaperPref( buffer );
				break;
				case ACTION_SET_COMPR_PREF		:
					rta = com_thread->setComprPref( buffer );
				break;
				case ACTION_PRINT_PERCEP_RETEN:
                    rta = com_thread->printRecPercep( buffer );
                    break;
                case ACTION_PRINT_REC_VOID_ITEM:
                    com_thread->PrintRecVoidItem( buffer );
                    break;
				case ACTION_PRINT_REC_VOID_ITEM_FAC:
                    com_thread->printRecItemVoidFac( buffer );
                    break;
				case ACTION_PRINT_BONIF_ITEM_FAC:
					rta = com_thread->printBonItemFac( buffer );
                    break;
				case ACTION_PRINT_ANUL_BONIF_ITEM_FAC:
                    rta =com_thread->printAnulBonifItemFac( buffer );
                    break;
                case ACTION_PRINT_REC_SUBTOTAL:
                    //com_thread->PrintRecSubtotal( buffer );
					com_thread->PrintRecSubtotal( buffer );
					
                    break;
                case ACTION_PRINT_REC_TOTAL:
                    rta = com_thread->PrintRecTotal( buffer );
                    break;
                case ACTION_PRINT_REC_VOID:
                    com_thread->PrintRecVoid( buffer );
                    break;
                case ACTION_PRINT_REC_MESSAGE:
                    rta = com_thread->PrintRecMessage( buffer );
                    break;
                case ACTION_PRINT_X_REPORT:
                    rta = com_thread->PrintXReport(buffer);
                    break;
                case ACTION_PRINT_Z_REPORT:
                    rta = com_thread->PrintZReport( buffer);
                    break;
                case ACTION_PRINT_REC_ADJUST_ITEM:
                    com_thread->PrintRecItemAdjustment( buffer );
                    break;
				case ACTION_PRINT_REC_VOID_ADJUST_ITEM:
                    com_thread->PrintRecItemVoidAdjustment( buffer );
                    break;
                case ACTION_SET_TRAILER_LINE:
                    com_thread->SetTrailerLine( buffer );
                    break;
                case ACTION_SET_HEADER_LINE:
                    com_thread->SetHeaderLine( buffer );
                    break;
                case ACTION_SET_MODO_IMPRESION_FINAL:
                    com_thread->setModoImpresionFinal( SI );
                    break;
                case ACTION_SET_MODO_IMPRESION_NORMAL:
                    com_thread->setModoImpresionFinal( NO );
                    break;
                case ACTION_FORCE_PRINT:
                    com_thread->ForcePrint( );
                    break;
                case ACTION_SET_PRINT_LINE_SPACE:
                    com_thread->setLineSpace( buffer );
                    break;
                case ACTION_SET_PRINT_LINE_CHARS:
                    com_thread->setLineChars( buffer );
                    break;
				case ACTION_PRINT_VOUCHER://Seba
                    com_thread->printVoucher( buffer );
                    break;
				case ACTION_FEED_PAPER://Seba
                    com_thread->feedPaper( buffer );
                    break;
				case ACTION_PRINT_NORMAL_FISCAL://Seba
                    com_thread->PrintNormalFiscal( buffer ); 
                    break;
				case ACTION_PRINT_REC_REFUND://Seba
                    com_thread->printRecRefund( buffer ); 
                    break;
				case ACTION_PRINT_REC_ITEM_FAC://Seba
                    com_thread->printRecItemFac( buffer ); 
                    break;
				case ACTION_PRINT_REC_ITEM_VOID_FAC://Seba
                    com_thread->printRecItemVoidFac( buffer ); 
                    break;
				case ACTION_BEGIN_FAC://Seba
                    com_thread->beginFac( buffer ); 
                    break;
				case ACTION_BEGIN_REPAYMENT_NOTE://Seba
                    com_thread->beginRepaymentNote( buffer ); 
                    break;
				case ACTION_END_FAC://Seba
                    rta = com_thread->endFac( buffer ); 
                    break;
				case ACTION_END_REPAYMENT_NOTE://Seba
                    com_thread->endRepaymentNote( buffer ); 
                    break;
				case ACTION_PRINT_REC_TOTAL_FAC://Seba
                    rta = com_thread->printRecTotalFac( buffer ); 
                    break;
				case ACTION_PRINT_REC_VOID_FAC://Seba
                    com_thread->printRecVoidFac( buffer ); 
                    break;
				case ACTION_PRINT_REC_SUBTOTAL_FAC://Seba
					com_thread->PrintRecSubtotalFac( buffer );
                    break;
				case ACTION_DOPRINT: 
					com_thread->Print();
					break;
				case ACTION_FISCAL_ESTABLECER_FECHA_HORA:
					com_thread->setDateFechaHora( buffer );
					break;
				case  ACTION_PINPAD_MOSTRAR:
					rta = com_thread->pinpadMostrar( buffer );
					break;
				case  ACTION_PINPAD_SELECT_MK:	
					com_thread->pinpad_select_mk( buffer );
					break;
				case  ACTION_PINPAD_CANCEL_SESSION:
					rta = com_thread->pinpad_cancel_session( buffer );
					break;
				case  ACTION_PINPAD_TIMEOUT:
					com_thread->pinpad_timeout( buffer );
					break;
				case  ACTION_PINPAD_MENSAJES_ROT:
					rta = com_thread->pinpad_mensajes_rot( buffer );
					break;
				case  ACTION_PINPAD_TRANSFER_MK:
					com_thread->pinpad_transfer_mk( buffer );
					break;
            }
		}
        else {
            switch( action ) {
                case ACTION_SET_DATA_PRINTER:
                    com_thread->Print( buffer );
                    break;
                case ACTION_SET_POSITION:
                    com_thread->SetPostion( buffer );
                    break;
                case ACTION_SET_PRINT_STATION:
                    com_thread->PrintSelect( atoi( buffer ) );
                    break;
                case ACTION_DOPRINT:
                    com_thread->Print();
                    break;
                case ACTION_NORMAL_LETTER: //BORRAR(ignorada por JavaServer) 
                    com_thread->SetLetter( 0 ); //BORRAR(ignorada por JavaServer) 
                    break;
                case ACTION_BIG_LETTER: //BORRAR(ignorada por JavaServer) 
                    com_thread->SetLetter( 1 ); //BORRAR(ignorada por JavaServer) 
                    break;
                case ACTION_SMALL_LETTER: //BORRAR(ignorada por JavaServer) 
                    com_thread->SetLetter( 2 ); //BORRAR(ignorada por JavaServer) 
                    break;
                case ACTION_IMAGE:
                    com_thread->SetImage( buffer );
                    break;
                case ACTION_SET_TAM_IMAGE:
                    com_thread->SetTamImage( buffer );
                    break;
                case ACTION_TIMBRADORA:
                    com_thread->Timbradora( buffer );
                    break;
                case ACTION_SET_FONT:
                    com_thread->SetFont( buffer, len );
                    break;
            }
        }     
        return rta;
    }

    void RESTORE_SCREEN_( int num_pantalla )
    {
    }

    void LLENAR_ATRIBUTO_( int x1, int y1, int x2, int y2, char atributo )
    {
    }

    void CAJA( int x1, int y1, int x2, int y2 )
    {
    }

    void LLENAR_( int x1, int y1, int x2, int y2, char caracter, char atributo )
    {
    }

    void CURSOR_ON_()
    {
    }

    void SET_ESTADO_CURSOR_( int estado )
    {
    }

    void CURSOR_OFF_()
    {
    }

    void PUT_GRAN_CHAR_( int x, int y, int caracter )
    {
    }

    void PUT_CHAR_( int x, int y, char caracter )
    {
    }

    void PUT_CURSOR_( int x, int y )
    {
    }

    void SCROLL_( int x1, int y1, int x2, int y2, int dir )
    {
    }

    void ACTUALIZA_CURSOR_()
    {
    }

    void SET_POSICION_CURSOR_( int x, int y )
    {
    }


    //public void GET_POSICION_CURSOR(org.omg.CORBA.ShortHolder x, org.omg.CORBA.ShortHolder y)  {
    //  x.value = (short)0;
    //  y.value = (short)0;
    //}

    void CLRSCR_( char atributo )
    {
    }

    void SALVA_CURSOR_()
    {
    }

    void RESTAURA_CURSOR_()
    {
    }

    void MOSTRAR_GRAN_CADENA_( int x, int y, char *cadena )
    {
    }

    void MOSTRAR_CADENA_( int x, int y, char *cadena )
    {
        int i = 0;
        i++;
    }

    void MOSTRAR_CARACTER_( int x, int y, char caracter )
    {
        //PUT_CHAR( x, y, caracter );
    }

    void SET_ATRIBUTO_( char atributo )
    {
    }


    void INIT_PEDIR_( int x, int y, int largo )
    {
    }

    void CLOSE_PEDIR_()
    {
    }

    void Presentacion( char *vesion, char *fecha, char *branch )
    {
        if( panInicio != NULL ) {
            panInicio->Close();
            panInicio = NULL;
        }
        panVentas->DesActivate();
        panCobrosEsp->DesActivate();
        frame.NuevoPanelPresentacion( vesion, fecha, branch );
    }

    void MOSTRAR_INPUT_CADENA( char *s, int CaretPosition )
    {
        #ifdef INVEL_L
        MutexGuiEnter();
        #endif
        if( frame.getFocus() != NULL ) {
            frame.getFocus()->SetText( s, CaretPosition );
        }
        #ifdef INVEL_L
        frame.Refresh();
        MutexGuiLeave();
        #endif
    }

	void MOSTRAR_INPUT_CANT( char *s, char *CaretPosition )
    {
		panDenominaciones->mostrarInput(s, CaretPosition); 
    }

    void OBTENER_INPUT_CADENA( char *s )
    {
        #ifdef INVEL_L
        MutexGuiEnter();
        #endif
        if( frame.getFocus() != NULL ) {
            strcpy(s, frame.getFocus()->GetValue() );
        }
        #ifdef INVEL_L
        MutexGuiLeave();
        #endif
   }
    
    void AGREGAR_INPUT_CADENA( char *s, int pos ) 
    {
        #ifdef INVEL_L
        MutexGuiEnter();
        #endif
		//wxYield();  //solo lo puede llamar el mainthread
        if( frame.getFocus() != NULL ) {
            frame.getFocus()->AddText( s, pos );
        }
        #ifdef INVEL_L
        frame.Refresh();
        MutexGuiLeave();
        #endif
    }
    /*void MOSTRAR_MENSAJE( char *s )
    {
        //MsgPantalla_thread->AddMsg( s );
    }*/

    void MOSTRAR_MENSAJE2( char *s, unsigned long pPausa, int pCore )
    {
        MsgPantalla_thread->AddMsg( s, pPausa, pCore );
    }

    void MOSTRAR_MENSAJE_POP_UP( char *pTitulo, char **pMsg, int pCantLinea )
    {
        MsgEmerg_thread->AddMsg(pTitulo,pMsg,pCantLinea);
    }

void MOSTRAR_MENSAJE_UBUNTU( int icono, char *pMsg, int segTimeout )
{
		char *pathTPS;
		char comando1[200];
   	    memset(comando1,0,200);
		STARTUPINFO         siStartupInfo;
		PROCESS_INFORMATION piProcessInfo;
//		DWORD exitCode;
		pathTPS = GetAppExePath();
		/* iconos
		    Case 0 "error.png"
    Case 1 "IMPORTANTE.png"
    Case 2 "informacion.png"
    Case 3 "pregunta.png"
    Case 4 "stop.png"
    Case 5 "promocion.png"
otr warning
		*/

		
		if(segTimeout == 0)
			segTimeout =3000; //por lo menos 3 segundos

		_snprintf( comando1, sizeof(comando1 )-1,"\"%s%s\" %i \"%s\" %li",pathTPS,"minotificacion.exe ",icono ,pMsg,segTimeout /*/ 1000*/);
		memset(&siStartupInfo, 0, sizeof(siStartupInfo));
		memset(&piProcessInfo, 0, sizeof(piProcessInfo));
		siStartupInfo.cb = sizeof(siStartupInfo);

		CreateProcess(NULL, comando1, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE/*CREATE_NO_WINDOW*/, NULL, NULL, &siStartupInfo, &piProcessInfo);
		//WaitForSingleObject( piProcessInfo.hProcess, 50 );
		WaitForSingleObject( piProcessInfo.hProcess, 25 );
		CloseHandle(piProcessInfo.hProcess);
		CloseHandle(piProcessInfo.hThread);
}

int EJECUTAR_SCRIPT_AUTOIT( char *script, char * parametros, int esperarrespuesta )
{
	   	//int retval = 0;
		DWORD exit_code;
  	    wxString comando;

		char *pathTPS;
		char comando1[300];
   	    memset(comando1,0,300);
		STARTUPINFO         siStartupInfo;
		PROCESS_INFORMATION piProcessInfo;
		DWORD exitCode = -10;
		ifstream inFile;
		string aux;
		char pathcompleto[300]; 
		pathTPS = GetAppExePath();
		sprintf(pathcompleto,"%s%s",pathTPS,script);


		aux = pathcompleto ;
		inFile.open(aux.c_str());
		if(!inFile.is_open()) {
			glog("FALTA SCRIPT AU3",1,1);
			glog(pathcompleto,1,1);
				return 0;
		}
		pathTPS = GetAppExePath();
		_snprintf( comando1, sizeof(comando1 )-1,"\"%s%s\" \"%s%s\" %s",pathTPS,"AutoIt3.exe ",pathTPS, script,parametros);
	
		memset(&siStartupInfo, 0, sizeof(siStartupInfo));
		memset(&piProcessInfo, 0, sizeof(piProcessInfo));
		siStartupInfo.cb = sizeof(siStartupInfo);

		CreateProcess(NULL, comando1, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE/*CREATE_NO_WINDOW*/, NULL, NULL, &siStartupInfo, &piProcessInfo);
		if( esperarrespuesta ==SI) {
		
			while(!(::WaitForInputIdle(piProcessInfo.hProcess, INFINITE))){
				WaitForSingleObject( piProcessInfo.hProcess, 25 );
				Sleep( 100 );
				GetExitCodeProcess(piProcessInfo.hProcess, &exit_code);
			}
		}else {
				WaitForSingleObject( piProcessInfo.hProcess, 25 );
				Sleep( 100 );
		}
			
		CloseHandle(piProcessInfo.hProcess);
		CloseHandle(piProcessInfo.hThread);
		if ( exit_code == -10 || exit_code == 0)
			exit_code = 1;
				
	return exit_code;
}



    void BORRAR_MENSAJE_(int pCore)
    {
        MsgPantalla_thread->AddMsg( " ", NO, pCore);
        //frame.BorrarMensaje();
        #ifdef INVEL_L
        frame.Refresh();
        #endif
    }

    void CR1_MOSTRAR_ITEM_( char *signo, char *cantidad, char *cinterno, char *cbarra, char *nombre,
                            char *precio )
    {
        panVentas->addArticle( signo, cantidad, cinterno, cbarra, nombre, precio );
    }
	void CR1_BORRAR_MOSTRAR_ITEM_( int pos )
    {
        panVentas->delArticle( pos );
    }
	void CR1_MODIFICAR_MOSTRAR_ITEM_( int pos, char *signo, char *cantidad, char *cinterno, char *cbarra, char *nombre,
                            char *precio )
    {
        panVentas->updArticle( pos,signo, cantidad, cinterno, cbarra, nombre, precio );
    }
	

    void ExecuteAction( int Action )
    {
       /* if( GRABAR_LOG_ACTIONS() == 'S' ) {
            char buff[1024];
			memset(buff,0,sizeof(buff));
            _snprintf(buff, sizeof(buff)-1, "ACTION: %i time: %i", Action, mtime() );
            //no tiene mucho sentido grabar este action
			GRABAR_LOG_SISTEMA( buff ,LOG_PANTALLAS,2);
			memset(buff,0,sizeof(buff));
        }*/

        switch( Action ) {
            case A_ACTIVAR_PANEL_VENTAS:
				MostrarMemoria( );
				frame.BorrarMensaje();
                #ifdef INVEL_L
                frame.desactivarPanelPrincipal();
                frame.activarPanelVentas();
                #else
                //frame.desactivarPanelVentas();
                frame.activarPanelVentas();
                frame.desactivarPanelPrincipal();
                #endif
                break;
            case A_ACTIVAR_PANEL_COBROS_ESPECIALES:
				frame.BorrarMensaje();
                panCobrosEsp->init( "" );
                #ifdef INVEL_L
                frame.desactivarPanelPrincipal();
                frame.activarPanelCobrosEsp();
                #else
                frame.activarPanelCobrosEsp();
                frame.desactivarPanelPrincipal();
                #endif
                break;
            case A_ACTIVAR_PANEL_DEUDA_SOCIO:
                frame.activarPanelDeudaSocio();
                break;
			case A_ACTIVAR_PANEL_VUELTO:
                frame.activarPanelVuelto();
                break;
			case A_ACTIVAR_PANEL_DENOMINACIONES:
                frame.activarPanelDenominaciones();
                break;
			case A_DESACTIVAR_PANEL_DENOMINACIONES:
				frame.desactivarPanelDenominaciones();
                break;
            case A_DESACTIVAR_PANEL_DEUDA_SOCIO:
                frame.desactivarPanelDeudaSocio();
                break;
			case A_DESACTIVAR_PANEL_VUELTO:
                frame.desactivarPanelVuelto();
                break;
            case A_ACTIVAR_PANEL_CHEQUES:
                frame.activarPanelCheques();
                break;
            case A_DESACTIVAR_PANEL_CHEQUES:
                frame.desactivarPanelCheques();
                break;
            case A_ACTIVAR_PANEL_IMPORTACION:
                frame.activarPanelImportacion();
                break;
            case A_DESACTIVAR_PANEL_IMPORTACION:
                frame.desactivarPanelImportacion();
                break;
            case A_FIN_PEDIR:
                frame.FinPanelPedir();
                break;
            case A_PAGOS_INIT:
                panPagos->init( "" );
                break;
            case A_BORRAR_MEDIOS:
                panPagos->borrarMedios();
                panRetiros->borrarMedios();
                panFondoFijo->borrarMedios();
				panCambioMedio->borrarMedios();
                break;
            case A_BORRAR_PAGO:
                panPagos->borrarPago();
                break;
            case A_BORRAR_PAGOS:
                panPagos->borrarPagos();
                break;
            case A_DES_VENTAS_ACT_PAGOS:
				frame.BorrarMensaje();
				//MostrarMemoria( );
                panPagos->init( "PAGOS" );
                panPagos->borrarPagos();

                #ifdef INVEL_L
                frame.desactivarPanelVentas();
                frame.activarPanelPagos();
                #else
				frame.desactivarPanelVentas();
				frame.activarPanelPagos();

                #endif
                /*if( MODO_NEGOCIO != SERVICIOS ) {
                    //com_thread->scanOut.PutData( "false" );
                }*/
                break;
            case A_DES_VENTAS_ACT_FFIJO:
                panFondoFijo->init( "FONDO FIJO" );
                #ifdef INVEL_L
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.desactivarPanelCobrosEsp();
                }
                else {
                    frame.desactivarPanelVentas();
                }

                frame.activarPanelFondoFijo();
                #else
                
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.desactivarPanelCobrosEsp();
                }
                else {
                    frame.desactivarPanelVentas();
                }
				frame.activarPanelFondoFijo();      
                #endif
                break;
            case A_DES_VENTAS_ACT_RETIROS:
                panRetiros->init( "RETIROS" );
                #ifdef INVEL_L
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.desactivarPanelCobrosEsp();
                }
                else {
                    frame.desactivarPanelVentas();
                }

                frame.activarPanelRetiros();
                #else
//                frame.activarPanelRetiros();
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.desactivarPanelCobrosEsp();
                }
                else {
                    frame.desactivarPanelVentas();
                }
				frame.activarPanelRetiros();
                #endif
                break;
            case A_DES_PAGOS_ACT_VENTAS:
				MostrarMemoria( );
				frame.BorrarMensaje();
                #ifdef INVEL_L
                frame.desactivarPanelPagos();
                frame.activarPanelVentas();
                #else
                panPagos->init( "" );
                frame.activarPanelVentas();
                frame.desactivarPanelPagos();
                #endif
                break;
            case A_DES_FFIJO_ACT_VENTAS:
				frame.BorrarMensaje();
                panFondoFijo->init( "FONDO FIJO" );
                #ifdef INVEL_L
                frame.desactivarPanelFondoFijo();
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.activarPanelCobrosEsp();
                }
                else {
                    frame.activarPanelVentas();
                }

                #else
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.activarPanelCobrosEsp();
                }
                else {
                    frame.activarPanelVentas();
                }

                frame.desactivarPanelFondoFijo();
                #endif
                break;
            case A_DES_RETIROS_ACT_VENTAS:
				frame.BorrarMensaje();
                panRetiros->init( "RETIROS" );
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.activarPanelCobrosEsp();
                }
                else {
                    frame.activarPanelVentas();
                }

                frame.desactivarPanelRetiros();
                break;
            case A_DES_VENTAS_ACT_COBROS:
				frame.BorrarMensaje();
				frame.desactivarPanelVentas();
                frame.activarPanelCobros();
                break;
			case A_DES_COBROS_ACT_VENTAS:
				frame.BorrarMensaje();
                frame.desactivarPanelCobros();
                frame.activarPanelVentas();
                break;
            case A_DES_COBROS_ESP_ACT_PAGOS_ESP:
                #ifdef INVEL_L
                frame.desactivarPanelCobrosEsp();
                frame.activarPanelPagosEsp();
                #else
                frame.activarPanelPagosEsp();
                frame.desactivarPanelCobrosEsp();
                #endif
                break;
            case A_DES_PAGOS_ESP_ACT_COBROS_ESP:
                #ifdef INVEL_L
                frame.activarPanelCobrosEsp();
                frame.desactivarPanelPagosEsp();
                #else
                frame.desactivarPanelPagosEsp();
                frame.activarPanelCobrosEsp();
                #endif
                break;

            case A_DES_VENTAS_ACT_PAGOS_CAJA:
                #ifdef INVEL_L
                frame.desactivarPanelVentas();
                frame.activarPanelPagosCaja();
                #else
				//Se agregar BorrarMensaje porque el msj "INGRESAR CLAVE" se borra en Pant. Pago
				//cuando deberia borrarse en Pant. Ventas de manera que queda pegado cuando sale 
				//de PAGOSCAJA A VENTA.
				frame.BorrarMensaje();
                frame.desactivarPanelVentas();
				frame.activarPanelPagosCaja();
                #endif
                break;
            case A_DES_PAGOS_CAJA_ACT_VENTAS:
				frame.BorrarMensaje();
                #ifdef INVEL_L
                frame.desactivarPanelPagosCaja();
                frame.activarPanelVentas();
                #else
                frame.activarPanelVentas();
                frame.desactivarPanelPagosCaja();
                #endif
                break;
            case A_DES_VENTAS_ACT_STANDBY:
                frame.desactivarPanelVentas();
				frame.activarPanelStandby();
                break;
            case A_DES_STANDBY_ACT_VENTAS:
				frame.BorrarMensaje();
                frame.activarPanelVentas();
                frame.desactivarPanelStandby();
                break;
            case A_DES_COBROS_ACT_PAGOS:
				frame.BorrarMensaje();
                panPagos->init( "PAGOS" );
                panPagos->borrarPago();
                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.activarPanelPagos();
                    frame.desactivarPanelCobrosEsp();
                }
                else {
                    frame.activarPanelPagos();
                    frame.desactivarPanelCobros();
                }
                break;
            case A_INIT_VENTAS:
                panVentas->init();
                break;
            case A_REINTENT_ARQUEO_CIERRE:
                panCierreSub->init( "99" );     
                break;
            case A_DES_CIERRE_ACT_PRESENT:
                panCierreSub->init( "99" );
                #ifdef INVEL_L
                frame.desactivarPanelCierre();
                frame.activarPanelPresentacion();
                #else
                frame.activarPanelPresentacion();
                frame.desactivarPanelCierre();
                #endif
                break;
            case A_DES_CIERRE_ACT_VENTAS:
				frame.BorrarMensaje();
                panCierreSub->init( "99" );
                frame.desactivarPanelCierre();

                if( MODO_NEGOCIO == SERVICIOS ) {
                    frame.activarPanelCobrosEsp();
                }
                else {
                    frame.activarPanelVentas();
                }

                break;
            case A_CEREAR_ITEM_RE_CIERRE:
                //if( panCierreSub->isVisible()){
                panCierreSub->cerearSubmedios();
                //}
                break;
            case A_MUESTRA_DATO_COMPROB_COBRO:
            case A_MUESTRA_DATO_CUENTA_COBRO:
            case A_MUESTRA_DATO_IMPORTE_COBRO:
                panCobro->setPosInputBox( Action );
                break;
            case A_FIN_PEDIR_CL_PRES:
                frame.FinPanelPedirClaveEnPresentacion();
                break;
            case A_LIBRE:
                break;
            case A_DES_PRESEN_ACT_CIERRE:
				frame.BorrarMensaje();
                frame.activarPanelCierre();
                frame.desactivarPanelPrincipal();
                break;
            case A_DESELECCIONA_MEDIO:
                if(panFondoFijo->isShowing())
                    panFondoFijo->desseleccionaMedio();
                if(panPagos->isShowing())
                    panPagos->desseleccionaMedio();
                if(panRetiros->isShowing())
                    panRetiros->desseleccionaMedio();
                if(panCambioMedio->isShowing())
                    panCambioMedio->desseleccionaMedio();
                break;
            case A_DES_VENTAS_ACT_CAMBIOMEDIO:
                panCambioMedio->init( "CAMBIO MEDIO" );
                frame.activarPanelCambioMedio();
                frame.desactivarPanelVentas();
                break;
            case A_DES_CAMBIOMEDIO_ACT_VENTAS:
				frame.BorrarMensaje();
                frame.activarPanelVentas();
                panCambioMedio->init( "CAMBIO MEDIO" );
                frame.desactivarPanelCambioMedio();
                break;
            case  A_MUESTRA_DISCRIMINA_PAGOS_CAJA:
                panPagosCaja->ShowDiscrimina( true );
                break;
            case  A_OCULTA_DISCRIMINA_PAGOS_CAJA:
                panPagosCaja->ShowDiscrimina( false );
                break;
            case A_MUESTRA_DATO_COMPROB_PAGOS_CAJA:
            case A_MUESTRA_DATO_CUENTA_PAGOS_CAJA:
            case A_MUESTRA_DATO_IMPORTE_PAGOS_CAJA:
            case A_MUESTRA_DATO_GRAVADO_PAGOS_CAJA:
            case A_MUESTRA_DATO_EXENTO_PAGOS_CAJA:
            case A_MUESTRA_DATO_IVA_PAGOS_CAJA:
                panPagosCaja->setPosInputBox( Action );
                break;
			/*  **** Codigo Obsoleto ****
            case A_MUESTRA_DATO_CODIGO_BARRA_PT:
			case A_MUESTRA_DATO_IMPORTE_PT:
				panPagaTodo->setPosInputBox( Action );
				break;
			case A_MUESTRA_DATO_IMPORTE_G:*/
			case A_MUESTRA_DATO_FOLIO_G:
			case A_MUESTRA_COD_ARTICULO_G:
                /*  **** Codigo Obsoleto ****
				panGarantia->setPosInputBox( Action );
				break;*/
			case A_MUESTRA_DATO_CODIGO_BARRA:
				panCobro->setPosInputBox( Action );
				break;
            case A_MUESTRA_DATO_UNIDAD_NEGOCIO:
            case A_MUESTRA_DATO_CLIENTE:
                panCobrosEsp->setPosInputBox( Action );
                break;
			case A_MUESTRA_DATO_COD_BARRA_PROMO:
				panConsultaPromos->setPosInputBox(0);
				break;
            case A_LIMPIA_PANEL_COBROS_ESP:
                panCobrosEsp->clear();
                break;
            case A_FIN_GRILLA:
                frame.FinPanelLista();
                break;
            case A_DES_PAGOS_ACT_COBROS_ESP:
				frame.BorrarMensaje();
                panPagos->init( "" );
                panCobrosEsp->clear();
                #ifdef INVEL_L
                frame.desactivarPanelPagos();
                frame.activarPanelCobrosEsp();
                #else
                frame.activarPanelCobrosEsp();
                frame.desactivarPanelPagos();
                #endif
                break;
            case A_DES_COBROSESP_ACT_STANDBY:
                frame.activarPanelStandby();
                frame.desactivarPanelCobrosEsp();
                break;
            case A_DES_STANDBY_ACT_COBROSESP:
                frame.activarPanelCobrosEsp();
                frame.desactivarPanelStandby();
                break;
            case A_DES_COBROSESP_ACT_MEDIOREINT:
				frame.BorrarMensaje();
                panMedioReint->init();
                frame.activarPanelMedioReint();
                frame.desactivarPanelCobrosEsp();
                break;
            case A_DES_MEDIOREINT_ACT_COBROSESP:
                panMedioReint->finish();   
                frame.activarPanelCobrosEsp();
                frame.desactivarPanelMedioReint();
                break;
            case A_QUITAR_FILAS_REINTEGROS:
                panMedioReint->removeAllRowsGrid();
                break;
            case A_MOSTRAR_OP_MULT:
                panCobrosEsp->setOpMult( true );   
                break;
            case A_OCULTAR_OP_MULT:
                panCobrosEsp->setOpMult( false );
                break;
            case A_MUESTRA_TIT_PAGOS_ESP:
                panPagosEsp->createTitles();
                break;
            case A_MUESTRA_MODO_PAGOS_ESP:
                panPagosEsp->setModo();
                break;
			case A_BORRA_DIF_SUBMEDIOS:
				panCierreSub->borrarImporteSubmedioDif();
				break;
            case A_REFRESH_PAGOS:
                panPagos->refreshGrids();
                break;
			case A_ACTIVAR_PANEL_PAGATODO:
                frame.activarPanelPagaTodo();
                break;
            case A_DESACTIVAR_PANEL_PAGATODO:
                frame.desactivarPanelPagaTodo();
                break;
			case A_ACTIVAR_PANEL_GARANTIA:
                frame.activarPanelGarantia();
                break;
            case A_DESACTIVAR_PANEL_GARANTIA:
                frame.desactivarPanelGarantia();
                break;
			case A_REBOOT_SYSTEM:
                wxShutdown(wxSHUTDOWN_REBOOT);
                break;
			case A_SHUTDOWN_SYSTEM:
                char comando[35];
                _snprintf(comando,sizeof(comando),"%spsshutdown -f",PATH_BIN);
                wxShell(comando);
                //wxShutdown(wxSHUTDOWN_POWEROFF);
                break;
            case A_ACTIVAR_PANEL_CONSULTA_PROMOS:
				frame.activarPanelConsultaPromos();
                break;
            case A_DESACTIVAR_PANEL_CONSULTA_PROMOS:
                frame.desactivarPanelConsultaPromos();
                break;
            case A_DES_TODOS_ACT_VENTAS:
//				MostrarMemoria( );
                frame.BorrarMensaje();
				//if(panPagos->isShowing()){
				  //  frame.desactivarPanelPagos();
//                    frame.desactivarTodosLosPaneles();
//                }
                if( !panVentas->isShowing() ) {
					if( ACELERACION_PANTALLAS == NO) {
						frame.activarPanelVentas();
						frame.desactivarTodosLosPaneles();
					}else {
						frame.desactivarTodosLosPaneles();
						if( ACELERACION_PANTALLAS == SI)
							panPagos->mYield(); //si bien dice panpagos se aplica a la aplicacion sirve para todos los paneles
						frame.activarPanelVentas();
					}
                }
                break;
            case A_ACTIVA_MODO_INVENTARIO:
                 if( panVentas->isShowing()) {
				      	panVentas->ShowRenglones( true  );
				      }

                break;
            case A_DESACT_MODO_INVENTARIO:
                 if( panVentas->isShowing()) {
				      	panVentas->ShowRenglones( false );

				      }
                break;
            case A_MUESTRA_PANTALLA_OFFLINE:
                panPagos->setErrorBackground();
                break;
            case A_OCULTA_PANTALLA_OFFLINE:
                panPagos->setNormalBackground();
                break;
			case A_MUESTRA_MEMORIA:				
				MostrarMemoria( );
				break;
			case A_PAGOS_FOCUS:
				panPagos->SetFocus();
				break;
			case A_VENTAS_FOCUS:
				if( panVentas->isShowing()) 
					panVentas->SetFocus();
				break;
			case A_DES_TECLADO_ALFABETICO:
				//La pantalla entran en PanelBrowser seteando TECLADO_ALFABETICO = 1
				//Al activarse screenSaver no permite que tome las teclas presionadas
				if( panPagosCaja->isShowing()
						|| panCobro->isShowing()
						|| panPagosEsp->isShowing() ) {
					TECLADO_ALFABETICO = 0;
				}
				break;
			case A_DES_BROWSE:
				if (panBrowse->isShowing()) {
					panBrowse->DesActivate();
					TECLADO_ALFABETICO = 0;
				}
				break;
			case A_DES_WIN_ON_TOP:
				//BORRAR_MENSAJES_EN_PANTALLA( );
				if( panInput->isShowing() ) {
					panInput->DesActivate();
				}
				if(panBrowse->isShowing()) {
					panBrowse->DesActivate();
				}
				if (panConsultaPromos->isShowing()) {
					panConsultaPromos->DesActivate();
				}
				if (panMenu->isShowing()) {
					panMenu->DesActivate();
				}
				TECLADO_ALFABETICO = 0;
		        break;
            default:
                GRABAR_LOG_SISTEMA( "Error: No reconocido" ,LOG_PANTALLAS,3);
        }
        /*if( GRABAR_LOG_ACTIONS() == 'S' ) {
            GRABAR_LOG_SISTEMA( "FIN EXECUTE ACTION", LOG_PANTALLAS,2);
        }*/
    }

    void ExecuteSAction( int Action, char *s )
    {
        if( GRABAR_LOG_ACTIONS() == 'S' ) {
            char buff[1024];
			memset(buff,0,sizeof(buff));
			if( s!= NULL) {
				_snprintf(buff, sizeof(buff)-1, "ACTIONS: %i time: %i, '%s'", Action, mtime(), s );
				if( strlen( s) > 0 )
					GRABAR_LOG_SISTEMA( buff ,LOG_PANTALLAS,5);
				memset(buff,0,sizeof(buff));
			} else {
				Action = 999; //hubo un fallo no dejo que entre
			}
		}

        int i,c,len;

        switch( Action ) {
            case A_VENTA_MUESTRA_TOTAL:
                panVentas->setTotal( s );
                break;
			case A_MUESTRA_TITULO_BROWSE:
				panBrowse->tituloPanelBrowse->setText( s );
				break;
            case A_MUESTRA_ARTICULO:
                panVentas->setDetalle( s );
                break;
            case A_PAGO_MUESTRA_TOTAL:
				{
				bool activo = false;
				while(activo == false){
					if(panPagos->isShowing()){
						panPagos->muestraTotal( s );
						activo = true;
					}
					if(panFondoFijo->isShowing()){
						panFondoFijo->muestraTotal( s );
						activo = true;
					}
					if(panCambioMedio->isShowing()){
						panCambioMedio->muestraTotal( s );
						activo = true;
					}
					if(panRetiros->isShowing()){
						panRetiros->muestraTotal( s );
						activo = true;
					}
				}
				}
                break;
            case A_PAGO_MUESTRA_IMPORTE_TEMPORAL:
                panPagos->setImporteTemporal( s );
                break;
            case A_MUESTRA_BULTOS:
                c = 0;
                len = strlen( s );
                for( i = 0;i < len;i++ ) {
                    c += ( s[i] == 32 ) ? 1 : 0;
                }
                panVentas->setBultos( &s[c] );
                break;
            case A_MUESTRA_TIT_BULTOS:
                panVentas->setTitBultos( s );
                break;
            case A_MUESTRA_MODO:
                panVentas->setModo( s );
                break;
            case A_MUESTRA_MODO_COBROS:
                panCobro->setModo( s );
                break;
            case A_MUESTRA_MODO_PAGOS_CAJA:
                panPagosCaja->setModo( s );
                break;
            case A_MUESTRA_TITULO_FFIJO:
                panFondoFijo->setTituloFfijo( s );
                panCambioMedio->setTituloCambioMedio( s );
                break;
            case A_MUESTRA_TIT_TOTAL_FFIJO:
				{
				bool activo = false;
				while(activo == false){
					if(panFondoFijo->isShowing()){
						panFondoFijo->setTitotalFfijo( s );
						activo = true;
					}
					if(panCambioMedio->isShowing()){
						panCambioMedio->setTitotalCambioMedio( s );
						activo = true;
					}
				}
				}
                break;
            case A_MUESTRA_TIT_TOTAL_PAGOS:
                panPagos->setTitotalPpagos( s );
                break;
			case A_MUESTRA_TIT_TOTAL_RETIROS:
                panRetiros->setTitotalRetiros( s );
                break;
            case A_MUESTRA_TITULO_RETIROS:
                panRetiros->setTituloRetiros( s );
                break;
            case A_MUESTRA_DESCRIPCION:
                panVentas->setDescripcion( s );
                break;
			case A_MUESTRA_DESCRIPCION_CONTADOR:
				panVentas->setDescContador( s );
				break;
			case A_MUESTRA_CONTADOR_DIFERENCIAL:
				panVentas->setCantConsecutivos( s );
				break;
			case A_MUESTRA_CONTADOR_TOTAL_IDEN:
				panVentas->setCantIdenticos( s );
                break;
            case A_MUESTRA_CANTIDAD:
                panVentas->setCantidad( s );
                break;
            case A_MUESTRA_SUBTOTAL:
                panVentas->setSubtotal( s );
                break;
			case A_MUESTRA_IMG_ART:
				panVentas->ShowImgArt( s );
				break;
            case A_MUESTRA_MULTIPLICACION:
                panVentas->setMultiplica( s );
                break;
            case A_MUESTRA_TIT_COD_BARRA_COBRO:
                panCobro->setTitCodBarra( s );
                break;;
            case A_MUESTRA_TIT_COMPROB_COBRO:
                panCobro->setTitComprob( s );
                break;;
            case A_MUESTRA_TIT_CUENTA_COBRO:
                panCobro->setTitCuenta( s );
                break;
            case A_MUESTRA_TIT_IMPORTE_COBRO:
                panCobro->setTitImporte( s );
                break;
            case A_MUESTRA_TIT_CANTIDAD_COBRO:
                panCobro->setTitCantidad( s );
                break;
            case A_MUESTRA_TIT_TOTAL_COBRO:
                panCobro->setTitTotal( s );
                break;
            case A_MUESTRA_CANTIDAD_COBRO:
                panCobro->setCant_pc( s );
                break;
            case A_MUESTRA_COBRO_IMPORTE_ME:
                panCobro->setImp_me( s );
                break;
            case A_MUESTRA_TOTAL_COBRO:
                panCobro->setTotal_pc( s );
                break;
            case A_MUESTRA_SOCIO_PAGOS_ESP:
                panPagosEsp->setNroSoc( s );
                break;
            case A_MUESTRA_REINT_PAGOS_ESP:
                panPagosEsp->setNroReint( s );
                break;
            case A_MUESTRA_IMPORTE_PAGOS_ESP:
                panPagosEsp->setNImport( s );
                break;
            case A_MUESTRA_MEDIO_PAGOS_ESP:
                panPagosEsp->setNomMedio( s );
                break;
            case A_MUESTRA_SUBMEDIO_PAGOS_ESP:
                panPagosEsp->setNomSubmedio( s );
                break;
            case A_MUESTRA_CHEQUE_PAGOS_ESP:
                panPagosEsp->setNroCheq( s );
                break;
            case A_MUESTRA_BANCO_PAGOS_ESP:
                panPagosEsp->setNomBanco( s );
                break;

            case A_MUESTRA_TIT_COMPROB_PAGOS_CAJA:
                panPagosCaja->setTitComprob( s );
                break;
            case A_MUESTRA_TIT_CUENTA_PAGOS_CAJA:
                panPagosCaja->setTitCuenta( s );
                break;
            case A_MUESTRA_TIT_IMPORTE_PAGOS_CAJA:
                panPagosCaja->setTitImporte( s );
                break;
            case A_MUESTRA_TIT_GRAVADO_PAGOS_CAJA:
                panPagosCaja->setTitGravado( s );
                break;
            case A_MUESTRA_TIT_EXENTO_PAGOS_CAJA:
                panPagosCaja->setTitExento( s );
                break;
            case A_MUESTRA_TIT_IVA_PAGOS_CAJA:
                panPagosCaja->setTitIva( s );
                break;
            case A_MUESTRA_TIT_FECHA_PRES:
                frame.TitFechaEnPresentacion( s );
                break;
            case A_MUESTRA_TIT_USUARIO_PRES:
                frame.TitUsuarioEnPresentacion( s );
                break;
            case A_MUESTRA_TIT_CLAVE_PRES:
                frame.TitPasswordEnPresentacion( s );
                break;
            case A_MUESTRA_FECHA_PRES:
                panPres->setFechaOp( s );
                break;
            case A_MUESTRA_FECHA_OP_VENTA:
				if( panPres->isShowing() ) {
					panPres->ShowFechaOp( s );
				}
				if( panVentas->isShowing()) {
					panVentas->ShowFechaOp( s );
				}
                break;
			case A_MUESTRA_FECHA_SISTEMA:
				if( panPres->isShowing() ) {
					panPres->ShowFechaSis( s );
				}
                break;
            case A_AGREGAR_MEDIO_CIERRE:
                panCierreSub->agregarMedioCierreSub( s );
                break;
            case A_ELIMINAR_ARTICULO:
                panVentas->delArticle( ( int )s );
                break;
            case A_SCROLL_TICKET:
                panVentas->scroll( s );
                break;
            case A_BATCH_TICKET:
                panVentas->batch( s );
                break;
            case A_SCROLL_UNIDAD_NEGOCIO:
                panCobrosEsp->ScrollUniNeg( s );
                break;
            case A_SCROLL_DEUDA_SOCIO:
                panDeudaSocio->ScrollDeuda( s );
                break;
			case A_SCROLL_VUELTO:
                panVuelto->ScrollVuelto( s );
                break;
			case A_SCROLL_DENOMINACIONES:
				panDenominaciones->ScrollDenominaciones( s );
                break;
            case A_SCROLL_CHEQUES:
                panCheques->ScrollCheques( s );
                break;
            case A_SCROLL_PROMOS:
				panConsultaPromos->ScrollConsultaPromos( s );
                break;
            case A_SCROLL_IMPORTACION:
                panImportacion->ScrollImportacion( s );
                break;
            case A_MUESTRA_DATO_SOCIO_DEUDA:
                panDeudaSocio->setSocioLabel( s );
                break;
                /*case A_MUESTRA_DATO_SOCIO_COBRO:
                   panCobrosEsp->setSocio(s);
                   break;*/
            case A_MUESTRA_SUBTOTAL_COBRO_ESP:
                panCobrosEsp->setSubTotal( s );
                break;
			case A_SUBTOTAL_DENOMINACIONES:
				panDenominaciones->setSubTotal( s );
                break;
            case A_AGREGAR_FILA_GRILLA:
                panGrill->addRow( s );
                break;
            case A_SELECCION_FILA_GRILLA:
                panGrill->scrollGrill( s );
                break;
            case A_MOSTRAR_FOTO_PRES:
                panPres->MostrarFoto( s );
                break;
            case A_SELEC_FILA_MED_ING:
                panMedioReint->scrollGridIng( s );
                break;
            case A_QUITAR_MEDIO_REINTEGRO:
                panMedioReint->remGridIng( s );
                break;
            case A_QUITAR_FILA_MED_DISP:
                panMedioReint->removeRowGridDisp( s );
                break;
            /*case A_GET_DATA_MEDREIN_ING:
                panMedioReint->pedirGridIng( s, s1 );
                break;*/
            case A_GET_DATA_MEDREIN_DISP:
                panMedioReint->pedirGridDisp( s );
                break;
            case A_OCULTAR_PAN_PEDIR:
                frame.EsconderPanelPedir( s );
                break;
            case A_SET_INPUT_TEXT:
                panInput->setInputText( s );
                break;
			case A_CAMBIO_PANTALLA_TOUCH:
				frame.currPaneltouch = atoi( s );
				break;
			case A_MUESTRA_TIT_COD_FOLIO_G:
				//panGarantia->setFolioLabel( s );
                panCobro->setTitFolio( s );
				break;
			case A_MUESTRA_TIT_COD_ARTICULO_G:
				//panGarantia->setCodArticuloLabel( s );
                panCobro->setTitCodArticulo( s );
				break;
			case A_MUESTRA_TIT_IMPORTE_G:
				panGarantia->setImporteLabel( s );
				break;
			case A_MUESTRA_TIT_IMPORTE_PAGATODO:
                panPagaTodo->setImporteLabel( s );
                break;
			case A_MOSTRAR_CAJA_CAJERO_PRESENT:
                if( panPres->isShowing() )
                    panPres->setCajeroActual( s );
                break;
         case A_ACTIVA_MODO_INVENTARIO:
              if( panVentas->isShowing()) {
					  panVentas->ShowTipoInventario( s );
				  }
				  break;
/*			case A_DESACT_MODO_INVENTARIO:
              if( panVentas->isShowing()) {
					  panVentas->ShowTipoInventario( s );
				  }
				  break;*/
			case A_GET_PANEL_ACTIVO:
                if( panVentas->isShowing( ) ) {
                    *s = PANTALLA_VENTA;
                }
                else if( panPagos->isShowing( ) ) {
                    *s = PANTALLA_PAGO;
                }
                else if( panStandby->isShowing( ) ) {
                    *s = PANTALLA_STANDBY;
                }
                else if( panPres->isShowing( ) ) {
                    *s = PANTALLA_PRESENTACION;
                }
                else if( panFondoFijo->isShowing( ) ) {
                    *s = PANTALLA_FONDO_FIJO;
                }
                else if( panRetiros->isShowing( ) ) {
                    *s = PANTALLA_RETIRO;
                }
                else if( panCambioMedio->isShowing( ) ) {
                    *s = PANTALLA_CAMBIO_MEDIO;
                }
				break;
            default:
                GRABAR_LOG_SISTEMA( "Error: No reconocido" ,LOG_INCONDICIONAL,1);
        }
       
    }

    void ExecuteSSAction( int Action, int type, char *s, char *s1, char *mascara, char *s2 )
    {
        if( DEBE_GRABAR_LOG_SISTEMA1( LOG_PANTALLAS,5 ) ) {
            char buff[1024];
			if( s != NULL && s1 != NULL) {
				_snprintf( buff, 1024, "ACTIONSS: %i type %i time: %i, '%s' '%s'", Action, type, mtime(), s,
                     s1 );
			if( strlen( s) > 1)
				GRABAR_LOG_SISTEMA( buff,LOG_PANTALLAS,5 );
			} else {
				Action = 999;
			}
        }
		
        switch( Action ) {
            case A_AGREGAR_MEDIO:
				{
					int activa = 0;
					while(!activa){
						if(panPagos->isShowing()){
							panPagos->addFlag( s, s1, type );
							activa = 1;
						}
						if(panFondoFijo->isShowing()){
							panFondoFijo->addFlag( s, s1, type );
							activa = 1;
						}
						if(panRetiros->isShowing()){
							panRetiros->addFlag( s, s1, type );
							activa = 1;
						}
						if(panCambioMedio->isShowing()){
							panCambioMedio->addFlag( s, s1, type );
							activa = 1;
						}
					}
				}
                break;
            case A_PEDIR:
                if( s == NULL ) {
                    s = "";
                }
                if( s1 == NULL ) {
                    s1 = "";
                }
                if( s2 == NULL ) {
                    s2 = "";
                }
                if( mascara == NULL ) {
                    mascara = "";
                }
                
                //if(s == NULL)
                //  frame.PanelPedir(type," ",s1,mascara);
                //else
                frame.PanelPedir( type, s, s1, mascara, s2 );
                break;
            case A_PEDIR_DATO:
                frame.PanelMoveInput( type, s, s1 );
                break;
            case A_MOSTRAR_GRILLA:
                frame.PanelLista( s, type );   
                break;
            /*case A_MUESTRA_RESTO:
                panPagos->setResto( s, s1);
                break;*/
            case A_MUESTRA_FLAGS:
                panVentas->addFlag( s, s1, type );
                break;
            case A_MUESTRA_DATOS:
				//Al comentarse este if, se hizo necesario crear la accion 172 y 173
				//porque la accion 140 y 141 era utilizada en PanelVenta y PanelPago de 
				//manera que labels para PanelPagos eran escritos en PanelVenta y al final
				//no se cereaban estos.
                //if(panVentas->isShowing())
                    panVentas->addFlag( s, s1, type );
                if(panPagos->isShowing())
                    panPagos->addFlag( s, s1, type );
				if(panCambioMedio->isShowing());
					panCambioMedio->addFlag( s, s1, type );
                break;
            case A_DES_VTAS_ACT_CIERRE:
                frame.BorrarMensaje();
                if( type == T_CIERRE_MEDIOS ) {
                    #ifdef INVEL_L
                    frame.desactivarPanelPrincipal();

                    if( MODO_NEGOCIO == SERVICIOS ) {
                        frame.desactivarPanelCobrosEsp();
                    }
                    else {
                        frame.desactivarPanelVentas();
                    }

                    frame.activarPanelCierre();
                    #else
                    frame.desactivarPanelPrincipal();
					frame.activarPanelCierre();
                    #endif
                }
                else {
                    panCierreSub->init( s );
                    #ifdef INVEL_L
                    if( MODO_NEGOCIO == SERVICIOS ) {
                        frame.desactivarPanelCobrosEsp();
                    }
                    else {
                        frame.desactivarPanelVentas();
                    }

                    frame.desactivarPanelPrincipal();
                    frame.activarPanelCierreSubmedios();
                    #else
                    if( MODO_NEGOCIO == SERVICIOS ) {
                        frame.desactivarPanelCobrosEsp();
                    }
                    else {
                        frame.desactivarPanelVentas();
                    }
                    frame.desactivarPanelPrincipal();
					frame.activarPanelCierreSubmedios();
                    #endif
                }
                break;
            case A_MOSTRAR_IMPO_EN_CIERRE:
                panCierreSub->mostrarImporteMedioInf( s, s1, type );
                panCierreSub->mostrarImporteMedioDif( s, mascara, type );
                break;
            case A_MOSTRAR_ITEM_RE_CIERRE:
                //if( panCierreSub->isVisible()){
                panCierreSub->mostrarItemRendicion( s, s1, type );
                //}
                break;
            case A_RESALTAR_MEDIO_CIERRE:
                panCierreSub->resaltarMedioElegido( s, type );
                break;
            case A_RESALTAR_SUBMEDIO_CIER:
                panCierreSub->resaltarSubmedioElegido( s, type );
                break;
            case A_MOSTRAR_SUMA_EN_CIERRE:
                //if( panCierreSub->isVisible() ) {
                    //panCierreSub->mostrarSumaCierre( s, s1, type, 1 );
					panCierreSub->mostrarSumaParcialMedio( s, s1, type, 1 );
					
                //}
                break;
            case A_MOSTRAR_IMPO_SUB_CIERR:
                //  if( panCierreSub->isVisible())
                panCierreSub->mostrarImporteSubmedio( s, s1, type );
				panCierreSub->mostrarImporteSubmedioDif( s, (mascara) ? mascara : "", type );
                break;
            case A_MOSTRAR_SUMA_SUB_CIERR:
                //if( panCierreSub->isVisible() ) {
                    //panCierreSub->mostrarSumaCierre( s, s1, type, 0 );
					panCierreSub->mostrarSumaParcialSubmedio( s, s1, type, 0 );
                //}
                break;
            case A_MENSAJE_PAGINA_CIERRE:
                //if( panCierreSub->isVisible()){
                panCierreSub->mostrarMsgPaginaCierre( type );
                //}
                break;
            case A_MOSTRAR_CAJA_CAJERO:
                panVentas->showCashier( type, s, s1 );
                break;
            case A_PEDIR_CL_PRES:
                if( s == NULL ) {
                    s = "";
                }
                if( s1 == NULL ) {
                    s1 = "";
                }
                frame.PanelPedirClaveEnPresentacion( type, s, s1 );
                break;
                //case A_ACTIVA_CUADRO_PETICION:
                //  panVentas->addFlag(s, s1, type);
                //  break;
            case A_SET_TIT_STANDBY:
                panStandby->setTitulos( s, s1 );
                break;
            case A_LIMPIA_CUADRO_PETICION:
                panVentas->addFlag( s, s1, type );
                break;
            case A_MUESTRA_TITULO_BOX:
				if(panPagos->isShowing()){
					panPagos->muestraTitulo( type, s, s1 );
				}
				if(panFondoFijo->isShowing()){
					panFondoFijo->muestraTitulo( type, s, s1 );
				}
				if(panRetiros->isShowing()){
					panRetiros->muestraTitulo( type, s, s1 );
				}
				if(panCambioMedio->isShowing()){
					panCambioMedio->muestraTitulo( type, s, s1 );
				}
                break;
            case A_RESALTAR_MEDIO_PP:
                panPagos->resaltarMedio( type + 1, s, s1 );
                break;
            case A_RESALTAR_MEDIO_PR:
                panRetiros->resaltarMedio( type + 1, s, s1 );
                break;
            case A_RESALTAR_MEDIO_PFF:
                panFondoFijo->resaltarMedio( type + 1, s, s1 );
                break;
            case A_RESALTAR_MEDIO_PCM:
                panCambioMedio->resaltarMedio( type + 1, s, s1 );
                break;
            case A_SET_PROP_INPUT:
                if( strcmp( s, "_CONCATENADO" ) == 0 ) {
                    panInput->positionInput( type, _CONCATENADO );
                }
                else if( strcmp( s, "_HIBRIDO" ) == 0 ) {
                    panInput->positionInput( type, _HIBRIDO );
                }
                else if( strcmp( s, "_MOVIL" ) == 0 ) {
                    panInput->positionInput( type, _MOVIL );
                }
                else {
                    panInput->positionInput( type, _SIMPLE );
                }
                break;
            case AGREGAR_UNIDAD_DE_NEGOCIO:
                panCobrosEsp->AddUniNegocio( s, s1 );
                break;
            case A_GET_DATA_MEDREIN_ING:
                panMedioReint->pedirGridIng( s, s1 );
                break;
	        case A_OCULTA_DIF_SUBMEDIO:
		 		panCierreSub->ocultarImporteSubmedioDif( type );
                break;
            case A_MUESTRA_ICON_COBRO:
                panCobro->addIcon( s, type );
                break;
            case A_MUESTRA_FLAG_CAMBIO_MEDIO:
                panCambioMedio->addFlag( s, s1, type );
                break;
            default:
                GRABAR_LOG_SISTEMA( "Error: No reconocido ",LOG_INCONDICIONAL,1 );
        }
    }

    void ExecuteSSSAction( int Action, char *s, char *s1, char *s2 )
    {
         if( DEBE_GRABAR_LOG_SISTEMA1( LOG_PANTALLAS,5 ) ) {
            char buff[1024];
			if( s!= NULL&& s1 != NULL && s2 != NULL) {
				 _snprintf(buff, sizeof(buff)-1, "ACTIONSSS: %i time: %i, '%s' '%s' '%s'", Action, mtime(), s, s1, s2 );
				 GRABAR_LOG_SISTEMA( buff , LOG_PANTALLAS,5);
			}else {
				Action = 999;
			}
        }

        switch( Action ) {
            case A_SELEC_FILA_MED_REINT:
                panMedioReint->scrollGridDisp( s, s1, s2 );
                break;
			case A_MUESTRA_RESTO:
                panPagos->setResto( s, s1, s2);
                break;
        }
    }


    void ExecuteSSSSSAction( int Action, int type, char *s, char *s1, char *s2, char *s3, char *s4 )
    {
         if( DEBE_GRABAR_LOG_SISTEMA1( LOG_PANTALLAS,5 ) ) {
            char buff[1024];
			if( s!= NULL&& s1 != NULL && s2 != NULL && s3 != NULL && s4 != NULL) {
            _snprintf(buff, sizeof(buff)-1, "ACTIONSSSSS: %i type %i time: %i, '%s' '%s' '%s' '%s' '%s'", Action, type,
                     mtime(), s, s1, s2, s3, s4 );
                     GRABAR_LOG_SISTEMA( buff , LOG_PANTALLAS,5);
        }
        }

        switch( Action ) {
            case A_AGREGAR_MEDIO_REINTEGRO:
                panMedioReint->addGridDisp( s, s1, s2, s3, s4 );
                break;
            case A_AGREGAR_MEDIO_REINTEGRO_ING:
                panMedioReint->addGridIng( s, s1, s2, s3, s4 );
                break;
        }
        /*if( GRABAR_LOG_ACTIONS() == 'S' ) {
            _GRABAR_LOG_SISTEMA( "FIN EXECUTE SSSSACTION" , LOG_PANTALLAS,2);
        }*/
    }


    void mostrarCliCobros( char *Nombre, char *Tipo, char *Domicilio )
    {
        panCobrosEsp->setSocio( Nombre, Tipo, Domicilio );
    }

	void agregarVuelto( char *s, char *s1, char *s2 ) 
    {
        panVuelto->AddVuelto( s, s1, s2 );
    }
	
	void agregarDenominacion( char *s, char *s1 ) 
    {
		panDenominaciones->AddDenominaciones( s, s1 );
    }
	
	void clearInput()
	{
		panDenominaciones->clearInputs();
	}

	int getRowCursor() 
    {
		return panDenominaciones->getRowCursor();
    }

	void setCellValue(char *row, char *col, char *text)
	{
		panDenominaciones->setCellText(row, col, text);
	}

	void getCellValue(char *row, char *col, char *text)
	{
		panDenominaciones->getCellText(row, col, text);
	}

    void agregarDeuda( char *s, char *s1, char *s2, char *s3, char *s4, char *s5 ) //agrega una deuda en la grilla del panel deudaSocio
    {
        panDeudaSocio->AddDeuda( s, s1, s2, s3, s4, s5 );
    }

    void agregarCheque( char *s, char *s1, char *s2, char *s3, char *s4 ) //agrega una deuda en la grilla del panel deudaSocio
    {
        panCheques->AddCheque( s, s1, s2, s3, s4 );
    }
    
    void agregarImportacion( char *s, char *s1, char *s2 , char *s3 ) 
    {
        panImportacion->AddImportacion( s, s1, s2, s3 );
    }

    //void agregarCobroEsp(char *s,char *s1,char *s2) //agrega un cobro a la grilla de cobros especiales 
    void agregarCobroEsp( char *numeroComp, char *letra, char *tipoMov, char *reglaAntigua,
                          char *fechaVenc, char *importeMin, char *importe, char *moneda,
                          char *fechaEmision, char *fechaHasta, char *periodoFact )
    {
        //panCobrosEsp->AddCobroGrid(s,s1,s2);
        panCobrosEsp->AddCobroGrid( numeroComp, letra, tipoMov, reglaAntigua, fechaVenc, importeMin,
                                    importe, moneda, fechaEmision, fechaHasta, periodoFact );
    }


    void agregarPago( int aditivo, char *s0, char *s1, char *s2, char *s3 )
    {
        char tmp[] = "";
        int Show = 0;
        if( s0 == 0 ) {
            s0 = tmp;
        }
        if( s1 == 0 ) {
            s1 = tmp;
        }
        if( s2 == 0 ) {
            s2 = tmp;
        }
        if( s3 == 0 ) {
            s3 = tmp;
        }
        while(!Show){
            if( panPagos->isShowing() ) {
                panPagos->agregarPago( ( aditivo ) ? true : false, s0, s1, s2, s3 );
                Show = 1;
            }
            if( panFondoFijo->isShowing() ) {
                panFondoFijo->agregarPago( ( aditivo ) ? true : false, s0, s1, s2, s3 );
                Show = 1;
            }
            if( panRetiros->isShowing() ) {
                panRetiros->agregarPago( ( aditivo ) ? true : false, s0, s1, s2, s3 );
                Show = 1;
            }
            if( panCambioMedio->isShowing() ) {
                panCambioMedio->agregarPago( ( aditivo ) ? true : false, s0, s1, s2, s3 );
                Show = 1;
            }
        }
    }

    void agregarPromo( char *nombre, char *descripcion ) //agrega una promocion en la grilla del panel consultaPromos
    {
		panConsultaPromos->AddPromo( nombre, descripcion );
    }

    void clearPromos( void ) //vacia la grilla de promociones del panel consultaPromos
    {
		panConsultaPromos->ClearGrid( );
    }
	void SetConfig_TPS(  )
	{
		configtps = new CConfigTPS("conftps.xml");
	}

    void SetConfig( int var, int valor, char *strValor )
    {
        switch( var ) {
            case CONF_MODO_NEGOCIO:
                Config->set_modo_negocio( valor );
                break;
            case CONF_TOUCH_SCREEN:
                Config->set_touch_screen( valor );
                break;
            case CONF_JAVA_SERVER:
                Config->set_IpJavaServer(strValor);
                break;
            case CONF_TIEMPO_POP_UP:
                Config->setTiempoPopUp(valor);
                break;
            case CONF_IP_FTP:
                Config->setIpFtp(strValor);
                break;
            case CONF_DIR_VIDEO_VENTAS:
                Config->setVideoVentas(strValor);
                break;
            case CONF_DIR_SKIN:
                Config->setDirSkin(strValor);
                break;
            default:
                break;
        }
    }

    int GetConfig( int var )
    {
        int valor = 0;
        switch( var ) {
            case CONF_MODO_NEGOCIO:
                valor = Config->get_modo_negocio();
                break;
            case CONF_TOUCH_SCREEN:
                valor = Config->get_touch_screen();
                break;
            default:
                break;
        }
        return valor;
    }
    void SLEEP(long milSeg)
    {
		#ifdef INVEL_W
        	Sleep(milSeg);
		#else
        	usleep(milSeg * 1000);
		#endif
    }
    void BEEP(int freq, int duracion){
		//#ifdef INVEL_W
        	Beep(freq,duracion);
    }

	void ScreenSaverOn(){
		//frame.desactivarPanelPrincipal();
		frame.activarPanelScreenSaver();
	}

	void ScreenSaverOff(){
		frame.desactivarPanelScreenSaver();
		if(panVentas->isShowing())
			DIRECT_VIDEO_PLAY_DEFAULT(Config->get_videoVentas());
		//frame.activarPanelPresentacion();
	}
	void SetCurrPanel( int recuperar ) {
		frame.SetCurrPanel( recuperar);
	}
/******************************************************************************/
void ENCABEZADO_LOG_SISTEMA_2( int origen, char * buff )
/******************************************************************************/
{
	switch(origen){
		case 0					:sprintf (buff,		"****** Configuracion Logs *****" );   
		break;                           
		case LOG_PANTALLAS      :sprintf (buff,		"* [%03i]   Log Pantallas       ",origen );   
		break;                           
		case LOG_PAGOS          :sprintf (buff,		"* [%03i]   Log Pagos           ",origen );   
		break;                           
		case LOG_VENTAS         :sprintf (buff,		"* [%03i]   Log Ventas          " ,origen);   
		break;                           
		case LOG_COBROS         :sprintf (buff,		"* [%03i]   Log Cobros          " ,origen);   
		break;                           
		case LOG_MOTOR_PROMO    :sprintf (buff,		"* [%03i]   Log Motor Promo     " ,origen);   
		break;                           
		case LOG_FONDO_FIJO     :sprintf (buff,		"* [%03i]   Log Fondo Fijo      " ,origen);   
		break;                           
		case LOG_RETIROS        :sprintf (buff,		"* [%03i]   Log Retiros         " ,origen);   
		break;                           
		case LOG_CIERRE         :sprintf (buff,		"* [%03i]   Log Cierre          " ,origen);   
		break;                           
		case LOG_JAVA_SERVER    :sprintf (buff,		"* [%03i]   Log Java server     " ,origen);   
		break;                           
		case LOG_VARIOS        	:sprintf (buff,		"* [%03i]   Log Varios          " ,origen );
		break;                           
		case LOG_CAMBIO_MEDIO  	:sprintf (buff,		"* [%03i]   Log Cambio Medio    " ,origen );
		break;                           
		case LOG_ODBC				:sprintf (buff,	"* [%03i]   Log Odbc            " ,origen);  
		break;                           
		case LOG_CON_SERVIDOR	:sprintf (buff,		"* [%03i]   Log Con Servidor    ",origen );
		break;                           
		case LOG_COMUNICACIONES	:sprintf (buff,		"* [%03i]   Log Comunicaciones  " ,origen );
		break;                           
		case LOG_ERRORES			:sprintf (buff,	"* [%03i]   Log Errores         ",origen );  
		break;                           
		case LOG_DEBUG				:sprintf (buff,	"* [%03i]   Log Debug           "  ,origen  );
		break;                           
		case LOG_ROBOT				:sprintf (buff, "* [%03i]   Log Robot           " ,origen   );
		break;                           
		case LOG_SUPERVISOR		:sprintf (buff,		"* [%03i]   Log Supervisor      " ,origen);  
		break;                           
		case LOG_ARCHIVOS			:sprintf (buff, "* [%03i]   Log Archivos        " ,origen   );
		break;                            
		case LOG_MEMORIA			:sprintf (buff,	"* [%03i]   Log Memoria         ",origen );  
		break;                           
		case LOG_BTRIEVE			:sprintf (buff,	"* [%03i]   Log Btrieve         ",origen );  
		break;                                   
		case LOG_BASE_DATO		:sprintf (buff,		"* [%03i]   Log Base Dato       ",origen );
		break;                           
		case LOG_COMPROBANTES	 :sprintf (buff,	"* [%03i]   Log Comprobantes    " ,origen);
		break;
		case (LOG_COMPROBANTES+2):sprintf (buff,	"*******************************" );
		break;
		}
		if(strlen(buff)>20){
			buff[0]='*' ;
			buff[30]='*' ;
			buff[31]='\n' ;
		}

}
/****************************************************************************************************/
void GRABAR_LOG(char *msg, char *archivo, int origen, int nivel)
/****************************************************************************************************/
{        
	if( msg!= NULL && strlen(archivo) > 4 && origen <100 ) {
		char strArch[30];
	    char buffer2[STRLEN_LOG];
        int handle = 0;
		
		memset(buffer2,0,sizeof(buffer2));
		memset(strArch,0,sizeof(strArch));
        wxDateTime DataTime;
        //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
        wxMutexLocker lock(s_mutexLog);
        DataTime.SetToCurrent();
		memset(strArch,0,sizeof(strArch));
        _snprintf( strArch,29, "%s%s", PATH_LOGS,archivo);
        strArch[29] = '\0';

        #ifdef INVEL_L
        handle = open( strArch, O_RDWR, __LINE__, __FILE__ );
        #else
        handle = _open( strArch, O_WRONLY | O_BINARY/*, __LINE__, __FILE__*/ );
        #endif
		

		if( CORTAR_ARCHIVOS_LOG_GRANDES( ) > 10000 ){//EN BYTES
			long tamanio=CORTAR_ARCHIVOS_LOG_GRANDES( );
			/* ---> buscar esto en el codigo ticket -->> 2164 
			Esta modificación es provisoria y configurable,
			se pasa la cantidad EN BYTES... para cortar parcialmente
			los archivos logs y pasarlos a los baks.
			No es una opcion recomendables , debe ser eliminada al organizar mejor los logs

			*/
				if( handle > 0 &&( filelength(handle)> tamanio  ) ){
					char cop_strArch[50];
					 int fecha;
					if( UTILIZAR_DIA_OPERATIVO ){
						fecha = NRO_DIA_OPERATIVO;
					}else{
						fecha = GET_FECHA_DOS(  );
					}
					tamanio=filelength(handle);
					memset(cop_strArch,0,sizeof(cop_strArch));
					_snprintf( cop_strArch,sizeof(cop_strArch)-1, "%s%s%s.%02i", PATH_BACKUPS,archivo, DataTime.Format("%H-%M").c_str(),DAY( fecha ));
					cop_strArch[sizeof(cop_strArch)-1] = '\0';
					_close( handle );
					COPIAR_ARCHIVO( strArch, cop_strArch, __LINE__, __FILE__ );
					unlink(strArch);
					//_VACIAR_LOG( cop_strArch );
				
			#ifdef INVEL_L
				handle = open( strArch, O_RDWR, __LINE__, __FILE__ );
			#else
				handle = _open( strArch, O_WRONLY | O_BINARY/*, __LINE__, __FILE__*/ );
			#endif
				}
		}


        if( handle < 0 ) {
            handle = _CREAT( strArch,  S_IWRITE | S_IREAD, __LINE__, __FILE__ );
			if( handle > 0 ) {
				int i = 0;
				for(i=0;i<30; i++){
					memset( buffer2, 0, sizeof( buffer2 ) );
					ENCABEZADO_LOG_SISTEMA_2(  i, buffer2 );
					buffer2[STRLEN_LOG - 1] = '\0';
					_write( handle, buffer2, strlen( buffer2 ) );
				}
                _CLOSE( handle, __LINE__, __FILE__ );
                handle = _OPEN( strArch, O_WRONLY | O_BINARY, __LINE__, __FILE__ );
            }
        }

		//handle = _open( strArch, O_RDWR | (_open(strArch,O_RDONLY) > 0) ? O_CREAT : 0 );
        assert(handle >= 0);
        _lseek( handle, 0, SEEK_END );
			memset(buffer2,0,sizeof(buffer2));
			_snprintf(buffer2,STRLEN_LOG - 1,"%s[%03i] [N%02i] - %s\n",DataTime.Format("%d/%m/%y %H:%M:%S").c_str(),origen,nivel,msg);
        
        buffer2[STRLEN_LOG - 1] = '\0';
	    // mmancuso truchada /
		_write( handle, buffer2, strlen( buffer2 ) );
		_close( handle );
	}
}

    int TOMAR_FTP_SEGUNDO_PLANO(char *pOrigen,char *pDestino, int pModo, int pIdScript,int pIdNroSec){
        char pOrigen_aux[50];
		char password[20];
        char login[20];
        int rta = 3; //1 = ok, 2 = esperar, 3 = ejecutar, -1 = error
		
        if(Ftp_thread->getIdComandoScript() > 0 || (Ftp_thread->getIdNumeroSecuencia()== 2 && pIdNroSec==2 )){         
            if(Ftp_thread->getFinalizado()){                            //pregunta si ya finalizo
				if(Ftp_thread->getError()){                             //Pregunta si hay error
                    if(Ftp_thread->getReintento() == 3) {
                        /**/glog((char *)Ftp_thread->getStrError(),LOG_CON_SERVIDOR,2);
                        Ftp_thread->SetClearReintentos(); //reintentos en 0
						if( mReintento_aux == 1 ) {
							Ftp_thread->setIdComandoScript( 0 );// cereo mIdComandoScript
							mReintento_aux = 2;
						}
                        rta = -1;
                    }else{
                        rta = 3; //reintenta de nuevo
                    }
				}else{ //si no hay ningun error
                    if(Ftp_thread->getIdComandoScript() == pIdScript){  //si es el mismo script
                        rta = 1; //transferencia ok
                    }else{
                        rta = 3; //indica que ejecute el ftp
                    }
				
					Ftp_thread->SetClearReintentos(); //reintentos en 0
					
					if( Ftp_thread->getIdNumeroSecuencia() == 1  ) // si es el 1ero de 2 solo el 1ero limpio
						Ftp_thread->setIdComandoScript( 0 );// cereo mIdComandoScript

					if(Ftp_thread->getIdNumeroSecuencia()== 2 && pIdNroSec == 2 && rta ==1) {
						Ftp_thread->setgetIdNumeroSecuencia(0);
						Ftp_thread->setIdComandoScript( 0 );
					}
					mReintento_aux = 0;
                }
            }else{
                rta = 2; //esperar
            }

        }
		memset( pOrigen_aux, 0, sizeof( pOrigen_aux ) );
		strncpy( pOrigen_aux, pOrigen, sizeof(pOrigen_aux)-1 );
		if( mReintento_aux == 1 && rta == 3 ) {
			//Reintento con minuscula, con error y sin haber completado los 3 reintentos
			strlwr( pOrigen_aux );//Convierto nuevamente en minuscula para el reintento
		}
		if( rta == -1 ) {//Alcanza el tope de reintentos
			if(!mReintento_aux ) {
				//strupr( pOrigen_aux );//reintentos con mayuscula
				strlwr( pOrigen_aux );//reintentos con minuscula
				mReintento_aux = 1;
				rta = 3;
			} else {
				if( mReintento_aux == 2 ) {
					mReintento_aux = 0;
				}
			}

		}
        if(rta == 3){
            if( strcmp( SERVER_LOGIN, "NRO_CAJA" ) == 0 || strcmp( SERVER_LOGIN, "nro_caja" ) == 0 ) {
                _snprintf(login, sizeof(login)-1, "caja%s", NRO_NOM_CAJA );
            }
            else {
                strcpy( login, SERVER_LOGIN );
            }
            if( strcmp( SERVER_PASSWORD, "NRO_CAJA" ) == 0 || strcmp( SERVER_PASSWORD, "nro_caja" ) == 0 ) {
                _snprintf(password, sizeof(password)-1, "caja%s", NRO_NOM_CAJA );
            }
            else {
                strcpy( password, SERVER_PASSWORD );
            }
			if( MODO_DEBUG == 50 ) {
				char aa[100];
				
				_snprintf(aa, sizeof(aa)-1,"___login:%s password:%s",login, password);
				GRABAR_LOG_SISTEMA( aa ,LOG_CON_SERVIDOR,5);
			}
            Ftp_thread->setIpOrNameServer(Config->getIpFtp());
            Ftp_thread->setIdComandoScript(pIdScript);
			Ftp_thread->setgetIdNumeroSecuencia(pIdNroSec);
			Ftp_thread->ObtenerFtp(login,password,pOrigen_aux,pDestino,pModo);
            rta = 2; //esperar
        }
        return rta;
    }

    int TOMANDO_FTP(){
        if(Ftp_thread->getIdComandoScript() > 0 && !Ftp_thread->getFinalizado()){
            return 1;
        }
        return 0;
    }

	void LOAD_PANTALLA_ENTR(int b)
	{
    
		char straux[80];
		char ending[7];
		int offset;
		if(b){
			offset = 4;
			strcpy(ending,"_E.gif");
		}else{
			offset = 6;
			strcpy(ending,".gif");
		}
	
		/*strcpy(straux, panCobrosEsp->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panCobrosEsp->screenXml->setBackground((string)straux);
		panCobrosEsp->SetBackGround( panCobrosEsp->screenXml->getBackgroundXML().c_str() );*/
		
		strcpy(straux, panVentas->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panVentas->screenXml->setBackground((string)straux);
		panVentas->SetBackGround( panVentas->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panCierre->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panCierre->screenXml->setBackground((string)straux);
		panCierre->SetBackGround( panCierre->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panPagos->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panPagos->screenXml->setBackground((string)straux);
		panPagos->SetBackGround( panPagos->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panCobro->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panCobro->screenXml->setBackground((string)straux);
		panCobro->SetBackGround( panCobro->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panPagosCaja->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panPagosCaja->screenXml->setBackground((string)straux);
		panPagosCaja->SetBackGround( panPagosCaja->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panFondoFijo->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panFondoFijo->screenXml->setBackground((string)straux);
		panFondoFijo->SetBackGround( panFondoFijo->screenXml->getBackgroundXML().c_str() );
	
	    strcpy(straux, panRetiros->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panRetiros->screenXml->setBackground((string)straux);
		panRetiros->SetBackGround( panRetiros->screenXml->getBackgroundXML().c_str() );
		
		/*strcpy(straux, panBrowse->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panBrowse->screenXml->setBackground((string)straux);*/
	
		strcpy(straux, panStandby->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panStandby->screenXml->setBackground((string)straux);
		panStandby->SetBackGround( panStandby->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panCierreSub->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panCierreSub->screenXml->setBackground((string)straux);
		panCierreSub->SetBackGround( panCierreSub->screenXml->getBackgroundXML().c_str() );
	
		/*strcpy(straux, panPres->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panPres->screenXml->setBackground((string)straux);
		panPres->SetBackGround( panPres->screenXml->getBackgroundXML().c_str() );*/
	
		/*strcpy(straux, panInput->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panInput->screenXml->setBackground((string)straux);*/
	
		strcpy(straux, panCambioMedio->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panCambioMedio->screenXml->setBackground((string)straux);
		panCambioMedio->SetBackGround( panCambioMedio->screenXml->getBackgroundXML().c_str() );
	
		/*strcpy(straux, panMenu->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panMenu->screenXml->setBackground((string)straux);*/
	
		strcpy(straux, panPagosEsp->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panPagosEsp->screenXml->setBackground((string)straux);
		panPagosEsp->SetBackGround( panPagosEsp->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panMedioReint->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panMedioReint->screenXml->setBackground((string)straux);
		panMedioReint->SetBackGround( panMedioReint->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panDeudaSocio->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panDeudaSocio->screenXml->setBackground((string)straux);
	    panDeudaSocio->SetBackGround( panDeudaSocio->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panVuelto->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panVuelto->screenXml->setBackground((string)straux);
		panVuelto->SetBackGround( panVuelto->screenXml->getBackgroundXML().c_str() );
	
		/*strcpy(straux, panCheques->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panCheques->screenXml->setBackground((string)straux);
	    panCheques->SetBackGround( panCheques->screenXml->getBackgroundXML().c_str() );*/
	
	    strcpy(straux, panGrill->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panGrill->screenXml->setBackground((string)straux);
		panGrill->SetBackGround( panGrill->screenXml->getBackgroundXML().c_str() );

		strcpy(straux, panPagaTodo->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
		panPagaTodo->screenXml->setBackground((string)straux);
		panPagaTodo->SetBackGround( panPagaTodo->screenXml->getBackgroundXML().c_str() );
	
		strcpy(straux, panGarantia->screenXml->getBackgroundXML().c_str());
		straux[strlen(straux)-offset] = '\0';
		strcat(straux,ending);
	    panGarantia->screenXml->setBackground((string)straux);
		panGarantia->SetBackGround( panGarantia->screenXml->getBackgroundXML().c_str() );
		
		panVentas->Refresh();
		panCobro->Refresh();
		panPagos->Refresh();
		panPagosEsp->Refresh();
		//panCobrosEsp->Refresh();
		//panCheques->Refresh();
		panGarantia->Refresh();
		panVuelto->Refresh();
		panGrill->Refresh();
		panCambioMedio->Refresh();
		panMedioReint->Refresh();
		panPagaTodo->Refresh();
		panCierre->Refresh();
		panCierreSub->Refresh();
		panStandby->Refresh();
		//panPres->Refresh();
		panFondoFijo->Refresh();
		panRetiros->Refresh();
		panPagosCaja->Refresh();
		panDeudaSocio->Refresh();
	}

}//extern

/*****************************************************************************/
int PANT_VENTAS_ACTIVA( void )
/*****************************************************************************/
{
	return ((panVentas->isShowing())?SI:NO);
}

/*****************************************************************************/
int PANT_COBROS_ESP_ACTIVA( void )
/*****************************************************************************/
{
	return ((panCobrosEsp->isShowing())?SI:NO);
}

/*****************************************************************************/
int WIN_ACTIVA_ONTOP(void)
/*****************************************************************************/
{
	int rta = NO;

	if( panInput->isShowing()
			|| panBrowse->isShowing()
			|| panConsultaPromos->isShowing()
			|| panMenu->isShowing()) {
		rta = SI;
	}

	return (rta);
}
int IsScreenSaverActivo()
{
	int activo = 0;
	if(panScreenSaver->isShowing())
		activo = 1;
	return ( activo);
}

int IsStandbyActivo()
{
	int activo = NO;
	if(panStandby->isShowing())
		activo =SI;
	return(activo);

}

int IsCashActivo()
{
	int activo = NO;
	if(panPagos->isCashActivo())
		activo =SI;
	return(activo);

}

int GetNumerRowsCash()
{
	if(panPagos->isShowing())
        return(panPagos->GetNumberRows());
	if(panCambioMedio->isShowing());
		return(panCambioMedio->GetNumberRows());
	
	return 1 ;

}
void MostrarMemoria( )
{
  
  #define DIV 1024
  #define WIDTH 7

 char mensa[60];
 
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof (statex);
  GlobalMemoryStatusEx (&statex);

  sprintf (mensa,"Hay %*ld porciento de memoria en uso",
          WIDTH, statex.dwMemoryLoad);
  glog(mensa, LOG_VENTAS,5);
  sprintf (mensa,"Hay %*I64d  Kbytes libres de memoria fisica",
          WIDTH, statex.ullAvailPhys/DIV);
  glog(mensa, LOG_VENTAS,5);
  sprintf (mensa,"Hay %*I64d Kbytes libres de archivo de paging ",
          WIDTH, statex.ullAvailPageFile/DIV);
  glog(mensa, LOG_VENTAS,5);
  sprintf (mensa,"Hay %*I64d Kbytes libres de memoria virtual",
          WIDTH, statex.ullAvailVirtual/DIV);
  glog(mensa, LOG_VENTAS,5);
}

/*****************************************************************************/
//Devuelve el Path del TPSV.exe
char * GetAppExePath()
/*****************************************************************************/
{
	char buffer[MAX_PATH];
	int bytes =0;
	memset(buffer,0, MAX_PATH);
    bytes =  GetModuleFileName( NULL, buffer, MAX_PATH );

	if(bytes == 0)
		return " ";
	else { //bytes es el tamaño de la cadena del path
		//aqui devuelve con el nombre del binario ...\bin\TPSVd.exe , se lo saquemos
		int posicioncortar= bytes;
		string str(buffer);
		if( strstr( buffer, "TPSVd" ) != NULL ){
			posicioncortar -=9;
			//si existe TPSVd estoy en debug entonces tengo que cortar la cadena sacando TPSVd.exe
		}else
			posicioncortar -=8;
		//string cad1=str.substr(0,posicioncortar); 
		buffer[posicioncortar]=0;
		return buffer;
	}
}
