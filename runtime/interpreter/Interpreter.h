#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../values/Values.h"
#include "../../ast/AST.h"
#include "../environment/Environment.h"

#include <iostream>
#include <cmath>


class Interpreter {
  public:
    static std::unique_ptr<RuntimeVal> evaluate(std::unique_ptr<Stmt> astNode, Environment* env);

    // Expressions
    static std::unique_ptr<RuntimeVal> eval_assignment(std::unique_ptr<AssignmentExpr> node, Environment* env);
    // static std::unique_ptr<RuntimeVal> eval_call_expr(std::unique_ptr<CallExpr> call, Environment* env);
    static std::unique_ptr<RuntimeVal> eval_identifer(std::unique_ptr<IdentifierExpr> ident, Environment* env);
    static std::unique_ptr<RuntimeVal> eval_binary_expr(std::unique_ptr<BinaryExpr> binop, Environment* env);
    static std::unique_ptr<RuntimeVal> eval_unary_expr(std::unique_ptr<UnaryExpr> expr, Environment* env);

    // Statements
    // static std::unique_ptr<RuntimeVal> eval_program(std::unique_ptr<Program> program, Environment* env);
    // static std::unique_ptr<RuntimeVal> eval_if_statement(std::unique_ptr<IfStatement> ifStmt, Environment* env);
    // static std::unique_ptr<RuntimeVal> eval_while_statement(std::unique_ptr<WhileLoop> whileStmt, Environment* env);
    // static std::unique_ptr<RuntimeVal> eval_stmt_vector(const std::vector<std::unique_ptr<Stmt>>& stmts, Environment* env);
    // static std::unique_ptr<RuntimeVal> eval_function_declaration(std::unique_ptr<FunctionDeclaration> declaration, Environment* env);
    // static std::unique_ptr<RuntimeVal> eval_return_statement(std::unique_ptr<ReturnStatement> returnStmt, Environment* env);
    // static std::unique_ptr<RuntimeVal> eval_var_declaration(std::unique_ptr<VarDeclaration> declaration, Environment* env);

};

#endif
