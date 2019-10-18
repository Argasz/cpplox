#include "Expr.h"
Binary::Binary(Expr left,Token op,Expr right) : left(left), op(op), right(right)
{};
Grouping::Grouping(Expr expression) : expression(expression)
{};
Literal::Literal(VARIANT value) : value(value)
{};
Unary::Unary(Token op,Expr right) : op(op), right(right)
{};
