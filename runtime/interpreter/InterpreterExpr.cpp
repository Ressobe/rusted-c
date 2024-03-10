#include "Interpreter.h"

RuntimeVal *Interpreter::eval_identifer(IdentifierExpr *ident,
                                        Environment *env) {
  try {
    RuntimeVal *val = env->lookupVar(ident->symbol);
    return val;
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_assignment(AssignmentExpr *node,
                                         Environment *env) {

  try {
    if (node->assigne->kind == NodeType::MemberAccessExpr) {
      return eval_member_access_assignment(
          dynamic_cast<MemberAccessExpr *>(node->assigne.get()),
          node->value.get(), env);
    }

    if (node->assigne->kind != NodeType::Identifier) {
      throw InterpreterError("Invalid LHS inside assignment expr");
    }

    IdentifierExpr *ident = dynamic_cast<IdentifierExpr *>(node->assigne.get());
    const std::string varname = ident->symbol;
    return env->assignVar(varname, Interpreter::evaluate(node->value.get(), env));
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_call_expr(CallExpr *expr, Environment *env) {
  try {
    std::vector<RuntimeVal *> args;

    for (auto &arg : expr->args) {
      args.push_back(Interpreter::evaluate(arg.get(), env));
    }

    RuntimeVal *caller = Interpreter::evaluate(expr->caller.get(), env);

    if (caller->type == ValueType::StructValue) {
      StructVal *structVal = dynamic_cast<StructVal *>(caller);
      if (!structVal->isDeclaration) {
        throw InterpreterError("Cannot create struct instance from other struct instance");
      }

      StructVal *newStructInstance = new StructVal(structVal->structName, false);

      for (const auto &field : structVal->fields) {
        newStructInstance->addField(field.first, field.second);
      }

      auto it = newStructInstance->fields.begin();
      for (const auto &arg : args) {
        if (it != newStructInstance->fields.end()) {
          it->second = arg;
          ++it;
        }
      }

      return newStructInstance;
    }

    if (caller->type == ValueType::NativeFunction) {
      NativeFnVal *nativeFn = dynamic_cast<NativeFnVal *>(caller);
      return nativeFn->call(args, env);
    }

    if (caller->type == ValueType::Function) {
      FnVal *func = dynamic_cast<FnVal *>(caller);
      Environment *functionEnv = new Environment(env);

      for (size_t i = 0; i < func->parameters.size(); ++i) {
        if (i < args.size()) {
          functionEnv->declareVar(func->parameters[i], args[i], false);
        }
      }
      RuntimeVal *result = new NullVal;

      for (Stmt *stmt : func->body) {
        result = Interpreter::evaluate(stmt, functionEnv);
        if (result->type == ValueType::ReturnValue) {
          return result;
          delete functionEnv;
        }
      }

      delete functionEnv;
      return result;
    }

  throw InterpreterError("Cannot call value that is not a function");
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_member_access(MemberAccessExpr *memberAccess,
                                            Environment *env) {
  try {
    RuntimeVal *object = evaluate(memberAccess->object.get(), env);

    if (object->type != ValueType::StructValue) {
      throw InterpreterError("Error: Member access is only supported for structs.");
    }

    StructVal *structVal = dynamic_cast<StructVal *>(object);
    return structVal->getField(memberAccess->memberName);
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal* Interpreter::eval_logical_expr(LogicalExpr* logicalExpr, Environment* env) {
  try {
    RuntimeVal* left = evaluate(logicalExpr->left.get(), env);
    RuntimeVal* right = evaluate(logicalExpr->right.get(), env);

    if (left->type == ValueType::ReturnValue) {
        left = dynamic_cast<ReturnValue*>(left)->value;
    }

    if (right->type == ValueType::ReturnValue) {
        right = dynamic_cast<ReturnValue*>(right)->value;
    }

    if (left->type == ValueType::NumberValue && right->type == ValueType::NumberValue) {
        NumberVal* leftBool = dynamic_cast<NumberVal*>(left);
        NumberVal* rightBool = dynamic_cast<NumberVal*>(right);

        NumberVal left = (*leftBool);
        NumberVal right = (*rightBool);

        if (logicalExpr->logicalOperator == "&&") {
            return left && right;
        } else if (logicalExpr->logicalOperator == "||") {
            return left || right;
        } else {
          throw InterpreterError("Invalid logical operator: " + logicalExpr->logicalOperator);
        }
    }

    throw InterpreterError("Invalid operands for logical expression");
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *
Interpreter::eval_member_access_assignment(MemberAccessExpr *memberAccessExpr,
                                           Expr *valueExpr, Environment *env) {
  try {
    RuntimeVal *object = evaluate(memberAccessExpr->object.get(), env);
    if (object->type != ValueType::StructValue) {
      throw InterpreterError("Error: Member access is only supported for structs.");
    }
    StructVal *structVal = dynamic_cast<StructVal *>(object);

    RuntimeVal *value = evaluate(valueExpr, env);

    structVal->setField(memberAccessExpr->memberName, value);

    return value;
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_unary_expr(UnaryExpr *expr, Environment *env) {
  try {
    RuntimeVal *rightValue = Interpreter::evaluate(expr->right.get(), env);

    if (expr->op == "!") {
      if (rightValue->type == ValueType::NumberValue) {
        NumberVal *number = dynamic_cast<NumberVal *>(rightValue);
        return new NumberVal(!number->value);
      }
    }

    if (expr->op == "-") {
      if (rightValue->type == ValueType::NumberValue) {
        NumberVal *number = dynamic_cast<NumberVal *>(rightValue);
        return new NumberVal(-number->value);
      }
    }

    throw InterpreterError("Unsupported unary operator: " + expr->op);
  }
  catch (const InterpreterError& e) {
    throw;
  }
}

RuntimeVal *Interpreter::eval_binary_expr(BinaryExpr *binop, Environment *env) {
  try {
    RuntimeVal *lhs = Interpreter::evaluate(binop->left.get(), env);
    RuntimeVal *rhs = Interpreter::evaluate(binop->right.get(), env);


    RuntimeVal* result = new NullVal();

    if (lhs->type == ValueType::ReturnValue) {
      lhs = dynamic_cast<ReturnValue *>(lhs)->value;
    }

    if (rhs->type == ValueType::ReturnValue) {
      rhs = dynamic_cast<ReturnValue *>(rhs)->value;
    }

    if (lhs->type == ValueType::NumberValue &&
        rhs->type == ValueType::NumberValue) {
        NumberVal* leftNumber = dynamic_cast<NumberVal*>(lhs);
        NumberVal* rightNumber = dynamic_cast<NumberVal*>(rhs);

        NumberVal left = (*leftNumber);
        NumberVal right = (*rightNumber);

         
      if (binop->binaryOperator == "+") {
          result = left + right;
      } else if (binop->binaryOperator == "-") {
          result = left - right;
      } else if (binop->binaryOperator == "*") {
          result = left * right;
      } else if (binop->binaryOperator == ">") {
          result = left > right;
      } else if (binop->binaryOperator == "<") {
          result = left < right;
      } else if (binop->binaryOperator == "<=") {
          result = left <= right;
      } else if (binop->binaryOperator == ">=") {
          result = left >= right;
      } else if (binop->binaryOperator == "==") {
          result = left == right;
      } else if (binop->binaryOperator == "!=") {
          result = left != right;
      } else if (binop->binaryOperator == "/") {
        if (rightNumber->value != 0) {
          result = left / right;
        } else {
          throw InterpreterError("Division by zero error");
        }
      } else if (binop->binaryOperator == "%") {
        if (rightNumber->value != 0) {
          result = left % right;
        } else {
          throw InterpreterError("Modulo by zero error");
        }
      }

    } 
    return result;
  }
  catch (const InterpreterError& e) {
    throw;
  }
}
