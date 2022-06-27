#include "object.h"

using namespace GDPP;


static Obj* allocate_obj(size_t size, ObjType type)
{
	Obj* object = (Obj*)reallocate(NULL,0, size);
	object->type = type;
	return object;
}

#define ALLOCATE_OBJ(type, obj_type) \
	(type*)allocate_obj(sizeof(type), obj_type)

static inline bool is_obj_type(Value value, ObjType type)
{
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

ObjString* copy_str(const char* chars, int length)
{
	char* heap_chars = ALLOCATE(char, length + 1);
	memcpy(heap_chars, chars, length);
	heap_chars[length] = '\0';
	return allocate_str(heap_chars, length);
}

static ObjString* allocate_str(char* chars, int length)
{
	ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
	string->length = length;
	string->chars = chars;
	return string;
}

