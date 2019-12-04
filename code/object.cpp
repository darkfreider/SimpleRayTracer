
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

	float d = ray_dir.dot(n);
	if (d != 0.0f)
	{
		float t = w.dot(n) / d;
		if (t > tolerance)
		{
			Vector3 intersection_p = ray_orig + t * ray_dir;

			Vector3 edge0 = m_vertecies[1] - m_vertecies[0];
			Vector3 edge1 = m_vertecies[2] - m_vertecies[1];
			Vector3 edge2 = m_vertecies[0] - m_vertecies[2];

			float k0 = edge0.cross(intersection_p - m_vertecies[0]).dot(n);
			float k1 = edge1.cross(intersection_p - m_vertecies[1]).dot(n);
			float k2 = edge2.cross(intersection_p - m_vertecies[2]).dot(n);

			if (k0 > 0.0f && k1 > 0.0f && k2 > 0.0f)
			{
				hit_t = t;
				result = true;
			}
		}
	}

	return (result);
}

bool Quad::intersect(const Vector3& ray_orig, const Vector3& ray_dir, float& hit_t) const
{
	return (false);
}