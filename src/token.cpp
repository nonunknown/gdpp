#include "token.h"

using namespace GDPP;


std::string Token::ToString()
{
    std::string t = std::string(TOKEN_NAMES[type]);
	
    return "[Token] -> { type: " + t + ", lexeme: \'" + lexeme + "\', literal: " + literal + " }; - Line: " + std::to_string(line) + " | IndentLevel: " + std::to_string(indentLevel);
}

Token::Token(TokenType p_type, std::string p_lexeme, std::string p_literal, int p_line, int p_indentLevel)
{
    type = p_type;
    lexeme = p_lexeme;
    literal = p_literal;
    line = p_line;
    indentLevel = p_indentLevel;
}

Token::~Token()
{

}