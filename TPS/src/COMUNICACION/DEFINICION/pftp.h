#pragma pack (push,1) 
/* Phoenix FTP
     - A stand-alone, portable, generic, fully-functional FTP client and server
   package.  Written specifically for VxWorks and generic UNIX.

   - Version .90, Jeff Hay, jrhay@lanl.gov

   pFTP has many options configurable just by specifying new compiler 
   directives.  A list of all of these variables (along with a brief
   description) can be obtained by extracting all lines that begin with
   "#ifndef" from this header file.  Any default values are also specified.
   (see pftp_vars for a sample UNIX shell script to extract config variables)

*/

#ifndef __pFTP_H__ /* Phoenix FTP Package by Jeff Hay */
#define __pFTP_H__

/*--------
   Platform Defines and Includes
-------- */

#ifndef _pFTP_UNIX    /* Compile for generic UNIX platform [1] */
#define _pFTP_UNIX 1
#endif
#ifndef _pFTP_VXWORKS /* Compile for VxWorks RTOS [0] */
#define _pFTP_VXWORKS 0
#endif

#ifndef FALSE  /* Value compiler uses for to indicate FALSE [0] */
#define FALSE 0
#endif
#ifndef TRUE   /* Value used to indicate a TRUE condition [1] */
#define TRUE 1
#endif

#ifndef MEMCLEAR /* Macro to zero-out memory [bzero((void*)memory, size)] */
#define MEMCLEAR(memory, size) bzero((void *)memory, size)
#endif

/*-------
   Default values for various FTP-related items.  These default values come
  from RFC 959, and should not need to be changed.  But, change them if you
  like!  :)  (or have a need to)
-------- */

#ifndef _pFTP_SERVER_PORT  /* Default FTP command port number [21] */
#define _pFTP_SERVER_PORT 21 
#endif
#ifndef _pFTP_DATA_PORT    /* Default FTP server data port number [20] */
#define _pFTP_DATA_PORT 20   
#endif
#ifndef _pFTP_NEWLINE      /* String indicating end-of-text-line [<CRLF>] */
#define _pFTP_NEWLINE "\x0D\x0A"
#endif

/*--------
  Default values for various variables intrinsic to pFTP.  (ie, not defined
  in RFC 959 - implementation values as opposed to standard values)
-------- */

#ifndef _pFTP_MAX_COMMAND  /* Maximum chars in a FTP command sequence [255] */
#define _pFTP_MAX_COMMAND 255
#endif

#ifndef _pFTP_MAX_TRYS  /* Maximum number of automated sending retrys [5] */
#define _pFTP_MAX_TRYS 5
#endif

#ifndef _pFTP_END_ON_NOTHING /*Abort Communications if recv() returns 0 [TRUE]*/
/* Some OSes (such as Solaris 2.6) implement the recv() function call such 
  that it returns a read amount of 0 if the remote socket cleanly closed the
  connection.  If running under such an OS, this provides a clean way to abort
  the FTP session */
#define _pFTP_END_ON_NOTHING TRUE
#endif

/*--------
  FTP reply specifications.

   FTP replies consists of a three-digit number, a space, and some text.  The
  numeric reply is fixed by standards, and is intended to be read and 
  intrepreted by code.  The text is human-readable, and is generally allowed to
  vary at will.  The following defines aliases for the coder for specifing
  replies as well as default text strings for each reply
-------- */

typedef int FTPstat;

/* 1yz Replies - Positive Preliminary Replies
       - Command is going to happen; expect another reply before proceeding 
       with another command.
*/
#define _pFTP_RESTART_MARK       110
#define _pFTP_SERVICE_READY_TIME 120
#define _pFTP_DATA_OPEN_XFER     125
#define _pFTP_FILE_STAT_OK       150

/* 2yz Replies - Positive Completion Reply
       - Requested command completed sucessfully; proceed with next command.
*/
#define _pFTP_COMMAND_OK         200
#define _pFTP_NOT_IMPLEMENTED    202
#define _pFTP_SYSTEM_STATUS      211
#define _pFTP_DIR_STATUS         212
#define _pFTP_FILE_STATUS        213
#define _pFTP_HELP_MESSAGE       214
#define _pFTP_SYSTEM_TYPE        215
#define _pFTP_READY_NEW_USER     220
#define _pFTP_CLOSE_CONTROL      221
#define _pFTP_DATA_OPEN          225
#define _pFTP_DATA_CLOSE         226
#define _pFTP_PASSIVE_MODE       227
#define _pFTP_USER_LOGGED_IN     230
#define _pFTP_REQUEST_COMPLETE   250
#define _pFTP_DIR_CREATED        257

