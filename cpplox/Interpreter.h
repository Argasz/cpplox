#pragma once
#include "Expr.h"

class LRunTimeError : public std::exception
{
public:
	const Token token;
	const std::string msg;

	LRunTimeError(const Token& token, std::string msg) : token(token){}
	const char* what() const throw()
	{
		std::string ret = token.lexeme;
		ret += " ";
		ret += msg;

		return ret.c_str();
	}

};


class Interpreter : public IExprVisitor
{
public:
	Interpreter();
	virtual ~Interpreter();

	void interpret(Expr& expression);

	varLiteral visit(Literal& expr);
	varLiteral visit(Grouping& expr);
	varLiteral visit(Unary& expr);
	varLiteral visit(Binary& expr);

private:
	varLiteral evaluate(Expr& expr)
	{
		return expr.accept(*this);
	}

	bool isTruthy(varLiteral v);
	bool isEqual(varLiteral a, varLiteral b);
	void checkNumberOperand(Token op, varLiteral operand);
	void checkNumberOperands(Token op, varLiteral left, varLiteral right);
};



