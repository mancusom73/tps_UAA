#pragma pack (push,1) 
/*
 * MODULO Estructuras de Datos para Modulo Tolerancia a Fallas
 */

#ifndef FBENEF
#define FBENEF

#include "loglib.h"

#pragma pack (1)

/**
 * Lista de los digerentes tipos de beneficios y sus respectivos
 * datos, según el tipo de beneficio.
 */
typedef struct benefList
{
    logtype type;
    identifier id1;
    identifier id2;
    identifier id3;
    number count;
    number value;
    struct benefList *tail;
} *benefList;


void freeBenefList( benefList l );

/** "pegar por detras" un beneficio */
benefList benefListCons( benefList l, logtype t, identifier id, identifier type, number count,
                         number value, identifier label ); 


/** 
 * Determina si los datos dados por parametro coinciden con el 
 * primer elemento de la lista.
 */
int benefListCompareFirst( benefList l, logtype t, identifier id, identifier type, number count,
                           number value, identifier label );

benefList tailBenefList( benefList l );

#endif // FBENEF



#pragma pack (pop) 
