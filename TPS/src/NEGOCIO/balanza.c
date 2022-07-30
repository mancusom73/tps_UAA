#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
//#include <tpv_dat.h>
#include <actions.h>
#include <balanza.h> 
#include <b_mens.h> 
#include <cmemory.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <driver.h> 
#include <getch.h> 
#include <mensaje.h> 
#include <mstring.h> 
#include <tkt.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Prueba interfaz con Balanza KRETZ
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_BALANZA
#include <string.h>
float LEER_BALANZA();
int INIT_BALANZA();
float LEER_BALANZA_KRETZ();
float LEER_BALANZA_TOLEDO();
float _LEER_BALANZA_TOLEDO();
int BALANZA_obtener_peso( char *buffer );
int _obtener_byte( unsigned char *byte );
void _INIT_BALANZA();
/***************************************************************************/
float LEER_BALANZA()
/***************************************************************************/
{
    float peso = 0.0;
    int weight;
    switch( TIPO_BALANZA ) {
        case BALANZA_DRIVER:
            setData( ACTION_HABILITA_SCALE, "true", 0 );
            getData( ACTION_READ_WEIGHT, ( char* )&weight, 0 );
            setData( ACTION_HABILITA_SCALE, "false", 0 );
            if( weight ) {
                if( config.pais == ELSALVADOR ) {
                    peso = ( float )( weight / 1000.0 );
                }
                else {
                    peso = ( float )( weight / 1000.0 );
                }
            }
            break;
    }
    if( TIPO_BALANZA ) {
        BORRAR_MENSAJE();
    }
    return ( peso );
}
/***************************************************************************/
int INIT_BALANZA()
/***************************************************************************/
{
    int rta = 1;
    switch( TIPO_BALANZA ) {
        case BALANZA_KRETZ:
        case BALANZA_TOLEDO:
            _INIT_BALANZA();
            break;
        case BALANZA_DRIVER:
            rta = INIT_DRIVER_BALANZA();
            if( !rta ) {
                SET_MEMORY_CHAR( __tipo_balanza, NO );
            }
            break;
    }
    return rta;
}
/***************************************************************************/
float LEER_BALANZA_KRETZ()
/***************************************************************************/
{
 /*   char _buffer[10], buffer[10], primero = 1;
    int seguir = 1, cant = 0;
    float peso = 0.0;

    while( seguir ) {
        if( BALANZA_obtener_peso( buffer ) ) {
            if( strcmp( buffer, "00.000" ) ) {
                if( primero ) {
                    strcpy( _buffer, buffer );
                    primero = 0;
                }
                else {
                    if( strcmp( buffer, _buffer ) ) {
                        cant = 0;
                        strcpy( _buffer, buffer );
                    }
                    else {
                        cant++;
                        if( cant >= BALANZA_CANTIDAD_LECTURAS ) {
                            seguir = 0;
                            peso = ( float )atof( buffer );
                        }
                    }
                }
            }
        }
        if( KBHIT() && _GETCH() == 27 ) {
            seguir = 0;
        }
    }

    return ( peso );*/
    return ( 0.0 );
}


/***************************************************************************/
int BALANZA_obtener_peso( char *buffer )
/***************************************************************************/
{
    int rta = 0;
    return ( rta );
}
/***************************************************************************/
int _obtener_byte( unsigned char *byte )
/***************************************************************************/
{
    int rta = 0;
    return ( rta );
}
/***************************************************************************/
void _INIT_BALANZA()
/***************************************************************************/
{
}
/***************************************************************************/
int INIT_DRIVER_BALANZA()
/***************************************************************************/
{
    int rta;
    DRIVER( _DRV_balanza, _BAL_inicializacion, 0, &rta, sizeof( rta ) );
    return ( !rta );
}
/***************************************************************************/
float LEER_BALANZA_TOLEDO()
/***************************************************************************/
{
    int seguir = 1, primero = 1, cant = 1;
    float peso = 0, peso1, peso2;
    while( seguir ) {
        peso1 = _LEER_BALANZA_TOLEDO();
        if( peso1 > 0 ) {
            if( primero ) {
                peso2 = peso1;
                primero = 0;
            }
            else {
                if( peso2 == peso1 ) {
                    cant++;
                    if( cant >= BALANZA_CANTIDAD_LECTURAS ) {
                        seguir = 0;
                        peso = peso2;
                    }
                }
                else {
                    peso2 = peso1;
                    cant = 1;
                }
            }
        }
        else {
            seguir = 0;
        }
    }
    return ( peso );
}
/***************************************************************************/
float _LEER_BALANZA_TOLEDO()
/***************************************************************************/
{
    float peso = 0.0;
    return ( peso );
}
#endif

