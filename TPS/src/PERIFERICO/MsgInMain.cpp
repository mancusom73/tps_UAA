#include "MsgInMain.h"
#include "cr.h"

extern "C"
{
 	extern /*int*/ void GRABAR_LOG_SISTEMA( char *operacion, int origen, int nivel );
	extern void MENSAJE(char *cadena);
    extern void MENSAJE_STRING( int id );
}

static wxMutex *s_msrMutex;

MsgInMain::MsgInMain() : wxThread( wxTHREAD_DETACHED ) {
    Init("localhost", 2222 );
}

MsgInMain::MsgInMain( char *ipserv, int port ) : wxThread( wxTHREAD_DETACHED ) {
    Init( ipserv, port );
    s_msrMutex = new wxMutex();
}

MsgInMain::~MsgInMain()
{
    if( s_msrMutex ) {
        delete( s_msrMutex );
    }
    if( msgClient ) {
        delete( msgClient );
    }
}

void MsgInMain::Init( wxString ipserv, int port )
{
    doConnect = ipserv.IsSameAs( "no", false )? false : true;
    //mutex_com = new wxMutex(/*wxMUTEX_RECURSIVE*/);
    msgClient = new PCom( ipserv, port );

    if( doConnect ) {
        //wxMutexLocker lock( *mutex_com );
        msgClient->ConnectToServer( );
        msgClient->SetTimeout( 10 );
    }
}


void * MsgInMain::Entry()
{
    unsigned int size;
    char msg[255];
    char log[100];

    //En caso de que no se trabaja con JavaServer no podemos salir de Entry
    //ya que el objeto se autodestruye causando violaciones cuando se intentan
    //ejecutar metodos internos.
    if ( !doConnect ) {
        while( true ) {
            //Dormimos 70000 horas para evitar hacer Wait() desde otro hilo.
            Sleep( ULONG_MAX );
        }
    }
    
    while( !msgClient->Connected() ) {
        if ( !Reconect() ){
            GRABAR_LOG_SISTEMA( "MsgInMain: Imposible conectar. Esperando 5 seg...",LOG_JAVA_SERVER,2 );
            Sleep( 5000 );
        }
    }

    while( true ) {

        //Se bloquea el tiempo configurado, esperando datos.
        if( !msgClient->Read( msg, &size ) ) {
           // printf("%s\n", msgClient->GetErrorMessage().c_str());
            if( wxSOCKET_IOERR == msgClient->GetErrorCode() 
                || !msgClient->Connected() ) {
                if ( !Reconect() ){
                    GRABAR_LOG_SISTEMA( "MsgInMain: Imposible conectar. Esperando 5 seg...",LOG_JAVA_SERVER,2 );
                    Sleep( 5000 );
                }
            }
            msg[0] = '\0';
        }
        
        switch( msg[0] ) {
            case 'K':
                unsigned int numero;
                memcpy( &numero, &msg[2], sizeof( int ) );
                numero = (numero>>24) | ((numero<<8) & 0x00FF0000) | ((numero>>8) & 0x0000FF00) | (numero<<24);
                if( keyQueue.PushElement( numero ) ) {
                    _snprintf(log, sizeof(log), "MsgInMain: se recibe tecla %s%d", &msg[2], numero );
                } else {
                    _snprintf(log, sizeof(log), "MsgInMain: Error, cola de teclas llena %s%d", &msg[2], numero );
                }
                break;
            case 'D':
                _snprintf(log, sizeof(log), "MsgInMain: Datos: %s", &msg[2]);
                ProcesarDatos( &msg[2] );
                break;
            case 'E':
                _snprintf(log, sizeof(log), "MsgInMain: Error: %s", &msg[2]);
                MENSAJE( &msg[4] );
                break;
            default:
                //Cada vez que salta el timeout (10 segundos) entra aca:
                //Chequear lo que sea necesario...
                //_snprintf(log, sizeof(log), "MsgInMain: Skip" );
                break;
        }
        if( msg[0] ) {
            GRABAR_LOG_SISTEMA( log ,LOG_JAVA_SERVER,5);
        }

        TestDestroy();
	    Yield();
        //Sleep( 1 );
    }
    return 0;
}

unsigned int MsgInMain::getKey() {

    unsigned int key;

    if( keyQueue.PopElement( key ) ) {
        return key;
    } else {
        return -999;
    }
}

void MsgInMain::ProcesarDatos( char *datos ) {
    
    if( strncmp(datos, "MSR1", 4) == 0 ) {
        s_msrMutex->Lock();
        MSRTrack1 = &datos[5];
        s_msrMutex->Unlock();
    }
    else if( strncmp(datos, "MSR2", 4) == 0 ) {
        s_msrMutex->Lock();
        MSRTrack2 = &datos[5];
        s_msrMutex->Unlock();
    }
}

wxString MsgInMain::GetMSRTrack1() {
    char strTmp[200];
    wxString track;
    wxMutexLocker lock(*s_msrMutex);
    //********************************LOG***********************************************************
    _snprintf(strTmp,200,"MSR LEN TRACK1: %i",MSRTrack1.length());
    strTmp[199] = 0;
    GRABAR_LOG_SISTEMA( strTmp ,LOG_JAVA_SERVER,4);
    _snprintf(strTmp,200,"MSR BUFFER TRACK1: %s",MSRTrack1.c_str());
    strTmp[199] = 0;
    GRABAR_LOG_SISTEMA( strTmp ,LOG_JAVA_SERVER,4);
    //********************************LOG***********************************************************
    track = MSRTrack1;
    MSRTrack1.Empty();
    return track;
}

wxString MsgInMain::GetMSRTrack2() {
    char strTmp[200];
    wxString track;
    wxMutexLocker lock(*s_msrMutex);
    //********************************LOG***********************************************************
    _snprintf(strTmp,200,"MSR LEN TRACK2: %i",MSRTrack2.length());
    strTmp[199] = 0;
    GRABAR_LOG_SISTEMA( strTmp ,LOG_JAVA_SERVER,4);
    _snprintf(strTmp,200,"MSR BUFFER TRACK2: %s",MSRTrack2.c_str());
    strTmp[199] = 0;
    GRABAR_LOG_SISTEMA( strTmp ,LOG_JAVA_SERVER,4);
    //********************************LOG***********************************************************
    track = MSRTrack2;
    MSRTrack2.Empty();
    return track;
}

int MsgInMain::GetMSREmpty() {
    wxMutexLocker lock(*s_msrMutex);
    return( MSRTrack1.IsEmpty() || MSRTrack2.IsEmpty() );
}

bool MsgInMain::Reconect(int retrys , int timeout ) {
    
    int reintentos = 0;
    
    while( !msgClient->Connected() && reintentos <= retrys ) {
        msgClient->Close();
        Sleep( timeout );
        msgClient->ConnectToServer();
        reintentos ++;
    }
    return ( msgClient->Connected() );
}