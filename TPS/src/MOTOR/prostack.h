#pragma pack (push,1) 
#ifdef __MSDOS__
#include "types2.h"
#else
#include "types2.h"
#endif

#pragma pack (1)


//typedef enum {PROG, PROMO, PARAMS, PARAM, CONDS, COND, EXPRS, EXPR, BENEFS, BENEF} tVal;

typedef struct vstack
{
    void *top;
    struct vstack *next;
} *vstack;

expr ps_pop_e( vstack *vs );
list ps_pop_l( vstack *vs );
mlist ps_pop_ml( vstack *vs );
part ps_pop_p( vstack *vs );
identifier ps_pop_id( vstack *vs );
benef ps_pop_b( vstack *vs ); 
param ps_pop_pa( vstack *vs ); 
promo ps_pop_pr( vstack *vs );

vstack ps_push_e( vstack vs, expr v );
vstack ps_push_l( vstack vs, list v );
vstack ps_push_ml( vstack vs, mlist v );
vstack ps_push_p( vstack vs, part v );
vstack ps_push_id( vstack vs, identifier v );
vstack ps_push_b( vstack vs, benef v );
vstack ps_push_pa( vstack vs, param v );
vstack ps_push_pr( vstack vs, promo v );

vstack ps_swap( vstack vs );
vstack ps_swap_2( vstack vs );

#pragma pack (pop) 
