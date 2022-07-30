#pragma pack (push,8) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#ifndef _BIGENDIAN_H_
#define _BIGENDIAN_H_

short READ_SHORT( char *buffer );
long READ_LONG( char *buffer );
void SET_SHORT( char *Buffer, short Value );
void SET_LONG( char *Buffer, long Value );

short TurnShort( short );
long TurnLong( long );
double TurnDouble( double );
float TurnFloat( float );

#endif

#pragma pack (pop) 
