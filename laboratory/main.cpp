#include <iostream>
#include <unordered_map>
#include <memory.h>
#include <vector>
#include "stack.hpp"

using namespace GDPP;

enum VarType
{
	VAL_INT,
	VAL_FLOAT,
	VAL_DOUBLE
};

const std::unordered_map<VarType, int> sizeTable
{
	{ VAL_INT, sizeof(int) },
	{ VAL_FLOAT, sizeof(float) }
};


template<typename T>
struct Value
{
	byte type;
	T data;
};

#define INT_VAL(value) (Value {VAL_INT, value})

int main()
{
	StackManager sm = StackManager();
	


	int i = 10;
	double da;
	double d = 200.22;

	
	Value<int> v {(byte)200, 10};
	byte* bytes = (byte*)&i;

	std::cout << "size of value: " << sizeof(v) << std::endl;
	for(size_t n=0;n<8;i++)
	{
		printf("%02x", bytes[n]);
	}

	// for(int i=0;i<100;i++)
	// {
	// 	sm.get_stack()->append(&i, sizeof(int));
	// 	sm.get_stack()->append(&d, sizeof(double));
	// }
	
	// sm.get_stack()->push(&d, sizeof(double));
	// sm.get_stack()->pop(sizeof(double), &da);
	// sm.get_stack()->push(&da, sizeof(double));
	// sm.get_stack()->push(&da, sizeof(double));
	// sm.get_stack()->push(&i, sizeof(int));

	// sm.get_stack()->pop(sizeof(int), &ia);

	// byte* a = (byte*)calloc(256,sizeof(byte));

	// if (a == NULL)
	// {
	// 	std::cerr << "unable to allocate memory" << std::endl;
	// 	exit(-1);
	// }
	// int i = 5000;
	// int ia = 200;
	// memcpy(a,&i,sizeof(int));
	// memcpy(a+sizeof(int),&ia,sizeof(int));

	// printf("[ ");
	
	// for(size_t n=0;n<256;n++)
	// {
	// 	printf("%02x, ", a[n]);
	// }
	// printf(" ];\n");
	// std::cout << (int)(*a) << std::endl;
	// std::cout << (int)(*(a+sizeof(int)) ) << std::endl;
	// std::cout << sizeof(INT_VALUE(22)) << " | " << sizeof(int) << std::endl;
	// std::cout << sizeof(FLOAT_VALUE(10.1)) << " | " << sizeof(float) << std::endl;
	// std::cout << sizeof(DOUBLE_VALUE(0.231231231)) << " | " << sizeof(double) << std::endl;
	
	// free(a);
	return 0;
}