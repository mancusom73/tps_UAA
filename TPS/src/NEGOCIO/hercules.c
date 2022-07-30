#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <dbrouter.h> 
#include <malloc.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifdef COMPILAR_HERCULES
#undef MK_FP
extern char _ATRIBUTO;
static char *BUFFER_CHAR = NULL;
static char eliminar_arriba[256];
static unsigned char *_MEM = ( char * )0xb0000000;
/*************************************************************************/
INIT_HERCULES()
/*************************************************************************/
{
    int h;
    unsigned char datos[] = {0x35,0x2d,0x2e,0x07,0x5b,0x02,0x57,0x57,0x02,0x03,0x00,0x00};
    outportb( 0x3bf, 3 );
    outportb( 0x3b8, 0x02 );
    for( h = 0;h < 12;h++ ) {
        outportb( 0x3b4, h );
        outportb( 0x3b5, datos[h] );
    }
    outportb( 0x3b8, 0x0a );
    memset( _MEM, 0, 32768 );
}
/*****************************************************************************/
PUT_GRAF_CHAR_HERCULES( int x, int y, unsigned char caracter )
/*****************************************************************************/
{
    unsigned offset;
    unsigned char *dato;
    int h;
    x += 5;
    y *= 14;
    dato = &BUFFER_CHAR[caracter * 16 + 1];
    if( eliminar_arriba[caracter] ) {
        dato += 2;
    }
    for( h = 0;h < 14;h++, y++ ) {
        offset = 0x2000 * ( y % 4 ) + 90 * ( y / 4 ) + x;
        if( ( _ATRIBUTO & 0x70 ) == 0x70 ) {
            _MEM[offset] = ( char )( ~( *dato ) );
        }
        else {
            _MEM[offset] = *dato;
        }
        dato++;
    }
}
/*****************************************************************************/
HERCULES_LOADFONT()
/*****************************************************************************/
{
    int handle,h;
    int especiales[] = {2,3,6,7,10,11,15,16,18,19,22,23,26,27,125,126,129,131,134,135,139,143,146,
                        150,151,152,155,158,159,171,172,175,177,182,183,186,187,190,193,198,199,202,
                        203,206,207,210,211,214,215,220,221,224,225,228,229,232,233,236,237,240,241,
                        244,245,248,249,252,253,0};
    if( !BUFFER_CHAR ) {
        BUFFER_CHAR = ( char* )MALLOC( 4100 );
        handle = _open( "cr.fnt", O_RDONLY );
        if( handle > 0 && BUFFER_CHAR ) {
            read( handle, BUFFER_CHAR, 4100 );
            _close( handle );
        }
        /*-------------- Carga la tabla de caracteres especiales ------------*/
        memset( eliminar_arriba, 1, sizeof( eliminar_arriba ) );
        for( h = 0;especiales[h];h++ ) {
            eliminar_arriba[especiales[h]] = 0;
        }
    }
}
/*****************************************************************************/
DIBUJA_CURSOR_HERCULES( int x, int y, char estado )
/*****************************************************************************/
{
    unsigned offset;
    static unsigned char byte_ant;
    y = y * 14 + 13;
    offset = 0x2000 * ( y % 4 ) + 90 * ( y / 4 ) + x + 5;
    if( estado ) {
        byte_ant = _MEM[offset];
        _MEM[offset] = ~_MEM[offset];
    }
    else {
        _MEM[offset] = byte_ant;
    }
}
#endif

