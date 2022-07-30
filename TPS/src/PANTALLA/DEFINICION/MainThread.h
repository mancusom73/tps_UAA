#pragma pack (push,8) 
/* ------------------------------------------------------------------------------
 * Sistema: TPV 1000 - IV
 * ------------------------------------------------------------------------------
 */

#ifndef __MAINTHREAD_H
#define __MAINTHREAD_H

#include "CObject.h"
#include "AppMain.h"
#include "tpv_dat.h"

extern "C"
{
    extern int CR0( char *version, char *branch, int _argc, char **_argv );
    extern char ver_posvisual[40];
    //extern char ver_branch_posvisual[60];
	extern char ver_branch_posvisual[100];
    extern struct _version VERSION;
}


class MainThread: public wxThread
{
  public:
    MainThread() : wxThread()
    { 
    };
    void Create( AppMain *_app )
    {
        app = _app;
        wxThread::Create();
        m_mutex = new wxMutex();
        m_condition = new wxCondition( *m_mutex );
    };
    void OnExit() 
    {
        delete m_mutex;
        delete m_condition;
    }
    static int activate_from;
    static wxMutex *m_mutex;
    static wxCondition *m_condition;
    
  private:
    AppMain *app;
    virtual ExitCode MainThread::Entry()
    {
        CR0( ver_posvisual,ver_branch_posvisual, ((AppMain *) app)->argc, ((AppMain *) app)->argv );
        exit(0);
        
        /*while(1) {
            m_condition->Wait();
        }*/
        return 0;
    }
};
#endif

#pragma pack (pop) 
