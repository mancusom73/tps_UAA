extern "C"{
#include <cmemory.h> //Nuevo Include
#include <mensaje.h> //Nuevo Include
#ifdef INVEL_W
#include <io.h>
#else
#include <sys/io.h>
#endif
}
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * Esta clase se encarga de proveer coneccion como cliente,
 * en el caso de pipes bajo tcpip
 * en caso de que se use la conecion provista por el acceptor(ej coneccion con
 * un POS) no usar esta clase
 * ------------------------------------------------------------------------------
 */

#include "config.h"
#pragma pack(1)
#include "cr.h"
#include "memory.h"

#include "clientsock.h"
#include "pftp.h"
#include "crc16.h"
#include "bigendian.h"
#include "driver.h"
#include <math.h>
#include <fcntl.h>

#ifdef INVEL_W
#include "conio.h"
#include "CObject.h"
#endif /* INVEL_W */

#ifdef INVEL_L
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
//#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <curses.h>
#include <unistd.h>
#include <sys/io.h>
//#include <ace/Time_Value.h>
#endif /* INVEL_L */
#define DEBUG_TCP
/*---------------------------- Globales ----------------------------------*/
#define CANT_BUFFERS_FTP   8
#define SIZE_BUFFER_FTP  512
#define CANT_CANALES       2
#define BUFFER_CANAL    6144
//#define ACK                6
//#define SOH                1
//#define STX                2
//#define ETX                3

// #define DEBUG_TCP

static char _RTA_ACK[] = {ACK,0,0x20,0,0,STX,ETX,0,0};
static int posicion = 0;
static int posicion_tar = 0;
/*
 * struct _frame{
 * char start;
 * char add;
 * char crl;
 * int  bytes;
 * char buffer[BUFFER_CANAL];
 * };
 */
/*
 * static struct{
 * int  socket;
 * struct _frame frame;
 * }CANAL_TCP[CANT_CANALES];
 *
 * static struct _p_trans{
 * char  *buffer;
 * int   bytes;
 * }far *p_trans;
 */
static struct _p_abrir_socket
{
    char *buffer;
    int bytes;
    int timeout;
    #ifdef INVEL_L
} *p_socket;
#else
} far *p_socket;
#endif /* INVEL_L */

struct _p_ftp
{
    int rta;
    char *ip;
    char *user_name;
    char *password;
    char *file_origen;
    char *file_destino;
    int modo;          // 0 ASCII, 1 BINARIO
    ///////// 24.
    char d_ip[32];
    char d_user_name[32];
    char d_password[32];
    char d_file_origen[64];
    char d_file_destino[64];
    #ifdef INVEL_L
} *p_ftp;
#else
} far *p_ftp;
#endif /* INVEL_L */

/*
static struct _p_ftp 
{
    char     *ip;
    char     *user_name;
    char     *password;
    char     *file_origen;
    char     *file_destino;
    short int modo;                               // 0 ASCII, 1 BINARIO
#ifdef INVEL_L
} *p_ftp;
#else
} far    *p_ftp;
#endif
*/

static int _SOCKET,SA_IN_LEN,TIMEOUT_PING,ftp_handle;
static struct sockaddr_in SA_OUT,SA_IN;
static long nro_seq;
static struct
{
    char buffer[SIZE_BUFFER_FTP];
    int usado;
} BUF_FTP[CANT_BUFFERS_FTP];

//static char HEAP[1000];
//ClientSock server_dbrouter;
//ClientSock server_ping;
struct _rta_drv
{
    int rta;
    char *buffer;
    int l_buffer;
};

servers_tcp servers;
_p_trans *p_trans;
static int canal_on[7];
ACE_Time_Value *FTP_timeOut;

/* The basic constructor just sets our flags to reasonable values.  */
ClientSock::ClientSock( void )
{
    SetInitial();
}


/* This constructor also sets the flags but then calls open().  If the
 * open() fails, the flags will be set appropriately.  Use the two
 * inline method calls initialized() and error() to check the object
 * state after using this constructor.  */
ClientSock::ClientSock( const char *server, u_short port )
{
    SetInitial();
    //this->open (server, port,0);
}


void ClientSock::SetInitial()
{
    error_ = 0;
    //frame.buffer= new char[BUFFER_CANAL];
    timeOut = new ACE_Time_Value( 6, 0 );
    //fabiana
    //SERV_TCP_PORT[_CANAL_DBROUTER] = 6543;
    SERV_TCP_PORT[_CANAL_TARJETAS] = 6543;
    SERV_TCP_PORT[_CANAL_VAL_TELE] = 6543;
    SERV_TCP_PORT[_CANAL_AUTORIZADOR] = 6543;   
    SERV_TCP_PORT[_CANAL_TOUCH_SCREEN]= 3000;
    
    isOpen = 0;
    for( int i = 0;i <= 5;i++ ) {
        canal_on[i] = 0;
    }
}


ClientSock::~ClientSock( void )
{
    //delete frame.buffer;
    delete    timeOut;
}


ClientSock::ClientSock( u_short port, const char *ip )
{
    error_ = 0;
    //frame.buffer= new char[BUFFER_CANAL];
    //this->open ( ip,port,1);
}


/* Open a connection to the server.  This hides the use of
 * ACE_SOCK_Connector from our caller.  Since our caller probably
 * doesn't care *how* we connect, this is a good thing.  */
