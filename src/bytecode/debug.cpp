#include "debug.h"

using namespace GDPP;

Disassemble::Disassemble()
{

}

Disassemble::~Disassemble()
{

}

void Disassemble::fromChunk(Chunk* chunk, std::string name)
{
	std::cout << "== " << name << " == " << std::endl;
	

	for (int offset = 0; offset < chunk->count;)
	{
		offset = fromInstruction(chunk, offset);
	}

}

int Disassemble::fromInstruction(Chunk* chunk, int offset)
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
		case OP_RETURN:
			return simpleInstruction(OPCODES[OP_RETURN], offset);
		case OP_CONSTANT:
			return constantInstruction(OPCODES[OP_CONSTANT], chunk, offset);
		case OP_NEGATE:
			return simpleInstruction(OPCODES[OP_NEGATE], offset);
		case OP_ADD:
			return simpleInstruction(OPCODES[OP_ADD], offset);
		case OP_SUB:
			return simpleInstruction(OPCODES[OP_SUB], offset);
		case OP_DIV:
			return simpleInstruction(OPCODES[OP_DIV], offset);
		case OP_MUL:
			return simpleInstruction(OPCODES[OP_MUL], offset);

		default:
			std::cout << "Unknown opcode at: " << instruction;
			return offset+1;
	}
	return 0;
}

int Disassemble::simpleInstruction(std::string name, int offset)
{
	std::cout << name << std::endl;
	return offset+1;
}

int Disassemble::constantInstruction(std::string name, Chunk* chunk, int offset)
{
	uint8_t constant = chunk->code[offset+1];
	printf("%-16s %4d '", name.c_str(), constant);
	printf("%i'\n", chunk->constants.at(constant));

	return offset + 2;
}
