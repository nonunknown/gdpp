#include <vector>
#include "token.h"

namespace GDPP
{
	
    struct Binary;
    struct Assign;
    struct Call;
    struct Get;
    struct Grouping;
    struct Literal;
    struct Logical;
    struct Set;
    struct Self;
    struct Unary;
    struct Variable;

    struct Visitor
    {
        public:
            virtual void Visit(Assign&) = 0;
            virtual void Visit(Binary&) = 0;
            virtual void Visit(Call&) = 0;
            virtual void Visit(Get&) = 0;
            virtual void Visit(Grouping&) = 0;
            virtual void Visit(Literal&) = 0;
            virtual void Visit(Logical&) = 0;
            virtual void Visit(Set&) = 0;
            virtual void Visit(Self&) = 0;
            virtual void Visit(Unary&) = 0;
            virtual void Visit(Variable&) = 0;
    };

    struct Expression
    {
        virtual void Accept(Visitor& visitor) = 0;
    };

    struct Assign : Expression
    {
        public:
            Token* name;
            Expression* value;
            Assign(Token* p_name, Expression* p_value);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }
    };

    struct Binary : Expression
    {
        public:
            Expression* left;
            Expression* right;
            Token* opt;
            Binary(Expression* p_left, Token* p_opt, Expression* p_right);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Call : Expression
    {
        public:
            Expression* calle;
            Token* parent;
            std::vector<Expression*> args;
            Call(Expression* p_calle, Token* p_parent, std::vector<Expression*> p_args);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Get : Expression
    {
        public:
            Expression* object;
            Token* name;
            Get(Expression* p_object, Token* p_name);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Grouping : Expression
    {
        public:
            Expression* expression;
            Grouping(Expression* p_expression);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };
    
    struct Literal : Expression
    {
        public:
            std::string* value;
            Literal(std::string* p_value);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Logical : Expression
    {
        public:
            Expression* left, *right;
            Token* op;
            Logical(Expression* p_left, Token* p_op, Expression* p_right);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Set : Expression
    {
        public:
            Expression* object, *value;
            Token* name;
            Set(Expression* p_object, Token* p_name, Expression* p_value);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Self : Expression
    {
        public:
            Token* keyword;
            Self(Token* p_keyword);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Unary : Expression
    {
        public:
            Token* token;
            Expression* right;
            Unary(Token* p_token, Expression* p_right);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };

    struct Variable : Expression
    {
        public:
            Token* name;
            Variable(Token* p_name);
            void Accept(Visitor& visitor) final { visitor.Visit(*this); }

    };


    
}