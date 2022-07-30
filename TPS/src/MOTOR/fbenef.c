#pragma pack (1) 

/*
 * MODULO Estructuras de Datos para Modulo Tolerancia a Fallas
 */

#include "fbenef.h"
#include <string.h>

#pragma pack (1)
 
/*
 typedef struct benefList {
   logtype type;
   identifier id1;
   identifier id2;
   identifier id3;
   number count;
   number value;
   struct benefList * tail;
 } * benefList;
*/

void freeBenefList( benefList l )
{
    benefList r;
    while( l ) {
        r = l->tail;
        if( l->id1 ) {
            free( l->id1 );
        }
        if( l->id2 ) {
            free( l->id2 );
        }
        if( l->id3 ) {
            free( l->id3 );
        }
        free( l );
        l = r;
    }
}

// "pegar por detras"
benefList benefListCons( benefList l, logtype t, identifier id, identifier type, number count,
                         number value, identifier label )
{
    benefList r1 = l;
    benefList r2 = l;
    benefList r3 = NULL;

    r3 = ( benefList )malloc( sizeof( struct benefList ) );
    memset( r3, 0, sizeof( struct benefList ) );

    r3->type = t;
    if( !id ) {
        fprintf( stderr, "benefListCons: error: identificador NULL\n" );
        return l;
    }
    else {
        r3->id1 = ( char* )malloc( strlen( id ) + 1 );
        strcpy( r3->id1, id );
    }
    if( type ) {
        r3->id2 = ( char* )malloc( strlen( type ) + 1 );
        strcpy( r3->id2, type );
    }
    if( label ) {
        r3->id3 = ( char* )malloc( strlen( label ) + 1 );
        strcpy( r3->id3, label );
    }
    r3->count = count;
    r3->value = value;
    r3->tail = NULL;


    while( r1 ) {
        r2 = r1;
        r1 = r1->tail;
    }
    if( r1 == r2 ) {
        // agregar a la lista vacia.
        return r3;
    }
    else {
        // agregar al final de la lista no vacia.
        r2->tail = r3;
        return l;
    }
}


/* 
 * determina si los datos dados por parametro coinciden con el 
 * primer elemento de la lista.
 */
int benefListCompareFirst( benefList l, logtype t, identifier id, identifier type, number count,
                           number value, identifier label )
{
    int r = 0;

    if( !l ) {
        fprintf( stderr, "benefListCompareFirst: error, lista vacia\n" );
        return -1;
    }

    //fprintf(stderr,"benefListCompareFirst: %i[%s,%s,%lf,%lf,%s]\n",
    //    l->type, l->id1, l->id2, l->count, l->value, l->id3 );
    //fprintf(stderr,"benefListCompareFirst: %i[%s,%s,%lf,%lf,%s]\n",
    //  t, id, type, count, value, label );

    r = ( ( l->type == t ) && ( strcmp( l->id1, id ) == 0 ) && ( l->count == count )
       && ( l->value == value ) );

    if( ( l->id2 ) && ( type ) ) {
        r = r & ( strcmp( l->id2, type ) == 0 );
    }
    if( ( l->id3 ) && ( label ) ) {
        r = r & ( strcmp( l->id3, label ) == 0 );
    }

    //fprintf(stderr,"benefListCompareFirst: RETURN %i\n",r);

    return r;
}



benefList tailBenefList( benefList l )
{
    benefList r;
    if( l ) {
        r = l->tail;
        if( l->id1 ) {
            free( l->id1 );
        }
        if( l->id2 ) {
            free( l->id2 );
        }
        if( l->id3 ) {
            free( l->id3 );
        }
        free( l );
        return r;
    }
    else {
        return NULL;
    }
}


