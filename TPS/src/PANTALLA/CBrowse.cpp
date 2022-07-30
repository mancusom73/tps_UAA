#pragma pack(8)

#include "CBrowse.h"
#include "CPanel.h"
#include "AppMain.h"
#include "PanelCobro.h"
#include "PanelPagosCaja.h"
#include "PanelPagosEsp.h"

extern PanelCobro *panCobro;
#define pCobro (*panCobro)

extern PanelPagosCaja *panPagosCaja;
#define pPagosCaja (*panPagosCaja)

extern PanelPagosEsp *panPagosEsp;
#define pPagosEsp (*panPagosEsp)

CBrowse::CBrowse( CPanel *_parent, wxPoint p, wxSize s ) : CGrid( _parent, p, s,
                                                                  wxNO_BORDER/*wxDOUBLE_BORDER*/ )
{
}


CBrowse::~CBrowse()
{
}


void CBrowse::init( int id, const vector<browse_field> &fieldsDefinition, int selected_field,
                    int width, int pAlto )
{
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    Init( fieldsDefinition, selected_field, width );

    SetDefaultCellFont( font );
    SetCellBackgroundColour( backgroundColour );
    SetCellTextColour( fontColour );
    SetSelectionBackground( selBackgroundColour );
	alto = pAlto;

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}


void CBrowse::selRow( int id, int linea )
{
    SelectRow( linea );
}


void CBrowse::scroll( int id, int lineas )
{
	wxYield();
	ForceRefresh();
    if( lineas < 0 ) {
        DeleteRows( 0, lineas * -1, TRUE );
    }
    else {
		if(this->GetNumberRows() == alto)
        DeleteRows( alto - 1, lineas, TRUE );
    }
	wxYield();
	ForceRefresh();
    //current_row += lineas;
}


void CBrowse::setRow( int id, const vector<string> &row )
{
    if( AppendRows( 1 ) ) {
        for( unsigned int i = 0;i < row.size();i++ ) {
            SetCellValue( GetRows() - 1, i, row[i].c_str() );
        }
    }
}

void CBrowse::setRow( int id, const vector<string> &row, int agrego )
{
	if( agrego == 1) {
		if( AppendRows( 0 ) ) {
			for( unsigned int i = 0;i < row.size();i++ ) {
            SetCellValue( GetRows() - 1, i, row[i].c_str() );
			}
		}
	}else {
			for( unsigned int i = 0;i < row.size();i++ ) {
            SetCellValue( GetRows() - 1, i, row[i].c_str() );
			}
	}

}


void CBrowse :: deleteRows( int rows )
{
    if( GetRows() > 0 ) {
        DeleteRows( 0, ( rows > 0 ) ? rows : GetRows(), TRUE );
    }
}


void CBrowse::insertRow( int id, int linea, const vector<string> &row )
{
    InsertRows( linea, 1 );
    for( unsigned int i = 0;i < row.size();i++ ) {
        SetCellValue( linea, i, row[i].c_str() );
    }
	wxYield();
	ForceRefresh();

}


//extern CPanel *panel_browse;
//extern CInput *input_browse;

void CBrowse::setEnable( int id, bool state )
{
    if( state ) {
        _panel_browse->Activate();
              //((AppMain *)CPanel::GetLauncher())->ChangeFocus(_input_browse);
    }
    else {
        //((AppMain *)CPanel::GetLauncher())->ChangeFocus(_input_browse);
    }
}


void CBrowse::close( int id )
{
    if( !( pCobro.isShowing() ) && !( pPagosCaja.isShowing() ) && !( pPagosEsp.isShowing() ) ) {
        _panel_browse->DesActivate();
    }
}


void CBrowse::deleteInput()
{
    if( _input_browse != NULL ) {
        _input_browse->Clear();
    }
}

void CBrowse::setTargetWindow( wxWindow *target )
{
    SetTargetWindow( target );
}
