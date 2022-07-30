#pragma pack (1) 
//#ifdef MODULO_PROMOCIONES

#include "comstack.h"
#include <stdlib.h>
#include <stdio.h>

#pragma pack (1)


cstack cs_push( cstack cs, comm c )
{
    cstack tmp;
    tmp = ( cstack )malloc( sizeof( struct cstack ) );
    tmp->top = c;
    tmp->next = cs;
    return tmp;
}

comm cs_pop( cstack *cs )
{
    cstack tmp;
    comm r;
    r = ( *cs )->top;
    tmp = *cs;
    *cs = ( *cs )->next;
    free( tmp );
    return r;
}

cstack cs_free( cstack cs )
{
    cstack ctmp;

    while( cs ) {
        ctmp = cs;
        cs = cs->next;
        free( ctmp );
    }

    return cs;
}

//#endif

