#pragma pack (1) 

/** \file obuf.c
 *   \brief Modulo que se encarga de administrar y optimizar las escrituras al log de Tolerancia a Fallas.
 * 
 * Modulo que se encarga de administrar y optimizar las escrituras al log.
 */

#include "obuf.h"
#include <string.h>

#pragma pack (1)

/**
 * 
 * @param ind ind < 0 indica que no se trata de un arreglo.
 * ind >= 0 indica el indice en el arreglo id.
 */
int oValue( obuf *o, id id, char ind, void *data, size_t size, FILE *log )
{
    obuf of = *o;
    while( of ) {
        if( of->id == id ) {
            if( ( ind < 0 ) || ( ( ind >= 0 ) && ( ind == of->ind ) ) ) {
                if( memcmp( data, of->data, size ) == 0 ) {
                    //fprintf(stderr,"oValue: coincide -> se incrementa en memoria\n");
                    // coincide con el antiguo valor. solo se incrementa la cuenta.
                    of->count++;
                    return 0;
                }
                else {
                    // difiere con el antiguo valor, por lo que se graba la cuenta
                    // en el log, y se reinicia la cuenta a 1 con el nuevo valor.
                    int r;

                    //fprintf(stderr,"oValue: no coincide -> se graba cuenta y se inicia nuevo valor\n");

                    r = ( ind < 0 ? putCountValue( id, of->count, log )
                        : putCountSubValue( id, ind, of->count, log ) );
                    free( of->data );
                    of->data = malloc( size );
                    memcpy( of->data, data, size );
                    of->count = 1;
                    r = r
                      | ( ind < 0 ? putInitValue( id, data, size, log )
                        : putInitSubValue( id, ind, data, size, log ) );
                    return r;
                }
            }
        }
        of = of->tail;
    }
    {
        // se crea un nuevo nodo y se lo agrega al comienzo de la lista.
        // se agrega la inicializacion del valor en el log
        obuf nuevo = NULL;
        int r;

        //fprintf(stderr,"oValue: NUEVO -> se inicia nuevo valor\n");

        nuevo = ( obuf )malloc( sizeof( struct obuf ) );
        if( nuevo == NULL ) {
            fprintf( stderr, "oValue: falta de memoria" );
            return 1;
        }
        nuevo->id = id;
        nuevo->ind = ind;
        nuevo->data = malloc( size );
        memcpy( nuevo->data, data, size );
        nuevo->count = 1;
        nuevo->tail = *o;
        *o = nuevo;

        if( ind < 0 ) {
            r = putInitValue( id, data, size, log );
        }
        else {
            r = putInitSubValue( id, ind, data, size, log );
        }
        return r;
    }  

    return 0;
}


/**
 * 
 * se libera la info del bufer y se guardan las cantidades en el log.
 */
void freeOBuf( obuf o, FILE *log )
{
    obuf o1;

    //fprintf(stderr,"freeOBuf:\n");
    while( o ) {
        if( o->ind < 0 ) {
            putCountValue( o->id, o->count, log );
        }
        else {
            putCountSubValue( o->id, o->ind, o->count, log );
        }
        o1 = o;
        o = o->tail;

        free( o1->data );
        free( o1 );
    }
}

