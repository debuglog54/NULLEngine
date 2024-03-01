#include <NULLEngine/Inc/NULLEngine.h>
#include "GameState.h"

enum class States :size_t
{
    Cube = 0,
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Simple Draw";
    config.winHeight = 720;
    config.winWidth = 1280;

    NULLEngine::App& raidShadowLegends = NULLEngine::MainApp();
    raidShadowLegends.AddState<GameState>((size_t)States::Cube);

    raidShadowLegends.Run(config);
    return 0;
}