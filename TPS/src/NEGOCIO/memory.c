/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#include <stdlib.h>
#ifndef INVEL_L
#include <dos.h>
#pragma pack(1)
#endif
#include <cr.h>
#include <_dd.h>
#include <driver.h>
extern long DD_OFFSET( int var );
struct _dd
{
    char clase;
    long offset;
    unsigned largo;
    unsigned elementos;
    char tipo_variable;
};
extern struct _dd *_DD;
void _ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int tipo );
char aux_temp[256];
void _ADD_BUFFER_TO_STRUCT( char *origen, int var, int sub, int tipo );
void _SET_MEMORY_STR_CHAR( int var, int subindice, int elemento, char valor );
void _SET_MEMORY_DOUBLE( int var, int subindice, double valor );
void _SET_MEMORY_INT( int var, int subindice, int valor );
void _SET_MEMORY_CHAR( int var, int subindice, int valor );
void _SET_MEMORY_LONG( int var, int subindice, long valor );
void _SET_MEMORY_FLOAT( int var, int subindice, double valor );
/*****************************************************************************/
void _LOAD_MEMORY( int var, int subindice, char *valor )
/*****************************************************************************/
{
    long offset = 0;    
    if( subindice > ( signed )_DD[var].elementos ) {
        char cadena[90];
        sprintf( cadena, "DD Error: Subindice overflow en variable %i (max:%i, req:%i)", var,
                 _DD[var].elementos, subindice );
        ABORT( cadena );
    }
    offset = _DD[var].offset + _DD[var].largo * subindice;
    _DD_EXEC( _DM_GET, _DD[var].clase, offset, valor, _DD[var].largo );
}
/*****************************************************************************/
char _GET_MEMORY_STR_CHAR( int var, int subindice, int elemento )
/*****************************************************************************/
{
    char aux;
    long offset;
    offset = DD_OFFSET( var ) + DD_LARGO( var ) * subindice + elemento;
    _DD_EXEC( _DM_GET, DD_CLASE( var ), offset, &aux, 1 );
    return ( aux );
}
/*****************************************************************************/
void _SET_MEMORY( int var, int subindice, char *valor )
/*****************************************************************************/
{
    DD_EXEC( _DM_SET, var, subindice, valor );
}
/*****************************************************************************/
void _CEREAR_MEMORY( int var, int subindice )
/*****************************************************************************/
{
    DD_EXEC( _DM_CER, var, subindice, NULL );
}
/*****************************************************************************/
char GET_MEMORY_STR_CHAR( int var, int elemento )
/*****************************************************************************/
{
    char a;
    a = _GET_MEMORY_STR_CHAR( var, 0, elemento );
    return ( a );
}
/*****************************************************************************/
char * GET_MEMORY_STR( int var )
/*****************************************************************************/
{
	memset(aux_temp,0,256);
    _LOAD_MEMORY( var, 0, aux_temp );
    return ( aux_temp );
	}
/*****************************************************************************/
char * _GET_MEMORY_STR( int var, int subindice )
/*****************************************************************************/
{
	memset(aux_temp,0,256);
    _LOAD_MEMORY( var, subindice, aux_temp );
    return ( aux_temp );
}
/*****************************************************************************/
void _COPY_MEMORY( int destino, int sub_des, int origen, int sub_ori )
/*****************************************************************************/
{
    char aux[256];
    _LOAD_MEMORY( origen, sub_ori, aux );
    _SET_MEMORY( destino, sub_des, aux );
}
/*****************************************************************************/
int SIZEOF_VAR( int var )
/*****************************************************************************/
{
    return ( DD_LARGO( var ) );
}
/*****************************************************************************/
int SIZEOF_ARRAY( int var )
/*****************************************************************************/
{
    return ( DD_LARGO( var ) * DD_ELEMENTOS( var ) );
}
/*****************************************************************************/
void SET_MEMORY( int var, char *valor )
/*****************************************************************************/
{
    _SET_MEMORY( var, 0, valor );
}
/*****************************************************************************/
void CEREAR_MEMORY( int var )
/*****************************************************************************/
{
    _CEREAR_MEMORY( var, 0 );
}
/*****************************************************************************/
void COPY_MEMORY( int destino, int origen )
/*****************************************************************************/
{
    _COPY_MEMORY( destino, 0, origen, 0 );
}
/*****************************************************************************/
void CEREAR_ARRAY( int cod )
/*****************************************************************************/
{
    int i;
    for( i = 0;i < DD_ELEMENTOS( cod );i++ ) {
        _CEREAR_MEMORY( cod, i );
    }
}
/*
 * -----------------------------------------------------------------------------
 * COLOCAR VALORES EN LA MEMORIA
 * -----------------------------------------------------------------------------
 */
