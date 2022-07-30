extern "C"
{
	#include <log.h>
	#include <_cr_var.h>

}
#include "VideoPlayer.h"
#include "MainPanel.h"
//#include <log.h>
#include "PanelVentas.h"
#include "PanelPagos.h"
#include "PanelCierre.h"
#include "PanelCobro.h"
#include "PanelPagosCaja.h"
#include "PanelFondoFijo.h"
#include "PanelRetiros.h"
#include "PanelStandby.h"
#include "PanelScreenSaver.h"
#include "PanelCierreSubmedios.h"
#include "PanelPresentacion.h"
#include "PanelInicio.h"
#include "PanelInput.h"
#include "CPanelBrowse.h"
#include "PanelCambioMedio.h"
#include "PanelCobrosEsp.h"
#include "PanelDeudaSocio.h"
#include "PanelVuelto.h"
#include "PanelCheques.h"
#include "PanelImportacion.h"
#include "PanelGrill.h"
#include "PanelPagosEsp.h"
#include "PanelMedioReint.h"
#include "CConfig.h"
#include "PanelPagaTodo.h"
#include "PanelGarantia.h"
#include "MainThread.h"
#include "PanelDenominaciones.h"
#include "PanelConsultaPromos.h"
#include "MsgPantalla.h"
int idPanelVentas = 1;
int idPanelPresentacion = 2;
int idPanelPagos = 3;
int idPanelCierre = 4;
int idPanelCierreSub = 5;
int idPanelCobro = 6;
int idPanelFondoFijo = 7;
int idPanelRetiros = 8;
int idPanelStandby = 9;
int idPanelCambioMedio = 10;
int idPanelPagosCaja = 11;
int idPanelCobrosEsp = 12;
int idPanelDeudaSocio = 13;
int idPanelPagosEsp = 14;
int idPanelMedioReint = 15;
int idPanelCheques = 16;
int idPanelVuelto = 17;
int idPanelPagaTodo = 18;
int idPanelGarantia = 19;
int idPanelDenominaciones = 20;
int idPanelConsultaPromos = 21;
int idPanelImportacion = 22;
int idPanelScreenSaver = 23;


extern CConfig *Config;

extern PanelVentas *panVentas;
extern PanelPagos *panPagos;
extern PanelCierre *panCierre;
extern PanelCobro *panCobro;
extern PanelFondoFijo *panFondoFijo;
extern PanelRetiros *panRetiros;
extern PanelStandby *panStandby;
extern PanelCierreSubmedios *panCierreSub;
extern PanelPresentacion *panPres;
extern PanelInput *panInput;
extern CPanelBrowse *panBrowse;
extern PanelCambioMedio *panCambioMedio;
extern PanelPagosCaja *panPagosCaja;
extern PanelCobrosEsp *panCobrosEsp;
extern PanelDeudaSocio *panDeudaSocio;
extern PanelVuelto *panVuelto;
extern PanelDenominaciones *panDenominaciones;
extern PanelCheques *panCheques;
extern PanelImportacion *panImportacion;
extern PanelGrill *panGrill;
extern PanelPagosEsp *panPagosEsp;
extern PanelMedioReint *panMedioReint;
extern PanelPagaTodo *panPagaTodo;
extern PanelGarantia *panGarantia;
extern MainThread *main_thread;
extern PanelConsultaPromos *panConsultaPromos;
extern CWindow *browseWin;
extern PanelScreenSaver *panScreenSaver;
//PanelInput *panInput;
extern MsgPantalla *MsgPantalla_thread;
#include "AppMain.h"

#define panelVentas (*panVentas)
#define panelPagos (*panPagos)
#define panelCierre (*panCierre)
#define panelCobro (*panCobro)
#define panelFondoFijo (*panFondoFijo)
#define panelStandby (*panStandby)
#define panelCierreSub (*panCierreSub)
#define panelPres (*panPres)
#define panelRetiros (*panRetiros)
#define panelBrowse (*panBrowse)
#define panelCambioMedio (*panCambioMedio)
#define panelPagosCaja (*panPagosCaja)
#define panelCobrosEsp (*panCobrosEsp)
#define panelDeudaSocio (*panDeudaSocio)
#define panelVuelto (*panVuelto)
#define panelDenominaciones (*panDenominaciones)
#define panelCheques (*panCheques)
#define panelImportacion (*panImportacion)
#define panelPagosEsp (*panPagosEsp)
#define panelMedioReint (*panMedioReint)
#define panelPagaTodo (*panPagaTodo)
#define panelGarantia (*panGarantia)
#define panelConsultaPromos (*panConsultaPromos)
#define panelScreenSaver (*panScreenSaver)

static wxMutex s_mutexMsg;

void MainPanel::NuevoPanelPresentacion( const string &ver, const string &fec, const string &branch )
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelVentas.setVisible( false );
    panelPres.NomVersion->setText( ver );
    panelPres.NomVersion->Draw();
    panelPres.NomBranch->setText( branch );
    panelPres.NomBranch->Draw();
    panelPres.FecOper->setText( fec );
    panelPres.FecOper->Draw();
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
    activarPanelPresentacion();
}

void MainPanel::SetCurrPanel( int recuperar )
{
	if( recuperar ) {
		if(panStandby->isShowing()){
			//Al activarser primero ScreenSaver setea Pant.Venta(solo en esta pant. se activa standby)
			//Luego se activa standby. Al desactivar ScreenSaver tiene que recuperar el current = Standby
			//y no venta.
			currPanel = currPanel_ant;
			currPaneltouch = idPanelStandby;
			currPanelMsg = idPanelStandby;
		} else {
			currPanel = currPanel_ant;
			currPaneltouch = currPaneltouch_ant;
			currPanelMsg = currPanelMsg_ant;
		}
	} else {
		currPanel_ant = currPanel;
		currPaneltouch_ant = currPaneltouch;
		currPanelMsg_ant = currPanelMsg;
	}
}

