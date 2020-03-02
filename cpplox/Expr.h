#pragma once
#include "Token.h"
#include <vector>
#include <variant>

typedef std::variant<std::string, double, bool, std::nullptr_t, std::monostate> varLiteral;

class Binary;
class Grouping;
class Literal;
class Unary;

class IExprVisitor
{
public:
	virtual varLiteral visit(Binary& expr) = 0;
	virtual varLiteral visit(Grouping& expr) = 0;
	virtual varLiteral visit(Literal& expr) = 0;
	virtual varLiteral visit(Unary& expr) = 0;

};


class Expr
{
public:
	virtual varLiteral accept(IExprVisitor& visitor) = 0;
};

class Binary : public Expr
{
public:
	Binary(Expr& left,Token op,Expr& right);
	Expr& left;
	Token op;
	Expr& right;
	varLiteral accept(IExprVisitor& visitor) { return visitor.visit(*this); };
 };
class Grouping : public Expr
{
public:
	Grouping(Expr& expression);
	Expr& expression;
	varLiteral accept(IExprVisitor& visitor) { return visitor.visit(*this); };
 };
class Literal : public Expr
{
public:
	Literal(varLiteral value);
	varLiteral value;
	varLiteral accept(IExprVisitor& visitor) { return visitor.visit(*this); };
	friend std::ostream& operator << (std::ostream& stream, const Literal& lit);
 };
class Unary : public Expr
{
public:
	Unary(Token op,Expr& right);
	Token op;
	Expr& right;
	varLiteral accept(IExprVisitor& visitor) { return visitor.visit(*this); }
 };



std::ostream& operator << (std::ostream& stream, const Literal& lit);