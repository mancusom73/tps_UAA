#pragma pack (1) 

#include <stdio.h>              /* Standard I/O.     */
#include <string.h>             /* String functions. */
#include <stdlib.h>             /* Standard C library header. */
#include <errno.h>              /* Error handling.   */

#include "libcache.h"


#pragma pack (1)

/* -------------------------------------------------------------------------- **
 * Typedefs...
 *
 */

typedef struct
  {
    ubi_cacheEntry node;
	struct codsArt {
		COD_INT cod_int;
		COD_BAR cod_bar;
	} codsArt;
    //COD_INT cod_int;
    //COD_BAR cod_bar;
    COD_DPTO cod_dep;
    double cost;
	char promocionable;
  } Rec;

typedef Rec *RecPtr;

/*
typedef struct
  {
    COD_INT cod_int;
    COD_BAR cod_bar;
  } codsArt;
*/
typedef struct codsArt *codsArtPtr;



/* -------------------------------------------------------------------------- **
 * Global Variables...
 *
 */

static ubi_cacheRoot CachePtr[1];

/* -------------------------------------------------------------------------- **
 * Functions...
 */

int compfunc( ubi_trItemPtr ItemPtr, ubi_trNodePtr NodePtr )
  {
    COD_INT A1 = ((codsArtPtr)ItemPtr)->cod_int;
    COD_BAR A2 = ((codsArtPtr)ItemPtr)->cod_bar;
//    COD_INT A1 = *(COD_INT*)ItemPtr;
//    COD_BAR A2 = * ( COD_BAR* ) ( ( (void*)ItemPtr ) + sizeof( COD_INT ) );

    COD_INT B1 = ((RecPtr)NodePtr)->codsArt.cod_int;
    COD_BAR B2 = ((RecPtr)NodePtr)->codsArt.cod_bar;

    if ( A1 == B1 ) {
      if ( A2 == B2 ) {
	return 0;
      } else {
	return (A2 < B2 ? -1 : 1);
      }
    } else {
      return (A1 < B1 ? -1 : 1);
    }

  } /* compfunc */


void freefunc( ubi_trNodePtr Node )
  {
    //  RecPtr r = (RecPtr)Node;

  //free( r->key );
  //free( r->content );
  free( Node );
  } /* freefunc */


/*
static void dumpcache( void )
  {
  unsigned long l = 0;
  RecPtr        Entry;

  Entry = (RecPtr)ubi_trFirst( CachePtr->root.root );
  while( Entry )
    {
    l++;
    printf( "%5lu: %3lu: [%li, %lli, %li, %lf]\n",
            l, Entry->node.entry_size, Entry->cod_int, Entry->cod_bar,
	    Entry->cod_dep, Entry->cost);
    Entry = (RecPtr)ubi_trNext( Entry );
    }
  fprintf( stderr, "Memory Used: %ld bytes\n", CachePtr->mem_used );
  fprintf( stderr, "Cache Size:  %ld entries\n", CachePtr->root.count );
  } */ /* dumpcache */

//static void prunecache( void )
  /* ------------------------------------------------------------------------ **
   * Semi-randomly delete nodes in the cache.
   * ------------------------------------------------------------------------ **
   */
/*  {
  int    mem = 0;
  int    ent = 0;
  int    i   = 42;
  RecPtr Entry;
  RecPtr Next;

  Entry = (RecPtr)ubi_trFirst( CachePtr->root.root );
  while( Entry )
    {
    Next = (RecPtr)ubi_trNext( Entry );
    i--;
    if( i < 1 )
      {
      i = Entry->node.entry_size;
      mem += i;
      ent += 1;
      (void)ubi_cacheDelete( CachePtr, Entry->key );
      }
    Entry = Next;
    }
  fprintf( stderr, "Freed %d bytes, %d entries.\n", mem, ent );
  }*/ /* prunecache */



void init_arts_cache ( ) {
  (void)ubi_cacheInit( CachePtr, compfunc, freefunc, 0, 0 );
}

void destroy_arts_cache ( ) {
  ubi_cacheClear( CachePtr );
  //CachePtr = NULL;
}


