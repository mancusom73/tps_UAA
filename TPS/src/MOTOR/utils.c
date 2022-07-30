#pragma pack (1) 

//#ifdef MODULO_PROMOCIONES

//#define PROMO_STR

#include "types2.h"
#include "codes.h"
#include "utils.h"
#include "etpv.h"

#pragma pack (1)

void expr_stack_push( expr c, expr_stack *cs )
{
    expr_stack new_cs;
    new_cs = ( expr_stack )malloc( sizeof( struct expr_stack ) );
    new_cs->head = c;
    new_cs->tail = *cs;
    *cs = new_cs;
}

expr expr_stack_pop( expr_stack *cs )
{
    expr_stack old_stack;
    expr head;
    if( *cs == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_POPEAR_EXPR_STACK_VACIO ) );
        exit( -1 );
    }
    old_stack = ( *cs );
    head = ( *cs )->head;
    *cs = ( *cs )->tail;
    free( old_stack );
    return head;
}


/***************************************/

void benef_stack_push( benef c, benef_stack *cs )
{
    benef_stack new_cs;
    new_cs = ( benef_stack )malloc( sizeof( struct benef_stack ) );
    new_cs->head = c;
    new_cs->tail = *cs;
    *cs = new_cs;
}

benef benef_stack_pop( benef_stack *cs )
{
    benef_stack old_stack;
    benef head;
    if( *cs == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_POPEAR_BENEF_STACK_VACIO ) );
        exit( -1 );
    }
    old_stack = *cs;
    head = ( *cs )->head;
    *cs = ( *cs )->tail;
    free( old_stack );
    return head;
}



/************************************************/

void param_stack_push( param c, param_stack *cs )
{
    param_stack new_cs;
    new_cs = ( param_stack )malloc( sizeof( struct param_stack ) );
    new_cs->head = c;
    new_cs->tail = *cs;
    *cs = new_cs;
}

param param_stack_pop( param_stack *cs )
{
    param_stack old_stack;
    param head;
    if( *cs == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_POPEAR_PARAM_STACK_VACIO ) );
        exit( -1 );
    }
    old_stack = *cs;
    head = ( *cs )->head;
    *cs = ( *cs )->tail;
    free( old_stack );
    return head;
}


/*****************************************************/

void promo_stack_push( promo c, promo_stack *cs )
{
    promo_stack new_cs;
    new_cs = ( promo_stack )malloc( sizeof( struct promo_stack ) );
    new_cs->head = c;
    new_cs->tail = *cs;
    *cs = new_cs;
}

promo promo_stack_pop( promo_stack *cs )
{
    promo_stack old_stack;
    promo head;
    if( *cs == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_POPEAR_PROMO_STACK_VACIO ) );
        exit( -1 );
    }
    old_stack = *cs;
    head = ( *cs )->head;
    *cs = ( *cs )->tail;
    free( old_stack );
    return head;
}

//#endif


