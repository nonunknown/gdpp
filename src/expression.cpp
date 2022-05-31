#include "expression.h"

using namespace GDPP;

Binary::Binary(Expression* p_left, Token* p_opt, Expression* p_right)
{
    left = p_left;
    opt = p_opt;
    right = p_right;
}

Assign::Assign(Token* p_name, Expression* p_value)
{
    name = p_name;
    value = p_value;
}

Call::Call(Expression* p_calle, Token* p_parent, std::vector<Expression*> p_args)
{
    calle = p_calle;
    parent = p_parent;
    args = p_args;
}

Get::Get(Expression* p_object, Token* p_name)
{
    object = p_object;
    name = p_name;
}

Grouping::Grouping(Expression* p_expression)
{
    expression = p_expression;
}

Literal::Literal(variant p_value)
{
    value = p_value;
}

Logical::Logical(Expression* p_left, Token* p_op, Expression* p_right)
{
    left = p_left;
    op = p_op;
    right = p_right;
}

Set::Set(Expression* p_object, Token* p_name, Expression* p_value)
{
    object = p_object;
    name = p_name;
    value = p_value;
}

Self::Self(Token* p_keyword)
{
    keyword = p_keyword;
}

Unary::Unary(Token* p_token, Expression* p_right)
{
    token = p_token;
    right = p_right;
}

Variable::Variable(Token* p_name)
{
    name = p_name;
}
