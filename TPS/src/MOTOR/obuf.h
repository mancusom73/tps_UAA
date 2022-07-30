#pragma pack (push,1) 
/*
 * Output Buffer
 * Modulo que se encarga de administrar las escrituras
 * al log.
 */

#include "ftypes.h"

#pragma pack (1)


/**
 * Lista que almacena la cantidad 'count' de veces que se
 * ha leido el mismo valor 'data' para cada identificador
 * de variable RNV 'id'.
 */
typedef struct obuf
{
    id id;
    char ind;
    void *data;
    //  size_t size; // se asume que el tama#o de los datos para un id es constante
    int count;
    struct obuf *tail;
} *obuf;


/**
 * ind < 0 indica que no se trata de un arreglo.
 * ind >= 0 indica el indice en el arreglo id.
 */
int oValue( obuf *o, id id, char ind, void *data, size_t size, FILE *log );

// se libera la info del bufer y se guardan las cantidades en el log.
void freeOBuf( obuf o, FILE *log );


#pragma pack (pop) 
