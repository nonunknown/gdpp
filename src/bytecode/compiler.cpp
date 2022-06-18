
#include "compiler.h"

using namespace GDPP;

bool Compiler::compile(std::string* p_src, Chunk* p_chunk)
{
	scanner = Scanner(p_src);
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

ParseRule* Compiler::getRule(TokenType p_type)
{
	return &parseRules[p_type];
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

	prefixRule();

	while ( precedence <= getRule(parser.current.type)->precedence )
	{
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule();
	}
}

void Compiler::expression()
{
	parsePrecedence(PREC_ASSIGNMENT);
}

void Compiler::unary()
{
	TokenType operatorType = parser.previous.type;

	//compile the operand;
	expression();

	parsePrecedence(PREC_UNARY);

	switch(operatorType)
	{
		case MINUS: emitByte(OP_NEGATE); break;
		default: return;
	}
}

void Compiler::binary()
{
	TokenType operatorType = parser.previous.type;

	ParseRule* rule = getRule(operatorType);
	parsePrecedence((Precendence)(rule->precedence + 1));
	switch(operatorType)
	{
		case PLUS:				emitByte(OP_ADD); break;
		case MINUS:				emitByte(OP_SUB); break;
		case STAR:				emitByte(OP_MUL); break;
		case SLASH_FORWARD:		emitByte(OP_DIV); break;
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

void Compiler::number()
{
	int value = (int)(parser.previous.start);
	emitConstant(value);
}

void Compiler::grouping()
{
	expression();
	consume(RIGHT_PAREN, "Expect ')' after expression.");
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