void MainPanel::activarPanelVentas()
{
    //#ifndef INVEL_L
	if(!panVentas->isShowing())
		DIRECT_VIDEO_PLAY_DEFAULT(Config->get_videoVentas());
    if(ACELERACION_PANTALLAS == 0)
		MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL VENTAS",LOG_PANTALLAS,5 );
    currPanel = idPanelVentas;
	currPaneltouch = idPanelVentas;
    currPanelMsg = idPanelVentas;
    panelVentas.Activate();
    //panelList[idPanelVentas] =panelVentas;
    incrementStack( currPanel );
    //this.validate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelVentas.getFocus() );
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)
		MutexGuiLeave();
    if(ACELERACION_PANTALLAS == SI)
		panelVentas.mYield();
	//MostrarMensaje( " " );
	//DIRECT_VIDEO_PLAY(Config->get_videoVentas());
	panelVentas.SetFocus();
}


void MainPanel::activarPanelCobrosEsp()
{
    GRABAR_LOG_SISTEMA( "ACTIVA COBROS ESPECIALES",LOG_PANTALLAS,5 );
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif    
    currPanel = idPanelCobrosEsp;
    currPaneltouch = currPanel;
    panelCobrosEsp.Activate();
    //panelCobrosEsp.Init();
    incrementStack( currPanel );
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelCobrosEsp.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::activarPanelDeudaSocio()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    //GRABAR_LOG_SISTEMA( "ACTIVAR DEUDA SOCIO" );
    currPanel = idPanelDeudaSocio;
    currPaneltouch = currPanel;
    panelDeudaSocio.Activate();
    panelDeudaSocio.Init();
    incrementStack( currPanel );
	( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelDeudaSocio.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}
void MainPanel::activarPanelDenominaciones()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL DENOMINACIONES", LOG_PANTALLAS,5);
    currPanel = idPanelDenominaciones;
    currPaneltouch = currPanel;
	panelDenominaciones.Activate();
    panelDenominaciones.Init();
	panelDenominaciones.clearInputs();
    incrementStack( currPanel );
	( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelDenominaciones.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::desactivarPanelDenominaciones()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelDenominaciones.DesActivate();
    //#ifndef INVEL_L
    MutexGuiLeave();
    MutexGuiEnter();
    //#endif
    panelDenominaciones.ClearGrid();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}
void MainPanel::activarPanelVuelto()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL VUELTO" ,LOG_PANTALLAS,5);
    currPanel = idPanelVuelto;
    currPaneltouch = currPanel;
    panelVuelto.Activate();
    panelVuelto.Init();
    incrementStack( currPanel );
	( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelVuelto.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::desactivarPanelVuelto()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelVuelto.DesActivate();
    //#ifndef INVEL_L
    MutexGuiLeave();
    MutexGuiEnter();
    //#endif
    panelVuelto.ClearGrid();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::desactivarPanelDeudaSocio()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelDeudaSocio.DesActivate();
    //#ifndef INVEL_L
    MutexGuiLeave();
    MutexGuiEnter();
    //#endif
    panelDeudaSocio.ClearGrid();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::activarPanelCheques()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL CHEQUES", LOG_PANTALLAS,5);
    currPanel = idPanelCheques;
    currPaneltouch = currPanel;
    panelCheques.Activate();
    panelCheques.Init();
    incrementStack( currPanel );
	( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelCheques.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}



void MainPanel::desactivarPanelCheques()
{
   // #ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelCheques.DesActivate();
    //#ifndef INVEL_L
    MutexGuiLeave();
    MutexGuiEnter();
    //#endif
    panelCheques.ClearGrid();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::activarPanelImportacion()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL IMPORTACION", LOG_PANTALLAS,5);
    currPanel = idPanelImportacion;
    currPaneltouch = currPanel;
    panelImportacion.Activate();
    panelImportacion.Init();
    incrementStack( currPanel );
	( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelImportacion.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}



void MainPanel::desactivarPanelImportacion()
{
    //#ifndef INVEL_L
    //MutexGuiEnter();
    //#endif
    panelImportacion.DesActivate();
    //#ifndef INVEL_L
    //MutexGuiLeave();
    //MutexGuiEnter();
    //#endif
    panelImportacion.ClearGrid();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    //MutexGuiLeave();
    //#endif
}


void MainPanel::desactivarPanelPrincipal()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelPres.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::desactivarPanelVentas()
{
    #ifndef INVEL_L
    MutexGuiEnter();
    #endif
	KILL_ALL_VIDEOS();
    //panelVentas.setVisible( false );
    panelVentas.DesActivate();
    decrementStack();
    currPanel = -1;
    #ifndef INVEL_L
    MutexGuiLeave();
    #endif
}


void MainPanel::activarPanelPagos()
{
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)
		MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL PAGOS" ,LOG_PANTALLAS,5);
    currPanel = idPanelPagos;
    currPaneltouch = currPanel;
    incrementStack( currPanel );
    currPanelMsg = idPanelPagos;
    //panelList[idPanelPagos] =panelPagos;
    panelPagos.Activate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPagos.getFocus() );
    //  this.validate();
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)	
		MutexGuiLeave();
	MsgPantalla_thread->AddMsg( " ", 0, 0 );
	//MOSTRAR_MENSAJE2(" ",NO,NO);
    //MostrarMensaje( " " );
}


