#include <iostream>
#include <unordered_map>
#include <map>
#include <memory.h>
#include <vector>
#include "stack.hpp"
#include <chrono>
#include <functional>

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




void do_switch(int value)
{

	switch(value)
	{
		case 0: printf("0"); break;
		case 1: printf("1"); break;
		case 2: printf("2"); break;
		case 3: printf("3"); break;
		case 4: printf("4"); break;
		case 5: printf("5"); break;
		case 6: printf("6"); break;
		case 7: printf("7"); break;
		case 8: printf("8"); break;
		default: printf("NULL"); break;
	}

}

typedef void (*any)();

const std::map<int,std::function<void()>> mapped {
	{0, [](){printf("0");}},
	{1, [](){printf("1");}},
	{2, [](){printf("2");}},
	{3, [](){printf("3");}},
	{4, [](){printf("4");}},
	{5, [](){printf("5");}},
	{6, [](){printf("6");}},
	{7, [](){printf("7");}},
	{8, [](){printf("8");}}
};

void do_map(int value)
{
	try
	{
		mapped.at(value)();
	}
	catch(const std::exception& e)
	{
		printf("NULL");
	}
	
}

int main()
{
	int total_switch = 0;
	int total_mapped = 0;
	const int TIMES = 500;


	for (int i = 0; i < TIMES; i++)
	{
	
		auto ts_switch = std::chrono::steady_clock::now();

		for (size_t i = 0; i < 1000000; i++)
		{
			do_switch(i % 20);
		}

		auto te_switch = std::chrono::steady_clock::now();

		auto ts_map = std::chrono::steady_clock::now();

		for (size_t i = 0; i < 1000000; i++)
		{
			do_map(i % 20);
		}
		

		auto te_map = std::chrono::steady_clock::now();

		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;

		// std::cout << "SWITCH TIME" << std::endl;

		// std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(te_switch - ts_switch).count() << " nanoseconds" << std::endl;
		// std::cout << std::chrono::duration_cast<std::chrono::microseconds>(te_switch - ts_switch).count() << " microseconds" << std::endl;
		// std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(te_switch - ts_switch).count() << " milliseconds" << std::endl;
		// std::cout << std::chrono::duration_cast<std::chrono::seconds>(te_switch - ts_switch).count() << " seconds" << std::endl;

		// std::cout << "MAPPED TIME" << std::endl;

		// std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(te_map - ts_map).count() << " nanoseconds" << std::endl;
		// std::cout << std::chrono::duration_cast<std::chrono::microseconds>(te_map - ts_map).count() << " microseconds" << std::endl;
		// std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(te_map - ts_map).count() << " milliseconds" << std::endl;
		// std::cout << std::chrono::duration_cast<std::chrono::seconds>(te_map - ts_map).count() << " seconds" << std::endl;

		total_switch += std::chrono::duration_cast<std::chrono::milliseconds>(te_switch - ts_switch).count();
		total_mapped += std::chrono::duration_cast<std::chrono::milliseconds>(te_map - ts_map).count();
	}

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

	std::cout << "Median Switch: " << total_switch / TIMES << std::endl;
	std::cout << "Median Mapped: " << total_mapped / TIMES << std::endl;

	
	return 0;
}