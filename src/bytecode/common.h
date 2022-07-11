
#ifndef GD_COMMON
#define GD_COMMON

namespace GDPP
{
	#define DEBUG_TRACE_EXECUTION
	#define DEBUG_PRINT_CODE
	
	enum OpCode 
	{
		OP_CONSTANT,
		OP_NEGATE,
		OP_ADD,
		OP_SUB,
		OP_DIV,
		OP_MUL,
		OP_RETURN,
		OP_NIL,
		OP_TRUE,
		OP_FALSE,
		OP_NOT,
		OP_EQUAL,
		OP_GREATER,
		OP_LESS,
		OP_PRINT,
		OP_POP,
		OP_DEFINE_GLOBAL,
		OP_GET_GLOBAL,
		OP_SET_GLOBAL,
		OP_EXIT,

		MAX,
	};

	const char * const OPCODES[21] = 
	{
		"OP_CONSTANT",
		"OP_NEGATE",
		"OP_ADD",
		"OP_SUB",
		"OP_DIV",
		"OP_MUL",
		"OP_RETURN",
		"OP_NIL",
		"OP_TRUE",
		"OP_FALSE",
		"OP_NOT",
		"OP_EQUAL",
		"OP_GREATER",
		"OP_LESS",
		"OP_PRINT",
		"OP_POP",
		"OP_DEFINE_GLOBAL",
		"OP_GET_GLOBAL",
		"OP_SET_GLOBAL",
		"OP_EXIT",

		"MAX"
	};

}; // namespace GDPP


#endif