#include "Interpreter.h"

RuntimeVal *Interpreter::eval_program(Program *program, Environment *env) {
  try {
    RuntimeVal *lastEvaluated = new NullVal;
    for (std::unique_ptr<Stmt> &statement : program->body) {
      lastEvaluated = Interpreter::evaluate(statement.get(), env);
    }
    return lastEvaluated;
    }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_return_statement(ReturnStatement *stmt,
                                               Environment *env) {
  try {
    if (stmt->returnValue) {
      RuntimeVal *result = Interpreter::evaluate(stmt->returnValue.get(), env);
      return new ReturnValue(result);
    } else {
      return new NullVal;
    }
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *
Interpreter::eval_stmt_vector(const std::vector<std::unique_ptr<Stmt>> &stmts,
                              Environment *env) {
  try {
    RuntimeVal *result = nullptr;

    for (auto &stmt : stmts) {
      result = Interpreter::evaluate(stmt.get(), env);
      if (result->type == ValueType::ReturnValue) {
        return result;
      }
    }

    return result;
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_if_statement(IfStatement *ifStmt,
                                           Environment *env) {
  try {
    RuntimeVal *conditionValue =
        Interpreter::evaluate(ifStmt->condition.get(), env);

    if (conditionValue->type == ValueType::NumberValue) {
      NumberVal *numCondition = dynamic_cast<NumberVal *>(conditionValue);

      if (numCondition->value != 0) {
        return Interpreter::eval_stmt_vector(ifStmt->ifBody, env);
      } else if (ifStmt->elseBody.size() > 0) {
        return Interpreter::eval_stmt_vector(ifStmt->elseBody, env);
      }
    } else {
      throw InterpreterError("If statement condition must evaluate to a numeric value.");
    }

    return new NullVal;
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_while_statement(WhileLoop *loop,
                                              Environment *env) {
  try {
    RuntimeVal *result = new NullVal;

    bool conditionMet = true;

    while (conditionMet) {
      RuntimeVal *conditionValue =
          Interpreter::evaluate(loop->condition.get(), env);

      if (conditionValue->type == ValueType::NumberValue) {
        NumberVal *numCondition = dynamic_cast<NumberVal *>(conditionValue);

        if (numCondition->value == 1) {
          RuntimeVal *loopResult =
              Interpreter::eval_stmt_vector(loop->loopBody, env);

          if (loopResult->type == ValueType::ReturnValue) {
            delete result;
            return loopResult;
          }

          delete result;
          result = loopResult;

        } else {
          conditionMet = false;
        }
      } else {
        throw InterpreterError("While loop condition must evaluate to a numeric value.");
      }
    }
    return result;
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_var_declaration(VarDeclaration *declaration,
                                              Environment *env) {
  try {
    RuntimeVal *value = Interpreter::evaluate(declaration->value.get(), env);
    return env->declareVar(declaration->identifier, value, declaration->constant);
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_struct_declaration(StructDeclaration *structDecl,
                                                 Environment *env) {
  try {
    StructVal *structVal = new StructVal(structDecl->structName, true);

    for (const auto &stmt : structDecl->structBody) {
      if (stmt->kind == NodeType::VarDeclaration) {
        auto fieldDecl = dynamic_cast<VarDeclaration *>(stmt.get());
        structVal->addField(fieldDecl->identifier,
                            Interpreter::evaluate(fieldDecl->value.get(), env));
      }
    }

    return env->declareVar(structDecl->structName, structVal, true);
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *
Interpreter::eval_function_declaration(FunctionDeclaration *declaration,
                                       Environment *env) {
  try {
    FnVal *fn = new FnVal(declaration->name, declaration->parameters, env,
                          declaration->body);
    return env->declareVar(declaration->name, fn, true);
  }
  catch (const InterpreterError& e) {
    throw;
  }
}