/* 3yz Replies - Positive Intermediate Reply
       - Command will be completed pending further information.  Usually, a
       specific command is expected to be next 
*/
#define _pFTP_USERNAME_OK        331
#define _pFTP_NEED_ACCOUNT       332
#define _pFTP_REQUEST_PENDING    350

/* 4yz Replies - Transient Negative Completion Reply
       - Command failed, but may succeed again with another try, or with 
         further information from user (ie, account info) 
*/
#define _pFTP_NOT_AVAILABLE      421
#define _pFTP_NO_DATA_CONNECT    425
#define _pFTP_XFER_ABORTED       446
#define _pFTP_REQUEST_REFUSED    450
#define _pFTP_ABORTED_LOCAL      451
#define _pFTP_ABORTED_NO_STORAGE 452

/* 5yz Replies - Permament Negative Completion Reply
       - Command failed. 
*/
#define _pFTP_SYNTAX_ERROR       500
#define _pFTP_INVALID_PARAMS     501
#define _pFTP_NO_COMMAND         502
#define _pFTP_BAD_SEQUENCE       503
#define _pFTP_NO_COMMAND_PARAM   504
#define _pFTP_NOT_LOGGED_IN      530
#define _pFTP_NEED_ACCOUNT_STORE 532
#define _pFTP_FILE_UNAVALIABLE   550
#define _pFTP_PAGE_TYPE_UNKNOWN  551
#define _pFTP_EXCEED_ALLOCATION  552
#define _pFTP_INVALID_NAME       553

#endif /* __pFTP_H__ */


/*--------
  Possible Function-Return Codes
--------*/

#define pFTP_NOERROR  0x0   /* No Error, Success */
#define pFTP_ERROR    0x01  /* Some Un-determined Error */
#define pFTP_NOMEM    0x10  /* Out of memory or other resources */
#define pFTP_NULL     0x20  /* NULL pointer passed; can not continue */

/* Plus any FTP reply code above */

/*--------
  LOG Direction
--------*/

#define LOG_SEND     0x0
#define LOG_RECV     0x1

/*--------
  File Transfer Modes/Types/Structures
--------*/

typedef enum { pASCII, Image } FTP_Type;

#ifndef _pFTP_TYPE /* FTP File Encoding Type [Image] */
#define _pFTP_TYPE Image
#endif

typedef enum { File } FTP_Data;

#ifndef _pFTP_DATA /* FTP Data Type [File] */
#define _pFTP_DATA File
#endif

typedef enum { Stream } FTP_Mode;

#ifndef _pFTP_MODE /* FTP Transfer Mode [Stream] */
#define _pFTP_MODE Stream
#endif

/*--------
  Hook Function Pointers
--------*/
typedef struct pFTPdev *pFTP; /* pointer to data strct defined below */

typedef int ( *pFTPAuthenticate )( char *, char * );
/* Authenticate user.  First string is user name; second string is submitted
password.  Return TRUE if user OK, FALSE otherwise. */

typedef void ( *pFTPHook )( char * );
/* General FTP Hook.  See specific usage for meaning of parameter. */

typedef void ( *pFTPCommand )( pFTP );
/* Hook for replacing FTP server commands or responses. */

/*--------
  FTP Session Data Structure
-------*/

typedef struct pFTPdev
{
    char Ip[16];         /* FTP Address */
    int CommandPort;  /* FTP Command Port */
    int DataPort;     /* Curreently active FTP Data Port */
    int Command;      /* FTP Command Socket */
    int Data;         /* FTP Data Socket */
    int Bx;           /* Identificador (ACE) */
    int BxDat;        /* Identificador (ACE) */

    /* Flags and status indicators */
    int Active;       /* FTP Session (command) currently active? */
    int DataActive;   /* FTP Session Data-Connection currently active? */

    /* FTP Transfer parameters */ 
    FTP_Type Type;
    FTP_Data DataType;
    FTP_Mode Mode;

    /* Hooks to call for certain events */
    pFTPCommand NewLogin,   /* Called when a new client logs into a server */
                Logout;     /* Called when a client exits the server */
    pFTPHook PreSave,    /* Called with filename before saving a file */
             PostSave,   /* Called with filename after saving a file  */
             PreSend,    /* Called with filename before sending a file */
             PostSend;   /* Called with filename after sending a file  */
    pFTPAuthenticate Authenticate;  /* User Authentication Routine */

    pFTPCommand Welcome;  /* Welcome a new client to the FTP server */
} ftpsession;

