#pragma pack (push,1) 
#include "types2.h"

#pragma pack (1)


typedef struct pc_version {
	int macro;
	int version;
	int subversion;
	int build;
} pc_version;

void to_disk( promo p, char *file, pc_version *ver );

promo from_disk( char *file );

int compatible_version( char *file, pc_version *ver );


#pragma pack (pop) 
