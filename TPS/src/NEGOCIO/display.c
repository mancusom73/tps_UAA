#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <btrv.h> 
#include <cr.h> 
#include <dbrouter.h> 
#include <display.h> 
#include <driver.h> 
#include <omron.h> 
#include <tkt.h> 
#include <cmemory.h>
#include <actions.h>
#include <db.h>
#include <deftables.h>
#include <path.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_DISPLAY
#if defined(INVEL_W) || defined(INVEL_L)
int CorbaInitDisplay();
#endif
/*****************************************************************************/
void DISPLAY( char *buffer )
/*****************************************************************************/
{
    char _buffer[21];
    if( TIPO_DISPLAY == DISPLAY_DRIVER ) {
        memcpy( _buffer, buffer, 20 );
        _buffer[20] = 0;
        DISPLAY_LINEA( _buffer, 1 );
        memcpy( _buffer, &buffer[20], 20 );
        _buffer[20] = 0;
        DISPLAY_LINEA( _buffer, 2 );
    }
}
/***************************************************************************/
void DISPLAY_LINEA( char *buffer, int linea )
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    if( linea == 1 ) {
        setData( ACTION_SET_DISPLAY_TEXT_LINE_1, buffer, strlen( buffer ) );
    }
    else {
        setData( ACTION_SET_DISPLAY_TEXT_LINE_2, buffer, strlen( buffer ) );
    }
    //  DISPLAY_LINEA_( buffer, linea );
    #else
    struct
    {
        int rta;
        char *buffer;
    } rta_drv;
    int i;
    if( TIPO_DISPLAY == DISPLAY_DRIVER ) {
        if( DISPLAY_SET_XY( 1, linea ) ) {
            for( i = 0;i < 20;i++ ) {
                if( buffer[i] < ' ' || buffer[i] > 'z' ) {
                    buffer[i] = ' ';
                }
            }
            memcpy( puntero_memoria_driver_invel, buffer, 20 );
            rta_drv.buffer = puntero_memoria_real_driver_invel;
            DRIVER( _DRV_display, _DSP_display, _DISPLAY_cli_y_ope, &rta_drv, sizeof( rta_drv ) );
            //memcpy(buffer, puntero_memoria_driver_invel, 20);
        }
    }
    else if( HARD_OMRON ) {
        #ifdef COMPILAR_OMRON
        OMRON_DISPLAY_LINEA( buffer, linea );
        #endif
    }
    else if( HARD_TEC ) {
        #ifdef COMPILAR_TEC
        TEC_DISPLAY_LINEA( buffer, linea );
        #endif
    }
    #endif
}
/***************************************************************************/
int INIT_DISPLAY()
/***************************************************************************/
{
    int error = 0;
    #if defined(INVEL_W) || defined(INVEL_L)
    //error = CorbaInitDisplay(  );
    #else
    if( TIPO_DISPLAY == DISPLAY_DRIVER ) {
        DRIVER( _DRV_display, _DSP_inicializacion, _DISPLAY_cli_y_ope, &error, sizeof( error ) );
    }
    else if( HARD_OMRON ) {
        #ifdef COMPILAR_OMRON
        OMRON_INIT_DISPLAY();
        #endif
    }
    else if( HARD_TEC ) {
        #ifdef COMPILAR_TEC
        TEC_INIT_DISPLAY();
        #endif
    }
    #endif
    return ( !error );
}
/***************************************************************************/
void BORRAR_DISPLAY()
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    setData( ACTION_SET_DISPLAY_CLEAR_TEXT, NULL, 0 );
    //BORRAR_DISPLAY_(  );
    #else
    int error;
    if( TIPO_DISPLAY == DISPLAY_DRIVER ) {
        DRIVER( _DRV_display, _DSP_borrar_display, _DISPLAY_cli_y_ope, &error, sizeof( error ) );
    }
    else if( HARD_OMRON ) {
        #ifdef COMPILAR_OMRON
        OMRON_BORRAR_DISPLAY();
        #endif
    }
    else if( HARD_TEC ) {
        #ifdef COMPILAR_TEC
        TEC_BORRAR_DISPLAY();
        #endif
    }
    #endif
}
/***************************************************************************/
void BORRAR_DISPLAY_LINEA( int linea )
/***************************************************************************/
{
    #if defined(INVEL_W) || defined(INVEL_L)
    #else
    int error;
    if( TIPO_DISPLAY == DISPLAY_DRIVER ) {
        if( DISPLAY_SET_XY( 1, linea ) ) {
            DRIVER( _DRV_display, _DSP_borrar_linea, _DISPLAY_cli_y_ope, &error, sizeof( error ) );
        }
    }
    else if( HARD_OMRON ) {
        #ifdef COMPILAR_OMRON
        OMRON_BORRAR_DISPLAY_LINEA( linea );
        #endif
    }
    else if( HARD_TEC ) {
        #ifdef COMPILAR_TEC
        TEC_BORRAR_DISPLAY_LINEA( linea );
        #endif
    }
    #endif
}
/***************************************************************************/
int DISPLAY_SET_XY( int x, int y )
/***************************************************************************/
{
    int error,xy;
    if( TIPO_DISPLAY == DISPLAY_DRIVER ) {
        xy = ( x << 8 ) + y;
        DRIVER( _DRV_display, _DSP_setxy, xy | _DISPLAY_cli_y_ope, &error, sizeof( error ) );
    }
    return ( !error );
}
/*****************************************************************************/
void DISPLAY_STRING( int id )
/*****************************************************************************/
{
 /*   int area;
    area = SELECTED();
    SELECT( AREA_STRING_TABLE );
    GET_EQUAL( ( char* )&id );
    if( FOUND() ) {
        DISPLAY( string_table->mensaje );
    }
    SELECT( area );*/
    int area_ant, tipo_ant;
    area_ant = SELECTED2();
	tipo_ant = SELECTED_TIPO();
    //SELECT( AREA_STRING_TABLE );
    SELECT_TABLE(T_STRINGT, TT_ORIG);
    SET_WHERE("id = %d", id);
    RUN_QUERY( NO );
    if( FOUND2() ) {
        DISPLAY( string_table->mensaje );
    }
    SELECT_TABLE(area_ant, tipo_ant);
}
#endif