void MainPanel::activarPanelFondoFijo()
{
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)	
		MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL FONDO FIJO", LOG_PANTALLAS,5);
    currPanel = idPanelFondoFijo;
    currPaneltouch = currPanel;
    incrementStack( currPanel );
    currPanelMsg = idPanelFondoFijo;
    //panelList[idPanelFondoFijo] =panelFondoFijo;
    panelFondoFijo.Activate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelFondoFijo.getFocus() );
    //  this.validate();
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)    
		MutexGuiLeave();
    //MostrarMensaje( " " );
    //#endif
}


void MainPanel::activarPanelRetiros()
{
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)
		MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL RETIROS", LOG_PANTALLAS,5);
    currPanel = idPanelRetiros;
    currPaneltouch = currPanel;
    incrementStack( currPanel );
    currPanelMsg = idPanelRetiros;
    //panelList[idPanelRetiros] =panelRetiros;
    panelRetiros.Activate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelRetiros.getFocus() );
    //  this.validate();
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)
		MutexGuiLeave();
    //MostrarMensaje( " " );
}


void MainPanel::desactivarPanelPagos()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelPagos.setVisible( false );
    panelPagos.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::desactivarPanelFondoFijo()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelFondoFijo.setVisible( false );
    panelFondoFijo.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::desactivarPanelRetiros()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelRetiros.setVisible( false );
    panelRetiros.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}




void MainPanel::activarPanelPresentacion()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL PRESENTACION", LOG_PANTALLAS,5);
    panelPres.Activate();
    currPanel = idPanelPresentacion;
    currPaneltouch = currPanel;
    currPanelMsg = idPanelPresentacion;
    //panelList[idPanelPresentacion] =panelPres;
    incrementStack( currPanel );
    //this.validate();
	while( !panelPres.isShowing( ) ) {
	//Sincronizamos el hilo con la caja para que se visualice
	//los objetos asociados a la pant. presentacion
		#ifdef INVEL_W
			Sleep(1);
		#else
			usleep(10000);
		#endif
	}	
	if( panelPres.isShowCajeroActual(  ) ) {
		( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPres.input_usuario );
	}
    MutexGuiLeave();
    //MostrarMensaje( " " );
}


void MainPanel::activarPanelCobros()
{
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)	
		MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL COBROS", LOG_PANTALLAS,5);
    currPanel = idPanelCobro;
    currPaneltouch = currPanel;
    panelCobro.Activate();
     currPanelMsg = idPanelCobro;
    //panelList[idPanelCobro] =panelCobro;
    incrementStack( currPanel );
    //this.validate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelCobro.getFocus() );
    panCobro->clearInputs();
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)
		MutexGuiLeave();
    //MostrarMensaje( " " );
}


void MainPanel::desactivarPanelCobros()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelCobro.clearInputs();
    panelCobro.setVisible( false );
    panelCobro.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::desactivarPanelCobrosEsp()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    //panelCobrosEsp.clearInputs();
    panelCobrosEsp.setVisible( false );
    panelCobrosEsp.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::activarPanelPagosCaja()
{
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL PAGOS X CAJA" ,LOG_PANTALLAS,5);
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)
		MutexGuiEnter();
    //#endif
    currPanel = idPanelPagosCaja;
    currPaneltouch = currPanel;
    panelPagosCaja.Activate();
    currPanelMsg = idPanelPagosCaja;
    //panelList[idPanelCobro] =panelCobro;
    incrementStack( currPanel );
    //this.validate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPagosCaja.getFocus() );
    panPagosCaja->clearInputs();
    //#ifndef INVEL_L
	if(ACELERACION_PANTALLAS == 0)
		MutexGuiLeave();
    //MostrarMensaje( " " );
}


void MainPanel::desactivarPanelPagosCaja()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelPagosCaja.clearInputs();
    panelPagosCaja.setVisible( false );
    panelPagosCaja.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::activarPanelPagosEsp()
{
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL PAGOS ESP", LOG_PANTALLAS,5);
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    currPanel = idPanelPagosEsp;
    currPaneltouch = currPanel;
    panelPagosEsp.Activate();
    currPanelMsg = idPanelPagosEsp;
    //panelList[idPanelCobro] =panelCobro;
    incrementStack( currPanel );
    //this.validate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPagosEsp.getFocus() );
    panPagosEsp->clearInputs();
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::desactivarPanelPagosEsp()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelPagosEsp.clearInputs();
    panelPagosEsp.setVisible( false );
    panelPagosEsp.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::activarPanelStandby()
{
    //#ifndef INVEL_L
	if( ACELERACION_PANTALLAS == 0 ) 
		MutexGuiEnter();
    //#endif
    //currPanel = idPanelStandby;
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL STAND BY", LOG_PANTALLAS,5);
    currPaneltouch = idPanelStandby;
    currPanelMsg = idPanelStandby;
    panelStandby.Activate();
    //panelList[idPanelStandby] = panelStandby;
    incrementStack( currPanel );
    //  this.validate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelStandby.getFocus() );
    //#ifndef INVEL_L
	if( ACELERACION_PANTALLAS == 0 ) 
		MutexGuiLeave();
    //MostrarMensaje( " " );
}


void MainPanel::desactivarPanelStandby()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelStandby.setVisible( false );
    panelStandby.DesActivate();
    decrementStack();
    //currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}



void MainPanel::activarPanelScreenSaver()
{
    MutexGuiEnter();

    //GRABAR_LOG_SISTEMA( "ACTIVA PANEL SCREEN SAVER", LOG_PANTALLAS,2);
    currPaneltouch = idPanelScreenSaver;
    currPanelMsg = idPanelScreenSaver;
    panelScreenSaver.Activate();
    incrementStack( currPanel );
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelScreenSaver.getFocus() );

    MutexGuiLeave();
}


