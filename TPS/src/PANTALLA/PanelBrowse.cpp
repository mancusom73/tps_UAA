#pragma pack(1)
#pragma warning (disable:4243)
#include <cr.h>
#if defined(INVEL_W) || defined(INVEL_L)
#ifdef INVEL_W
#include "stdafx.h"
#endif
#include <stdlib.h>
#include <btrv.h>
#include <db.h>
#pragma pack(8)
#include "PanelBrowse.h"
#include "PanelCobro.h"
#include "PanelPagosCaja.h"
#include "PanelPagosEsp.h"
#include "AppMain.h"
#include "CInput.h"
#include "CScreencfg.h"
#include "bridge.h"
#include "actions.h"
#include "CConfig.h"
#include "MainPanel.h"
#include <time.h>
extern "C"{
#include <cmemory.h>
#include <b_mens.h>
#include <pant.h>
extern int GET_POSICION_PANTALLAS_X(void);
extern int GET_POSICION_PANTALLAS_Y(void);
}
int EVALUA_TECLA( int tecla );
int CAMBIO_TECLA_TOTAL_COBROS_VARIOS( int *tecla );
static int idArray[MAXBROWSE];
PanelBrowse  *_brow[MAXBROWSE];
extern CPanelBrowse *panBrowse;
extern CWindow *browseWin;
extern PanelCobro *panCobro;
extern PanelPagosCaja *panPagosCaja;
extern PanelPagosEsp *panPagosEsp;
extern CConfig *Config;
extern MainPanel frame;
extern wxSize *TamanioVentana;

CInput   *input_browse1    = NULL;

#define CONSULA_DESCRIPCION  2
#define CONSULTA_COD_INTERNO 3
#define CONSULTA_MARCA       5
#define L_CADENA             25
#define ANCHO_PANEL_BROWSE   700
AppMain *app_browse;

extern CDesktop *properties;
CRepository* repository;
CSkin* def;
CTLabel* titBrowse, *titGridBrowse;
CLang* lang;
void setBrowseXML( CPanelBrowse *browse, CBrowse *browse1, string name, int index )
{
    CTGrid *grid; /*CWindow *browseWin;*/ CComponentRef *compRef;
    CMessage *msgs;
    int i;
    char width[5],row[5],aux[100];
    string id,background,font,textColor,backColor,selColor;
    string colSize,align,msg;

	//titBrowse = (CTLabel*)repository->GetComponentByName("TitPanelBrowse");
    //titGridBrowse = (CTLabel*)repository->GetComponentByName("TitGridPanelBrowse");

    browseWin = properties->GetWindows()->GetWindowByName( name );
	id = browseWin->GetImageRef()->GetID();

    background = def->GetImageByName( id )->GetFilename();
    panBrowse->SetBackGround( background.c_str() );


    grid = ( CTGrid * )repository->GetComponentByName( name, "TGrid" );
    font = grid->GetGridColumnAt( 1 )->GetFont()->ToString();
    textColor = grid->GetGridColumnAt( 1 )->GetColor()->GetText();
    backColor = grid->GetGridColumnAt( 1 )->GetColor()->GetBackground();
    selColor = grid->GetGridColumnAt( 1 )->GetSelectedColor()->GetBackground();

	sprintf( row, "%d", grid->GetGridRowSize() );

    panBrowse->setCharPanelBrowse( background, _INDICE_GIF, index );
    panBrowse->setCharPanelBrowse( font, _INDICE_FONT_PAN, index );
    panBrowse->setCharPanelBrowse( textColor, _INDICE_FONT_COLOR_PAN, index );
    panBrowse->setCharPanelBrowse( backColor, _INDICE_BACK_COLOR_PAN, index );
    panBrowse->setCharPanelBrowse( selColor, _INDICE_SEL_BACK_COLOR_PAN, index );


    /* --- asignacion del tamao de las filas de la grilla del panel--- */
    panBrowse->setCharPanelBrowse( row, _INDICE_ROW_SIZE_PAN, index );

    /* --- asignacion de la fuente del texto de la grilla del panel--- */
    panBrowse->setCharPanelBrowse( font, _INDICE_FONT_TEXT_PAN, index );

    /* --- asignacion del color de la fuente del texto de la grilla del panel--- */
    panBrowse->setCharPanelBrowse( "(0-200-0)", _INDICE_FONT_COLOR_TEXT_PAN, index );

    /* --- asignacion de todas las posisciones del texto del panel--- */
    panBrowse->setCharPanelBrowse( "(0-0)", _INDICE_TEXT_POS_TEXT_PAN, index );

    /* --- asignacion de valores del browse de consulta articulo por codigo de barras ---*/
    colSize = "(";
    for( i = 1;i <= grid -> GetGridColumnCount();i++ ) {
        sprintf( width, "%d", grid->GetGridColumnAt( i )->GetWidth() );
        colSize += string( width ) + "-";
    }
    colSize[colSize.length() - 1] = ')';
    browse1->setWidthColBrowse( colSize, index );
    compRef = browseWin->GetComponents()->GetComponentRefByName( name );

    sprintf( aux, "(%d-%d-%d-%d)", compRef->GetPosx(), compRef->GetPosy(),
             grid->GetGridSize()->GetWidth(), grid->GetGridSize()->GetHeight() );
    panBrowse->setSizePanelBrowse( aux, index );

    compRef = browseWin->GetComponents()->GetComponentRefByName( "TitPanelBrowse" );
    font = titBrowse->GetLabelFont()->ToString();
    textColor = titBrowse->GetLabelColor();
    align = titBrowse->GetLabelAlign();
    panBrowse->setCharTit( font, _INDICE_FUENTE, index );
    panBrowse->setCharTit( textColor, _INDICE_COLOR, index );
    sprintf( aux, "(%d-%d)", compRef->GetPosx(), compRef->GetPosy() );
    panBrowse->setCharTit( aux, _INDICE_POSI, index );
    panBrowse->setCharTit( align, _INDICE_ALIN, index );

    compRef = browseWin->GetComponents()->GetComponentRefByName( "TitGridPanelBrowse" );
    font = titGridBrowse->GetLabelFont()->ToString();
    textColor = titGridBrowse->GetLabelColor();
    align = titGridBrowse->GetLabelAlign();
    panBrowse->setCharTit( font, _INDICE_FUENTE_TIT_GRID, index );
    panBrowse->setCharTit( textColor, _INDICE_COLOR_TIT_GRID, index );
    sprintf( aux, "(%d-%d)", compRef->GetPosx(), compRef->GetPosy() );
    panBrowse->setCharTit( aux, _INDICE_POSI_TIT_GRID, index );
    panBrowse->setCharTit( align, _INDICE_ALIN_TIT_GRID, index );

    sprintf( aux, "tit%s", name.c_str() );
    msgs = lang->GetMessageByName( aux );
    msg = msgs->GetMessage();
    panBrowse->setCharTit( msg, _INDICE_TEXT, index );
    sprintf( aux, "grid%s", name.c_str() );
    msgs = lang->GetMessageByName( aux );
    msg = msgs->GetMessage();
    panBrowse->setCharTit( msg, _INDICE_TEXT_COL, index );

    //compRef = browseWin->GetComponents()->GetComponentRefByName("TitGridPanelBrowse");
    //sprintf(aux,"(%d-%d)", compRef->GetPosx(), compRef->GetPosy());
    //panBrowse->setCharTit(aux, _INDICE_POSI_TEXT_COL, index);
}

char * CREATE_BROWSE_PANEL( AppMain *_app )
{
    int x_grid = 15, y_grid = 30, w_grid = 700, h_grid = 270;
    int x_input,y_input,w_input,h_input;
    //int x_title, y_title, w_title, h_title;
    //int x_browse, y_browse, w_browse, h_browse;

    CBrowse *browse1 = NULL;
    CInput *input_browse1 = NULL;

    CComponentRef *compRef;
    CTInput *input;
    //CTLabel* label;
    

    app_browse = _app;

    repository = properties->GetRepository();
    def = properties->GetImages()->GetSkinByName( "default" );

    browseWin = properties->GetWindows()->GetWindowByName( "browseArticuloCodbarra" );
    compRef = browseWin->GetComponents()->GetComponentRefByName( "InputBrowse" );
    x_input = compRef->GetPosx();
    y_input = compRef->GetPosy();
    input = ( CTInput * )( repository->GetComponentByName( "InputBrowse" ) );
    w_input = input->GetInputSize()->GetWidth();
    h_input = input->GetInputSize()->GetHeight();
    delete input;
    delete compRef;
    //delete browseWin;

    lang = properties->GetText()->GetLangByName( "default" );

    titBrowse = ( CTLabel * )repository->GetComponentByName( "TitPanelBrowse" );
    titGridBrowse = ( CTLabel * )repository->GetComponentByName( "TitGridPanelBrowse" );

    /* --- creacin del panel ---*/

    /*if(Config->get_touch_screen()){
        panBrowse = new CPanelBrowse( "Consulta", wxPoint( 0, 0 ), wxSize( w_grid + x_grid * 2, 645 ));
    }
    else{*/
	
	
	
    panBrowse = new CPanelBrowse( "Consulta",
		wxPoint( 
		GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
		GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2))
		),

		wxSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight()));
    if(!Config->get_touch_screen()){
		//panBrowse->Center();
	}
    //}
    panBrowse->createLabels();

    /* --- creacin del input box en el panel ---*/
    input_browse1 = new CInput( panBrowse, x_input, y_input, w_input, h_input, false,
                                wxDOUBLE_BORDER );
    //input_browse1->SetNotVisible();


    /* --- creacin del browse dentro del panel ---*/
    browse1 = new CBrowse( panBrowse, wxPoint( x_grid, y_grid ), wxSize( w_grid, h_grid ) );
    browse1->setPanelBrowseActivo( ( char* )panBrowse );

    /* --- asignacion del input box y browse al panel activo ---*/
    panBrowse->SetDefaultBrowseBrowse( browse1 );
    panBrowse->SetDefaultInputBrowse( input_browse1 );

    /* --- creacin del t?ulo del panel del browse ---*/
    panBrowse->tituloPanelBrowse->setText( "" );
    panBrowse->AddObject( ( panBrowse->tituloPanelBrowse ) );

    panBrowse->labelbrowse->setText( " " );
    panBrowse->AddObject( ( panBrowse->labelbrowse ) );

    setBrowseXML( panBrowse, browse1, "browseArticuloCodbarra", _CONSULTA_ART_CODBARRA );
    setBrowseXML( panBrowse, browse1, "browseConsultaDescrip1", _CONSULTA_ART_DESCRIPC1 );
    setBrowseXML( panBrowse, browse1, "browseConsultaDescrip2", _CONSULTA_ART_DESCRIPC2 );
    setBrowseXML( panBrowse, browse1, "browseConsultaDescrip3", _CONSULTA_ART_DESCRIPC3 );
    setBrowseXML( panBrowse, browse1, "browseConsultaDescrip4", _CONSULTA_ART_DESCRIPC4 );
    setBrowseXML( panBrowse, browse1, "browseConsultaDescrip5", _CONSULTA_ART_DESCRIPC5 );
    setBrowseXML( panBrowse, browse1, "browseMotivo", _CONSULTA_MOTIVO );
    setBrowseXML( panBrowse, browse1, "browseMutuales", _CONSULTA_MUTUALES );
    setBrowseXML( panBrowse, browse1, "browsePideCuoMut", _CONSULTA_PIDE_CUOMUT );
    setBrowseXML( panBrowse, browse1, "browsePerfil", _CONSULTA_PIDE_PERFIL );
    setBrowseXML( panBrowse, browse1, "browsePideTarjeta", _CONSULTA_PIDE_TARJETA );
    setBrowseXML( panBrowse, browse1, "browseClientes", _CONSULTA_CLIENTES );
    setBrowseXML( panBrowse, browse1, "browseFunciones", _CONSULTA_FUNCION );
    setBrowseXML( panBrowse, browse1, "browseInstituciones", _CONSULTA_INSTITUCIONES );
    setBrowseXML( panBrowse, browse1, "browseTCompra", _CONSULTA_T_COMPRA );
    setBrowseXML( panBrowse, browse1, "browseCuotasTar", _CONSULTA_CUOTAS_TARJ );
    setBrowseXML( panBrowse, browse1, "browseVigencia", _CONSULTA_VIGENCIA );
    setBrowseXML( panBrowse, browse1, "browseDiferimiento", _CONSULTA_DIFERIMIENTO );
    setBrowseXML( panBrowse, browse1, "browseTarjeta", _CONSULTA_TARJETA );
    setBrowseXML( panBrowse, browse1, "browseBancos", _CONSULTA_BANCOS );
    setBrowseXML( panBrowse, browse1, "browseRepactacion", _CONSULTA_REPACTACION );
    setBrowseXML( panBrowse, browse1, "browseElegirCuotas", _CONSULTA_ELEGIR_CUOTAS );
    setBrowseXML( panBrowse, browse1, "browseElegirRemito", _CONSULTA_ELEGIR_REMITO );
    setBrowseXML( panBrowse, browse1, "browseDataEntry", _CONSULTA_DATA_ENTRY );
    setBrowseXML( panBrowse, browse1, "browseTipoCliente", _CONSULTA_TIPO_CLIENTES );
    setBrowseXML( panBrowse, browse1, "browseGruposCli", _CONSULTA_GRUPOS_CLI );
    setBrowseXML( panBrowse, browse1, "browseCliReintegros", _CONSULTA_CLI_REINTEGROS );
	setBrowseXML( panBrowse, browse1, "browseDescrip", _CONSULTA_DESCRIPT );

    return ( ( char* )panBrowse );
}

