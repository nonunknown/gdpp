#include <string>
#include <vector>
#include <any>
#include <string>

#ifndef TOKEN_H
#define TOKEN_H

namespace GDPP
{

	enum TokenType {
		// Single-character tokens.
		LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
		COMMA, DOT, MINUS, PLUS, SLASH_FORWARD, STAR, HASHTAG, COLON, SEMI,

		// One or two character tokens.
		BANG, BANG_EQUAL,
		EQUAL, EQUAL_EQUAL,
		GREATER, GREATER_EQUAL,
		LESS, LESS_EQUAL, FORWARD_ARROW,

		// Literals.
		IDENTIFIER, STRING, FLOAT, INT,

		// Keywords.
		AND, IN, NOT, AS, CLASS, EXTENDS, IS,
		SIGNAL, AWAIT, CONST, ENUM, STATIC, ELIF, MATCH,
		 ELSE, FALSE, FUNC, FOR, IF, OR,
		PRINT, RETURN, TRUE, VAR, WHILE, VOID,

		PI, TAU, INF, NAN, SELF, BREAKPOINT, TOOL, SUPER,
		BREAK, CONTINUE, PASS, ASSERT, PRELOAD,

		NIL, TK_ERROR, TK_EOF
		};

	const char* const TOKEN_NAMES[68] = { 
		// Single-character tokens.
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
		"COMMA", "DOT", "MINUS", "PLUS", "SLASH_FORWARD", "STAR", "HASHTAG", "COLON", "SEMI",

		// One or two character tokens.
		"BANG", "BANG_EQUAL",
		"EQUAL", "EQUAL_EQUAL",
		"GREATER", "GREATER_EQUAL",
		"LESS", "LESS_EQUAL", "FORWARD_ARROW",

		// Literals.
		"IDENTIFIER", "STRING", "FLOAT", "INT",

		// Keywords.
		"AND", "IN", "NOT", "AS", "CLASS", "EXTENDS", "IS",
		"SIGNAL", "AWAIT", "CONST", "ENUM", "STATIC", "ELIF", "MATCH",
		 "ELSE", "FALSE", "FUNC", "FOR", "IF", "OR",
		"PRINT", "RETURN", "TRUE", "VAR", "WHILE", "VOID",

		"PI", "TAU", "INF", "NAN", "SELF", "BREAKPOINT", "TOOL", "SUPER",
		"BREAK", "CONTINUE", "PASS", "ASSERT", "PRELOAD",

		"NIL", "TK_ERROR", "TK_EOF"
	};
	
    class Token
    {

        public:
            Token(TokenType p_type, std::string p_lexeme, std::string p_literal, int p_line, int p_indentLevel)
			{
				    type = p_type;
					lexeme = p_lexeme;
					literal = p_literal;
					line = p_line;
					indentLevel = p_indentLevel;
			}

            std::string ToString()
			{
				    std::string t = std::string(TOKEN_NAMES[type]);
	
    				return "[Token] -> { type: " + t + ", lexeme: \'" + lexeme + "\', literal: " + literal + " }; - Line: " + std::to_string(line) + " | IndentLevel: " + std::to_string(indentLevel);
			}

            TokenType type;
			std::string literal;
            std::string lexeme;
            int line, indentLevel;

    };
}

#endif
