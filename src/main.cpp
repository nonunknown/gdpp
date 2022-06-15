#include <iostream>
// #include "scanner.h"
// #include "parser.h"
// #include "interpreter/ast_printer.hpp"
#include "bytecode/chunk.h"
#include "bytecode/debug.h"
#include "bytecode/vm.h"
#include <fstream>
#include <string>

using namespace GDPP;


// void testAST()
// {
// 	Literal<int> lit = Literal<int>(10);
// 	Literal<bool> b = Literal<bool>(true);
// 	Literal<float> f = Literal<float>(10.5f);
// 	Literal<std::string> s = Literal<std::string>("Hello world");
// 	ASTPrinter pr = ASTPrinter();
// 	std::cout << pr.print(&lit) << std::endl;
// 	std::cout << pr.print(&s) << std::endl;
// 	std::cout << std::string(typeid(lit).name()) << std::endl;
// 	std::cout << std::string(typeid(s).name()) << std::endl;
// 	std::cout << std::string(typeid(b).name()) << std::endl;
// 	std::cout << std::string(typeid(f).name()) << std::endl;
// }

static std::string sourceCode = "null";
static void readFile(const char* path)
{
    try
    {
       std::ifstream inputFile(path);
        sourceCode = std::string(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
		exit(65);
    }
    
    
}
int main() 
{

	VM vm = VM();

	readFile("scripts/example.gdpp");
	std::cout << sourceCode << std::endl;
	
	// Chunk c = Chunk();

	// #define WRITE_CONST(name, num) \
	// 	int name = c.addConstant(num); \
	// 	c.write(OpCode::OP_CONSTANT, 1); \
	// 	c.write(name, 1); 
	
	// #define WRITE_NEG(num) \
	// 	WRITE_CONST(num) \
	// 	c.write(OpCode::OP_NEGATE, 1);

	// WRITE_CONST(a, 10);
	// WRITE_CONST(d, 40);
	// c.write(OpCode::OP_SUB, 1);


	// c.write(OpCode::OP_RETURN, 1);

	// vm.interpret(&c);

	// #undef ADD_CONST
	// #undef WRITE_NEG
	return 0;
}