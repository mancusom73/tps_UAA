#include "PanelInput.h"
#include "cr.h"
#include "CScreencfg.h"
#include "AppMain.h"

extern CDesktop *properties;

PanelInput::PanelInput( const wxString &title, const wxPoint &pos, const wxSize &size, long style ) 
: CPanel( title, pos, size, style )
{
    LabelTitulo = new CLabel();
}

void PanelInput::init()
{
}

void PanelInput::createLabels()
{
    LabelTitulo->setParent( this );
    create();
}
void PanelInput::create()
{
    CForms *forms;    
    CForm *form;
    CImageRef *img;
    string id,background;
    CSkin *def;

    forms = properties->GetForms();
    form = forms->GetFormByName( "input" );
    img = form->GetImageRef();
    id = img->GetID();

    def = properties->GetImages()->GetSkinByName( "default" );
    background = def->GetImageByName( id )->GetFilename();
    SetBackGround( background.c_str() );

    setPropInput( ini_def.get( "PANTALLA_INPUT", "DEFAULT_COORD_INPUT_GENERICO" ).value.String,
                  _INDICE_POSI, _INPUT_GENERICO );    
    setPropInput( ini_def.get( "PANTALLA_INPUT",
                               "DEFAULT_COORD_PANEL_INPUT_GENERICO" ).value.String,
                  _INDICE_POSI_PAN_INPUT, _INPUT_GENERICO );    
    inicializarVariables();

    start_point_label = ini_def.get( "PANTALLA_INPUT", "DEFAULT_START_POINT_LABEL" ).value.Integer;
    label_font = ini_def.get( "PANTALLA_INPUT", "DEFAULT_LABEL_FONT" ).value.String;
    label_colour = ini_def.get( "PANTALLA_INPUT", "DEFAULT_LABEL_COLOUR" ).value.String;
    label_align = ini_def.get( "PANTALLA_INPUT", "DEFAULT_TEXT_POS_LABEL" ).value.String;

    titulo_font = ini_def.get( "PANTALLA_INPUT", "DEFAULT_TITULO_FONT" ).value.String;
    titulo_colour = ini_def.get( "PANTALLA_INPUT", "DEFAULT_TITULO_COLOUR" ).value.String;
    titulo_pos_x = ini_def.get( "PANTALLA_INPUT", "DEFAULT_POS_X_TITULO" ).value.Integer;
    titulo_pos_y = ini_def.get( "PANTALLA_INPUT", "DEFAULT_POS_Y_TITULO" ).value.Integer;
    titulo_align = ini_def.get( "PANTALLA_INPUT", "DEFAULT_TEXT_POS_TITULO" ).value.String;

    panel_width_default = ini_def.get( "PANTALLA_INPUT", "DEFAULT_PANEL_WIDTH" ).value.Integer;

    LabelTitulo->setFont( titulo_font );
    LabelTitulo->setColour( titulo_colour );
    LabelTitulo->setText( "" );
    LabelTitulo->setPosition( titulo_pos_x, titulo_pos_y );
    LabelTitulo->setNotVisible();
    LabelTitulo->setAlign( titulo_align );
    this->AddObject( LabelTitulo );    

    for( unsigned int i = 0 ;i < CANTIDAD_INPUTS;i++ ) {
        CInput *input = new CInput( this, start_point_input,
                                    start_offset + inputs.size() * ( input_size + space_size ),
                                    width_input, input_size, false, wxSIMPLE_BORDER );
        CLabel *label = new CLabel( this );
        label->setFont( label_font );
        label->setColour( label_colour );
        label->setPosition( start_point_label,
                            start_offset + inputs.size() * ( input_size + space_size ) );
        label->setVisible( false );
        label->setAlign( label_align );
        input->SetNotVisible();
        inputs.push_back( input );
        labels.push_back( label );
        this->AddObject( label );
    }
    visible_inputs = 0;
    visible_labels = 0;

    /*-------- Guardo los datos de todos los inputs ---------------*/
    setPropInput( ini_def.get( "PANTALLA_INPUT", "DEFAULT_COORD_INPUT_TARJETA" ).value.String,
                  _INDICE_POSI, _INPUT_TARJETA );
    setPropInput( ini_def.get( "PANTALLA_INPUT", "DEFAULT_COORD_PANEL_INPUT_TARJETA" ).value.String,
                  _INDICE_POSI_PAN_INPUT, _INPUT_TARJETA );    
    setPropInput( ini_def.get( "PANTALLA_INPUT", "DEFAULT_COORD_INPUT_ALTA_CLIENTES" ).value.String,
                  _INDICE_POSI, _INPUT_ALTA_CLIENTES );
    setPropInput( ini_def.get( "PANTALLA_INPUT",
                               "DEFAULT_COORD_PANEL_INPUT_ALTA_CLIENTES" ).value.String,
                  _INDICE_POSI_PAN_INPUT, _INPUT_ALTA_CLIENTES );    

    setPropInput( ini_def.get( "PANTALLA_INPUT", "DEFAULT_COORD_INPUT_COBROS_ESP" ).value.String,
                  _INDICE_POSI, _INPUT_COBROS_ESP );
    setPropInput( ini_def.get( "PANTALLA_INPUT",
                               "DEFAULT_COORD_PANEL_INPUT_COBROS_ESP" ).value.String,
                  _INDICE_POSI_PAN_INPUT, _INPUT_COBROS_ESP );
}


