/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Definci¢n de Funciones gr ficas genericas para modos VGA X.
 * ------------------------------------------------------------------------------
 */

#ifndef __VGAGRAF_H__
#define __VGAGRAF_H__

#define MODO_80x25_16c      0x003
#define MODO_640x480_256c   0x101
#define MODO_800x600_16c    0x6A
#define MODO_800x600_256c   0x103

void MarcoGris( int Left, int Top, int Right, int Bottom );

//void Placa3DDN(int Left, int Top, int Right, int Bottom);
void Placa3DDN( int Left, int Top, int Right, int Bottom, int Color );
void Marco3DDN( int Left, int Top, int Right, int Bottom );
void Marco3DUP( int Left, int Top, int Right, int Bottom );

//void Placa3DUP(int Left, int Top, int Right, int Bottom);
void Placa3DUP( int Left, int Top, int Right, int Bottom, int Color );
void Rectangle( int left, int top, int right, int bottom, int color );
void Line( int x1, int y1, int x2, int y2, unsigned char lpcol );
void Bitmap( int x, int y, char *archivo );
void PutPixel( int x, int y, unsigned int color );
void SetVideoMode( int Mode );
void ErrorFatal( char *msg );

#endif
