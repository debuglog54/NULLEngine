#include "Precompiled.h"
#include "Camera.h"

#include "GraphicsSystem.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;

void Camera::SetMode(ProjectionMode mode)
{
	mProjectionMode = mode;
}

void Camera::SetPosition(const Vector3& position)
{
	mPosition = position;
}

void Camera::SetDirection(const Vector3& direction)
{
	// Prevent setting direction straight up or down
	 auto dir = Normalize(direction);
	if (EMath::Abs(EMath::Dot(dir, Vector3::YAxis())) < 0.995f)
		mDirection = dir;
}

void Camera::SetLookAt(const Vector3& target)
{
	SetDirection(target - mPosition);
}

void Camera::SetFov(float fov)
{
	const float kMinFov = 10.0f * kDegToRad;
	const float kMaxFov = 170.0f * kDegToRad;
	mFov = Clamp(fov, kMinFov, kMaxFov);
}

void Camera::SetAspectRatio(float ratio)
{
	mAspectRatio = ratio;
}

void Camera::SetSize(float width, float height)
{
	mWidth = width;
	mHeight = height;
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	const Vector3 right = Normalize(Cross(Vector3::YAxis(), mDirection));
	mPosition += right * distance;
}

void Camera::Rise(float distance)
{
	mPosition += Vector3::YAxis() * distance;
}

void Camera::Yaw(float radian)
{
	EMath::Matrix4 matRotate =Matrix4::RotationY(radian);
	mDirection =TransformNormal(mDirection, matRotate);
}

void Camera::Pitch(float radian)
{
	const Vector3 right = Normalize(Cross(EMath::Vector3::YAxis(), mDirection));
	const Matrix4 matRot = RotationAxis(right, radian);
	const Vector3 newLook = TransformNormal(mDirection, matRot);
	SetDirection(newLook);
}

void Camera::Zoom(float amount)
{
	const float minZoom = 170.0f * kDegToRad;
	const float maxZoom = 10.0f * kDegToRad;
	mFov = Clamp(mFov - amount, maxZoom, minZoom);
}

const Vector3& Camera::GetPosition() const
{
	return mPosition;
}

const Vector3& Camera::GetDirection() const
{
	return mDirection;
}

EMath::Matrix4 Camera::GetViewMatrix() const
{
	const Vector3 l = mDirection;
	const Vector3 r = Normalize(EMath::Cross(EMath::Vector3::YAxis(), l));
	const Vector3 u = Normalize(EMath::Cross(l, r));
	const float x = -EMath::Dot(r, mPosition);
	const float y = -EMath::Dot(u, mPosition);
	const float z = -EMath::Dot(l, mPosition);

	return
	{
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		x,   y,   z,   1.0f
	};
}

EMath::Matrix4 Camera::GetProjectionMatrix() const
{
	return (mProjectionMode == ProjectionMode::Perspective) ? GetPerspectiveMatrix() : GetOrthographicMatrix();
}

EMath::Matrix4 Camera::GetPerspectiveMatrix() const
{
	const float a = (mAspectRatio == 0.0f) ? GraphicsSystem::Get()->GetBackBufferAspectRatio() : mAspectRatio;
	const float h = 1.0f / tan(mFov * 0.5f);
	const float w = h / a;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
	const float q = zf / (zf - zn);

	return {
		w,    0.0f, 0.0f,    0.0f,
		0.0f, h,    0.0f,    0.0f,
		0.0f, 0.0f, q,       1.0f,
		0.0f, 0.0f, -zn * q, 0.0f
	};
}

EMath::Matrix4 Camera::GetOrthographicMatrix() const
{
	const float w = (mWidth == 0.0f) ? GraphicsSystem::Get()->GetBackBufferWidth() : mWidth;
	const float h = (mHeight == 0.0f) ? GraphicsSystem::Get()->GetBackBufferHeight() : mHeight;
	const float f = mFarPlane;
	const float n = mNearPlane;
	return 	{
		2 / w, 0.0f,  0.0f,        0.0f,
		0.0f,  2 / h, 0.0f,        0.0f,
		0.0f,  0.0f,  1 / (f - n), 0.0f,
		0.0f,  0.0f,  n / (n - f), 1.0f
	};
}