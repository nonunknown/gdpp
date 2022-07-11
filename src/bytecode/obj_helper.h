#ifndef GD_OBJ_HELPER
#define GD_OBJ_HELPER

#include "vm.h"
#include "memory.h"
#include "object.h"

namespace GDPP
{

	class ObjHelper
	{
		public:
            static Obj* allocate_obj(size_t size, ObjType type);
			static ObjString* allocate_str(char* chars, int length);
			static ObjString* copy_str(const char* chars, int length);
			static bool is_obj_type(Value value, ObjType type);
            static void free_obj(Obj* obj);
		    static void free_objs();
	};

    #define ALLOCATE_OBJ(type, obj_type) \
	    (type*)ObjHelper::allocate_obj(sizeof(type), obj_type)

    #define OBJ_TYPE(value)         ( AS_OBJ(value)->type )
    #define IS_STRING(value)        ( ObjHelper::is_obj_type(value, OBJ_STRING) )

    #define AS_STRING(value)		( (ObjString*)AS_OBJ(value) )
    #define AS_CSTRING(value)		( ((ObjString*)AS_OBJ(value))->chars )

};


#endif
