#ifndef GD_TOKEN
#define GD_TOKEN

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
} // namespace GDPP



#endif