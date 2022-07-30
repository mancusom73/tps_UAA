#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <cr.h> 
#include <malloc.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
/***************************************************************************/
void * MALLOC( unsigned memoria )
/***************************************************************************/
{
    void *puntero;
    puntero = malloc( memoria );
    if( puntero == NULL ) {
        //ERROR__( 99, "Error en MALLOC" );
    }
    return ( puntero );
}
/***************************************************************************/
void * MALLOC_CLEAR( unsigned memoria )
/***************************************************************************/
{
    void *puntero;
    puntero = MALLOC( memoria );
    if( puntero ) {
        memset( puntero, 0, memoria );
    }
    return ( puntero );
}
/***************************************************************************/
void FREEMEM( char *puntero )
/***************************************************************************/
{
    if( puntero ) {
        free( ( void* )puntero );
    }
}

