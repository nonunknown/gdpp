#ifndef GD_COMPILER
#define GD_COMPILER


#include <string>
#include "scanner.h"
#include "common.h"


namespace GDPP
{
	class Compiler
	{
		static void compile(std::string* src);
	};
} // namespace GDPP


#endif