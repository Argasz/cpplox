#pragma once
#include <string>
#include "Expr.h"

class AstPrinter : public IExprVisitor
{
public:
	AstPrinter() {};
	virtual varLiteral visit(Binary&);
	virtual varLiteral visit(Grouping&);
	virtual varLiteral visit(Literal&);
	virtual varLiteral visit(Unary&);
private:
	std::string parenthesize(std::string& name, std::vector<Expr*> exprs);
};