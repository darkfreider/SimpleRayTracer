
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


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

Vector3 ray_trace(const std::vector<Object *>& objects, const Vector3& ray_origin, const Vector3& ray_dir)
{
	Vector3 result = Vector3(0.2, 0, 0.8);

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
		result = obj->get_material()->get_color();
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
		std::cerr << "usage: <main> gscript.txt" << std::endl;
	}

	Geometry_builder builder;
	builder.generate_geometry(get_file_contents(argv[1]));

	Image32 image(1280, 720);

	// TODO(max): make a camera abstraction
	Vector3 camera_pos(0, -15, 6);
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

			uint32_t color = unpack_vector3_to_argb(ray_trace(builder.get_objects(), ray_origin, ray_dir));

			out[y * image.get_width() + x] = color;

		}
	}
	
	image.write_image("test.bmp");

	std::cout << "DONE rendering" << std::endl;

	return (0);
}





















