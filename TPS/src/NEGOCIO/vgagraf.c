#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
//#include <vgagraf.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_DYNAKEY
#if !defined(INVEL_W) && !defined(INVEL_L)
#endif
extern unsigned char *_MEM_VGA;
//static unsigned char *_MEM_VGA = (char *)0xA0000000;
void MarcoGris( int Left, int Top, int Right, int Bottom )
{
    Right -= 3;
    Top += 3;
    Left += 3;
    Bottom -= 3;
    Line( Right, Top, Right, Bottom, DARKGRAY );
    Line( Right, Bottom, Left, Bottom, DARKGRAY );
    Line( Left, Top, Right, Top, DARKGRAY );
    Line( Left, Bottom, Left, Top, DARKGRAY );
};
void Marco3DUP( int Left, int Top, int Right, int Bottom )
{
    Line( Right, Top, Right, Bottom, DARKGRAY );
    Line( Right, Bottom, Left, Bottom, DARKGRAY );
    Line( Right - 1, Top, Right - 1, Bottom - 1, DARKGRAY );
    Line( Right - 1, Bottom - 1, Left, Bottom - 1, DARKGRAY );
    Line( Left, Top, Right - 1, Top, WHITE );
    Line( Left, Bottom - 1, Left, Top, WHITE );
    Line( Left + 1, Top + 1, Right - 1, Top + 1, LIGHTGRAY );
    Line( Left + 1, Bottom - 1, Left + 1, Top + 1, LIGHTGRAY );
};
void Marco3DDN( int Left, int Top, int Right, int Bottom )
{
    Line( Left + 1, Top + 1, Right, Top + 1, DARKGRAY );
    Line( Left + 1, Bottom, Left + 1, Top + 1, DARKGRAY );
    Line( Left, Top, Right, Top, DARKGRAY );
    Line( Left, Bottom, Left, Top, DARKGRAY );
    Line( Right, Top + 1, Right, Bottom, WHITE );
    Line( Right, Bottom, Left + 1, Bottom, WHITE );
    Line( Right - 1, Top + 1, Right - 1, Bottom - 1, LIGHTGRAY );
    Line( Right - 1, Bottom - 1, Left + 1, Bottom - 1, LIGHTGRAY );
};
void Placa3DUP( int Left, int Top, int Right, int Bottom, int Color )
{
    Rectangle( Left, Top, Right, Bottom, Color );
    Line( Right, Top, Right, Bottom, DARKGRAY );
    Line( Right, Bottom, Left, Bottom, DARKGRAY );
    Line( Right - 1, Top, Right - 1, Bottom - 1, DARKGRAY );
    Line( Right - 1, Bottom - 1, Left, Bottom - 1, DARKGRAY );
    Line( Left, Top, Right - 1, Top, WHITE );
    Line( Left, Bottom - 1, Left, Top, WHITE );
};
void Placa3DDN( int Left, int Top, int Right, int Bottom, int Color )
{
    Rectangle( Left, Top, Right, Bottom, Color );
    Line( Left + 1, Top + 1, Right, Top + 1, DARKGRAY );
    Line( Left + 1, Bottom, Left + 1, Top + 1, DARKGRAY );
    Line( Left, Top, Right, Top, DARKGRAY );
    Line( Left, Bottom, Left, Top, DARKGRAY );
    Line( Right, Top + 1, Right, Bottom, WHITE );
    Line( Right, Bottom, Left + 1, Bottom, WHITE );
};
void SetVideoMode( int Mode )
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    union REGS ent,sal;
    ent.h.al = Mode;
    ent.h.ah = 0x00;
    int86( 0x10, &ent, &sal );
    ent.h.ah = 0x0F;
    int86( 0x10, &ent, &sal );
    if( sal.h.al != Mode ) {
        ErrorFatal( "No pudo cambiar modo" );
    };
    #endif
};
void ErrorFatal( char *msg )
{
    #if !defined(INVEL_W) && !defined(INVEL_L)
    union REGS ent;
    ent.h.al = MODO_80x25_16c;
    ent.h.ah = 0x00;
    int86( 10, &ent, &ent );
    #endif
    printf( "\nError: %s", msg );
    exit( 1 );
};
void Rectangle( int left, int top, int right, int bottom, int color )
{
    unsigned offset;
    unsigned char dato;
    int i,Faltan;
    int y,x;
    x = left;
    y = top;
    // Averiguar en que bit comienza a dibujar.
    dato = 7 - ( x % 8 );
    while( dato != 7 && x < right )               // Si es cero comienza un nuevo Byte.
    {
        dato = 1 << ( ( x & 0x07 ) ^ 0x07 );
        for( y = top;y < bottom;y++ ) {
            offset = y * 80 + ( int )( x / 8 );
            #if !defined(INVEL_W) && !defined(INVEL_L)
            outport( 0x3ce, 0x0205 );
            outport( 0x3c4, 0x0f02 );
            outport( 0x3ce, ( dato << 8 ) | 8 );
            _MEM_VGA[offset] = color;
            #endif
        };
        dato = 7 - ( ++x % 8 );
    };
    // Cuantos Bytes Completos faltan para terminar
    Faltan = ( int )( ( right - x ) / 8 );
    for( i = 0;i < Faltan;i++ ) {
        for( y = top;y < bottom;y++ ) {
            offset = y * 80 + ( int )( x / 8 );
            dato = 0xff;
            #if !defined(INVEL_W) && !defined(INVEL_L)
            outport( 0x3ce, 0x0205 );
            outport( 0x3c4, 0x0f02 );
            outport( 0x3ce, ( dato << 8 ) | 8 );
            _MEM_VGA[offset] = color;
            #endif
        };
        x += 8;
    };
    // Queda alguno por Pintar?
    while( x < right ) {
        dato = 1 << ( ( x & 0x07 ) ^ 0x07 );
        for( y = top;y < bottom;y++ ) {
            offset = y * 80 + ( int )( x / 8 );
            #if !defined(INVEL_W) && !defined(INVEL_L)
            outport( 0x3ce, 0x0205 );
            outport( 0x3c4, 0x0f02 );
            outport( 0x3ce, ( dato << 8 ) | 8 );
            _MEM_VGA[offset] = color;
            #endif
        };
        x++;
    };
};
void PutPixel( int x, int y, unsigned int color )
{
    unsigned offset;
    unsigned char dato;
    offset = y * 80 + ( int )( x / 8 );
    dato = 1 << ( ( x & 0x07 ) ^ 0x07 );
    #if !defined(INVEL_W) && !defined(INVEL_L)
    outport( 0x3ce, 0x0205 );
    outport( 0x3c4, 0x0f02 );
    outport( 0x3ce, ( dato << 8 ) | 8 );
    _MEM_VGA[offset] = color;
    #endif
}
void SwapInt( int *i1, int *i2 )
{
    int dummy;
    dummy = *i2;
    *i2 = *i1;
    *i1 = dummy;
}
void Line( int x1, int y1, int x2, int y2, unsigned char lpcol )
{
    int d,dx,dy,aincr,bincr,xincr,yincr,x,y;
    if( abs( x2 - x1 ) < abs( y2 - y1 ) )         /* X- or Y-axis overflow? *//* Check
    {
    * * * Y-axis */ {
        if( y1 > y2 )                             /* y1 > y2? */ {
            SwapInt( &x1, &x2 );                  /* Yes --> Swap X1 with X2 */
            SwapInt( &y1, &y2 );                  /* and Y1 with Y2  */
        }
    }
    xincr = ( x2 > x1 ) ? 1 : -1;             /* Set X-axis increment */
    dy = y2 - y1;
    dx = abs( x2 - x1 );
    d = 2 * dx - dy;
    aincr = 2 * ( dx - dy );
    bincr = 2 * dx;
    x = x1;
    y = y1;
    PutPixel( x, y, lpcol );
    for( y = y1 + 1;y <= y2;++y )           /* Execute line on Y-axes */ {
        if( d >= 0 ) {
            x += xincr;
            d += aincr;
        }
        else {
            d += bincr;
        }
        PutPixel( x, y, lpcol );
    }
}                                             /* Check X-axes */
else {
    if( x1 > x2 )                             /* x1 > x2? */ {
        SwapInt( &x1, &x2 );                  /* Yes --> Swap X1 with X2 */
        SwapInt( &y1, &y2 );                  /* and Y1 with Y2  */
    }
    yincr = ( y2 > y1 ) ? 1 : -1;             /* Set Y-axis increment */
    dx = x2 - x1;
    dy = abs( y2 - y1 );
    d = 2 * dy - dx;
    aincr = 2 * ( dy - dx );
    bincr = 2 * dy;
    x = x1;
    y = y1;
    PutPixel( x, y, lpcol );                  /* Set first pixel */
    for( x = x1 + 1;x <= x2;++x )           /* Execute line on X-axes */ {
        if( d >= 0 ) {
            y += yincr;
            d += aincr;
        }
        else {
            d += bincr;
        }
        PutPixel( x, y, lpcol );
    }
}
}
#endif

