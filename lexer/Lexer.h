#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

enum TokenType {
  // Name of variable, function itp ...
  Identifier,

  // Types
  NumberLiteral,
  StringLiteral,
  FloatLiteral,

  // KEYWORDS
  Null,
  Let,
  Const,
  Func,
  If,
  Else,
  While,
  Return,
  StructToken,

  // Assigment
  Equals, // =

  // Comapare
  EqualEqual,   // ==
  NotEqual,     // !=
  LessThan,     // <
  LessEqual,    // <=
  GreaterThan,  // >
  GreaterEqual, // >=

  // Operations
  BinaryOperator, // + - * / %

  // Logical operators
  And, // &&
  Or,  // ||
  Not, // !

  // Other characters
  Semicolon,    // ;
  OpenParen,    // (
  CloseParen,   // )
  OpenBrace,    // {
  CloseBrace,   // }
  OpenBracket,  // [
  CloseBracket, // ]
  Comma,        // ,
  Dot,          // .

  // End of file token
  EOFToken,
};

class Token {
public:
  Token(const std::string &value, TokenType type);
  std::string getValue() const;
  TokenType getType() const;
  std::string getTokeTypeName();

private:
  std::string value;
  TokenType type;
};

class Lexer {
public:
  Lexer(const std::string &sourceCode);
  void tokenize();
  std::vector<Token> getTokens();
  void printTokens();

private:
  char currentChar;
  std::vector<Token> tokens;
  std::string sourceCode;
  std::vector<char> src;

  bool isAlpha(char c) const;
  bool isSkippable(char c) const;
  bool isInt(char c) const;
  void unrecognizedChar(char c) const;
  char peek() const;
  char eat();

  void createNumberToken();
  void createStringToken();
  void createAndToken();
  void createOrToken();
  void createBinaryOperatorToken();
  void createCompareToken(std::string firstChar, std::string secondChar,
                          TokenType firstToken, TokenType secondToken);
  void createOneCharToken(std::string tokenChar, TokenType charType);
  void createIdentifierToken();

  void skipComments();
};

#endif
