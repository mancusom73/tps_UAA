#pragma pack (1) 

//#ifdef MODULO_PROMOCIONES

//#define PROMO_STR

#include "valstack.h"
#include "etpv.h"
#include <string.h>

#pragma pack (1)


expr vs_pop_e( vstack *vs )
{
    vstack tmp;
    expr r;
    r = ( expr )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

list vs_pop_l( vstack *vs )
{
    vstack tmp;
    list r;
    r = ( list )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

mlist vs_pop_ml( vstack *vs )
{
    vstack tmp;
    mlist r;
    r = ( mlist )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

part vs_pop_p( vstack *vs )
{
    vstack tmp;
    part r;
    r = ( part )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

identifier vs_pop_id( vstack *vs )
{
    vstack tmp;
    identifier r;
    r = ( identifier )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}



benef vs_pop_b( vstack *vs )
{
    vstack tmp;
    benef r;
    r = ( benef )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

param vs_pop_pa( vstack *vs )
{
    vstack tmp;
    param r;
    r = ( param )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

promo vs_pop_pr( vstack *vs )
{
    vstack tmp;
    promo r;
    r = ( promo )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}


vstack vs_push_e( vstack vs, expr v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_E;
    tmp->top = ref_e( v );
    tmp->next = vs;
    return tmp;
}

vstack vs_push_l( vstack vs, list v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_L;
    tmp->top = ref_l( v );
    tmp->next = vs;
    return tmp;
}

vstack vs_push_ml( vstack vs, mlist v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_ML;
    tmp->top = ref_ml( v );
    tmp->next = vs;
    return tmp;
}

vstack vs_push_p( vstack vs, part v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_P;
    tmp->top = ref_pl( v );
    tmp->next = vs;
    return tmp;
}

vstack vs_push_id( vstack vs, identifier v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_ID;
    tmp->top = ( char* )malloc( strlen( v ) + 1 );
    strcpy( tmp->top, v );
    tmp->next = vs;
    return tmp;
}



vstack vs_push_b( vstack vs, benef v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_B;
    tmp->top = ref_b( v );
    tmp->next = vs;
    return tmp;
}

vstack vs_push_pa( vstack vs, param v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_PA;
    tmp->top = ref_pa( v );
    tmp->next = vs;
    return tmp;
}

vstack vs_push_pr( vstack vs, promo v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->type = VS_PR;
    tmp->top = ref_pro( v );
    tmp->next = vs;
    return tmp;
}


/*
 * Asegurarse de que alla al menos 2 elementos en la pila.
 */
vstack vs_swap( vstack vs )
{
    vstack tmp1,tmp2;
    if( vs == NULL ) {
        /* Estos errores nunca deben ocurrir. */
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_UNA_PILA_VACIA_ ) );
        exit( 1 );
    }
    else if( vs->next == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_UNA_PILA_CON_ ) );
        exit( 1 );
    }
    tmp1 = vs;
    tmp2 = vs->next;
    vs->next = ( vs->next )->next;
    tmp2->next = tmp1;
    return tmp2;
}

/*
 * Asegurarse de que alla al menos 3 elementos en la pila.
 */
vstack vs_swap_2( vstack vs )
{
    vstack tmp1,tmp2,tmp3;
    if( vs == NULL ) {
        /* Estos errores nunca deben ocurrir. */
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_2_UNA_PILA_VACIA_ ) );
        exit( 1 );
    }
    else if( vs->next == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_2_UNA_PILA1 ) );
        exit( 1 );
    }
    else if( vs->next->next == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_2_UNA_PILA2 ) );
        exit( 1 );
    }
    tmp1 = vs;
    tmp2 = vs->next;
    tmp3 = vs->next->next;
    tmp1->next = tmp3->next;
    tmp3->next = tmp1;
    return tmp2;
}

/*
 * Asegurarse de que alla al menos 4 elementos en la pila.
 */
vstack vs_swap_3( vstack vs )
{
    vstack tmp1,tmp2,tmp4;
    if( vs == NULL ) {
        /* Estos errores nunca deben ocurrir. */
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_2_UNA_PILA_VACIA_ ) );
        exit( 1 );
    }
    else if( vs->next == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_2_UNA_PILA1 ) );
        exit( 1 );
    }
    else if( vs->next->next == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_2_UNA_PILA2 ) );
        exit( 1 );
    }
    else if( vs->next->next->next == NULL ) {
        fprintf( stderr, ST( S_INTENTO_DE_SWAPEAR_2_UNA_PILA2 ) );
        exit( 1 );
    }
    tmp1 = vs;
    tmp2 = tmp1->next;
    //tmp3 = tmp2->next;
    tmp4 = tmp2->next->next;
    tmp1->next = tmp4->next;
    tmp4->next = tmp1;
    return tmp2;
}

vstack vs_free( vstack vs )
{
    vstack vtmp;
    while( vs ) {
        vtmp = vs;
        vs = vs->next;
        switch( vtmp->type ) {
            case VS_E:
                unref_e( vtmp->top );
                break;
            case VS_L:
                unref_l( vtmp->top );
                break;
            case VS_ML:
                unref_ml( vtmp->top );
                break;
            case VS_P:
                unref_pl( vtmp->top );
                break;
            case VS_ID:
                free( vtmp->top );
                break;
            case VS_B:
                unref_b( vtmp->top );
                break;
            case VS_PA:
                unref_pa( vtmp->top );
                break;
            case VS_PR:
                unref_pr( vtmp->top );
                break;
            default:
                fprintf( stderr, ST( S_VALSTACK_C__ESTO_NO_DEBERIA_OCURRIR_ ) );
                break;
        }
        free( vtmp );
    }
    return vs;
}

//#endif


