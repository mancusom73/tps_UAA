#pragma pack (push,1) 
#include "types2.h"

#pragma pack (1)

typedef enum { NORM, ABNORM, DIVBYZERO, NEGATIVEMSET } evalres;

typedef struct states *states;

typedef struct recover_info * recover_info;

evalres eval( evalTime et, promo ps, states *ss );

evalres add_articles( long article, identifier barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, int por_importe, promo ps, states *ss );

evalres remove_articles( long article, identifier barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, int por_importe, promo ps, states *ss );

evalres update_articles( long article, identifier barcod, long codPresentacion, double cant, double precio, long depart, int promocionable, promo ps, states *ss );

evalres add_external_discount( double monto, promo ps, states *ss );

/*
 * Devuelve el programa que resulta de concatenar p1 ++ p2
 * Se modifica p1.
 */
promo concat( promo p1, promo p2 );

states free_states( states ss );

states states_clone( states ss );

recover_info actual_info( states ss );

states recover( recover_info r, states ss );

recover_info free_recover_info( recover_info s );

void freeall( void );


#pragma pack (pop) 
