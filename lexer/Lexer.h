#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

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

  // Assigment
  Equals, // =

  // Comapare
  EqualEqual,    // ==
  NotEqual,      // !=
  LessThan,      // <
  LessEqual,     // <=
  GreaterThan,   // >
  GreaterEqual,  // >= 
                 
  BinaryOperator, // + - * / %
            
  // Logical operators
  And,      // &&
  Or,       // ||
  Not,      // !


	Semicolon, // ;
  OpenParen, // (
  CloseParen, // )
	OpenBrace, // {
	CloseBrace, // }
	OpenBracket, // [
	CloseBracket, // ]
	Comma, // ,

	EOFToken, // End of file
};

class Token {
	public:
		Token(const std::string& value, TokenType type);
		std::string getValue() const;
		TokenType getType() const;
    std::string getTokeTypeName();

	private:
		std::string value;
		TokenType type;
};

class Lexer {
	public:
		Lexer(const std::string& sourceCode);
		std::vector<Token> tokenize();

	private:
		std::string sourceCode;
		std::vector<char> src;
		bool isAlpha(char c) const;
		bool isSkippable(char c) const;
		bool isInt(char c) const;
    void unrecognizedChar(char c) const;
    char peek() const;
    char eat();
};

#endif
