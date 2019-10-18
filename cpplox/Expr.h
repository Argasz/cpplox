#include "Token.h"
#include <vector>
#include <variant>
#define VARIANT std::variant<std::string, double>
class Expr
{
};
struct ExprVisitor
{
void operator()(Binary&);
void operator()(Grouping&);
void operator()(Literal&);
void operator()(Unary&);

};

class Binary : public Expr
{
	Binary(Expr left,Token op,Expr right);
	Expr left;
	Token op;
	Expr right;
 };
class Grouping : public Expr
{
	Grouping(Expr expression);
	Expr expression;
 };
class Literal : public Expr
{
	Literal(VARIANT value);
	VARIANT value;
 };
class Unary : public Expr
{
	Unary(Token op,Expr right);
	Token op;
	Expr right;
 };