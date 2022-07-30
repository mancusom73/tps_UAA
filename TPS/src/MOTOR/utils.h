#pragma pack (push,1) 
#ifndef _UTILS
#define _UTILS
#include "codes.h"
#include "types2.h"                        

#pragma pack (1)

typedef struct expr_stack
{
    expr head;
    struct expr_stack *tail;
} *expr_stack;
void expr_stack_push( expr c, expr_stack *cs );
expr expr_stack_pop( expr_stack *cs );


typedef struct benef_stack
{
    benef head;
    struct benef_stack *tail;
} *benef_stack;
void benef_stack_push( benef c, benef_stack *cs );
benef benef_stack_pop( benef_stack *cs );

typedef struct param_stack
{
    param head;
    struct param_stack *tail;
} *param_stack;
void param_stack_push( param c, param_stack *cs );
param param_stack_pop( param_stack *cs );

typedef struct promo_stack
{
    promo head;
    struct promo_stack *tail;
} *promo_stack;
void promo_stack_push( promo c, promo_stack *cs );
promo promo_stack_pop( promo_stack *cs );

#endif

#pragma pack (pop) 
