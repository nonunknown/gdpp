#include "obj_helper.h"


using namespace GDPP;


Obj* ObjHelper::allocate_obj(size_t size, ObjType type)
{
	Obj* object = (Obj*)reallocate(NULL,0, size);
	object->type = type;
	object->next = VM::instance->objects;
	VM::instance->objects = object;
	return object;
}

ObjString* ObjHelper::allocate_str(char* chars, int length)
{
	ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
	string->length = length;
	string->chars = chars;
	return string;
}

ObjString* ObjHelper::copy_str(const char* chars, int length)
{
	char* heap_chars = ALLOCATE(char, length + 1);
	memcpy(heap_chars, chars, length);
	heap_chars[length] = '\0';
	return allocate_str(heap_chars, length);
}

bool ObjHelper::is_obj_type(Value value, ObjType type)
{
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

void ObjHelper::free_obj(Obj* obj)
{
	switch(obj->type)
	{
		case OBJ_STRING:
		{
			ObjString* str = (ObjString*)obj;
			FREE_ARRAY(char, str->chars, str->length+1);
			FREE(ObjString, obj);
			break;
		}
	}
}

void ObjHelper::free_objs()
{
	Obj* object = VM::instance->objects;
	while(  object != nullptr)
	{
		Obj* next = object->next;
		free_obj(object);
		object = next;
	}
}