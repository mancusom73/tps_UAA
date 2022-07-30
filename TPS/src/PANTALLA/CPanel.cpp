
#include "CPanel.h"
#include "AppMain.h"
#include "cr.h"
#include "MainPanel.h"
#include "odbc.h"
#include "deftables.h"
#include "CConfig.h"
#include "conio.h"
#if !defined(INVEL_L) && (defined(__WXGTK__) || defined(__WXMOTIF__))
#include "res/prueba.xpm"
#endif
extern MainPanel frame;
extern CConfig *Config;
extern ODBC *oODBCConect;          //Conexion con ODBC
#ifdef __cplusplus
#ifdef INVEL_L
extern "C"
{
	#include "str.h"
}
#endif
#endif
#ifdef COMPILAR_UML
IMPLEMENT_CLASS( CPanel, wxFrame )
BEGIN_EVENT_TABLE( CPanel, wxFrame )
EVT_LEFT_DOWN( CPanel::PosClick)
EVT_LEFT_DCLICK( CPanel::PosClick)
EVT_PAINT( CPanel::OnPaint )
END_EVENT_TABLE()
#endif

wxApp *CPanel::main_application = NULL;

CPanel::CPanel( CPanel *parent, const wxString &title, const wxPoint &pos, const wxSize &size,
                long style ) : wxFrame( parent, ID_CPANEL, title, pos, size, style )
{
    background = false;
    input = false;
    text = NULL;

    #ifndef INVEL_L
    SetIcon( wxICON( prueba ) );
    #endif
    panel = new wxPanel( this, -1, wxDefaultPosition, wxDefaultSize, style );
}


CPanel::CPanel( const wxString &title, const wxPoint &pos, const wxSize &size, long style ) 
: wxFrame( ( wxFrame * )NULL, ID_CPANEL, title, pos, size, style )
{
    background = false;
    input = false;
    text = NULL;

    #ifndef INVEL_L
    SetIcon( wxICON( prueba ) );
    #endif
    panel = new wxPanel( this, -1, wxDefaultPosition, wxDefaultSize, style );
}


CPanel::~CPanel()
{
    delete panel;
}


void CPanel::SetBackGround( const wxString &filename )
{
    background = true;
    /*#ifndef INVEL_L
      bmp = wxImage( filename, wxBITMAP_TYPE_GIF ).ConvertToBitmap();
    #else*/
    bmp = wxBitmap( filename, wxBITMAP_TYPE_GIF );
    //#endif
}


void CPanel::OnPaint( wxPaintEvent &event )
{
    wxPaintDC dc( this );
    if( background ) {
        dc.DrawBitmap( bmp, 0, 0, false );
    }
    for( unsigned int i = 0;i < objects.size();i++ ) {
        if( objects[i]->GetRefresh() ) {
            objects[i]->Draw( &dc );
        }
    }
}

void CPanel::OnPaint( )
{
    wxPaintDC dc( this );
    if( background ) {
        dc.DrawBitmap( bmp, 0, 0, false );
    }
    for( unsigned int i = 0;i < objects.size();i++ ) {
        if( objects[i]->GetRefresh() ) {
            objects[i]->Draw( &dc );
        }
    }
}


void CPanel::AddObject( CObject *o )
{
    o->SetID( objects.size() + 1 );
    objects.push_back( o );    
}


bool CPanel::isVisible()
{
    return false;
}


void CPanel::setVisible( bool v )
{
}


void CPanel::Activate()
{
    ( ( AppMain * )main_application )->ShowPanel( this, NULL );
}


void CPanel::mYield(){
	( ( AppMain * )main_application )->liberar();
}

void CPanel::DesActivate()
{
    ( ( AppMain * )main_application )->HidePanel( this );
}


bool CPanel::isShowing()
{
    //return false;
    return IsShown();
}


void CPanel::ClearObjects()
{
    objects.clear();    
}

void CPanel::DelAllObject( int nitems )
{
    int i, cant;
    
    cant = objects.size();

    for( i = 0; i < nitems; i++ ) {        
        objects.pop_back( );
    }
    //objects.clear();    
}

void CPanel::PosClick(wxMouseEvent& event){
    if( Config->get_touch_screen() ){
		SDWORD      cb;
        pos_click=event.GetPosition();
		char ayudero[6];
		int i;
		char *touchquery = (char *) malloc( 110*sizeof(char));
		strcpy (touchquery,"SELECT RETORNO FROM TOUCH WHERE ID_PANTALLA=");
		strcat (touchquery,_itoa(frame.currPaneltouch,ayudero,10));
		strcat (touchquery," AND X1<=");
		_itoa(pos_click.x,ayudero,10);
		strcat (touchquery,ayudero);
		strcat (touchquery," AND Y1<=");
		_itoa(pos_click.y,ayudero,10);
		strcat (touchquery,ayudero);
		strcat (touchquery," AND X2>=");
		_itoa(pos_click.x,ayudero,10);
		strcat (touchquery,ayudero);
		strcat (touchquery," AND Y2>=");
		_itoa(pos_click.y,ayudero,10);
		strcat (touchquery,ayudero);
		oODBCConect->getDb(ODBC_LOCAL)->ExecSql(touchquery);
		if(oODBCConect->getDb(ODBC_LOCAL)->GetNext()){
			oODBCConect->getDb(ODBC_LOCAL)->GetData(1, C_INT, (char *)&i, 4, &cb);
			if(cb>0){
				#ifdef INVEL_W
				Beep(1450,70);
				#endif
				ungetch(i);
			}
		}
		free(touchquery);
	}
}
