#pragma once
#include <string>
#include <variant>
#include "TokenType.h"

class Token
{
public:
	TokenType type;
	std::string lexeme;
	std::variant<double, std::string> lit;
	int line;

	Token(TokenType type, std::string lexeme, std::variant<double, std::string> literal, int line) : type(type), lexeme(lexeme), lit(literal), line(line) {};
	~Token() = default;
	friend std::ostream& operator << (std::ostream& stream, const Token& token);
};

std::ostream& operator << (std::ostream& stream, const Token& token);

