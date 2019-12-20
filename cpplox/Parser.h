#pragma once
#include <vector>
#include "Token.h"
#include "Expr.h"

class Stmt;

class ParseException : public std::exception
{
	const char* what() const throw()
	{
		return "ParseException";
	}
};

class Parser
{
public:
	Parser(std::vector<Token*> tokens) : tokens(tokens) {};
	std::vector<Stmt*>* parse();
	~Parser();
private:
	std::vector<Token*> tokens;
	int current = 0;
	
	Expr* expression();
	Expr* equality();
	Expr* comparison();
	Expr* addition();
	Expr* multiplication();
	Expr* unary();
	Expr* primary();

	Stmt* statement();
	Stmt* printStatement();
	Stmt* expressionStatement();

	bool match(std::vector<TokenType> types);
	Token& consume(TokenType token, std::string message);
	bool check(TokenType type);
	Token& advance();
	bool isAtEnd();
	Token& peek();
	Token& previous();
	ParseException error(const Token& token, const std::string& message);
	void synchronize();
};

