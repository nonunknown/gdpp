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
	if ( is_at_end() ) return false;
	if ( *data.current != expected ) return false;
	data.numCurrent++;
	data.current++;
	return true;
}

char Scanner::peek()
{
	return *data.current;
}

char Scanner::peek_next()
{
	if (is_at_end()) return '\0';
	return data.current[1];
}
void Scanner::skip_whitespace()
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
				while (peek() != '\n' && !is_at_end()) advance(); 
				break;
			default: return;
		}
	}
}

Token Scanner::scan_token()
{
	skip_whitespace();
	data.start = data.current;
	data.numStart = data.numCurrent;
	if (is_at_end()) return make_token(TK_EOF);
	char c = advance();
	if ( isalpha(c) ) return identifier();
	if (isdigit(c)) return number();

	switch(c)
	{
		case '(': return make_token(LEFT_PAREN);
		case ')': return make_token(RIGHT_PAREN);
		case '{': return make_token(LEFT_BRACE);
		case '}': return make_token(RIGHT_BRACE);
		case ';': return make_token(COLON);
		case ',': return make_token(COMMA);
		case '.': return make_token(DOT);
		case '+': return make_token(PLUS);
		case '/': return make_token(SLASH_FORWARD);
		case '*': return make_token(STAR);
		case '-':
			return make_token(
				match('>') ? FORWARD_ARROW : MINUS);
		case '!':
			return make_token(
				match('=') ? BANG_EQUAL : BANG);
		case '=':
			return make_token(
				match('=') ? EQUAL_EQUAL : EQUAL);
		case '<':
			return make_token(
				match('=') ? LESS_EQUAL : LESS);
		case '>':
			return make_token(
				match('=') ? GREATER_EQUAL : GREATER);
		
		case '"': return string();
	}

	std::string msg = "Unexpected character.";
	return error_token(&msg);
}

Token Scanner::string()
{
	while( peek() != '"' && !is_at_end() )
	{
		if ( peek() == '\n' ) data.line++;
		advance();
	}

	std::string msg = "Unterminated string";
	if ( is_at_end() ) return error_token(&msg);

	advance();
	return make_token(STRING);

}

Token Scanner::number()
{
	while( isdigit( peek() ) ) advance();

	if (peek() == '.' && isdigit(peek_next()))
	{
		advance();
		while( isdigit( peek() ) ) advance();
		return make_token(FLOAT);
	}

	return make_token(INT);
}

Token Scanner::identifier()
{
	while( isalnum(peek()) ) advance();

	std::string target = src->substr(data.numStart,data.numCurrent-data.numStart);
	std::cout << "trying to find identifier: " << target << std::endl;

	// const auto it = keywordMap.find(target.c_str());

	// if (it != keywordMap.end())
	// {
	// 	std::cout << "found key: " << TOKEN_NAMES[it->second] << std::endl;
	// 	return makeToken(it->second);
	// }

	try
	{
		TokenType result;
		result = keywordMap.at(target);
		std::cout << "found identifier: " << TOKEN_NAMES[result] << std::endl;
		return make_token(result);
	}
	catch(const std::exception& e)
	{
		std::cout << "not a keyword, generating identifier: " << target << std::endl;
		return make_token(IDENTIFIER);
	}
	

	return make_token(IDENTIFIER); //unreachable;
}

TokenType Scanner::check_keyword(int start, int length, const char* rest, TokenType type)
{
	if (data.current - data.start == start + length &&
		memcmp(data.start + start, rest, length) == 0)
		{
			return type;
		}

	return IDENTIFIER;
}

Token Scanner::make_token(TokenType t)
{
	Token token;
	token.type = t;
	token.start = data.start;
	token.length = (int)(data.current - data.start);
	token.line = data.line;


	return token;
}

Token Scanner::error_token(std::string* message)
{
	Token token;
	token.type = TokenType::TK_ERROR;
	token.start = message->c_str();
	token.length = (int)message->length();
	token.line = data.line;
	return token;

}

bool Scanner::is_at_end()
{
	return *data.current == '\0';
}