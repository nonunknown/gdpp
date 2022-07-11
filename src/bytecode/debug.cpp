#include "debug.h"

using namespace GDPP;

Disassemble::Disassemble()
{

}

Disassemble::~Disassemble()
{

}

void Disassemble::from_chunk(Chunk* chunk, std::string name)
{
	std::cout << "== " << name << " == " << std::endl;
	

	for (int offset = 0; offset < chunk->count;)
	{
		offset = from_instruction(chunk, offset);
	}

}

int Disassemble::from_instruction(Chunk* chunk, int offset)
{
	printf("%04d   ", offset);

	if ( offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1] )
	{
		printf("    | ");
	}
	else
	{
		printf("%4d ", chunk->lines[offset]);
	}

	uint8_t instruction = chunk->code[offset];

	switch(instruction)
	{
		case OP_CONSTANT:
		case OP_DEFINE_GLOBAL:
		case OP_GET_GLOBAL:
		case OP_SET_GLOBAL:
			return constant_instruction(OPCODES[instruction], chunk, offset);
		case OP_RETURN:
		case OP_NEGATE:
		case OP_ADD:
		case OP_SUB:
		case OP_DIV:
		case OP_MUL:
		case OP_NIL:
		case OP_FALSE:
		case OP_TRUE:
		case OP_NOT:
		case OP_EQUAL:
		case OP_GREATER:
		case OP_LESS:
		case OP_PRINT:
		case OP_POP:
		case OP_EXIT:
			return simple_instruction(OPCODES[instruction], offset);
			
		default:
			std::cout << "Unknown opcode at: " << instruction;
			return offset+1;
	}
	return 0;
}

int Disassemble::simple_instruction(std::string name, int offset)
{
	std::cout << name << std::endl;
	return offset+1;
}

int Disassemble::constant_instruction(std::string name, Chunk* chunk, int offset)
{
	uint8_t constant = chunk->code[offset+1];
	printf("%-16s %4d '", name.c_str(), constant);
	const Value* v = &chunk->constants.at(constant);
	Print::print_value(*v);
	printf("\n");
	return offset + 2;
}
