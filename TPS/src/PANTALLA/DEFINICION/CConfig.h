#pragma pack (push,8) 
#ifndef __C_CONFIG_H
#define __C_CONFIG_H

/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 * Objeto CConfig: contiene la configuracion de la rnv o configuraciones
 * necesarias en las pantallas
 * Este objeto es cargado en IPantalla por la funcion SetConfig la cual es llamada
 * desde el core 
 *
 * ModoNegocio: informa el modo en el que se ejecuta la pos, el cual puede ser 
 *              Retail / Servicios (se configura en el conftpv)
 * ******************************************************************************
 */

#include <wx/string.h>
#include "actions.h"

class CConfig
{
    
  public:
    int get_modo_negocio();
    void set_modo_negocio( int modo ); 
        /* Geter y seter para el teclado */
    int  get_touch_screen();
    void set_touch_screen( int touch );
    char  *get_IpJavaServer();
    void set_IpJavaServer( char *ip );
    void setTiempoPopUp(int tmpo);
    int  getTiempoPopUp();
    void setIpFtp(wxString pIpFtp);
    wxString  getIpFtp();
	char  *get_videoVentas();
	void setVideoVentas( wxString path );
	char *getDirSkin();
	void setDirSkin(wxString path);


  private:
    int ModoNegocio;
    int TouchScreen;
    wxString IpJavaServer;
    wxString IpFtp;
	wxString dirVideoVentas;
	wxString dirSkin;
    int Tiempo_pop_up;
    //char IpJavaServer[20];
};
#endif

#pragma pack (pop) 
