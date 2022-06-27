#ifndef GD_SCANNER
#define GD_SCANNER

#include <string.h>
#include <map>
#include <iostream>

#include "common.h"
#include "../base/token.h"

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

	struct BToken // the name is due to Token used in the include, stands for Token Bytecode
	{
		TokenType type;
		const char* start;
		int length;
		int line;
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
			BToken scanToken();
			BToken errorToken(std::string* message);
			BToken makeToken(TokenType t);
			BToken string();
			BToken number();
			BToken identifier();
			bool isAtEnd();
			bool match(char expected);
			char advance();
			char peek();
			char peekNext();
			void skipWhitespace();
			TokenType checkKeyword(int start, int length, const char* rest, TokenType type);

	};
} // namespace GDPP



#endif