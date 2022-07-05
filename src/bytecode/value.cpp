#include "value.h"
#include "object.h"
#include "string.h"

using namespace GDPP;


void Print::printObject(Value value)
{
	switch(OBJ_TYPE(value))
	{
		case OBJ_STRING:
		{
			printf("%s", AS_CSTRING(value));
			break;
		}
			
	}
}

void Print::printValue(Value value)
{
	switch(value.type)
	{
		case VAL_BOOL:
			printf(AS_BOOL(value) ? "true" : "false"); break;
		case VAL_NIL:
			printf("NULL"); break;
		case VAL_INT:
			printf("%i", AS_INT(value)); break;
		case VAL_FLOAT:
			printf("%g", AS_FLOAT(value)); break;
		case VAL_OBJ:
			Print::printObject(value); break;
	}
}

bool ValueHelper::values_equal(Value a, Value b)
{
	if (a.type != b.type) return false;
	switch(a.type)
	{
		case VAL_BOOL: return AS_BOOL(a) == AS_BOOL(b);
		case VAL_NIL: return true;
		case VAL_INT: return ( AS_INT(a) == AS_INT(b) );
		case VAL_FLOAT: return (AS_FLOAT(a) == AS_FLOAT(b));
		case VAL_OBJ:
		{
			ObjString* a_str 	= AS_STRING(a);
			ObjString* b_str 	= AS_STRING(b);
			return a_str->length == b_str->length && memcmp(a_str->chars, b_str->chars, a_str->length) == 0;
			break;
		}
		default: return false; //unreachable
	}

}