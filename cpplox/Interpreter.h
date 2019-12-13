#pragma once
#include "Expr.h"

class Interpreter : public IExprVisitor
{
public:
	Interpreter();
	virtual ~Interpreter();

	varLiteral visitLiteralExpr(Literal& expr);

	varLiteral visitGroupingExpr(Grouping& expr);

	varLiteral visitUnaryExpr(Unary& expr);

	varLiteral visitBinaryExpr(Binary& expr);

private:
	varLiteral evaluate(Expr& expr)
	{
		return expr.accept(*this);
	}

	bool isTruthy(varLiteral v);
};

