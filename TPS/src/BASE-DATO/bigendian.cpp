/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#include "config.h"
#ifdef INVEL_W
#ifndef _BIGENDIAN_CPP_
#define _BIGENDIAN_CPP_

#include "bigendian.h"

short READ_SHORT( char *buffer )
{
    short iValue = *( short * )buffer;

    return TurnShort( iValue );
}


long READ_LONG( char *buffer )
{
    long iValue = *( long * )buffer;

    return TurnLong( iValue );
}


void SET_SHORT( char *Buffer, short Value )
{
    *( short* )Buffer = TurnShort( Value );
}


void SET_LONG( char *Buffer, long Value )
{
    *( long* )Buffer = TurnLong( Value );
}


short TurnShort( short iToConvert )
{
    #ifdef BIGENDIAN
    short iTurnedValue;

    char *scr = ( char * )&iToConvert, *tar = ( char * )&iTurnedValue;

    tar[0] = scr[1];
    tar[1] = scr[0];

    return iTurnedValue;
    #else
    return iToConvert;
    #endif
}


long TurnLong( long iToConvert )
{
    #ifdef BIGENDIAN

    long iTurnedValue;

    char *scr = ( char * )&iToConvert, *tar = ( char * )&iTurnedValue;

    tar[0] = scr[3];
    tar[1] = scr[2];
    tar[2] = scr[1];
    tar[3] = scr[0];

    return iTurnedValue;
    #else
    return iToConvert;
    #endif
}


double TurnDouble( double dToConvert )
{
    #ifdef BIGENDIAN

    double dTurnedValue;

    char *scr = ( char * )&dToConvert, *tar = ( char * )&dTurnedValue;

    tar[0] = scr[7];
    tar[1] = scr[6];
    tar[2] = scr[5];
    tar[3] = scr[4];
    tar[4] = scr[3];
    tar[5] = scr[2];
    tar[6] = scr[1];
    tar[7] = scr[0];

    return dTurnedValue;
    #else
    return dToConvert;
    #endif
}


float TurnFloat( float dToConvert )
{
    #ifdef BIGENDIAN
    float dTurnedValue;

    char *scr = ( char * )&dToConvert, *tar = ( char * )&dTurnedValue;

    tar[0] = scr[3];
    tar[1] = scr[2];
    tar[2] = scr[1];
    tar[3] = scr[0];

    return dTurnedValue;
    #else
    return dToConvert;
    #endif
}
#endif
#endif
