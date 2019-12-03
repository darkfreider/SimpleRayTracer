
#if !defined (_GEOMETRY_BUILDER_H)

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>

class Tokeniser
{
	typedef enum Token_kind
	{
		TOKEN_EOF = 0,
		TOKEN_LAST = 128,

		TOKEN_NAME,
		TOKEN_INT,
		TOKEN_FLOAT,
	} Token_kind;

	struct Token
	{
		Token_kind kind;
		union
		{
			uint32_t int_val;
			float float_val;
		};
		std::string str_val;

		explicit Token(Token_kind k = TOKEN_EOF) : kind(k)
		{

		}

		Token(const Token& nt)
		{
			kind = nt.kind;
			if (kind == TOKEN_FLOAT)
			{
				float_val = nt.float_val;
			}
			else if (kind == TOKEN_INT)
			{
				int_val = nt.int_val;
			}
			else if (kind == TOKEN_NAME)
			{
				str_val = nt.str_val;
			}
		}

		~Token()
		{

		}
	};

	uint32_t current_char;
	std::string src;
	std::vector<Token> m_tokens;

	Token m_token;

public:

	Tokeniser(const std::string& s) : src(s), current_char(0)
	{
		generate_tokens();
	}

	~Tokeniser()
	{

	}

	std::vector<Token> get_tokens()
	{
		return m_tokens;
	}

	void print_tokens()
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
					// TODO(max): implement float recognistion
					assert(0);
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

private:

	void generate_tokens()
	{
		next_token();
		while (current_char != src.length())
		{
			next_token();
		}
		m_tokens.push_back(Token(TOKEN_EOF));
	}

	void next_token(void)
	{
		while (isspace(src[current_char]))
		{
			current_char++;
		}

		switch (src[current_char])
		{
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
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
};

class Geometry_builder
{

public:
	Geometry_builder()
	{

	}

	~Geometry_builder()
	{

	}

private:

};

#define _GEOMETRY_BUILDER_H
#endif