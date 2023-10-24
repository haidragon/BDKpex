#pragma once
#include <numbers>
#include <cmath>
#define PI 3.14159265359f
struct Vector3
{
	constexpr Vector3(
		const float x = 0.0f,
		const float y = 0.0f,
		const float z = 0.0f) noexcept :
		x(x), y(y), z(z) {}
	constexpr const Vector3& operator-(const Vector3& other) const noexcept
	{
		return Vector3{ x - other.x, y - other.y , z - other.z };
	}

	constexpr const Vector3& operator+(const Vector3& other) const noexcept
	{
		return Vector3{ x + other.x, y + other.y, z + other.z };
	}

	constexpr const Vector3& operator/(const float factor) const noexcept
	{
		return Vector3{ x / factor, y / factor, z / factor };
	}

	constexpr const Vector3& operator*(const float factor) const noexcept
	{
		return Vector3{ x * factor, y * factor, z * factor };
	}

	constexpr const Vector3& ToAngle() const noexcept
	{
		return Vector3{
			std::atan2(-z, std::hypot(x,y)) * (180.0f / PI),
			std::atan2(y , x) * (180.0f / PI),
			0.0f };
	}

	constexpr const bool IsZero() const noexcept
	{
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	float x, y, z;
};

typedef struct ULONGVector3
{
	ULONG x, y, z;
};

typedef struct ULONGmatrix3x4_t
{
	ULONG m[3][4];
};

typedef struct matrix3x4_t
{
	float m[3][4];
};

typedef struct ULONGMatrixA
{
	ULONG u[16];
};

typedef struct MatrixA
{
	float m[16];
};