void MainPanel::desactivarPanelScreenSaver()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelScreenSaver.setVisible( false );
    panelScreenSaver.DesActivate();
    decrementStack();
    //currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}



void MainPanel::activarPanelMedioReint()
{
    
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL MEDIOS PARA REINTEGRO" ,LOG_PANTALLAS,5);
    currPanel = idPanelMedioReint;
    currPaneltouch = currPanel;
    currPanelMsg = idPanelMedioReint;
    panelMedioReint.Activate();
    incrementStack( currPanel );
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelMedioReint.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //MostrarMensaje( " " );
}

void MainPanel::desactivarPanelMedioReint()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelMedioReint.setVisible( false );
    panelMedioReint.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::activarPanelConsultaPromos()
{
    //#ifndef INVEL_L
    //MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL CONSULTA PROMOCIONES", LOG_PANTALLAS,5);
    currPanel = idPanelConsultaPromos;
    currPaneltouch = currPanel;
    panelConsultaPromos.Activate();
    panelConsultaPromos.Init();
    incrementStack( currPanel );
	( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelConsultaPromos.getFocus() );
    //#ifndef INVEL_L
    //MutexGuiLeave();
    //#endif
}



void MainPanel::desactivarPanelConsultaPromos()
{
   // #ifndef INVEL_L
    //MutexGuiEnter();
    //#endif
    panelConsultaPromos.DesActivate();
    //#ifndef INVEL_L
    //MutexGuiLeave();
    //MutexGuiEnter();
    //#endif
    panelConsultaPromos.ClearGrid();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    //MutexGuiLeave();
    //#endif
}

void MainPanel::desactivarTodosLosPaneles()
{
    int desactivado = 0;

    if( panelPres.isShowing() ) {
        panelPres.DesActivate();
        desactivado = 1;
    }
    else if( panelBrowse.isShowing() ) {
        panelBrowse.DesActivate();
        desactivado = 1;
    }
    else if( panelPagos.isShowing() ) {
        panelPagos.init( "" );
        panelPagos.DesActivate();
        desactivado = 1;
    }
    else if( panelConsultaPromos.isShowing() ) {
		//if( panelVentas.isShowing() ) {
		panelConsultaPromos.DesActivate();
        desactivado = 1;
    }
    else if( panelPagaTodo.isShowing() ) {
        panelPagaTodo.DesActivate();
        desactivado = 1;
    }
	else if( panelGarantia.isShowing() ) {
        panelGarantia.DesActivate();
        desactivado = 1;
    }
	else if( panelCobro.isShowing() ) {
        panelCobro.DesActivate();
        desactivado = 1;
    }
    else if( panelPagosCaja.isShowing() ) {
        panelPagosCaja.DesActivate();
        desactivado = 1;
    }
    else if( panelPagosEsp.isShowing() ) {
        panelPagosEsp.DesActivate();
        desactivado = 1;
    }
    else if( panelCierreSub.isShowing() ) {
        panelCierreSub.DesActivate();
        desactivado = 1;
    }
    else if( panelCierre.isShowing() ) {
        panelCierre.DesActivate();
        desactivado = 1;
    }
    else if( panelFondoFijo.isShowing() ) {
        panelFondoFijo.DesActivate();
        desactivado = 1;
    }
    else if( panelRetiros.isShowing() ) {
        panelRetiros.DesActivate();
        desactivado = 1;
    }
    else if( panelCambioMedio.isShowing() ) {
        panelCambioMedio.DesActivate();
        desactivado = 1;
    }
    else if( panelCobrosEsp.isShowing() ) {
        panelCobrosEsp.DesActivate();
        desactivado = 1;
    }
    else if( panelMedioReint.isShowing() ) {
        panelMedioReint.DesActivate();
        desactivado = 1;
    }
	else if( panelVentas.isShowing() ) {
        panelVentas.DesActivate();
        desactivado = 1;
    }
    if( desactivado ) {
        decrementStack();
    }
    currPanel = -1;
}

