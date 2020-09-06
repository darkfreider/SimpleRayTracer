#if !defined(MATH_H)

#include <iostream>

struct Vector3
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		
		struct
		{
			float r;
			float g;
			float b;
		};

		float e[3];
	};

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float val) : x(val), y(val), z(val) {}
	Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	Vector3(const Vector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
	}
	Vector3& operator=(const Vector3& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;

		return (*this);
	}

	const Vector3 operator+() const;
	const Vector3 operator-() const;

	const Vector3 operator+ (const Vector3& right) const;
	const Vector3 operator- (const Vector3& right) const;
	const Vector3 operator* (const float right) const;
	const Vector3 operator/ (const float right) const;

	Vector3& operator+= (const Vector3& right);
	Vector3& operator-= (const Vector3& right);
	Vector3& operator*= (float right);
	Vector3& operator/= (float right);

	float operator[] (int i) const;
	float& operator[] (int i);

	float length() const;
	float length2() const;
	const Vector3 normalize() const;
	float dot(const Vector3& right) const;
	const Vector3 hadamard(const Vector3& right) const;
	const Vector3 cross(const Vector3& right) const;
	const Vector3 reflect(const Vector3& n) const;
	const Vector3 lerp(float k, const Vector3& end);

	friend const Vector3 operator* (float left, const Vector3& right);
	//friend const Vector3 normalize(const Vector3& v);
	friend std::ostream& operator<< (std::ostream& out, const Vector3& v);
};

float clamp_float(float value, float low, float high);
uint32_t xorshift32();
float random_unilateral();
float random_bilateral();
Vector3 random_in_unit_sphere();

#define MATH_H
#endif