#pragma pack (1) 

//#ifdef MODULO_PROMOCIONES

#ifdef __MSDOS__
#include "proStack.h"
#else
#include "prostack.h"
#endif

#pragma pack (1)

expr ps_pop_e( vstack *vs )
{
    vstack tmp;
    expr r;
    r = ( expr )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

list ps_pop_l( vstack *vs )
{
    vstack tmp;
    list r;
    r = ( list )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

mlist ps_pop_ml( vstack *vs )
{
    vstack tmp;
    mlist r;
    r = ( mlist )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

part ps_pop_p( vstack *vs )
{
    vstack tmp;
    part r;
    r = ( part )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

identifier ps_pop_id( vstack *vs )
{
    vstack tmp;
    identifier r;
    r = ( identifier )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}



benef ps_pop_b( vstack *vs )
{
    vstack tmp;
    benef r;
    r = ( benef )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

param ps_pop_pa( vstack *vs )
{
    vstack tmp;
    param r;
    r = ( param )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}

promo ps_pop_pr( vstack *vs )
{
    vstack tmp;
    promo r;
    r = ( promo )( ( *vs )->top );
    tmp = *vs;
    *vs = ( *vs )->next;
    free( tmp );
    return r;
}



vstack ps_push_e( vstack vs, expr v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}

vstack ps_push_l( vstack vs, list v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}

vstack ps_push_ml( vstack vs, mlist v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}

vstack ps_push_p( vstack vs, part v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}

vstack ps_push_id( vstack vs, identifier v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}



vstack ps_push_b( vstack vs, benef v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}

vstack ps_push_pa( vstack vs, param v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}

vstack ps_push_pr( vstack vs, promo v )
{
    vstack tmp;
    tmp = ( vstack )malloc( sizeof( struct vstack ) );
    tmp->top = v;
    tmp->next = vs;
    return tmp;
}


/*
 * Asegurarse de que alla al menos 2 elementos en la pila.
 */
vstack ps_swap( vstack vs )
{
    vstack tmp1,tmp2;
    if( vs == NULL ) {
        /* Estos errores nunca deben ocurrir. */
        fprintf( stderr, "Intento de swapear una pila vacia.\n" );
        exit( 1 );
    }
    else if( vs->next == NULL ) {
        fprintf( stderr, "Intento de swapear una pila con un solo elemento.\n" );
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
vstack ps_swap_2( vstack vs )
{
    vstack tmp1,tmp2,tmp3;
    if( vs == NULL ) {
        /* Estos errores nunca deben ocurrir. */
        fprintf( stderr, "Intento de swapear_2 una pila vacia.\n" );
        exit( 1 );
    }
    else if( vs->next == NULL ) {
        fprintf( stderr, "Intento de swapear_2 una pila con un solo elemento.\n" );
        exit( 1 );
    }
    else if( vs->next->next == NULL ) {
        fprintf( stderr, "Intento de swapear_2 una pila con solo dos elementos.\n" );
        exit( 1 );
    }
    tmp1 = vs;
    tmp2 = vs->next;
    tmp3 = vs->next->next;
    tmp1->next = tmp3->next;
    tmp3->next = tmp1;
    return tmp2;
}


//#endif


