#include "scanner.h"

using namespace GDPP;

void Scanner::ReadFile(const char* path)
{
    try
    {
       std::ifstream inputFile(path);
        sourceCode = std::string(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    
}

std::string Scanner::GetSourceCode()
{
    return sourceCode;
}

std::vector<Token> Scanner::ScanTokens()
{

    current = start = indentLevel = 0; // make sure everything start with 0
    line = 1;
    tokens.clear(); //clear tokens


    while(!IsAtEnd())
    {
        start = current;
        ScanToken();
    }

    // tokens.push_back(Token(TokenType::TK_EOF, "", "", line, indentLevel));
	AddToken(TokenType::TK_EOF);
    return tokens;


}

void Scanner::ScanToken()
{
    char c = Advance();

    switch (c)
    {
        case L'#': while(Peek() != '\n' && !IsAtEnd()) Advance(); break;

        case L'"' : EatStringLiteral(); break;

        case L'(': AddToken(TokenType::LEFT_PAREN); break;
        case L')': AddToken(TokenType::RIGHT_PAREN); break;
        case L',': AddToken(TokenType::COMMA); break;
        case L'+': AddToken(TokenType::PLUS); break;
        case L'/': AddToken(TokenType::SLASH_FORWARD); break;
        case L'*': AddToken(TokenType::STAR); break;
        case L':': AddToken(TokenType::COLON); break;

        case L'!': AddToken(Match(L'=')? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case L'=': AddToken(Match(L'=')? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case L'<': AddToken(Match(L'=')? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case L'>': AddToken(Match(L'=')? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
        case L'-': AddToken(Match(L'>')? TokenType::FORWARD_ARROW : TokenType::MINUS); break;

		case L'.': 
			if (IsDigit(PeekNext()))
				EatNumber();
			else
				AddToken(TokenType::DOT);
			break;

        case ' ':
        case '\r':
            break;
        case '\t': 
            indentLevel++; 
            break;

        case '\n':
            indentLevel = 0;
            line++;
            break; 
    
        default:
                if (IsDigit(c)) EatNumber();
            else
                if (IsAlpha(c)) EatIdentifier();
            else
                Error::Push(line, "Unexpected character.");
            break;
    }

    

}

char Scanner::Advance()
{
    return sourceCode.at(current++);
}

void Scanner::AddToken(TokenType p_type)
{
    AddToken(p_type, "");
}

void Scanner::AddToken(TokenType p_type, std::string p_literal)
{
    std::string text = Substr(&sourceCode, start, current);
    Token t = Token(p_type, text, p_literal, line, indentLevel);
    tokens.push_back(t);
    std::cout << "[TOKEN]: " << t.ToString() << std::endl;
}

bool Scanner::Match(char expected)
{
    if (IsAtEnd()) return false;
    if (sourceCode.at(current) != expected) return false;

    current++;
    return true;
}

bool Scanner::IsAtEnd()
{
    return current >= static_cast<int>(sourceCode.size());
}

char Scanner::Peek()
{
    if (IsAtEnd()) return '\0';
    return sourceCode.at(current);
}

char Scanner::PeekNext()
{
    
    if (current+1 >= static_cast<int>(sourceCode.size())) return '\0';
    return sourceCode.at(current+1);
}



void Scanner::EatStringLiteral()
{
    // "Test string";
    while(Peek() != L'"' && !IsAtEnd())
    {
        if (Peek() == '\n') line++;
        Advance();
    }

    if ( IsAtEnd() )
    {
        Error::Push(line, "Unterminated string");
        return;
    }

    Advance();

    std::string value = Substr(&sourceCode, start + 1, current - 1 );

    AddToken(TokenType::STRING,value);
}

void Scanner::EatNumber()
{
	bool isFloat = false;

    while(IsDigit(Peek())) { Advance(); }

    //Look for fractions
    if (Peek() == '.' && IsDigit(PeekNext()))
    {
        //Consume the '.'
        Advance();
		isFloat = true;
        while (IsDigit(Peek())) Advance();
    }

    AddToken( (isFloat)?TokenType::FLOAT:TokenType::INT ,Substr(&sourceCode, start,current));
}

void Scanner::EatIdentifier()
{
    while ( IsAlphaNumeric(Peek()) ) Advance();

    std::string text = Substr(&sourceCode, start,current);

    if ( keywords.find(text) != keywords.end() )
    {
        AddToken(keywords[text]);
    }
    else
    {
        AddToken(TokenType::IDENTIFIER,text);
    }
}



bool Scanner::IsDigit(char c)
{
    return isdigit(c);
}

bool Scanner::IsAlpha(char c)
{
    return isalpha(c);
}

bool Scanner::IsAlphaNumeric(char c)
{
    return isalnum(c);
}

// Scanner::Scanner()
// {
//     keywords.clear();
//     keywords["and"] = TokenType::AND;
//     keywords["class"] = TokenType::CLASS;
//     keywords["else"] = TokenType::ELSE;
//     keywords["false"] = TokenType::FALSE;
//     keywords["true"] = TokenType::TRUE;
//     keywords["for"] = TokenType::FOR;
//     keywords["func"] = TokenType::FUNC;
//     keywords["if"] = TokenType::IF;
//     keywords["or"] = TokenType::OR;
//     keywords["print"] = TokenType::PRINT;
//     keywords["return"] = TokenType::RETURN;
//     keywords["var"] = TokenType::VAR;
//     keywords["while"] = TokenType::WHILE;
//     keywords["void"] = TokenType::VOID;

// }


std::string Scanner::Substr(std::string* p_str, int p_beginIndex, int p_endIndex)
{
    std::string result = "";

    if (p_beginIndex < 0) {throw std::range_error("p_beginIndex");}
    if (p_endIndex > static_cast<int>(p_str->size()) ) {throw std::range_error("p_endIndex");}
    int subLen = p_endIndex - p_beginIndex;
    if (subLen < 0) {throw std::out_of_range("subLen is less than zero");}
    
    result = ( (p_beginIndex == 0) && (p_endIndex == static_cast<int>( p_str->size() ) ) ) ? *p_str : std::string(*p_str,p_beginIndex,subLen);

    return result;
}