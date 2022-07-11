#ifndef GD_OBJ
#define GD_OBJ

#include "value.h"

namespace GDPP
{
	enum ObjType
	{
		OBJ_STRING
	};

	const char * const OBJ_NAMES[1]
	{
		"OBJ_STRING"
	};

	struct Obj
	{
		ObjType type;
		Obj* next;
	};

	struct ObjString : Obj
	{
		int length;
		char* chars;
	};
	

}; // namespace GDPP



#endif