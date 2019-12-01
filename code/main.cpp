
#include <iostream>
#include <string>
#include <stdint.h>
#include <cmath>
#include <vector>

#include <memory>

#include "math.h"

#include "math.cpp"

class Material
{
	Vector3 color;

public:
	Material(const Vector3& c = Vector3(0, 0, 0)) : color(c) 
	{
		//std::cout << "Material created" << std::endl;
	}

	~Material() 
	{
		//std::cout << "Material destoyed" << std::endl;
	}

	const Vector3& get_color() const
	{
		return (color);
	}
};

class Object
{
	Material *material;

public:

	Object() : material(0) { }

	Object(Material *m) : material(m) 
	{
		//std::cout << "Object created" << std::endl;
	}

	~Object() {
		//std::cout << "Object destoyed" << std::endl;
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


class Image32
{
	// packing srtucture is IMPORTANT
#pragma pack(push, 1)
	struct Bitmap_header
	{
		uint16_t file_type;
		uint32_t file_size;
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t offset;

		uint32_t dib_header_size;
		int32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bits_per_pixel;
		uint32_t compression;
		uint32_t raw_size;
		int32_t hor_resolution;
		int32_t vert_resolution;
		uint32_t colors;
		uint32_t important_colors;
	};
#pragma pack(pop)

	uint32_t m_width;
	uint32_t m_height;
	uint32_t *m_pixels;
	
public:	
	
	Image32(uint32_t w, uint32_t h) : m_width(w), m_height(h)
	{
		m_pixels = new uint32_t[m_width * m_height];
	}
	
	~Image32()
	{
		delete[] m_pixels;
	}

	uint32_t get_width() { return (m_width); }
	uint32_t get_height() { return (m_height); }
	uint32_t *get_pixels() { return (m_pixels); }
	
	void write_image(const std::string& file_name) const;
};

void Image32::write_image(const std::string& file_name) const
{
	uint32_t output_image_size = sizeof(uint32_t) * m_width* m_height;

	Bitmap_header header = {};
	header.file_type = 0x4d42;
	header.file_size = sizeof(Bitmap_header) + output_image_size;
	header.offset = sizeof(Bitmap_header);
	header.dib_header_size = sizeof(Bitmap_header) - 14;
	header.width = (int32_t)m_width;
	header.height = (int32_t)m_height; // NOTE(max): bottom-up bitmap
	header.planes = 1;
	header.bits_per_pixel = 32;

	FILE *bmp_file = fopen(file_name.c_str(), "wb");
	if (bmp_file != NULL)
	{
		fwrite(&header, sizeof(Bitmap_header), 1, bmp_file);
		fwrite(m_pixels, output_image_size, 1, bmp_file);
		fclose(bmp_file);
	}
	else
	{
		printf("ERROR :: file \"%s\" can't open...", file_name.c_str());
	}
}

Vector3 ray_trace(const Object& obj, const Vector3& ray_origin, const Vector3& ray_dir)
{
	Vector3 result = Vector3(0.2, 0, 0.8);

	float t = 0.0f;
	if (obj.intersect(ray_origin, ray_dir, t))
	{
		Material *mat = obj.get_material();

		result = mat->get_color();
	}

	return (result);
}

uint32_t unpack_vector3_to_argb(const Vector3& color)
{
	uint32_t result = 0;

	result = (0xFF << 24) |
			 uint8_t(255.0f * color.r) << 16 |
			 uint8_t(255.0f * color.g) << 8  |
			 uint8_t(255.0f * color.b) << 0;

	return (result);
}

int main(void)
{
	Image32 image(1280, 720);


	Vector3 camera_pos(0, -10, 0);

	Vector3 camera_z = camera_pos.normalize(); // look at (0, 0, 0)
	Vector3 camera_x = Vector3(0, 0, 1).cross(camera_z).normalize();
	Vector3 camera_y = camera_z.cross(camera_x).normalize();

	float film_distance = 1.0f;
	float film_width = 1.0f;
	float film_height = 1.0f;

	if (image.get_width() > image.get_height())
	{
		film_width *= float(image.get_width()) / float(image.get_height());
	}
	if (image.get_height() > image.get_width())
	{
		film_height *= float(image.get_height()) / float(image.get_width());
	}

	float half_film_width  = 0.5f * film_width;
	float half_film_height = 0.5f * film_height;

	Vector3 film_center = camera_pos - film_distance * camera_z;

	std::vector<Material *> materials;
	materials.push_back(new Material(Vector3(1, 0, 0)));

	std::vector<Object *> objects;
	objects.push_back(new Sphere(Vector3(0, 0, 0), 2));

	objects.at(0)->set_material(materials.at(0));

	uint32_t *out = image.get_pixels();
	for (int y = 0; y < image.get_height(); y++)
	{
		float film_y = -1.0f + 2.0f * (float(y) / image.get_height());

		for (int x = 0; x < image.get_width(); x++)
		{
			float film_x = -1.0f + 2.0f * (float(x) / image.get_width());

			Vector3 film_pos = film_center + camera_x * film_x * half_film_width + camera_y * film_y * half_film_height;
			
			Vector3 ray_origin(camera_pos);
			Vector3 ray_dir = (film_pos - ray_origin).normalize();

			uint32_t color = unpack_vector3_to_argb(ray_trace(*objects.at(0), ray_origin, ray_dir));

			out[y * image.get_width() + x] = color;

		}
	}
	
	image.write_image("test.bmp");

	return (0);
}





















