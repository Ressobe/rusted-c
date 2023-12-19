#include "Lexer.h"
#include <cctype>
#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, TokenType> KEYWORDS = {
    {"null", Null},   {"let", Let},       {"const", Const},
    {"func", Func},   {"if", If},         {"else", Else},
    {"while", While}, {"return", Return}, {"struct", StructToken}};

Token::Token(const std::string &value, TokenType type)
    : value(value), type(type) {}

std::string Token::getValue() const { return value; }

TokenType Token::getType() const { return type; }

Lexer::Lexer(const std::string &sourceCode)
    : sourceCode(sourceCode), currentChar(sourceCode[0]) {
  this->tokenize();
}

void Lexer::createNumberToken() {
  std::string num;
  num += currentChar;

  int amountOfDots = 0;

  while (!this->src.empty() &&
         (std::isdigit(this->peek()) || this->peek() == '.')) {
    if (this->peek() == '.')
      amountOfDots++;
    if (amountOfDots > 1)
      this->unrecognizedChar(currentChar);
    num += this->eat();
  }
  if (amountOfDots == 0) {
    tokens.push_back(Token(num, NumberLiteral));
  } else {
    tokens.push_back(Token(num, FloatLiteral));
  }
}

void Lexer::createStringToken() {
  std::string stringLiteral;

  while (!src.empty() && peek() != '"') {
    stringLiteral += eat();
  }

  if (!src.empty() && peek() == '"') {
    // Zjedz cudzys³ów, aby zakoñczyæ litera³ stringowy
    eat();
    tokens.push_back(Token(stringLiteral, TokenType::StringLiteral));
  } else {
    this->unrecognizedChar(currentChar);
  }
}

void Lexer::createAndToken() {
  if (!src.empty() && this->peek() == '&') {
    tokens.push_back(Token("&&", TokenType::And));
    this->eat();
  } else {
    this->unrecognizedChar(currentChar);
  }
}

void Lexer::createOrToken() {
  if (!src.empty() && this->peek() == '|') {
    tokens.push_back(Token("||", TokenType::Or));
    this->eat();
  } else {
    this->unrecognizedChar(currentChar);
  }
}

void Lexer::createBinaryOperatorToken() {
  tokens.push_back(Token(std::string(1, currentChar), BinaryOperator));
}

void Lexer::createCompareToken(std::string firstChar, std::string secondChar,
                               TokenType firstToken, TokenType secondToken) {
  std::string z = firstChar + secondChar;
  char c = secondChar[0];
  if (!src.empty() && peek() == c) {
    tokens.push_back(Token(z, firstToken));
    this->eat();
  } else {
    tokens.push_back(Token(firstChar, secondToken));
  }
}

void Lexer::createOneCharToken(std::string tokenChar, TokenType charType) {
  tokens.push_back(Token(tokenChar, charType));
}

void Lexer::createIdentifierToken() {
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
}

void Lexer::skipComments() {
    while (!src.empty() && peek() != '\n') {
        this->eat();
    }
}

void Lexer::tokenize() {
  src = std::vector<char>(sourceCode.begin(), sourceCode.end());

  while (!src.empty()) {
    currentChar = this->eat();

    if (this->isSkippable(currentChar)) {
      continue;
    }
    if (std::isdigit(currentChar)) {
      createNumberToken();
    } else if (this->isAlpha(currentChar)) {
      createIdentifierToken();
    } else {
      switch (currentChar) {
      case '#':
        skipComments();
        break;
      case '(':
        createOneCharToken("(", TokenType::OpenParen);
        break;
      case ')':
        createOneCharToken(")", TokenType::CloseParen);
        break;
      case '{':
        createOneCharToken("{", TokenType::OpenBrace);
        break;
      case '}':
        createOneCharToken("}", TokenType::CloseBrace);
        break;
      case '[':
        createOneCharToken("[", TokenType::OpenBracket);
        break;
      case ']':
        createOneCharToken("]", TokenType::CloseBracket);
        break;
      case ',':
        createOneCharToken(",", TokenType::Comma);
        break;
      case '.':
        createOneCharToken(".", TokenType::Dot);
        break;
      case '-':
        if (std::isdigit(this->peek())) {
          createNumberToken();
        } else {
          createBinaryOperatorToken();
        }
        break;
      case '+':
      case '*':
      case '/':
      case '%':
        createBinaryOperatorToken();
        break;
      case ';':
        createOneCharToken(";", TokenType::Semicolon);
        break;
      case '&':
        createAndToken();
        break;
      case '|':
        createOrToken();
        break;
      case '!':
        createCompareToken("!", "=", TokenType::NotEqual, TokenType::Not);
        break;
      case '=':
        createCompareToken("=", "=", TokenType::EqualEqual, TokenType::Equals);
        break;
      case '<':
        createCompareToken("<", "=", TokenType::LessEqual, TokenType::LessThan);
        break;
      case '>':
        createCompareToken(">", "=", TokenType::GreaterEqual,
                           TokenType::GreaterThan);
        break;
      case '"':
        if (isAlpha(this->peek())) {
          createStringToken();
        }
        break;
      default:
        this->unrecognizedChar(currentChar);
        break;
      }
    }
  }

  createOneCharToken("EndOfFile", TokenType::EOFToken);
}

char Lexer::eat() {
  char currentChar = this->src[0];
  this->src.erase(this->src.begin());
  return currentChar;
}

char Lexer::peek() const { return this->src[0]; }

bool Lexer::isAlpha(char c) const { return std::isalpha(c) || c == '_'; }

bool Lexer::isSkippable(char c) const {
  return c == ' ' || c == '\n' || c == '\t';
}

bool Lexer::isInt(char c) const { return std::isdigit(c); }

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
      {StructToken, "Struct"},
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
      {EOFToken, "EOFToken"}};

  auto it = tokenTypeNames.find(this->getType());
  if (it != tokenTypeNames.end()) {
    return it->second;
  } else {
    return "Unknown";
  }
}

std::vector<Token> Lexer::getTokens() { return this->tokens; }

void Lexer::printTokens() {
  std::cout << "[ " << std::endl;
  for (Token &token : this->getTokens()) {
    std::cout << token.getTokeTypeName() << ", " << std::endl;
  }
  std::cout << " ]" << std::endl;
}
