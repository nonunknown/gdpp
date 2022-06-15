#include <string>
#include <iostream>
#include "token.h"

#ifndef ERROR_H
#define ERROR_H

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
			ParserError(const std::string arg)
			{
				std::cout << "ParserError:" << arg << std::endl;
			}

	};
}
#endif
