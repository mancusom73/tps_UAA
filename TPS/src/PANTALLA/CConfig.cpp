#include "CConfig.h"




int CConfig :: get_modo_negocio()
{
    return ModoNegocio;
}


void CConfig :: set_modo_negocio( int modo )
{
    ModoNegocio = modo;
}
    /* Funcion agregada para devolver el tipo de teclado */
int CConfig :: get_touch_screen()
{
    return TouchScreen;
}
    /* Funcion que setea si el teclado es touch screen*/
void CConfig :: set_touch_screen( int touch )
{
    TouchScreen = touch;
}


char *CConfig :: get_IpJavaServer()
{
    return (char *)IpJavaServer.c_str();
}

char *CConfig :: get_videoVentas()
{
    return (char *)dirVideoVentas.c_str();
}

char *CConfig::getDirSkin()
{
	return (char *)dirSkin.c_str();
}




void CConfig :: set_IpJavaServer( char *ip )
{
    IpJavaServer.Printf("%s",ip);
    //sprintf(IpJavaServer,"%s",ip);
}
void CConfig::setTiempoPopUp(int tmpo)
{
    Tiempo_pop_up = tmpo;
}
int  CConfig::getTiempoPopUp()
{
    return Tiempo_pop_up;
}

void CConfig::setIpFtp(wxString pIpFtp)
{
    IpFtp = pIpFtp;
}

void CConfig::setVideoVentas(wxString path)
{
    dirVideoVentas = path;
}

void CConfig::setDirSkin(wxString path)
{
    dirSkin = path;
}

wxString  CConfig::getIpFtp()
{
    return IpFtp;
}
