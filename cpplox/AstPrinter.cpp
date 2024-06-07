#include "AstPrinter.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>

Var_literal AstPrinter::visit(Binary& expr)
{
	std::string s = expr.op.lexeme;
	return parenthesize(s, { &expr.left,&expr.right });
}

Var_literal AstPrinter::visit(Grouping& expr)
{
	std::string grp = "group";
	return parenthesize(grp, { &expr.expression });
}

Var_literal AstPrinter::visit(Literal& expr)
{
	return expr.value;
}

Var_literal AstPrinter::visit(Unary& expr)
{
	return parenthesize(expr.op.lexeme, { &expr.right });
}

std::string AstPrinter::parenthesize(std::string& name, std::vector<Expr*> exprs)
{
	std::ostringstream result;
	result << "(";
	result << name << " ";

	std::transform(begin(exprs), end(exprs), std::ostream_iterator<std::string>(result, " "),
		[this](Expr* e) -> std::string
		{
			std::ostringstream ret;
			ret << e->accept(*this);
			return ret.str();
		}
	);

	result << ")";

	return result.str();
}

