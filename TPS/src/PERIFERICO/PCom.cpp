
#include "PCom.h"
#include <wx/thread.h>

//#pragma pack(8)

extern "C"
{
	#include <cr.h>
	extern int DEBE_GRABAR_LOG_SISTEMA1( int origen, int nivel );
}

enum EventTypes
{
        SOCKET_EVENT = 1000,
};
static int lastId = 1;

//Tabla de eventos para el socket
BEGIN_EVENT_TABLE(PCom, wxEvtHandler)
    EVT_SOCKET(SOCKET_EVENT, PCom::OnSocketEvent)
END_EVENT_TABLE()

PCom::PCom( wxString dir, int puerto )/*: wxThread()*/ { 
	if( dir != "no") {
		serverAddress.Hostname( dir );
		serverAddress.Service( puerto );
	}
    PSocket = NULL;
    objectId = lastId++;

//#ifdef _PCOM_LOG
	if( DEBE_GRABAR_LOG_SISTEMA1( LOG_JAVA_SERVER, 4 )) {
    wxString name;
    name.Printf("..\\LOG\\PComDebug%d.log", objectId);
    PComLogFile = new wxFile( name, wxFile::write_append );
	}
//#endif

}

PCom::~PCom() {
    if( PSocket ) {
        Close();
    }

//#ifdef _PCOM_LOG
	if( DEBE_GRABAR_LOG_SISTEMA1( LOG_JAVA_SERVER, 4 )) {
    delete(PComLogFile);
	}
//#endif
}

bool PCom::ConnectToServer() {

    PSocket = new wxSocketClient();
    //Se inicializa el handler y se subscribe a los eventos 
    //conexion exitosa y el de perdida de conexion
    PSocket->SetEventHandler( *this, SOCKET_EVENT );
    PSocket->SetNotify( wxSOCKET_CONNECTION_FLAG /*| wxSOCKET_INPUT_FLAG*/ | wxSOCKET_LOST_FLAG );
    PSocket->Notify( true );
    //Se inicia la conexion
    PSocket->SetTimeout( 5 );
	//PSocket->GetTimeouts();
    PSocket->SetFlags( wxSOCKET_BLOCK ); 
    PSocket->Connect( serverAddress, false );
    PSocket->WaitOnConnect( 10 );
    return ( PSocket->IsConnected() &&  PSocket->Ok() );
}

bool PCom::Send( char *buffer , unsigned int size ) {

    char *netBuff;
	int borrar = 0;

    netBuff = (char*) malloc( size + 2 );
    memcpy( netBuff, buffer, size );
    memcpy( netBuff + size, "\x0D\x0A", 2 );
    PSocket->Write( netBuff, size + 2 );
//#ifdef _PCOM_LOG
	if( DEBE_GRABAR_LOG_SISTEMA1( LOG_JAVA_SERVER, 4 )) {
    borrar = strcmp(buffer ,"isScanner{i0");
	if( borrar != 0) {
			time_t tSac = time(NULL);  // instante actual
	   struct tm* pt1 = localtime(&tSac);
	    wxString fecha_log;
		fecha_log.Printf(" %02i/%02i/%04i - %02i:%02i:%02i ", 
	   pt1->tm_mday,
	   pt1->tm_mon,
	   (pt1->tm_year+1900),
	   pt1->tm_hour,
	   pt1->tm_min,
	   pt1->tm_sec);
		PComLogFile->Write( fecha_log, fecha_log.size() );
		PComLogFile->Write( "> ", 2 );
		
		PComLogFile->Write( netBuff, size + 2 );
	}
	}
//#endif
    free( netBuff );
    return !PSocket->Error();
}

