#pragma pack (push,1) 
#ifndef _STATICSTATE_
#define _STATICSTATE_

#include "state.h"
#include "types2.h"

#pragma pack (1)

typedef struct staticStateNode
{
    identifier name; // nombre de la promo
    state s;         // estado que almacena las variables estaticas de esa promo
    struct staticStateNode *next;
}*staticState;

/* Para representar el estado vacio utilizar NULL. */

/*
 * Actualiza o Agrega una variable x con valor e, para la promo pr.
 * Si e=NULL, se elimina x del estado.
 */
staticState static_update( staticState st, identifier pr, identifier x, expr e );

/*
 * Devuelve el valor de x en la promo pr, si x in dom(s(pr)).
 *          NULL caso contrario.
 */
expr static_value( staticState st, identifier pr, identifier x );


/*
 * Libera el espacio ocupado por el estado, dereferenciando
 * la expresiones a las que apuntaba y borrando los strings.
 */

staticState free_static_state( staticState st );

staticState staticState_clone( staticState st );

#endif // _STATICSTATE_

#pragma pack (pop) 
