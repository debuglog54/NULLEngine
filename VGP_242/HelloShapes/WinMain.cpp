#include <NULLEngine/Inc/NULLEngine.h>
#include "GameState.h"

enum class States :size_t
{
    GameState = 0,
    TriForceSymbol = 1,
    Diamond = 2,
    Heart = 3,
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    NULLEngine::App& raidShadowLegends = NULLEngine::MainApp();
    raidShadowLegends.AddState<GameState>((size_t)States::GameState);
    raidShadowLegends.AddState<TriForceSymbol>((size_t)States::TriForceSymbol);
    raidShadowLegends.AddState<Diamond>((size_t)States::Diamond);
    raidShadowLegends.AddState<Heart>((size_t)States::Heart);
    raidShadowLegends.Run();
    
    return 0;
}