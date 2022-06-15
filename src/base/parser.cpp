#include "parser.h"

using namespace GDPP;

Parser::Parser(std::vector<Token>* p_tokens)
{
	current = 0;
	tokens = p_tokens;
}

//EXPRESSIONS ===========

// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
Expression Parser::Comparision()
{
	Expression expr = Term();
	std::vector<TokenType> types = {TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL};
	while( Match(&types) )
	{
		Token* op = Previous();
		Expression right = Term();
		expr = Binary(&expr, op, &right);
	}
	return expr;
}

//equality       → comparison ( ( "!=" | "==" ) comparison )* ;
Expression Parser::Equality()
{
	Expression expr = Comparision();
	std::vector<TokenType> types = {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL};
	while( Match(&types) )
	{
		Token* op = Previous();
		Expression right = Comparision();
		expr = Binary(&expr, op, &right);

	}
	return expr;
}

//term       → factor ( ( "-" | "+" ) factor )* ;
Expression Parser::Term()
{
	Expression expr = Factor();
	std::vector<TokenType> types = {TokenType::MINUS, TokenType::PLUS};
	while(Match(&types))
	{
		Token* op = Previous();
		Expression right = Factor();
		expr = Binary(&expr, op, &right);
	}

	return expr;
}

//factor         → unary ( ( "/" | "*" ) unary )* ;
Expression Parser::Factor()
{
	Expression expr = Unary();
	std::vector<TokenType> types = {TokenType::SLASH_FORWARD, TokenType::STAR};
	while(Match(&types))
	{
		Token* op = Previous();
		Expression right = Parser::Unary();
		expr = Binary(&expr, op, &right);
	}

	return expr;
}

//unary          → ( "!" | "-" ) unary | primary ;

Expression Parser::Unary()
{
	std::vector<TokenType> types = {TokenType::BANG, TokenType::MINUS};
	if (Match(&types))
	{
		Token* op = Previous();
		Expression right = Parser::Unary();
		return GDPP::Unary(op, &right);
	}

	return Primary();
}

//primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
Expression Parser::Primary()
{
	if (Match(TokenType::FALSE))
	{
		return Literal(false);
	}
	if (Match(TokenType::TRUE))
	{
		return Literal(true);
	}

	std::vector<TokenType> types = {TokenType::INT, TokenType::FLOAT, TokenType::STRING};

	if (Match(&types))
	{
		return Literal(Previous()->literal);
	}

	if (Match(TokenType::LEFT_PAREN))
	{
		Expression expr = Expr();
		Consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
		return Grouping(&expr);
	}

	Error::Push(Peek(), "Expected expression");
	return Expr();
}

Expression Parser::Expr()
{
	return Equality();
}

// END EXPRESSIONS ==================


Token* Parser::Consume(TokenType p_type, std::string msg)
{
	if (Check(p_type)) return Advance();
	throw Error(Peek(), msg);
	//TODO: Implement error handling
}


bool Parser::Match(std::vector<TokenType>* p_types)
{
	for(auto &type : *p_types)
	{
		if (Check(type))
		{
			Advance();
			return true;
		}
	}
	return false;
}

bool Parser::Match(TokenType p_type)
{
	if (Check(p_type))
	{
		Advance();
		return true;
	}

	return false;
}

Token* Parser::Advance()
{
	if (!IsAtEnd())
	{
		current++;
	}
	return Previous();
}

Token* Parser::Peek()
{
	return &tokens->at(current);
}

Token* Parser::Previous()
{
	return &tokens->at(current-1);
}

bool Parser::IsAtEnd()
{
	return Peek()->type == TokenType::TK_EOF;
}

bool Parser::Check(TokenType p_type)
{
	if (IsAtEnd())
	{
		return false;
	}

	return Peek()->type == p_type;
}

ParserError Parser::Error(Token* err, std::string msg)
{
	Error::Push(err, msg);
	return ParserError(msg);
}

void Parser::Synchronize()
{
	Advance();

	while (!IsAtEnd())
	{
		// if (Previous()->type == TokenType::)
		switch(Peek()->type)
		{
			case CLASS:
			case FUNC:
			case VAR:
			case FOR:
			case IF:
			case WHILE:
			case PRINT:
			case RETURN:
				return;
		}

		Advance();
	}
}

Expression Parser::Parse()
{
	try
	{
		return Expr();
	}
	catch(const ParserError& e)
	{
		std::cerr << "error parsing" << '\n';
		return Expression();
	}
	
}