/*--------
  Default Hook Procedures
--------*/

int pFTPDefaultAuthenticate( char *User, char *Password );
/* Default (simple-minded) user authentication routine.  Accepts user if
   username and password match, otherwise refuses user.  */

/*-------
  Function Prototypes
--------*/

/* All functions returning FTPstat types may return pFTP_NULL or pFTP_NOERROR as
   well as any listed return values */

int TCP_pFTP( unsigned funcion, unsigned bx, char *buff );
/* Funcion principal*/

pFTP NewFTP();
/* Initialize a new FTP Session.  Returns pointer to session info on success
   or NULL if not enough memory is avaliable to init session. */

void EndFTP( pFTP FTP );
/*  End an FTP session immediately.  Always succeeds.  Any open connections
    (Command, Data) are closed, and the FTP structure is free()'d.  */

FTPstat FTP_SetCommandPort( pFTP FTP, int Port );
/* If the FTP session is not active, sets the command port to be "Port"
   (instead of current value or default of _pFTP_SERVER_PORT)  Returns 
   _pFTP_REQUEST_REFUSED if FTP session is currently active. */

FTPstat FTPConnect( pFTP FTP );
/* Connects FTP session to the FTP Returns:
            pFTP_NOMEM = Out of Resources locally; can not create connection
    _pFTP_INVALID_NAME = Invalid/Unknown Host Name or address given
 _pFTP_REQUEST_REFUSED = Connection attempt refused or timed out
                       = FTP Session already active
   _pFTP_NOT_AVAILABLE = No Route to host found or host not responding */

FTPstat FTPSend( int Socket, void *Buffer, int Length );
/*  Should *ONLY* be used internally, by other pFTP routines.  Forcably sends
   a message over Socket - only returns when complete message is sent OR a
   premament system error forces termination of transmission.  Returns ONLY:
      pFTP_NOERROR        -> Sent Successfully
      _pFTP_XFER_ABORTED  -> Max retrys reached when trying to send; gave up
                             waiting for resources to be freed
      _pFTP_ABORTED_LOCAL -> Bad Error (ie, invalid socket, etc) */ 

FTPstat FTPRecv( int Socket, void *Buffer, int *Length );
/*  Should *ONLY* be used internally, by other pFTP routines.  Receives a 
  message from a socket.  Sets Length to the length of the received message;
  Length should originally be maximum message size in bytes.  Returns ONLY:
      pFTP_NOERROR          -> Received Successfully
      _pFTP_XFER_ABORTED    -> Max retrys reached while trying to receive; gave
                               up waiting for resources to be freed
      _pFTP_ABORTED_LOCAL   -> Bad Error (ie, invalid socket, etc)
      _pFTP_NO_DATA_CONNECT -> Other side closed socket; conection terminated
                               (socket automatically closed in this case!) */

FTPstat FTPGetReply( pFTP FTP, char **ReplyMsg );
/* Returns the reply code from the remote FTP.  If "ReplyMsg" is non-null, it is
   assumed to point to an indefinite-length buffer.  On return, it will point to
   a char string containing the entire (potentially multi-line) reply.  If there
   is a memory error allocating the reply message space, the function returns as
   normal, but with "ReplyMsg" pointing to NULL.  "ReplyMsg" may be NULL, in
   which case it is ignored. The space pointed to by ReplyMsg must originally
   be set to NULL; no further user maintaince of this space is required.
   Returns the reply code OR
               pFTP_ERROR  = FTP Session not active (or has been terminated by
                              other side)
               pFTP_NOMEM  = Error reading reply (timedout, no socket bufs, etc) 
   Note that is MAY be possible for the reply code to not correspond with any of
   the #defined reply codes (if the FTP server is running a post-RFC959 FTP)  */

FTPstat FTPSendCommand( pFTP FTP, char *Command );
/* Sends a pre-formed command string (Command, " ", [Parameter]) to the remote
   Attaches _pFTP_NEWLINE before sending.  Returns
          _pFTP_ABORTED_LOCAL = No Resources to send command or read reply */




#pragma pack (pop) 
