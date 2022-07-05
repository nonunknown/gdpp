#ifndef GD_STACK
#define GD_STACK

#include <iostream>
#include <map>
#include <memory.h>

namespace GDPP
{

typedef unsigned char byte;

#define STACK_MAX 256

class StackManager;
class Stack;

class Stack
{
	private:
		void printStack();
		void printBeginEnd();

	public:
		byte* begin;
		byte* top;
		int id;
		StackManager* owner;
		Stack();
		~Stack();
		void set_owner(StackManager* p_owner);
		void push(void* what, int size);
		void pop(int size, void* destination);

};

class StackManager
{
	private:
		std::map<int, Stack> stacks;
		int current_id;
		Stack* create_stack();

	public:
		StackManager();
		Stack* get_stack();
		Stack* get_new_stack();
		Stack* get_stack_by_id(int id);

	
};

};

#endif