int ClientSock::open( const char *server, u_short port, int type )
{
    /* This is right out of Tutorial 3.  The only thing we've added is
     * to set the initialized_ member variable on success.  */
    ACE_INET_Addr addr;
    int rta = 0;

    if( type == 0 ) {
        // puerto, nombre del server
        addr.set( port, server );
    }
    else {
        /*    char tmp[30];
            // ip:puerto
            sprintf( tmp, "%s:%d", server, port );
            addr.set( tmp );*/
    }

    addr.set( port, server );

    ACE_SOCK_Connector connector;
    ACE_Time_Value timeout = ACE_DEFAULT_TIMEOUT;
    if( /*!isOpen && */connector.connect( *this, addr, &timeout ) == -1 ) {
        char cadaux[50];
        //    #ifdef INVEL_L
        sprintf( cadaux, "Error Abrir Canal" );
        //    #else
        //    sprintf( cadaux,"Error Abrir Canal: %d", LM_ERROR );
        //    #endif
        MENSAJE_SIN_SONIDO( cadaux,NO );
        isOpen = 0;
        rta = 1;
        //ACE_ERROR_RETURN( ( LM_ERROR, "%p\n", "open" ), -1 );
        //ACE_ERROR(22);
    }


    //isOpen = 1;
    if( !rta ) {
        isOpen = 1;
    }
    return rta;
}


/***************************************************************************/
void ClientSock::init( int bx )
/***************************************************************************/
{
    if( !inicializado ) {
        *( short* )&_RTA_ACK[7] = crc16( 0, _RTA_ACK, 7 );
        inicializado = 1;
        //for( h=0 ; h<CANT_CANALES ; h++ )CANAL_TCP[h].socket = 0;
        if( bx ) {
            //fabiana
            //SERV_TCP_PORT = bx;           
        }
        //cprintf( "Heap Add: %i \r\n", _heapadd( &HEAP, sizeof(HEAP) ) );
        //cprintf( "Heap Test: %i\r\n", _heapchk() );

        switch( bx ) {
            case _CANAL_DBROUTER:
                //SERV_TCP_PORT[bx] = PUERTO_SERVIDOR_DBROUTER;
                break;

            case _CANAL_TARJETAS:
                SERV_TCP_PORT[bx] = PUERTO_CANAL_TCP_IP;
                break;         

            case _CANAL_VAL_TELE:
                break;

			case _CANAL_JGATEWAY:
                SERV_TCP_PORT[bx] = PORT_JGATEWAY;
                break;
        }
    }
}


int ClientSock::handle()
{
    ACE_HANDLE ha;
    short int handle;

    ha = this->get_handle();
    handle = *( short int* )&ha;
    return handle;
}


int ClientSock::Send( struct _p_trans *p_trans, unsigned canal )
{
    int socket = handle();
    int rta = 1, enviados, error, aux;
    unsigned short crc;
    //int handle;

    if( isOpen ) {
        aux = p_trans->bytes ;
        if( socket && aux < BUFFER_CANAL - 4 ) {
            frame.start = SOH;
            frame.add = 0;
            frame.crl = 0x20;
            frame.bytes = ( short int )p_trans->bytes;
            frame.buffer[0] = STX;
            memcpy( &frame.buffer[1], p_trans->buffer, p_trans->bytes );
            frame.buffer[p_trans->bytes + 1] = ETX;
            crc = crc16( 0, ( char* )&frame, p_trans->bytes + 7 );
            /* Lo */
            frame.buffer[p_trans->bytes + 2] = ( crc & 0x00ff );
            /* Hi */
            frame.buffer[p_trans->bytes + 3] = ( crc & 0xff00 ) >> 8;

            error = 1;

            /*switch( canal ) {
                case _CANAL_DBROUTER:
                    #ifdef INVEL_W
                    handle = _open( "dbrouter.snd", _O_CREAT | O_WRONLY, _S_IREAD | _S_IWRITE );
                    if( handle >= 0 ) {
                        if( posicion > 0 ) {
                            _lseek( handle, posicion * ( sizeof( struct _frame ) ), SEEK_SET );
                        }
                        else {
                            _lseek( handle, 0, SEEK_END );
                        }
                        posicion++;
                        error = _write( handle, ( char* )&frame, p_trans->bytes + 6 + 3 );
                        _close( handle );
                    }
                    #endif
                    break;
                case _CANAL_TARJETAS:
                    #ifdef INVEL_W
                    handle = _open( "auton.snd", _O_CREAT | O_WRONLY, _S_IREAD | _S_IWRITE );
                    if( handle >= 0 ) {
                        if( posicion_tar > 0 ) {
                            _lseek( handle, posicion_tar * ( sizeof( struct _frame ) ), SEEK_SET );
                        }
                        else {
                            _lseek( handle, 0, SEEK_END );
                        }
                        posicion_tar++;
                        error = _write( handle, ( char* )&frame, p_trans->bytes + 6 + 3 );
                        _close( handle );
                    }
                    #endif
                    break;
            }*/
            #ifdef INVEL_L
            enviados = send_n( ( void* )( &frame ), p_trans->bytes + 6 + 3 );
            printf( "%s\n", strerror( errno ) );
            #else
            enviados = this->send_n( &frame, p_trans->bytes + 6 + 3 );
            #endif /* INVEL_L */
            if( enviados == p_trans->bytes + 9 ) {
                if(canal != _CANAL_TOUCH_SCREEN){
                    error = recibir_cadena( ACK );
                    rta = ( error ) ? 2 + error : 0;
                }
            }
            else {
                rta = 2;
            }
        }
        return ( rta );
    }
    /* Get the actual data held in the string object */
    /* Send that data to the peer using send_n() as before.  If we
     * have a problem, we'll set error_ so that subsequent <<
     * operations won't try to use a broken stream.  */

    //ACE_OS::memcpy(&Buffer[6],Buff,length);
    /*
     * memcpy(&Buffer__tmp[6],Buff,length);
     * Buffer__tmp[0]=SOH;
     * *((short int *) &Buffer__tmp[3])=length;
     * Buffer__tmp[5]=STX;
     * Buffer__tmp[6+length]=ETX;
     * //CRC
     * Buffer__tmp[7+length]=0;
     * Buffer__tmp[8+length]=0;
     * SET_SHORT( &Buffer__tmp[7+length], crc16( 0, Buffer__tmp, 7 +length));
     *
     * if (this->send_n (Buffer__tmp,length+6+3) == -1)
     * return -1;
     * }
     * else
     */
    /* Be sure that error_ is set if somebody tries to use us when
     * we're not initialized.  */
    //return -1;

    return 1;
}


