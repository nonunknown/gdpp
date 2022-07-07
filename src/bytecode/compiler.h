#ifndef GD_COMPILER
#define GD_COMPILER


#include <string>
#include <unordered_map>
#include <functional>

#include "token.h"
#include "scanner.h"
#include "chunk.h"
#include "common.h"
// #include "object.h"

#ifdef DEBUG_PRINT_CODE
	#include "debug.h"
#endif

namespace GDPP
{

	struct Parser
	{
		Token current;
		Token previous;
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
		//TODO: separate public from private vars
		public:
			Compiler();
			Parser parser;
			Scanner scanner;
			Chunk* compilingChunk;
			Chunk* current_chunk();
			bool compile(std::string* src, Chunk* p_chunk);
			void advance();
			void consume(TokenType type, const char* message);
			void error_at_current(const char* message);
			void error(const char* message);
			void error_at(Token* token, const char* message);
			void emit_byte(uint8_t byte);
			void emit_bytes(uint8_t byte, uint8_t byte2);
			void end_compiler();
			void emit_return();
 			const ParseRule* get_rule(TokenType p_type);
			void parse_precedence(Precendence precedence);
 			void expression();
			 //Those functions below are static, in order to user prets parser table -> look at parseRule variable
			static void unary(Compiler* comp);
			static void binary(Compiler* comp);
 			static void number(Compiler* comp);
			static void grouping(Compiler* comp);
			static void literal(Compiler* comp);
			static void string(Compiler* comp);
			static void variable(Compiler* comp);
 			void emit_constant(Value value);
			uint8_t make_constant(Value value);

			bool check(TokenType type);
			bool match(TokenType type);
			void statement();
			void declaration();
			void synchronize();

			void expression_statement();
			void print_statement();
			void var_declaration();

			uint8_t parse_var(const char* message);
			void define_var(uint8_t global);
			void named_var(Token name);
			uint8_t identifier_constant(Token* name);

	};


	typedef std::function<void(Compiler* comp)> ParseFn;

	struct ParseRule
	{
		std::function<void(Compiler* comp)> prefix;
		std::function<void(Compiler* comp)> infix;
		Precendence precendence;
	};


	static ParseRule makeData(std::function<void(Compiler* comp)> pref, std::function<void(Compiler* comp)> inf, Precendence pre)
	{
		ParseRule fd = {pref, inf, pre};
		return fd;
	}

	typedef std::unordered_map<TokenType,ParseRule> prets;
	const prets parseRule =
	{
		//TOKEN								PREFIX					INFIX					PRECEDENCE
		{ AND, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ BREAKPOINT, 			makeData(	NULL,					NULL,					PREC_NONE		)},
		{ BREAK, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ LEFT_PAREN, 			makeData(	Compiler::grouping,		NULL,					PREC_NONE		)},
		{ RIGHT_PAREN, 			makeData(	NULL,					NULL,					PREC_NONE		)},
		{ LEFT_BRACE, 			makeData(	NULL,					NULL,					PREC_NONE		)},
		{ RIGHT_BRACE, 			makeData(	NULL,					NULL,					PREC_NONE		)},
		{ COMMA, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ DOT, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ MINUS, 				makeData(	Compiler::unary,		Compiler::binary,		PREC_TERM		)},
		{ PLUS, 				makeData(	NULL,					Compiler::binary,		PREC_TERM		)},
		{ SLASH_FORWARD, 		makeData(	NULL,					Compiler::binary,		PREC_FACTOR		)},
		{ STAR, 				makeData(	NULL,					Compiler::binary,		PREC_FACTOR		)},
		{ HASHTAG, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ COLON, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ SEMI, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ BANG, 				makeData(	Compiler::unary,		NULL,					PREC_NONE		)},
		{ BANG_EQUAL, 			makeData(	NULL,					Compiler::binary,		PREC_EQUALITY	)},
		{ EQUAL, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ EQUAL_EQUAL, 			makeData(	NULL,					Compiler::binary,		PREC_EQUALITY	)},
		{ GREATER, 				makeData(	NULL,					Compiler::binary,		PREC_COMPARISION)},
		{ GREATER_EQUAL, 		makeData(	NULL,					Compiler::binary,		PREC_COMPARISION)},
		{ LESS, 				makeData(	NULL,					Compiler::binary,		PREC_COMPARISION)},
		{ LESS_EQUAL, 			makeData(	NULL,					Compiler::binary,		PREC_COMPARISION)},
		{ FORWARD_ARROW, 		makeData(	NULL,					NULL,					PREC_NONE		)},
		{ IDENTIFIER, 			makeData(	Compiler::variable,		NULL,					PREC_NONE		)},
		{ STRING, 				makeData(	Compiler::string,		NULL,					PREC_NONE		)},
		{ FLOAT, 				makeData(	Compiler::number,		NULL,					PREC_NONE		)},
		{ INT, 					makeData(	Compiler::number,		NULL,					PREC_NONE		)},
		{ IN, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ NOT, 					makeData(	Compiler::unary,		NULL,					PREC_NONE		)},
		{ AS, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ CLASS, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ EXTENDS, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ IS, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ SIGNAL, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ AWAIT, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ CONST, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ ENUM, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ STATIC, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ ELIF, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ MATCH, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ ELSE, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ FALSE, 				makeData(	Compiler::literal,		NULL,					PREC_NONE		)},
		{ FUNC, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ FOR, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ IF, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ OR, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ PRINT, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ RETURN, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ TRUE, 				makeData(	Compiler::literal,		NULL,					PREC_NONE		)},
		{ VAR, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ WHILE, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ VOID, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ PI, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ TAU, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ INF, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ NAN, 					makeData(	NULL,					NULL,					PREC_NONE		)},
		{ SELF, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ TOOL, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ SUPER, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ CONTINUE, 			makeData(	NULL,					NULL,					PREC_NONE		)},
		{ PASS, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ ASSERT, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ PRELOAD, 				makeData(	NULL,					NULL,					PREC_NONE		)},
		{ NIL, 					makeData(	Compiler::literal,		NULL,					PREC_NONE		)},
		{ TK_ERROR, 			makeData(	NULL,					NULL,					PREC_NONE		)},
		{ TK_EOF, 				makeData(	NULL,					NULL,					PREC_NONE		)}
	};

}; // namespace GDPP


#endif