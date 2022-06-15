
#include "compiler.h"

using namespace GDPP;

void Compiler::compile(std::string* p_src)
{
	Scanner scanner = Scanner(p_src);
	int line = -1;
	for (;;)
	{
		BToken token = scanner.scanToken();
		if (token.line != line)
		{
			printf("%4d", token.line);
			line = token.line;

		} 
		else
		{
			printf("	| ");

		}

		printf("%s '%.*s'\n", TOKEN_NAMES[token.type], token.length, token.start);

		if (token.type == TK_EOF) break;
	}
}