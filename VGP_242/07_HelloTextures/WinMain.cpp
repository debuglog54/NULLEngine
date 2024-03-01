#include <NULLEngine/Inc/NULLEngine.h>
#include "GameState.h"

enum class States :size_t
{
    Cube = 0,
    Rectangle = 1,
    Plane = 2,
    Cyllinder = 3,
    Sphere = 4,
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    NULLEngine::App& raidShadowLegends = NULLEngine::MainApp();
    raidShadowLegends.AddState<GameState>((size_t)States::Cube);
   //raidShadowLegends.AddState<RectangleDemo>((size_t)States::Rectangle);
   //raidShadowLegends.AddState<PlaneDemo>((size_t)States::Plane);
   //raidShadowLegends.AddState<CylinderDemo>((size_t)States::Cyllinder);
   //raidShadowLegends.AddState<SphereDemo>((size_t)States::Sphere);

    raidShadowLegends.Run();
    return 0;
}