#pragma once

// framework-headers
#include <Math/Inc/EMath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Transform.h>
#include <Graphics/Inc/Colors.h>

//bullet files 
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>

template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

inline btVector3 ConvertToBtVector3(const NULLEngine::EMath::Vector3& vec)
{
	return { vec.x, vec.y, vec.z };
}

inline const NULLEngine::EMath::Vector3 ConvertToVector3(const btVector3& vec)
{
	return { vec.x(), vec.y(), vec.z() };
}

inline btQuaternion ConvertToBtQuaternion(const NULLEngine::EMath::Quaternion& q)
{
	return { q.x, q.y, q.z, q.w };
}

inline NULLEngine::EMath::Quaternion ConvertToQuaternion(const btQuaternion& q)
{
	return	{ q.x(), q.y(), q.z(), q.w() };
}

inline const NULLEngine::Color ConvertToColor(const btVector3& btColor)
{
	return { btColor.x(), btColor.y(), btColor.z(),1.0f };
}

inline const btTransform ConvertToBtTransform(const NULLEngine::Graphics::Transform& trans)
{
	return btTransform(ConvertToBtQuaternion(trans.rotation),
		ConvertToBtVector3(trans.position));
}

inline const NULLEngine::Graphics::Transform ConvertToBtTransform(const btTransform& trans)
{
	const auto& origin = trans.getOrigin();
	const auto& rotation = trans.getRotation();
	NULLEngine::Graphics::Transform retTrans;
	retTrans.position = ConvertToVector3(origin);
	retTrans.rotation = ConvertToQuaternion(rotation);
	return retTrans;
}

inline void ApplyBtTransformToTransform(const btTransform& btTrans, NULLEngine::Graphics::Transform& trans)
{
	trans.position = ConvertToVector3(btTrans.getOrigin());
	trans.rotation = ConvertToQuaternion(btTrans.getRotation());
}