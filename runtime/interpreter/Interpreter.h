#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../values/Values.h"
#include "../../ast/AST.h"
#include "../environment/Environment.h"
#include "./statement/EvaluateStatement.h"
#include "./expression/EvaluateExpression.h"

#include <iostream>
#include <cmath>


class Interpreter {
  public:
    static RuntimeVal* evaluate(Stmt* astNode, Environment* env);
};

#endif
