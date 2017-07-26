#include "MyWinEventFilter.h"
#include <windows.h>
#include <QDebug>

MyWinEventFilter::MyWinEventFilter()
{
}

MyWinEventFilter::~MyWinEventFilter()
{

}

bool MyWinEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        MSG* msg = reinterpret_cast<MSG*>(message);
        if(msg->message == WM_HOTKEY)
        {
            qDebug("wParam=%d,keycode=%d,modifiers=%d",msg->wParam,HIWORD(msg->lParam),LOWORD(msg->lParam));
        }
    }
    return true;
}