int PanelBrowse::BROWSE( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario,
                         char *titulo, double monto )
{
    return BROWSE( x1, y1, x2, y2, l_cadena, f_usuario, titulo, SI, monto, 0 );
}

int PanelBrowse::BROWSE( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario,
                         char *titulo, double monto, unsigned short cod_medio )
{
    return BROWSE( x1, y1, x2, y2, l_cadena, f_usuario, titulo, SI, monto, cod_medio );
}

int PanelBrowse::BROWSE( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario,
                         char *titulo, int buscar, double monto, unsigned short cod_medio )
{
    int rta = 0, h, ok, i/*,cadTemp*/;
    char cadena[61] = {""};
    long reg;
    browse.seguir = 1;
    browse.mostrar = 2;
    browse.linea = 0;
    browse.pos = 0;
    browse.posicionar = 0;
    browse.cant_cola = 0;
    browse.index = this->id;
    browse.alto = y2 - y1 + 1;
    cadena[l_cadena] = 0;
    clock_t timex = clock();
    int limit_input = min( sizeof( browse.cola_proceso ), l_cadena );
    f_usuario( _SET_MODO, NULL, 0, &browse, monto, cod_medio );
    if( !browse.modo ) {
        browse.registros = RECCOUNT( );
    } else {
        f_usuario( _SET_REGISTROS, NULL, 0, &browse, monto, cod_medio ); //Setea el nro de registros.
    }

    //__browse->init( id, fieldsDefinition, SelectedField, ( x2-x1 >= 200? x2-x1 : 200 ) );
    __browse->init( id, fieldsDefinition, SelectedField, ( 50 ), browse.alto );
    ///#ifdef INVEL_L
    MutexGuiEnter();
    ///#endif
    __browse->SetColLabelSize( 0 );
    __browse->SetRowLabelSize( 0 );
    __browse->EnableEditing( false );
    __browse->EnableGridLines( false );
    __browse->deleteInput();
    ///#ifdef INVEL_L
    MutexGuiLeave();
    ///#endif


    TECLADO_ALFABETICO = 1;
    while( browse.seguir ) {
		if( browse.posicionar ) {
            _ATRIBUTO = 112;
            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
			f_usuario( _BUSCAR, cadena, 0, &browse, monto, cod_medio );

            browse.linea = 0;
            browse.posicionar = 0;
            browse.mostrar = 2;
            if( browse.alto > 0 ) {
                __browse->BeginBatch();
                __browse->deleteRows( browse.alto );
                __browse->EndBatch();
            }
            #ifdef INVEL_L
            MutexGuiLeave();
            #endif
        }

        if( browse.mostrar ) {
            //*------------------- Refresca la pantalla -------------------
            if( browse.mostrar == 2 ) {
				char first = 1;
                _ATRIBUTO = 120;
                reg = GET_POSITION();
                #ifdef INVEL_L
                MutexGuiEnter();
                #endif
                f_usuario( _SKIP, cadena, -browse.linea, &browse, monto, cod_medio );
                ok = 0;
                __browse->setEnable( id, false );
                __browse->BeginBatch();
                for( h = 0;h < browse.alto;h++ ) {
                    f_usuario( _VACIO, cadena, h, &browse, monto, cod_medio );
                    //Cobros Marginales Online: Para el caso en donde el ticket contiene un
                    //Cobros Marginales Online( por restriccion no se puede tener más de uno por ticket )
                    //Si el 1er reg a evaluar es una Cobros Marg. Online lo muestra igual porque
                    //esta ok = 0 y no valida la resp del SKIP quien condiciona la restricción
                    //mencionada anteriormente.
                    if( !mostrar_cobro ) {
                        ok = 1;
                        mostrar_cobro = 1;
                    }

                    if( !ok ) {
                        f_usuario( _MOSTRAR, cadena, h, &browse, monto, cod_medio );
                        setRow();
						//Se descomenta porque necesita posicionarse en el primer reg que se visualiza en el
						//browser ya que si se da ENTER(sin el uso de SKIP) 
						//y el primero que visualiza no es el primero de la posicion que se guardo
						//se posiciona incorrectamente. Se verifica este defecto para COBROS.
						if( first ){
							reg = GET_POSITION();
							first = 0;
						}
                    }
                    //ok = f_usuario( _SKIP, cadena, 1, &browse, monto, cod_medio );
                    ok = (!ok) ? f_usuario( _SKIP, cadena, 1, &browse, monto, cod_medio ) : ok;
				}
                __browse->EndBatch();
                GET_DIRECT( reg );
                #ifdef INVEL_L
                MutexGuiLeave();
                #endif
                __browse->setEnable( id, true );

                #ifdef INVEL_L
                __browse->ForceRefresh();
                #endif
            }
            else if( browse.mostrar == 3 ) {
                #ifdef INVEL_L
                MutexGuiEnter();
                #endif
                f_usuario( _MOSTRAR, cadena, browse.linea, &browse, monto, cod_medio );
                setRow();
                #ifdef INVEL_L
                MutexGuiLeave();
                #endif
            }
            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
            __browse->selRow( id, browse.linea );
            #ifdef INVEL_L
            MutexGuiLeave();
            #endif

            browse.mostrar = 0;
            BORRAR_MENSAJE();
        }

        //////////////////////// DEBUG ///////////////////////////
/*#ifdef _DEBUG
        //Se verifica la equivalencia entre lo seleccionado en pantalla y lo que esta en la estructura
        wxString test;
        test = __browse->GetCellValue(__browse->GetSelectedRows()[0], 0);
        //Definir el siguiente 'define', segun el caso que se pruebe
#ifdef _DEBUG_BROWSE_ARTICULO 
        //Para tabla articulos:
        assert( test.IsSameAs( articulo.cod_barra ) ); 
#endif
#endif*/
        //////////////////////////////////////////////////////////

        //*-------------------------- Ingresa teclas --------------------*      
        timex = clock();
        do {
            browse.tecla = FAST_GETCH();
			if( browse.tecla != -999 ) {
              int h = browse.tecla  ;
            }
            /* Las teclas de desplazamiento se evaluan inmediatamente */
            if( browse.tecla == 8 || browse.tecla == 13 || browse.tecla == 27 || 
                browse.tecla == -81 || browse.tecla == -80 || browse.tecla == -73 || 
                browse.tecla == -72 ||browse.tecla == -75 ||browse.tecla == -74 || browse.tecla == 700 ) {
                break;
            }
            /* Si la tecla es -999 no se puede salir ya q podemos estar esperando
             * mas input */
            if( browse.tecla != -999 && ( browse.pos + browse.cant_cola < limit_input ) ) {
                if( f_usuario( _VALIDAR, cadena, browse.tecla, &browse, monto, cod_medio ) ) {
                    browse.cola_proceso[browse.cant_cola] = browse.tecla;
                    AGREGAR_INPUT_CADENA( (char*)&browse.cola_proceso[browse.cant_cola], browse.pos + browse.cant_cola );
                    browse.cant_cola++;
                    timex = clock();
                }
            }
        }while( ( clock() - timex ) < 200 && browse.cant_cola );
        /* Si se sale por timeout y hay datos en la cola, se necesita recuperar la ultima
         * tecla ya que en la espera (while) se sobreescribio con -999 */
        if( browse.cant_cola ) {
            browse.tecla = browse.cola_proceso[browse.cant_cola - 1];
        }

        #ifdef INVEL_L
        MutexGuiEnter();
        #endif
        f_usuario( _TECLA, cadena, browse.tecla, &browse, monto, cod_medio );
        #ifdef INVEL_L
        MutexGuiLeave();
        #endif

        switch( browse.tecla ) {
            case -72:
                //*-------- FLECHA ARRIBA ---------*
                #ifdef INVEL_L
                MutexGuiEnter();
                #endif

                if( f_usuario( _SKIP, cadena, -1, &browse, monto, cod_medio ) == 0 ) {
                    
                    browse.mostrar = 1;
                    if( browse.linea > 0 ) {
                        browse.linea--;
                        __browse->selRow( id, browse.linea );
                    }
                    else {
						f_usuario( _MOSTRAR, cadena, 0, &browse, monto, cod_medio );
						//#ifdef INVEL_W
						///MutexGuiEnter();
						//#endif
						insertRow( browse.linea );
						__browse->scroll( id, 1 );
						//#ifdef INVEL_W
						/// MutexGuiLeave();
						//#endif
                    }
                }

                #ifdef INVEL_L
                MutexGuiLeave();
                #endif

                break;

            case -73:
                //*-------- /PAGINA ARRIBA ---------*
                //#ifdef INVEL_L
                MutexGuiEnter();
                //#endif

                /*if( !f_usuario( _EXTREMO, cadena, TOP, &browse, monto, cod_medio ) ) {
                    f_usuario( _SKIP, cadena, -browse.alto, &browse, monto, cod_medio );
                    browse.linea = 0;
                }
                __browse->deleteRows( 0 );
                browse.mostrar = 2;*/

                //Simulamos que se presiona flecha abajo "browse.alto" cantidad de veces.
                __browse->BeginBatch();
                for(i = browse.alto; i > 0; i-- ) {
                    if( f_usuario( _SKIP, cadena, -1, &browse, monto, cod_medio ) == 0 ) {
                        
                        browse.mostrar = 1;
                        if( browse.linea > 0 ) {
                            browse.linea--;
                            __browse->selRow( id, browse.linea );
                        }
                        else {						
						    f_usuario( _MOSTRAR, cadena, 0, &browse, monto, cod_medio );
						    insertRow( browse.linea );               
						    __browse->scroll( id, 1 );
                        }
                    }
                    //estamos en el limite, salimos.
                    else {
                        break;
                    }
                }
                __browse->EndBatch();
                // #ifdef INVEL_L
                MutexGuiLeave();
                //#endif

                break;
            case -80:
                //*-------- FLECHA ABAJO ---------*
                #ifdef INVEL_L
                MutexGuiEnter();
                #endif

                if( f_usuario( _SKIP, cadena, 1, &browse, monto, cod_medio ) == 0 ) {
                    browse.mostrar = 1;
                    if( browse.linea < browse.alto - 1 ) {
                        browse.linea++;
                        __browse->selRow( id, browse.linea );
                    }
                    else {
                        f_usuario( _MOSTRAR, cadena, browse.alto - 1, &browse, monto, cod_medio );
                        __browse->scroll( id, -1 );
                        setRow();
                    }
                }
                #ifdef INVEL_L
                MutexGuiLeave();
                #endif

                break;

            case -81:
                //*-------- PAGINA ABAJO ---------*
                //#ifdef INVEL_L
                MutexGuiEnter();
                //#endif

                /*if( !f_usuario( _EXTREMO, cadena, BOTTOM, &browse, monto, cod_medio ) ) {
                    if( f_usuario( _SKIP, cadena, browse.alto, &browse, monto, cod_medio ) != 0 ) {
                        browse.linea = browse.alto - 1;
                        if( ( unsigned )browse.linea > browse.registros ) {
                            browse.linea = browse.registros - 1;
                        }
                    }
                    else {
                        browse.linea = browse.alto - 1;
                    }
                    if( browse.alto > 0 ) {
                        __browse->deleteRows( browse.alto );
                    }
                    browse.mostrar = 2;
                }*/
                
                __browse->BeginBatch();
                for( i=0; i < browse.alto; i++ ){
                    if( f_usuario( _SKIP, cadena, 1, &browse, monto, cod_medio ) == 0 ) {
                        browse.mostrar = 1;
                        if( browse.linea < browse.alto - 1 ) {
                            browse.linea++;
                            __browse->selRow( id, browse.linea );
                        }
                        else {
                            f_usuario( _MOSTRAR, cadena, browse.alto - 1, &browse, monto, cod_medio );
                            __browse->scroll( id, -1 );
                            setRow();
                        }
                    }
                    else {
                        break;
                    }
                }
                __browse->EndBatch();

                //#ifdef INVEL_L
                MutexGuiLeave();
                //#endif

                break;

            case 27:
                //*-------- ESC ---------*
                browse.seguir = 0;
                break;

            case 13:
                //*-------- ENTER ---------*
                if( rta = f_usuario( _ENTER, cadena, 0, &browse, monto, cod_medio ) ) {
                    browse.seguir = 0;
                    //rta = 1;
                }
                break;

            case 8:
                //*------ BACKSPACE -------*
                if( browse.pos > 0 ) {
                    browse.pos--;
                    cadena[browse.pos] = '\0';
                    browse.posicionar = 1;
                    MOSTRAR_INPUT_CADENA( cadena, browse.pos );
                    //__browse->scroll( id, 100 );
                }
                //--Fabi-04/04/99-Tecla total en cobros varios
            case 700:
                // *------- TOTAL -----------*
                if( f_usuario( _SPACE_, cadena, 0, &browse, monto, cod_medio ) ) {
                    browse.seguir = 0;
                }
                break;
            case -999:
                //*--- Sale por espera ---*
                f_usuario( _ESPECIAL, cadena, 0, &browse, monto, cod_medio );
                break;
            default:
                //*---------- caracteres de acercamiento ----------------*
                /*que sean distintas de: inicio, fin, flecha derecha, flecha izquierda*/
                if( buscar && browse.tecla != -75 && browse.tecla != -74
                    && browse.tecla != -76 && browse.tecla != -77) {

                    int i, procesar;

                    if( browse.pos + browse.cant_cola > l_cadena ) {
                        procesar = l_cadena - browse.pos;
                    }
                    else {
                        procesar = browse.cant_cola;
                    }

                    if( procesar ) {

                        for(i = 0; i < procesar; i++ ){
                            cadena[browse.pos] = browse.cola_proceso[i];
                            browse.pos++;
                        }
                        browse.posicionar = 1;
                        browse.cant_cola = 0;
                        MOSTRAR_INPUT_CADENA( cadena, browse.pos );
                    }

                }
                break;
        }
    }

    __browse->close( id );

    TECLADO_ALFABETICO = 0;

    return ( rta );
}


