
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

class Geometry_builder
{
	Tokeniser m_tokeniser;

	std::unordered_map<std::string, Material*> m_material_table;
	std::vector<Material *> m_materials;
	std::vector<Object *> m_objects;

	// QUESTION(max)
	// Scene shoud be represented as:
	//     vector<Materials *>
	//     vector<Objects *>
	//     vector<Light *>
public:
	Geometry_builder()
	{
	}

	~Geometry_builder()
	{

	}

	void generate_geometry(const std::string& geometry_descr);

	const std::vector<Object *>& get_objects()
	{
		return (m_objects);
	}

private:
	bool is_token(Tokeniser::Token_kind kind);
	bool expect_token(Tokeniser::Token_kind kind);
	bool match_token(Tokeniser::Token_kind kind);
	bool match_token_int(uint32_t int_val);
	bool match_token_str(const std::string& str_val);

	void definition();
	void material_definition();
	void sphere_definition();

	void init_material_attributes(Material *m);
	void init_sphere_attributes(Sphere *s);
};

#define _GEOMETRY_BUILDER_H
#endif