/***************************************************************************/
int ClientSock::recibir_cadena( char _START )
/***************************************************************************/
{
    //int      rta
    int bytes,cant_bytes;
        /* Utilizado para entrar en rutinas touch screen */
    char touch = _START;

    //unsigned st, lrc, i, cant;
    //unsigned char seguir, error=1, estado ;
    unsigned char error = 1;

    if( get_handle() ) {
        status();
        /*-------------- Inicializaciones -----------*/
        error = 0;

        //bytes = TCP_READ( CANAL_TCP[canal].socket, ((char *)&CANAL_TCP[canal].frame), 5 );
            /* Modificacion para el touch screen */
        if(_START == (char)255 ){
                /* Cambio el tiempo de espera, lo hago m�s chico */
            timeOut->set( 0, 50 );
            bytes = recv_n( ( char* )&frame, 5, timeOut );
                /* Asigno el valor que debe tener _START */
            _START = SOH;          
        }
        else{
            bytes = recv_n( ( char* )&frame, 5, timeOut );
        }
        
        
        if( bytes != 5 ) {

            error = 2;
        }
        else {
            //p = &((char *)&CANAL_TCP[canal].frame)[3];
            //cant_bytes = *(unsigned *)p;
            cant_bytes = frame.bytes;
            if( cant_bytes + 4 > BUFFER_CANAL ) {
                //printf("Desborde de buffer en recepcion. Requeridos %i Limite %i\n", cant_bytes+4, BUFFER_CANAL );
                error = 6;
            }
            else {
                bytes = recv_n( ( char* )&frame + 5, cant_bytes + 4, timeOut );
                //bytes = TCP_READ( CANAL_TCP[canal].socket, ((char *)&CANAL_TCP[canal].frame)+5, cant_bytes+4 );
                if( bytes != cant_bytes + 4 ) {
                    error = 3;
                    #ifdef DEBUG_TCP
                    printf( "Requeridos: %i Leidos: %i", cant_bytes + 4, bytes );
                    getch();
                    #endif /* DEBUG_TCP */
                }
                else {
                    if( ( ( char* )&frame )[0] == _START
                     && ( ( char* )&frame )[cant_bytes + 6] == ETX
                     && ( ( char* )&frame )[5] == STX ) {
                        //printf( "Bytes: %i Cabecera: %x ETX: %x STX: %x\n", cant_bytes+4 , (int)( ((char*)&CANAL_TCP[canal].frame)[0] ),
                        //  (int)(((char *)&CANAL_TCP[canal].frame)[cant_bytes+6]),
                        //  (int)(((char *)&CANAL_TCP[canal].frame)[5]) );

                            /* En la pantalla touch no chequeo el CRC */
                        if(touch != (char)255){
                            if( crc16( 0, ( char* )&frame, cant_bytes + 9 ) ) {
                                error = 5;
                                #ifdef DEBUG_TCP
                                printf( "Error de CRC" );
                                getch();
                                #endif /* DEBUG_TCP */
                            }
                        }
                    }
                    else {
                        /*#ifdef DEBUG_TCP
                                                printf( "Bytes: %i Cabecera: %x [%x] ETX: %x STX: %x\n", cant_bytes,
                                                    ( int ) ( ( ( char * ) &CANAL_TCP[canal].frame )[0] ),
                                                    ( int ) _START,
                                                    ( int ) ( ( ( char * ) &CANAL_TCP[canal].frame )[cant_bytes + 6] ),
                                                    ( int ) ( ( ( char * ) &CANAL_TCP[canal].frame )[5] ) );
                                                getch(  );
                        #endif*/
                        error = 4;
                    }
                }
            }
        }
    }
    //printf( "RECIBIR: %i Bytes: %i Start: %i STX: %i ETX: %i\n", error, i, CANAL_TCP[canal].frame.start, CANAL_TCP[canal].frame.buffer[0], ((char*)&CANAL_TCP[canal].frame)[i-3] );

    return ( error );
}


/***************************************************************************/
int ClientSock::Receive( struct _p_trans *p_trans, int canal )
/***************************************************************************/
{
    int rta = 1, error;

    if( handle() ) {
        if(canal != _CANAL_TOUCH_SCREEN ){
        error = recibir_cadena( SOH );
        }
        else{
                /* Envio un dato 0xff para poder distinguir dentro de la */
                /* funcion que se trata de la pantalla touch screen      */
            error = recibir_cadena( (char)0xFF );
        }

        if( !error ) {
            // Esto debiera ser configurable !!!!
            if( (canal > 0) && (canal != _CANAL_TOUCH_SCREEN )) {
                if( this->send_n( ( char* )&_RTA_ACK, sizeof( _RTA_ACK ) ) != sizeof( _RTA_ACK ) ) {
                    error = 4;
                }
            }
            if( !error ) {
                // lo lleno recibir cadena
                if( frame.bytes <= p_trans->bytes ) {
                    memcpy( p_trans->buffer, ( char* )&frame.buffer[1], frame.bytes );
                    p_trans->bytes = frame.bytes;
                    rta = 0;
                }
                else {
                    rta = 3;
                }
            }
        }
        else {
            rta = 4 + error;
        }
    }
    return ( rta );
}

/***************************************************************************/
int ClientSock::Send_Command( char *command, int largo )
/***************************************************************************/
{
    int bytes;

    bytes = send_n( command, largo );

    return bytes;
}

/***************************************************************************/
int ClientSock::Receive_Command( char *reply, int largo )
/***************************************************************************/
{
    size_t bytes;
    size_t largo_t;
    largo_t = largo;

    //recv_n( reply, largo, timeOut, &bytes );
    bytes = ACE_SOCK_IO::recv( (void *)reply, largo_t, FTP_timeOut );  

    return ( bytes ? bytes : -1 );
}

