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

	// struct KeywordData
	// {
	// 	public:
	// 		int start;
	// 		int length;
	// 		const char* rest;
	// 		TokenType type;
	// 		KeywordData(const char* p_keyword, TokenType p_type, int p_start=1)
	// 		{
	// 			start = p_start;
	// 			length = strlen(p_keyword) - 1;
	// 			rest = p_keyword + 1;
	// 			type = p_type;
	// 		}

	// 		void toString()
	// 		{
	// 			std::cout << "{ start: " << start << ", length: " << length << ", rest: " << rest << ", tokenType: " << TOKEN_NAMES[type] << " }" << std::endl;
	// 		}
	// };


	const std::map<const char*, TokenType> keywordMap = {
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
			// std::map<char,KeywordData> keywordMap;
			std::string* src;
			Data data;
		public:
			Scanner(std::string* src);
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