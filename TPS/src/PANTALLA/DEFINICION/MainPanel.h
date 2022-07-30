#pragma pack (push,8) 
#ifndef __MAIN_PANEL_H
#define __MAIN_PANEL_H
#include <wx/thread.h>
#include "CObject.h"
#include "CInput.h"

class MainPanel
{
  private:
    int currPanel;
	int currPanel_ant;
  public:
    int currPaneltouch;
	int currPaneltouch_ant;
    int currPanelMsg;
	int currPanelMsg_ant;
    void decrementStack()
    {
    };
    void incrementStack( int c )
    {
    };
    void NuevoPanelPresentacion( const string &ver, const string &fec, const string &branch );
    void activarPanelVentas();
    void desactivarPanelPrincipal();
    void desactivarPanelVentas();
    void activarPanelPagos();
    void activarPanelFondoFijo();
    void activarPanelRetiros();
    void desactivarPanelPagos();
    void desactivarPanelFondoFijo();
    void desactivarPanelRetiros();
    void activarPanelPresentacion();
    void activarPanelCobros();
    void activarPanelCobrosEsp();
    void activarPanelDeudaSocio();
    void desactivarPanelDeudaSocio();
	void activarPanelVuelto();
	void desactivarPanelVuelto();
	void activarPanelDenominaciones();
	void desactivarPanelDenominaciones();
    void activarPanelCheques();
    void desactivarPanelCheques();
    void activarPanelImportacion();
    void desactivarPanelImportacion();
    void desactivarPanelCobros();
    void desactivarPanelCobrosEsp();
    void activarPanelPagosCaja();
    void desactivarPanelPagosCaja();
    void activarPanelPagosEsp();
    void desactivarPanelPagosEsp();
    void activarPanelStandby();
    void desactivarPanelStandby();
    void MostrarMensaje( const string &s );
    void BorrarMensaje();
    void PanelMoveInput( short type, const string &s, const string &s1 );
    void PanelPedir( short type, const string &s, const string &s1, const string &mascara, const string &s2 );
    void EsconderPanelPedir( const string &s );
    void FinPanelPedir();
    void activarPanelCierre();
    void desactivarPanelCierre();
    void activarPanelCierreSubmedios();
    void PanelPedirClaveEnPresentacion( short type, const string &s, const string &s1 );
    void FinPanelPedirClaveEnPresentacion();
    void TitFechaEnPresentacion( const string &s );
    void TitUsuarioEnPresentacion( const string &s );
    void TitPasswordEnPresentacion( const string &s );
    void activarPanelCambioMedio();
    void desactivarPanelCambioMedio();
    void PanelLista( const string &s, int type );
    void FinPanelLista();
    void activarPanelMedioReint();
    void desactivarPanelMedioReint();
    void Refresh();
	void activarPanelPagaTodo();
	void desactivarPanelPagaTodo();
	void activarPanelGarantia();
	void desactivarPanelGarantia();
    void activarPanelConsultaPromos();
    void desactivarPanelConsultaPromos();
    void desactivarTodosLosPaneles();
	void activarPanelScreenSaver();
	void desactivarPanelScreenSaver();
    int getch();
	void SetCurrPanel( int recuperar );
    bool verifyLogo( bool mostrar, bool cerrar, const string &pantalla, int x, int y );
    CInput *getFocus();
};
#endif

#pragma pack (pop) 
