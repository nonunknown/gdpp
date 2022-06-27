#include "vm.h"


using namespace GDPP;

VM::VM()
{
	compiler = Compiler();
	resetStack();
}

VM::~VM()
{

}

InterpretResult VM::interpret(std::string* p_src)
{
	Chunk c = Chunk();
	src = p_src;

	if (!compiler.compile(p_src, &c))
	{
		//TODO: free chunk
		return INTERPRET_COMPILE_ERROR;
	}

	chunk = &c;
	ip = chunk->code;


	InterpretResult result = run();

	//TODO: ? free chunk
	return result;
}

InterpretResult VM::run()
{
	#ifdef DEBUG_TRACE_EXECUTION
		Disassemble disassemble = Disassemble();
	#endif
	#define READ_BYTE() (*ip++) // Instruction pointer increment, or Program counter
	#define READ_CONSTANT() (chunk->constants.at(READ_BYTE()));
	#define BINARY_OP(valueType, op) 							\
		do { 													\
			if ( !IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1)) )	\
			{													\
				runtime_error("operands must be number");		\
				return INTERPRET_RUNTIME_ERROR;					\
			}													\
			int b = AS_INT(pop());								\
			int a = AS_INT(pop());								\
			push( valueType(a op b));							\
		} while(false)

	for(;;)
	{
		#ifdef DEBUG_TRACE_EXECUTION

			std::cout << "STACK ->	";
			for( Value* slot = stack; slot < stackTop; slot++ )
			{
				printValue(*slot);
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

			// case OP_NEGATE:
			// {
			// 	push(-pop());
			// 	break;
			// }

			case OP_ADD: BINARY_OP(INT_VAL, +); break;
			case OP_SUB: BINARY_OP(INT_VAL, -); break;
			case OP_MUL: BINARY_OP(INT_VAL, *); break;
			case OP_DIV: BINARY_OP(INT_VAL, /); break;

			case OP_NOT: push( BOOL_VAL(is_falsey(pop())) ); break;

			case OP_NEGATE:
				{
					if (!IS_NUMBER( peek(0) ))
					{
						runtime_error("operand must be a number");
						return INTERPRET_RUNTIME_ERROR;
					}
					push( INT_VAL( -AS_INT( pop() ) ) );
				}
			
			case OP_NIL: push(NIL_VAL); break;
			case OP_FALSE: push(BOOL_VAL(false)); break;
			case OP_TRUE: push(BOOL_VAL(true)); break;

			case OP_EQUAL:
				{
					Value b = pop();
					Value a = pop();
					push(BOOL_VAL(values_equal(a,b)));
					break;
				}
			
			case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
			case OP_LESS: 	 BINARY_OP(BOOL_VAL, <); break;



			case OP_RETURN:
				std::cout << "finished" << std::endl;
				return INTERPRET_OK;
			

		}
	}

	#undef READ_BYTE
	#undef READ_CONSTANT
	#undef BINARY_OP

	return INTERPRET_OK;
}

bool VM::is_falsey(Value value)
{
	return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

Value VM::peek(int distance)
{
	return stackTop[-1 - distance];
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

void VM::runtime_error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fputs("\n", stderr);

	size_t instruction = ip - chunk->code - 1;
	int line = chunk->lines[instruction];
	fprintf(stderr, "[line %d] in script\n", line);
	resetStack();

}
