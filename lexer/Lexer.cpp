#include "Lexer.h"
#include <unordered_map>
#include <cctype>

std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"null", Null},
    {"let", Let},
    {"const", Const},
    {"func", Func},
    {"if", If},
    {"else", Else},
    {"while", While},
    {"return", Return}
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
        char currentChar = this->eat();

        if (this->isSkippable(currentChar)) {
            continue;
        }

        if (std::isdigit(currentChar)) {
            std::string num;
            num += currentChar;
            
            int amountOfDots = 0;

            while (!src.empty() && (std::isdigit(this->peek()) || this->peek() == '.' )) {
                if (this->peek() == '.') amountOfDots++;

                if (amountOfDots > 1) this->unrecognizedChar(currentChar);

                num += this->eat();
            }

            if (amountOfDots == 0) {
              tokens.push_back(Token(num, NumberLiteral));
            }
            else {
              tokens.push_back(Token(num, FloatLiteral));
            }
        } else if (this->isAlpha(currentChar)) {
            std::string ident;
            ident += currentChar;
            while (!src.empty() && (std::isalnum(peek()) || peek() == '_')) {
                ident += this->eat();
            }

            auto it = KEYWORDS.find(ident);
            if (it != KEYWORDS.end()) {
                tokens.push_back(Token(ident, it->second));
            } else {
                tokens.push_back(Token(ident, Identifier));
            }
        } else {
            switch (currentChar) {
                case '(':
                    tokens.push_back(Token("(", OpenParen));
                    break;
                case ')':
                    tokens.push_back(Token(")", CloseParen));
                    break;
                case '{':
                    tokens.push_back(Token("{", OpenBrace));
                    break;
                case '}':
                    tokens.push_back(Token("}", CloseBrace));
                    break;
                case '[':
                    tokens.push_back(Token("[", OpenBracket));
                    break;
                case ']':
                    tokens.push_back(Token("]", CloseBracket));
                    break;
                case ',':
                    tokens.push_back(Token(",", Comma));
                    break;
                case '+':
                case '-':
                case '*':
                case '/':
                case '%':
                    tokens.push_back(Token(std::string(1, currentChar), BinaryOperator));
                    break;
                case ';':
                    tokens.push_back(Token(";", Semicolon));
                    break;
                case '&':
                    if (!src.empty() && this->peek() == '&') {
                        tokens.push_back(Token("&&", TokenType::And));
                        this->eat();
                    } else {
                        this->unrecognizedChar(currentChar);
                    }
                    break;
                case '|':
                    if (!src.empty() && this->peek() == '|') {
                        tokens.push_back(Token("||", TokenType::Or));
                        this->eat();
                    } else {
                        this->unrecognizedChar(currentChar);
                    }
                    break;
                case '!':
                    if (!src.empty() && peek() == '=') {
                        tokens.push_back(Token("!=", TokenType::NotEqual));
                        this->eat();
                    } else {
                      tokens.push_back(Token("!", TokenType::Not));
                    }
                    break;
                case '=':
                    if (!src.empty() && peek() == '=') {
                        tokens.push_back(Token("==", TokenType::EqualEqual));
                        this->eat();
                    } else {
                        tokens.push_back(Token("=", TokenType::Equals));
                    }
                    break;
                case '<':
                    if (!src.empty() && peek() == '=') {
                      tokens.push_back(Token("<=", TokenType::LessEqual));
                      this->eat();
                    }
                    else {
                      tokens.push_back(Token("<", TokenType::LessThan));
                    }
                    break;
                case '>':
                    if (!src.empty() && peek() == '=') {
                      tokens.push_back(Token(">=", TokenType::GreaterEqual));
                      this->eat();
                    }
                    else {
                      tokens.push_back(Token(">", TokenType::GreaterThan));
                    }
                    break;
                case '"': 
                    if (isAlpha(src[0]))  {
                      std::string stringLiteral;

                      while(!src.empty() && (this->isAlpha(peek()) || peek() == ' ') ) {
                        stringLiteral += this->eat();
                      }

                      if (src[0] == '"') {
                        tokens.push_back(Token(stringLiteral, TokenType::StringLiteral));
                        break;
                      }
                      else {
                        this->unrecognizedChar(currentChar);
                      }
                    }
                    break;
                default:
                    this->unrecognizedChar(currentChar);
                    break;
            }
        }
    }

    tokens.push_back(Token("EndOfFile", EOFToken));
    return tokens;
}

char Lexer::eat() {
    char currentChar = this->src[0];
    this->src.erase(this->src.begin());
    return currentChar;
}

char Lexer::peek() const {
    return this->src[0];
}

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isSkippable(char c) const {
    return c == ' ' || c == '\n' || c == '\t';
}

bool Lexer::isInt(char c) const {
    return std::isdigit(c);
}

void Lexer::unrecognizedChar(char c) const {
    std::cerr << "Unrecognized character found in source: " << c << std::endl;
    exit(1);
}

std::string Token::getTokeTypeName() {
  static const std::map<TokenType, std::string> tokenTypeNames = {
        {Null, "Null"},
        {NumberLiteral, "NumberLiteral"},
        {Identifier, "Identifier"},
        {StringLiteral, "StringLiteral"},
        {FloatLiteral, "FloatLiteral"},
        {Let, "Let"},
        {Const, "Const"},
        {Func, "Func"},
        {If, "If"},
        {Else, "Else"},
        {While, "While"},
        {Return, "Return"},
        {EqualEqual, "EqualEqual"},
        {NotEqual, "NotEqual"},
        {LessThan, "LessThan"},
        {LessEqual, "LessEqual"},
        {GreaterThan, "GreaterThan"},
        {GreaterEqual, "GreaterEqual"},
        {And, "And"},
        {Or, "Or"},
        {Not, "Not"},
        {BinaryOperator, "BinaryOperator"},
        {Equals, "Equals"},
        {Semicolon, "Semicolon"},
        {OpenParen, "OpenParen"},
        {CloseParen, "CloseParen"},
        {OpenBrace, "OpenBrace"},
        {CloseBrace, "CloseBrace"},
        {OpenBracket, "OpenBracket"},
        {CloseBracket, "CloseBracket"},
        {Comma, "Comma"},
        {EOFToken, "EOFToken"}
    };

    auto it = tokenTypeNames.find(this->getType());
    if (it != tokenTypeNames.end()) {
        return it->second;
    } else {
        return "Unknown";
    }
}
