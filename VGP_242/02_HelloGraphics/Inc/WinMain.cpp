#include <NULLEngine/Inc/NULLEngine.h>

enum class States :size_t
{
    GameState = 0,
};
class GameState : public NULLEngine::AppState
{
public:
    virtual ~GameState() = default;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    NULLEngine::App& raidShadowLegends = NULLEngine::MainApp();
    raidShadowLegends.AddState<GameState>((size_t)States::GameState);
    raidShadowLegends.Run();

    return 0;
}