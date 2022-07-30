#pragma pack (1) 

//#ifdef MODULO_PROMOCIONES

#include "staticSt.h"
#include <string.h>

#pragma pack (1)

/* Para representar el estado vacio utilizar NULL. */

/*
 * Actualiza o Agrega una variable x con valor e, para la promo pr.
 * Si e=NULL, se elimina x del estado.
 */
staticState static_update( staticState st, identifier pr, identifier x, expr e )
{
    staticState st1 = st;
    while( st1 != NULL ) {
        if( strcmp( pr, st1->name ) == 0 ) {
            st1->s = update( st1->s, x, e );
            return st;
        }
        st1 = st1->next;
    }
    // st == NULL -> crear un nodo para una nueva promo pr.
    st1 = ( staticState )malloc( sizeof( struct staticStateNode ) );
    st1->name = ( identifier )malloc( strlen( pr ) + 1 );
    strcpy( st1->name, pr );
    st1->s = update( NULL, x, e );
    st1->next = st;
    return st1;
}

/*
 * Devuelve el valor de x en la promo pr, si x in dom(s(pr)).
 *          NULL caso contrario.
 */
expr static_value( staticState st, identifier pr, identifier x )
{
    staticState st1 = st;
    while( st1 != NULL ) {
        if( strcmp( st1->name, pr ) == 0 ) {
            return value( st1->s, x );
        }
        st1 = st1->next;
    }
    return NULL;
}


/*
 * Libera el espacio ocupado por el estado, dereferenciando
 * la expresiones a las que apuntaba y borrando los strings.
 */

staticState free_static_state( staticState st )
{
    staticState st1 = st, st2;
    while( st1 != NULL ) {
        st2 = st1->next;
        st1->s = free_state( st1->s );
        free( st1->name );
        free( st1 );
        st1 = st2;
    }
    return NULL;
}

staticState staticState_clone( staticState st )
{
    staticState st2 = st;
    if( st2 != NULL ) {
        st2 = ( staticState )malloc( sizeof( struct staticStateNode ) );
        st2->name = ( identifier )malloc( strlen( st->name ) + 1 );
        strcpy( st2->name, st->name );
        st2->s = state_clone( st->s );
        st2->next = staticState_clone( st->next );
    }
    return st2;
}

//#endif


