#include "vm.h"
#include "common.h"
#include "obj_helper.h"

#ifdef DEBUG_TRACE_EXECUTION
	#include "debug.h"
#endif

using namespace GDPP;

VM* VM::instance;

VM::VM()
{
	instance = this;
	objects = nullptr;
	compiler = Compiler();
	resetStack();
}

VM::~VM()
{
	ObjHelper::free_objs();
}

InterpretResult VM::interpret(std::string* p_src)
{
	chunk = Chunk();
	src = p_src;

	if (!compiler.compile(p_src, &chunk))
	{
		//TODO: free chunk
		return INTERPRET_COMPILE_ERROR;
	}

	ip = chunk.code;


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
	#define READ_CONSTANT() (chunk.constants.at(READ_BYTE()))
	#define READ_STRING() AS_STRING(READ_CONSTANT())
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
		// TODO(fusion): Have these checks only in debug builds. Have assert macros.

		if(ip >= (chunk.code + chunk.count)){
			std::cout << "REACHED BYTECODE END WITHOUT AN OP_EXIT INSTRUCTION\n";
			abort();
		}

		if(stackTop < stack){
			std::cout << "STACK UNDERFLOW\n";
			abort();
		}else if(stackTop >= (stack + STACK_MAX)){
			std::cout << "STACK OVERFLOW\n";
			abort();
		}

		#ifdef DEBUG_TRACE_EXECUTION

			std::cout << "STACK[" << (stackTop - stack) << "] ->\t";
			for( Value* slot = stack; slot < stackTop; slot++ )
			{
				Print::print_value(*slot);
			}

			std::cout << std::endl;
			

			disassemble.from_instruction(&chunk, (int)(ip - chunk.code));
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

			case OP_ADD:
			{
				Value a = peek(0);
				Value b = peek(1);

				if ( IS_STRING(a) && IS_STRING(b) )
				{
					concatenate_str();
				}
				else if ( IS_NUMBER(a) && IS_NUMBER(b) )
				{
					if ( IS_INT(a) && IS_INT(b) )
					{
						int b = AS_INT(pop());
						int a = AS_INT(pop());
						push(INT_VAL(a + b));
					}
					else if ( IS_FLOAT(a) && IS_FLOAT(b) )
					{
						float b = AS_FLOAT(pop());
						float a = AS_FLOAT(pop());
						push(FLOAT_VAL(a + b));
					}
				}
				else
				{
					runtime_error("Operands must be int, float or string.");
					return INTERPRET_RUNTIME_ERROR;
				}
				break;
			}
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
					break;
				}
			
			case OP_NIL: push(NIL_VAL); break;
			case OP_FALSE: push(BOOL_VAL(false)); break;
			case OP_TRUE: push(BOOL_VAL(true)); break;

			case OP_EQUAL:
				{
					Value b = pop();
					Value a = pop();
					push(BOOL_VAL(ValueHelper::values_equal(a,b)));
					break;
				}
			
			case OP_GREATER: BINARY_OP(BOOL_VAL, >); break;
			case OP_LESS: 	 BINARY_OP(BOOL_VAL, <); break;

			case OP_PRINT:
			{
				Print::print_value(pop());
				printf("\n");
				break;
			}

			case OP_POP:
			{
				pop();
				break;
			}

			case OP_DEFINE_GLOBAL:
			{
				ObjString* name = READ_STRING();
				std::cout << "defined variable: " << name->chars << std::endl;
				// globals.insert(name,peek(0));
				globals[name] = peek(0);
				pop();

				break;
			}

			case OP_GET_GLOBAL:
			{
				ObjString* name = READ_STRING();
				Value value;

				try
				{
					std::cout << "looking for: " << name->chars << std::endl;
					value = globals[name];
					push(value);
				}
				catch(const std::exception& e)
				{
					runtime_error("undefined variable '%s'.", name->chars);
					return INTERPRET_RUNTIME_ERROR;
				}
				break;
			}

			case OP_SET_GLOBAL:
			{
				ObjString* name = READ_STRING();
				auto it = globals.find(name);
				if (it == globals.end()) //if the global variable doesnt exists, report err
				{
					runtime_error("Undefined variable '%s'", name->chars);
				}
				break;
			}

			case OP_EXIT:
				std::cout << "finished" << std::endl;
				return INTERPRET_OK;
		}
	}

	#undef READ_BYTE
	#undef READ_CONSTANT
	#undef BINARY_OP
	#undef READ_STRING

	return INTERPRET_OK;
}

bool VM::is_falsey(Value value)
{
	return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

void VM::concatenate_str()
{
	ObjString* b = AS_STRING(pop());
	ObjString* a = AS_STRING(pop());
	int length = a->length + b->length;
	char* chars = ALLOCATE(char, length + 1);
	memcpy(chars, a->chars,a->length);
	memcpy(chars + a->length, b->chars, b->length);
	chars[length] = '\0';
	ObjString* result = take_str(chars, length);
	push(OBJ_VAL(result));
}

ObjString* VM::take_str(char* chars, int length)
{
	return ObjHelper::allocate_str(chars,length);
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

	size_t instruction = ip - chunk.code - 1;
	int line = chunk.lines[instruction];
	fprintf(stderr, "[line %d] in script\n", line);
	resetStack();

}
