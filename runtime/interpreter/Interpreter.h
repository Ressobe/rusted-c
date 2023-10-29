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
    static RuntimeVal* eval_program(Program* program, Environment* env);
    static RuntimeVal* eval_binary_expr(BinaryExpr* binop, Environment* env);
    static RuntimeVal* evaluate(Stmt* astNode, Environment* env);
    static RuntimeVal* eval_identifer(IdentifierExpr* ident, Environment* env);
    static RuntimeVal* eval_var_declaration(VarDeclaration* declaration, Environment* env);
    static RuntimeVal* eval_function_declaration(FunctionDeclaration* declaration, Environment* env);
    static RuntimeVal* eval_assignment(AssignmentExpr* node, Environment* env);
    static RuntimeVal* eval_call_expr(CallExpr* call, Environment* env);
};

#endif
