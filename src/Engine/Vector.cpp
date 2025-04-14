#include "pch.h"
#include "Vector.h"

// ====================================================================
// ===	VECTOR 2													===
// ====================================================================

const Vector2 Vector2::ZERO(0, 0);
const Vector2 Vector2::ONE(1, 1);
const Vector2 Vector2::RIGHT(1, 0);
const Vector2 Vector2::LEFT(-1, 0);
const Vector2 Vector2::UP(0, 1);
const Vector2 Vector2::DOWN(0, -1);

double Vector2::Magnitude()
{
	return sqrt(x * x + y * y);
}

float Vector2::SQ_Magnitude()
{
	return x * x + y * y;
}

void Vector2::Normalize()
{
	*this /= static_cast<float>(Magnitude());
}

Vector2 Vector2::Normalized()
{
	return *this / static_cast<float>(Magnitude());
}

void Vector2::Rotate(double angle)
{
	float _newX = static_cast<float>(cos(angle) * x - sin(angle) * y);
	y = static_cast<float>(sin(angle) * x + cos(angle) * y);
	x = _newX;
}

Vector2 Vector2::Rotated(double angle)
{
	return Vector2(
		static_cast<float>(cos(angle) * x - sin(angle) * y),
		static_cast<float>(sin(angle) * x + cos(angle) * y)
	);
}

bool Vector2::IsColinearTo(const Vector2& other)
{
	return x / other.x == y / other.y;
}

bool Vector2::IsColinearTo(const Vector2& other, const float precision)
{
	return abs(x / other.x - y / other.y) <= precision;
}
//
//sf::Vector2f Vector2::ToSFML() const {
//	return sf::Vector2f(x, y);
//}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator*(const Vector2& other) const
{
	return Vector2(x * other.x, y * other.y);
}

Vector2 Vector2::operator/(const Vector2& other) const
{
	return Vector2(x / other.x, y / other.y);
}

Vector2 Vector2::operator+(const float& other) const
{
	return Vector2(x + other, y + other);
}

Vector2 Vector2::operator-(const float& other) const
{
	return Vector2(x - other, y - other);
}

Vector2 Vector2::operator*(const float& other) const
{
	return Vector2(x * other, y * other);
}

