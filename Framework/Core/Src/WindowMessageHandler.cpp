#include "Precompiled.h"
#include "DebugUtil.h"
#include "..\Inc\WindowMessageHandler.h"

using namespace NULLEngine::Core;

void WindowMessageHandler::Hook(HWND window, Calback cb)
{
    mWindow = window;
    mPreviousCalback = (Calback)GetWindowLongPtrA(window, GWLP_WNDPROC);
    SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)cb);
}

void WindowMessageHandler::Unhook()
{
    SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviousCalback);
    mWindow = nullptr;
}

LRESULT NULLEngine::Core::WindowMessageHandler::ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    ASSERT(mPreviousCalback, "WindowMessageHandler --- No Callback is hooked.");
    return CallWindowProcA((WNDPROC)mPreviousCalback, window, message, wParam, lParam);
}
