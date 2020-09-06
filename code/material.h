
#if !defined(_MATERIAL_H)

#include "math.h"

class Material
{
	Vector3 m_color;
	float m_reflectance;

public:
	Material(const Vector3& c = Vector3(0, 0, 0), float r = 0.0f) : m_color(c), m_reflectance(r)
	{
		
	}

	~Material()
	{
		
	}

	void set_reflectance(float r)
	{
		m_reflectance = r;
	}

	float get_reflectance() const
	{
		return (m_reflectance);
	}

	const Vector3 get_color() const
	{
		return (m_color);
	}

	void set_color(const Vector3& color)
	{
		m_color = color;
	}
};

#define _MATERIAL_H
#endif