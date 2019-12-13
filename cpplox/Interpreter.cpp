#include "Interpreter.h"



Interpreter::Interpreter()
{
}


Interpreter::~Interpreter()
{
}

varLiteral Interpreter::visitLiteralExpr(Literal & expr)
{
	return expr.value;
}

varLiteral Interpreter::visitGroupingExpr(Grouping & expr)
{
	return evaluate(expr);
}

varLiteral Interpreter::visitUnaryExpr(Unary & expr)
{
	varLiteral right = evaluate(expr.right);

	switch (expr.op.type)
	{
	case TokenType::MINUS:
		return varLiteral(-std::get<double>(right));
	case TokenType::BANG:
		return !isTruthy(right);
	}
	//unreachable
	return varLiteral(nullptr);
}

varLiteral Interpreter::visitBinaryExpr(Binary & expr)
{
	varLiteral left = evaluate(expr.left);
	varLiteral right = evaluate(expr.right);

	switch (expr.op.type)
	{
	case TokenType::MINUS:
		return varLiteral(std::get<double>(left) - std::get<double>(right));
	case TokenType::SLASH:
		return varLiteral(std::get<double>(left) / std::get<double>(right));
	case TokenType::STAR:
		return varLiteral(std::get<double>(left) * std::get<double>(right));
	case TokenType::PLUS:
		if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
		{
			return varLiteral(std::get<double>(left) + std::get<double>(right));
		}
		if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
		{
			return varLiteral(std::get<std::string>(left) + std::get<std::string>(right));
		}
	case TokenType::GREATER:
		return varLiteral();
	}

	//unreachable
	return varLiteral(nullptr);

}

bool Interpreter::isTruthy(varLiteral v)
{
	if (std::holds_alternative<nullptr_t>(v))
	{
		return false;
	}
	if (std::holds_alternative<bool>(v))
	{
		return std::get<bool>(v);
	}
	
	return true;
}
