#pragma pack (push,1) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Proveer la definicion de las estructuras de un archivo BMP de Windows.
 * ------------------------------------------------------------------------------
 */

#include "tipos.h"

#ifndef __BITMAP_H__
#define __BITMAP_H__


typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReverved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAP
{
    char BitmapFile[13];
    BITMAPINFOHEADER bmih;
    BYTE *Pixel;
} BITMAP;

void LoadBitmap( char *FileName, BITMAP *Bmp );
void ShowBitmap( int x, int y, BITMAP *Bmp, BOOLEAN Active );
void PrepareColorTable( UINT ColorTable[16], BOOLEAN Active );
void ShowBitmapEx( int x, int y, char *FileName, BOOLEAN Active );


// agregadas para que compile 
// ale
#define LIGHTGRAY 0
#define BLACK 100
#define WHITE   ((unsigned char)300)
#define DARKGRAY 200
#endif /* __BITMAP_H__ */
#pragma pack (pop) 
