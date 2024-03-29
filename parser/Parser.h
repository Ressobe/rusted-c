#ifndef PARSER_H
#define PARSER_H

#include "../ast/AST.h"
#include "../lexer/Lexer.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Parser {
private:
  std::vector<Token> tokens;
  bool eof();

  Token at();
  Token eat();
  Token expect(TokenType type, const std::string &err);
  Token lookahead(size_t num);

  std::unique_ptr<Stmt> parse_stmt();
  std::unique_ptr<Stmt> parse_if_statement();
  std::unique_ptr<Stmt> parse_while_statement();
  std::unique_ptr<Stmt> parse_return_statement();
  std::unique_ptr<Stmt> parse_var_declaration();
  std::unique_ptr<Stmt> parse_function_declaration();
  std::unique_ptr<Stmt> parse_struct_declaration();

  std::unique_ptr<Expr> parse_expr();
  std::unique_ptr<Expr> parse_additive_expr();
  std::unique_ptr<Expr> parse_multiplicative_expr();
  std::unique_ptr<Expr> parse_primary_expr();
  std::unique_ptr<Expr> parse_assignment_expr();
  std::unique_ptr<Expr> parse_comparision_expr();
  std::unique_ptr<Expr> parse_logical_expr();
  std::unique_ptr<Expr> parse_call_member_expr();
  std::unique_ptr<Expr> parse_member_access(std::unique_ptr<Expr> left);

  std::vector<std::unique_ptr<Expr>> parse_arguments_list();
  std::vector<std::unique_ptr<Expr>> parse_args();

  bool is_comparison_operator(TokenType type);
  bool is_additive_operator(std::string type);
  bool is_multiplicative_operator(std::string value);
  bool is_logical_operator(TokenType type);

public:
  std::unique_ptr<Program> produceAST(std::vector<Token> tokens);
};

class ParserError : public std::runtime_error {
  public:
      ParserError(const std::string& message) : std::runtime_error(message) {}
};

#endif
