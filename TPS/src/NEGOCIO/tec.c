#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_TEC
#undef MK_FP
#ifndef INVEL_L
#endif
#if !defined(INVEL_W) && !defined(INVEL_L)
/*----------------------------- Definiciones -------------------------------*/
#define TEC_PORT  0x10c
#define TEC_DP    0x10
#define TEC_RST   0x1f
#define TEC_DC1   0x11
#define TEC_PORT_CAJON 0x100
#define TEC_PORT_DISPLAY
/*------------------------------ Variables --------------------------------*/
static unsigned int _TEC_DELAY_DISPLAY = 4000, _TEC_DELAY_CAJON = 0xffff;
static FILE *fp_liu;
#ifdef COMPILAR_DISPLAY
/***************************************************************************/
void _TEC_display( unsigned char byte )
/***************************************************************************/
{
    unsigned int i;
    outportb( TEC_PORT, byte );
    for( i = 0;i < _TEC_DELAY_DISPLAY;i++ ) {
        ;
    }
}
/***************************************************************************/
void TEC_DISPLAY( char *buffer )
/***************************************************************************/
{
    int i;
    char buf[21];
    if( HARD_TEC == _TEC_3500 ) {
        /*---------- setea cursor -------------*/
        _TEC_display( TEC_DP );
        _TEC_display( 0 );
        /*---------- coloca mensaje -------------*/
        for( i = 0;buffer[i] && i < 40;i++ ) {
            _TEC_display( buffer[i] );
        }
    }
    else if( HARD_TEC == _TEC_5000 || HARD_TEC == _TEC_5500 ) {
        memcpy( buf, buffer, 20 );
        buf[20] = 0;
        TEC_DISPLAY_LINEA( buf, 1 );
        TEC_DISPLAY_LINEA( &buffer[20], 2 );
    }
}
/***************************************************************************/
void TEC_DISPLAY_LINEA( char *buffer, int linea )
/***************************************************************************/
{
    char buf[41];
    int l,i;
    if( HARD_TEC == _TEC_3500 ) {
        /*---------- setea cursor -------------*/
        _TEC_display( TEC_DP );
        _TEC_display( ( ( linea == 1 ) ? 0 : 20 ) );
        /*---------- coloca mensaje -------------*/
        strcpy( buf, buffer );
        l = strlen( buffer );
        memset( &buf[l], ' ', 20 - l );
        buf[20] = 0;
        for( i = 0;i < 20;i++ ) {
            _TEC_display( buf[i] );
        }
    }
    else if( HARD_TEC == _TEC_5000 || HARD_TEC == _TEC_5500 ) {
        sprintf( buf, "\x1b[%1i;1H", linea );
        fprintf( fp_liu, buf );
        fprintf( fp_liu, buffer );
        fprintf( fp_liu, "\r" );
    }
}
/***************************************************************************/
void TEC_BORRAR_DISPLAY()
/***************************************************************************/
{
    TEC_DISPLAY( "                                        " );
}
/***************************************************************************/
void TEC_BORRAR_DISPLAY_LINEA( int linea )
/***************************************************************************/
{
    TEC_DISPLAY_LINEA( "                    ", linea );
}
/***************************************************************************/
void TEC_INIT_DISPLAY()
/***************************************************************************/
{
    if( HARD_TEC == _TEC_3500 ) {
        /*---------- Inicializa -------------*/
        _TEC_display( TEC_RST );
        _TEC_display( TEC_DC1 );
    }
    else if( HARD_TEC == _TEC_5000 || HARD_TEC == _TEC_5500 ) {
        fp_liu = fopen( "LIU", "wb" );
        fprintf( fp_liu, "\x1b[2J" );
        fprintf( fp_liu, "\x1b[1;1H" );
    }
}
#endif
/***************************************************************************/
int TEC_INIT()
/***************************************************************************/
{
    char *p;
    /*---------------- Demora para display ---------------*/
    p = getenv( "TEC_DELAY_DISPLAY" );
    if( p ) {
        _TEC_DELAY_DISPLAY = atoi( p );
    }
    /*---------------- Demora para cajon -----------------*/
    p = getenv( "TEC_DELAY_CAJON" );
    if( p ) {
        _TEC_DELAY_CAJON = atoi( p );
    }
    return ( 1 );
}
/***************************************************************************/
void TEC_ABRIR_CAJON()
/***************************************************************************/
{
    unsigned i,st;
    if( HARD_TEC == _TEC_3500 ) {
        outportb( TEC_PORT_CAJON + 4, 1 );
        outportb( TEC_PORT_CAJON + 3, 1 );
        for( i = 0;i < _TEC_DELAY_CAJON;i++ ) {
            ;
        }
        outportb( TEC_PORT_CAJON + 3, 0 );
        outportb( TEC_PORT_CAJON + 4, 1 );
    }
    else if( HARD_TEC == _TEC_5000 || HARD_TEC == _TEC_5500 ) {
        st = inp( 0x310 );
        outp( 0x310, st | 0x40 );
    }
}
/***************************************************************************/
char TEC_CAJON_CERRADO()
/***************************************************************************/
{
    char estado;
    int st;
    if( HARD_TEC == _TEC_3500 ) {
        estado = inportb( TEC_PORT_CAJON + 0x0e );
        estado &= 0x01;
    }
    else if( HARD_TEC == _TEC_5000 || HARD_TEC == _TEC_5500 ) {
        st = inp( 0x310 );
        estado = ( st & 0x40 ) ? 0 : 1;
    }
    return ( estado );
}
#endif
#endif

