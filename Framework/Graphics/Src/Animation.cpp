#include "Precompiled.h"
#include "Animation.h"

using namespace NULLEngine;
using namespace NULLEngine::Graphics;
using namespace NULLEngine::EMath;

namespace
{
    float GetLerpTime(float startTime, float endTime, float time, EaseType easeType)
    {
        float t = (time - startTime) / (endTime - startTime);

        switch (easeType)
        {
            //case	EaseIn:break;
            //case EaseOut:break;
            //case	EaseInOut:break;
        case EaseInQuad: t = t * t; break;
        case EaseOutQuad: t = -t * (t - 2.0f); break;
        case EaseInOutQuad:
        {
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * t * t;
                break;
            }
            t -= 1.0f;
            t = -0.5f * ((t * (t - 2.0f)) - 1.0f);
            break;
        }
        case Linear: break;

        default:
            break;
        }

        return t;
    }
}

Vector3 Animation::GetPosition(float time) const
{
    for (int i = 0; i < mPositionKeys.size(); ++i)
    {
        if (time < mPositionKeys[i].time)
        {
            if (i > 0)
            {
                float lerpTime = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time, mPositionKeys[i].easeType);
                return Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, lerpTime);
            }
            return mPositionKeys[i].key;
        }
    }

    if (mPositionKeys.size() > 0)
    {
        return mPositionKeys[mPositionKeys.size()-1].key;
    }

    return Vector3::Zero();
}

Quaternion Animation::GetRotation(float time) const
{

    if (mRotationKeys.size() == 1)
    {
        return mRotationKeys[0].key;
    }

    for (int i = 0; i < mRotationKeys.size(); ++i)
    {
        if (time < mRotationKeys[i].time)
        {
            if (i > 0)
            {
                float lerpTime = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time, mRotationKeys[i].easeType);
                return Quaternion::Slerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, lerpTime);
            }
            return mRotationKeys[i].key;
        }
    }

    if (mRotationKeys.size() > 0)
    {
        return mRotationKeys[mRotationKeys.size() - 1].key;
    }

    return Quaternion::Zero;
}

Vector3 Animation::GetScale(float time) const
{
    for (int i = 0; i < mScalesKeys.size(); ++i)
    {
        if (time < mScalesKeys[i].time)
        {
            if (i > 0)
            {
                float lerpTime = GetLerpTime(mScalesKeys[i - 1].time, mScalesKeys[i].time, time, mScalesKeys[i].easeType);
                return Lerp(mScalesKeys[i - 1].key, mScalesKeys[i].key, lerpTime);
            }
            return mScalesKeys[i].key;
        }
    }

    if (mScalesKeys.size() > 0)
    {
        return mScalesKeys[mScalesKeys.size()-1].key;
    }

    return Vector3::One();
}

Transform Animation::GetTransform(float time) const
{
    Transform transform;
    transform.position = GetPosition(time);
    transform.rotation = GetRotation(time);
    transform.scale = GetScale(time);

    return transform;
}

float Animation::GetDuration() const
{
    return mDuration;
}