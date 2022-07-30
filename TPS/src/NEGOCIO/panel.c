#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <bitmap.h> 
//#include <browser.h> 
#include <btrv.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <log.h> 
#include <mensaje.h> 
#include <panel.h> 
#include <timer.h> 
//#include <vgagraf.h> 
#include <_cr0.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
#ifndef INVEL_L
#endif
#undef MK_FP
/* Macro que inactiva el uso de un buffer en memoria para los bitmaps */
//int  BLACK = 0;
//#define   _NO_BUFFERING_
/* Variables Globales para el acceso unificado a los Archivos Btrieve */
#define _SKIP_HEADER_
_BUTTON _button;
_PANEL _panel;
#undef  _SKIP_HEADER_
int PanelActivo = 0;
PANEL *pPanel = NULL;
int Orden = 1;
char aux[80];
#ifdef COMPILAR_DYNAKEY
int GetWidth( RECT *pRect )
{
    return pRect->right - pRect->left;
};
int GetHeight( RECT *pRect )
{
    return pRect->bottom - pRect->top;
};
BUTTON * CreateButton( int Orden, STRING Title, STRING FileName, BOOLEAN Activado, int Lugar,
                       int Tecla, RECT Area )
{
    BUTTON *pButton = NULL;
    pButton = ( BUTTON * )malloc( sizeof( BUTTON ) );
    if( !pButton ) {
        MENSAJE( "error en malloc" );
        exit( 1 );
    }
    pButton->Orden = Orden;
    strcpy( pButton->Title, Title );
    pButton->Activado = Activado;
    pButton->Lugar = Lugar;
    pButton->Tecla = Tecla;
    pButton->Area = Area;
    #ifndef _NO_BUFFERING_
    LoadBitmap( FileName, &pButton->Imagen );
    #else
    strcpy( pButton->Imagen.BitmapFile, FileName );
    #endif
    return pButton;
};
void DestroyButton( BUTTON *pButton )
{
    if( pButton->Imagen.Pixel)
    free( pButton->Imagen.Pixel );

    if( pButton)
    free( pButton );
};
PANEL * CreatePanel( int Orden, STRING Title, RECT Area, int PrimerBoton, int Espaciado )
{
    PANEL *pPanel = NULL;
    pPanel = ( PANEL * )malloc( sizeof( struct tagPANEL ) );
    pPanel->Orden = Orden;
    strcpy( pPanel->Title, Title );
    pPanel->Area = Area;
    pPanel->Espaciado = Espaciado;
    pPanel->CantBotones = 0;
    pPanel->Altura = PrimerBoton;
    pPanel->BotonActual = NONE;
    // pPanel->Botones=NULL;
    return pPanel;
};
void AddButton( PANEL *pPanel, BUTTON *pButton )
{
    if( pPanel->CantBotones < 13 ) {
        pPanel->Botones[pPanel->CantBotones++] = pButton;
    };
    /* Cdigo utilizado para asignar memoria en tiempo de ejecucin */
    /*
     * BUTTON* pAux = *pPanel->Botones;
     * if(pPanel->CantBotones==0) {
     * pAux = (BUTTON*)malloc(sizeof(BUTTON));
     * pPanel->Botones[pPanel->CantBotones++]=pButton;
     * } else {
     * pAux = (BUTTON*)realloc(pAux, pPanel->CantBotones+1 * sizeof(BUTTON));
     * pPanel->Botones[pPanel->CantBotones++]=pButton;
     * };
     */
};
void ShowButton( BUTTON *pButton, int Altura )
{
    pButton->Area.top += Altura;
    pButton->Area.bottom += Altura;
    Placa3DUP( pButton->Area.left, pButton->Area.top, pButton->Area.right, pButton->Area.bottom,
               LIGHTGRAY );
    #ifndef _NO_BUFFERING_
    ShowBitmap( pButton->Area.left + 5, pButton->Area.top + 5, &pButton->Imagen,
                pButton->Activado );
    #else
    ShowBitmapEx( pButton->Area.left + 5, pButton->Area.top + 5, pButton->Imagen.BitmapFile,
                  pButton->Activado );
    #endif
};
void ShowButtonEx( BUTTON *pButton, int Altura, BOOLEAN Pressed )
{
    pButton->Area.top += Altura;
    pButton->Area.bottom += Altura;
    switch( Pressed ) {
        case TRUE:
            Placa3DDN( pButton->Area.left, pButton->Area.top, pButton->Area.right,
                       pButton->Area.bottom, LIGHTGRAY );
            #ifndef _NO_BUFFERING_
            ShowBitmap( pButton->Area.left + 6, pButton->Area.top + 6, &pButton->Imagen,
                        pButton->Activado );
            #else
            ShowBitmapEx( pButton->Area.left + 6, pButton->Area.top + 6, pButton->Imagen.BitmapFile,
                          pButton->Activado );
            #endif
            break;
        case FALSE:
            Placa3DUP( pButton->Area.left, pButton->Area.top, pButton->Area.right,
                       pButton->Area.bottom, LIGHTGRAY );
            #ifndef _NO_BUFFERING_
            ShowBitmap( pButton->Area.left + 5, pButton->Area.top + 5, &pButton->Imagen,
                        pButton->Activado );
            #else
            ShowBitmapEx( pButton->Area.left + 5, pButton->Area.top + 5, pButton->Imagen.BitmapFile,
                          pButton->Activado );
            #endif
            break;
    };
};
#endif
void ShowPanel( PANEL *pPanel )
{
    #ifdef COMPILAR_DYNAKEY
    int i;
    if( ancho_panel ) {
        Placa3DUP( pPanel->Area.left, pPanel->Area.top, pPanel->Area.right, pPanel->Area.bottom,
                   LIGHTGRAY );
        for( i = 0;i < pPanel->CantBotones;i++ ) {
            ShowButton( pPanel->Botones[i], pPanel->Altura + ( i * pPanel->Espaciado ) );
        };
    }
    #endif
};
void RefreshPanel( PANEL *pPanel )
{
    #ifdef COMPILAR_DYNAKEY
    int i;
    if( ancho_panel ) {
        Placa3DUP( pPanel->Area.left, pPanel->Area.top, pPanel->Area.right, pPanel->Area.bottom,
                   LIGHTGRAY );
        for( i = 0;i < pPanel->CantBotones;i++ ) {
            ShowButton( pPanel->Botones[i], 0 );
        };
    }
    #endif
};
void HidePanel( PANEL *pPanel )
{
    #ifdef COMPILAR_DYNAKEY
    if( ancho_panel ) {
        Rectangle( pPanel->Area.left, pPanel->Area.top, pPanel->Area.right + 1,
                   pPanel->Area.bottom + 1, BLACK );
    }
    #endif
};
void ClearPanel( PANEL *pPanel )
{
    #ifdef COMPILAR_DYNAKEY
    HidePanel( pPanel );
    DestroyPanel( pPanel );
    PanelActivo = 0;
    #endif
};
#ifdef COMPILAR_DYNAKEY
void SelectButton( BUTTON *pButton, BOOLEAN Select )
{
    pButton->Seleccionado = Select;
    switch( Select ) {
        case BUTTON_SELECTED:
            MarcoGris( pButton->Area.left, pButton->Area.top, pButton->Area.right,
                       pButton->Area.bottom );
            break;
        case BUTTON_UNSELECTED:
            ShowButton( pButton, 0 );
            break;
    };
};
void SetBotonActual( PANEL *pPanel, int Boton )
{
    if( Boton != NONE ) {
        SelectButton( pPanel->Botones[Boton], TRUE );
    };
    if( pPanel->BotonActual != NONE ) {
        SelectButton( pPanel->Botones[pPanel->BotonActual], FALSE );
    };
    pPanel->BotonActual = Boton;
};
void ButtonDown( BUTTON *pButton )
{
    Marco3DDN( pButton->Area.left, pButton->Area.top, pButton->Area.right, pButton->Area.bottom );
};
void ButtonUp( BUTTON *pButton )
{
    Marco3DUP( pButton->Area.left, pButton->Area.top, pButton->Area.right, pButton->Area.bottom );
};
void ButtonDownEx( BUTTON *pButton, int Style )
{
    switch( Style ) {
        case 0:
            Marco3DDN( pButton->Area.left, pButton->Area.top, pButton->Area.right,
                       pButton->Area.bottom );
            break;
        case 1:
            ShowButtonEx( pButton, 0, TRUE );
            break;
    };
};
void ButtonUpEx( BUTTON *pButton, int Style )
{
    switch( Style ) {
        case 0:
            Marco3DUP( pButton->Area.left, pButton->Area.top, pButton->Area.right,
                       pButton->Area.bottom );
            break;
        case 1:
            ShowButtonEx( pButton, 0, FALSE );
            break;
    };
};
void PressButton( PANEL *pPanel, int Boton )
{
    pPanel->BotonActual = Boton;
    ButtonDown( pPanel->Botones[Boton] );
};
void ReleaseButton( PANEL *pPanel, int Boton )
{
    pPanel->BotonActual = NONE;
    ButtonUp( pPanel->Botones[Boton] );
};
int Click( PANEL *pPanel, int Boton )
{
    int Tecla = 0;
    if( pPanel->Botones[Boton]->Activado ) {
        PressButton( pPanel, Boton );
        DELAY( 256 );
        ReleaseButton( pPanel, Boton );
        Tecla = pPanel->Botones[Boton]->Tecla;
    };
    return Tecla;
};
int ClickEx( PANEL *pPanel, int Boton, int Style )
{
    int Tecla = NONE;
    if( pPanel->Botones[Boton]->Activado ) {
        ButtonDownEx( pPanel->Botones[Boton], Style );
        DELAY( 256 );
        ButtonUpEx( pPanel->Botones[Boton], Style );
        Tecla = pPanel->Botones[Boton]->Tecla;
    };
    return Tecla;
};
void ActivateButton( PANEL *pPanel, int Boton )
{
    pPanel->Botones[Boton]->Activado = TRUE;
    ShowButton( pPanel->Botones[Boton], 0 );
};
void DesactivateButton( PANEL *pPanel, int Boton )
{
    pPanel->Botones[Boton]->Activado = FALSE;
    ShowButton( pPanel->Botones[Boton], 0 );
};
void AsignarTecla( PANEL *pPanel, int Boton, int Tecla )
{
    pPanel->TeclaBoton[Boton] = Tecla;
};
void AsignarTeclas( PANEL *pPanel, int Teclas[12] )
{
    int i;
    for( i = 0;i < 12;i++ ) {
        pPanel->TeclaBoton[i] = Teclas[i];
    };
};
int BuscarBoton( PANEL *pPanel, int Tecla )
{
    int i;
    int Response;
    for( i = 0;( ( i < pPanel->CantBotones ) && ( Tecla != pPanel->TeclaBoton[i] ) );i++ ) {
    };
    if( Tecla != pPanel->TeclaBoton[i] || i >= pPanel->CantBotones ) {
        Response = NONE;
    }
    else {
        Response = i;
    };
    return Response;
};
BOOLEAN SeekPanel( int Orden )
{
    BOOLEAN Response = FALSE;
    SELECT_TABLE( T_PANEL_VGA );
    SET_WHERE( "orden = '%d'",Orden );
	RUN_QUERY( NO );
    if( FOUND2() ) {
        Response = TRUE;
    };
    return Response;
};
BOOLEAN SeekButton( int Orden )
{
    BOOLEAN Response = FALSE;
    SELECT_TABLE( T_BOTON_VGA, TT_ORIG );
	SET_WHERE( "orden = '%d'", Orden );
	RUN_QUERY( NO );
    if( FOUND2() ) {
        Response = TRUE;
    };
    return Response;
};
BOOLEAN PrepareButton( BUTTON **pButton, int Orden )
{
    BOOLEAN Response = FALSE;
    RECT Area;
    if( ( Response = SeekButton( Orden ) ) == TRUE ) {
        Area.left = _button.Left;
        Area.top = _button.Top;
        Area.right = _button.Right;
        Area.bottom = _button.Bottom;
        *pButton = CreateButton( Orden, _button.Title, _button.Imagen, TRUE, _button.Lugar,
                                 _button.Tecla, Area );
        Response = TRUE;
    };
    return Response;
};
void RegistroPanel( PANEL *pPanel )
{
    _panel.Orden = pPanel->Orden;
    strcpy( _panel.Title, pPanel->Title );
    _panel.Left = pPanel->Area.left;
    _panel.Top = pPanel->Area.top;
    _panel.Right = pPanel->Area.right;
    _panel.Bottom = pPanel->Area.bottom;
    _panel.BotonActual = pPanel->BotonActual;
    _panel.Altura = pPanel->Altura;
    _panel.Espaciado = pPanel->Espaciado;
    _panel.CantBotones = pPanel->CantBotones;
    _panel.TeclaBoton0 = pPanel->TeclaBoton[0];
    _panel.TeclaBoton1 = pPanel->TeclaBoton[1];
    _panel.TeclaBoton2 = pPanel->TeclaBoton[2];
    _panel.TeclaBoton3 = pPanel->TeclaBoton[3];
    _panel.TeclaBoton4 = pPanel->TeclaBoton[4];
    _panel.TeclaBoton5 = pPanel->TeclaBoton[5];
    _panel.TeclaBoton6 = pPanel->TeclaBoton[6];
    _panel.TeclaBoton7 = pPanel->TeclaBoton[7];
    _panel.TeclaBoton8 = pPanel->TeclaBoton[8];
    _panel.TeclaBoton9 = pPanel->TeclaBoton[9];
    _panel.TeclaBoton10 = pPanel->TeclaBoton[10];
    _panel.TeclaBoton11 = pPanel->TeclaBoton[11];
};
void RegistroButton( BUTTON *pButton )
{
    _button.Orden = pButton->Orden;
    _button.Left = pButton->Area.left;
    _button.Top = pButton->Area.top;
    _button.Right = pButton->Area.right;
    _button.Bottom = pButton->Area.bottom;
    strcpy( _button.Title, pButton->Title );
    strcpy( _button.Imagen, pButton->Imagen.BitmapFile );
    _button.Lugar = pButton->Lugar;
    _button.Tecla = pButton->Tecla;
};
#endif
void ActivatePanel( PANEL *pPanel )
{
    #ifdef COMPILAR_DYNAKEY
    int i;
    if( ancho_panel ) {
        for( i = 0;i < pPanel->CantBotones;i++ ) {
            pPanel->Botones[i]->Activado = TRUE;
            ShowButton( pPanel->Botones[i], 0 );
        };
    }
    #endif
};
void DesactivatePanel( PANEL *pPanel )
{
    #ifdef COMPILAR_DYNAKEY
    int i;
    if( ancho_panel ) {
        for( i = 0;i < pPanel->CantBotones;i++ ) {
            pPanel->Botones[i]->Activado = FALSE;
            ShowButton( pPanel->Botones[i], 0 );
        };
    }
    #endif
};
BOOLEAN PreparePanel( PANEL **pPanel, int Orden )
{
    BOOLEAN Response = TRUE;
    #ifdef COMPILAR_DYNAKEY
    int i;
    int bOrden;
    BUTTON *pButton = NULL;
    RECT Area;
    if( ancho_panel ) {
        if( ( Response = SeekPanel( Orden ) ) == TRUE ) {
            Area.left = _panel.Left;
            Area.top = _panel.Top;
            Area.right = _panel.Right;
            Area.bottom = _panel.Bottom;
            *pPanel = CreatePanel( Orden, _panel.Title, Area, _panel.Altura, _panel.Espaciado );
            for( i = 0;i < _panel.CantBotones;i++ ) {
                bOrden = ( Orden * 100 ) + i + 1;
                Response = PrepareButton( &pButton, bOrden );
                if( !Response ) {
                    return FALSE;
                };
                AddButton( *pPanel, pButton );
            };
            AsignarTecla( *pPanel, 0, _panel.TeclaBoton0 );
            AsignarTecla( *pPanel, 1, _panel.TeclaBoton1 );
            AsignarTecla( *pPanel, 2, _panel.TeclaBoton2 );
            AsignarTecla( *pPanel, 3, _panel.TeclaBoton3 );
            AsignarTecla( *pPanel, 4, _panel.TeclaBoton4 );
            AsignarTecla( *pPanel, 5, _panel.TeclaBoton5 );
            AsignarTecla( *pPanel, 6, _panel.TeclaBoton6 );
            AsignarTecla( *pPanel, 7, _panel.TeclaBoton7 );
            AsignarTecla( *pPanel, 8, _panel.TeclaBoton8 );
            AsignarTecla( *pPanel, 9, _panel.TeclaBoton9 );
            AsignarTecla( *pPanel, 10, _panel.TeclaBoton10 );
            AsignarTecla( *pPanel, 11, _panel.TeclaBoton11 );
            PanelActivo = 1;
        }
        else {
            PanelActivo = 0;
        }
    }
    else {
        PanelActivo = 0;
    }
    #endif
    return Response;
};
void DestroyPanel( PANEL *pPanel )
{
    #ifdef COMPILAR_DYNAKEY
    int i;
    if( ancho_panel ) {
        for( i = 0;i < pPanel->CantBotones;i++ ) {
            DestroyButton( pPanel->Botones[i] );
        };
        PanelActivo = 0;
        free( pPanel );
    }
    #endif
};
void HidePanelArea()
{
    #ifdef COMPILAR_DYNAKEY
    if( ancho_panel ) {
        Rectangle( 580, 0, 639, 479, BLACK );
    }
    #endif
};
void AbrirArchivosPanel()
{
    #ifdef COMPILAR_DYNAKEY
    int a;
    if( ancho_panel ) {
        a = OPEN_TABLE( T_PANEL_VGA, TT_ORIG, ( char* )&_panel, sizeof( tag_PANEL ) );
        if( a ) {
            GRABAR_LOG_SISTEMA( "error al abrir Table PANEL_VGA" );
            CR0_EXIT( 1 );
        }
        a = OPEN_TABLE( T_BOTON_VGA, TT_ORIG, ( char* )&_button, sizeof( tag_BUTTON ) );
        if( a ) {
            GRABAR_LOG_SISTEMA( "error al abrir Table BOTON_VGA " );
            CR0_EXIT( 1 );
        }
    }
    #endif
};
void CerrarArchivosPanel()
{
    #ifdef COMPILAR_DYNAKEY
    //CLOSE_DATABASE( AREA_BOTON );
    CLOSE_TABLE( T_PANEL_VGA, TT_ORIG );
    #endif
};
int TeclaPresionada( PANEL *pPanel, int Tecla )
{
    int Returned = NONE;
    #ifdef COMPILAR_DYNAKEY
    int Boton;
    Boton = BuscarBoton( pPanel, Tecla );
    if( Boton != NONE ) {
        Returned = ClickEx( pPanel, Boton, 1 );
    };
    #endif
    return Returned;
};

