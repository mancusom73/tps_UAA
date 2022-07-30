#pragma pack (push,1) 
#include "ftypes.h"
#include "fbenef.h"

#pragma pack (1)


int loadLog( FILE *log, eventQueue *qs, dataList *ls, benefList *bs );

int recoverState( eventQueue *qs );



#pragma pack (pop) 
