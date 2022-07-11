#ifndef GD_TOKEN
#define GD_TOKEN

// TODO(fusion): This is a C macro that may be defined in math.h "if and only if
// the implementation supports quiet NaNs for the float type" (C99 spec, section
// 7.12 paragraph #5).
#ifdef NAN
#	undef NAN
#endif

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
		IDENTIFIER, STRING, FLOAT, INT, BOOL,

		// Keywords.
		AND, IN, NOT, AS, CLASS, EXTENDS, IS,
		SIGNAL, AWAIT, CONST, ENUM, STATIC, ELIF, MATCH,
		 ELSE, FALSE, FUNC, FOR, IF, OR,
		PRINT, RETURN, TRUE, VAR, WHILE, VOID, EXIT,

		PI, TAU, INF, NAN, SELF, BREAKPOINT, TOOL, SUPER,
		BREAK, CONTINUE, PASS, ASSERT, PRELOAD,

		NIL, TK_ERROR, TK_EOF
		};

	const char* const TOKEN_NAMES[70] = { 
		// Single-character tokens.
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
		"COMMA", "DOT", "MINUS", "PLUS", "SLASH_FORWARD", "STAR", "HASHTAG", "COLON", "SEMI",

		// One or two character tokens.
		"BANG", "BANG_EQUAL",
		"EQUAL", "EQUAL_EQUAL",
		"GREATER", "GREATER_EQUAL",
		"LESS", "LESS_EQUAL", "FORWARD_ARROW",

		// Literals.
		"IDENTIFIER", "STRING", "FLOAT", "INT","BOOL",

		// Keywords.
		"AND", "IN", "NOT", "AS", "CLASS", "EXTENDS", "IS",
		"SIGNAL", "AWAIT", "CONST", "ENUM", "STATIC", "ELIF", "MATCH",
		 "ELSE", "FALSE", "FUNC", "FOR", "IF", "OR",
		"PRINT", "RETURN", "TRUE", "VAR", "WHILE", "VOID", "EXIT",

		"PI", "TAU", "INF", "NAN", "SELF", "BREAKPOINT", "TOOL", "SUPER",
		"BREAK", "CONTINUE", "PASS", "ASSERT", "PRELOAD",

		"NIL", "TK_ERROR", "TK_EOF"
	};

	struct Token
	{
		TokenType type;
		const char* start;
		int length;
		int line;
	};
	
}; // namespace GDPP



#endif