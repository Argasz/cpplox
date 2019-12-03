#pragma once
#include <string>
#include "Expr.h"

class AstPrinter : public IExprVisitor
{
public:
	AstPrinter(std::ostream& ostream) : os(ostream) {};
	virtual void visit(Binary&);
	virtual void visit(Grouping&);
	virtual void visit(Literal&);
	virtual void visit(Unary&);
private:
	std::ostream& os;
	void parenthesize(std::string& name, std::vector<Expr*> exprs);
};