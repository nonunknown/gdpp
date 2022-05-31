#include <vector>
#include "expression.h"

namespace GDPP
{
	class Parser
	{
		private:
			std::vector<Token*> tokens;
			int current;

		public:
			Parser(std::vector<Token*> p_tokens);
			Expression* Equality();
			Expression* Comparision();
			Expression* Term();
			Expression* Factor();
			Expression* Primary();
			Expression* Unary();
			Expression* Expr();
			
			bool Match(std::vector<Token::TokenType>* p_types);
			bool Match(Token::TokenType p_type);
			Token* Advance();
			Token* Peek();
			Token* Previous();
			Token* Consume(Token::TokenType p_type, std::string msg);
			bool IsAtEnd();
			bool Check(Token::TokenType p_type);
			
	};
}