#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "TokenType.h"
#include "Token.h"
#include <variant>

typedef std::variant<std::string, double, bool, std::nullptr_t> varLiteral;

class Scanner
{
private:
	std::string source;
	std::vector<Token*> tokens;
	std::unordered_map<std::string, TokenType> keywords;

	int start = 0;
	int current = 0;
	int line = 1;

	bool isAtEnd();
	char advance();
	void addToken(TokenType type, varLiteral literal = "");
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
	std::vector<Token*> scanTokens();

	Scanner(std::string source);
	~Scanner() = default;
};

