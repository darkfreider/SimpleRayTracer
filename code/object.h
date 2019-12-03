
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

	Sphere(const Vector3& c, float r) : Object(), m_center(c), m_radius(r)
	{
		
	}

	~Sphere()
	{
		
	}

	bool intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const;
};

class Triangle : public Object
{
	Vector3 m_vertecies[3];

public:

	Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2) : Object()
	{
		m_vertecies[0] = v0;
		m_vertecies[1] = v1;
		m_vertecies[2] = v2;
	}

	~Triangle() {}

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