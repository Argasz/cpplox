#include "Expr.h"
#include <ostream>
#include "cpplox.cpp"
Binary::Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) : left(std::move(left)), op(op), right(std::move(right)){};
Grouping::Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression)){};
Literal::Literal(Var_literal  value) : value(value){};
Unary::Unary(Token op, std::unique_ptr<Expr> right) : op(op), right(std::move(right)){};

std::ostream & operator<<(std::ostream & stream, const Literal & lit)
{
	unpack_write_to_stream(stream, lit.value);

	return stream;
}
