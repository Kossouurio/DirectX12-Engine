#ifndef VECTOR_H__
#define VECTOR_H__

//#include <iostream>
//#include <assert.h>/*
//#include <SFML/Graphics.hpp>*/

class Vector2 {
public:
	float x, y;
	Vector2(float _x = 0, float _y = 0) : x(_x), y(_y) {};

	/*template<typename T>
	Vector2(sf::Vector2<T>& _vec);
	template<typename T>
	Vector2(const sf::Vector2<T>& _vec) : x(static_cast<float>(_vec.x)), y(static_cast<float>(_vec.y)) {}*/


	// Return the length of the Vector
	double Magnitude();
	// Return the length squared of the Vector
	float SQ_Magnitude();

	// Set the Length of the Vector to 1
	// without changing it's direction
	void Normalize();
	// Return the direction of the Vector as a unit Vector
	Vector2 Normalized();

	// Turn The Vector of <angle> radians
	void Rotate(double angle);
	// Return the Vector turned by <angle> radians
	Vector2 Rotated(double angle);

	// Return if this Vector is colinear to an other Vector
	bool IsColinearTo(const Vector2&);
	// Return if this Vector is colinear to an other Vector with a set precision
	bool IsColinearTo(const Vector2&, const float);


public:
	// = (A.x + B.x, A.y + B.y)
	Vector2 operator + (const Vector2&) const;
	// = (A.x - B.x, A.y - B.y)
	Vector2 operator - (const Vector2&) const;
	// = (A.x * B.x, A.y * B.y)
	Vector2 operator * (const Vector2&) const;
	// = (A.x / B.x, A.y / B.y)
	Vector2 operator / (const Vector2&) const;

	// = (A.x + B, A.y + B)
	Vector2 operator + (const float&) const;
	// = (A.x - B, A.y - B)
	Vector2 operator - (const float&) const;
	// = (A.x * B, A.y * B)
	Vector2 operator * (const float&) const;
	// = (A.x / B, A.y / B)
	Vector2 operator / (const float&) const;

	// = (-A.x, -A.y)
	Vector2 operator - () const;

	// = (A.x + B.x, A.y + B.y)
	Vector2& operator += (const Vector2&);	
	// = (A.x - B.x, A.y - B.y)
	Vector2& operator -= (const Vector2&);
	// = (A.x * B.x, A.y * B.y)
	Vector2& operator *= (const Vector2&);
	// = (A.x / B.x, A.y / B.y)
	Vector2& operator /= (const Vector2&);

	// = (A.x + B, A.y + B)
	Vector2& operator += (const float&);
	// = (A.x - B, A.y - B)
	Vector2& operator -= (const float&);
	// = (A.x * B, A.y * B)
	Vector2& operator *= (const float&);
	// = (A.x / B, A.y / B)
	Vector2& operator /= (const float&);

	// = A.x == B.x && A.y == B.y
	bool operator == (const Vector2&) const;
	// = A.x == B && A.y == B
	bool operator == (const float&) const;

	// = A.x <= B.x && A.y <= B.y
	bool operator <= (const Vector2&) const;
	// = A.x <= B && A.y <= B
	bool operator <= (const float&) const;

	// = A.x >= B.x && A.y >= B.y
	bool operator >= (const Vector2&) const;
	// = A.x >= B && A.y >= B
	bool operator >= (const float&) const;

	// = A.x < B.x && A.y < B.y
	bool operator < (const Vector2&) const;
	// = A.x < B && A.y < B
	bool operator < (const float&) const;

	// = A.x > B.x && A.y > B.y
	bool operator > (const Vector2&) const;
	// = A.x > B && A.y > B
	bool operator > (const float& ) const;

	// = A.x != B.x || A.y != B.y
	bool operator != (const Vector2&) const;
	// = A.x != B || A.y != B
	bool operator != (const float&) const;

	// = A.x = B 
	// = A.y = B
	void operator = (const float&);

	//operator sf::Vector2f();

	// Vector2(1, 1)
	static const Vector2 ONE;
	// Vector2(0, 0)
	static const Vector2 ZERO;

	// Vector2(1, 0)
	static const Vector2 RIGHT;
	// Vector2(-1, 0)
	static const Vector2 LEFT;

	// Vector2(0, 1)
	static const Vector2 UP;
	// Vector2(0, -1)
	static const Vector2 DOWN;
};
// float(A) + Vector2(B)
// = (A + B.x, A + B.y)
Vector2 operator + (float lhs, const Vector2& rhs);
// float(A) - Vector2(B)
// = (A - B.x, A - B.y)
Vector2 operator - (float lhs, const Vector2& rhs);
// float(A) * Vector2(B)
// = (A * B.x, A * B.y)
Vector2 operator * (float lhs, const Vector2& rhs);

class Vector3 {
public:
	float x, y, z;
	Vector3(float _x = 0, float _y = 0, float _z = 0) : x(_x), y(_y), z(_z) {};

	// Return the length of the Vector
	double Magnitude();
	// Return the length squared of the Vector
	float SQ_Magnitude();

	// Set the Length of the Vector to 1
	// without changing it's direction
	void Normalize();
	// Return the direction of the Vector as a unit Vector
	Vector3 Normalized();

