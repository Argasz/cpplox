#pragma once
#include "Expr.h"
#include "Stmt.h"

class LRunTimeError : public std::exception
{
public:
	const Token token;
	const std::string msg;
	std::string message;

	LRunTimeError(const Token& token, const std::string& msg) : token(token), msg(msg){
		message = token.lexeme;
		message += " ";
		message += msg;
	}
	const char* what() const throw()
	{
		return message.c_str();
	}

};


class Interpreter : public IExprVisitor, public IStmtVisitor
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
	void checkDivisionByZero(Token op, varLiteral right);
};



