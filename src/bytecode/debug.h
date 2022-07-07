#ifndef GD_DEBUG
#define GD_DEBUG

#include <string>
#include <iostream>

#include "chunk.h"
#include "common.h"

namespace GDPP
{
	class Disassemble
	{
		public:
			Disassemble();
			~Disassemble();
			void from_chunk(Chunk* chunk, std::string name);
			int from_instruction(Chunk* chunk, int offset);
			int simple_instruction(std::string name, int offset);
			int constant_instruction(std::string name,Chunk* chunk, int offset);
	};
}; // namespace GDPP


#endif