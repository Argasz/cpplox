#include "AstPrinter.h"
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>

varLiteral AstPrinter::visit(Binary& expr)
{
	std::string s = expr.op.lexeme;
	return parenthesize(s, { &expr.left,&expr.right });
}

varLiteral AstPrinter::visit(Grouping& expr)
{
	std::string grp = "group";
	return parenthesize(grp, {&expr.expression});
}

varLiteral AstPrinter::visit(Literal& expr)
{
	return expr.value;
}

varLiteral AstPrinter::visit(Unary & expr)
{
	return parenthesize(expr.op.lexeme, { &expr.right });
}

std::string AstPrinter::parenthesize(std::string & name, std::vector<Expr*> exprs)
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

