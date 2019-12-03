#include "AstPrinter.h"
#include <iostream>
#include <string>

void AstPrinter::visit(Binary& expr)
{
	std::string s = expr.op.lexeme;
	parenthesize(s, { &expr.left,&expr.right });
}

void AstPrinter::visit(Grouping& expr)
{
	std::string grp = "group";
	parenthesize(grp, {&expr.expression});
}

void AstPrinter::visit(Literal& expr)
{
	os << expr.value;
}

void AstPrinter::visit(Unary & expr)
{
	parenthesize(expr.op.lexeme, { &expr.right });
}

void AstPrinter::parenthesize(std::string & name, std::vector<Expr*> exprs)
{
	os << "(";
	os << name;
	for (auto expr : exprs)
	{
		os << " ";
		expr->accept(*this);
	}
	os << ")";
}

