
#include "object.h"

bool Sphere::intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const
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

bool Triangle::intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const
{
	bool result = false;
	const float tolerance = 0.0001f;

	Vector3 a = m_vertecies[1] - m_vertecies[0];
	Vector3 b = m_vertecies[2] - m_vertecies[0];
	Vector3 n = a.cross(b).normalize();

	Vector3 w = m_vertecies[0] - ray_orig;

	float t = w.dot(n) / ray_dir.dot(n);
	if (t <= tolerance)
	{
		return (false);
	}

	Vector3 intersection_p = ray_orig + t * ray_dir;

	float k0 = (m_vertecies[1] - m_vertecies[0]).dot(intersection_p - m_vertecies[0]);
	float k1 = (m_vertecies[2] - m_vertecies[1]).dot(intersection_p - m_vertecies[1]);
	float k2 = (m_vertecies[0] - m_vertecies[2]).dot(intersection_p - m_vertecies[2]);

	if (k0 > 0.0f && k1 > 0.0f && k2 > 0.0f)
	{
		result = true;
	}

	return (result);
}

bool Quad::intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const
{
	return (false);
}