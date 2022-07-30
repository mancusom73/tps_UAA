#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <bitmap.h> 
#include <cr.h>  
#include <dbrouter.h> 
#include <log.h> 
#include <vgagraf.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Implementacion de las Funciones para el manejo de Bitmaps.
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_DYNAKEY
#include <dos.h>
#include <stdarg.h>
#include <string.h>
#ifndef  INVEL_W
#include <bios.h>
#include <alloc.h>
#endif
extern unsigned char *_MEM_VGA;
void LoadBitmap( char *FileName, BITMAP *Bmp )
{
    UINT Bytes;
    int handle;
    LONG Offset;
    BITMAPINFOHEADER bmih;
    BITMAPFILEHEADER bmfh;
    strcpy( Bmp->BitmapFile, FileName );
    Bmp->Pixel = NULL;
    handle = _open( FileName, O_RDONLY );
    if( handle != -1 ) {
        _read( handle, &bmfh, sizeof( struct tagBITMAPFILEHEADER ) );
        _read( handle, &bmih, sizeof( struct tagBITMAPINFOHEADER ) );
        memcpy( &Bmp->bmih, &bmih, sizeof( struct tagBITMAPINFOHEADER ) );
        if( memcmp( &bmfh.bfType, "BM", 2 ) == 0 && bmih.biBitCount == 4
         && bmih.biCompression == 0 ) {
            Offset = bmfh.bfOffBits;
            _lseek( handle, Offset, SEEK_SET );
            Bytes = bmfh.bfSize - bmfh.bfOffBits;
            if( ( Bmp->Pixel = ( BYTE * )malloc( Bytes + 1 ) ) != NULL ) {
                if( ( _read( handle, Bmp->Pixel, Bytes ) ) == -1 ) {
                    exit( 1 );
                };
            };
            _close( handle );
        }
    }
    else {
        char auxi[100];
        sprintf( auxi, "bitmap.c  error al abrir %s ", FileName );
        GRABAR_LOG_SISTEMA( auxi );
    };
};
void PrepareColorTable( UINT ColorTable[16], BOOLEAN Active )
{
    switch( Active ) {
        case TRUE:
            ColorTable[0] = 0;
            ColorTable[1] = 4;
            ColorTable[2] = 2;
            ColorTable[3] = 6;
            ColorTable[4] = 1;
            ColorTable[5] = 5;
            ColorTable[6] = 3;
            ColorTable[7] = 7;
            ColorTable[8] = 8;
            ColorTable[9] = 12;
            ColorTable[10] = 10;
            ColorTable[11] = 14;
            ColorTable[12] = 9;
            ColorTable[13] = 13;
            ColorTable[14] = 11;
            ColorTable[15] = 15;
            break;
        case FALSE:
            ColorTable[0] = 15;
            ColorTable[1] = 15;
            ColorTable[2] = 15;
            ColorTable[3] = 15;
            ColorTable[4] = 15;
            ColorTable[5] = 15;
            ColorTable[6] = 15;
            ColorTable[7] = 7;
            ColorTable[8] = 8;
            ColorTable[9] = 8;
            ColorTable[10] = 8;
            ColorTable[11] = 8;
            ColorTable[12] = 8;
            ColorTable[13] = 8;
            ColorTable[14] = 8;
            ColorTable[15] = 8;
            break;
    };
};
void ShowBitmap( int x, int y, BITMAP *Bmp, BOOLEAN Active )
{
    UINT ColorTable[16];
    UINT h = 0;
    UINT ii, yy, xx = 0;
    UINT Color;
    PrepareColorTable( ColorTable, Active );
    // si fue cargado , puede no haber sido encontrado;
    if( Bmp->Pixel ) {
        for( yy = Bmp->bmih.biHeight;yy > 0;yy-- ) {
            for( xx = 0;xx < ( unsigned )Bmp->bmih.biWidth;xx++ ) {
                for( ii = 0;ii < 3;ii++ ) {
                    if( xx < ( unsigned )Bmp->bmih.biWidth ) {
                        Color = ( ( Bmp->Pixel[h] & 0xf0 ) >> 4 );
                        PutPixel( x + xx, y + yy, ColorTable[Color] );
                    };
                    xx++;
                    if( xx < ( unsigned )Bmp->bmih.biWidth ) {
                        Color = ( Bmp->Pixel[h] & 0x0f );
                        PutPixel( x + xx, y + yy, ColorTable[Color] );
                    };
                    xx++;
                    h++;
                };
                if( xx < ( unsigned )Bmp->bmih.biWidth ) {
                    Color = ( ( Bmp->Pixel[h] & 0xf0 ) >> 4 );
                    PutPixel( x + xx, y + yy, ColorTable[Color] );
                };
                xx++;
                if( xx < ( unsigned )Bmp->bmih.biWidth ) {
                    Color = ( Bmp->Pixel[h] & 0x0f );
                    PutPixel( x + xx, y + yy, ColorTable[Color] );
                };
                h++;
            };
        };
    };
};
void ShowBitmapEx( int x, int y, char *FileName, BOOLEAN Active )
{
    UINT ColorTable[16];
    UINT h = 0;
    UINT ii, yy, xx = 0;
    UINT Color;
    BITMAP Bmp;
    LoadBitmap( FileName, &Bmp );
    PrepareColorTable( ColorTable, Active );
    for( yy = Bmp.bmih.biHeight;yy > 0;yy-- ) {
        for( xx = 0;xx < ( unsigned )Bmp.bmih.biWidth;xx++ ) {
            for( ii = 0;ii < 3;ii++ ) {
                if( xx < ( unsigned )Bmp.bmih.biWidth ) {
                    Color = ( ( Bmp.Pixel[h] & 0xf0 ) >> 4 );
                    PutPixel( x + xx, y + yy, ColorTable[Color] );
                };
                xx++;
                if( xx < ( unsigned )Bmp.bmih.biWidth ) {
                    Color = ( Bmp.Pixel[h] & 0x0f );
                    PutPixel( x + xx, y + yy, ColorTable[Color] );
                };
                xx++;
                h++;
            };
            if( xx < ( unsigned )Bmp.bmih.biWidth ) {
                Color = ( ( Bmp.Pixel[h] & 0xf0 ) >> 4 );
                PutPixel( x + xx, y + yy, ColorTable[Color] );
            };
            xx++;
            if( xx < ( unsigned )Bmp.bmih.biWidth ) {
                Color = ( Bmp.Pixel[h] & 0x0f );
                PutPixel( x + xx, y + yy, ColorTable[Color] );
            };
            h++;
        };
    };
    if( Bmp.Pixel)
    free( Bmp.Pixel );
};
#endif

