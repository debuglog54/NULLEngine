#pragma once
#include "Common.h"

namespace NULLEngine::Graphics
{
	struct Transform
	{
		EMath::Vector3 position = EMath::Vector3::Zero();
		EMath::Quaternion rotation = EMath::Quaternion::Identity;
		EMath::Vector3 scale = EMath::Vector3::One();

		Transform() = default;
		Transform(const EMath::Matrix4& m)
		{
			position = EMath::GetTranslation(m);
			rotation = EMath::Quaternion::CreateFromRotationMatrix(m);
			scale = EMath::GetScale(m);
		}

		EMath::Matrix4 GetMatrix4() const
		{
			return
			{
				EMath::Matrix4::Scaling(scale) *
				EMath::Matrix4::MatrixRotationQuaternion(rotation) *
				EMath::Matrix4::Translation(position)
			};
		}
	};
}