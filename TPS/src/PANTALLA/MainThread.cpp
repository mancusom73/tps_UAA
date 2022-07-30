
#include "MainThread.h"

wxMutex *MainThread::m_mutex;
wxCondition *MainThread::m_condition;
int MainThread::activate_from;
