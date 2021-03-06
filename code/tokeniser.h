
#if !defined(_TOKENISER_H)

#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <cassert>

class Tokeniser
{
public:
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
		uint32_t line_number;

		explicit Token(Token_kind k = TOKEN_EOF) : kind(k)
		{
		}

		Token(const Token& nt)
		{
			kind = nt.kind;
			line_number = nt.line_number;
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

		Token& operator=(const Token& rhs)
		{
			if (this == &rhs)
				return (*this);

			kind = rhs.kind;
			line_number = rhs.line_number;
			if (kind == TOKEN_FLOAT)
			{
				float_val = rhs.float_val;
			}
			else if (kind == TOKEN_INT)
			{
				int_val = rhs.int_val;
			}
			else if (kind == TOKEN_NAME)
			{
				str_val = rhs.str_val;
			}

			return (*this);
		}

		~Token()
		{

		}
	};

private:

	uint32_t current_char;
	std::string src;

	Token m_token;
	std::vector<Token> m_tokens;
	uint32_t m_current_token;

	uint32_t m_line_number;

public:

	Tokeniser() : current_char(0), m_current_token(0), m_line_number(1)
	{
	}

	~Tokeniser()
	{
	}

	Token& get_token()
	{
		return m_tokens.at(m_current_token);
	}

	void advance_token()
	{
		m_current_token++;
	}

	void generate_tokens(const std::string &s);

	void print_tokens();

private:

	void next_token();
	void tokenise_int_val();
	void tokenise_float_val();
};

#define _TOKENISER_H
#endif