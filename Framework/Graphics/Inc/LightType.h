#pragma once
#include "Colors.h"

namespace NULLEngine::Graphics
{
	struct DirectionalLight
	{
		NULLEngine::EMath::Vector3 direction = NULLEngine::EMath::Vector3::Zero();
		float padding = 0.0f;
		Color ambient = Colors::White;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
	};
}