/***************************************************************************/
int ClientSock::Receive_Data( char *reply, int largo )
/***************************************************************************/
{
    size_t bytes;

    recv_n( reply, largo, FTP_timeOut, &bytes );
    //bytes = recv( reply, largo, timeOut );  

    return ( bytes ? bytes : -1 );
}

int ClientSock::status()
{
    //#if defined (FIONREAD)
    ACE_Handle_Set handle_set;
    handle_set.reset();
    handle_set.set_bit( this->get_handle() );

    //io_vec->iov_base = 0;
    ACE_Time_Value timeout = ACE_Time_Value( 0, 5000 );

    // Check the status of the current socket.

    //   #ifdef INVEL_L
    switch( ACE::select( int ( this->get_handle() ) + 1, handle_set, &timeout ) )
    //   #else
    //   switch ( ACE_OS::select( int ( this->get_handle(  ) ) + 1, handle_set, 0, 0, &timeout ) )
    //   #endif
    {
        case -1:
            return -1;
            /* NOTREACHED */
        case 0:
            errno = ETIME;
            return 0;
            /* NOTREACHED */
        default:
            // Goes fine, fallthrough to get data
            break;
    }

    u_long inlen = 0;              // bytes por leer


    if( ACE_OS::ioctl( this->get_handle(), FIONREAD, ( u_long * )&inlen ) == -1 ) {
        return 0;
    }
    return inlen;
}

extern "C"
{
    /***************************************************************************/
    int TCP( unsigned funcion, unsigned bx, char *buff, int longitud )
    /***************************************************************************/
    {
        int rta;
        rta = 0;
        switch( funcion ) {
            case _TCP_inicializacion:
                //*------------------- Inicializacion ------------------------*
                servers.server[bx].init( bx ); 
                break;

            case _TCP_fin:
                //*------------------------ Fin ------------------------------*
                break;
            case _TCP_version:
                //*----------------------- Version ---------------------------/            
                strcpy( buff, "TCP                 v3.00" );
                break;
            case _TCP_abrir_canal:
                //*----------------------- Abrir Canal ----------------------------*
                //fabiana
                //if( !canal_on )

                if( canal_on[bx] == 0 ) {
                    char *ip;
                    ip = ( ( _rta_drv * )buff )->buffer;
                    rta = servers.server[bx].open( ip, servers.server[bx].SERV_TCP_PORT[bx], 1 );
                    if( !rta ) {
                        //canal_on=1;
                        canal_on[bx] = 1;
                    }
                }
                break;
            case _TCP_cerrar_canal:
                //*----------------------- Cerrar Canal ----------------------------*

                //fabiana
                //if( canal_on ) 

                if( canal_on[bx] == 1 ) {
                    rta = servers.server[bx].close();
                    //canal_on = 0;
                    canal_on[bx] = 0;
                }
                break;

            case _TCP_enviar:
                //*-------------------- Enviar Paquete -----------------------*
                //fabiana
                //if( canal_on ) 

                if( canal_on[bx] == 1 ) {
                    p_trans = ( struct _p_trans * )&( ( ( _rta_drv * )buff )->buffer );
                    rta = servers.server[bx].Send( ( struct _p_trans * )p_trans, bx );
                }
                break;

            case _TCP_recibir:
                //*-------------------- Recibir Paquete -----------------------*
                //fabiana
                //if( canal_on ) 
                if( canal_on[bx] == 1 ) {
                    p_trans = ( struct _p_trans * )&( ( ( _rta_drv * )buff )->buffer );
                    rta = servers.server[bx].Receive( p_trans, bx );
                }
                break;

            case _TCP_status:
                //----------------------- Status ----------------------------
                //if( canal_on )
                if( canal_on[bx] == 1 ) {
                    rta = servers.server[bx].status();
                }
                break;

            case _TCP_abrir_socket_ping:
                //-------------------- Socket Ping --------------------------
                break;

            case _TCP_ping:
                //*-------------------- Ping --------------------------*
                //rta = TCP_ping();
                break;

                // case _TCP_ftp_put:
                // case _TCP_ftp_get:
                //p_ftp = (struct _p_ftp*)(ds * 0x10000 + si + 2);
                //rta = TCP_ftp( funcion );
                //    break;
            case _FTP_get:
            case _FTP_put:
                rta = TCP_pFTP( funcion, bx, buff );
                break;
        }
        return ( rta );
    }
}

/***************************************************************************/
void FTPLog( char *mensaje, int dir )
/***************************************************************************/
{
    char mensajefinal[259];

    _snprintf( mensajefinal, sizeof(mensajefinal)-1, "FTP: %s %s", ( dir == LOG_SEND ) ? "->" : "<-", mensaje );

    GRABAR_LOG_DBROUTER( mensajefinal ,LOG_COMUNICACIONES,4);
}

