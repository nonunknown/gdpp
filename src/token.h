#include <string>
#include <vector>
#include <any>
#include <string>
#include "types.h"

namespace GDPP
{
    class Token
    {

        public:
            enum TokenType {
            // Single-character tokens.
            LEFT_PAREN, RIGHT_PAREN,
            COMMA, DOT, MINUS, PLUS, SLASH_FORWARD, STAR, HASHTAG, COLON,

            // One or two character tokens.
            BANG, BANG_EQUAL,
            EQUAL, EQUAL_EQUAL,
            GREATER, GREATER_EQUAL,
            LESS, LESS_EQUAL, FORWARD_ARROW,

            // Literals.
            IDENTIFIER, STRING, NUMBER,

            // Keywords.
            AND, CLASS, ELSE, FALSE, FUNC, FOR, IF, OR,
            PRINT, RETURN, TRUE, VAR, WHILE, VOID,

            TK_EOF
            };

            const char* TOKEN_NAMES[37] = { // Single-character tokens.
            "LEFT_PAREN", "RIGHT_PAREN",
            "COMMA", "DOT", "MINUS", "PLUS", "SLASH_FORWARD", "STAR", "HASHTAG", "COLON",

            // One or two character tokens.
            "BANG", "BANG_EQUAL",
            "EQUAL", "EQUAL_EQUAL",
            "GREATER", "GREATER_EQUAL",
            "LESS", "LESS_EQUAL", "FORWARD_ARROW",

            // Literals.
            "IDENTIFIER", "STRING", "NUMBER",

            // Keywords.
            "AND", "CLASS", "ELSE", "FALSE", "FUNC", "FOR", "IF", "OR",
            "PRINT", "RETURN", "TRUE", "VAR", "WHILE", "VOID",

            "TK_EOF"
            };

            Token(TokenType p_type, std::string p_lexeme, std::string p_literal, int p_line, int p_indentLevel);
            ~Token();

            std::string ToString();
            TokenType type;
            variant literal;
        
        private:
            std::string lexeme;
            int line, indentLevel;

    };
}