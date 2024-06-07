#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "TokenType.h"
#include "Token.h"
#include <variant>

typedef std::variant<std::string, double, bool, std::nullptr_t, std::monostate> Var_literal;

class Scanner
{
private:
	std::string source;
	std::vector<Token> tokens;
	std::unordered_map<std::string, TokenType> keywords;

	std::string::size_type start = 0;
	std::string::size_type current = 0;
	int line = 1;

	bool isAtEnd();
	char advance();
	void addToken(TokenType type, Var_literal literal = "");
	void scanToken();
	bool match(char expected);
	char peek();
	void string();
	bool isDigit(char c);
	void number();
	char peekNext();
	void identifier();
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);
public:
	std::vector<Token> scanTokens();

	Scanner(std::string source);
	~Scanner() = default;
};

