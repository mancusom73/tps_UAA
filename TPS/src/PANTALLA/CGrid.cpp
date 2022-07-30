#include "CGrid.h"
#include "CFontMapper.h"
#include "memory.h"
#include "PanelVentas.h"
#include "AppMain.h"

#ifdef COMPILAR_UML
IMPLEMENT_CLASS( CGrid, wxGrid )
BEGIN_EVENT_TABLE( CGrid, wxGrid )
EVT_KEY_DOWN( CGrid::OnKey )

/*EVT_GRID_CELL_LEFT_CLICK( CGrid::Dummy )
EVT_GRID_CELL_RIGHT_CLICK( CGrid::Dummy )
EVT_GRID_CELL_LEFT_DCLICK( CGrid::Dummy )
EVT_GRID_CELL_RIGHT_DCLICK( CGrid::Dummy )
EVT_GRID_ROW_SIZE( CGrid::Dummy1 )
EVT_GRID_COL_SIZE( CGrid::Dummy1 )

EVT_MOUSE_EVENTS( CGrid::Dummy2 )

EVT_LEFT_DOWN( CGrid::Dummy2 )
EVT_LEFT_UP( CGrid::Dummy2 )
EVT_MIDDLE_DOWN( CGrid::Dummy2 )
EVT_MIDDLE_UP( CGrid::Dummy2 )
EVT_RIGHT_DOWN( CGrid::Dummy2 )
EVT_RIGHT_UP( CGrid::Dummy2 )
EVT_MOTION( CGrid::Dummy2 )
EVT_ENTER_WINDOW( CGrid::Dummy2 )
EVT_LEAVE_WINDOW( CGrid::Dummy2 )
EVT_LEFT_DCLICK( CGrid::Dummy2 )
EVT_MIDDLE_DCLICK( CGrid::Dummy2 )
EVT_RIGHT_DCLICK( CGrid::Dummy2 )
EVT_SET_FOCUS( CGrid::Dummy3 )
EVT_KILL_FOCUS( CGrid::Dummy3 )
//EVT_CHILD_FOCUS(CGrid::Dummy3)
EVT_MOUSEWHEEL( CGrid::Dummy2 )*/
END_EVENT_TABLE()
#endif
extern PanelVentas *panVentas;

CGrid::CGrid( CPanel *_parent, wxPoint p, wxSize s ) : wxGrid( _parent, -1, p, s,
                                                               wxTRANSPARENT_WINDOW | wxNO_BORDER )
{
    parent = _parent;
    row_size = 17;
    CreateGrid( 0, 0, wxGrid::wxGridSelectRows );
    DisableDragColSize();
    DisableDragGridSize();
    DisableDragRowSize();
    SetColLabelSize( 0 );
    SetRowLabelSize( 0 );
}



CGrid::CGrid( CPanel *_parent, wxPoint p, wxSize s, long style ) : wxGrid( _parent, -1, p, s,
                                                                           style )
{
    parent = _parent;
    row_size = 17;
    CreateGrid( 0, 0, wxGrid::wxGridSelectRows );
    DisableDragColSize();
    DisableDragGridSize();
    DisableDragRowSize();
    SetColLabelSize( 0 );
    SetRowLabelSize( 0 );
}


CGrid::~CGrid()
{
}


void CGrid::Draw()
{
}


void CGrid::Draw( wxDC *dc )
{
}


void CGrid::OnKey( wxKeyEvent &event )
{
	/*  Este cambio se realiza para que una vez dentro de la grilla, 
	por ejemplo por un click de mouse, 
	al presionar ESC se libere de esta seleccion */
	if(event.KeyCode()==WXK_ESCAPE){
		( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panVentas->getFocus() );
	}
}


void CGrid::Init( const vector<browse_field> &fieldsDefinition, int selected_field, int width )
{
    if( GetRows() > 0 ) {
        DeleteRows( 0, GetRows(), TRUE );
    }

    /*if( row_size < 15 )
        SetColLabelSize( 17 );
    else
        SetColLabelSize( row_size + 3 );
    */

    SetDefaultRowSize( row_size, true );

    if( GetNumberCols() > 0 ) {
        DeleteCols( 0, GetNumberCols() );
    }


    AppendCols( fieldsDefinition.size() );
    col_alignment.resize( fieldsDefinition.size() );

    for( unsigned int i = 0;i < fieldsDefinition.size();i++ ) {
        //SetColSize( i, fieldsDefinition[i].Width * 5 );
        SetColSize( i, fieldsDefinition[i].Width );
        //SetColLabelValue( i, fieldsDefinition[i].Name.c_str( ) );
        switch( fieldsDefinition[i].Alignment ) {
            case ALIGN_LEFT:
                col_alignment[i] = wxALIGN_LEFT;
                break;
            case ALIGN_CENTER:
                col_alignment[i] = wxALIGN_CENTRE;
                break;
            case ALIGN_RIGHT:
                col_alignment[i] = wxALIGN_RIGHT;
                break;
        }
    }
}