int PanelInput::getWidth()
{
    return panel_width_final;
}


int PanelInput::getHeight()
{
    return visible_inputs * ( input_size + space_size ) + end_offset + start_offset;
}


void PanelInput::setLabels( int type, const string &s )
{
    LabelTitulo->setText( s );
    LabelTitulo->Draw();
}


void PanelInput::adjust()
{
    SetSize( getWidth(), getHeight() );
    Refresh();
}

void PanelInput::addInput( const string &s, const string &mascara )
{
    addInput( s, -1, -1, -1, -1, mascara, -1 );
}

void PanelInput::addInput( const string &s )
{
    addInput( s, -1, -1, -1, -1, "", -1 );
}

void PanelInput::addInput( const string &s, int x, int y, int w, int h, const string &mascara, int w1 )
{
    //char aux2[200];

    if( visible_inputs >= 0 ) {
        visible_inputs++;

        if( x == -1 && y == -1 && w == -1 && h == -1 && w1 == -1 ) {
            inputs[visible_inputs - 1]->SetPositionSizeDefault();
            panel_width_final = panel_width_default;
        }
        else {
            inputs[visible_inputs - 1]->__setPosition( x, y, w, h );
            if( w1 == -1 ) {
                panel_width_final = w + 80;
            } else {
                panel_width_final = w1 + 15;
            }
        }

        #ifdef INVEL_L
        strncpy( aux2, s.c_str(),sizeof(aux2));
        for( int i = strlen( aux ) - 1;i >= 0;i-- ) {
            if( aux2[i] == ' ' ) {
                aux2[i] = '\0';
            }
            else {
                break;
            }
        }
        #endif
        inputs[visible_inputs - 1]->SetVisible();
        //if( mascara.length ) {         
        inputs[visible_inputs - 1]->SetText( mascara.c_str(), 0 );
        //} else {
        //   inputs[visible_inputs - 1]->SetText(" ",0);      
        //}
        labels[visible_inputs - 1]->setVisible( true );
        #ifdef INVEL_L
        labels[visible_inputs - 1]->setText( string( aux2 ) );
        #else
        labels[visible_inputs - 1]->setText( s );
        #endif
        labels[visible_inputs - 1]->Draw();
        focus = 0;
    }
}

void PanelInput::addLabel( const string &s )
{
    if( visible_inputs >= 0 ) {
        visible_inputs++;
        visible_labels++;
        panel_width_final = panel_width_default;
        labels[visible_inputs - 1]->setVisible( true );
        labels[visible_inputs - 1]->setText( s );
        labels[visible_inputs - 1]->Draw();
        focus = 0;
    }
}

void PanelInput::changeFocus( int type )
{
    focus = type;
    if( focus >= (int)inputs.size() ) {
        focus = 0;
    }
}


void PanelInput::deleteInputs()
{
    visible_inputs = 0;
    visible_labels = 0;
    for( unsigned int i = 0;i < inputs.size();i++ ) {
        inputs[i]->SetNotVisible();
        labels[i]->setVisible( false );
    }
    /*
    for(unsigned int i = 0;i<inputs.size();i++)
        delete inputs[i];
    inputs.clear();
    */
}

void PanelInput::setInputText( const string &t )
{
    if( inputs[focus] ) {
        ( inputs[focus] )->SetText( t.c_str(), 0 );
    }
}

