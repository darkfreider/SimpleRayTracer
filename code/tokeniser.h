
#if !defined(_TOKENISER_H)

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

	Token m_token;
	std::vector<Token> m_tokens;

public:

	Tokeniser() : current_char(0)
	{

	}

	~Tokeniser()
	{

	}

	void generate_tokens(const std::string &s);

	std::vector<Token> get_tokens();
	void print_tokens();

private:

	void next_token(void);
};

#define _TOKENISER_H
#endif