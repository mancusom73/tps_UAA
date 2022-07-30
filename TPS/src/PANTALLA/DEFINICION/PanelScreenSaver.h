#pragma pack (push,8) 
#ifndef __PANEL_SCREEN_SAVER_H
#define __PANEL_SCREEN_SAVER_H
#pragma warning (disable:4786)
#include <string>
#include "CPanel.h"
#include "CInput.h"

#include "CScreencfg.h"

class PanelScreenSaver : public CPanel
{
  public:
    PanelScreenSaver( const wxString &title, const wxPoint &pos, const wxSize &size, long style );

    void createLabels();
    void create();
	CScreencfg *screenXml;
    CInput * getFocus()
    {
		//wxActivateEvent(10020);
        return input;
    };
	private:
	CInput *input;
};

#endif

#pragma pack (pop) 