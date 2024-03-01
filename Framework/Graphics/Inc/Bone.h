#pragma once
#include "Common.h"

namespace NULLEngine::Graphics
{
	struct Bone
	{
		std::string name;
		int index = -1;

		Bone* parent = nullptr;
		int parentIndex = 1;

		std::vector<Bone*> children;
		std::vector<int> childrenIndices;

		EMath::Matrix4 toParentTransform;
		EMath::Matrix4 offsetTransform;
	};
}