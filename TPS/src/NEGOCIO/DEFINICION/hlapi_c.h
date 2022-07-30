#ifndef HLAPI_H
#define HLAPI_H
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

/****************************************************************************/
/**                                                                        **/
/**                              Hardlock                                  **/
/**                    API-Structures and definitions                      **/
/**                                                                        **/
/**   This file contains some helpful defines to access a Hardlock using   **/
/**   the application programing interface (API) for Hardlock.             **/
/**                                                                        **/
/**                    (c) 1993-94 Glenco Engineering, Inc.                **/
/**                                                                        **/
/**                                                                        **/
/**              Author  : Henri Volk                                      **/
/**              Version : 3.20 API                                        **/
/**              Date    : 22-Jun-1994                                     **/
/**                                                                        **/
/**  Revision history                                                      **/
/**  ----------------                                                      **/
/**  3.18  -- Define HIGH_DLL changed to HLHIGH_DLL (Watcom 9.5 doesn't    **/
/**           like my define)                                              **/
/**        -- New function call API_GET_TASKID                             **/
/**        -- HLXLOGIN no longer needed.                                   **/
/**        -- Add UNIX features (M_UNIX defined)                           **/
/**        -- Add Windows NT feature (WINNT defined)                       **/
/**  3.19  -- ifdef for Symantec C included (DOS386)                       **/
/**  3.20  -- ifdef for WIN32s included                                    **/
/**           ifdef for Intel Codebuilder removed (compiler to buggy)      **/
/****************************************************************************/
#ifdef M_UNIX
#define __386__
#pragma pack(1)
#endif

#ifdef __WIN32__
#define __386__
#endif

#ifdef WINNT
#ifndef __386__                   /* Watcom doesnt like it */
#define __386__
#endif
#pragma pack(1)
#endif

#ifdef DOS386
#define __386__
#pragma pack(2)
#endif

#ifdef __HIGHC__
#define __386__
#define _PACKED _Packed
#endif

#ifdef __386__
#define DATAFAR_
#define FAR_
#else
#define DATAFAR_ far
#define FAR_ far
#endif

#ifdef HLHIGH_DLL
#define CALL_ pascal _export
#endif

#ifdef LOAD_DS
#define CALL_ _loadds
#endif

#ifndef CALL_
#define CALL_
#endif

#ifndef _PACKED
#define _PACKED
#endif

/* -------------------------------- */
/* Definitions and API structures : */
/* -------------------------------- */
typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned long Long;

typedef struct
{
    Word Use_Key;
    Byte Key[8];
} DES_MODE;

typedef struct
{
    Word ModAd;               /* Hardlock module address */
    Word Reg;                     /* Memory register adress  */
    Word Value;               /* Memory value            */
    Byte Reserved[4];
} EYE_MODE;

typedef union
{
    DES_MODE Des;
    EYE_MODE Eye;
} HARDWARE;

typedef _PACKED struct hl_api
                {
                    Byte API_Version_ID[2]; /* Version                    */
                    Word API_Options[2];      /* API Optionflags            */
                    Word ModID;               /* Modul-ID (EYE = 0...)      */
                    HARDWARE Module;          /* Hardware type              */
                    void DATAFAR_ *Data;      /* Pointer to cipher data     */
                    Word Bcnt;                /* Number of blocks           */
                    Word Function;            /* Function number            */
                    Word Status;              /* Actual status              */
                    Word Remote;              /* Remote or local??          */
                    Word Port;                /* Port address if local      */
                    Word Speed;               /* Speed of port if local     */
                    Word NetUsers;            /* Current Logins (HL-Server) */
                    Byte ID_Ref[8];           /* Referencestring            */
                    Byte ID_Verify[8];        /* Encrypted ID_Ref           */
                    Long Task_ID;             /* Multitasking program ID    */
                    Word MaxUsers;            /* Maximum Logins (HL-Server) */
                    Long Timeout;             /* Login Timeout in minutes   */
                    Word ShortLife;           /* (multiple use)             */
                    Word Application;         /* Application number         */
                    Word Protocol;            /* Protocol flags             */
                    Word PM_Host;             /* DOS Extender type          */
                    Byte Reserved[186];   /* Reserved area              */
                } HL_API;

#ifdef M_UNIX
#pragma pack()
#endif

/* ------------- */
/* Module-ID's : */
/* ------------- */
#define EYE_DONGLE       0        /* Hardlock E-Y-E             */
#define DES_DONGLE       1        /* FAST DES                   */

/* --------------------- */
/* API function calls  : */
/* --------------------- */
#define API_INIT            0     /* Init API structure      */
#define API_DOWN            1     /* Free API structure      */
#define API_FORCE_DOWN      31  /* Force deinintialization */
#define API_MULTI_SHELL_ON  2     /* MTS is enabled          */
#define API_MULTI_SHELL_OFF 3     /* MTS is disabled         */
#define API_MULTI_ON        4     /* Enable MTS              */
#define API_MULTI_OFF       5     /* Disable  MTS            */
#define API_AVAIL           6     /* Dongle available?       */
#define API_LOGIN           7     /* Login dongle server     */
#define API_LOGOUT          8     /* Logout dongle server    */
#define API_INFO            9     /* Get API informations    */
#define API_GET_TASKID      32  /* Get TaskID from API     */