int PanelBrowse::BROWSE_MYSQL( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario,
                         char *titulo, double monto )
{
    return BROWSE_MYSQL( x1, y1, x2, y2, l_cadena, f_usuario, titulo, SI, monto, 0 );
}

int PanelBrowse::BROWSE_MYSQL( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario,
                         char *titulo, double monto, unsigned short cod_medio )
{
    return BROWSE_MYSQL( x1, y1, x2, y2, l_cadena, f_usuario, titulo, SI, monto, cod_medio );
}


int PanelBrowse::BROWSE_MYSQL( int x1, int y1, int x2, int y2, int l_cadena, f_usu f_usuario,
                              char *titulo, int buscar, double monto, unsigned short cod_medio )
{
	int rta = 0, h, ok, pag;
	char cadena[61] = {""};
	long reg;
    //Cantidad de registros en la busqueda
    long total_regs = 0;
	bool enable = false;

    browse.seguir = 1;
	browse.mostrar = 2;
	browse.linea = 0;
	browse.pos = 0;
	browse.posicionar = 0;
	browse.index = this->id;
	browse.alto = y2 - y1 + 1;
    cadena[l_cadena] = 0;

	//__browse->init( id, fieldsDefinition, SelectedField, ( x2-x1 >= 200? x2-x1 : 200 ) );
    __browse->init( id, fieldsDefinition, SelectedField, (50 ),  browse.alto);
#ifdef INVEL_L
    MutexGuiEnter();
#endif
	__browse->SetColLabelSize(0);
    __browse->SetRowLabelSize(0);
    __browse->EnableEditing(false);
    __browse->EnableGridLines(false);
    __browse->deleteInput();
#ifdef INVEL_L
    MutexGuiLeave();
#endif

    f_usuario( _LIMIT, "", y2, &browse, 0.0, 0 );
    f_usuario( _BUSCAR, "", 0, &browse, 0.0, 0 );

    TECLADO_ALFABETICO = 1;
	while( browse.seguir ) {
        if( browse.posicionar ) {
		    _ATRIBUTO = 112;
            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
			__browse->BeginBatch();
			f_usuario( _BUSCAR, cadena, 0, &browse, monto, cod_medio );
            browse.linea = 0;
			browse.posicionar = 0;
            browse.mostrar = 2;
            if( browse.alto > 0 ) {
				#ifdef INVEL_W
					//MutexGuiEnter();
				#endif
			    __browse->deleteRows( browse.alto );
				#ifdef INVEL_W
					//MutexGuiLeave();
				#endif
            }
			__browse->EndBatch();
            #ifdef INVEL_L
            MutexGuiLeave();
            #endif
		}

        if( browse.mostrar ) {
		    //*------------------- Refresca la pantalla -------------------
			if( browse.mostrar == 2 ) {
			    char first = 1;
				_ATRIBUTO = 120;
                //Almacenamos el registro que esta seleccionado
				reg = GET_POSITION_REG(  );
                pag = GET_PAGINA_REG( );
// comento poir caidas 2nucle                #ifdef INVEL_L
				if(ACELERACION_PANTALLAS == SI) {
				//esta solucion quizas resuelva el problema de los mutex que tienen
				//ir si o si pero retrasan un poco la velocidad del tps
					panBrowse->mYield(); 
					SLEEP(10);
				}else
                MutexGuiEnter();
//                #endif
				//MutexGuiEnter(); //parece buenoooooo
                //Se retrocede lo necesario desde el registro seleccionado para poder rellenar el browse
                f_usuario( _SKIP, cadena, -browse.linea, &browse, monto, cod_medio );
                ok = 0;
                __browse->setEnable( id, false );
                __browse->BeginBatch();
				for( h = 0; h < browse.alto; h++ ) {
                    f_usuario( _VACIO, cadena, h, &browse, monto, cod_medio );
                    //Cobros Marginales Online: Para el caso en donde el ticket contiene un
                    //Cobros Marginales Online( por restriccion no se puede tener más de uno por ticket )
                    //Si el 1er reg a evaluar es una Cobros Marg. Online lo muestra igual porque
                    //esta ok = 0 y no valida la resp del SKIP quien condiciona la restricción
                    //mencionada anteriormente.
                    if( !mostrar_cobro ) {
                        ok = 1;
                        mostrar_cobro = 1;
                    }
					if( !ok ) {
                        f_usuario( _MOSTRAR, cadena, h, &browse, monto, cod_medio );
						//MutexGuiEnter();
						setRow(  );
						//MutexGuiLeave();
                    }
                    ok = (!ok) ? f_usuario( _SKIP, cadena, 1, &browse, monto, cod_medio ) : ok;
				}
				//MutexGuiLeave();//parece buenoooooo
				//Tomamos nuevamente el registro seleccionado
                GET_DIRECT_REG( reg, pag ); //forray
				__browse->selRow( id, browse.linea );
				__browse->EndBatch();
 //               #ifdef INVEL_L
				if(ACELERACION_PANTALLAS == NO)
                	MutexGuiLeave();
//                #endif
				if(enable == false){
			    __browse->setEnable( id, true );
					enable = true;
            }
                //__browse->ForceRefresh();
				
            }
            else if( browse.mostrar == 3 ) {
				__browse->BeginBatch();
            f_usuario( _MOSTRAR, cadena, browse.linea, &browse, monto, cod_medio );
			setRow(  );
				__browse->EndBatch();
		}
        browse.mostrar = 0;
    		BORRAR_MENSAJE(  );
		}
__browse;
        //////////////////////// DEBUG ///////////////////////////
#ifdef _DEBUG
        //Se verifica la equivalencia entre lo seleccionado en pantalla y lo que esta en la estructura
        wxString test;
        test = __browse->GetCellValue(__browse->GetSelectedRows()[0], 0);
        //Definir el siguiente 'define', segun el caso que se pruebe
#ifdef _DEBUG_BROWSE_ARTICULO
        //Para tabla articulos:
        assert( testa.IsSameAs( articulo.cod_barra ) );
#endif
#endif
        //////////////////////////////////////////////////////////

        //*-------------------------- Ingresa teclas --------------------*
		browse.tecla = GETCH(  );
		#ifdef INVEL_L
        MutexGuiEnter();
        #endif
        f_usuario( _TECLA, cadena, browse.tecla, &browse, monto, cod_medio );
	    #ifdef INVEL_L
        MutexGuiLeave();
        #endif

        switch ( browse.tecla ) {
        case -72:					  //*-------- FLECHA ARRIBA ---------*
			__browse->BeginBatch();
            MutexGuiEnter();
            if( f_usuario( _SKIP, cadena, -1, &browse, monto, cod_medio ) == 0 ) {
                browse.mostrar = 1;
			    if( browse.linea > 0 ) {
			        browse.linea--;
				    __browse->selRow( id, browse.linea );
                }
                else {
					//MutexGuiEnter(); //agreGado pero hace lento el browse
					__browse->scroll( id, 1 );
			        f_usuario( _MOSTRAR, cadena, 0, &browse, monto, cod_medio );
                    insertRow( browse.linea );
					//setRow(  ); //agregado esto evita que se cuelgue pero agrega barras de desplazamiento			
				    __browse->selRow( id, browse.linea );
		        }
		    }
            MutexGuiLeave();
			__browse->EndBatch();
		    break;
        case -73:					  //*-------- /PAGINA ARRIBA ---------*
            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
			__browse->BeginBatch();
			MutexGuiEnter();
            {
                //Calculamos el desplazamiento maximo del browse
                long reg_actual = GET_POSITION_REG(  );
                long reg_top_browse = reg_actual - browse.linea;
                long max_desplazamiento = 1 - reg_top_browse;

                if( reg_top_browse == 1 ) {
                    //Estamos en el tope. Solo se sube la barra de seleccion
                    //Skip hasta el primero del browse.
                    f_usuario( _SKIP, cadena, -browse.linea, &browse, monto, cod_medio );
                    browse.linea = 0;
                    browse.mostrar = 1;
                }
                else if( f_usuario( _SKIP, cadena, max( -browse.alto, max_desplazamiento ), &browse, monto, cod_medio ) == 0) {
    				//browse.linea = 0;
                    __browse->deleteRows(0);
		    	    browse.mostrar = 2;
                }
			}
			MutexGuiLeave();
			__browse->EndBatch();
            #ifdef INVEL_L
            MutexGuiLeave();
            #endif
            break;
		case -80:					  //*-------- FLECHA ABAJO ---------*
            //#ifdef INVEL_L
			__browse->BeginBatch();
            MutexGuiEnter();
            //#endif
            if( f_usuario( _SKIP, cadena, 1, &browse, monto, cod_medio ) == 0 ) {
    			browse.mostrar = 1;
				if( browse.linea < browse.alto - 1 ) {
				    browse.linea++;
					__browse->selRow( id, browse.linea );
                } else {
                    f_usuario( _MOSTRAR, cadena, browse.alto - 1, &browse, monto, cod_medio );
					__browse->scroll( id, -1 );
					setRow(  );
					__browse->selRow( id, browse.linea );
				}
			}
            //#ifdef INVEL_L
            MutexGuiLeave();
            //#endif
			__browse->EndBatch();
			break;
		case -81:		  			  //*-------- PAGINA ABAJO ---------*
            //Se necesita actualizar la cantidad de registros en la busqueda ya que
            //cambia cada vez que se agrega un caracter al input de busqueda.
			__browse->BeginBatch();
			MutexGuiEnter();
            total_regs = RECCOUNT2( NO, NULL );
            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
            //Solo se avanza pagina si hay mas registros para ver.
            if ( total_regs > browse.alto ) {
                //Calculamos el desplazamiento maximo del browse
                long reg_actual = GET_POSITION_REG(  );
                long reg_bottom_browse = browse.alto - browse.linea - 1 + reg_actual;
                long max_desplazamiento = total_regs - reg_bottom_browse;

                if( reg_bottom_browse == total_regs ) {
                    //Estamos al fondo. Solo bajamos la barra de seleccion.
                    //Skip hasta el ultimo del browse (en este caso el ultimo de la busqueda tambien)
                    f_usuario( _SKIP, cadena, browse.alto - browse.linea - 1, &browse, monto, cod_medio );
                    browse.linea = browse.alto - 1;
                    __browse->selRow( id, browse.linea );
                    browse.mostrar = 1;
                }
                else if( f_usuario( _SKIP, cadena, min( browse.alto, max_desplazamiento ), &browse, monto, cod_medio ) == 0 ) {
                    if( browse.linea > RECCOUNT2( NO, NULL ) ) {
						browse.linea = RECCOUNT2( NO, NULL ) - 1;
                    }
                    if( browse.alto > 0 ) {
                        __browse->deleteRows( browse.alto );
                    }
    			    browse.mostrar = 2;
                }
            }
			MutexGuiLeave();
			__browse->EndBatch();
            #ifdef INVEL_L
            MutexGuiLeave();
            #endif
			break;
		case 27:					  //*-------- ESC ---------*
			browse.seguir = 0;
			break;

		case 13:					  //*-------- ENTER ---------*
            if( rta = f_usuario( _ENTER, cadena, 0, &browse, monto, cod_medio ) ) {
			    browse.seguir = 0;
				//rta = 1;
			}
			break;
		case 43:					  //*-------- " + " ---------*
            rta = 1;	//mas simple que el ENTER, para no complicar los browsers existentes
		break;
		case 8:						  //*------ BACKSPACE -------*
			if( browse.pos > 0 ) {
			    browse.pos--;
				cadena[browse.pos] = 0;/////' ';
				browse.posicionar = 1;
                MOSTRAR_INPUT_CADENA( cadena, browse.pos );
				//__browse->scroll( id, 100 );
			}
			//--Fabi-04/04/99-Tecla total en cobros varios
		case 700:					  // *------- TOTAL -----------*
            if( f_usuario( _SPACE_, cadena, 0, &browse, monto, cod_medio ) ) {
				browse.seguir = 0;
			}
			break;
      case -999:					  //*--- Sale por espera ---*
            f_usuario( _ESPECIAL, cadena, 0, &browse, monto, cod_medio );
			break;
      default:					  //*---------- caracteres de acercamiento ----------------*
            /*que sean distintas de: inicio, fin, flecha derecha, flecha izquierda, tecla '+'*/
            if( buscar && browse.tecla != -75 && browse.tecla != -74 && browse.tecla != -76 && browse.tecla != -77&& browse.tecla != 43) {
                /*int press = 0;
                do {
                    if ( press ) {
                        browse.tecla = GETCH();
                    }*/
                if( browse.pos < l_cadena ) {
                    if( f_usuario( _VALIDAR, cadena, browse.tecla, &browse, monto, cod_medio ) ) {
                        cadena[browse.pos] = browse.tecla;
                        browse.pos++;
                        browse.posicionar = 1;
                        MOSTRAR_INPUT_CADENA( cadena, browse.pos );
                    }
                }
                //}while ( press = KBHIT() );
            }
            break;
		}
	}
	__browse->close( id );
    TECLADO_ALFABETICO = 0;
	f_usuario( _LIMIT, "", 0, &browse, 0.0, 0 );
    return ( rta );
}




