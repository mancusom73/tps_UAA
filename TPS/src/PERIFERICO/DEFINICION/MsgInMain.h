#pragma pack (push,8) 
#ifndef __MSG_IN_MAIN_H
#define __MSG_IN_MAIN_H
#define Uses_wxThread
#include <wx/wx.h>
#include <string>
#include <vector>
#include <list>
#include <wx/hashmap.h>
#include <wx/tokenzr.h>
#include "PCom.h"
#include "Queue.h"

enum clientLimits
{
    CONNECT_RETRYS = 5,
};

class MsgInMain : public wxThread
{
  private:
     //wxMutex *mutex_com;
     PCom* msgClient;
     bool doConnect;
     bool Reconect( int retrys = CONNECT_RETRYS, int timeout = 700 );
     void ProcesarDatos( char *datos );
     CQueue<unsigned int> keyQueue;
     wxString MSRTrack1;
     wxString MSRTrack2;

  public:

    MsgInMain();
    MsgInMain( char *ipserv, int port );
    virtual ~MsgInMain();
    void Init( wxString ipserv, int port );
    void *Entry();
    unsigned int getKey();
    wxString GetMSRTrack1();
    wxString GetMSRTrack2();
    int GetMSREmpty();
};

#endif
#pragma pack (pop) 