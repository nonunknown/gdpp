#include "stack.hpp"

using namespace GDPP;

Stack::Stack()
{
	begin = (byte*)calloc(STACK_MAX,sizeof(byte));
	top = begin;
	
}

Stack::~Stack()
{
	std::cout << "destroying stack: " << id << std::endl;
}

void Stack::set_owner(StackManager* p_owner)
{
	owner = p_owner;
}


void Stack::printStack()
{
	printf("STACK -> %d [ ", id);
	for(size_t n=0;n<STACK_MAX;n++)
	{

		printf("%02x, ", begin[n]);

		if ( n == (size_t)(top - begin) )
		{
			printf("<- TOP | %d |", (byte)(*top));
		}

	}
	printf(" ]\n\n");
}

void Stack::printBeginEnd()
{

	if (top == begin)
	{
		printf("STACK -> %d | [ 0 ]", id);
		return;
	}

	printf("STACK -> %d | [ ", id);
	for (int n=0;n<(int)(top - begin);n++)
	{
		printf("%d", n);
	}

	printf("\n");

}

void Stack::push(void* what, int size)
{
	if (begin+STACK_MAX - top < size) // check if new bytes fit in the stack
	{
		std::cout << "creating another stack" << std::endl;
		Stack* newStack = owner->get_new_stack();
		newStack->push(what, size);
		return;
	}

	memcpy(top,what,size); //copy the bytes from what to top
	top += size; // move the top pointer to a empty part of the byte array
	printStack();

	printBeginEnd();
}

void Stack::pop(int size, void* destination=nullptr)
{

	top = top - size; // move to the start byte of the poping value
	
	printf("%02x \n", (byte)(*top));

	//if no destination is specified, the top pointer moves backwards
	if (destination == nullptr)
	{
		memset(top,0,size); //clear the bytes in front of top
		return;
		// memcpy(destination, top - (size+1), size);
	}
	memcpy(destination, top, size); //copy the bytes to destination

	memset(top,0,size); //clear the bytes in front of top
	printStack();


}




StackManager::StackManager()
{
	current_id = -1;
}

Stack* StackManager::get_stack()
{
	if (stacks.size() == 0)
	{
		return create_stack();
	}
	else
	{
		return &stacks.find(current_id)->second;
	}
}

Stack* StackManager::get_new_stack()
{
	return create_stack();
}

Stack* StackManager::get_stack_by_id(int id)
{
	return &stacks.find(id)->second;
}

Stack* StackManager::create_stack()
{
		current_id++;
		stacks[current_id] = Stack();
		Stack* stack = &stacks.find(current_id)->second;
		stack->set_owner(this);
		stack->id = current_id;
		std::cout << "Created stack with ID: " << current_id << " | at -> " << &stack << std::endl;
		return stack;
}