#pragma pack (1) 

#include "ftypes.h"
#include <string.h>

#pragma pack (1)

void freeDataList( dataList l )
{
    dataList l1;
    while( l ) {
        l1 = l->tail;

        freeLogReg( l->e );
        free( l );

        l = l1;
    }
}

/** 
 * "pegar por detras" un registro de logs.
 */
dataList dataListCons( dataList l, logReg r )
{
    dataList l1,l2;
    logReg e1;
    switch( r->type ) {
        case InitValue:
        case InitSubValue:
            l1 = l2 = l;
            while( l1 != NULL ) {
                l2 = l1;
                l1 = l1->tail;
            }
            l1 = ( dataList )malloc( sizeof( struct dataList ) );
            l1->e = r;
            l1->tail = NULL;
            if( l2 ) {
                l2->tail = l1;
            }
            else {
                l = l1;
            }
            break;
        case CountValue:
        case CountSubValue:
            l1 = l;
            e1 = NULL;
            while( l1 != NULL ) {
                if( l1->e->id == r->id ) {
                    if( ( r->type == CountSubValue ) && ( l1->e->type == InitSubValue ) ) {
                        if( l1->e->subind == r->subind ) {
                            if( l1->e->count == 0 ) {
                                e1 = l1->e;
                                break;
                            }
                        }
                    }
                    else if( ( r->type == CountValue ) && ( l1->e->type == InitValue ) ) {
                        if( l1->e->count == 0 ) {
                            e1 = l1->e;
                            break;
                        }
                    }
                }
                l1 = l1->tail;
            }
            if( e1 ) {
                e1->count = r->count;
            }
            else {
                fprintf(
                stderr,
                "dataListCons: Error: intento de agregar la cantidad de un valor sin inicializar\n" );
            }

            break;
        default:
            fprintf( stderr, "dataListCons: Error: intento de agregar un registro invalido" );
            break;
    }
    return l;
}

/**
 * Toma de la lista una lectura del elemento i,
 * se decrementa el contador para el elemento i en la lista ls,
 * y se guarda el valor leido en r. se devuelve un puntero a r.
 */
void * dataListGetValue( dataList *ls, id i, void *r, size_t size )
{
    dataList l1 = *ls, l = *ls;
    //  void * r = NULL;

    while( l ) {
        if( ( l->e->type == InitValue ) && ( l->e->id == i ) ) {
            //fprintf(stderr,".");
            memcpy( r, l->e->data, size );
            //fprintf(stderr,".");
            if( l->e->count > 0 ) {
                l->e->count--;
                if( l->e->count == 0 ) {
                    // En este caso hay que eliminar el nodo con el dato,
                    // ya que se han leido todas las veces requeridas.
                    if( l1 == l ) {
                        // el nodo que se elimina es el primero de la lista
                        *ls = l->tail;
                    }
                    else {
                        // el nodo que se elimina es "interno".
                        l1->tail = l->tail;
                    }
                    //fprintf(stderr,"'");
                    freeLogReg( l->e );
                    free( l );
                    //fprintf(stderr,"'");
                }
            }
            else {
                // En este caso, el valor no tiene una cantidad, lo que indica que
                // el error en la ejecucion se produjo antes que este id cambien 
                // de valor, por lo que no hay registro de cuantas veces se debe
                // seguir devolviendo el mismo valor.

                /**/ fprintf( stderr, "dataListGetValue: posible fin del MODO RECOVER" );
            }
            return r;
        }

        l1 = l;
        l = l->tail;
    }
    return NULL;
}

/**
 * Toma de la lista una lectura del elemento i[ind] (para i un arreglo de elementos),
 * se decrementa el contador para el elemento i en la lista ls,
 * y se guarda el valor leido en r. se devuelve un puntero a r.
 */
void * dataListGetSubValue( dataList *ls, id i, char ind, void *r, size_t size )
{
    // toma de la lista una lectura del elemento i[ind]
    dataList l1 = *ls, l = *ls;
    //  void * r = NULL;

    while( l ) {
        if( ( l->e->type == InitSubValue ) && ( l->e->id == i ) && ( l->e->subind == ind ) ) {
            memcpy( r, l->e->data, size );
            if( l->e->count > 0 ) {
                l->e->count--;
                if( l->e->count == 0 ) {
                    // En este caso hay que eliminar el nodo con el dato,
                    // ya que se han leido todas las veces requeridas.
                    if( l1 == l ) {
                        // el nodo que se elimina es el primero de la lista
                        *ls = l->tail;
                    }
                    else {
                        // el nodo que se elimina es "interno".
                        l1->tail = l->tail;
                    }
                    freeLogReg( l->e );
                    free( l );
                }
            }
            else {
                // En este caso, el valor no tiene una cantidad, lo que indica que
                // el error en la ejecucion se produjo antes que este id cambien 
                // de valor, por lo que no hay registro de cuantas veces se debe
                // seguir devolviendo el mismo valor.
            }
            return r;
        }

        l1 = l;
        l = l->tail;
    }
    return NULL;
}

