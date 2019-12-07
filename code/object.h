
#if !defined(_OBJECT_H)

#include "math.h"
#include "material.h"

class Object
{
	Material *material;

public:

	Object() : material(0) { }

	Object(Material *m) : material(m)
	{
		
	}

	~Object() {
		
	}

	Material *get_material() const
	{
		return (material);
	}

	void set_material(Material *mat)
	{
		material = mat;
	}

	virtual bool intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const = 0;

};

// PERFORMANCE(max): precompute r*r and a*a
class Sphere : public Object
{
	float m_radius;
	Vector3 m_center;

public:

	Sphere(const Vector3& c = Vector3(), float r = 0.0f) : Object(), m_center(c), m_radius(r)
	{
		
	}

	~Sphere()
	{
		
	}

	void set_radius(float f)
	{
		m_radius = f;
	}

	void set_position(const Vector3& p)
	{
		m_center = p;
	}

	bool intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const;
};

class Triangle : public Object
{
	Vector3 m_vertecies[3];

public:

	Triangle(const Vector3& v0 = Vector3(), const Vector3& v1 = Vector3(), const Vector3& v2 = Vector3()) : Object()
	{
		m_vertecies[0] = v0;
		m_vertecies[1] = v1;
		m_vertecies[2] = v2;
	}

	~Triangle() {}

	Vector3& operator[] (int i)
	{
		return (m_vertecies[i]);
	}

	bool intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const;
};

class Quad : public Object
{
	Vector3 m_vertecies[4];

public:
	// TODO(max): how to initialize quad?
	//            1. give a base point and two vectors
	//            2. give a base point and 3 points relative to the base point
	Quad() : Object()
	{

	}
	~Quad() {}

	bool intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const;
};

#define _OBJECT_H
#endif