#include "PanelScreenSaver.h"
#include "AppMain.h"

extern CDesktop *properties;
static wxMutex s_mutexHoraStand;

PanelScreenSaver::PanelScreenSaver( const wxString &title, const wxPoint &pos, const wxSize &size,
                            long style ) : CPanel( title, pos, size, style )
{
    screenXml = new CScreencfg( "screensaver", "default", "default" );
}


void PanelScreenSaver::createLabels()
{
    SetBackGround( screenXml->getBackgroundXML().c_str() );

    create();
}


void PanelScreenSaver::create()
{
    input = new CInput( this, 300, 200, 90, 17, false );
    input->SetNotVisible();
}
