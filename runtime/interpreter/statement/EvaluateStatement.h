#pragma once

#ifndef EVALUATE_STATEMENT_H
#define EVALUATE_STATEMENT_H

#include "../../values/Values.h"
#include "../../environment/Environment.h"
#include "../Interpreter.h"

class EvaluateStatement {
  public:
    static RuntimeVal* eval_program(Program* program, Environment* env);
    static RuntimeVal* eval_if_statement(IfStatement* ifStmt, Environment* env);
    static RuntimeVal* eval_function_declaration(FunctionDeclaration* declaration, Environment* env);
    static RuntimeVal* eval_var_declaration(VarDeclaration* declaration, Environment* env);
};

#endif
