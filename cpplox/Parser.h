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
	Parser(std::vector<Token> tokens) : tokens(tokens) {};
	std::vector<std::unique_ptr<Stmt>> parse();
	~Parser();
private:
	std::vector<Token> tokens;
	std::vector<Token>::size_type current = 0;
	
	std::unique_ptr<Expr> expression();
	std::unique_ptr<Expr> equality();
	std::unique_ptr<Expr> comparison();
	std::unique_ptr<Expr> addition();
	std::unique_ptr<Expr> multiplication();
	std::unique_ptr<Expr> unary();
	std::unique_ptr<Expr> primary();

	std::unique_ptr<Stmt> statement();
	std::unique_ptr<Stmt> printStatement();
	std::unique_ptr<Stmt> expressionStatement();
	std::unique_ptr<Stmt> declarationStatement();
	std::unique_ptr<Stmt> varDeclarationStatement();

	bool match(std::vector<TokenType> types);
	Token consume(TokenType token, std::string message);
	bool check(TokenType type);
	Token advance();
	bool isAtEnd() const;
	Token peek() const;
	Token previous() const;
	ParseException error(const Token& token, const std::string& message);
	void synchronize();
};

