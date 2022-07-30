#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <crypt.h> 
#include <get_hora.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifndef INVEL_L
#endif
/******************************************************************************/
int GET_HORA_DOS()
/******************************************************************************/
{
    //struct time t;
    unsigned hora, conversor = 1749;
    time_t now;
    struct tm f;
    //gettime(&t);
    CRYPT( __CONVERSOR_HORA, ( char* )&conversor );
    time( &now );
    f = *localtime( &now );
    hora = f.tm_hour * conversor;
    hora += f.tm_min;
    return ( hora );
}
/******************************************************************************/
long GET_HORA_COMPLETA_DOS()
/******************************************************************************/
{
    //struct time t;
    long hora;
    time_t now;
    struct tm f;
    //gettime(&t);
    time( &now );
    f = *localtime( &now );
    hora = f.tm_hour * 10000L;
    hora += f.tm_min * 100L;
    hora += f.tm_sec;
    return ( hora );
}
/******************************************************************************/
long GET_HORA_DOS_BYTE()
/******************************************************************************/
{
    //  struct time t;
    time_t now;
    struct tm f;
    unsigned long hora;
    time( &now );
    f = *localtime( &now );
    //gettime(&t);
    hora = ( ( ( long )f.tm_hour ) << 24 ) + ( ( ( long )f.tm_min ) << 16 )
         + ( ( ( long )f.tm_sec ) << 8 )
           //+ ((long) f.tm_hund)
           ;
    return ( hora );
}
/******************************************************************************/
long GET_HORA_SEGUNDOS()
/******************************************************************************/
{
    // struct time t;
    time_t now;
    struct tm f;
    long hora;
    // gettime( &t );
    time( &now );                                 /* Get time as long integer. */
    f = *localtime( &now );                       /* Convert to local time. */
    hora = f.tm_hour * 3600L;
    hora += f.tm_min * 60L;
    hora += f.tm_sec;
    return ( hora );
}

