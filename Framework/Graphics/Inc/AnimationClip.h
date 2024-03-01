#pragma once
#include "Animation.h"

namespace NULLEngine::Graphics
{
	class AnimationClip
	{
	public:
		std::string name;
		float tickDuration = 0.0f;
		float ticksPersSecond = 0.0f;

		std::vector<std::unique_ptr<Animation>> boneAnimations;
	};
}