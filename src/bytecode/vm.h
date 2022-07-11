#ifndef GD_VM
#define GD_VM

#include <string.h>
#include <stdarg.h>
#include "value.h"
#include "object.h"
#include "compiler.h"
#include <map>

namespace GDPP
{

	#define STACK_MAX 256

	typedef enum
	{
		INTERPRET_OK,
		INTERPRET_COMPILE_ERROR,
		INTERPRET_RUNTIME_ERROR
	} InterpretResult;

	inline int ObjStringCmp(const ObjString *lhs, const ObjString *rhs){
		int min_length = lhs->length <= rhs->length ? lhs->length : rhs->length;
		return strncmp(lhs->chars, rhs->chars, min_length);
	}

	struct ObjString_CmpLess{
		bool operator()(const ObjString *lhs, const ObjString *rhs) const {
			return ObjStringCmp(lhs, rhs) < 0;
		}
	};

	struct ObjString_CmpEq{
		bool operator()(const ObjString *lhs, const ObjString *rhs) const {
			return ObjStringCmp(lhs, rhs) == 0;
		}
	};

	inline uint32_t HashFNV1(uint8_t *data, int len){
		static const uint32_t FNV1_BASIS = 0x811C9DC5;
		static const uint32_t FNV1_PRIME = 0x01000193;

		uint32_t h = FNV1_BASIS;
		while(len > 0){
			h ^= *data;
			h *= FNV1_PRIME;
			data += 1;
			len -= 1;
		}
		return h;
	}

	struct ObjString_Hash{
		size_t operator()(const ObjString *obj) const {
			// NOTE(fusion): We're using the FNV-1a hash here because the stdlib's
			// hash function only supports hashing whole types but not a buffer.
			return (size_t)HashFNV1((uint8_t*)obj->chars, obj->length);
		}
	};

	class VM
	{
		public:
			VM();
			~VM();
			static VM* instance;
			Obj* objects;
			InterpretResult interpret(std::string* p_src);
			void resetStack();
			void push(Value value);
			Value pop();
			Value peek(int distance);
			void runtime_error(const char* format, ...);
			bool is_falsey(Value value);
			void concatenate_str();
			ObjString* take_str(char* chars, int length);
		private:
			Chunk chunk;
			Compiler compiler;
			// StackManager sm;
			Value stack[STACK_MAX];
			Value* stackTop;
			// NOTE(fusion): Using std::unordered_map here is better because we don't
			// need globals to be ordered (I think?). If that is the case, we should
			// go back to using std::map.
			//std::map<ObjString*, Value, ObjString_CmpLess> globals;
			std::unordered_map<ObjString*, Value,
				ObjString_Hash, ObjString_CmpEq> globals;
			uint8_t* ip;
			InterpretResult run();
			std::string* src;

	};


} // namespace GDPP


#endif