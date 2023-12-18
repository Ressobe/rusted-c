#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "../lexer/Lexer.h"
#include "../ast/AST.h"


class Parser {
private:
    std::vector<Token> tokens;
    bool eof();

    Token at();
    Token eat();
    Token expect(TokenType type, const std::string& err);
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
    std::unique_ptr<Expr> parse_call_member_expr();
    std::unique_ptr<Expr> parse_member_access(std::unique_ptr<Expr> left);

    std::vector<std::unique_ptr<Expr>> parse_arguments_list();
    std::vector<std::unique_ptr<Expr>> parse_args();

    bool is_comparison_operator(TokenType type);
    bool is_additive_operator(std::string type);
    bool is_multiplicative_operator(std::string value);
public:
    std::unique_ptr<Program> produceAST(std::vector<Token> tokens);
};

#endif
