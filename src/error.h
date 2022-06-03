#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <iostream>
#include "token.h"

namespace GDPP
{
    class Error
    {
        public:
            static bool hadError;
            static void Push(int line, std::string msg);
			static void Push(Token* token, std::string msg);
            static void Report(int line, std::string where, std::string msg);

			
    };

	class ParserError
	{
		public:
			ParserError() {}
	};
}
#endif
