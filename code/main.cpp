
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
/*
#include "math.cpp"
#include "material.cpp"
#include "object.cpp"
#include "tokeniser.cpp"
#include "geometry_builder.cpp"
*/
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


Vector3 ray_trace(const std::vector<Object *>& objects, const std::vector<Light *>& lights, const Vector3& ray_origin, const Vector3& ray_dir, int depth)
{
	Vector3 result(44.f/ 255.f, 53.f / 255.f, 235.f / 255.f);
	//Vector3 result(0., 0., 0.);
	if (depth > 10) return (result);

	Vector3 next_dir(ray_dir);
	Vector3 next_origin(ray_origin);

	Object *obj = 0;
	float t_min = FLT_MAX;
	for (int i = 0; i < objects.size(); i++)
	{
		float t = 0.0f;
		if (objects[i]->intersect(next_origin, next_dir, t))
		{
			if (t < t_min)
			{
				t_min = t;
				obj = objects[i];
			}
		}
	}

	if (!obj)
	{
		return (result);
	}

	if (obj)
	{
		Vector3 hit_point = next_origin + t_min * next_dir;
		Vector3 hit_normal;
		obj->get_surface_properties(hit_point, hit_normal);
		

		Vector3 recurcive_color(1.0, 1.0, 1.0);
		float reflectance = obj->get_material()->get_reflectance();
		Vector3 ideal_reflection = next_dir.reflect(hit_normal);
		Vector3 random_reflection = hit_normal + random_in_unit_sphere();
		Vector3 reflection_ray = random_reflection.lerp(reflectance, ideal_reflection).normalize();

		next_dir = reflection_ray;
		next_origin = hit_point + 0.0001 * next_dir;
		recurcive_color = ray_trace(objects, lights, next_origin, next_dir, depth + 1);

		Vector3 obj_color = obj->get_material()->get_color();
		Vector3 illumination_color;
		for (int i = 0; i < lights.size(); i++)
		{
			Vector3 light_dir;
			Vector3 light_color;
			lights[i]->get_shading_info(hit_point, hit_normal, light_dir, light_color);

			float in_shade = does_intersect(objects, hit_point, light_dir);
			illumination_color += in_shade * light_color * std::max(0.0f, hit_normal.dot(light_dir));
		}

		result = obj_color.hadamard(recurcive_color + illumination_color);

		result.r = clamp_float(result.r, 0.0f, 1.0f);
		result.g = clamp_float(result.g, 0.0f, 1.0f);
		result.b = clamp_float(result.b, 0.0f, 1.0f);
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
		return (1);
	}

	Geometry_builder builder;
	builder.generate_geometry(get_file_contents(argv[1]));
	

	for (size_t camera_index = 0; camera_index < builder.get_camera_count(); camera_index++)
	{
		Camera camera(builder.get_camera(camera_index));
		int rays_per_pixel = camera.get_rays_per_pixel();
		
		Image32 image(camera.get_width(), camera.get_height());

		uint32_t *out = image.get_pixels();
		for (int y = 0; y < image.get_height(); y++)
		{
			float film_y = camera.get_film_y(y);
			for (int x = 0; x < image.get_width(); x++)
			{
				float film_x = camera.get_film_x(x);

				Vector3 ray_origin = camera.get_origin();
				Vector3 color;
				for (int ray_index = 0; ray_index < rays_per_pixel; ray_index++)
				{
					Vector3 antialiased_dir = camera.get_randomized_dir(film_x, film_y);
					color += ray_trace(builder.get_objects(), builder.get_lights(), ray_origin, antialiased_dir, 0);
				}
				color /= rays_per_pixel;

				uint32_t out_color = unpack_vector3_to_argb(Vector3(
					linear_to_srgb(color.r),
					linear_to_srgb(color.g),
					linear_to_srgb(color.b)
				));

				out[y * image.get_width() + x] = out_color;

			}
		}
	
		int digit0 = camera_index / 100;
		int digit1 = (camera_index / 10) % 10;
		int digit2 = camera_index % 10;
		image.write_image("img" + std::to_string(digit0) + std::to_string(digit1) + std::to_string(digit2) + ".bmp");
	}

	std::cout << "DONE rendering" << std::endl;

	return (0);
}