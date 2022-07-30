#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <ctime.h> 
#include <timer.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#ifndef INVEL_L
#endif
/***************************************************************************/
int INIT_TIMER()
/***************************************************************************/
{
    //int off,seg;
    //unsigned far *p, *_delay;
    /*--------- Cantidad de ciclos para 1 milisegundo = 1193 * 2 -------*/
    /*
     * p = MK_FP( FP_SEG( delay ), FP_OFF( delay ) );
     * seg = *(unsigned *)( (char *)p + 9);
     * off = *(unsigned *)( (char *)p + 32);
     * _delay = MK_FP( seg, off );
     * *_delay = 1193*2;
     */
    /*--- Se setea el counter 0 en modo 3 ( Square Wave Generator ) ---*/
    /*
     * outportb( 0x43, 0x36 );
     * outportb( 0x40, 0xff );
     * outportb( 0x40, 0xff );
     */
    return ( 1 );
}
/*****************************************************************************/
unsigned long TIEMPO_PASADO_DESDE( unsigned long *desde )
/*****************************************************************************/
{
    unsigned long t,dif;
    t = GET_TIME();
    if( t < *desde ) {
        dif = 0;
        *desde = t;
    }
    else {
        dif = t - *desde;
    }
    return ( dif );
}
/*****************************************************************************/
int YA_PASARON_SEGUNDOS( unsigned long *desde, int segundos )
/*****************************************************************************/
{
    int rta = 0;
    if( TIEMPO_PASADO_DESDE( desde ) > ( unsigned long ) ( segundos * 18.2 ) ) {
        rta = 1;
    }
    return ( rta );
}
/***************************************************************************/
void DELAY( int milisegundos )
/***************************************************************************/
{
    /*
     * int    centesimas, s, m, h;
     * long   i;
     * struct time time1, time2;
     *
     * centesimas = milisegundos / 10;
     * if( !centesimas ) centesimas=1;
     *
     * gettime( &time1 );
     *
     * for( i=0 ; i<centesimas; ){
     * gettime( &time2 );
     * i = (char)time2.ti_hund - (char)time1.ti_hund;
     * s = (char)time2.ti_sec - (char)time1.ti_sec;
     * i += s*100;
     * m = (char)time2.ti_min - (char)time1.ti_min;
     * i += (long)m*6000;
     * h = (char)time2.ti_hour - (char)time1.ti_hour;
     * i += (long)h * 6000 * 60;
     * }
     */
}

