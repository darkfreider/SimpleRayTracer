#include <cmath>
#include <iostream>
#include "math.h"

std::ostream& operator<< (std::ostream& out, const Vector3& v)
{
	out << v.x << " ";
	out << v.y << " ";
	out << v.z << " ";
	out << std::endl;

	return (out);
}

const Vector3 Vector3::reflect(const Vector3& n) const
{
	Vector3 result = *this - (2.0f * this->dot(n) * n);
	return (result.normalize());
}

const Vector3 Vector3::lerp(float k, const Vector3& end)
{
	return (1.0 - k) * (*this) + k * end;
}

float Vector3::operator[] (int i) const
{
	return e[i];
}

float& Vector3::operator[] (int i)
{
	return e[i];
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

const Vector3 Vector3::normalize() const
{
	Vector3 result;

	float len = length();
	if (len != 0.0f)
	{
		float inv_len = 1.0f / len;
		result.x = x * inv_len;
		result.y = y * inv_len;
		result.z = z * inv_len;
	}

	return (result);
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


// misc


float clamp_float(float value, float low, float high)
{
	float result = value;
	if (value < low)
		result = low;
	else if (value > high)
		result = high;

	return (result);
}

static uint32_t xorshift_state = 713795911;
uint32_t xorshift32()
{
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = xorshift_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return xorshift_state = x;
}

float random_unilateral()
{
	return (float)xorshift32() / (float)UINT32_MAX;
}

float random_bilateral()
{
	return 2.0f * ((float)xorshift32() / (float)UINT32_MAX) - 1.0f;
}

Vector3 random_in_unit_sphere()
{
	Vector3 result;
	do
	{
		result = 2.0f * Vector3(random_unilateral(), random_unilateral(), random_unilateral()) - Vector3(1., 1., 1.);
	} while (result.length2() >= 1.0f);
	return (result);
}