void PanelBrowse::setRow()
{
    __browse->setRow( id, rowString );
}

void PanelBrowse::setRow2()
{
    __browse->setRow( id, rowString, 0 );
}


void PanelBrowse::deleteRows( int rows )
{
    if( rows > 0 ) {
        __browse->deleteRows( rows );
    }
}


void PanelBrowse::insertRow( int linea )
{
    __browse->insertRow( id, linea, rowString );
}


void PanelBrowse::setFieldValue( int field, double value )
{
    char str[1024];
    sprintf( str, "%f", value );
    rowString[field] = str;

    //( *pRow )[field] <<= value;
    //row[field]<<= value;
}


void PanelBrowse::setFieldValue( int field, float value )
{
    char str[1024];
    sprintf( str, "%f", value );
    rowString[field] = str;

    //( *pRow )[field] <<= value;
    //row[field]<<= value;
}


void PanelBrowse::setFieldValue( int field, int value )
{
}


void PanelBrowse::setFieldValue( int field, char *value )
{
    char str[1024];
    sprintf( str, "%s", value );
    rowString[field] = str;
}


void PanelBrowse::setFieldValueStringList( int field, char *value )
{
    char str[1024];
    sprintf( str, "%s", value );
    rowString[field] = str;
}


PanelBrowse::PanelBrowse( struct _dat *dat, int n ) : browseAny( false )
{
    int j;
    /*
    if( browseAny )
        pRow = new Invel::ListAny( n );
    else
        pRow = 0;
    */
    numberOfFields = dat->nFields;
    this->SelectedField = dat->selectedField;

    for( j = 0;j < dat->nFields;j++ ) {
        browse_field def;
        fieldsDefinition.push_back( def );
        rowString.push_back( "" );

        fieldsDefinition[j].Name = ( const char* )dat->FieldDescription[j].colName;
        fieldsDefinition[j].Width = dat->FieldDescription[j].width;
        fieldsDefinition[j].Alignment = dat->FieldDescription[j].alignment;
        fieldsDefinition[j].Type = dat->FieldDescription[j].type;
    }
    for( j = 0;j < MAXBROWSE;j++ ) {
        if( idArray[j] == 0 ) {
            id = 1;
            idArray[id] = 1;
            _brow[id] = this;
            break;
        }
    }
}


PanelBrowse::~PanelBrowse()
{
    idArray[id] = 0;
    _brow[id] = 0;
    /*
        if( browseAny )
            delete    pRow;
    */
}


/*void PanelBrowse::initBrowse()
{
    __browse->init( id, fieldsDefinition, SelectedField, width );
}*/


void _setFieldValueInt( int index, int field, int value )
{
    _brow[index]->setFieldValue( field, value );
}


void _setFieldValueDouble( int index, int field, double value )
{
    _brow[index]->setFieldValue( field, value );
}


void _setFieldValueFloat( int index, int field, float value )
{
    _brow[index]->setFieldValue( field, value );
}


void _setFieldValuepChar( int index, int field, char *value )
{
    if( _brow[index]->browseAny ) {
        _brow[index]->setFieldValue( field, value );
    }
    else {
        _brow[index]->setFieldValueStringList( field, value );
    }
}


extern "C"
{
void setFieldValueInt( int index, int field, int value )
{
_setFieldValueInt( index, field, value );
}
void setFieldValueFloat( int index, int field, float value )
{
_setFieldValueFloat( index, field, value );
}
void setFieldValueDouble( int index, int field, double value )
{
_setFieldValueDouble( index, field, value );
}
void setFieldValuepChar( int index, int field, char *value )
{
_setFieldValuepChar( index, field, value );
}

/****************************************************************************/
/* Consulta de la tabla de art?ulos por cdigo de barras.                  */
/****************************************************************************/
int Consulta_articulo_codbarra( f_usu f_usuario )
{
    int rta = 0,d, longitud = 0;
    int j;
    struct _dat dat;
    memset( &dat, 0, sizeof( struct _dat ) );
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseArticuloCodbarra" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
	//panBrowse->Center();
	panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
	GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    dat.nFields = 6;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod barra" );
    strcpy( dat.FieldDescription[1].colName, "cod interno" );
    strcpy( dat.FieldDescription[2].colName, "nombre" );
    strcpy( dat.FieldDescription[3].colName, "unidad" );
    strcpy( dat.FieldDescription[4].colName, "marca" );
    strcpy( dat.FieldDescription[5].colName, "precio" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    dat.FieldDescription[2].alignment = ALIGN_LEFT;
    dat.FieldDescription[3].alignment = ALIGN_CENTER;
    dat.FieldDescription[4].alignment = ALIGN_LEFT;
    dat.FieldDescription[5].alignment = ALIGN_RIGHT;
    //15;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ART_CODBARRA, 0 );
    //8;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ART_CODBARRA, 1 );
    //30;
    dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ART_CODBARRA, 2 );
    //3;
    dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ART_CODBARRA, 3 );
    //8;
    dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ART_CODBARRA, 4 );
    //10;
    dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ART_CODBARRA, 5 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );

    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_ART_CODBARRA,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_ART_CODBARRA,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_ART_CODBARRA,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_ART_CODBARRA, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_ART_CODBARRA, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_ART_CODBARRA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_ART_CODBARRA,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_ART_CODBARRA,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_ART_CODBARRA,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_ART_CODBARRA,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_ART_CODBARRA,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_ART_CODBARRA, _INDICE_GIF ) );;
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
    //panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_ART_CODBARRA,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
	}
	longitud = L_CODIGO;
    if( PRESENTACIONES_EN_DUN14 && L_CODIGO == 14 ) {
		longitud = L_CODIGO - 1;
    }

    rta = b.BROWSE_MYSQL( 2, 1, 650, 14, longitud/*L_CODIGO*/, f_usuario, "CONSULTA CODIGO DE BARRAS", 0.0, 0 );
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = 1;//idPanelVentas;
	}
	return rta;
}

