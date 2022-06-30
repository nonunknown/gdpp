
#include "compiler.h"

using namespace GDPP;

Compiler::Compiler()
{

}

bool Compiler::compile(std::string* p_src, Chunk* p_chunk)
{
	scanner = Scanner();
	scanner.start(p_src);
	parser = Parser();
	parser.hadError = false;
	parser.panicMode = false;
	compilingChunk = p_chunk;
	advance();
	// expression();
	// consume(TK_EOF, "Expect end of expression");
	while (!match(TK_EOF))
	{
		declaration();
	}
	endCompiler();
	return !parser.hadError;
}


void Compiler::declaration()
{
	statement();
}

void Compiler::statement()
{
	if (match(PRINT)) statement_print();
}

void Compiler::statement_print()
{
	expression();
	emitByte(OP_PRINT);
}

bool Compiler::match(TokenType type)
{
	if ( !check(type) ) return false;
	advance();
	return true;
}

bool Compiler::check(TokenType type)
{
	return parser.current.type == type;
}



const ParseRule* Compiler::getRule(TokenType p_type)
{
	const auto it = parseRule.find(p_type);
	if (it != parseRule.end())
	{
		return &(it->second);
	}
	return nullptr; //unreachable
}


void Compiler::parsePrecedence(Precendence precedence)
{
	advance();

	ParseFn prefixRule = getRule(parser.previous.type)->prefix;
	if ( prefixRule == NULL )
	{
		error("Expected expression.");
		return;
	}

	prefixRule(this);

	while ( precedence <= getRule(parser.current.type)->precendence )
	{
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule(this);
	}
}

void Compiler::expression()
{
	parsePrecedence(PREC_ASSIGNMENT);
}

void Compiler::unary(Compiler* comp)
{
	TokenType operatorType = comp->parser.previous.type;

	//compile the operand;
	comp->expression();

	comp->parsePrecedence(PREC_UNARY);

	switch(operatorType)
	{
		case NOT:
		case BANG:
			comp->emitByte(OP_NOT); break;
		case MINUS: comp->emitByte(OP_NEGATE); break;
		default: return;
	}
}

void Compiler::binary(Compiler* comp)
{
	TokenType operatorType = comp->parser.previous.type;

	const ParseRule* rule = comp->getRule(operatorType);
	comp->parsePrecedence((Precendence)(rule->precendence + 1));
	switch(operatorType)
	{
		case PLUS:				comp->emitByte(OP_ADD); break;
		case MINUS:				comp->emitByte(OP_SUB); break;
		case STAR:				comp->emitByte(OP_MUL); break;
		case SLASH_FORWARD:		comp->emitByte(OP_DIV); break;

		case BANG_EQUAL:		comp->emitBytes(OP_EQUAL, OP_NOT); 		break;
		case EQUAL_EQUAL:		comp->emitByte(OP_EQUAL); 				break;
		case GREATER:			comp->emitByte(OP_GREATER); 			break;
		case GREATER_EQUAL:		comp->emitBytes(OP_LESS, OP_NOT); 		break;
		case LESS:				comp->emitByte(OP_LESS);				break;
		case LESS_EQUAL:		comp->emitBytes(OP_GREATER,OP_NOT);		break;

		default: return;
	}
}

void Compiler::string(Compiler* comp)
{
	comp->emitConstant(OBJ_VAL(ObjHelper::copy_str(comp->parser.previous.start + 1, comp->parser.previous.length - 2)));
}

uint8_t Compiler::makeConstant(Value value)
{
	int constant = Compiler::currentChunk()->addConstant( value );
	if (constant > UINT8_MAX)
	{
		Compiler::error("Too many constants in one chunk.");
		return 0;
	}

	return (uint8_t)constant;
}

void Compiler::emitConstant(Value value)
{
	emitBytes(OP_CONSTANT, makeConstant(value));
}

void Compiler::number(Compiler* comp)
{
	int value = atoi(comp->parser.previous.start);
	comp->emitConstant(INT_VAL(value));
}

void Compiler::grouping(Compiler* comp)
{
	comp->expression();
	comp->consume(RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::literal(Compiler* comp)
{
	switch(comp->parser.previous.type)
	{
		case FALSE:
			comp->emitByte(OP_FALSE); break;
		
		case TRUE:
			comp->emitByte(OP_TRUE); break;
		
		case NIL:
			comp->emitByte(OP_NIL); break;
		
		default: return; //unreachable

	}
}

Chunk* Compiler::currentChunk()
{
	return compilingChunk;
}

void Compiler::emitByte(uint8_t byte)
{
	currentChunk()->write(byte, parser.previous.line);
}

void Compiler::emitBytes(uint8_t byte, uint8_t byte2)
{
	emitByte(byte);
	emitByte(byte2);
}

void Compiler::endCompiler()
{
	emitReturn();

	#ifdef DEBUG_PRINT_CODE
		if (!parser.hadError)
		{
			Disassemble dis = Disassemble();
			dis.fromChunk(currentChunk(),"code");
		}
	#endif
}

void Compiler::emitReturn()
{
	emitByte(OP_RETURN);
}

void Compiler::advance()
{
	parser.previous = parser.current;
	for(;;)
	{
		parser.current = scanner.scanToken();
		if ( parser.current.type != TK_ERROR ) break;
		errorAtCurrent(parser.current.start);
	}
}


void Compiler::consume(TokenType type, const char* message)
{
	if (parser.current.type == type)
	{
		advance();
		return;
	}

	errorAtCurrent(message);
}

void Compiler::errorAtCurrent(const char* message)
{	
	errorAt(&parser.current, message);
}

void Compiler::error(const char* message)
{
	errorAt(&parser.previous, message);
}

void Compiler::errorAt(Token* token, const char* message)
{
	if (parser.panicMode) return;
	parser.panicMode = true;
	std::cout << "[ line " << token->line << "] Error" << std::endl;
	if (token->type == TK_EOF )
	{
		std::cout << "at end." << std::endl;
	}
	else if (token->type == TK_ERROR)
	{

	}
	else
	{
		std::cout << "at -> " << token->length << " | " << token->start << std::endl;
	}


	std::cout << "msg: " << message << std::endl;
	parser.hadError = true;
}