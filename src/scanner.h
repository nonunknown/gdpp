
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <stdexcept>

#include "error.h"
#include "token.h"

namespace GDPP // means GDScript Plus Plus
{

    class Scanner
    {
        private:
            std::string sourceCode;
            std::vector<Token> tokens;
            std::map<std::string, Token::TokenType> keywords;
            int start, current, line,indentLevel;

            char Advance();
            char Peek();
            char PeekNext();
            void ScanToken();
            void AddToken(Token::TokenType p_type);
            void AddToken(Token::TokenType p_type, std::string literal);
            void EatStringLiteral();
            void EatNumber();
            void EatIdentifier();

            bool Match(char expected);
            bool IsAtEnd();
            bool IsDigit(char c);
            bool IsAlpha(char c);
            bool IsAlphaNumeric(char c);

            std::string Substr(std::string* p_str, int p_beginIndex, int p_endIndex);



        public:
            Scanner();
            void ReadFile(const char* path);
            std::string GetSourceCode();
            std::vector<Token> ScanTokens();

    };

}