#pragma once

#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>

enum TokenType {
	Null, // null
  Number, // just number
  Identifier, // name of variable, function

  // KEYWORDS
  Let,
	Const,
	Func,
  If,  
  Else, 

  EqualEqual,    // ==
  NotEqual,      // !=
  LessThan,      // <
  LessEqual,     // <=
  GreaterThan,   // >
  GreaterEqual,  // >= 
                 
                 
  And,      // &&
  Or,       // ||
  Not,      // !


  BinaryOperator, // + - * / %
  Equals, // =
	Semicolon, // ;
  OpenParen, // (
  CloseParen, // )

	OpenBrace, // {
	CloseBrace, // }

	OpenBracket, // [
	CloseBracket, // ]
	Comma, // ,

	EOFToken, // end of file
};

class Token {
	public:
		Token(const std::string& value, TokenType type);
		std::string getValue() const;
		TokenType getType() const;

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
		bool isAlpha(char c);
		bool isSkippable(char c);
		bool isInt(char c);
};

#endif
