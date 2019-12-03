#pragma once
#include "Token.h"
#include <vector>
#include <variant>

typedef std::variant<std::string, double, bool, std::nullptr_t> varLiteral;

class Binary;
class Grouping;
class Literal;
class Unary;

class IExprVisitor
{
public:
	virtual void visit(Binary& expr) = 0;
	virtual void visit(Grouping& expr) = 0;
	virtual void visit(Literal& expr) = 0;
	virtual void visit(Unary& expr) = 0;

};


class Expr
{
public:
	virtual void accept(IExprVisitor& visitor) = 0;
};

class Binary : public Expr
{
public:
	Binary(Expr& left,Token op,Expr& right);
	Expr& left;
	Token op;
	Expr& right;
	void accept(IExprVisitor& visitor) { visitor.visit(*this); };
 };
class Grouping : public Expr
{
public:
	Grouping(Expr& expression);
	Expr& expression;
	void accept(IExprVisitor& visitor) { visitor.visit(*this); };
 };
class Literal : public Expr
{
public:
	Literal(varLiteral value);
	varLiteral value;
	void accept(IExprVisitor& visitor) { visitor.visit(*this); };
	friend std::ostream& operator << (std::ostream& stream, const Literal& lit);
 };
class Unary : public Expr
{
public:
	Unary(Token op,Expr& right);
	Token op;
	Expr& right;
	void accept(IExprVisitor& visitor) { visitor.visit(*this); }
 };



std::ostream& operator << (std::ostream& stream, const Literal& lit);