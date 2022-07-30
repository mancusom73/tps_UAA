#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <dbrouter.h> 
#include <modo_pro.h> 
#include <omron.h> 
#include <pausa.h> 
#include <ram.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_OMRON
/*----------------------------- Definiciones -------------------------------*/
#define _IMC_INIT    0
#define _IMC_KBHIT   1
#define _IMC_GETKEY  2
#define _IMC_PRINT   3
#define _IMC_DISPLAY 4
#define _IMC_CARD    5
#define _IMC_CAJON   6
/*----------------------------- Estructuras --------------------------------*/
static struct
{
    int cod;
    int key;
} imc_teclado;
static struct
{
    int cod;
    char *texto;
    int status1;
    int status2;
} imc_printer;
static struct
{
    int cod;
    int comando;
    char *texto;
    int delay;
    int speed;
    int posicion;
    int port;
} imc_display;
static struct
{
    char *buffer;
    int largo;
} imc_card;
static struct
{
    int cajon;
    int comando;
    int estado;
} imc_cajon;
/*------------------------------ Variables --------------------------------*/
static int status_printer;
static int _DELAY_GETKEY = 1, _VECES_GETKEY = 8;
struct _ica
{
    int tipo;
    char *puntero;
    int rta;
};
struct _ica *ica = ( struct _ica * )NULL;
SEL selector;
USHORT paragraph;
/***************************************************************************/
int _OMRON( char tipo, char *estructura, int longitud )
/***************************************************************************/
{
    REGS16 regs;
    char *pun;
    SET_RAM( OFF );
    //   ica =(struct _ica *)MK_FP_PROTEGIDO( 0,0x204, 8 );
    //Ahora apunta a la direccion 0x0210 (0x0210 / 16 = 33)
    /*      ica =(struct _ica *)MK_FP_PROTEGIDO( 33,0, 8 );
     * if( longitud )if( DosAllocRealSeg(longitud, &paragraph, &selector) != 0){
     * printf("DosAllocRealSeg() failed\n");
     * exit(255);
     * } */
    pun = MK_FP( selector, 0 );
    //   ica->puntero =(char*)(((unsigned int)paragraph) <<16);
    if( ica == NULL ) {
        ALOJA_ESPACIO_OMRON();
    }
    ica->puntero = ( char far * )( ( ( long )paragraph ) * 0x10000 );
    ica->tipo = tipo;
    if( estructura && longitud > 0 ) {
        memcpy( pun, estructura, longitud );
    }
    regs.ax = 0;
    regs.bx = 0;
    regs.ds = paragraph;
    regs.si = 0;
    DosRealIntr( 0x80, &regs, 0L, 0 );
    if( estructura && longitud > 0 ) {
        memcpy( estructura, pun, longitud );
    }
    //   if( longitud )DosFreeRealSeg( selector );
    SET_RAM( ON );
    return ( ica->rta );
}
/***************************************************************************/
int OMRON_KBHIT()
/***************************************************************************/
{
    int rta = 0, error;
    imc_teclado.cod = 3;
    imc_teclado.key = 0;
    error = _OMRON( _IMC_KBHIT, ( char* )&imc_teclado, sizeof( imc_teclado ) );
    rta = ( error ) ? 0 : imc_teclado.key;
    return ( rta );
}
/***************************************************************************/
void OMRON_SETUP_GETKEY( int delay, int veces )
/***************************************************************************/
{
    _DELAY_GETKEY = delay;
    _VECES_GETKEY = veces;
}
/***************************************************************************/
void OMRON_GET_SETUP_GETKEY( int *delay, int *veces )
/***************************************************************************/
{
    *delay = _DELAY_GETKEY;
    *veces = _VECES_GETKEY;
}
/***************************************************************************/
int OMRON_GETKEY()
/***************************************************************************/
{
    int rta = 0, error, c = 0;
    imc_teclado.cod = 3;
    imc_teclado.key = 0;
    while( !rta ) {
        if( OMRON_KBHIT() ) {
            error = _OMRON( _IMC_GETKEY, ( char* )&imc_teclado, sizeof( imc_teclado ) );
        }
        else {
            error = 1;
        }
        rta = ( error ) ? 0 : imc_teclado.key;
        c++;
        if( _DELAY_GETKEY ) {
            PAUSA( _DELAY_GETKEY );
        }
        if( c > _VECES_GETKEY && !rta ) {
            rta = -999;
        }
    }
    return ( rta );
}
/***************************************************************************/
void OMRON_PRINT( char *buffer )
/***************************************************************************/
{
    SEL selector;
    USHORT paragraph;
    char *puntero_real,*puntero_prot;
    if( strlen( buffer ) ) {
        if( DosAllocRealSeg( strlen( buffer ) + 1, &paragraph, &selector ) != 0 ) {
            printf( "DosAllocRealSeg() failed\n" );
            exit( 255 );
        }
    }
    puntero_prot = MK_FP( selector, 0 );
    puntero_real = ( char far * )( ( ( long )paragraph ) * 0x10000 );
    strcpy( puntero_prot, buffer );
    imc_printer.cod = 4;
    imc_printer.texto = puntero_real;
    imc_printer.status1 = 0;
    imc_printer.status2 = 0;
    status_printer = _OMRON( _IMC_PRINT, ( char* )&imc_printer, sizeof( imc_printer ) );
    if( strlen( buffer ) ) {
        DosFreeRealSeg( selector );
    }
}
/***************************************************************************/
int OMRON_IS_PRINTER()
/***************************************************************************/
{
    int rta = 1;
    if( status_printer == 151 || status_printer == 168 ) {
        rta = 0;
    }
    if( rta ) {
        rta = !OMRON_PAPER_OUT();
        if( !rta ) {
            OMRON_PRINT( "" );
            rta = !OMRON_PAPER_OUT();
        }
    }
    return ( rta );
}
/***************************************************************************/
int OMRON_PAPER_OUT()
/***************************************************************************/
{
    int rta = 0;
    if( status_printer == 1 || imc_printer.status2 & 0x300 ) {
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
void OMRON_SET_SLIP()
/***************************************************************************/
{
    while( !( imc_printer.status2 & 1 ) ) {
        OMRON_PRINT( "" );
    }
}
/***************************************************************************/
void OMRON_ESPERAR_QUE_SAQUEN_LA_HOJA()
/***************************************************************************/
{
    while( imc_printer.status2 & 1 ) {
        OMRON_PRINT( "" );
    }
}
#ifdef COMPILAR_DISPLAY
/***************************************************************************/
void OMRON_DISPLAY( char *buffer )
/***************************************************************************/
{
    SEL selector;
    USHORT paragraph;
    char *puntero_real,*puntero_prot;
    if( HARD_OMRON ) {
        if( strlen( buffer ) ) {
            if( DosAllocRealSeg( strlen( buffer ) + 1, &paragraph, &selector ) != 0 ) {
                printf( "DosAllocRealSeg() failed\n" );
                exit( 255 );
            }
        }
        puntero_prot = MK_FP( selector, 0 );
        puntero_real = ( char far * )( ( ( long )paragraph ) * 0x10000 );
        strcpy( puntero_prot, buffer );
        /*---------- setea cursor -------------*/
        memset( &imc_display, 0, sizeof( imc_display ) );
        imc_display.cod = 3;
        imc_display.comando = 5;
        imc_display.posicion = 0;
        _OMRON( _IMC_DISPLAY, ( char* )&imc_display, sizeof( imc_display ) );
        /*---------- coloca mensaje -------------*/
        memset( &imc_display, 0, sizeof( imc_display ) );
        imc_display.cod = 3;
        imc_display.comando = 1;
        imc_display.texto = puntero_real;
        _OMRON( _IMC_DISPLAY, ( char* )&imc_display, sizeof( imc_display ) );
        if( strlen( buffer ) ) {
            DosFreeRealSeg( selector );
        }
    }
}
/***************************************************************************/
void OMRON_DISPLAY_LINEA( char *buffer, int linea )
/***************************************************************************/
{
    char buf[41];
    int l;
    SEL selector;
    USHORT paragraph;
    char *puntero_real,*puntero_prot;
    if( HARD_OMRON ) {
        if( strlen( buffer ) ) {
            if( DosAllocRealSeg( 41, &paragraph, &selector ) != 0 ) {
                printf( "DosAllocRealSeg() failed\n" );
                exit( 255 );
            }
        }
        puntero_prot = MK_FP( selector, 0 );
        puntero_real = ( char far * )( ( ( long )paragraph ) * 0x10000 );
        strcpy( puntero_prot, buffer );
        /*---------- setea cursor -------------*/
        memset( &imc_display, 0, sizeof( imc_display ) );
        imc_display.cod = 3;
        imc_display.comando = 5;
        imc_display.posicion = ( linea == 1 ) ? 0 : 20;
        _OMRON( _IMC_DISPLAY, ( char* )&imc_display, sizeof( imc_display ) );
        /*---------- coloca mensaje -------------*/
        strcpy( buf, buffer );
        l = strlen( buffer );
        memset( &buf[l], ' ', 20 - l );
        buf[20] = 0;
        strcpy( puntero_prot, buf );
        memset( &imc_display, 0, sizeof( imc_display ) );
        imc_display.cod = 3;
        imc_display.comando = 1;
        imc_display.texto = puntero_real;
        _OMRON( _IMC_DISPLAY, ( char* )&imc_display, sizeof( imc_display ) );
        if( strlen( buffer ) ) {
            DosFreeRealSeg( selector );
        }
    }
}
/***************************************************************************/
void OMRON_INIT_DISPLAY()
/***************************************************************************/
{
    if( HARD_OMRON ) {
        memset( &imc_display, 0, sizeof( imc_display ) );
        imc_display.cod = 3;
        imc_display.comando = 9;
        _OMRON( _IMC_DISPLAY, ( char* )&imc_display, sizeof( imc_display ) );
    }
}
/***************************************************************************/
void OMRON_BORRAR_DISPLAY()
/***************************************************************************/
{
    if( HARD_OMRON ) {
        OMRON_DISPLAY( "                                        " );
    }
}
/***************************************************************************/
void OMRON_BORRAR_DISPLAY_LINEA( int linea )
/***************************************************************************/
{
    if( HARD_OMRON ) {
        OMRON_DISPLAY_LINEA( "                    ", linea );
    }
}
#endif
/***************************************************************************/
int OMRON_INIT()
/***************************************************************************/
{
    int rta = 0;
    if( getvect( 0x80 ) ) {
        _OMRON( _IMC_INIT, NULL, 0 );
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
int OMRON_TARJETA_MAGNETICA( unsigned char *buffer )
/***************************************************************************/
{
    int rta = 0, error;
    //unsigned char buf[300];
    SEL selector;
    USHORT paragraph;
    char *puntero_real,*puntero_prot;
    if( DosAllocRealSeg( 300, &paragraph, &selector ) != 0 ) {
        printf( "DosAllocRealSeg() failed\n" );
        exit( 255 );
    }
    puntero_prot = MK_FP( selector, 0 );
    puntero_real = ( char far * )( ( ( long )paragraph ) * 0x10000 );
    memset( &imc_card, 0, sizeof( imc_card ) );
    imc_card.buffer = puntero_real;
    imc_card.largo = 100;
    error = _OMRON( _IMC_CARD, ( char* )&imc_card, sizeof( imc_card ) );
    if( !error && imc_card.largo ) {
        memcpy( buffer, puntero_prot /*buf */, 50 );
        buffer[0] = imc_card.largo - 2;
        rta = 1;
        imc_card.largo = 100;
        error = _OMRON( _IMC_CARD, ( char* )&imc_card, sizeof( imc_card ) );
    }
    DosFreeRealSeg( selector );
    return ( rta );
}
/***************************************************************************/
void OMRON_ABRIR_CAJON()
/***************************************************************************/
{
    memset( &imc_cajon, 0, sizeof( imc_cajon ) );
    imc_cajon.cajon = NRO_CAJON + 5;
    imc_cajon.comando = 1;
    imc_cajon.estado = 0;
    _OMRON( _IMC_CAJON, ( char* )&imc_cajon, sizeof( imc_cajon ) );
}
/***************************************************************************/
int OMRON_CAJON_CERRADO()
/***************************************************************************/
{
    memset( &imc_cajon, 0, sizeof( imc_cajon ) );
    imc_cajon.cajon = 6;
    imc_cajon.comando = 2;
    imc_cajon.estado = 0;
    _OMRON( _IMC_CAJON, ( char* )&imc_cajon, sizeof( imc_cajon ) );
    return ( imc_cajon.estado );
}
/***************************************************************************/
void ALOJA_ESPACIO_OMRON()
{
    /***************************************************************************/
    //Ahora apunta a la direccion 0x0210 (0x0210 / 16 = 33)
    ica = ( struct _ica * )MK_FP_PROTEGIDO( 33, 0, 8 );
    if( DosAllocRealSeg( sizeof( imc_display ), &paragraph, &selector ) != 0 ) {
        printf( "DosAllocRealSeg() failed\n" );
        exit( 255 );
    }
};
#endif

