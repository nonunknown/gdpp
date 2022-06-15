#ifndef GD_SCANNER
#define GD_SCANNER

#include <string>
#include "common.h"
#include "../base/token.h"

namespace GDPP
{
	typedef struct Data
	{
		const char* start;
		const char* current;
		int line;
		int tabLevel;
	};

	typedef struct BToken // the name is due to Token used in the include, stands for Token Bytecode
	{
		TokenType type;
		const char* start;
		int length;
		int line;
	};
	

	class Scanner
	{
		private:
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


	};
} // namespace GDPP



#endif