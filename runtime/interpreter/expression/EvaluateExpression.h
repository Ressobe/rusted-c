#pragma once

#ifndef EVALUATE_EXPRESSION_H
#define EVALUATE_EXPRESSION_H

#include "../../values/Values.h"
#include "../../environment/Environment.h"
#include "../Interpreter.h"

class EvaluateExpression {
  public:
      static RuntimeVal* eval_assignment(AssignmentExpr* node, Environment* env);
      static RuntimeVal* eval_call_expr(CallExpr* call, Environment* env);
      static RuntimeVal* eval_identifer(IdentifierExpr* ident, Environment* env);
      static RuntimeVal* eval_binary_expr(BinaryExpr* binop, Environment* env);
};


#endif
