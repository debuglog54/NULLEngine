#include "Core/Inc/Core.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    while (true)
    {
        LOG("NEW LOG");
        Sleep(0);
      ASSERT(false, "SMTH");
    }

    return 0;
}