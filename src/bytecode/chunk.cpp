#include "chunk.h"

using namespace GDPP;

Chunk::Chunk()
{
	// cppcheck-suppress useInitializationList
	constants = std::vector<Value>();
	count = 0;
	capacity = 0;
	code = nullptr;
}

Chunk::~Chunk()
{

}

void Chunk::write(uint8_t byte, int line = -1)
{
	if ( capacity < count + 1 )
	{
		int oldCapacity = capacity;
		capacity = GROW_CAPACITY(oldCapacity);
		code = GROW_ARRAY(uint8_t, code, oldCapacity, capacity);
	}

	code[count] = byte;
	lines.push_back(line);
	count++;
}

void Chunk::free()
{
	FREE_ARRAY(uint8_t, code, capacity);
	Chunk();
}

int Chunk::add_constant(Value value)
{
	constants.push_back(value);
	return (int)(constants.size() -1);
}
