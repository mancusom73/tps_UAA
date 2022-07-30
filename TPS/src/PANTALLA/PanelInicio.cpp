#pragma warning (disable:4786)
#include "PanelInicio.h"
#include "actions.h"
#include "typeacti.h"
#include "CPanel.h"
#include "AppMain.h"
#include "path.h"

/*Posibles Colores del progress (m_gauge)*/
#define RED    255,000,51
#define BLUE   91,102,193

void PanelInicio::createLabels()
{
    Procefont.SetPointSize( 10 );
    Procefont.SetStyle( wxNORMAL );
    Procefont.SetWeight( wxNORMAL );
    create();
}

void PanelInicio::create()
{
    #ifdef INVEL_W       
    m_gauge = new wxGauge( this, -1, 304, wxPoint( 48, 177 ), wxSize( 322, 10 ),
                           wxGA_HORIZONTAL | wxGA_SMOOTH | wxBORDER );
    m_gauge->SetForegroundColour( wxColor( BLUE ) );
    Value = 0;
    #endif
}


void PanelInicio::init()
{
}


void PanelInicio::setProceso( const string &s )
{
    #ifdef INVEL_W  
    wxClientDC dc( this );
    wxBitmap bitmap;
    if( bitmap.LoadFile( image, wxBITMAP_TYPE_GIF ) ) {
        dc.SetFont( Procefont );
        dc.DrawBitmap( bitmap, 0, 0 );
        dc.DrawText( s.c_str(), 76, 195 );
    }
    m_gauge->SetValue( Value );
    Value += 10;
    #else
    wxClientDC dc( this );
    wxBitmap bitmap;
    if( bitmap.LoadFile( IMAGE_INICIO, wxBITMAP_TYPE_GIF ) ) {
        dc.DrawBitmap( bitmap, 0, 0 );
    }
    #endif
}
