#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#ifdef INVEL_W
#include <windows.h>
#endif
#include <zlib32b.h> 
#include <cr.h> 
#include <log.h> 
#include <mensaje.h> 
#include <zip.h> 
#include <ini.h>
///////////////////////////////////////////////////
//  Sistema: TPV 1000 - IV
//  Funciones para comprimir y descomprimir archivos
//  en formato Gzip compatible con linux
////////////////////////////////////////////////////
/////////////////////////////////////////////////////
//  Comprime un archivo especifico
//  Retorna 1 en caso de exito y 0 en caso de error
/////////////////////////////////////////////////////
int COMPRIMIR( char *filename )
{
    int rta = 0;
    char *MsgError;
    char StrLog[80];
    #if defined(INVEL_L)
    char aux[100];
    #endif
    #if defined(INVEL_W)
    MsgError = Gzip( filename );
    if( MsgError == NULL ) {
        rta = 1;
    }
    else {
        sprintf( StrLog, "Error al comprimir archivo: %s  Descripcion: %s", filename, MsgError );
        GRABAR_LOG_SISTEMA( StrLog, LOG_VARIOS,2);
        MENSAJE( "NO SE PUDO COMPRIMIR EL ARCHIVO!" );
    }
    #else
    sprintf( aux, "gzip %s", filename );
    rta = system( aux );
    if( rta != -1 ) {
        rta = 1;
    }
    else {
        sprintf( StrLog, "Error al comprimir archivo: %s  Descripcion: %s", filename, MsgError );
        GRABAR_LOG_SISTEMA( StrLog );
        MENSAJE( "NO SE PUDO COMPRIMIR EL ARCHIVO!" );
    }
    #endif
    return rta;
}
/////////////////////////////////////////////////////
//  Descomprime un archivo especifico
//  Retorna 1 en caso de exito y 0 en caso de error
/////////////////////////////////////////////////////
int DESCOMPRIMIR( char *filename )
{
    int rta = 0;
    char *MsgError;
    char StrLog[80];
    #if defined(INVEL_L)
    char aux[100];
    #endif
    #if defined(INVEL_W)
    MsgError = Gunzip( filename );
    if( MsgError == NULL ) {
        rta = 1;
    }
    else {
        sprintf( StrLog, "Error al descomprimir archivo: %s  Descripcion: %s", filename, MsgError );
        GRABAR_LOG_SISTEMA( StrLog ,LOG_VARIOS,2);
    }
    #else
    sprintf( aux, "gunzip %s", filename );
    rta = system( aux );
    if( rta != -1 ) {
        rta = 1;
    }
    else {
        sprintf( StrLog, "Error al comprimir archivo: %s  Descripcion: %s", filename, MsgError );
        GRABAR_LOG_SISTEMA( StrLog );
        MENSAJE( "NO SE PUDO COMPRIMIR EL ARCHIVO!" );
    }
    #endif
    return rta;
}

