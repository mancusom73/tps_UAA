#pragma pack (push,1) 
#ifndef LIB_ARTS_CACHE
#define LIB_ARTS_CACHE

#include "ubiCache.h"

#pragma pack (1)


#define COD_INT long
#ifndef WIN32
#define COD_BAR long long
#else
#define COD_BAR __int64
#endif
#define COD_DPTO long

void init_arts_cache ( );

void destroy_arts_cache ( ) ;

void put_art_cache ( COD_INT cint, COD_BAR cbar, COD_DPTO cdep, double cost, char promocionable);

void change_art_cache ( COD_INT cint, COD_BAR cbar, COD_DPTO cdep, double cost, char promocionable);

COD_DPTO get_dpto_cache ( COD_INT cint, COD_BAR cbar );

double get_cost_cache ( COD_INT cint, COD_BAR cbar );

int get_promocionable_cache ( COD_INT cint, COD_BAR cbar );


#endif

#pragma pack (pop) 
