#pragma once
#include "Vector.h"

namespace NULLEngine::EMath
{
    struct Matrix4;

    //------------------------------------------------------------------------------
    // Quaternion
    struct Quaternion
    {
        float x, y, z, w;
        Quaternion() = default;
        constexpr Quaternion(float ix, float iy, float iz, float iw) noexcept : x(ix), y(iy), z(iz), w(iw) {}

        Quaternion(const Quaternion&) = default;
        Quaternion& operator=(const Quaternion&) = default;

        Quaternion(Quaternion&&) = default;
        Quaternion& operator=(Quaternion&&) = default;

        // Comparison operators
        bool operator == (const Quaternion& q) const { return x == q.x && y == q.y && z == q.z && w == q.w; }
        bool operator != (const Quaternion& q) const { return x != q.x || y != q.y || z != q.z || w != q.w; }

        // Unary operators		
        Quaternion operator+(const Quaternion& rhs) const { return Quaternion(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        Quaternion operator*(float s) const { return Quaternion(x * s, y * s, z * s, w * s); }
        Quaternion operator/(float s) const { return Quaternion(x / s, y / s, z / s, w / s); }

        void Conjugate() noexcept;
        void Conjugate(Quaternion& result) const noexcept;

        void Inverse(Quaternion& result) const noexcept;
        float Dot(const Quaternion& Q) const noexcept;

        static float Magnitude(const Quaternion& q);
        static Quaternion Normalize(const Quaternion& q);

        // Static functions
        static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;
        static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
        static Quaternion CreateFromRotationMatrix(const Matrix4& M) noexcept;
        static Quaternion LookRotation(const Vector3& lookAt,const Vector3& upDirection) noexcept;
        static Quaternion CreateFromEulerAngles(const Vector3 eulers) noexcept;

        static Quaternion Lerp(Quaternion q0, Quaternion q1, float t);
        static Quaternion Slerp(Quaternion q1, Quaternion q2, float t);

        // Constants
        static const Quaternion Identity;
        static const Quaternion Zero;
    };
}