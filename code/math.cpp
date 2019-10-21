
#if !defined(MATH_H)

#include <cmath>

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
		float e[3];
	};

	Vector3() : x(0), y(0), z(0) {}
	Vector3(float val) : x(val), y(val), z(val) {}
	Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	// TODO(max): add copy constructor

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

	float length() const;
	float length2() const;
	Vector3& normalize();
	float dot(const Vector3& right) const;
	const Vector3 hadamard(const Vector3& right) const;
	const Vector3 cross(const Vector3& right) const;

	friend const Vector3 operator* (float left, const Vector3& right);
	friend const Vector3 normalize(const Vector3& v);
	friend std::ostream& operator<< (std::ostream& out, const Vector3& v);
};

std::ostream& operator<< (std::ostream& out, const Vector3& v)
{
	out << v.x << " ";
	out << v.y << " ";
	out << v.z << " ";
	out << std::endl;

	return (out);
}

const Vector3 normalize(const Vector3& v)
{
	Vector3 result;

	float len = v.length();
	if (len != 0.0f)
	{
		float inv_len = 1.0f / len;
		result.x = v.x * inv_len;
		result.y = v.y * inv_len;
		result.z = v.z * inv_len;
	}

	return (result);
}

const Vector3 operator* (float left, const Vector3& right)
{
	return Vector3(left * right.x, left * right.y, left * right.z);
}

float Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

float Vector3::length2() const
{
	return (x * x + y * y + z * z);
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0)
	{
		float inv_len = 1.0f / len;
		x *= inv_len;
		y *= inv_len;
		z *= inv_len;
	}

	return (*this);
}

float Vector3::dot(const Vector3& right) const
{
	return (x * right.x + y * right.y + z * right.z);
}

const Vector3 Vector3::cross(const Vector3& right) const
{
	return Vector3(
		y * right.z - z * right.y,
		z * right.x - x * right.z,
		x * right.y - y * right.x);
}

const Vector3 Vector3::hadamard(const Vector3& right) const
{
	return Vector3(x * right.x, y * right.y, z * right.z);
}


const Vector3 Vector3::operator+() const
{
	return Vector3(+x, +y, +z);
}

const Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

const Vector3 Vector3::operator+ (const Vector3& right) const
{
	return Vector3(x + right.x, y + right.y, z + right.z);
}

const Vector3 Vector3::operator- (const Vector3& right) const
{
	return Vector3(x - right.x, y - right.y, z - right.z);
}

const Vector3 Vector3::operator* (const float right) const
{
	return Vector3(x * right, y * right, z * right);
}

const Vector3 Vector3::operator/ (const float right) const
{
	return Vector3(x / right, y / right, z / right);
}

Vector3& Vector3::operator+= (const Vector3& right)
{
	x += right.x;
	y += right.y;
	z += right.z;

	return (*this);
}

Vector3& Vector3::operator-= (const Vector3& right)
{
	x -= right.x;
	y -= right.y;
	z -= right.z;

	return (*this);
}

Vector3& Vector3::operator*= (float right)
{
	x *= right;
	y *= right;
	z *= right;

	return (*this);
}

Vector3& Vector3::operator/= (float right)
{
	float inv = 1.0f / right;
	x *= inv;
	y *= inv;
	z *= inv;

	return (*this);
}

#define MATH_H
#endif // MATH_H