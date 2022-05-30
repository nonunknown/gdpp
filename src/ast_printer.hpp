#include "expression.h"


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
				void Visit(Literal& literal){ name = *literal.value; };
				void Visit(Logical&){ name = "Logical"; };
				void Visit(Set&){ name = "Set"; };
				void Visit(Self&){ name = "Self"; };
				void Visit(Unary&){ name = "Unary"; };
				void Visit(Variable&){ name = "Variable"; };

				std::string name;
			};
			PrintVisitor printVisitor;

		public:
			ASTPrinter()
			{
				
			};

			std::string print(Expression* expression)
			{
				expression->Accept(printVisitor);
				return printVisitor.name;
			}
	};
}

