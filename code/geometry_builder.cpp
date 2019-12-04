

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>
#include <unordered_map>

#include "tokeniser.h"
#include "material.h"
#include "object.h"

void definition()
{
	
}

// QUESTION(max): should I return scene geometry or store it internally and have a getter function
void Geometry_builder::generate_geometry(const std::string& geometry_descr)
{
	m_tokeniser.generate_tokens(geometry_descr);

	auto& token = m_tokeniser.get_token();
	while (token.kind != Tokeniser::Token_kind::TOKEN_EOF)
	{
		token = m_tokeniser.get_token();
	}
}

void test_tokeniser()
{
	Tokeniser t;

	t.generate_tokens("123  hello_seilor	#this is a comment_lol  234 \n to_23	!434 2 234 \n it's me- 12^3&12");

	t.print_tokens();
}