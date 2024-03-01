#include "Precompiled.h"
#include "../Inc/Window.h"

using namespace NULLEngine::Core;

LRESULT CALLBACK WindowMessageHandler(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        // auto save
        // message or block to confirm  "Do you want to save?"
        PostQuitMessage(0);

        return 0;
    }

    return DefWindowProc(handle, message, wParam, lParam);
}

void Window::Initialize(HINSTANCE instance, std::wstring appName, uint32_t width, uint32_t height)
{
    mInstance = instance;
    mAppName = std::move(appName);

    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowMessageHandler;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = instance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = mAppName.c_str();
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    RegisterClassExW(&wcex);

    RECT rc = { 0,0,(LONG)width, (LONG)height };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    const int winWidth = std::min(static_cast<int>(rc.right - rc.left), screenWidth);
    const int winHeight = std::min(static_cast<int>(rc.bottom - rc.top), screenHeight);
    const int left = (screenWidth - winWidth) / 2;
    const int top = (screenHeight - winHeight) / 2;

    mWindow = CreateWindow(
        mAppName.c_str(),
        mAppName.c_str(),
        WS_OVERLAPPEDWINDOW,
        left, top,
        winWidth, winHeight,
        nullptr, nullptr,
        instance, nullptr
    );

    ShowWindow(mWindow, SW_SHOWNORMAL);
    SetCursorPos(screenWidth / 2, screenHeight / 2);

    mActive = (mWindow != nullptr);
}

void Window::Terminate()
{
    DestroyWindow(mWindow);
    UnregisterClassW(mAppName.c_str(), mInstance);

    mWindow = nullptr;
    mInstance = nullptr;
}

void Window::ProcessMessage()
{
    MSG msg{};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (WM_QUIT == msg.message)
        {
            mActive = false;
        }
    }
}