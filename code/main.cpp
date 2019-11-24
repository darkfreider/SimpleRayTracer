
#include <iostream>
#include <string>
#include <stdint.h>

#include "math.h"

#include "math.cpp"



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
	
public:	
	uint32_t m_width;
	uint32_t m_height;
	uint32_t *m_pixels;
	
	Image32(uint32_t w, uint32_t h) : m_width(w), m_height(h)
	{
		m_pixels = new uint32_t[m_width * m_height];
	}
	
	~Image32()
	{
		delete[] m_pixels;
	}
	
	void write_image(const std::string& file_name) const;
};

void Image32::write_image(const std::string& file_name) const
{
	uint32_t output_image_size = sizeof(uint32_t) * m_width* m_height;

	Image32::Bitmap_header header = {};
	header.file_type = 0x4d42;
	header.file_size = sizeof(Image32::Bitmap_header) + output_image_size;
	header.offset = sizeof(Image32::Bitmap_header);
	header.dib_header_size = sizeof(Image32::Bitmap_header) - 14;
	header.width = (int32_t)m_width;
	header.height = (int32_t)m_height;
	header.planes = 1;
	header.bits_per_pixel = 32;

	FILE *bmp_file = fopen(file_name.c_str(), "wb");
	if (bmp_file != NULL)
	{
		fwrite(&header, sizeof(Image32::Bitmap_header), 1, bmp_file);
		fwrite(m_pixels, output_image_size, 1, bmp_file);
		fclose(bmp_file);
	}
	else
	{
		printf("ERROR :: file \"%s\" can't open...", file_name.c_str());
	}
}


int main(void)
{
	Image32 image(1280, 720);
	
	for (int y = 0; y < image.m_height; y++)
	{
		for (int x = 0; x < image.m_width; x++)
		{
			image.m_pixels[y * image.m_width + x] = 0xFF0000FF; // green
		}
	}
	
	image.write_image("test.bmp");

	return (0);
}





















