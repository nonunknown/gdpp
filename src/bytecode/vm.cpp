#include "vm.h"
#include "debug.h"
#include "value.h"

using namespace GDPP;

VM::VM()
{
	resetStack();
}

VM::~VM()
{

}

InterpretResult VM::interpret(std::string* p_src)
{
	src = p_src;
	compile();
	return INTERPRET_OK;
	// chunk = p_chunk;
	// ip = chunk->code;

	// return run();
}

InterpretResult VM::run()
{
	#ifdef DEBUG_TRACE_EXECUTION
		Disassemble disassemble = Disassemble();
	#endif
	#define READ_BYTE() (*ip++) // Instruction pointer increment, or Program counter
	#define READ_CONSTANT() (chunk->constants.at(READ_BYTE()));
	#define BINARY_OP(op) 		\
		do { 					\
			Value b = pop();	\
			Value a = pop();	\
			push( a op b);		\
		} while(false)	

	for(;;)
	{
		#ifdef DEBUG_TRACE_EXECUTION

			std::cout << "STACK ->	";
			for( Value* slot = stack; slot < stackTop; slot++ )
			{
				std::cout << "[ " << *slot << " ] ";
			}

			std::cout << std::endl;
			

			disassemble.fromInstruction(chunk, (int)(ip - chunk->code));
		#endif

		uint8_t instruction;
		switch(instruction = READ_BYTE())
		{
			case OP_CONSTANT:
			{
				Value constant = READ_CONSTANT();
				// std::cout << constant << std::endl;
				push(constant);
				break;
			}

			case OP_NEGATE:
			{
				push(-pop());
				break;
			}

			case OP_ADD: BINARY_OP(+); break;
			case OP_SUB: BINARY_OP(-); break;
			case OP_MUL: BINARY_OP(*); break;
			case OP_DIV: BINARY_OP(/); break;
				
			case OP_RETURN:
				std::cout << pop() << std::endl;
				return INTERPRET_OK;
		}
	}

	#undef READ_BYTE
	#undef READ_CONSTANT
	#undef BINARY_OP

	return INTERPRET_OK;
}

void VM::resetStack()
{
	stackTop = stack;
}

void VM::push(Value value)
{
	*stackTop = value;
	stackTop++;
}

Value VM::pop()
{
	stackTop--;
	return *stackTop;
}
