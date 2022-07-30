#include "types2.h"
#include "state.h"
#include "uprinter.h"

#include <string.h>
#include <assert.h>

#pragma pack (1)


/*
 * MUCHO CUIDADO: se esta asumiendo que ninguna evaluacion
 * modifica las expresiones que estan en el estado.
 * Es decir, cuando se modifican, se crean nuevas expresiones y
 * se derreferencian las antiguas expresiones.
 * >>>> REVISAR que esto sea realmente asi.
 */




#include <stdio.h>              /* Standard I/O.     */
#include <string.h>             /* String functions. */
#include <stdlib.h>             /* Standard C library header. */
#include <errno.h>              /* Error handling.   */

#include "u_Atree.h"        /* AVL tree module.    */

#pragma pack (1)


typedef struct
{
  ubi_trNode Node;
  identifier name;
  expr e;
} stateNode;

typedef stateNode *stateNodePtr;

//typedef ubi_trRootPtr state;

//static ubi_trRoot    Root;
//static ubi_trRootPtr RootPtr = &Root;



static int CompareFunc( ubi_trItemPtr ItemPtr, ubi_trNodePtr NodePtr )

{
  identifier A, B;

  //A = (identifier)ItemPtr;
  A = *(identifier *)ItemPtr;
  B = ((stateNodePtr)NodePtr)->name;

  return strcmp(A,B);
}



static void PrintNode( ubi_trNodePtr NodePtr, void *Userdata )
{
  stateNodePtr RecPtr = (stateNodePtr)NodePtr;

  fprintf(stdout, "[%s]\n", RecPtr->name/*, RecPtr->Count*/ );
  up_expr(stdout, RecPtr->e, 1);
}



static void KillNode( stateNodePtr NodePtr )
{
  free ( ((stateNodePtr)NodePtr)->name ); // Se libera el identificador.
  unref_e ( ((stateNodePtr)NodePtr)->e );  // Se libera la expresion.
  free( NodePtr );
}




/*
 * Actualiza o Agrega una variable x con valor e.
 * Si e=NULL, se elimina x del estado.
 */
state update( state st, identifier x, expr e ) {
  state s = st;

  if ( !s ) {
    // Inicializar el estado
    s = (ubi_trRootPtr)malloc(sizeof(ubi_trRoot));
    (void)ubi_trInitTree( s,              /* Pointer to the tree header           */
			  CompareFunc,    /* Pointer to the comparison function.  */
			  0 );            /* Don't allow overwrites or duplicates.*/
                                          /* See the Insert() notes in BinTree.h. */
  }

  if ( e ) {
    // se crea el nodo a agregar   
    stateNodePtr recPtr, oldRecPtr;

    recPtr = (stateNodePtr)malloc(sizeof(stateNode));
    recPtr->name = (char*)malloc(strlen(x)+1);
    strcpy(recPtr->name,x);
    recPtr->e = ref_e(e);
    
    
    if( !ubi_trInsert( s,              /* To which tree are we adding this?    */
		       recPtr,         /* The new node to be added.            */
		       &recPtr->name,  /* Points to the comparison field.      */
		       &oldRecPtr )    /* Overwrites are not allowed.          */
	)
      {
	// No se pudo insertar el nodo, por lo que se modifica el existente.
	unref_e( oldRecPtr->e );
	oldRecPtr->e = ref_e( e );

	KillNode( recPtr );
      }
  } else {

    // antes de eliminar el nodo, se debe verificar que exista en el avl.
    // NOTA: Si esta condicion se pudiera tomar como pre-condicion,
    //       se podria eliminar este chequeo de la implementación, haciendo
    //       el borrado mucho mas eficiente.
    stateNodePtr n;
    n = (stateNodePtr) ubi_trFind( s, &x );
    if ( n ) {
      // n1 es el nodo que contiene la asignacion x->e
      // se elimina el nodo.
      n = (stateNodePtr) ubi_trRemove( s, n );
      KillNode( n );
    }
  }
  return s;
}



/*
 * Devuelve el valor de x en s si x in dom(s).
 *          NULL caso contrario.
 */
expr value( state s, identifier x )
{
  if ( s ) {
   stateNodePtr n;
    n = (stateNodePtr) ubi_trFind( s, &x );
    if ( n ) {
      // n es el nodo que contiene la asignacion x->e
      return n->e;
    }
  }
  return NULL;
}


state free_state( state s )
{
  if (s) {
    //ubi_trTraverse( s, PrintNode, NULL );
    ubi_trKillTree( s,          /* Tree root pointer. */
		    KillNode ); /* Function that frees the node. */
    free( s );
    return NULL;
  } else {
    return s;
  }
}



static void insNodeInToState( ubi_trNodePtr NodePtr, void *s )
{
//	stateNodePtr RecPtr = (stateNodePtr)NodePtr;
	s = update(  s, ((stateNodePtr)NodePtr)->name, ((stateNodePtr)NodePtr)->e );
}

state state_clone( state s )
{
	state cs = NULL;
//	assert( s != NULL );

	if (s) {
	    // Inicializar el estado
		cs = (ubi_trRootPtr)malloc(sizeof(ubi_trRoot));
		ubi_trInitTree( cs,             /* Pointer to the tree header           */
						CompareFunc,    /* Pointer to the comparison function.  */
						0 );            /* Don't allow overwrites or duplicates.*/
                                        /* See the Insert() notes in BinTree.h. */
		ubi_trTraverse( s, insNodeInToState, (void*) cs );
	}

	return cs;
	
	/*    state s1 = NULL;
    if( s != NULL ) {
        s1 = ( state )malloc( sizeof( struct stateNode ) );
        s1->name = ( identifier )malloc( strlen( s->name ) + 1 );
        strcpy( s1->name, s->name );
        s1->e = ref_e( s->e );
        s1->next = state_clone( s->next );
    }
    return s1;
  */
  return s;
}
















//void Prune( ubi_trRootPtr RootPtr )
/* Pone en Cero todos los contadores */
/* Con esto se puede implementar el CLONE */
  /*
{
  //  ubi_trNodePtr This, Next;
    This = ubi_trFirst( RootPtr->root );
    while( NULL != This )
    {
    Next = ubi_trNext( This );
    ((ArtsRecPtr)This)->Count = 0;
    
    This = Next;
    }
}
  */


