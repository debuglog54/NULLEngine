#pragma once
#include "InputTypes.h"
#include <../External/DirectXTK-main/Inc/GamePad.h>

namespace NULLEngine::Input
{
	class InputSystem final
	{
	public:
		static void StaticInitialize(HWND window);
		static void StaticTerminate();
		static InputSystem* Get();

	public:
		InputSystem() = default;
		~InputSystem();

		InputSystem(const InputSystem&) = delete;
		InputSystem& operator=(const InputSystem&) = delete;

		void Initialize(HWND window);
		void Terminate();

		void Update();

		bool IsKeyDown(KeyCode key) const;
		bool IsKeyPressed(KeyCode key) const;

		bool IsMouseDown(MouseButton button) const;
		bool IsMousePressed(MouseButton button) const;

		int GetMouseMoveX() const;
		int GetMouseMoveY() const;
		float GetMouseMoveZ() const;

		int GetMouseScreenX() const;
		int GetMouseScreenY() const;

		bool IsMouseLeftEdge() const;
		bool IsMouseRightEdge() const;
		bool IsMouseTopEdge() const;
		bool IsMouseBottomEdge() const;

		void ShowSystemCursor(bool show);
		void SetMouseClipToWindow(bool clip);
		bool IsMouseClipToWindow() const;

		bool IsGamePadConnected(int player) const;

		bool IsAPressed(int player) const;
		bool IsBPressed(int player) const;
		bool IsXPressed(int player) const;
		bool IsYPressed(int player) const;

		bool IsLeftShoulderPressed(int player) const;
		bool IsLeftTriggerPressed(int player) const;
		bool IsRightShoulderPressed(int player) const;
		bool IsRightTriggerPressed(int player) const;

		bool IsDPadUp(int player) const;
		bool IsDPadDown(int player) const;
		bool IsDPadLeft(int player) const;
		bool IsDPadRight(int player) const;

		bool IsLeftThumbStickUp(int player) const;
		bool IsLeftThumbStickDown(int player) const;
		bool IsLeftThumbStickLeft(int player) const;
		bool IsLeftThumbStickRight(int player) const;

		bool IsRightThumbStickUp(int player) const;
		bool IsRightThumbStickDown(int player) const;
		bool IsRightThumbStickLeft(int player) const;
		bool IsRightThumbStickRight(int player) const;

		float GetLeftAnalogX(int player) const;
		float GetLeftAnalogY(int player) const;
		float GetRightAnalogX(int player) const;
		float GetRightAnalogY(int player) const;

	private:
		static LRESULT CALLBACK InputSystemMessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

		HWND mWindow = nullptr;

		std::unique_ptr<DirectX::GamePad> mGamePad;
		DirectX::GamePad::State mGamePadState[4]{};

		bool mCurrKeys[512]{};
		bool mPrevKeys[512]{};
		bool mPressedKeys[512]{};

		bool mClipMouseToWindow = false;

		int mCurrMouseX = -1;
		int mCurrMouseY = -1;
		int mPrevMouseX = -1;
		int mPrevMouseY = -1;
		int mMouseMoveX = 0;
		int mMouseMoveY = 0;

		float mMouseWheel = 0.0f;

		bool mCurrMouseButtons[3]{};
		bool mPrevMouseButtons[3]{};
		bool mPressedMouseButtons[3]{};

		bool mMouseLeftEdge = false;
		bool mMouseRightEdge = false;
		bool mMouseTopEdge = false;
		bool mMouseBottomEdge = false;

		bool mInitialized = false;
	};
}