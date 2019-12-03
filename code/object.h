
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
		//std::cout << "Sphere created" << std::endl;
	}

	~Sphere()
	{
		//std::cout << "Sphere destoyed" << std::endl;
	}

	bool intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const
	{
		bool result = false;
		const float tolerance = 0.0001f;

		float t = ray_dir.dot(m_center - ray_orig);
		Vector3 p = ray_orig + t * ray_dir;
		float a = (m_center - p).length(); // PERFORMANCE(max) just compute length2()

		if (a < m_radius)
		{
			result = true;
			float b = sqrt(m_radius * m_radius - a * a);

			float tn = t - b;
			float tp = t + b;
			if (tn > tolerance && tn < tp)
			{
				hit_t = tn;
			}
			else if (tp > tolerance)
			{
				hit_t = tp;
			}
			else
			{
				result = false;
			}
		}

		return (result);
	}
};

class Quad : Object
{
	Vector3 vertecies[4];
public:
	Quad() : Object()
	{

	}
	~Quad() {}

	bool intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const
	{
		return (false);
	}
};

#define _OBJECT_H
#endif