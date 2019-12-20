#pragma once
#include <variant>
#include <string>
#include "Expr.h"
typedef std::variant<std::string, double, bool, std::nullptr_t> varLiteral;

class IStmtVisitor
{
public:
	virtual varLiteral visit(BlockStmt& stmt) = 0;
	virtual varLiteral visit(ClassStmt& expr) = 0;
	virtual varLiteral visit(ExpressionStmt& expr) = 0;
	virtual varLiteral visit(FunctionStmt& expr) = 0;
	virtual varLiteral visit(IfStmt& expr) = 0;
	virtual varLiteral visit(PrintStmt& expr) = 0;
	virtual varLiteral visit(ReturnStmt& expr) = 0;
	virtual varLiteral visit(VarStmt& expr) = 0;
	virtual varLiteral visit(WhileStmt& expr) = 0;
};

class Stmt
{
public:
	virtual varLiteral accept(IStmtVisitor& visitor) = 0;
};


class ExpressionStmt : public Stmt
{
public:
	const Expr* expression;

	ExpressionStmt(Expr& expression) : expression(&expression) {};

	varLiteral accept(IStmtVisitor& visitor) 
	{
		return visitor.visit(*this);
	};
};

class PrintStmt : public Stmt
{
public:
	const Expr* expression;
	PrintStmt(Expr& expr) : expression(&expr) {}//change to pointers or whatever

	varLiteral accept(IStmtVisitor& visitor)
	{
		return visitor.visit(*this);
	};
};