void MainPanel::MostrarMensaje( const string &s )
{
    main_thread->Yield();
    //wxYield(); //que ejecute los otros hilos antes de seguir
    Sleep(300);
    char msgTmp[200];
    //Se bloquea el ingreso, este objeto desbloquea el mutex en su destructor
    wxMutexLocker lock(s_mutexMsg);
	memset(msgTmp,0,sizeof(msgTmp));
    _snprintf(msgTmp,200,"MOSTRANDO MSG: %s PANEL: %i", s.c_str(), currPanelMsg);
    msgTmp[199] = 0;
	if( s.length() > 1)
		GRABAR_LOG_SISTEMA( msgTmp ,LOG_PANTALLAS,5);

	memset(msgTmp,0,sizeof(msgTmp));
    if( currPanelMsg == idPanelPresentacion ) {
        panelPres.label_mensaje->setText( s );
        panelPres.label_mensaje->Draw();
    }
    else if( currPanelMsg == idPanelVentas ) {
        panelVentas.label_mensaje->setText( s );
        panelVentas.label_mensaje->Draw();
    }
    else if( currPanelMsg == idPanelCobro ) {
        panelCobro.setLabelMensaje( s );
    }
    else if( currPanelMsg == idPanelPagosCaja ) {
		panelPagosCaja.label_mensaje->setText( s );
		panelPagosCaja.label_mensaje->Draw();
    }
    else if( currPanelMsg == idPanelPagos ) {
        panelPagos.label_mensaje->setText( s );
        panelPagos.label_mensaje->Draw();
    }
    else if( currPanelMsg == idPanelCierreSub ) {
        panelCierreSub.label_mensaje->setText( s );
		panelCierreSub.label_mensaje->Draw();
		//sglog("pyB");
    }
    else if( currPanelMsg == idPanelCierre ) {
		panelCierre.label_mensaje->setText( s );
		panelCierre.label_mensaje->Draw();
		//sglog("pyD");
    }
    else if( currPanelMsg == idPanelFondoFijo ) {
        panelFondoFijo.label_mensaje->setText( s );
        panelFondoFijo.label_mensaje->Draw();
    }
    else if( currPanelMsg == idPanelRetiros ) {
        panelRetiros.label_mensaje->setText( s );
        panelRetiros.label_mensaje->Draw();
    }
    else if( currPanelMsg == idPanelStandby ) {
        panelStandby.label_mensaje->setText( s );
        panelStandby.label_mensaje->Draw();
    }
    else if( currPanelMsg == idPanelCambioMedio ) {
        panelCambioMedio.label_mensaje->setText( s );
        panelCambioMedio.label_mensaje->Draw();
    }
}


void MainPanel::BorrarMensaje()
{
    MostrarMensaje("");   
}


CInput * MainPanel::getFocus()
{
    if( panInput->isShowing() ) {
        return panInput->getFocus();
    }
    else if( panelBrowse.isShowing() ) {
        return panelBrowse.getFocus();
    }
    else if( panelPres.isShowing() ) {
        return panelPres.getFocus();
    }
    else if( panelPagos.isShowing() ) {
        return panelPagos.getFocus();
    }
    else if( panelVentas.isShowing() && panelConsultaPromos.isShowing() ) {
		return panelConsultaPromos.getFocus();
    }
	else if( panelVentas.isShowing() && !panelConsultaPromos.isShowing() ) {
        return panelVentas.getFocus();
    }
    else if( panelPagaTodo.isShowing() ) {
        return panelPagaTodo.getFocus();
    }
	else if( panelGarantia.isShowing() ) {
        return panelGarantia.getFocus();
    }
	else if( panelCobro.isShowing() ) {
        return panelCobro.getFocus();
    }
    else if( panelPagosCaja.isShowing() ) {
        return panelPagosCaja.getFocus();
    }
    else if( panelPagosEsp.isShowing() ) {
        return panelPagosEsp.getFocus();
    }
    else if( panelCierreSub.isShowing() ) {
        return panelCierreSub.getFocus();
    }
    else if( panelCierre.isShowing() ) {
        return panelCierre.getFocus();
    }
    else if( panelFondoFijo.isShowing() ) {
        return panelFondoFijo.getFocus();
    }
    else if( panelRetiros.isShowing() ) {
        return panelRetiros.getFocus();
    }
    else if( panelCambioMedio.isShowing() ) {
        return panelCambioMedio.getFocus();
    }
    else if( panelCobrosEsp.isShowing() ) {
        return panelCobrosEsp.getFocus();
    }
    else if( panelMedioReint.isShowing() ) {
        return panelMedioReint.getFocus();
    }
    return NULL;
}


int MainPanel::getch()
{
    if( panInput->isShowing() ) {
        return panInput->getFocus()->Getch();
    }
    else if( panelPres.isShowing() ) {
        return panelPres.getFocus()->Getch();
    }
    else if( panelPagos.isShowing() ) {
        return panelPagos.getFocus()->Getch();
    }
    else if( panelVentas.isShowing() ) {
        return panelVentas.getFocus()->Getch();
    }
    else if( panelCobro.isShowing() ) {
        return panelCobro.getFocus()->Getch();
    }
    else if( panelPagosCaja.isShowing() ) {
        return panelPagosCaja.getFocus()->Getch();
    }
    else if( panelPagosEsp.isShowing() ) {
        return panelPagosEsp.getFocus()->Getch();
    }
    else if( panelCierreSub.isShowing() ) {
        return panelCierreSub.getFocus()->Getch();
    }
    else if( panelFondoFijo.isShowing() ) {
        return panelFondoFijo.getFocus()->Getch();
    }
    else if( panelRetiros.isShowing() ) {
        return panelRetiros.getFocus()->Getch();
    }
    else if( panelCierre.isShowing() ) {
        return panelCierre.getFocus()->Getch();
    }
    else if( panelCambioMedio.isShowing() ) {
        return panelCambioMedio.getFocus()->Getch();
    }
    else if( panelCobrosEsp.isShowing() ) {
        return panelCobrosEsp.getFocus()->Getch();
    }
    else if( panelMedioReint.isShowing() ) {
        return panelMedioReint.getFocus()->Getch();
    }
	else if( panelStandby.isShowing() ) {
        return panelStandby.getFocus()->Getch();
    }
	else if( panelScreenSaver.isShowing() ) {
        return panelScreenSaver.getFocus()->Getch();
    }
    return -999;
}


