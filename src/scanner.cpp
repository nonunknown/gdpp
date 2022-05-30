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

    tokens.push_back(Token(Token::TokenType::TK_EOF, "", "", line, indentLevel));
    return tokens;


}

void Scanner::ScanToken()
{
    char c = Advance();

    switch (c)
    {
        case L'#': while(Peek() != '\n' && !IsAtEnd()) Advance();

        case L'"' : EatStringLiteral(); break;

        case L'(': AddToken(Token::TokenType::LEFT_PAREN); break;
        case L')': AddToken(Token::TokenType::RIGHT_PAREN); break;
        case L',': AddToken(Token::TokenType::COMMA); break;
        case L'.': AddToken(Token::TokenType::DOT); break;
        case L'+': AddToken(Token::TokenType::PLUS); break;
        case L'/': AddToken(Token::TokenType::SLASH_FORWARD); break;
        case L'*': AddToken(Token::TokenType::STAR); break;
        case L':': AddToken(Token::TokenType::COLON); break;

        case L'!': AddToken(Match(L'=')? Token::TokenType::BANG_EQUAL : Token::TokenType::BANG); break;
        case L'=': AddToken(Match(L'=')? Token::TokenType::EQUAL_EQUAL : Token::TokenType::EQUAL); break;
        case L'<': AddToken(Match(L'=')? Token::TokenType::LESS_EQUAL : Token::TokenType::LESS); break;
        case L'>': AddToken(Match(L'=')? Token::TokenType::GREATER_EQUAL : Token::TokenType::GREATER); break;
        case L'-': AddToken(Match(L'>')? Token::TokenType::FORWARD_ARROW : Token::TokenType::MINUS); break;

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

void Scanner::AddToken(Token::TokenType p_type)
{
    AddToken(p_type, "");
}

void Scanner::AddToken(Token::TokenType p_type, std::string p_literal)
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

    AddToken(Token::TokenType::STRING,value);
}

void Scanner::EatNumber()
{
    while(IsDigit(Peek())) { Advance(); }

    //Look for fractions
    if (Peek() == '.' && IsDigit(PeekNext()))
    {
        //Consume the '.'
        Advance();
        while (IsDigit(Peek())) Advance();
    }

    AddToken(Token::TokenType::NUMBER,Substr(&sourceCode, start,current));
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
        AddToken(Token::TokenType::IDENTIFIER,text);
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

Scanner::Scanner()
{
    keywords.clear();
    keywords["and"] = Token::TokenType::AND;
    keywords["class"] = Token::TokenType::CLASS;
    keywords["else"] = Token::TokenType::ELSE;
    keywords["false"] = Token::TokenType::FALSE;
    keywords["true"] = Token::TokenType::TRUE;
    keywords["for"] = Token::TokenType::FOR;
    keywords["func"] = Token::TokenType::FUNC;
    keywords["if"] = Token::TokenType::IF;
    keywords["or"] = Token::TokenType::OR;
    keywords["print"] = Token::TokenType::PRINT;
    keywords["return"] = Token::TokenType::RETURN;
    keywords["var"] = Token::TokenType::VAR;
    keywords["while"] = Token::TokenType::WHILE;
    keywords["void"] = Token::TokenType::VOID;

}


std::string Scanner::Substr(std::string* p_str, int p_beginIndex, int p_endIndex)
{
    std::string result = "";

    if (p_beginIndex < 0) {throw std::range_error("p_beginIndex");}
    if (p_endIndex > p_str->size()) {throw std::range_error("p_endIndex");}
    int subLen = p_endIndex - p_beginIndex;
    if (subLen < 0) {throw std::out_of_range("subLen is less than zero");}
    
    result = ( (p_beginIndex == 0) && (p_endIndex == p_str->size()) ) ? *p_str : std::string(*p_str,p_beginIndex,subLen);

    return result;
}