/***************************************************************************/
FTPstat TCP_pFTP_Autenticar( pFTP FTP, char *user, char *pass )
/***************************************************************************/
{
    //verificar que el comando no mida mas de 28 caracteres

    FTPstat Result,Status=1;
    int End = FALSE;
    //char String[_pFTP_MAX_COMMAND];
    char Command[_pFTP_MAX_COMMAND];
    char *Message = NULL; /* Message must be set to NULL for FTPGetReply() to 
                            function properly */

    while( !End ) {
        Result = FTPGetReply( FTP, &Message );

        switch( Result ) {
            case _pFTP_READY_NEW_USER:
                _snprintf( Command, sizeof(Command)-1, "USER %s", user );
                Status = FTPSendCommand( FTP, Command );
                break;
            case _pFTP_USERNAME_OK:
                _snprintf( Command, sizeof(Command)-1,"PASS %s", pass );
                Status = FTPSendCommand( FTP, Command );
                break;
            case _pFTP_USER_LOGGED_IN:
                //Listo: Autenticado
            default:
                //Algun Error
                End = 1;
                break;
        }
        //Si hay un error enviando un comando
        if( Status != pFTP_NOERROR ) {
            break;
        }
    }

    switch( Result ) {
        case _pFTP_USER_LOGGED_IN:
            break;
        case _pFTP_NOT_LOGGED_IN:
            break;
        case _pFTP_INVALID_NAME:
            break;
        case _pFTP_SYNTAX_ERROR:
            break;
        default:
            break;
    }
    if( Message != NULL ) {
        free( Message );
    }
    return Result;
}
/***************************************************************************/
FTPstat TCP_pFTP_GoPasv( pFTP FTP, int *puerto )
/***************************************************************************/
{
    int p_a,p_b,Result,Status;
    char *Temp1,*Temp2;
    char Command[_pFTP_MAX_COMMAND];
    char *posComa[3];
    char *Message = NULL; /* Message must be set to NULL for FTPGetReply() to 
                            function */
    *puerto = 0;     
    sprintf( Command, "PASV" );
    Status = FTPSendCommand( FTP, Command );
    //verificar Status
    Result = FTPGetReply( FTP, &Message );
    if( Result != _pFTP_PASSIVE_MODE ) {
        if( Message != NULL ) {
            free( Message );
        }
        return Result;
    }

    //se parsea el puerto por el cual el servidor escuchara
    //se asume que la coneccion se establece con el mismo servidor
    //se busca la 4ta y 5ta coma
    Temp2 = Message;
    int coma_i; 
    for( coma_i = 1;( ( Temp1 = ( char * )strstr( Temp2, "," ) ) != NULL ) && ( coma_i < 6 );
         coma_i++ ) {
        Temp2 = Temp1 + 1;
        if( coma_i > 3 ) {
            posComa[coma_i - 4] = Temp2;
        }
    }
    if( coma_i == 6 ) {
        p_a = strtol( posComa[0], NULL, 10 );
        p_b = strtol( posComa[1], NULL, 10 );
        *puerto = p_a * 256 + p_b;
    }
    else {
        if( Message != NULL ) {
            free( Message );
        }
        return pFTP_ERROR;
    }
    if( Message != NULL ) {
        free( Message );
    }
    return Result;
}

/***************************************************************************/
FTPstat TCP_pFTP_PasvFile( pFTP FTP, int operacion, char *arch_local, char *arch_remoto,
                           FTP_Type type )
