#pragma pack (push,8) 
#ifndef __CDATA_TIME_H
#define __CDATA_TIME_H
#define Uses_wxThread
#include <wx/wx.h>
#include <wx/datetime.h>
#include "PanelVentas.h"
#include "PanelStandby.h"
#include "PanelPresentacion.h"
extern PanelVentas *panVentas;
extern PanelStandby *panStandby;
extern PanelPresentacion *panPres;

class CDataTime : public wxThread
{
  private:
    wxDateTime DataTime;
  public:    

    CDataTime() : wxThread( wxTHREAD_DETACHED )
    {
        StartShow();
    };
    void StartShow();
    void StopShow();
    void * Entry();
};
#endif

#pragma pack (pop) 