bool PCom::Read( char *buffer , unsigned int *size ) {
    
    int i = 0;
    unsigned int packLen = 0;
    unsigned int headLen = 0, sizeHead = 0;
	int borrar = 0;
    //Test-reintentos: unsigned int headretry = 0, bodyretry = 0;
    //Test-reintentos: char salida[20];

    PSocket->SetFlags(wxSOCKET_BLOCK); 
    //PSocket->WaitForRead(1,0);
    //Se averigua el tama±o de lo que viene
    sizeHead = sizeof( unsigned int ); //debe ser 4!!
    headLen = sizeHead;
    while( headLen > 0 ){
        PSocket->Read( ((char*)&packLen) + ( 4 - headLen ), headLen );
        headLen -= PSocket->LastCount();
        if( PSocket->Error() ){
            ErrorLog( PSocket->LastError(), "READ HEAD" );
            strcpy( buffer, "" );
            *size = 0;
            return false;
        }
        //PSocket->WaitForRead(0,100);
        wxThread::This()->Sleep(3);  //Esto da Error si se llama Read desde el Main Thread
        //Test-reintentos: headretry++;  
        //Yield();
    }
    //Se convierte de Big endian (Java) a Little endian (C++)
    packLen = (packLen>>24) | ((packLen<<8) & 0x00FF0000) | ((packLen>>8) & 0x0000FF00) | (packLen<<24);
    if( packLen > 0 ) { 
        *size = packLen;
        //Se continua leyendo lo faltante
        while( packLen > 0 ) {
            PSocket->Read( inBuffer + ( *size - packLen ), packLen ); //ver si el tam es correcto
            packLen -= PSocket->LastCount();
            if( PSocket->Error() ){
                ErrorLog( PSocket->LastError(), "READ BODY" );
                strcpy( buffer, "" );
                *size = 0;
                return false;
            }
            //PSocket->WaitForRead(0,100);
            wxThread::This()->Sleep(2); //Esto da Error si se llama Read desde el Main Thread
            //Test-reintentos: bodyretry++;
            //Yield();
        }
        memcpy( buffer, inBuffer, *size );
        buffer[*size ] = '\0';
    } else {
        strcpy( buffer, "" );    
    }
//#ifdef _PCOM_LOG
	if( DEBE_GRABAR_LOG_SISTEMA1( LOG_JAVA_SERVER, 4 )) {
	borrar = strcmp(buffer ,"isScanner{-1{");
	if( borrar != 0) {
		
		time_t tSac = time(NULL);  // instante actual
	   struct tm* pt1 = localtime(&tSac);
	    wxString fecha_log;
		fecha_log.Printf(" %02i/%02i/%04i - %02i:%02i:%02i ", 
	   pt1->tm_mday,
	   pt1->tm_mon,
	   (pt1->tm_year+1900),
	   pt1->tm_hour,
	   pt1->tm_min,
	   pt1->tm_sec);
		
		PComLogFile->Write( fecha_log, fecha_log.size() );
		
		PComLogFile->Write( "< ", 2 );
		PComLogFile->Write( buffer, *size );
		PComLogFile->Write( "\n", 1 );
	}
	}
//#endif

    //Test-reintentos: sprintf(salida, "%d %d\n", headretry, bodyretry );
    //Test-reintentos: check.Write( salida );
    //PSocket->SetFlags(wxSOCKET_BLOCK); 
    //PSocket->SetFlags(wxSOCKET_NONE); 

    return true;
}

void PCom::Close() {
    
    if( PSocket != NULL)
    PSocket->Destroy();
}

bool PCom::Connected() {

    return PSocket->IsConnected();
}

int PCom::Discard() {
    
    PSocket->Discard();
    return PSocket->LastCount();
}

void PCom::SetTimeout( int timeout ) {
    PSocket->SetTimeout( timeout );
}
long PCom::GetTimeout(  ) {
    return PSocket->GetTimeouts( );
}

bool PCom::ErrorLog( wxSocketError err, wxString op ) {
    
    errorCode = err;
    switch( err ) {
        case wxSOCKET_NOERROR:     //No error happened. 
            errorMsg.sprintf( "PCom: La operacion %s se realizo correctamente", op.c_str() );
            return false;
            break;
        case wxSOCKET_IOERR:       //Input/Output error.  
            errorMsg.sprintf( "PCom: I/O error en la operacion %s ", op.c_str() );
            break;
        case wxSOCKET_INVOP:       //Invalid operation.  
        case wxSOCKET_INVADDR:     //Invalid address passed to wxSocket.  
        case wxSOCKET_INVSOCK:     //Invalid socket (uninitialized).  
        case wxSOCKET_NOHOST:      //No corresponding host.  
        case wxSOCKET_INVPORT:     //Invalid port.  
        case wxSOCKET_WOULDBLOCK:  //The socket is non-blocking and the operation would block.  
            errorMsg.sprintf( "PCom: Error en la operacion %s ", op.c_str() );
            break;
        case wxSOCKET_TIMEDOUT:    //The timeout for this operation expired.  
            errorMsg.sprintf( "PCom: Timeout en la operacion %s. ", op.c_str() );
            break;
        case wxSOCKET_MEMERR:      //Memory exausted
            errorMsg.sprintf( "PCom: Sin memoria para operar ");
            break;
    }
    //sglog( msg );
    return true;    
}

wxString PCom::GetErrorMessage() {
    
    return errorMsg;
}

wxSocketError PCom::GetErrorCode() {
    
    return errorCode;
}

void PCom::OnSocketEvent(wxSocketEvent& event) {

    //El socket que genero el evento
    wxSocketBase* sock = event.GetSocket();

    switch(event.GetSocketEvent())
    {
        case wxSOCKET_CONNECTION:
        {
            break;
        }
        /*case wxSOCKET_INPUT:
        {
            dataAvailable = true;
            break;
        }*/
        case wxSOCKET_LOST:
        {
            //sock->Destroy();
            //Sleep(1000);
            //ConnectToServer( );
            break;
        }
    }
}
