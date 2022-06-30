#ifndef GD_MEMORY
#define GD_MEMORY

#include "common.h"
#include "object.h"
#include <stdlib.h>
#include <sys/types.h>

namespace GDPP
{

	inline void* reallocate(void* pointer, size_t oldSize, size_t newSize)
	{
		oldSize++; //TODO: Done this to avoid warnings, please remove or update
		if (newSize == 0)
		{
			free(pointer);
			return nullptr;
		}

		void* result = realloc(pointer, newSize);

		if (result == NULL) {exit(1);} //HARD QUIT NO MEMORY AVAILABLE

		return result;
	}

	inline int GROW_CAPACITY(int capacity)
	{
		return (capacity) < 8 ? 8 : (capacity * 2);
	}

	#define GROW_ARRAY(type, pointer, oldCount, newCount) \
		(type*)reallocate(pointer, sizeof(type) * (oldCount), \
		sizeof(type) * (newCount))

	#define FREE_ARRAY(type, pointer, oldCount) \
		reallocate(pointer, sizeof(type) * (oldCount), 0);
	
	#define ALLOCATE(type, count) \
		(type*)reallocate(NULL, 0, sizeof(type) * (count))
	
	#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0)

}

#endif