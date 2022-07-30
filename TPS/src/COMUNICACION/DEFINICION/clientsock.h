#pragma pack (push,1) 
#ifndef CLIENTSOCK_H
#define CLIENTSOCK_H

// $Id: clientsock.h,v 1.4.2.3 2005/04/19 17:28:52 cvs_nicolas Exp $

/* We need the connector object & we also bring in a simple string
 * class.  */
#include <ace/SOCK_Connector.h>
#include <ace/SString.h>
#include <ace/Handle_Set.h>



/* In this tutorial, we extend SOCK_Stream by adding a few wrappers
 * around the send_n() method.  */
struct _frame
{
    char start;
    char add;
    char crl;
    short int bytes;
    //char      buffer[6144];
    char buffer[12144];
};

struct _p_trans
{
    char *buffer;
    int bytes;
};

class ClientSock : public ACE_SOCK_Stream
{
  public:
    // Basic constructor
    ClientSock( void );

    /* Construct and open() in one call.  This isn't generally a good
     * idea because you don't have a clean way to inform the caller when
     * open() fails.  (Unless you use C++ exceptions.)  */
    ClientSock( const char *server, u_short port );
    ClientSock( u_short port, const char *ip );
    ClientSock::~ClientSock( void );
    /* Open the connection to the server.  Notice that this mirrors the
     * use of ACE_SOCK_Connector.  By providing our own open(), we can
     * hide the connector from our caller & make it's interaction easier.  */
    int open( const char *server, u_short port, int Type );

    /* These are necessary if you're going to use the constructor that
     * invokes open().  */
    //int initialized (void) { return initialized_; }
    int error( void )
    {
        return error_;
    } 
    int ClientSock::handle();
    int PutData( char *Buff, int length );
    //int       ClientSock::Send( struct _p_trans *p_trans );
    int ClientSock::Send( struct _p_trans *p_trans, unsigned canal );
    int Send_Command( char *command, int largo );
    int ClientSock::Receive( struct _p_trans *p_trans, int canal );
    int ClientSock::Receive_Command( char *reply, int largo );
    int ClientSock::Receive_Data( char *reply, int largo );
    int ClientSock::status();
    void ClientSock::init( int bx );
    int inicializado,SERV_TCP_PORT[7];

  protected:
    //u_char initialized_;
    u_char error_;
    _frame frame;

  private:
    int isOpen;
    char Buffer__tmp[2048];
    void ClientSock::SetInitial();
    ACE_Time_Value *timeOut;
    int ClientSock::recibir_cadena( char _START );

    ACE_Handle_Set handle_set;
};

#define ACK                     0x06
#define SOH                     0x01
#define STX                     0x02
#define ETX                     0x03
#define NAK                     0x15
class     servers_tcp
{
  public:
    ClientSock server[7];
};
#endif
#pragma pack (pop) 
