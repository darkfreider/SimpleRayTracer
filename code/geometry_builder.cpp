

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>

#include "tokeniser.h"
#include "material.h"
#include "object.h"

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
		// TODO(max): proper syntax error handling
		assert(0);
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

void Geometry_builder::init_sphere_attributes(Sphere *s)
{
	// pos
	// r
	// mat
	expect_token(Tokeniser::Token_kind('{'));

	// TODO(max): use this value to check that all the parameters of the sphere are initialised
	bool position_inited = false;
	bool radius_inited = false;
	bool material_inited = false;

	while (!match_token(Tokeniser::Token_kind('}')))
	{
		if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
		{
			if (match_token_str("radius"))
			{
				// TODO(max):
				expect_token(Tokeniser::Token_kind('='));

				if (is_token(Tokeniser::Token_kind::TOKEN_FLOAT))
				{
					float flt_val = m_tokeniser.get_token().float_val;
					expect_token(Tokeniser::Token_kind::TOKEN_FLOAT);
					s->set_radius(flt_val);
				}
				else
				{
					// TODO(max): proper syntax error handling
					assert(0);
				}
			}
			else if (match_token_str("position"))
			{
				// TODO(max): DOOOOO THIS
				expect_token(Tokeniser::Token_kind('='));

				Vector3 pos_value;
				for (int i = 0; i < 3; i++)
				{
					if (is_token(Tokeniser::Token_kind::TOKEN_FLOAT))
					{
						pos_value[i] = m_tokeniser.get_token().float_val;
						expect_token(Tokeniser::Token_kind::TOKEN_FLOAT);
					}
					else
					{
						// TODO(max): proper syntax error handling
						assert(0);
					}
				}

				s->set_position(pos_value);
			}
			else if (match_token_str("material"))
			{
				// TODO(max): abstract this in a function and use it within a triangle
				expect_token(Tokeniser::Token_kind('='));

				if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
				{
					auto entry = m_material_table.find(m_tokeniser.get_token().str_val);
					expect_token(Tokeniser::Token_kind::TOKEN_NAME);
					if (entry != m_material_table.end())
					{
						s->set_material(entry->second);
					}
					else
					{
						// TODO(max): proper syntax error handling
						assert(0);
					}
				}
				else
				{
					// TODO(max): proper syntax error handling
					assert(0);
				}
			}
			else
			{
				// TODO(max): proper syntax error handling
				assert(0);
			}

			expect_token(Tokeniser::Token_kind(';'));
		}
		else
		{
			// TODO(max): proper syntax error handling
			assert(0);
		}
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
							// TODO(max): proper syntax error handling
							assert(0);
						}
						expect_token(Tokeniser::Token_kind::TOKEN_INT);
					}
					else
					{
						// TODO(max): proper syntax error handling
						assert(0);
					}
				}

				color_value.r = float(rgb[0]) / 255.0f;
				color_value.g = float(rgb[1]) / 255.0f;
				color_value.b = float(rgb[2]) / 255.0f;
				mat->set_color(color_value);
			}
			else
			{
				// TODO(max): proper syntax error handling
				// unknown attribute
				assert(0);
			}

			expect_token(Tokeniser::Token_kind(';'));
		}
		else
		{
			// TODO(max): proper syntax error handling
			assert(0);
		}
	}
}

void Geometry_builder::material_definition()
{
	expect_token(Tokeniser::Token_kind(':'));

	if (is_token(Tokeniser::Token_kind::TOKEN_NAME))
	{
		// save name of the material
		// check if a material with such a name already exists

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
			std::cout << "ERROR: \"material : " << mat_name << "\" already exists" << std::endl;
			// TODO(max): proper syntax error handling
			assert(0);
		}
	}
	else
	{
		// TODO(max): proper syntax error handling
		assert(0);
	}
}

void Geometry_builder::definition()
{
	// objects
	// materials
	// lights
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
			// TODO(max): Implement it !!!!!!
		}
		else
		{
			// TODO(max): proper syntax error handling
			assert(0);
		}
	}
	else
	{
		// TODO(max): proper syntax error handling
		assert(0);
	}
}

void Geometry_builder::generate_geometry(const std::string& geometry_descr)
{
	m_tokeniser.generate_tokens(geometry_descr);
	m_tokeniser.print_tokens();

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