#pragma warning (disable:4786)
#include "MsgEmergentes.h"
#include "CConfig.h"
static wxMutex *mutexProtecting;
extern CConfig *Config;
void MsgEmergentes::AddMsg(const wxString &pTitulo, char **pMsg, int pCantLineas)
{
    int cnt = 0,cnt2 = 0;
    while(!mListMsg[cnt].GetTitulo().IsEmpty()){
        cnt++;
        if(cnt == CantMaxMsg){
            cnt = 0;
        }
        Sleep(10);
    }
    mListMsg[cnt].SetTitulo(pTitulo);
    mListMsg[cnt].SetCantLineas(pCantLineas);
    //mutexProtecting->Lock();
    for(cnt2=0;cnt2 < pCantLineas;cnt2++){
        mListMsg[cnt].SetMsg(pMsg[cnt2],cnt2);
    }
    //mutexProtecting->Unlock;
}

void * MsgEmergentes::Entry()
{
    int cnt = 0, i = 0;
    wxBitmap bitmap;
	wxString image;
	image.sprintf("%semergente.gif",Config->getDirSkin());
    mutexProtecting = new wxMutex();
    bitmap.LoadFile( image, wxBITMAP_TYPE_GIF );
    for(i=0;i<4;i++){
        ListPanelEmerg[i] = NULL;
    }
    while(1){
        while(!mListMsg[cnt].GetTitulo().IsEmpty()){
            for(int cnt2=0;cnt2<4;cnt2++){
                /*Nuevo hilo, nuevo msg emergente*/
                if(!ListPanelEmerg[cnt2]){
                    ListPanelEmerg[cnt2] = new PanelEmergente(bitmap,mPosPaneles.x - cWidth,(mPosPaneles.y - (cHeight * cnt2) - (cnt2 * 2)),mListMsg[cnt],mutexProtecting);
                    ListPanelEmerg[cnt2]->wxThread::Create();
                    ListPanelEmerg[cnt2]->SetPriority( 50 );
                    ListPanelEmerg[cnt2]->Run();
                    mListMsg[cnt].SetClear();
                    break;
                }
                Sleep(100);
            }
            for(i=0;i<4;i++){ //controla si terminó algun panel, para eliminarlo
                if(ListPanelEmerg[i] && ListPanelEmerg[i]->getStatusRun() == false){
                    //ListPanelEmerg[i]->Delete();
                    wxDELETE(ListPanelEmerg[i]);
                }
            }
        }
        for(i=0;i<4;i++){ //controla si terminó algun panel, para eliminarlo
            if(ListPanelEmerg[i] && ListPanelEmerg[i]->getStatusRun() == false){
                //ListPanelEmerg[i]->Delete();
                wxDELETE(ListPanelEmerg[i]);
            }
        }
        cnt++;
        if(cnt == CantMaxMsg){
            cnt = 0;
        }
        Sleep(100);
    }
    
}
