#pragma once
#include "Animation.h"

using namespace NULLEngine::EMath; 

namespace NULLEngine::Graphics 
{
	class AnimationBuilder 
	{
	public:
		AnimationBuilder& AddPositionKey(const Vector3 position, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddRotationKey(const Quaternion rotation, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddScaleKey(const Vector3 scale, float time, EaseType easeType = EaseType::Linear);
		
		[[nodiscard]] Animation Build();

	private:
		Animation mWorkingCopy;
	};
}