/****************************************************************************/
/* Consulta de la tabla de art?ulos por descripcion-codinterno-marca       */
/****************************************************************************/
int Consulta_articulo_descrip( f_usu f_usuario, int col )
{
    int rta = 0, j,d, formato = 0, _l_codigo = L_CODIGO;
    struct _dat dat;

    memset( &dat, 0, sizeof( struct _dat ) );
    dat.selectedField = 3;
    switch( FORMATO_CONSULTA ) {
        case 0:
            switch( col ) {
                case CONSULA_DESCRIPCION:
                    dat.selectedField = 3;
                    if(Config->get_touch_screen()){
                        panBrowse->SetSize(725,645);//se agranda la pantalla para q aparezca el teclado
                    }
                    break;
                case CONSULTA_COD_INTERNO:
                    dat.selectedField = 1;
                    break;
                case CONSULTA_MARCA:
                    dat.selectedField = 2;
                    if(Config->get_touch_screen()){
                        panBrowse->SetSize(725,645);//se agranda la pantalla para q aparezca el teclado
                    }
                    break;
            }
            dat.nFields = 6;
            strcpy( dat.FieldDescription[0].colName, "cod barra" );
            strcpy( dat.FieldDescription[1].colName, "cod interno" );
            strcpy( dat.FieldDescription[2].colName, "marca" );
            strcpy( dat.FieldDescription[3].colName, "nombre" );
            strcpy( dat.FieldDescription[4].colName, "unidad" );
            strcpy( dat.FieldDescription[5].colName, "precio" );
            dat.FieldDescription[0].alignment = ALIGN_LEFT;
            dat.FieldDescription[1].alignment = ALIGN_LEFT;
            dat.FieldDescription[2].alignment = ALIGN_LEFT;
            dat.FieldDescription[3].alignment = ALIGN_LEFT;
            dat.FieldDescription[4].alignment = ALIGN_CENTER;
            dat.FieldDescription[5].alignment = ALIGN_RIGHT;
            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
			browseWin = properties->GetWindows()->GetWindowByName( "browseConsultaDescrip1" );
			panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
			//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
           
            dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC1, 0 );

            dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC1, 1 );
            dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC1, 2 );
            dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC1, 3 );
            dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC1, 4 );

            dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC1, 5 );
            #ifdef INVEL_L
            MutexGuiLeave();
            #endif

            formato = _CONSULTA_ART_DESCRIPC1;
            break;
        case 1:
            switch( col ) {
                case CONSULA_DESCRIPCION:
                    dat.selectedField = 2;
                    break;
                case CONSULTA_COD_INTERNO:
                    dat.selectedField = 1;
                    break;
                case CONSULTA_MARCA:
                    dat.selectedField = 2;
                    break;
            }
            dat.nFields = 5;

            strcpy( dat.FieldDescription[0].colName, "cod barra" );
            strcpy( dat.FieldDescription[1].colName, "cod interno" );
            strcpy( dat.FieldDescription[2].colName, "nombre" );
            strcpy( dat.FieldDescription[3].colName, "precio" );
            strcpy( dat.FieldDescription[4].colName, "prec sin iva" );
            dat.FieldDescription[0].alignment = ALIGN_LEFT;
            dat.FieldDescription[1].alignment = ALIGN_LEFT;
            dat.FieldDescription[2].alignment = ALIGN_LEFT;
            dat.FieldDescription[3].alignment = ALIGN_RIGHT;
            dat.FieldDescription[4].alignment = ALIGN_RIGHT;

            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
			browseWin = properties->GetWindows()->GetWindowByName( "browseConsultaDescrip2" );
			panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
			//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);

            //15;
            dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC2, 0 );
            //8;
            dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC2, 1 );
            //30;
            dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC2, 2 );
            //10;
            dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC2, 3 );
            //10;
            dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC2, 4 );

            #ifdef INVEL_L
            MutexGuiLeave();
            #endif

            formato = _CONSULTA_ART_DESCRIPC2;
            break;
        case 2:
            dat.nFields = 6;
            switch( col ) {
                case CONSULA_DESCRIPCION:
                    dat.selectedField = 2;
                    break;
                case CONSULTA_COD_INTERNO:
                    dat.selectedField = 1;
                    break;
                case CONSULTA_MARCA:
                    dat.selectedField = 2;
                    break;
            }
            strcpy( dat.FieldDescription[0].colName, "cod barra" );
            strcpy( dat.FieldDescription[1].colName, "cod interno" );
            strcpy( dat.FieldDescription[2].colName, "nombre" );
            strcpy( dat.FieldDescription[3].colName, "contenido" );
            strcpy( dat.FieldDescription[4].colName, "unidad" );
            strcpy( dat.FieldDescription[5].colName, "precio" );
            dat.FieldDescription[0].alignment = ALIGN_LEFT;
            dat.FieldDescription[1].alignment = ALIGN_LEFT;
            dat.FieldDescription[2].alignment = ALIGN_LEFT;
            dat.FieldDescription[3].alignment = ALIGN_LEFT;
            dat.FieldDescription[4].alignment = ALIGN_CENTER;
            dat.FieldDescription[5].alignment = ALIGN_RIGHT;

            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
			browseWin = properties->GetWindows()->GetWindowByName( "browseConsultaDescrip3" );
			panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
			//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
            //15;
            dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC3, 0 );
            //8;
            dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC3, 1 );
            //30;
            dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC3, 2 );
            //7;
            dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC3, 3 );
            //3;
            dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC3, 4 );
            //10;
            dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC3, 5 );

            #ifdef INVEL_L
            MutexGuiLeave();
            #endif

            formato = _CONSULTA_ART_DESCRIPC3;
            break;
        case 3:
            dat.nFields = 7;
            switch( col ) {
                case CONSULA_DESCRIPCION:
                    dat.selectedField = 3;
                    break;
                case CONSULTA_COD_INTERNO:
                    dat.selectedField = 1;
                    break;
                case CONSULTA_MARCA:
                    dat.selectedField = 2;
                    break;
            }
            strcpy( dat.FieldDescription[0].colName, "cod barra" );
            strcpy( dat.FieldDescription[1].colName, "cod interno" );
            strcpy( dat.FieldDescription[2].colName, "marca" );
            strcpy( dat.FieldDescription[3].colName, "nombre" );
            strcpy( dat.FieldDescription[4].colName, "contenido" );
            strcpy( dat.FieldDescription[5].colName, "unidad" );
            strcpy( dat.FieldDescription[6].colName, "precio" );
            dat.FieldDescription[0].alignment = ALIGN_LEFT;
            dat.FieldDescription[1].alignment = ALIGN_LEFT;
            dat.FieldDescription[2].alignment = ALIGN_LEFT;
            dat.FieldDescription[3].alignment = ALIGN_LEFT;
            dat.FieldDescription[4].alignment = ALIGN_CENTER;
            dat.FieldDescription[5].alignment = ALIGN_CENTER;
            dat.FieldDescription[6].alignment = ALIGN_RIGHT;

            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
			browseWin = properties->GetWindows()->GetWindowByName( "browseConsultaDescrip3" );
			panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
			//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);

            //15;
            dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC4, 0 );
            //8;
            dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC4, 1 );
            //8;
            dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC4, 2 );
            //30;
            dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC4, 3 );
            //7;
            dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC4, 4 );
            //3;
            dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC4, 5 );
            //10;
            dat.FieldDescription[6].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC4, 6 );

            #ifdef INVEL_L
            MutexGuiLeave();
            #endif

            formato = _CONSULTA_ART_DESCRIPC4;
            break;
        case 4:
            dat.nFields = 6;
            switch( col ) {
                case CONSULA_DESCRIPCION:
                    dat.selectedField = 3;
                    break;
                case CONSULTA_COD_INTERNO:
                    dat.selectedField = 1;
                    break;
                case CONSULTA_MARCA:
                    dat.selectedField = 2;
                    break;
            }
            strcpy( dat.FieldDescription[0].colName, "cod barra" );
            strcpy( dat.FieldDescription[1].colName, "cod interno" );
            strcpy( dat.FieldDescription[2].colName, "marca" );
            strcpy( dat.FieldDescription[3].colName, "nombre" );
            strcpy( dat.FieldDescription[4].colName, "unidad" );
            strcpy( dat.FieldDescription[5].colName, "precio" );
            dat.FieldDescription[0].alignment = ALIGN_LEFT;
            dat.FieldDescription[1].alignment = ALIGN_LEFT;
            dat.FieldDescription[2].alignment = ALIGN_LEFT;
            dat.FieldDescription[3].alignment = ALIGN_LEFT;
            dat.FieldDescription[4].alignment = ALIGN_CENTER;
            dat.FieldDescription[5].alignment = ALIGN_RIGHT;

            #ifdef INVEL_L
            MutexGuiEnter();
            #endif
            //15;
			browseWin = properties->GetWindows()->GetWindowByName( "browseConsultaDescrip5" );
			panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
			//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
            dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC5, 0 );
            //8;
            dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC5, 1 );
            //8;
            dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC5, 2 );
            //30;
            dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC5, 3 );
            //3;
            dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC5, 4 );
            //10;
            dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
            _CONSULTA_ART_DESCRIPC5, 5 );

            #ifdef INVEL_L
            MutexGuiLeave();
            #endif

            formato = _CONSULTA_ART_DESCRIPC5;
            break;
    }
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );

    #ifdef INVEL_L
    MutexGuiEnter();
    #endif

    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( formato,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( formato,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( formato,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( formato, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( formato, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(formato,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(formato,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( formato, 0 ),
                                              panBrowse->getSizePanelBrowse( formato, 1 ),
                                              panBrowse->getSizePanelBrowse( formato, 2 ),
                                              panBrowse->getSizePanelBrowse( formato, 3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( formato, _INDICE_GIF ) );

	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}

    //panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( formato, _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( formato, _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( formato, _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( formato, _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( formato, _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( formato, _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( formato, _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( formato,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( formato, _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( formato, _INDICE_TEXT_COL ) );

    panBrowse->input_browse_del_panel->SetVisible();

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
	}
    /* Para las descripciones se requiere otro largo de la cadena */
    if( CONSULA_DESCRIPCION == col ) {
        _l_codigo = L_CADENA;
    }
    rta = b.BROWSE_MYSQL( 2, 1, 650, 14, _l_codigo, f_usuario, "CONSULTA DESCRIPCION", 0.0, 0 );
    if(Config->get_touch_screen()){
        /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = 1;//idPanelVentas;
        panBrowse->SetSize(725,350);//vuelve la pantalla al tamaño anterior por si antes fue modificado
    }
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_elegir_motivo( f_usu f_usuario )
{
    int rta = 0;
    int j,d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseMotivo" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_CENTER;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_MOTIVO,
                                                                                  0 );
    //30;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_MOTIVO,
                                                                                  1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}

	PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_MOTIVO,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_MOTIVO,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_MOTIVO,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_MOTIVO, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_MOTIVO, _INDICE_ROW_SIZE_PAN ) );
    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_MOTIVO,_INDICE_TEXT_COL));
    //panBrowbelbrowse.__setPosition(panBrowse->getCharTit(_CONSULTA_MOTIVO,_INDICE_POSI_TEXT_COL));se->la
    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_MOTIVO, 0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_MOTIVO, 1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_MOTIVO, 2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_MOTIVO,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_MOTIVO, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_MOTIVO,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_MOTIVO, _INDICE_TEXT_COL ) );

    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
	}
    rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, "MOTIVO", NO, 0.0, 0 );
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = 1;//panel ventas
	}
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_elegir_mutuales( f_usu f_usuario, double importe, unsigned short cod_medio )
{
    int rta = 0;
    int j,d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseMutuales" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 3;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
	strcpy( dat.FieldDescription[2].colName, "cotizacion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
	dat.FieldDescription[2].alignment = ALIGN_LEFT;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_MUTUALES, 0 );
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_MUTUALES, 1 );
	dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_MUTUALES, 2 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
	PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_MUTUALES,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_MUTUALES,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_MUTUALES,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
	/*panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_MUTUALES,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            2 );*/
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_MUTUALES, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_MUTUALES, _INDICE_ROW_SIZE_PAN ) );

	//panBrowse->SetSize( 326 , 350  );
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	panBrowse->SendSizeEvent();
    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_MUTUALES,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_MUTUALES,_INDICE_POSI_TEXT_COL));
    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_MUTUALES,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_MUTUALES,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_MUTUALES,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_MUTUALES,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_MUTUALES, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                                   _INDICE_ALIN ) );
    //panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_MUTUALES,
     //                                                             _INDICE_TEXT ) );


    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_MUTUALES,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
	//panBrowse->input_browse_del_panel->SetNotVisible();

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
    }
    rta = b.BROWSE_MYSQL( 28, 1, 360, panBrowse->browse_del_panel->getNumFilas(), L_CADENA, f_usuario, "MUTUAL", /*(int)*/importe, cod_medio );
	panBrowse->SetSize( 700 , 350  );
	panBrowse->SendSizeEvent();
	if( Config->get_touch_screen() ){
		panBrowse->Move( 0, 0 );
	} else {
		//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	}

	return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_pide_cuotas_mutual( f_usu f_usuario )
{
    int rta = 0,d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browsePideCuoMut" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_PIDE_CUOMUT, 0 );
    //20;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_PIDE_CUOMUT, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}

    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_CUOMUT,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_CUOMUT,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_CUOMUT,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_CUOMUT, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_CUOMUT, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_PIDE_CUOMUT,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_PIDE_CUOMUT,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_CUOMUT,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_CUOMUT,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_CUOMUT,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_CUOMUT,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_CUOMUT, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_PIDE_CUOMUT,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, "CUOTAS", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_pide_perfil( f_usu f_usuario )
{
    int rta = 0,d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browsePerfil" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_PIDE_PERFIL, 0 );
    //20;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_PIDE_PERFIL, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_PERFIL,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_PERFIL,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_PERFIL,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_PERFIL, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_PERFIL, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_PIDE_PERFIL,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_PIDE_PERFIL,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_PERFIL,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_PERFIL,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_PERFIL,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_PERFIL,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_PERFIL, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_PIDE_PERFIL,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
	if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
    }
    rta = b.BROWSE_MYSQL( 28, 1, 360, 14, L_CADENA, f_usuario, "PERFILES", 0.0, 0 );
	if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = 1;//idPanelVentas;
    }
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_pide_tarjeta( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    memset( &dat, 0, sizeof( struct _dat ) );
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browsePideTarjeta" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_PIDE_TARJETA, 0 );
    //20;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_PIDE_TARJETA, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_TARJETA,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_TARJETA,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_TARJETA,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_TARJETA, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_TARJETA, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_PIDE_TARJETA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_PIDE_TARJETA,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_TARJETA,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_TARJETA,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_TARJETA,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PIDE_TARJETA,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_PIDE_TARJETA, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_PIDE_TARJETA,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 28, 1, 360, panBrowse->browse_del_panel->getNumFilas(), L_CADENA, f_usuario, "TARJETAS ESPECIALES", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta de la tabla de clientes. En D.O.S. solo se puede mostrar nombre */
/* y codigo de cliente.                                                     */
/****************************************************************************/
int Consulta_cliente( f_usu f_usuario )
{
    int rta = 0, j,d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseClientes" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 8;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "Apellido y Nombre / Razon Social" );
    strcpy( dat.FieldDescription[1].colName, "Nro Socio" );
    strcpy( dat.FieldDescription[2].colName, "Cuit/Cuil" );
    strcpy( dat.FieldDescription[3].colName, "Compania" );
    strcpy( dat.FieldDescription[4].colName, "Prepaga" );
    strcpy( dat.FieldDescription[5].colName, "Tipo" );
    strcpy( dat.FieldDescription[6].colName, "Direccion" );
    strcpy( dat.FieldDescription[7].colName, "Localidad" );

    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    dat.FieldDescription[2].alignment = ALIGN_LEFT;
    dat.FieldDescription[3].alignment = ALIGN_LEFT;
    dat.FieldDescription[4].alignment = ALIGN_LEFT;
    dat.FieldDescription[5].alignment = ALIGN_LEFT;
    dat.FieldDescription[6].alignment = ALIGN_LEFT;
    dat.FieldDescription[7].alignment = ALIGN_CENTER;

    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 0 );
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 1 );
    dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 2 );
    dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 3 );
    dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 4 );
    dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 5 );
    dat.FieldDescription[6].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 6 );
    dat.FieldDescription[7].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 7 );

    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    if(Config->get_touch_screen()){
		panBrowse->SetSize(725,645);//se agranda la pantalla para q aparezca el teclado
    }
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);

    PanelBrowse b( &dat, dat.nFields );

    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_CLIENTES,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_CLIENTES,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
    }
	rta = b.BROWSE_MYSQL(20 - ancho_panel / 2, 1, 61 - ancho_panel / 2, 14, L_CADENA, f_usuario,
                    "CLIENTES",0.0, 0 );
    panBrowse->DesActivate();
	if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = 1;//idPanelVentas;
        panBrowse->SetSize(725,350);//vuelve la pantalla al tamaño anterior por si antes fue modificado
    }
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_elegir_funcion( f_usu f_usuario, int largo )
{
    int rta = 0,d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "orden" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
	browseWin = properties->GetWindows()->GetWindowByName( "browseFunciones" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_FUNCION,
                                                                                  0 );
    //41;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_FUNCION,
                                                                                  1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_FUNCION,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_FUNCION,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_FUNCION,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_FUNCION, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_FUNCION, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_FUNCION,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_FUNCION,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_FUNCION, 0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_FUNCION, 1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_FUNCION, 2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_FUNCION,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_FUNCION, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_FUNCION,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_FUNCION, _INDICE_TEXT_COL ) );

    //panBrowse->input_browse_del_panel->SetNotVisible();
	panBrowse->input_browse_del_panel->SetVisible();

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseTCompra;
    }
	if( largo == 0 ) {
		largo = L_CADENA;
	}
    //rta = b.BROWSE( 28, 1, 360, 18, L_CADENA, f_usuario, " FUNCION ", 0.0, 0 );
    rta = b.BROWSE_MYSQL( 28, 1, 360, 14, largo, f_usuario, " FUNCION ", 0.0, 0 );
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = 1;//idPanelVentas;
    }
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_intituciones( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseInstituciones" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "orden" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_INSTITUCIONES, 0 );
    //31;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_INSTITUCIONES, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_INSTITUCIONES,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_INSTITUCIONES,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_INSTITUCIONES,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_INSTITUCIONES, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_INSTITUCIONES, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_INSTITUCIONES,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_INSTITUCIONES,_INDICE_POSI_TEXT_COL));

    (
    panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_INSTITUCIONES,
                                                                           0 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_INSTITUCIONES,
                                                                           1 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_INSTITUCIONES,
                                                                           2 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_INSTITUCIONES,
                                                                           3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_INSTITUCIONES, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_INSTITUCIONES,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEl_L
    MutexGuiLeave();
    #endif
	rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, "INST. BENEFICAS", 0.0, 0 );
    //rta = b.BROWSE( 28, 1, 360, 18, L_CADENA, f_usuario, "INST. BENEFICAS", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_t_compra( f_usu f_usuario, unsigned short cod_medio )
{
    int rta = 0;
    int j,d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseTCompra" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_T_COMPRA, 0 );
    //12;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_T_COMPRA, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}

    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_T_COMPRA,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_T_COMPRA,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_T_COMPRA,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_T_COMPRA, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_T_COMPRA, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_T_COMPRA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_T_COMPRA,_INDICE_POSI_TEXT_COL));

	//panBrowse->SetSize( 326 , 350  );
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	panBrowse->SendSizeEvent();

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_T_COMPRA,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_T_COMPRA,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_T_COMPRA,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_T_COMPRA,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_T_COMPRA, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                                   _INDICE_ALIN ) );
    //panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
    //                                                              _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_T_COMPRA,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
	//panBrowse->input_browse_del_panel->SetNotVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseTCompra;
    }
    rta = b.BROWSE_MYSQL( 28, 1, 360, panBrowse->browse_del_panel->getNumFilas(), L_CADENA, f_usuario,
                    "INSTITUCIONES", 0.0, cod_medio );

	panBrowse->SetSize( 700 , 350  );
	panBrowse->SendSizeEvent();
	if( Config->get_touch_screen() ){
		panBrowse->Move( 0, 0 );
	} else {
		//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	}

	return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_cuotas_tarjeta( f_usu f_usuario, unsigned short cod_medio )
{
    int rta = 0;
    int j = 0, d = 0;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseCuotasTar" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
	//panBrowse->Center();
	panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
		GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
		/*
	wxPoint( 
		GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
		GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2))
		),*/







    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 3;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    strcpy( dat.FieldDescription[2].colName, "monto" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    dat.FieldDescription[2].alignment = ALIGN_LEFT;

    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CUOTAS_TARJ, 0 );
    //21;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CUOTAS_TARJ, 1 );
    //21;
    dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CUOTAS_TARJ, 2 );

    for( j = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
		d=d+dat.FieldDescription[j].width;
    }
	panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
	//panBrowse->SendSizeEvent();
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_CUOTAS_TARJ,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CUOTAS_TARJ,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CUOTAS_TARJ,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_CUOTAS_TARJ, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_CUOTAS_TARJ, _INDICE_ROW_SIZE_PAN ) );
    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_CUOTAS_TARJ,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_CUOTAS_TARJ,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_CUOTAS_TARJ,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CUOTAS_TARJ,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CUOTAS_TARJ,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CUOTAS_TARJ,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_CUOTAS_TARJ, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
	//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_CUOTAS_TARJ,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    //panBrowse->input_browse_del_panel->SetNotVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
	
	UNGETCH( 0 );
    rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, "CUOTAS", 0.0, cod_medio );
	//panBrowse->SetSize( 700 , 350  );
	panBrowse->SendSizeEvent();
	if( Config->get_touch_screen() ){
		panBrowse->Move( 0, 0 );
	} else {
		//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	}

    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_vigencia( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseVigencia" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "numero" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_VIGENCIA, 0 );
    //21;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_VIGENCIA, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_VIGENCIA,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_VIGENCIA,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_VIGENCIA,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_VIGENCIA, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_VIGENCIA, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_VIGENCIA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_VIGENCIA,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_VIGENCIA,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_VIGENCIA,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_VIGENCIA,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_VIGENCIA,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_VIGENCIA, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_VIGENCIA,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, "VIGENCIA", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_diferimiento( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseDiferimiento" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "numero" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_DIFERIMIENTO, 0 );
    //21;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_DIFERIMIENTO, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_DIFERIMIENTO,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_DIFERIMIENTO,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_DIFERIMIENTO,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_DIFERIMIENTO, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_DIFERIMIENTO, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_DIFERIMIENTO,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_DIFERIMIENTO,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_DIFERIMIENTO,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DIFERIMIENTO,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DIFERIMIENTO,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DIFERIMIENTO,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_DIFERIMIENTO, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_DIFERIMIENTO,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif

    rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, "DIFERIMIENTO", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_cobros( int registros,f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //12;
    dat.FieldDescription[0].width = panCobro->browse_del_panel->getWidthColGrid( _CONSULTA_COBRO,
                                                                                 0 );
    //45;
    dat.FieldDescription[1].width = panCobro->browse_del_panel->getWidthColGrid( _CONSULTA_COBRO,
                                                                                 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panCobro->browse_del_panel );
    ( panCobro->browse_del_panel )->setPanelBrowseActivo( ( char* )panCobro );
    ( panCobro->browse_del_panel )->setInputBrowseActivo( panCobro->input_browse_del_panel );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_COBRO,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_COBRO,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_COBRO, 0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_COBRO, 1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_COBRO, 2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_COBRO, 3 ) );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_COBRO, _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_COBRO,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_COBRO, _INDICE_TEXT_COL ) );

    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    //rta = b.BROWSE_MYSQL( 28, 1, 360, 12, L_CADENA, f_usuario, "COBROS", 0.0, 0 );
	rta = b.BROWSE_MYSQL( 28, 1, 360, ((registros > 0 && registros <12 )?registros:12), L_CADENA, f_usuario, "COBROS", 0.0, 0 );
    panCobro->clearInputs();

    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_Pagos_Caja( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //12;
    dat.FieldDescription[0].width = panPagosCaja->browse_del_panel->getWidthColGrid(
    _CONSULTA_PAGOS_CAJA, 0 );
    //45;
    dat.FieldDescription[1].width = panPagosCaja->browse_del_panel->getWidthColGrid(
    _CONSULTA_PAGOS_CAJA, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panPagosCaja->browse_del_panel );
    ( panPagosCaja->browse_del_panel )->setPanelBrowseActivo( ( char* )panPagosCaja );
    (
    panPagosCaja->browse_del_panel )->setInputBrowseActivo( panPagosCaja->input_browse_del_panel );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_PAGOS_CAJA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_PAGOS_CAJA,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             3 ) );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
 //   rta = b.BROWSE( 28, 1, 360, 18, L_CADENA, f_usuario, "PAGOSx", 0.0, 0 );
    rta = b.BROWSE_MYSQL( 28, 1, 360, 12, L_CADENA, f_usuario, "PAGOSx", 0.0, 0 );
    panPagosCaja->clearInputs();

    return rta;
}
/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_Pagos_Esp( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //12;
    dat.FieldDescription[0].width = panPagosEsp->browse_del_panel->getWidthColGrid(
    _CONSULTA_PAGOS_CAJA, 0 );
    //45;
    dat.FieldDescription[1].width = panPagosEsp->browse_del_panel->getWidthColGrid(
    _CONSULTA_PAGOS_CAJA, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panPagosEsp->browse_del_panel );
    ( panPagosEsp->browse_del_panel )->setPanelBrowseActivo( ( char* )panPagosEsp );
    ( panPagosEsp->browse_del_panel )->setInputBrowseActivo( panPagosEsp->input_browse_del_panel );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_PAGOS_CAJA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_PAGOS_CAJA,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             3 ) );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, "PAGOSx", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_elegir_tarjeta( f_usu f_usuario )
{
    int rta = 0;
    int j,d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseTarjeta" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_TARJETA,
                                                                                  0 );
    //11;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_TARJETA,
                                                                                  1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_TARJETA,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_TARJETA,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_TARJETA,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_TARJETA, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_TARJETA, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_TARJETA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_TARJETA,_INDICE_POSI_TEXT_COL));

    //se guarda el tamaño original
	wxSize pBrowseSize = panBrowse->GetSize();
	//panBrowse->SetSize( 326 ,350 );
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	panBrowse->SendSizeEvent();
    
    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_TARJETA, 0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_TARJETA, 1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_TARJETA, 2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_TARJETA,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_TARJETA, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_TARJETA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_TARJETA, _INDICE_TEXT_COL ) );

    //panBrowse->input_browse_del_panel->SetNotVisible();
	panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
	if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
	}

    rta = b.BROWSE_MYSQL( 28, 1, 360, panBrowse->browse_del_panel->getNumFilas(), L_CADENA, f_usuario, "TARJETA", 0.0, 0 );

   	//se vulve a aplicar el tamaño original
	//panBrowse->SetSize( pBrowseSize );
	panBrowse->SetSize( 700 , 350  );
	panBrowse->SendSizeEvent();
	if( Config->get_touch_screen() ){
		panBrowse->Move( 0, 0 );
	} else {
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	}

    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_bancos( f_usu f_usuario )
{
    int rta = 0;
    int j,d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseBancos" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_BANCOS,
                                                                                  0 );
    //11;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_BANCOS,
                                                                                  1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_BANCOS,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_BANCOS,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_BANCOS,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_BANCOS, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_BANCOS, _INDICE_ROW_SIZE_PAN ) );
    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_BANCOS,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_BANCOS,_INDICE_POSI_TEXT_COL));
	panBrowse->SendSizeEvent();
    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_BANCOS, 0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_BANCOS, 1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_BANCOS, 2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_BANCOS,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_BANCOS, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                                   _INDICE_ALIN ) );
    //panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                                 // _INDICE_TEXT ) );
    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_BANCOS,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_BANCOS, _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
    }
    rta = b.BROWSE_MYSQL( 28, 1, 360, panBrowse->browse_del_panel->getNumFilas(), L_CADENA, f_usuario,
                    "BANCOS", 0.0, 0 );

	panBrowse->SetSize( 700 , 350  );
	panBrowse->SendSizeEvent();
	if( Config->get_touch_screen() ){
		panBrowse->Move( 0, 0 );
	} else {
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	}

    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_repactacion( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseRepactacion" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_REPACTACION, 0 );
    //25;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_REPACTACION, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_REPACTACION,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_REPACTACION,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_REPACTACION,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_REPACTACION, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_REPACTACION, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_REPACTACION,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_REPACTACION,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_REPACTACION,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_REPACTACION,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_REPACTACION,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_REPACTACION,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_REPACTACION, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_REPACTACION,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 28, 1, 360, 5, L_CADENA, f_usuario, "BANCOS", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_elegir_cuotas( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseElegirCuotas" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    //no
    strcpy( dat.FieldDescription[0].colName, "orden" );
    //no
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ELEGIR_CUOTAS, 0 );
    //31;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ELEGIR_CUOTAS, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}

    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_CUOTAS,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_CUOTAS,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_CUOTAS,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_CUOTAS, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_CUOTAS, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_ELEGIR_CUOTAS,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_ELEGIR_CUOTAS,_INDICE_POSI_TEXT_COL));

    (
    panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_CUOTAS,
                                                                           0 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_CUOTAS,
                                                                           1 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_CUOTAS,
                                                                           2 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_CUOTAS,
                                                                           3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_CUOTAS, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
    //panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_ELEGIR_CUOTAS,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 0, 1, 18, 18, L_CADENA, f_usuario, "INST. BENEFICAS", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_elegir_remito( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseElegirRemito" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 4;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "estado" );
    strcpy( dat.FieldDescription[1].colName, "fecha" );
    strcpy( dat.FieldDescription[2].colName, "comprobante" );
    strcpy( dat.FieldDescription[3].colName, "total" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    dat.FieldDescription[2].alignment = ALIGN_LEFT;
    dat.FieldDescription[3].alignment = ALIGN_CENTER;
    //15;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ELEGIR_REMITO, 0 );
    //8;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ELEGIR_REMITO, 1 );
    //30;
    dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ELEGIR_REMITO, 2 );
    //3;
    dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_ELEGIR_REMITO, 3 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}

    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_REMITO,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_REMITO,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_REMITO,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_REMITO, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_REMITO, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_ELEGIR_REMITO,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_ELEGIR_REMITO,_INDICE_POSI_TEXT_COL));

    (
    panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_REMITO,
                                                                           0 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_REMITO,
                                                                           1 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_REMITO,
                                                                           2 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_ELEGIR_REMITO,
                                                                           3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_ELEGIR_REMITO, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_ELEGIR_REMITO,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 2, 1, 360, 14, L_CODIGO, f_usuario, "CONSULTA REMITOS", 0.0, 0 );
    return rta;
}


