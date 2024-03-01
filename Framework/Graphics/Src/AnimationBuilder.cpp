#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::EMath;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float time, EaseType easeType)
	{
		ASSERT(keyframes.empty() || keyframes.back().time < time, "AnimatioBuilder -- Cannot add keyframe back in time.");
		keyframes.emplace_back(value, time, easeType);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const Vector3 position, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mPositionKeys, position, time, easeType);
	mWorkingCopy.mDuration = Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const Quaternion rotation, float time, EaseType easeType)
{
	PushKey(mWorkingCopy.mRotationKeys, rotation, time, easeType);
	mWorkingCopy.mDuration = Max(mWorkingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const Vector3 scale, float time, EaseType easeType )
{
	PushKey(mWorkingCopy.mScalesKeys, scale, time, easeType);
	mWorkingCopy.mDuration = Max(mWorkingCopy.mDuration, time);
	return *this;
}

Animation AnimationBuilder::Build()
{
	ASSERT(!mWorkingCopy.mPositionKeys.empty() || !mWorkingCopy.mRotationKeys.empty() || !mWorkingCopy.mScalesKeys.empty(), "AnimationBuilder -- Animation has no keys");
	return std::move(mWorkingCopy);
}