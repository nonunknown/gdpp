#include "expression.h"
#include <string>

#ifndef ASTPRINTER_H
#define ASTPRINTER_H

namespace GDPP
{
	class ASTPrinter
	{

		private:
			struct PrintVisitor : Visitor
			{
				void Visit(Assign&){ name = "Assign"; };
				void Visit(Binary&){ name = "Binary"; };
				void Visit(Call&){ name = "Call"; };
				void Visit(Get&){ name = "Get"; };
				void Visit(Grouping&){ name = "Grouping"; };
				void Visit(Logical&){ name = "Logical"; };
				void Visit(Set&){ name = "Set"; };
				void Visit(Self&){ name = "Self"; };
				void Visit(Unary&){ name = "Unary"; };
				void Visit(Variable&){ name = "Variable"; };

				virtual void Visit(Literal<int>&) { name = "Variable"; };
				virtual void Visit(Literal<float>&) { name = "Variable"; };
				virtual void Visit(Literal<std::string>&) { name = "Variable"; };
				virtual void Visit(Literal<bool>&) { name = "Variable"; };

				void Visit(void*) {}

				std::string name;
			};
			PrintVisitor printer;

		public:
			ASTPrinter()
			{
				printer = PrintVisitor();

			};

			std::string print(Expression* expression)
			{
				expression->Accept(printer);
				return printer.name;
				
			}
	};
}

#endif