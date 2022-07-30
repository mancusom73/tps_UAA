#pragma pack (push,1) 
/**
 * MODULO Estructuras de Datos para Modulo Tolerancia a Fallas
 */

#ifndef FTYPES
#define FTYPES

//typedef int id; /// revisar esto esta definido en otros lugares

#include "loglib.h"

#pragma pack (1)


/**
 * Lista de registros del log
 */
typedef struct dataList
{
    logReg e;
    struct dataList *tail;
} *dataList;

/**
 * Lista de eventos de diferente tipo, con sus respectivos
 * datos, según el tipo de evento.
 */
typedef struct eventList
{
    logtype event;
    identifier id;
    number count;
    long depart;
    double precio;
	char promocionable;
	char porImporte;
    struct eventList *tail;
} *eventList;

/**
 * Cola de eventos.
 */
typedef struct eventQueue
{
    eventList first;
    eventList last;
} *eventQueue;

/** Libera la lista de registros. */
void freeDataList( dataList l );

/** "pegar por detras" un registro de logs */
dataList dataListCons( dataList l, logReg r ); 

/** Toma de la lista una lectura del elemento i */
void *dataListGetValue( dataList *l, id i, void *r, size_t size ); 

/** toma de la lista una lectura del elemento i[ind] */
void *dataListGetSubValue( dataList *l, id i, char ind, void *r, size_t size ); 


/**
 * Determina si una lista de datos l solo contiene valores inicializados
 * pero sin una cantidad determinada.
 */
int dataListOpen( dataList l ); 


eventQueue newEventQueue( void );

void freeEventQueue( eventQueue q );

eventQueue eventEnQueue( eventQueue q, logtype event, identifier id, number count, long depart, double precio, char promocionable, char porImporte );

logtype currentEvent( eventQueue q );
identifier currentIdentifier( eventQueue q );
number currentCount( eventQueue q );
long currentDepart( eventQueue q );
double currentPrecio( eventQueue q );
int currentPromocionable( eventQueue q );
int currentPorImporte( eventQueue q );

eventQueue eventDeQueue( eventQueue q );

int isEmptyQueue( eventQueue q );


#endif // FTYPES



#pragma pack (pop) 
