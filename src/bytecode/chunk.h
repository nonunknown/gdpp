#ifndef GD_CHUNK
#define GD_CHUNK

#include "value.h"

#include <vector>
#include <inttypes.h>

namespace GDPP
{
	struct Chunk
	{
		public:
			std::vector<Value> constants;
			std::vector<int> lines;
			uint8_t* code;
			int count;
			int capacity;
			Chunk();
			~Chunk();
			void write(uint8_t byte, int line);
			void free();
			int addConstant(Value value);
	};
}

#endif