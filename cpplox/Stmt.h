#pragma once
#include <variant>
#include <string>
#include "Expr.h"
typedef std::variant<std::string, double, bool, std::nullptr_t, std::monostate> Var_literal;

class IStmtVisitor
{
public:
	//virtual Var_literal visit(BlockStmt& stmt) = 0;
	//virtual Var_literal visit(ClassStmt& expr) = 0;
	virtual Var_literal visit(ExpressionStmt& expr) = 0;
	//virtual Var_literal visit(FunctionStmt& expr) = 0;
	//virtual Var_literal visit(IfStmt& expr) = 0;
	virtual Var_literal visit(PrintStmt& expr) = 0;
	//virtual Var_literal visit(ReturnStmt& expr) = 0;
	//virtual Var_literal visit(VarStmt& expr) = 0;
	//virtual Var_literal visit(WhileStmt& expr) = 0;
};

class Stmt
{
public:
	virtual Var_literal accept(IStmtVisitor& visitor) = 0;
};


class ExpressionStmt : public Stmt
{
public:
	const Expr* expression;

	ExpressionStmt(Expr& expression) : expression(&expression) {};

	Var_literal accept(IStmtVisitor& visitor) 
	{
		return visitor.visit(*this);
	};
};

class PrintStmt : public Stmt
{
public:
	const Expr* expression;
	PrintStmt(Expr& expr) : expression(&expr) {}//change to pointers or whatever

	Var_literal accept(IStmtVisitor& visitor)
	{
		return visitor.visit(*this);
	};
};
