
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


#include <cstdlib>
#include <cstdint>
#include <cfloat>
#include <cmath>
#include <cassert>
#include <cctype>

#include "math.h"
#include "geometry_builder.h"
#include "material.h"
#include "object.h"
#include "camera.h"
#include "light.h"

#include "math.cpp"
#include "material.cpp"
#include "object.cpp"
#include "tokeniser.cpp"
#include "geometry_builder.cpp"

class Image32
{
	// NOTE(max): packing srtucture is IMPORTANT
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

float does_intersect(const std::vector<Object *>& objects, const Vector3& ray_origin, const Vector3& ray_dir)
{
	float in_shade = 1.0f;
	float t_min = FLT_MAX;
	for (int i = 0; i < objects.size(); i++)
	{
		float t = 0.0f;
		if (objects[i]->intersect(ray_origin + 0.0001 * ray_dir, ray_dir, t))
		{
			in_shade = 0.15f;
			break;
		}
	}

	return (in_shade);
}

float clamp_float(float value, float low, float high)
{
	float result = value;
	if (value < low)
		result = low;
	else if (value > high)
		result = high;

	return (result);
}

Vector3 ray_trace(const std::vector<Object *>& objects, const std::vector<Light *>& lights, const Vector3& ray_origin, const Vector3& ray_dir)
{
	Vector3 result;
	Object *obj = 0;
	float t_min = FLT_MAX;
	for (int i = 0; i < objects.size(); i++)
	{
		float t = 0.0f;
		if (objects[i]->intersect(ray_origin, ray_dir, t))
		{
			if (t < t_min)
			{
				t_min = t;
				obj = objects[i];
			}
		}
	}

	if (obj)
	{
		Vector3 hit_point = ray_origin + t_min * ray_dir;
		Vector3 hit_normal;
		obj->get_surface_properties(hit_point, hit_normal);

		for (int i = 0; i < lights.size(); i++)
		{
			Vector3 light_dir;
			Vector3 light_intencity;
			lights[i]->get_shading_info(hit_point, hit_normal, light_dir, light_intencity);

			float in_shade = does_intersect(objects, hit_point, light_dir);

			Vector3 obj_color = obj->get_material()->get_color();
			Vector3 color = in_shade * obj_color.hadamard(light_intencity) * std::max(0.0f, hit_normal.dot(light_dir));
			//color.r = clamp_float(color.r, 0.0f, 1.0f);
			//color.g = clamp_float(color.g, 0.0f, 1.0f);
			//color.b = clamp_float(color.b, 0.0f, 1.0f);
			result += color;
		}
		result /= float(lights.size());

		result.r = clamp_float(result.r, 0.0f, 1.0f);
		result.g = clamp_float(result.g, 0.0f, 1.0f);
		result.b = clamp_float(result.b, 0.0f, 1.0f);
	}
	else
	{
		result = Vector3(0., 0., 1.);
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

float linear_to_srgb(float linear)
{
	float srgb;

	srgb = 1.055f * pow(linear, 1.0f / 2.4f) - 0.055f;
	if (linear < 0.00313066f)
	{
		srgb = linear * 12.92f;
	}

	return (srgb);
}

std::string get_file_contents(const char *file_name)
{
	std::ifstream in(file_name, std::ios::in | std::ios::binary);
	std::string contents;
	if (in)
	{
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
	}
	else
	{
		std::cerr << "ERROR: can't open file" << std::endl;
		std::exit(1);
	}

	return (contents);
}



int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "usage: <exe_name> <gscript.txt>" << std::endl;
	}

	Geometry_builder builder;
	builder.generate_geometry(get_file_contents(argv[1]));

	Camera camera(builder.get_camera());
	Image32 image(camera.get_width(), camera.get_height());

	std::vector<Light *> lights;
	//lights.push_back(new Distant_light(Vector3(0., 1., -0.5)));
	//lights.push_back(new Spherical_light(Vector3(0., 2., 3.), 5.0f));
	//lights.push_back(new Spherical_light(Vector3(0., -2., 3.), 10.0f));
	lights.push_back(new Spherical_light(Vector3(0., 0., 15.), 50.0f));
	//lights.back()->
	//lights.push_back(new Spherical_light(Vector3(5., 2., 5.), 10.0f));


	uint32_t *out = image.get_pixels();
	for (int y = 0; y < image.get_height(); y++)
	{
		float film_y = camera.get_film_y(y);
		for (int x = 0; x < image.get_width(); x++)
		{
			float film_x = camera.get_film_x(x);
			
			Vector3 ray_origin;
			Vector3 ray_dir;
			camera.get_origin_and_dir(film_x, film_y, ray_origin, ray_dir);

			Vector3 color = ray_trace(builder.get_objects(), builder.get_lights(), ray_origin, ray_dir);

#if 1
			uint32_t out_color = unpack_vector3_to_argb(Vector3(
				linear_to_srgb(color.r),
				linear_to_srgb(color.g),
				linear_to_srgb(color.b)
			));
#endif
			//uint32_t out_color = unpack_vector3_to_argb(color);
			out[y * image.get_width() + x] = out_color;

		}
	}
	
	image.write_image("test.bmp");

	std::cout << "DONE rendering" << std::endl;

	return (0);
}