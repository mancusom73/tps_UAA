#pragma pack(1)
#pragma warning (disable:4243)
#include <cr.h>
#if defined(INVEL_W) || defined(INVEL_L)
#ifdef INVEL_W
#include "stdafx.h"
#endif
#include <stdlib.h>
#pragma pack(8)
#include "PanelMenu.h"
#include "CInput.h"
#include "AppMain.h"

extern PanelMenu *panMenu;
extern CDesktop *properties;

extern "C"
{
    int MENU( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum )
    {
        return panMenu->Menu( titulo, items, nitem, cab, colum );
    }

int MENU_PRESENT( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum )
    {
		return panMenu->Menu_Present( titulo, items, nitem, cab, colum );
    }
}

PanelMenu::PanelMenu( const wxString &title, const wxPoint &pos, const wxSize &size ) 
: CPanel( title, pos, size, wxSTAY_ON_TOP | wxSIMPLE_BORDER )
{
    //PanelMenu::title = new CLabel();
    #ifdef INVEL_W
    input = new CInput( this, 35, 150, 90, 17, false );
    #else
    input = new CInput( this, 600, 150, 90, 17, false );
    #endif
};

/*PanelMenu::PanelMenu()
: CPanel("Invel Input",wxDefaultPosition, wxSize(150, 200), wxSTAY_ON_TOP | wxSIMPLE_BORDER  )
{title.setParent(this);};
*/
void PanelMenu::createLabels()
{
    //title->setParent( this );
    #ifdef INVEL_W
    input->SetNotVisible();
    #endif
    this->create();
}

int PanelMenu::Menu( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum )
{
    int rta = 0, k = 0, l_item = 0,  i = 0, salir = 0, y = 0, w = 0, h = 0, go = 0, lcad = 0, indice = 0;    
    char subtitulo[200];
    CLabel **vectorItems;
    vectorItems = new CLabel*[nitem+2]; //los items + titulo + subtitulo

    #ifdef INVEL_L
    MutexGuiEnter();	
    #endif      

	/* Titulo del Menu */
	indice = 0;
    vectorItems[indice] = new CLabel( this );
    vectorItems[indice]->setText( titulo );    
    vectorItems[indice]->setFont( title_font );
    vectorItems[indice]->setColour( title_color );
    w = vectorItems[indice]->getTextWidth() + 20;
    w = ( ( w > width ) ? width : width + 2 );
    SetSize( wxSize( w, nitem * 20 + 60 ) );
    vectorItems[indice]->setPosition( w / 2, 10 );
    vectorItems[indice]->setAlign( title_align );
    vectorItems[indice]->setVisible( true );
    this->AddObject( vectorItems[indice] );
	indice++;

	/* Subtitulo del Menu */
	if( cab != NULL ) {		
		memset( subtitulo, 0, sizeof( subtitulo ) );
		for( i = 0; i < colum; i++ ) {
			lcad += strlen( cab[i].nombre );
		}	
		if( lcad < sizeof( subtitulo ) ) {
			for( i = 0; i < colum; i++ ) {
				if( i != 0 ) {
					strncat( subtitulo, " | ", sizeof( subtitulo ) - strlen( subtitulo ) - 1 );
				}
				strncat( subtitulo, cab[i].nombre, sizeof( subtitulo ) - strlen( subtitulo ) - 1 );			
			}
		}
		vectorItems[indice] = new CLabel( this );
		vectorItems[indice]->setText( subtitulo );    
		vectorItems[indice]->setFont( title_font );
		vectorItems[indice]->setColour( title_color );
		w = vectorItems[indice]->getTextWidth() + 20;
		vectorItems[indice]->setPosition( w , 30 );
		vectorItems[indice]->setAlign( "right" );
		vectorItems[indice]->setVisible( true );
		this->AddObject( vectorItems[indice] );
		indice++;
	}

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif      
	
   this->Activate();
   ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input );    

    for( h = indice, i = y + 1;(h -indice) < nitem;h++ ) {
        if( items[h-indice].nom ) {                     
            vectorItems[h] = new CLabel( this );
            //#ifdef INVEL_L
            MutexGuiEnter();
           // #endif      
            vectorItems[h]->setFont( items_font );
            vectorItems[h]->setColour( items_color );
            vectorItems[h]->setAlign( items_align );
            vectorItems[h]->setText( items[h-indice].nom );
            vectorItems[h]->setPosition( 20, 20 + 20 * h );
            vectorItems[h]->setVisible( true );
            this->AddObject( vectorItems[h] );
            i++;
            //#ifdef INVEL_L
            Refresh();
            MutexGuiLeave();
            //#endif
			wxYield();
        }       
    }
	this->Update();
    /*---------------------------- Trata teclas ----------------------------*/
    while( !salir ) {
        k = GETCH();
		if( k!=-999  ){
			switch( k ) {
				case 27:
					rta = -1;
					salir = 1;
					break;
			}
			for( h = 0; h < nitem ;h++ ) {
				if( k == items[h].tecla ) {
					rta = items[h].rta;
					salir = 1;
				}
			}
		}
    }

    /*------------------------------- Sale ---------------------------------*/
    this->DesActivate();    
    /*- Elimino los objetos creados -*/
    
    for( h = 0; h < nitem + indice; h++ ) { //los items + titulo + subtitulo
        delete vectorItems[h];
    }
    delete vectorItems;

    //this->DelAllObject( nitem );    
    this->ClearObjects();

    return ( rta );
}