/****************************************************************************/
/* Consulta  DATA ENTRY                                  .                  */
/****************************************************************************/
int Consulta_data_entry( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    memset( &dat, 0, sizeof( struct _dat ) );
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseDataEntry" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "cod" );
    strcpy( dat.FieldDescription[1].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_DATA_ENTRY, 0 );
    //20;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_DATA_ENTRY, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_DATA_ENTRY,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_DATA_ENTRY,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_DATA_ENTRY,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_DATA_ENTRY, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_DATA_ENTRY, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_DATA_ENTRY,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_DATA_ENTRY,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_DATA_ENTRY,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DATA_ENTRY,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DATA_ENTRY,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DATA_ENTRY,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_DATA_ENTRY, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_DATA_ENTRY,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 28, 1, 360, 18, L_CADENA, f_usuario, " ", 0.0, 0 );
    return rta;
}



/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_tipos_de_clientes( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    memset( &dat, 0, sizeof( struct _dat ) );
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseTipoCliente" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    dat.nFields = 1;
    dat.selectedField = 0;        
    strcpy( dat.FieldDescription[0].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_TIPO_CLIENTES, 0 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_TIPO_CLIENTES,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_TIPO_CLIENTES,_INDICE_POSI_TEXT_COL));

    (
    panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           0 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           1 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           2 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 1, 1, 360, 14, L_CADENA, f_usuario, " TIPOS DE CLIENTES ", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_condicion_iva( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    memset( &dat, 0, sizeof( struct _dat ) );
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    dat.nFields = 1;
    dat.selectedField = 0;        
    strcpy( dat.FieldDescription[0].colName, "descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_TIPO_CLIENTES, 0 );

    browseWin = properties->GetWindows()->GetWindowByName( "browseTipoCliente" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);

    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES, _INDICE_ROW_SIZE_PAN ) );

    ///panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_TIPO_CLIENTES,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_TIPO_CLIENTES,_INDICE_POSI_TEXT_COL));

    (
    panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           0 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           1 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           2 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_TIPO_CLIENTES,
                                                                           3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_TIPO_CLIENTES, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_TIPO_CLIENTES,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    //el tama? del browse depende del vector TIPOS_IVA_MOSTRAR
    //rta = b.BROWSE( 1, 1, 360, 14, L_CADENA, f_usuario, " CONDICION IVA ", 0.0, 0 );
	rta = b.BROWSE_MYSQL( 1, 1, 360, 14, L_CADENA, f_usuario, " CONDICION IVA ", 0.0, 0 );
    return rta;
}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_grupos_cli( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    memset( &dat, 0, sizeof( struct _dat ) );
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseGruposCli" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    dat.nFields = 1;
    dat.selectedField = 0;        
    strcpy( dat.FieldDescription[0].colName, "Descripcion" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    //8;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_GRUPOS_CLI, 0 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_GRUPOS_CLI,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_GRUPOS_CLI,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_GRUPOS_CLI,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_GRUPOS_CLI, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_GRUPOS_CLI, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_GRUPOS_CLI,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_GRUPOS_CLI,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_GRUPOS_CLI,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_GRUPOS_CLI,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_GRUPOS_CLI,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_GRUPOS_CLI,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_GRUPOS_CLI, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_GRUPOS_CLI,
                                                            _INDICE_TEXT_COL ) );

    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 1, 1, 360, 14, L_CADENA, f_usuario, " GRUPOS DE CLIENTES ", 0.0, 0 );
    return rta;
}

}

