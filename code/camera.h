


#if !defined(CAMERA_H)

#include "math.h"

class Camera
{
	Vector3 look_from;
	Vector3 look_at;
	int width;
	int height;

	Vector3 camera_x;
	Vector3 camera_y;
	Vector3 camera_z;


	Vector3 film_center;
	float film_distance = 1.0f;
	float film_width = 1.0f;
	float film_height = 1.0f;
	float half_film_width = 0.5f;
	float half_film_height = 0.5f;

public:
	Camera() : width(0), height(0) {}

	void set_resolution(int w, int h)
	{
		width = w;
		height = h;
	}

	void set_look_at(const Vector3& lat)
	{
		look_at = lat;
	}

	void set_look_from(const Vector3& lfrom)
	{
		look_from = lfrom;
	}

	int get_width()
	{
		return (width);
	}

	int get_height()
	{
		return (height);
	}

	float get_film_x(int pixel_x)
	{
		float film_x = -1.0f + 2.0f * (float(pixel_x) / float(width));
		return (film_x);
	}

	float get_film_y(int pixel_y)
	{
		float film_y = -1.0f + 2.0f * (float(pixel_y) / float(height));
		return (film_y);
	}

	void get_origin_and_dir(float film_x, float film_y, Vector3& ray_origin, Vector3& ray_dir)
	{
		Vector3 film_pos = film_center + camera_x * film_x * half_film_width + camera_y * film_y * half_film_height;
		ray_origin = look_from;
		ray_dir = (film_pos - ray_origin).normalize();
	}

	void init()
	{
		camera_z = (look_from - look_at).normalize();
		camera_x = Vector3(0, 0, 1).cross(camera_z).normalize();
		camera_y = camera_z.cross(camera_x).normalize();

		if (width > height)
		{
			film_width *= float(width) / float(height);
		}
		if (height > width)
		{
			film_height *= float(height) / float(width);
		}

		half_film_width = 0.5f * film_width;
		half_film_height = 0.5f * film_height;

		film_center = look_from - film_distance * camera_z;
	}
};

#define CAMERA_H
#endif