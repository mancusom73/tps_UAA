#include "PanelGrill.h"
#include "cr.h"
#include "CScreencfg.h"

extern CDesktop *properties;

PanelGrill::PanelGrill( const wxString &title, const wxPoint &pos, const wxSize &size, long style ) 
: CPanel( title, pos, size, style )
{
    LabelTitulo = new CLabel();
    screenXml = new CScreencfg( "inputgrilla", "default", "default" );
    #ifdef INVEL_W
    input_de_grilla = new CInput( this, 10, 335, 270, 30, false );
    #else
    input_de_grilla = new CInput( this, 700, 335, 270, 30, false );
    #endif
    panel_width = size.GetWidth();
    panel_height = size.GetHeight();
};

void PanelGrill::init()
{
}

void PanelGrill::createTitle()
{
    LabelTitulo->setParent( this );
    this->AddObject( LabelTitulo );
    this->init();
    create();
}

void PanelGrill::create()
{
    SetBackGround( screenXml->getBackgroundXML().c_str() );

    screenXml->setLabelXML( LabelTitulo, "panelGrillaTitle" );
    LabelTitulo->setNotVisible();
    screenXml->setGridXML( Grilla, this, "grillaEnPanel" );

    #ifdef INVEL_W
    input_de_grilla->SetNotVisible();
    #endif

    this->AddObject( Grilla );
    Grilla->Draw();
}

CInput * PanelGrill::getFocus()
{
    return input_de_grilla;
}

int PanelGrill::getWidth()
{
    return panel_width;
}

int PanelGrill::getHeight()
{
    return panel_height;
}


void PanelGrill::setLabelTitulo( const string &s )
{
    LabelTitulo->setText( s );
    LabelTitulo->Draw();
}


void PanelGrill::adjust()
{
    SetSize( getWidth(), getHeight() );
    Refresh();
}

void PanelGrill::addRow( const string &s )
{
    vector<string> row;

    #ifdef INVEL_L
    MutexGuiEnter();
    #endif

    row.push_back( s );
    Grilla->AddRow( row );

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}


void PanelGrill::scrollGrill( const string &lines )
{
    static int curr_line = 0;
    int line = atoi( lines.c_str() );

    #ifdef INVEL_L
    MutexGuiEnter();
    #endif

    //Grilla->SetGridCursor(curr_line,0);
    Grilla->ClearSelection();

    if( line == -99 )                                  //deshabilita
    {
        Grilla->Refresh();
        //Grilla->SetGridCursor(curr_line,1);
        Grilla->ScrollLines( Grilla->GetRows() );
    }
    else if( line == 0 )                                 //habilita
    {
        Grilla->Refresh();
        curr_line = 0;
        //Grilla->SetGridCursor(curr_line,0);
        Grilla->ScrollLines( Grilla->GetRows() );
        Grilla->SelectRow( curr_line );
    }
    else if( line > 0 )                                    //baja line lineas
    {
        for( int l = 0;l < line;l++ ) {
            if( curr_line< Grilla->GetRows() - 1 ) {
                curr_line++;
                Grilla->MoveCursorDown( false );
            }
        }

        Grilla->SelectRow( curr_line );
    }
    else if( line < 0 )                                    //sube line lineas
    {
        for( int l = 0;l > line;l-- ) {
            if( curr_line > 0 ) {
                curr_line--;
                Grilla->MoveCursorUp( false );
            }
        }
        Grilla->SelectRow( curr_line );
    }

    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}

void PanelGrill::vaciar()
{
    //int i;   

    #ifdef INVEL_L
    MutexGuiEnter();
    #endif
    if( Grilla->GetRows() > 0 ) {
        Grilla->DeleteRows( 0, Grilla->GetRows() );
    }
    #ifdef INVEL_L
    MutexGuiLeave();
    #endif
}
