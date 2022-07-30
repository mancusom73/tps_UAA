#pragma pack (1) 

#include "lib_req.h"

#include <stdio.h>              /* Standard I/O.     */
#include <string.h>             /* String functions. */
#include <stdlib.h>             /* Standard C library header. */
#include <errno.h>              /* Error handling.   */

#include "u_Atree.h"        /* AVL tree module.    */

#pragma pack (1)



typedef struct {
  ubi_trNode Node;
  cod_art    Art;
  double     Count;
  } ArtsRec;

typedef ArtsRec *ArtsRecPtr;


static ubi_trRoot    Root;
static ubi_trRootPtr RootPtr = &Root;



static int CompareFunc( ubi_trItemPtr ItemPtr, ubi_trNodePtr NodePtr )

  {
  cod_art A, B;

  A = *(cod_art *)ItemPtr;
  B = ((ArtsRecPtr)NodePtr)->Art;

  return( A < B ? -1 : (A > B ? 1 : 0 ) );
  }



static void PrintNode( ubi_trNodePtr NodePtr, void *Userdata )
  {
  ArtsRecPtr  RecPtr = (ArtsRecPtr)NodePtr;

  (void)printf( "[%li,\t%lf]\n", RecPtr->Art, RecPtr->Count );
  }



static void KillNode( ubi_trNodePtr NodePtr )
  {
  free( NodePtr );
  }



void Prune( ubi_trRootPtr RootPtr )
/* Pone en Cero todos los contadores */
  {
  ubi_trNodePtr This, Next;

  This = ubi_trFirst( RootPtr->root );
  while( NULL != This )
    {
    Next = ubi_trNext( This );
    ((ArtsRecPtr)This)->Count = 0;

    This = Next;
    }
  }





void init_articles_counts ( void ) {
      /* Initialize the tree. */
  (void)ubi_trInitTree( RootPtr,      /* Pointer to the tree header           */
                        CompareFunc,  /* Pointer to the comparison function.  */
                        0 );          /* Don't allow overwrites or duplicates.*/
                                      /* See the Insert() notes in BinTree.h. */

}

double * article_count_ref( cod_art cod_int ) {
  /* Inicializa el código interno de articulo 'cod_int' en el AVL
   * y devuelve un puntero al contador que indica la cantidad en que
   * dicho artículo ha sido incluido en el ticket.
   */
    ArtsRecPtr  RecPtr, OldRecPtr = NULL;
    double * rta = NULL;

    /* Allocate a new node to be added to the tree. */
    RecPtr = (ArtsRecPtr)malloc( sizeof(ArtsRec) );
    if( !RecPtr )
      {
      perror( "lib_req: article_count_ref:" );
      exit( EXIT_FAILURE );
      }

    /* Copy the new data into the record. */
      RecPtr->Art = cod_int;
      RecPtr->Count = 0.0;

      if( !ubi_trInsert( RootPtr,       /* To which tree are we adding this?    */
			 RecPtr,        /* The new node to be added.            */
			 &RecPtr->Art,  /* Points to the comparison field.      */
			 &OldRecPtr )    /* Overwrites are not allowed.          */
	  )
	{
	  free( RecPtr );  /* Duplicate key causes entry failure. Free new entry. */
	  if (OldRecPtr) {
	    rta = &OldRecPtr->Count;
	  } 
	} else {
	rta = &RecPtr->Count;
      }
      return rta;
}

double * add_article_count( cod_art cod_int, double count ) {
  cod_art FindMe = cod_int;
  double * FinalCount = NULL;
  ubi_trNodePtr NodePtr;

  NodePtr = ubi_trFind( RootPtr, & FindMe );

  if (NodePtr) {
    ((ArtsRecPtr)NodePtr)->Count += count;
    FinalCount = &((ArtsRecPtr)NodePtr)->Count;
  } 

  return FinalCount;
}


void restart_articles_counts ( void ) {
  Prune( RootPtr );
}

void destroy_articles_counts ( void ) {
 //(void)ubi_trTraverse( RootPtr, PrintNode, NULL );

  (void)ubi_trKillTree( RootPtr,    /* Tree root pointer. */
                        KillNode ); /* Function that frees the node. */

}

