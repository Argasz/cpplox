#include "Expr.h"
Binary::Binary(Expr& left,Token op,Expr& right) : left(left), op(op), right(right){};
Grouping::Grouping(Expr& expression) : expression(expression){};
Literal::Literal(varLiteral  value) : value(value){};
Unary::Unary(Token op,Expr& right) : op(op), right(right){};

std::ostream & operator<<(std::ostream & stream, const Literal & lit)
{
	auto visitorLambda = [&stream](auto&& _in) { stream << " " << _in << " "; };
	std::visit(visitorLambda, lit.value);

	return stream;
}
