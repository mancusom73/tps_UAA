#pragma pack (push,8) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

// Definiciones generales
#define TOP                      -1
#define BOTTOM                   -2
#define MAX_AREA_BTRV           100
#define MAX_LONG_KEY            255

// Errores
#define ERR_PARAMETROS    -10000
#define ERR_AREA_INACTIVA -10001
#define ERR_MALLOC        -10002
#define ERR_BUFFER_ESCASO -10003

// Campo "formato" para BtrvOpen
#define FORMATO_NRO_REGISTRO 0x00000001
#define FORMATO_L_VARIABLE   0x00000002

// Campo "modo" para BtrvOpen
#define MODO_NORMAL           0
#define MODO_ACELERADO       -1
#define MODO_READ_ONLY       -2
#define MODO_EXCLUSIVO       -4

// Campo "operacion" para BtrvSeek
#define SEEK_EQ             5
#define SEEK_GT             8
#define SEEK_GE             9
#define SEEK_LT            10
#define SEEK_LE            11

#pragma pack (pop) 
