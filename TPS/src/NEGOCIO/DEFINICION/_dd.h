#pragma pack (push,1) 
#ifndef _DD_H
#define _DD_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

// clase
#define _VAR_RNV        1
#define _VAR_USUARIO    2
#define _VAR_INT_INI    3
#define _VAR_INT_NOINI  4

// tipo_variable
#define  _SHORT_INT      1
#define  _INT            2
#define  _FLOAT          3
#define  _DOUBLE         4
#define  _STRING         5
#define  _DATE           6
#define  _HORA           7
#define  _LONG           8
#define  _UNSIGNED       9

void DD_INS( int var, int clase, long offset, unsigned largo, int elementos, char tipo_variable );
#endif
#pragma pack (pop) 