/**
 * Determina si una lista de datos l solo contiene valores inicializados
 * pero sin una cantidad determinada.
 */
int dataListOpen( dataList l )
{
    int r = 1;
    while( l ) {
        if( l->e->count > 0 ) {
            return 0;
        }
        l = l->tail;
    }
    return r;
}


eventQueue newEventQueue()
{
    eventQueue q;
    q = ( eventQueue )malloc( sizeof( struct eventQueue ) );
    q->first = NULL;
    q->last = NULL;
    return q;
}

void freeEventQueue( eventQueue q )
{
    eventList l,l1;
    if( q ) {
        l = q->first;
        while( l ) {
            l1 = l->tail;
            free( l->id );
            free( l );
            l = l1;
        }
        free( q );
    }
}

eventQueue eventEnQueue( eventQueue q, logtype event, identifier id, number count, long depart, double precio, char promocionable, char porImporte )
{
    eventList l,qt;
    if( q ) {
        l = ( eventList )malloc( sizeof( struct eventList ) );
		memset(l, 0, sizeof( struct eventList ) );
        l->event = event;
        l->tail = NULL;
        switch( event ) {
            case AddArticle:
            case RemoveArticle:
            case UpdateArticle:
            case AddExternalDiscount:
                l->id = id;
                l->count = count;
                l->depart = depart;
                l->precio = precio;
                l->promocionable = promocionable;
                l->porImporte = porImporte;
                break;
            case PanelVentas:
            case PanelPagos:
            case PanelPosPago:
                l->id = NULL;
                l->count = 0;
                break;
            case CashdeskState:
                l->id = id;
                l->count = count;
                break;
            default:
                fprintf( stderr, "eventEnQueue: evento invalido" );
                free( l );
                return q;
                break;
        }
        qt = q->last;
        if( qt ) {
            qt->tail = l;
        }
        else {
            q->first = l;
        }
        q->last = l;
    }
    else {
        return q;
    }
    return q;
}

logtype currentEvent( eventQueue q )
{
    if( q ) {
        if( q->first ) {
            return q->first->event;
        }
        else {
            return -1;
        }
    }
    else {
        return -2;
    }
}

identifier currentIdentifier( eventQueue q )
{
    if( q ) {
        if( q->first ) {
            return q->first->id;
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

number currentCount( eventQueue q )
{
    if( q ) {
        if( q->first ) {
            return q->first->count;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

long currentDepart( eventQueue q )
{
    if( q ) {
        if( q->first ) {
            return q->first->depart;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

double currentPrecio( eventQueue q )
{
    if( q ) {
        if( q->first ) {
            return q->first->precio;
        }
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

int currentPromocionable( eventQueue q )
{
    if( q ) {
        if( q->first ) {
            return (int) (q->first->promocionable);
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

int currentPorImporte( eventQueue q )
{
    if( q ) {
        if( q->first ) {
            return (int) (q->first->porImporte);
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}



eventQueue eventDeQueue( eventQueue q )
{
    eventList l;
    if( q ) {
        if( q->first ) {
            l = q->first;

            q->first = q->first->tail;
            if( q->last == l ) {
                // este es el caso en que la cola contenia un unico elemento
                q->last = NULL;
            }

            switch( l->event ) {
                case AddArticle:
                case RemoveArticle:
                case UpdateArticle:
                case AddExternalDiscount:
                case CashdeskState:
                    free( l->id );
                    break;
                case PanelVentas:
                case PanelPagos:
                case PanelPosPago:
                    break;
                default:
                    fprintf( stderr, "eventDeQueue: evento invalido" );
                    free( l );
                    return q;
                    break;
            }
            free( l );
        }
    }
    return q;
}

int isEmptyQueue( eventQueue q )
{
    if( q ) {
        return !( q->first );
    }
    else {
        return 1;
    }
}

