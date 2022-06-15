#ifndef GD_DEBUG
#define GD_DEBUG

#include <string>
#include <iostream>

#include "chunk.h"

namespace GDPP
{
	class Disassemble
	{
		public:
			Disassemble();
			~Disassemble();
			void fromChunk(Chunk* chunk, std::string name);
			int fromInstruction(Chunk* chunk, int offset);
			int simpleInstruction(std::string name, int offset);
			int constantInstruction(std::string name,Chunk* chunk, int offset);
	};
} // namespace GDPP


#endif