void PanelInput::setPropInput( const string &f, int type, int ind1 )
{
    char f_x[100];
    int l_x = 0;

    if( ind1 > CANTIDAD_INPUTS ) {
        sglog( "Error en tamao de array de size's" );
        exit( 1 );
    }
    for( int i = 0;i < (int)f.length();i++ ) {
        if( f[i] != '"' ) {
            f_x[l_x++] = f[i];
        }
    }
    f_x[l_x] = 0;
    detailInput[ind1][type] = f_x;
}


string PanelInput::getPropInput( int ind1, int ind2 )
{
    return( detailInput[ind1][ind2] );
}

void PanelInput::positionInput( int input, int varios )
{
    #ifdef INVEL_L
    MutexGuiEnter();
    #endif   
    //this->__setPosition( getPropInput( input, _INDICE_POSI_PAN_INPUT ) );
    if( varios == _HIBRIDO || varios == _CONCATENADO ) {
        string posicion = getPropInput( input, _INDICE_POSI );

        for( int j = 0;j < ( visible_inputs - visible_labels );j++ ) {
            inputs[j]->SetNotVisible();
            inputs[j]->__setPosition( posicion );
            inputs[j]->__setYPosition( start_offset + j * ( input_size + space_size ) );
            inputs[j]->SetVisible();
        }
        adjust();
    }
    else if( varios == _MOVIL ) {
        this->Activate();
        ( ( AppMain * )CPanel::GetLauncher() )->ChangeFocus( this->getFocus() );
        inputs[visible_inputs - 1]->SetNotVisible();
        inputs[visible_inputs - 1]->__setPosition( getPropInput( input, _INDICE_POSI ) );
        inputs[visible_inputs - 1]->SetVisible();
    }
    else {
        inputs[visible_inputs - 1]->SetNotVisible();
        inputs[visible_inputs - 1]->__setPosition( getPropInput( input, _INDICE_POSI ) );
        inputs[visible_inputs - 1]->SetVisible();
    }
    Refresh();
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}

void PanelInput::inicializarVariables( void )
{
    space_size = ini_def.get( "PANTALLA_INPUT", "DEFAULT_SPACE_HEIGHT" ).value.Integer;
    end_offset = ini_def.get( "PANTALLA_INPUT", "DEFAULT_END_OFFSET" ).value.Integer;
    start_point_label = ini_def.get( "PANTALLA_INPUT", "DEFAULT_START_POINT_LABEL" ).value.Integer;

    string f = getPropInput( _INPUT_GENERICO, _INDICE_POSI );

    char f_x[10],f_y[10],f_w[10],f_h[10];
    int l_x = 0, l_y = 0, l_w = 0, l_h = 0;
    int j = 0;

    memset( f_x, 0, 10 );
    memset( f_y, 0, 10 );
    memset( f_w, 0, 10 );
    memset( f_h, 0, 10 );

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_x[l_x++] = f[i];
        }
        if( j == 1 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_y[l_y++] = f[i];
        }
        if( j == 2 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_w[l_w++] = f[i];
        }
        if( j == 3 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_h[l_h++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_x[l_x] = 0;
    f_y[l_y] = 0;
    f_w[l_w] = 0;
    f_h[l_h] = 0;

    start_point_input = atoi( f_x );
    start_offset = atoi( f_y );
    width_input = atoi( f_w );
    input_size = atoi( f_h );
}

void PanelInput::__setPosition( const string &f )
{
    char f_x[10],f_y[10],f_w[10],f_h[10];
    int l_x = 0, l_y = 0, l_w = 0, l_h = 0;
    int j = 0;
    int pos_x = 0, pos_y = 0, pos_w = 0, pos_h = 0;

    memset( f_x, 0, 10 );
    memset( f_y, 0, 10 );
    memset( f_w, 0, 10 );
    memset( f_h, 0, 10 );

    for( int i = 0;i < (int)f.length();i++ ) {
        if( j == 0 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_x[l_x++] = f[i];
        }
        if( j == 1 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_y[l_y++] = f[i];
        }
        if( j == 2 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_w[l_w++] = f[i];
        }
        if( j == 3 && f[i] != '-' && f[i] != '(' && f[i] != ')' ) {
            f_h[l_h++] = f[i];
        }
        if( f[i] == '-' ) {
            j++;
        }
    }
    f_x[l_x] = 0;
    f_y[l_y] = 0;
    f_w[l_w] = 0;
    f_h[l_h] = 0;

    pos_x = atoi( f_x );
    pos_y = atoi( f_y );
    pos_w = atoi( f_w );
    pos_h = atoi( f_h );
    SetSize( pos_x, pos_y, pos_w, pos_h );
}