Vector2 Vector2::operator/(const float& other) const
{
	return Vector2(x / other, y / other);
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2& Vector2::operator*=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Vector2& Vector2::operator/=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

Vector2& Vector2::operator+=(const float& other)
{
	x += other;
	y += other;

	return *this;
}

Vector2& Vector2::operator-=(const float& other)
{
	x -= other;
	y -= other;

	return *this;
}

Vector2& Vector2::operator*=(const float& other)
{
	x *= other;
	y *= other;

	return *this;
}

Vector2& Vector2::operator/=(const float& other)
{
	x /= other;
	y /= other;

	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

bool Vector2::operator==(const Vector2& other) const
{
	return x == other.x && y == other.y;
}

bool Vector2::operator==(const float& other) const
{
	return x == other && y == other;
}

bool Vector2::operator<=(const Vector2& other) const
{
	return x <= other.x && y <= other.y;
}

bool Vector2::operator<=(const float& other) const
{
	return x <= other && y <= other;
}

bool Vector2::operator>=(const Vector2& other) const
{
	return x >= other.x && y >= other.y;
}

bool Vector2::operator>=(const float& other) const
{
	return x >= other && y >= other;
}

bool Vector2::operator<(const Vector2& other) const
{
	return x < other.x && y < other.y;
}

bool Vector2::operator<(const float& other) const
{
	return x < other && y < other;
}

bool Vector2::operator>(const Vector2& other) const
{
	return x > other.x && y > other.y;
}

bool Vector2::operator>(const float& other) const
{
	return x > other && y > other;
}

bool Vector2::operator!=(const Vector2& other) const
{
	return !(*this == other);
}

bool Vector2::operator!=(const float& other) const
{
	return !(*this == other);
}

void Vector2::operator=(const float& other)
{
	x = other;
	y = other;
}
//
//Vector2::operator sf::Vector2f()
//{
//	return sf::Vector2f(
//		x,
//		y
//	);
//}

Vector2 operator+(float lhs, const Vector2& rhs)
{
	return rhs + lhs;
}

Vector2 operator-(float lhs, const Vector2& rhs)
{
	return -rhs + lhs;	
}

Vector2 operator*(float lhs, const Vector2& rhs)
{
	return rhs * lhs;
}

std::ostream& operator<<(std::ostream& flux, const Vector2& vec2)
{
	return flux << "(" << vec2.x << ", " << vec2.y << ")";
}



// ====================================================================
// ===	VECTOR 3													===
// ====================================================================

const Vector3 Vector3::ZERO(0, 0, 0);
const Vector3 Vector3::ONE(1, 1, 1);
const Vector3 Vector3::RIGHT(1, 0, 0);
const Vector3 Vector3::LEFT(-1, 0, 0);
const Vector3 Vector3::UP(0, 1, 0);
const Vector3 Vector3::DOWN(0, -1, 0);
const Vector3 Vector3::FORWARD(0, 0, 1);
const Vector3 Vector3::BACKWARD(0, 0, -1);

double Vector3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::SQ_Magnitude()
{
	return x * x + y * y + z * z;
}

void Vector3::Normalize()
{
	*this /= static_cast<float>(Magnitude());
}

Vector3 Vector3::Normalized()
{
	return (*this) / static_cast<float>(Magnitude());
}

bool Vector3::IsColinearTo(const Vector3& other)
{
	Vector3 div = *this / other;
	return (div.x == div.y) && (div.x == div.z) && (div.y == div.z);
}

bool Vector3::IsColinearTo(const Vector3& other, const float precision)
{
	Vector3 div = *this / other;
	Vector3 distance = Vector3(
		abs(div.x - div.y),
		abs(div.x - div.z),
		abs(div.y - div.z)
	);
	return distance <= precision;
}

bool Vector3::IsCoplanar(const Vector3& a, const Vector3& b)
{
	return Dot(*this, CrossProduct(a, b)) == 0;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(const Vector3& other) const
{
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3 Vector3::operator/(const Vector3& other) const
{
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3 Vector3::operator+(const float& other) const
{
	return Vector3(x + other, y + other, z + other);
}

Vector3 Vector3::operator-(const float& other) const
{
	return Vector3(x - other, y - other, z - other);
}

Vector3 Vector3::operator*(const float& other) const
{
	return Vector3(x * other, y * other, z * other);
}

Vector3 Vector3::operator/(const float& other) const
{
	return Vector3(x / other, y / other, z / other);
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

Vector3& Vector3::operator*=(const Vector3& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

Vector3& Vector3::operator/=(const Vector3& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;

	return *this;
}

Vector3& Vector3::operator+=(const float& other)
{
	x += other;
	y += other;
	z += other;

	return *this;
}

Vector3& Vector3::operator-=(const float& other)
{
	x -= other;
	y -= other;
	z -= other;

	return *this;
}

Vector3& Vector3::operator*=(const float& other)
{
	x *= other;
	y *= other;
	z *= other;

	return *this;
}

Vector3& Vector3::operator/=(const float& other)
{
	x /= other;
	y /= other;
	z /= other;

	return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
	return (x == other.x && y == other.y && z == other.z);
}

bool Vector3::operator==(const float& other) const
{
	return (x == other && y == other && z == other);
}

bool Vector3::operator<=(const Vector3& other) const
{
	return (x <= other.x && y <= other.y && z <= other.z);
}

bool Vector3::operator<=(const float& other) const
{
	return (x <= other && y <= other && z <= other);
}

bool Vector3::operator>=(const Vector3& other) const
{
	return (x >= other.x && y >= other.y && z >= other.z);
}

bool Vector3::operator>=(const float& other) const
{
	return (x >= other && y >= other && z >= other);
}

bool Vector3::operator<(const Vector3& other) const
{
	return (x < other.x && y < other.y && z < other.z);
}

bool Vector3::operator<(const float& other) const
{
	return (x < other && y < other && z < other);
}

bool Vector3::operator>(const Vector3& other) const
{
	return (x > other.x && y > other.y && z > other.z);
}

bool Vector3::operator>(const float& other) const
{
	return (x > other && y > other && z > other);
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}

bool Vector3::operator!=(const float& other) const
{
	return !(*this == other);
}

void Vector3::operator=(const float& other)
{
	x = other;
	y = other;
	z = other;
}


Vector3 operator+(float lhs, const Vector3& rhs)
{
	return rhs + lhs;
}

Vector3 operator-(float lhs, const Vector3& rhs)
{
	return -rhs + lhs;
}

Vector3 operator*(float lhs, const Vector3& rhs)
{
	return rhs * lhs;
}

std::ostream& operator<<(std::ostream& flux, const Vector3& vec3)
{
	return flux << "(" << vec3.x << ", " << vec3.y << ", " << vec3.z << ")";
}

double Distance(const Vector2& a, const Vector2& b)
{
	return (b - a).Magnitude();
}

double Distance(const Vector3& a, const Vector3& b)
{
	return (b - a).Magnitude();
}

float Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 CrossProduct(const Vector3& u, const Vector3& v)
{
	return Vector3(
		u.y * v.z - u.z * v.y,
		u.x * v.z - u.x * v.z,
		u.x * v.y - u.y * v.x
	);
}

Vector2 LinearInterpolation(const Vector2& a, const Vector2& b, const float t)
{
	assert(0 <= t && t <= 1);
	return a * (1 - t) + b * t;
}

Vector3 LinearInterpolation(const Vector3& a, const Vector3& b, const float t)
{
	assert(0 <= t && t <= 1);
	return a * (1 - t) + b * t;
}

bool IsNormalize(const Vector2& vec)
{
	return vec.x * vec.x + vec.y * vec.y == 1;
}

bool IsNormalize(const Vector3& vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z == 1;
}

bool IsNormalTo(const Vector2& a, const Vector2& b)
{
	return Dot(a, b) == 0;
}

bool IsNormalTo(const Vector3& a, const Vector3& b)
{
	return Dot(a, b) == 0;
}
