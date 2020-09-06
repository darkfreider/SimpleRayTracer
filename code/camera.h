


#if !defined(CAMERA_H)

#include "math.h"

class Camera
{
	Vector3 look_from;
	Vector3 look_at;
	int width = 0;
	int height = 0;
	int rays_per_pixel = 1;

	Vector3 camera_x;
	Vector3 camera_y;
	Vector3 camera_z;

	float pixel_width = 0.0f;
	float pixel_height = 0.0f;
	float half_pixel_width = 0.0f;
	float half_pixel_height = 0.0f;

	Vector3 film_center;
	float film_distance = 1.0f;
	float film_width = 1.0f;
	float film_height = 1.0f;
	float half_film_width = 0.5f;
	float half_film_height = 0.5f;

public:
	Camera() {}

	void set_rays_per_pixel(int rpp)
	{
		rays_per_pixel = rpp;
	}

	int get_rays_per_pixel() const
	{
		return (rays_per_pixel);
	}

	Vector3 get_randomized_dir(float film_x, float film_y)
	{
		Vector3 film_pos = film_center + camera_x * film_x * half_film_width + camera_y * film_y * half_film_height;
		film_pos += camera_x * half_pixel_width * random_bilateral() + camera_y * half_pixel_height * random_bilateral();
		
		return (film_pos - look_from).normalize();
	}

	Vector3 get_origin()
	{
		return (look_from);
	}

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

		pixel_width = 1.0f / width;
		pixel_height = 1.0f / height;
		half_pixel_width = 0.5f * pixel_width;
		half_pixel_height = 0.5f * pixel_height;
	}
};

#define CAMERA_H
#endif