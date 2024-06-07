#pragma once
#include <string>
#include "Expr.h"

class AstPrinter : public IExprVisitor
{
public:
	AstPrinter() {};
	virtual Var_literal visit(Binary&);
	virtual Var_literal visit(Grouping&);
	virtual Var_literal visit(Literal&);
	virtual Var_literal visit(Unary&);
private:
	std::string parenthesize(std::string& name, std::vector<Expr*> exprs);
};