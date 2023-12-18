#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../../ast/AST.h"
#include "../environment/Environment.h"
#include "../values/Values.h"

#include <cmath>
#include <iostream>

class Interpreter {
public:
  static RuntimeVal *evaluate(Stmt *astNode, Environment *env);

  static RuntimeVal *eval_program(Program *program, Environment *env);
  static RuntimeVal *eval_if_statement(IfStatement *ifStmt, Environment *env);
  static RuntimeVal *eval_while_statement(WhileLoop *whileStmt,
                                          Environment *env);
  static RuntimeVal *
  eval_stmt_vector(const std::vector<std::unique_ptr<Stmt>> &stmts,
                   Environment *env);
  static RuntimeVal *eval_function_declaration(FunctionDeclaration *declaration,
                                               Environment *env);
  static RuntimeVal *eval_return_statement(ReturnStatement *returnStmt,
                                           Environment *env);
  static RuntimeVal *eval_var_declaration(VarDeclaration *declaration,
                                          Environment *env);
  static RuntimeVal *eval_struct_declaration(StructDeclaration *declaration,
                                             Environment *env);

  static RuntimeVal *eval_assignment(AssignmentExpr *node, Environment *env);
  static RuntimeVal *eval_call_expr(CallExpr *call, Environment *env);
  static RuntimeVal *eval_identifer(IdentifierExpr *ident, Environment *env);
  static RuntimeVal *eval_binary_expr(BinaryExpr *binop, Environment *env);
  static RuntimeVal *eval_unary_expr(UnaryExpr *expr, Environment *env);
  static RuntimeVal *eval_member_access(MemberAccessExpr *memberAccess,
                                        Environment *env);
  static RuntimeVal *
  eval_member_access_assignment(MemberAccessExpr *memberAccessExpr,
                                Expr *valueExpr, Environment *env);
};

#endif