	// Return if this Vector is colinear to an other Vector
	bool IsColinearTo(const Vector3&);
	// Return if this Vector is colinear to an other Vector with a set precision
	bool IsColinearTo(const Vector3&, const float);
	// Return if this Vector is coplanar to two other Vectors
	bool IsCoplanar(const Vector3&, const Vector3&);
public: 
	// = (A.x + B.x, A.y + B.y, A.z + B.z)
	Vector3 operator + (const Vector3&) const;
	// = (A.x - B.x, A.y - B.y, A.z - B.z)
	Vector3 operator - (const Vector3&) const;
	// = (A.x * B.x, A.y * B.y, A.z * B.z)
	Vector3 operator * (const Vector3&) const;
	// = (A.x / B.x, A.y / B.y, A.z / B.z)
	Vector3 operator / (const Vector3&) const;

	// = (A.x + B, A.y + B, A.z + B)
	Vector3 operator + (const float&) const;
	// = (A.x - B, A.y - B, A.z - B)
	Vector3 operator - (const float&) const;
	// = (A.x * B, A.y * B, A.z * B)
	Vector3 operator * (const float&) const;
	// = (A.x / B, A.y / B, A.z / B)
	Vector3 operator / (const float&) const;

	// = (-A.x, -A.y)
	Vector3 operator - () const;
 
	// = (A.x + B.x, A.y + B.y, A.z + B.z)
	Vector3& operator += (const Vector3&);
	// = (A.x - B.x, A.y - B.y, A.z - B.z)
	Vector3& operator -= (const Vector3&);
	// = (A.x * B.x, A.y * B.y, A.z * B.z)
	Vector3& operator *= (const Vector3&);
	// = (A.x / B.x, A.y / B.y, A.z / B.z)
	Vector3& operator /= (const Vector3&);

	// = (A.x + B, A.y + B, A.z + B)
	Vector3& operator += (const float&);
	// = (A.x - B, A.y - B, A.z - B)
	Vector3& operator -= (const float&);
	// Vector3(A) *= float(B)
	// = (A.x * B, A.y * B, A.z * B)
	Vector3& operator *= (const float&);
	// = (A.x / B, A.y / B, A.z / B)
	Vector3& operator /= (const float&);

	// = A.x == B.x && A.y == B.y && A.z == B.z
	bool operator == (const Vector3&) const;
	// = A.x == B && A.y == B && A.z == B
	bool operator == (const float&) const;

	// = A.x <= B.x && A.y <= B.y && A.z <= B.z
	bool operator <= (const Vector3&) const;
	// = A.x <= B && A.y <= B && A.z <= B
	bool operator <= (const float&) const;

	// = A.x >= B.x && A.y >= B.y && A.z >= B.z
	bool operator >= (const Vector3&) const;
	// = A.x >= B && A.y >= B && A.z >= B
	bool operator >= (const float&) const;

	// = A.x < B.x && A.y < B.y && A.z < B.z
	bool operator < (const Vector3&) const;
	// = A.x < B && A.y < B && A.z < B
	bool operator < (const float&) const;

	// = A.x > B.x && A.y > B.y && A.z > B.z
	bool operator > (const Vector3&) const;
	// = A.x > B && A.y > B && A.z > B
	bool operator > (const float&) const;

	// = A.x != B.x || A.y != B.y || A.z != B.z
	bool operator != (const Vector3&) const;
	// = A.x != B || A.y != B || A.z != B
	bool operator != (const float&) const;

	// = A.x = B 
	// = A.y = B
	// = A.z = B
	void operator = (const float&);

	// Vector3(1, 1, 1)
	static const Vector3 ONE;
	// Vector3(0, 0, 0)
	static const Vector3 ZERO;

	// Vector3(1, 0, 0)
	static const Vector3 RIGHT;
	// Vector3(-1, 0, 0)
	static const Vector3 LEFT;

	// Vector3(0, 1, 0)
	static const Vector3 UP;
	// Vector3(0, -1, 0)
	static const Vector3 DOWN;

	// Vector3(0, 0, 1)
	static const Vector3 FORWARD;
	// Vector3(0, 0, -1)
	static const Vector3 BACKWARD;
};

// = (A + B.x, A + B.y, A + B.z)
Vector3 operator + (float lhs, const Vector3& rhs);
// = (A - B.x, A - B.y, A - B.z)
Vector3 operator - (float lhs, const Vector3& rhs);
// = (A * B.x, A * B.y, A * B.Z)
Vector3 operator * (float lhs, const Vector3& rhs);

// Return the distance between to points
double Distance(const Vector2&, const Vector2&);
// Return the distance between to points
double Distance(const Vector3&, const Vector3&);

// Return the scalar product of two Vectors
float Dot(const Vector2&, const Vector2&);
// Return the scalar product of two Vectors
float Dot(const Vector3&, const Vector3&);

// Return the cross product of two Vectors
Vector3 CrossProduct(const Vector3&, const Vector3&);

// Return the linear interpolation of two Vectors
Vector2 LinearInterpolation(const Vector2&, const Vector2&, const float);
// Return the linear interpolation of two Vectors
Vector3 LinearInterpolation(const Vector3&, const Vector3&, const float);

// Get if a Vector has a length of 1
bool IsNormalize(const Vector2&);
// Get if a Vector has a length of 1
bool IsNormalize(const Vector3&);

// Get if a Vector has a length of 1
bool IsNormalTo(const Vector2&, const Vector2&);
// Get if a Vector has a length of 1
bool IsNormalTo(const Vector3&, const Vector3&);




#endif
