

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>

#include "tokeniser.h"
#include "geometry_builder.h"


// QUESTION(max): should I return scene geometry or store it internally and have a getter function
void Geometry_builder::generate_geometry(const std::string& geometry_descr)
{
	m_tokeniser.generate_tokens(geometry_descr);
}

void test_tokeniser()
{
	Tokeniser t;

	t.generate_tokens("123  hello_seilor	#this is a comment_lol  234 \n to_23	!434 2 234 \n it's me- 12^3&12");

	t.print_tokens();
}