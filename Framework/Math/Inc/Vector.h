#pragma once

namespace NULLEngine::EMath
{
    struct Vector2
    {
        float x, y;

        constexpr Vector2() noexcept : Vector2{ 0.0f } {}
        constexpr Vector2(float f) noexcept : Vector2{ f, f } {}
        constexpr Vector2(float x, float y) noexcept : x{ x }, y{ y } {}

        static Vector2 Zero() { return Vector2(); }
        static Vector2 One() { return Vector2(1.0f, 1.0f); }
        static Vector2 XAxis() { return Vector2(1.0f, 0.0f); }
        static Vector2 YAxis() { return Vector2(0.0f, 1.0f); }

        Vector2 operator-() const { return Vector2(-x, -y); }
        Vector2 operator+(const Vector2& rhs) const { return Vector2(x + rhs.x, y + rhs.y); }
        Vector2 operator-(const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
        Vector2 operator*(float s) const { return Vector2(x * s, y * s); }
        Vector2 operator/(float s) const { return Vector2(x / s, y / s); }

        Vector2& operator+=(const Vector2& rhs) { x += rhs.x; y += rhs.y; return *this; }
        Vector2& operator-=(const Vector2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
        Vector2& operator*=(float s) { x *= s; y *= s; return *this; }
        Vector2& operator/=(float s) { x /= s; y /= s; return *this; }
    };

    struct Vector3
    {
        union
        {
            struct { float x, y, z; };
            std::array<float, 3> v;
        };

        constexpr Vector3() noexcept : Vector3{ 0.0f } {}
        constexpr Vector3(float f) noexcept : Vector3{ f, f, f } {}
        constexpr Vector3(float x, float y, float z) noexcept : x{ x }, y{ y }, z{ z } {}

        static Vector3 Zero() { return Vector3(); }
        static Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
        static Vector3 XAxis() { return Vector3(1.0f, 0.0f, 0.0f); }
        static Vector3 YAxis() { return Vector3(0.0f, 1.0f, 0.0f); }
        static Vector3 ZAxis() { return Vector3(0.0f, 0.0f, 1.0f); }

        Vector3 operator-() const { return Vector3(-x, -y, -z); }
        Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
        Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
        Vector3 operator*(float s) const { return Vector3(x * s, y * s, z * s); }
        Vector3 operator/(float s) const { return Vector3(x / s, y / s, z / s); }

        Vector3& operator+=(const Vector3& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
        Vector3& operator-=(const Vector3& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
        Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
        Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
    };

    struct Vector4
    {
        union { float x; float r; };
        union { float y; float g; };
        union { float z; float b; };
        union { float w; float a; };

        constexpr Vector4() noexcept : Vector4{ 0.0f } {}
        constexpr Vector4(float f) noexcept : Vector4{ f, f, f, f } {}
        constexpr Vector4(float x, float y, float z, float w) noexcept : x{ x }, y{ y }, z{ z }, w{ w } {}

        Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }
        Vector4 operator+(const Vector4& rhs) const { return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
        Vector4 operator-(const Vector4& rhs) const { return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
        Vector4 operator*(const Vector4& rhs) const { return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
        Vector4 operator*(float s) const { return Vector4(x * s, y * s, z * s, w * s); }
        Vector4 operator/(float s) const { return Vector4(x / s, y / s, z / s, w / s); }

        Vector4& operator+=(const Vector4& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
        Vector4& operator-=(const Vector4& rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w; return *this; }
        Vector4& operator*=(const Vector4& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; w *= rhs.w; return *this; }
        Vector4& operator*=(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        Vector4& operator/=(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
    };
}