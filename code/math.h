#if !defined(MATH_H)

// TODO(max): implement operator[]

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
	const Vector3 normalize() const;
	float dot(const Vector3& right) const;
	const Vector3 hadamard(const Vector3& right) const;
	const Vector3 cross(const Vector3& right) const;

	friend const Vector3 operator* (float left, const Vector3& right);
	friend const Vector3 normalize(const Vector3& v);
	friend std::ostream& operator<< (std::ostream& out, const Vector3& v);
};

#define MATH_H
#endif