#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <cr.h>  
#include <cr_displ.h> 
#include <dbrouter.h> 
#include <display.h> 
#include <ftoc.h> 
#include <getch.h> 
#include <get_fech.h> 
#include <get_hora.h> 
#include <htoa.h> 
#include <_cr1.h> 
#include <cmemory.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_DISPLAY
/*****************************************************************************/
void MOSTRAR_EN_DISPLAY( char *buffer1, double importe )
/*****************************************************************************/
{
    char buf_display[41],aux[15];
    int l;
    memset( buf_display, ' ', sizeof( buf_display ) );
    memcpy( buf_display, buffer1, ( strlen( buffer1 ) > 20 ? 20 : strlen( buffer1 ) ) );
    FTOC( importe, aux, ENTEROS, DECIMALES, 1, 1 );
    l = strlen( aux );
    memcpy( &buf_display[40 - l], aux, l + 1 );
    buf_display[40] = 0;
    DISPLAY( buf_display );
}
/*****************************************************************************/
void MOSTRAR_ITEM_EN_DISPLAY( char *nombre, double importe )
/*****************************************************************************/
{
    char buf_display[41],aux[15];
    int l;
    memset( buf_display, ' ', sizeof( buf_display ) );
    // TOLEDO: (OK) Mostrar el subtotal en display
    if( RAM_MODO_EJECUCION == INVENTARIO ) {
        MOSTRAR_EN_DISPLAY( nombre, importe );
    }
    else {
        if( MOSTRAR_SUBTOTAL_EN_DISPLAY ) {
            memcpy( &buf_display[20], ST( S_SUBTOTAL ), 8 );
            FTOC( RAM_TOTAL, aux, ENTEROS, DECIMALES, 1, 1 );
            l = strlen( aux );
            memcpy( &buf_display[40 - l], aux, l );
            memcpy( buf_display, nombre, 20 - ( ENTEROS + DECIMALES + 1 ) );
            /* memcpy( buf_display, "SUBTOTAL", 8 ); FTOC( RAM_TOTAL, aux,
                     * ENTEROS, DECIMALES, 1, 1 ); l = strlen( aux ); memcpy(
                     * &buf_display[20-l], aux, l ); memcpy( &buf_display[20],
                     * nombre, 20 - (ENTEROS+DECIMALES+1) ); */
        }
        else {
            memcpy( buf_display, nombre, strlen( nombre ) );
        }
        FTOC( importe, aux, ENTEROS, DECIMALES, 1, 1 );
        l = strlen( aux );
        memcpy( &buf_display[20 - l], aux, l );
        buf_display[40] = 0;
        DISPLAY( buf_display );
    }
}
/*****************************************************************************/
void CR1_DISPLAY_CAJA_ABIERTA()
/*****************************************************************************/
{
    char aux[21];
    memcpy( aux, config.nom_comercio, 20 );
    aux[20] = 0;
    DISPLAY_LINEA( aux, 1 );
    DISPLAY_LINEA( ST( S____CAJA__ABIERTA____ ), 2 );
    SET_MOSTRAR_HORA_DISPLAY( NO );
}
/*****************************************************************************/
void CR1_DISPLAY( char *buff1, char *buff2 )
/*****************************************************************************/
{
    char b1[] = "                    ";
    char b2[] = "                    ";
    int a;
    a = strlen( buff1 );
    memcpy( b1, buff1, ( a > 20 ) ? 20 : a );
    a = strlen( buff2 );
    memcpy( b2, buff2, ( a > 20 ) ? 20 : a );
    DISPLAY_LINEA( b1, 1 );
    DISPLAY_LINEA( b2, 2 );
}
/*****************************************************************************/
void CR1_DISPLAY_CAJA_ESPERA()
/*****************************************************************************/
{
    DISPLAY_LINEA( "                    ", 1 );
    DISPLAY_LINEA( ST( S____CAJA_EN_ESPERA___ ), 2 );
    SET_MOSTRAR_HORA_DISPLAY( SI );
}
/*****************************************************************************/
void CR1_DISPLAY_CAJA_CERRADA()
/*****************************************************************************/
{
	char aux[21]; 
	memcpy( aux, config.nom_comercio, 20 );
    aux[20] = 0;
	DISPLAY_LINEA( aux, 1 );
    DISPLAY_LINEA( ST( S_____CAJA_CERRADA____ ), 2 );
    SET_MOSTRAR_HORA_DISPLAY( SI );
	
}
/*****************************************************************************/
void MOSTRAR_HORA_EN_DISPLAY()
/*****************************************************************************/
{
    long hora;
    static long hora_ant;
    char buffer[21];
    hora = GET_HORA_COMPLETA_DOS();
    if( hora != hora_ant ) {
        hora_ant = hora;
        DTOC( _GET_FECHA_DOS(), buffer );
        strcat( buffer, "    " );
        HTOAL( hora, &buffer[12] );
        DISPLAY_LINEA( buffer, 1 );
    }
}
#endif

