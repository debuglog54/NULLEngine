#pragma once
#include "Common.h"

namespace  NULLEngine::EMath
{
    extern const float kPi;
    extern const float kTwoPi;
    extern const float kPiByTwo;
    extern const float kRootTwo;
    extern const float kRootThree;
    extern const float kDegToRad;
    extern const float kRadToDeg;

    template <typename T> inline T Min(T a, T b) { return (a > b) ? b : a; }
    template <typename T> inline T Max(T a, T b) { return (a < b) ? b : a; }
    template <typename T> inline T Clamp(T value, T min, T max) { return Max(min, Min(max, value)); }

    inline Vector2 Lerp(const Vector2& v0, const Vector2& v1, float t) { return v0 + ((v1 - v0) * t); }
    inline Vector3 Lerp(const Vector3& v0, const Vector3& v1, float t) { return v0 + ((v1 - v0) * t); }
    inline Vector4 Lerp(const Vector4& v0, const Vector4& v1, float t) { return v0 + ((v1 - v0) * t);}

    template <typename T> inline T Abs(T value) { return (value >= 0) ? value : -value; }
    inline float Sign(float value) { return (value >= 0.0f) ? 1.0f : -1.0f; }
    inline float Sqr(float value) { return value * value; }
    inline float Sqrt(float value) { return sqrtf(value); }

    inline float MagnitudeSqr(const Vector2& v) { return (v.x * v.x) + (v.y * v.y); }
    inline float MagnitudeSqr(const Vector3& v) { return (v.x * v.x) + (v.y * v.y) + (v.z * v.z); }
    inline float Magnitude(const Vector2& v) { return Sqrt(MagnitudeSqr(v)); }
    inline float Magnitude(const Vector3& v) { return Sqrt(MagnitudeSqr(v)); }
    inline float MagnitudeXZSqr(const Vector3& v) { return (v.x * v.x) + (v.z * v.z); }
    inline float MagnitudeXZ(const Vector3& v) { return Sqrt(MagnitudeXZSqr(v)); }

    inline Vector2 Normalize(const Vector2& v) { return v / Magnitude(v); }
    inline Vector3 Normalize(const Vector3& v) { return v / Magnitude(v); }

    inline float DistanceSqr(const Vector2& a, const Vector2& b) { return MagnitudeSqr(a - b); }
    inline float DistanceSqr(const Vector3& a, const Vector3& b) { return MagnitudeSqr(a - b); }
    inline float Distance(const Vector2& a, const Vector2& b) { return Sqrt(DistanceSqr(a, b)); }
    inline float Distance(const Vector3& a, const Vector3& b) { return Sqrt(DistanceSqr(a, b)); }
    inline float DistanceXZSqr(const Vector3& a, const Vector3& b) { return MagnitudeXZSqr(a - b); }
    inline float DistanceXZ(const Vector3& a, const Vector3& b) { return Sqrt(DistanceXZSqr(a, b)); }

    inline float Dot(const Vector2& a, const Vector2& b) { return (a.x * b.x) + (a.y * b.y); }
    inline float Dot(const Vector3& a, const Vector3& b) { return (a.x * b.x) + (a.y * b.y) + (a.z * b.z); }

    inline Vector3 Cross(const Vector3& a, const Vector3& b) { return Vector3((a.y * b.z) - (a.z * b.y), (a.z * b.x) - (a.x * b.z), (a.x * b.y) - (a.y * b.x)); }

    inline Vector2 Project(const Vector2& v, const Vector2& n) { return n * (Dot(v, n) / Dot(n, n)); }
    inline Vector3 Project(const Vector3& v, const Vector3& n) { return n * (Dot(v, n) / Dot(n, n)); }
    inline Vector2 Reflect(const Vector2& v, const Vector2& normal) { return v - (normal * Dot(v, normal) * 2.0f); }
    inline Vector3 Reflect(const Vector3& v, const Vector3& normal) { return v - (normal * Dot(v, normal) * 2.0f); }

    inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
    {
        float x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
        float y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
        float z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;

        return { x, y, z };
    }

    inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
    {
        float x = v.x * m._11 + v.y * m._21 + v.z * m._31;
        float y = v.x * m._12 + v.y * m._22 + v.z * m._32;
        float z = v.x * m._13 + v.y * m._23 + v.z * m._33;

        return { x, y, z };
    }

    inline Matrix4 Transpose(const Matrix4& m)
    {
        return Matrix4(
            m._11, m._21, m._31, m._41,
            m._12, m._22, m._32, m._42,
            m._13, m._23, m._33, m._43,
            m._14, m._24, m._34, m._44
        );
    }

    inline Matrix4 RotationAxis(const Vector3& axis, float rad)
    {
        const Vector3 u = Normalize(axis);
        const float x = u.x;
        const float y = u.y;
        const float z = u.z;
        const float s = sin(rad);
        const float c = cos(rad);

        return Matrix4
        (
            c + (x * x * (1.0f - c)),
            x * y * (1.0f - c) + z * s,
            x * z * (1.0f - c) - y * s,
            0.0f,

            x * y * (1.0f - c) - z * s,
            c + (y * y * (1.0f - c)),
            y * z * (1.0f - c) + x * s,
            0.0f,

            x * z * (1.0f - c) + y * s,
            y * z * (1.0f - c) - x * s,
            c + (z * z * (1.0f - c)),
            0.0f,

            0.0f,
            0.0f,
            0.0f,
            1.0f
        );
    }

    inline Vector3 GetRight(const Matrix4& m)
    {
        return { m._11, m._12, m._13 };
    }
    inline Vector3 GetUp(const Matrix4& m)
    {
        return { m._21, m._22, m._23 };
    }
    inline Vector3 GetForward(const Matrix4& m)
    {
        return { m._31, m._32, m._33 };
    }
    inline Vector3 GetTranslation(const Matrix4& m)
    {
        return { m._41, m._42, m._43 };
    }
    inline Vector3 GetScale(const Matrix4& m)
    {
        return { m._11, m._22, m._33 };
    }
}