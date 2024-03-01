#pragma once
#include "Precompiled.h"
#include "CameraComponent.h"
#include "CameraService.h"

using namespace NULLEngine;

const Graphics::Camera& CameraService::GetMain() const
{
	ASSERT(mMainCamera != nullptr, "CameraService: no main camera available");
	return mMainCamera->GetCamera();
}

void CameraService::Register(const CameraComponent* cameraComponent)
{
	mCameraEntries.push_back(cameraComponent);
	if (mMainCamera == nullptr)
	{
		mMainCamera = cameraComponent;
	}
}

void CameraService::Unregister(const CameraComponent* cameraComponent)
{
	auto iter = std::find(mCameraEntries.begin(), mCameraEntries.end(), cameraComponent);
	if (iter != mCameraEntries.end())
	{
		if (mMainCamera == *iter)
		{
			mMainCamera = nullptr;
		}

		mCameraEntries.erase(iter);
		if (mMainCamera == nullptr && !mCameraEntries.empty())
		{
			mMainCamera = mCameraEntries.front();
		}
	}
}