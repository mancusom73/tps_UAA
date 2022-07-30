#pragma pack (push,1) 
#ifndef _STATE_
#define _STATE_

#include "types2.h"
#include "u_Atree.h"        /* AVL tree module.    */

#pragma pack (1)

typedef ubi_trRootPtr state;

/* Para representar el estado vacio utilizar NULL. */

/*
 * Actualiza o Agrega una variable x con valor e.
 * Si e=NULL, se elimina x del estado.
 */
state update( state s, identifier x, expr e );

/*
 * Devuelve el valor de x en s si x in dom(s).
 *          NULL caso contrario.
 */
expr value( state s, identifier x );


/*
 * Libera el espacio ocupado por el estado, dereferenciando
 * la expresiones a las que apuntaba y borrando los strings.
 */

state free_state( state s );

state state_clone( state s );

#endif // _STATE_

#pragma pack (pop) 
