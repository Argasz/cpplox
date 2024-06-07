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

void Interpreter::interpret(const Expr & expression) const
{
	try
	{
		Var_literal val = evaluate(expression);
		std::cout << val;
	}
	catch (const LRunTimeError& e)
	{
		runtimeError(e);
	}
}

Var_literal Interpreter::visit(Literal & expr)
{
	return expr.value;
}

Var_literal Interpreter::visit(Grouping & expr)
{
	return evaluate(expr.expression);
}

Var_literal Interpreter::visit(Unary & expr)
{
	Var_literal right = evaluate(expr.right);

	switch (expr.op.type)
	{
	case TokenType::MINUS:
		checkNumberOperand(expr.op, right);
		return Var_literal(-std::get<double>(right));
	case TokenType::BANG:
		return !isTruthy(right);
	}
	//unreachable
	__assume(false);
}



Var_literal Interpreter::visit(Binary & expr)
{
	Var_literal left = evaluate(expr.left);
	Var_literal right = evaluate(expr.right);

	switch (expr.op.type)
	{
	case TokenType::MINUS:
		checkNumberOperands(expr.op, left, right);
		return Var_literal(std::get<double>(left) - std::get<double>(right));
	case TokenType::SLASH:
		checkNumberOperands(expr.op, left, right);
		checkDivisionByZero(expr.op, right);
		return Var_literal(std::get<double>(left) / std::get<double>(right));
	case TokenType::STAR:
		checkNumberOperands(expr.op, left, right);
		return Var_literal(std::get<double>(left) * std::get<double>(right));
	case TokenType::PLUS:
		if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
		{
			return Var_literal(std::get<double>(left) + std::get<double>(right));
		}
		if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
		{
			return Var_literal(std::get<std::string>(left) + std::get<std::string>(right));
		}
		throw LRunTimeError(expr.op, "Operands must be two numbers or two strings");
	case TokenType::GREATER:
		checkNumberOperands(expr.op, left, right);
		return Var_literal(std::get<double>(left) > std::get<double>(right));
	case TokenType::GREATER_EQUAL:
		checkNumberOperands(expr.op, left, right);
		return Var_literal(std::get<double>(left) >= std::get<double>(right));
	case TokenType::LESS:
		checkNumberOperands(expr.op, left, right);
		return Var_literal(std::get<double>(left) < std::get<double>(right));
	case TokenType::LESS_EQUAL:
		checkNumberOperands(expr.op, left, right);
		return Var_literal(std::get<double>(left) <= std::get<double>(right));
	case TokenType::EQUAL_EQUAL:
		return isEqual(left, right);
	case TokenType::BANG_EQUAL:
		return !isEqual(left, right);
	}

	//unreachable
	__assume(false);

}

bool Interpreter::isTruthy(Var_literal v)
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

bool Interpreter::isEqual(Var_literal a, Var_literal b)
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

void Interpreter::checkNumberOperand(Token op, Var_literal operand)
{
	if (std::holds_alternative<double>(operand))
	{
		return;
	}

	throw LRunTimeError(op, "Operand must be a number");
}

void Interpreter::checkNumberOperands(Token op, Var_literal left, Var_literal right)
{
	if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
	{
		return;
	}

	throw LRunTimeError(op, "Operands must be numbers");
}

void Interpreter::checkDivisionByZero(Token op, Var_literal right)
{
	if (std::get<double>(right) == 0.0)
	{
		throw LRunTimeError(op, "Division by zero");
	}

	return;
}
