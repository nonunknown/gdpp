#include <iostream>

#include "expression.h"

#ifndef INTERPRET
#define INTERPRET

namespace GDPP
{

	class InterpretVisitor : Visitor
	{
		
		public:
			Expression* innerExpression;
			void* value;

			void Visit(Literal<int>& literal) {value = &literal.value;}
			void Visit(Literal<float>& literal) {value = &literal.value;}
			void Visit(Literal<std::string>& literal) {value = &literal.value;}
			void Visit(Literal<bool>& literal) {value = &literal.value;}
			
			void Visit(Grouping& grouping) {innerExpression = grouping.expression;}
			void Visit(Assign&){  }
			void Visit(Binary&){  }
			void Visit(Call&){  }
			void Visit(Get&){  }
			void Visit(Logical&){  }
			void Visit(Set&){  }
			void Visit(Self&){  }
			void Visit(Unary&){
				// Expression* right = unary.right;
				// if (right )
				// switch(unary.token->type)
				// {
				// 	case MINUS:
				// 		value = -(float)*right;
				// }
			}
			void Visit(Variable&){  }
			void Visit(void*) {}

	};

	class Interpret
	{
		InterpretVisitor interpretVisitor;
		public:

			Interpret();
			~Interpret();
	};


}


#endif