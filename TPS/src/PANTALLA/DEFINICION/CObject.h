#pragma pack (push,8) 
#ifndef __COBJECT_H
#define __COBJECT_H

#define Uses_wxThread
#pragma warning (disable:4786)
#include "wx/wxprec.h"
#include <wx/image.h>
#include <wx/statline.h>
#include <wx/spinbutt.h>
#include <wx/spinctrl.h>
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/thread.h>
#include "wx/fontenum.h"
#include "wx/fontmap.h"
#include "wx/encconv.h"

#ifdef INVEL_L
#include <wx/frame.h>
#include <wx/app.h>
#include <wx/textctrl.h>
#include <wx/dcmemory.h>
#include <wx/event.h>
#endif

#include <string>
#include <vector>

#include "GuiProperties.h"

using namespace std;

#define  ALIGN_LEFT 1
#define  ALIGN_CENTER 2
#define  ALIGN_RIGHT 3

class CObject
{
  public:
    CObject()
    {
        id = 0;
		refresh = false;
    };
    ~CObject()
    {
    };
    bool GetRefresh()
    {
        return refresh;
    };
    void SetRefresh( bool r = true )
    {
        refresh = r;
    };
    virtual void Draw() = 0;
    virtual void Draw( wxDC *dc ) = 0;
    void SetID( int i );
  private:
    int id;
    bool refresh;
};

void sglog( const char *buffer);

void __mutex_enter( int line, char *file );
void __mutex_leave( int line, char *file );

#define MutexGuiEnter() __mutex_enter(__LINE__,__FILE__)
#define MutexGuiLeave() __mutex_leave(__LINE__,__FILE__)
#endif

#pragma pack (pop) 
