
#include "Parser.h"
#include "cpplox.h"
#include <vector>
#include "Stmt.h"

typedef std::variant<std::string, double, bool, std::nullptr_t, std::monostate> varLiteral;

Parser::~Parser()
{
}

std::vector<Stmt*>* Parser::parse()
{
	auto stmts = new std::vector<Stmt*>();
	while (!isAtEnd())
	{
		stmts->push_back(statement());
	}
	return stmts;
}

Expr* Parser::expression()
{
	return equality();
}

Stmt * Parser::statement()
{
	if (match({ TokenType::PRINT }))
	{
		return printStatement();
	}

	return expressionStatement();
}

Stmt * Parser::printStatement()
{
	Expr* value = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after value.");
	return new PrintStmt(*value);
}

Stmt * Parser::expressionStatement()
{
	Expr* expr = expression();
	consume(TokenType::SEMICOLON, "Expect ';' after expression.");
	return new ExpressionStmt(*expr);

}

Stmt * Parser::declarationStatement()
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

Stmt * Parser::varDeclarationStatement()
{
	Token name = consume(TokenType::IDENTIFIER, "Expect variable name");
	Expr* initializer;

	if (match({ TokenType::EQUAL }))
		initializer = expression();

	consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
	return new VarStmt(name, initializer);
}

Expr* Parser::equality()
{
	Expr* expr = comparison();

	while (match({ TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL }))
	{
		Token op = previous();
		Expr* right = comparison();
		expr = new Binary(*expr, op, *right);
		/*auto bin = new Binary(expr, op, right);
		return *bin;*/
	}

	return expr;
}

Expr* Parser::comparison()
{
	Expr* expr = addition();

	while (match({ TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL }))
	{
		Token op = previous();
		Expr* right = addition();
		expr = new Binary(*expr, op, *right);
		/*auto bin = new Binary(expr, op, right);
		return *bin;*/
	}

	return expr;
}

Expr* Parser::addition()
{
	Expr* expr = multiplication();

	while (match({ TokenType::MINUS, TokenType::PLUS }))
	{
		Token op = previous();
		Expr* right = multiplication();
		expr = new Binary(*expr, op, *right);
		//auto bin = new Binary(expr, op, right);
		//return *bin;
	}

	return expr;
}

Expr* Parser::multiplication()
{
	Expr* expr = unary();

	while (match({ TokenType::SLASH, TokenType::STAR }))
	{
		Token op = previous();
		Expr* right = unary();
		expr = new Binary(*expr, op, *right);
		//auto ret = new Binary(expr, op, right);
		//return *ret;
	}

	return expr;
}

Expr* Parser::unary()
{
	if (match({ TokenType::BANG, TokenType::MINUS }))
	{
		Token op = previous();
		Expr* right = unary();
		
		return new Unary(op, *right);
	}

	return primary();
}

Expr* Parser::primary()
{
	varLiteral val;
	if (match({ TokenType::FALSE }))
	{ 
		val = false;
		return new Literal(val);
	}
	if (match({ TokenType::TRUE }))
	{
		val = true;
		return new Literal(val);
	}
	if (match({ TokenType::NIL }))
	{
		val = nullptr;
		return new Literal(val);
	}
	if (match({TokenType::NUMBER, TokenType::STRING}))
	{
		return new Literal(previous().lit);
	}
	if (match({ TokenType::LEFT_PAREN }))
	{
		Expr* expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
		return new Grouping(*expr);
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

Token& Parser::consume(TokenType type, std::string message) {
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

Token& Parser::advance()
{
	if (!isAtEnd())
		current++;
	
	return previous();
}

bool Parser::isAtEnd()
{
	return peek().type == TokenType::EOFF;
}

Token& Parser::peek()
{
	return *tokens.at(current);
}

Token& Parser::previous()
{
	return *tokens.at(current-1);
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

