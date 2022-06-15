
#ifndef GD_COMMON
#define GD_COMMON

#include <map>

namespace GDPP
{
	#define DEBUG_TRACE_EXECUTION

	
	typedef enum {
		OP_CONSTANT,
		OP_NEGATE,
		OP_ADD,
		OP_SUB,
		OP_DIV,
		OP_MUL,
		OP_RETURN,


		MAX,
	} OpCode;

	const char * const OPCODES[8] = {
		"OP_CONSTANT",
		"OP_NEGATE",
		"OP_ADD",
		"OP_SUB",
		"OP_DIV",
		"OP_MUL",
		"OP_RETURN",

		"MAX"
	};

} // namespace GDPP


#endif