/***************************************************************************/
{
    #define BUFF_SIZE 4096
    FTPstat Result,Status;
    int End = FALSE;
    char type_transfer[2];
    char Command[_pFTP_MAX_COMMAND];
    char *pdest = NULL, *Message = NULL; /* Message must be set to NULL for FTPGetReply() to 
                                         function properly */
    char listRes[256];
    char datos[BUFF_SIZE];
    int Largo = BUFF_SIZE;
    int bytes = 0;
    int puerto = 0;
    long Last_bl_size = 0;
    int rta;   
    //char TmpLog[50];


    if( type == Image ) {
        type_transfer[0] = 'I';
    }
    else {
        type_transfer[0] = 'A';
    }
    type_transfer[1] = '\0';

    //////////// Cambio de Dir ////////////
    char *dir = NULL;
    char *ultima_barra = arch_remoto;

    while( ( dir = ( char* )strchr( ultima_barra, '/' ) ) != NULL ) {
        ultima_barra = dir + 1;
    }
    //Si hay que hacer cambio de dir
    if( ultima_barra != arch_remoto ) {
        dir = arch_remoto;
        arch_remoto = ultima_barra;
        *( ultima_barra - 1 ) = '\0';

        _snprintf( Command, sizeof(Command)-1, "CWD %s", dir );  
        Status = FTPSendCommand( FTP, Command );
        //Verificar Status
        Result = FTPGetReply( FTP, &Message );
        if( Result != _pFTP_REQUEST_COMPLETE ) {
            if( Message != NULL ) {
                free( Message );
            }
            return Result;
        }
    }
    ///////////////////////////////////////

    ////////// Tama+o de Archivo //////////
    if( operacion == _FTP_get ) {
        //Metodo 1 para averiguar el tamao, si falla,
        //luego se intenta con otro metodo.
        _snprintf( Command, sizeof(Command)-1,"SIZE %s", arch_remoto );
        Status = FTPSendCommand( FTP, Command );
        //verificar Status
        Result = FTPGetReply( FTP, &Message );
        if( Result == _pFTP_FILE_STATUS ) {
            pdest = strchr( Message, ' ' ) + 1;
            Last_bl_size = strtol( pdest, NULL, 10 );
        }
        if( Last_bl_size ) {
            Last_bl_size = ( long )( Last_bl_size
                                   - BUFF_SIZE * ( floor( ( double )Last_bl_size
                                                        / ( double )BUFF_SIZE ) ) );
        }
        //Fallo el metodo 1, se prueba otra forma (LIST)
        else {
            //Modo Pasivo
            Result = TCP_pFTP_GoPasv( FTP, &puerto );
            if( Result != _pFTP_PASSIVE_MODE ) {
                return Result;
            }

            //Conexion para recibir el status en el puerto indicado por servidor
            if( canal_on[FTP->BxDat] == 0 ) {
                servers.server[FTP->BxDat].init( FTP->BxDat );
                rta = servers.server[FTP->BxDat].open( FTP->Ip, puerto, 1 );
                if( !rta ) {
                    canal_on[FTP->BxDat] = 1;
                }
                else {
                    return _pFTP_REQUEST_REFUSED;
                }
            }
            else {
                return _pFTP_NOT_AVAILABLE;
            }

            _snprintf( Command, sizeof(Command)-1, "LIST %s", arch_remoto ); 
            Status = FTPSendCommand( FTP, Command );
            //verificar Status
            Result = FTPGetReply( FTP, &Message );   
            bytes = servers.server[FTP->BxDat].Receive_Data( listRes, 255 );
            //verificar Result
            rta = servers.server[FTP->BxDat].close();
            canal_on[FTP->BxDat] = 0;

            //Parsear el tama+o del archivo. Como la respuesta de LIST varia
            //entre servidores, hay que deducir que parte corresponde al tama+o
            char *token;

            token = strtok( listRes, "\n\t " );
            while( token != NULL ) {
                Last_bl_size = strtol( token, NULL, 10 );
                if( Last_bl_size > 100 ) {
                    //asumo archivo mayor a 100 bytes
                    Last_bl_size = ( long )( Last_bl_size
                                           - BUFF_SIZE * ( floor( ( double )Last_bl_size
                                                                / ( double )BUFF_SIZE ) ) );
                    break;
                }
                token = strtok( NULL, "\n\t " );
            }
            if( token == NULL ) {
                if( Message != NULL ) {
                    free( Message );
                }
                return pFTP_ERROR;
            }
        }
    }
    ///////////////////////////////////////

    //Modo Pasivo
    Result = TCP_pFTP_GoPasv( FTP, &puerto );
    if( Result != _pFTP_PASSIVE_MODE ) {
        return Result;
    }

    //Tipo
    _snprintf( Command, sizeof(Command)-1, "TYPE %s", type_transfer ); 
    Status = FTPSendCommand( FTP, Command );
    //verificar Status
    Result = FTPGetReply( FTP, &Message );
    if( Result != _pFTP_COMMAND_OK ) {
        if( Message != NULL ) {
            free( Message );
        }
        return Result;
    }

    /////////// Transferencia /////////////
	if( canal_on[FTP->BxDat] == 0 ) {
        servers.server[FTP->BxDat].init( FTP->BxDat );
        rta = servers.server[FTP->BxDat].open( FTP->Ip, puerto, 1 );
		{char prueba[100];_snprintf(prueba, sizeof(prueba)-1,"RTA %i BxDat %i Ip %s Puerto %i", rta, FTP->BxDat, FTP->Ip, puerto );glog(prueba,LOG_CON_SERVIDOR,5);
		GRABAR_LOG_DBROUTER(prueba,LOG_DEBUG,4);}
        if( !rta ) {
            canal_on[FTP->BxDat] = 1;
        }
        else {
            return _pFTP_REQUEST_REFUSED;
        }
    }
    else {
		glog(" RTA = _pFTP_NOT_AVAILABLE ",LOG_CON_SERVIDOR,3);
        return _pFTP_NOT_AVAILABLE;
    }

    if( operacion == _FTP_get ) {
        _snprintf( Command, sizeof(Command)-1, "RETR %s", arch_remoto );  
        Status = FTPSendCommand( FTP, Command );
        //verificar Status
        Result = FTPGetReply( FTP, &Message );

        if( Result == _pFTP_FILE_STAT_OK || Result == _pFTP_DATA_CLOSE
         || Result == _pFTP_REQUEST_COMPLETE || Result == _pFTP_DATA_OPEN_XFER ) {
            int fd,byteswritten;

            //if ( (fd = _CREAT("ftpdes.bin",S_IFREG, 763, "")) == -1)
            #ifdef INVEL_L
            if( ( fd = open( arch_local, O_RDWR | O_BINARY | O_TRUNC | O_CREAT, S_IWRITE ) ) == -1 )
                     #else
                if( ( fd
                    = _open( arch_local, _O_RDWR | _O_BINARY | _O_TRUNC | _O_CREAT, _S_IWRITE ) ) ==
                   -1 )
                         #endif /* INVEL_L */
                {
                    return pFTP_ERROR;
                }
            do {
                bytes = servers.server[FTP->BxDat].Receive_Data( datos, Largo );
                #ifdef INVEL_L
                if( bytes > 0 ) {
                    byteswritten = write( fd, ( void* )datos, Largo );
                }
                else {
                    byteswritten = write( fd, ( void* )datos, Last_bl_size );
                }
                #else
                if( bytes > 0 ) {
                    byteswritten = _write( fd, ( void* )datos, Largo );
                }
                else {
                    byteswritten = _write( fd, ( void* )datos, Last_bl_size );
                } 
                #endif /* INVEL_L */
                //, __LINE__, __FILE__
            }
            while( bytes == Largo );
            #ifdef INVEL_L
            close( fd );
            #else
            _commit( fd );
            _close( fd );
            #endif /* INVEL_L */

            rta = servers.server[FTP->BxDat].close();
            canal_on[FTP->BxDat] = 0;

            if( Result == _pFTP_FILE_STAT_OK || Result == _pFTP_DATA_OPEN_XFER ) {
                Result = FTPGetReply( FTP, &Message );
                if( Result != _pFTP_DATA_CLOSE ) {
                    if( Message != NULL ) {
                        free( Message );
                    }
                    return Result;
                }
            }
        }
        else {
            return pFTP_ERROR;
        }
    }
    else {
        //_FTP_put
        _snprintf( Command, sizeof(Command)-1,"STOR %s", arch_remoto );
        Status = FTPSendCommand( FTP, Command );
        //verificar Status
        Result = FTPGetReply( FTP, &Message );

        if( Result == _pFTP_FILE_STAT_OK || Result == _pFTP_DATA_CLOSE
         || Result == _pFTP_REQUEST_COMPLETE || Result == _pFTP_DATA_OPEN_XFER ) {
            int fd,bytesread;
            #ifdef INVEL_L
            if( ( fd = open( arch_local, O_RDONLY | O_BINARY ) ) == -1 )
                     #else
                if( ( fd = _open( arch_local, _O_RDONLY | _O_BINARY ) ) == -1 )
                         #endif /* INVEL_L */
                {
                    return pFTP_ERROR;
                }
            do {
                #ifdef INVEL_L
                bytesread = read( fd, ( void* )datos, Largo );
                #else
                bytesread = _read( fd, ( void* )datos, Largo );
                #endif /* INVEL_L */
                if( bytesread > 0 ) {
                    servers.server[FTP->BxDat].Send_Command( datos, bytesread );
                }
            }
            while( bytesread > 0 );
            #ifdef INVEL_L
            close( fd );
            #else
            _close( fd );
            #endif /* INVEL_L */

            rta = servers.server[FTP->BxDat].close();
            canal_on[FTP->BxDat] = 0;

            if( Result == _pFTP_FILE_STAT_OK || Result == _pFTP_DATA_OPEN_XFER ) {
                Result = FTPGetReply( FTP, &Message );
                if( Result != _pFTP_DATA_CLOSE ) {
                    if( Message != NULL ) {
                        free( Message );
                    }
                    return Result;
                }
            }
        }
        else {
            return pFTP_ERROR;
        }
    }
    ///////////////////////////////////////

    if( Message != NULL ) {
        free( Message );
    }
    return Result;
}

