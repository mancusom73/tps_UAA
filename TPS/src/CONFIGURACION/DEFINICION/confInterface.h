/*
 *   Sistema: TPV 1000 - IV
 *
 *   Funciones para interconectar los objetos de "Configuracion" en C++ con C
 *
 *   @author: Nicolas Zandarin (2007)
 */
#pragma once

#include "confTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
    int             CARGAR_CONFTPV();
    CTYPE_INT       GET_CONFIG_INT( int lugar );
    CTYPE_FLOAT     GET_CONFIG_FLOAT( int lugar );
    CTYPE_DOUBLE    GET_CONFIG_DOUBLE( int lugar );
    CTYPE_UNSIGNED  GET_CONFIG_UNSIGNED( int lugar );
    CTYPE_LONG      GET_CONFIG_LONG( int lugar );
    CTYPE_ULONG     GET_CONFIG_ULONG( int lugar );
    CTYPE_STRING    GET_CONFIG_STR( int lugar );
    CTYPE_CHAR      GET_CONFIG_CHAR( int lugar );
    int             GET_CONFIG_STR_LENGHT( int lugar );
#ifdef __cplusplus
}
#endif