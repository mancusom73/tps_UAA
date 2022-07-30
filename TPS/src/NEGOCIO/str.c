#pragma pack(1)
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
//#include <stdlib.h>
#include <global.h>
#include <tpv_dat.h>
#include <str.h>
#include <stdlib.h>
#include <string.h>
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 *
 * ------------------------------------------------------------------------------
 */
#ifdef INVEL_L
/*
 *  Devuelve un string ivertido
 *  @s : El string a invertir
 */
/***************************************************************************/
char * _strrev( char *s )
/***************************************************************************/
{
    int l = strlen( s );
    char *str = ( char * )malloc( l + 1 );
    int i;
    for( i = 0;i < l;i++ ) {
        str[i] = s[l - i - 1];
    }
    str[l] = '\0';
    return str;
}
/***************************************************************************/
char * _itoa( int value, char *s, int radix )
/***************************************************************************/
{
    if( radix != 10 ) {
        printf( "Error: _itoa : Funcionalidad no implementada!" );
        exit( 1 );
    }
    sprintf( s, "%i", value );
    return s;
}
/***************************************************************************/
char * _ltoa( long value, char *s, int radix )
/***************************************************************************/
{
    if( radix != 10 ) {
        printf( "Error: _itoa : Funcionalidad no implementada!" );
        exit( 1 );
    }
    sprintf( s, "%li", value );
    return s;
}
/***************************************************************************/
char * strset( char *s, char ch )
/***************************************************************************/
{
    int l = strlen( s );
    int i;
    for( i = 0;i < l;i++ ) {
        s[i] = ch;
    }
    return s;
}
/***************************************************************************/
char * _strupr( char *s )
/***************************************************************************/
{
    int l = strlen( s );
    int i;
    for( i = 0;i < l;i++ ) {
        s[i] = ( char )toupper( s[i] );
    }
    return s;
}
//(Nota Unific MySQL-Linux): se deja esta version, la de Linux que es menos propensa al memory leak
/***************************************************************************/
char * _strlwr( char *s )
/***************************************************************************/
{
    int l = strlen( s );
    int i;
    //char *c = ( char * )malloc( l + 1 );
    //memset( c, 0, l + 1 );
    for( i = 0;i < l;i++ ) {
        s[i] = ( char )tolower( s[i] );
    }
    return s;
}
#endif
