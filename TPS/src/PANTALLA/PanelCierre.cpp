#include "PanelCierre.h"

extern CDesktop *properties;

PanelCierre::PanelCierre( const wxString &title, const wxPoint &pos, const wxSize &size,
                          long style ) : CPanel( title, pos, size, style )
{
    label_mensaje = new CLabel();
    screenXml = new CScreencfg( "cierre_z", "default", "default" );
}

void PanelCierre::createLabels()
{
    label_mensaje->setParent( this );
    this->AddObject( label_mensaje );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createLabelXML();

    create();
}

CInput * PanelCierre::getFocus()
{
    return dummy;
}

void PanelCierre::createLabelXML()
{
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
}



void PanelCierre::create()
{
    dummy = new CInput( this, 700, 0, 10, 10, true );
}



