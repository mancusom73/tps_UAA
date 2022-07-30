#pragma pack (1) 

#include "lib_dreq.h"

#include <stdio.h>              /* Standard I/O.     */
#include <string.h>             /* String functions. */
#include <stdlib.h>             /* Standard C library header. */
#include <errno.h>              /* Error handling.   */

#include "u_Atree.h"        /* AVL tree module.    */

#pragma pack (1)



typedef struct {
  ubi_trNode Node;
  cod_dep    Dep;
  double     Count;
  } DepRec;

typedef DepRec *DepRecPtr;


static ubi_trRoot    Root;
static ubi_trRootPtr RootPtr = &Root;



static int CompareFunc( ubi_trItemPtr ItemPtr, ubi_trNodePtr NodePtr )

  {
  cod_dep A, B;

  A = *(cod_dep *)ItemPtr;
  B = ((DepRecPtr)NodePtr)->Dep;

  return( A < B ? -1 : (A > B ? 1 : 0 ) );
  }



static void dPrintNode( ubi_trNodePtr NodePtr, void *Userdata )
  {
  DepRecPtr  RecPtr = (DepRecPtr)NodePtr;

  (void)printf( "[department d%li,\t%lf]\n", RecPtr->Dep, RecPtr->Count );
  }



static void dKillNode( ubi_trNodePtr NodePtr )
  {
  free( NodePtr );
  }



void dPrune( ubi_trRootPtr RootPtr )
/* Pone en Cero todos los contadores */
  {
  ubi_trNodePtr This, Next;

  This = ubi_trFirst( RootPtr->root );
  while( NULL != This )
    {
    Next = ubi_trNext( This );
    ((DepRecPtr)This)->Count = 0;

    This = Next;
    }
  }





void init_department_counts ( void ) {
      /* Initialize the tree. */
  (void)ubi_trInitTree( RootPtr,      /* Pointer to the tree header           */
                        CompareFunc,  /* Pointer to the comparison function.  */
                        0 );          /* Don't allow overwrites or duplicates.*/
                                      /* See the Insert() notes in BinTree.h. */

}

double * department_count_ref( cod_dep dep ) {
  /* Inicializa el código de departamento 'cod_dep' en el AVL
   * y devuelve un puntero al contador que indica la cantidad de 
   * artículos de dicho departamento que han sido incluidos en el ticket.
   */
    DepRecPtr  RecPtr, OldRecPtr = NULL;
    double * rta = NULL;

    /* Allocate a new node to be added to the tree. */
    RecPtr = (DepRecPtr)malloc( sizeof(DepRec) );
    if( !RecPtr )
      {
      perror( "lib_dreq: dapartment_count_ref:" );
      exit( EXIT_FAILURE );
      }

    /* Copy the new data into the record. */
      RecPtr->Dep = dep;
      RecPtr->Count = 0.0;

      if( !ubi_trInsert( RootPtr,       /* To which tree are we adding this?    */
			 RecPtr,        /* The new node to be added.            */
			 &RecPtr->Dep,  /* Points to the comparison field.      */
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

double * add_department_count( cod_dep dep, double count ) {
  cod_dep FindMe = dep;
  double * FinalCount = NULL;
  ubi_trNodePtr NodePtr;

  NodePtr = ubi_trFind( RootPtr, & FindMe );

  if (NodePtr) {
    ((DepRecPtr)NodePtr)->Count += count;
    FinalCount = &((DepRecPtr)NodePtr)->Count;
  } 

  return FinalCount;
}


void restart_department_counts ( void ) {
  dPrune( RootPtr );
}

void destroy_department_counts ( void ) {
 //(void)ubi_trTraverse( RootPtr, dPrintNode, NULL );

  (void)ubi_trKillTree( RootPtr,    /* Tree root pointer. */
                        dKillNode ); /* Function that frees the node. */

}

