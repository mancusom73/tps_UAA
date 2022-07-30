#include "CDataTime.h"
#include "MainThread.h"
extern MainThread *main_thread;
void CDataTime::StartShow()
{

}

void CDataTime::StopShow()
{

}

void * CDataTime::Entry()
{
    while(1){
        if(panVentas->isShowing()){
            main_thread->Yield();
            DataTime.SetToCurrent(); 
            panVentas->ShowHora(DataTime.FormatTime());
            panVentas->ShowFecha(DataTime.Format("%d/%m/%Y"));
        }
        if(panStandby->isShowing()){
            main_thread->Yield();
            DataTime.SetToCurrent();
            panStandby->ShowHora(DataTime.FormatTime());
            panStandby->ShowFecha(DataTime.Format("%d/%m/%Y"));
        }
        if(panPres->isShowing()){
            main_thread->Yield();
            DataTime.SetToCurrent();
            panPres->ShowHora(DataTime.FormatTime());
        }
        this->Sleep(1000);
    }
}
