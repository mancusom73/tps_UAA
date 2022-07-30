#pragma pack (push,8) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */


#ifndef _DBRDEFINES_H_
#define _DBRDEFINES_H_


// no usar los token standard como WORD en vez de eso anteponerle
// DBR (DBRouter) pues entran en conflicto con las definiciones standard 
// del lenguaje, como ser visual c

//#ifndef WORD

#define DBR_WORD unsigned short
//#endif

#ifndef PIPE_BUF
#define PIPE_BUF 4096
#endif

//#ifndef BYTE
#define DBR_BYTE unsigned char
//#endif

//#ifndef BOOL
#define DBR_BOOL unsigned char
//#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#define DBROUTER_PETITION  0x0

#define INSERT_REG 0x01
#define DELETE_REG 0x02
#define UPDATE_REG 0x03
#define OBTAIN_REG 0x04

#define DBROUTER_OBTAIN_TABLES 0x100

#define OK                    0x01
#define CONNECTION_PROBLEM      0x02
#define PETITION_IN_PROCESS   0x03



#endif
#pragma pack (pop) 
