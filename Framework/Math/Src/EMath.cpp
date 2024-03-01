#include "Precompiled.h"
#include "./Math/Inc/EMath.h";

using namespace NULLEngine::EMath;

// Constants
const float NULLEngine::EMath::kPi = 3.14159265358979f;
const float NULLEngine::EMath::kTwoPi = 6.28318530717958f;
const float NULLEngine::EMath::kPiByTwo = 1.57079632679489f;
const float NULLEngine::EMath::kRootTwo = 1.41421356237309f;
const float NULLEngine::EMath::kRootThree = 1.73205080756887f;
const float NULLEngine::EMath::kDegToRad = kPi / 180.0f;
const float NULLEngine::EMath::kRadToDeg = 180.0f / kPi;

const  Matrix4 Matrix4::Identity = { 1.f, 0.f, 0.f, 0.f,
0.f, 1.f, 0.f, 0.f,
0.f, 0.f, 1.f, 0.f,
0.f, 0.f, 0.f, 1.f };

const  Matrix4 Matrix4::Zero = { 0.f, 0.f, 0.f, 0.f,
0.f, 0.f, 0.f, 0.f,
0.f, 0.f, 0.f, 0.f,
0.f, 0.f, 0.f, 0.f };

const Quaternion Quaternion::Identity = { 0.f, 0.f, 0.f, 1.f };
const Quaternion Quaternion::Zero = { 0.f, 0.f, 0.f, 0.f };