/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_Pagos_Esp2( f_usu f_usuario )
{
    int rta = 0, d;
    int j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "codigo" );
    strcpy( dat.FieldDescription[1].colName, "nombre" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //12;
    dat.FieldDescription[0].width = panPagosEsp->browse_del_panel->getWidthColGrid(
    _CONSULTA_PAGOS_CAJA, 0 );
    //45;
    dat.FieldDescription[1].width = panPagosEsp->browse_del_panel->getWidthColGrid(
    _CONSULTA_PAGOS_CAJA, 1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panPagosEsp->browse_del_panel );
    ( panPagosEsp->browse_del_panel )->setPanelBrowseActivo( ( char* )panPagosEsp );
    ( panPagosEsp->browse_del_panel )->setInputBrowseActivo( panPagosEsp->input_browse_del_panel );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_PAGOS_CAJA, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_PAGOS_CAJA, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_PAGOS_CAJA,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_PAGOS_CAJA,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_PAGOS_CAJA,
                                                                             3 ) );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_PAGOS_CAJA,
                                                            _INDICE_TEXT_COL ) );

    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    rta = b.BROWSE_MYSQL( 28, 1, 360, 10, L_CADENA, f_usuario, "PAGOSx", SI );
    return rta;
}


/****************************************************************************/
/* Consulta                                              .                  */
/****************************************************************************/
int Consulta_socio( f_usu f_usuario )
{
    int rta = 0, j, d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif         
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 9;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "Apellido y Nombre / Razon Social" );
    strcpy( dat.FieldDescription[1].colName, "Numero" );
    strcpy( dat.FieldDescription[2].colName, "Cuit/Cuil" );
    //strcpy( dat.FieldDescription[3].colName, "Unidad Negocio" );
    strcpy( dat.FieldDescription[3].colName, "Compania" );
    strcpy( dat.FieldDescription[4].colName, "Prepaga" );
    strcpy( dat.FieldDescription[5].colName, "Tipo" );
    strcpy( dat.FieldDescription[6].colName, "Direccion" );
    strcpy( dat.FieldDescription[7].colName, "Localidad" );
    strcpy( dat.FieldDescription[8].colName, "Fecha baja" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    dat.FieldDescription[2].alignment = ALIGN_LEFT;
    dat.FieldDescription[3].alignment = ALIGN_LEFT;
    dat.FieldDescription[4].alignment = ALIGN_LEFT;
    dat.FieldDescription[5].alignment = ALIGN_LEFT;
    dat.FieldDescription[6].alignment = ALIGN_LEFT;
    dat.FieldDescription[7].alignment = ALIGN_LEFT;
    dat.FieldDescription[8].alignment = ALIGN_LEFT;

    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 0 );
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 1 );
    dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 2 );
    dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 3 );
    dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 4 );
    dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 5 );
    dat.FieldDescription[6].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 6 );
    dat.FieldDescription[7].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 7 );
    dat.FieldDescription[8].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLIENTES, 8 );

    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}

    PanelBrowse b( &dat, dat.nFields );

    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_CLIENTES,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_CLIENTES,_INDICE_POSI_TEXT_COL));

    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_CLIENTES,
                                                                             3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_CLIENTES, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_CLIENTES,
                                                            _INDICE_TEXT_COL ) );

    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif

    /*rta = b.BROWSE( 20 - ancho_panel / 2, 1, 61 - ancho_panel / 2, panBrowse->browse_del_panel->getNumFilas(), L_CADENA,
        f_usuario, "CLIENTES" );
        */
    rta = b.BROWSE_MYSQL( 28, 1, 300, panBrowse->browse_del_panel->getNumFilas(), L_CADENA, f_usuario,
                    "CLIENTES",SI );
    panBrowse->DesActivate();
    return rta;
}