void CGrid::AddRow( const vector<string> &row )
{
    int a = col_alignment[0];
    int b = wxALIGN_LEFT;
    int c = wxALIGN_CENTRE;
    a = a + b + c;
    AppendRows( 1 );
    for( unsigned int i = 0;i < (unsigned int)row.size() && i < (unsigned int)GetNumberCols() ;i++ ) {
        SetCellValue( GetRows() - 1, i, row[i].c_str() );
        a = col_alignment[i];
        SetCellAlignment( GetRows() - 1, i, col_alignment[i], wxALIGN_CENTRE );
    }
    SetRowSize( GetRows() - 1, row_size );//setea el size para la fila actual
    Refresh( true );
    ForceRefresh();
}


void CGrid::InsertRow( int linea, const vector<string> &row )
{
    InsertRows( linea, 1 );
    for( unsigned int i = 0;i < (unsigned int)row.size() && i < (unsigned int)GetNumberCols();i++ ) {
        SetCellValue( linea, i, row[i].c_str() );
        //   SetCellAlignment(linea,i,col_alignment[i],wxCENTRE);
    }
}

void CGrid::UpdateRow( int linea, const vector<string> &row )
{
    //UpdateRows( linea, 1 );
	if( GetRows() > 0 && GetRows() > linea && linea >= 0 ) 
		for( unsigned int i = 0;i < (unsigned int)row.size() && i < (unsigned int)GetNumberCols();i++ ) {
			SetCellValue( linea, i, row[i].c_str() );
			//   SetCellAlignment(linea,i,col_alignment[i],wxCENTRE);
		}
}


void CGrid::SetRowsSize( int size )
{
    row_size = size;
}


void CGrid::_SetRowSize( const string &f )
{
    char size[20];
    int l = 0;

    for( int i = 0;i < (int)f.length();i++ ) {
        if( f[i] != ' ' && f[i] != '_' ) {
            size[l++] = f[i];
        }
    }
    size[l] = 0x00;
    row_size = atoi( size );
}


int CGrid::getRowSize( void )
{
    return row_size;
}


int CGrid::getHeight( void )
{
    GetSize( &width, &height );
    return height;
}


int CGrid::getNumFilas( void )
{
    GetSize( &width, &height );

    return ( (height / row_size)-1 );
}


void CGrid::RemoveRow( int linea )
{
    if( GetRows() > 0 ) {
        DeleteRows( linea, 1 );
    }
}


