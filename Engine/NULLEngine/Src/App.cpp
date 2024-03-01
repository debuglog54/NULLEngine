#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

//#define _IS_EDITOR 

using namespace NULLEngine;
using namespace NULLEngine::Core;
using namespace NULLEngine::Input;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::Physics;

void NULLEngine::App::ChangeState(size_t stateId)
{
	auto iter = mAppStates.find(stateId);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}

/// <summary>
/// Depricated use Run(const AppConfig& config) instead.
/// </summary>
void App::Run()
{
	AppConfig config;
	config.appName = L"DefaultConfig";
	config.winWidth = 1280;
	config.winHeight = 720;

	ASSERT(false, "Depricated use Run(const AppConfig& config) instead.");

	Run(config);
}

void App::Run(const AppConfig& config)
{
	Window myWindow;
	myWindow.Initialize(GetModuleHandle(nullptr), config.appName, config.winWidth, config.winHeight);

	auto handle = myWindow.GetWindowHandle();
	NULLEngine::Input::InputSystem::StaticInitialize(handle);
	NULLEngine::Graphics::GraphicsSystem::StaticInitialize(handle, false);
	NULLEngine::Graphics::SimpleDraw::StaticInitialize(config.debugDrawLimit);
	NULLEngine::Graphics::DebugUI::StaticInitialize(handle, false, true);
	NULLEngine::Graphics::TextureManager::StaticInitialize("../../Assets/");
	NULLEngine::Graphics::ModelManager::StaticInitialize();

	PhysicsWorld::Settings settings = {
		config.gravity,
		config.simulationSteps,
		config.fixedTimeStep
	};
	PhysicsWorld::StaticInitialize(settings);

	ASSERT(mCurrentState, "App ---- no app state found!");
	mCurrentState->Initialize();
	mRunning = true;
	size_t currentState = 0;
	while (mRunning)
	{
		myWindow.ProcessMessage();

		auto inputSystem = Input::InputSystem::Get();
		inputSystem->Update();
		if (!myWindow.IsActive() || inputSystem->IsKeyPressed(Input::KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		if (inputSystem->IsKeyPressed(Input::KeyCode::ENTER))
		{
			currentState = (currentState + 1) % mAppStates.size();
			ChangeState(currentState);
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();

		}

		auto deltaTime = TimeUtil::GetDeltaTime();
		if (deltaTime < 0.5f)
		{
#ifndef _IS_EDITOR
			PhysicsWorld::Get()->Update(deltaTime);
#endif
			mCurrentState->Update(deltaTime);
		}
		mCurrentState->Update(deltaTime);

		auto graphiceSystem = Graphics::GraphicsSystem::Get();
		graphiceSystem->BeginRender();

		mCurrentState->Render();

#ifdef _IS_EDITOR
		NULLEngine::Graphics::DebugUI::BeginRender();
		mCurrentState->EditorUI();
		NULLEngine::Graphics::DebugUI::EndRender();
#else
		NULLEngine::Graphics::DebugUI::BeginRender();
		mCurrentState->DebugUI();
		NULLEngine::Graphics::DebugUI::EndRender();
#endif // !IS_DEBUG
	
		graphiceSystem->EndRender();
	}
	mCurrentState->Terminate();

	PhysicsWorld::StaticTerminate();
	NULLEngine::Graphics::ModelManager::StaticTerminate();
	NULLEngine::Graphics::TextureManager::StaticTerminate();
	NULLEngine::Graphics::DebugUI::StaticTerminate();
	NULLEngine::Graphics::SimpleDraw::StaticTerminate();
	NULLEngine::Graphics::GraphicsSystem::StaticTerminate();
	NULLEngine::Input::InputSystem::StaticTerminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}
