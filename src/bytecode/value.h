#ifndef GD_VALUE
#define GD_VALUE

#include <stdio.h>

namespace GDPP
{
	enum ValueType
	{
		VAL_NIL,
		VAL_BOOL,
		VAL_INT,
		VAL_FLOAT,
		VAL_OBJ,
	};

	const char * const VALUE_TYPES[]
	{
		"VAL_NIL",
		"VAL_BOOL",
		"VAL_INT",
		"VAL_FLOAT",
		"VAL_OBJ",
	};

	typedef struct Obj Obj;
	typedef struct ObjString ObjString;
	
	struct Value
	{
		ValueType type;
		union 
		{
			bool boolean;
			int numi;
			float numf;
			Obj* obj;
		} as;
	};



	#define BOOL_VAL(value) 	(Value{ VAL_BOOL	, 		{.boolean = value		}})
	#define INT_VAL(value)		(Value{ VAL_INT		,		{.numi = value			}})
	#define FLOAT_VAL(value)	(Value{ VAL_FLOAT	,		{.numf = value			}})
	#define NIL_VAL				(Value{ VAL_NIL		,		{.numi = 0				}})
	#define OBJ_VAL(object)		(Value{ VAL_OBJ		,		{.obj = (Obj*)object	}})

	#define AS_BOOL(value)		((value).as.boolean)
	#define AS_INT(value)		((value).as.numi)
	#define AS_FLOAT(value)		((value).as.numf)
	#define AS_OBJ(value)		((value).as.obj)

	#define IS_BOOL(value)		((value).type == VAL_BOOL)
	#define IS_INT(value)		((value).type == VAL_INT)
	#define IS_FLOAT(value)		((value).type == VAL_FLOAT)
	#define IS_NIL(value)		((value).type == VAL_NIL)
	#define IS_NUMBER(value)	( IS_INT(value) || IS_FLOAT(value) )
	#define IS_OBJ(value)		((value).type == VAL_OBJ)


	class Print
	{
		public:
			static void print_object(Value value);
			static void print_value(Value value);
	};

	class ValueHelper
	{
		public:
			static bool values_equal(Value a, Value b);
	};





};

#endif