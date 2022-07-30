#pragma pack (push,8) 
#ifndef __MSG_EMERGENTES_H
#define __MSG_EMERGENTES_H

#include <wx/string.h>
#include "PanelEmergente.h"
#include "CPanel.h"
#include "path.h"
#include <wx/thread.h>

#define CantMaxMsg 16

class MsgEmergentes : public wxThread
{
  public:
    MsgEmergentes(wxPoint pPosPaneles) : wxThread( wxTHREAD_DETACHED )
    {
        mPosPaneles = pPosPaneles;
    };
    void AddMsg(const wxString &pTitulo, char **pMsg, int pCantLineas);
    void * Entry();
  private:
    CMensaje mListMsg[CantMaxMsg];
    PanelEmergente *ListPanelEmerg[4];
    wxPoint mPosPaneles;



};

#endif

#pragma pack (pop) 
