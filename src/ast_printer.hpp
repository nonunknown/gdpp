#ifndef ASTPRINTER_H
#define ASTPRINTER_H

#include "expression.h"
#include <string>

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

				void Visit(Literal<int>& literal){ name = std::to_string( literal.value ); };
				void Visit(Literal<float>& literal){ name = std::to_string( literal.value ); };
				void Visit(Literal<std::string>& literal){ name = literal.value; };
				void Visit(Literal<bool>& literal){ name = std::to_string( literal.value ); };



				std::string name;
			};
			PrintVisitor printVisitor;

		public:
			ASTPrinter()
			{
				
			};

			std::string print(Expression* expression)
			{
				expression->visitor->Accept(printVisitor);
				return printVisitor.name;
			}
	};
}

#endif