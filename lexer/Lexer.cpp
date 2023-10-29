#include "Lexer.h"
#include <unordered_map>
#include <cctype>

std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"null", Null},
    {"let", Let},
    {"const", Const},
    {"func", Func}
};

Token::Token(const std::string& value, TokenType type) : value(value), type(type) {}

std::string Token::getValue() const {
    return value;
}

TokenType Token::getType() const {
    return type;
}

Lexer::Lexer(const std::string& sourceCode) : sourceCode(sourceCode) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    src = std::vector<char>(sourceCode.begin(), sourceCode.end());

    while (!src.empty()) {
        char currentChar = src[0];
        src.erase(src.begin());

        if (currentChar == '(') {
            tokens.push_back(Token("(", OpenParen));
        }
        else if (currentChar == ')') {
            tokens.push_back(Token(")", CloseParen));
        }
         else if (currentChar == '{') {
            tokens.push_back(Token("{", OpenBrace));
        }
        else if (currentChar == '}') {
            tokens.push_back(Token("}", CloseBrace));
        }
        else if (currentChar == '[') {
            tokens.push_back(Token("[", OpenBracket));
        }
        else if (currentChar == ']') {
            tokens.push_back(Token("]", CloseBracket));
        }
        else if (currentChar == ',') {
            tokens.push_back(Token(",", Comma));
        }
        else if (currentChar == '+' || currentChar == '-' || currentChar == '*' || currentChar == '/' || currentChar == '%') {
            tokens.push_back(Token(std::string(1, currentChar), BinaryOperator));
        }
        else if (currentChar == '=') {
            tokens.push_back(Token("=", Equals));
        } else if (currentChar == ';') {
            tokens.push_back(Token(";", Semicolon));
        }
        else {
            if (isInt(currentChar)) {
                std::string num;
                num += currentChar;
                while (!src.empty() && isInt(src[0])) {
                    num += src[0];
                    src.erase(src.begin());
                }
                tokens.push_back(Token(num, Number));
            }
            else if (isAlpha(currentChar)) {
                std::string ident;
                ident += currentChar;
                while (!src.empty() && isAlpha(src[0])) {
                    ident += src[0];
                    src.erase(src.begin());
                }
                auto it = KEYWORDS.find(ident);
                if (it != KEYWORDS.end()) {
                    tokens.push_back(Token(ident, it->second));
                }
                else {
                    tokens.push_back(Token(ident, Identifier));
                }
            }
            else if (isSkippable(currentChar)) {
                // Skip whitespace characters
            }
            else {
                std::cerr << "Unrecognized character found in source: " << static_cast<int>(currentChar) << " " << currentChar << std::endl;
                exit(1);
            }
        }
    }
    tokens.push_back(Token("EndOfFile", EOFToken));
    return tokens;
}

bool Lexer::isAlpha(char c) {
    return std::isalpha(c);
}

bool Lexer::isSkippable(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

bool Lexer::isInt(char c) {
    return std::isdigit(c);
}