int PanelMenu::Menu_Present( char *titulo, struct _menu *items, int nitem, struct _cabmenu *cab, int colum )
{
    int      /*pant,*/ rta, k, l_item = 0,  i = 0, salir = 0,y = 0;
    int p = 0, q = 0;
	int w,h;    
    int go = 0;
	char stringData1[20], stringData2[20], separador[3] = {" :"};
    CLabel **vectorItems;
    vectorItems = new CLabel*[2*nitem + 2];

    #ifdef INVEL_L
    MutexGuiEnter();
    #endif      

    vectorItems[0] = new CLabel( this );
    vectorItems[0]->setText( titulo );
    vectorItems[0]->setFont( title_font );
    vectorItems[0]->setColour( title_color );
    w = vectorItems[0]->getTextWidth() + 20;
    w = ( ( w > width ) ? width : width + 2 );
    SetSize( wxSize( w, nitem * 20 + 60 ) );

    vectorItems[0]->setPosition( w / 2, 10 );
    vectorItems[0]->setAlign( title_align );
    vectorItems[0]->setVisible( true );
    this->AddObject( vectorItems[0] );

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif      
  
    this->Activate();
    ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( input );
    
	for( h = 1/*, i = y + 1*/;h <= (nitem + 1) ;h++ ) {
		if( h == 1){
			_snprintf(stringData1, sizeof(stringData1)-1, "%s","Cod. Pres." );
			_snprintf(stringData2, sizeof(stringData2)-1, "%s","Cant. Art." );
		}
		else{
			sscanf( items[h-2].nom, "%s - %s", &stringData1, &stringData2 );
		}
		if( items[h].nom/*items[h-2].nom */) {       
            vectorItems[h] = new CLabel( this );
            #ifdef INVEL_L
            MutexGuiEnter();
            #endif      
            vectorItems[h]->setFont( items_font );
            vectorItems[h]->setColour( items_color );
			vectorItems[h]->setAlign( items_align );
			vectorItems[h]->setAlign( items_align );
			vectorItems[h]->setText( stringData1 );
			if( h == 1){
				vectorItems[h]->setPosition( 40, 15 + 20 * h );
			}
			else{
				vectorItems[h]->setPosition( 60, 15 + 20 * h );
			}
            vectorItems[h]->setVisible( true );
            this->AddObject( vectorItems[h] );
	
			vectorItems[h + nitem] = new CLabel( this );      
            vectorItems[h + nitem]->setFont( items_font );
            vectorItems[h + nitem]->setColour( items_color );
            vectorItems[h + nitem]->setAlign( items_align );
            vectorItems[h + nitem]->setText( stringData2 );
			if( h == 1){
				vectorItems[h + nitem]->setPosition( 110, 15 + 20 * h );
			}
			else{
				vectorItems[h + nitem]->setPosition( 130, 15 + 20 * h );
			}
			vectorItems[h + nitem]->setVisible( true );
            this->AddObject( vectorItems[h + nitem] );

            //i++;
            #ifdef INVEL_L
            Refresh();
            MutexGuiLeave();
            #endif                
        }        
    }
   
    /*---------------------------- Trata teclas ----------------------------*/
    while( !salir ) {
        k = GETCH();
        switch( k ) {
            case 27:
                rta = -1;
                salir = 1;
                break;
        }
        for( h = 0;h < nitem;h++ ) {
            if( k == items[h].tecla ) {
                rta = items[h].rta;
                salir = 1;
            }
        }
    }

    /*------------------------------- Sale ---------------------------------*/
    this->DesActivate();    
    /*- Elimino los objetos creados -*/
    
    for( h = 0; h < nitem; h++ ) {
        delete vectorItems[h];
    }
    delete vectorItems;

    //this->DelAllObject( nitem );    
    this->ClearObjects();

    return ( rta );
}

void PanelMenu::init()
{
}


void PanelMenu::changeFocus( int type )
{
    focus = type;
}

CInput * PanelMenu::getFocus()
{
    return input;
}

void PanelMenu::create()
{
    int input_size = 17;
    int offset = 3;
    int start_offset = 35;
    string id,background;

    CWindow *menu;
    CRepository *repository;
    CSkin *def;
    CTLabel *label;

    repository = properties->GetRepository();
    menu = properties->GetWindows()->GetWindowByName( "menu" );
    id = menu->GetImageRef()->GetID();
    def = properties->GetImages()->GetSkinByName( "default" );
    background = def->GetImageByName( id )->GetFilename();
    SetBackGround( background.c_str() );

    width = menu->GetSize()->GetWidth();
    height = menu->GetSize()->GetHeight();



    label = ( CTLabel * )repository->GetComponentByName( "TitMenu", "TLabel" );
    title_font = label->GetLabelFont()->ToString();
    title_color = label->GetLabelColor();
    title_align = label->GetLabelAlign();
    delete label;
    label = ( CTLabel * )repository->GetComponentByName( "ItemsMenu", "TLabel" );
    items_font = label->GetLabelFont()->ToString();
    items_color = label->GetLabelColor();
    items_align = label->GetLabelAlign();
    delete label;
    delete menu;
}

#endif
