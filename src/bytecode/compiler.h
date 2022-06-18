#ifndef GD_COMPILER
#define GD_COMPILER


#include <string>
#include "scanner.h"
#include "common.h"
#include "chunk.h"
#include "../base/token.h"

#ifdef DEBUG_PRINT_CODE
	#include "debug.h"
#endif

namespace GDPP
{

	struct Parser
	{
		BToken current;
		BToken previous;
		bool hadError;
		bool panicMode;
	};

	enum Precendence
	{
		PREC_NONE,			// 
		PREC_ASSIGNMENT,	// 	=
		PREC_OR,			// 	or
		PREC_AND,			// 	and
		PREC_EQUALITY,		// 	== !=
		PREC_COMPARISION,	// 	< > <= >=
		PREC_TERM,			// 	+ - 
		PREC_FACTOR,		// 	* /
		PREC_UNARY,			//	! -
		PREC_CALL, 			//	. ()
		PREC_PRIMARY 		//	primary or literals, same thing
	};

	struct ParseRule;

	class Compiler
	{
		public:
			static Parser parser;
			static Scanner scanner;
			static Chunk* compilingChunk;
			static Chunk* currentChunk();
			static bool compile(std::string* src, Chunk* p_chunk);
			static void advance();
			static void consume(TokenType type, const char* message);
			static void errorAtCurrent(const char* message);
			static void error(const char* message);
			static void errorAt(BToken* token, const char* message);
			static void emitByte(uint8_t byte);
			static void emitBytes(uint8_t byte, uint8_t byte2);
			static void endCompiler();
			static void emitReturn();

			static ParseRule* getRule(TokenType p_type);
			static void parsePrecedence(Precendence precedence);

			static void expression();
			static void unary();
			static void binary();

			static void number();
			static void grouping();

			static void emitConstant(int value);
			static uint8_t makeConstant(int value);

	};


	typedef void (*ParseFn)();
	
	struct ParseRule
	{
		ParseFn prefix;
		ParseFn infix;
		Precendence precedence;

	};
	//TODO: Implement remaining tokens
	static const ParseRule parseRules[]
	{
		[LEFT_PAREN]    	= {Compiler::grouping,		NULL,   			PREC_NONE},
		[RIGHT_PAREN]   	= {NULL,     				NULL,   			PREC_NONE},
		[LEFT_BRACE]    	= {NULL,     				NULL,   			PREC_NONE}, 
		[RIGHT_BRACE]   	= {NULL,     				NULL,   			PREC_NONE},
		[COMMA]         	= {NULL,     				NULL,   			PREC_NONE},
		[DOT]           	= {NULL,     				NULL,   			PREC_NONE},
		[MINUS]         	= {Compiler::unary,    		Compiler::binary, 	PREC_TERM},
		[PLUS]          	= {NULL,     				Compiler::binary, 	PREC_TERM},
		[SEMI]     			= {NULL,     				NULL,   			PREC_NONE},
		[COLON]     		= {NULL,     				NULL,   			PREC_NONE},
		[SLASH_FORWARD]     = {NULL,     				Compiler::binary, 	PREC_FACTOR},
		[STAR]          	= {NULL,     				Compiler::binary, 	PREC_FACTOR},
		[BANG]          	= {NULL,     				NULL,   			PREC_NONE},
		[BANG_EQUAL]    	= {NULL,     				NULL,   			PREC_NONE},
		[EQUAL]         	= {NULL,     				NULL,   			PREC_NONE},
		[EQUAL_EQUAL]   	= {NULL,     				NULL,   			PREC_NONE},
		[GREATER]       	= {NULL,     				NULL,   			PREC_NONE},
		[GREATER_EQUAL] 	= {NULL,     				NULL,   			PREC_NONE},
		[LESS]          	= {NULL,     				NULL,   			PREC_NONE},
		[LESS_EQUAL]    	= {NULL,     				NULL,   			PREC_NONE},
		[FORWARD_ARROW]		= {NULL,	 				NULL,				PREC_NONE},
		[IDENTIFIER]    	= {NULL,     				NULL,   			PREC_NONE},
		[STRING]        	= {NULL,     				NULL,   			PREC_NONE},
		[INT]        		= {Compiler::number,   		NULL,   			PREC_NONE},
		[FLOAT]        		= {Compiler::number,   		NULL,   			PREC_NONE},
		[AND]           	= {NULL,     				NULL,   			PREC_NONE},
		[CLASS]         	= {NULL,     				NULL,   			PREC_NONE},
		[ELSE]          	= {NULL,     				NULL,   			PREC_NONE},
		[FALSE]         	= {NULL,     				NULL,   			PREC_NONE},
		[FOR]           	= {NULL,     				NULL,   			PREC_NONE},
		[FUNC]           	= {NULL,     				NULL,   			PREC_NONE},
		[IF]            	= {NULL,     				NULL,   			PREC_NONE},
		[NIL]           	= {NULL,     				NULL,   			PREC_NONE},
		[OR]            	= {NULL,     				NULL,   			PREC_NONE},
		[PRINT]         	= {NULL,     				NULL,   			PREC_NONE},
		[RETURN]        	= {NULL,     				NULL,   			PREC_NONE},
		[SUPER]         	= {NULL,     				NULL,   			PREC_NONE},
		[SELF]          	= {NULL,     				NULL,   			PREC_NONE},
		[TRUE]          	= {NULL,     				NULL,   			PREC_NONE},
		[VAR]           	= {NULL,     				NULL,   			PREC_NONE},
		[WHILE]        		= {NULL,     				NULL,   			PREC_NONE},
		[TK_ERROR]         	= {NULL,     				NULL,   			PREC_NONE},
		[TK_EOF]           	= {NULL,     				NULL,   			PREC_NONE},
	};
} // namespace GDPP


#endif