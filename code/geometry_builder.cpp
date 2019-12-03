

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>

#include "geometry_builder.h"

void test_tokeniser()
{
	Tokeniser t("123  hello_seilor	#this is a comment_lol  234 \n to_23	!434 2 234 \n it's me- 12^3&12");

	t.print_tokens();
}