/***************************************************************************/
int TCP_pFTP( unsigned funcion, unsigned bx, char *buff )
/***************************************************************************/
{
    #define PORT 21
    #define DAT_FTP ( ( _p_ftp* ) buff )
    char mens[256];
    pFTP FTP;
    FTPstat Result;
    FTP_Type type;
    FTP_timeOut = new ACE_Time_Value( 5, 400000 );

    if( ( FTP = NewFTP() ) == NULL ) {
        return -1;
    }

    strcpy( FTP->Ip, DAT_FTP->ip );
    FTP_SetCommandPort( FTP, PORT );
    FTP->Bx = bx /*1*/;
    FTP->BxDat = _CANAL_FTP_2;
    type = ( DAT_FTP->modo == 1 ) ? Image : pASCII;

    if( ( Result = FTPConnect( FTP ) ) != pFTP_NOERROR ) {
        FTPLog( "Error de conexion al servidor, verifique IP", LOG_SEND );
        DAT_FTP->rta = 1;
        return 1;
    }
    else {
        Result = TCP_pFTP_Autenticar( FTP, DAT_FTP->d_user_name, DAT_FTP->d_password );
        {char prueba[100];_snprintf(prueba, sizeof(prueba)-1,"1RESULTADO %i USER %s PASS %s", Result, DAT_FTP->d_user_name, DAT_FTP->d_password);glog(prueba,LOG_CON_SERVIDOR,5);
		GRABAR_LOG_DBROUTER(prueba,LOG_DEBUG,4);}
		if( Result == _pFTP_USER_LOGGED_IN ) {
            {char prueba[50];_snprintf(prueba, sizeof(prueba)-1,"FUNCION %i ", funcion );glog(prueba,LOG_CON_SERVIDOR,4);
			GRABAR_LOG_DBROUTER(prueba,LOG_DEBUG,4);}
            if( funcion == _FTP_get ) {
                Result = TCP_pFTP_PasvFile( FTP, funcion, DAT_FTP->d_file_destino,
                                            DAT_FTP->d_file_origen, type );
            }
            else {
                Result = TCP_pFTP_PasvFile( FTP, funcion, DAT_FTP->d_file_origen,
                                            DAT_FTP->d_file_destino, type );
            }
        }
        EndFTP( FTP );
        {char prueba[50];_snprintf(prueba, sizeof(prueba)-1,"2RESULTADO %i ", Result );glog(prueba,LOG_CON_SERVIDOR,5);
		GRABAR_LOG_DBROUTER(prueba,LOG_DEBUG,4);}
        if( Result == _pFTP_REQUEST_REFUSED || Result == _pFTP_NOT_AVAILABLE ) {
            FTPLog( "Error abriendo conexiones para transferir", LOG_SEND );
        }
    }
    if( Result != _pFTP_DATA_CLOSE ) {
        sprintf( mens, "FTP: Error Transferencia" );
        MENSAJE( mens );
        DAT_FTP->rta = 1;
        return 1;
    }
    DAT_FTP->rta = 0;
    delete( FTP_timeOut );
    return 0;
    #undef DAT_FTP
}


//***************************** FUNCIONES pFTP ********************************