/*****************************************************************************/
void SET_MEMORY_STR_CHAR( int var, int elemento, char valor )
/*****************************************************************************/
{
    _SET_MEMORY_STR_CHAR( var, 0, elemento, valor );
}
/*****************************************************************************/
void _SET_MEMORY_STR_CHAR( int var, int subindice, int elemento, char valor )
/*****************************************************************************/
{
    long offset;
    offset = DD_OFFSET( var ) + DD_LARGO( var ) * subindice + elemento;
    _DD_EXEC( _DM_SET, DD_CLASE( var ), offset, &valor, 1 );
}
#if defined(INVEL_W) || defined(INVEL_L)
/*****************************************************************************/
void SET_MEMORY_CHAR( int var, int valor )
/*****************************************************************************/
{
    _SET_MEMORY_CHAR( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_CHAR( int var, int subindice, int valor )
/*****************************************************************************/
{
    char v;
    v = ( char )valor;
    _SET_MEMORY( var, subindice, &v );
}
#else
/*****************************************************************************/
void SET_MEMORY_CHAR( int var, char valor )
/*****************************************************************************/
{
    _SET_MEMORY_CHAR( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_CHAR( int var, int subindice, char valor )
/*****************************************************************************/
{
    _SET_MEMORY( var, subindice, &valor );
}
#endif
/*****************************************************************************/
void SET_MEMORY_INT( int var, int valor )
/*****************************************************************************/
{
    _SET_MEMORY_INT( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_INT( int var, int subindice, int valor )
/*****************************************************************************/
{
    short int v;
    v = ( short int ) valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
/*****************************************************************************/
void SET_MEMORY_LONG( int var, long valor )
/*****************************************************************************/
{
    _SET_MEMORY_LONG( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_LONG( int var, int subindice, long valor )
/*****************************************************************************/
{
    _SET_MEMORY( var, subindice, ( char* )&valor );
}
#if defined(INVEL_W) || defined(INVEL_L)
/*****************************************************************************/
void SET_MEMORY_FLOAT( int var, double valor )
/*****************************************************************************/
{
    _SET_MEMORY_FLOAT( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_FLOAT( int var, int subindice, double valor )
/*****************************************************************************/
{
    float v;
    v = ( float )valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
#else
/*****************************************************************************/
void SET_MEMORY_FLOAT( int var, float valor )
/*****************************************************************************/
{
    _SET_MEMORY_FLOAT( var, 0, valor );
}
/*****************************************************************************/
void _SET_MEMORY_FLOAT( int var, int subindice, float valor )
/*****************************************************************************/
{
    float v;
    v = ( float )valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
#endif
/*****************************************************************************/
void SET_MEMORY_DOUBLE( int var, double valor )
/*****************************************************************************/
{
    double v;
    v = ( double )valor;
    _SET_MEMORY_DOUBLE( var, 0, v );
}
/*****************************************************************************/
void _SET_MEMORY_DOUBLE( int var, int subindice, double valor )
/*****************************************************************************/
{
    double v;
    v = ( double )valor;
    _SET_MEMORY( var, subindice, ( char* )&v );
}
/*
 * -----------------------------------------------------------------------------
 * OBTENER VALORES DESDE LA MEMORIA
 * -----------------------------------------------------------------------------
 */
/*****************************************************************************/
char GET_MEMORY_CHAR( int var )
/*****************************************************************************/
{
    char aux;
    _LOAD_MEMORY( var, 0, &aux );
    return ( aux );
}
/*****************************************************************************/
char _GET_MEMORY_CHAR( int var, int subindice )
/*****************************************************************************/
{
    char aux;
    _LOAD_MEMORY( var, subindice, &aux );
    return ( aux );
}
/*****************************************************************************/
unsigned char GET_MEMORY_UCHAR( int var )
/*****************************************************************************/
{
    unsigned char aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
unsigned char _GET_MEMORY_UCHAR( int var, int subindice )
/*****************************************************************************/
{
    unsigned char aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
int GET_MEMORY_INT( int var )
/*****************************************************************************/
{
    short int aux = 0;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
int _GET_MEMORY_INT( int var, int subindice )
/*****************************************************************************/
{
    short int aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
unsigned GET_MEMORY_UNSIGNED( int var )
/*****************************************************************************/
{
    unsigned short aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
unsigned _GET_MEMORY_UNSIGNED( int var, int subindice )
/*****************************************************************************/
{
    unsigned short aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
long GET_MEMORY_LONG( int var )
/*****************************************************************************/
{
    long aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
long _GET_MEMORY_LONG( int var, int subindice )
/*****************************************************************************/
{
    long aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
unsigned long GET_MEMORY_ULONG( int var )
/*****************************************************************************/
{
    unsigned long aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
unsigned long _GET_MEMORY_ULONG( int var, int subindice )
/*****************************************************************************/
{
    unsigned long aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
float GET_MEMORY_FLOAT( int var )
/*****************************************************************************/
{
    float aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
float _GET_MEMORY_FLOAT( int var, int subindice )
/*****************************************************************************/
{
    float aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
double GET_MEMORY_DOUBLE( int var )
/*****************************************************************************/
{
    double aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    return ( aux );
}
/*****************************************************************************/
double _GET_MEMORY_DOUBLE( int var, int subindice )
/*****************************************************************************/
{
    double *aux,aux1;
    aux = ( double* )malloc( sizeof( double ) );
    if( aux != NULL ) {
        _LOAD_MEMORY( var, subindice, ( char* )aux );
        aux1 = *aux;
    }
    if( aux ) {
        free( aux );
    }
    return ( aux1 );
}
/*
 * -----------------------------------------------------------------------------
 * SUMAR VALORES EN LA MEMORIA
 * -----------------------------------------------------------------------------
 */
/*****************************************************************************/
void ADD_MEMORY_CHAR( int var, char valor )
/*****************************************************************************/
{
    char aux;
    _LOAD_MEMORY( var, 0, &aux );
    aux += valor;
    _SET_MEMORY( var, 0, &aux );
    //_ADD_MEMORY_CHAR( var, 0, valor );
}
/*****************************************************************************/
void _ADD_MEMORY_CHAR( int var, int subindice, char valor )
/*****************************************************************************/
{
    char aux;
    _LOAD_MEMORY( var, subindice, &aux );
    aux += valor;
    _SET_MEMORY( var, subindice, &aux );
}
/*****************************************************************************/
void ADD_MEMORY_UCHAR( int var, unsigned char valor )
/*****************************************************************************/
{
    unsigned char aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_UCHAR( int var, int subindice, unsigned char valor )
/*****************************************************************************/
{
    unsigned char aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_INT( int var, int valor )
/*****************************************************************************/
{
    int aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_INT( int var, int subindice, int valor )
/*****************************************************************************/
{
    int aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_UINT( int var, unsigned int valor )
/*****************************************************************************/
{
    unsigned int aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_UINT( int var, int subindice, unsigned int valor )
/*****************************************************************************/
{
    unsigned int aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_LONG( int var, long valor )
/*****************************************************************************/
{
    long aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_LONG( int var, int subindice, long valor )
/*****************************************************************************/
{
    long aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_ULONG( int var, unsigned long valor )
/*****************************************************************************/
{
    unsigned long aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_ULONG( int var, int subindice, unsigned long valor )
/*****************************************************************************/
{
    unsigned long aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_FLOAT( int var, float valor )
/*****************************************************************************/
{
    float aux;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_FLOAT( int var, int subindice, float valor )
/*****************************************************************************/
{
    float aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*****************************************************************************/
void ADD_MEMORY_DOUBLE( int var, double valor )
/*****************************************************************************/
{
    double aux = 0.0;
    _LOAD_MEMORY( var, 0, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, 0, ( char* )&aux );
}
/*****************************************************************************/
void _ADD_MEMORY_DOUBLE( int var, int subindice, double valor )
/*****************************************************************************/
{
    double aux;
    _LOAD_MEMORY( var, subindice, ( char* )&aux );
    aux += valor;
    _SET_MEMORY( var, subindice, ( char* )&aux );
}
/*
 * --------------------------------------
 * E S T R U C T U R A S
 * --------------------------------------
 */
/*****************************************************************************/
int _APUNTAR_STRUCT( int cod )
/*****************************************************************************/
{
    int h;
    for( h = 0;EST_COD( h ) != 999 && EST_COD( h ) != cod;h++ ) {
        ;
    }
    return ( h );
}
/*****************************************************************************/
void CEREAR_STRUCT( int cod )
/*****************************************************************************/
{
    int h,i;
    for( h = _APUNTAR_STRUCT( cod );EST_COD( h ) == cod;h++ ) {
        for( i = 0;i < DD_ELEMENTOS( EST_VAR( h ) );i++ ) {
            _CEREAR_MEMORY( EST_VAR( h ), i );
        }
    }
}
/*****************************************************************************/
void COPY_STRUCT( int destino, int origen )
/*****************************************************************************/
{
    int h,i,po,pd;
    po = _APUNTAR_STRUCT( origen );
    pd = _APUNTAR_STRUCT( destino );
    for( h = 0;EST_COD( po + h ) == origen;h++ ) {
        for( i = 0;i < DD_ELEMENTOS( EST_VAR( pd + h ) );i++ ) {
            _COPY_MEMORY( EST_VAR( pd + h ), i, EST_VAR( po + h ), i );
        }
    }
}
/*****************************************************************************/
int SIZEOF_STRUCT( int cod )
/*****************************************************************************/
{
    int h, l = 0;
    for( h = _APUNTAR_STRUCT( cod );EST_COD( h ) == cod;h++ ) {
        l += DD_LARGO( EST_VAR( h ) ) * DD_ELEMENTOS( EST_VAR( h ) );
    }
    return ( l );
}
/*****************************************************************************/
int _SIZEOF_STRUCT( int cod )
/*****************************************************************************/
{
    int h, l = 0;
    for( h = _APUNTAR_STRUCT( cod );EST_COD( h ) == cod;h++ ) {
        l += DD_LARGO( EST_VAR( h ) );
    }
    return ( l );
}
/*****************************************************************************/
void COPY_STRUCT_TO_BUFFER( char *buffer, int origen )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    for( h = _APUNTAR_STRUCT( origen );EST_COD( h ) == origen;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v ) * DD_ELEMENTOS( v );
        _DD_EXEC( _DM_GET, DD_CLASE( v ), DD_OFFSET( v ), &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void COPY_BUFFER_TO_STRUCT( char *buffer, int destino )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    for( h = _APUNTAR_STRUCT( destino );EST_COD( h ) == destino;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v ) * DD_ELEMENTOS( v );
        _DD_EXEC( _DM_SET, DD_CLASE( v ), DD_OFFSET( v ), &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void _COPY_STRUCT_TO_BUFFER( char *buffer, int origen, int subindice )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    long offset;
    for( h = _APUNTAR_STRUCT( origen );EST_COD( h ) == origen;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v );
        offset = DD_OFFSET( v ) + l * subindice;
        _DD_EXEC( _DM_GET, DD_CLASE( v ), offset, &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void _COPY_BUFFER_TO_STRUCT( char *buffer, int destino, int subindice )
/*****************************************************************************/
{
    int h, pos = 0, v, l;
    long offset;
    for( h = _APUNTAR_STRUCT( destino );EST_COD( h ) == destino;h++ ) {
        v = EST_VAR( h );
        l = DD_LARGO( v );
        offset = DD_OFFSET( v ) + l * subindice;
        _DD_EXEC( _DM_SET, DD_CLASE( v ), offset, &buffer[pos], l );
        pos += l;
    }
}
/*****************************************************************************/
void ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int estructura )
/*****************************************************************************/
{
    int h, pos = 0, v, s;
    for( h = _APUNTAR_STRUCT( estructura );EST_COD( h ) == estructura;h++ ) {
        v = EST_VAR( h );
        for( s = 0;s < DD_ELEMENTOS( v );s++ ) {
            _ADD_BUFFER_TO_BUFFER( &origen[pos], &destino[pos], DD_TIPO( v ) );
            pos += DD_LARGO( v );
        }
    }
}
/*****************************************************************************/
void _ADD_BUFFER_TO_BUFFER( char *origen, char *destino, int tipo )
/*****************************************************************************/
{
    switch( tipo ) {
        case _SHORT_INT:
            *destino += *origen;
            break;
        case _INT:
            *( int* )destino += *( int* )origen;
            break;
        case _UNSIGNED:
            *( unsigned * )destino += *( unsigned * )origen;
            break;
        case _LONG:
            *( long* )destino += *( long* )origen;
            break;
        case _FLOAT:
            *( float* )destino += *( float* )origen;
            break;
        case _DOUBLE:
            *( double* )destino += *( double* )origen;
            break;
    }
}
/*****************************************************************************/
int _APUNTAR_STRUCT_VERIFICANDO( int cod )
/*****************************************************************************/
{
    int h;
    for( h = 0;EST_COD( h ) != 999 && EST_COD( h ) != cod;h++ ) {
        ;
    }
    if( EST_COD( h ) == 999 ) {
        h = -1;
    }
    return ( h );
}
/*****************************************************************************/
void ADD_BUFFER_TO_STRUCT( char *origen, int destino )
/*****************************************************************************/
{
    int h, pos = 0, v, s;
    for( h = _APUNTAR_STRUCT( destino );EST_COD( h ) == destino;h++ ) {
        v = EST_VAR( h );
        for( s = 0;s < DD_ELEMENTOS( v );s++ ) {
            _ADD_BUFFER_TO_STRUCT( &origen[pos], v, s, DD_TIPO( v ) );
            pos += DD_LARGO( v );
        }
    }
}
/*****************************************************************************/
void _ADD_BUFFER_TO_STRUCT( char *origen, int var, int sub, int tipo )
/*****************************************************************************/
{
    char charvar;
    int intvar;
    unsigned unsvar;
    long longvar;
    float floatvar;
    double doublevar;
    switch( tipo ) {
        case _SHORT_INT:
            charvar = _GET_MEMORY_CHAR( var, sub ) + *origen;
            _SET_MEMORY_CHAR( var, sub, charvar );
            break;
        case _INT:
            intvar = _GET_MEMORY_INT( var, sub ) + *( int* )origen;
            _SET_MEMORY_INT( var, sub, intvar );
            break;
        case _UNSIGNED:
            unsvar = _GET_MEMORY_UNSIGNED( var, sub ) + *( unsigned * )origen;
            _SET_MEMORY_INT( var, sub, unsvar );
            break;
        case _LONG:
            longvar = _GET_MEMORY_LONG( var, sub ) + *( long* )origen;
            _SET_MEMORY_LONG( var, sub, longvar );
            break;
        case _FLOAT:
            floatvar = _GET_MEMORY_FLOAT( var, sub ) + *( float* )origen;
            _SET_MEMORY_FLOAT( var, sub, floatvar );
            break;
        case _DOUBLE:
            doublevar = _GET_MEMORY_DOUBLE( var, sub ) + *( double* )origen;
            _SET_MEMORY_DOUBLE( var, sub, doublevar );
            break;
    }
}

