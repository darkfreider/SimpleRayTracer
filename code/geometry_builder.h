
#if !defined(_GEOMETRY_BUILDER_H)

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>

#include "tokeniser.h"
#include "material.h"
#include "object.h"
#include "camera.h"
#include "light.h"

class Geometry_builder
{
	Tokeniser m_tokeniser;

	std::unordered_map<std::string, Material*> m_material_table;
	std::vector<Material *> m_materials;
	std::vector<Object *> m_objects;
	std::vector<Light *> m_lights;
	std::vector<Camera *> m_cameras;

public:
	Geometry_builder()
	{
	}

	~Geometry_builder()
	{
	}

	void generate_geometry(const std::string& geometry_descr);

	size_t get_camera_count()
	{
		return m_cameras.size();
	}

	Camera& get_camera(int i)
	{
		return *m_cameras[i];
	}

	const std::vector<Light *>& get_lights() const 
	{
		return (m_lights);
	}

	const std::vector<Object *>& get_objects() const
	{
		return (m_objects);
	}

private:

	void syntax_error(const std::string& message);

	bool is_token(Tokeniser::Token_kind kind);
	bool expect_token(Tokeniser::Token_kind kind);
	bool match_token(Tokeniser::Token_kind kind);
	bool match_token_int(uint32_t int_val);
	bool match_token_str(const std::string& str_val);

	void definition();
	void material_definition();
	void sphere_definition();
	void triangle_definition();
	void camera_definition();
	void distant_light_definition();
	void spherical_light_definition();


	void init_material_attributes(Material *m);
	void init_sphere_attributes(Sphere *s);
	void init_triangle_attributes(Triangle *t);
	void init_camera_attributes(Camera *c);
	void init_distant_light_attributes(Distant_light *light);
	void init_spherical_light_attributes(Spherical_light *light);

	void init_object_material(Object *obj);
	Vector3 parse_vector3_float();
	void parse_vector_int(uint32_t *result, uint32_t n);
};

#define _GEOMETRY_BUILDER_H
#endif