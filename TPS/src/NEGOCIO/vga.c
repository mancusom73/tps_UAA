#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <malloc.h> 
#include <modo_pro.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
#if !defined(INVEL_W) && !defined(INVEL_L)
#define EGAVGA_SEQUENCER 0x3C4                    /* Port de datos/direcciones Sequencer */
#define EGAVGA_MONCTR    0x3D4                    /* Monitor-Controller */
#define EGAVGA_GRAPHCTR  0x3CE                    /* Port datos/dir. Graphics-Controller */
#define L_CAR 16
extern    VGA_CHARDEF( char, char, char, char, unsigned, unsigned );
//static unsigned char *_MEM_VGA = (char *)0xA0000000;
static unsigned char *_MEM_VGA;
//static unsigned char _FONTS[4097];
static unsigned char *_FONTS = NULL;
extern char _ATRIBUTO;
extern char *_PANTALLA,_TIPO_PANTALLA,_MODO_GRAFICO;
int _VGA_ACTIVA = 0;
/*************************************************************************/
INICIA_VGA()
/*************************************************************************/
{
    if( !_VGA_ACTIVA ) {
        _VGA_ACTIVA = SI;
        _MEM_VGA = MK_FP_PROTEGIDO( 0xa000, 0, 0XFFFF );
    }
}
/*************************************************************************/
int VGA_LOADFONT()
/*************************************************************************/
{
    int handle, rta = 0;
    if( _FONTS == NULL ) {
        _FONTS = MALLOC( 4097 );
    }
    handle = _open( "cr.fnt", O_RDONLY );
    if( handle > 0 ) {
        if( _read( handle, _FONTS, 4097 ) ) {
            VGA_CHARDEF( 0, 0, 16, 128, FP_SEG( &_FONTS[1] ), FP_OFF( &_FONTS[1] ) );
            VGA_CHARDEF( 128, 0, 16, 128, FP_SEG( &_FONTS[1 + 16 * 128] ),
                         FP_OFF( &_FONTS[1 + 16 * 128] ) );
            rta = 1;
        }
        _close( handle );
    }
    return ( rta );
}
/*************************************************************************/
_INIT_FONTS()
/*************************************************************************/
{
    textmode( 3 );
    VGA_LOADFONT();
    SET_ANCHO_CARACTER( 8 );
}
/*****************************************************************************/
SET_ANCHO_CARACTER( unsigned char ancho )
/*****************************************************************************/
/* El ancho puede ser 8 o 9 puntos                                           */
/*****************************************************************************/
{
    union REGS Regs;
    unsigned char x;                              /* acoge el valor para el Misc-Output-Reg. */
    Regs.x.bx = ( ancho == 8 ) ? 0x0001 : 0x0800;
    /*----- conmutar resolucin horizontal de 720 a 640 puntos o al revs ----*/
    x = inp( 0x3CC ) & ( 255 - 12 );
    if( ancho == 9 ) {
        x |= 4;
    }
    outp( 0x3C2, x );
    /*------------- conmutar Sequencer de 8 a 9 puntos o al revs ------------*/
    disable();
    outport( EGAVGA_SEQUENCER, 0x0100 );
    outport( EGAVGA_SEQUENCER, 0x01 + ( Regs.h.bl << 8 ) );
    outport( EGAVGA_SEQUENCER, 0x0300 );
    enable();
    /*--------------------- ajustar pantalla horizontal ----------------------*/
    Regs.x.ax = 0x1000;
    Regs.h.bl = 0x13;
    int86( 0x10, &Regs, &Regs );
}
/*****************************************************************************/
RESET_FONTS()
/*****************************************************************************/
{
    union REGS Regs;
    SET_ANCHO_CARACTER( 9 );
    Regs.x.ax = 0x1104;                           /* cargar juego de caracteres 8*16 */
    Regs.h.bl = 0;
    int86( 0x10, &Regs, &Regs );
    SELECCIONA_FONTS( 0, 0 );
}
/*****************************************************************************/
SELECCIONA_FONTS( unsigned char map0, unsigned char map1 )
/*****************************************************************************/
{
    union REGS Regs;
    /*------------ Programar el Character-Map-Select-Register ----------------*/
    Regs.x.ax = 0x1103;
    Regs.h.bl = ( map0 & 3 ) + ( ( map0 & 4 ) << 2 ) + ( ( map1 & 3 ) << 2 )
              + ( ( map1 & 4 ) << 3 );
    int86( 0x10, &Regs, &Regs );
}
/*****************************************************************************/
PUT_GRAF_CHAR_VGA( int x, int y, unsigned char caracter )
/*****************************************************************************/
{
    unsigned offset,of;
    unsigned char *dato;
    int h,color_fondo;
    offset = 80 * L_CAR * y + x + 3200;           // el 3200 es para centrarlo
    of = offset;
    dato = &_FONTS[caracter * 16 + 1];
    color_fondo = ( _ATRIBUTO & 0xf0 ) >> 4;
    /*--------------- dibuja fondo -----------------*/
    outport( 0x3ce, 0x0205 );
    outport( 0x3c4, 0x0f02 );
    outport( 0x3ce, 0xff08 );
    for( h = 0;h < L_CAR;h++ ) {
        _MEM_VGA[offset] = color_fondo;
        offset += 80;
    }
    outport( 0x3ce, 0x0205 );
    outport( 0x3c4, 0x0f02 );
    offset = of;
    for( h = 0;h < L_CAR;h++ ) {
        outport( 0x3ce, ( *dato << 8 ) | 8 );
        //a = _MEM_VGA[offset];
        _MEM_VGA[offset] = _ATRIBUTO;
        offset += 80;
        dato++;
    }
    /* outport( 0x3ce, 0x0005 ); outport( 0x3ce, 0xff08 ); */
}
/*****************************************************************************/
PUT_PIXEL( int x, int y, unsigned char color )
/*****************************************************************************/
{
    unsigned offset;
    unsigned char dato;
    offset = 80 * y + ( x >> 3 );
    dato = 1 << ( ( x & 0x07 ) ^ 0x07 );
    outport( 0x3ce, 0x0205 );
    outport( 0x3c4, 0x0f02 );
    outport( 0x3ce, ( dato << 8 ) | 8 );
    //a = _MEM_VGA[offset];
    _MEM_VGA[offset] = color;
}
/*****************************************************************************/
SHOW_BMP16( char *archivo )
/*****************************************************************************/
{
    unsigned h,colores;
    int y, handle, ok = 0, x;
    struct
    {
        char sign[2];
        unsigned size;
        char res2[14];
        unsigned ancho;
        char res3[2];
        unsigned alto;
        char res4[4];
        char bits;
        char res5[25];
    } cabecera;
    unsigned char buffer[1024];
    /*------------------ Abre el archivo ----------------------*/
    handle = _open( archivo, O_RDONLY );
    if( handle ) {
        /*--------------------------- Lee la cabecera --------------------------*/
        _read( handle, &cabecera, 54 );
        colores = ( unsigned )pow( 2, cabecera.bits );
        if( memcmp( cabecera.sign, "BM", 2 ) == 0 && colores == 16 ) {
            /*----------------------- Lee la tabla de colores ----------------------*/
            _read( handle, buffer, colores * 4 );
            /*--------------------------- Lee la imgen ----------------------------*/
            for( y = cabecera.alto;y > 0;y-- ) {
                _read( handle, buffer, cabecera.ancho / 2 );
                for( x = 0, h = 0;x < cabecera.ancho;x += 2, h++ ) {
                    PUT_PIXEL( x, y, ( buffer[h] & 0xf0 ) >> 4 );
                    PUT_PIXEL( x + 1, y, buffer[h] & 0x0f );
                }
            }
            ok = 1;
            /*------------------------- Cierra el archivo --------------------------*/
        }
        _close( handle );
    }
    return ( ok );
}
/*****************************************************************************/
DIBUJA_CURSOR_VGA( int x, int y, char estado, int atributo )
/*****************************************************************************/
{
    unsigned offset;
    #define BUF 38400
    offset = 80 * ( L_CAR * y + L_CAR - 2 ) + x + 3200;
    outport( 0x3c4, 0x0f02 );
    outport( 0x3ce, 0xff08 );
    if( estado ) {
        /*----------- mueve el byte a un buffer ----------*/
        outport( 0x3ce, 0x0105 );
        _MEM_VGA[BUF] = _MEM_VGA[offset];
        /*--------------- Dibuja cursor ------------*/
        outport( 0x3ce, 0x0205 );
        //a = _MEM_VGA[offset];
        _MEM_VGA[offset] = atributo;
    }
    else {
        /*---------- Recupera el byte -----------*/
        outport( 0x3ce, 0x0105 );
        //a = _MEM_VGA[offset];
        //a = _MEM_VGA[BUF];
        _MEM_VGA[offset] = _MEM_VGA[BUF];
    }
}
#endif

