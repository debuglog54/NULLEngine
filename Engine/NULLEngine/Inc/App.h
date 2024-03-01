#pragma once

namespace NULLEngine
{
	class AppState;

	struct AppConfig
	{
		std::wstring appName = L"Empty";
		uint32_t winWidth = 1280;
		uint32_t winHeight = 720;
		uint32_t debugDrawLimit = 1000000;
		Vector3 gravity = { 0.0f,-9.81f,0.0f };
		int simulationSteps = 10;
		float fixedTimeStep = 1.0f / 60.0f;
	};

	class App
	{
	public:
		template <class StateType>
		void AddState(size_t stateId)
		{
			static_assert(std::is_base_of_v<AppState, StateType>, "AddState must be type of AppState");
			auto [iter, success] = mAppStates.try_emplace(stateId, nullptr);
			if (success)
			{
				auto& ptr = iter->second;
				ptr = std::make_unique<StateType>();
				if (mCurrentState == nullptr)
				{
					LOG("App ---- Curent state %d", (int)iter->first);
					mCurrentState = ptr.get();
				}
			}
		}

		void ChangeState(size_t stateId);
		void Run();
		void Run(const AppConfig& config);
		void Quit();

	private:
		using AppStateMap = std::map<size_t, std::unique_ptr<AppState>>;

		AppStateMap mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
		bool mRunning = false;
	};
}