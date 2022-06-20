#include "scanner.h"

using namespace GDPP;

Scanner::Scanner()
{
}


Scanner::~Scanner()
{
	
}

void Scanner::start(std::string* p_src)
{
	src = p_src;
	data = Data();
	data.numStart = data.numCurrent = 0;
	data.start = &src->at(data.numStart);
	data.current = &src->at(data.numCurrent);
	data.line = 1;
	data.tabLevel = 0;
}

char Scanner::advance()
{
	data.numCurrent++;
	data.current++;
	return data.current[-1];
}

bool Scanner::match(char expected)
{
	if ( isAtEnd() ) return false;
	if ( *data.current != expected ) return false;
	data.numCurrent++;
	data.current++;
	return true;
}

char Scanner::peek()
{
	return *data.current;
}

char Scanner::peekNext()
{
	if (isAtEnd()) return '\0';
	return data.current[1];
}
void Scanner::skipWhitespace()
{
	for(;;)
	{
		char c = peek();
		switch(c)
		{
			case ' ':
			case '\r':
				advance();
				break;
			case '\t':
				data.tabLevel++;
				advance();
				break;
			case '\n':
				data.line++;
				data.tabLevel = 0;
				advance();
				break;
			case '#':
				while (peek() != '\n' && !isAtEnd()) advance(); 
				break;
			default: return;
		}
	}
}

BToken Scanner::scanToken()
{
	skipWhitespace();
	data.start = data.current;
	data.numStart = data.numCurrent;
	if (isAtEnd()) return makeToken(TK_EOF);
	char c = advance();
	if ( isalpha(c) ) return identifier();
	if (isdigit(c)) return number();

	switch(c)
	{
		case '(': return makeToken(LEFT_PAREN);
		case ')': return makeToken(RIGHT_PAREN);
		case '{': return makeToken(LEFT_BRACE);
		case '}': return makeToken(RIGHT_BRACE);
		case ';': return makeToken(COLON);
		case ',': return makeToken(COMMA);
		case '.': return makeToken(DOT);
		case '+': return makeToken(PLUS);
		case '/': return makeToken(SLASH_FORWARD);
		case '*': return makeToken(STAR);
		case '-':
			return makeToken(
				match('>') ? FORWARD_ARROW : MINUS);
		case '!':
			return makeToken(
				match('=') ? BANG_EQUAL : BANG);
		case '=':
			return makeToken(
				match('=') ? EQUAL_EQUAL : EQUAL);
		case '<':
			return makeToken(
				match('=') ? LESS_EQUAL : LESS);
		case '>':
			return makeToken(
				match('=') ? GREATER_EQUAL : GREATER);
		
		case '"': return string();
	}

	std::string msg = "Unexpected character.";
	return errorToken(&msg);
}

BToken Scanner::string()
{
	while( peek() != '"' && !isAtEnd() )
	{
		if ( peek() == '\n' ) data.line++;
		advance();
	}

	std::string msg = "Unterminated string";
	if ( isAtEnd() ) return errorToken(&msg);

	advance();
	return makeToken(STRING);

}

BToken Scanner::number()
{
	while( isdigit( peek() ) ) advance();

	if (peek() == '.' && isdigit(peekNext()))
	{
		advance();
		while( isdigit( peek() ) ) advance();
		return makeToken(FLOAT);
	}

	return makeToken(INT);
}

BToken Scanner::identifier()
{
	while( isalnum(peek()) ) advance();

	std::string target = src->substr(data.numStart,data.numCurrent-data.numStart);
	std::cout << "trying to find identifier: " << target << std::endl;
	
	const auto it = keywordMap.find(target.c_str());

	if (it != keywordMap.end())
	{
		std::cout << "found key: " << TOKEN_NAMES[it->second] << std::endl;
		return makeToken(it->second);
	}

	return makeToken(IDENTIFIER);
}

TokenType Scanner::checkKeyword(int start, int length, const char* rest, TokenType type)
{
	if (data.current - data.start == start + length &&
		memcmp(data.start + start, rest, length) == 0)
		{
			return type;
		}

	return IDENTIFIER;
}

BToken Scanner::makeToken(TokenType t)
{
	BToken token;
	token.type = t;
	token.start = data.start;
	token.length = (int)(data.current - data.start);
	token.line = data.line;


	return token;
}

BToken Scanner::errorToken(std::string* message)
{
	BToken token;
	token.type = TokenType::TK_ERROR;
	token.start = message->c_str();
	token.length = (int)message->length();
	token.line = data.line;
	return token;

}

bool Scanner::isAtEnd()
{
	return *data.current == '\0';
}