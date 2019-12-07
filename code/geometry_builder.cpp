

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include <cctype>
#include <cassert>
#include <cstdlib>

#include "math.h"
#include "tokeniser.h"
#include "material.h"
#include "object.h"


void Geometry_builder::syntax_error(const std::string& message)
{
	std::cerr << "Syntas error: " << message << " at line " << m_tokeniser.get_token().line_number << std::endl;
	std::exit(1234);
}

bool Geometry_builder::is_token(Tokeniser::Token_kind kind)
{
	if (m_tokeniser.get_token().kind == kind)
	{
		return (true);
	}

	return (false);
}

bool Geometry_builder::expect_token(Tokeniser::Token_kind kind)
{
	if (m_tokeniser.get_token().kind != kind)
	{
		std::cerr << "Unexpected token (?)" << "at line " << m_tokeniser.get_token().line_number << std::endl;
		exit(1234);
	}

	m_tokeniser.advance_token();
	return (true);
}

bool Geometry_builder::match_token(Tokeniser::Token_kind kind)
{
	if (m_tokeniser.get_token().kind == kind)
	{
		m_tokeniser.advance_token();
		return (true);
	}

	return (false);
}

bool Geometry_builder::match_token_int(uint32_t int_val)
{
	if (m_tokeniser.get_token().int_val == int_val)
	{
		m_tokeniser.advance_token();
		return (true);
	}

	return (false);
}

bool Geometry_builder::match_token_str(const std::string& str_val)
{
	if (m_tokeniser.get_token().str_val == str_val)
	{
		m_tokeniser.advance_token();
		return (true);
	}

	return (false);
}

Vector3 Geometry_builder::parse_vector3_float()
{
	// TODO(max): abstract it into a function (init_float_vector)
	expect_token(Tokeniser::Token_kind('='));

	Vector3 result;
	for (int i = 0; i < 3; i++)
	{
		bool need_negate = false;
		if (match_token(Tokeniser::Token_kind('-')))
		{
			need_negate = true;
		}

		if (is_token(Tokeniser::Token_kind::TOKEN_FLOAT))
		{
			result[i] = m_tokeniser.get_token().float_val;
			result[i] = (need_negate) ? -result[i] : result[i];
			expect_token(Tokeniser::Token_kind::TOKEN_FLOAT);
		}
		else
		{
			syntax_error("Expected Token_kind::TOKEN_FLOAT");
		}
	}

	return (result);
}

void Geometry_builder::init_object_material(Object *obj)
{
	expect_token(Tokeniser::Token_kind('='));

	if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
	{
		auto entry = m_material_table.find(m_tokeniser.get_token().str_val);
		expect_token(Tokeniser::Token_kind::TOKEN_NAME);
		if (entry != m_material_table.end())
		{
			obj->set_material(entry->second);
		}
		else
		{
			syntax_error("Undefined material name");
		}
	}
	else
	{
		syntax_error("Expected material name");
	}
}

void Geometry_builder::init_triangle_attributes(Triangle *t)
{
	// TODO(max): check for all attributes to be inited
	expect_token(Tokeniser::Token_kind('{'));

	while (!match_token(Tokeniser::Token_kind('}')))
	{
		if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
		{
			if (match_token_str("material"))
			{
				init_object_material(t);
			}
			else if (match_token_str("v0"))
			{
				t->operator[](0) = parse_vector3_float();
			}
			else if (match_token_str("v1"))
			{
				t->operator[](1) = parse_vector3_float();
			}
			else if (match_token_str("v2"))
			{
				t->operator[](2) = parse_vector3_float();
			}
			else
			{
				syntax_error("Unknown triangle attribute");
			}

			expect_token(Tokeniser::Token_kind(';'));
		}
		else
		{
			syntax_error("Expected material attribute name");
		}
	}
}

void Geometry_builder::triangle_definition()
{
	Triangle *t = new Triangle();
	m_objects.push_back(t);
	init_triangle_attributes(t);
}

