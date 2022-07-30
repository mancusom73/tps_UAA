#pragma pack (push,1) 
#ifndef _UPRINT_
#define _UPRINT_

#include "types2.h"
#include "time2.h"

#pragma pack (1)

void up_expr( FILE *out, expr e, int prec );

void up_benef( FILE *out, benef b, int offset );

void up_promo( FILE *out, promo p );

void up_promos( FILE *out, promo p );

#endif

#pragma pack (pop) 
