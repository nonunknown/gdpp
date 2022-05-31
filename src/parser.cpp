#include "parser.h"

using namespace GDPP;

Parser::Parser(std::vector<Token*> p_tokens)
{
	current = 0;
	tokens = p_tokens;
}

//EXPRESSIONS ===========

// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
Expression* Parser::Comparision()
{
	Expression* expr = Term();
	std::vector<Token::TokenType> types = {Token::TokenType::GREATER, Token::TokenType::GREATER_EQUAL, Token::TokenType::LESS, Token::TokenType::LESS_EQUAL};
	while( Match(&types) )
	{
		Token* op = Previous();
		Expression* right = Term();
		expr = &Binary(expr, op, right);
	}
	return expr;
}

//equality       → comparison ( ( "!=" | "==" ) comparison )* ;
Expression* Parser::Equality()
{
	Expression* expr = Comparision();
	std::vector<Token::TokenType> types = {Token::TokenType::BANG_EQUAL, Token::TokenType::EQUAL_EQUAL};
	while( Match(&types) )
	{
		Token* op = Previous();
		Expression* right = Comparision();
		expr = &Binary(expr, op, right);

	}
	return expr;
}

//term       → factor ( ( "-" | "+" ) factor )* ;
Expression* Parser::Term()
{
	Expression* expr = Factor();
	std::vector<Token::TokenType> types = {Token::TokenType::MINUS, Token::TokenType::PLUS};
	while(Match(&types))
	{
		Token* op = Previous();
		Expression* right = Factor();
		expr = &Binary(expr, op, right);
	}

	return expr;
}

//factor         → unary ( ( "/" | "*" ) unary )* ;
Expression* Parser::Factor()
{
	Expression* expr = Parser::Unary();
	std::vector<Token::TokenType> types = {Token::TokenType::SLASH_FORWARD, Token::TokenType::STAR};
	while(Match(&types))
	{
		Token* op = Previous();
		Expression* right = Parser::Unary();
		expr = &Binary(expr, op, right);
	}

	return expr;
}

//unary          → ( "!" | "-" ) unary | primary ;

Expression* Parser::Unary()
{
	std::vector<Token::TokenType> types = {Token::TokenType::BANG, Token::TokenType::MINUS};
	if (Match(&types))
	{
		Token* op = Previous();
		Expression* right = Parser::Unary();
		return &GDPP::Unary(op, right);
	}

	return Primary();
}

//primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
Expression* Parser::Primary()
{
	if (Match(Token::TokenType::FALSE))
	{
		return &Literal(false);
	}
	if (Match(Token::TokenType::TRUE))
	{
		return &Literal(true);
	}

	std::vector<Token::TokenType> types = {Token::TokenType::NUMBER, Token::TokenType::STRING};

	if (Match(&types))
	{
		return &Literal(Previous()->literal);
	}

	if (Match(Token::TokenType::LEFT_PAREN))
	{
		Expression* expr = Expr();
		Consume(Token::TokenType::RIGHT_PAREN, "Expect ')' after expression");
		return &Grouping(expr);
	}


}

Expression* Parser::Expr()
{
	return Equality();
}

// END EXPRESSIONS ==================


Token* Parser::Consume(Token::TokenType p_type, std::string msg)
{
	if (Check(p_type)) return Advance();
	// throw Error(Peek(), msg);
	//TODO: Implement error handling
}


bool Parser::Match(std::vector<Token::TokenType>* p_types)
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

bool Parser::Match(Token::TokenType p_type)
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
	return tokens[current];
}

Token* Parser::Previous()
{
	return tokens[current-1];
}

bool Parser::IsAtEnd()
{
	return Peek()->type == Token::TokenType::TK_EOF;
}

bool Parser::Check(Token::TokenType p_type)
{
	if (IsAtEnd())
	{
		return false;
	}

	return Peek()->type == p_type;
}