int pFTPDefaultAuthenticate( char *User, char *Password )
{
    if( ( User != NULL ) && ( Password != NULL ) && ( !strcmp( User, Password ) ) ) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

pFTP NewFTP()
{
    pFTP FTP;

    FTP = ( pFTP )malloc( sizeof( struct pFTPdev ) );
    if( FTP == NULL ) {
        return NULL;
    }

    FTP->CommandPort = _pFTP_SERVER_PORT;
    FTP->DataPort = _pFTP_DATA_PORT;
    FTP->Command = FTP->Data = 0;
    FTP->Bx = 0;
    FTP->BxDat = 1;

    FTP->Type = _pFTP_TYPE;
    FTP->DataType = _pFTP_DATA;
    FTP->Mode = _pFTP_MODE;

    FTP->NewLogin = FTP->Logout = NULL;
    FTP->PreSave = FTP->PostSave = NULL;
    FTP->PreSend = FTP->PostSend = NULL;
    FTP->Authenticate = pFTPDefaultAuthenticate;
    FTP->Welcome = NULL;

    FTP->Active = FALSE;
    FTP->DataActive = FALSE;

    return FTP;
} /* NewFTP() */

void EndFTP( pFTP FTP )
{
    int rta;

    if( FTP == NULL ) {
        return;
    }

    if( FTP->Active ) {
        FTPSendCommand( FTP, "QUIT" );
        rta = servers.server[FTP->Bx].close();
        canal_on[FTP->Bx] = 0;
    }

    free( FTP );
    return;
} /* EndFTP */

FTPstat FTP_SetCommandPort( pFTP FTP, int Port )
{
    if( FTP == NULL ) {
        return pFTP_NULL;
    }
    if( FTP->Active ) {
        return _pFTP_REQUEST_REFUSED;
    }

    FTP->CommandPort = Port;
    return pFTP_NOERROR;
} /* FTP_SetCommandPort() */ 

FTPstat FTPConnect( pFTP FTP )
{
    int rta;

    if( FTP == NULL ) {
        return pFTP_NULL;
    }
    if( FTP->Active ) {
        return _pFTP_REQUEST_REFUSED;
    }

    if( canal_on[FTP->Bx] == 0 ) {
        rta = servers.server[FTP->Bx].open( FTP->Ip, FTP->CommandPort, 1 );
        if( !rta ) {
            canal_on[FTP->Bx] = 1;
        }
        else {
            return _pFTP_REQUEST_REFUSED;
        }
    }
    else {
        return _pFTP_NOT_AVAILABLE;
    }

    FTP->Active = TRUE;
    return pFTP_NOERROR;
} /* FTPConnect() */

FTPstat FTPSend( int bx, void *buff, int Length )
{
    int bytes;

    bytes = servers.server[bx].Send_Command( ( char* )buff, Length );

    if( bytes < Length ) {
        return _pFTP_ABORTED_LOCAL;
    }

    FTPLog( ( char* )buff, LOG_SEND );
    return pFTP_NOERROR;
} /* FTPSend() */

FTPstat FTPRecv( int bx, void *buff, int *Length )
{
    int bytes;

    bytes = servers.server[bx].Receive_Command( ( char* )buff, *Length );

    *Length = bytes;

    if( *Length < 0 ) {
        /* Error Reading */
        FTPLog( "Error de lectura, reintentar", LOG_RECV );
        return _pFTP_ABORTED_LOCAL;
    }

    #ifdef _pFTP_END_ON_NOTHING
    if( *Length == 0 ) {
        /* Connection Closed cleanly */
        FTPLog( "Error, lectura sin datos, reintentar", LOG_RECV );
        return _pFTP_NO_DATA_CONNECT;
    }
    #endif /* _pFTP_END_ON_NOTHING */
    if( *Length > 0 ) {
        ( ( char* )buff )[*Length] = '\0';
        return pFTP_NOERROR;
    }
    return pFTP_NOERROR;
} /* FTPRecv() */

FTPstat FTPGetReply( pFTP FTP, char **ReplyMsg )
{
    char *NewLine = "\n";
    char Reply[_pFTP_MAX_COMMAND + 1];

    char EndChars[5];
    char *Message,*Temp,*ReplyPart;
    int Size,TotalSize;
    int /*Offset,*/ Count;
    int GotIt;
    FTPstat Result;

    if( FTP == NULL ) {
        return pFTP_NULL;
    }
    if( !FTP->Active ) {
        return pFTP_ERROR;
    }

    Message = NULL;
    GotIt = FALSE;  
    TotalSize = 0;
    EndChars[0] = '\0';
    while( !GotIt ) {
        Size = _pFTP_MAX_COMMAND;
        Result = FTPRecv( FTP->Bx, Reply, &Size );
        if( Result == _pFTP_NO_DATA_CONNECT ) {
            FTP->Active = FALSE;
            return pFTP_ERROR;
        }
        if( Result != pFTP_NOERROR ) {
            return pFTP_NOMEM;
        }

        /* Store multi-line message */
        /*  (unsafe if strlen(NewLine) > strlen(_pFTP_NEWLINE)) */
        if( strlen( EndChars ) < 4 ) {
            strncat( EndChars, Reply, 4 );
        }

        TotalSize = TotalSize + strlen( Reply );
        if( ( Temp = ( char* )realloc( Message, TotalSize ) ) != NULL ) {
            Message = Temp;
            //nico
            Message[0] = '\0';
            ReplyPart = Reply;
            while( ( Temp = ( char* )strstr( ReplyPart, _pFTP_NEWLINE ) ) != NULL ) {
                for( Count = 0;Count < (int)strlen( _pFTP_NEWLINE );Count++ ) {
                    Temp[Count] = '\0';
                }
                strcat( Message, ReplyPart );
                FTPLog( Message, LOG_RECV );
                strcat( Message, NewLine );
                ReplyPart = ( Temp + Count );
                if( strlen( ReplyPart ) > 0 ) {
                    strncpy( EndChars, ReplyPart, 4 );
                }
                else {
                    GotIt = TRUE;
                } /* Input ends with newline, so this is potentially
                    the end of the entire reply message */
            }
            strcat( Message, ReplyPart );
        }
        else {
            GotIt = TRUE;
        }  /* realloc() failed!  To be safe, check if this could be
           the last line of the reply message */

        if( GotIt ) {
            /* Stop reading reply message iff the first four characters after
               the last newline are a three digit number followed by a space */
            Result = ( FTPstat )strtoul( EndChars, NULL, 10 );
            GotIt = ( ( EndChars[3] == ' ' ) && ( Result > 100 ) && ( Result < 999 ) );
        }
    } /* while (!GotIt) */

    if( ReplyMsg != NULL ) {
        if( *ReplyMsg != NULL ) {
            free( *ReplyMsg );
        }
        *ReplyMsg = Message;
    }

    return Result;
} /* FTPGetReply() */

FTPstat FTPSendCommand( pFTP FTP, char *Command )
{
    char *NewLine = "\n";
    FTPstat Result;
    char *CmdString;
    //char CmdString[30];

    if( FTP == NULL ) {
        return pFTP_NULL;
    }
    if( !FTP->Active ) {
        return _pFTP_REQUEST_REFUSED;
    }

    /* Strip off any trailing "\n" */
    if( !strcmp( Command + ( strlen( Command ) - strlen( NewLine ) ), NewLine ) ) {
        Command[strlen( Command ) - strlen( NewLine )] = '\0';
    }

    CmdString = (char *)malloc(strlen(Command) + strlen(_pFTP_NEWLINE) + 1 );
    if (CmdString == NULL)
    return _pFTP_ABORTED_LOCAL;

    strcpy( CmdString, Command );
    strcat( CmdString, _pFTP_NEWLINE );
    Result = FTPSend( FTP->Bx, CmdString, strlen( CmdString ) );
    free(CmdString);

    if( Result != pFTP_NOERROR ) {
        return Result;
    }
    else {
        return pFTP_NOERROR;
    }
} /* FTPSendCommand() */


//**************************** FIN FUNCIONES pFTP ****************************



