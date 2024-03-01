#pragma once
#include "Precompiled.h"
#include "NULLEngine.h"

using namespace NULLEngine;

App& NULLEngine::MainApp()
{
    static App sApp;
    return sApp;
}