void put_art_cache ( COD_INT cint, COD_BAR cbar, COD_DPTO cdep, double cost, char promocionable) {
  RecPtr  rp;

  rp = (RecPtr)malloc( sizeof( Rec ) );

  rp->codsArt.cod_int = cint;
  rp->codsArt.cod_bar = cbar;
  rp->cod_dep = cdep;
  rp->cost = cost;
  rp->promocionable = promocionable;

  
	{ // Averiguar si existe el articulo en el cache, para no sobreescribirlo
	  ubi_cacheEntryPtr r = NULL;
	  Rec find;

	  find.codsArt.cod_int = cint;
	  find.codsArt.cod_bar = cbar;

	  r = ubi_cacheGet( CachePtr, (ubi_trItemPtr) &(find.codsArt) );

	  if ( r ) {
			// El Artículo ya está en el cache -> no se hace nada.
	  } else {
			// El Artículo NO está en el cache -> Se agrega.
		  ubi_cachePut( CachePtr, sizeof( Rec ), (ubi_cacheEntryPtr)rp, (ubi_trItemPtr)&(rp->codsArt) );
	  }

  }

  
}

void change_art_cache ( COD_INT cint, COD_BAR cbar, COD_DPTO cdep, double cost, char promocionable) {
  RecPtr  rp;

  rp = (RecPtr)malloc( sizeof( Rec ) );

  rp->codsArt.cod_int = cint;
  rp->codsArt.cod_bar = cbar;
  rp->cod_dep = cdep;
	rp->cost = cost;
  rp->promocionable = promocionable;

  ubi_cacheDelete( CachePtr, (ubi_trItemPtr)&(rp->codsArt) );
 
  (void)ubi_cachePut( CachePtr, sizeof( Rec ), (ubi_cacheEntryPtr)rp, (ubi_trItemPtr)&(rp->codsArt) );
  
}


COD_DPTO get_dpto_cache ( COD_INT cint, COD_BAR cbar ) {
  ubi_cacheEntryPtr r = NULL;
  Rec find;

  find.codsArt.cod_int = cint;
  find.codsArt.cod_bar = cbar;

  r = ubi_cacheGet( CachePtr, (ubi_trItemPtr) &(find.codsArt) );

  if ( r ) {
    return ((RecPtr)r)->cod_dep;
  } else {
    return -1;
  }
}

double get_cost_cache ( COD_INT cint, COD_BAR cbar ) {
  ubi_cacheEntryPtr r = NULL;
  Rec find;

  find.codsArt.cod_int = cint;
  find.codsArt.cod_bar = cbar;

  r = ubi_cacheGet( CachePtr, (ubi_trItemPtr) &(find.codsArt) );

  if ( r ) {
    return ((RecPtr)r)->cost;
  } else {
    return -1;
  }
}

int get_promocionable_cache ( COD_INT cint, COD_BAR cbar ) {
  ubi_cacheEntryPtr r = NULL;
  Rec find;

  find.codsArt.cod_int = cint;
  find.codsArt.cod_bar = cbar;

  r = ubi_cacheGet( CachePtr, (ubi_trItemPtr) &(find.codsArt) );

  if ( r ) {
    return (int)((RecPtr)r)->promocionable;
  } else {
    return -1;
  }
}




    /*  
  init_arts_cache( );

  put_art_cache ( 5, 555, 505050, 5.55 );
  put_art_cache ( 25, 5552, 5050, 25.55 );
  put_art_cache ( 15, 5551, 50, 15.55 );
  put_art_cache ( 15, 5552, 5, 125.55 );
  put_art_cache ( 50, 5550, 1, 50.55 );
  put_art_cache ( 5, 55, 555, 55.5555 );
  put_art_cache ( 5, 555, 1, 11.5555 );
  put_art_cache ( 15, 5551, 5550, 21235.55 );

  fprintf(stderr,"[%li, %li] -> %li - %lf\n", 5, 55, get_dpto_cache(5,55),get_cost_cache(5,55) );
  fprintf(stderr,"[%li, %li] -> %li - %lf\n", 5, 555, get_dpto_cache(5,555),get_cost_cache(5,555) );
  fprintf(stderr,"[%li, %li] -> %li - %lf\n", 50, 55, get_dpto_cache(55,55),get_cost_cache(55,55) );
  fprintf(stderr,"[%li, %li] -> %li - %lf\n", 55, 55, get_dpto_cache(55,55),get_cost_cache(55,55) );
  fprintf(stderr,"[%li, %li] -> %li - %lf\n", 15, 5551, get_dpto_cache(15,5551),get_cost_cache(15,5551) );

  dumpcache();
    */
/*
  (void)ubi_cacheSetMaxEntries( CachePtr, 500 );
  dumpcache();
*/

/*
  (void)ubi_cacheSetMaxMemory( CachePtr, 32768 );
  dumpcache();
*/

/*
  prunecache();
  dumpcache();
*/