void MainPanel::PanelMoveInput( short type, const string &s, const string &s1 )
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panInput->changeFocus( type );
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panInput->getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::PanelPedir( short type, const string &s, const string &s1, const string &mascara, const string &s2 )
{
    MutexGuiEnter();
	panInput->deleteInputs();

	if( panInput->LabelTitulo->x_ant ) {
		panInput->LabelTitulo->setPosition(panInput->LabelTitulo->x_ant, panInput->LabelTitulo->y_ant);
		panInput->LabelTitulo->x_ant = 0;
		panInput->LabelTitulo->y_ant = 0;
	}

	if( ( type == _SIMPLE ) || ( type == _MOVIL ) ) {
        if( mascara.length() ) {
            panInput->addInput( s1, mascara );
        }
        else {
            panInput->addInput( s1 );
        }
        panInput->setLabels( type, s );
    }

    /***********************************************************************************/

    if( type == _AJUSTABLE ) {
        int last_pos_cad = 0;
        int last_pos_tam = 0;
        int tamanio = 0, length_nCadena = 0, multiplicador = 0;
        float a = 4;

        char Cadena[ 40 ] = { 0 };
        char Digito[ 10 ] = { 0 };
        basic_string <char>:: value_type *CadenaPtr = Cadena;
        basic_string <char>:: value_type *DigitoPtr = Digito;
        basic_string <char>:: size_type nCadena;
        basic_string <char>:: size_type nCadena_M;
        basic_string <char>:: size_type nDigito;
        basic_string <char>:: size_type nDigito_M;
        
        int pos_cad = s1.find_first_of( '~', 0 );
        int pos_tam = s2.find_first_of( '~', 0 );
        
        /**************** Determinamos si viene o no concatenado los label ****************/

        //Cuando hay mas de una cadena
        if( pos_cad != -1 ) {
            length_nCadena = pos_cad + 1;
            //s2.copy( cadena donde copia, Numero de caracteres a copiar, Apartir de que posicion se copia  )
            //retorna el nro de caracteres copiados 
            nCadena = s1.copy( CadenaPtr , pos_cad, 0  );
            nDigito = s2.copy( DigitoPtr , pos_tam, 0  );
            nCadena_M = 0;
            nDigito_M = 0;
        } else { //Cuando hay una sola cadena
            nCadena = s1.length( );
            nDigito = s2.length( );
            nCadena_M = s1.copy( CadenaPtr , nCadena, 0  );
            nDigito_M = s2.copy( DigitoPtr , nDigito, 0  );
            nDigito_M = atoi( DigitoPtr );
            pos_cad = nCadena;
            pos_tam = nDigito;
        }
        if( length_nCadena ) {
            /*** Buscamos el label y digitos configurados mas grandes para graficar el input y panel ***/

            while( length_nCadena <= ( int )s1.length( ) ) {
               
                //Compara el mayor tamaño entre las cadenas 
                if( nCadena > ( int )nCadena_M ) {
                    nCadena_M = nCadena;
                }
                
                //Compara el mayor tamaño entre digitos configurados 
                if( atoi( DigitoPtr ) > ( int )nDigito_M ) {
                    nDigito_M = atoi( DigitoPtr );
                }
                
                //Proxima posicion y tamaño de la cadena. 
                last_pos_cad = pos_cad + 1;
                pos_cad = s1.find_first_of( '~', last_pos_cad );
                last_pos_tam = pos_tam + 1;
                pos_tam = s2.find_first_of( '~', last_pos_tam );
                
                //Verificamos si hay otra cadena
                //pos_cad = -1 si no hay otro '~' en la cadena total
                if( pos_cad == -1  ) {
                    //Posicion y tamaño de la cadena apartir de la posicion last_pos_cad
                    nCadena = s1.length( ) - last_pos_cad ;
                    nDigito = s2.length( ) - last_pos_tam ;
                    pos_cad = last_pos_cad - 1;
                    pos_tam = last_pos_tam - 1;
                } else {
                    nCadena = pos_cad - last_pos_cad;
                    nDigito = pos_tam - last_pos_tam;
                }

                //Inicializo arrays
                memset( Cadena, 0, sizeof( Cadena ) );
                memset( Digito, 0, sizeof( Digito ) );
                
                //s2.copy( cadena donde copia, Numero de caracteres a copiar, Apartir de que posicion se copia  )
                //retorna el nro de caracteres copiados 
                //Optenemos la proxima cadena con su tamaño
                nCadena = s1.copy( CadenaPtr , nCadena, last_pos_cad );
                nDigito = s2.copy( DigitoPtr , nDigito, last_pos_tam );

                //Calculamos tamaño total de la cadena hasta el momento
                //Verificamos que no sea el ultimo para sumarle 1 ('~')
                length_nCadena = length_nCadena + nCadena;

                if( s1.length( ) != length_nCadena ) {
                    length_nCadena = length_nCadena + 1;
                } 
            }

            /****** Determinamos la cadena y el digito configurado a pasar por el input ******/
            
            //Inicializacion
            memset( Cadena, 0, sizeof( Cadena ) );
            memset( Digito, 0, sizeof( Digito ) );
            last_pos_cad = 0;
            last_pos_tam = 0;
            pos_cad = 0;
            pos_tam = 0;
            length_nCadena = 0;

            pos_cad = s1.find_first_of( '~', 0 );
            pos_tam = s2.find_first_of( '~', 0 );
            nDigito = s2.copy( DigitoPtr , pos_tam, 0  );
            tamanio = atoi( DigitoPtr );
            
            while( pos_cad != -1 ) {
                //Inicializacion
                memset( Digito, 0, sizeof( Digito ) ); 
                
                //Para que se ajuste mejor el input a la cantidad de dig a ingresar
                switch( tamanio ) {
                    case 1:
                        multiplicador = 17;
                        break;
                    case 2:
                        multiplicador = 15;
                        break;
                    case 3:
                        multiplicador = 13;
                        break;
                    default:
                        if( tamanio <= 6 ) {
                            multiplicador = 12;
                        } else {
                            multiplicador = 11;
                        }
                }
                panInput->addInput( s1.substr( last_pos_cad, pos_cad - last_pos_cad ), 
                    nCadena_M * 9.5, 10 * a, tamanio * multiplicador, 20, "",
                    ( nCadena_M * 9.5 ) + ( nDigito_M * 11 ) );

                tamanio = 0;
                last_pos_cad = pos_cad + 1;
                pos_cad = s1.find_first_of( '~', last_pos_cad );
                last_pos_tam = pos_tam + 1;
                pos_tam = s2.find_first_of( '~', last_pos_tam );
                if( pos_tam  == -1  ) {//para la ultima cadena
                    nDigito = s2.length( ) - last_pos_tam ;
                } else {
                    nDigito = pos_tam - last_pos_tam;
                }
                nDigito = s2.copy( DigitoPtr , nDigito , last_pos_tam  );
                tamanio = atoi( DigitoPtr );
                a = a + 2.3;
            }
            pos_cad = last_pos_cad;
            pos_tam = last_pos_tam;
        }

        nCadena = s1.length( ) - last_pos_cad;
        nDigito = s2.length( ) - last_pos_tam;

        memset( Digito, 0, sizeof( Digito ) );
        nDigito = s2.copy( DigitoPtr , nDigito, last_pos_tam  );
        tamanio = atoi( DigitoPtr );
        
        //Para que se ajuste mejor el input a la cantidad de dig a ingresar
        switch( tamanio ) {
            case 1:
                multiplicador = 17;
                break;
            case 2:
                multiplicador = 15;
                break;
            case 3:
                multiplicador = 13;
                break;
            default:
                if( tamanio <= 6 ) {
                    multiplicador = 12;
                } else {
                    multiplicador = 11;
                }
        }
        panInput->addInput( s1.substr( last_pos_cad, pos_cad ), 
            nCadena_M * 9.5, 10 * a, tamanio * multiplicador, 20, "",
            ( nCadena_M * 9.5 ) + ( nDigito_M * 11 ) );

        panInput->LabelTitulo->setText( s );

    }
    /***********************************************************************************/
    if( type == _DOBLE ) {
		panInput->LabelTitulo->x_ant = panInput->LabelTitulo->x;
		panInput->LabelTitulo->y_ant = panInput->LabelTitulo->y;
		panInput->LabelTitulo->setPosition( panInput->LabelTitulo->x * 2, panInput->LabelTitulo->y_ant );
        panInput->addInput( s1, 65, 40, 500, 20, "", -1 );
		panInput->setLabels( type, s );
    }
    if( type == _CONCATENADO ) {
        int last_pos = 0;
        int pos = s1.find_first_of( '~', 0 );
        while( pos != -1 ) {
            panInput->addInput( s1.substr( last_pos, pos - last_pos ) );
            last_pos = pos + 1;
            pos = s1.find_first_of( '~', pos + 1 );
        }
        panInput->addInput( s1.substr( last_pos, s1.length() ) );
        panInput->LabelTitulo->setText( s );
    }
    if( type == _HIBRIDO ) {
        char tokens[] = {'~','&'};
        int pos = s1.find_first_of( tokens, 0, sizeof( tokens ) );
        int lastPos = s1.find_first_of( tokens, pos + 1, sizeof( tokens ) );

        while( lastPos != -1 ) {
            if( s1[pos] == '~' ) {
                panInput->addInput( s1.substr( pos + 1, lastPos - pos - 1 ) );
            }
            else //&
            {
                panInput->addLabel( s1.substr( pos + 1, lastPos - pos - 1 ) );
            }
            pos = lastPos;
            lastPos = s1.find_first_of( tokens, pos + 1, sizeof( tokens ) );
        }

        if( pos - s1.length() > 0 ) {
            if( s1[pos] == '~' ) {
                panInput->addInput( s1.substr( pos + 1, s1.length() - pos ) );
            }
            else //&
            {
                panInput->addLabel( s1.substr( pos + 1, s1.length() - pos ) );
            }
        }

        panInput->LabelTitulo->setText( s );
    }
    panInput->adjust();
	
	{//reposicionamiento de acuerdo a la pantalla de ventas
		int x=0,y=0, t=0,l=0,inT=0,inL=0;
		panVentas->GetSize(&t,&l);
		panVentas->GetPosition(&x,&y);

		panInput->GetSize(&inT,&inL);
		
		panInput->Move( x +(t/2)-(inT/2),y+(l/2)-(inL/2));
	}
	//este es el originaL
	//panInput->Center();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif

    if( type != _MOVIL ) {
        panInput->Activate();
        ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panInput->getFocus() );
    }

    //#ifdef INVEL_L
    panInput->Refresh();
    //#endif

   //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
	wxYield();
}