void CGrid::iniFont( const string &f )
{
    char f_name[100];
    char f_size[100];
    char f_style[100];
    int l_name = 0;
    int l_size = 0;
    int l_style = 0;
    int j = 0;

    memset( f_name, 0, sizeof( f_name ) );
    memset( f_size, 0, sizeof( f_size ) );
    memset( f_style, 0, sizeof( f_style ) );

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' ) {
            f_name[l_name++] = f[i];
        }
        if( j == 1 && f[i] != '-' ) {
            f_style[l_style++] = f[i];
        }
        if( j == 2 && f[i] != '-' ) {
            f_size[l_size++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_name[l_name] = 0x00;
    f_size[l_size] = 0x00;
    f_style[l_style] = 0x00;

    #ifndef INVEL_L
    font.SetFaceName( fontEnumerator.GetFacename( f_name ) );
    #endif
    font.SetPointSize( atoi( f_size ) );

    //;# Fuente para el texto. name,style,size.
    //;# The style can be
    //;# NORMAL+NORMAL=0,NORMAL+BOLD=1,ITALIC+NORMAL=2,ITALIC+BOLD=3,NORMAL+LIGHT=4,ITALIC+LIGHT=5,
    //;# SLANT+NORMAL=6,SLANT+LIGHT=7,SLANT+BOLD=8
    //;# (The style  can be wxNORMAL=90, wxSLANT=94 and wxITALIC=93)
    //;# (The weight can be wxNORMAL=90, wxLIGHT=91 and wxBOLD=92.)
    switch( atoi( f_style ) ) {
        case _NORMAL_NORMAL:
            font.SetStyle( wxNORMAL );
            font.SetWeight( wxNORMAL );
            break;
        case _NORMAL_BOLD:
            font.SetStyle( wxNORMAL );
            font.SetWeight( wxBOLD );
            break;
        case _ITALIC_NORMAL:
            font.SetStyle( wxITALIC );
            font.SetWeight( wxNORMAL );
            break;
        case _ITALIC_BOLD:
            font.SetStyle( wxITALIC );
            font.SetWeight( wxBOLD );
            break;
        case _NORMAL_LIGHT:
            font.SetStyle( wxNORMAL );
            font.SetWeight( wxLIGHT );
            break;
        case _ITALIC_LIGHT:
            font.SetStyle( wxITALIC );
            font.SetWeight( wxLIGHT );
            break;
        case _SLANT_NORMAL:
            font.SetStyle( wxSLANT );
            font.SetWeight( wxNORMAL );
            break;
        case _SLANT_LIGHT:
            font.SetStyle( wxSLANT );
            font.SetWeight( wxLIGHT );
            break;
        case _SLANT_BOLD:
            font.SetStyle( wxSLANT );
            font.SetWeight( wxBOLD );
            break;
    }
    SetDefaultCellFont( font );
}


void CGrid::iniColour( const string &f, int type )
{
    char f_red[100];
    char f_blue[100];
    char f_green[100];
    int l_red = 0;
    int l_blue = 0;
    int l_green = 0;
    int j = 0;

    memset( f_red, 0, sizeof( f_red ) );
    memset( f_blue, 0, sizeof( f_blue ) );
    memset( f_green, 0, sizeof( f_green ) );

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' ) {
            f_red[l_red++] = f[i];
        }
        if( j == 1 && f[i] != '-' ) {
            f_blue[l_blue++] = f[i];
        }
        if( j == 2 && f[i] != '-' ) {
            f_green[l_green++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_red[l_red] = 0x00;
    f_blue[l_blue] = 0x00;
    f_green[l_green] = 0x00;
    wxColour c( atoi( f_red ), atoi( f_blue ), atoi( f_green ) );
    //   SetCellBackgroundColour(c);
    switch( type ) {
        case 0:
            //font
            fontColour = c;
            SetCellTextColour( c );
            break;
        case 1:
            //backgraund
            backgroundColour = c;
            SetCellBackgroundColour( c );
            break;
        case 2:
            //selBackgroundColour;
            selBackgroundColour = c;
            //SetSelectionForeground(c);
            SetSelectionBackground( c );
            break;
    }
    Refresh( true );
}


void CGrid::setWidthColBrowse( const string &f, int ind1 )
{
    int l = 0;
    int j = 0, jant = 0;
    char f_aux[10];
    int i;

    memset( f_aux, 0, 10 );
    for( i = 0;i < 30;i++ ) {
        if( ind1 > 30 || i > 30 ) {
            sglog( "Error en tamao de array de size's" );
        }
        vectWidth[ind1][i] = 0;
    }
    for( i = 0;i < (int)f.length();i++ ) {
        if( j == jant && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_aux[l++] = f[i];
        }
        if( f[i] == '-' || f[i] == ')' ) {
            setWidthColGrid( ind1, j, atoi( f_aux ) );
            j++;
            jant = j;
            memset( f_aux, 0, sizeof( f_aux ) );
            l = 0;
        }
    }
}


void CGrid::setWidthColGrid( int ind1, int ind2, int w )
{
    vectWidth[ind1][ind2] = w;
};

int CGrid::getWidthColGrid( int ind1, int ind2 )
{
    return( vectWidth[ind1][ind2] );
};

void CGrid::Dummy( wxGridEvent &event ) //no utilizamos mouse
{
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panVentas->getFocus() );
}

void CGrid::Dummy1( wxGridSizeEvent &event ) //no utilizamos mouse
{
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( panVentas->getFocus() );
	
}

void CGrid::Dummy2( wxMouseEvent &event )
{
}

void CGrid::Dummy3( wxFocusEvent &event )
{
}
