#pragma once

#include "Keyframe.h"
#include "Transform.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::EMath;

namespace NULLEngine::Graphics
{
	class Animation
	{
	public:
		Vector3 GetPosition(float time) const;
		Quaternion GetRotation(float time) const;
		Vector3 GetScale(float time) const;

		Transform GetTransform(float time) const;

		float GetDuration() const;

	private:
		friend class AnimationBuilder;
		friend class AnimationIO;

		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScalesKeys;
		float mDuration;
	};
}