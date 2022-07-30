#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <p_float.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
/******************************************************************************/
double PEDIR_GRAN_FLOTANTE( int x, int y, int enteros, int decimales )
/******************************************************************************/
{
    double importe;
    _GRANDE = 1;
    importe = PEDIR_FLOTANTE( x, y, enteros, decimales );
    _GRANDE = 0;
    return ( importe );
}
/******************************************************************************/
double PEDIR_GRAN_FLOTANTE_CON_SIGNO( int x, int y, int enteros, int decimales )
/******************************************************************************/
{
    double importe;
    _GRANDE = 1;
    importe = PEDIR_FLOTANTE_CON_SIGNO( ( char )x, ( char )y, ( char )enteros, ( char )decimales );
    _GRANDE = 0;
    return ( importe );
}
/******************************************************************************/
double PEDIR_GRAN_FLOTANTE_CON_VALOR( int x, int y, int enteros, int decimales, double valor )
/******************************************************************************/
{
    double importe;
    _GRANDE = 1;
    importe = PEDIR_FLOTANTE_CON_VALOR( ( char )x, ( char )y, ( char )enteros, ( char )decimales,
                                        valor );
    _GRANDE = 0;
    return ( importe );
}

