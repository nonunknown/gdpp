#ifndef GD_SCANNER
#define GD_SCANNER

#include <string.h>
#include <map>
#include <iostream>

#include "token.h"

namespace GDPP
{
	struct Data
	{
		const char* start;
		const char* current;
		int numStart;
		int numCurrent;
		int line;
		int tabLevel;
	};




	const std::map<const std::string, TokenType> keywordMap = {
		{"and",			AND},
		{"as",			AS},
		{"assert",		ASSERT},
		{"await",		AWAIT},
		{"breakpoint",	BREAKPOINT},
		{"break",		BREAK},
		{"class",		CLASS},
		{"const",		CONST},
		{"continue",	CONTINUE},
		{"extends",		EXTENDS},
		{"elif",		ELIF},
		{"else",		ELSE},
		{"enum",		ENUM},
		{"false",		FALSE},
		{"for",			FOR},
		{"func",		FUNC},
		{"is",			IS},
		{"in",			IN},
		{"if",			IF},
		{"INF",			INF},
		{"match",		MATCH},
		{"NAN",			NAN},
		{"null",		NIL},
		{"or",			OR},
		{"pass",		PASS},
		{"PI",			PI},
		{"preload",		PRELOAD},
		{"print",		PRINT},
		{"return",		RETURN},
		{"signal",		SIGNAL},
		{"self",		SELF},
		{"static",		STATIC},
		{"super",		SUPER},
		{"TAU",			TAU},
		{"true",		TRUE},
		{"tool",		TOOL},
		{"var",			VAR},
		{"void",		VOID},
		{"while",		WHILE},
	};

	class Scanner
	{
		private:
			std::string* src;
			Data data;
		public:
			Scanner();
			~Scanner();
			void start(std::string* p_src);
			Token scan_token();
			Token error_token(std::string* message);
			Token make_token(TokenType t);
			Token string();
			Token number();
			Token identifier();
			bool is_at_end();
			bool match(char expected);
			char advance();
			char peek();
			char peek_next();
			void skip_whitespace();
			TokenType check_keyword(int start, int length, const char* rest, TokenType type);

	};
}; // namespace GDPP



#endif