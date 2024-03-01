#pragma once
#include "Common.h"

namespace NULLEngine::Graphics
{
	enum EaseType
	{
		Linear,
		EaseIn,
		EaseOut,
		EaseInOut,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,
	};
	template<class T>
	struct KeyFrame
	{
		KeyFrame() = default;
		KeyFrame(const T& k, float t, EaseType ease) :key(k), time(t), easeType(ease)
		{

		}

		T key = T();
		float time = 0.0f;
		EaseType easeType = EaseType::Linear;
	};
	
	template<class T>
	using Keyframes = std::vector<KeyFrame<T>>;

	using PositionKeys = Keyframes<NULLEngine::EMath::Vector3>;
	using RotationKeys = Keyframes<NULLEngine::EMath::Quaternion>;
	using ScaleKeys = Keyframes<NULLEngine::EMath::Vector3>;
}