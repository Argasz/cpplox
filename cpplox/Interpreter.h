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

	void interpret(const Expr& expression) const;

	Var_literal visit(Literal& expr);
	Var_literal visit(Grouping& expr);
	Var_literal visit(Unary& expr);
	Var_literal visit(Binary& expr);
	Var_literal visit(Variable& expr) {};
	Var_literal visit(ExpressionStmt& stmt) {};
	Var_literal visit(PrintStmt& stmt) {};

private:
	Var_literal evaluate(const Expr& expr) const
	{
		return expr.accept(*this);
	}

	bool isTruthy(Var_literal v);
	bool isEqual(Var_literal a, Var_literal b);
	void checkNumberOperand(Token op, Var_literal operand);
	void checkNumberOperands(Token op, Var_literal left, Var_literal right);
	void checkDivisionByZero(Token op, Var_literal right);
};



