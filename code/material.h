
#if !defined(_MATERIAL_H)

#include "math.h"

class Material
{
	Vector3 color;

public:
	Material(const Vector3& c = Vector3(0, 0, 0)) : color(c)
	{
		
	}

	~Material()
	{
		
	}

	const Vector3 get_color() const
	{
		return (color);
	}
};

#define _MATERIAL_H
#endif