void MainPanel::EsconderPanelPedir( const string &s )
{
    if( panInput ) {
        if( !strcmp( s.c_str(), "si" ) ) {
            panInput->DesActivate();
        }
        else {
            panInput->Activate();
        }
    }
}

void MainPanel::FinPanelPedir()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    if( panInput ) {
        panInput->deleteInputs();
        panInput->DesActivate();
    }
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::PanelLista( const string &s, int type )
{
    MutexGuiEnter();
    panGrill->setLabelTitulo( s );
    panGrill->Activate();
    #ifdef INVEL_W
    if( type == PERMITE_SCROLL )
    #endif
    {
        ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panGrill->getFocus() );
    }
    MutexGuiLeave();
}

void MainPanel::FinPanelLista()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    if( panGrill ) {
        panGrill->DesActivate();
        panGrill->vaciar();
    }
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::activarPanelCierre()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    GRABAR_LOG_SISTEMA( "ACTIVA PANEL CIERRE", LOG_PANTALLAS,5);
    currPanel = idPanelCierre;
    currPaneltouch = currPanel;
    incrementStack( currPanel );
    currPanelMsg = idPanelCierre;
    //panelList[idPanelCierre] = panelCierre;
    panelCierre.Activate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelCierre.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::desactivarPanelCierre()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    if( panelCierre.isShowing() ) {
        panelCierre.DesActivate();
        decrementStack();
        currPanel = -1;
    }
    if( panelCierreSub.isShowing() ) {
        panelCierreSub.DesActivate();
        decrementStack();
        currPanel = -1;
    }
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::activarPanelCierreSubmedios()
{
    currPanel = idPanelCierreSub;
    currPanelMsg = idPanelCierreSub;
    currPaneltouch = currPanel;
    incrementStack( currPanel );
    //panelList[idPanelCierreSub] = panelCierreSub;
    panelCierreSub.cerearMedios();

    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelCierreSub.Activate();
    //panelCierreSub.cerearSubmedios();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelCierreSub.getFocus() );
    //this.validate();
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::PanelPedirClaveEnPresentacion( short type, const string &s, const string &s1 )
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelPres.setPosInputBox( true );
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPres.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::FinPanelPedirClaveEnPresentacion()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelPres.setPosInputBox( false );
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPres.getFocus() );
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}


