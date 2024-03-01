#pragma once

namespace NULLEngine::Core
{
    class WindowMessageHandler
    {
    public:
        using Calback = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);

        void Hook(HWND window, Calback cb);
        void Unhook();

        LRESULT ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        HWND mWindow = nullptr;
        Calback mPreviousCalback = nullptr;
    };
}