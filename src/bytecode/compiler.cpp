
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
	expression();
	consume(TK_EOF, "Expect end of expression");
	endCompiler();
	return !parser.hadError;
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
		default: return;
	}
}

uint8_t Compiler::makeConstant(int value)
{
	int constant = Compiler::currentChunk()->addConstant(value);
	if (constant > UINT8_MAX)
	{
		Compiler::error("Too many constants in one chunk.");
		return 0;
	}

	return (uint8_t)constant;
}

void Compiler::emitConstant(int value)
{
	emitBytes(OP_CONSTANT, makeConstant(value));
}

void Compiler::number(Compiler* comp)
{
	int value = atoi(comp->parser.previous.start);
	comp->emitConstant(value);
}

void Compiler::grouping(Compiler* comp)
{
	comp->expression();
	comp->consume(RIGHT_PAREN, "Expect ')' after expression.");
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

void Compiler::errorAt(BToken* token, const char* message)
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