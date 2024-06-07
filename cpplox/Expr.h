#pragma once
#include "Token.h"
#include <vector>

typedef std::variant<std::string, double, bool, std::nullptr_t, std::monostate> Var_literal;

class Binary;
class Grouping;
class Literal;
class Unary;

class IExprVisitor
{
public:
	virtual Var_literal visit(Binary& expr) = 0;
	virtual Var_literal visit(Grouping& expr) = 0;
	virtual Var_literal visit(Literal& expr) = 0;
	virtual Var_literal visit(Unary& expr) = 0;
	virtual Var_literal visit(Variable& expr) = 0;

};

class Expr
{
public:
	virtual Var_literal accept(IExprVisitor& visitor) = 0;
};

class Binary : public Expr
{
public:
	Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right);
	std::unique_ptr<Expr> left;
	Token op;
	std::unique_ptr<Expr> right;
	Var_literal accept(IExprVisitor& visitor) { return visitor.visit(*this); };
};
class Grouping : public Expr
{
public:
	Grouping(std::unique_ptr<Expr> expression);
	std::unique_ptr<Expr> expression;
	Var_literal accept(IExprVisitor& visitor) { return visitor.visit(*this); };
};
class Literal : public Expr
{
public:
	Literal(Var_literal value);
	Var_literal value;
	Var_literal accept(IExprVisitor& visitor) { return visitor.visit(*this); };
	friend std::ostream& operator << (std::ostream& stream, const Literal& lit);
};
class Unary : public Expr
{
public:
	Unary(Token op, std::unique_ptr<Expr> right);
	Token op;
	std::unique_ptr<Expr> right;
	Var_literal accept(IExprVisitor& visitor) { return visitor.visit(*this); }
};

class Variable : public Expr
{
public:
	Variable(Token name) : name(name) {};
	const Token name;
	Var_literal accept(IExprVisitor& visitor) { return visitor.visit(*this); }
};

std::ostream& operator << (std::ostream& stream, const Literal& lit);