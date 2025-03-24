#pragma once
#include "config.h"

#include "event.h"
#include "container.h"

namespace WACore
{

class WIN_APP_CORE IWPEvent
{
public:
    virtual ~IWPEvent() = default;

    virtual void OnSetFocus(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnKillFocus(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnSize(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnUpdate(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnMove(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnDestroy(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnKeyDown(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnKeyUp(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
    virtual void OnMouse(std::unique_ptr<IContainer>& container, UINT msg, WPARAM wParam, LPARAM lParam){};
};

using WPEventSet = EventSet<HWND, UINT, IWPEvent, UINT, WPARAM, LPARAM>;
void WIN_APP_CORE RegisterWPEventFuncs(WPEventSet& eventSet);

} // namespace WACore
