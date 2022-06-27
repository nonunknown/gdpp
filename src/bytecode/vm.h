#ifndef GD_VM
#define GD_VM

#include "chunk.h"
#include "value.h"
#include "compiler.h"
#include "debug.h"
#include "stack.hpp"
#include <stdarg.h>

namespace GDPP
{
	#define STACK_MAX 256

	typedef enum
	{
		INTERPRET_OK,
		INTERPRET_COMPILE_ERROR,
		INTERPRET_RUNTIME_ERROR
	} InterpretResult;

	class VM
	{
		private:
			Chunk* chunk;
			Compiler compiler;
			StackManager sm;
			Value stack[STACK_MAX];
			Value* stackTop;
			uint8_t* ip;
			InterpretResult run();
			std::string* src;
		public:
			VM();
			~VM();
			InterpretResult interpret(std::string* p_src);
			void resetStack();
			void push(Value value);
			Value pop();
			Value peek(int distance);
			void runtime_error(const char* format, ...);
			bool is_falsey(Value value);

	};
} // namespace GDPP


#endif