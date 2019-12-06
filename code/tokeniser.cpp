
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>

#include "tokeniser.h"

void Tokeniser::generate_tokens(const std::string &s)
{
	src = s;

	next_token();
	while (current_char < src.length())
	{
		next_token();
	}
	m_tokens.push_back(Token(TOKEN_EOF));
}

void Tokeniser::print_tokens()
{
	for (uint32_t i = 0; i < m_tokens.size(); i++)
	{
		switch (m_tokens[i].kind)
		{
			case TOKEN_NAME:
			{
				std::cout << "TOKEN_NAME: " << m_tokens[i].str_val << std::endl;
			} break;

			case TOKEN_INT:
			{
				std::cout << "TOKEN_INT: " << m_tokens[i].int_val << std::endl;
			} break;

			case TOKEN_FLOAT:
			{
				std::cout << "TOKEN_FLOAT: " << m_tokens[i].float_val << std::endl;
			} break;

			case TOKEN_EOF:
			{
				std::cout << "TOKEN_EOF" << std::endl;
			};

			default:
			{
				std::cout << (unsigned char)m_tokens[i].kind << std::endl;
			} break;
		}
	}
}

void Tokeniser::tokenise_int_val()
{
	// TODO(max): check for overflow
	m_token.kind = TOKEN_INT;
	m_token.int_val = 0;
	while (isdigit(src[current_char]))
	{
		m_token.int_val *= 10;
		m_token.int_val += src[current_char] - '0';
		current_char++;
	}

	m_tokens.push_back(m_token);
}

void Tokeniser::tokenise_float_val()
{
	m_token.kind = TOKEN_FLOAT;

	std::string::size_type sz = 0;
	m_token.float_val = std::stof(src.substr(current_char), &sz);
	if (sz == 0)
	{
		// error
		assert(0);
	}
	current_char += sz;
	m_tokens.push_back(m_token);
}

void Tokeniser::next_token(void)
{
	while (isspace(src[current_char]))
	{
		current_char++;
	}

	switch (src[current_char])
	{
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
		{
			uint32_t book_mark = current_char;
			while (isdigit(src[current_char]))
			{
				current_char++;
			}

			if (src[current_char] == '.')
			{
				current_char = book_mark;
				tokenise_float_val();
			}
			else
			{
				current_char = book_mark;
				tokenise_int_val();
			}
		} break;

		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
		case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i':
		case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		case '_':
		{
			m_token.kind = TOKEN_NAME;

			uint32_t str_start = current_char;
			while (isalnum(src[current_char]) || src[current_char] == '_')
				current_char++;

			uint32_t str_len = current_char - str_start;
			m_token.str_val = src.substr(str_start, str_len);

			m_tokens.push_back(m_token);
		} break;

		case '#':
		{
			while (src[current_char] != '\n')
				current_char++;
		} break;

		default:
		{
			m_token.kind = (Token_kind)src[current_char++];
			m_tokens.push_back(m_token);
		} break;
	}
}