#include "CPanelBrowse.h"
#include "actions.h"
#include "typeacti.h"
#include "GuiProperties.h"
#include "CPanel.h"
#include "AppMain.h"

#ifdef _DEBUG
CPanelBrowse::CPanelBrowse( const wxString &title, const wxPoint &pos, const wxSize &size ) 
                            : CPanel( title, pos, size, wxSIMPLE_BORDER /*| wxSTAY_ON_TOP*/ )
#else
CPanelBrowse::CPanelBrowse( const wxString &title, const wxPoint &pos, const wxSize &size ) 
                            : CPanel( title, pos, size, wxSIMPLE_BORDER | wxSTAY_ON_TOP )
#endif
{
    labelbrowse = new CLabel();
    tituloPanelBrowse = new CLabel();
}

void CPanelBrowse::createLabels()
{
    labelbrowse->setParent( this );
    tituloPanelBrowse->setParent( this );
    create();
}

void CPanelBrowse::create()
{
    //string nomImagePres = "images/dflt/browse.gif";
    //SetBackGround(nomImagePres.c_str());
}


void CPanelBrowse::setSizePanelBrowse( const string &f, int ind1 )
{
    char f_x[10],f_y[10],f_w[10],f_h[10];
    int l_x = 0, l_y = 0, l_w = 0, l_h = 0;
    int j = 0;
    int i;

    if( ind1 > 30 ) {
        sglog( "Error en tamao de array de size's" );
        exit( 1 );
    }

    for( i = 0;i < 30;i++ ) {
        vectCoord[ind1][i] = 0;
    }

    memset( f_x, 0, sizeof( f_x ) );
    memset( f_y, 0, sizeof( f_y ) );
    memset( f_w, 0, sizeof( f_w ) );
    memset( f_h, 0, sizeof( f_h ) );

    for( i = 0;i < (int)f.length();i++ ) {
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

    vectCoord[ind1][0] = atoi( f_x );
    vectCoord[ind1][1] = atoi( f_y );
    vectCoord[ind1][2] = atoi( f_w );
    vectCoord[ind1][3] = atoi( f_h );
}


int CPanelBrowse::getSizePanelBrowse( int ind1, int ind2 )
{
    int x = 0;

    x = vectCoord[ind1][ind2];
    return( x );
}


void CPanelBrowse::setCharTit( const string &f, int type, int ind1 )
{
    char f_x[400];
    int l_x = 0;

    if( ind1 > 30 ) {
        sglog( "Error en tamao de array de size's" );
        exit( 1 );
    }
    for( int i = 0;i < (int)f.length();i++ ) {
        if( f[i] != '"' ) {
            f_x[l_x++] = f[i];
        }
    }
    f_x[l_x] = 0;
    detailTit[ind1][type] = f_x;
}


string CPanelBrowse::getCharTit( int ind1, int ind2 )
{
    return( detailTit[ind1][ind2] );
}


void CPanelBrowse::setCharPanelBrowse( const string &f, int type, int ind1 )
{
    char f_x[400];
    int l_x = 0;

    if( ind1 > 30 ) {
        sglog( "Error en tamaño de array de size's" );
        exit( 1 );
    }
    for( int i = 0;i < (int)f.length();i++ ) {
        if( f[i] != '"' ) {
            f_x[l_x++] = f[i];
        }
    }
    f_x[l_x] = 0;
    detailPanelBrowse[ind1][type] = f_x;
}


string CPanelBrowse::getCharPanelBrowse( int ind1, int ind2 )
{
    return( detailPanelBrowse[ind1][ind2] );
}
