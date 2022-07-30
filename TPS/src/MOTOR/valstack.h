#pragma pack (push,1) 
#include "types2.h"

#pragma pack (1)

/*
typedef enum {PROG, PROMO, PARAMS, PARAM, CONDS, COND, EXPRS, EXPR, BENEFS, BENEF} tVal;
*/


typedef enum { VS_E, VS_L, VS_ML, VS_P, VS_ID, VS_B, VS_PA, VS_PR } vs_type;


typedef struct vstack
{
    vs_type type;
    void *top;
    struct vstack *next;
} *vstack;


expr vs_pop_e( vstack *vs );
list vs_pop_l( vstack *vs );
mlist vs_pop_ml( vstack *vs );
part vs_pop_p( vstack *vs );
identifier vs_pop_id( vstack *vs );
benef vs_pop_b( vstack *vs ); 
param vs_pop_pa( vstack *vs ); 
promo vs_pop_pr( vstack *vs );

vstack vs_push_e( vstack vs, expr v );
vstack vs_push_l( vstack vs, list v );
vstack vs_push_ml( vstack vs, mlist v );
vstack vs_push_p( vstack vs, part v );
vstack vs_push_id( vstack vs, identifier v );
vstack vs_push_b( vstack vs, benef v );
vstack vs_push_pa( vstack vs, param v );
vstack vs_push_pr( vstack vs, promo v );

vstack vs_swap( vstack vs );
vstack vs_swap_2( vstack vs );
vstack vs_swap_3( vstack vs );

vstack vs_free( vstack vs );

#pragma pack (pop) 