void Geometry_builder::init_sphere_attributes(Sphere *s)
{
	expect_token(Tokeniser::Token_kind('{'));

	bool position_inited = false;
	bool radius_inited = false;
	bool material_inited = false;

	while (!match_token(Tokeniser::Token_kind('}')))
	{
		if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
		{
			if (match_token_str("radius"))
			{
				if (radius_inited)
				{
					syntax_error("Sphere's radius redefinition");
				}

				expect_token(Tokeniser::Token_kind('='));
				if (is_token(Tokeniser::Token_kind::TOKEN_FLOAT))
				{
					float flt_val = m_tokeniser.get_token().float_val;
					expect_token(Tokeniser::Token_kind::TOKEN_FLOAT);
					s->set_radius(flt_val);

					radius_inited = true;
				}
				else
				{
					syntax_error("Expected Token_kind::TOKEN_FLOAT");
				}
			}
			else if (match_token_str("position"))
			{
				if (position_inited)
				{
					syntax_error("Sphere's position redefinition");
				}
				
				Vector3 pos_value = parse_vector3_float();

				s->set_position(pos_value);
				position_inited = true;
			}
			else if (match_token_str("material"))
			{
				if (material_inited)
				{
					syntax_error("Sphere's material redefinition");
				}
				
				init_object_material(s);
				material_inited = true;
			}
			else
			{
				syntax_error("Unknown sphere attribute");
			}

			expect_token(Tokeniser::Token_kind(';'));
		}
		else
		{
			syntax_error("Expected material attribute name");
		}
	}

	// TODO(max): generate an error for each attribute
	if (!material_inited || !position_inited || !radius_inited)
	{
		syntax_error("Not all sphere attributes are initialised");
	}
}

void Geometry_builder::sphere_definition()
{
	Sphere *s = new Sphere();
	m_objects.push_back(s);
	init_sphere_attributes(s);
}

void Geometry_builder::init_material_attributes(Material *mat)
{
	// TODO(max): check for all attributes to be inited
	expect_token(Tokeniser::Token_kind('{'));

	while (!match_token(Tokeniser::Token_kind('}')))
	{
		if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
		{
			if (match_token_str("color"))
			{
				expect_token(Tokeniser::Token_kind('='));

				Vector3 color_value;
				uint32_t rgb[3];
				for (int i = 0; i < 3; i++)
				{
					if (is_token(Tokeniser::Token_kind::TOKEN_INT))
					{
						rgb[i] = m_tokeniser.get_token().int_val;
						if (rgb[i] > 255)
						{
							syntax_error("Expected color value in range 0-255");
						}
						expect_token(Tokeniser::Token_kind::TOKEN_INT);
					}
					else
					{
						syntax_error("Expected Token_kind::TOKEN_INT");
					}
				}

				color_value.r = float(rgb[0]) / 255.0f;
				color_value.g = float(rgb[1]) / 255.0f;
				color_value.b = float(rgb[2]) / 255.0f;
				mat->set_color(color_value);
			}
			else
			{
				syntax_error("Unknown material attribute");
			}

			expect_token(Tokeniser::Token_kind(';'));
		}
		else
		{
			syntax_error("Expected material attribute name");
		}
	}
}

void Geometry_builder::material_definition()
{
	expect_token(Tokeniser::Token_kind(':'));

	if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
	{
		std::string mat_name(m_tokeniser.get_token().str_val);
		expect_token(Tokeniser::Token_kind::TOKEN_NAME);
		if (m_material_table.find(mat_name) == m_material_table.end())
		{
			m_materials.push_back(new Material());

			init_material_attributes(m_materials.back());
			
			m_material_table.insert(std::make_pair(mat_name, m_materials.back()));
		}
		else
		{
			syntax_error("Redefinition of material");
		}
	}
	else
	{
		syntax_error("Expected material name");
	}
}

void Geometry_builder::definition()
{
	if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
	{
		if (match_token_str("material"))
		{
			material_definition();
		}
		else if (match_token_str("sphere"))
		{
			sphere_definition();
		}
		else if (match_token_str("triangle"))
		{
			triangle_definition();
		}
		else if (match_token_str("light"))
		{
			// TODO(max): Implement it !!!!!!
			assert(0);
		}
		else
		{
			syntax_error("Trying to define unknown type of object");
		}
	}
	else
	{
		syntax_error("Expected Token_kind::TOKEN_NAME in object definition");
	}
}

void Geometry_builder::generate_geometry(const std::string& geometry_descr)
{
	m_tokeniser.generate_tokens(geometry_descr);

	while (!match_token(Tokeniser::Token_kind::TOKEN_EOF))
	{
		definition();
	}
}

void test_tokeniser()
{
	Tokeniser t;

	t.generate_tokens("123  hello_seilor	#this is a comment_lol  234 \n to_23	!434 2 234 \n it's me- 12^3&12");

	t.print_tokens();
}