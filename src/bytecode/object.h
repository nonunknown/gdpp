#ifndef GD_OBJ
#define GD_OBJ

#include "memory.h"
#include "value.h"
#include "vm.h"

namespace GDPP
{
	enum ObjType
	{
		OBJ_STRING
	};

	// const char * const OBJ_NAMES[]
	// {
	// 	"OBJ_STRING"
	// };

	struct Obj
	{
		ObjType type;
		Obj* next;
	};

	struct ObjString
	{
		Obj obj;
		int length;
		char* chars;
	};

	class ObjHelper
	{
		public:
			static ObjString* allocate_str(char* chars, int length);
			static ObjString* copy_str(const char* chars, int length);
			static bool is_obj_type(Value value, ObjType type);
	};

	class MemoryHelper
	{
		static void free_obj(Obj* obj);
		static void free_objs();
	};

	#define OBJ_TYPE(value) ( AS_OBJ(value)->type )
	#define IS_STRING(value) ( ObjHelper::is_obj_type(value, OBJ_STRING) )

	#define AS_STRING(value)		( (ObjString*)AS_OBJ(value) )
	#define AS_CSTRING(value)		( ((ObjString*)AS_OBJ(value))->chars )
	

}; // namespace GDPP



#endif