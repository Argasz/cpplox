#pragma once
#include "Parser.h"
#include "cpplox.h"
#include <vector>
#include "Stmt.h"

typedef std::variant<std::string, double, bool, std::nullptr_t, std::monostate> Var_literal;

Parser::~Parser()
{
}

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
	auto stmts = std::vector<std::unique_ptr<Stmt>>();
	while (!isAtEnd())
	{
		stmts.push_back(statement());
	}
	return stmts;
}

std::unique_ptr<Expr> Parser::expression()
{
	return equality();
}

std::unique_ptr<Stmt> Parser::statement()
{
	if (match({ TokenType::PRINT }))
	{
		return printStatement();
	}

	return expressionStatement();
}

std::unique_ptr<Stmt> Parser::printStatement()
{
	auto value = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after value.");
	return value;
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
	auto expr = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after expression.");
	return std::make_unique<ExpressionStmt>(expr);

}

std::unique_ptr<Stmt> Parser::declarationStatement()
{
	try {
		if (match({ TokenType::VAR }))
			return varDeclarationStatement();
		return statement();
	}
	catch (ParseException e)
	{
		synchronize();
		return nullptr;
	}
}

std::unique_ptr<Stmt> Parser::varDeclarationStatement()
{
	Token name = consume(TokenType::IDENTIFIER, "Expect variable name");
	Expr* initializer;

	if (match({ TokenType::EQUAL }))
		initializer = expression();

	consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
	return new VarStmt(name, initializer);
}

std::unique_ptr<Expr> Parser::equality()
{
	auto expr = comparison();

	while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL }))
	{
		Token op = previous();
		auto right = comparison();
		expr = new Binary(*expr, op, *right);
		/*auto bin = new Binary(expr, op, right);
		return *bin;*/
	}

	return expr;
}

std::unique_ptr<Expr> Parser::comparison()
{
	auto expr = addition();

	while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL }))
	{
		Token op = previous();
		auto right = addition();
		expr = new Binary(*expr, op, *right);
		/*auto bin = new Binary(expr, op, right);
		return *bin;*/
	}

	return expr;
}

std::unique_ptr<Expr> Parser::addition()
{
	auto expr = multiplication();

	while (match({ TokenType::MINUS, TokenType::PLUS }))
	{
		Token op = previous();
		auto right = multiplication();
		expr = new Binary(expr, op, right);
		//auto bin = new Binary(expr, op, right);
		//return *bin;
	}

	return expr;
}

std::unique_ptr<Expr> Parser::multiplication()
{
	auto expr = unary();

	while (match({ TokenType::SLASH, TokenType::STAR }))
	{
		Token op = previous();
		auto right = unary();
		// I think this works because the original expr pointer will be moved into the new Expr and then the now nulled expr pointer can be reassigned safely
		expr = std::make_unique<Expr>(expr, op, right);
		//auto ret = new Binary(expr, op, right);
		//return *ret;
	}

	return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
	if (match({ TokenType::BANG, TokenType::MINUS }))
	{
		Token op = previous();
		auto right = unary();
		
		return std::make_unique<Unary>(op, right);
	}

	return primary();
}

std::unique_ptr<Expr> Parser::primary()
{
	Var_literal val;
	if (match({ TokenType::FALSE }))
	{ 
		val = false;
		return std::make_unique<Literal>(val);
	}
	if (match({ TokenType::TRUE }))
	{
		val = true;
		return std::make_unique<Literal>(val);
	}
	if (match({ TokenType::NIL }))
	{
		val = nullptr;
		return std::make_unique<Literal>(val);
	}
	if (match({TokenType::NUMBER, TokenType::STRING}))
	{
		return std::make_unique<Literal>(previous().lit);
	}
	if (match({ TokenType::LEFT_PAREN }))
	{
		auto expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
		return std::make_unique<Grouping>(expr);
	}
	
	throw error(peek(), "Expect expression.");
}

bool Parser::match(std::vector<TokenType> types)
{
	for (auto type : types)
	{
		if (check(type))
		{
			advance();
			return true;
		}
	}
	return false;
}

Token Parser::consume(TokenType type, std::string message) {
	if (check(type)) {
		return advance();
	}
	
	throw error(peek(), message);
}

bool Parser::check(TokenType type)
{
	if (isAtEnd())
		return false;

	return peek().type == type;
}

Token Parser::advance()
{
	if (!isAtEnd())
		current++;
	
	return previous();
}

bool Parser::isAtEnd() const
{
	return peek().type == TokenType::EOFF;
}

Token Parser::peek() const
{
	return tokens.at(current);
}

Token Parser::previous() const
{
	return tokens.at(current-1);
}

ParseException Parser::error(const Token& token, const std::string& message)
{
	cpploxError(token, message);
	return ParseException();
}

void Parser::synchronize()
{
	advance();

	while (!isAtEnd())
	{
		if (previous().type == TokenType::SEMICOLON)
		{
			return;
		}
		
		switch (peek().type)
		{
		case TokenType::CLASS:
		case TokenType::FUN:
		case TokenType::VAR:
		case TokenType::FOR:
		case TokenType::IF:
		case TokenType::WHILE:
		case TokenType::PRINT:
		case TokenType::RETURN:
			return;
		}

		advance();
	}
}

