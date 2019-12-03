#pragma once
#include <string>
#include <variant>
#include "TokenType.h"


typedef std::variant<std::string, double, bool, std::nullptr_t> varLiteral;

class Token
{
public:
	TokenType type;
	std::string lexeme;
	varLiteral lit;
	int line;

	Token(TokenType type, std::string lexeme, varLiteral literal, int line) : type(type), lexeme(lexeme), lit(literal), line(line) {};
	~Token() = default;
	friend std::ostream& operator << (std::ostream& stream, const Token& token);
};

std::ostream& operator << (std::ostream& stream, const Token& token);

