#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <pinpad1.h> 
#include <val.h> 
#include <val2.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/*****************************************************************************/
int GP416_PRESENT_KEY( char clave[2] )
/*****************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_GP416
    int st;
    switch( HARD_INVEL ) {
        case VERSION_1:
            memcpy( BUFFER, clave, 2 );
            st = EJECUTAR_COMANDO( 0xe1, 0x20, 4, 8, 2 );
            if( !st && res1 == 0x89 ) {
                rta = res2;
            }
            /* printf( "[PKEY: %i - %i,%i] ",rta, BUFFER[0], BUFFER[1] ); */
            break;
        case VERSION_2:
            rta = !VAL_COD_SECRETO_TARJETA( &clave, NULL, NULL );
            break;
    }
    #endif
    return ( rta );
}
/*****************************************************************************/
int GP416_PRESENT_IKEY( char clave[4] )
/*****************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_GP416
    int st;
    switch( HARD_INVEL ) {
        case VERSION_1:
            memcpy( BUFFER, clave, 4 );
            st = EJECUTAR_COMANDO( 0xe1, 0x20, 40, 28, 4 );
            if( !st && res1 == 0x89 ) {
                rta = res2;
            }
            break;
        case VERSION_2:
            rta = !VAL_COD_BORRADO1_TARJETA( &clave, NULL, NULL );
            break;
    }
    #endif
    return ( rta );
}
/*****************************************************************************/
int GP416_WRITE( unsigned char dir, unsigned char cant, char *buffer )
/*****************************************************************************/
{
    int st = 0;
    #ifdef COMPILAR_GP416
    switch( HARD_INVEL ) {
        case VERSION_1:
            memcpy( BUFFER, buffer, cant );
            st = EJECUTAR_COMANDO( 0xe1, 0xd0, 0, dir, cant );
            if( res1 != 0x89 || res2 != 0x00 ) {
                st = 1;
            }
            break;
        case VERSION_2:
            st = VAL_ESCRIBIR_TARJETA( dir, cant, buffer, NULL, NULL );
            break;
    }
    #endif
    return ( st );
}
/*****************************************************************************/
int GP416_READ( unsigned char dir, unsigned char cant, char *buffer )
/*****************************************************************************/
{
    int st = 0;
    #ifdef COMPILAR_GP416
    switch( HARD_INVEL ) {
        case VERSION_1:
            st = EJECUTAR_COMANDO( 0xe2, 0xb0, 0, dir, cant );
            if( res1 != 0x89 || res2 != 0x00 ) {
                st = 1;
            }
            if( !st ) {
                memcpy( buffer, BUFFER, cant );
            }
            break;
        case VERSION_2:
            st = VAL_LEER_TARJETA( dir, cant, buffer, NULL, NULL );
            break;
    }
    #endif
    return ( st );
}
/*****************************************************************************/
int GP416_ERASE( unsigned char dir, unsigned char cant )
/*****************************************************************************/
{
    int st = 0;
    #ifdef COMPILAR_GP416
    switch( HARD_INVEL ) {
        case VERSION_1:
            st = EJECUTAR_COMANDO( 0xe2, 0xde, cant, dir, 2 );
            if( res1 != 0x89 || res2 != 0x00 ) {
                st = 1;
            }
            break;
        case VERSION_2:
            st = VAL_BORRAR_TARJETA( dir, cant, NULL, NULL );
            break;
    }
    #endif
    return ( st );
}
/*****************************************************************************/
int GP416_DETECT()
/*****************************************************************************/
{
    int rta = 0;
    #ifdef COMPILAR_GP416
    switch( HARD_INVEL ) {
        case VERSION_1:
            EJECUTAR_COMANDO( 0xe2, 0xff, 0, 0, 2 );
            if( res1 != 0x89 || res2 != 0x00 ) {
                rta = 0;
            }
            else {
                rta = ( BUFFER[0] == 0x3c ) ? 1 : 0;
                /* printf( "[DETECT: %i - %i,%i] ",BUFFER[0], BUFFER[0],
                 * BUFFER[1] ); */
            }
            break;
        case VERSION_2:
            if( VAL_STATUS() & _TARJETA_INGRESADA ) {
                VAL_SELECT_CARD( _GMP416 );
                VAL_POWER_ON();
            }
            break;
    }
    #endif
    return ( rta );
}
/*****************************************************************************/
int GP416_SACA_TENSION()
/*****************************************************************************/
{
    int rta = 1;
    #ifdef COMPILAR_GP416
    int st;
    switch( HARD_INVEL ) {
        case VERSION_1:
            st = EJECUTAR_COMANDO( 0xe2, 0xfe, 0, 0, 2 );
            if( !st && res1 == 0x90 && res2 == 0x00 ) {
                rta = 0;
            }
            break;
        case VERSION_2:
            rta = VAL_POWER_OFF();
            break;
    }
    #endif
    return ( rta );
}

