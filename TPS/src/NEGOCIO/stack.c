#pragma pack(1)
#include <tpv_dat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <global.h>
#include <abort.h> 
#include <cmemory.h> 
#include <cr.h> 
#include <ini.h> 
#include <dd.h> 
#include <log.h> 
#include <stack.h> 
#include <_dd.h> 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */
#undef MK_FP
#ifdef INVEL_L
#else
#endif
extern long DD_OFFSET( int var );
/*****************************************************************************/
void STACK_INIT()
/*****************************************************************************/
{
    CEREAR_MEMORY( __stack_puntero );
    CEREAR_MEMORY( __stack_handle );
}
/*****************************************************************************/
int STACK_INS( int bytes )
/*****************************************************************************/
{
    char aux[80];
    int handle,valor;
    unsigned offset;
    handle = STACK_HANDLE + DD_ULTIMO_HANDLE() + 1;
    if( STACK_PUNTERO + bytes > L_STACK_RECUP ) {
        ABORT( "ERROR FATAL EN STACK: No queda memoria" );
    }
    else if( handle > DD_MAX_HANDLE() ) {
        ABORT( "ERROR FATAL EN STACK: No quedan handles" );
    }
    else {
        ADD_MEMORY_INT( __stack_puntero, bytes );
        ADD_MEMORY_INT( __stack_handle, 1 );
        offset = DD_OFFSET( __stack_datos ) + STACK_PUNTERO - bytes;
        DD_INS( handle, DD_CLASE( __stack_datos ), offset, bytes, 1, 0 );
        if( bytes == 2 ) {
            valor = GET_MEMORY_INT( handle );
            if( valor ) {
                _snprintf(aux, sizeof(aux)-1, "Stack: %i %i", handle, valor );
                GRABAR_LOG_SISTEMA( aux ,LOG_VENTAS,1);
            }
        }
    }
    return ( handle );
}
/*****************************************************************************/
void STACK_DEL( int bytes )
/*****************************************************************************/
{
    ADD_MEMORY_INT( __stack_puntero, -bytes );
    ADD_MEMORY_INT( __stack_handle, -1 );
    if( STACK_PUNTERO < 0 ) {
        SET_MEMORY_INT( __stack_puntero, 0 );
    }
}
/*****************************************************************************/
int STACK_GET_POINTER()
/*****************************************************************************/
{
    return ( STACK_PUNTERO );
}
/*****************************************************************************/
void STACK_SET_POINTER( int puntero )
/*****************************************************************************/
{
    SET_MEMORY_INT( __stack_puntero, puntero );
}
/*****************************************************************************/
int STACK_GET_HANDLE()
/*****************************************************************************/
{
    return ( STACK_HANDLE );
}
/*****************************************************************************/
void STACK_SET_HANDLE( int handle )
/*****************************************************************************/
{
    SET_MEMORY_INT( __stack_handle, handle );
}
/*****************************************************************************/
void STACK_CLEAR( int handle_desde )
/*****************************************************************************/
{
    int h;
    for( h = handle_desde + 1;h <= STACK_HANDLE;h++ ) {
        CEREAR_MEMORY( h + DD_ULTIMO_HANDLE() );
    }
}
/*****************************************************************************/
void OPEN_RECUPERO( int *ptr, int *handle )
/*****************************************************************************/
{
    *ptr = STACK_GET_POINTER();
    *handle = STACK_GET_HANDLE();
    /*
     * {
     * char mens[80];
     * sprintf( mens, "P: %i H: %i", STACK_GET_POINTER(), STACK_GET_HANDLE() );
     * GRABAR_LOG_DEBUG( mens );
     * }
     */
}
/*****************************************************************************/
void CLOSE_RECUPERO( int ptr, int handle )
/*****************************************************************************/
{
    STACK_CLEAR( handle );
    STACK_SET_POINTER( ptr );
    STACK_SET_HANDLE( handle );
}
/*****************************************************************************/
void STACK_INIT_DATOS()
/*****************************************************************************/
{
    CEREAR_MEMORY( __stack_datos );
}
/*****************************************************************************/
void CEREO_STACK()
/*****************************************************************************/
{
    CEREAR_MEMORY( __stack_puntero );
    CEREAR_MEMORY( __stack_handle );
    CEREAR_MEMORY( __stack_datos );
}

