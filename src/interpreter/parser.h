#include <vector>
#include <iostream>
#include "expression.h"
#include "token.h"
#include "error.h"

#ifndef PARSER_H
#define PARSER_H

namespace GDPP
{
	class Parser
	{
		private:
			std::vector<Token>* tokens;
			int current;

		public:
			Parser(std::vector<Token>* p_tokens);
			Expression Equality();
			Expression Comparision();
			Expression Term();
			Expression Factor();
			Expression Primary();
			Expression Unary();
			Expression Expr();
			
			bool Match(std::vector<TokenType>* p_types);
			bool Match(TokenType p_type);
			bool IsAtEnd();
			bool Check(TokenType p_type);
			Token* Advance();
			Token* Peek();
			Token* Previous();
			Token* Consume(TokenType p_type, std::string msg);
			void Synchronize();
			Expression Parse();


			ParserError Error(Token* err, std::string msg);
			
	};
}

#endif