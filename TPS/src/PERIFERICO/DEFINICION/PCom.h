#ifndef __PCOM_H
#define __PCOM_H
//#define _PCOM_LOG
#pragma warning (disable:4786)
#include <wx/event.h>
#include <wx/socket.h>
#include <wx/string.h>
#include <wx/file.h>


class PCom : public wxEvtHandler/*, public wxThread*/
{
  public:
      PCom( wxString pDir, int pPuerto );
      virtual ~PCom();
      //void *Entry();
      bool Send( char *buffer , unsigned int size );
      bool Read( char *buffer , unsigned int *size );
      bool ConnectToServer( );
      void Close();
      bool Connected();
      int Discard();
      void SetTimeout( int timeout );
	  long GetTimeout(  );
      wxString GetErrorMessage();
      wxSocketError GetErrorCode();
  private:
      void OnSocketEvent( wxSocketEvent& event );
      bool ErrorLog( wxSocketError err, wxString method );
      wxSocketClient* PSocket;
      wxString direccion;
      wxIPV4address serverAddress;
      int puerto;
      int objectId;
      //int SOCKET_ID;
      char inBuffer[255];
      int cantRead;
      wxString errorMsg;
      wxSocketError errorCode;
      //Test-reintentos: wxFile check;
//#ifdef _PCOM_LOG
      wxFile *PComLogFile;
//#endif      
      DECLARE_EVENT_TABLE()
};
#endif
