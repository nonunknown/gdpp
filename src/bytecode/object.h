#ifndef GD_OBJ
#define GD_OBJ

#include "common.h"
#include "memory.h"
#include "value.h"
#include "vm.h"
#include "object.h"

namespace GDPP
{
	enum ObjType
	{
		OBJ_STRING
	};

	const char * const OBJ_NAMES[]
	{
		"OBJ_STRING"
	};

	struct Obj
	{
		ObjType type;
	};

	struct ObjString
	{
		Obj obj;
		int length;
		char* chars;
	};

	#define OBJ_TYPE(value) ( AS_OBJ(value)->type )
	#define IS_STRING(value) ( is_obj_type(value, OBJ_STRING) )



	#define AS_STRING(value)		( (ObjString*)AS_OBJ(value) )
	#define AS_CSTRING(value)		( ((ObjString*)AS_OBJ(value))->chars )

	ObjString* copy_str(const char* chars, int length);

} // namespace GDPP



#endif