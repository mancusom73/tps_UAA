#include "GuiProperties.h"
#include "typeacti.h"


#include "IniParser.h"

string SAUX( const string &s, const string &s1 )
{
    return s + s1;
}
#define ADD_INI(g,t,f) e.group = #g;e.type = t;e.attribute = #f;ini_def.add(e);\

IniFile ini_def;

bool CREATE_PROPERTIES_CLABEL()
{
    IniEntity e;
    e.type = IT_STRING;
    e.setValue( "" );
    ADD_INI( CLABEL, IT_STRING, DEFAULT_IMAGE_FILE_NOT_FOUND );
    return true;
}

bool CREATE_PROPERTIES_INPUT()
{
    IniEntity e;
    e.setValue( "" );

    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_IMAGE_NAME_PANEL_INPUT );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_INPUT_GENERICO );
    ADD_INI( PANTALLA_INPUT, IT_INTEGER, DEFAULT_SPACE_HEIGHT );
    ADD_INI( PANTALLA_INPUT, IT_INTEGER, DEFAULT_END_OFFSET );
    ADD_INI( PANTALLA_INPUT, IT_INTEGER, DEFAULT_START_POINT_LABEL );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_PANEL_INPUT_GENERICO );

    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_LABEL_FONT );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_LABEL_COLOUR );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_TEXT_POS_LABEL );

    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_TITULO_FONT );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_TITULO_COLOUR );
    ADD_INI( PANTALLA_INPUT, IT_INTEGER, DEFAULT_POS_X_TITULO );
    ADD_INI( PANTALLA_INPUT, IT_INTEGER, DEFAULT_POS_Y_TITULO );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_TEXT_POS_TITULO );

    ADD_INI( PANTALLA_INPUT, IT_INTEGER, DEFAULT_PANEL_WIDTH );

    /*------------------------- Input de Datos Tarjetas -------------------------------------*/
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_INPUT_TARJETA );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_PANEL_INPUT_TARJETA );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_INPUT_ALTA_CLIENTES );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_PANEL_INPUT_ALTA_CLIENTES );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_INPUT_COBROS_ESP );
    ADD_INI( PANTALLA_INPUT, IT_STRING, DEFAULT_COORD_PANEL_INPUT_COBROS_ESP );
    return true;
}

bool CREATE_PROPERTIES(char *path)
{
    IniEntity e;
    e.type = IT_STRING;
    e.setValue( "" );

    if( !CREATE_PROPERTIES_CLABEL() ) {
        return false;
    }
    if( !CREATE_PROPERTIES_INPUT() ) {
        return false;
    }

    #ifdef INVEL_L
    IniParser ini( path );
    #else
    IniParser ini( path );
    #endif

    if( !ini.pars( &ini_def ) ) {
        return false;
    }

    return true;
}
