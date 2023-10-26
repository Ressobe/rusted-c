#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "../lexer/Lexer.h"
#include "../ast/AST.h"


class Parser {
private:
    std::vector<Token> tokens;

    bool not_eof();
    Token at();
    Token eat();
    Token expect(TokenType type, const std::string& err);

public:
    Program produceAST(const std::string& sourceCode);
    Stmt* parse_stmt();
    Stmt* parse_var_declaration();
    Stmt* parse_function_declaration();
    Expr* parse_expr();
    Expr* parse_additive_expr();
    Expr* parse_multiplicative_expr();
    Expr* parse_primary_expr();
    Expr* parse_assignment_expr();
    Expr* parse_call_member_expr();
    Expr* parse_call_expr(Expr* caller);
    std::vector<Expr*> parse_arguments_list();
    std::vector<Expr*> parse_args();
};

#endif
