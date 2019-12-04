
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
	std::unordered_map<std::string, Material*> m_material_table; // QUESTION(max): do I need to store <string, Material*> or <string, Material>?

	std::vector<Material *> m_materials;
	std::vector<Object *> m_objects;

	// QUESTION(max)
	// Scene shoud be represented as:
	//     vector<Materials *>
	//     vector<Objects *>
	//     vector<Light *>

	/*
		material_vector.push_back(new Material());
		material_table.insert(std::make_pair("metal", material_vector.back()));
	*/
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

};

void test_tokeniser();

#define _GEOMETRY_BUILDER_H
#endif