/* --------------------------- */
/* Data and memory functions : */
/* --------------------------- */
#define API_KEYE             11 /* Use KEYE for encryption         */
#define API_READ             20 /* Read one word of dongle EEPROM  */
#define API_WRITE            21 /* Write one word of dongle EEPROM */
#define API_READ_BLOCK       23 /* Read EEPROM in one block        */
#define API_WRITE_BLOCK      24 /* Write EEPROM in one block       */
#define API_ABORT            51 /* Critical Error Abort            */

/* -------------------- */
/* Dongle access mode : */
/* -------------------- */
#define LOCAL_DEVICE    1         /* Query local HL only         */
#define NET_DEVICE      2         /* Query remote HL only        */
#define DONT_CARE       3         /* Query local or remote HL    */

/* ------------------ */
/* API PM_Host ID's : */
/* ------------------ */
#define API_XTD_DETECT    0
#define API_XTD_DPMI      1   /* QDPMI, Borland, Windows ... */
#define API_XTD_PHAR386   2
#define API_XTD_PHAR286   3
#define API_XTD_CODEBLDR  4   /* Intel Code Builder          */
#define API_XTD_COBOLXM   5

/* ------------------ */
/* API Status Codes : */
/* ------------------ */
#define STATUS_OK                 0 /* API call was succesfull        */
#define NOT_INIT                  1 /* DONGLE not initialized         */
#define ALREADY_INIT              2 /* Already initialized            */
#define UNKNOWN_DONGLE            3 /* Device not supported           */
#define UNKNOWN_FUNCTION          4 /* Function not supported         */
#define NO_DONGLE                 7 /* No device available            */
#define NETWORK_ERROR             8 /* A network error occured        */
#define NO_ACCESS                 9 /* No device available            */
#define INVALID_PARAM            10 /* A wrong parameter occured      */
#define VERSION_MISMATCH         11 /* HL-Server not API version      */
#define DOS_ALLOC_ERROR          12 /* Error on memory allocation     */
#define CANNOT_OPEN_DRIVER   14  /* Can not open driver (NT,UNIX) */
#define TOO_MANY_USERS          256 /* Login table full (remote)      */
#define SELECT_DOWN             257 /* Printer not On-line            */

/* --------------------- */
/* Function prototypes : */
/* --------------------- */
#ifdef __cplusplus
extern    "C"
{
    #endif

    Word FAR_ CALL_ HL_LOGIN( Word ModAd, Word Access, Byte DATAFAR_ *RefKey,
                              Byte DATAFAR_ *VerKey );
    Word FAR_ CALL_ HL_LOGOUT( void );
    Word FAR_ CALL_ HL_AVAIL( void );
    Word FAR_ CALL_ HL_PORTINF( void );
    Word FAR_ CALL_ HL_ACCINF( void );
    Word FAR_ CALL_ HL_USERINF( void );
    Word FAR_ CALL_ HL_MAXUSER( void );
    Word FAR_ CALL_ HL_MEMINF( void );
    Word FAR_ CALL_ HL_CODE( void DATAFAR_ *Data, Word Count );
    Word FAR_ CALL_ HL_WRITE( Word Reg, Word Value );
    Word FAR_ CALL_ HL_READ( Word Reg, Word DATAFAR_ *Value );
    Word FAR_ CALL_ HL_READBL( char DATAFAR_ *Eeprom );
    Word FAR_ CALL_ HL_WRITEBL( char DATAFAR_ *Eeprom );
    Word FAR_ CALL_ HL_ABORT( void );
    Word FAR_ CALL_ HL_VERSION( void );
    Word FAR_ CALL_ HL_HLSVERS( void );

    /****************************************************************************/
    /****************************************************************************/
    /* The following functions map the old Hardlock Calls on the new API. These */
    /* functions are defined only for compatibility reasons.                    */
    /* !!! Don't mix old and new functions. Don't use if it is not necessary.!!! */
    /****************************************************************************/
    /****************************************************************************/
    void FAR_ CALL_ HL_ON( Word Port, Word ModAd );
    void FAR_ CALL_ HL_OFF( Word Port );
    Word FAR_ CALL_ K_EYE( Word Port, char DATAFAR_ *Inp, Word BlkCnt );
    void FAR_ CALL_ HL_WR( Word Port, Word Reg, Word Val );
    Word FAR_ CALL_ HL_RD( Word Port, Word Reg );
    void FAR_ CALL_ INT_ON( void );
    void FAR_ CALL_ INT_OFF( void );
    /****************************************************************************/
    /* The old calculation function : */
    /* ------------------------------ */
    Word FAR_ CALL_ HL_CALC( Word i1, Word i2, Word i3, Word i4 );
    /****************************************************************************/

    #ifdef __cplusplus
};
#endif
#endif