void MainPanel::TitFechaEnPresentacion( const string &s )
{
    // ya tiene mutex
    panelPres.setFecha( s );
}


void MainPanel::TitUsuarioEnPresentacion( const string &s )
{
    // ya tiene mutex
    panelPres.setUsuario( s );
}


void MainPanel::TitPasswordEnPresentacion( const string &s )
{
    // ya tiene mutex
    panelPres.setPassword( s );
}


void MainPanel::activarPanelCambioMedio()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    /**/glog ("ACTIVA PANEL CAMBIO DE MEDIOS",LOG_PANTALLAS,5);
    currPanel = idPanelCambioMedio;
    currPanelMsg = idPanelCambioMedio;
    currPaneltouch = currPanel;
    incrementStack( currPanel );
	BorrarMensaje();
    panelCambioMedio.Activate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelCambioMedio.getFocus() );    
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::desactivarPanelCambioMedio()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelCambioMedio.setVisible( false );
    panelCambioMedio.DesActivate();
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

void MainPanel::Refresh()
{
    MutexGuiEnter();
    if( panInput->isShowing() ) {
        panInput->Refresh();
    }
    else if( panelPres.isShowing() ) {
        panelPres.Refresh( false );
    }
    else if( panelPagos.isShowing() ) {
        panelPagos.Refresh( false );
    }
    else if( panelVentas.isShowing() ) {
        panelVentas.Refresh( false );
    }
    else if( panelCobro.isShowing() ) {
        panelCobro.Refresh( false );
    }
    else if( panelCierreSub.isShowing() ) {
        panelCierreSub.Refresh( false );
    }
    else if( panelFondoFijo.isShowing() ) {
        panelFondoFijo.Refresh( false );
    }
    else if( panelRetiros.isShowing() ) {
        panelRetiros.Refresh( false );
    }
    else if( panelCierre.isShowing() ) {
        panelCierre.Refresh( false );
    }
    else if( panelCambioMedio.isShowing() ) {
        panelCambioMedio.Refresh( false );
    }
    else if( panelMedioReint.isShowing() ) {
        panelMedioReint.Refresh( false );
    }
    MutexGuiLeave();
}

void MainPanel::activarPanelPagaTodo()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    /**/glog ("ACTIVA PANEL PAGA TODO",LOG_PANTALLAS,5);
    currPanel = idPanelPagaTodo;
    panelPagaTodo.Activate();
    //panelPagaTodo.Init();
    incrementStack( currPanel );
	//( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPagaTodo.getFocus() );
    MutexGuiLeave();
	panelPagaTodo.clearInputs();
    //#ifndef INVEL_L
    //#endif
}



void MainPanel::desactivarPanelPagaTodo()
{
    panelPagaTodo.clearInputs();
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    
	panelPagaTodo.DesActivate();
    //#ifndef INVEL_L
    MutexGuiLeave();
    MutexGuiEnter();
    //#endif
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
   // #endif
}

void MainPanel::activarPanelGarantia()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    /**/glog ("ACTIVA PANEL GARANTIA",LOG_PANTALLAS,5);
    currPanel = idPanelGarantia;
    panelGarantia.Activate();
    //panelPagaTodo.Init();
    incrementStack( currPanel );
	//( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panelPagaTodo.getFocus() );
    MutexGuiLeave();
	panelGarantia.clearInputs();
    //#ifndef INVEL_L
    //#endif
}



void MainPanel::desactivarPanelGarantia()
{
    //#ifndef INVEL_L
    MutexGuiEnter();
    //#endif
    panelGarantia.clearInputs();
	panelGarantia.DesActivate();
    //#ifndef INVEL_L
    MutexGuiLeave();
    MutexGuiEnter();
    //#endif
    decrementStack();
    currPanel = -1;
    //#ifndef INVEL_L
    MutexGuiLeave();
    //#endif
}

