#include "object.h"

using namespace GDPP;


static Obj* allocate_obj(size_t size, ObjType type)
{
	Obj* object = (Obj*)reallocate(NULL,0, size);
	object->type = type;
	object->next = vm_instance->objects;
	vm_instance->objects = object;
	return object;
}

#define ALLOCATE_OBJ(type, obj_type) \
	(type*)allocate_obj(sizeof(type), obj_type)

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

void MemoryHelper::free_obj(Obj* obj)
{
	switch(obj->type)
	{
		case OBJ_STRING:
		{
			ObjString* str = (ObjString*)obj;
			FREE_ARRAY(char, str->chars, str->length+1);
			FREE(ObjString, obj);
		}
	}
}

void MemoryHelper::free_objs()
{
	Obj* object = vm_instance->objects;
	while(  object != nullptr)
	{
		Obj* next = object->next;
		free_obj(object);
		object = next;
	}
}