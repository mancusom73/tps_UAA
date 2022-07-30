/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Definicin de las estructuras para el manejo de los Paneles de
 * La idea es que el orden de los paneles sean del tipo 001,002,003,ect;
 * mientras que el orden los botones sea 100,101,102,etc para el panel
 * 001 y 200,201,202,etc. para el panel 002.
 * ------------------------------------------------------------------------------
 */

#ifndef __PANEL_H__
#define __PANEL_H__

#include "boton.h"
#include "tipos.h"
#include "bitmap.h"

#define BUTTON_SELECTED    1
#define BUTTON_UNSELECTED  0
#define NONE               -1

#define NUMERO_PANEL_MINIMO_EN_PAGOS    7
#define NUMERO_PANEL_MAXIMO_EN_PAGOS    8
#define NUMERO_PANEL_BILLETES           6
#define NUMERO_PANEL_LETRAS             30
#define NUMERO_PANEL_MINIMO_EN_RETIROS  20
#define NUMERO_PANEL_MAXIMO_EN_RETIROS  21
#define NUMERO_PANEL_CLIENTE            22
#define NUMERO_PANEL_SUPERVISOR         23
#define NUMERO_PANEL_CAJERO             24
#define NUMERO_PANEL_ESPECIALES         25
#define NUMERO_PANEL_VALIDA_TARJETA     26
#define NUMERO_PANEL_LETRAS             30
#define NUMERO_PANEL_PRINCIPAL          1


typedef struct tagRECT
{
    int left;
    int right;
    int top;
    int bottom;
} RECT;

typedef struct tagBUTTON
{
    int Orden;
    char Title[41];
    BITMAP Imagen;
    BOOLEAN Activado;
    BOOLEAN Seleccionado;
    int Lugar;
    int Tecla;
    RECT Area;
} BUTTON;

typedef struct tagPANEL
{
    int Orden;
    char Title[41];
    RECT Area;
    int BotonActual;
    int Altura;
    int Espaciado;
    int CantBotones;
    BUTTON *Botones[12];
    int TeclaBoton[12];
} PANEL;

/* Estructuras de los archivos BTrieve */
/* Archivo PanelVga.Btr */
typedef struct tag_PANEL
{
    BINARY status;
    int Orden;
    char Title[41];
    int Left;
    int Top;
    int Right;
    int Bottom;
    int BotonActual;
    int Altura;
    int Espaciado;
    int CantBotones;
    int TeclaBoton0;
    int TeclaBoton1;
    int TeclaBoton2;
    int TeclaBoton3;
    int TeclaBoton4;
    int TeclaBoton5;
    int TeclaBoton6;
    int TeclaBoton7;
    int TeclaBoton8;
    int TeclaBoton9;
    int TeclaBoton10;
    int TeclaBoton11;
} _PANEL;

/* Archivo BotonVga.Btr */
typedef struct tag_BUTTON
{
    BINARY status;
    int Orden;
    int Left;
    int Top;
    int Right;
    int Bottom;
    char Title[41];
    char Imagen[13];
    int Lugar;
    int Tecla;
} _BUTTON;

/* Variables Globales para el acceso unificado a los Archivos Btrieve */
#ifndef _SKIP_HEADER_
extern _BUTTON _boton;
extern _PANEL _panel;
#endif

/* Prototipos de Funcin para el manejo de las estructuras */
int GetWidth( RECT *pRect );
int GetHeight( RECT *pRect );
BUTTON *CreateButton( int Orden, char *Title, char *FileName, BOOLEAN Activado, int Lugar,
                      int Tecla, RECT Area );
void DestroyButton( BUTTON *pButton );
PANEL *CreatePanel( int Orden, STRING Title, RECT Area, int PrimerBoton, int Espaciado );
void AddButton( PANEL *pPanel, BUTTON *pButton );
void DestroyPanel( PANEL *pPanel );
void ShowButton( BUTTON *pButton, int Altura );
void ShowPanel( PANEL *pPanel );
void HidePanel( PANEL *pPanel );
void ClearPanel( PANEL *pPanel );
void SelectButton( BUTTON *pButton, BOOLEAN Select );
void SetBotonActual( PANEL *pPanel, int Boton );
void ButtonDown( BUTTON *pButton );
void ButtonUp( BUTTON *pButton );
void PressButton( PANEL *pPanel, int Boton );
void ReleaseButton( PANEL *pPanel, int Boton );
int Click( PANEL *pPanel, int Boton );
void ShowButtonEx( BUTTON *pButton, int Altura, BOOLEAN Pressed );
void ButtonDownEx( BUTTON *pButton, int Style );
void ButtonUpEx( BUTTON *pButton, int Style );
int ClickEx( PANEL *pPanel, int Boton, int Style );
void ActivateButton( PANEL *pPanel, int Boton );
void DesactivateButton( PANEL *pPanel, int Boton );
void AsignarTecla( PANEL *pPanel, int Boton, int Tecla );
void AsignarTeclas( PANEL *pPanel, int Teclas[12] );
int BuscarBoton( PANEL *pPanel, int Tecla );
int TeclaPresionada( PANEL *pPanel, int Tecla );

/* Funciones especiales para el acceso a BTrieve */
void AbrirArchivosBtrieve();
void CerrarArchivosBtrieve();
BOOLEAN SeekPanel( int Orden );
BOOLEAN SeekButton( int Orden );
BOOLEAN PrepareButton( BUTTON **pButton, int Orden );
BOOLEAN PreparePanel( PANEL **pPanel, int Orden );
void RegistroPanel( PANEL *pPanel );
void RegistroButton( BUTTON *pButton );
void ActivatePanel( PANEL *pPanel );
void DesactivatePanel( PANEL *pPanel );
void RefreshPanel( PANEL *pPanel );
extern PANEL *pPanel;
extern int Orden;
extern int PanelActivo;
extern int Response;
void HidePanelArea();

void AbrirArchivosPanel();
#endif
