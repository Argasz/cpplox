#pragma once
#include "Interpreter.h"
#include <iostream>
#include "cpplox.h"

Interpreter::Interpreter()
{
}


Interpreter::~Interpreter()
{
}

void Interpreter::interpret(Expr & expression)
{
	try
	{
		varLiteral val = evaluate(expression);
		std::cout << val;
	}
	catch (const LRunTimeError& e)
	{
		runtimeError(e);
	}
}

varLiteral Interpreter::visit(Literal & expr)
{
	return expr.value;
}

varLiteral Interpreter::visit(Grouping & expr)
{
	return evaluate(expr.expression);
}

varLiteral Interpreter::visit(Unary & expr)
{
	varLiteral right = evaluate(expr.right);

	switch (expr.op.type)
	{
	case TokenType::MINUS:
		checkNumberOperand(expr.op, right);
		return varLiteral(-std::get<double>(right));
	case TokenType::BANG:
		return !isTruthy(right);
	}
	//unreachable
	return varLiteral(nullptr);
}



varLiteral Interpreter::visit(Binary & expr)
{
	varLiteral left = evaluate(expr.left);
	varLiteral right = evaluate(expr.right);

	switch (expr.op.type)
	{
	case TokenType::MINUS:
		checkNumberOperands(expr.op, left, right);
		return varLiteral(std::get<double>(left) - std::get<double>(right));
	case TokenType::SLASH:
		checkNumberOperands(expr.op, left, right);
		checkDivisionByZero(expr.op, right);
		return varLiteral(std::get<double>(left) / std::get<double>(right));
	case TokenType::STAR:
		checkNumberOperands(expr.op, left, right);
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
		throw LRunTimeError(expr.op, "Operands must be two numbers or two strings");
	case TokenType::GREATER:
		checkNumberOperands(expr.op, left, right);
		return varLiteral(std::get<double>(left) > std::get<double>(right));
	case TokenType::GREATER_EQUAL:
		checkNumberOperands(expr.op, left, right);
		return varLiteral(std::get<double>(left) >= std::get<double>(right));
	case TokenType::LESS:
		checkNumberOperands(expr.op, left, right);
		return varLiteral(std::get<double>(left) < std::get<double>(right));
	case TokenType::LESS_EQUAL:
		checkNumberOperands(expr.op, left, right);
		return varLiteral(std::get<double>(left) <= std::get<double>(right));
	case TokenType::EQUAL_EQUAL:
		return isEqual(left, right);
	case TokenType::BANG_EQUAL:
		return !isEqual(left, right);
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

bool Interpreter::isEqual(varLiteral a, varLiteral b)
{
	if (std::holds_alternative<nullptr_t>(a) && std::holds_alternative<nullptr_t>(b))
	{
		return true;
	}
	if (std::holds_alternative<nullptr_t>(a))
	{
		return false;
	}

	return a == b;
}

void Interpreter::checkNumberOperand(Token op, varLiteral operand)
{
	if (std::holds_alternative<double>(operand))
	{
		return;
	}

	throw LRunTimeError(op, "Operand must be a number");
}

void Interpreter::checkNumberOperands(Token op, varLiteral left, varLiteral right)
{
	if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
	{
		return;
	}

	throw LRunTimeError(op, "Operands must be numbers");
}

void Interpreter::checkDivisionByZero(Token op, varLiteral right)
{
	if (std::get<double>(right) == 0.0)
	{
		throw LRunTimeError(op, "Division by zero");
	}

	return;
}
