
#if !defined(_MATERIAL_H)

#include "math.h"

class Material
{
	Vector3 m_color;

public:
	Material(const Vector3& c = Vector3(0, 0, 0)) : m_color(c)
	{
		
	}

	~Material()
	{
		
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