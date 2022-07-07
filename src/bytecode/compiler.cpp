
#include "compiler.h"

using namespace GDPP;

Compiler::Compiler()
{
	parser = Parser();
	compilingChunk = nullptr;
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
	end_compiler();
	return !parser.hadError;
}

void Compiler::synchronize()
{
	parser.panicMode = false;
	while(parser.current.type != TK_EOF)
	{
		
		// if (parser.previous.type == )
		switch(parser.current.type)
		{
			case CLASS:
			case FUNC:
			case VAR:
			case FOR:
			case IF:
			case WHILE:
			case PRINT:
			case RETURN:
				return;
			
			default: ;//do nothing
		}

		advance();
	}
}

void Compiler::declaration()
{
	if (match(VAR))
	{
		var_declaration();
	}
	else
	{
		statement();
	}
	if (parser.panicMode) synchronize();
}



void Compiler::statement()
{
	if (match(PRINT)) 
	{
		print_statement();
	}
	else
	{
		expression_statement();
	}
}

void Compiler::expression_statement()
{
	expression();
	emit_byte(OP_POP);
}

void Compiler::print_statement()
{
	if (parser.current.type != LEFT_PAREN)
	{
		error_at_current("Expected '('");
		return;
	}
	expression();
	emit_byte(OP_PRINT);
}

void Compiler::var_declaration()
{
	uint8_t global = parse_var("Expected variable name");
	
	//TODO: strict typed variables
	// consume(COLON,"Expected ':'");
	
	if (match(EQUAL)) expression();
	else emit_byte(OP_NIL);

	define_var(global);
}

uint8_t Compiler::parse_var(const char* message)
{
	consume(IDENTIFIER, message);
	return identifier_constant(&parser.previous);
}


void Compiler::define_var(uint8_t global)
{
	emit_bytes(OP_DEFINE_GLOBAL, global);
}

void Compiler::named_var(Token name)
{
	uint8_t arg = identifier_constant(&name);
	emit_bytes(OP_GET_GLOBAL, arg);
}

#include "object.h"
uint8_t Compiler::identifier_constant(Token* name)
{
	return make_constant(OBJ_VAL( ObjHelper::copy_str(name->start, name->length) ));
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



const ParseRule* Compiler::get_rule(TokenType p_type)
{
	const auto it = parseRule.find(p_type);
	if (it != parseRule.end())
	{
		return &(it->second);
	}
	return nullptr; //unreachable
}


void Compiler::parse_precedence(Precendence precedence)
{
	advance();

	ParseFn prefixRule = get_rule(parser.previous.type)->prefix;
	if ( prefixRule == NULL )
	{
		error("Expected expression.");
		return;
	}

	prefixRule(this);

	while ( precedence <= get_rule(parser.current.type)->precendence )
	{
		advance();
		ParseFn infixRule = get_rule(parser.previous.type)->infix;
		infixRule(this);
	}
}

void Compiler::expression()
{
	parse_precedence(PREC_ASSIGNMENT);
}

void Compiler::unary(Compiler* comp)
{
	TokenType operatorType = comp->parser.previous.type;

	//compile the operand;
	comp->expression();

	comp->parse_precedence(PREC_UNARY);

	switch(operatorType)
	{
		case NOT:
		case BANG:
			comp->emit_byte(OP_NOT); break;
		case MINUS: comp->emit_byte(OP_NEGATE); break;
		default: return;
	}
}

void Compiler::binary(Compiler* comp)
{
	TokenType operatorType = comp->parser.previous.type;

	const ParseRule* rule = comp->get_rule(operatorType);
	comp->parse_precedence((Precendence)(rule->precendence + 1));
	switch(operatorType)
	{
		case PLUS:				comp->emit_byte(OpCode::OP_ADD); break;
		case MINUS:				comp->emit_byte(OP_SUB); break;
		case STAR:				comp->emit_byte(OP_MUL); break;
		case SLASH_FORWARD:		comp->emit_byte(OP_DIV); break;

		case BANG_EQUAL:		comp->emit_bytes(OP_EQUAL, OP_NOT); 		break;
		case EQUAL_EQUAL:		comp->emit_byte(OP_EQUAL); 				break;
		case GREATER:			comp->emit_byte(OP_GREATER); 			break;
		case GREATER_EQUAL:		comp->emit_bytes(OP_LESS, OP_NOT); 		break;
		case LESS:				comp->emit_byte(OP_LESS);				break;
		case LESS_EQUAL:		comp->emit_bytes(OP_GREATER,OP_NOT);		break;

		default: return;
	}
}

#include "object.h"
void Compiler::string(Compiler* comp)
{
	comp->emit_constant(OBJ_VAL(ObjHelper::copy_str(comp->parser.previous.start + 1, comp->parser.previous.length - 2)));
}

void Compiler::variable(Compiler* comp)
{
	comp->named_var(comp->parser.previous);
}

uint8_t Compiler::make_constant(Value value)
{
	int constant = Compiler::current_chunk()->add_constant( value );
	if (constant > UINT8_MAX)
	{
		Compiler::error("Too many constants in one chunk.");
		return 0;
	}

	return (uint8_t)constant;
}

void Compiler::emit_constant(Value value)
{
	emit_bytes(OP_CONSTANT, make_constant(value));
}

void Compiler::number(Compiler* comp)
{
	int value = atoi(comp->parser.previous.start);
	comp->emit_constant(INT_VAL(value));
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
			comp->emit_byte(OP_FALSE); break;
		
		case TRUE:
			comp->emit_byte(OP_TRUE); break;
		
		case NIL:
			comp->emit_byte(OP_NIL); break;
		
		default: return; //unreachable

	}
}

Chunk* Compiler::current_chunk()
{
	return compilingChunk;
}

void Compiler::emit_byte(uint8_t byte)
{
	current_chunk()->write(byte, parser.previous.line);
}

void Compiler::emit_bytes(uint8_t byte, uint8_t byte2)
{
	emit_byte(byte);
	emit_byte(byte2);
}

void Compiler::end_compiler()
{
	emit_return();

	#ifdef DEBUG_PRINT_CODE
		if (!parser.hadError)
		{
			Disassemble dis = Disassemble();
			dis.from_chunk(current_chunk(),"code");
		}
	#endif
}

void Compiler::emit_return()
{
	emit_byte(OP_RETURN);
}

void Compiler::advance()
{
	parser.previous = parser.current;
	for(;;)
	{
		parser.current = scanner.scan_token();
		if ( parser.current.type != TK_ERROR ) break;
		error_at_current(parser.current.start);
	}
}


void Compiler::consume(TokenType type, const char* message)
{
	if (parser.current.type == type)
	{
		advance();
		return;
	}

	error_at_current(message);
}

void Compiler::error_at_current(const char* message)
{	
	error_at(&parser.current, message);
}

void Compiler::error(const char* message)
{
	error_at(&parser.previous, message);
}

void Compiler::error_at(Token* token, const char* message)
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