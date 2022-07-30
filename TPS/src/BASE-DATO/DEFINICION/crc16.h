#pragma pack (push,8) 
/* compute crc's */
/* crc-16 is based on the polynomial x^16+x^15+x^2+1 */
/*  The bits are inserted from least to most significant */
/* crc-ccitt is based on the polynomial x^16+x^12+x^5+1 */
/*  The bits are inserted from most to least significant */
/* The prescription for determining the mask to use for a given polynomial
 * is as follows:
 * 1.  Represent the polynomial by a 17-bit number
 * 2.  Assume that the most and least significant bits are 1
 * 3.  Place the right 16 bits into an integer
 * 4.  Bit reverse if serial LSB's are sent first
 */


#ifndef _CRC16_H_
#define _CRC16_H_


#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    "defines.h"

#define     M16 0xA001        /* crc-16 mask */
#define     MTT 0x1021        /* crc-ccitt mask */

/* function declarations */
unsigned short updcrc( unsigned short, short );
unsigned short updcrcr( unsigned short, short );
unsigned short crc16( unsigned short, char *, unsigned short );

#endif

#pragma pack (pop) 