/****************************************************************************/
/* Consulta                                                                 */
/****************************************************************************/
int Consulta_socio_reintegro( f_usu f_usuario )
{
    int rta = 0, j;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif         
	browseWin = properties->GetWindows()->GetWindowByName( "browseCliReintegros" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 8;
    dat.selectedField = 0;
    strcpy( dat.FieldDescription[0].colName, "Apellido y Nombre / Razon Social" );
    strcpy( dat.FieldDescription[1].colName, "Numero" );
    strcpy( dat.FieldDescription[2].colName, "Cuit/Cuil" );
    //strcpy( dat.FieldDescription[3].colName, "Unidad Negocio" );
    strcpy( dat.FieldDescription[3].colName, "Compania" );
    strcpy( dat.FieldDescription[4].colName, "Prepaga" );
    strcpy( dat.FieldDescription[5].colName, "Tipo" );
    strcpy( dat.FieldDescription[6].colName, "Direccion" );
    strcpy( dat.FieldDescription[7].colName, "Localidad" );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    dat.FieldDescription[2].alignment = ALIGN_LEFT;
    dat.FieldDescription[3].alignment = ALIGN_LEFT;
    dat.FieldDescription[4].alignment = ALIGN_LEFT;
    dat.FieldDescription[5].alignment = ALIGN_LEFT;
    dat.FieldDescription[6].alignment = ALIGN_LEFT;
    dat.FieldDescription[7].alignment = ALIGN_LEFT;

    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 0 );
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 1 );
    dat.FieldDescription[2].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 2 );
    dat.FieldDescription[3].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 3 );
    dat.FieldDescription[4].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 4 );
    dat.FieldDescription[5].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 5 );
    dat.FieldDescription[6].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 6 );
    dat.FieldDescription[7].width = panBrowse->browse_del_panel->getWidthColGrid(
    _CONSULTA_CLI_REINTEGROS, 7 );

    for( j = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
    }

    PanelBrowse b( &dat, dat.nFields );

    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_CLI_REINTEGROS, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_CLI_REINTEGROS, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_CLI_REINTEGROS,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_CLI_REINTEGROS,_INDICE_POSI_TEXT_COL));

    (
    panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                                           0 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                                           1 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                                           2 ),
                                            panBrowse->getSizePanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                                           3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_CLI_REINTEGROS,
                                                           _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_CLI_REINTEGROS,
                                                            _INDICE_TEXT_COL ) );
    panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif

    rta = b.BROWSE_MYSQL( 20 - ancho_panel / 2, 1, 61 - ancho_panel / 2, 14, L_CADENA, f_usuario,
                    "CLIENTES", 0.0, 0 );
    panBrowse->DesActivate();
    return rta;
}

/*************************************************************************************************/
int Consulta_descrip( f_usu f_usuario )
/*************************************************************************************************/
{
    int rta = 0;
    int j,d;
    struct _dat dat;
    //char cadena[80];
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
	browseWin = properties->GetWindows()->GetWindowByName( "browseDescrip" );
	panBrowse->SetSize( browseWin->GetSize()->GetWidth(), browseWin->GetSize()->GetHeight());
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
    memset( &dat, 0, sizeof( struct _dat ) );
    dat.nFields = 2;
    dat.selectedField = 0;
    strncpy( dat.FieldDescription[0].colName, "Valor", sizeof( dat.FieldDescription[0].colName ) );
    strncpy( dat.FieldDescription[1].colName, "Descripcion", sizeof( dat.FieldDescription[1].colName ) );
    dat.FieldDescription[0].alignment = ALIGN_LEFT;
    dat.FieldDescription[1].alignment = ALIGN_LEFT;
    //4;
    dat.FieldDescription[0].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_DESCRIPT,
                                                                                  0 );
    //11;
    dat.FieldDescription[1].width = panBrowse->browse_del_panel->getWidthColGrid( _CONSULTA_DESCRIPT,
                                                                                  1 );
    for( j = 0,d = 0;j < dat.nFields;j++ ) {
        dat.FieldDescription[j].type = 0;
        d=d+dat.FieldDescription[j].width;
    }
    //if(Config->get_touch_screen()){
		panBrowse->input_browse_del_panel->__setPosition( (browseWin->GetSize()->GetWidth()-d)/2, -1, d, -1);
	//}
    PanelBrowse b( &dat, dat.nFields );
    b.SetDefaultBrowseBrowse( panBrowse->browse_del_panel );
    ( panBrowse->browse_del_panel )->setPanelBrowseActivo( ( char* )panBrowse );
    ( panBrowse->browse_del_panel )->setInputBrowseActivo( panBrowse->input_browse_del_panel );
    panBrowse->browse_del_panel->iniFont( panBrowse->getCharPanelBrowse( _CONSULTA_DESCRIPT,
                                                                         _INDICE_FONT_PAN ) );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_DESCRIPT,
                                                                           _INDICE_FONT_COLOR_PAN ),
                                            0 );
    panBrowse->browse_del_panel->iniColour( panBrowse->getCharPanelBrowse( _CONSULTA_DESCRIPT,
                                                                           _INDICE_BACK_COLOR_PAN ),
                                            1 );
    panBrowse->browse_del_panel->iniColour(
    panBrowse->getCharPanelBrowse( _CONSULTA_DESCRIPT, _INDICE_SEL_BACK_COLOR_PAN ), 2 );
    panBrowse->browse_del_panel->_SetRowSize(
    panBrowse->getCharPanelBrowse( _CONSULTA_DESCRIPT, _INDICE_ROW_SIZE_PAN ) );

    //panBrowse->labelbrowse->setText(panBrowse->getCharTit(_CONSULTA_BANCOS,_INDICE_TEXT_COL));
    //panBrowse->labelbrowse->__setPosition(panBrowse->getCharTit(_CONSULTA_BANCOS,_INDICE_POSI_TEXT_COL));
	//panBrowse->SetSize( 426 , 350  );
	//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	panBrowse->SendSizeEvent();	
    ( panBrowse->browse_del_panel )->SetSize( panBrowse->getSizePanelBrowse( _CONSULTA_DESCRIPT, 0 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DESCRIPT, 1 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DESCRIPT, 2 ),
                                              panBrowse->getSizePanelBrowse( _CONSULTA_DESCRIPT, 3 ) );

    string nomImagePres = ( panBrowse->getCharPanelBrowse( _CONSULTA_DESCRIPT, _INDICE_GIF ) );
	{
		char aux[80];
		strcpy(aux,nomImagePres.c_str());
		if( RAM_MODO_EJECUCION == ENTRENAMIENTO ) {
			strcpy(&aux[strlen(aux)-4],"_E.gif");
		}
		panBrowse->SetBackGround( aux );
	}
//    panBrowse->SetBackGround( nomImagePres.c_str() );

    panBrowse->tituloPanelBrowse->setFont( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                                  _INDICE_FUENTE ) );
    panBrowse->tituloPanelBrowse->setColour( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                                    _INDICE_COLOR ) );
    panBrowse->tituloPanelBrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                                        _INDICE_POSI ) );
    panBrowse->tituloPanelBrowse->setAlign( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                                   _INDICE_ALIN ) );
    panBrowse->tituloPanelBrowse->setText( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                                  _INDICE_TEXT ) );

    panBrowse->labelbrowse->setFont( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                            _INDICE_FUENTE_TIT_GRID ) );
    panBrowse->labelbrowse->setColour( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                              _INDICE_COLOR_TIT_GRID ) );
    panBrowse->labelbrowse->__setPosition( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                                  _INDICE_POSI_TIT_GRID ) );
    panBrowse->labelbrowse->setAlign( panBrowse->getCharTit( _CONSULTA_DESCRIPT,
                                                             _INDICE_ALIN_TIT_GRID ) );
    panBrowse->labelbrowse->setText( panBrowse->getCharTit( _CONSULTA_DESCRIPT, _INDICE_TEXT_COL ) );

    //panBrowse->input_browse_del_panel->SetVisible();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
    if( Config->get_touch_screen() ){
            /* Informo al touch screen un cambio de pantalla */
		frame.currPaneltouch = idBrowseClientes;
    }
    rta = b.BROWSE_MYSQL( 28, 1, 360, panBrowse->browse_del_panel->getNumFilas(), L_CADENA, f_usuario,
                    "BANCOS", 0 );

	panBrowse->SetSize( 700 , 350  );
	panBrowse->SendSizeEvent();
	if( Config->get_touch_screen() ){
		panBrowse->Move( 0, 0 );
	} else {
//panBrowse->Center();
			panBrowse->Move(GET_POSICION_PANTALLAS_X()+((TamanioVentana->GetX()/2)-(browseWin->GetSize()->GetWidth()/2)), 
			GET_POSICION_PANTALLAS_Y()+ ((TamanioVentana->GetY()/2)-(browseWin->GetSize()->GetHeight()/2)),0);
	}

	return rta;
}

#endif
