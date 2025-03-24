#include "pch.h"
#include "window_proc_event.h"

void WIN_APP_CORE WACore::RegisterWPEventFuncs(WPEventSet &eventSet)
{
    eventSet.AddFunc(WM_SETFOCUS, &IWPEvent::OnSetFocus);
    eventSet.AddFunc(WM_KILLFOCUS, &IWPEvent::OnKillFocus);
    eventSet.AddFunc(WM_PAINT, &IWPEvent::OnUpdate);
    eventSet.AddFunc(WM_SIZE, &IWPEvent::OnSize);
    eventSet.AddFunc(WM_PAINT, &IWPEvent::OnUpdate);
    eventSet.AddFunc(WM_MOVE, &IWPEvent::OnMove);
    eventSet.AddFunc(WM_DESTROY, &IWPEvent::OnDestroy);

    eventSet.AddFunc(WM_KEYDOWN, &IWPEvent::OnKeyDown);
    eventSet.AddFunc(WM_SYSKEYDOWN, &IWPEvent::OnKeyDown);
    eventSet.AddFunc(WM_KEYUP, &IWPEvent::OnKeyUp);
    eventSet.AddFunc(WM_SYSKEYUP, &IWPEvent::OnKeyUp);

    eventSet.AddFunc(WM_LBUTTONDOWN, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_LBUTTONUP, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_RBUTTONDOWN, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_RBUTTONUP, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MBUTTONDOWN, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MBUTTONUP, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MOUSEWHEEL, &IWPEvent::OnMouse);
    eventSet.AddFunc(WM_MOUSEMOVE, &IWPEvent::OnMouse);
}