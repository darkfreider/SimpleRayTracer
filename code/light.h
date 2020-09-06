
#if !defined(_LIGHT_H)

#include "math.h"

class Light
{
protected:
	Vector3 color;

public:
	Light(const Vector3& c = Vector3(1., 1., 1.)) : color(c) {}

	virtual void get_shading_info(const Vector3& hit_position, const Vector3& hit_normal, Vector3& light_dir, Vector3& intencity) const = 0;

	Vector3& get_color()
	{
		return (color);
	}

	const Vector3& get_color() const
	{
		return (color);
	}

	void set_color(const Vector3& c)
	{
		color = c;
	}
};

class Distant_light : public Light
{
	Vector3 dir;
public:
	Distant_light(const Vector3& d = Vector3()) : dir(d.normalize()) {}

	void set_direction(const Vector3& d)
	{
		dir = d.normalize();
	}

	void get_shading_info(const Vector3& hit_position, const Vector3& hit_normal, Vector3& light_dir, Vector3& intencity) const
	{
		light_dir = -dir;
		intencity = 0.1f * color;
	}
};


class Spherical_light : public Light
{
	float intencity;
	Vector3 pos;
public:
	Spherical_light(const Vector3& p = Vector3(), float pp = 3.0f) : pos(p), intencity(pp) {}

	void set_intencity(float i)
	{
		intencity = i;
	}

	void set_position(const Vector3& p)
	{
		pos = p;
	}

	void get_shading_info(const Vector3& hit_position, const Vector3& hit_normal, Vector3& light_dir, Vector3& c) const
	{
		Vector3 obj_to_light = pos - hit_position;
		float r2 = obj_to_light.length2();
		c = (intencity * color) / r2;
		light_dir = obj_to_light.normalize();
	}
};

#define _LIGHT_H
#endif